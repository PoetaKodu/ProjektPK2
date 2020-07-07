#pragma once

#include <stdbool.h>

typedef struct PrefixTree PrefixTree;

bool filterInputFile(char const* inFilePath, char const* outFilePath, PrefixTree const* prefixTree);