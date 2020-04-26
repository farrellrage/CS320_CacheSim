#include "ConversionUtilities.h"

int BinStrToInt(string binStr)
{
	int retNum = 0;
	unsigned int strLength = binStr.length();

	for (unsigned int b = 0; b < (unsigned int)strLength; b++)
	{
		binStr.at(strLength - 1 - b) == '1' ? retNum += (int)pow(2, b) : 0;
	} //for

	return retNum;
} //BinStrToInt

int BinStrToInt(string binStr, const int& numBits)
{
	int retNum = 0;
	unsigned int strLength = binStr.length();

	for (unsigned int b = 0;
		(b < (unsigned int)strLength) && (b < (unsigned int)numBits);
		b++)
	{
		binStr.at(strLength - 1 - b) == '1' ? retNum += (int)pow(2, b) : 0;
	} //for

	return retNum;
} //BinStrToInt

string IntToBinStr(int integer)
{
	string retStr = "";
	unsigned int MSB = (unsigned int)log2(integer);

	for (unsigned int b = MSB; b > 0; b--)
	{
		if ((integer & (1 << b)) == 0) retStr += '0';
		else retStr += '1';
	} //for

	((integer & 1) == 0) ? retStr += '0' : retStr += '1';

	return retStr;
} //IntToBinStr