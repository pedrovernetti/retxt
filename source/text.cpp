#include "text.hpp"

namespace retxt
{

	void text::word::load( const byte * string, const size_t stringSize )
	{
		if (stringSize == 0) return;
		simpleQueue<uint32_t> buffer;
		size_t i = 0, spaces = 0, size = 0;
		uint32_t space = this->Separator;
		for (byte character; size < stringSize; i++, size++)
		{
			character = string[i];
			if ((character == 0) || (character == space)) break;
			this->CharacterCount++;
			if (character < 128) buffer.enqueue(character);
			else if ((character >= 194) && (character <= 240))
			{
				if ((character <= 223) && ((i + 1) < stringSize))
					buffer.enqueue(decoded2UTF8Bytes(string, i));
				else if ((character <= 239) && ((i + 2) < stringSize))
					buffer.enqueue(decoded3UTF8Bytes(string, i));
				else if ((i + 3) < stringSize)
					buffer.enqueue(decoded4UTF8Bytes(string, i));
			}
			else this->CharacterCount--;
		}
		for (; (string[i] == space) && (i <= stringSize); i++, spaces++, size++);
		this->Characters = new uint32_t[this->CharacterCount];
		this->FollowingSpaces = spaces;
		this->EncodedSize = size;
		for (i = 0, size = buffer.size(); i < size; i++)
			this->Characters[i] = buffer.dequeue();
	}

	text::word::word( simpleQueue<uint32_t> & buffer, 
					  const size_t positionInText,
					  const uint16_t encodedSize,
					  const uint16_t followingSpaces ) :
		Characters(new uint32_t[buffer.size()]), 
		PositionInText(positionInText), CharacterCount(buffer.size()),
		EncodedSize(encodedSize), FollowingSpaces(followingSpaces)
	{
		for (size_t i = 0, size = buffer.size(); i < size; i++)
			this->Characters[i] = buffer.dequeue();
	}
	
	text::word::word( const uint32_t character, 
		  const size_t positionInText,
		  const uint16_t followingSpaces ) :
		Characters(new uint32_t[1]{ character }), 
		PositionInText(positionInText), CharacterCount(1),
		EncodedSize(1), FollowingSpaces(followingSpaces)
	{
	}
	
	text::word::word( const word & another ) :
		Characters(new uint32_t[another.CharacterCount]),
		PositionInText(another.PositionInText), 
		CharacterCount(another.CharacterCount), 
		EncodedSize(another.EncodedSize),
		FollowingSpaces(another.FollowingSpaces)
	{
		for (size_t i = 0, size = another.CharacterCount; i < size; i++)
			this->Characters[i] = another.Characters[i];
	}

	text::word::word() :
		Characters(nullptr), PositionInText(0), 
		CharacterCount(0), EncodedSize(0), FollowingSpaces(0)
	{
	}
	
	text::word::word( const char * UTF8Text ) :
		Characters(nullptr), PositionInText(0), 
		CharacterCount(0), EncodedSize(0), FollowingSpaces(0)
	{
		this->load((const byte *)(UTF8Text));
	}
	
	text::word::word( const char * UTF8Text, const size_t textSize ) :
		Characters(nullptr), PositionInText(0), 
		CharacterCount(0), EncodedSize(0), FollowingSpaces(0)
	{
		this->load(((const byte *)(UTF8Text)), textSize);
	}
	
	text::word::word( std::string UTF8Text ) :
		Characters(nullptr), PositionInText(0), 
		CharacterCount(0), EncodedSize(0), FollowingSpaces(0)
	{
		this->load(((const byte *)(UTF8Text.data())), UTF8Text.size());
	}
	
	text::word::~word()
	{
		this->clear();
	}

	text::word & text::word::operator = ( const text::word & another )
	{
		this->clear();
		this->Characters = new uint32_t[another.CharacterCount];
		for (size_t i = 0, size = another.CharacterCount; i < size; i++)
			this->Characters[i] = another.Characters[i];
		this->Separator = another.Separator;
		this->CharacterCount = another.CharacterCount;
		this->EncodedSize = another.EncodedSize;
		this->PositionInText = another.PositionInText;
		this->FollowingSpaces = another.FollowingSpaces;
		return (*this);
	}

	byte * text::word::toUTF8( const bool zeroTerminated ) const
	{
		if (this->EncodedSize < 1) return new byte[1]{};
		size_t byteCount = this->EncodedSize, whichByte, size, i;
		byte * UTF8String = new byte[byteCount + zeroTerminated];
		uint32_t * characters = this->Characters;
		for (whichByte = 0, i = 0, size = this->CharacterCount; i < size; i++)
		{
			if (characters[i] < 128) UTF8String[whichByte++] = characters[i];
			else 
			{
				if (buffer[i] > 65535)
					UTF8String[whichByte++] = (((characters[i] >> 18) & 0x7) | 0xF0);
				if (buffer[i] > 2047)
					UTF8String[whichByte++] = (((characters[i] >> 12) & 0xF) | 0xE0);
				UTF8String[whichByte++] = (((characters[i] >> 6) & 0x1F) | 0xC0);
				UTF8String[whichByte++] = ((characters[i] & 0x3F) | 0x80);
			}
		}
		for (size += this->FollowingSpaces; i < size; i++)
			UTF8String[whichByte++] = this->Separator;
		if (zeroTerminated) UTF8String[byteCount] = 0;
		return UTF8String;
	}
	
	uint32_t * text::word::toUTF32( const bool zeroTerminated ) const
	{
		if (this->EncodedSize < 1) return new uint32_t[1]{};
		size_t characterCount = this->CharacterCount + this->FollowingSpaces, i;
		size_t size = this->CharacterCount;
		uint32_t * UTF32String = new uint32_t[characterCount + zeroTerminated];
		uint32_t * characters = this->Characters;                
		for (i = 0; i < size; i++) UTF32String[i] = characters[i];                    
		for (size += this->FollowingSpaces; i < size; i++)
			UTF32String[i++] = this->Separator;                    
		if (zeroTerminated) UTF32String[characterCount] = 0;
		return UTF32String;
	}
	
	std::string text::word::stringified() const
	{
		byte * array = this->toUTF8();
		std::string newString((char *)(array));
		delete[] array;
		return newString;
	}

	void text::loadLeadingSpaces( const byte * UTF8Text, const size_t textSize )
	{
		size_t i, limit = (textSize < (uint16_t)(-1)) ? textSize : (uint16_t)(-1);
		const uint32_t space = this->Separator;
		for (i = 0; (UTF8Text[i] == space) && (i < limit); i++);
		this->LeadingSpaces = i;
		this->CharacterCount = this->LeadingSpaces;
	}
	
	void text::insertFinalLineBreakIfNeeded( simpleQueue<word> textBuffer )
	{
		if (textBuffer.tail().Characters && (textBuffer.tail().Characters[0] != 10)) 
		{
			textBuffer.enqueue(word(10, this->CharacterCount, 0));
			this->CharacterCount++;
			this->LineCount++;
		}
	}

	void text::tokenizeAndEnqueue( const byte * UTF8Text, 
								   const size_t textSize,
								   simpleQueue<word> & textBuffer )
	{
		simpleQueue<uint32_t> buffer;
		size_t i = this->LeadingSpaces;
		size_t spaces, size = i, position = i, positionInBytes = i;
		const uint32_t space = this->Separator;                
		for (byte character, previous = 0; size < textSize; i++, previous = character)
		{
			if ((UTF8Text[i] == 13) && isCloseToLF(UTF8Text, textSize, i)) continue;
			character = UTF8Text[i];
			if ((character == space) || (character == 10) || (previous == 10))
			{
				if (previous == 10) this->LineCount++;
				for (spaces = 0; (UTF8Text[i] == space) && (i <= textSize); i++, spaces++);
				this->CharacterCount += spaces;
				size += spaces;             
				textBuffer.enqueue(word(buffer, position, (size - positionInBytes), spaces));
				character = UTF8Text[i];
				position = this->CharacterCount;
				positionInBytes = size;
			}
			if (character == 0) break;
			size++;
			if (character < 128) buffer.enqueue(character);
			else if ((character >= 194) && (character <= 240))
			{
				if ((character <= 223) && ((i + 1) < textSize))
					buffer.enqueue(decoded2UTF8Bytes(UTF8Text, i));
				else if ((character <= 239) && ((i + 2) < textSize))
					buffer.enqueue(decoded3UTF8Bytes(UTF8Text, i));
				else if ((i + 3) < textSize)
					buffer.enqueue(decoded4UTF8Bytes(UTF8Text, i));
			}
			else continue;
			this->CharacterCount++;
		}
		this->insertFinalLineBreakIfNeeded(textBuffer);
		this->EncodedSize = size;
	}
	
	void text::loadFromQueue( simpleQueue<word> & textBuffer )
	{
		this->WordCount = textBuffer.size();
		this->Words = new word[this->WordCount];
		this->LineBreaks = new size_t[this->LineCount];
		for (size_t i = 0, j = 0, size = textBuffer.size(); i < size; i++)
		{
			if (textBuffer.head()[0] == 10) this->LineBreaks[j++] = i;
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
				this->EncodedSize -= (x - leaveOne);
				this->CharacterCount -= (x - leaveOne);
				this->Words[i].setFollowingSpaces(leaveOne);
			}
		}
	}
	
	text::text( const text & another ) :
		Words(new word[another.WordCount]), LineBreaks(new size_t[another.LineCount]), 
		EncodedSize(another.EncodedSize),
		CharacterCount(another.CharacterCount), WordCount(another.WordCount),
		LineCount(another.LineCount),
		Separator(another.Separator), LeadingSpaces(another.LeadingSpaces),
		LineBreak(another.LineBreak)
	{
		for (size_t i = 0, size = another.WordCount; i < size; i++)
			this->Words[i] = another.Words[i];
		for (size_t i = 0, size = another.LineCount; i < size; i++)
			this->LineBreaks[i] = another.LineBreaks[i];
	}
	
	text::text( const byte * UTF8Text, 
	            const uint32_t spaceCharacter, 
			    const lineBreakType lineBreak ) :
		Words(nullptr), LineBreaks(nullptr), EncodedSize(0), 
		CharacterCount(0), WordCount(0), LineCount(0), 
		Separator(spaceCharacter), LeadingSpaces(0), LineBreak(lineBreak)
	{
		this->load(UTF8Text);
	}
	
	text::text( const byte * UTF8Text, 
	            const size_t textSize, 
	            const uint32_t spaceCharacter, 
			    const lineBreakType lineBreak ) :
		Words(nullptr), LineBreaks(nullptr), EncodedSize(0), 
		CharacterCount(0), WordCount(0), LineCount(0), 
		Separator(spaceCharacter), LeadingSpaces(0), LineBreak(lineBreak)
	{
		this->load(UTF8Text, textSize);
	}
	
	text::text( const char * UTF8Text, 
	            const uint32_t spaceCharacter, 
			    const lineBreakType lineBreak ) :
		Words(nullptr), LineBreaks(nullptr), EncodedSize(0), 
		CharacterCount(0), WordCount(0), LineCount(0), 
		Separator(spaceCharacter), LeadingSpaces(0), LineBreak(lineBreak)
	{
		this->load((const byte *)(UTF8Text));
	}
	
	text::text( const char * UTF8Text, 
	            const size_t textSize, 
	            const uint32_t spaceCharacter, 
			    const lineBreakType lineBreak ) :
		Words(nullptr), LineBreaks(nullptr), EncodedSize(0), 
		CharacterCount(0), WordCount(0), LineCount(0), 
		Separator(spaceCharacter), LeadingSpaces(0), LineBreak(lineBreak)
	{
		this->load(((const byte *)(UTF8Text)), textSize);
	}
	
	text::text( std::string UTF8Text, 
	            const uint32_t spaceCharacter, 
			    const lineBreakType lineBreak ) :
		Words(nullptr), LineBreaks(nullptr), EncodedSize(0), 
		CharacterCount(0), WordCount(0), LineCount(0), 
		Separator(spaceCharacter), LeadingSpaces(0), LineBreak(lineBreak)
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
		if ((whichCharacter == 10) ||
			((whichCharacter == 13) && (this->LineBreak | lineBreakType::CRLineBreak)))
			return this->LineCount;
		size_t count = 0;
		const word * wordsOfThis = this->Words;
		if (whichCharacter == this->Separator)
		{
			count += this->LeadingSpaces;
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
		else if (this->WordCount == 1) return this->Words[0].CharacterCount;
		size_t sum = 0;
		for (size_t i = 0, size = this->WordCount; i < size; i++)
			sum += this->Words[i].CharacterCount;
		return (((double)(sum))/((double)(this->WordCount)));
	}
	
	size_t text::smallestLineSize( const bool considerEmptyLines ) const
	{
		if (!(this->LineCount)) return this->CharacterCount;
		const size_t lineCount = this->LineCount;
		size_t smallest = (size_t)(-1), current = 0;
		for (size_t i = 0, beg = 0, end = 0; i < lineCount; i++, beg = end)
		{
			end = this->LineBreaks[i];
			current = this->Words[end].PositionInText;
			current -= (this->Words[beg].PositionInText + 1);
			if (!(considerEmptyLines || current)) continue;
			if (current <= smallest) smallest = current;
		}
		return smallest;
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

	size_t text::largestLineSize() const
	{
		if (!(this->LineCount)) return this->CharacterCount;
		const size_t lineCount = this->LineCount;
		size_t largest = 0, current = 0;
		for (size_t i = 0, beg = 0, end = 0; i < lineCount; i++, beg = end)
		{
			end = this->LineBreaks[i];
			current = this->Words[end].PositionInText;
			current -= (this->Words[beg].PositionInText + 1);
			if (current >= largest) largest = current;
		}
		return largest;
	}
	
	void text::trimLeadingSpaces()
	{
		if (this->LeadingSpaces == this->CharacterCount) this->clear();
		uint16_t leadingSpaces = this->LeadingSpaces;
		this->LeadingSpaces = 0;
		this->CharacterCount -= leadingSpaces;
		this->EncodedSize -= leadingSpaces;
		for (size_t i = 0, total = this->WordCount; i < total; i++)
		{
			this->Words[i].PositionInText -= leadingSpaces;
		}
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
		this->EncodedSize -= lastWord.followingSpaces();
		this->EncodedSize -= lastWord.followingSpaces();
		lastWord.setFollowingSpaces(0);
	}
	
	text::word * text::toArrayOfWords() const
	{
		word * array = new word[this->WordCount];
		for (size_t i = 0, size = this->WordCount; i < size; i++)
			array[i] = this->Words[i];
		return array;
	}
	
	uint32_t * * text::toArrayOfUTF32Words() const
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
	
	uint32_t * * text::toArrayOfUTF32Lines() const
	{
		simpleQueue<uint32_t> buffer;
		uint32_t * * array = new uint32_t *[this->LineCount], * wordData;
		size_t wordSize, bufferSize;
		for (size_t i = 0, j, k = 0, size = this->WordCount; i < size; i++)
		{
			wordSize = this->Words[i].CharacterCount;
			wordData = this->Words[i].Characters;
			if ((wordSize) && (wordData[0] == 10))
			{
				bufferSize = buffer.size();
				array[k] = new uint32_t[bufferSize + 1];
				array[k][bufferSize] = 0;
				for (j = 0; j < bufferSize; j++) array[k][j] = buffer.dequeue();
				k++;
			}
			else
			{
				for (j = 0; j < wordSize; j++) buffer.enqueue(wordData[j]);
				for (j = this->Words[i].FollowingSpaces; j > 0; j--) buffer.enqueue(this->Separator);
			}
		}
		return array;
	}
	
	byte * * text::toArrayOfUTF8Lines() const
	{
		simpleQueue<byte> buffer;
		byte * * array = new byte *[this->LineCount], * UTF8Word;
		size_t wordSize, bufferSize;
		for (size_t i = 0, j, k = 0, size = this->WordCount; i < size; i++)
		{
			wordSize = this->Words[i].EncodedSize;
			if ((wordSize) && (this->Words[i].Characters[0] == 10))
			{
				bufferSize = buffer.size();
				array[k] = new byte[bufferSize + 1];
				array[k][bufferSize] = 0;
				for (j = 0; j < bufferSize; j++) array[k][j] = buffer.dequeue();
				k++;
			}
			else
			{
				UTF8Word = this->Words[i].toUTF8(false);
				for (j = 0; j < wordSize; j++) buffer.enqueue(UTF8Word[j]);
				delete[] UTF8Word;
			}
		}
		return array;
	}
	
	std::string * text::toArrayOfLinesAsUTF8Strings() const
	{
		simpleQueue<byte> buffer;
		std::string * array = new std::string[this->LineCount];
		byte * UTF8Word;
		size_t wordSize, bufferSize;
		for (size_t i = 0, j, k = 0, size = this->WordCount; i < size; i++)
		{
			wordSize = this->Words[i].EncodedSize;
			if ((wordSize) && (this->Words[i].Characters[0] == 10))
			{
				bufferSize = buffer.size();
				array[k].reserve(bufferSize);
				for (j = 0; j < bufferSize; j++) array[k].push_back(buffer.dequeue());
				k++;
			}
			else
			{
				UTF8Word = this->Words[i].toUTF8(false);
				for (j = 0; j < wordSize; j++) buffer.enqueue(UTF8Word[j]);
				delete[] UTF8Word;
			}
		}
		return array;
	}
	
	byte * text::toUTF8( const bool zeroTerminated ) const
	{
		const size_t byteCount = 
			this->EncodedSize + ((this->encodedLineBreakSize() == 2) ? this->LineCount : 0);
		const size_t wordCount = this->WordCount;
		size_t whichWord = 0, whichByte = 0, wordSize, i = this->LeadingSpaces;
		byte * UTF8String = new byte[byteCount + zeroTerminated];
		while (whichByte < i) UTF8String[whichByte++] = this->Separator;
		for (word buffer; whichWord < wordCount; whichWord++)
		{
			buffer = this->Words[whichWord];
			for (i = 0, wordSize = buffer.characterCount(); i < wordSize; i++)
			{
				if (buffer[i] == 10) pushLineBreak(this->LineBreak, UTF8String, whichByte);
				else if (buffer[i] < 128) UTF8String[whichByte++] = buffer[i];
				else
				{
					if (buffer[i] > 65535)
						UTF8String[whichByte++] = (((buffer[i] >> 18) & 0x7) | 0xF0);
					if (buffer[i] > 2047)
						UTF8String[whichByte++] = (((buffer[i] >> 12) & 0xF) | 0xE0);
					UTF8String[whichByte++] = (((buffer[i] >> 6) & 0x1F) | 0xC0);
					UTF8String[whichByte++] = ((buffer[i] & 0x3F) | 0x80);
				}
			}
			for (i = buffer.followingSpaces(); i > 0; i--)
				UTF8String[whichByte++] = this->Separator;
		}
		if (zeroTerminated) UTF8String[whichByte] = 0;
		return UTF8String;
	}
	
	uint32_t * text::toUTF32( const bool zeroTerminated ) const
	{
		const size_t characterCount = 
			this->CharacterCount + ((this->encodedLineBreakSize() == 2) ? this->LineCount : 0);
		const size_t wordCount = this->WordCount;
		size_t whichWord = 0, whichCharacter = 0, wordSize, i;
		uint32_t * UTF32String = new uint32_t[characterCount + zeroTerminated];
		for (i = this->LeadingSpaces; whichCharacter < i;)
			UTF32String[whichCharacter++] = this->Separator;
		for (word buffer; whichWord < wordCount; whichWord++)
		{
			buffer = this->Words[whichWord];
			if (buffer[0] == 10) 
				pushLineBreak(this->LineBreak, UTF32String, whichCharacter);
			else
			{
				for (i = 0, wordSize = buffer.characterCount(); i < wordSize; i++)
					UTF32String[whichCharacter++] = buffer[i];
			}
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
		this->EncodedSize = another.EncodedSize;
		this->WordCount = another.WordCount;
		this->LineCount = another.LineCount;
		this->LeadingSpaces = another.LeadingSpaces;
		this->Words = new word[another.WordCount];
		for (size_t i = 0, size = another.WordCount; i < size; i++)
			this->Words[i] = another.Words[i];
		this->LineBreaks = new size_t[another.LineCount];
		for (size_t i = 0, size = another.LineCount; i < size; i++)
			this->LineBreaks[i] = another.LineBreaks[i];
		this->Separator = another.Separator;
		this->LineBreak = another.LineBreak;
		return (*this);
	}
}
