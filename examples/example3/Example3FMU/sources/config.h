#ifndef config_h
#define config_h

// define class name and unique id
#define MODEL_IDENTIFIER example3
#define INSTANTIATION_TOKEN "d1d81023-20d1-482c-bb91-12c75afe2a4a"

#define CO_SIMULATION

#define GET_FLOAT64
#define SET_FLOAT64
#if FMI_VERSION > 2
#define GET_FLOAT32
#define SET_FLOAT32
#endif

#define FIXED_SOLVER_STEP 1
#define DEFAULT_STOP_TIME 1

typedef enum {
    // Always include time
    vr_time,
    vr_input_0_0_0,
    vr_input_0_0_1,
    vr_input_0_1_0,
    vr_input_0_1_1,
    vr_input_0_2_0,
    vr_input_0_2_1,
    vr_input_0_3_0,
    vr_input_0_3_1,
    vr_input_0_4_0,
    vr_input_0_4_1,
    vr_input_0_5_0,
    vr_input_0_5_1,
    vr_input_0_6_0,
    vr_input_0_6_1,
    vr_input_0_7_0,
    vr_input_0_7_1,
    vr_input_0_8_0,
    vr_input_0_8_1,
    vr_input_0_9_0,
    vr_input_0_9_1,
    vr_input_0_10_0,
    vr_input_0_10_1,
    vr_input_0_11_0,
    vr_input_0_11_1,
    vr_input_0_12_0,
    vr_input_0_12_1,
    vr_input_0_13_0,
    vr_input_0_13_1,
    vr_input_0_14_0,
    vr_input_0_14_1,
    vr_input_0_15_0,
    vr_input_0_15_1,
    vr_input_0_16_0,
    vr_input_0_16_1,
    vr_input_0_17_0,
    vr_input_0_17_1,
    vr_input_0_18_0,
    vr_input_0_18_1,
    vr_input_0_19_0,
    vr_input_0_19_1,
    vr_input_0_20_0,
    vr_input_0_20_1,
    vr_input_0_21_0,
    vr_input_0_21_1,
    vr_input_0_22_0,
    vr_input_0_22_1,
    vr_input_0_23_0,
    vr_input_0_23_1,
    vr_input_0_24_0,
    vr_input_0_24_1,
    vr_input_0_25_0,
    vr_input_0_25_1,
    vr_input_0_26_0,
    vr_input_0_26_1,
    vr_input_0_27_0,
    vr_input_0_27_1,
    vr_input_0_28_0,
    vr_input_0_28_1,
    vr_input_0_29_0,
    vr_input_0_29_1,
    vr_input_0_30_0,
    vr_input_0_30_1,
    vr_input_0_31_0,
    vr_input_0_31_1,
    vr_input_0_32_0,
    vr_input_0_32_1,
    vr_input_0_33_0,
    vr_input_0_33_1,
    vr_input_0_34_0,
    vr_input_0_34_1,
    vr_input_0_35_0,
    vr_input_0_35_1,
    vr_input_0_36_0,
    vr_input_0_36_1,
    vr_input_0_37_0,
    vr_input_0_37_1,
    vr_input_0_38_0,
    vr_input_0_38_1,
    vr_input_0_39_0,
    vr_input_0_39_1,
    vr_input_0_40_0,
    vr_input_0_40_1,
    vr_input_0_41_0,
    vr_input_0_41_1,
    vr_input_0_42_0,
    vr_input_0_42_1,
    vr_input_0_43_0,
    vr_input_0_43_1,
    vr_input_0_44_0,
    vr_input_0_44_1,
    vr_input_0_45_0,
    vr_input_0_45_1,
    vr_input_0_46_0,
    vr_input_0_46_1,
    vr_input_0_47_0,
    vr_input_0_47_1,
    vr_input_0_48_0,
    vr_input_0_48_1,
    vr_input_0_49_0,
    vr_input_0_49_1,
    vr_input_0_50_0,
    vr_input_0_50_1,
    vr_input_0_51_0,
    vr_input_0_51_1,
    vr_input_0_52_0,
    vr_input_0_52_1,
    vr_input_0_53_0,
    vr_input_0_53_1,
    vr_input_0_54_0,
    vr_input_0_54_1,
    vr_input_0_55_0,
    vr_input_0_55_1,
    vr_input_0_56_0,
    vr_input_0_56_1,
    vr_input_0_57_0,
    vr_input_0_57_1,
    vr_input_0_58_0,
    vr_input_0_58_1,
    vr_input_0_59_0,
    vr_input_0_59_1,
    vr_input_0_60_0,
    vr_input_0_60_1,
    vr_input_0_61_0,
    vr_input_0_61_1,
    vr_input_0_62_0,
    vr_input_0_62_1,
    vr_input_0_63_0,
    vr_input_0_63_1,
    vr_input_0_64_0,
    vr_input_0_64_1,
    vr_input_0_65_0,
    vr_input_0_65_1,
    vr_input_0_66_0,
    vr_input_0_66_1,
    vr_input_0_67_0,
    vr_input_0_67_1,
    vr_input_0_68_0,
    vr_input_0_68_1,
    vr_input_0_69_0,
    vr_input_0_69_1,
    vr_input_0_70_0,
    vr_input_0_70_1,
    vr_input_0_71_0,
    vr_input_0_71_1,
    vr_input_0_72_0,
    vr_input_0_72_1,
    vr_input_0_73_0,
    vr_input_0_73_1,
    vr_input_0_74_0,
    vr_input_0_74_1,
    vr_input_0_75_0,
    vr_input_0_75_1,
    vr_input_0_76_0,
    vr_input_0_76_1,
    vr_input_0_77_0,
    vr_input_0_77_1,
    vr_input_0_78_0,
    vr_input_0_78_1,
    vr_input_0_79_0,
    vr_input_0_79_1,
    vr_input_0_80_0,
    vr_input_0_80_1,
    vr_input_0_81_0,
    vr_input_0_81_1,
    vr_input_0_82_0,
    vr_input_0_82_1,
    vr_input_0_83_0,
    vr_input_0_83_1,
    vr_input_0_84_0,
    vr_input_0_84_1,
    vr_input_0_85_0,
    vr_input_0_85_1,
    vr_input_0_86_0,
    vr_input_0_86_1,
    vr_input_0_87_0,
    vr_input_0_87_1,
    vr_input_0_88_0,
    vr_input_0_88_1,
    vr_input_0_89_0,
    vr_input_0_89_1,
    vr_input_0_90_0,
    vr_input_0_90_1,
    vr_input_0_91_0,
    vr_input_0_91_1,
    vr_input_0_92_0,
    vr_input_0_92_1,
    vr_input_0_93_0,
    vr_input_0_93_1,
    vr_input_0_94_0,
    vr_input_0_94_1,
    vr_input_0_95_0,
    vr_input_0_95_1,
    vr_input_0_96_0,
    vr_input_0_96_1,
    vr_input_0_97_0,
    vr_input_0_97_1,
    vr_input_0_98_0,
    vr_input_0_98_1,
    vr_input_0_99_0,
    vr_input_0_99_1,
    vr_input_0_100_0,
    vr_input_0_100_1,
    vr_input_0_101_0,
    vr_input_0_101_1,
    vr_input_0_102_0,
    vr_input_0_102_1,
    vr_input_0_103_0,
    vr_input_0_103_1,
    vr_input_0_104_0,
    vr_input_0_104_1,
    vr_input_0_105_0,
    vr_input_0_105_1,
    vr_input_0_106_0,
    vr_input_0_106_1,
    vr_input_0_107_0,
    vr_input_0_107_1,
    vr_input_0_108_0,
    vr_input_0_108_1,
    vr_input_0_109_0,
    vr_input_0_109_1,
    vr_input_0_110_0,
    vr_input_0_110_1,
    vr_input_0_111_0,
    vr_input_0_111_1,
    vr_input_0_112_0,
    vr_input_0_112_1,
    vr_input_0_113_0,
    vr_input_0_113_1,
    vr_input_0_114_0,
    vr_input_0_114_1,
    vr_input_0_115_0,
    vr_input_0_115_1,
    vr_input_0_116_0,
    vr_input_0_116_1,
    vr_input_0_117_0,
    vr_input_0_117_1,
    vr_input_0_118_0,
    vr_input_0_118_1,
    vr_input_0_119_0,
    vr_input_0_119_1,
    vr_input_0_120_0,
    vr_input_0_120_1,
    vr_input_0_121_0,
    vr_input_0_121_1,
    vr_input_0_122_0,
    vr_input_0_122_1,
    vr_input_0_123_0,
    vr_input_0_123_1,
    vr_input_0_124_0,
    vr_input_0_124_1,
    vr_input_0_125_0,
    vr_input_0_125_1,
    vr_input_0_126_0,
    vr_input_0_126_1,
    vr_input_0_127_0,
    vr_input_0_127_1,
    vr_input_0_128_0,
    vr_input_0_128_1,
    vr_input_0_129_0,
    vr_input_0_129_1,
    vr_input_0_130_0,
    vr_input_0_130_1,
    vr_input_0_131_0,
    vr_input_0_131_1,
    vr_input_0_132_0,
    vr_input_0_132_1,
    vr_input_0_133_0,
    vr_input_0_133_1,
    vr_input_0_134_0,
    vr_input_0_134_1,
    vr_input_0_135_0,
    vr_input_0_135_1,
    vr_input_0_136_0,
    vr_input_0_136_1,
    vr_input_0_137_0,
    vr_input_0_137_1,
    vr_input_0_138_0,
    vr_input_0_138_1,
    vr_input_0_139_0,
    vr_input_0_139_1,
    vr_input_0_140_0,
    vr_input_0_140_1,
    vr_input_0_141_0,
    vr_input_0_141_1,
    vr_input_0_142_0,
    vr_input_0_142_1,
    vr_input_0_143_0,
    vr_input_0_143_1,
    vr_input_0_144_0,
    vr_input_0_144_1,
    vr_input_0_145_0,
    vr_input_0_145_1,
    vr_input_0_146_0,
    vr_input_0_146_1,
    vr_input_0_147_0,
    vr_input_0_147_1,
    vr_input_0_148_0,
    vr_input_0_148_1,
    vr_input_0_149_0,
    vr_input_0_149_1,
    vr_input_0_150_0,
    vr_input_0_150_1,
    vr_input_0_151_0,
    vr_input_0_151_1,
    vr_input_0_152_0,
    vr_input_0_152_1,
    vr_input_0_153_0,
    vr_input_0_153_1,
    vr_input_0_154_0,
    vr_input_0_154_1,
    vr_input_0_155_0,
    vr_input_0_155_1,
    vr_input_0_156_0,
    vr_input_0_156_1,
    vr_input_0_157_0,
    vr_input_0_157_1,
    vr_input_0_158_0,
    vr_input_0_158_1,
    vr_input_0_159_0,
    vr_input_0_159_1,
    vr_input_0_160_0,
    vr_input_0_160_1,
    vr_input_0_161_0,
    vr_input_0_161_1,
    vr_input_0_162_0,
    vr_input_0_162_1,
    vr_input_0_163_0,
    vr_input_0_163_1,
    vr_input_0_164_0,
    vr_input_0_164_1,
    vr_input_0_165_0,
    vr_input_0_165_1,
    vr_input_0_166_0,
    vr_input_0_166_1,
    vr_input_0_167_0,
    vr_input_0_167_1,
    vr_input_0_168_0,
    vr_input_0_168_1,
    vr_input_0_169_0,
    vr_input_0_169_1,
    vr_input_0_170_0,
    vr_input_0_170_1,
    vr_input_0_171_0,
    vr_input_0_171_1,
    vr_input_0_172_0,
    vr_input_0_172_1,
    vr_input_0_173_0,
    vr_input_0_173_1,
    vr_input_0_174_0,
    vr_input_0_174_1,
    vr_input_0_175_0,
    vr_input_0_175_1,
    vr_input_0_176_0,
    vr_input_0_176_1,
    vr_input_0_177_0,
    vr_input_0_177_1,
    vr_input_0_178_0,
    vr_input_0_178_1,
    vr_input_0_179_0,
    vr_input_0_179_1,
    vr_input_0_180_0,
    vr_input_0_180_1,
    vr_input_0_181_0,
    vr_input_0_181_1,
    vr_input_0_182_0,
    vr_input_0_182_1,
    vr_input_0_183_0,
    vr_input_0_183_1,
    vr_input_0_184_0,
    vr_input_0_184_1,
    vr_input_0_185_0,
    vr_input_0_185_1,
    vr_input_0_186_0,
    vr_input_0_186_1,
    vr_input_0_187_0,
    vr_input_0_187_1,
    vr_input_0_188_0,
    vr_input_0_188_1,
    vr_input_0_189_0,
    vr_input_0_189_1,
    vr_input_0_190_0,
    vr_input_0_190_1,
    vr_input_0_191_0,
    vr_input_0_191_1,
    vr_input_0_192_0,
    vr_input_0_192_1,
    vr_input_0_193_0,
    vr_input_0_193_1,
    vr_input_0_194_0,
    vr_input_0_194_1,
    vr_input_0_195_0,
    vr_input_0_195_1,
    vr_input_0_196_0,
    vr_input_0_196_1,
    vr_input_0_197_0,
    vr_input_0_197_1,
    vr_input_0_198_0,
    vr_input_0_198_1,
    vr_input_0_199_0,
    vr_input_0_199_1,
    vr_output_0,
} ValueReference;

typedef struct {
    // Always include time
    double time;
    double input_0_0_0;
    double input_0_0_1;
    double input_0_1_0;
    double input_0_1_1;
    double input_0_2_0;
    double input_0_2_1;
    double input_0_3_0;
    double input_0_3_1;
    double input_0_4_0;
    double input_0_4_1;
    double input_0_5_0;
    double input_0_5_1;
    double input_0_6_0;
    double input_0_6_1;
    double input_0_7_0;
    double input_0_7_1;
    double input_0_8_0;
    double input_0_8_1;
    double input_0_9_0;
    double input_0_9_1;
    double input_0_10_0;
    double input_0_10_1;
    double input_0_11_0;
    double input_0_11_1;
    double input_0_12_0;
    double input_0_12_1;
    double input_0_13_0;
    double input_0_13_1;
    double input_0_14_0;
    double input_0_14_1;
    double input_0_15_0;
    double input_0_15_1;
    double input_0_16_0;
    double input_0_16_1;
    double input_0_17_0;
    double input_0_17_1;
    double input_0_18_0;
    double input_0_18_1;
    double input_0_19_0;
    double input_0_19_1;
    double input_0_20_0;
    double input_0_20_1;
    double input_0_21_0;
    double input_0_21_1;
    double input_0_22_0;
    double input_0_22_1;
    double input_0_23_0;
    double input_0_23_1;
    double input_0_24_0;
    double input_0_24_1;
    double input_0_25_0;
    double input_0_25_1;
    double input_0_26_0;
    double input_0_26_1;
    double input_0_27_0;
    double input_0_27_1;
    double input_0_28_0;
    double input_0_28_1;
    double input_0_29_0;
    double input_0_29_1;
    double input_0_30_0;
    double input_0_30_1;
    double input_0_31_0;
    double input_0_31_1;
    double input_0_32_0;
    double input_0_32_1;
    double input_0_33_0;
    double input_0_33_1;
    double input_0_34_0;
    double input_0_34_1;
    double input_0_35_0;
    double input_0_35_1;
    double input_0_36_0;
    double input_0_36_1;
    double input_0_37_0;
    double input_0_37_1;
    double input_0_38_0;
    double input_0_38_1;
    double input_0_39_0;
    double input_0_39_1;
    double input_0_40_0;
    double input_0_40_1;
    double input_0_41_0;
    double input_0_41_1;
    double input_0_42_0;
    double input_0_42_1;
    double input_0_43_0;
    double input_0_43_1;
    double input_0_44_0;
    double input_0_44_1;
    double input_0_45_0;
    double input_0_45_1;
    double input_0_46_0;
    double input_0_46_1;
    double input_0_47_0;
    double input_0_47_1;
    double input_0_48_0;
    double input_0_48_1;
    double input_0_49_0;
    double input_0_49_1;
    double input_0_50_0;
    double input_0_50_1;
    double input_0_51_0;
    double input_0_51_1;
    double input_0_52_0;
    double input_0_52_1;
    double input_0_53_0;
    double input_0_53_1;
    double input_0_54_0;
    double input_0_54_1;
    double input_0_55_0;
    double input_0_55_1;
    double input_0_56_0;
    double input_0_56_1;
    double input_0_57_0;
    double input_0_57_1;
    double input_0_58_0;
    double input_0_58_1;
    double input_0_59_0;
    double input_0_59_1;
    double input_0_60_0;
    double input_0_60_1;
    double input_0_61_0;
    double input_0_61_1;
    double input_0_62_0;
    double input_0_62_1;
    double input_0_63_0;
    double input_0_63_1;
    double input_0_64_0;
    double input_0_64_1;
    double input_0_65_0;
    double input_0_65_1;
    double input_0_66_0;
    double input_0_66_1;
    double input_0_67_0;
    double input_0_67_1;
    double input_0_68_0;
    double input_0_68_1;
    double input_0_69_0;
    double input_0_69_1;
    double input_0_70_0;
    double input_0_70_1;
    double input_0_71_0;
    double input_0_71_1;
    double input_0_72_0;
    double input_0_72_1;
    double input_0_73_0;
    double input_0_73_1;
    double input_0_74_0;
    double input_0_74_1;
    double input_0_75_0;
    double input_0_75_1;
    double input_0_76_0;
    double input_0_76_1;
    double input_0_77_0;
    double input_0_77_1;
    double input_0_78_0;
    double input_0_78_1;
    double input_0_79_0;
    double input_0_79_1;
    double input_0_80_0;
    double input_0_80_1;
    double input_0_81_0;
    double input_0_81_1;
    double input_0_82_0;
    double input_0_82_1;
    double input_0_83_0;
    double input_0_83_1;
    double input_0_84_0;
    double input_0_84_1;
    double input_0_85_0;
    double input_0_85_1;
    double input_0_86_0;
    double input_0_86_1;
    double input_0_87_0;
    double input_0_87_1;
    double input_0_88_0;
    double input_0_88_1;
    double input_0_89_0;
    double input_0_89_1;
    double input_0_90_0;
    double input_0_90_1;
    double input_0_91_0;
    double input_0_91_1;
    double input_0_92_0;
    double input_0_92_1;
    double input_0_93_0;
    double input_0_93_1;
    double input_0_94_0;
    double input_0_94_1;
    double input_0_95_0;
    double input_0_95_1;
    double input_0_96_0;
    double input_0_96_1;
    double input_0_97_0;
    double input_0_97_1;
    double input_0_98_0;
    double input_0_98_1;
    double input_0_99_0;
    double input_0_99_1;
    double input_0_100_0;
    double input_0_100_1;
    double input_0_101_0;
    double input_0_101_1;
    double input_0_102_0;
    double input_0_102_1;
    double input_0_103_0;
    double input_0_103_1;
    double input_0_104_0;
    double input_0_104_1;
    double input_0_105_0;
    double input_0_105_1;
    double input_0_106_0;
    double input_0_106_1;
    double input_0_107_0;
    double input_0_107_1;
    double input_0_108_0;
    double input_0_108_1;
    double input_0_109_0;
    double input_0_109_1;
    double input_0_110_0;
    double input_0_110_1;
    double input_0_111_0;
    double input_0_111_1;
    double input_0_112_0;
    double input_0_112_1;
    double input_0_113_0;
    double input_0_113_1;
    double input_0_114_0;
    double input_0_114_1;
    double input_0_115_0;
    double input_0_115_1;
    double input_0_116_0;
    double input_0_116_1;
    double input_0_117_0;
    double input_0_117_1;
    double input_0_118_0;
    double input_0_118_1;
    double input_0_119_0;
    double input_0_119_1;
    double input_0_120_0;
    double input_0_120_1;
    double input_0_121_0;
    double input_0_121_1;
    double input_0_122_0;
    double input_0_122_1;
    double input_0_123_0;
    double input_0_123_1;
    double input_0_124_0;
    double input_0_124_1;
    double input_0_125_0;
    double input_0_125_1;
    double input_0_126_0;
    double input_0_126_1;
    double input_0_127_0;
    double input_0_127_1;
    double input_0_128_0;
    double input_0_128_1;
    double input_0_129_0;
    double input_0_129_1;
    double input_0_130_0;
    double input_0_130_1;
    double input_0_131_0;
    double input_0_131_1;
    double input_0_132_0;
    double input_0_132_1;
    double input_0_133_0;
    double input_0_133_1;
    double input_0_134_0;
    double input_0_134_1;
    double input_0_135_0;
    double input_0_135_1;
    double input_0_136_0;
    double input_0_136_1;
    double input_0_137_0;
    double input_0_137_1;
    double input_0_138_0;
    double input_0_138_1;
    double input_0_139_0;
    double input_0_139_1;
    double input_0_140_0;
    double input_0_140_1;
    double input_0_141_0;
    double input_0_141_1;
    double input_0_142_0;
    double input_0_142_1;
    double input_0_143_0;
    double input_0_143_1;
    double input_0_144_0;
    double input_0_144_1;
    double input_0_145_0;
    double input_0_145_1;
    double input_0_146_0;
    double input_0_146_1;
    double input_0_147_0;
    double input_0_147_1;
    double input_0_148_0;
    double input_0_148_1;
    double input_0_149_0;
    double input_0_149_1;
    double input_0_150_0;
    double input_0_150_1;
    double input_0_151_0;
    double input_0_151_1;
    double input_0_152_0;
    double input_0_152_1;
    double input_0_153_0;
    double input_0_153_1;
    double input_0_154_0;
    double input_0_154_1;
    double input_0_155_0;
    double input_0_155_1;
    double input_0_156_0;
    double input_0_156_1;
    double input_0_157_0;
    double input_0_157_1;
    double input_0_158_0;
    double input_0_158_1;
    double input_0_159_0;
    double input_0_159_1;
    double input_0_160_0;
    double input_0_160_1;
    double input_0_161_0;
    double input_0_161_1;
    double input_0_162_0;
    double input_0_162_1;
    double input_0_163_0;
    double input_0_163_1;
    double input_0_164_0;
    double input_0_164_1;
    double input_0_165_0;
    double input_0_165_1;
    double input_0_166_0;
    double input_0_166_1;
    double input_0_167_0;
    double input_0_167_1;
    double input_0_168_0;
    double input_0_168_1;
    double input_0_169_0;
    double input_0_169_1;
    double input_0_170_0;
    double input_0_170_1;
    double input_0_171_0;
    double input_0_171_1;
    double input_0_172_0;
    double input_0_172_1;
    double input_0_173_0;
    double input_0_173_1;
    double input_0_174_0;
    double input_0_174_1;
    double input_0_175_0;
    double input_0_175_1;
    double input_0_176_0;
    double input_0_176_1;
    double input_0_177_0;
    double input_0_177_1;
    double input_0_178_0;
    double input_0_178_1;
    double input_0_179_0;
    double input_0_179_1;
    double input_0_180_0;
    double input_0_180_1;
    double input_0_181_0;
    double input_0_181_1;
    double input_0_182_0;
    double input_0_182_1;
    double input_0_183_0;
    double input_0_183_1;
    double input_0_184_0;
    double input_0_184_1;
    double input_0_185_0;
    double input_0_185_1;
    double input_0_186_0;
    double input_0_186_1;
    double input_0_187_0;
    double input_0_187_1;
    double input_0_188_0;
    double input_0_188_1;
    double input_0_189_0;
    double input_0_189_1;
    double input_0_190_0;
    double input_0_190_1;
    double input_0_191_0;
    double input_0_191_1;
    double input_0_192_0;
    double input_0_192_1;
    double input_0_193_0;
    double input_0_193_1;
    double input_0_194_0;
    double input_0_194_1;
    double input_0_195_0;
    double input_0_195_1;
    double input_0_196_0;
    double input_0_196_1;
    double input_0_197_0;
    double input_0_197_1;
    double input_0_198_0;
    double input_0_198_1;
    double input_0_199_0;
    double input_0_199_1;
    double output_0;
} ModelData;

#endif /* config_h */
