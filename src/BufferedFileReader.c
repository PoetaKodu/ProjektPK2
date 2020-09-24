#include <Filter/BufferedFileReader.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>

BufferedFileReader* bfrOpen(const char* path)
{
	FILE* f = fopen(path, "r");
	
	BufferedFileReader* reader = NULL;	
	if (f)
	{
		reader = malloc(sizeof(BufferedFileReader));
		reader->file = f;
		reader->readCount = 0;
	} 
	return reader;
}

bool bfrReadNext(BufferedFileReader* reader_)
{
	assert(reader_ != NULL);

	reader_->readCount = fread(reader_->buf, 1, BFR_BUF_SIZE, reader_->file);
	return reader_->readCount == BFR_BUF_SIZE;
}

void bfrConsume(BufferedFileReader* reader_, size_t count_)
{
	assert(reader_ != NULL);

	if (count_ < reader_->readCount)
	{
		// Move not yet consumed bytes to the left;
		for(size_t i = count_; i < reader_->readCount; ++i)
		{
			reader_->buf[i - count_] = reader_->buf[i]; 
		}

		reader_->readCount -= count_;
	}
	else
	{
		reader_->readCount = 0;
	}
}

String bfrReadUntilWs(BufferedFileReader* reader)
{
	assert(reader != NULL);

	String result = makeString();

	int64_t sepIdx = -1;
	bool skippedEntireBuf;
	size_t iteration = 0;
	do
	{
		skippedEntireBuf = false;

		bool fileError = false;
		while (reader->readCount == 0 || fileError)
		{
			// Read next portion of memory
			bool succeeded = bfrReadNext(reader);

			// No more data to read.
			if (!succeeded && reader->readCount == 0)
			{
				fileError = true;
				break;
			}

			// Trim whitespaces on the left:
			if (iteration == 0)
			{
				String buf = wrapWithString(reader->buf, reader->readCount);
				int64_t nonWs = findNonWsInString(&buf, 0);
				bfrConsume(reader, nonWs > -1 ? nonWs : buf.len);
			}
		}
		if (fileError)
			break;

		String appended = wrapWithString(reader->buf, reader->readCount);
		sepIdx = findWsInString(&appended, 0);

		size_t appendedLength = sepIdx > -1 ? sepIdx : appended.len;
		appendStringRaw(&result, appended.data, appendedLength);
		
		int64_t nonWs = findNonWsInString(&appended, appendedLength);
		bfrConsume(reader, nonWs > -1 ? nonWs : appended.len);

		if (nonWs == -1 && sepIdx == 0 && result.len == 0)
			skippedEntireBuf = true;

		++iteration;
	}
	while(sepIdx == -1 || skippedEntireBuf);

	shrinkStringToFit(&result);
	return result;
}

void bfrClose(BufferedFileReader* reader)
{
	assert(reader != NULL);

	fclose(reader->file);

	free(reader);
}
