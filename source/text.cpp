#include "text.hpp"

namespace retxt
{

	void text::loadFromQueue( simpleQueue<word> & textBuffer )
	{
		this->WordCount = textBuffer.size();
		this->Words = new word[this->WordCount];
		for (size_t i = 0, size = textBuffer.size(); i < size; i++)
		{
			this->Words[i] = textBuffer.dequeue();
		}
	}
	
	void text::load( const byte * UTF8Text, const size_t textSize )
	{
		if (textSize == 0) return;
		simpleQueue<word> textBuffer;
		this->loadLeadingSpaces(UTF8Text, textSize);
		this->tokenizeAndEnqueue(UTF8Text, textSize, textBuffer);
		this->loadFromQueue(textBuffer);
	}
	
	void text::trimWordsFollowingSpaces( bool leaveOne )
	{
		for (size_t i = 0, count = this->WordCount, x; i < count; i++)
		{            
			x = this->Words[i].followingSpaces();
			if (x > 0)
			{
				this->CurrentSize -= (x - leaveOne);
				this->CharacterCount -= (x - leaveOne);
				this->Words[i].setFollowingSpaces(leaveOne);
			}
		}
	}
	
	text::text( const text & another ) :
		dataStructure<byte>(), Words(new word[another.WordCount]),
		CharacterCount(another.CharacterCount), WordCount(another.WordCount),
		Separator(another.Separator), LeadingSpaces(another.LeadingSpaces)
	{
		for (size_t i = 0, size = another.WordCount; i < size; i++)
			this->Words[i] = another.Words[i];
	}
	
	text::text( const byte * UTF8Text, const uint32_t spaceCharacter ) :
		dataStructure<byte>(), Words(nullptr),
		CharacterCount(0), WordCount(0), Separator(spaceCharacter), LeadingSpaces(0)
	{
		this->load(UTF8Text);
	}
	
	text::text( const byte * UTF8Text, const size_t textSize, const uint32_t spaceCharacter ) :
		dataStructure<byte>(), Words(nullptr),
		CharacterCount(0), WordCount(0), Separator(spaceCharacter), LeadingSpaces(0)
	{
		this->load(UTF8Text, textSize);
	}
	
	text::text( const char * UTF8Text, const uint32_t spaceCharacter ) :
		dataStructure<byte>(), Words(nullptr),
		CharacterCount(0), WordCount(0), Separator(spaceCharacter), LeadingSpaces(0)
	{
		this->load((const byte *)(UTF8Text));
	}
	
	text::text( const char * UTF8Text, const size_t textSize, const uint32_t spaceCharacter ) :
		dataStructure<byte>(), Words(nullptr),
		CharacterCount(0), WordCount(0), Separator(spaceCharacter), LeadingSpaces(0)
	{
		this->load(((const byte *)(UTF8Text)), textSize);
	}
	
	text::text( std::string UTF8Text, const uint32_t spaceCharacter ) :
		dataStructure<byte>(), Words(nullptr),
		CharacterCount(0), WordCount(0), Separator(spaceCharacter), LeadingSpaces(0)
	{
		this->load((const byte *)(UTF8Text.data()), UTF8Text.size());
	}
	
	text::~text()
	{
		this->clear();
	}
	
	uint32_t text::NthCharacter( const size_t N ) const
	{
		if (N >= this->CharacterCount) return 0;
		size_t i = 0;
		for (size_t size = this->WordCount; i < size; i++)
		{
			if (this->Words[i].PositionInText > N)
				return this->Words[i - 1].NthCharacter(N - this->Words[i - 1].PositionInText);
		}
		return this->Words[i - 1].NthCharacter(N - this->Words[i - 1].PositionInText);
	}
	
	size_t text::count( const uint32_t & whichCharacter ) const
	{
		size_t count = 0;
		const word * wordsOfThis = this->Words;
		if (whichCharacter == this->Separator)
		{
			for (size_t i = 0, size = this->WordCount; i < size; i++)
				count += this->Words[i].FollowingSpaces;
		}
		else
		{
			for (size_t i = 0, j, size = this->WordCount, wordSize; i < size; i++)
			{
				for (j = 0, wordSize = wordsOfThis[i].CharacterCount; j < wordSize; j++)
					if (wordsOfThis[i].Characters[j] == whichCharacter) count++;
			}
		}
		return count;
	}
        
	size_t text::smallestWordSize() const
	{
		if (this->WordCount == 0) return 0;
		size_t smallest = (size_t)(-1);
		for (size_t i = 0, size = this->WordCount; i < size; i++)
		{
			if (this->Words[i].CharacterCount < smallest)
				smallest = this->Words[i].CharacterCount;
		}
		return smallest;
	}
	
	size_t text::largestWordSize() const
	{
		if (this->WordCount == 0) return 0;
		size_t largest = 0;
		for (size_t i = 0, size = this->WordCount; i < size; i++)
		{
			if (this->Words[i].CharacterCount > largest)
				largest = this->Words[i].CharacterCount;
		}
		return largest;
	}
	
	double text::averageWordSize() const
	{
		if (this->WordCount == 0) return 0;
		size_t sum = 0;
		for (size_t i = 0, size = this->WordCount; i < size; i++)
			sum += this->Words[i].CharacterCount;
		return (((double)(sum))/((double)(this->WordCount)));
	}
	
	size_t text::mostFrequentWordSize( const bool preferTheSmaller ) const
	{
		if (this->WordCount == 0) return 0;
		else if (this->WordCount == 1) return this->Words[0].CharacterCount;
		uint16_t * sizes = new uint16_t[256]{}, largest = 0;
		for (size_t i = 0, value, size = this->WordCount; i < size; i++)
		{
			value = this->Words[i].CharacterCount;
			if (value < 255) sizes[this->Words[i].CharacterCount]++;
		}
		if (preferTheSmaller)
		{
			for (size_t i = 0; i <= 255; i++)
				if (sizes[i] > sizes[largest]) largest = i;
		}
		else
		{
			for (size_t i = 0; i <= 255; i++)
				if (sizes[i] >= sizes[largest]) largest = i;
		}
		return largest;
	}
	
	void text::trimFollowingSpaces()
	{
		if (this->WordCount == 0)
		{
			this->trimLeadingSpaces();
			return;
		}
		word & lastWord = this->Words[this->WordCount - 1];
		this->CharacterCount -= lastWord.followingSpaces();
		this->CurrentSize -= lastWord.followingSpaces();
		this->CurrentSize -= lastWord.followingSpaces();
		lastWord.setFollowingSpaces(0);
	}
	
	text::word * text::toArrayOfWords() const
	{
		word * array = new word[this->WordCount];
		for (size_t i = 0, size = this->WordCount; i < size; i++)
			array[i] = this->Words[i];
		return array;
	}
	
	uint32_t * * text::toArrayOfRawWords() const
	{
		uint32_t * * array = new uint32_t *[this->WordCount];
		for (size_t i = 0, j, size = this->WordCount, wordSize; i < size; i++)
		{
			wordSize = this->Words[i].CharacterCount;
			array[i] = new uint32_t[wordSize + 1];
			array[i][wordSize] = 0;
			for (j = 0; j < wordSize; j++)
				array[i][j] = this->Words[i][j];
		}
		return array;
	}
	
	byte * * text::toArrayOfUTF8Words() const
	{
		byte * * array = new byte *[this->WordCount];
		for (size_t i = 0, size = this->WordCount; i < size; i++)
			array[i] = this->Words[i].toUTF8();
		return array;
	}
	
	std::string * text::toArrayOfWordsAsUTF8Strings() const
	{
		std::string * array = new std::string[this->WordCount];
		for (size_t i = 0, size = this->WordCount; i < size; i++)
			array[i] = this->Words[i];
		return array;
	}
	
	byte * text::toUTF8( const bool zeroTerminated ) const
	{
		size_t byteCount = this->CurrentSize, wordCount = this->WordCount;
		size_t whichWord = 0, whichByte = 0, wordSize, i;
		byte * UTF8String = new byte[byteCount + zeroTerminated];
		for (i = this->LeadingSpaces; whichByte < i;)
			UTF8String[whichByte++] = this->Separator;
		for (word buffer; whichWord < wordCount; whichWord++)
		{
			buffer = this->Words[whichWord];
			for (i = 0, wordSize = buffer.characterCount(); i < wordSize; i++)
			{
				if (buffer[i] < 128) UTF8String[whichByte++] = buffer[i];
				else if (buffer[i] < 2048) 
				{
					UTF8String[whichByte++] = (((buffer[i] >> 6) & 0x1F) | 0xC0);
					UTF8String[whichByte++] = ((buffer[i] & 0x3F) | 0x80);
				}
				else if (buffer[i] < 65536) 
				{
					UTF8String[whichByte++] = (((buffer[i] >> 12) & 0xF) | 0xE0);
					UTF8String[whichByte++] = (((buffer[i] >> 6) & 0x3F) | 0x80);
					UTF8String[whichByte++] = ((buffer[i] & 0x3F) | 0x80);
				}
				else
				{
					UTF8String[whichByte++] = (((buffer[i] >> 18) & 0x7) | 0xF0);
					UTF8String[whichByte++] = (((buffer[i] >> 12) & 0x3F) | 0x80);
					UTF8String[whichByte++] = (((buffer[i] >> 6) & 0x3F) | 0x80);
					UTF8String[whichByte++] = ((buffer[i] & 0x3F) | 0x80);
				}
			}
			for (i = buffer.followingSpaces(); i > 0; i--)
				UTF8String[whichByte++] = this->Separator;
		}
		if (zeroTerminated) UTF8String[byteCount] = 0;
		return UTF8String;
	}
	
	int32_t * text::toUTF32( const bool zeroTerminated ) const
	{
		size_t characterCount = this->CharacterCount, wordCount = this->WordCount;
		size_t whichWord = 0, whichCharacter = 0, wordSize, i;
		int32_t * UTF32String = new int32_t[characterCount + zeroTerminated];
		for (i = this->LeadingSpaces; whichCharacter < i;)
			UTF32String[whichCharacter++] = this->Separator;
		for (word buffer; whichWord < wordCount; whichWord++)
		{
			buffer = this->Words[whichWord];
			for (i = 0, wordSize = buffer.characterCount(); i < wordSize; i++)
				UTF32String[whichCharacter++] = buffer[i];
			for (i = buffer.followingSpaces(); i > 0; i--)
				UTF32String[whichCharacter++] = this->Separator;
		}
		if (zeroTerminated) UTF32String[characterCount] = 0;
		return UTF32String;
	}
	
	std::string text::stringified() const
	{
		byte * array = this->toUTF8();
		std::string newString((char *)(array));
		delete[] array;
		return newString;
	}
        
	text & text::operator = ( const text & another )
	{
		this->CharacterCount = another.CharacterCount;
		this->CurrentSize = another.CurrentSize;
		this->WordCount = another.WordCount;
		this->LeadingSpaces = another.LeadingSpaces;
		this->Words = new word[another.WordCount];
		for (size_t i = 0, size = another.WordCount; i < size; i++)
			this->Words[i] = another.Words[i];
		return (*this);
	}
}
