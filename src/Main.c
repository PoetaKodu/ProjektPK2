#include <stdio.h>
#include <stdlib.h>

#include <Filter/ParseArgs.h>


int main(int argc, char* argv[])
{
	LaunchConfig launchCfg;
	if (!parseArgs(argc, argv, &launchCfg))
		return EXIT_FAILURE;


}