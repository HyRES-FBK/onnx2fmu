from onnx import TensorProto


FMI_VERSIONS = ["2.0", "3.0"]
VARIABILITY = ["discrete", "continuous"]
CAUSALITY = ["input", "output", "local"]

FMI2TYPES = {
    TensorProto.FLOAT:  {"FMIType": "Real",    "CType": "double"},
    TensorProto.DOUBLE: {"FMIType": "Real",    "CType": "double"},
    TensorProto.INT4:   {"FMIType": "Integer", "CType": "int"},
    TensorProto.INT8:   {"FMIType": "Integer", "CType": "int"},
    TensorProto.INT16:  {"FMIType": "Integer", "CType": "int"},
    TensorProto.INT32:  {"FMIType": "Integer", "CType": "int"},
    TensorProto.INT64:  {"FMIType": "Integer", "CType": "int"},
    TensorProto.UINT8:  {"FMIType": "Integer", "CType": "int"},
    TensorProto.UINT16: {"FMIType": "Integer", "CType": "int"},
    TensorProto.UINT32: {"FMIType": "Integer", "CType": "int"},
    TensorProto.UINT64: {"FMIType": "Integer", "CType": "int"},
    TensorProto.BOOL:   {"FMIType": "Boolean", "CType": "bool"},
    TensorProto.STRING: {"FMIType": "String", "CType": "char"},
}

FMI3TYPES = {
    TensorProto.FLOAT:  {"FMIType": "Float32", "CType": "float"},
    TensorProto.DOUBLE: {"FMIType": "Float64", "CType": "double"},
    TensorProto.INT4:   {"FMIType": "Int8",    "CType": "int"},
    TensorProto.INT8:   {"FMIType": "Int8",    "CType": "int"},
    TensorProto.INT16:  {"FMIType": "Int16",   "CType": "int"},
    TensorProto.INT32:  {"FMIType": "Int32",   "CType": "int"},
    TensorProto.INT64:  {"FMIType": "Int64",   "CType": "int"},
    TensorProto.UINT8:  {"FMIType": "UInt8",   "CType": "int"},
    TensorProto.UINT16: {"FMIType": "UInt16",  "CType": "int"},
    TensorProto.UINT32: {"FMIType": "UInt32",  "CType": "int"},
    TensorProto.UINT64: {"FMIType": "UInt64",  "CType": "int"},
    TensorProto.BOOL:   {"FMIType": "Boolean", "CType": "bool"},
    TensorProto.STRING: {"FMIType": "String",  "CType": "char"},
}
