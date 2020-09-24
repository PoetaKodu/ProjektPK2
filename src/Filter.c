#include <Filter/Filter.h>
#include <Filter/PrefixTree.h>
#include <Filter/BufferedFileReader.h>

#include <assert.h>

///////////////////////////////////////////
bool filterInputFile(char const* inFilePath, char const* outFilePath, PrefixTree const* prefixTree)
{
	assert(inFilePath != NULL && outFilePath != NULL);

	BufferedFileReader* reader = bfrOpen(inFilePath);
	if (!reader)
		return false;

	String word = bfrReadUntilWs(reader);
	while(word.len > 0)
	{
		// String prefix;
		bool filterResult = prefixFilter(prefixTree, word);

		printf("(%c) Word: %.*s", filterResult ? '+' : '-', word.len, word.data);
		// if (filterResult)
		// 	printf(", prefix: \"%.*s\"\n", prefix.len, prefix.data);
		// else
		putchar('\n');

		destroyString(&word);
		word = bfrReadUntilWs(reader);
	}
	bfrClose(reader);

	return true;
}