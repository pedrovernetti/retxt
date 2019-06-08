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
					buffer.enqueue(characterFrom2UTF8Bytes(string, i));
				else if ((character <= 239) && ((i + 2) < stringSize))
					buffer.enqueue(characterFrom3UTF8Bytes(string, i));
				else if ((i + 3) < stringSize)
					buffer.enqueue(characterFrom4UTF8Bytes(string, i));
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
	
	void text::word::load( const uint32_t * string, const size_t stringSize )
	{
		size_t i;
		for (size_t i = 0; (i < stringSize) && (string[i] != this->Separator); i++)
		{
			this->Characters[i] = string[i];
			this->EncodedSize += UTF8EncodedSize(string[i]);
		}
		while (string[i++] == this->Separator) this->FollowingSpaces++;
		this->CharacterCount -= this->FollowingSpaces;
		this->EncodedSize += (this->FollowingSpaces * UTF8EncodedSize(this->Separator));
	}

	text::word::word( const uint32_t * UTF32String, 
					  const size_t stringSize,
					  const size_t positionInText,
					  const uint16_t followingSpaces,
					  const uint32_t spaceCharacter ) :
		Characters(new uint32_t[stringSize]), PositionInText(positionInText), 
		Separator(spaceCharacter), CharacterCount(stringSize),
		EncodedSize(0), FollowingSpaces(followingSpaces)
	{
		size_t i, L;
		for (i = 0, L = 0; i < stringSize; i++)
		{
			this->Characters[i] = UTF32String[i];
			this->EncodedSize += UTF8EncodedSize(this->Characters[i]);
			if (unicode::classOf(UTF32String[i]) & 0b00100001000100000000) L++; // TODO
		}
		this->EncodedSize += (this->FollowingSpaces * UTF8EncodedSize(this->Separator));
		if (L == i) this->Type = wordType::Literal; // TODO
	}
	
	text::word::word( const uint32_t character, 
					  const size_t positionInText,
		              const uint16_t followingSpaces,
					  const uint32_t spaceCharacter ) :
		Characters(new uint32_t[1]{ character }), PositionInText(positionInText), 
		Separator(spaceCharacter), CharacterCount(1),
		EncodedSize(1), FollowingSpaces(followingSpaces)
	{
	}
	
	text::word::word( const word & another ) :
		Characters(new uint32_t[another.CharacterCount]),
		PositionInText(another.PositionInText), Separator(another.Separator),
		CharacterCount(another.CharacterCount), EncodedSize(another.EncodedSize),
		FollowingSpaces(another.FollowingSpaces)
		, Type(another.Type) // TODO
	{
		for (size_t i = 0, size = another.CharacterCount; i < size; i++)
			this->Characters[i] = another.Characters[i];
	}

	text::word::word( const uint32_t spaceCharacter ) :
		Characters(nullptr), PositionInText(0), Separator(spaceCharacter),
		CharacterCount(0), EncodedSize(0), FollowingSpaces(0)
	{
	}
	
	text::word::word( const byte * UTF8Text, const uint32_t spaceCharacter ) :
		Characters(nullptr), PositionInText(0), Separator(spaceCharacter),
		CharacterCount(0), EncodedSize(0), FollowingSpaces(0)
	{
		this->load(UTF8Text);
	}
	
	text::word::word( const byte * UTF8Text, 
	                  const size_t textSize, 
	                  const uint32_t spaceCharacter ) :
		Characters(nullptr), PositionInText(0), Separator(spaceCharacter),
		CharacterCount(0), EncodedSize(0), FollowingSpaces(0)
	{
		this->load(UTF8Text, textSize);
	}
	
	text::word::word( const char * UTF8Text, const uint32_t spaceCharacter ) :
		Characters(nullptr), PositionInText(0), Separator(spaceCharacter),
		CharacterCount(0), EncodedSize(0), FollowingSpaces(0)
	{
		this->load((const byte *)(UTF8Text));
	}
	
	text::word::word( const char * UTF8Text, 
	                  const size_t textSize, 
	                  const uint32_t spaceCharacter ) :
		Characters(nullptr), PositionInText(0), Separator(spaceCharacter),
		CharacterCount(0), EncodedSize(0), FollowingSpaces(0)
	{
		this->load(((const byte *)(UTF8Text)), textSize);
	}
	
	text::word::word( std::string UTF8Text, const uint32_t spaceCharacter ) :
		Characters(nullptr), PositionInText(0), Separator(spaceCharacter),
		CharacterCount(0), EncodedSize(0), FollowingSpaces(0)
	{
		this->load(((const byte *)(UTF8Text.data())), UTF8Text.size());
	}
            
    text::word::word( const uint32_t * UTF32Text, 
					  const size_t textSize, 
					  const uint32_t spaceCharacter ) :
		Characters(new uint32_t[textSize]), PositionInText(0), Separator(spaceCharacter),
		CharacterCount(textSize), EncodedSize(0), FollowingSpaces(0)
	{
		this->load(UTF32Text, textSize);
	}
	
	text::word::~word()
	{
		this->clear();
	}

    bool text::word::startsWith( text::word prefix ) const
    {
		if (prefix.Characters[0] == 0) return true;
		if ((!this->CharacterCount) || (prefix.CharacterCount > this->CharacterCount)) return false;
		const size_t size = prefix.CharacterCount;
		for (size_t i = 0; i < size; i++)
		{
			if (this->Characters[i]  != prefix.Characters[i]) return false;
		}
		return true;
	}
            
    bool text::word::endsWith( text::word suffix ) const
    {
		if (suffix.Characters[0] == 0) return true;
		if ((!this->CharacterCount) || (suffix.CharacterCount > this->CharacterCount)) return false;
		const size_t dif = (this->CharacterCount - suffix.CharacterCount);
		for (size_t j = suffix.CharacterCount - 1; j > 0; j--)
		{
			if (this->Characters[j + dif] != suffix.Characters[j]) return false;
		}
		return true;
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
		this->Type = another.Type; // TODO
		return (*this);
	}

	text::word text::word::essentialCopy() const
	{
		word copy;
		copy.CharacterCount = this->CharacterCount;
		copy.EncodedSize = this->EncodedSize;
		copy.EncodedSize -= 
			(this->FollowingSpaces * UTF8EncodedSize(this->Separator));
		copy.Characters = new uint32_t[copy.CharacterCount];
		for (size_t i = 0, total = this->CharacterCount; i < total; i++)
			copy.Characters[i] = this->Characters[i];
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
			else if (characters[i] < 2048)
				characterTo2UTF8Bytes(characters[i], UTF8String, whichByte);
			else if (characters[i] < 65536)
				characterTo3UTF8Bytes(characters[i], UTF8String, whichByte);
			else
				characterTo4UTF8Bytes(characters[i], UTF8String, whichByte);
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

	uint32_t * text::decode( const byte * UTF8Text, const size_t textSize )
	{
		uint32_t * UTF32Text = new uint32_t[textSize];
		byte character;
		size_t decodedBytes = 0, decodedCharacters = 0;
		for (size_t i = 0; i < textSize; i++)
		{
			if (!(character = UTF8Text[i])) break;
			if ((character == 13) && isCloseToLF(UTF8Text, textSize, i)) continue;
			decodedBytes++;
			if ((character > 193) && (character < 245))
			{
				if ((character < 224) && ((i + 1) < textSize))
					UTF32Text[decodedCharacters++] = characterFrom2UTF8Bytes(UTF8Text, i);
				else if ((character < 240) && ((i + 2) < textSize))
					UTF32Text[decodedCharacters++] = characterFrom3UTF8Bytes(UTF8Text, i);
				else if ((i + 3) < textSize)
					UTF32Text[decodedCharacters++] = characterFrom4UTF8Bytes(UTF8Text, i);
			}
			else if (character < 128) UTF32Text[decodedCharacters++] = character;
		}
		this->EncodedSize = decodedBytes;
		this->CharacterCount = decodedCharacters;
		return UTF32Text;
	}
			
	void text::analyze( const uint32_t * UTF32Text, const size_t textSize )
	{
		uint32_t character;
		size_t characterCount = 0;
		for (size_t i = 0; i < textSize; i++)
		{
			if (!(character = UTF32Text[i])) break;
			if ((character == 13) && isCloseToLF(UTF32Text, textSize, i)) continue; // TODO
			characterCount++;
		}
		this->CharacterCount = characterCount;
	}
			
	void text::loadFromQueue( simpleQueue<word> & textBuffer )
	{
		if (textBuffer.tail().Characters && (textBuffer.tail().Characters[0] != 10)) 
		{
			textBuffer.enqueue(word(10, this->CharacterCount, 0, this->Separator));
			this->CharacterCount++;
			this->LineCount++;
		}
		this->WordCount = textBuffer.size();
		this->Words = new word[this->WordCount];
		this->LineBreaks = new size_t[this->LineCount];
		for (size_t i = 0, j = 0, size = textBuffer.size(); i < size; i++)
		{
			if (textBuffer.head()[0] == 10) this->LineBreaks[j++] = i;
			this->Words[i] = textBuffer.dequeue();
		}
	}
	
	void text::load( const byte * UTF8Text, size_t textSize )
	{
		if (textSize == 0) return;	
		if (textSize == ((size_t)(-1))) for (textSize = 0; UTF8Text[textSize]; textSize++);
		uint32_t * UTF32Text = this->decode(UTF8Text, textSize);			
		size_t i;
		const size_t limit = this->CharacterCount;
		for (i = 0; (UTF32Text[i] == this->Separator) && (i < limit); i++);
		this->LeadingSpaces = i;
		if (i == limit) return;
		simpleQueue<word> textBuffer;
		this->tokenize(UTF32Text, textBuffer);
		delete[] UTF32Text;
		this->loadFromQueue(textBuffer);
	}
	
	void text::load( const uint32_t * UTF32Text, size_t textSize )
	{
		if (textSize == 0) return;
		if (textSize == ((size_t)(-1))) for (textSize = 0; UTF32Text[textSize]; textSize++);
		this->analyze(UTF32Text, textSize);
		size_t i;
		const size_t limit = this->CharacterCount;
		for (i = 0; (UTF32Text[i] == this->Separator) && (i < limit); i++);
		this->LeadingSpaces = i;
		if (i == limit) return;
		simpleQueue<word> textBuffer;
		this->tokenize(UTF32Text, textBuffer);
		this->loadFromQueue(textBuffer);
	}
	
	void text::trimWordsFollowingSpaces( bool leaveOne )
	{
		size_t displacement = 0;
		for (size_t i = 0, count = this->WordCount; i < count; i++)
		{            
			this->Words[i].PositionInText -= displacement;
			if (this->Words[i].FollowingSpaces)
			{
				this->CharacterCount -= (this->Words[i].FollowingSpaces - leaveOne);
				displacement += (this->Words[i].FollowingSpaces - leaveOne);
				this->Words[i].setFollowingSpaces(leaveOne);
			}
		}
	}
	
	text::text( const text & another ) :
		Words(new word[another.WordCount]), LineBreaks(new size_t[another.LineCount]), 
		EncodedSize(another.EncodedSize), CharacterCount(another.CharacterCount),
		WordCount(another.WordCount), LineCount(another.LineCount),
		Separator(another.Separator), LeadingSpaces(another.LeadingSpaces),
		LineBreak(another.LineBreak)
	{
		for (size_t i = 0, size = another.WordCount; i < size; i++)
			this->Words[i] = another.Words[i];
		for (size_t i = 0, size = another.LineCount; i < size; i++)
			this->LineBreaks[i] = another.LineBreaks[i];
	}
	
	text::text( uint32_t spaceCharacter, lineBreakType lineBreak ) :
		Words(nullptr), LineBreaks(nullptr), EncodedSize(0),
		CharacterCount(0), WordCount(0), LineCount(0),
		Separator(spaceCharacter), LeadingSpaces(0), LineBreak(lineBreak)
	{
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
	
	text::text( const uint32_t * UTF32Text,
			    const size_t textSize,
			    const uint32_t spaceCharacter,
			    const lineBreakType lineBreak ) :
		Words(nullptr), LineBreaks(nullptr), EncodedSize(0), 
		CharacterCount(0), WordCount(0), LineCount(0),
		Separator(spaceCharacter), LeadingSpaces(0), LineBreak(lineBreak)
	{
		this->load(UTF32Text, textSize);
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
		size_t smallest = (size_t)(-1), current;
		for (size_t i = 0, size = this->WordCount; i < size; i++)
		{
			current = this->Words[i].CharacterCount;
			if ((current == 1) && (this->Words[i][0] == 10)) continue;
			if (current < smallest) smallest = current;
		}
		return smallest;
	}
	
	size_t text::largestWordSize() const
	{
		if (this->WordCount == 0) return 0;
		size_t largest = 0, current;
		for (size_t i = 0, size = this->WordCount; i < size; i++)
		{
			current = this->Words[i].CharacterCount;
			if ((current == 1) && (this->Words[i][0] == 10)) continue;
			if (current > largest) largest = current;
		}
		return largest;
	}
	
	double text::averageWordSize() const
	{
		if ((this->WordCount - this->LineCount) < 1) return 0;
		else if (this->WordCount == 1) return this->Words[0].CharacterCount;
		size_t sum = 0, current;
		for (size_t i = 0, size = this->WordCount; i < size; i++)
		{
			current = this->Words[i].CharacterCount;
			if ((current == 1) && (this->Words[i][0] == 10)) continue;
			sum += current;
		}
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
		const uint16_t leadingSpaces = this->LeadingSpaces;
		if (leadingSpaces == this->CharacterCount) 
		{
			this->clear();
			return;
		}
		this->CharacterCount -= leadingSpaces;
		this->EncodedSize = this->LeadingSpaces = 0;
		for (size_t i = 0, total = this->WordCount; i < total; i++)
		{
			this->Words[i].PositionInText -= leadingSpaces;
		}
	}
	
	void text::trimFollowingSpaces()
	{
		if (this->WordCount == 0)
		{
			this->clear();
			return;
		}
		word & lastWord = this->Words[this->WordCount - 1];
		this->CharacterCount -= lastWord.FollowingSpaces;
		lastWord.setFollowingSpaces(0);
		this->EncodedSize = 0;
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
	
	byte * * text::toArrayOfUTF8Lines() const // TODO: encoded size
	{
		simpleQueue<byte> buffer;
		byte * * array = new byte *[this->LineCount], * UTF8Word;
		size_t wordSize, bufferSize;
		for (size_t i = 0, j, k = 0, size = this->WordCount; i < size; i++)
		{
			wordSize = this->Words[i].encodedSize();
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
	
	std::string * text::toArrayOfLinesAsUTF8Strings() const // TODO: encoded size
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
	
	byte * text::toUTF8( const bool zeroTerminated ) const // TODO: encoded size
	{
		const size_t byteCount = 
			this->encodedSize() + ((this->encodedLineBreakSize() == 2) ? this->LineCount : 0);
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
				else if (buffer[i] < 2048)
					characterTo2UTF8Bytes(buffer[i], UTF8String, whichByte);
				else if (buffer[i] < 65536)
					characterTo3UTF8Bytes(buffer[i], UTF8String, whichByte);
				else
					characterTo4UTF8Bytes(buffer[i], UTF8String, whichByte);
			}
			for (i = buffer.followingSpaces(); i > 0; i--)
				UTF8String[whichByte++] = this->Separator;
		}
		if (zeroTerminated) UTF8String[whichByte] = 0;
		return UTF8String;
	}
	
	uint32_t * text::toUTF32( const bool zeroTerminated ) const // TODO: linebreaksize
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
        
    void text::refactor()
    {
	}
        
	text & text::operator () ( const text & another )
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
