#include <Filter/String.h>

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

//////////////////////////////////////////////////////////////
String makeString()
{
	String str;

	str.data = NULL;
	str.capacity = 0;
	str.len = 0;

	return str;
}

//////////////////////////////////////////////////////////////
String makeStringWith(char const* bytes_, size_t len_)
{
	String str;

	str.data = malloc(len_);
	strncpy(str.data, bytes_, len_);
	str.capacity = len_;
	str.len = len_;

	return str;
}

//////////////////////////////////////////////////////////////
String wrapWithString(char *bytes_, size_t len_)
{
	String str;

	str.data = bytes_;
	str.capacity = len_;
	str.len = len_;

	return str;
}

//////////////////////////////////////////////////////////////
String wrapLiteralString(char const* literal_)
{
	String str;

	str.data = literal_;
	str.capacity = strlen(literal_);
	str.len = str.capacity;

	return str;
}

//////////////////////////////////////////////////////////////
void destroyString(String* str_)
{
	assert(str_ != NULL);

	free(str_->data);
	str_->capacity = 0;
	str_->len = 0;
}

//////////////////////////////////////////////////////////////
String copyString(String other_)
{
	return makeStringWith(other_.data, other_.len);
}

//////////////////////////////////////////////////////////////
void cutStringLeft(String *str_, size_t where_)
{
	assert(str_ != NULL && where_ <= str_->len);

	str_->len = where_;
}

//////////////////////////////////////////////////////////////
int64_t findWsInString(String const *str_, size_t startPos_)
{
	assert(str_ != NULL);

	for(int64_t i = startPos_; i < (int64_t)str_->len; ++i)
	{
		if (isspace(str_->data[i]))
			return i;
	}
	return -1;
}

//////////////////////////////////////////////////////////////
int64_t findNonWsInString(String const *str_, size_t startPos_)
{
	assert(str_ != NULL);

	for(int64_t i = startPos_; i < (int64_t)str_->len; ++i)
	{
		char c = str_->data[i];
		int s = isspace((unsigned char)(c));
		if (!s)
			return i;
	}
	return -1;
}


//////////////////////////////////////////////////////////////
PairOfStrings splitString(String *src_, size_t where_)
{
	assert(src_ != NULL);


	PairOfStrings result;
	result.left = makeString();
	result.right = makeString();

	if (src_->len > 1)
	{
		reserveStringCapacity(&result.left, where_);
		reserveStringCapacity(&result.right, src_->len - where_);

		appendStringRaw(&result.left, src_->data, where_);
		appendStringRaw(&result.right, src_->data + where_, src_->len - where_);
	}

	return result;
}

//////////////////////////////////////////////////////////////
void appendString(String* str_, String const* toAppend_)
{
	assert(str_ != NULL && toAppend_ != NULL);

	size_t const reqCapacity = str_->len + toAppend_->len;
	if (str_->capacity < reqCapacity)
	{
		reserveStringCapacity(str_, reqCapacity);
	}

	for(size_t i = 0; i < toAppend_->len; ++i)
	{
		str_->data[i + str_->len] = toAppend_->data[i];
	}
	str_->len = reqCapacity;
}

//////////////////////////////////////////////////////////////
void appendStringRaw(String* str_, char const* bytes_, size_t len_)
{
	assert(str_ != NULL && bytes_ != NULL);

	String appended = wrapWithString(bytes_, len_);

	appendString(str_, &appended);
}


//////////////////////////////////////////////////////////////
void reserveStringCapacity(String* str_, size_t newCapacity_)
{
	assert(str_ != NULL);

	if (newCapacity_ > str_->capacity)
	{
		str_->data = realloc(str_->data, newCapacity_);
		str_->capacity = newCapacity_;
	}
}

//////////////////////////////////////////////////////////////
void shrinkStringToFit(String* str_)
{
	assert(str_ != NULL);

	if (str_->len > 0)
	{
		str_->data = realloc(str_->data, str_->len);
	}
	else
	{
		free(str_->data);
		str_->data = NULL;
	}
	str_->capacity = str_->len;
}