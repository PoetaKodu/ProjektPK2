#include <Filter/ParseArgs.h>

#include <assert.h>
#include <string.h>

////////////////////////////////////////////////
bool parseArgs(int argc, char *argv[], LaunchConfig * config_)
{
	assert(config_ != NULL);

	bool printHelp = false;
	bool paramProvided[3] = { false, false, false };

	for(int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i], "-h") == 0) {
			printHelp = true;
			break;
		}
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
				++i;
			}
		}
	}

	bool valid = paramProvided[0] && paramProvided[1] && paramProvided[2];

	if (!valid || printHelp)
	{
		puts(
				"Usage: Filter -i <input-file-path> -f <prefix-file-path> -o <output-file-path>\n"
				"\nRuns prefix filter over input file, filtering out words that are not prefixed.\n"
				"\nAvailable switches:\n"
				"\t-i <string>\tinput file path\n"
				"\t-f <string>\tprefix file path\n"
				"\t-o <string>\toutput file path\n"
				"\t-h\t\tprint help\n"
			);
	}

	return valid && !printHelp;
}