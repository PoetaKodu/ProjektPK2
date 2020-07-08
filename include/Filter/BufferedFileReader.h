#pragma once

#include <Filter/String.h>

#include <stdio.h>
#include <stdbool.h>

#define BFR_BUF_SIZE 10

typedef struct BufferedFileReader
{
	FILE* file;

	char buf[BFR_BUF_SIZE];
	size_t readCount;

} BufferedFileReader;


BufferedFileReader* bfrOpen(const char* path);

bool bfrReadNext(BufferedFileReader* reader_);

void bfrConsume(BufferedFileReader* reader_, size_t count_);

String bfrReadUntilWs(BufferedFileReader* reader);

void bfrClose(BufferedFileReader* reader);
