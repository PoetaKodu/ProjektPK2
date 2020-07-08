#include <Filter/PrefixTree.h>

#include <Filter/BufferedFileReader.h>

#include <stdlib.h>
#include <assert.h>

PrefixTree* makePrefixTree()
{
	PrefixTree* pt = malloc(sizeof(PrefixTree));
	pt->prefix = makeString();
	for(size_t i = 0; i < CHAR_COMBINATIONS; ++i)
	{
		pt->children[i] = NULL;
	}
	return pt;
}

///////////////////////////////////////////////////////////
PrefixTree* buildPrefixTree(const char* prefixFilePath)
{
	assert(prefixFilePath != NULL);
	
	BufferedFileReader* reader = bfrOpen(prefixFilePath);
	if (!reader)
		return NULL;

	PrefixTree* prefixTree = makePrefixTree();

	String word = bfrReadUntilWs(reader);
	while(word.len > 0)
	{
		printf("Read word: %.*s\n", word.len, word.data);

		insertPrefixIntoTree(prefixTree, word);
		
		word = bfrReadUntilWs(reader);
	}

	bfrClose(reader);

	return prefixTree;
}

///////////////////////////////////////////////////////////
void destroyPrefixTree(PrefixTree* prefixTree_)
{
	for(size_t i = 0; i < CHAR_COMBINATIONS; ++i)
	{
		PrefixTree* child = prefixTree_->children[i];
		if (child != NULL)
		{
			destroyPrefixTree(child);
			free(child);
			prefixTree_->children[i] = NULL;
		}
	}
}

///////////////////////////////////////////////////////////
void insertPrefixIntoTree(PrefixTree* root_, String prefix_)
{
	// Temporary:
	destroyString(&prefix_);
	// TODO: implement
}
