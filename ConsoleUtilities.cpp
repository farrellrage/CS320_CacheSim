#include "ConsoleUtilities.h"

void PrintOutputLine(const char& symbol, const int& length)
{
	cout << setfill(symbol)
		<< setw(length)
		<< symbol
		<< setfill(' ')
		<< endl;
} //PrintOutputLine