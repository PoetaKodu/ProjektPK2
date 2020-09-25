#pragma once

#include <Filter/String.h>

#include <stdio.h>
#include <stdbool.h>

#define CHAR_COMBINATIONS 256

/** Węzeł drzewa prefixowego.
 * */
typedef struct PrefixTree
{
	struct PrefixTree* children[CHAR_COMBINATIONS];	///< Dzieci węzła
	String prefix;									///< Zawartość (część prefixu) węzła
	bool isLeaf;									///< Czy aktualny węzeł jest liściem drzewa?
} PrefixTree;

/** Buduje drzewo przedrostkowe z wyrazów wczytanych z pliku. 
 * @param prefixFilePath_ 	ścieżka do pliku z wyrazami
 * @return Stworzone dynamicznie drzewo.
 * */
PrefixTree* buildPrefixTree(const char* prefixFilePath_);

/** Zwalnia z pamięci zawartość drzewa. 
 * Nie zwalnia korzenia!
 * @param prefixTree_ 		korzeń drzewa przedrostkowego.
 * */
void destroyPrefixTree(PrefixTree* prefixTree_);

/** Sprawdza, czy wyraz powinien zostać zaakceptowany przez filtr przedrostkowy. 
 * @param root_				korzeń drzewa przedrostkowego
 * @param str_ 				wyraz do sprawdzenia
 * @returns true jeśli wyraz posiada przedrostek znajdujący się w drzewie,
 * 			w przeciwnym wypadku false.
 * */
bool prefixFilter(PrefixTree const* root_, String str_);