#pragma once
#ifndef _RETXT_TEXT_INCLUDED
#define _RETXT_TEXT_INCLUDED

#include "globalUtilities.hpp"
#include "basicDataStructures.hpp"

namespace // internal parts
{
    inline uint32_t decoded2UTF8Bytes( const byte * bytes, const size_t position )
    {
        return (((bytes[position] & 0x1F) << 6) | 
                ((bytes[position + 1]) & 0x3F));
    }
    
    inline uint32_t decoded3UTF8Bytes( const byte * bytes, const size_t position )
    {
        return (((bytes[position] & 0xF) << 12) | 
                ((bytes[position + 1] & 0x3F) <<  6) | 
                (bytes[position + 2] & 0x3F));
    }
    
    inline uint32_t decoded4UTF8Bytes( const byte * bytes, const size_t position )
    {
        return (((bytes[position] & 0x7) << 18) | 
                ((bytes[position + 1] & 0x3F) << 12) | 
                ((bytes[position + 2] & 0x3F) <<  6) | 
                (bytes[position + 3] & 0x3F));
    }
    
    inline bool isCloseToLF( const byte * bytes, 
							 const size_t totalBytes, 
							 const size_t position )
    {
        return ((((position + 1) < totalBytes) && (bytes[position + 1] == 10)) ||
                ((position > 0) && (bytes[position + 1] == 10)));
	}
}

namespace retxt
{    
    class text : 
        public dataStructure<byte>,
        public stringifiable
    {
        public:
        
        class word : public stringifiable
        {
			friend class text;
			
            protected:
            
            uint32_t * Characters;
            size_t PositionInText;
            uint32_t Separator =  32;
            uint16_t CharacterCount;
            uint16_t EncodedSize;
            byte FollowingSpaces;
            
            void load( const byte * string, const size_t stringSize )
            {
                using namespace unicode::UTF8;
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
        
            inline void load( const byte * UTF8Text )
            {
                load(UTF8Text, ((size_t)(-1)));
            }
            
            word( simpleQueue<uint32_t> & buffer, 
                  const size_t positionInText,
                  const uint16_t encodedSize,
                  const uint16_t followingSpaces );
            
            word( const uint32_t character, 
                  const size_t positionInText,
                  const uint16_t followingSpaces );
            
            public:
            
            word( const word & another );
            
            word();
            
            word( const char * UTF8Text );
            
            word( const char * UTF8Text, const size_t textSize );
            
            word( std::string UTF8Text );
            
            ~word();
            
            void clear()
            {
                if (this->Characters != nullptr) delete[] this->Characters;
                this->Characters = nullptr;
                this->CharacterCount = this->EncodedSize = this->FollowingSpaces = 0;
                this->PositionInText = 0;
            }
            
            inline size_t characterCount() const
            {
                return this->CharacterCount;
            }
            
            inline size_t encodedSize() const
            {
                return this->EncodedSize;
            }
            
            inline size_t positionInText() const
            {
                return this->PositionInText;
            }
            
            inline int16_t followingSpaces() const
            {
                return this->FollowingSpaces;
            }
            
            inline void setFollowingSpaces( const int16_t amount )
            {
                this->FollowingSpaces = amount;
            }
            
            inline void trimFollowingSpaces( const bool leaveOne = true )
            {
                this->FollowingSpaces = leaveOne;
            }
                
            inline uint32_t NthCharacter( const size_t N ) const
            {
                if (N >= this->CharacterCount) 
                {
                    if (N >= (this->CharacterCount + this->FollowingSpaces)) return 0;
                    else return this->Separator;
                }
                return this->Characters[N];
            }
            
            inline void toUppercase()
            {
                for (size_t i = 0, size = this->CharacterCount; i < size; i++)
                    this->Characters[i] = unicode::uppercase(this->Characters[i]);
            }
            
            inline void toLowercase()
            {
                for (size_t i = 0, size = this->CharacterCount; i < size; i++)
                    this->Characters[i] = unicode::lowercase(this->Characters[i]);
            }
            
            byte * toUTF8( const bool zeroTerminated = true ) const
            {
                if (this->EncodedSize < 1) return new byte[1]{};
                size_t byteCount = this->EncodedSize, whichByte, size, i;
                byte * UTF8String = new byte[byteCount + zeroTerminated];
                uint32_t * characters = this->Characters;
                for (whichByte = 0, i = 0, size = this->CharacterCount; i < size; i++)
                {
                    if (characters[i] < 128) UTF8String[whichByte++] = characters[i];
                    else if (characters[i] < 2048) 
                    {
                        UTF8String[whichByte++] = (((characters[i] >> 6) & 0x1F) | 0xC0);
                        UTF8String[whichByte++] = ((characters[i] & 0x3F) | 0x80);
                    }
                    else if (characters[i] < 65536) 
                    {
                        UTF8String[whichByte++] = (((characters[i] >> 12) & 0xF) | 0xE0);
                        UTF8String[whichByte++] = (((characters[i] >> 6) & 0x3F) | 0x80);
                        UTF8String[whichByte++] = ((characters[i] & 0x3F) | 0x80);
                    }
                    else
                    {
                        UTF8String[whichByte++] = (((characters[i] >> 18) & 0x7) | 0xF0);
                        UTF8String[whichByte++] = (((characters[i] >> 12) & 0x3F) | 0x80);
                        UTF8String[whichByte++] = (((characters[i] >> 6) & 0x3F) | 0x80);
                        UTF8String[whichByte++] = ((characters[i] & 0x3F) | 0x80);
                    }
                }
                for (size += this->FollowingSpaces; i < size; i++)
                    UTF8String[whichByte++] = this->Separator;
                if (zeroTerminated) UTF8String[byteCount] = 0;
                return UTF8String;
            }
            
            int32_t * toUTF32( const bool zeroTerminated = true ) const
            {
                if (this->EncodedSize < 1) return new int32_t[1]{};
                size_t characterCount = this->CharacterCount + this->FollowingSpaces, i;
                size_t size = this->CharacterCount;
                int32_t * UTF32String = new int32_t[characterCount + zeroTerminated];
                uint32_t * characters = this->Characters;                
                for (i = 0; i < size; i++) UTF32String[i] = characters[i];                    
                for (size += this->FollowingSpaces; i < size; i++)
                    UTF32String[i++] = this->Separator;                    
                if (zeroTerminated) UTF32String[characterCount] = 0;
                return UTF32String;
            }
            
            inline byte * toArray() const
            {
                return this->toUTF8();
            }
            
            inline operator byte * () const
            {
                return this->toUTF8();
            }
            
            std::string stringified() const
            {
                byte * array = this->toUTF8();
                std::string newString((char *)(array));
                delete[] array;
                return newString;
            }
            
            inline uint32_t & operator [] ( const size_t position ) const
            {
                return this->Characters[position];
            }
            
            inline word & operator () ( const char * UTF8Text )
            {
                this->clear();
                this->load((const byte *)(UTF8Text));
                return (*this);
            }
            
            inline word & operator () ( std::string UTF8Text )
            {
                this->clear();
                this->load((const byte *)(UTF8Text.data()), UTF8Text.size());
                return (*this);
            }
            
            word & operator = ( const word & another );
            
            inline word & operator = ( const char * UTF8Text )
            {
                this->clear();
                this->load((const byte *)(UTF8Text));
                return (*this);
            }
        
            inline word & operator = ( std::string UTF8Text )
            {
                this->clear();
                this->load((const byte *)(UTF8Text.data()), UTF8Text.size());
                return (*this);
            }
        };
        
        protected:
        
        word * Words;
        
        size_t * LineBreaks;
        
        size_t CharacterCount;
        size_t WordCount;
        size_t LineCount;
        
        uint32_t Separator;
        
        uint16_t LeadingSpaces;
        
        lineBreakType LineBreak;
        
        void pushProperLineBreak( byte * UTF8String, size_t & position ) const;
        
        void loadLeadingSpaces( const byte * UTF8Text, const size_t textSize );
        
        void tokenizeAndEnqueue( const byte * UTF8Text, 
                                 const size_t textSize,
                                 simpleQueue<word> & textBuffer );
        
        void loadFromQueue( simpleQueue<word> & textBuffer );
        
        void load( const byte * UTF8Text, const size_t textSize );
        
        inline void load( const byte * UTF8Text )
        {
            this->load(UTF8Text, ((size_t)(-1)));
        }
        
        void trimWordsFollowingSpaces( bool leaveOne );
        
        public:
        
        text( const text & another );
        
        text( const byte * UTF8Text, 
			  const uint32_t spaceCharacter = 32, 
			  const lineBreakType lineBreak = lineBreakType::CRLFLineBreak );
        
        text( const byte * UTF8Text, 
              const size_t textSize, 
              const uint32_t spaceCharacter = 32, 
			  const lineBreakType lineBreak = lineBreakType::CRLFLineBreak );
        
        text( const char * UTF8Text, 
			  const uint32_t spaceCharacter = 32, 
			  const lineBreakType lineBreak = lineBreakType::CRLFLineBreak );
        
        text( const char * UTF8Text, 
			  const size_t textSize, 
			  const uint32_t spaceCharacter = 32, 
			  const lineBreakType lineBreak = lineBreakType::CRLFLineBreak );
        
        text( std::string UTF8Text, 
			  const uint32_t spaceCharacter = 32, 
			  const lineBreakType lineBreak = lineBreakType::CRLFLineBreak );
        
        ~text();
        
        inline void clear()
        {
            if (this->Words != nullptr) delete[] this->Words;
            this->Words = nullptr;
            this->CurrentSize = this->CharacterCount = this->WordCount = 0;
            this->LeadingSpaces = 0;
        }
        
        inline size_t encodedSize() const
        {
            return this->CurrentSize;
        }
        
        inline size_t encodedLineBreakSize() const
        {
			if ((this->LineBreak == lineBreakType::CRLFLineBreak) ||
				(this->LineBreak == lineBreakType::LFCRLineBreak))
				return 2;
            return 1;
        }
        
        inline size_t decodedSize() const
        {
            return (this->CharacterCount * sizeof(uint32_t));
        }
        
        inline size_t characterCount() const
        {
            return this->CharacterCount;
        }
        
        inline size_t wordCount() const
        {
            return this->WordCount;
        }
        
        inline size_t lineCount() const
        {
			return this->LineCount;
		}
        
        inline word firstWord() const
        {
            if (this->WordCount == 0) return word();
            return this->Words[0];
        }
        
        inline word lastWord() const
        {
            if (this->WordCount == 0) return word();
            return this->Words[this->WordCount - 1];
        }
        
        uint32_t NthCharacter( const size_t N ) const;
        
        inline word NthWord( const size_t N ) const
        {
            if (this->WordCount <= N) return word();
            return this->Words[N];
        }
        
        size_t count( const uint32_t & whichCharacter ) const;
        
        size_t smallestWordSize() const;
        
        size_t largestWordSize() const;
        
        double averageWordSize() const;
        
        size_t mostFrequentWordSize( const bool preferTheSmaller = false ) const;
        
        /*word * NLargestWords( const size_t N ) const
        {
            simpleQueue<word> words;
        }
        
        word * NMostFrequentWords( const size_t N ) const
        {
            simpleQueue<word> word
        }
        
        inline pair<uint32_t, size_t> * charactersFrequency() const
        {
            
        }
        
        inline pair<word, size_t> * wordsFrequency() const
        {
        }
        
        inline void push( const word pushedWord )
        {
        }
        
        inline void push( const std::string pushedWord )
        {
        }
        
        inline void push( const char * pushedWord )
        {
        }
        
        inline void push( const char * pushedWord, const size_t wordSize )
        {
        }
        
        inline word pop( const word pushedWord )
        {
        }*/
        
        inline void toUppercase()
        {
            if (this->WordCount == 0) return;
            for (size_t i = this->WordCount - 1; i >= 0; i--)
                this->Words[i].toUppercase();
        }
        
        inline void toLowercase()
        {
            if (this->WordCount == 0) return;
            for (size_t i = this->WordCount - 1; i >= 0; i--)
                this->Words[i].toLowercase();
        }
        
        inline uint32_t spaceCharacter() const
        {
            return this->Separator;
        }
        
        inline uint32_t setSpaceCharacter( const uint32_t character )
        {
            return this->Separator = character;
        }
        
        inline uint32_t lineBreak() const
        {
			return this->LineBreak;
		}
        
        inline uint32_t setLineBreak( const lineBreakType lineBreak )
        {
            return this->LineBreak = lineBreak;
        }
        
        inline void trimLeadingSpaces()
        {
            this->CharacterCount -= this->LeadingSpaces;
            this->CurrentSize -= this->LeadingSpaces;
            this->LeadingSpaces = 0;
        }
        
        void trimFollowingSpaces();
        
        inline void trimSpaces()
        {
            this->trimLeadingSpaces();
            this->trimWordsFollowingSpaces(true);
        }
        
        inline void removeSpaces()
        {
            this->trimLeadingSpaces();
            this->trimWordsFollowingSpaces(false);
        }
        
        word * toArrayOfWords() const;
        
        uint32_t * * toArrayOfUTF32Words() const;
        
        byte * * toArrayOfUTF8Words() const;
        
        std::string * toArrayOfWordsAsUTF8Strings() const;
        
        uint32_t * * toArrayOfUTF32Lines() const;
        
        byte * * toArrayOfUTF8Lines() const;
        
		std::string * toArrayOfLinesAsUTF8Strings() const;
	
        byte * toUTF8( const bool zeroTerminated = true ) const;
        
        int32_t * toUTF32( const bool zeroTerminated = true ) const;
        
        inline byte * toArray() const
        {
            return this->toUTF8();
        }
        
        std::string stringified() const;
        
        inline word operator [] ( const size_t N ) const
        {
            return this->Words[N];
        }
        
        inline text & operator () ( const char * UTF8Text )
        {
            this->clear();
            this->load((const byte *)(UTF8Text));
            return (*this);
        }
        
        inline text & operator () ( std::string UTF8Text )
        {
            this->clear();
            this->load((const byte *)(UTF8Text.data()), UTF8Text.size());
            return (*this);
        }
        
        text & operator = ( const text & another );
        
        inline text & operator = ( const char * UTF8Text )
        {
            this->clear();
            this->load((const byte *)(UTF8Text));
            return (*this);
        }
        
        inline text & operator = ( std::string & UTF8Text )
        {
            this->clear();
            this->load((const byte *)(UTF8Text.data()), UTF8Text.size());
            return (*this);
        }
    };
    
    template <typename charType = int32_t, typename wordTagType = byte>
    class taggedText : public text
    {
        public:
        
        class word : public text::word
        {
            template <typename textCharType, typename tagType>
            friend class taggedText;
            
            protected:
            
            wordTagType Tag;
            
            public:
            
            
        };
    };
}

#endif // _RETXT_TEXT_INCLUDED
