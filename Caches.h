#ifndef Caches_h
#define Caches_h

#include "Main.h"
#include "ConversionUtilities.h"

struct DirectMappedCache {
	bool valid = false;
	unsigned int tag = 0;
}; //DirectMappedCache

struct SetAscCacheEntry {
	bool valid = false;
	unsigned int tag = 0;
	unsigned int turnsSinceLastUsed = 0;
}; //SetAsscCacheEntry

struct FullAscCacheEntry {
	unsigned int tag = 0;
	unsigned int lruCount = 0;
}; //FullAscCacheEntry

void InitCache(vector<vector<DirectMappedCache>>& cache, const int& cacheSize,
	const int& lineSize);

void InitSetAscCache(vector<vector<SetAscCacheEntry>>& cache, const int& cacheSize,
	const int& numberOfSets);

void DirectMappedCacheSim(const string& inputFileName, ofstream& fout);

void SetAscLruUpdate(vector<vector<SetAscCacheEntry>>& cache, const int& setIndex,
	const int& usedWay);
void SetAssociativeCacheSim(const string& inputFileName, ofstream& fout);

void FullAscLruUpdate(vector<FullAscCacheEntry>& cache, int usedIndex);
void FullAscLruReplacementPolicy(ifstream& fin, ofstream& fout);
int FindHotColdLruIndex(vector<int>& map);
void FullAscHotColdLruReplacementPolicy(ifstream& fin, ofstream& fout);
void FullyAssociativeCacheSim(const string& inputFileName, ofstream& fout);

#endif //Caches_h