#pragma once

#include <stdio.h>
#include <stdbool.h>

/** Informacje o konfiguracji uruchomieniowej programu.
 * */
typedef struct LaunchConfig
{
	char inFilePath[FILENAME_MAX];		///< ścieżka do pliku wejściowego
	char prefixFilePath[FILENAME_MAX]; 	///< ścieżka do pliku z przedrostkami
	char outFilePath[FILENAME_MAX];		///< ścieżka do pliku wyjściowego
} LaunchConfig;

/** Wypełnia ustawienie programu na podstawie argumentów.
 * @param argc_			ilość argumentów
 * @param argv_			zawartość argumentów
 * @param config_		wskaźnik na ustawienie programu, do wypełnienia.
 * */
bool parseArgs(int argc_, char *argv_[], LaunchConfig * config_);