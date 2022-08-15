#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FileStreamGen.h"
#include "Utils.h"

#include "bbattery.h"
#include "sres.h"
#include "smultin.h"
#include "sknuth.h"
#include "smarsa.h"
#include "snpair.h"
#include "svaria.h"
#include "sstring.h"
#include "swalk.h"
#include "scomp.h"
#include "sspectral.h"
#include "swrite.h"
#include "sres.h"
#include "gofw.h"

typedef struct {
    int batteryMode;
    int testConstant;
    int pSamples;
    int parametersCount;
    double * parameters;
    double bit_nb;
    int bit_r;
    int bit_s;
    int bit_w;
    char * inputFilePath;
} TestU01Settings;

TestU01Settings * initSettings(int argc , char * argv[]);

void deleteSettings(TestU01Settings * settings);

void tu01_printUsage();

void executeTest(const TestU01Settings * settings);

void tu01_execSmallCrushTest(const TestU01Settings * settings);

void tu01_execCrushTest(const TestU01Settings * settings);

void tu01_execBigCrushTest(const TestU01Settings * settings);

void tu01_execRabbitTest(const TestU01Settings * settings);

void tu01_execAlphabitTest(const TestU01Settings * settings);

void tu01_execBlockAlphabitTest(const TestU01Settings * settings);

int do_smarsa_SerialOver(const TestU01Settings * settings , unif01_Gen * generator);

int do_smarsa_CollisionOver(const TestU01Settings * settings , unif01_Gen * generator);

int do_smarsa_BirthdaySpacings(const TestU01Settings * settings , unif01_Gen * generator);

int do_snpair_ClosePairs(const TestU01Settings * settings , unif01_Gen * generator);

int do_snpair_ClosePairsBitMatch(const TestU01Settings * settings , unif01_Gen * generator);

int do_sknuth_Collision(const TestU01Settings * settings , unif01_Gen * generator);

int do_sknuth_SimpPoker(const TestU01Settings * settings , unif01_Gen * generator);

int do_sknuth_CouponCollector(const TestU01Settings * settings , unif01_Gen * generator);

int do_sknuth_Gap(const TestU01Settings * settings , unif01_Gen * generator);

int do_sknuth_Run(const TestU01Settings * settings , unif01_Gen * generator);

int do_sknuth_Permutation(const TestU01Settings * settings , unif01_Gen * generator);

int do_sknuth_CollisionPermut(const TestU01Settings * settings , unif01_Gen * generator);

int do_sknuth_MaxOft(const TestU01Settings * settings , unif01_Gen * generator);

int do_svaria_SampleProd(const TestU01Settings * settings , unif01_Gen * generator);

int do_svaria_SampleMean(const TestU01Settings * settings , unif01_Gen * generator);

int do_svaria_SampleCorr(const TestU01Settings * settings , unif01_Gen * generator);

int do_svaria_AppearanceSpacings(const TestU01Settings * settings , unif01_Gen * generator);

int do_svaria_WeightDistrib(const TestU01Settings * settings , unif01_Gen * generator);

int do_svaria_SumCollector(const TestU01Settings * settings , unif01_Gen * generator);

int do_smarsa_MatrixRank(const TestU01Settings * settings , unif01_Gen * generator);

int do_smarsa_Savir2(const TestU01Settings * settings , unif01_Gen * generator);

int do_smarsa_GCD(const TestU01Settings * settings , unif01_Gen * generator);

int do_swalk_RandomWalk1(const TestU01Settings * settings , unif01_Gen * generator);

int do_scomp_LinearComp(const TestU01Settings * settings , unif01_Gen * generator);

int do_scomp_LempelZiv(const TestU01Settings * settings , unif01_Gen * generator);

int do_sspectral_Fourier3(const TestU01Settings * settings , unif01_Gen * generator);

int do_sstring_LongestHeadRun(const TestU01Settings * settings , unif01_Gen * generator);

int do_sstring_PeriodsInStrings(const TestU01Settings * settings , unif01_Gen * generator);

int do_sstring_HammingWeight2(const TestU01Settings * settings , unif01_Gen * generator);

int do_sstring_HammingCorr(const TestU01Settings * settings , unif01_Gen * generator);

int do_sstring_HammingIndep(const TestU01Settings * settings , unif01_Gen * generator);

int do_sstring_Run(const TestU01Settings * settings , unif01_Gen * generator);

int do_sstring_AutoCor(const TestU01Settings * settings , unif01_Gen * generator);

void print_double(double num);

void print_1stlvl_statcoll_Collector(const statcoll_Collector * result);

void print_1stlvl_sres_Basic(const sres_Basic * result);

void print_1stlvl_sres_Chi2(const sres_Chi2 * result);

void print_1stlvl_sres_Disc(const sres_Disc * result);

void print_1stlvl_sknuth_Res1(const sknuth_Res1 * result);

void print_1stlvl_sknuth_Res2(const sknuth_Res2 * result);

void print_1stlvl_snpair_Res(const snpair_Res * result);

void print_1stlvl_smarsa_Res(const smarsa_Res * result);

void print_1stlvl_smarsa_Res2(const smarsa_Res2 * result);

void print_1stlvl_sres_Poisson(const sres_Poisson * result);

void print_1stlvl_swalk_Res(const swalk_Res * result);

void print_1stlvl_scomp_Res(const scomp_Res * result);

void print_1stlvl_sspectral_Res(const sspectral_Res * result);

void print_1stlvl_sstring_Res(const sstring_Res * result);

void print_1stlvl_sstring_Res2(const sstring_Res2 * result);

void print_1stlvl_sstring_Res3(const sstring_Res3 * result);
