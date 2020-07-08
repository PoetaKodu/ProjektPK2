#pragma once

#include <stdlib.h>
#include <inttypes.h>

typedef struct
{
	char* data;
	size_t len;
	size_t capacity;
} String;

String makeString();

String makeStringWith(char const* bytes_, size_t len_);

String wrapWithString(char *bytes_, size_t len_);

String wrapLiteralString(char const* literal_);

void destroyString(String* str_);

String copyString(String other_);

int64_t findWsInString(String const *str_, size_t startPos_);

int64_t findNonWsInString(String const *str_, size_t startPos_);

// Cut before `where` index and preserve the left part;
void cutStringLeft(String *str_, size_t where_);

void appendString(String* str_, String const* toAppend_);

void appendStringRaw(String* str_, char const* bytes_, size_t len_);

void reserveStringCapacity(String* str_, size_t newCapacity_);

void shrinkStringToFit(String* str_);
