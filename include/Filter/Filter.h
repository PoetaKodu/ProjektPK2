#pragma once

#include <stdbool.h>

typedef struct PrefixTree PrefixTree;

/** Filtruje plik źródłowy po przedrostkach zawartych w drzewie przedrostkowym. 
 * Zapisuje przefiltrowane dane do osobnego pliku.
 * @param inFilePath_ 		ścieżka do pliku wejściowego
 * @param outFilePath_ 		ścieżka do pliku wyjściowego
 * @param prefixTree_ 		wskaźnik na korzeń drzewa przedrostkowego
 * */
bool filterInputFile(char const* inFilePath_, char const* outFilePath_, PrefixTree const* prefixTree_);