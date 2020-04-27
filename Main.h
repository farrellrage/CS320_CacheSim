#ifndef main_h
#define main_h

#include <functional>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <vector>
#include <ctime>
#include <cmath>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

#define ERROR_CHAR '#'

#define CACHE_LINE_SIZE_BYTES		32
#define ADDRESS_BITS			32

#define SET_ASSOCIATIVE_LINE_SIZE_BYTES		32
#define SET_ASSOCIATIVE_CACHE_SIZE_BYTES	16384

#define FULL_ASSOCIATIVE_LINE_SIZE_BYTES	32
#define FULL_ASSOCIATIVE_CACHE_SIZE_BYTES	16384

const int OUTPUT_LINE = 50;

const vector<int> DirectMappedCacheSizeBytes {
	1024,
	4096,
	16384,
	32768
};

const vector<int> SetAssociativity {
	2,
	4,
	8,
	16
};

vector<char> charset(void)
{
	return vector<char>({
		'0','1','2','3','4','5','6','7','8','9'
		});
}

#endif // !main_h
