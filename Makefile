
all: Main.cpp Caches.o CacheUtilities.o ConversionUtilities.o ConsoleUtilities.o Main.h
	g++ -g -Wall -o cache-sim Main.cpp Caches.o CacheUtilities.o ConversionUtilities.o ConsoleUtilities.o

ConversionUtilities.o: ConversionUtilities.cpp ConversionUtilities.h Main.h
	g++ -g -Wall -c ConversionUtilities.cpp

Caches.o: Caches.cpp Caches.h Main.h
	g++ -g -Wall -c Caches.cpp

CacheUtilities.o: CacheUtilities.cpp Caches.h Main.h
	g++ -g -Wall -c CacheUtilities.cpp

ConsoleUtilities.o: ConsoleUtilities.cpp ConsoleUtilities.h Main.h
	g++ -g -Wall -c ConsoleUtilities.cpp

clean:
	rm -f cache-sim *.o
