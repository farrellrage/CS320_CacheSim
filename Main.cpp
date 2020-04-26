
#include "Main.h"
#include "ConsoleUtilities.h"
#include "Caches.h"
#include "ConversionUtilities.h"

int main(int argc, char** argv)
{
	//Determine whether an invalid number of arguments were given
	if (argc != 3)
	{
		PrintOutputLine(ERROR_CHAR, OUTPUT_LINE);
		cout << "Error - must supply an input and output file" << endl;
		PrintOutputLine(ERROR_CHAR, OUTPUT_LINE);

		return -1;
	} // if
	
	string inputFileName(argv[1]);
	string outputFileName(argv[2]);

	ofstream fout(outputFileName);	//Output file stream
	
	DirectMappedCacheSim(inputFileName, fout);
	SetAssociativeCacheSim(inputFileName, fout);
	FullyAssociativeCacheSim(inputFileName, fout);

	//Close the output file stream
	fout.close();
    
	return 0;
} //main
