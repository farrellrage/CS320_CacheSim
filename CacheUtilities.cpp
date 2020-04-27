#include "Caches.h"

void InitCache(vector<vector<DirectMappedCache>>& cache, const int& cacheSize,
	const int& lineSize)
{
	//For each entry in the cache
	for (size_t i = 0; i < cache.size(); i++)
	{
		cache[i].resize(lineSize);
	} //for
} //InitCache

void InitSetAscCache(vector<vector<SetAscCacheEntry>>& cache,
	const int& cacheSize, const int& numberOfSets)
{
	unsigned int setSize = (unsigned int)(cacheSize / numberOfSets);
	
	//Resize the cache to fit the given number of sets
	cache.clear();
	cache.resize(numberOfSets);

	//For each set in the cache
	for (size_t i = 0; i < cache.size(); i++)
	{
		//Resize the current set to fit the portion of the cache
		cache[i].resize((int)(setSize / SET_ASSOCIATIVE_LINE_SIZE_BYTES));
	} //for
} //InitSetAsscCache

void SetAscLruUpdate(vector<vector<SetAscCacheEntry>>& cache,
	const int& setIndex, const int& usedWay)
{
	//For each way in the cache
	for (size_t way = 0; way < cache.size(); way++)
	{
		if (way != usedWay)
		{
			//Increment the LRU count for the entry
			cache[way][setIndex].lruCount++;
		} //if
	} //for

	//Reset the LRU count for the used entry
	cache[usedWay][setIndex].lruCount = 0;
} //SetAscLruUpdate

void FullAscLruUpdate(vector<FullAscCacheEntry>& cache, int usedIndex)
{
	//For each entry in the cache, except the used entry
	for (size_t i = 0; i < cache.size(); i++)
	{
		if (i != usedIndex)
		{
			//Increment the LRU count for the entry
			cache[i].lruCount++;
		} //if
	} //for

	//Reset the LRU count for the used entry
	cache[usedIndex].lruCount = 0;
} //FullAscLruUpdate

void FullAscLruReplacementPolicy(ifstream& fin, ofstream& fout)
{
	#pragma region AlgorithmVariables
	unsigned int cacheHits = 0;
	unsigned int memAccesses = 0;

	unsigned int tagBits,	offsetBits;
	unsigned int tagMask;
	unsigned int tag;

	unsigned int targetAddr;
	char instructionType;

	int lruIndex;

	bool cacheHit;

	vector<FullAscCacheEntry> cache;
	#pragma endregion

	//Initialize the cache
	cache.resize((int)(FULL_ASSOCIATIVE_CACHE_SIZE_BYTES
		/ FULL_ASSOCIATIVE_LINE_SIZE_BYTES));

	//Calculate the number of offset bits in the addresses
	offsetBits = (unsigned int)log2(FULL_ASSOCIATIVE_LINE_SIZE_BYTES);

	//Calculate the number of tag bits in the addresses
	tagBits = ADDRESS_BITS - offsetBits;

	//Build mask bits
	tagMask = ((int)pow(2, tagBits) - 1) << (offsetBits);

	//While there is data in the input file
	while (fin >> instructionType >> hex >> targetAddr)
	{
		//Determine the tag for the current address
		tag = (targetAddr & tagMask) >> (offsetBits);

		cacheHit = false;	//Reset hit flag for the current loop
		lruIndex = 0;		//Reset LRU index for the current loop

		//For each set in the cache
		for (unsigned int i = 0; i < cache.size() && !cacheHit; i++)
		{
			//Determine whether the tag in the current set matches the address
			if (tag == cache[i].tag)
			{
				cacheHit = true;	//Flip the hit flag
				lruIndex = i;		//Set the LRU index for the found entry
				cacheHits++;		//Increment cache hits
			} //if
			else
			{
				//Determine whether the non-matching set is more least recently
				//used than the record
				if (cache[i].lruCount > cache[lruIndex].lruCount)
				{
					//Update the least recently used record
					lruIndex = i;
				} //if
			} //else
		} //for

		//Determine whether the tag was found in the cache
		if (!cacheHit)
		{
			//Replace the LRU entry in cache
			cache[lruIndex].tag = tag;
		} //if

		FullAscLruUpdate(cache, lruIndex);

		//Increment number of cache access
		memAccesses++;
	} //while

	//Close the input file to reset for the next loop
	fin.close();

	//Output results to the output file
	fout << cacheHits << ',' << memAccesses << ';' << endl;
} //FullAscLruReplacement

//int FindHotColdLruIndex(int map, int numberOfBits)
//{
//	int bitMask = 1 << (numberOfBits - 1);
//	int numberOfLevels = (int)(ceil(log2(numberOfBits)));
//	int bitNum = 0;
//	int indexOffset = 0;
//
//
//	//For each level in the hot/cold map
//	for (int i = 0; i < numberOfLevels; i++)
//	{
//		if ((map & bitMask) == 0)
//		{
//			//Create the bit mask to get the next bit
//			bitMask = (1 << (numberOfBits - 1) - bitNum) - 1;
//		} //if
//		else if ((map & bitMask) == 1)
//		{
//			//Create the bit mask to get the next bit
//			bitMask = (1 << (numberOfBits - 1) - bitNum) - 2;
//		} //else if
//		else
//		{
//			return -1;
//		} //else
//
//		//Increment the level of the bit structure
//		bitNum++;
//	} //for
//
//
//} //FindHotColdLruIndex

int FindHotColdLruIndex(vector<int>& map)
{
	int hotColdEntry = ceil(map.size() / 2);
	int levels = ceil(log2(map.size()));
	int partition = hotColdEntry;

	//Scan through each level in the hot/cold structure
	for (int i = 0; i < levels; i++)
	{
		//Determine which direction to follow down the structure
		if (map[hotColdEntry] == 1)
		{
			//Calculate the next entry to check
			hotColdEntry -= int(partition / 2);
			partition = int(partition / 2);
		} //if
		else 
		{
			//Calculate the next entry to check
			hotColdEntry += int(partition / 2);
			partition = int(partition / 2) + 1;
		} //else if
	} //for

	return (map[hotColdEntry] == 0) ? hotColdEntry + 1 : hotColdEntry;
} //FindHotColdLruIndex

void FullAscHotColdLruUpdate(vector<int>& map, int usedIndex, int cacheSize)
{
	int hotColdEntry = ceil(map.size() / 2);
	int levels = ceil(log2(map.size()));
	int partition = hotColdEntry;

	for (int i = 0; i < levels; i++)
	{
		if (usedIndex <= hotColdEntry)
		{
			map[hotColdEntry] = 0;
			hotColdEntry -= int(partition / 2);
			partition = int(partition / 2);
		}
		else
		{
			map[hotColdEntry] = 1;
			hotColdEntry += int(partition / 2);
			partition = int(partition / 2) + 1;
		}
	} //for
} //FullAscUpdateHotColdLru

void FullAscHotColdLruReplacementPolicy(ifstream& fin, ofstream& fout)
{
#pragma region AlgorithmVariables
	unsigned int cacheHits = 0;
	unsigned int memAccesses = 0;

	unsigned int tagBits, offsetBits;
	unsigned int tagMask;
	unsigned int tag;

	unsigned int targetAddr;
	char instructionType;

	int lruIndex;		//The index of the cache hit, or the index to replace
	int hotColdMap = 0;	//Represent the map to the LRU entry

	bool cacheHit;

	vector<FullAscCacheEntry> cache;
	vector<int> hotCold;
#pragma endregion

	//Initialize the cache
	cache.resize((int)(FULL_ASSOCIATIVE_CACHE_SIZE_BYTES
		/ FULL_ASSOCIATIVE_LINE_SIZE_BYTES));

	//We need enough hot/cold bits to index the cache
	hotCold.resize((cache.size() - 1), 0);

	offsetBits = (unsigned int)log2(FULL_ASSOCIATIVE_LINE_SIZE_BYTES);
	tagBits = ADDRESS_BITS - offsetBits;
	tagMask = ((int)pow(2, tagBits) - 1) << (offsetBits);

	//While there is data in the input file
	while (fin >> instructionType >> hex >> targetAddr)
	{
		//Determine the tag for the current address
		tag = (targetAddr & tagMask) >> (offsetBits);

		cacheHit = false;	//Reset hit flag for the current loop
		lruIndex = 0;		//Reset LRU index for the current loop

		//For each set in the cache
		for (unsigned int i = 0; i < cache.size() && !cacheHit; i++)
		{
			//Determine whether the tag in the current set matches the address
			if (tag == cache[i].tag)
			{
				cacheHit = true;	//Flip the hit flag
				lruIndex = i;		//Set the LRU index for the found entry
				cacheHits++;		//Increment cache hits
			} //if
		} //for

		//Determine whether the tag was found in the cache
		if (!cacheHit)
		{
			//Determine the least recently used entry
			lruIndex = FindHotColdLruIndex(hotCold);

			//Replace the entry at the index
			cache[lruIndex].tag = tag;
		} //if
		
		//Update LRU map for the cache hit
		FullAscHotColdLruUpdate(hotCold, lruIndex, cache.size());

		//Increment number of cache access
		memAccesses++;
	} //while

	//Close the input file to reset for the next loop
	fin.close();

	//Output results to the output file
	fout << cacheHits << ',' << memAccesses << ';' << endl;
} //FullAscHotColdLruReplacementPolicy




