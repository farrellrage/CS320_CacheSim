#ifndef main_h
#define main_h

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
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

enum HotColdLru {
	LEFT = 0,
	RIGHT = 1
};

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

#endif // !main_h
