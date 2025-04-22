#pragma once

#define MAX_LINES 200
#define MAX_WORDS_PER_LINE 100

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <string>
#include <algorithm>
#include <vector>

#include "helper.h"

using namespace std;

/* Structure definitions */
/* -------------------------------------------- */
typedef struct {
	int lockLineNumber;
	string lockVariable;
} grammar_omp_init_lock;

/* Function definitions */
bool checkOmpFor(string codeLines[], int ompForLineNumber);
bool checkOmpParallel(string codeLines[], int ompParallelLineNumber);
string Identify_Loop_Var(string codeLines[], int ompForLineNumber);
returnMessage detectRaceCondOpenMP(string loopVariable, string codeLines[], int forLineNumber);
