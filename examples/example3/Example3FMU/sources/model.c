#include"config.h"
#include "model.h"
#include "onnxruntime_c_api.h"

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef _WIN32
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#endif

#define MAX_PATH_LENGTH 4096

#define ORT_ABORT_ON_ERROR(expr, comp) \
    do { \
    OrtStatus* onnx_status = (expr); \
    if (onnx_status != NULL) { \
            const char* msg = comp->g_ort->GetErrorMessage(onnx_status); \
            logError(comp, "%s\n", msg); \
            comp->g_ort->ReleaseStatus(onnx_status); \
        } \
    } while (0);

void setStartValues(ModelInstance *comp) {
    // Input variables
    M(input_0_0_0) = (double) 1.0;
    M(input_0_0_1) = (double) 1.0;
    M(input_0_1_0) = (double) 1.0;
    M(input_0_1_1) = (double) 1.0;
    M(input_0_2_0) = (double) 1.0;
    M(input_0_2_1) = (double) 1.0;
    M(input_0_3_0) = (double) 1.0;
    M(input_0_3_1) = (double) 1.0;
    M(input_0_4_0) = (double) 1.0;
    M(input_0_4_1) = (double) 1.0;
    M(input_0_5_0) = (double) 1.0;
    M(input_0_5_1) = (double) 1.0;
    M(input_0_6_0) = (double) 1.0;
    M(input_0_6_1) = (double) 1.0;
    M(input_0_7_0) = (double) 1.0;
    M(input_0_7_1) = (double) 1.0;
    M(input_0_8_0) = (double) 1.0;
    M(input_0_8_1) = (double) 1.0;
    M(input_0_9_0) = (double) 1.0;
    M(input_0_9_1) = (double) 1.0;
    M(input_0_10_0) = (double) 1.0;
    M(input_0_10_1) = (double) 1.0;
    M(input_0_11_0) = (double) 1.0;
    M(input_0_11_1) = (double) 1.0;
    M(input_0_12_0) = (double) 1.0;
    M(input_0_12_1) = (double) 1.0;
    M(input_0_13_0) = (double) 1.0;
    M(input_0_13_1) = (double) 1.0;
    M(input_0_14_0) = (double) 1.0;
    M(input_0_14_1) = (double) 1.0;
    M(input_0_15_0) = (double) 1.0;
    M(input_0_15_1) = (double) 1.0;
    M(input_0_16_0) = (double) 1.0;
    M(input_0_16_1) = (double) 1.0;
    M(input_0_17_0) = (double) 1.0;
    M(input_0_17_1) = (double) 1.0;
    M(input_0_18_0) = (double) 1.0;
    M(input_0_18_1) = (double) 1.0;
    M(input_0_19_0) = (double) 1.0;
    M(input_0_19_1) = (double) 1.0;
    M(input_0_20_0) = (double) 1.0;
    M(input_0_20_1) = (double) 1.0;
    M(input_0_21_0) = (double) 1.0;
    M(input_0_21_1) = (double) 1.0;
    M(input_0_22_0) = (double) 1.0;
    M(input_0_22_1) = (double) 1.0;
    M(input_0_23_0) = (double) 1.0;
    M(input_0_23_1) = (double) 1.0;
    M(input_0_24_0) = (double) 1.0;
    M(input_0_24_1) = (double) 1.0;
    M(input_0_25_0) = (double) 1.0;
    M(input_0_25_1) = (double) 1.0;
    M(input_0_26_0) = (double) 1.0;
    M(input_0_26_1) = (double) 1.0;
    M(input_0_27_0) = (double) 1.0;
    M(input_0_27_1) = (double) 1.0;
    M(input_0_28_0) = (double) 1.0;
    M(input_0_28_1) = (double) 1.0;
    M(input_0_29_0) = (double) 1.0;
    M(input_0_29_1) = (double) 1.0;
    M(input_0_30_0) = (double) 1.0;
    M(input_0_30_1) = (double) 1.0;
    M(input_0_31_0) = (double) 1.0;
    M(input_0_31_1) = (double) 1.0;
    M(input_0_32_0) = (double) 1.0;
    M(input_0_32_1) = (double) 1.0;
    M(input_0_33_0) = (double) 1.0;
    M(input_0_33_1) = (double) 1.0;
    M(input_0_34_0) = (double) 1.0;
    M(input_0_34_1) = (double) 1.0;
    M(input_0_35_0) = (double) 1.0;
    M(input_0_35_1) = (double) 1.0;
    M(input_0_36_0) = (double) 1.0;
    M(input_0_36_1) = (double) 1.0;
    M(input_0_37_0) = (double) 1.0;
    M(input_0_37_1) = (double) 1.0;
    M(input_0_38_0) = (double) 1.0;
    M(input_0_38_1) = (double) 1.0;
    M(input_0_39_0) = (double) 1.0;
    M(input_0_39_1) = (double) 1.0;
    M(input_0_40_0) = (double) 1.0;
    M(input_0_40_1) = (double) 1.0;
    M(input_0_41_0) = (double) 1.0;
    M(input_0_41_1) = (double) 1.0;
    M(input_0_42_0) = (double) 1.0;
    M(input_0_42_1) = (double) 1.0;
    M(input_0_43_0) = (double) 1.0;
    M(input_0_43_1) = (double) 1.0;
    M(input_0_44_0) = (double) 1.0;
    M(input_0_44_1) = (double) 1.0;
    M(input_0_45_0) = (double) 1.0;
    M(input_0_45_1) = (double) 1.0;
    M(input_0_46_0) = (double) 1.0;
    M(input_0_46_1) = (double) 1.0;
    M(input_0_47_0) = (double) 1.0;
    M(input_0_47_1) = (double) 1.0;
    M(input_0_48_0) = (double) 1.0;
    M(input_0_48_1) = (double) 1.0;
    M(input_0_49_0) = (double) 1.0;
    M(input_0_49_1) = (double) 1.0;
    M(input_0_50_0) = (double) 1.0;
    M(input_0_50_1) = (double) 1.0;
    M(input_0_51_0) = (double) 1.0;
    M(input_0_51_1) = (double) 1.0;
    M(input_0_52_0) = (double) 1.0;
    M(input_0_52_1) = (double) 1.0;
    M(input_0_53_0) = (double) 1.0;
    M(input_0_53_1) = (double) 1.0;
    M(input_0_54_0) = (double) 1.0;
    M(input_0_54_1) = (double) 1.0;
    M(input_0_55_0) = (double) 1.0;
    M(input_0_55_1) = (double) 1.0;
    M(input_0_56_0) = (double) 1.0;
    M(input_0_56_1) = (double) 1.0;
    M(input_0_57_0) = (double) 1.0;
    M(input_0_57_1) = (double) 1.0;
    M(input_0_58_0) = (double) 1.0;
    M(input_0_58_1) = (double) 1.0;
    M(input_0_59_0) = (double) 1.0;
    M(input_0_59_1) = (double) 1.0;
    M(input_0_60_0) = (double) 1.0;
    M(input_0_60_1) = (double) 1.0;
    M(input_0_61_0) = (double) 1.0;
    M(input_0_61_1) = (double) 1.0;
    M(input_0_62_0) = (double) 1.0;
    M(input_0_62_1) = (double) 1.0;
    M(input_0_63_0) = (double) 1.0;
    M(input_0_63_1) = (double) 1.0;
    M(input_0_64_0) = (double) 1.0;
    M(input_0_64_1) = (double) 1.0;
    M(input_0_65_0) = (double) 1.0;
    M(input_0_65_1) = (double) 1.0;
    M(input_0_66_0) = (double) 1.0;
    M(input_0_66_1) = (double) 1.0;
    M(input_0_67_0) = (double) 1.0;
    M(input_0_67_1) = (double) 1.0;
    M(input_0_68_0) = (double) 1.0;
    M(input_0_68_1) = (double) 1.0;
    M(input_0_69_0) = (double) 1.0;
    M(input_0_69_1) = (double) 1.0;
    M(input_0_70_0) = (double) 1.0;
    M(input_0_70_1) = (double) 1.0;
    M(input_0_71_0) = (double) 1.0;
    M(input_0_71_1) = (double) 1.0;
    M(input_0_72_0) = (double) 1.0;
    M(input_0_72_1) = (double) 1.0;
    M(input_0_73_0) = (double) 1.0;
    M(input_0_73_1) = (double) 1.0;
    M(input_0_74_0) = (double) 1.0;
    M(input_0_74_1) = (double) 1.0;
    M(input_0_75_0) = (double) 1.0;
    M(input_0_75_1) = (double) 1.0;
    M(input_0_76_0) = (double) 1.0;
    M(input_0_76_1) = (double) 1.0;
    M(input_0_77_0) = (double) 1.0;
    M(input_0_77_1) = (double) 1.0;
    M(input_0_78_0) = (double) 1.0;
    M(input_0_78_1) = (double) 1.0;
    M(input_0_79_0) = (double) 1.0;
    M(input_0_79_1) = (double) 1.0;
    M(input_0_80_0) = (double) 1.0;
    M(input_0_80_1) = (double) 1.0;
    M(input_0_81_0) = (double) 1.0;
    M(input_0_81_1) = (double) 1.0;
    M(input_0_82_0) = (double) 1.0;
    M(input_0_82_1) = (double) 1.0;
    M(input_0_83_0) = (double) 1.0;
    M(input_0_83_1) = (double) 1.0;
    M(input_0_84_0) = (double) 1.0;
    M(input_0_84_1) = (double) 1.0;
    M(input_0_85_0) = (double) 1.0;
    M(input_0_85_1) = (double) 1.0;
    M(input_0_86_0) = (double) 1.0;
    M(input_0_86_1) = (double) 1.0;
    M(input_0_87_0) = (double) 1.0;
    M(input_0_87_1) = (double) 1.0;
    M(input_0_88_0) = (double) 1.0;
    M(input_0_88_1) = (double) 1.0;
    M(input_0_89_0) = (double) 1.0;
    M(input_0_89_1) = (double) 1.0;
    M(input_0_90_0) = (double) 1.0;
    M(input_0_90_1) = (double) 1.0;
    M(input_0_91_0) = (double) 1.0;
    M(input_0_91_1) = (double) 1.0;
    M(input_0_92_0) = (double) 1.0;
    M(input_0_92_1) = (double) 1.0;
    M(input_0_93_0) = (double) 1.0;
    M(input_0_93_1) = (double) 1.0;
    M(input_0_94_0) = (double) 1.0;
    M(input_0_94_1) = (double) 1.0;
    M(input_0_95_0) = (double) 1.0;
    M(input_0_95_1) = (double) 1.0;
    M(input_0_96_0) = (double) 1.0;
    M(input_0_96_1) = (double) 1.0;
    M(input_0_97_0) = (double) 1.0;
    M(input_0_97_1) = (double) 1.0;
    M(input_0_98_0) = (double) 1.0;
    M(input_0_98_1) = (double) 1.0;
    M(input_0_99_0) = (double) 1.0;
    M(input_0_99_1) = (double) 1.0;
    M(input_0_100_0) = (double) 1.0;
    M(input_0_100_1) = (double) 1.0;
    M(input_0_101_0) = (double) 1.0;
    M(input_0_101_1) = (double) 1.0;
    M(input_0_102_0) = (double) 1.0;
    M(input_0_102_1) = (double) 1.0;
    M(input_0_103_0) = (double) 1.0;
    M(input_0_103_1) = (double) 1.0;
    M(input_0_104_0) = (double) 1.0;
    M(input_0_104_1) = (double) 1.0;
    M(input_0_105_0) = (double) 1.0;
    M(input_0_105_1) = (double) 1.0;
    M(input_0_106_0) = (double) 1.0;
    M(input_0_106_1) = (double) 1.0;
    M(input_0_107_0) = (double) 1.0;
    M(input_0_107_1) = (double) 1.0;
    M(input_0_108_0) = (double) 1.0;
    M(input_0_108_1) = (double) 1.0;
    M(input_0_109_0) = (double) 1.0;
    M(input_0_109_1) = (double) 1.0;
    M(input_0_110_0) = (double) 1.0;
    M(input_0_110_1) = (double) 1.0;
    M(input_0_111_0) = (double) 1.0;
    M(input_0_111_1) = (double) 1.0;
    M(input_0_112_0) = (double) 1.0;
    M(input_0_112_1) = (double) 1.0;
    M(input_0_113_0) = (double) 1.0;
    M(input_0_113_1) = (double) 1.0;
    M(input_0_114_0) = (double) 1.0;
    M(input_0_114_1) = (double) 1.0;
    M(input_0_115_0) = (double) 1.0;
    M(input_0_115_1) = (double) 1.0;
    M(input_0_116_0) = (double) 1.0;
    M(input_0_116_1) = (double) 1.0;
    M(input_0_117_0) = (double) 1.0;
    M(input_0_117_1) = (double) 1.0;
    M(input_0_118_0) = (double) 1.0;
    M(input_0_118_1) = (double) 1.0;
    M(input_0_119_0) = (double) 1.0;
    M(input_0_119_1) = (double) 1.0;
    M(input_0_120_0) = (double) 1.0;
    M(input_0_120_1) = (double) 1.0;
    M(input_0_121_0) = (double) 1.0;
    M(input_0_121_1) = (double) 1.0;
    M(input_0_122_0) = (double) 1.0;
    M(input_0_122_1) = (double) 1.0;
    M(input_0_123_0) = (double) 1.0;
    M(input_0_123_1) = (double) 1.0;
    M(input_0_124_0) = (double) 1.0;
    M(input_0_124_1) = (double) 1.0;
    M(input_0_125_0) = (double) 1.0;
    M(input_0_125_1) = (double) 1.0;
    M(input_0_126_0) = (double) 1.0;
    M(input_0_126_1) = (double) 1.0;
    M(input_0_127_0) = (double) 1.0;
    M(input_0_127_1) = (double) 1.0;
    M(input_0_128_0) = (double) 1.0;
    M(input_0_128_1) = (double) 1.0;
    M(input_0_129_0) = (double) 1.0;
    M(input_0_129_1) = (double) 1.0;
    M(input_0_130_0) = (double) 1.0;
    M(input_0_130_1) = (double) 1.0;
    M(input_0_131_0) = (double) 1.0;
    M(input_0_131_1) = (double) 1.0;
    M(input_0_132_0) = (double) 1.0;
    M(input_0_132_1) = (double) 1.0;
    M(input_0_133_0) = (double) 1.0;
    M(input_0_133_1) = (double) 1.0;
    M(input_0_134_0) = (double) 1.0;
    M(input_0_134_1) = (double) 1.0;
    M(input_0_135_0) = (double) 1.0;
    M(input_0_135_1) = (double) 1.0;
    M(input_0_136_0) = (double) 1.0;
    M(input_0_136_1) = (double) 1.0;
    M(input_0_137_0) = (double) 1.0;
    M(input_0_137_1) = (double) 1.0;
    M(input_0_138_0) = (double) 1.0;
    M(input_0_138_1) = (double) 1.0;
    M(input_0_139_0) = (double) 1.0;
    M(input_0_139_1) = (double) 1.0;
    M(input_0_140_0) = (double) 1.0;
    M(input_0_140_1) = (double) 1.0;
    M(input_0_141_0) = (double) 1.0;
    M(input_0_141_1) = (double) 1.0;
    M(input_0_142_0) = (double) 1.0;
    M(input_0_142_1) = (double) 1.0;
    M(input_0_143_0) = (double) 1.0;
    M(input_0_143_1) = (double) 1.0;
    M(input_0_144_0) = (double) 1.0;
    M(input_0_144_1) = (double) 1.0;
    M(input_0_145_0) = (double) 1.0;
    M(input_0_145_1) = (double) 1.0;
    M(input_0_146_0) = (double) 1.0;
    M(input_0_146_1) = (double) 1.0;
    M(input_0_147_0) = (double) 1.0;
    M(input_0_147_1) = (double) 1.0;
    M(input_0_148_0) = (double) 1.0;
    M(input_0_148_1) = (double) 1.0;
    M(input_0_149_0) = (double) 1.0;
    M(input_0_149_1) = (double) 1.0;
    M(input_0_150_0) = (double) 1.0;
    M(input_0_150_1) = (double) 1.0;
    M(input_0_151_0) = (double) 1.0;
    M(input_0_151_1) = (double) 1.0;
    M(input_0_152_0) = (double) 1.0;
    M(input_0_152_1) = (double) 1.0;
    M(input_0_153_0) = (double) 1.0;
    M(input_0_153_1) = (double) 1.0;
    M(input_0_154_0) = (double) 1.0;
    M(input_0_154_1) = (double) 1.0;
    M(input_0_155_0) = (double) 1.0;
    M(input_0_155_1) = (double) 1.0;
    M(input_0_156_0) = (double) 1.0;
    M(input_0_156_1) = (double) 1.0;
    M(input_0_157_0) = (double) 1.0;
    M(input_0_157_1) = (double) 1.0;
    M(input_0_158_0) = (double) 1.0;
    M(input_0_158_1) = (double) 1.0;
    M(input_0_159_0) = (double) 1.0;
    M(input_0_159_1) = (double) 1.0;
    M(input_0_160_0) = (double) 1.0;
    M(input_0_160_1) = (double) 1.0;
    M(input_0_161_0) = (double) 1.0;
    M(input_0_161_1) = (double) 1.0;
    M(input_0_162_0) = (double) 1.0;
    M(input_0_162_1) = (double) 1.0;
    M(input_0_163_0) = (double) 1.0;
    M(input_0_163_1) = (double) 1.0;
    M(input_0_164_0) = (double) 1.0;
    M(input_0_164_1) = (double) 1.0;
    M(input_0_165_0) = (double) 1.0;
    M(input_0_165_1) = (double) 1.0;
    M(input_0_166_0) = (double) 1.0;
    M(input_0_166_1) = (double) 1.0;
    M(input_0_167_0) = (double) 1.0;
    M(input_0_167_1) = (double) 1.0;
    M(input_0_168_0) = (double) 1.0;
    M(input_0_168_1) = (double) 1.0;
    M(input_0_169_0) = (double) 1.0;
    M(input_0_169_1) = (double) 1.0;
    M(input_0_170_0) = (double) 1.0;
    M(input_0_170_1) = (double) 1.0;
    M(input_0_171_0) = (double) 1.0;
    M(input_0_171_1) = (double) 1.0;
    M(input_0_172_0) = (double) 1.0;
    M(input_0_172_1) = (double) 1.0;
    M(input_0_173_0) = (double) 1.0;
    M(input_0_173_1) = (double) 1.0;
    M(input_0_174_0) = (double) 1.0;
    M(input_0_174_1) = (double) 1.0;
    M(input_0_175_0) = (double) 1.0;
    M(input_0_175_1) = (double) 1.0;
    M(input_0_176_0) = (double) 1.0;
    M(input_0_176_1) = (double) 1.0;
    M(input_0_177_0) = (double) 1.0;
    M(input_0_177_1) = (double) 1.0;
    M(input_0_178_0) = (double) 1.0;
    M(input_0_178_1) = (double) 1.0;
    M(input_0_179_0) = (double) 1.0;
    M(input_0_179_1) = (double) 1.0;
    M(input_0_180_0) = (double) 1.0;
    M(input_0_180_1) = (double) 1.0;
    M(input_0_181_0) = (double) 1.0;
    M(input_0_181_1) = (double) 1.0;
    M(input_0_182_0) = (double) 1.0;
    M(input_0_182_1) = (double) 1.0;
    M(input_0_183_0) = (double) 1.0;
    M(input_0_183_1) = (double) 1.0;
    M(input_0_184_0) = (double) 1.0;
    M(input_0_184_1) = (double) 1.0;
    M(input_0_185_0) = (double) 1.0;
    M(input_0_185_1) = (double) 1.0;
    M(input_0_186_0) = (double) 1.0;
    M(input_0_186_1) = (double) 1.0;
    M(input_0_187_0) = (double) 1.0;
    M(input_0_187_1) = (double) 1.0;
    M(input_0_188_0) = (double) 1.0;
    M(input_0_188_1) = (double) 1.0;
    M(input_0_189_0) = (double) 1.0;
    M(input_0_189_1) = (double) 1.0;
    M(input_0_190_0) = (double) 1.0;
    M(input_0_190_1) = (double) 1.0;
    M(input_0_191_0) = (double) 1.0;
    M(input_0_191_1) = (double) 1.0;
    M(input_0_192_0) = (double) 1.0;
    M(input_0_192_1) = (double) 1.0;
    M(input_0_193_0) = (double) 1.0;
    M(input_0_193_1) = (double) 1.0;
    M(input_0_194_0) = (double) 1.0;
    M(input_0_194_1) = (double) 1.0;
    M(input_0_195_0) = (double) 1.0;
    M(input_0_195_1) = (double) 1.0;
    M(input_0_196_0) = (double) 1.0;
    M(input_0_196_1) = (double) 1.0;
    M(input_0_197_0) = (double) 1.0;
    M(input_0_197_1) = (double) 1.0;
    M(input_0_198_0) = (double) 1.0;
    M(input_0_198_1) = (double) 1.0;
    M(input_0_199_0) = (double) 1.0;
    M(input_0_199_1) = (double) 1.0;
    // Local variables
}

Status calculateValues(ModelInstance *comp) {

    // Do I need memory info?
    OrtMemoryInfo* memory_info;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->CreateCpuMemoryInfo(
            OrtArenaAllocator, OrtMemTypeDefault, &memory_info
        ),
        comp);

    // Create input tensor
    OrtValue* input_tensor;

    // Store the shape of the input tensor
    const size_t input_shape[] = { 1, 200, 2 };

    // Determine the dimensions of the input tensor
    const size_t input_dim = sizeof(input_shape) / sizeof(input_shape[0]);
    size_t input_size = 1;
    for (size_t i = 0; i < input_dim; ++i) {
        input_size *= input_shape[i];
    }

    // Store values in the flattened array
    float* input_float = (float*)malloc(input_size * sizeof(float));
    if (input_float == NULL) {
        logError(comp, "Failed to allocate memory for input_float");
        return Error;
    }

    // Flatten the input array
    input_float[0] = (float)M(input_0_0_0);
    input_float[1] = (float)M(input_0_0_1);
    input_float[2] = (float)M(input_0_1_0);
    input_float[3] = (float)M(input_0_1_1);
    input_float[4] = (float)M(input_0_2_0);
    input_float[5] = (float)M(input_0_2_1);
    input_float[6] = (float)M(input_0_3_0);
    input_float[7] = (float)M(input_0_3_1);
    input_float[8] = (float)M(input_0_4_0);
    input_float[9] = (float)M(input_0_4_1);
    input_float[10] = (float)M(input_0_5_0);
    input_float[11] = (float)M(input_0_5_1);
    input_float[12] = (float)M(input_0_6_0);
    input_float[13] = (float)M(input_0_6_1);
    input_float[14] = (float)M(input_0_7_0);
    input_float[15] = (float)M(input_0_7_1);
    input_float[16] = (float)M(input_0_8_0);
    input_float[17] = (float)M(input_0_8_1);
    input_float[18] = (float)M(input_0_9_0);
    input_float[19] = (float)M(input_0_9_1);
    input_float[20] = (float)M(input_0_10_0);
    input_float[21] = (float)M(input_0_10_1);
    input_float[22] = (float)M(input_0_11_0);
    input_float[23] = (float)M(input_0_11_1);
    input_float[24] = (float)M(input_0_12_0);
    input_float[25] = (float)M(input_0_12_1);
    input_float[26] = (float)M(input_0_13_0);
    input_float[27] = (float)M(input_0_13_1);
    input_float[28] = (float)M(input_0_14_0);
    input_float[29] = (float)M(input_0_14_1);
    input_float[30] = (float)M(input_0_15_0);
    input_float[31] = (float)M(input_0_15_1);
    input_float[32] = (float)M(input_0_16_0);
    input_float[33] = (float)M(input_0_16_1);
    input_float[34] = (float)M(input_0_17_0);
    input_float[35] = (float)M(input_0_17_1);
    input_float[36] = (float)M(input_0_18_0);
    input_float[37] = (float)M(input_0_18_1);
    input_float[38] = (float)M(input_0_19_0);
    input_float[39] = (float)M(input_0_19_1);
    input_float[40] = (float)M(input_0_20_0);
    input_float[41] = (float)M(input_0_20_1);
    input_float[42] = (float)M(input_0_21_0);
    input_float[43] = (float)M(input_0_21_1);
    input_float[44] = (float)M(input_0_22_0);
    input_float[45] = (float)M(input_0_22_1);
    input_float[46] = (float)M(input_0_23_0);
    input_float[47] = (float)M(input_0_23_1);
    input_float[48] = (float)M(input_0_24_0);
    input_float[49] = (float)M(input_0_24_1);
    input_float[50] = (float)M(input_0_25_0);
    input_float[51] = (float)M(input_0_25_1);
    input_float[52] = (float)M(input_0_26_0);
    input_float[53] = (float)M(input_0_26_1);
    input_float[54] = (float)M(input_0_27_0);
    input_float[55] = (float)M(input_0_27_1);
    input_float[56] = (float)M(input_0_28_0);
    input_float[57] = (float)M(input_0_28_1);
    input_float[58] = (float)M(input_0_29_0);
    input_float[59] = (float)M(input_0_29_1);
    input_float[60] = (float)M(input_0_30_0);
    input_float[61] = (float)M(input_0_30_1);
    input_float[62] = (float)M(input_0_31_0);
    input_float[63] = (float)M(input_0_31_1);
    input_float[64] = (float)M(input_0_32_0);
    input_float[65] = (float)M(input_0_32_1);
    input_float[66] = (float)M(input_0_33_0);
    input_float[67] = (float)M(input_0_33_1);
    input_float[68] = (float)M(input_0_34_0);
    input_float[69] = (float)M(input_0_34_1);
    input_float[70] = (float)M(input_0_35_0);
    input_float[71] = (float)M(input_0_35_1);
    input_float[72] = (float)M(input_0_36_0);
    input_float[73] = (float)M(input_0_36_1);
    input_float[74] = (float)M(input_0_37_0);
    input_float[75] = (float)M(input_0_37_1);
    input_float[76] = (float)M(input_0_38_0);
    input_float[77] = (float)M(input_0_38_1);
    input_float[78] = (float)M(input_0_39_0);
    input_float[79] = (float)M(input_0_39_1);
    input_float[80] = (float)M(input_0_40_0);
    input_float[81] = (float)M(input_0_40_1);
    input_float[82] = (float)M(input_0_41_0);
    input_float[83] = (float)M(input_0_41_1);
    input_float[84] = (float)M(input_0_42_0);
    input_float[85] = (float)M(input_0_42_1);
    input_float[86] = (float)M(input_0_43_0);
    input_float[87] = (float)M(input_0_43_1);
    input_float[88] = (float)M(input_0_44_0);
    input_float[89] = (float)M(input_0_44_1);
    input_float[90] = (float)M(input_0_45_0);
    input_float[91] = (float)M(input_0_45_1);
    input_float[92] = (float)M(input_0_46_0);
    input_float[93] = (float)M(input_0_46_1);
    input_float[94] = (float)M(input_0_47_0);
    input_float[95] = (float)M(input_0_47_1);
    input_float[96] = (float)M(input_0_48_0);
    input_float[97] = (float)M(input_0_48_1);
    input_float[98] = (float)M(input_0_49_0);
    input_float[99] = (float)M(input_0_49_1);
    input_float[100] = (float)M(input_0_50_0);
    input_float[101] = (float)M(input_0_50_1);
    input_float[102] = (float)M(input_0_51_0);
    input_float[103] = (float)M(input_0_51_1);
    input_float[104] = (float)M(input_0_52_0);
    input_float[105] = (float)M(input_0_52_1);
    input_float[106] = (float)M(input_0_53_0);
    input_float[107] = (float)M(input_0_53_1);
    input_float[108] = (float)M(input_0_54_0);
    input_float[109] = (float)M(input_0_54_1);
    input_float[110] = (float)M(input_0_55_0);
    input_float[111] = (float)M(input_0_55_1);
    input_float[112] = (float)M(input_0_56_0);
    input_float[113] = (float)M(input_0_56_1);
    input_float[114] = (float)M(input_0_57_0);
    input_float[115] = (float)M(input_0_57_1);
    input_float[116] = (float)M(input_0_58_0);
    input_float[117] = (float)M(input_0_58_1);
    input_float[118] = (float)M(input_0_59_0);
    input_float[119] = (float)M(input_0_59_1);
    input_float[120] = (float)M(input_0_60_0);
    input_float[121] = (float)M(input_0_60_1);
    input_float[122] = (float)M(input_0_61_0);
    input_float[123] = (float)M(input_0_61_1);
    input_float[124] = (float)M(input_0_62_0);
    input_float[125] = (float)M(input_0_62_1);
    input_float[126] = (float)M(input_0_63_0);
    input_float[127] = (float)M(input_0_63_1);
    input_float[128] = (float)M(input_0_64_0);
    input_float[129] = (float)M(input_0_64_1);
    input_float[130] = (float)M(input_0_65_0);
    input_float[131] = (float)M(input_0_65_1);
    input_float[132] = (float)M(input_0_66_0);
    input_float[133] = (float)M(input_0_66_1);
    input_float[134] = (float)M(input_0_67_0);
    input_float[135] = (float)M(input_0_67_1);
    input_float[136] = (float)M(input_0_68_0);
    input_float[137] = (float)M(input_0_68_1);
    input_float[138] = (float)M(input_0_69_0);
    input_float[139] = (float)M(input_0_69_1);
    input_float[140] = (float)M(input_0_70_0);
    input_float[141] = (float)M(input_0_70_1);
    input_float[142] = (float)M(input_0_71_0);
    input_float[143] = (float)M(input_0_71_1);
    input_float[144] = (float)M(input_0_72_0);
    input_float[145] = (float)M(input_0_72_1);
    input_float[146] = (float)M(input_0_73_0);
    input_float[147] = (float)M(input_0_73_1);
    input_float[148] = (float)M(input_0_74_0);
    input_float[149] = (float)M(input_0_74_1);
    input_float[150] = (float)M(input_0_75_0);
    input_float[151] = (float)M(input_0_75_1);
    input_float[152] = (float)M(input_0_76_0);
    input_float[153] = (float)M(input_0_76_1);
    input_float[154] = (float)M(input_0_77_0);
    input_float[155] = (float)M(input_0_77_1);
    input_float[156] = (float)M(input_0_78_0);
    input_float[157] = (float)M(input_0_78_1);
    input_float[158] = (float)M(input_0_79_0);
    input_float[159] = (float)M(input_0_79_1);
    input_float[160] = (float)M(input_0_80_0);
    input_float[161] = (float)M(input_0_80_1);
    input_float[162] = (float)M(input_0_81_0);
    input_float[163] = (float)M(input_0_81_1);
    input_float[164] = (float)M(input_0_82_0);
    input_float[165] = (float)M(input_0_82_1);
    input_float[166] = (float)M(input_0_83_0);
    input_float[167] = (float)M(input_0_83_1);
    input_float[168] = (float)M(input_0_84_0);
    input_float[169] = (float)M(input_0_84_1);
    input_float[170] = (float)M(input_0_85_0);
    input_float[171] = (float)M(input_0_85_1);
    input_float[172] = (float)M(input_0_86_0);
    input_float[173] = (float)M(input_0_86_1);
    input_float[174] = (float)M(input_0_87_0);
    input_float[175] = (float)M(input_0_87_1);
    input_float[176] = (float)M(input_0_88_0);
    input_float[177] = (float)M(input_0_88_1);
    input_float[178] = (float)M(input_0_89_0);
    input_float[179] = (float)M(input_0_89_1);
    input_float[180] = (float)M(input_0_90_0);
    input_float[181] = (float)M(input_0_90_1);
    input_float[182] = (float)M(input_0_91_0);
    input_float[183] = (float)M(input_0_91_1);
    input_float[184] = (float)M(input_0_92_0);
    input_float[185] = (float)M(input_0_92_1);
    input_float[186] = (float)M(input_0_93_0);
    input_float[187] = (float)M(input_0_93_1);
    input_float[188] = (float)M(input_0_94_0);
    input_float[189] = (float)M(input_0_94_1);
    input_float[190] = (float)M(input_0_95_0);
    input_float[191] = (float)M(input_0_95_1);
    input_float[192] = (float)M(input_0_96_0);
    input_float[193] = (float)M(input_0_96_1);
    input_float[194] = (float)M(input_0_97_0);
    input_float[195] = (float)M(input_0_97_1);
    input_float[196] = (float)M(input_0_98_0);
    input_float[197] = (float)M(input_0_98_1);
    input_float[198] = (float)M(input_0_99_0);
    input_float[199] = (float)M(input_0_99_1);
    input_float[200] = (float)M(input_0_100_0);
    input_float[201] = (float)M(input_0_100_1);
    input_float[202] = (float)M(input_0_101_0);
    input_float[203] = (float)M(input_0_101_1);
    input_float[204] = (float)M(input_0_102_0);
    input_float[205] = (float)M(input_0_102_1);
    input_float[206] = (float)M(input_0_103_0);
    input_float[207] = (float)M(input_0_103_1);
    input_float[208] = (float)M(input_0_104_0);
    input_float[209] = (float)M(input_0_104_1);
    input_float[210] = (float)M(input_0_105_0);
    input_float[211] = (float)M(input_0_105_1);
    input_float[212] = (float)M(input_0_106_0);
    input_float[213] = (float)M(input_0_106_1);
    input_float[214] = (float)M(input_0_107_0);
    input_float[215] = (float)M(input_0_107_1);
    input_float[216] = (float)M(input_0_108_0);
    input_float[217] = (float)M(input_0_108_1);
    input_float[218] = (float)M(input_0_109_0);
    input_float[219] = (float)M(input_0_109_1);
    input_float[220] = (float)M(input_0_110_0);
    input_float[221] = (float)M(input_0_110_1);
    input_float[222] = (float)M(input_0_111_0);
    input_float[223] = (float)M(input_0_111_1);
    input_float[224] = (float)M(input_0_112_0);
    input_float[225] = (float)M(input_0_112_1);
    input_float[226] = (float)M(input_0_113_0);
    input_float[227] = (float)M(input_0_113_1);
    input_float[228] = (float)M(input_0_114_0);
    input_float[229] = (float)M(input_0_114_1);
    input_float[230] = (float)M(input_0_115_0);
    input_float[231] = (float)M(input_0_115_1);
    input_float[232] = (float)M(input_0_116_0);
    input_float[233] = (float)M(input_0_116_1);
    input_float[234] = (float)M(input_0_117_0);
    input_float[235] = (float)M(input_0_117_1);
    input_float[236] = (float)M(input_0_118_0);
    input_float[237] = (float)M(input_0_118_1);
    input_float[238] = (float)M(input_0_119_0);
    input_float[239] = (float)M(input_0_119_1);
    input_float[240] = (float)M(input_0_120_0);
    input_float[241] = (float)M(input_0_120_1);
    input_float[242] = (float)M(input_0_121_0);
    input_float[243] = (float)M(input_0_121_1);
    input_float[244] = (float)M(input_0_122_0);
    input_float[245] = (float)M(input_0_122_1);
    input_float[246] = (float)M(input_0_123_0);
    input_float[247] = (float)M(input_0_123_1);
    input_float[248] = (float)M(input_0_124_0);
    input_float[249] = (float)M(input_0_124_1);
    input_float[250] = (float)M(input_0_125_0);
    input_float[251] = (float)M(input_0_125_1);
    input_float[252] = (float)M(input_0_126_0);
    input_float[253] = (float)M(input_0_126_1);
    input_float[254] = (float)M(input_0_127_0);
    input_float[255] = (float)M(input_0_127_1);
    input_float[256] = (float)M(input_0_128_0);
    input_float[257] = (float)M(input_0_128_1);
    input_float[258] = (float)M(input_0_129_0);
    input_float[259] = (float)M(input_0_129_1);
    input_float[260] = (float)M(input_0_130_0);
    input_float[261] = (float)M(input_0_130_1);
    input_float[262] = (float)M(input_0_131_0);
    input_float[263] = (float)M(input_0_131_1);
    input_float[264] = (float)M(input_0_132_0);
    input_float[265] = (float)M(input_0_132_1);
    input_float[266] = (float)M(input_0_133_0);
    input_float[267] = (float)M(input_0_133_1);
    input_float[268] = (float)M(input_0_134_0);
    input_float[269] = (float)M(input_0_134_1);
    input_float[270] = (float)M(input_0_135_0);
    input_float[271] = (float)M(input_0_135_1);
    input_float[272] = (float)M(input_0_136_0);
    input_float[273] = (float)M(input_0_136_1);
    input_float[274] = (float)M(input_0_137_0);
    input_float[275] = (float)M(input_0_137_1);
    input_float[276] = (float)M(input_0_138_0);
    input_float[277] = (float)M(input_0_138_1);
    input_float[278] = (float)M(input_0_139_0);
    input_float[279] = (float)M(input_0_139_1);
    input_float[280] = (float)M(input_0_140_0);
    input_float[281] = (float)M(input_0_140_1);
    input_float[282] = (float)M(input_0_141_0);
    input_float[283] = (float)M(input_0_141_1);
    input_float[284] = (float)M(input_0_142_0);
    input_float[285] = (float)M(input_0_142_1);
    input_float[286] = (float)M(input_0_143_0);
    input_float[287] = (float)M(input_0_143_1);
    input_float[288] = (float)M(input_0_144_0);
    input_float[289] = (float)M(input_0_144_1);
    input_float[290] = (float)M(input_0_145_0);
    input_float[291] = (float)M(input_0_145_1);
    input_float[292] = (float)M(input_0_146_0);
    input_float[293] = (float)M(input_0_146_1);
    input_float[294] = (float)M(input_0_147_0);
    input_float[295] = (float)M(input_0_147_1);
    input_float[296] = (float)M(input_0_148_0);
    input_float[297] = (float)M(input_0_148_1);
    input_float[298] = (float)M(input_0_149_0);
    input_float[299] = (float)M(input_0_149_1);
    input_float[300] = (float)M(input_0_150_0);
    input_float[301] = (float)M(input_0_150_1);
    input_float[302] = (float)M(input_0_151_0);
    input_float[303] = (float)M(input_0_151_1);
    input_float[304] = (float)M(input_0_152_0);
    input_float[305] = (float)M(input_0_152_1);
    input_float[306] = (float)M(input_0_153_0);
    input_float[307] = (float)M(input_0_153_1);
    input_float[308] = (float)M(input_0_154_0);
    input_float[309] = (float)M(input_0_154_1);
    input_float[310] = (float)M(input_0_155_0);
    input_float[311] = (float)M(input_0_155_1);
    input_float[312] = (float)M(input_0_156_0);
    input_float[313] = (float)M(input_0_156_1);
    input_float[314] = (float)M(input_0_157_0);
    input_float[315] = (float)M(input_0_157_1);
    input_float[316] = (float)M(input_0_158_0);
    input_float[317] = (float)M(input_0_158_1);
    input_float[318] = (float)M(input_0_159_0);
    input_float[319] = (float)M(input_0_159_1);
    input_float[320] = (float)M(input_0_160_0);
    input_float[321] = (float)M(input_0_160_1);
    input_float[322] = (float)M(input_0_161_0);
    input_float[323] = (float)M(input_0_161_1);
    input_float[324] = (float)M(input_0_162_0);
    input_float[325] = (float)M(input_0_162_1);
    input_float[326] = (float)M(input_0_163_0);
    input_float[327] = (float)M(input_0_163_1);
    input_float[328] = (float)M(input_0_164_0);
    input_float[329] = (float)M(input_0_164_1);
    input_float[330] = (float)M(input_0_165_0);
    input_float[331] = (float)M(input_0_165_1);
    input_float[332] = (float)M(input_0_166_0);
    input_float[333] = (float)M(input_0_166_1);
    input_float[334] = (float)M(input_0_167_0);
    input_float[335] = (float)M(input_0_167_1);
    input_float[336] = (float)M(input_0_168_0);
    input_float[337] = (float)M(input_0_168_1);
    input_float[338] = (float)M(input_0_169_0);
    input_float[339] = (float)M(input_0_169_1);
    input_float[340] = (float)M(input_0_170_0);
    input_float[341] = (float)M(input_0_170_1);
    input_float[342] = (float)M(input_0_171_0);
    input_float[343] = (float)M(input_0_171_1);
    input_float[344] = (float)M(input_0_172_0);
    input_float[345] = (float)M(input_0_172_1);
    input_float[346] = (float)M(input_0_173_0);
    input_float[347] = (float)M(input_0_173_1);
    input_float[348] = (float)M(input_0_174_0);
    input_float[349] = (float)M(input_0_174_1);
    input_float[350] = (float)M(input_0_175_0);
    input_float[351] = (float)M(input_0_175_1);
    input_float[352] = (float)M(input_0_176_0);
    input_float[353] = (float)M(input_0_176_1);
    input_float[354] = (float)M(input_0_177_0);
    input_float[355] = (float)M(input_0_177_1);
    input_float[356] = (float)M(input_0_178_0);
    input_float[357] = (float)M(input_0_178_1);
    input_float[358] = (float)M(input_0_179_0);
    input_float[359] = (float)M(input_0_179_1);
    input_float[360] = (float)M(input_0_180_0);
    input_float[361] = (float)M(input_0_180_1);
    input_float[362] = (float)M(input_0_181_0);
    input_float[363] = (float)M(input_0_181_1);
    input_float[364] = (float)M(input_0_182_0);
    input_float[365] = (float)M(input_0_182_1);
    input_float[366] = (float)M(input_0_183_0);
    input_float[367] = (float)M(input_0_183_1);
    input_float[368] = (float)M(input_0_184_0);
    input_float[369] = (float)M(input_0_184_1);
    input_float[370] = (float)M(input_0_185_0);
    input_float[371] = (float)M(input_0_185_1);
    input_float[372] = (float)M(input_0_186_0);
    input_float[373] = (float)M(input_0_186_1);
    input_float[374] = (float)M(input_0_187_0);
    input_float[375] = (float)M(input_0_187_1);
    input_float[376] = (float)M(input_0_188_0);
    input_float[377] = (float)M(input_0_188_1);
    input_float[378] = (float)M(input_0_189_0);
    input_float[379] = (float)M(input_0_189_1);
    input_float[380] = (float)M(input_0_190_0);
    input_float[381] = (float)M(input_0_190_1);
    input_float[382] = (float)M(input_0_191_0);
    input_float[383] = (float)M(input_0_191_1);
    input_float[384] = (float)M(input_0_192_0);
    input_float[385] = (float)M(input_0_192_1);
    input_float[386] = (float)M(input_0_193_0);
    input_float[387] = (float)M(input_0_193_1);
    input_float[388] = (float)M(input_0_194_0);
    input_float[389] = (float)M(input_0_194_1);
    input_float[390] = (float)M(input_0_195_0);
    input_float[391] = (float)M(input_0_195_1);
    input_float[392] = (float)M(input_0_196_0);
    input_float[393] = (float)M(input_0_196_1);
    input_float[394] = (float)M(input_0_197_0);
    input_float[395] = (float)M(input_0_197_1);
    input_float[396] = (float)M(input_0_198_0);
    input_float[397] = (float)M(input_0_198_1);
    input_float[398] = (float)M(input_0_199_0);
    input_float[399] = (float)M(input_0_199_1);

    ORT_ABORT_ON_ERROR(
        comp->g_ort->CreateTensorWithDataAsOrtValue(
            memory_info,
            input_float,
            input_size * sizeof(float),
            (const int64_t*)input_shape,
            input_dim,
            ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT,
            &input_tensor
        ),
        comp);

    int input_is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(
            input_tensor,
            &input_is_tensor),
            comp
        );

    assert(input_is_tensor);

    // LOCAL variables

    // Release the memory info
    comp->g_ort->ReleaseMemoryInfo(memory_info);

    // Create output tensors
    OrtValue* output_tensor = NULL;
    // Create output tensors for local variables


    // Declare input node names
    const char* input_names[] = {
        "input"
    };

    // Declare output node names
    const char* output_names[] = {
        "output"
    };

    // Gather input tensors
    const OrtValue* input_tensors[] = {
        input_tensor
    };

    // Gather output tensors
    OrtValue* output_tensors[] = {
        output_tensor
    };

    // Run inference
    ORT_ABORT_ON_ERROR(
        comp->g_ort->Run(
            comp->session,
            NULL,
            input_names,
            input_tensors,
            1,
            output_names,
            1,
            output_tensors
        ),
        comp
    );

    // Check output tensors to be tensors
    int output_is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(
            output_tensors[0],
            &output_is_tensor),
            comp
        );
    assert (output_is_tensor);

    // Check local output tensors to be tensors

    // Retrieve pointer to the output tensor
    float* output_tensor_data = NULL;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetTensorMutableData(
            output_tensors[0],
            (void**)&output_tensor_data
        ),
        comp
    );

    // Retrieve output tensor info
    OrtTensorTypeAndShapeInfo* output_info;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetTensorTypeAndShape(
            output_tensors[0],
            &output_info
        ),
        comp
    );

    // Retrieve output tensor shape
    size_t output_dims;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetDimensionsCount(
            output_info,
            &output_dims
        ),
        comp
    );

    // Set output tensor data to model
    M(output_0) = output_tensor_data[0];

    // Free tensors
    comp->g_ort->ReleaseValue(output_tensor);
    comp->g_ort->ReleaseValue(input_tensor);

    // Free arrays
    free(input_float);

    return OK;
}

Status getFloat64(ModelInstance *comp, ValueReference vr, double values[],
                  size_t nValues, size_t *index) {

    switch (vr)
    {
    case vr_time:
        ASSERT_NVALUES(1);
        values[(*index)++] = M(time);
        return OK;
#if FMI_VERSION < 3
        // Inputs
        case vr_input_0_0_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_0_0);
            return OK;
        case vr_input_0_0_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_0_1);
            return OK;
        case vr_input_0_1_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_1_0);
            return OK;
        case vr_input_0_1_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_1_1);
            return OK;
        case vr_input_0_2_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_2_0);
            return OK;
        case vr_input_0_2_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_2_1);
            return OK;
        case vr_input_0_3_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_3_0);
            return OK;
        case vr_input_0_3_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_3_1);
            return OK;
        case vr_input_0_4_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_4_0);
            return OK;
        case vr_input_0_4_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_4_1);
            return OK;
        case vr_input_0_5_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_5_0);
            return OK;
        case vr_input_0_5_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_5_1);
            return OK;
        case vr_input_0_6_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_6_0);
            return OK;
        case vr_input_0_6_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_6_1);
            return OK;
        case vr_input_0_7_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_7_0);
            return OK;
        case vr_input_0_7_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_7_1);
            return OK;
        case vr_input_0_8_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_8_0);
            return OK;
        case vr_input_0_8_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_8_1);
            return OK;
        case vr_input_0_9_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_9_0);
            return OK;
        case vr_input_0_9_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_9_1);
            return OK;
        case vr_input_0_10_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_10_0);
            return OK;
        case vr_input_0_10_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_10_1);
            return OK;
        case vr_input_0_11_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_11_0);
            return OK;
        case vr_input_0_11_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_11_1);
            return OK;
        case vr_input_0_12_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_12_0);
            return OK;
        case vr_input_0_12_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_12_1);
            return OK;
        case vr_input_0_13_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_13_0);
            return OK;
        case vr_input_0_13_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_13_1);
            return OK;
        case vr_input_0_14_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_14_0);
            return OK;
        case vr_input_0_14_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_14_1);
            return OK;
        case vr_input_0_15_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_15_0);
            return OK;
        case vr_input_0_15_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_15_1);
            return OK;
        case vr_input_0_16_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_16_0);
            return OK;
        case vr_input_0_16_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_16_1);
            return OK;
        case vr_input_0_17_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_17_0);
            return OK;
        case vr_input_0_17_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_17_1);
            return OK;
        case vr_input_0_18_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_18_0);
            return OK;
        case vr_input_0_18_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_18_1);
            return OK;
        case vr_input_0_19_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_19_0);
            return OK;
        case vr_input_0_19_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_19_1);
            return OK;
        case vr_input_0_20_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_20_0);
            return OK;
        case vr_input_0_20_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_20_1);
            return OK;
        case vr_input_0_21_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_21_0);
            return OK;
        case vr_input_0_21_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_21_1);
            return OK;
        case vr_input_0_22_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_22_0);
            return OK;
        case vr_input_0_22_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_22_1);
            return OK;
        case vr_input_0_23_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_23_0);
            return OK;
        case vr_input_0_23_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_23_1);
            return OK;
        case vr_input_0_24_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_24_0);
            return OK;
        case vr_input_0_24_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_24_1);
            return OK;
        case vr_input_0_25_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_25_0);
            return OK;
        case vr_input_0_25_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_25_1);
            return OK;
        case vr_input_0_26_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_26_0);
            return OK;
        case vr_input_0_26_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_26_1);
            return OK;
        case vr_input_0_27_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_27_0);
            return OK;
        case vr_input_0_27_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_27_1);
            return OK;
        case vr_input_0_28_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_28_0);
            return OK;
        case vr_input_0_28_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_28_1);
            return OK;
        case vr_input_0_29_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_29_0);
            return OK;
        case vr_input_0_29_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_29_1);
            return OK;
        case vr_input_0_30_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_30_0);
            return OK;
        case vr_input_0_30_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_30_1);
            return OK;
        case vr_input_0_31_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_31_0);
            return OK;
        case vr_input_0_31_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_31_1);
            return OK;
        case vr_input_0_32_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_32_0);
            return OK;
        case vr_input_0_32_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_32_1);
            return OK;
        case vr_input_0_33_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_33_0);
            return OK;
        case vr_input_0_33_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_33_1);
            return OK;
        case vr_input_0_34_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_34_0);
            return OK;
        case vr_input_0_34_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_34_1);
            return OK;
        case vr_input_0_35_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_35_0);
            return OK;
        case vr_input_0_35_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_35_1);
            return OK;
        case vr_input_0_36_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_36_0);
            return OK;
        case vr_input_0_36_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_36_1);
            return OK;
        case vr_input_0_37_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_37_0);
            return OK;
        case vr_input_0_37_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_37_1);
            return OK;
        case vr_input_0_38_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_38_0);
            return OK;
        case vr_input_0_38_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_38_1);
            return OK;
        case vr_input_0_39_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_39_0);
            return OK;
        case vr_input_0_39_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_39_1);
            return OK;
        case vr_input_0_40_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_40_0);
            return OK;
        case vr_input_0_40_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_40_1);
            return OK;
        case vr_input_0_41_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_41_0);
            return OK;
        case vr_input_0_41_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_41_1);
            return OK;
        case vr_input_0_42_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_42_0);
            return OK;
        case vr_input_0_42_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_42_1);
            return OK;
        case vr_input_0_43_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_43_0);
            return OK;
        case vr_input_0_43_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_43_1);
            return OK;
        case vr_input_0_44_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_44_0);
            return OK;
        case vr_input_0_44_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_44_1);
            return OK;
        case vr_input_0_45_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_45_0);
            return OK;
        case vr_input_0_45_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_45_1);
            return OK;
        case vr_input_0_46_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_46_0);
            return OK;
        case vr_input_0_46_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_46_1);
            return OK;
        case vr_input_0_47_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_47_0);
            return OK;
        case vr_input_0_47_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_47_1);
            return OK;
        case vr_input_0_48_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_48_0);
            return OK;
        case vr_input_0_48_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_48_1);
            return OK;
        case vr_input_0_49_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_49_0);
            return OK;
        case vr_input_0_49_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_49_1);
            return OK;
        case vr_input_0_50_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_50_0);
            return OK;
        case vr_input_0_50_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_50_1);
            return OK;
        case vr_input_0_51_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_51_0);
            return OK;
        case vr_input_0_51_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_51_1);
            return OK;
        case vr_input_0_52_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_52_0);
            return OK;
        case vr_input_0_52_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_52_1);
            return OK;
        case vr_input_0_53_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_53_0);
            return OK;
        case vr_input_0_53_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_53_1);
            return OK;
        case vr_input_0_54_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_54_0);
            return OK;
        case vr_input_0_54_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_54_1);
            return OK;
        case vr_input_0_55_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_55_0);
            return OK;
        case vr_input_0_55_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_55_1);
            return OK;
        case vr_input_0_56_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_56_0);
            return OK;
        case vr_input_0_56_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_56_1);
            return OK;
        case vr_input_0_57_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_57_0);
            return OK;
        case vr_input_0_57_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_57_1);
            return OK;
        case vr_input_0_58_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_58_0);
            return OK;
        case vr_input_0_58_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_58_1);
            return OK;
        case vr_input_0_59_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_59_0);
            return OK;
        case vr_input_0_59_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_59_1);
            return OK;
        case vr_input_0_60_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_60_0);
            return OK;
        case vr_input_0_60_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_60_1);
            return OK;
        case vr_input_0_61_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_61_0);
            return OK;
        case vr_input_0_61_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_61_1);
            return OK;
        case vr_input_0_62_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_62_0);
            return OK;
        case vr_input_0_62_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_62_1);
            return OK;
        case vr_input_0_63_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_63_0);
            return OK;
        case vr_input_0_63_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_63_1);
            return OK;
        case vr_input_0_64_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_64_0);
            return OK;
        case vr_input_0_64_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_64_1);
            return OK;
        case vr_input_0_65_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_65_0);
            return OK;
        case vr_input_0_65_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_65_1);
            return OK;
        case vr_input_0_66_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_66_0);
            return OK;
        case vr_input_0_66_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_66_1);
            return OK;
        case vr_input_0_67_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_67_0);
            return OK;
        case vr_input_0_67_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_67_1);
            return OK;
        case vr_input_0_68_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_68_0);
            return OK;
        case vr_input_0_68_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_68_1);
            return OK;
        case vr_input_0_69_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_69_0);
            return OK;
        case vr_input_0_69_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_69_1);
            return OK;
        case vr_input_0_70_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_70_0);
            return OK;
        case vr_input_0_70_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_70_1);
            return OK;
        case vr_input_0_71_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_71_0);
            return OK;
        case vr_input_0_71_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_71_1);
            return OK;
        case vr_input_0_72_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_72_0);
            return OK;
        case vr_input_0_72_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_72_1);
            return OK;
        case vr_input_0_73_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_73_0);
            return OK;
        case vr_input_0_73_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_73_1);
            return OK;
        case vr_input_0_74_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_74_0);
            return OK;
        case vr_input_0_74_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_74_1);
            return OK;
        case vr_input_0_75_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_75_0);
            return OK;
        case vr_input_0_75_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_75_1);
            return OK;
        case vr_input_0_76_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_76_0);
            return OK;
        case vr_input_0_76_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_76_1);
            return OK;
        case vr_input_0_77_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_77_0);
            return OK;
        case vr_input_0_77_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_77_1);
            return OK;
        case vr_input_0_78_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_78_0);
            return OK;
        case vr_input_0_78_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_78_1);
            return OK;
        case vr_input_0_79_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_79_0);
            return OK;
        case vr_input_0_79_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_79_1);
            return OK;
        case vr_input_0_80_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_80_0);
            return OK;
        case vr_input_0_80_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_80_1);
            return OK;
        case vr_input_0_81_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_81_0);
            return OK;
        case vr_input_0_81_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_81_1);
            return OK;
        case vr_input_0_82_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_82_0);
            return OK;
        case vr_input_0_82_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_82_1);
            return OK;
        case vr_input_0_83_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_83_0);
            return OK;
        case vr_input_0_83_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_83_1);
            return OK;
        case vr_input_0_84_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_84_0);
            return OK;
        case vr_input_0_84_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_84_1);
            return OK;
        case vr_input_0_85_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_85_0);
            return OK;
        case vr_input_0_85_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_85_1);
            return OK;
        case vr_input_0_86_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_86_0);
            return OK;
        case vr_input_0_86_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_86_1);
            return OK;
        case vr_input_0_87_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_87_0);
            return OK;
        case vr_input_0_87_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_87_1);
            return OK;
        case vr_input_0_88_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_88_0);
            return OK;
        case vr_input_0_88_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_88_1);
            return OK;
        case vr_input_0_89_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_89_0);
            return OK;
        case vr_input_0_89_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_89_1);
            return OK;
        case vr_input_0_90_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_90_0);
            return OK;
        case vr_input_0_90_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_90_1);
            return OK;
        case vr_input_0_91_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_91_0);
            return OK;
        case vr_input_0_91_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_91_1);
            return OK;
        case vr_input_0_92_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_92_0);
            return OK;
        case vr_input_0_92_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_92_1);
            return OK;
        case vr_input_0_93_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_93_0);
            return OK;
        case vr_input_0_93_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_93_1);
            return OK;
        case vr_input_0_94_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_94_0);
            return OK;
        case vr_input_0_94_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_94_1);
            return OK;
        case vr_input_0_95_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_95_0);
            return OK;
        case vr_input_0_95_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_95_1);
            return OK;
        case vr_input_0_96_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_96_0);
            return OK;
        case vr_input_0_96_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_96_1);
            return OK;
        case vr_input_0_97_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_97_0);
            return OK;
        case vr_input_0_97_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_97_1);
            return OK;
        case vr_input_0_98_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_98_0);
            return OK;
        case vr_input_0_98_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_98_1);
            return OK;
        case vr_input_0_99_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_99_0);
            return OK;
        case vr_input_0_99_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_99_1);
            return OK;
        case vr_input_0_100_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_100_0);
            return OK;
        case vr_input_0_100_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_100_1);
            return OK;
        case vr_input_0_101_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_101_0);
            return OK;
        case vr_input_0_101_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_101_1);
            return OK;
        case vr_input_0_102_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_102_0);
            return OK;
        case vr_input_0_102_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_102_1);
            return OK;
        case vr_input_0_103_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_103_0);
            return OK;
        case vr_input_0_103_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_103_1);
            return OK;
        case vr_input_0_104_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_104_0);
            return OK;
        case vr_input_0_104_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_104_1);
            return OK;
        case vr_input_0_105_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_105_0);
            return OK;
        case vr_input_0_105_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_105_1);
            return OK;
        case vr_input_0_106_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_106_0);
            return OK;
        case vr_input_0_106_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_106_1);
            return OK;
        case vr_input_0_107_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_107_0);
            return OK;
        case vr_input_0_107_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_107_1);
            return OK;
        case vr_input_0_108_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_108_0);
            return OK;
        case vr_input_0_108_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_108_1);
            return OK;
        case vr_input_0_109_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_109_0);
            return OK;
        case vr_input_0_109_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_109_1);
            return OK;
        case vr_input_0_110_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_110_0);
            return OK;
        case vr_input_0_110_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_110_1);
            return OK;
        case vr_input_0_111_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_111_0);
            return OK;
        case vr_input_0_111_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_111_1);
            return OK;
        case vr_input_0_112_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_112_0);
            return OK;
        case vr_input_0_112_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_112_1);
            return OK;
        case vr_input_0_113_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_113_0);
            return OK;
        case vr_input_0_113_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_113_1);
            return OK;
        case vr_input_0_114_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_114_0);
            return OK;
        case vr_input_0_114_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_114_1);
            return OK;
        case vr_input_0_115_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_115_0);
            return OK;
        case vr_input_0_115_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_115_1);
            return OK;
        case vr_input_0_116_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_116_0);
            return OK;
        case vr_input_0_116_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_116_1);
            return OK;
        case vr_input_0_117_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_117_0);
            return OK;
        case vr_input_0_117_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_117_1);
            return OK;
        case vr_input_0_118_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_118_0);
            return OK;
        case vr_input_0_118_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_118_1);
            return OK;
        case vr_input_0_119_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_119_0);
            return OK;
        case vr_input_0_119_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_119_1);
            return OK;
        case vr_input_0_120_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_120_0);
            return OK;
        case vr_input_0_120_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_120_1);
            return OK;
        case vr_input_0_121_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_121_0);
            return OK;
        case vr_input_0_121_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_121_1);
            return OK;
        case vr_input_0_122_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_122_0);
            return OK;
        case vr_input_0_122_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_122_1);
            return OK;
        case vr_input_0_123_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_123_0);
            return OK;
        case vr_input_0_123_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_123_1);
            return OK;
        case vr_input_0_124_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_124_0);
            return OK;
        case vr_input_0_124_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_124_1);
            return OK;
        case vr_input_0_125_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_125_0);
            return OK;
        case vr_input_0_125_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_125_1);
            return OK;
        case vr_input_0_126_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_126_0);
            return OK;
        case vr_input_0_126_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_126_1);
            return OK;
        case vr_input_0_127_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_127_0);
            return OK;
        case vr_input_0_127_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_127_1);
            return OK;
        case vr_input_0_128_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_128_0);
            return OK;
        case vr_input_0_128_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_128_1);
            return OK;
        case vr_input_0_129_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_129_0);
            return OK;
        case vr_input_0_129_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_129_1);
            return OK;
        case vr_input_0_130_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_130_0);
            return OK;
        case vr_input_0_130_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_130_1);
            return OK;
        case vr_input_0_131_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_131_0);
            return OK;
        case vr_input_0_131_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_131_1);
            return OK;
        case vr_input_0_132_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_132_0);
            return OK;
        case vr_input_0_132_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_132_1);
            return OK;
        case vr_input_0_133_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_133_0);
            return OK;
        case vr_input_0_133_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_133_1);
            return OK;
        case vr_input_0_134_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_134_0);
            return OK;
        case vr_input_0_134_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_134_1);
            return OK;
        case vr_input_0_135_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_135_0);
            return OK;
        case vr_input_0_135_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_135_1);
            return OK;
        case vr_input_0_136_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_136_0);
            return OK;
        case vr_input_0_136_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_136_1);
            return OK;
        case vr_input_0_137_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_137_0);
            return OK;
        case vr_input_0_137_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_137_1);
            return OK;
        case vr_input_0_138_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_138_0);
            return OK;
        case vr_input_0_138_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_138_1);
            return OK;
        case vr_input_0_139_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_139_0);
            return OK;
        case vr_input_0_139_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_139_1);
            return OK;
        case vr_input_0_140_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_140_0);
            return OK;
        case vr_input_0_140_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_140_1);
            return OK;
        case vr_input_0_141_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_141_0);
            return OK;
        case vr_input_0_141_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_141_1);
            return OK;
        case vr_input_0_142_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_142_0);
            return OK;
        case vr_input_0_142_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_142_1);
            return OK;
        case vr_input_0_143_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_143_0);
            return OK;
        case vr_input_0_143_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_143_1);
            return OK;
        case vr_input_0_144_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_144_0);
            return OK;
        case vr_input_0_144_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_144_1);
            return OK;
        case vr_input_0_145_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_145_0);
            return OK;
        case vr_input_0_145_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_145_1);
            return OK;
        case vr_input_0_146_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_146_0);
            return OK;
        case vr_input_0_146_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_146_1);
            return OK;
        case vr_input_0_147_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_147_0);
            return OK;
        case vr_input_0_147_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_147_1);
            return OK;
        case vr_input_0_148_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_148_0);
            return OK;
        case vr_input_0_148_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_148_1);
            return OK;
        case vr_input_0_149_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_149_0);
            return OK;
        case vr_input_0_149_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_149_1);
            return OK;
        case vr_input_0_150_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_150_0);
            return OK;
        case vr_input_0_150_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_150_1);
            return OK;
        case vr_input_0_151_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_151_0);
            return OK;
        case vr_input_0_151_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_151_1);
            return OK;
        case vr_input_0_152_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_152_0);
            return OK;
        case vr_input_0_152_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_152_1);
            return OK;
        case vr_input_0_153_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_153_0);
            return OK;
        case vr_input_0_153_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_153_1);
            return OK;
        case vr_input_0_154_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_154_0);
            return OK;
        case vr_input_0_154_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_154_1);
            return OK;
        case vr_input_0_155_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_155_0);
            return OK;
        case vr_input_0_155_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_155_1);
            return OK;
        case vr_input_0_156_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_156_0);
            return OK;
        case vr_input_0_156_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_156_1);
            return OK;
        case vr_input_0_157_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_157_0);
            return OK;
        case vr_input_0_157_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_157_1);
            return OK;
        case vr_input_0_158_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_158_0);
            return OK;
        case vr_input_0_158_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_158_1);
            return OK;
        case vr_input_0_159_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_159_0);
            return OK;
        case vr_input_0_159_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_159_1);
            return OK;
        case vr_input_0_160_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_160_0);
            return OK;
        case vr_input_0_160_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_160_1);
            return OK;
        case vr_input_0_161_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_161_0);
            return OK;
        case vr_input_0_161_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_161_1);
            return OK;
        case vr_input_0_162_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_162_0);
            return OK;
        case vr_input_0_162_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_162_1);
            return OK;
        case vr_input_0_163_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_163_0);
            return OK;
        case vr_input_0_163_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_163_1);
            return OK;
        case vr_input_0_164_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_164_0);
            return OK;
        case vr_input_0_164_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_164_1);
            return OK;
        case vr_input_0_165_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_165_0);
            return OK;
        case vr_input_0_165_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_165_1);
            return OK;
        case vr_input_0_166_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_166_0);
            return OK;
        case vr_input_0_166_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_166_1);
            return OK;
        case vr_input_0_167_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_167_0);
            return OK;
        case vr_input_0_167_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_167_1);
            return OK;
        case vr_input_0_168_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_168_0);
            return OK;
        case vr_input_0_168_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_168_1);
            return OK;
        case vr_input_0_169_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_169_0);
            return OK;
        case vr_input_0_169_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_169_1);
            return OK;
        case vr_input_0_170_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_170_0);
            return OK;
        case vr_input_0_170_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_170_1);
            return OK;
        case vr_input_0_171_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_171_0);
            return OK;
        case vr_input_0_171_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_171_1);
            return OK;
        case vr_input_0_172_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_172_0);
            return OK;
        case vr_input_0_172_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_172_1);
            return OK;
        case vr_input_0_173_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_173_0);
            return OK;
        case vr_input_0_173_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_173_1);
            return OK;
        case vr_input_0_174_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_174_0);
            return OK;
        case vr_input_0_174_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_174_1);
            return OK;
        case vr_input_0_175_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_175_0);
            return OK;
        case vr_input_0_175_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_175_1);
            return OK;
        case vr_input_0_176_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_176_0);
            return OK;
        case vr_input_0_176_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_176_1);
            return OK;
        case vr_input_0_177_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_177_0);
            return OK;
        case vr_input_0_177_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_177_1);
            return OK;
        case vr_input_0_178_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_178_0);
            return OK;
        case vr_input_0_178_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_178_1);
            return OK;
        case vr_input_0_179_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_179_0);
            return OK;
        case vr_input_0_179_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_179_1);
            return OK;
        case vr_input_0_180_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_180_0);
            return OK;
        case vr_input_0_180_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_180_1);
            return OK;
        case vr_input_0_181_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_181_0);
            return OK;
        case vr_input_0_181_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_181_1);
            return OK;
        case vr_input_0_182_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_182_0);
            return OK;
        case vr_input_0_182_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_182_1);
            return OK;
        case vr_input_0_183_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_183_0);
            return OK;
        case vr_input_0_183_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_183_1);
            return OK;
        case vr_input_0_184_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_184_0);
            return OK;
        case vr_input_0_184_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_184_1);
            return OK;
        case vr_input_0_185_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_185_0);
            return OK;
        case vr_input_0_185_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_185_1);
            return OK;
        case vr_input_0_186_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_186_0);
            return OK;
        case vr_input_0_186_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_186_1);
            return OK;
        case vr_input_0_187_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_187_0);
            return OK;
        case vr_input_0_187_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_187_1);
            return OK;
        case vr_input_0_188_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_188_0);
            return OK;
        case vr_input_0_188_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_188_1);
            return OK;
        case vr_input_0_189_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_189_0);
            return OK;
        case vr_input_0_189_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_189_1);
            return OK;
        case vr_input_0_190_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_190_0);
            return OK;
        case vr_input_0_190_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_190_1);
            return OK;
        case vr_input_0_191_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_191_0);
            return OK;
        case vr_input_0_191_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_191_1);
            return OK;
        case vr_input_0_192_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_192_0);
            return OK;
        case vr_input_0_192_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_192_1);
            return OK;
        case vr_input_0_193_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_193_0);
            return OK;
        case vr_input_0_193_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_193_1);
            return OK;
        case vr_input_0_194_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_194_0);
            return OK;
        case vr_input_0_194_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_194_1);
            return OK;
        case vr_input_0_195_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_195_0);
            return OK;
        case vr_input_0_195_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_195_1);
            return OK;
        case vr_input_0_196_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_196_0);
            return OK;
        case vr_input_0_196_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_196_1);
            return OK;
        case vr_input_0_197_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_197_0);
            return OK;
        case vr_input_0_197_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_197_1);
            return OK;
        case vr_input_0_198_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_198_0);
            return OK;
        case vr_input_0_198_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_198_1);
            return OK;
        case vr_input_0_199_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_199_0);
            return OK;
        case vr_input_0_199_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_199_1);
            return OK;
        // Outputs
        case vr_output_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(output_0);
            return OK;
        // Locals
#endif
    default:
        // Compose message for log with value reference
        logError(comp, "getFloat64: ValueReference %d not available.", vr);
        return Error;
    }
}

Status setFloat64(ModelInstance *comp, ValueReference vr, const double values[],
                  size_t nValues, size_t *index) {
    // Switch on the value reference
    switch (vr)
    {
        // Time is always a double value
        case vr_time:
            ASSERT_NVALUES(1);
            M(time) = values[(*index)++];
            return OK;
#if FMI_VERSION < 3
        // Inputs
        case vr_input_0_0_0:
            ASSERT_NVALUES(1);
            M(input_0_0_0) = values[(*index)++];
            return OK;
        case vr_input_0_0_1:
            ASSERT_NVALUES(1);
            M(input_0_0_1) = values[(*index)++];
            return OK;
        case vr_input_0_1_0:
            ASSERT_NVALUES(1);
            M(input_0_1_0) = values[(*index)++];
            return OK;
        case vr_input_0_1_1:
            ASSERT_NVALUES(1);
            M(input_0_1_1) = values[(*index)++];
            return OK;
        case vr_input_0_2_0:
            ASSERT_NVALUES(1);
            M(input_0_2_0) = values[(*index)++];
            return OK;
        case vr_input_0_2_1:
            ASSERT_NVALUES(1);
            M(input_0_2_1) = values[(*index)++];
            return OK;
        case vr_input_0_3_0:
            ASSERT_NVALUES(1);
            M(input_0_3_0) = values[(*index)++];
            return OK;
        case vr_input_0_3_1:
            ASSERT_NVALUES(1);
            M(input_0_3_1) = values[(*index)++];
            return OK;
        case vr_input_0_4_0:
            ASSERT_NVALUES(1);
            M(input_0_4_0) = values[(*index)++];
            return OK;
        case vr_input_0_4_1:
            ASSERT_NVALUES(1);
            M(input_0_4_1) = values[(*index)++];
            return OK;
        case vr_input_0_5_0:
            ASSERT_NVALUES(1);
            M(input_0_5_0) = values[(*index)++];
            return OK;
        case vr_input_0_5_1:
            ASSERT_NVALUES(1);
            M(input_0_5_1) = values[(*index)++];
            return OK;
        case vr_input_0_6_0:
            ASSERT_NVALUES(1);
            M(input_0_6_0) = values[(*index)++];
            return OK;
        case vr_input_0_6_1:
            ASSERT_NVALUES(1);
            M(input_0_6_1) = values[(*index)++];
            return OK;
        case vr_input_0_7_0:
            ASSERT_NVALUES(1);
            M(input_0_7_0) = values[(*index)++];
            return OK;
        case vr_input_0_7_1:
            ASSERT_NVALUES(1);
            M(input_0_7_1) = values[(*index)++];
            return OK;
        case vr_input_0_8_0:
            ASSERT_NVALUES(1);
            M(input_0_8_0) = values[(*index)++];
            return OK;
        case vr_input_0_8_1:
            ASSERT_NVALUES(1);
            M(input_0_8_1) = values[(*index)++];
            return OK;
        case vr_input_0_9_0:
            ASSERT_NVALUES(1);
            M(input_0_9_0) = values[(*index)++];
            return OK;
        case vr_input_0_9_1:
            ASSERT_NVALUES(1);
            M(input_0_9_1) = values[(*index)++];
            return OK;
        case vr_input_0_10_0:
            ASSERT_NVALUES(1);
            M(input_0_10_0) = values[(*index)++];
            return OK;
        case vr_input_0_10_1:
            ASSERT_NVALUES(1);
            M(input_0_10_1) = values[(*index)++];
            return OK;
        case vr_input_0_11_0:
            ASSERT_NVALUES(1);
            M(input_0_11_0) = values[(*index)++];
            return OK;
        case vr_input_0_11_1:
            ASSERT_NVALUES(1);
            M(input_0_11_1) = values[(*index)++];
            return OK;
        case vr_input_0_12_0:
            ASSERT_NVALUES(1);
            M(input_0_12_0) = values[(*index)++];
            return OK;
        case vr_input_0_12_1:
            ASSERT_NVALUES(1);
            M(input_0_12_1) = values[(*index)++];
            return OK;
        case vr_input_0_13_0:
            ASSERT_NVALUES(1);
            M(input_0_13_0) = values[(*index)++];
            return OK;
        case vr_input_0_13_1:
            ASSERT_NVALUES(1);
            M(input_0_13_1) = values[(*index)++];
            return OK;
        case vr_input_0_14_0:
            ASSERT_NVALUES(1);
            M(input_0_14_0) = values[(*index)++];
            return OK;
        case vr_input_0_14_1:
            ASSERT_NVALUES(1);
            M(input_0_14_1) = values[(*index)++];
            return OK;
        case vr_input_0_15_0:
            ASSERT_NVALUES(1);
            M(input_0_15_0) = values[(*index)++];
            return OK;
        case vr_input_0_15_1:
            ASSERT_NVALUES(1);
            M(input_0_15_1) = values[(*index)++];
            return OK;
        case vr_input_0_16_0:
            ASSERT_NVALUES(1);
            M(input_0_16_0) = values[(*index)++];
            return OK;
        case vr_input_0_16_1:
            ASSERT_NVALUES(1);
            M(input_0_16_1) = values[(*index)++];
            return OK;
        case vr_input_0_17_0:
            ASSERT_NVALUES(1);
            M(input_0_17_0) = values[(*index)++];
            return OK;
        case vr_input_0_17_1:
            ASSERT_NVALUES(1);
            M(input_0_17_1) = values[(*index)++];
            return OK;
        case vr_input_0_18_0:
            ASSERT_NVALUES(1);
            M(input_0_18_0) = values[(*index)++];
            return OK;
        case vr_input_0_18_1:
            ASSERT_NVALUES(1);
            M(input_0_18_1) = values[(*index)++];
            return OK;
        case vr_input_0_19_0:
            ASSERT_NVALUES(1);
            M(input_0_19_0) = values[(*index)++];
            return OK;
        case vr_input_0_19_1:
            ASSERT_NVALUES(1);
            M(input_0_19_1) = values[(*index)++];
            return OK;
        case vr_input_0_20_0:
            ASSERT_NVALUES(1);
            M(input_0_20_0) = values[(*index)++];
            return OK;
        case vr_input_0_20_1:
            ASSERT_NVALUES(1);
            M(input_0_20_1) = values[(*index)++];
            return OK;
        case vr_input_0_21_0:
            ASSERT_NVALUES(1);
            M(input_0_21_0) = values[(*index)++];
            return OK;
        case vr_input_0_21_1:
            ASSERT_NVALUES(1);
            M(input_0_21_1) = values[(*index)++];
            return OK;
        case vr_input_0_22_0:
            ASSERT_NVALUES(1);
            M(input_0_22_0) = values[(*index)++];
            return OK;
        case vr_input_0_22_1:
            ASSERT_NVALUES(1);
            M(input_0_22_1) = values[(*index)++];
            return OK;
        case vr_input_0_23_0:
            ASSERT_NVALUES(1);
            M(input_0_23_0) = values[(*index)++];
            return OK;
        case vr_input_0_23_1:
            ASSERT_NVALUES(1);
            M(input_0_23_1) = values[(*index)++];
            return OK;
        case vr_input_0_24_0:
            ASSERT_NVALUES(1);
            M(input_0_24_0) = values[(*index)++];
            return OK;
        case vr_input_0_24_1:
            ASSERT_NVALUES(1);
            M(input_0_24_1) = values[(*index)++];
            return OK;
        case vr_input_0_25_0:
            ASSERT_NVALUES(1);
            M(input_0_25_0) = values[(*index)++];
            return OK;
        case vr_input_0_25_1:
            ASSERT_NVALUES(1);
            M(input_0_25_1) = values[(*index)++];
            return OK;
        case vr_input_0_26_0:
            ASSERT_NVALUES(1);
            M(input_0_26_0) = values[(*index)++];
            return OK;
        case vr_input_0_26_1:
            ASSERT_NVALUES(1);
            M(input_0_26_1) = values[(*index)++];
            return OK;
        case vr_input_0_27_0:
            ASSERT_NVALUES(1);
            M(input_0_27_0) = values[(*index)++];
            return OK;
        case vr_input_0_27_1:
            ASSERT_NVALUES(1);
            M(input_0_27_1) = values[(*index)++];
            return OK;
        case vr_input_0_28_0:
            ASSERT_NVALUES(1);
            M(input_0_28_0) = values[(*index)++];
            return OK;
        case vr_input_0_28_1:
            ASSERT_NVALUES(1);
            M(input_0_28_1) = values[(*index)++];
            return OK;
        case vr_input_0_29_0:
            ASSERT_NVALUES(1);
            M(input_0_29_0) = values[(*index)++];
            return OK;
        case vr_input_0_29_1:
            ASSERT_NVALUES(1);
            M(input_0_29_1) = values[(*index)++];
            return OK;
        case vr_input_0_30_0:
            ASSERT_NVALUES(1);
            M(input_0_30_0) = values[(*index)++];
            return OK;
        case vr_input_0_30_1:
            ASSERT_NVALUES(1);
            M(input_0_30_1) = values[(*index)++];
            return OK;
        case vr_input_0_31_0:
            ASSERT_NVALUES(1);
            M(input_0_31_0) = values[(*index)++];
            return OK;
        case vr_input_0_31_1:
            ASSERT_NVALUES(1);
            M(input_0_31_1) = values[(*index)++];
            return OK;
        case vr_input_0_32_0:
            ASSERT_NVALUES(1);
            M(input_0_32_0) = values[(*index)++];
            return OK;
        case vr_input_0_32_1:
            ASSERT_NVALUES(1);
            M(input_0_32_1) = values[(*index)++];
            return OK;
        case vr_input_0_33_0:
            ASSERT_NVALUES(1);
            M(input_0_33_0) = values[(*index)++];
            return OK;
        case vr_input_0_33_1:
            ASSERT_NVALUES(1);
            M(input_0_33_1) = values[(*index)++];
            return OK;
        case vr_input_0_34_0:
            ASSERT_NVALUES(1);
            M(input_0_34_0) = values[(*index)++];
            return OK;
        case vr_input_0_34_1:
            ASSERT_NVALUES(1);
            M(input_0_34_1) = values[(*index)++];
            return OK;
        case vr_input_0_35_0:
            ASSERT_NVALUES(1);
            M(input_0_35_0) = values[(*index)++];
            return OK;
        case vr_input_0_35_1:
            ASSERT_NVALUES(1);
            M(input_0_35_1) = values[(*index)++];
            return OK;
        case vr_input_0_36_0:
            ASSERT_NVALUES(1);
            M(input_0_36_0) = values[(*index)++];
            return OK;
        case vr_input_0_36_1:
            ASSERT_NVALUES(1);
            M(input_0_36_1) = values[(*index)++];
            return OK;
        case vr_input_0_37_0:
            ASSERT_NVALUES(1);
            M(input_0_37_0) = values[(*index)++];
            return OK;
        case vr_input_0_37_1:
            ASSERT_NVALUES(1);
            M(input_0_37_1) = values[(*index)++];
            return OK;
        case vr_input_0_38_0:
            ASSERT_NVALUES(1);
            M(input_0_38_0) = values[(*index)++];
            return OK;
        case vr_input_0_38_1:
            ASSERT_NVALUES(1);
            M(input_0_38_1) = values[(*index)++];
            return OK;
        case vr_input_0_39_0:
            ASSERT_NVALUES(1);
            M(input_0_39_0) = values[(*index)++];
            return OK;
        case vr_input_0_39_1:
            ASSERT_NVALUES(1);
            M(input_0_39_1) = values[(*index)++];
            return OK;
        case vr_input_0_40_0:
            ASSERT_NVALUES(1);
            M(input_0_40_0) = values[(*index)++];
            return OK;
        case vr_input_0_40_1:
            ASSERT_NVALUES(1);
            M(input_0_40_1) = values[(*index)++];
            return OK;
        case vr_input_0_41_0:
            ASSERT_NVALUES(1);
            M(input_0_41_0) = values[(*index)++];
            return OK;
        case vr_input_0_41_1:
            ASSERT_NVALUES(1);
            M(input_0_41_1) = values[(*index)++];
            return OK;
        case vr_input_0_42_0:
            ASSERT_NVALUES(1);
            M(input_0_42_0) = values[(*index)++];
            return OK;
        case vr_input_0_42_1:
            ASSERT_NVALUES(1);
            M(input_0_42_1) = values[(*index)++];
            return OK;
        case vr_input_0_43_0:
            ASSERT_NVALUES(1);
            M(input_0_43_0) = values[(*index)++];
            return OK;
        case vr_input_0_43_1:
            ASSERT_NVALUES(1);
            M(input_0_43_1) = values[(*index)++];
            return OK;
        case vr_input_0_44_0:
            ASSERT_NVALUES(1);
            M(input_0_44_0) = values[(*index)++];
            return OK;
        case vr_input_0_44_1:
            ASSERT_NVALUES(1);
            M(input_0_44_1) = values[(*index)++];
            return OK;
        case vr_input_0_45_0:
            ASSERT_NVALUES(1);
            M(input_0_45_0) = values[(*index)++];
            return OK;
        case vr_input_0_45_1:
            ASSERT_NVALUES(1);
            M(input_0_45_1) = values[(*index)++];
            return OK;
        case vr_input_0_46_0:
            ASSERT_NVALUES(1);
            M(input_0_46_0) = values[(*index)++];
            return OK;
        case vr_input_0_46_1:
            ASSERT_NVALUES(1);
            M(input_0_46_1) = values[(*index)++];
            return OK;
        case vr_input_0_47_0:
            ASSERT_NVALUES(1);
            M(input_0_47_0) = values[(*index)++];
            return OK;
        case vr_input_0_47_1:
            ASSERT_NVALUES(1);
            M(input_0_47_1) = values[(*index)++];
            return OK;
        case vr_input_0_48_0:
            ASSERT_NVALUES(1);
            M(input_0_48_0) = values[(*index)++];
            return OK;
        case vr_input_0_48_1:
            ASSERT_NVALUES(1);
            M(input_0_48_1) = values[(*index)++];
            return OK;
        case vr_input_0_49_0:
            ASSERT_NVALUES(1);
            M(input_0_49_0) = values[(*index)++];
            return OK;
        case vr_input_0_49_1:
            ASSERT_NVALUES(1);
            M(input_0_49_1) = values[(*index)++];
            return OK;
        case vr_input_0_50_0:
            ASSERT_NVALUES(1);
            M(input_0_50_0) = values[(*index)++];
            return OK;
        case vr_input_0_50_1:
            ASSERT_NVALUES(1);
            M(input_0_50_1) = values[(*index)++];
            return OK;
        case vr_input_0_51_0:
            ASSERT_NVALUES(1);
            M(input_0_51_0) = values[(*index)++];
            return OK;
        case vr_input_0_51_1:
            ASSERT_NVALUES(1);
            M(input_0_51_1) = values[(*index)++];
            return OK;
        case vr_input_0_52_0:
            ASSERT_NVALUES(1);
            M(input_0_52_0) = values[(*index)++];
            return OK;
        case vr_input_0_52_1:
            ASSERT_NVALUES(1);
            M(input_0_52_1) = values[(*index)++];
            return OK;
        case vr_input_0_53_0:
            ASSERT_NVALUES(1);
            M(input_0_53_0) = values[(*index)++];
            return OK;
        case vr_input_0_53_1:
            ASSERT_NVALUES(1);
            M(input_0_53_1) = values[(*index)++];
            return OK;
        case vr_input_0_54_0:
            ASSERT_NVALUES(1);
            M(input_0_54_0) = values[(*index)++];
            return OK;
        case vr_input_0_54_1:
            ASSERT_NVALUES(1);
            M(input_0_54_1) = values[(*index)++];
            return OK;
        case vr_input_0_55_0:
            ASSERT_NVALUES(1);
            M(input_0_55_0) = values[(*index)++];
            return OK;
        case vr_input_0_55_1:
            ASSERT_NVALUES(1);
            M(input_0_55_1) = values[(*index)++];
            return OK;
        case vr_input_0_56_0:
            ASSERT_NVALUES(1);
            M(input_0_56_0) = values[(*index)++];
            return OK;
        case vr_input_0_56_1:
            ASSERT_NVALUES(1);
            M(input_0_56_1) = values[(*index)++];
            return OK;
        case vr_input_0_57_0:
            ASSERT_NVALUES(1);
            M(input_0_57_0) = values[(*index)++];
            return OK;
        case vr_input_0_57_1:
            ASSERT_NVALUES(1);
            M(input_0_57_1) = values[(*index)++];
            return OK;
        case vr_input_0_58_0:
            ASSERT_NVALUES(1);
            M(input_0_58_0) = values[(*index)++];
            return OK;
        case vr_input_0_58_1:
            ASSERT_NVALUES(1);
            M(input_0_58_1) = values[(*index)++];
            return OK;
        case vr_input_0_59_0:
            ASSERT_NVALUES(1);
            M(input_0_59_0) = values[(*index)++];
            return OK;
        case vr_input_0_59_1:
            ASSERT_NVALUES(1);
            M(input_0_59_1) = values[(*index)++];
            return OK;
        case vr_input_0_60_0:
            ASSERT_NVALUES(1);
            M(input_0_60_0) = values[(*index)++];
            return OK;
        case vr_input_0_60_1:
            ASSERT_NVALUES(1);
            M(input_0_60_1) = values[(*index)++];
            return OK;
        case vr_input_0_61_0:
            ASSERT_NVALUES(1);
            M(input_0_61_0) = values[(*index)++];
            return OK;
        case vr_input_0_61_1:
            ASSERT_NVALUES(1);
            M(input_0_61_1) = values[(*index)++];
            return OK;
        case vr_input_0_62_0:
            ASSERT_NVALUES(1);
            M(input_0_62_0) = values[(*index)++];
            return OK;
        case vr_input_0_62_1:
            ASSERT_NVALUES(1);
            M(input_0_62_1) = values[(*index)++];
            return OK;
        case vr_input_0_63_0:
            ASSERT_NVALUES(1);
            M(input_0_63_0) = values[(*index)++];
            return OK;
        case vr_input_0_63_1:
            ASSERT_NVALUES(1);
            M(input_0_63_1) = values[(*index)++];
            return OK;
        case vr_input_0_64_0:
            ASSERT_NVALUES(1);
            M(input_0_64_0) = values[(*index)++];
            return OK;
        case vr_input_0_64_1:
            ASSERT_NVALUES(1);
            M(input_0_64_1) = values[(*index)++];
            return OK;
        case vr_input_0_65_0:
            ASSERT_NVALUES(1);
            M(input_0_65_0) = values[(*index)++];
            return OK;
        case vr_input_0_65_1:
            ASSERT_NVALUES(1);
            M(input_0_65_1) = values[(*index)++];
            return OK;
        case vr_input_0_66_0:
            ASSERT_NVALUES(1);
            M(input_0_66_0) = values[(*index)++];
            return OK;
        case vr_input_0_66_1:
            ASSERT_NVALUES(1);
            M(input_0_66_1) = values[(*index)++];
            return OK;
        case vr_input_0_67_0:
            ASSERT_NVALUES(1);
            M(input_0_67_0) = values[(*index)++];
            return OK;
        case vr_input_0_67_1:
            ASSERT_NVALUES(1);
            M(input_0_67_1) = values[(*index)++];
            return OK;
        case vr_input_0_68_0:
            ASSERT_NVALUES(1);
            M(input_0_68_0) = values[(*index)++];
            return OK;
        case vr_input_0_68_1:
            ASSERT_NVALUES(1);
            M(input_0_68_1) = values[(*index)++];
            return OK;
        case vr_input_0_69_0:
            ASSERT_NVALUES(1);
            M(input_0_69_0) = values[(*index)++];
            return OK;
        case vr_input_0_69_1:
            ASSERT_NVALUES(1);
            M(input_0_69_1) = values[(*index)++];
            return OK;
        case vr_input_0_70_0:
            ASSERT_NVALUES(1);
            M(input_0_70_0) = values[(*index)++];
            return OK;
        case vr_input_0_70_1:
            ASSERT_NVALUES(1);
            M(input_0_70_1) = values[(*index)++];
            return OK;
        case vr_input_0_71_0:
            ASSERT_NVALUES(1);
            M(input_0_71_0) = values[(*index)++];
            return OK;
        case vr_input_0_71_1:
            ASSERT_NVALUES(1);
            M(input_0_71_1) = values[(*index)++];
            return OK;
        case vr_input_0_72_0:
            ASSERT_NVALUES(1);
            M(input_0_72_0) = values[(*index)++];
            return OK;
        case vr_input_0_72_1:
            ASSERT_NVALUES(1);
            M(input_0_72_1) = values[(*index)++];
            return OK;
        case vr_input_0_73_0:
            ASSERT_NVALUES(1);
            M(input_0_73_0) = values[(*index)++];
            return OK;
        case vr_input_0_73_1:
            ASSERT_NVALUES(1);
            M(input_0_73_1) = values[(*index)++];
            return OK;
        case vr_input_0_74_0:
            ASSERT_NVALUES(1);
            M(input_0_74_0) = values[(*index)++];
            return OK;
        case vr_input_0_74_1:
            ASSERT_NVALUES(1);
            M(input_0_74_1) = values[(*index)++];
            return OK;
        case vr_input_0_75_0:
            ASSERT_NVALUES(1);
            M(input_0_75_0) = values[(*index)++];
            return OK;
        case vr_input_0_75_1:
            ASSERT_NVALUES(1);
            M(input_0_75_1) = values[(*index)++];
            return OK;
        case vr_input_0_76_0:
            ASSERT_NVALUES(1);
            M(input_0_76_0) = values[(*index)++];
            return OK;
        case vr_input_0_76_1:
            ASSERT_NVALUES(1);
            M(input_0_76_1) = values[(*index)++];
            return OK;
        case vr_input_0_77_0:
            ASSERT_NVALUES(1);
            M(input_0_77_0) = values[(*index)++];
            return OK;
        case vr_input_0_77_1:
            ASSERT_NVALUES(1);
            M(input_0_77_1) = values[(*index)++];
            return OK;
        case vr_input_0_78_0:
            ASSERT_NVALUES(1);
            M(input_0_78_0) = values[(*index)++];
            return OK;
        case vr_input_0_78_1:
            ASSERT_NVALUES(1);
            M(input_0_78_1) = values[(*index)++];
            return OK;
        case vr_input_0_79_0:
            ASSERT_NVALUES(1);
            M(input_0_79_0) = values[(*index)++];
            return OK;
        case vr_input_0_79_1:
            ASSERT_NVALUES(1);
            M(input_0_79_1) = values[(*index)++];
            return OK;
        case vr_input_0_80_0:
            ASSERT_NVALUES(1);
            M(input_0_80_0) = values[(*index)++];
            return OK;
        case vr_input_0_80_1:
            ASSERT_NVALUES(1);
            M(input_0_80_1) = values[(*index)++];
            return OK;
        case vr_input_0_81_0:
            ASSERT_NVALUES(1);
            M(input_0_81_0) = values[(*index)++];
            return OK;
        case vr_input_0_81_1:
            ASSERT_NVALUES(1);
            M(input_0_81_1) = values[(*index)++];
            return OK;
        case vr_input_0_82_0:
            ASSERT_NVALUES(1);
            M(input_0_82_0) = values[(*index)++];
            return OK;
        case vr_input_0_82_1:
            ASSERT_NVALUES(1);
            M(input_0_82_1) = values[(*index)++];
            return OK;
        case vr_input_0_83_0:
            ASSERT_NVALUES(1);
            M(input_0_83_0) = values[(*index)++];
            return OK;
        case vr_input_0_83_1:
            ASSERT_NVALUES(1);
            M(input_0_83_1) = values[(*index)++];
            return OK;
        case vr_input_0_84_0:
            ASSERT_NVALUES(1);
            M(input_0_84_0) = values[(*index)++];
            return OK;
        case vr_input_0_84_1:
            ASSERT_NVALUES(1);
            M(input_0_84_1) = values[(*index)++];
            return OK;
        case vr_input_0_85_0:
            ASSERT_NVALUES(1);
            M(input_0_85_0) = values[(*index)++];
            return OK;
        case vr_input_0_85_1:
            ASSERT_NVALUES(1);
            M(input_0_85_1) = values[(*index)++];
            return OK;
        case vr_input_0_86_0:
            ASSERT_NVALUES(1);
            M(input_0_86_0) = values[(*index)++];
            return OK;
        case vr_input_0_86_1:
            ASSERT_NVALUES(1);
            M(input_0_86_1) = values[(*index)++];
            return OK;
        case vr_input_0_87_0:
            ASSERT_NVALUES(1);
            M(input_0_87_0) = values[(*index)++];
            return OK;
        case vr_input_0_87_1:
            ASSERT_NVALUES(1);
            M(input_0_87_1) = values[(*index)++];
            return OK;
        case vr_input_0_88_0:
            ASSERT_NVALUES(1);
            M(input_0_88_0) = values[(*index)++];
            return OK;
        case vr_input_0_88_1:
            ASSERT_NVALUES(1);
            M(input_0_88_1) = values[(*index)++];
            return OK;
        case vr_input_0_89_0:
            ASSERT_NVALUES(1);
            M(input_0_89_0) = values[(*index)++];
            return OK;
        case vr_input_0_89_1:
            ASSERT_NVALUES(1);
            M(input_0_89_1) = values[(*index)++];
            return OK;
        case vr_input_0_90_0:
            ASSERT_NVALUES(1);
            M(input_0_90_0) = values[(*index)++];
            return OK;
        case vr_input_0_90_1:
            ASSERT_NVALUES(1);
            M(input_0_90_1) = values[(*index)++];
            return OK;
        case vr_input_0_91_0:
            ASSERT_NVALUES(1);
            M(input_0_91_0) = values[(*index)++];
            return OK;
        case vr_input_0_91_1:
            ASSERT_NVALUES(1);
            M(input_0_91_1) = values[(*index)++];
            return OK;
        case vr_input_0_92_0:
            ASSERT_NVALUES(1);
            M(input_0_92_0) = values[(*index)++];
            return OK;
        case vr_input_0_92_1:
            ASSERT_NVALUES(1);
            M(input_0_92_1) = values[(*index)++];
            return OK;
        case vr_input_0_93_0:
            ASSERT_NVALUES(1);
            M(input_0_93_0) = values[(*index)++];
            return OK;
        case vr_input_0_93_1:
            ASSERT_NVALUES(1);
            M(input_0_93_1) = values[(*index)++];
            return OK;
        case vr_input_0_94_0:
            ASSERT_NVALUES(1);
            M(input_0_94_0) = values[(*index)++];
            return OK;
        case vr_input_0_94_1:
            ASSERT_NVALUES(1);
            M(input_0_94_1) = values[(*index)++];
            return OK;
        case vr_input_0_95_0:
            ASSERT_NVALUES(1);
            M(input_0_95_0) = values[(*index)++];
            return OK;
        case vr_input_0_95_1:
            ASSERT_NVALUES(1);
            M(input_0_95_1) = values[(*index)++];
            return OK;
        case vr_input_0_96_0:
            ASSERT_NVALUES(1);
            M(input_0_96_0) = values[(*index)++];
            return OK;
        case vr_input_0_96_1:
            ASSERT_NVALUES(1);
            M(input_0_96_1) = values[(*index)++];
            return OK;
        case vr_input_0_97_0:
            ASSERT_NVALUES(1);
            M(input_0_97_0) = values[(*index)++];
            return OK;
        case vr_input_0_97_1:
            ASSERT_NVALUES(1);
            M(input_0_97_1) = values[(*index)++];
            return OK;
        case vr_input_0_98_0:
            ASSERT_NVALUES(1);
            M(input_0_98_0) = values[(*index)++];
            return OK;
        case vr_input_0_98_1:
            ASSERT_NVALUES(1);
            M(input_0_98_1) = values[(*index)++];
            return OK;
        case vr_input_0_99_0:
            ASSERT_NVALUES(1);
            M(input_0_99_0) = values[(*index)++];
            return OK;
        case vr_input_0_99_1:
            ASSERT_NVALUES(1);
            M(input_0_99_1) = values[(*index)++];
            return OK;
        case vr_input_0_100_0:
            ASSERT_NVALUES(1);
            M(input_0_100_0) = values[(*index)++];
            return OK;
        case vr_input_0_100_1:
            ASSERT_NVALUES(1);
            M(input_0_100_1) = values[(*index)++];
            return OK;
        case vr_input_0_101_0:
            ASSERT_NVALUES(1);
            M(input_0_101_0) = values[(*index)++];
            return OK;
        case vr_input_0_101_1:
            ASSERT_NVALUES(1);
            M(input_0_101_1) = values[(*index)++];
            return OK;
        case vr_input_0_102_0:
            ASSERT_NVALUES(1);
            M(input_0_102_0) = values[(*index)++];
            return OK;
        case vr_input_0_102_1:
            ASSERT_NVALUES(1);
            M(input_0_102_1) = values[(*index)++];
            return OK;
        case vr_input_0_103_0:
            ASSERT_NVALUES(1);
            M(input_0_103_0) = values[(*index)++];
            return OK;
        case vr_input_0_103_1:
            ASSERT_NVALUES(1);
            M(input_0_103_1) = values[(*index)++];
            return OK;
        case vr_input_0_104_0:
            ASSERT_NVALUES(1);
            M(input_0_104_0) = values[(*index)++];
            return OK;
        case vr_input_0_104_1:
            ASSERT_NVALUES(1);
            M(input_0_104_1) = values[(*index)++];
            return OK;
        case vr_input_0_105_0:
            ASSERT_NVALUES(1);
            M(input_0_105_0) = values[(*index)++];
            return OK;
        case vr_input_0_105_1:
            ASSERT_NVALUES(1);
            M(input_0_105_1) = values[(*index)++];
            return OK;
        case vr_input_0_106_0:
            ASSERT_NVALUES(1);
            M(input_0_106_0) = values[(*index)++];
            return OK;
        case vr_input_0_106_1:
            ASSERT_NVALUES(1);
            M(input_0_106_1) = values[(*index)++];
            return OK;
        case vr_input_0_107_0:
            ASSERT_NVALUES(1);
            M(input_0_107_0) = values[(*index)++];
            return OK;
        case vr_input_0_107_1:
            ASSERT_NVALUES(1);
            M(input_0_107_1) = values[(*index)++];
            return OK;
        case vr_input_0_108_0:
            ASSERT_NVALUES(1);
            M(input_0_108_0) = values[(*index)++];
            return OK;
        case vr_input_0_108_1:
            ASSERT_NVALUES(1);
            M(input_0_108_1) = values[(*index)++];
            return OK;
        case vr_input_0_109_0:
            ASSERT_NVALUES(1);
            M(input_0_109_0) = values[(*index)++];
            return OK;
        case vr_input_0_109_1:
            ASSERT_NVALUES(1);
            M(input_0_109_1) = values[(*index)++];
            return OK;
        case vr_input_0_110_0:
            ASSERT_NVALUES(1);
            M(input_0_110_0) = values[(*index)++];
            return OK;
        case vr_input_0_110_1:
            ASSERT_NVALUES(1);
            M(input_0_110_1) = values[(*index)++];
            return OK;
        case vr_input_0_111_0:
            ASSERT_NVALUES(1);
            M(input_0_111_0) = values[(*index)++];
            return OK;
        case vr_input_0_111_1:
            ASSERT_NVALUES(1);
            M(input_0_111_1) = values[(*index)++];
            return OK;
        case vr_input_0_112_0:
            ASSERT_NVALUES(1);
            M(input_0_112_0) = values[(*index)++];
            return OK;
        case vr_input_0_112_1:
            ASSERT_NVALUES(1);
            M(input_0_112_1) = values[(*index)++];
            return OK;
        case vr_input_0_113_0:
            ASSERT_NVALUES(1);
            M(input_0_113_0) = values[(*index)++];
            return OK;
        case vr_input_0_113_1:
            ASSERT_NVALUES(1);
            M(input_0_113_1) = values[(*index)++];
            return OK;
        case vr_input_0_114_0:
            ASSERT_NVALUES(1);
            M(input_0_114_0) = values[(*index)++];
            return OK;
        case vr_input_0_114_1:
            ASSERT_NVALUES(1);
            M(input_0_114_1) = values[(*index)++];
            return OK;
        case vr_input_0_115_0:
            ASSERT_NVALUES(1);
            M(input_0_115_0) = values[(*index)++];
            return OK;
        case vr_input_0_115_1:
            ASSERT_NVALUES(1);
            M(input_0_115_1) = values[(*index)++];
            return OK;
        case vr_input_0_116_0:
            ASSERT_NVALUES(1);
            M(input_0_116_0) = values[(*index)++];
            return OK;
        case vr_input_0_116_1:
            ASSERT_NVALUES(1);
            M(input_0_116_1) = values[(*index)++];
            return OK;
        case vr_input_0_117_0:
            ASSERT_NVALUES(1);
            M(input_0_117_0) = values[(*index)++];
            return OK;
        case vr_input_0_117_1:
            ASSERT_NVALUES(1);
            M(input_0_117_1) = values[(*index)++];
            return OK;
        case vr_input_0_118_0:
            ASSERT_NVALUES(1);
            M(input_0_118_0) = values[(*index)++];
            return OK;
        case vr_input_0_118_1:
            ASSERT_NVALUES(1);
            M(input_0_118_1) = values[(*index)++];
            return OK;
        case vr_input_0_119_0:
            ASSERT_NVALUES(1);
            M(input_0_119_0) = values[(*index)++];
            return OK;
        case vr_input_0_119_1:
            ASSERT_NVALUES(1);
            M(input_0_119_1) = values[(*index)++];
            return OK;
        case vr_input_0_120_0:
            ASSERT_NVALUES(1);
            M(input_0_120_0) = values[(*index)++];
            return OK;
        case vr_input_0_120_1:
            ASSERT_NVALUES(1);
            M(input_0_120_1) = values[(*index)++];
            return OK;
        case vr_input_0_121_0:
            ASSERT_NVALUES(1);
            M(input_0_121_0) = values[(*index)++];
            return OK;
        case vr_input_0_121_1:
            ASSERT_NVALUES(1);
            M(input_0_121_1) = values[(*index)++];
            return OK;
        case vr_input_0_122_0:
            ASSERT_NVALUES(1);
            M(input_0_122_0) = values[(*index)++];
            return OK;
        case vr_input_0_122_1:
            ASSERT_NVALUES(1);
            M(input_0_122_1) = values[(*index)++];
            return OK;
        case vr_input_0_123_0:
            ASSERT_NVALUES(1);
            M(input_0_123_0) = values[(*index)++];
            return OK;
        case vr_input_0_123_1:
            ASSERT_NVALUES(1);
            M(input_0_123_1) = values[(*index)++];
            return OK;
        case vr_input_0_124_0:
            ASSERT_NVALUES(1);
            M(input_0_124_0) = values[(*index)++];
            return OK;
        case vr_input_0_124_1:
            ASSERT_NVALUES(1);
            M(input_0_124_1) = values[(*index)++];
            return OK;
        case vr_input_0_125_0:
            ASSERT_NVALUES(1);
            M(input_0_125_0) = values[(*index)++];
            return OK;
        case vr_input_0_125_1:
            ASSERT_NVALUES(1);
            M(input_0_125_1) = values[(*index)++];
            return OK;
        case vr_input_0_126_0:
            ASSERT_NVALUES(1);
            M(input_0_126_0) = values[(*index)++];
            return OK;
        case vr_input_0_126_1:
            ASSERT_NVALUES(1);
            M(input_0_126_1) = values[(*index)++];
            return OK;
        case vr_input_0_127_0:
            ASSERT_NVALUES(1);
            M(input_0_127_0) = values[(*index)++];
            return OK;
        case vr_input_0_127_1:
            ASSERT_NVALUES(1);
            M(input_0_127_1) = values[(*index)++];
            return OK;
        case vr_input_0_128_0:
            ASSERT_NVALUES(1);
            M(input_0_128_0) = values[(*index)++];
            return OK;
        case vr_input_0_128_1:
            ASSERT_NVALUES(1);
            M(input_0_128_1) = values[(*index)++];
            return OK;
        case vr_input_0_129_0:
            ASSERT_NVALUES(1);
            M(input_0_129_0) = values[(*index)++];
            return OK;
        case vr_input_0_129_1:
            ASSERT_NVALUES(1);
            M(input_0_129_1) = values[(*index)++];
            return OK;
        case vr_input_0_130_0:
            ASSERT_NVALUES(1);
            M(input_0_130_0) = values[(*index)++];
            return OK;
        case vr_input_0_130_1:
            ASSERT_NVALUES(1);
            M(input_0_130_1) = values[(*index)++];
            return OK;
        case vr_input_0_131_0:
            ASSERT_NVALUES(1);
            M(input_0_131_0) = values[(*index)++];
            return OK;
        case vr_input_0_131_1:
            ASSERT_NVALUES(1);
            M(input_0_131_1) = values[(*index)++];
            return OK;
        case vr_input_0_132_0:
            ASSERT_NVALUES(1);
            M(input_0_132_0) = values[(*index)++];
            return OK;
        case vr_input_0_132_1:
            ASSERT_NVALUES(1);
            M(input_0_132_1) = values[(*index)++];
            return OK;
        case vr_input_0_133_0:
            ASSERT_NVALUES(1);
            M(input_0_133_0) = values[(*index)++];
            return OK;
        case vr_input_0_133_1:
            ASSERT_NVALUES(1);
            M(input_0_133_1) = values[(*index)++];
            return OK;
        case vr_input_0_134_0:
            ASSERT_NVALUES(1);
            M(input_0_134_0) = values[(*index)++];
            return OK;
        case vr_input_0_134_1:
            ASSERT_NVALUES(1);
            M(input_0_134_1) = values[(*index)++];
            return OK;
        case vr_input_0_135_0:
            ASSERT_NVALUES(1);
            M(input_0_135_0) = values[(*index)++];
            return OK;
        case vr_input_0_135_1:
            ASSERT_NVALUES(1);
            M(input_0_135_1) = values[(*index)++];
            return OK;
        case vr_input_0_136_0:
            ASSERT_NVALUES(1);
            M(input_0_136_0) = values[(*index)++];
            return OK;
        case vr_input_0_136_1:
            ASSERT_NVALUES(1);
            M(input_0_136_1) = values[(*index)++];
            return OK;
        case vr_input_0_137_0:
            ASSERT_NVALUES(1);
            M(input_0_137_0) = values[(*index)++];
            return OK;
        case vr_input_0_137_1:
            ASSERT_NVALUES(1);
            M(input_0_137_1) = values[(*index)++];
            return OK;
        case vr_input_0_138_0:
            ASSERT_NVALUES(1);
            M(input_0_138_0) = values[(*index)++];
            return OK;
        case vr_input_0_138_1:
            ASSERT_NVALUES(1);
            M(input_0_138_1) = values[(*index)++];
            return OK;
        case vr_input_0_139_0:
            ASSERT_NVALUES(1);
            M(input_0_139_0) = values[(*index)++];
            return OK;
        case vr_input_0_139_1:
            ASSERT_NVALUES(1);
            M(input_0_139_1) = values[(*index)++];
            return OK;
        case vr_input_0_140_0:
            ASSERT_NVALUES(1);
            M(input_0_140_0) = values[(*index)++];
            return OK;
        case vr_input_0_140_1:
            ASSERT_NVALUES(1);
            M(input_0_140_1) = values[(*index)++];
            return OK;
        case vr_input_0_141_0:
            ASSERT_NVALUES(1);
            M(input_0_141_0) = values[(*index)++];
            return OK;
        case vr_input_0_141_1:
            ASSERT_NVALUES(1);
            M(input_0_141_1) = values[(*index)++];
            return OK;
        case vr_input_0_142_0:
            ASSERT_NVALUES(1);
            M(input_0_142_0) = values[(*index)++];
            return OK;
        case vr_input_0_142_1:
            ASSERT_NVALUES(1);
            M(input_0_142_1) = values[(*index)++];
            return OK;
        case vr_input_0_143_0:
            ASSERT_NVALUES(1);
            M(input_0_143_0) = values[(*index)++];
            return OK;
        case vr_input_0_143_1:
            ASSERT_NVALUES(1);
            M(input_0_143_1) = values[(*index)++];
            return OK;
        case vr_input_0_144_0:
            ASSERT_NVALUES(1);
            M(input_0_144_0) = values[(*index)++];
            return OK;
        case vr_input_0_144_1:
            ASSERT_NVALUES(1);
            M(input_0_144_1) = values[(*index)++];
            return OK;
        case vr_input_0_145_0:
            ASSERT_NVALUES(1);
            M(input_0_145_0) = values[(*index)++];
            return OK;
        case vr_input_0_145_1:
            ASSERT_NVALUES(1);
            M(input_0_145_1) = values[(*index)++];
            return OK;
        case vr_input_0_146_0:
            ASSERT_NVALUES(1);
            M(input_0_146_0) = values[(*index)++];
            return OK;
        case vr_input_0_146_1:
            ASSERT_NVALUES(1);
            M(input_0_146_1) = values[(*index)++];
            return OK;
        case vr_input_0_147_0:
            ASSERT_NVALUES(1);
            M(input_0_147_0) = values[(*index)++];
            return OK;
        case vr_input_0_147_1:
            ASSERT_NVALUES(1);
            M(input_0_147_1) = values[(*index)++];
            return OK;
        case vr_input_0_148_0:
            ASSERT_NVALUES(1);
            M(input_0_148_0) = values[(*index)++];
            return OK;
        case vr_input_0_148_1:
            ASSERT_NVALUES(1);
            M(input_0_148_1) = values[(*index)++];
            return OK;
        case vr_input_0_149_0:
            ASSERT_NVALUES(1);
            M(input_0_149_0) = values[(*index)++];
            return OK;
        case vr_input_0_149_1:
            ASSERT_NVALUES(1);
            M(input_0_149_1) = values[(*index)++];
            return OK;
        case vr_input_0_150_0:
            ASSERT_NVALUES(1);
            M(input_0_150_0) = values[(*index)++];
            return OK;
        case vr_input_0_150_1:
            ASSERT_NVALUES(1);
            M(input_0_150_1) = values[(*index)++];
            return OK;
        case vr_input_0_151_0:
            ASSERT_NVALUES(1);
            M(input_0_151_0) = values[(*index)++];
            return OK;
        case vr_input_0_151_1:
            ASSERT_NVALUES(1);
            M(input_0_151_1) = values[(*index)++];
            return OK;
        case vr_input_0_152_0:
            ASSERT_NVALUES(1);
            M(input_0_152_0) = values[(*index)++];
            return OK;
        case vr_input_0_152_1:
            ASSERT_NVALUES(1);
            M(input_0_152_1) = values[(*index)++];
            return OK;
        case vr_input_0_153_0:
            ASSERT_NVALUES(1);
            M(input_0_153_0) = values[(*index)++];
            return OK;
        case vr_input_0_153_1:
            ASSERT_NVALUES(1);
            M(input_0_153_1) = values[(*index)++];
            return OK;
        case vr_input_0_154_0:
            ASSERT_NVALUES(1);
            M(input_0_154_0) = values[(*index)++];
            return OK;
        case vr_input_0_154_1:
            ASSERT_NVALUES(1);
            M(input_0_154_1) = values[(*index)++];
            return OK;
        case vr_input_0_155_0:
            ASSERT_NVALUES(1);
            M(input_0_155_0) = values[(*index)++];
            return OK;
        case vr_input_0_155_1:
            ASSERT_NVALUES(1);
            M(input_0_155_1) = values[(*index)++];
            return OK;
        case vr_input_0_156_0:
            ASSERT_NVALUES(1);
            M(input_0_156_0) = values[(*index)++];
            return OK;
        case vr_input_0_156_1:
            ASSERT_NVALUES(1);
            M(input_0_156_1) = values[(*index)++];
            return OK;
        case vr_input_0_157_0:
            ASSERT_NVALUES(1);
            M(input_0_157_0) = values[(*index)++];
            return OK;
        case vr_input_0_157_1:
            ASSERT_NVALUES(1);
            M(input_0_157_1) = values[(*index)++];
            return OK;
        case vr_input_0_158_0:
            ASSERT_NVALUES(1);
            M(input_0_158_0) = values[(*index)++];
            return OK;
        case vr_input_0_158_1:
            ASSERT_NVALUES(1);
            M(input_0_158_1) = values[(*index)++];
            return OK;
        case vr_input_0_159_0:
            ASSERT_NVALUES(1);
            M(input_0_159_0) = values[(*index)++];
            return OK;
        case vr_input_0_159_1:
            ASSERT_NVALUES(1);
            M(input_0_159_1) = values[(*index)++];
            return OK;
        case vr_input_0_160_0:
            ASSERT_NVALUES(1);
            M(input_0_160_0) = values[(*index)++];
            return OK;
        case vr_input_0_160_1:
            ASSERT_NVALUES(1);
            M(input_0_160_1) = values[(*index)++];
            return OK;
        case vr_input_0_161_0:
            ASSERT_NVALUES(1);
            M(input_0_161_0) = values[(*index)++];
            return OK;
        case vr_input_0_161_1:
            ASSERT_NVALUES(1);
            M(input_0_161_1) = values[(*index)++];
            return OK;
        case vr_input_0_162_0:
            ASSERT_NVALUES(1);
            M(input_0_162_0) = values[(*index)++];
            return OK;
        case vr_input_0_162_1:
            ASSERT_NVALUES(1);
            M(input_0_162_1) = values[(*index)++];
            return OK;
        case vr_input_0_163_0:
            ASSERT_NVALUES(1);
            M(input_0_163_0) = values[(*index)++];
            return OK;
        case vr_input_0_163_1:
            ASSERT_NVALUES(1);
            M(input_0_163_1) = values[(*index)++];
            return OK;
        case vr_input_0_164_0:
            ASSERT_NVALUES(1);
            M(input_0_164_0) = values[(*index)++];
            return OK;
        case vr_input_0_164_1:
            ASSERT_NVALUES(1);
            M(input_0_164_1) = values[(*index)++];
            return OK;
        case vr_input_0_165_0:
            ASSERT_NVALUES(1);
            M(input_0_165_0) = values[(*index)++];
            return OK;
        case vr_input_0_165_1:
            ASSERT_NVALUES(1);
            M(input_0_165_1) = values[(*index)++];
            return OK;
        case vr_input_0_166_0:
            ASSERT_NVALUES(1);
            M(input_0_166_0) = values[(*index)++];
            return OK;
        case vr_input_0_166_1:
            ASSERT_NVALUES(1);
            M(input_0_166_1) = values[(*index)++];
            return OK;
        case vr_input_0_167_0:
            ASSERT_NVALUES(1);
            M(input_0_167_0) = values[(*index)++];
            return OK;
        case vr_input_0_167_1:
            ASSERT_NVALUES(1);
            M(input_0_167_1) = values[(*index)++];
            return OK;
        case vr_input_0_168_0:
            ASSERT_NVALUES(1);
            M(input_0_168_0) = values[(*index)++];
            return OK;
        case vr_input_0_168_1:
            ASSERT_NVALUES(1);
            M(input_0_168_1) = values[(*index)++];
            return OK;
        case vr_input_0_169_0:
            ASSERT_NVALUES(1);
            M(input_0_169_0) = values[(*index)++];
            return OK;
        case vr_input_0_169_1:
            ASSERT_NVALUES(1);
            M(input_0_169_1) = values[(*index)++];
            return OK;
        case vr_input_0_170_0:
            ASSERT_NVALUES(1);
            M(input_0_170_0) = values[(*index)++];
            return OK;
        case vr_input_0_170_1:
            ASSERT_NVALUES(1);
            M(input_0_170_1) = values[(*index)++];
            return OK;
        case vr_input_0_171_0:
            ASSERT_NVALUES(1);
            M(input_0_171_0) = values[(*index)++];
            return OK;
        case vr_input_0_171_1:
            ASSERT_NVALUES(1);
            M(input_0_171_1) = values[(*index)++];
            return OK;
        case vr_input_0_172_0:
            ASSERT_NVALUES(1);
            M(input_0_172_0) = values[(*index)++];
            return OK;
        case vr_input_0_172_1:
            ASSERT_NVALUES(1);
            M(input_0_172_1) = values[(*index)++];
            return OK;
        case vr_input_0_173_0:
            ASSERT_NVALUES(1);
            M(input_0_173_0) = values[(*index)++];
            return OK;
        case vr_input_0_173_1:
            ASSERT_NVALUES(1);
            M(input_0_173_1) = values[(*index)++];
            return OK;
        case vr_input_0_174_0:
            ASSERT_NVALUES(1);
            M(input_0_174_0) = values[(*index)++];
            return OK;
        case vr_input_0_174_1:
            ASSERT_NVALUES(1);
            M(input_0_174_1) = values[(*index)++];
            return OK;
        case vr_input_0_175_0:
            ASSERT_NVALUES(1);
            M(input_0_175_0) = values[(*index)++];
            return OK;
        case vr_input_0_175_1:
            ASSERT_NVALUES(1);
            M(input_0_175_1) = values[(*index)++];
            return OK;
        case vr_input_0_176_0:
            ASSERT_NVALUES(1);
            M(input_0_176_0) = values[(*index)++];
            return OK;
        case vr_input_0_176_1:
            ASSERT_NVALUES(1);
            M(input_0_176_1) = values[(*index)++];
            return OK;
        case vr_input_0_177_0:
            ASSERT_NVALUES(1);
            M(input_0_177_0) = values[(*index)++];
            return OK;
        case vr_input_0_177_1:
            ASSERT_NVALUES(1);
            M(input_0_177_1) = values[(*index)++];
            return OK;
        case vr_input_0_178_0:
            ASSERT_NVALUES(1);
            M(input_0_178_0) = values[(*index)++];
            return OK;
        case vr_input_0_178_1:
            ASSERT_NVALUES(1);
            M(input_0_178_1) = values[(*index)++];
            return OK;
        case vr_input_0_179_0:
            ASSERT_NVALUES(1);
            M(input_0_179_0) = values[(*index)++];
            return OK;
        case vr_input_0_179_1:
            ASSERT_NVALUES(1);
            M(input_0_179_1) = values[(*index)++];
            return OK;
        case vr_input_0_180_0:
            ASSERT_NVALUES(1);
            M(input_0_180_0) = values[(*index)++];
            return OK;
        case vr_input_0_180_1:
            ASSERT_NVALUES(1);
            M(input_0_180_1) = values[(*index)++];
            return OK;
        case vr_input_0_181_0:
            ASSERT_NVALUES(1);
            M(input_0_181_0) = values[(*index)++];
            return OK;
        case vr_input_0_181_1:
            ASSERT_NVALUES(1);
            M(input_0_181_1) = values[(*index)++];
            return OK;
        case vr_input_0_182_0:
            ASSERT_NVALUES(1);
            M(input_0_182_0) = values[(*index)++];
            return OK;
        case vr_input_0_182_1:
            ASSERT_NVALUES(1);
            M(input_0_182_1) = values[(*index)++];
            return OK;
        case vr_input_0_183_0:
            ASSERT_NVALUES(1);
            M(input_0_183_0) = values[(*index)++];
            return OK;
        case vr_input_0_183_1:
            ASSERT_NVALUES(1);
            M(input_0_183_1) = values[(*index)++];
            return OK;
        case vr_input_0_184_0:
            ASSERT_NVALUES(1);
            M(input_0_184_0) = values[(*index)++];
            return OK;
        case vr_input_0_184_1:
            ASSERT_NVALUES(1);
            M(input_0_184_1) = values[(*index)++];
            return OK;
        case vr_input_0_185_0:
            ASSERT_NVALUES(1);
            M(input_0_185_0) = values[(*index)++];
            return OK;
        case vr_input_0_185_1:
            ASSERT_NVALUES(1);
            M(input_0_185_1) = values[(*index)++];
            return OK;
        case vr_input_0_186_0:
            ASSERT_NVALUES(1);
            M(input_0_186_0) = values[(*index)++];
            return OK;
        case vr_input_0_186_1:
            ASSERT_NVALUES(1);
            M(input_0_186_1) = values[(*index)++];
            return OK;
        case vr_input_0_187_0:
            ASSERT_NVALUES(1);
            M(input_0_187_0) = values[(*index)++];
            return OK;
        case vr_input_0_187_1:
            ASSERT_NVALUES(1);
            M(input_0_187_1) = values[(*index)++];
            return OK;
        case vr_input_0_188_0:
            ASSERT_NVALUES(1);
            M(input_0_188_0) = values[(*index)++];
            return OK;
        case vr_input_0_188_1:
            ASSERT_NVALUES(1);
            M(input_0_188_1) = values[(*index)++];
            return OK;
        case vr_input_0_189_0:
            ASSERT_NVALUES(1);
            M(input_0_189_0) = values[(*index)++];
            return OK;
        case vr_input_0_189_1:
            ASSERT_NVALUES(1);
            M(input_0_189_1) = values[(*index)++];
            return OK;
        case vr_input_0_190_0:
            ASSERT_NVALUES(1);
            M(input_0_190_0) = values[(*index)++];
            return OK;
        case vr_input_0_190_1:
            ASSERT_NVALUES(1);
            M(input_0_190_1) = values[(*index)++];
            return OK;
        case vr_input_0_191_0:
            ASSERT_NVALUES(1);
            M(input_0_191_0) = values[(*index)++];
            return OK;
        case vr_input_0_191_1:
            ASSERT_NVALUES(1);
            M(input_0_191_1) = values[(*index)++];
            return OK;
        case vr_input_0_192_0:
            ASSERT_NVALUES(1);
            M(input_0_192_0) = values[(*index)++];
            return OK;
        case vr_input_0_192_1:
            ASSERT_NVALUES(1);
            M(input_0_192_1) = values[(*index)++];
            return OK;
        case vr_input_0_193_0:
            ASSERT_NVALUES(1);
            M(input_0_193_0) = values[(*index)++];
            return OK;
        case vr_input_0_193_1:
            ASSERT_NVALUES(1);
            M(input_0_193_1) = values[(*index)++];
            return OK;
        case vr_input_0_194_0:
            ASSERT_NVALUES(1);
            M(input_0_194_0) = values[(*index)++];
            return OK;
        case vr_input_0_194_1:
            ASSERT_NVALUES(1);
            M(input_0_194_1) = values[(*index)++];
            return OK;
        case vr_input_0_195_0:
            ASSERT_NVALUES(1);
            M(input_0_195_0) = values[(*index)++];
            return OK;
        case vr_input_0_195_1:
            ASSERT_NVALUES(1);
            M(input_0_195_1) = values[(*index)++];
            return OK;
        case vr_input_0_196_0:
            ASSERT_NVALUES(1);
            M(input_0_196_0) = values[(*index)++];
            return OK;
        case vr_input_0_196_1:
            ASSERT_NVALUES(1);
            M(input_0_196_1) = values[(*index)++];
            return OK;
        case vr_input_0_197_0:
            ASSERT_NVALUES(1);
            M(input_0_197_0) = values[(*index)++];
            return OK;
        case vr_input_0_197_1:
            ASSERT_NVALUES(1);
            M(input_0_197_1) = values[(*index)++];
            return OK;
        case vr_input_0_198_0:
            ASSERT_NVALUES(1);
            M(input_0_198_0) = values[(*index)++];
            return OK;
        case vr_input_0_198_1:
            ASSERT_NVALUES(1);
            M(input_0_198_1) = values[(*index)++];
            return OK;
        case vr_input_0_199_0:
            ASSERT_NVALUES(1);
            M(input_0_199_0) = values[(*index)++];
            return OK;
        case vr_input_0_199_1:
            ASSERT_NVALUES(1);
            M(input_0_199_1) = values[(*index)++];
            return OK;
        // Locals
#endif
    default:
        // Compose message for log with value reference
        logError(comp, "setFloat64: ValueReference %d not available.", vr);
        return Error;
    }
}

#if FMI_VERSION > 2
Status getFloat32(ModelInstance *comp, ValueReference vr, float values[],
                  size_t nValues, size_t *index) {

    switch (vr)
    {
        // Inputs
        case vr_input_0_0_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_0_0);
            return OK;
        case vr_input_0_0_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_0_1);
            return OK;
        case vr_input_0_1_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_1_0);
            return OK;
        case vr_input_0_1_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_1_1);
            return OK;
        case vr_input_0_2_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_2_0);
            return OK;
        case vr_input_0_2_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_2_1);
            return OK;
        case vr_input_0_3_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_3_0);
            return OK;
        case vr_input_0_3_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_3_1);
            return OK;
        case vr_input_0_4_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_4_0);
            return OK;
        case vr_input_0_4_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_4_1);
            return OK;
        case vr_input_0_5_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_5_0);
            return OK;
        case vr_input_0_5_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_5_1);
            return OK;
        case vr_input_0_6_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_6_0);
            return OK;
        case vr_input_0_6_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_6_1);
            return OK;
        case vr_input_0_7_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_7_0);
            return OK;
        case vr_input_0_7_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_7_1);
            return OK;
        case vr_input_0_8_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_8_0);
            return OK;
        case vr_input_0_8_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_8_1);
            return OK;
        case vr_input_0_9_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_9_0);
            return OK;
        case vr_input_0_9_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_9_1);
            return OK;
        case vr_input_0_10_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_10_0);
            return OK;
        case vr_input_0_10_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_10_1);
            return OK;
        case vr_input_0_11_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_11_0);
            return OK;
        case vr_input_0_11_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_11_1);
            return OK;
        case vr_input_0_12_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_12_0);
            return OK;
        case vr_input_0_12_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_12_1);
            return OK;
        case vr_input_0_13_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_13_0);
            return OK;
        case vr_input_0_13_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_13_1);
            return OK;
        case vr_input_0_14_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_14_0);
            return OK;
        case vr_input_0_14_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_14_1);
            return OK;
        case vr_input_0_15_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_15_0);
            return OK;
        case vr_input_0_15_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_15_1);
            return OK;
        case vr_input_0_16_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_16_0);
            return OK;
        case vr_input_0_16_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_16_1);
            return OK;
        case vr_input_0_17_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_17_0);
            return OK;
        case vr_input_0_17_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_17_1);
            return OK;
        case vr_input_0_18_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_18_0);
            return OK;
        case vr_input_0_18_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_18_1);
            return OK;
        case vr_input_0_19_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_19_0);
            return OK;
        case vr_input_0_19_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_19_1);
            return OK;
        case vr_input_0_20_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_20_0);
            return OK;
        case vr_input_0_20_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_20_1);
            return OK;
        case vr_input_0_21_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_21_0);
            return OK;
        case vr_input_0_21_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_21_1);
            return OK;
        case vr_input_0_22_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_22_0);
            return OK;
        case vr_input_0_22_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_22_1);
            return OK;
        case vr_input_0_23_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_23_0);
            return OK;
        case vr_input_0_23_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_23_1);
            return OK;
        case vr_input_0_24_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_24_0);
            return OK;
        case vr_input_0_24_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_24_1);
            return OK;
        case vr_input_0_25_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_25_0);
            return OK;
        case vr_input_0_25_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_25_1);
            return OK;
        case vr_input_0_26_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_26_0);
            return OK;
        case vr_input_0_26_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_26_1);
            return OK;
        case vr_input_0_27_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_27_0);
            return OK;
        case vr_input_0_27_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_27_1);
            return OK;
        case vr_input_0_28_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_28_0);
            return OK;
        case vr_input_0_28_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_28_1);
            return OK;
        case vr_input_0_29_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_29_0);
            return OK;
        case vr_input_0_29_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_29_1);
            return OK;
        case vr_input_0_30_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_30_0);
            return OK;
        case vr_input_0_30_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_30_1);
            return OK;
        case vr_input_0_31_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_31_0);
            return OK;
        case vr_input_0_31_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_31_1);
            return OK;
        case vr_input_0_32_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_32_0);
            return OK;
        case vr_input_0_32_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_32_1);
            return OK;
        case vr_input_0_33_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_33_0);
            return OK;
        case vr_input_0_33_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_33_1);
            return OK;
        case vr_input_0_34_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_34_0);
            return OK;
        case vr_input_0_34_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_34_1);
            return OK;
        case vr_input_0_35_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_35_0);
            return OK;
        case vr_input_0_35_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_35_1);
            return OK;
        case vr_input_0_36_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_36_0);
            return OK;
        case vr_input_0_36_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_36_1);
            return OK;
        case vr_input_0_37_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_37_0);
            return OK;
        case vr_input_0_37_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_37_1);
            return OK;
        case vr_input_0_38_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_38_0);
            return OK;
        case vr_input_0_38_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_38_1);
            return OK;
        case vr_input_0_39_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_39_0);
            return OK;
        case vr_input_0_39_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_39_1);
            return OK;
        case vr_input_0_40_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_40_0);
            return OK;
        case vr_input_0_40_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_40_1);
            return OK;
        case vr_input_0_41_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_41_0);
            return OK;
        case vr_input_0_41_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_41_1);
            return OK;
        case vr_input_0_42_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_42_0);
            return OK;
        case vr_input_0_42_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_42_1);
            return OK;
        case vr_input_0_43_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_43_0);
            return OK;
        case vr_input_0_43_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_43_1);
            return OK;
        case vr_input_0_44_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_44_0);
            return OK;
        case vr_input_0_44_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_44_1);
            return OK;
        case vr_input_0_45_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_45_0);
            return OK;
        case vr_input_0_45_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_45_1);
            return OK;
        case vr_input_0_46_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_46_0);
            return OK;
        case vr_input_0_46_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_46_1);
            return OK;
        case vr_input_0_47_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_47_0);
            return OK;
        case vr_input_0_47_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_47_1);
            return OK;
        case vr_input_0_48_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_48_0);
            return OK;
        case vr_input_0_48_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_48_1);
            return OK;
        case vr_input_0_49_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_49_0);
            return OK;
        case vr_input_0_49_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_49_1);
            return OK;
        case vr_input_0_50_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_50_0);
            return OK;
        case vr_input_0_50_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_50_1);
            return OK;
        case vr_input_0_51_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_51_0);
            return OK;
        case vr_input_0_51_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_51_1);
            return OK;
        case vr_input_0_52_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_52_0);
            return OK;
        case vr_input_0_52_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_52_1);
            return OK;
        case vr_input_0_53_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_53_0);
            return OK;
        case vr_input_0_53_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_53_1);
            return OK;
        case vr_input_0_54_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_54_0);
            return OK;
        case vr_input_0_54_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_54_1);
            return OK;
        case vr_input_0_55_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_55_0);
            return OK;
        case vr_input_0_55_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_55_1);
            return OK;
        case vr_input_0_56_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_56_0);
            return OK;
        case vr_input_0_56_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_56_1);
            return OK;
        case vr_input_0_57_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_57_0);
            return OK;
        case vr_input_0_57_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_57_1);
            return OK;
        case vr_input_0_58_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_58_0);
            return OK;
        case vr_input_0_58_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_58_1);
            return OK;
        case vr_input_0_59_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_59_0);
            return OK;
        case vr_input_0_59_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_59_1);
            return OK;
        case vr_input_0_60_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_60_0);
            return OK;
        case vr_input_0_60_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_60_1);
            return OK;
        case vr_input_0_61_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_61_0);
            return OK;
        case vr_input_0_61_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_61_1);
            return OK;
        case vr_input_0_62_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_62_0);
            return OK;
        case vr_input_0_62_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_62_1);
            return OK;
        case vr_input_0_63_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_63_0);
            return OK;
        case vr_input_0_63_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_63_1);
            return OK;
        case vr_input_0_64_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_64_0);
            return OK;
        case vr_input_0_64_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_64_1);
            return OK;
        case vr_input_0_65_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_65_0);
            return OK;
        case vr_input_0_65_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_65_1);
            return OK;
        case vr_input_0_66_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_66_0);
            return OK;
        case vr_input_0_66_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_66_1);
            return OK;
        case vr_input_0_67_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_67_0);
            return OK;
        case vr_input_0_67_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_67_1);
            return OK;
        case vr_input_0_68_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_68_0);
            return OK;
        case vr_input_0_68_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_68_1);
            return OK;
        case vr_input_0_69_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_69_0);
            return OK;
        case vr_input_0_69_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_69_1);
            return OK;
        case vr_input_0_70_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_70_0);
            return OK;
        case vr_input_0_70_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_70_1);
            return OK;
        case vr_input_0_71_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_71_0);
            return OK;
        case vr_input_0_71_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_71_1);
            return OK;
        case vr_input_0_72_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_72_0);
            return OK;
        case vr_input_0_72_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_72_1);
            return OK;
        case vr_input_0_73_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_73_0);
            return OK;
        case vr_input_0_73_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_73_1);
            return OK;
        case vr_input_0_74_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_74_0);
            return OK;
        case vr_input_0_74_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_74_1);
            return OK;
        case vr_input_0_75_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_75_0);
            return OK;
        case vr_input_0_75_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_75_1);
            return OK;
        case vr_input_0_76_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_76_0);
            return OK;
        case vr_input_0_76_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_76_1);
            return OK;
        case vr_input_0_77_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_77_0);
            return OK;
        case vr_input_0_77_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_77_1);
            return OK;
        case vr_input_0_78_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_78_0);
            return OK;
        case vr_input_0_78_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_78_1);
            return OK;
        case vr_input_0_79_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_79_0);
            return OK;
        case vr_input_0_79_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_79_1);
            return OK;
        case vr_input_0_80_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_80_0);
            return OK;
        case vr_input_0_80_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_80_1);
            return OK;
        case vr_input_0_81_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_81_0);
            return OK;
        case vr_input_0_81_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_81_1);
            return OK;
        case vr_input_0_82_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_82_0);
            return OK;
        case vr_input_0_82_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_82_1);
            return OK;
        case vr_input_0_83_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_83_0);
            return OK;
        case vr_input_0_83_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_83_1);
            return OK;
        case vr_input_0_84_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_84_0);
            return OK;
        case vr_input_0_84_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_84_1);
            return OK;
        case vr_input_0_85_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_85_0);
            return OK;
        case vr_input_0_85_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_85_1);
            return OK;
        case vr_input_0_86_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_86_0);
            return OK;
        case vr_input_0_86_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_86_1);
            return OK;
        case vr_input_0_87_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_87_0);
            return OK;
        case vr_input_0_87_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_87_1);
            return OK;
        case vr_input_0_88_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_88_0);
            return OK;
        case vr_input_0_88_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_88_1);
            return OK;
        case vr_input_0_89_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_89_0);
            return OK;
        case vr_input_0_89_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_89_1);
            return OK;
        case vr_input_0_90_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_90_0);
            return OK;
        case vr_input_0_90_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_90_1);
            return OK;
        case vr_input_0_91_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_91_0);
            return OK;
        case vr_input_0_91_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_91_1);
            return OK;
        case vr_input_0_92_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_92_0);
            return OK;
        case vr_input_0_92_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_92_1);
            return OK;
        case vr_input_0_93_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_93_0);
            return OK;
        case vr_input_0_93_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_93_1);
            return OK;
        case vr_input_0_94_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_94_0);
            return OK;
        case vr_input_0_94_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_94_1);
            return OK;
        case vr_input_0_95_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_95_0);
            return OK;
        case vr_input_0_95_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_95_1);
            return OK;
        case vr_input_0_96_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_96_0);
            return OK;
        case vr_input_0_96_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_96_1);
            return OK;
        case vr_input_0_97_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_97_0);
            return OK;
        case vr_input_0_97_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_97_1);
            return OK;
        case vr_input_0_98_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_98_0);
            return OK;
        case vr_input_0_98_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_98_1);
            return OK;
        case vr_input_0_99_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_99_0);
            return OK;
        case vr_input_0_99_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_99_1);
            return OK;
        case vr_input_0_100_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_100_0);
            return OK;
        case vr_input_0_100_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_100_1);
            return OK;
        case vr_input_0_101_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_101_0);
            return OK;
        case vr_input_0_101_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_101_1);
            return OK;
        case vr_input_0_102_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_102_0);
            return OK;
        case vr_input_0_102_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_102_1);
            return OK;
        case vr_input_0_103_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_103_0);
            return OK;
        case vr_input_0_103_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_103_1);
            return OK;
        case vr_input_0_104_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_104_0);
            return OK;
        case vr_input_0_104_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_104_1);
            return OK;
        case vr_input_0_105_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_105_0);
            return OK;
        case vr_input_0_105_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_105_1);
            return OK;
        case vr_input_0_106_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_106_0);
            return OK;
        case vr_input_0_106_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_106_1);
            return OK;
        case vr_input_0_107_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_107_0);
            return OK;
        case vr_input_0_107_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_107_1);
            return OK;
        case vr_input_0_108_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_108_0);
            return OK;
        case vr_input_0_108_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_108_1);
            return OK;
        case vr_input_0_109_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_109_0);
            return OK;
        case vr_input_0_109_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_109_1);
            return OK;
        case vr_input_0_110_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_110_0);
            return OK;
        case vr_input_0_110_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_110_1);
            return OK;
        case vr_input_0_111_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_111_0);
            return OK;
        case vr_input_0_111_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_111_1);
            return OK;
        case vr_input_0_112_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_112_0);
            return OK;
        case vr_input_0_112_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_112_1);
            return OK;
        case vr_input_0_113_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_113_0);
            return OK;
        case vr_input_0_113_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_113_1);
            return OK;
        case vr_input_0_114_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_114_0);
            return OK;
        case vr_input_0_114_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_114_1);
            return OK;
        case vr_input_0_115_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_115_0);
            return OK;
        case vr_input_0_115_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_115_1);
            return OK;
        case vr_input_0_116_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_116_0);
            return OK;
        case vr_input_0_116_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_116_1);
            return OK;
        case vr_input_0_117_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_117_0);
            return OK;
        case vr_input_0_117_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_117_1);
            return OK;
        case vr_input_0_118_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_118_0);
            return OK;
        case vr_input_0_118_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_118_1);
            return OK;
        case vr_input_0_119_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_119_0);
            return OK;
        case vr_input_0_119_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_119_1);
            return OK;
        case vr_input_0_120_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_120_0);
            return OK;
        case vr_input_0_120_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_120_1);
            return OK;
        case vr_input_0_121_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_121_0);
            return OK;
        case vr_input_0_121_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_121_1);
            return OK;
        case vr_input_0_122_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_122_0);
            return OK;
        case vr_input_0_122_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_122_1);
            return OK;
        case vr_input_0_123_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_123_0);
            return OK;
        case vr_input_0_123_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_123_1);
            return OK;
        case vr_input_0_124_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_124_0);
            return OK;
        case vr_input_0_124_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_124_1);
            return OK;
        case vr_input_0_125_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_125_0);
            return OK;
        case vr_input_0_125_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_125_1);
            return OK;
        case vr_input_0_126_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_126_0);
            return OK;
        case vr_input_0_126_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_126_1);
            return OK;
        case vr_input_0_127_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_127_0);
            return OK;
        case vr_input_0_127_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_127_1);
            return OK;
        case vr_input_0_128_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_128_0);
            return OK;
        case vr_input_0_128_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_128_1);
            return OK;
        case vr_input_0_129_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_129_0);
            return OK;
        case vr_input_0_129_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_129_1);
            return OK;
        case vr_input_0_130_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_130_0);
            return OK;
        case vr_input_0_130_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_130_1);
            return OK;
        case vr_input_0_131_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_131_0);
            return OK;
        case vr_input_0_131_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_131_1);
            return OK;
        case vr_input_0_132_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_132_0);
            return OK;
        case vr_input_0_132_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_132_1);
            return OK;
        case vr_input_0_133_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_133_0);
            return OK;
        case vr_input_0_133_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_133_1);
            return OK;
        case vr_input_0_134_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_134_0);
            return OK;
        case vr_input_0_134_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_134_1);
            return OK;
        case vr_input_0_135_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_135_0);
            return OK;
        case vr_input_0_135_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_135_1);
            return OK;
        case vr_input_0_136_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_136_0);
            return OK;
        case vr_input_0_136_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_136_1);
            return OK;
        case vr_input_0_137_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_137_0);
            return OK;
        case vr_input_0_137_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_137_1);
            return OK;
        case vr_input_0_138_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_138_0);
            return OK;
        case vr_input_0_138_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_138_1);
            return OK;
        case vr_input_0_139_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_139_0);
            return OK;
        case vr_input_0_139_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_139_1);
            return OK;
        case vr_input_0_140_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_140_0);
            return OK;
        case vr_input_0_140_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_140_1);
            return OK;
        case vr_input_0_141_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_141_0);
            return OK;
        case vr_input_0_141_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_141_1);
            return OK;
        case vr_input_0_142_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_142_0);
            return OK;
        case vr_input_0_142_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_142_1);
            return OK;
        case vr_input_0_143_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_143_0);
            return OK;
        case vr_input_0_143_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_143_1);
            return OK;
        case vr_input_0_144_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_144_0);
            return OK;
        case vr_input_0_144_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_144_1);
            return OK;
        case vr_input_0_145_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_145_0);
            return OK;
        case vr_input_0_145_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_145_1);
            return OK;
        case vr_input_0_146_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_146_0);
            return OK;
        case vr_input_0_146_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_146_1);
            return OK;
        case vr_input_0_147_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_147_0);
            return OK;
        case vr_input_0_147_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_147_1);
            return OK;
        case vr_input_0_148_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_148_0);
            return OK;
        case vr_input_0_148_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_148_1);
            return OK;
        case vr_input_0_149_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_149_0);
            return OK;
        case vr_input_0_149_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_149_1);
            return OK;
        case vr_input_0_150_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_150_0);
            return OK;
        case vr_input_0_150_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_150_1);
            return OK;
        case vr_input_0_151_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_151_0);
            return OK;
        case vr_input_0_151_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_151_1);
            return OK;
        case vr_input_0_152_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_152_0);
            return OK;
        case vr_input_0_152_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_152_1);
            return OK;
        case vr_input_0_153_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_153_0);
            return OK;
        case vr_input_0_153_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_153_1);
            return OK;
        case vr_input_0_154_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_154_0);
            return OK;
        case vr_input_0_154_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_154_1);
            return OK;
        case vr_input_0_155_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_155_0);
            return OK;
        case vr_input_0_155_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_155_1);
            return OK;
        case vr_input_0_156_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_156_0);
            return OK;
        case vr_input_0_156_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_156_1);
            return OK;
        case vr_input_0_157_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_157_0);
            return OK;
        case vr_input_0_157_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_157_1);
            return OK;
        case vr_input_0_158_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_158_0);
            return OK;
        case vr_input_0_158_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_158_1);
            return OK;
        case vr_input_0_159_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_159_0);
            return OK;
        case vr_input_0_159_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_159_1);
            return OK;
        case vr_input_0_160_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_160_0);
            return OK;
        case vr_input_0_160_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_160_1);
            return OK;
        case vr_input_0_161_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_161_0);
            return OK;
        case vr_input_0_161_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_161_1);
            return OK;
        case vr_input_0_162_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_162_0);
            return OK;
        case vr_input_0_162_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_162_1);
            return OK;
        case vr_input_0_163_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_163_0);
            return OK;
        case vr_input_0_163_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_163_1);
            return OK;
        case vr_input_0_164_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_164_0);
            return OK;
        case vr_input_0_164_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_164_1);
            return OK;
        case vr_input_0_165_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_165_0);
            return OK;
        case vr_input_0_165_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_165_1);
            return OK;
        case vr_input_0_166_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_166_0);
            return OK;
        case vr_input_0_166_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_166_1);
            return OK;
        case vr_input_0_167_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_167_0);
            return OK;
        case vr_input_0_167_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_167_1);
            return OK;
        case vr_input_0_168_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_168_0);
            return OK;
        case vr_input_0_168_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_168_1);
            return OK;
        case vr_input_0_169_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_169_0);
            return OK;
        case vr_input_0_169_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_169_1);
            return OK;
        case vr_input_0_170_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_170_0);
            return OK;
        case vr_input_0_170_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_170_1);
            return OK;
        case vr_input_0_171_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_171_0);
            return OK;
        case vr_input_0_171_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_171_1);
            return OK;
        case vr_input_0_172_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_172_0);
            return OK;
        case vr_input_0_172_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_172_1);
            return OK;
        case vr_input_0_173_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_173_0);
            return OK;
        case vr_input_0_173_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_173_1);
            return OK;
        case vr_input_0_174_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_174_0);
            return OK;
        case vr_input_0_174_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_174_1);
            return OK;
        case vr_input_0_175_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_175_0);
            return OK;
        case vr_input_0_175_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_175_1);
            return OK;
        case vr_input_0_176_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_176_0);
            return OK;
        case vr_input_0_176_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_176_1);
            return OK;
        case vr_input_0_177_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_177_0);
            return OK;
        case vr_input_0_177_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_177_1);
            return OK;
        case vr_input_0_178_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_178_0);
            return OK;
        case vr_input_0_178_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_178_1);
            return OK;
        case vr_input_0_179_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_179_0);
            return OK;
        case vr_input_0_179_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_179_1);
            return OK;
        case vr_input_0_180_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_180_0);
            return OK;
        case vr_input_0_180_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_180_1);
            return OK;
        case vr_input_0_181_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_181_0);
            return OK;
        case vr_input_0_181_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_181_1);
            return OK;
        case vr_input_0_182_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_182_0);
            return OK;
        case vr_input_0_182_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_182_1);
            return OK;
        case vr_input_0_183_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_183_0);
            return OK;
        case vr_input_0_183_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_183_1);
            return OK;
        case vr_input_0_184_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_184_0);
            return OK;
        case vr_input_0_184_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_184_1);
            return OK;
        case vr_input_0_185_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_185_0);
            return OK;
        case vr_input_0_185_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_185_1);
            return OK;
        case vr_input_0_186_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_186_0);
            return OK;
        case vr_input_0_186_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_186_1);
            return OK;
        case vr_input_0_187_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_187_0);
            return OK;
        case vr_input_0_187_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_187_1);
            return OK;
        case vr_input_0_188_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_188_0);
            return OK;
        case vr_input_0_188_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_188_1);
            return OK;
        case vr_input_0_189_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_189_0);
            return OK;
        case vr_input_0_189_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_189_1);
            return OK;
        case vr_input_0_190_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_190_0);
            return OK;
        case vr_input_0_190_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_190_1);
            return OK;
        case vr_input_0_191_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_191_0);
            return OK;
        case vr_input_0_191_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_191_1);
            return OK;
        case vr_input_0_192_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_192_0);
            return OK;
        case vr_input_0_192_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_192_1);
            return OK;
        case vr_input_0_193_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_193_0);
            return OK;
        case vr_input_0_193_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_193_1);
            return OK;
        case vr_input_0_194_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_194_0);
            return OK;
        case vr_input_0_194_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_194_1);
            return OK;
        case vr_input_0_195_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_195_0);
            return OK;
        case vr_input_0_195_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_195_1);
            return OK;
        case vr_input_0_196_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_196_0);
            return OK;
        case vr_input_0_196_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_196_1);
            return OK;
        case vr_input_0_197_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_197_0);
            return OK;
        case vr_input_0_197_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_197_1);
            return OK;
        case vr_input_0_198_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_198_0);
            return OK;
        case vr_input_0_198_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_198_1);
            return OK;
        case vr_input_0_199_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_199_0);
            return OK;
        case vr_input_0_199_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(input_0_199_1);
            return OK;
        // Outputs
        case vr_output_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(output_0);
            return OK;
        // Locals
    default:
        // Compose message for log with value reference
        logError(comp, "getFloat32: ValueReference %d not available.", vr);
        return Error;
    }
}

Status setFloat32(ModelInstance *comp, ValueReference vr, const float values[],
                  size_t nValues, size_t *index) {
    // Switch on the value reference
    switch (vr)
    {
        // Inputs
        case vr_input_0_0_0:
            ASSERT_NVALUES(1);
            M(input_0_0_0) = values[(*index)++];
            return OK;
        case vr_input_0_0_1:
            ASSERT_NVALUES(1);
            M(input_0_0_1) = values[(*index)++];
            return OK;
        case vr_input_0_1_0:
            ASSERT_NVALUES(1);
            M(input_0_1_0) = values[(*index)++];
            return OK;
        case vr_input_0_1_1:
            ASSERT_NVALUES(1);
            M(input_0_1_1) = values[(*index)++];
            return OK;
        case vr_input_0_2_0:
            ASSERT_NVALUES(1);
            M(input_0_2_0) = values[(*index)++];
            return OK;
        case vr_input_0_2_1:
            ASSERT_NVALUES(1);
            M(input_0_2_1) = values[(*index)++];
            return OK;
        case vr_input_0_3_0:
            ASSERT_NVALUES(1);
            M(input_0_3_0) = values[(*index)++];
            return OK;
        case vr_input_0_3_1:
            ASSERT_NVALUES(1);
            M(input_0_3_1) = values[(*index)++];
            return OK;
        case vr_input_0_4_0:
            ASSERT_NVALUES(1);
            M(input_0_4_0) = values[(*index)++];
            return OK;
        case vr_input_0_4_1:
            ASSERT_NVALUES(1);
            M(input_0_4_1) = values[(*index)++];
            return OK;
        case vr_input_0_5_0:
            ASSERT_NVALUES(1);
            M(input_0_5_0) = values[(*index)++];
            return OK;
        case vr_input_0_5_1:
            ASSERT_NVALUES(1);
            M(input_0_5_1) = values[(*index)++];
            return OK;
        case vr_input_0_6_0:
            ASSERT_NVALUES(1);
            M(input_0_6_0) = values[(*index)++];
            return OK;
        case vr_input_0_6_1:
            ASSERT_NVALUES(1);
            M(input_0_6_1) = values[(*index)++];
            return OK;
        case vr_input_0_7_0:
            ASSERT_NVALUES(1);
            M(input_0_7_0) = values[(*index)++];
            return OK;
        case vr_input_0_7_1:
            ASSERT_NVALUES(1);
            M(input_0_7_1) = values[(*index)++];
            return OK;
        case vr_input_0_8_0:
            ASSERT_NVALUES(1);
            M(input_0_8_0) = values[(*index)++];
            return OK;
        case vr_input_0_8_1:
            ASSERT_NVALUES(1);
            M(input_0_8_1) = values[(*index)++];
            return OK;
        case vr_input_0_9_0:
            ASSERT_NVALUES(1);
            M(input_0_9_0) = values[(*index)++];
            return OK;
        case vr_input_0_9_1:
            ASSERT_NVALUES(1);
            M(input_0_9_1) = values[(*index)++];
            return OK;
        case vr_input_0_10_0:
            ASSERT_NVALUES(1);
            M(input_0_10_0) = values[(*index)++];
            return OK;
        case vr_input_0_10_1:
            ASSERT_NVALUES(1);
            M(input_0_10_1) = values[(*index)++];
            return OK;
        case vr_input_0_11_0:
            ASSERT_NVALUES(1);
            M(input_0_11_0) = values[(*index)++];
            return OK;
        case vr_input_0_11_1:
            ASSERT_NVALUES(1);
            M(input_0_11_1) = values[(*index)++];
            return OK;
        case vr_input_0_12_0:
            ASSERT_NVALUES(1);
            M(input_0_12_0) = values[(*index)++];
            return OK;
        case vr_input_0_12_1:
            ASSERT_NVALUES(1);
            M(input_0_12_1) = values[(*index)++];
            return OK;
        case vr_input_0_13_0:
            ASSERT_NVALUES(1);
            M(input_0_13_0) = values[(*index)++];
            return OK;
        case vr_input_0_13_1:
            ASSERT_NVALUES(1);
            M(input_0_13_1) = values[(*index)++];
            return OK;
        case vr_input_0_14_0:
            ASSERT_NVALUES(1);
            M(input_0_14_0) = values[(*index)++];
            return OK;
        case vr_input_0_14_1:
            ASSERT_NVALUES(1);
            M(input_0_14_1) = values[(*index)++];
            return OK;
        case vr_input_0_15_0:
            ASSERT_NVALUES(1);
            M(input_0_15_0) = values[(*index)++];
            return OK;
        case vr_input_0_15_1:
            ASSERT_NVALUES(1);
            M(input_0_15_1) = values[(*index)++];
            return OK;
        case vr_input_0_16_0:
            ASSERT_NVALUES(1);
            M(input_0_16_0) = values[(*index)++];
            return OK;
        case vr_input_0_16_1:
            ASSERT_NVALUES(1);
            M(input_0_16_1) = values[(*index)++];
            return OK;
        case vr_input_0_17_0:
            ASSERT_NVALUES(1);
            M(input_0_17_0) = values[(*index)++];
            return OK;
        case vr_input_0_17_1:
            ASSERT_NVALUES(1);
            M(input_0_17_1) = values[(*index)++];
            return OK;
        case vr_input_0_18_0:
            ASSERT_NVALUES(1);
            M(input_0_18_0) = values[(*index)++];
            return OK;
        case vr_input_0_18_1:
            ASSERT_NVALUES(1);
            M(input_0_18_1) = values[(*index)++];
            return OK;
        case vr_input_0_19_0:
            ASSERT_NVALUES(1);
            M(input_0_19_0) = values[(*index)++];
            return OK;
        case vr_input_0_19_1:
            ASSERT_NVALUES(1);
            M(input_0_19_1) = values[(*index)++];
            return OK;
        case vr_input_0_20_0:
            ASSERT_NVALUES(1);
            M(input_0_20_0) = values[(*index)++];
            return OK;
        case vr_input_0_20_1:
            ASSERT_NVALUES(1);
            M(input_0_20_1) = values[(*index)++];
            return OK;
        case vr_input_0_21_0:
            ASSERT_NVALUES(1);
            M(input_0_21_0) = values[(*index)++];
            return OK;
        case vr_input_0_21_1:
            ASSERT_NVALUES(1);
            M(input_0_21_1) = values[(*index)++];
            return OK;
        case vr_input_0_22_0:
            ASSERT_NVALUES(1);
            M(input_0_22_0) = values[(*index)++];
            return OK;
        case vr_input_0_22_1:
            ASSERT_NVALUES(1);
            M(input_0_22_1) = values[(*index)++];
            return OK;
        case vr_input_0_23_0:
            ASSERT_NVALUES(1);
            M(input_0_23_0) = values[(*index)++];
            return OK;
        case vr_input_0_23_1:
            ASSERT_NVALUES(1);
            M(input_0_23_1) = values[(*index)++];
            return OK;
        case vr_input_0_24_0:
            ASSERT_NVALUES(1);
            M(input_0_24_0) = values[(*index)++];
            return OK;
        case vr_input_0_24_1:
            ASSERT_NVALUES(1);
            M(input_0_24_1) = values[(*index)++];
            return OK;
        case vr_input_0_25_0:
            ASSERT_NVALUES(1);
            M(input_0_25_0) = values[(*index)++];
            return OK;
        case vr_input_0_25_1:
            ASSERT_NVALUES(1);
            M(input_0_25_1) = values[(*index)++];
            return OK;
        case vr_input_0_26_0:
            ASSERT_NVALUES(1);
            M(input_0_26_0) = values[(*index)++];
            return OK;
        case vr_input_0_26_1:
            ASSERT_NVALUES(1);
            M(input_0_26_1) = values[(*index)++];
            return OK;
        case vr_input_0_27_0:
            ASSERT_NVALUES(1);
            M(input_0_27_0) = values[(*index)++];
            return OK;
        case vr_input_0_27_1:
            ASSERT_NVALUES(1);
            M(input_0_27_1) = values[(*index)++];
            return OK;
        case vr_input_0_28_0:
            ASSERT_NVALUES(1);
            M(input_0_28_0) = values[(*index)++];
            return OK;
        case vr_input_0_28_1:
            ASSERT_NVALUES(1);
            M(input_0_28_1) = values[(*index)++];
            return OK;
        case vr_input_0_29_0:
            ASSERT_NVALUES(1);
            M(input_0_29_0) = values[(*index)++];
            return OK;
        case vr_input_0_29_1:
            ASSERT_NVALUES(1);
            M(input_0_29_1) = values[(*index)++];
            return OK;
        case vr_input_0_30_0:
            ASSERT_NVALUES(1);
            M(input_0_30_0) = values[(*index)++];
            return OK;
        case vr_input_0_30_1:
            ASSERT_NVALUES(1);
            M(input_0_30_1) = values[(*index)++];
            return OK;
        case vr_input_0_31_0:
            ASSERT_NVALUES(1);
            M(input_0_31_0) = values[(*index)++];
            return OK;
        case vr_input_0_31_1:
            ASSERT_NVALUES(1);
            M(input_0_31_1) = values[(*index)++];
            return OK;
        case vr_input_0_32_0:
            ASSERT_NVALUES(1);
            M(input_0_32_0) = values[(*index)++];
            return OK;
        case vr_input_0_32_1:
            ASSERT_NVALUES(1);
            M(input_0_32_1) = values[(*index)++];
            return OK;
        case vr_input_0_33_0:
            ASSERT_NVALUES(1);
            M(input_0_33_0) = values[(*index)++];
            return OK;
        case vr_input_0_33_1:
            ASSERT_NVALUES(1);
            M(input_0_33_1) = values[(*index)++];
            return OK;
        case vr_input_0_34_0:
            ASSERT_NVALUES(1);
            M(input_0_34_0) = values[(*index)++];
            return OK;
        case vr_input_0_34_1:
            ASSERT_NVALUES(1);
            M(input_0_34_1) = values[(*index)++];
            return OK;
        case vr_input_0_35_0:
            ASSERT_NVALUES(1);
            M(input_0_35_0) = values[(*index)++];
            return OK;
        case vr_input_0_35_1:
            ASSERT_NVALUES(1);
            M(input_0_35_1) = values[(*index)++];
            return OK;
        case vr_input_0_36_0:
            ASSERT_NVALUES(1);
            M(input_0_36_0) = values[(*index)++];
            return OK;
        case vr_input_0_36_1:
            ASSERT_NVALUES(1);
            M(input_0_36_1) = values[(*index)++];
            return OK;
        case vr_input_0_37_0:
            ASSERT_NVALUES(1);
            M(input_0_37_0) = values[(*index)++];
            return OK;
        case vr_input_0_37_1:
            ASSERT_NVALUES(1);
            M(input_0_37_1) = values[(*index)++];
            return OK;
        case vr_input_0_38_0:
            ASSERT_NVALUES(1);
            M(input_0_38_0) = values[(*index)++];
            return OK;
        case vr_input_0_38_1:
            ASSERT_NVALUES(1);
            M(input_0_38_1) = values[(*index)++];
            return OK;
        case vr_input_0_39_0:
            ASSERT_NVALUES(1);
            M(input_0_39_0) = values[(*index)++];
            return OK;
        case vr_input_0_39_1:
            ASSERT_NVALUES(1);
            M(input_0_39_1) = values[(*index)++];
            return OK;
        case vr_input_0_40_0:
            ASSERT_NVALUES(1);
            M(input_0_40_0) = values[(*index)++];
            return OK;
        case vr_input_0_40_1:
            ASSERT_NVALUES(1);
            M(input_0_40_1) = values[(*index)++];
            return OK;
        case vr_input_0_41_0:
            ASSERT_NVALUES(1);
            M(input_0_41_0) = values[(*index)++];
            return OK;
        case vr_input_0_41_1:
            ASSERT_NVALUES(1);
            M(input_0_41_1) = values[(*index)++];
            return OK;
        case vr_input_0_42_0:
            ASSERT_NVALUES(1);
            M(input_0_42_0) = values[(*index)++];
            return OK;
        case vr_input_0_42_1:
            ASSERT_NVALUES(1);
            M(input_0_42_1) = values[(*index)++];
            return OK;
        case vr_input_0_43_0:
            ASSERT_NVALUES(1);
            M(input_0_43_0) = values[(*index)++];
            return OK;
        case vr_input_0_43_1:
            ASSERT_NVALUES(1);
            M(input_0_43_1) = values[(*index)++];
            return OK;
        case vr_input_0_44_0:
            ASSERT_NVALUES(1);
            M(input_0_44_0) = values[(*index)++];
            return OK;
        case vr_input_0_44_1:
            ASSERT_NVALUES(1);
            M(input_0_44_1) = values[(*index)++];
            return OK;
        case vr_input_0_45_0:
            ASSERT_NVALUES(1);
            M(input_0_45_0) = values[(*index)++];
            return OK;
        case vr_input_0_45_1:
            ASSERT_NVALUES(1);
            M(input_0_45_1) = values[(*index)++];
            return OK;
        case vr_input_0_46_0:
            ASSERT_NVALUES(1);
            M(input_0_46_0) = values[(*index)++];
            return OK;
        case vr_input_0_46_1:
            ASSERT_NVALUES(1);
            M(input_0_46_1) = values[(*index)++];
            return OK;
        case vr_input_0_47_0:
            ASSERT_NVALUES(1);
            M(input_0_47_0) = values[(*index)++];
            return OK;
        case vr_input_0_47_1:
            ASSERT_NVALUES(1);
            M(input_0_47_1) = values[(*index)++];
            return OK;
        case vr_input_0_48_0:
            ASSERT_NVALUES(1);
            M(input_0_48_0) = values[(*index)++];
            return OK;
        case vr_input_0_48_1:
            ASSERT_NVALUES(1);
            M(input_0_48_1) = values[(*index)++];
            return OK;
        case vr_input_0_49_0:
            ASSERT_NVALUES(1);
            M(input_0_49_0) = values[(*index)++];
            return OK;
        case vr_input_0_49_1:
            ASSERT_NVALUES(1);
            M(input_0_49_1) = values[(*index)++];
            return OK;
        case vr_input_0_50_0:
            ASSERT_NVALUES(1);
            M(input_0_50_0) = values[(*index)++];
            return OK;
        case vr_input_0_50_1:
            ASSERT_NVALUES(1);
            M(input_0_50_1) = values[(*index)++];
            return OK;
        case vr_input_0_51_0:
            ASSERT_NVALUES(1);
            M(input_0_51_0) = values[(*index)++];
            return OK;
        case vr_input_0_51_1:
            ASSERT_NVALUES(1);
            M(input_0_51_1) = values[(*index)++];
            return OK;
        case vr_input_0_52_0:
            ASSERT_NVALUES(1);
            M(input_0_52_0) = values[(*index)++];
            return OK;
        case vr_input_0_52_1:
            ASSERT_NVALUES(1);
            M(input_0_52_1) = values[(*index)++];
            return OK;
        case vr_input_0_53_0:
            ASSERT_NVALUES(1);
            M(input_0_53_0) = values[(*index)++];
            return OK;
        case vr_input_0_53_1:
            ASSERT_NVALUES(1);
            M(input_0_53_1) = values[(*index)++];
            return OK;
        case vr_input_0_54_0:
            ASSERT_NVALUES(1);
            M(input_0_54_0) = values[(*index)++];
            return OK;
        case vr_input_0_54_1:
            ASSERT_NVALUES(1);
            M(input_0_54_1) = values[(*index)++];
            return OK;
        case vr_input_0_55_0:
            ASSERT_NVALUES(1);
            M(input_0_55_0) = values[(*index)++];
            return OK;
        case vr_input_0_55_1:
            ASSERT_NVALUES(1);
            M(input_0_55_1) = values[(*index)++];
            return OK;
        case vr_input_0_56_0:
            ASSERT_NVALUES(1);
            M(input_0_56_0) = values[(*index)++];
            return OK;
        case vr_input_0_56_1:
            ASSERT_NVALUES(1);
            M(input_0_56_1) = values[(*index)++];
            return OK;
        case vr_input_0_57_0:
            ASSERT_NVALUES(1);
            M(input_0_57_0) = values[(*index)++];
            return OK;
        case vr_input_0_57_1:
            ASSERT_NVALUES(1);
            M(input_0_57_1) = values[(*index)++];
            return OK;
        case vr_input_0_58_0:
            ASSERT_NVALUES(1);
            M(input_0_58_0) = values[(*index)++];
            return OK;
        case vr_input_0_58_1:
            ASSERT_NVALUES(1);
            M(input_0_58_1) = values[(*index)++];
            return OK;
        case vr_input_0_59_0:
            ASSERT_NVALUES(1);
            M(input_0_59_0) = values[(*index)++];
            return OK;
        case vr_input_0_59_1:
            ASSERT_NVALUES(1);
            M(input_0_59_1) = values[(*index)++];
            return OK;
        case vr_input_0_60_0:
            ASSERT_NVALUES(1);
            M(input_0_60_0) = values[(*index)++];
            return OK;
        case vr_input_0_60_1:
            ASSERT_NVALUES(1);
            M(input_0_60_1) = values[(*index)++];
            return OK;
        case vr_input_0_61_0:
            ASSERT_NVALUES(1);
            M(input_0_61_0) = values[(*index)++];
            return OK;
        case vr_input_0_61_1:
            ASSERT_NVALUES(1);
            M(input_0_61_1) = values[(*index)++];
            return OK;
        case vr_input_0_62_0:
            ASSERT_NVALUES(1);
            M(input_0_62_0) = values[(*index)++];
            return OK;
        case vr_input_0_62_1:
            ASSERT_NVALUES(1);
            M(input_0_62_1) = values[(*index)++];
            return OK;
        case vr_input_0_63_0:
            ASSERT_NVALUES(1);
            M(input_0_63_0) = values[(*index)++];
            return OK;
        case vr_input_0_63_1:
            ASSERT_NVALUES(1);
            M(input_0_63_1) = values[(*index)++];
            return OK;
        case vr_input_0_64_0:
            ASSERT_NVALUES(1);
            M(input_0_64_0) = values[(*index)++];
            return OK;
        case vr_input_0_64_1:
            ASSERT_NVALUES(1);
            M(input_0_64_1) = values[(*index)++];
            return OK;
        case vr_input_0_65_0:
            ASSERT_NVALUES(1);
            M(input_0_65_0) = values[(*index)++];
            return OK;
        case vr_input_0_65_1:
            ASSERT_NVALUES(1);
            M(input_0_65_1) = values[(*index)++];
            return OK;
        case vr_input_0_66_0:
            ASSERT_NVALUES(1);
            M(input_0_66_0) = values[(*index)++];
            return OK;
        case vr_input_0_66_1:
            ASSERT_NVALUES(1);
            M(input_0_66_1) = values[(*index)++];
            return OK;
        case vr_input_0_67_0:
            ASSERT_NVALUES(1);
            M(input_0_67_0) = values[(*index)++];
            return OK;
        case vr_input_0_67_1:
            ASSERT_NVALUES(1);
            M(input_0_67_1) = values[(*index)++];
            return OK;
        case vr_input_0_68_0:
            ASSERT_NVALUES(1);
            M(input_0_68_0) = values[(*index)++];
            return OK;
        case vr_input_0_68_1:
            ASSERT_NVALUES(1);
            M(input_0_68_1) = values[(*index)++];
            return OK;
        case vr_input_0_69_0:
            ASSERT_NVALUES(1);
            M(input_0_69_0) = values[(*index)++];
            return OK;
        case vr_input_0_69_1:
            ASSERT_NVALUES(1);
            M(input_0_69_1) = values[(*index)++];
            return OK;
        case vr_input_0_70_0:
            ASSERT_NVALUES(1);
            M(input_0_70_0) = values[(*index)++];
            return OK;
        case vr_input_0_70_1:
            ASSERT_NVALUES(1);
            M(input_0_70_1) = values[(*index)++];
            return OK;
        case vr_input_0_71_0:
            ASSERT_NVALUES(1);
            M(input_0_71_0) = values[(*index)++];
            return OK;
        case vr_input_0_71_1:
            ASSERT_NVALUES(1);
            M(input_0_71_1) = values[(*index)++];
            return OK;
        case vr_input_0_72_0:
            ASSERT_NVALUES(1);
            M(input_0_72_0) = values[(*index)++];
            return OK;
        case vr_input_0_72_1:
            ASSERT_NVALUES(1);
            M(input_0_72_1) = values[(*index)++];
            return OK;
        case vr_input_0_73_0:
            ASSERT_NVALUES(1);
            M(input_0_73_0) = values[(*index)++];
            return OK;
        case vr_input_0_73_1:
            ASSERT_NVALUES(1);
            M(input_0_73_1) = values[(*index)++];
            return OK;
        case vr_input_0_74_0:
            ASSERT_NVALUES(1);
            M(input_0_74_0) = values[(*index)++];
            return OK;
        case vr_input_0_74_1:
            ASSERT_NVALUES(1);
            M(input_0_74_1) = values[(*index)++];
            return OK;
        case vr_input_0_75_0:
            ASSERT_NVALUES(1);
            M(input_0_75_0) = values[(*index)++];
            return OK;
        case vr_input_0_75_1:
            ASSERT_NVALUES(1);
            M(input_0_75_1) = values[(*index)++];
            return OK;
        case vr_input_0_76_0:
            ASSERT_NVALUES(1);
            M(input_0_76_0) = values[(*index)++];
            return OK;
        case vr_input_0_76_1:
            ASSERT_NVALUES(1);
            M(input_0_76_1) = values[(*index)++];
            return OK;
        case vr_input_0_77_0:
            ASSERT_NVALUES(1);
            M(input_0_77_0) = values[(*index)++];
            return OK;
        case vr_input_0_77_1:
            ASSERT_NVALUES(1);
            M(input_0_77_1) = values[(*index)++];
            return OK;
        case vr_input_0_78_0:
            ASSERT_NVALUES(1);
            M(input_0_78_0) = values[(*index)++];
            return OK;
        case vr_input_0_78_1:
            ASSERT_NVALUES(1);
            M(input_0_78_1) = values[(*index)++];
            return OK;
        case vr_input_0_79_0:
            ASSERT_NVALUES(1);
            M(input_0_79_0) = values[(*index)++];
            return OK;
        case vr_input_0_79_1:
            ASSERT_NVALUES(1);
            M(input_0_79_1) = values[(*index)++];
            return OK;
        case vr_input_0_80_0:
            ASSERT_NVALUES(1);
            M(input_0_80_0) = values[(*index)++];
            return OK;
        case vr_input_0_80_1:
            ASSERT_NVALUES(1);
            M(input_0_80_1) = values[(*index)++];
            return OK;
        case vr_input_0_81_0:
            ASSERT_NVALUES(1);
            M(input_0_81_0) = values[(*index)++];
            return OK;
        case vr_input_0_81_1:
            ASSERT_NVALUES(1);
            M(input_0_81_1) = values[(*index)++];
            return OK;
        case vr_input_0_82_0:
            ASSERT_NVALUES(1);
            M(input_0_82_0) = values[(*index)++];
            return OK;
        case vr_input_0_82_1:
            ASSERT_NVALUES(1);
            M(input_0_82_1) = values[(*index)++];
            return OK;
        case vr_input_0_83_0:
            ASSERT_NVALUES(1);
            M(input_0_83_0) = values[(*index)++];
            return OK;
        case vr_input_0_83_1:
            ASSERT_NVALUES(1);
            M(input_0_83_1) = values[(*index)++];
            return OK;
        case vr_input_0_84_0:
            ASSERT_NVALUES(1);
            M(input_0_84_0) = values[(*index)++];
            return OK;
        case vr_input_0_84_1:
            ASSERT_NVALUES(1);
            M(input_0_84_1) = values[(*index)++];
            return OK;
        case vr_input_0_85_0:
            ASSERT_NVALUES(1);
            M(input_0_85_0) = values[(*index)++];
            return OK;
        case vr_input_0_85_1:
            ASSERT_NVALUES(1);
            M(input_0_85_1) = values[(*index)++];
            return OK;
        case vr_input_0_86_0:
            ASSERT_NVALUES(1);
            M(input_0_86_0) = values[(*index)++];
            return OK;
        case vr_input_0_86_1:
            ASSERT_NVALUES(1);
            M(input_0_86_1) = values[(*index)++];
            return OK;
        case vr_input_0_87_0:
            ASSERT_NVALUES(1);
            M(input_0_87_0) = values[(*index)++];
            return OK;
        case vr_input_0_87_1:
            ASSERT_NVALUES(1);
            M(input_0_87_1) = values[(*index)++];
            return OK;
        case vr_input_0_88_0:
            ASSERT_NVALUES(1);
            M(input_0_88_0) = values[(*index)++];
            return OK;
        case vr_input_0_88_1:
            ASSERT_NVALUES(1);
            M(input_0_88_1) = values[(*index)++];
            return OK;
        case vr_input_0_89_0:
            ASSERT_NVALUES(1);
            M(input_0_89_0) = values[(*index)++];
            return OK;
        case vr_input_0_89_1:
            ASSERT_NVALUES(1);
            M(input_0_89_1) = values[(*index)++];
            return OK;
        case vr_input_0_90_0:
            ASSERT_NVALUES(1);
            M(input_0_90_0) = values[(*index)++];
            return OK;
        case vr_input_0_90_1:
            ASSERT_NVALUES(1);
            M(input_0_90_1) = values[(*index)++];
            return OK;
        case vr_input_0_91_0:
            ASSERT_NVALUES(1);
            M(input_0_91_0) = values[(*index)++];
            return OK;
        case vr_input_0_91_1:
            ASSERT_NVALUES(1);
            M(input_0_91_1) = values[(*index)++];
            return OK;
        case vr_input_0_92_0:
            ASSERT_NVALUES(1);
            M(input_0_92_0) = values[(*index)++];
            return OK;
        case vr_input_0_92_1:
            ASSERT_NVALUES(1);
            M(input_0_92_1) = values[(*index)++];
            return OK;
        case vr_input_0_93_0:
            ASSERT_NVALUES(1);
            M(input_0_93_0) = values[(*index)++];
            return OK;
        case vr_input_0_93_1:
            ASSERT_NVALUES(1);
            M(input_0_93_1) = values[(*index)++];
            return OK;
        case vr_input_0_94_0:
            ASSERT_NVALUES(1);
            M(input_0_94_0) = values[(*index)++];
            return OK;
        case vr_input_0_94_1:
            ASSERT_NVALUES(1);
            M(input_0_94_1) = values[(*index)++];
            return OK;
        case vr_input_0_95_0:
            ASSERT_NVALUES(1);
            M(input_0_95_0) = values[(*index)++];
            return OK;
        case vr_input_0_95_1:
            ASSERT_NVALUES(1);
            M(input_0_95_1) = values[(*index)++];
            return OK;
        case vr_input_0_96_0:
            ASSERT_NVALUES(1);
            M(input_0_96_0) = values[(*index)++];
            return OK;
        case vr_input_0_96_1:
            ASSERT_NVALUES(1);
            M(input_0_96_1) = values[(*index)++];
            return OK;
        case vr_input_0_97_0:
            ASSERT_NVALUES(1);
            M(input_0_97_0) = values[(*index)++];
            return OK;
        case vr_input_0_97_1:
            ASSERT_NVALUES(1);
            M(input_0_97_1) = values[(*index)++];
            return OK;
        case vr_input_0_98_0:
            ASSERT_NVALUES(1);
            M(input_0_98_0) = values[(*index)++];
            return OK;
        case vr_input_0_98_1:
            ASSERT_NVALUES(1);
            M(input_0_98_1) = values[(*index)++];
            return OK;
        case vr_input_0_99_0:
            ASSERT_NVALUES(1);
            M(input_0_99_0) = values[(*index)++];
            return OK;
        case vr_input_0_99_1:
            ASSERT_NVALUES(1);
            M(input_0_99_1) = values[(*index)++];
            return OK;
        case vr_input_0_100_0:
            ASSERT_NVALUES(1);
            M(input_0_100_0) = values[(*index)++];
            return OK;
        case vr_input_0_100_1:
            ASSERT_NVALUES(1);
            M(input_0_100_1) = values[(*index)++];
            return OK;
        case vr_input_0_101_0:
            ASSERT_NVALUES(1);
            M(input_0_101_0) = values[(*index)++];
            return OK;
        case vr_input_0_101_1:
            ASSERT_NVALUES(1);
            M(input_0_101_1) = values[(*index)++];
            return OK;
        case vr_input_0_102_0:
            ASSERT_NVALUES(1);
            M(input_0_102_0) = values[(*index)++];
            return OK;
        case vr_input_0_102_1:
            ASSERT_NVALUES(1);
            M(input_0_102_1) = values[(*index)++];
            return OK;
        case vr_input_0_103_0:
            ASSERT_NVALUES(1);
            M(input_0_103_0) = values[(*index)++];
            return OK;
        case vr_input_0_103_1:
            ASSERT_NVALUES(1);
            M(input_0_103_1) = values[(*index)++];
            return OK;
        case vr_input_0_104_0:
            ASSERT_NVALUES(1);
            M(input_0_104_0) = values[(*index)++];
            return OK;
        case vr_input_0_104_1:
            ASSERT_NVALUES(1);
            M(input_0_104_1) = values[(*index)++];
            return OK;
        case vr_input_0_105_0:
            ASSERT_NVALUES(1);
            M(input_0_105_0) = values[(*index)++];
            return OK;
        case vr_input_0_105_1:
            ASSERT_NVALUES(1);
            M(input_0_105_1) = values[(*index)++];
            return OK;
        case vr_input_0_106_0:
            ASSERT_NVALUES(1);
            M(input_0_106_0) = values[(*index)++];
            return OK;
        case vr_input_0_106_1:
            ASSERT_NVALUES(1);
            M(input_0_106_1) = values[(*index)++];
            return OK;
        case vr_input_0_107_0:
            ASSERT_NVALUES(1);
            M(input_0_107_0) = values[(*index)++];
            return OK;
        case vr_input_0_107_1:
            ASSERT_NVALUES(1);
            M(input_0_107_1) = values[(*index)++];
            return OK;
        case vr_input_0_108_0:
            ASSERT_NVALUES(1);
            M(input_0_108_0) = values[(*index)++];
            return OK;
        case vr_input_0_108_1:
            ASSERT_NVALUES(1);
            M(input_0_108_1) = values[(*index)++];
            return OK;
        case vr_input_0_109_0:
            ASSERT_NVALUES(1);
            M(input_0_109_0) = values[(*index)++];
            return OK;
        case vr_input_0_109_1:
            ASSERT_NVALUES(1);
            M(input_0_109_1) = values[(*index)++];
            return OK;
        case vr_input_0_110_0:
            ASSERT_NVALUES(1);
            M(input_0_110_0) = values[(*index)++];
            return OK;
        case vr_input_0_110_1:
            ASSERT_NVALUES(1);
            M(input_0_110_1) = values[(*index)++];
            return OK;
        case vr_input_0_111_0:
            ASSERT_NVALUES(1);
            M(input_0_111_0) = values[(*index)++];
            return OK;
        case vr_input_0_111_1:
            ASSERT_NVALUES(1);
            M(input_0_111_1) = values[(*index)++];
            return OK;
        case vr_input_0_112_0:
            ASSERT_NVALUES(1);
            M(input_0_112_0) = values[(*index)++];
            return OK;
        case vr_input_0_112_1:
            ASSERT_NVALUES(1);
            M(input_0_112_1) = values[(*index)++];
            return OK;
        case vr_input_0_113_0:
            ASSERT_NVALUES(1);
            M(input_0_113_0) = values[(*index)++];
            return OK;
        case vr_input_0_113_1:
            ASSERT_NVALUES(1);
            M(input_0_113_1) = values[(*index)++];
            return OK;
        case vr_input_0_114_0:
            ASSERT_NVALUES(1);
            M(input_0_114_0) = values[(*index)++];
            return OK;
        case vr_input_0_114_1:
            ASSERT_NVALUES(1);
            M(input_0_114_1) = values[(*index)++];
            return OK;
        case vr_input_0_115_0:
            ASSERT_NVALUES(1);
            M(input_0_115_0) = values[(*index)++];
            return OK;
        case vr_input_0_115_1:
            ASSERT_NVALUES(1);
            M(input_0_115_1) = values[(*index)++];
            return OK;
        case vr_input_0_116_0:
            ASSERT_NVALUES(1);
            M(input_0_116_0) = values[(*index)++];
            return OK;
        case vr_input_0_116_1:
            ASSERT_NVALUES(1);
            M(input_0_116_1) = values[(*index)++];
            return OK;
        case vr_input_0_117_0:
            ASSERT_NVALUES(1);
            M(input_0_117_0) = values[(*index)++];
            return OK;
        case vr_input_0_117_1:
            ASSERT_NVALUES(1);
            M(input_0_117_1) = values[(*index)++];
            return OK;
        case vr_input_0_118_0:
            ASSERT_NVALUES(1);
            M(input_0_118_0) = values[(*index)++];
            return OK;
        case vr_input_0_118_1:
            ASSERT_NVALUES(1);
            M(input_0_118_1) = values[(*index)++];
            return OK;
        case vr_input_0_119_0:
            ASSERT_NVALUES(1);
            M(input_0_119_0) = values[(*index)++];
            return OK;
        case vr_input_0_119_1:
            ASSERT_NVALUES(1);
            M(input_0_119_1) = values[(*index)++];
            return OK;
        case vr_input_0_120_0:
            ASSERT_NVALUES(1);
            M(input_0_120_0) = values[(*index)++];
            return OK;
        case vr_input_0_120_1:
            ASSERT_NVALUES(1);
            M(input_0_120_1) = values[(*index)++];
            return OK;
        case vr_input_0_121_0:
            ASSERT_NVALUES(1);
            M(input_0_121_0) = values[(*index)++];
            return OK;
        case vr_input_0_121_1:
            ASSERT_NVALUES(1);
            M(input_0_121_1) = values[(*index)++];
            return OK;
        case vr_input_0_122_0:
            ASSERT_NVALUES(1);
            M(input_0_122_0) = values[(*index)++];
            return OK;
        case vr_input_0_122_1:
            ASSERT_NVALUES(1);
            M(input_0_122_1) = values[(*index)++];
            return OK;
        case vr_input_0_123_0:
            ASSERT_NVALUES(1);
            M(input_0_123_0) = values[(*index)++];
            return OK;
        case vr_input_0_123_1:
            ASSERT_NVALUES(1);
            M(input_0_123_1) = values[(*index)++];
            return OK;
        case vr_input_0_124_0:
            ASSERT_NVALUES(1);
            M(input_0_124_0) = values[(*index)++];
            return OK;
        case vr_input_0_124_1:
            ASSERT_NVALUES(1);
            M(input_0_124_1) = values[(*index)++];
            return OK;
        case vr_input_0_125_0:
            ASSERT_NVALUES(1);
            M(input_0_125_0) = values[(*index)++];
            return OK;
        case vr_input_0_125_1:
            ASSERT_NVALUES(1);
            M(input_0_125_1) = values[(*index)++];
            return OK;
        case vr_input_0_126_0:
            ASSERT_NVALUES(1);
            M(input_0_126_0) = values[(*index)++];
            return OK;
        case vr_input_0_126_1:
            ASSERT_NVALUES(1);
            M(input_0_126_1) = values[(*index)++];
            return OK;
        case vr_input_0_127_0:
            ASSERT_NVALUES(1);
            M(input_0_127_0) = values[(*index)++];
            return OK;
        case vr_input_0_127_1:
            ASSERT_NVALUES(1);
            M(input_0_127_1) = values[(*index)++];
            return OK;
        case vr_input_0_128_0:
            ASSERT_NVALUES(1);
            M(input_0_128_0) = values[(*index)++];
            return OK;
        case vr_input_0_128_1:
            ASSERT_NVALUES(1);
            M(input_0_128_1) = values[(*index)++];
            return OK;
        case vr_input_0_129_0:
            ASSERT_NVALUES(1);
            M(input_0_129_0) = values[(*index)++];
            return OK;
        case vr_input_0_129_1:
            ASSERT_NVALUES(1);
            M(input_0_129_1) = values[(*index)++];
            return OK;
        case vr_input_0_130_0:
            ASSERT_NVALUES(1);
            M(input_0_130_0) = values[(*index)++];
            return OK;
        case vr_input_0_130_1:
            ASSERT_NVALUES(1);
            M(input_0_130_1) = values[(*index)++];
            return OK;
        case vr_input_0_131_0:
            ASSERT_NVALUES(1);
            M(input_0_131_0) = values[(*index)++];
            return OK;
        case vr_input_0_131_1:
            ASSERT_NVALUES(1);
            M(input_0_131_1) = values[(*index)++];
            return OK;
        case vr_input_0_132_0:
            ASSERT_NVALUES(1);
            M(input_0_132_0) = values[(*index)++];
            return OK;
        case vr_input_0_132_1:
            ASSERT_NVALUES(1);
            M(input_0_132_1) = values[(*index)++];
            return OK;
        case vr_input_0_133_0:
            ASSERT_NVALUES(1);
            M(input_0_133_0) = values[(*index)++];
            return OK;
        case vr_input_0_133_1:
            ASSERT_NVALUES(1);
            M(input_0_133_1) = values[(*index)++];
            return OK;
        case vr_input_0_134_0:
            ASSERT_NVALUES(1);
            M(input_0_134_0) = values[(*index)++];
            return OK;
        case vr_input_0_134_1:
            ASSERT_NVALUES(1);
            M(input_0_134_1) = values[(*index)++];
            return OK;
        case vr_input_0_135_0:
            ASSERT_NVALUES(1);
            M(input_0_135_0) = values[(*index)++];
            return OK;
        case vr_input_0_135_1:
            ASSERT_NVALUES(1);
            M(input_0_135_1) = values[(*index)++];
            return OK;
        case vr_input_0_136_0:
            ASSERT_NVALUES(1);
            M(input_0_136_0) = values[(*index)++];
            return OK;
        case vr_input_0_136_1:
            ASSERT_NVALUES(1);
            M(input_0_136_1) = values[(*index)++];
            return OK;
        case vr_input_0_137_0:
            ASSERT_NVALUES(1);
            M(input_0_137_0) = values[(*index)++];
            return OK;
        case vr_input_0_137_1:
            ASSERT_NVALUES(1);
            M(input_0_137_1) = values[(*index)++];
            return OK;
        case vr_input_0_138_0:
            ASSERT_NVALUES(1);
            M(input_0_138_0) = values[(*index)++];
            return OK;
        case vr_input_0_138_1:
            ASSERT_NVALUES(1);
            M(input_0_138_1) = values[(*index)++];
            return OK;
        case vr_input_0_139_0:
            ASSERT_NVALUES(1);
            M(input_0_139_0) = values[(*index)++];
            return OK;
        case vr_input_0_139_1:
            ASSERT_NVALUES(1);
            M(input_0_139_1) = values[(*index)++];
            return OK;
        case vr_input_0_140_0:
            ASSERT_NVALUES(1);
            M(input_0_140_0) = values[(*index)++];
            return OK;
        case vr_input_0_140_1:
            ASSERT_NVALUES(1);
            M(input_0_140_1) = values[(*index)++];
            return OK;
        case vr_input_0_141_0:
            ASSERT_NVALUES(1);
            M(input_0_141_0) = values[(*index)++];
            return OK;
        case vr_input_0_141_1:
            ASSERT_NVALUES(1);
            M(input_0_141_1) = values[(*index)++];
            return OK;
        case vr_input_0_142_0:
            ASSERT_NVALUES(1);
            M(input_0_142_0) = values[(*index)++];
            return OK;
        case vr_input_0_142_1:
            ASSERT_NVALUES(1);
            M(input_0_142_1) = values[(*index)++];
            return OK;
        case vr_input_0_143_0:
            ASSERT_NVALUES(1);
            M(input_0_143_0) = values[(*index)++];
            return OK;
        case vr_input_0_143_1:
            ASSERT_NVALUES(1);
            M(input_0_143_1) = values[(*index)++];
            return OK;
        case vr_input_0_144_0:
            ASSERT_NVALUES(1);
            M(input_0_144_0) = values[(*index)++];
            return OK;
        case vr_input_0_144_1:
            ASSERT_NVALUES(1);
            M(input_0_144_1) = values[(*index)++];
            return OK;
        case vr_input_0_145_0:
            ASSERT_NVALUES(1);
            M(input_0_145_0) = values[(*index)++];
            return OK;
        case vr_input_0_145_1:
            ASSERT_NVALUES(1);
            M(input_0_145_1) = values[(*index)++];
            return OK;
        case vr_input_0_146_0:
            ASSERT_NVALUES(1);
            M(input_0_146_0) = values[(*index)++];
            return OK;
        case vr_input_0_146_1:
            ASSERT_NVALUES(1);
            M(input_0_146_1) = values[(*index)++];
            return OK;
        case vr_input_0_147_0:
            ASSERT_NVALUES(1);
            M(input_0_147_0) = values[(*index)++];
            return OK;
        case vr_input_0_147_1:
            ASSERT_NVALUES(1);
            M(input_0_147_1) = values[(*index)++];
            return OK;
        case vr_input_0_148_0:
            ASSERT_NVALUES(1);
            M(input_0_148_0) = values[(*index)++];
            return OK;
        case vr_input_0_148_1:
            ASSERT_NVALUES(1);
            M(input_0_148_1) = values[(*index)++];
            return OK;
        case vr_input_0_149_0:
            ASSERT_NVALUES(1);
            M(input_0_149_0) = values[(*index)++];
            return OK;
        case vr_input_0_149_1:
            ASSERT_NVALUES(1);
            M(input_0_149_1) = values[(*index)++];
            return OK;
        case vr_input_0_150_0:
            ASSERT_NVALUES(1);
            M(input_0_150_0) = values[(*index)++];
            return OK;
        case vr_input_0_150_1:
            ASSERT_NVALUES(1);
            M(input_0_150_1) = values[(*index)++];
            return OK;
        case vr_input_0_151_0:
            ASSERT_NVALUES(1);
            M(input_0_151_0) = values[(*index)++];
            return OK;
        case vr_input_0_151_1:
            ASSERT_NVALUES(1);
            M(input_0_151_1) = values[(*index)++];
            return OK;
        case vr_input_0_152_0:
            ASSERT_NVALUES(1);
            M(input_0_152_0) = values[(*index)++];
            return OK;
        case vr_input_0_152_1:
            ASSERT_NVALUES(1);
            M(input_0_152_1) = values[(*index)++];
            return OK;
        case vr_input_0_153_0:
            ASSERT_NVALUES(1);
            M(input_0_153_0) = values[(*index)++];
            return OK;
        case vr_input_0_153_1:
            ASSERT_NVALUES(1);
            M(input_0_153_1) = values[(*index)++];
            return OK;
        case vr_input_0_154_0:
            ASSERT_NVALUES(1);
            M(input_0_154_0) = values[(*index)++];
            return OK;
        case vr_input_0_154_1:
            ASSERT_NVALUES(1);
            M(input_0_154_1) = values[(*index)++];
            return OK;
        case vr_input_0_155_0:
            ASSERT_NVALUES(1);
            M(input_0_155_0) = values[(*index)++];
            return OK;
        case vr_input_0_155_1:
            ASSERT_NVALUES(1);
            M(input_0_155_1) = values[(*index)++];
            return OK;
        case vr_input_0_156_0:
            ASSERT_NVALUES(1);
            M(input_0_156_0) = values[(*index)++];
            return OK;
        case vr_input_0_156_1:
            ASSERT_NVALUES(1);
            M(input_0_156_1) = values[(*index)++];
            return OK;
        case vr_input_0_157_0:
            ASSERT_NVALUES(1);
            M(input_0_157_0) = values[(*index)++];
            return OK;
        case vr_input_0_157_1:
            ASSERT_NVALUES(1);
            M(input_0_157_1) = values[(*index)++];
            return OK;
        case vr_input_0_158_0:
            ASSERT_NVALUES(1);
            M(input_0_158_0) = values[(*index)++];
            return OK;
        case vr_input_0_158_1:
            ASSERT_NVALUES(1);
            M(input_0_158_1) = values[(*index)++];
            return OK;
        case vr_input_0_159_0:
            ASSERT_NVALUES(1);
            M(input_0_159_0) = values[(*index)++];
            return OK;
        case vr_input_0_159_1:
            ASSERT_NVALUES(1);
            M(input_0_159_1) = values[(*index)++];
            return OK;
        case vr_input_0_160_0:
            ASSERT_NVALUES(1);
            M(input_0_160_0) = values[(*index)++];
            return OK;
        case vr_input_0_160_1:
            ASSERT_NVALUES(1);
            M(input_0_160_1) = values[(*index)++];
            return OK;
        case vr_input_0_161_0:
            ASSERT_NVALUES(1);
            M(input_0_161_0) = values[(*index)++];
            return OK;
        case vr_input_0_161_1:
            ASSERT_NVALUES(1);
            M(input_0_161_1) = values[(*index)++];
            return OK;
        case vr_input_0_162_0:
            ASSERT_NVALUES(1);
            M(input_0_162_0) = values[(*index)++];
            return OK;
        case vr_input_0_162_1:
            ASSERT_NVALUES(1);
            M(input_0_162_1) = values[(*index)++];
            return OK;
        case vr_input_0_163_0:
            ASSERT_NVALUES(1);
            M(input_0_163_0) = values[(*index)++];
            return OK;
        case vr_input_0_163_1:
            ASSERT_NVALUES(1);
            M(input_0_163_1) = values[(*index)++];
            return OK;
        case vr_input_0_164_0:
            ASSERT_NVALUES(1);
            M(input_0_164_0) = values[(*index)++];
            return OK;
        case vr_input_0_164_1:
            ASSERT_NVALUES(1);
            M(input_0_164_1) = values[(*index)++];
            return OK;
        case vr_input_0_165_0:
            ASSERT_NVALUES(1);
            M(input_0_165_0) = values[(*index)++];
            return OK;
        case vr_input_0_165_1:
            ASSERT_NVALUES(1);
            M(input_0_165_1) = values[(*index)++];
            return OK;
        case vr_input_0_166_0:
            ASSERT_NVALUES(1);
            M(input_0_166_0) = values[(*index)++];
            return OK;
        case vr_input_0_166_1:
            ASSERT_NVALUES(1);
            M(input_0_166_1) = values[(*index)++];
            return OK;
        case vr_input_0_167_0:
            ASSERT_NVALUES(1);
            M(input_0_167_0) = values[(*index)++];
            return OK;
        case vr_input_0_167_1:
            ASSERT_NVALUES(1);
            M(input_0_167_1) = values[(*index)++];
            return OK;
        case vr_input_0_168_0:
            ASSERT_NVALUES(1);
            M(input_0_168_0) = values[(*index)++];
            return OK;
        case vr_input_0_168_1:
            ASSERT_NVALUES(1);
            M(input_0_168_1) = values[(*index)++];
            return OK;
        case vr_input_0_169_0:
            ASSERT_NVALUES(1);
            M(input_0_169_0) = values[(*index)++];
            return OK;
        case vr_input_0_169_1:
            ASSERT_NVALUES(1);
            M(input_0_169_1) = values[(*index)++];
            return OK;
        case vr_input_0_170_0:
            ASSERT_NVALUES(1);
            M(input_0_170_0) = values[(*index)++];
            return OK;
        case vr_input_0_170_1:
            ASSERT_NVALUES(1);
            M(input_0_170_1) = values[(*index)++];
            return OK;
        case vr_input_0_171_0:
            ASSERT_NVALUES(1);
            M(input_0_171_0) = values[(*index)++];
            return OK;
        case vr_input_0_171_1:
            ASSERT_NVALUES(1);
            M(input_0_171_1) = values[(*index)++];
            return OK;
        case vr_input_0_172_0:
            ASSERT_NVALUES(1);
            M(input_0_172_0) = values[(*index)++];
            return OK;
        case vr_input_0_172_1:
            ASSERT_NVALUES(1);
            M(input_0_172_1) = values[(*index)++];
            return OK;
        case vr_input_0_173_0:
            ASSERT_NVALUES(1);
            M(input_0_173_0) = values[(*index)++];
            return OK;
        case vr_input_0_173_1:
            ASSERT_NVALUES(1);
            M(input_0_173_1) = values[(*index)++];
            return OK;
        case vr_input_0_174_0:
            ASSERT_NVALUES(1);
            M(input_0_174_0) = values[(*index)++];
            return OK;
        case vr_input_0_174_1:
            ASSERT_NVALUES(1);
            M(input_0_174_1) = values[(*index)++];
            return OK;
        case vr_input_0_175_0:
            ASSERT_NVALUES(1);
            M(input_0_175_0) = values[(*index)++];
            return OK;
        case vr_input_0_175_1:
            ASSERT_NVALUES(1);
            M(input_0_175_1) = values[(*index)++];
            return OK;
        case vr_input_0_176_0:
            ASSERT_NVALUES(1);
            M(input_0_176_0) = values[(*index)++];
            return OK;
        case vr_input_0_176_1:
            ASSERT_NVALUES(1);
            M(input_0_176_1) = values[(*index)++];
            return OK;
        case vr_input_0_177_0:
            ASSERT_NVALUES(1);
            M(input_0_177_0) = values[(*index)++];
            return OK;
        case vr_input_0_177_1:
            ASSERT_NVALUES(1);
            M(input_0_177_1) = values[(*index)++];
            return OK;
        case vr_input_0_178_0:
            ASSERT_NVALUES(1);
            M(input_0_178_0) = values[(*index)++];
            return OK;
        case vr_input_0_178_1:
            ASSERT_NVALUES(1);
            M(input_0_178_1) = values[(*index)++];
            return OK;
        case vr_input_0_179_0:
            ASSERT_NVALUES(1);
            M(input_0_179_0) = values[(*index)++];
            return OK;
        case vr_input_0_179_1:
            ASSERT_NVALUES(1);
            M(input_0_179_1) = values[(*index)++];
            return OK;
        case vr_input_0_180_0:
            ASSERT_NVALUES(1);
            M(input_0_180_0) = values[(*index)++];
            return OK;
        case vr_input_0_180_1:
            ASSERT_NVALUES(1);
            M(input_0_180_1) = values[(*index)++];
            return OK;
        case vr_input_0_181_0:
            ASSERT_NVALUES(1);
            M(input_0_181_0) = values[(*index)++];
            return OK;
        case vr_input_0_181_1:
            ASSERT_NVALUES(1);
            M(input_0_181_1) = values[(*index)++];
            return OK;
        case vr_input_0_182_0:
            ASSERT_NVALUES(1);
            M(input_0_182_0) = values[(*index)++];
            return OK;
        case vr_input_0_182_1:
            ASSERT_NVALUES(1);
            M(input_0_182_1) = values[(*index)++];
            return OK;
        case vr_input_0_183_0:
            ASSERT_NVALUES(1);
            M(input_0_183_0) = values[(*index)++];
            return OK;
        case vr_input_0_183_1:
            ASSERT_NVALUES(1);
            M(input_0_183_1) = values[(*index)++];
            return OK;
        case vr_input_0_184_0:
            ASSERT_NVALUES(1);
            M(input_0_184_0) = values[(*index)++];
            return OK;
        case vr_input_0_184_1:
            ASSERT_NVALUES(1);
            M(input_0_184_1) = values[(*index)++];
            return OK;
        case vr_input_0_185_0:
            ASSERT_NVALUES(1);
            M(input_0_185_0) = values[(*index)++];
            return OK;
        case vr_input_0_185_1:
            ASSERT_NVALUES(1);
            M(input_0_185_1) = values[(*index)++];
            return OK;
        case vr_input_0_186_0:
            ASSERT_NVALUES(1);
            M(input_0_186_0) = values[(*index)++];
            return OK;
        case vr_input_0_186_1:
            ASSERT_NVALUES(1);
            M(input_0_186_1) = values[(*index)++];
            return OK;
        case vr_input_0_187_0:
            ASSERT_NVALUES(1);
            M(input_0_187_0) = values[(*index)++];
            return OK;
        case vr_input_0_187_1:
            ASSERT_NVALUES(1);
            M(input_0_187_1) = values[(*index)++];
            return OK;
        case vr_input_0_188_0:
            ASSERT_NVALUES(1);
            M(input_0_188_0) = values[(*index)++];
            return OK;
        case vr_input_0_188_1:
            ASSERT_NVALUES(1);
            M(input_0_188_1) = values[(*index)++];
            return OK;
        case vr_input_0_189_0:
            ASSERT_NVALUES(1);
            M(input_0_189_0) = values[(*index)++];
            return OK;
        case vr_input_0_189_1:
            ASSERT_NVALUES(1);
            M(input_0_189_1) = values[(*index)++];
            return OK;
        case vr_input_0_190_0:
            ASSERT_NVALUES(1);
            M(input_0_190_0) = values[(*index)++];
            return OK;
        case vr_input_0_190_1:
            ASSERT_NVALUES(1);
            M(input_0_190_1) = values[(*index)++];
            return OK;
        case vr_input_0_191_0:
            ASSERT_NVALUES(1);
            M(input_0_191_0) = values[(*index)++];
            return OK;
        case vr_input_0_191_1:
            ASSERT_NVALUES(1);
            M(input_0_191_1) = values[(*index)++];
            return OK;
        case vr_input_0_192_0:
            ASSERT_NVALUES(1);
            M(input_0_192_0) = values[(*index)++];
            return OK;
        case vr_input_0_192_1:
            ASSERT_NVALUES(1);
            M(input_0_192_1) = values[(*index)++];
            return OK;
        case vr_input_0_193_0:
            ASSERT_NVALUES(1);
            M(input_0_193_0) = values[(*index)++];
            return OK;
        case vr_input_0_193_1:
            ASSERT_NVALUES(1);
            M(input_0_193_1) = values[(*index)++];
            return OK;
        case vr_input_0_194_0:
            ASSERT_NVALUES(1);
            M(input_0_194_0) = values[(*index)++];
            return OK;
        case vr_input_0_194_1:
            ASSERT_NVALUES(1);
            M(input_0_194_1) = values[(*index)++];
            return OK;
        case vr_input_0_195_0:
            ASSERT_NVALUES(1);
            M(input_0_195_0) = values[(*index)++];
            return OK;
        case vr_input_0_195_1:
            ASSERT_NVALUES(1);
            M(input_0_195_1) = values[(*index)++];
            return OK;
        case vr_input_0_196_0:
            ASSERT_NVALUES(1);
            M(input_0_196_0) = values[(*index)++];
            return OK;
        case vr_input_0_196_1:
            ASSERT_NVALUES(1);
            M(input_0_196_1) = values[(*index)++];
            return OK;
        case vr_input_0_197_0:
            ASSERT_NVALUES(1);
            M(input_0_197_0) = values[(*index)++];
            return OK;
        case vr_input_0_197_1:
            ASSERT_NVALUES(1);
            M(input_0_197_1) = values[(*index)++];
            return OK;
        case vr_input_0_198_0:
            ASSERT_NVALUES(1);
            M(input_0_198_0) = values[(*index)++];
            return OK;
        case vr_input_0_198_1:
            ASSERT_NVALUES(1);
            M(input_0_198_1) = values[(*index)++];
            return OK;
        case vr_input_0_199_0:
            ASSERT_NVALUES(1);
            M(input_0_199_0) = values[(*index)++];
            return OK;
        case vr_input_0_199_1:
            ASSERT_NVALUES(1);
            M(input_0_199_1) = values[(*index)++];
            return OK;
        // Locals
    default:
        // Compose message for log with value reference
        logError(comp, "setFloat32: ValueReference %d not available.", vr);
        return Error;
    }
}
#endif

Status eventUpdate(ModelInstance *comp) {

    comp->valuesOfContinuousStatesChanged   = false;
    comp->nominalsOfContinuousStatesChanged = false;
    comp->terminateSimulation               = false;
    comp->nextEventTimeDefined              = false;

    return OK;
}
