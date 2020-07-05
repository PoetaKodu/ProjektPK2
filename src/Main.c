#include <stdio.h>
#include <stdlib.h>

#include <Filter/ParseArgs.h>
#include <Filter/App.h>


int main(int argc, char* argv[])
{
	LaunchConfig launchCfg;
	if (!parseArgs(argc, argv, &launchCfg))
		return EXIT_FAILURE;

	if (!run(&launchCfg))
		return EXIT_FAILURE;
}