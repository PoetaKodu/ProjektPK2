#pragma once

#include <stdlib.h>
#include <inttypes.h>

/** Struktura do zarządzania napisami */
typedef struct String
{
	char* data;			///< Wskaźnik na początek znaków w napisie
	size_t len;			///< Długość napis
	size_t capacity;	///< Długość zarezerwowanej pamięci dla napisu (zawsze >= len).
} String;

/** Para napisów. */
typedef struct PairOfStrings
{
	String left;
	String right;
} PairOfStrings;

/** Tworzy pusty String.
 * @return pusty String.
 * */
String makeString();

/** Tworzy String kopiując inny istniejący.
 * @param bytes_ 		źródło do kopiowania danych
 * @param len_ 			ilość bajtów do skopiowania
 * @return nowo utworzony String.
 * */
String makeStringWith(char const* bytes_, size_t len_);

/** Tworzy String opakowując istniejące dane.
 * @param bytes_ 		źródło danych
 * @param len_ 			ilość bajtów
 * @return nowo utworzony String (opakowujący istniejące dane).
 * */
String wrapWithString(char *bytes_, size_t len_);

/** Usuwa zawartość istniejącego obiektu String. 
 * Nie usuwa przekazanego obiektu z pamięci!
 * @param str_ 			String, z którego zostanie zwolniona pamięć
 * */
void destroyString(String* str_);

/** Znajduje indeks pierwszego białego znaku w napisie.
 * @param str_ 			rozpatrywany napis
 * @param startPos_		pozycja, od której rozpoczynamy wyszukiwanie
 * @return pozycja znalezionego białego znaku, lub -1 gdy nie znaleziono.
 * */
int64_t findWsInString(String const *str_, size_t startPos_);

/** Znajduje indeks pierwszego elementu nie będącego białym znakiem w napisie.
 * @param str_ 			rozpatrywany napis
 * @param startPos_		pozycja, od której rozpoczynamy wyszukiwanie
 * @return pozycja znalezionego znaku, lub -1 gdy nie znaleziono.
 * */
int64_t findNonWsInString(String const *str_, size_t startPos_);

// Creates left and right string as follows:
// left: [0; where_)
// right: [where; length)
// TODO: implement this:

/** Rozbija napis na dwa osobne napisy w następujący sposób: 
 * lewy zawiera indeksy [0, where_), 
 * prawy zawiera indeksy [where_, length). 
 * Nie zwalnia rozbijanego zapisu z pamięci!
 * @param src_ 			rozbijany napis
 * @param where_		pozycja do rozbicia napisu
 * @return struktura zawierająca lewą i prawą część napisu
 * */
PairOfStrings splitString(String *src_, size_t where_);

/** Dopisuje napis na koniec drugiego.
 * @param str_ 			napis, do którego zostanie dopisany inny
 * @param toAppend_		dopisywany napis
 * */
void appendString(String* str_, String const* toAppend_);

/** Dopisuje napis na koniec drugiego (wersja dla czystego C-stringa)
 * @param str_ 			napis, do którego zostanie dopisany inny
 * @param bytes_		początek napisu do dopisania
 * @param len_			ilość znaków do dopisania
 * */
void appendStringRaw(String* str_, char const* bytes_, size_t len_);

/** Rezerwuje pamięć wewnątrz Stringa. Nigdy nie zmniejsza zaalokowanej pamięci.
 * @param str_			napis, któremu rezerwujemy pamięć
 * @param newCapacity_ 	nowa pojemność pamięci.
 * */
void reserveStringCapacity(String* str_, size_t newCapacity_);

/** Ucina zarezerwowana pamięć do ilości znaków faktycznie używanych przez Stringa.
 * @param str_ 			modyfikowany napis
 * */
void shrinkStringToFit(String* str_);
