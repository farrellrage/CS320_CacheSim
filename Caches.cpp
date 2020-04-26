#include "Caches.h"

void DirectMappedCacheSim(const string& inputFileName, ofstream& fout)
{
	#pragma region AlgorithmVariables
	ifstream fin;

	int cacheHits = 0;
	int memoryAccesses = 0;
	int blocksInCache;
	unsigned int tagBits, indexBits, offsetBits;
	unsigned int tagMask, indexMask;
	unsigned int tag, index;

	unsigned int targetAddr;
	char instructionType;

	vector<DirectMappedCache> cache;
	#pragma endregion
	
	//Calculate number of bits in the offset
	offsetBits = (unsigned int)log2(CACHE_LINE_SIZE_BYTES);

	//For each cache size to simulate
	for (size_t i_cache = 0;
		i_cache < DirectMappedCacheSizeBytes.size();
		i_cache++)
	{
		#pragma region LoopInit
		//Initialize counters for the current loop
		cacheHits = 0; memoryAccesses = 0;
		fin.open(inputFileName);

		//Calculate the number of blocks in the current cache
		blocksInCache = DirectMappedCacheSizeBytes[i_cache] / CACHE_LINE_SIZE_BYTES;

		//Size the cache for the current loop
		cache.clear();
		cache.resize(blocksInCache);

		//Calculate number of bits in index
		indexBits = (unsigned int)log2(
			(int)(DirectMappedCacheSizeBytes[i_cache] / CACHE_LINE_SIZE_BYTES));

		//Calculate number of tag bits for the current loop
		tagBits = ADDRESS_BITS - (indexBits + offsetBits);

		//Build bit masks for the current loop
		indexMask = ((int)pow(2, indexBits) - 1) << offsetBits;
		tagMask = ((int)pow(2, tagBits) - 1) << (offsetBits + indexBits);
		#pragma endregion

		//While there is data in the input file
		while (fin >> instructionType >> hex >> targetAddr)
		{
			//Determine the tag for the current address
			tag = (targetAddr & tagMask) >> (offsetBits + indexBits);
			//Determine the cache location to access
			index = (targetAddr & indexMask) >> offsetBits;

			//Determine whether the cache entry is valid and contains the tag
			if (cache[index].valid == true
				&& cache[index].tag == tag)
			{
				//Increment cache hits
				cacheHits++;
			} //if
			else
			{
				//Write the data to the cache entry
				cache[index].valid = true;
				cache[index].tag = tag;
			} //else

			//Increment number of cache access
			memoryAccesses++;
		} //while

		//Close the input file to reset for the next loop
		fin.close();

		//Add cache hit and memory access counters to output file
		fout << cacheHits << ',' << memoryAccesses << ';' << ' ';
	} //for

	fout << endl;	//Add a newline to the output file
} //DirectMappedCache

void SetAssociativeCacheSim(const string& inputFileName, ofstream& fout)
{
	#pragma region AlgorithmVariables
	ifstream fin;

	int cacheHits = 0;
	int memoryAccesses = 0;

	bool validEntry = false;
	int wayIndexToReplace;
	int changedWay = 0;
	int hitWay;
	bool tagMatch = false;

	unsigned int tagBits, indexBits, offsetBits;
	unsigned int tagMask, indexMask;
	unsigned int tag, index;

	unsigned int targetAddr;
	char instructionType;

	vector<vector<SetAscCacheEntry>> cache;
	vector<unsigned int> waysToReplace;
	vector<unsigned int> waysToIncreaseLRU;

	//Calculate number of bits in the offset
	offsetBits = (unsigned int)log2(SET_ASSOCIATIVE_LINE_SIZE_BYTES);
	#pragma endregion

	//For each set size to simulate
	for (size_t i_setType = 0;
		i_setType < SetAssociativity.size();
		i_setType++)
	{
		#pragma region LoopInit
		//Initialize counters for the current loop
		cacheHits = 0; memoryAccesses = 0;
		fin.open(inputFileName);

		//Initialize the cache for the current loop
		InitSetAscCache(cache, SET_ASSOCIATIVE_CACHE_SIZE_BYTES,
			SetAssociativity[i_setType]);

		//Calculate number of bits in index
		indexBits = (unsigned int)log2(cache[0].size());

		//Calculate number of tag bits for the current loop
		tagBits = ADDRESS_BITS - (indexBits + offsetBits);

		//Build bit masks for the current loop
		indexMask = ((int)pow(2, indexBits) - 1) << offsetBits;
		tagMask = ((int)pow(2, tagBits) - 1) << (offsetBits + indexBits);

		#pragma endregion


		//While there is data in the input file
		while (fin >> instructionType >> hex >> targetAddr)
		{
			#pragma region AddrLoopInit
			wayIndexToReplace = 0;
			changedWay = 0;

			//Determine the tag for the current address
			tag = (targetAddr & tagMask) >> (offsetBits + indexBits);
			//Determine the cache location to access
			index = (targetAddr & indexMask) >> offsetBits;

			//Reset the set entry list
			waysToReplace.clear();
			waysToIncreaseLRU.clear();
			tagMatch = false;
			hitWay = -1;
			#pragma endregion

			////For each way in the cache
			//for (unsigned int way = 0; way < cache.size(); way++)
			//{
			//	//Determine whether the set in the current way is invalid
			//	if (cache[way][index].valid == false)
			//	{
			//		//Save the way index in case another way holds the tag
			//		waysToReplace.push_back(way);
			//		//wayIndexToReplace = way;
			//	} // if
			//	else if ((cache[way][index].valid == true)
			//		&& (cache[way][index].tag == tag))
			//	{
			//		//If the entry is valid and the tag matches, it's a hit
			//		cacheHits++;
			//		
			//		//Save the way of the entry that matched
			//		changedWay = way;

			//		waysToReplace.clear(); //Clear the buffer of sets to check
			//		break; //Don't need to check sets in the other ways
			//	} //else if
			//	else
			//	{
			//		waysToReplace.push_back(way);
			//	} //else
			//} //for

			for (unsigned int way = 0; way < cache.size(); way++)
			{
				if (cache[way][index].valid == true)
				{
					if (cache[way][index].tag == tag)
					{
						if (tagMatch)
						{
							waysToIncreaseLRU.push_back(way);
						}
						else
						{
							tagMatch = true;
							hitWay = way;
						}
					} //if
					else
					{
						//Add the current way entry as a candidate for
						//replacement in case a tag match isn't found
						waysToReplace.push_back(way);
						waysToIncreaseLRU.push_back(way);
					} //else
				} //if
				else if (cache[way][index].valid == false)
				{
					//Add the current way entry as a candidate for replacement
					//in case a tag match isn't found
					waysToReplace.push_back(way);
					waysToIncreaseLRU.push_back(way);
				} //else if
			} //for

			//No tag was found, replace an entry with the tag
			if (!tagMatch)
			{
				wayIndexToReplace = waysToReplace.front();

				for (int i = 1; i < waysToReplace.size(); i++)
				{
					if (cache[waysToReplace[i]][index].turnsSinceLastUsed
						> cache[waysToReplace[wayIndexToReplace]][index].turnsSinceLastUsed)
					{
						wayIndexToReplace = waysToReplace[i];
					}
				} //for

				cache[wayIndexToReplace][index].valid = true;
				cache[wayIndexToReplace][index].tag = tag;
				hitWay = wayIndexToReplace;
			} //if
			else
			{
				cacheHits++;
			} //else

			//Determine whether we need to replace
			//if (waysToReplace.size() > 0)
			//{
			//	wayIndexToReplace = waysToReplace.front();
			//
			//	//For each way set we need to check
			//	for (unsigned int way = 1; way < waysToReplace.size(); way++)
			//	{
			//		if ((cache[waysToReplace[way]][index].turnsSinceLastUsed
			//			> cache[wayIndexToReplace][index].turnsSinceLastUsed))
			//		{
			//			//Update the index of the way set we will replace
			//			wayIndexToReplace = waysToReplace[way];
			//		} //if
			//	} //for
			//
			//	//Replace the cache way entry
			//	cache[wayIndexToReplace][index].valid = true;
			//	cache[wayIndexToReplace][index].tag = tag;
			//	cache[wayIndexToReplace][index].turnsSinceLastUsed = 0;
			//	changedWay = wayIndexToReplace;
			//} //if

			SetAscLruUpdate(cache, index, hitWay);

			////Update the use counts for each unused way entry
			//for (unsigned int i = 0; i < waysToIncreaseLRU.size(); i++)
			//{
			//	//Only update the count for valid entries
			//	//Don't update the count for the replaced entry
			//	if ((cache[waysToIncreaseLRU[i]][index].valid == true)
			//		&& (waysToIncreaseLRU[i] != hitWay))
			//	{
			//		cache[waysToIncreaseLRU[i]][index].turnsSinceLastUsed++;
			//	} //if
			//} //for

			//cache[hitWay][index].turnsSinceLastUsed = 0;

			//Increment number of cache access
			memoryAccesses++;
		} //while

		//Close the input file to reset for the next loop
		fin.close();

		//Add cache hit and memory access counters to output file
		fout << cacheHits << ',' << memoryAccesses << ';' << ' ';
	} //for

	fout << endl;	//Add a newline to the output file
} //SetAssociativeCacheSim

void FullyAssociativeCacheSim(const string& inputFileName, ofstream& fout)
{
	#pragma region AlgorithmVariables
	ifstream fin(inputFileName);

	#pragma endregion


	FullAscLruReplacementPolicy(fin, fout);

	//Reset the input file for the next run
	//fin.close();
	//fin.open(inputFileName);

	//FullAscHotColdLruReplacementPolicy(fin, fout);

	fin.close();
} //FullyAssociativeCacheSim

