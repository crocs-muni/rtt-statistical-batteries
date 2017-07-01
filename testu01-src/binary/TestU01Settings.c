#include "TestU01Settings.h"

#define MODE_SMALL_CRUSH        0
#define MODE_CRUSH              1
#define MODE_BIG_CRUSH          2
#define MODE_RABBIT             3
#define MODE_ALPHABIT           4
#define MODE_BLOCK_ALPHABIT     5

#define TCOUNT_SMALL_CRUSH      10
#define TCOUNT_CRUSH            96
#define TCOUNT_BIG_CRUSH        106
#define TCOUNT_RABBIT           26
#define TCOUNT_ALPHABIT         9

#define ALPHA                   0.01

/* Horrible macro for writing consistent text around p-values */
#define PVAL_TEXT_WRAP(command) { \
    printf("=== First level p-values/statistics of the test ===\n"); \
    command \
    printf("===================================================\n"); \
}

/*
 * Defined constants for tests in respective batteries. Each constant
 * represents index in battery on which test occurrs last. E.g. Crush battery
 * has test smarsa_CollisionOver on indices from 3 to 10, therefore constant of
 * smarsa_CollisionOver test will be 10. These constants are used in elseif's
 * to determine which test will be run with certain parameters.
 */
/* Small Crush tests enum */
enum {
    sc_smarsa_BirthdaySpacing = 1,
    sc_sknuth_Collision = 2,
    sc_sknuth_Gap = 3,
    sc_sknuth_SimpPoker = 4,
    sc_sknuth_CouponCollector = 5,
    sc_sknuth_MaxOft = 6,
    sc_svaria_WeightDistrib = 7,
    sc_smarsa_MatrixRank = 8,
    sc_sstring_HammingIndep = 9,
    sc_swalk_RandomWalk1 = 10
};
/* Crush tests enum */
enum {
    c_smarsa_SerialOver = 2,
    c_smarsa_CollisionOver = 10,
    c_smarsa_BirthdaySpacings = 17,
    c_snpair_ClosePairs = 20,
    c_snpair_ClosePairsBitMatch = 22,
    c_sknuth_SimpPoker = 26,
    c_sknuth_CouponCollector = 30,
    c_sknuth_Gap = 34,
    c_sknuth_Run = 36,
    c_sknuth_Permutation = 38,
    c_sknuth_CollisionPermut = 40,
    c_sknuth_MaxOft = 44,
    c_svaria_SampleProd = 46,
    c_svaria_SampleMean = 47,
    c_svaria_SampleCorr = 48,
    c_svaria_AppearanceSpacings = 50,
    c_svaria_WeightDistrib = 54,
    c_svaria_SumCollector = 55,
    c_smarsa_MatrixRank = 61,
    c_smarsa_Savir2 = 62,
    c_smarsa_GCD = 64,
    c_swalk_RandomWalk1 = 70,
    c_scomp_LinearComp = 72,
    c_scomp_LempelZiv = 73,
    c_sspectral_Fourier3 = 75,
    c_sstring_LongestHeadRun = 77,
    c_sstring_PeriodsInStrings = 79,
    c_sstring_HammingWeight2 = 81,
    c_sstring_HammingCorr = 84,
    c_sstring_HammingIndep = 90,
    c_sstring_Run = 92,
    c_sstring_AutoCor = 96
};
/* Big Crush tests enum */
enum {
    bc_smarsa_SerialOver = 2,
    bc_smarsa_CollisionOver = 12,
    bc_smarsa_BirthdaySpacings = 21,
    bc_snpair_ClosePairs = 25,
    bc_sknuth_SimpPoker = 29,
    bc_sknuth_CouponCollector = 33,
    bc_sknuth_Gap = 37,
    bc_sknuth_Run = 39,
    bc_sknuth_Permutation = 43,
    bc_sknuth_CollisionPermut = 45,
    bc_sknuth_MaxOft = 49,
    bc_svaria_SampleProd = 52,
    bc_svaria_SampleMean = 54,
    bc_svaria_SampleCorr = 56,
    bc_svaria_AppearanceSpacings = 58,
    bc_svaria_WeightDistrib = 64,
    bc_svaria_SumCollector = 65,
    bc_smarsa_MatrixRank = 71,
    bc_smarsa_Savir2 = 72,
    bc_smarsa_GCD = 73,
    bc_swalk_RandomWalk1 = 79,
    bc_scomp_LinearComp = 81,
    bc_scomp_LempelZiv = 83,
    bc_sspectral_Fourier3 = 85,
    bc_sstring_LongestHeadRun = 87,
    bc_sstring_PeriodsInStrings = 89,
    bc_sstring_HammingWeight2 = 91,
    bc_sstring_HammingCorr = 94,
    bc_sstring_HammingIndep = 100,
    bc_sstring_Run = 102,
    bc_sstring_AutoCor = 106
};
/* Rabbit tests enum */
enum {
    rab_smultin_MultinomialBitsOver = 1,
    rab_snpair_ClosePairsBitMatch = 3,
    rab_svaria_AppearanceSpacings = 4,
    rab_scomp_LinearComp = 5,
    rab_scomp_LempelZiv = 6,
    rab_sspectral_Fourier1 = 7,
    rab_sspectral_Fourier3 = 8,
    rab_sstring_LongestHeadRun = 9,
    rab_sstring_PeriodsInStrings = 10,
    rab_sstring_HammingWeight = 11,
    rab_sstring_HammingCorr = 14,
    rab_sstring_HammingIndep = 17,
    rab_sstring_AutoCor = 19,
    rab_sstring_Run = 20,
    rab_smarsa_MatrixRank = 23,
    rab_swalk_RandomWalk1 = 26
};
/* Alphabit tests enum - same for BlockAlphabit */
enum {
    alpha_smultin_MultinomialBitsOver = 4,
    alpha_sstring_HammingIndep = 6,
    alpha_sstring_HammingCorr = 7,
    alpha_swalk_RandomWalk1 = 9
};


TestU01Settings * initSettings(int argc, char *argv[]) {
    if(!argv || argc <= 1) {
        fprintf(stderr , "[ERROR] Invalid usage of command-line arguments.\n");
        return NULL;
    }

    TestU01Settings * settings =
            (TestU01Settings *) malloc(sizeof(TestU01Settings));
    if(!settings) {
        fprintf(stderr , "[ERROR] Memory allocation error.\n");
        return NULL;
    }
    settings->batteryMode = -1;
    settings->testConstant = -1;
    settings->pSamples = 1;
    settings->bit_nb = -1;
    settings->bit_r = 0;
    settings->bit_s = 32;
    settings->bit_w = -1;
    settings->parametersCount = 0;
    settings->parameters = NULL;
    settings->inputFilePath = NULL;

    int i;
    for(i = 1 ; i < argc ; i += 2) {
        /* Option without value */
        if(i == (argc - 1)) {
            fprintf(stderr , "[ERROR] Invalid usage of CLI arguments.\n");
            return NULL;
        }

        /* Parsing arguments and their values */
        if(strcmp(argv[i] , "-m") == 0) {
            /* Mode option */
            if(settings->batteryMode != -1 || argv[i + 1][0] == '-') {
                fprintf(stderr , "[ERROR] Argument \"-m\" can't "
                                 "be empty or set multiple times.\n");
                deleteSettings(settings);
                return NULL;
            }
            if(strcmp(argv[i + 1] , "small_crush") == 0) {
                settings->batteryMode = MODE_SMALL_CRUSH;
            } else if(strcmp(argv[i + 1] , "crush") == 0) {
                settings->batteryMode = MODE_CRUSH;
            } else if(strcmp(argv[i + 1] , "big_crush") == 0) {
                settings->batteryMode = MODE_BIG_CRUSH;
            } else if(strcmp(argv[i + 1] , "rabbit") == 0) {
                settings->batteryMode = MODE_RABBIT;
            } else if(strcmp(argv[i + 1] , "alphabit") == 0) {
                settings->batteryMode = MODE_ALPHABIT;
            } else if(strcmp(argv[i + 1] , "block_alphabit") == 0) {
                settings->batteryMode = MODE_BLOCK_ALPHABIT;
            } else {
                fprintf(stderr , "[ERROR] Invalid battery mode entered: %s\n" ,
                        argv[i + 1]);
                deleteSettings(settings);
                return NULL;
            }

        } else if(strcmp(argv[i] , "-t") == 0) {
            /* Test constant option */
            if(settings->testConstant != -1 || argv[i + 1][0] == '-') {
                fprintf(stderr , "[ERROR] Argument \"-t\" can't "
                                 "be empty or set multiple times.\n");
                deleteSettings(settings);
                return NULL;
            }
            if(u_strtoi(argv[i + 1] , &settings->testConstant) != 0) {
                deleteSettings(settings);
                return NULL;
            }

        } else if(strcmp(argv[i] , "-i") == 0) {
            /* Input file path option */
            if(settings->inputFilePath || argv[i + 1][0] == '-') {
                fprintf(stderr , "[ERROR] Argument \"-i\" can't "
                                 "be empty or set multiple times.\n");
                deleteSettings(settings);
                return NULL;
            }
            settings->inputFilePath = (char *)
                    malloc(strlen(argv[i + 1]) + 1 * sizeof(char));
            if(!settings->inputFilePath) {
                fprintf(stderr , "[ERROR] Memory allocation error.\n");
                deleteSettings(settings);
                return NULL;
            }
            strncpy(settings->inputFilePath , argv[i + 1] , strlen(argv[i + 1]) + 1);

        } else if(strcmp(argv[i] , "-r") == 0) {
            /* Repetitions option (optional) */
            if(settings->pSamples != 1 || argv[i+1][0] == '-') {
                fprintf(stderr , "[ERROR] Argument \"-r\" can't "
                                 "be empty or set multiple times.\n");
                deleteSettings(settings);
                return NULL;
            }
            if(u_strtoi(argv[i + 1] , &settings->pSamples) != 0) {
                deleteSettings(settings);
                return NULL;
            }

        } else if(strcmp(argv[i] , "--bit_nb") == 0) {
            /* Number of bits option - ignored for crush batteries */
            /* Mandatory for Rabbit and (Block) Alphabit batteries */
            if(settings->bit_nb != -1 || argv[i+1][0] == '-') {
                fprintf(stderr , "[ERROR] Argument \"--bit_nb\" can't "
                                 "be empty or set multiple times.\n");
                deleteSettings(settings);
                return NULL;
            }
            if(u_strtod(argv[i+1] , &settings->bit_nb) != 0) {
                deleteSettings(settings);
                return NULL;
            }

        } else if(strcmp(argv[i] , "--bit_r") == 0) {
            /* Dropped bits option - ignored for crush and rabbit batteries */
            /* Optional for (Block) Alphabit battery */
            if(settings->bit_r != 0 || argv[i+1][0] == '-') {
                fprintf(stderr , "[ERROR] Argument \"--bit_r\" can't "
                                 "be empty or set multiple times.\n");
                deleteSettings(settings);
                return NULL;
            }
            if(u_strtoi(argv[i+1] , &settings->bit_r) != 0) {
                deleteSettings(settings);
                return NULL;
            }

        } else if(strcmp(argv[i] , "--bit_s") == 0) {
            /* Processed bits option - ignored for crush and rabit batteries */
            /* Optional for (Block) Alphabit battery */
            if(settings->bit_s != 32 || argv[i+1][0] == '-') {
                fprintf(stderr , "[ERROR] Argument \"--bit_s\" can't "
                                 "be empty or set multiple times.\n");
                deleteSettings(settings);
                return NULL;
            }
            if(u_strtoi(argv[i+1] , &settings->bit_s) != 0) {
                deleteSettings(settings);
                return NULL;
            }

        } else if(strcmp(argv[i] , "--bit_w") == 0) {
            /* Bit reordering option - ignored for crush, rabbit and alphabit batteries */
            /* Optional for Block Alphabit */
            if(settings->bit_w != -1 || argv[i+1][0] == '-') {
                fprintf(stderr , "[ERROR] Argument \"--bit_w\" can't "
                                 "be empty or set multiple times.\n");
                deleteSettings(settings);
                return NULL;
            }
            if(u_strtoi(argv[i+1] , &settings->bit_w) != 0) {
                deleteSettings(settings);
                return NULL;
            }

        } else if(strcmp(argv[i] , "--params") == 0) {
            /* Parameters option (optional) - ignored in Rabbit/Alphabit */
            if(settings->parametersCount != 0 || argv[i+1][0] == '-') {
                fprintf(stderr , "[ERROR] Argument \"--params\" can't "
                                 "be empty or set multiple times.\n");
                deleteSettings(settings);
                return NULL;
            }
            int parSize = 6;
            settings->parameters = (double *) malloc (parSize * sizeof(double));
            if(!settings->parameters) {
                fprintf(stderr , "[ERROR] Memory allocation failed.\n");
                deleteSettings(settings);
                return NULL;
            }
            double paramValue = 0;
            for(;;) {
                ++i;
                if(i == argc || argv[i][0] == '-') {
                    /* End of arguments or '-' detected */
                    i -= 2;
                    break;
                }
                if(settings->parametersCount == parSize) {
                    /* Not enough space, realloc */
                    parSize *= 2;
                    double * backup = (double *) realloc(settings->parameters , parSize * sizeof(double));
                    if(!backup) {
                        fprintf(stderr , "[ERROR] Memory allocation failed.\n");
                        deleteSettings(settings);
                        return NULL;
                    }
                    settings->parameters = backup;
                }
                /* Everything should be okay, store parameter */
                if(u_strtod(argv[i] , &paramValue) != 0) {
                    deleteSettings(settings);
                    return NULL;
                }
                settings->parameters[settings->parametersCount] = paramValue;
                ++settings->parametersCount;
            }
            /* Parameters are stored, realloc so we don't waste space */
            double * backup = (double *) realloc
                    (settings->parameters , settings->parametersCount * sizeof(double));
            if(!backup) {
                fprintf(stderr , "[ERROR] Memory allocation failed.\n");
                deleteSettings(settings);
                return NULL;
            }
            settings->parameters = backup;

        } else {
            /* Unknown option => fail */
            fprintf(stderr , "[ERROR] Invalid option found: %s\n" , argv[i]);
            deleteSettings(settings);
            return NULL;
        }
    } /* FOR loop end */

    /* Settings sanity checking */
    if(settings->batteryMode < 0 ||
            settings->testConstant < 0 ||
            !settings->inputFilePath) {
        fprintf(stderr , "[ERROR] Some of the arbitrary arguments weren't set.\n");
        deleteSettings(settings);
        return NULL;
    }

    if(settings->bit_nb < 1 && (settings->batteryMode == MODE_RABBIT ||
                                settings->batteryMode == MODE_ALPHABIT ||
                                settings->batteryMode == MODE_BLOCK_ALPHABIT)) {
        fprintf(stderr , "[ERROR] Option \"--bit_nb\" must be set in batteries "
                         "Rabbit and (Block) Alphabit.");
        deleteSettings(settings);
        return NULL;
    }

    return settings;
}

void deleteSettings(TestU01Settings * settings) {
    if(!settings)
        return;
    if(settings->inputFilePath)
        free(settings->inputFilePath);
    if(settings->parameters)
        free(settings->parameters);
    free(settings);
}

void tu01_printUsage() {
    printf("\n================================== [USAGE] ==================================\n"
           "\n"
           "For information about tests, their parameters and such, see\n"
           "TestU01 library documentation.\n"
           "http://simul.iro.umontreal.ca/testu01/guideshorttestu01.pdf\n"
           "\n"
           "      ./TestU01 [-m <battery>] [-t <test>] [-i <in_path>]\n"
           "                [-r <reps>] [--bit_nb <nb>] [--bit_r <r>]\n"
           "                [--bit_s <s>] [--params <pars>]\n"
           "\n"
           "-m <battery>    (Mandatory) Option for choosing from which battery\n"
           "                the test will be picked. Possible values\n"
           "                for <battery> are: \"small_crush\", \"crush\",\n"
           "                \"big_crush\", \"rabbit\", \"alphabit\" or \"block_alphabit\".\n"
           "\n"
           "-t <test>       (Mandatory) Picks single test from <battery> that\n"
           "                will be executed. <test> is numeric constant of test.\n"
           "                For constants see library documetation.\n"
           "\n"
           "-i <in_path>    (Mandatory) Path to input file with binary data.\n"
           "                Analysis will be done on this data.\n"
           "\n"
           "-r <reps>       (Optional) Sets number of test repetitions. If set,\n"
           "                test will be executed <reps> times resulting in <reps>\n"
           "                p-values. Default is 1.\n"
           "\n"
           "--bit_nb <nb>   (Mandatory when <battery> is rabbit or (block_)alphabit )\n"
           "                Sets numbers of bits that will be taken from file\n"
           "                for analysis. The rest of the bits is left out.\n"
           "\n"
           "--bit_r <r>     (Optional when <battery> is (block) alphabit) Sets how\n"
           "                many most significant bits are left out from each\n"
           "                4 byte block in analysis. When <r> == 0, no bits\n"
           "                are dropped. Default is 0.\n"
           "\n"
           "--bit_s <s>     (Optional when <battery> is (block_)alphabit) Sets how many\n"
           "                consecutive bits after <r> will be processed. When\n"
           "                <r> == 0 and <s> == 32 full blocks are processed.\n"
           "                When <r> == 0 and <s> == 10 only first 10 bits from\n"
           "                each 32 bits are processed. Default is 32.\n"
           "\n"
           "--bit_w <w>     (Optional when <battery> is block_alphabit) Sets bit reordering\n"
           "                before applying alphabit battery to stream. When not set, values\n"
           "                from set {1, 2, 4, 8, 16, 32} that are lesser or equal to <s>\n"
           "                will be used.\n"
           "\n"
           "--params <pars> (Optional but ignored when <battery> is rabbit or\n"
           "                (block) alphabit)\n"
           "                Sets custom parameters for tests from Crush batteries.\n"
           "                <pars> is consecutive list of parameters. All parameters\n"
           "                must be set when using this option and must be in order\n"
           "                that is used in documentation.\n"
           "                Example: smarsa_BirthdaySpacings test takes 6 parameters:\n"
           "                N, n, r, d, t, p. If we wanted to run it in default settings,\n"
           "                then <pars> would be \"1 5000000 0 1073741824 2 1\".\n"
           "                Decimals can be used too, for Bool values use 1 for true\n"
           "                and 0 for false. If this option is ommited, test will be\n"
           "                run in default settings according to library documentation.\n"
           "\n================================== [USAGE] ==================================\n\n"
           );
}

void executeTest(const TestU01Settings * settings) {
    if(!settings)
        return;
    gofw_Suspectp = ALPHA;

    printf("[INFO]  Significance level is set to alpha = %.3f%% (%.3f)\n" , ALPHA*100,ALPHA);

    if(settings->batteryMode == MODE_SMALL_CRUSH) {
        tu01_execSmallCrushTest(settings);
    } else if(settings->batteryMode == MODE_CRUSH) {
        tu01_execCrushTest(settings);
    } else if(settings->batteryMode == MODE_BIG_CRUSH) {
        tu01_execBigCrushTest(settings);
    } else if(settings->batteryMode == MODE_RABBIT) {
        tu01_execRabbitTest(settings);
    } else if(settings->batteryMode == MODE_ALPHABIT) {
        tu01_execAlphabitTest(settings);
    } else if(settings->batteryMode == MODE_BLOCK_ALPHABIT) {
        tu01_execBlockAlphabitTest(settings);
    } else {
        /* This shouldn't happen */
        fprintf(stderr , "[ERROR] Unknown battery constant in settings.\n");
        return;
    }
}

void tu01_execSmallCrushTest(const TestU01Settings * settings) {
    if(!settings)
        return;

    if(settings->testConstant > TCOUNT_SMALL_CRUSH || settings->testConstant < 1) {
        fprintf(stderr , "[ERROR] Unknown test in SmallCrush used: %i\n" ,
                settings->testConstant);
        return;
    }

    unif01_Gen * generator = unif01_CreateFileStreamGen(settings->inputFilePath);
    if(!generator) {
        fprintf(stderr , "[ERROR] FileStream Generator creation failed.\n");
        return;
    }

    if(settings->parametersCount == 0) {
        /* Test will be run with default parameters */
        int reps[TCOUNT_SMALL_CRUSH + 1] = {0};
        reps[settings->testConstant] = settings->pSamples;
        bbattery_RepeatSmallCrush(generator , reps);
    } else {
        /* Test will be run with custom parameters */
        int (*doTest) (const TestU01Settings * , unif01_Gen *) = NULL;

        if(settings->testConstant <= sc_smarsa_BirthdaySpacing) {
            doTest = do_smarsa_BirthdaySpacings;
        } else if (settings->testConstant <= sc_sknuth_Collision) {
            doTest = do_sknuth_Collision;
        } else if (settings->testConstant <= sc_sknuth_Gap) {
            doTest = do_sknuth_Gap;
        } else if (settings->testConstant <= sc_sknuth_SimpPoker) {
            doTest = do_sknuth_Gap;
        } else if (settings->testConstant <= sc_sknuth_CouponCollector) {
            doTest = do_sknuth_CouponCollector;
        } else if (settings->testConstant <= sc_sknuth_MaxOft) {
            doTest = do_sknuth_MaxOft;
        } else if (settings->testConstant <= sc_svaria_WeightDistrib) {
            doTest = do_svaria_WeightDistrib;
        } else if (settings->testConstant <= sc_smarsa_MatrixRank) {
            doTest = do_smarsa_MatrixRank;
        } else if (settings->testConstant <= sc_sstring_HammingIndep) {
            doTest = do_sstring_HammingIndep;
        } else if (settings->testConstant <= sc_swalk_RandomWalk1) {
            doTest = do_swalk_RandomWalk1;
        } else {
            /* This won't ever happen */
            fprintf(stderr , "[ERROR] Unknown test in SmallCrush used: %i\n" ,
                    settings->testConstant);
            return;
        }

        /* Running test */
        int rval = 0;
        int i = 0;
        for(i = 0; i < settings->pSamples ; ++i) {
            rval = doTest(settings , generator);
            if(rval != 0) {
                fprintf(stderr , "[ERROR] Test wasn't executed. Terminating.\n");
                return;
            }
        }
    }
    unif01_DeleteFileStreamGen(generator);
}

void tu01_execCrushTest(const TestU01Settings * settings) {
    if(!settings)
        return;

    if(settings->testConstant > TCOUNT_CRUSH || settings->testConstant < 1) {
        fprintf(stderr , "[ERROR] Unknown test in Crush used: %i\n" ,
                settings->testConstant);
        return;
    }

    unif01_Gen * generator = unif01_CreateFileStreamGen(settings->inputFilePath);
    if(!generator) {
        fprintf(stderr , "[ERROR] FileStream Generator creation failed.\n");
        return;
    }

    if(settings->parametersCount == 0) {
        /* Test will be run with default parameters */
        int reps[TCOUNT_CRUSH + 1] = {0};
        reps[settings->testConstant] = settings->pSamples;
        bbattery_RepeatCrush(generator , reps);
    } else {
        /* Test will be run with custom parameters */
        int (*doTest) (const TestU01Settings * , unif01_Gen *) = NULL;

        if(settings->testConstant <= c_smarsa_SerialOver) {
            doTest = do_smarsa_SerialOver;
        } else if(settings->testConstant <= c_smarsa_CollisionOver) {
            doTest = do_smarsa_CollisionOver;
        } else if(settings->testConstant <= c_smarsa_BirthdaySpacings) {
            doTest = do_smarsa_BirthdaySpacings;
        } else if(settings->testConstant <= c_snpair_ClosePairs) {
            doTest = do_snpair_ClosePairs;
        } else if(settings->testConstant <= c_snpair_ClosePairsBitMatch) {
            doTest = do_snpair_ClosePairsBitMatch;
        } else if(settings->testConstant <= c_sknuth_SimpPoker) {
            doTest = do_sknuth_SimpPoker;
        } else if(settings->testConstant <= c_sknuth_CouponCollector) {
            doTest = do_sknuth_CouponCollector;
        } else if(settings->testConstant <= c_sknuth_Gap) {
            doTest = do_sknuth_Gap;
        } else if(settings->testConstant <= c_sknuth_Run) {
            doTest = do_sknuth_Run;
        } else if(settings->testConstant <= c_sknuth_Permutation) {
            doTest = do_sknuth_Permutation;
        } else if(settings->testConstant <= c_sknuth_CollisionPermut) {
            doTest = do_sknuth_CollisionPermut;
        } else if(settings->testConstant <= c_sknuth_MaxOft) {
            doTest = do_sknuth_MaxOft;
        } else if(settings->testConstant <= c_svaria_SampleProd) {
            doTest = do_svaria_SampleProd;
        } else if(settings->testConstant <= c_svaria_SampleMean) {
            doTest = do_svaria_SampleMean;
        } else if(settings->testConstant <= c_svaria_SampleCorr) {
            doTest = do_svaria_SampleCorr;
        } else if(settings->testConstant <= c_svaria_AppearanceSpacings) {
            doTest = do_svaria_AppearanceSpacings;
        } else if(settings->testConstant <= c_svaria_WeightDistrib) {
            doTest = do_svaria_WeightDistrib;
        } else if(settings->testConstant <= c_svaria_SumCollector) {
            doTest = do_svaria_SumCollector;
        } else if(settings->testConstant <= c_smarsa_MatrixRank) {
            doTest = do_smarsa_MatrixRank;
        } else if(settings->testConstant <= c_smarsa_Savir2) {
            doTest = do_smarsa_Savir2;
        } else if(settings->testConstant <= c_smarsa_GCD) {
            doTest = do_smarsa_GCD;
        } else if(settings->testConstant <= c_swalk_RandomWalk1) {
            doTest = do_swalk_RandomWalk1;
        } else if(settings->testConstant <= c_scomp_LinearComp) {
            doTest = do_scomp_LinearComp;
        } else if(settings->testConstant <= c_scomp_LempelZiv) {
            doTest = do_scomp_LempelZiv;
        } else if(settings->testConstant <= c_sspectral_Fourier3) {
            doTest = do_sspectral_Fourier3;
        } else if(settings->testConstant <= c_sstring_LongestHeadRun) {
            doTest = do_sstring_LongestHeadRun;
        } else if(settings->testConstant <= c_sstring_PeriodsInStrings) {
            doTest = do_sstring_PeriodsInStrings;
        } else if(settings->testConstant <= c_sstring_HammingWeight2) {
            doTest = do_sstring_HammingWeight2;
        } else if(settings->testConstant <= c_sstring_HammingCorr) {
            doTest = do_sstring_HammingCorr;
        } else if(settings->testConstant <= c_sstring_HammingIndep) {
            doTest = do_sstring_HammingIndep;
        } else if(settings->testConstant <= c_sstring_Run) {
            doTest = do_sstring_Run;
        } else if(settings->testConstant <= c_sstring_AutoCor) {
            doTest = do_sstring_AutoCor;
        } else {
            /* This won't ever happen */
            fprintf(stderr , "[ERROR] Unknown test in Crush used: %i\n" ,
                    settings->testConstant);
            return;
        }

        /* Running test */
        int rval = 0;
        int i = 0;
        for(i = 0; i < settings->pSamples ; ++i) {
            rval = doTest(settings , generator);
            if(rval != 0) {
                fprintf(stderr , "[ERROR] Test wasn't executed. Terminating.\n");
                return;
            }
        }
    }
    unif01_DeleteFileStreamGen(generator);
}

void tu01_execBigCrushTest(const TestU01Settings * settings) {
    if(!settings)
        return;
    if(settings->testConstant > TCOUNT_BIG_CRUSH || settings->testConstant < 1) {
        fprintf(stderr , "[ERROR] Unknown test in BigCrush used: %i\n" ,
                settings->testConstant);
        return;
    }
    unif01_Gen * generator = unif01_CreateFileStreamGen(settings->inputFilePath);
    if(!generator) {
        fprintf(stderr , "[ERROR] FileStream Generator creation failed.\n");
        return;
    }

    if(settings->parametersCount == 0) {
        /* Test will be run with default parameters */
        int reps[TCOUNT_BIG_CRUSH + 1] = {0};
        reps[settings->testConstant] = settings->pSamples;
        bbattery_RepeatBigCrush(generator , reps);
    } else {
        /* Test will be run with custom parameters */
        int (*doTest) (const TestU01Settings * , unif01_Gen *) = NULL;

        /* Picking test selected via CLI */
        if(settings->testConstant <= bc_smarsa_SerialOver) {
            doTest = do_smarsa_SerialOver;
        } else if(settings->testConstant <= bc_smarsa_CollisionOver) {
            doTest = do_smarsa_CollisionOver;
        } else if(settings->testConstant <= bc_smarsa_BirthdaySpacings) {
            doTest = do_smarsa_BirthdaySpacings;
        } else if(settings->testConstant <= bc_snpair_ClosePairs) {
            doTest = do_snpair_ClosePairs;
        } else if(settings->testConstant <= bc_sknuth_SimpPoker) {
            doTest = do_sknuth_SimpPoker;
        } else if(settings->testConstant <= bc_sknuth_CouponCollector) {
            doTest = do_sknuth_CouponCollector;
        } else if(settings->testConstant <= bc_sknuth_Gap) {
            doTest = do_sknuth_Gap;
        } else if(settings->testConstant <= bc_sknuth_Run) {
            doTest = do_sknuth_Run;
        } else if(settings->testConstant <= bc_sknuth_Permutation) {
            doTest = do_sknuth_Permutation;
        } else if(settings->testConstant <= bc_sknuth_CollisionPermut) {
            doTest = do_sknuth_CollisionPermut;
        } else if(settings->testConstant <= bc_sknuth_MaxOft) {
            doTest = do_sknuth_MaxOft;
        } else if(settings->testConstant <= bc_svaria_SampleProd) {
            doTest = do_svaria_SampleProd;
        } else if(settings->testConstant <= bc_svaria_SampleMean) {
            doTest = do_svaria_SampleMean;
        } else if(settings->testConstant <= bc_svaria_SampleCorr) {
            doTest = do_svaria_SampleCorr;
        } else if(settings->testConstant <= bc_svaria_AppearanceSpacings) {
            doTest = do_svaria_AppearanceSpacings;
        } else if(settings->testConstant <= bc_svaria_WeightDistrib) {
            doTest = do_svaria_WeightDistrib;
        } else if(settings->testConstant <= bc_svaria_SumCollector) {
            doTest = do_svaria_SumCollector;
        } else if(settings->testConstant <= bc_smarsa_MatrixRank) {
            doTest = do_smarsa_MatrixRank;
        } else if(settings->testConstant <= bc_smarsa_Savir2) {
            doTest = do_smarsa_Savir2;
        } else if(settings->testConstant <= bc_smarsa_GCD) {
            doTest = do_smarsa_GCD;
        } else if(settings->testConstant <= bc_swalk_RandomWalk1) {
            doTest = do_swalk_RandomWalk1;
        } else if(settings->testConstant <= bc_scomp_LinearComp) {
            doTest = do_scomp_LinearComp;
        } else if(settings->testConstant <= bc_scomp_LempelZiv) {
            doTest = do_scomp_LempelZiv;
        } else if(settings->testConstant <= bc_sspectral_Fourier3) {
            doTest = do_sspectral_Fourier3;
        } else if(settings->testConstant <= bc_sstring_LongestHeadRun) {
            doTest = do_sstring_LongestHeadRun;
        } else if(settings->testConstant <= bc_sstring_PeriodsInStrings) {
            doTest = do_sstring_PeriodsInStrings;
        } else if(settings->testConstant <= bc_sstring_HammingWeight2) {
            doTest = do_sstring_HammingWeight2;
        } else if(settings->testConstant <= bc_sstring_HammingCorr) {
            doTest = do_sstring_HammingCorr;
        } else if(settings->testConstant <= bc_sstring_HammingIndep) {
            doTest = do_sstring_HammingIndep;
        } else if(settings->testConstant <= bc_sstring_Run) {
            doTest = do_sstring_Run;
        } else if(settings->testConstant <= bc_sstring_AutoCor) {
            doTest = do_sstring_AutoCor;
        } else {
            /* This won't ever happen */
            fprintf(stderr , "[ERROR] Unknown test in BigCrush used: %i\n" ,
                    settings->testConstant);
            return;
        }

        /* Running test */
        int rval = 0;
        int i = 0;
        for(i = 0; i < settings->pSamples ; ++i) {
            rval = doTest(settings , generator);
            if(rval != 0) {
                fprintf(stderr , "[ERROR] Test wasn't executed. Terminating.\n");
                return;
            }
        }
    }
    unif01_DeleteFileStreamGen(generator);
}

void tu01_execRabbitTest(const TestU01Settings * settings) {
    if(!settings)
        return;
    if(settings->testConstant > TCOUNT_RABBIT || settings->testConstant < 1) {
        fprintf(stderr , "[ERROR] Unknown test in Rabbit used: %i\n" ,
                settings->testConstant);
        return;
    }
    unif01_Gen * generator = unif01_CreateFileStreamGen(settings->inputFilePath);
    if(!generator) {
        fprintf(stderr , "[ERROR] FileStream Generator creation failed.\n");
        return;
    }

    int reps[TCOUNT_RABBIT + 1] = {0};
    reps[settings->testConstant] = settings->pSamples;
    bbattery_RepeatRabbit(generator , settings->bit_nb , reps);
}

void tu01_execAlphabitTest(const TestU01Settings * settings) {
    if(!settings)
        return;
    if(settings->testConstant > TCOUNT_ALPHABIT || settings->testConstant < 1) {
        fprintf(stderr , "[ERROR] Unknown test in Alphabit used: %i\n" ,
                settings->testConstant);
        return;
    }
    unif01_Gen * generator = unif01_CreateFileStreamGen(settings->inputFilePath);
    if(!generator) {
        fprintf(stderr , "[ERROR] FileStream Generator creation failed.\n");
        return;
    }

    int reps[TCOUNT_ALPHABIT + 1] = {0};
    reps[settings->testConstant] = settings->pSamples;
    bbattery_RepeatAlphabit(generator ,
                            settings->bit_nb ,
                            settings->bit_r , settings->bit_s , reps);
}

void tu01_execBlockAlphabitTest(const TestU01Settings * settings) {
    if(!settings)
        return;
    if(settings->testConstant > TCOUNT_ALPHABIT || settings->testConstant < 1) {
        fprintf(stderr , "[ERROR] Unknown test in Block Alphabit used: %i\n" ,
                settings->testConstant);
        return;
    }
    unif01_Gen * generator = unif01_CreateFileStreamGen(settings->inputFilePath);
    if(!generator) {
        fprintf(stderr , "[ERROR] FileStream Generator creation failed.\n");
        return;
    }

    int reps[TCOUNT_ALPHABIT + 1] = {0};
    reps[settings->testConstant] = settings->pSamples;

    if(settings->bit_w == -1) {
        /* Running Block Alphabit with all default w */
        int default_w[] = {1 , 2 , 4 , 8 , 16 , 32};
        int default_w_size = sizeof(default_w) / sizeof(int);
        int i = 0;
        for( ; i < default_w_size && default_w[i] <= settings->bit_s ;++i) {
            bbattery_RepeatBlockAlphabit(generator ,
                                         settings->bit_nb ,
                                         settings->bit_r ,
                                         settings->bit_s ,
                                         reps ,
                                         default_w[i]);
        }
    } else {
        /* Running Block Alphabit with one specific w */
        bbattery_RepeatBlockAlphabit(generator ,
                                     settings->bit_nb ,
                                     settings->bit_r ,
                                     settings->bit_s ,
                                     reps ,
                                     settings->bit_w);
    }
}

/* ================================================== */
/* ================================================== */
/* ==================== DO TESTS ==================== */
/* ================================================== */
/* ================================================== */

int do_smarsa_SerialOver(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 5) {
        fprintf(stderr , "[ERROR] Invalid parameters for smarsa_SerialOver.\n");
        return -1;
    }
    sres_Basic * result = sres_CreateBasic();

    int rval, r, t;
    long N, n, d;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[3] , &d);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[4] , &t);

    smarsa_SerialOver(generator , result ,
                      N , n , r , d , t);
    PVAL_TEXT_WRAP(print_1stlvl_statcoll_Collector(result->sVal1);)
    sres_DeleteBasic(result);
    return 0;
}

int do_smarsa_CollisionOver(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 5) {
        fprintf(stderr , "[ERROR] Invalid parameters for smarsa_CollisionOver.\n");
        return -1;
    }
    smarsa_Res * result = smarsa_CreateRes();

    int rval, r, t;
    long N, n, d;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[3] , &d);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[4] , &t);

    smarsa_CollisionOver(generator , result ,
                         N , n , r , d , t);
    PVAL_TEXT_WRAP(print_1stlvl_smarsa_Res(result);)
    smarsa_DeleteRes(result);
    return 0;
}

int do_smarsa_BirthdaySpacings(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 6) {
        fprintf(stderr , "[ERROR] Invalid parameters for smarsa_BirthdaySpacing.\n");
        return -1;
    }
    sres_Poisson * result = sres_CreatePoisson();

    int rval, r, t, p;
    long N, n, d;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[3] , &d);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[4] , &t);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[5] , &p);
    if(rval != 0) return -1;

    smarsa_BirthdaySpacings(generator , result ,
                            N , n , r , d , t , p);
    PVAL_TEXT_WRAP(print_1stlvl_sres_Poisson(result);)
    sres_DeletePoisson(result);
    return 0;
}

int do_snpair_ClosePairs(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 6) {
        fprintf(stderr , "[ERROR] Invalid parameters for snpair_ClosePairs.\n");
        return -1;
    }
    snpair_Res * result = snpair_CreateRes();

    int rval, r, t, p, m;
    long N, n;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[3] , &t);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[4] , &p);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[5] , &m);
    if(rval != 0) return -1;

    snpair_ClosePairs(generator , result ,
                      N , n , r , t , p , m);
    PVAL_TEXT_WRAP(print_1stlvl_snpair_Res(result);)
    snpair_DeleteRes(result);
    return 0;
}

int do_snpair_ClosePairsBitMatch(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 4) {
        fprintf(stderr , "[ERROR] Invalid parameters for snpair_ClosePairsBitMatch.\n");
        return -1;
    }
    snpair_Res * result = snpair_CreateRes();

    int rval, r, t;
    long N, n;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[3] , &t);
    if(rval != 0) return -1;

    snpair_ClosePairsBitMatch(generator , result ,
                              N , n , r , t);
    PVAL_TEXT_WRAP(print_1stlvl_statcoll_Collector(result->BitMax);)
    snpair_DeleteRes(result);
    return 0;
}

int do_sknuth_Collision(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 5) {
        fprintf(stderr , "[ERROR] Invalid parameters for sknuth_Collision.\n");
        return -1;
    }
    sknuth_Res2 * result = sknuth_CreateRes2();

    int rval, r, t;
    long N, n, d;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[3] , &d);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[4] , &t);
    if(rval != 0) return -1;

    sknuth_Collision(generator , result ,
                     N , n , r , d , t);
    PVAL_TEXT_WRAP(print_1stlvl_sknuth_Res2(result);)
    sknuth_DeleteRes2(result);
    return 0;
}

int do_sknuth_SimpPoker(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 5) {
        fprintf(stderr , "[ERROR] Invalid parameters for sknuth_SimpPoker.\n");
        return -1;
    }
    sres_Chi2 * result = sres_CreateChi2();

    int rval, r, d, k;
    long N, n;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[3] , &d);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[4] , &k);
    if(rval != 0) return -1;

    sknuth_SimpPoker(generator , result ,
                     N , n , r , d , k);
    PVAL_TEXT_WRAP(print_1stlvl_sres_Chi2(result);)
    sres_DeleteChi2(result);
    return 0;
}

int do_sknuth_CouponCollector(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 4) {
        fprintf(stderr , "[ERROR] Invalid parameters for sknuth_CouponCollector.\n");
        return -1;
    }
    sres_Chi2 * result = sres_CreateChi2();

    int rval, r, d;
    long N, n;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[3] , &d);
    if(rval != 0) return -1;

    sknuth_CouponCollector(generator , result ,
                           N , n , r , d);
    PVAL_TEXT_WRAP(print_1stlvl_sres_Chi2(result);)
    sres_DeleteChi2(result);
    return 0;
}

int do_sknuth_Gap(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 5) {
        fprintf(stderr , "[ERROR] Invalid parameters for sknuth_Gap.\n");
        return -1;
    }
    sres_Chi2 * result = sres_CreateChi2();

    int rval, r;
    long N, n;
    double Alpha, Beta;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    Alpha = settings->parameters[3];
    Beta = settings->parameters[4];

    sknuth_Gap(generator , result ,
               N , n , r , Alpha , Beta);
    PVAL_TEXT_WRAP(print_1stlvl_statcoll_Collector(result->sVal1);)
    sres_DeleteChi2(result);
    return 0;
}

int do_sknuth_Run(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 4) {
        fprintf(stderr , "[ERROR] Invalid parameters for sknuth_Run.\n");
        return -1;
    }
    sres_Chi2 * result = sres_CreateChi2();

    int rval, r;
    long N, n;
    lebool Up;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[3] , &Up);
    if(Up != 0 && Up != 1) {
        fprintf(stderr , "[ERROR] Invalid parameters for sknuth_Run.\n");
        return -1;
    }

    sknuth_Run(generator , result ,
               N , n , r , Up);
    PVAL_TEXT_WRAP(print_1stlvl_sres_Chi2(result);)
    sres_DeleteChi2(result);
    return 0;
}

int do_sknuth_Permutation(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 4) {
        fprintf(stderr , "[ERROR] Invalid parameters for sknuth_Permutation.\n");
        return -1;
    }
    sres_Chi2 * result = sres_CreateChi2();

    int rval, r, t;
    long N, n;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[3] , &t);
    if(rval != 0) return -1;

    sknuth_Permutation(generator , result ,
                       N , n , r , t);
    PVAL_TEXT_WRAP(print_1stlvl_statcoll_Collector(result->sVal1);)
    sres_DeleteChi2(result);
    return 0;
}

int do_sknuth_CollisionPermut(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 4) {
        fprintf(stderr , "[ERROR] Invalid parameters for sknuth_CollisionPermut.\n");
        return -1;
    }
    sknuth_Res2 * result = sknuth_CreateRes2();

    int rval, r, t;
    long N, n;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[3] , &t);
    if(rval != 0) return -1;

    sknuth_CollisionPermut(generator , result ,
                           N , n , r , t);
    PVAL_TEXT_WRAP(print_1stlvl_sknuth_Res2(result);)
    sknuth_DeleteRes2(result);
    return 0;
}

int do_sknuth_MaxOft(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 5) {
        fprintf(stderr , "[ERROR] Invalid parameters for sknuth_MaxOft.\n");
        return -1;
    }
    sknuth_Res1 * result = sknuth_CreateRes1();

    int rval, r, d, t;
    long N, n;

    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[3] , &d);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[4] , &t);
    if(rval != 0) return -1;

    sknuth_MaxOft(generator, result ,
                  N , n , r , d , t);
    PVAL_TEXT_WRAP(print_1stlvl_sknuth_Res1(result);)
    sknuth_DeleteRes1(result);
    return 0;
}

int do_svaria_SampleProd(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 4) {
        fprintf(stderr , "[ERROR] Invalid parameters for svaria_SampleProd.\n");
        return -1;
    }
    sres_Basic * result = sres_CreateBasic();

    int rval, r, t;
    long N, n;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[3] , &t);
    if(rval != 0) return -1;

    svaria_SampleProd(generator , result ,
                      N , n , r , t);
    PVAL_TEXT_WRAP(print_1stlvl_sres_Basic(result);)
    sres_DeleteBasic(result);
    return 0;
}

int do_svaria_SampleMean(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 3) {
        fprintf(stderr , "[ERROR] Invalid parameters for svaria_SampleMean.\n");
        return -1;
    }
    sres_Basic * result = sres_CreateBasic();

    int rval, r;
    long N, n;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;

    svaria_SampleMean(generator , result ,
                      N , n , r);
    PVAL_TEXT_WRAP(print_1stlvl_sres_Basic(result);)
    sres_DeleteBasic(result);
    return 0;
}

int do_svaria_SampleCorr(const TestU01Settings *settings, unif01_Gen *generator) {
    if(settings->parametersCount != 4) {
        fprintf(stderr , "[ERROR] Invalid parameters for svaria_SampleCorr.\n");
        return -1;
    }
    sres_Basic * result = sres_CreateBasic();

    int rval, r, k;
    long N, n;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[3] , &k);
    if(rval != 0) return -1;

    svaria_SampleCorr(generator , result ,
                      N , n , r , k);
    PVAL_TEXT_WRAP(print_1stlvl_sres_Basic(result);)
    sres_DeleteBasic(result);
    return 0;
}


int do_svaria_AppearanceSpacings(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 6) {
        fprintf(stderr , "[ERROR] Invalid parameters for svaria_AppearanceSpacings.\n");
        return -1;
    }
    sres_Basic * result = sres_CreateBasic();

    int rval, r, s, L;
    long N, Q, K;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &Q);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[2] , &K);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[3] , &r);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[4] , &s);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[5] , &L);
    if(rval != 0) return -1;

    svaria_AppearanceSpacings(generator , result ,
                              N , Q , K , r , s , L);
    PVAL_TEXT_WRAP(print_1stlvl_sres_Basic(result);)
    sres_DeleteBasic(result);
    return 0;
}


int do_svaria_WeightDistrib(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 6) {
        fprintf(stderr , "[ERROR] Invalid parameters for svaria_WeightDistrib.\n");
        return -1;
    }
    sres_Chi2 * result = sres_CreateChi2();

    int rval, r;
    long N, n, k;
    double alpha, beta;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[3] , &k);
    if(rval != 0) return -1;
    alpha = settings->parameters[4];
    beta = settings->parameters[5];

    svaria_WeightDistrib(generator , result ,
                         N , n , r , k , alpha , beta);
    PVAL_TEXT_WRAP(print_1stlvl_sres_Chi2(result);)
    sres_DeleteChi2(result);
    return 0;
}


int do_svaria_SumCollector(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 4) {
        fprintf(stderr , "[ERROR] Invalid parameters for svaria_SumCollector.\n");
        return -1;
    }
    sres_Chi2 * result = sres_CreateChi2();
    int rval, r;
    long N, n;
    double g;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    g = settings->parameters[3];

    svaria_SumCollector(generator , result ,
                        N , n , r , g);
    PVAL_TEXT_WRAP(print_1stlvl_sres_Chi2(result);)
    sres_DeleteChi2(result);
    return 0;
}


int do_smarsa_MatrixRank(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 6) {
        fprintf(stderr , "[ERROR] Invalid parameters for smarsa_MatrixRank.\n");
        return -1;
    }
    sres_Chi2 * result = sres_CreateChi2();

    int rval, r, s, L, k;
    long N, n;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[3] , &s);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[4] , &L);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[5] , &k);
    if(rval != 0) return -1;

    smarsa_MatrixRank(generator , result ,
                      N , n , r , s , L , k);
    PVAL_TEXT_WRAP(print_1stlvl_sres_Chi2(result);)
    sres_DeleteChi2(result);
    return 0;
}


int do_smarsa_Savir2(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 5) {
        fprintf(stderr , "[ERROR] Invalid parameters for smarsa_Savir2.\n");
        return -1;
    }
    sres_Chi2 * result = sres_CreateChi2();

    int rval, r, t;
    long N, n, m;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[3] , &m);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[4] , &t);

    smarsa_Savir2(generator , result ,
                  N , n , r , m , t);
    PVAL_TEXT_WRAP(print_1stlvl_sres_Chi2(result);)
    sres_DeleteChi2(result);
    return 0;
}


int do_smarsa_GCD(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 4) {
        fprintf(stderr , "[ERROR] Invalid parameters for smarsa_GCD.\n");
        return -1;
    }
    smarsa_Res2 * result = smarsa_CreateRes2();

    int rval, r, s;
    long N, n;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[3] , &s);
    if(rval != 0) return -1;

    smarsa_GCD(generator , result ,
               N , n , r , s);
    PVAL_TEXT_WRAP(print_1stlvl_smarsa_Res2(result);)
    smarsa_DeleteRes2(result);
    return 0;
}


int do_swalk_RandomWalk1(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 6) {
        fprintf(stderr , "[ERROR] Invalid parameters for swalk_RandomWalk1.\n");
        return -1;
    }
    swalk_Res * result = swalk_CreateRes();

    int rval, r, s;
    long N, n, L0, L1;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[3] , &s);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[4] , &L0);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[5] , &L1);
    if(rval != 0) return -1;

    swalk_RandomWalk1(generator , result ,
                      N , n , r , s , L0 , L1);
    PVAL_TEXT_WRAP(print_1stlvl_swalk_Res(result);)
    swalk_DeleteRes(result);
    return 0;
}


int do_scomp_LinearComp(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 4) {
        fprintf(stderr , "[ERROR] Invalid parameters for scomp_LinearComp.\n");
        return -1;
    }
    scomp_Res * result = scomp_CreateRes();

    int rval, r, s;
    long N, n;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[3] , &s);
    if(rval != 0) return -1;

    scomp_LinearComp(generator , result ,
                     N , n , r , s);
    PVAL_TEXT_WRAP(print_1stlvl_scomp_Res(result);)
    scomp_DeleteRes(result);
    return 0;
}


int do_scomp_LempelZiv(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 4) {
        fprintf(stderr , "[ERROR] Invalid parameters for scomp_LempelZiv.\n");
        return -1;
    }
    sres_Basic * result = sres_CreateBasic();


    int rval, k, r, s;
    long N;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[1] , &k);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[3] , &s);
    if(rval != 0) return -1;

    scomp_LempelZiv(generator , result ,
                    N , k , r , s);
    PVAL_TEXT_WRAP(print_1stlvl_sres_Basic(result);)
    sres_DeleteBasic(result);
    return 0;
}


int do_sspectral_Fourier3(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 4) {
        fprintf(stderr , "[ERROR] Invalid parameters for sspectral_Fourier3.\n");
        return -1;
    }
    sspectral_Res * result = sspectral_CreateRes();

    int rval, k, r, s;
    long N;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[1] , &k);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[3] , &s);
    if(rval != 0) return -1;

    sspectral_Fourier3(generator , result ,
                       N , k , r , s);
    PVAL_TEXT_WRAP(print_1stlvl_sspectral_Res(result);)
    sspectral_DeleteRes(result);
    return 0;
}


int do_sstring_LongestHeadRun(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 5) {
        fprintf(stderr , "[ERROR] Invalid parameters for sstring_LongestHeadRun.\n");
        return -1;
    }
    sstring_Res2 * result = sstring_CreateRes2();

    int rval, r, s;
    long N , n , L;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[3] , &s);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[4] , &L);
    if(rval != 0) return -1;

    sstring_LongestHeadRun(generator , result ,
                           N , n , r , s , L);
    PVAL_TEXT_WRAP(print_1stlvl_sstring_Res2(result);)
    sstring_DeleteRes2(result);
    return 0;
}


int do_sstring_PeriodsInStrings(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 4) {
        fprintf(stderr , "[ERROR] Invalid parameters for sstring_PeriodsInStrings.\n");
        return -1;
    }
    sres_Chi2 * result = sres_CreateChi2();

    int rval, r, s;
    long N , n;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[3] , &s);
    if(rval != 0) return -1;

    sstring_PeriodsInStrings(generator , result ,
                             N , n , r , s);
    PVAL_TEXT_WRAP(print_1stlvl_sres_Chi2(result);)
    sres_DeleteChi2(result);
    return 0;
}


int do_sstring_HammingWeight2(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 5) {
        fprintf(stderr , "[ERROR] Invalid parameters for sstring_HammingWeight2.\n");
        return -1;
    }
    sres_Basic * result = sres_CreateBasic();

    int rval, r, s;
    long N , n , L;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[3] , &s);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[4] , &L);
    if(rval != 0) return -1;

    sstring_HammingWeight2(generator , result ,
                           N , n , r , s , L);
    PVAL_TEXT_WRAP(print_1stlvl_sres_Basic(result);)
    sres_DeleteBasic(result);
    return  0;
}


int do_sstring_HammingCorr(const TestU01Settings *settings, unif01_Gen *generator) {
    if(settings->parametersCount != 5) {
        fprintf(stderr , "[ERROR] Invalid parameters for sstring_HammingCorr.\n");
        return -1;
    }
    sstring_Res * result = sstring_CreateRes();

    int rval, r, s, L;
    long N , n;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[3] , &s);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[4] , &L);
    if(rval != 0) return -1;

    sstring_HammingCorr(generator , result ,
                        N , n , r , s , L);
    PVAL_TEXT_WRAP(print_1stlvl_sstring_Res(result);)
    sstring_DeleteRes(result);
    return  0;
}


int do_sstring_HammingIndep(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 6) {
        fprintf(stderr , "[ERROR] Invalid parameters for sstring_HammingIndep.\n");
        return -1;
    }
    sstring_Res * result = sstring_CreateRes();

    int rval, r, s, L, d;
    long N, n;

    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[3] , &s);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[4] , &L);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[5] , &d);
    if(rval != 0) return -1;

    sstring_HammingIndep(generator , result ,
                         N , n , r , s , L , d);
    PVAL_TEXT_WRAP(print_1stlvl_sstring_Res(result);)
    sstring_DeleteRes(result);
    return  0;
}


int do_sstring_Run(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 4) {
        fprintf(stderr , "[ERROR] Invalid parameters for sstring_Run.\n");
        return -1;
    }
    sstring_Res3 * result = sstring_CreateRes3();

    int rval, r, s;
    long N, n;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[3] , &s);

    sstring_Run(generator , result ,
                N , n , r , s);
    PVAL_TEXT_WRAP(print_1stlvl_sstring_Res3(result);)
    sstring_DeleteRes3(result);
    return  0;
}


int do_sstring_AutoCor(const TestU01Settings * settings, unif01_Gen * generator) {
    if(settings->parametersCount != 5) {
        fprintf(stderr , "[ERROR] Invalid parameters for sstring_AutoCor.\n");
        return -1;
    }
    sres_Basic * result = sres_CreateBasic();

    int rval, r, s, d;
    long N, n;
    rval = u_dtolong(settings->parameters[0] , &N);
    if(rval != 0) return -1;
    rval = u_dtolong(settings->parameters[1] , &n);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[2] , &r);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[3] , &s);
    if(rval != 0) return -1;
    rval = u_dtoi(settings->parameters[4] , &d);

    sstring_AutoCor(generator , result ,
                    N , n , r , s , d);
    PVAL_TEXT_WRAP(print_1stlvl_sres_Basic(result);)
    sres_DeleteBasic(result);
    return  0;
}

void print_double(double num) {
    printf("%.8f\n", num);
}

void print_1stlvl_statcoll_Collector(const statcoll_Collector * result) {
    int i;
    for(i = 1; i <= result->NObs; ++i)
        print_double(result->V[i]);
    print_double(-1);
}

void print_1stlvl_sres_Basic(const sres_Basic * result) {
    print_1stlvl_statcoll_Collector(result->pVal1);
}

void print_1stlvl_sres_Chi2(const sres_Chi2 * result) {
    print_1stlvl_statcoll_Collector(result->pVal1);
}

void print_1stlvl_sknuth_Res1(const sknuth_Res1 * result) {
    print_1stlvl_sres_Basic(result->Bas);
    print_1stlvl_sres_Chi2(result->Chi);
}

void print_1stlvl_snpair_Res(const snpair_Res * result) {
    print_1stlvl_statcoll_Collector(result->TheWn);
    print_1stlvl_statcoll_Collector(result->ThepValAD);
}

void print_1stlvl_smarsa_Res(const smarsa_Res * result) {
    print_1stlvl_sres_Poisson(result->Pois);
}

void print_1stlvl_sres_Poisson(const sres_Poisson * result) {
    print_1stlvl_statcoll_Collector(result->sVal1);
}

void print_1stlvl_sknuth_Res2(const sknuth_Res2 * result) {
    print_1stlvl_sres_Poisson(result->Pois);
}

void print_1stlvl_smarsa_Res2(const smarsa_Res2 * result) {
    print_1stlvl_statcoll_Collector(result->GCD->pVal1);
    print_1stlvl_statcoll_Collector(result->NumIter->sVal1);
}

void print_1stlvl_swalk_Res(const swalk_Res * result) {
    int i;
    for(i = 0; i <= result->imax; ++i) {
        print_1stlvl_sres_Chi2(result->H[i]);
        print_1stlvl_sres_Chi2(result->M[i]);
        print_1stlvl_sres_Chi2(result->J[i]);
        print_1stlvl_sres_Chi2(result->R[i]);
        print_1stlvl_sres_Chi2(result->C[i]);
        print_double(-1);
    }
}

void print_1stlvl_scomp_Res(const scomp_Res * result) {
    print_1stlvl_sres_Basic(result->JumpNum);
    print_1stlvl_sres_Chi2(result->JumpSize);
}

void print_1stlvl_sspectral_Res(const sspectral_Res * result) {
    print_1stlvl_sres_Basic(result->Bas);
}

void print_1stlvl_sstring_Res2(const sstring_Res2 * result) {
    print_1stlvl_sres_Chi2(result->Chi);
    print_1stlvl_sres_Disc(result->Disc);
}

void print_1stlvl_sres_Disc(const sres_Disc * result) {
    print_1stlvl_statcoll_Collector(result->sVal1);
}

void print_1stlvl_sstring_Res(const sstring_Res * result) {
    print_1stlvl_sres_Basic(result->Bas);
}

void print_1stlvl_sstring_Res3(const sstring_Res3 * result) {
    print_1stlvl_sres_Basic(result->NBits);
    print_1stlvl_sres_Chi2(result->NRuns);
}
