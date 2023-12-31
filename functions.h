#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

// Include necessary headers
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>
#include <ctype.h>
#include <string.h>


void readScoreTableFromFile(const char* filename);

void calcAlignmentScore(char* localSequence, char* mainSequence,int* alignmentScore, int* alignment,int* mutation);

void strToUpper(char* str);

int calcScoreOfTwoChars(char c1, char c2);

void printResults(int* results,int numSequences);

#endif /* FUNCTIONS_H_ */




