#pragma once

#include <Filter/String.h>

#include <stdio.h>
#include <stdbool.h>

#define CHAR_COMBINATIONS 256

typedef struct PrefixTree
{
	struct PrefixTree* children[CHAR_COMBINATIONS];
	String prefix;
	bool isLeaf;
} PrefixTree;

PrefixTree* makePrefixTree();

PrefixTree* buildPrefixTree(const char* prefixFilePath);

void destroyPrefixTree(PrefixTree* prefixTree_);

void insertPrefixIntoTree(PrefixTree* root_, String prefix_, size_t startCharacter);