#pragma once

#include <stdio.h>
#include <stdbool.h>

typedef struct
{
	char inFilePath[FILENAME_MAX];
	char prefixesFilePath[FILENAME_MAX];

	char outFilePath[FILENAME_MAX];
} LaunchConfig;

bool parseArgs(int argc, char *argv[], LaunchConfig * config);