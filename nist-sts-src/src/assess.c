
/* --------------------------------------------------------------------------
   Title       :  The NIST Statistical Test Suite

   Date        :  December 1999

   Programmer  :  Juan Soto

   Summary     :  For use in the evaluation of the randomness of bitstreams
                  produced by cryptographic random number generators.

   Package     :  Version 1.0

   Copyright   :  (c) 1999 by the National Institute Of Standards & Technology

   History     :  Version 1.0 by J. Soto, October 1999
                  Revised by J. Soto, November 1999
                  Revised by Larry Bassham, March 2008

   Keywords    :  Pseudorandom Number Generator (PRNG), Randomness, Statistical 
                  Tests, Complementary Error functions, Incomplete Gamma 
                  Function, Random Walks, Rank, Fast Fourier Transform, 
                  Template, Cryptographically Secure PRNG (CSPRNG),
                  Approximate Entropy (ApEn), Secure Hash Algorithm (SHA-1), 
                  Blum-Blum-Shub (BBS) CSPRNG, Micali-Schnorr (MS) CSPRNG, 

   Source      :  David Banks, Elaine Barker, James Dray, Allen Heckert, 
                  Stefan Leigh, Mark Levenson, James Nechvatal, Andrew Rukhin, 
                  Miles Smid, Juan Soto, Mark Vangel, and San Vo.

   Technical
   Assistance  :  Larry Bassham, Ron Boisvert, James Filliben, Daniel Lozier,
                  and Bert Rust.

   Warning     :  Portability Issues.

   Limitation  :  Amount of memory allocated for workspace.

   Restrictions:  Permission to use, copy, and modify this software without 
                  fee is hereby granted, provided that this entire notice is 
                  included in all copies of any software which is or includes
                  a copy or modification of this software and in all copies 
                  of the supporting documentation for such software.
   -------------------------------------------------------------------------- */
#include "../include/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../include/decls.h"
#include "../include/cephes.h"  
#include "../include/utilities.h"


void	partitionResultFile(int numOfFiles, int numOfSequences, int option, int testNameID);

#ifdef FILE_OUTPUT
void	postProcessResults(int option);
int		computeMetrics(char *s, int test);
#endif

#ifdef KS
void	postProcessResults();
int	process_pvals_single_test(double* A, int test);
#endif

int		cmp(const double *a, const double *b);


#if  defined(FILE_OUTPUT) || defined(KS)
int
main(int argc, char *argv[])
{
#ifdef FILE_OUTPUT
	int		i;
#endif
	int		option;			/* TEMPLATE LENGTH/STREAM LENGTH/GENERATOR*/
	char	*streamFile;	/* STREAM FILENAME     */
	

	if ( (argc != 2  && argc != 3) || (argc==3 && strcmp(argv[2],"-fast") && strcmp(argv[2],"--fast")) ) {
		printf("Usage: %s <stream length> [-fast]\n", argv[0]);
		printf("   <stream length> is the length of the individual bit stream(s) to be processed\n");
		printf("   -fast           use the faster alternative implementation of tests\n");
		return 0;
	}

	if(argc==3 && (!strcmp(argv[2],"-fast") || !strcmp(argv[2],"--fast")))
		tp.fast=1;
	else
		tp.fast=0;

	tp.n = atoi(argv[1]);
	tp.blockFrequencyBlockLength = 128;
	tp.nonOverlappingTemplateBlockLength = 9;
	tp.overlappingTemplateBlockLength = 9;
	tp.approximateEntropyBlockLength = 10;
	tp.serialBlockLength = 16;
	tp.linearComplexitySequenceLength = 500;
	tp.numOfBitStreams = 1;
	option = generatorOptions(&streamFile);
	chooseTests();
	fixParameters();
	

	openOutputStreams(option);
	invokeTestSuite(option, streamFile);
	fclose(freqfp);

#ifdef FILE_OUTPUT
	for( i=1; i<=NUMOFTESTS; i++ ) {
		if ( stats[i] != NULL )
			fclose(stats[i]);
		if ( results[i] != NULL )
			fclose(results[i]);
	}
	if ( (testVector[0] == 1) || (testVector[TEST_CUSUM] == 1) ) 
		partitionResultFile(2, tp.numOfBitStreams, option, TEST_CUSUM);
	if ( (testVector[0] == 1) || (testVector[TEST_NONPERIODIC] == 1) ) 
		partitionResultFile(MAXNUMOFTEMPLATES, tp.numOfBitStreams, option, TEST_NONPERIODIC);
	if ( (testVector[0] == 1) || (testVector[TEST_RND_EXCURSION] == 1) )
		partitionResultFile(8, tp.numOfBitStreams, option, TEST_RND_EXCURSION);
	if ( (testVector[0] == 1) || (testVector[TEST_RND_EXCURSION_VAR] == 1) )
		partitionResultFile(18, tp.numOfBitStreams, option, TEST_RND_EXCURSION_VAR);
	if ( (testVector[0] == 1) || (testVector[TEST_SERIAL] == 1) )
		partitionResultFile(2, tp.numOfBitStreams, option, TEST_SERIAL);
	fprintf(summary, "------------------------------------------------------------------------------\n");
	fprintf(summary, "RESULTS FOR THE UNIFORMITY OF P-VALUES AND THE PROPORTION OF PASSING SEQUENCES\n");
	fprintf(summary, "------------------------------------------------------------------------------\n");
	fprintf(summary, "   generator is <%s>\n", streamFile);
	fprintf(summary, "------------------------------------------------------------------------------\n");
	fprintf(summary, " C1  C2  C3  C4  C5  C6  C7  C8  C9 C10  P-VALUE  PROPORTION  STATISTICAL TEST\n");
	fprintf(summary, "------------------------------------------------------------------------------\n");
	postProcessResults(option);
#endif

#ifdef KS
	fprintf(summary, "------------------------------------------------------------------------------\n");
	fprintf(summary, "RESULTS FOR THE UNIFORMITY OF P-VALUES AND THE PROPORTION OF PASSING SEQUENCES\n");
	fprintf(summary, "------------------------------------------------------------------------------\n");
	fprintf(summary, "   generator is <%s>\n", streamFile);
	fprintf(summary, "-------------------------------------------------------------------------------------------\n");
	fprintf(summary, " C1  C2  C3  C4  C5  C6  C7  C8  C9 C10  P-VALUE  P-value(KS) PROPORTION  STATISTICAL TEST \n");
	fprintf(summary, "-------------------------------------------------------------------------------------------\n");
	postProcessResults();
#endif
	fclose(summary);

	return 1;
}
#endif

void
partitionResultFile(int numOfFiles, int numOfSequences, int option, int testNameID)
{ 
	int		i, k, m, j, start, end, num, numread;
	float	c;
	FILE	**fp = (FILE **)calloc(numOfFiles+1, sizeof(FILE *));
	int		*results = (int *)calloc(numOfFiles, sizeof(int *));
	char	*s[MAXFILESPERMITTEDFORPARTITION];
	char	resultsDir[200];

	for ( i=0; i<MAXFILESPERMITTEDFORPARTITION; i++ )
		s[i] = (char*)calloc(200, sizeof(char));
	
	sprintf(resultsDir, "experiments/%s/%s/results.txt", generatorDir[option], testNames[testNameID]);
	
	if ( (fp[numOfFiles] = fopen(resultsDir, "r")) == NULL ) {
		printf("%s", resultsDir);
		printf(" -- file not found. Exiting program.\n");
		exit(-1);
	}
	
	for ( i=0; i<numOfFiles; i++ ) {
		if ( i < 10 )
			sprintf(s[i], "experiments/%s/%s/data%1d.txt", generatorDir[option], testNames[testNameID], i+1);
		else if (i < 100)
			sprintf(s[i], "experiments/%s/%s/data%2d.txt", generatorDir[option], testNames[testNameID], i+1);
		else
			sprintf(s[i], "experiments/%s/%s/data%3d.txt", generatorDir[option], testNames[testNameID], i+1);
	}
	numread = 0;
	m = numOfFiles/20;
	if ( (numOfFiles%20) != 0 )
		m++;
	for ( i=0; i<numOfFiles; i++ ) {
		if ( (fp[i] = fopen(s[i], "w")) == NULL ) {
			printf("%s", s[i]);
			printf(" -- file not found. Exiting program.\n");
			exit(-1);
		}
		fclose(fp[i]);
	}
	for ( num=0; num<numOfSequences; num++ ) {
		for ( k=0; k<m; k++ ) { 			/* FOR EACH BATCH */
			
			start = k*20;		/* BOUNDARY SEGMENTS */
			end   = k*20+19;
			if ( k == (m-1) )
				end = numOfFiles-1;
			
			for ( i=start; i<=end; i++ ) {		/* OPEN FILE */
				if ( (fp[i] = fopen(s[i], "a")) == NULL ) {
					printf("%s", s[i]);
					printf(" -- file not found. Exiting program.\n");
					exit(-1);
				}
			}
			
			for ( j=start; j<=end; j++ ) {		/* POPULATE FILE */
				fscanf(fp[numOfFiles], "%f", &c);
				fprintf(fp[j], "%f\n", c);
				numread++;
			}

			for ( i=start; i<=end; i++ )		/* CLOSE FILE */
				fclose(fp[i]);
		}
	}
	fclose(fp[numOfFiles]);
	for ( i=0; i<MAXFILESPERMITTEDFORPARTITION; i++ )
		free(s[i]);

	return;
}

int
cmp(const double *a, const double *b)
{
	if ( *a < *b )
		return -1;
	if ( *a == *b )
		return 0;
	return 1;
}

#ifdef FILE_OUTPUT
void
postProcessResults(int option)
{
	int		i, k, randomExcursionSampleSize, generalSampleSize;
	int		case1, case2, numOfFiles = 2;
	double	passRate;
	char	s[200];
	
	for ( i=1; i<=NUMOFTESTS; i++ ) {		// FOR EACH TEST
		if ( testVector[i] ) {
			// SPECIAL CASES -- HANDLING MULTIPLE FILES FOR A SINGLE TEST
			if ( ((i == TEST_CUSUM) && testVector[TEST_CUSUM] ) ||
				 ((i == TEST_NONPERIODIC) && testVector[TEST_NONPERIODIC] ) ||
				 ((i == TEST_RND_EXCURSION) && testVector[TEST_RND_EXCURSION]) ||
				 ((i == TEST_RND_EXCURSION_VAR) && testVector[TEST_RND_EXCURSION_VAR]) || 
				 ((i == TEST_SERIAL) && testVector[TEST_SERIAL]) ) {
				
				if ( (i == TEST_NONPERIODIC) && testVector[TEST_NONPERIODIC] )  
					numOfFiles = MAXNUMOFTEMPLATES;
				else if ( (i == TEST_RND_EXCURSION) && testVector[TEST_RND_EXCURSION] ) 
					numOfFiles = 8;
				else if ( (i == TEST_RND_EXCURSION_VAR) && testVector[TEST_RND_EXCURSION_VAR] ) 
					numOfFiles = 18;
				else
					numOfFiles = 2;
				for ( k=0; k<numOfFiles; k++ ) {
					if ( k < 10 )
						sprintf(s, "experiments/%s/%s/data%1d.txt", generatorDir[option], testNames[i], k+1);
					else if ( k < 100 )
						sprintf(s, "experiments/%s/%s/data%2d.txt", generatorDir[option], testNames[i], k+1);
					else
						sprintf(s, "experiments/%s/%s/data%3d.txt", generatorDir[option], testNames[i], k+1);
					if ( (i == TEST_RND_EXCURSION) || (i == TEST_RND_EXCURSION_VAR) ) 
						randomExcursionSampleSize = computeMetrics(s,i);
					else
						generalSampleSize = computeMetrics(s,i);
				}
			}
			else {
				sprintf(s, "experiments/%s/%s/results.txt", generatorDir[option], testNames[i]);
				generalSampleSize = computeMetrics(s,i);
			}

		}
	}

	fprintf(summary, "\n\n- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
	case1 = 0;
	case2 = 0;
	if ( testVector[TEST_RND_EXCURSION] || testVector[TEST_RND_EXCURSION_VAR] ) 
		case2 = 1;
	for ( i=1; i<=NUMOFTESTS; i++ ) {
		if ( testVector[i] && (i != TEST_RND_EXCURSION) && (i != TEST_RND_EXCURSION_VAR) ) {
			case1 = 1;
			break;
		}
	}
	if ( case1 ) {
		if ( generalSampleSize == 0 ) {
			fprintf(summary, "The minimum pass rate for each statistical test with the exception of the\n");
			fprintf(summary, "random excursion (variant) test is undefined.\n\n");
		}
		else {
			passRate = 0.99-2.6*sqrt(0.01*(1.0-ALPHA)/(double)generalSampleSize);
			fprintf(summary, "The minimum pass rate for each statistical test with the exception of the\n");
			fprintf(summary, "random excursion (variant) test is approximately = %f for a\n", generalSampleSize ? passRate : 0.0);
			fprintf(summary, "sample size = %d binary sequences.\n\n", generalSampleSize);
		}
	}
	if ( case2 ) {
		if ( randomExcursionSampleSize == 0 )
			fprintf(summary, "The minimum pass rate for the random excursion (variant) test is undefined.\n\n");
		else {
			passRate = 0.99-2.6*sqrt(0.01*(1.0-ALPHA)/(double)randomExcursionSampleSize);
			fprintf(summary, "The minimum pass rate for the random excursion (variant) test\n");
			fprintf(summary, "is approximately %f for a sample size = %d binary sequences.\n\n", passRate, randomExcursionSampleSize);
		}
	}
	fprintf(summary, "For further guidelines construct a probability table using the MAPLE program\n");
	fprintf(summary, "provided in the addendum section of the documentation.\n");
	fprintf(summary, "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
}

int
computeMetrics(char *s, int test)
{
	int		j, pos, count, sampleSize, expCount;
	int		freqPerBin[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	*A, *T, chi2, proportion, uniformity, p_hat, proportion_threshold_min, proportion_threshold_max;
	float	c;
	FILE	*fp;
	
	if ( (fp = fopen(s, "r")) == NULL ) {
		printf("%s",s);
		printf(" -- file not found. Exiting program.\n");
		exit(-1);
	}
	
	if ( (A = (double *)calloc(tp.numOfBitStreams, sizeof(double))) == NULL ) {
		printf("Final Analysis Report aborted due to insufficient workspace\n");
		return 0;
	}
	
	/* Compute Metric 1: Proportion of Passing Sequences */
	
	count = 0; 		
	sampleSize = tp.numOfBitStreams;
	
	if ( (test == TEST_RND_EXCURSION) || (test == TEST_RND_EXCURSION_VAR) ) { /* Special Case: Random Excursion Tests */
		if ( (T = (double *)calloc(tp.numOfBitStreams, sizeof(double))) == NULL ) {
			printf("Final Analysis Report aborted due to insufficient workspace\n");
			return 0;
		}
		for ( j=0; j < sampleSize; j++ ) {
			fscanf(fp, "%f", &c);
			if ( c > 0.000000 )
				T[count++] = c;
		}
		
		if ( (A = (double *)calloc(count, sizeof(double))) == NULL ) {
			printf("Final Analysis Report aborted due to insufficient workspace\n");
			return 0;
		}
		
		for ( j=0; j<count; j++ )
			A[j] = T[j];
		
		sampleSize = count;
		count = 0;
		for ( j=0; j<sampleSize; j++ )
			if ( A[j] < ALPHA )
				count++;
		free(T);
	}
	else {
		if ( (A = (double *)calloc(sampleSize, sizeof(double))) == NULL ) {
			printf("Final Analysis Report aborted due to insufficient workspace\n");
			return 0;
		}
		
		for ( j=0; j<sampleSize; j++ ) {
			fscanf(fp, "%f", &c);
			if ( c < ALPHA )
				count++;
			A[j] = c;
		}
	}
	if ( sampleSize == 0 )
		proportion = 0.0;
	else
		proportion = 1.0-(double)count/sampleSize;
	
	p_hat = 1.0 - ALPHA;
	proportion_threshold_max = p_hat + 2.6 * sqrt((p_hat*ALPHA)/sampleSize);
	proportion_threshold_min = p_hat - 2.6 * sqrt((p_hat*ALPHA)/sampleSize);
	
	/* Compute Metric 2: Histogram */
	
	qsort((void *)A, sampleSize, sizeof(double), (void *)cmp);
	for ( j=0; j<sampleSize; j++ ) {
		pos = (int)floor(A[j]*10);
		if ( pos == 10 )
			pos--;
		freqPerBin[pos]++;
	}
	chi2 = 0.0;
	expCount = sampleSize/10;
	if ( expCount == 0 )
		uniformity = 0.0;
	else {
		for ( j=0; j<10; j++ )
			chi2 += pow(freqPerBin[j]-expCount, 2)/expCount;
		uniformity = cephes_igamc(9.0/2.0, chi2/2.0);
	}
	
	for ( j=0; j<10; j++ )			/* DISPLAY RESULTS */
		fprintf(summary, "%3d ", freqPerBin[j]);
	
	if ( expCount == 0 )
		fprintf(summary, "    ----    ");
	else if ( uniformity < 0.0001 )
		fprintf(summary, " %8.6f * ", uniformity);
	else
		fprintf(summary, " %8.6f   ", uniformity);
	
	if ( sampleSize == 0 )
		fprintf(summary, " ----     %s\n", testNames[test]);
	//	else if ( proportion < 0.96 )
	else if ( (proportion < proportion_threshold_min) || (proportion > proportion_threshold_max))
		fprintf(summary, "%6.4f *  %s\n", proportion, testNames[test]);
	else
		fprintf(summary, "%6.4f    %s\n", proportion, testNames[test]);
	
	fclose(fp);
	free(A);
	
	return sampleSize;
}
#endif

#ifdef KS

int	process_pvals_single_test(double* A, int test){
	int		j, pos, count, sampleSize, generalSampleSize, expCount;
	int		freqPerBin[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	proportion, uniformity, p_hat, KSpval, proportion_threshold_min, proportion_threshold_max, chi2;
	double Dmax = 0, Dplus, Dminus;

	
	count = 0; 		
	generalSampleSize = tp.numOfBitStreams;
	sampleSize = 0;

	

	count = 0;
	fprintf(pvals.results, "%s \n", testNames[test]);
	for (j = 0; j < generalSampleSize; j++){
		
		if ((float)A[j] > 0.000000){
			A[sampleSize] = A[j];
			fprintf(pvals.results, "%f ", (float)A[j]);
			sampleSize++;
			if (A[j] < ALPHA)
				count++;
		}
		else fprintf(pvals.results, "%f ", 0);
		
		
	}
	fprintf(pvals.results, "\n \n");
	if (sampleSize == 0)
		proportion = 0.0;
	else
		proportion = 1.0 - (double)count / sampleSize;

	p_hat = 1.0 - ALPHA;
	proportion_threshold_max = p_hat + 3.0 * sqrt((p_hat*ALPHA) / sampleSize);
	proportion_threshold_min = p_hat - 3.0 * sqrt((p_hat*ALPHA) / sampleSize);

	/* Compute Metric 2: Histogram */

	qsort((void *)A, sampleSize, sizeof(double), (void *)cmp);
	
	for (j = 0; j<sampleSize; j++) {
		pos = (int)floor(A[j] * 10);
		if (pos == 10)
			pos--;
		freqPerBin[pos]++;
	}

	chi2 = 0.0;
	expCount = sampleSize / 10;
	if (expCount == 0)
		uniformity = 0.0;
	else {
		for (j = 0; j<10; j++)
			chi2 += pow(freqPerBin[j] - expCount, 2) / expCount;
		uniformity = cephes_igamc(9.0 / 2.0, chi2 / 2.0);
	}

	Dmax = 0;
	for ( j = 1; j <= sampleSize; j++){

		Dplus = fabs(j / (double)sampleSize - A[j - 1]);
		Dminus = fabs(A[j - 1] - (j - 1) / (double)sampleSize);

		if(Dplus > Dmax) Dmax = Dplus;
		if(Dminus > Dmax) Dmax = Dminus;
	}

	KSpval =  2 * exp(-2 * sampleSize*Dmax*Dmax);


	for (j = 0; j<10; j++)			/* DISPLAY RESULTS */
		fprintf(summary, "%3d ", freqPerBin[j]);

	if (expCount == 0)
		fprintf(summary, "    ----    ");
	else if (uniformity < 0.0001)
		fprintf(summary, " %8.6f * ", uniformity);
	else
		fprintf(summary, " %8.6f   ", uniformity);
	if (expCount == 0)
		fprintf(summary, "    ----    ");
	else if (uniformity < 0.0001)
		fprintf(summary, " %8.6f * ", KSpval);
	else
		fprintf(summary, " %8.6f   ", KSpval);

	if (sampleSize == 0)
		fprintf(summary, " ----     %s\n", testNames[test]);
	//	else if ( proportion < 0.96 )
	else if ((proportion < proportion_threshold_min) || (proportion > proportion_threshold_max))
		fprintf(summary, "%6.4f *  %s\n", proportion, testNames[test]);
	else
		fprintf(summary, "%6.4f    %s\n", proportion, testNames[test]);

	return sampleSize;
}

void
postProcessResults(){
	int i, case1, case2, randomExcursionSampleSize, generalSampleSize;
	double passRate;

	generalSampleSize = tp.numOfBitStreams;

	if (testVector[1])process_pvals_single_test(pvals.frequency_pvals, TEST_FREQUENCY);
	if (testVector[2])process_pvals_single_test(pvals.blockfrequency_pvals, TEST_BLOCK_FREQUENCY);
	if (testVector[3]){
		process_pvals_single_test(pvals.cusum_pvals[0], TEST_CUSUM);
		process_pvals_single_test(pvals.cusum_pvals[1], TEST_CUSUM);
	}
	if (testVector[4])process_pvals_single_test(pvals.runs_pvals, TEST_RUNS);
	if (testVector[5])process_pvals_single_test(pvals.longestrunofones_pvals, TEST_LONGEST_RUN);
	if (testVector[6])process_pvals_single_test(pvals.rank_pvals, TEST_RANK);
	if (testVector[7])process_pvals_single_test(pvals.dft_pvals, TEST_FFT);
	if (testVector[8])
		for (i = 0; i < pvals.num_Nonoverlap_pvals; i++)
			process_pvals_single_test(pvals.nonoverlapping_pvals[i], TEST_NONPERIODIC);
		if (testVector[9])process_pvals_single_test(pvals.overlapping_pvals, TEST_OVERLAPPING);
		if (testVector[10])process_pvals_single_test(pvals.universal_pvals, TEST_UNIVERSAL);
		if (testVector[11])process_pvals_single_test(pvals.approximate_entropy_pvals, TEST_APEN);
	if (testVector[12])
		for (i = 0; i < 8; i++)
			randomExcursionSampleSize = process_pvals_single_test(pvals.random_excursion_pvals[i], TEST_RND_EXCURSION);
	if (testVector[13])
		for (i = 0; i < 18; i++)
			process_pvals_single_test(pvals.random_excursion_variant_pvals[i], TEST_RND_EXCURSION_VAR);

	if (testVector[14]){
		process_pvals_single_test(pvals.serial_pvals[0], TEST_SERIAL);
		process_pvals_single_test(pvals.serial_pvals[1], TEST_SERIAL);
	}
	if (testVector[15])process_pvals_single_test(pvals.linear_complexity_pvals, TEST_LINEARCOMPLEXITY);

	fprintf(summary, "\n\n- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
	case1 = 0;
	case2 = 0;
	if (testVector[TEST_RND_EXCURSION] || testVector[TEST_RND_EXCURSION_VAR])
		case2 = 1;
	for (i = 1; i <= NUMOFTESTS; i++) {
		if (testVector[i] && (i != TEST_RND_EXCURSION) && (i != TEST_RND_EXCURSION_VAR)) {
			case1 = 1;
			break;
		}
	}
	if (case1) {
		if (generalSampleSize == 0) {
			fprintf(summary, "The minimum pass rate for each statistical test with the exception of the\n");
			fprintf(summary, "random excursion (variant) test is undefined.\n\n");
		}
		else {
			passRate = 0.99 - 3.0*sqrt(0.01*(1.0 - ALPHA) / (double)generalSampleSize);
			fprintf(summary, "The minimum pass rate for each statistical test with the exception of the\n");
			fprintf(summary, "random excursion (variant) test is approximately = %f for a\n", generalSampleSize ? passRate : 0.0);
			fprintf(summary, "sample size = %d binary sequences.\n\n", generalSampleSize);
		}
	}
	if (case2) {
		if (randomExcursionSampleSize == 0)
			fprintf(summary, "The minimum pass rate for the random excursion (variant) test is undefined.\n\n");
		else {
			passRate = 0.99 - 3.0*sqrt(0.01*(1.0 - ALPHA) / (double)randomExcursionSampleSize);
			fprintf(summary, "The minimum pass rate for the random excursion (variant) test\n");
			fprintf(summary, "is approximately %f for a sample size = %d binary sequences.\n\n", passRate, randomExcursionSampleSize);
		}
	}
	fprintf(summary, "For further guidelines construct a probability table using the MAPLE program\n");
	fprintf(summary, "provided in the addendum section of the documentation.\n");
	fprintf(summary, "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
}
#endif


