cmake_minimum_required (VERSION 3.22)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

FUNCTION(cat IN_FILE OUT_FILE)
  file(READ ${IN_FILE} CONTENTS)
  file(APPEND ${OUT_FILE} "${CONTENTS}")
ENDFUNCTION()

project (ONNX2FMU)

# Set default FMI version to 2
set(FMI_VERSION 2 CACHE STRING "FMI Version")
set_property(CACHE FMI_VERSION PROPERTY STRINGS 1 2 3)

set(FMI_ARCHITECTURE "" CACHE STRING "FMI Architecture")
set_property(CACHE FMI_ARCHITECTURE PROPERTY STRINGS "" "aarch64" "x86" "x86_64")

if (NOT FMI_ARCHITECTURE)
  if (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES "AMD64|x86_64")
    set(FMI_ARCHITECTURE "x86_64")
  elseif (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES "aarch64")
    set(FMI_ARCHITECTURE "aarch64")
  else ()
    message(FATAL_ERROR "Unknown System Architecture: ${CMAKE_SYSTEM_PROCESSOR}")
  endif ()
endif ()

if (MSVC)
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
  # Minimum supported MSVC version is 1800 = Visual Studio 12.0/2013
  # See also https://cmake.org/cmake/help/latest/variable/MSVC_VERSION.html
  if (MSVC_VERSION VERSION_LESS 1800)
    message (SEND_ERROR "MSVC_VERSION ${MSVC_VERSION} is not a supported Visual Studio compiler version. Please use Visual Studio 2013 or any later version.")
  endif ()
endif ()

# hide internal functions
if (UNIX AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
endif()

# FMI platforms names differ between FMI 3, and FMI 2 and 1
if (${FMI_VERSION} GREATER 2)

  if (WIN32)
    set(FMI_PLATFORM "${FMI_ARCHITECTURE}-windows")
  elseif (APPLE)
    set(FMI_PLATFORM "${FMI_ARCHITECTURE}-darwin")
  else ()
    set(FMI_PLATFORM "${FMI_ARCHITECTURE}-linux")
  endif ()
# FMI 2 and 1
else ()

  if (WIN32)
    set(FMI_PLATFORM win)
  elseif (APPLE)
    set(FMI_PLATFORM darwin)
  else ()
    set(FMI_PLATFORM linux)
  endif ()

  if ("${FMI_ARCHITECTURE}" STREQUAL "x86_64")
    set (FMI_PLATFORM ${FMI_PLATFORM}64)
  elseif ("${FMI_ARCHITECTURE}" STREQUAL "x86")
    set (FMI_PLATFORM ${FMI_PLATFORM}32)
  else ()
    message(FATAL_ERROR "Unsupported architecture for FMI version < 3.0: ${FMI_ARCHITECTURE}")
  endif ()

endif ()

if (APPLE)

  if ("${FMI_ARCHITECTURE}" STREQUAL "x86_64")
    set(CMAKE_OSX_ARCHITECTURES "x86_64")
  elseif ("${FMI_ARCHITECTURE}" STREQUAL "aarch64")
    set(CMAKE_OSX_ARCHITECTURES "arm64")
  else ()
    message(FATAL_ERROR "Unsupported architecture darwin: ${FMI_ARCHITECTURE}")
  endif ()

endif ()

message("FMI_PLATFORM: " ${FMI_PLATFORM})

if (${FMI_VERSION} LESS 3)
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/all.c" "#define FMI_VERSION ${FMI_VERSION}

#include \"fmi${FMI_VERSION}Functions.c\"
#include \"model.c\"
#include \"cosimulation.c\"
")
endif ()

set(TARGET_NAME ${MODEL_NAME})

SET(HEADERS
    ${MODEL_NAME}/config.h
    include/cosimulation.h
    include/model.h
    include/ortUtils.h
)

if (${FMI_VERSION} EQUAL 2)
  SET(HEADERS
    ${HEADERS}
    include/fmi2Functions.h
    include/fmi2FunctionTypes.h
    include/fmi2TypesPlatform.h
  )
elseif (${FMI_VERSION} EQUAL 3)
  SET(HEADERS
    ${HEADERS}
    include/fmi3Functions.h
    include/fmi3FunctionTypes.h
    include/fmi3PlatformTypes.h
  )
endif()

SET(SOURCES
  ${MODEL_NAME}/model.c
  src/fmi${FMI_VERSION}Functions.c
  src/cosimulation.c
  src/ortUtils.c
)

# Manually set the latest release tag
set (LATEST_RELEASE_TAG "1.20.1")

# Detect the operating system for onnxruntime download
if (WIN32)
  set(ONNXRUNTIME_OS "win")
  if (${FMI_ARCHITECTURE} MATCHES "AMD64|x86_64")
    set(FMI_ARCHITECTURE "x64")
  endif ()
elseif (APPLE)
  set(ONNXRUNTIME_OS "osx")
elseif (UNIX)
  set(ONNXRUNTIME_OS "linux")
  if (${FMI_ARCHITECTURE} MATCHES "AMD64|x86_64")
    set(FMI_ARCHITECTURE "x64")
  endif ()
else ()
  message(FATAL_ERROR "Unknown operating system")
endif ()

# Set the onnxruntime directory name
set(ONNXRUNTIME onnxruntime-${ONNXRUNTIME_OS}-${FMI_ARCHITECTURE}-${LATEST_RELEASE_TAG})

if (WIN32)
    set(ONNXRUNTIME_ARCHIVE ${ONNXRUNTIME}.zip)
else ()
    set(ONNXRUNTIME_ARCHIVE ${ONNXRUNTIME}.tgz)
endif ()

# Set the URL to download ONNX Runtime
set(ONNXRUNTIME_URL "https://github.com/microsoft/onnxruntime/releases/download/v${LATEST_RELEASE_TAG}/${ONNXRUNTIME_ARCHIVE}")

# Path to the downloaded ONNX Runtime archive
if (WIN32)
    set(ONNXRUNTIME_ARCHIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${ONNXRUNTIME_ARCHIVE}")
else ()
    set(ONNXRUNTIME_ARCHIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${ONNXRUNTIME_ARCHIVE}")
endif ()

# Download the ONNX Runtime release only if the file does not exist
if (NOT EXISTS ${ONNXRUNTIME_ARCHIVE_PATH})
    message(STATUS "Downloading ONNX Runtime from ${ONNXRUNTIME_URL}")
    file(DOWNLOAD ${ONNXRUNTIME_URL} ${ONNXRUNTIME_ARCHIVE_PATH})
else ()
    message(STATUS "ONNX Runtime archive already exists at ${ONNXRUNTIME_ARCHIVE_PATH}")
endif ()

# Extract the ONNX Runtime release
execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar xzf ${ONNXRUNTIME_ARCHIVE_PATH}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Check if the extraction was successful by verifying the existence of a known file or directory
if (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/${ONNXRUNTIME}/include/onnxruntime_c_api.h")
  message(STATUS "ONNX Runtime extraction successful.")
else ()
  message(FATAL_ERROR "ONNX Runtime extraction failed. File not found: ${CMAKE_CURRENT_BINARY_DIR}/${ONNXRUNTIME}/include/onnxruntime_c_api.h")
endif ()

# Include the onnxruntime directory like in https://github.com/microsoft/onnxruntime-inference-examples/blob/main/c_cxx/CMakeLists.txt
include_directories(${CMAKE_CURRENT_BINARY_DIR}/${ONNXRUNTIME}/include)

# Link the onnxruntime library
link_directories(${CMAKE_CURRENT_BINARY_DIR}/${ONNXRUNTIME}/lib)

# Add only the library
add_library(${TARGET_NAME} SHARED
  ${HEADERS}
  ${SOURCES}
  ${MODEL_NAME}/FMI${FMI_VERSION}${FMI_TYPE}.xml
  ${MODEL_NAME}/buildDescription.xml
)

# Link the onnxruntime library
target_link_libraries(${TARGET_NAME} onnxruntime)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/fmus)

target_compile_definitions(${TARGET_NAME} PRIVATE
  FMI_VERSION=${FMI_VERSION}
  DISABLE_PREFIX
)

if (MSVC)
  target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
  target_link_options(${TARGET_NAME} PRIVATE "-static-libgcc")
endif()

if (CMAKE_C_COMPILER_ID STREQUAL "Intel")
  target_link_options(${TARGET_NAME} PRIVATE "-static-intel" "-static-libgcc")
endif()

if (${FMI_VERSION} EQUAL 1 AND "${FMI_TYPE}" STREQUAL CS)
  target_compile_definitions(${TARGET_NAME} PRIVATE FMI_COSIMULATION)
endif()

target_include_directories(${TARGET_NAME} PRIVATE
  include
  ${MODEL_NAME}
)

set(FMU_BUILD_DIR temp/${MODEL_NAME})

set_target_properties(${TARGET_NAME} PROPERTIES
    INSTALL_RPATH "$ORIGIN:$ORIGIN"
    BUILD_WITH_INSTALL_RPATH TRUE
)

set_target_properties(${TARGET_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY         "${FMU_BUILD_DIR}/binaries/${FMI_PLATFORM}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${FMU_BUILD_DIR}/binaries/${FMI_PLATFORM}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${FMU_BUILD_DIR}/binaries/${FMI_PLATFORM}"
    LIBRARY_OUTPUT_DIRECTORY         "${FMU_BUILD_DIR}/binaries/${FMI_PLATFORM}"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG   "${FMU_BUILD_DIR}/binaries/${FMI_PLATFORM}"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${FMU_BUILD_DIR}/binaries/${FMI_PLATFORM}"
    ARCHIVE_OUTPUT_DIRECTORY         "${FMU_BUILD_DIR}/binaries/${FMI_PLATFORM}"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${FMU_BUILD_DIR}/binaries/${FMI_PLATFORM}"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${FMU_BUILD_DIR}/binaries/${FMI_PLATFORM}"
)

set_target_properties(${TARGET_NAME} PROPERTIES PREFIX "")
set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME ${MODEL_NAME})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# modelDescription.xml
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_CURRENT_SOURCE_DIR}/${MODEL_NAME}/FMI${FMI_VERSION}${FMI_TYPE}.xml
  "${FMU_BUILD_DIR}/modelDescription.xml"
)

# model specific header and source
foreach (SOURCE_FILE config.h model.c)
  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/${MODEL_NAME}/${SOURCE_FILE}"
    "${FMU_BUILD_DIR}/sources/${SOURCE_FILE}"
  )
endforeach(SOURCE_FILE)

# common headers
foreach (SOURCE_FILE model.h cosimulation.h)
  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/include/${SOURCE_FILE}"
    "${FMU_BUILD_DIR}/sources/${SOURCE_FILE}"
  )
endforeach(SOURCE_FILE)

# common sources
foreach (SOURCE_FILE fmi${FMI_VERSION}Functions.c cosimulation.c)
  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/src/${SOURCE_FILE}"
    "${FMU_BUILD_DIR}/sources/${SOURCE_FILE}"
  )
endforeach(SOURCE_FILE)

# all.c / buildDescription.xml
if (${FMI_VERSION} LESS 3)
  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_BINARY_DIR}/all.c"
    "${FMU_BUILD_DIR}/sources/all.c"
  )
else()
  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/${MODEL_NAME}/buildDescription.xml"
    "${FMU_BUILD_DIR}/sources/buildDescription.xml"
  )
endif()

# onnxruntime include files
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_CURRENT_BINARY_DIR}/${ONNXRUNTIME}/include/onnxruntime_c_api.h
  "${FMU_BUILD_DIR}/sources/onnxruntime_c_api.h"
)

# ortUtils
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
  "${CMAKE_CURRENT_SOURCE_DIR}/include/ortUtils.h"
  "${FMU_BUILD_DIR}/sources/ortUtils.h"
)

# Define which files to copy
set (TEMP_LIB ${CMAKE_CURRENT_BINARY_DIR}/${ONNXRUNTIME}/lib/)
if (WIN32)
  file(GLOB ONNXRUNTIME_LIBS ${TEMP_LIB}/*.dll ${TEMP_LIB}/*.lib ${TEMP_LIB}/*.pdb)
elseif (APPLE)
  file(GLOB ONNXRUNTIME_LIBS ${TEMP_LIB}/*.dylib)
elseif (UNIX)
  file(GLOB ONNXRUNTIME_LIBS ${TEMP_LIB}/*.so ${TEMP_LIB}/*.so*)
endif ()

# copy onnxruntime lib
foreach (ONNXRUNTIME_LIB ${ONNXRUNTIME_LIBS})
  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
    ${ONNXRUNTIME_LIB}
    "${FMU_BUILD_DIR}/binaries/${FMI_PLATFORM}"
  )
endforeach(ONNXRUNTIME_LIB)

set(ARCHIVE_FILES "modelDescription.xml" "binaries" "sources")

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${MODEL_NAME}/resources")
  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/${MODEL_NAME}/resources"
    "${FMU_BUILD_DIR}/resources/"
  )
  set(ARCHIVE_FILES ${ARCHIVE_FILES} "resources")
endif()

# delete unintended files from binaries (Release configuration only)
if (MSVC)
  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND if $<CONFIG:Release> neq 0 (
      ${CMAKE_COMMAND} -E rm -f
      "${FMU_BUILD_DIR}/binaries/${FMI_PLATFORM}/${MODEL_NAME}.exp"
      "${FMU_BUILD_DIR}/binaries/${FMI_PLATFORM}/${MODEL_NAME}.lib"
    )
  )
endif()

# create ZIP archive
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E tar
  "cfv"
  ${CMAKE_CURRENT_BINARY_DIR}/fmus/${MODEL_NAME}.fmu --format=zip
  ${ARCHIVE_FILES}
  WORKING_DIRECTORY ${FMU_BUILD_DIR}
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/fmus/${MODEL_NAME}.fmu DESTINATION ${CMAKE_INSTALL_PREFIX})
