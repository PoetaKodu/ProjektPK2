#include <stdio.h>
#include <stdlib.h>

#include <Filter/ParseArgs.h>
#include <Filter/App.h>

/** Funkcja główna programu.
 * Parsuje argumenty i uruchamia właściwą logikę programu.
 * @param argc ilość argumentów
 * @param argv zawartość argumentów
 * */
int main(int argc, char* argv[])
{
	LaunchConfig launchCfg;
	if (!parseArgs(argc, argv, &launchCfg))
		return EXIT_FAILURE;

	if (!run(&launchCfg))
		return EXIT_FAILURE;
}