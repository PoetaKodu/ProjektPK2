#pragma once

#include <Filter/String.h>

#include <stdio.h>
#include <stdbool.h>

/// Rozmiar bloku danych wczytywanego przez BufferedFileReader
#define BFR_BUF_SIZE 1024 * 1024

/** Buforowany wczytywacz (reader) danych z pliku.
 * Wczytuje dane z pliku porcjami w celu zwiększenia wydajności.
 * */
typedef struct BufferedFileReader
{
	FILE* file; 				///< Uchwyt do pliku.

	char buf[BFR_BUF_SIZE]; 	///< Bufor do wczytywania danych.
	size_t readCount; 			///< Ilość dostępnych znaków w buforze.

} BufferedFileReader;

/** Otwiera buforowany reader wraz z danym plikiem.
 * @param path_ 		ścieżka do pliku
 * @return wskaźnik na dynamicznie stworzony reader.
 * */
BufferedFileReader* bfrOpen(const char* path_);

/** Wczytuje dane do napotkania białego znaku.
 * @param reader_ 		wskaźnik na reader
 * @return Ciąg wczytanych znaków/danych, lub pusty String, jeśli się nie udało wczytać.
 * */
String bfrReadUntilWs(BufferedFileReader* reader_);

/** Usuwa z pamięci reader oraz zamyka plik.
 * @param reader_ 		wskaźnik na reader
 * */
void bfrClose(BufferedFileReader* reader_);
