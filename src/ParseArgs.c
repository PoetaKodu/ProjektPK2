#include <Filter/ParseArgs.h>

#include <assert.h>

bool parseArgs(int argc, char *argv[], LaunchConfig * config)
{
	assert(config != NULL);

	bool paramProvided[3] = { false, false, false };

	for(int i = 1; i < argc; ++i)
	{
		if (i + 1 < argc)
		{
			char * dest = NULL;

			if (strcmp(argv[i], "-i") == 0) {
				paramProvided[0] = true;
				dest = config->inFilePath;
			}
			else if (strcmp(argv[i], "-f") == 0) {
				paramProvided[1] = true;
				dest = config->prefixFilePath;
			}
			else if (strcmp(argv[i], "-o") == 0) {
				paramProvided[2] = true;
				dest = config->outFilePath;
			}

			if (dest != NULL) {
				strncpy(dest, argv[i + 1], FILENAME_MAX - 1);
				dest[FILENAME_MAX - 1] = '\0';
			}
		}
	}

	return paramProvided[0] && paramProvided[1] && paramProvided[2];
}