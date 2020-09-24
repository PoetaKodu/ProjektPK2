#include <Filter/PrefixTree.h>

#include <Filter/BufferedFileReader.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TAB_SIZE 4

void printIndent(size_t indent_)
{
	for(size_t i = 0; i < indent_; ++i)
	{
		for (int j = 0; j < TAB_SIZE; ++j)
			putchar(' ');
	}
}

///////////////////////////////////////////////////
void printTree(PrefixTree *node_, size_t indent_)
{
	if (node_)
	{
		printIndent(indent_); fputs("{\n", stdout);
	
		printIndent(indent_+1); fputs("\"prefix\": \"", stdout);
		for(size_t i = 0; i < node_->prefix.len; ++i)
			putchar(node_->prefix.data[i]);
		fputs("\"", stdout);
		
		size_t numValidChildren = 0;
		for(size_t i = 0; i < CHAR_COMBINATIONS; ++i)
			if (node_->children[i] != NULL)
				++numValidChildren;

		if (numValidChildren > 0)
		{
			fputs(",\n", stdout);
			printIndent(indent_+1); fputs("\"children\": [", stdout);
			numValidChildren = 0;
			for(size_t i = 0; i < CHAR_COMBINATIONS; ++i)
			{
				if (node_->children[i] != NULL)
				{
					if (numValidChildren > 0)
						fputs(",\n", stdout);
					else
						putchar('\n');
					printTree(node_->children[i], indent_+2);

					++numValidChildren;
				}
			}
			if (numValidChildren > 0)
			{
				putchar('\n');
				printIndent(indent_+1);
			}
			fputs("]\n", stdout);
		}
		else
		{
			putchar('\n');
		}

		printIndent(indent_); fputs("}", stdout);
	}
}

///////////////////////////////////////////////////////////
void PrefixTree_ctor(PrefixTree* node)
{
	node->isLeaf = false;
	node->prefix = makeString();
	for(size_t i = 0; i < CHAR_COMBINATIONS; ++i)
	{
		node->children[i] = NULL;
	}
}

///////////////////////////////////////////////////////////
void PrefixTree_ctorInit(PrefixTree* node, String str_)
{
	node->isLeaf = false;
	node->prefix = str_;
	for(size_t i = 0; i < CHAR_COMBINATIONS; ++i)
	{
		node->children[i] = NULL;
	}
}


///////////////////////////////////////////////////////////
PrefixTree* makePrefixTree()
{
	PrefixTree* pt = malloc(sizeof(PrefixTree));
	PrefixTree_ctor(pt);
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
		printf("Read prefix: %.*s\n", word.len, word.data);

		insertPrefixIntoTree(prefixTree, word, 0);
		
		word = bfrReadUntilWs(reader);
	}
	printTree(prefixTree, 0);
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
bool shouldContinueDown(PrefixTree* node_, String prefix_, size_t* startCharacter)
{
	size_t numMatched = 0;
	for(; numMatched < node_->prefix.len; ++numMatched)
	{
		if (node_->prefix.data[numMatched] != prefix_.data[*startCharacter + numMatched])
			break;
	}
	*startCharacter += numMatched;
	return numMatched == node_->prefix.len;
}

///////////////////////////////////////////////////////////
void insertPrefixIntoTree(PrefixTree* root_, String prefix_, size_t startCharacter)
{
	assert(prefix_.len > 0);

	while (true)
	{
		const char first = prefix_.data[startCharacter];
	
		if (root_->children[first])
		{
			PrefixTree* node = root_->children[first];

			size_t newStart = startCharacter;
			if (shouldContinueDown(node, prefix_, &newStart))
			{
				root_ = node;
				startCharacter = newStart;
			}
			else
			{
				PrefixTree* toSplit = node;

				size_t splitAt = newStart - startCharacter;

				PairOfStrings splitted = splitString(&toSplit->prefix, splitAt);

				destroyString(&toSplit->prefix);

				root_->children[first] = malloc(sizeof(PrefixTree));
				node = root_->children[first];

				PrefixTree_ctorInit(node, splitted.left);
				toSplit->prefix = splitted.right;

				node->children[toSplit->prefix.data[0]] = toSplit;

				node->children[prefix_.data[newStart]] = malloc(sizeof(PrefixTree));
				PrefixTree_ctorInit(node->children[prefix_.data[newStart]], makeStringWith(prefix_.data + newStart, prefix_.len - newStart));
				node->children[prefix_.data[newStart]]->isLeaf = true;

				break;
			}
		}
		else
		{
			root_->children[first] = malloc(sizeof(PrefixTree));
			PrefixTree* node = root_->children[first];
			
			if (startCharacter > 0)
			{
				PrefixTree_ctorInit(node, makeStringWith(prefix_.data + startCharacter, prefix_.len - startCharacter));
				destroyString(&prefix_);
			}
			else
			{
				PrefixTree_ctorInit(node, prefix_);
			}

			node->isLeaf = true;

			break;
		}
	}
}

///////////////////////////////////////////////////////////
bool prefixFilter(PrefixTree const* root_, String str_, String* matchedPrefix)
{
	size_t charIdx = 0;

	if (matchedPrefix)
		*matchedPrefix = makeString();

	while(charIdx < str_.len)
	{
		root_ = root_->children[str_.data[charIdx]];
		if (!root_)
			return false;

		if (charIdx + root_->prefix.len > str_.len)
			return false;

		if (strncmp(str_.data + charIdx, root_->prefix.data, root_->prefix.len) != 0)
			return false;

		if (matchedPrefix)
			appendString(matchedPrefix, &root_->prefix);

		if (root_->isLeaf)
			return true;
		
		charIdx += root_->prefix.len;
	}

	return false;
}
