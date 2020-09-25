#include <Filter/Filter.h>
#include <Filter/PrefixTree.h>
#include <Filter/BufferedFileReader.h>

#include <string.h>
#include <assert.h>

/** Rozmiar bufora dla pliku wyjściowego */
#define OUTPUT_BUF_SIZE 1024 * 1024

///////////////////////////////////////////
bool filterInputFile(char const* inFilePath_, char const* outFilePath_, PrefixTree const* prefixTree_)
{
	assert(inFilePath_ != NULL && outFilePath_ != NULL);

	BufferedFileReader* reader = bfrOpen(inFilePath_);
	if (!reader)
	{
		printf("Couldn't open input file for reading, exiting...");
		return false;
	}

	FILE *outFile = fopen(outFilePath_, "w");
	if (!outFile)
	{
		printf("Couldn't open output file for writing, exiting...");
		bfrClose(reader);
		return false;
	}

	char outBuf[OUTPUT_BUF_SIZE];
	outBuf[0] = 0;
	size_t bufLen = 0;

	String word = bfrReadUntilWs(reader);
	while(word.len > 0)
	{
		// String prefix;
		bool filterResult = prefixFilter(prefixTree_, word);

		printf("(%c) Word: %.*s", filterResult ? '+' : '-', word.len, word.data);
		putchar('\n');

		if (filterResult)
		{
			if (bufLen + word.len + 1 <= OUTPUT_BUF_SIZE)
			{
				strncpy(outBuf + bufLen, word.data, word.len);
				bufLen += word.len;
				outBuf[bufLen++] = '\n';
			}
			else
			{
				fwrite(outBuf, 1, bufLen, outFile);

				strncpy(outBuf, word.data, word.len);
				bufLen = word.len;
				outBuf[bufLen++] = '\n';
			}
		}
	
		destroyString(&word);
		word = bfrReadUntilWs(reader);
	}

	if (bufLen != 0)
		fwrite(outBuf, 1, bufLen, outFile);

	fclose(outFile);
	bfrClose(reader);

	return true;
}