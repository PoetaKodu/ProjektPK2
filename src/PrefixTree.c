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

			// Czy można iść w dół drzewa?
			if (shouldContinueDown(node, prefix_, &newStart))
			{
				root_ = node;
				startCharacter = newStart;
			}
			else
			{
				// Trzeba rozbić aktualny przedrostek, by dodać nowy
				PrefixTree* toSplit = node;

				size_t splitAt = newStart - startCharacter;

				// Rozbijanie przedrostka:
				PairOfStrings splitted = splitString(&toSplit->prefix, splitAt);

				// Stary tekst jest usuwany
				destroyString(&toSplit->prefix);

				// Ustawianie zamiennika dla rozbitego kawałka prefixu:
				// (zamiennik wchodzi na miejsce poprzedniego,
				// poprzedni jest przypinany jako dziecko zamiennika)
				// Notka: zamiennik nie jest liściem!
				PrefixTree* replacement = malloc(sizeof(PrefixTree));
				{
					PrefixTree_ctorInit(replacement, splitted.left);
					root_->children[first] = replacement;
					node = root_->children[first];
				}
				
				toSplit->prefix = splitted.right;

				// Przypiecie pod zamiennika:
				node->children[toSplit->prefix.data[0]] = toSplit;

				// Ustawianie nowo dodanego prefixu:
				PrefixTree* newlyAdded = malloc(sizeof(PrefixTree));
				{
					String pref = makeStringWith(prefix_.data + newStart, prefix_.len - newStart);
					PrefixTree_ctorInit(newlyAdded, pref);
					newlyAdded->isLeaf = true;

					node->children[prefix_.data[newStart]] = newlyAdded;
				}

				break;
			}
		}
		else
		{
			// Na miejscu wstawiania pusto.
			// Wstawiamy nowy:
			PrefixTree* node = malloc(sizeof(PrefixTree));
			root_->children[first] = node;
			
			// Czy trzeba uciąć prefix?
			if (startCharacter > 0)
			{
				String prefixCutout = makeStringWith(prefix_.data + startCharacter, prefix_.len - startCharacter);

				PrefixTree_ctorInit(node, prefixCutout);

				// Zwalniam stary, nieużywany prefix.
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
bool prefixFilter(PrefixTree const* root_, String str_, String* matchedPrefix_)
{
	size_t charIdx = 0;

	if (matchedPrefix_)
		*matchedPrefix_ = makeString();

	while(charIdx < str_.len)
	{
		root_ = root_->children[str_.data[charIdx]];

		if (!root_)
			return false;

		// Długości prefixów pasują?
		if (charIdx + root_->prefix.len > str_.len)
			return false;

		// Wszystkie znaki prefixu pasują?
		if (strncmp(str_.data + charIdx, root_->prefix.data, root_->prefix.len) != 0)
			return false;

		// TEMP: zbieram info o faktycznym przedrostku.
		if (matchedPrefix_)
			appendString(matchedPrefix_, &root_->prefix);

		// Czy jest liściem drzewa?
		if (root_->isLeaf)
			return true;
		
		charIdx += root_->prefix.len;
	}

	return false;
}
