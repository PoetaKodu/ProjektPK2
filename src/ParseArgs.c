#include <Filter/ParseArgs.h>

#include <assert.h>
#include <string.h>

////////////////////////////////////////////////
bool parseArgs(int argc, char *argv[], LaunchConfig * config_)
{
	assert(config_ != NULL);

	bool paramProvided[3] = { false, false, false };

	for(int i = 1; i < argc; ++i)
	{
		if (i + 1 < argc)
		{
			char * dest = NULL;

			if (strcmp(argv[i], "-i") == 0) {
				paramProvided[0] = true;
				dest = config_->inFilePath;
			}
			else if (strcmp(argv[i], "-f") == 0) {
				paramProvided[1] = true;
				dest = config_->prefixFilePath;
			}
			else if (strcmp(argv[i], "-o") == 0) {
				paramProvided[2] = true;
				dest = config_->outFilePath;
			}

			if (dest != NULL) {
				strncpy(dest, argv[i + 1], FILENAME_MAX - 1);
				dest[FILENAME_MAX - 1] = '\0';
			}
		}
	}

	return paramProvided[0] && paramProvided[1] && paramProvided[2];
}