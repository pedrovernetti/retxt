#pragma once
#ifndef _RETXT_TEXT_INCLUDED
#define _RETXT_TEXT_INCLUDED

#include "globalUtilities.hpp"
#include "basicDataStructures.hpp"
#include "charEncoding.hpp"

namespace // internal parts
{	
	inline byte UTF8EncodedSize( const uint32_t codepoint )
	{
		if (codepoint <= 0x7F) return 1;
		if (codepoint <= 0x7FF) return 2;
		if (codepoint <= 0xFFFF) return 3;
		if (codepoint <= 0x1FFFFF) return 4;
		if (codepoint <= 0x3FFFFFF) return 5;
		return 6;
	}
            
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
    
    template <typename generic>
	void pushLineBreak( retxt::lineBreakType lineBreak, 
						generic * UTF8String, 
						size_t & position )
	{
		if (lineBreak == retxt::lineBreakType::CRLFLineBreak)
			UTF8String[position++] = 13;
		if (lineBreak | retxt::lineBreakType::LFLineBreak)
			UTF8String[position++] = 10;
		if ((lineBreak | retxt::lineBreakType::CRLineBreak) &&
			(lineBreak != retxt::lineBreakType::CRLFLineBreak))
			UTF8String[position++] = 13;
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
        public virtual dataStructure<byte>,
        public stringifiable
    {
        public:
        
        class word : public stringifiable
        {
			friend class text;
			
            protected:
            
            uint32_t * Characters;
            size_t PositionInText;
            uint32_t Separator;
            uint16_t CharacterCount;
            uint16_t EncodedSize;
            byte FollowingSpaces;
            
            void load( const byte * string, const size_t stringSize );
        
            void load( const uint32_t * string, const size_t stringSize );
        
            inline void load( const byte * UTF8Text )
            {
                load(UTF8Text, ((size_t)(-1)));
            }
            
            word( simpleQueue<uint32_t> & buffer, 
                  const size_t positionInText,
                  const uint16_t encodedSize,
                  const uint16_t followingSpaces,
                  const uint32_t spaceCharacter = 32 );
            
            word( const uint32_t character, 
                  const size_t positionInText,
                  const uint16_t followingSpaces,
                  const uint32_t spaceCharacter = 32 );
            
            public:
            
            word( const word & another );
            
            word( const uint32_t spaceCharacter = 32 );
            
            word( const char * UTF8Text, const uint32_t spaceCharacter = 32 );
            
            word( const char * UTF8Text, 
                  const size_t textSize, 
                  const uint32_t spaceCharacter = 32 );
            
            word( std::string UTF8Text, const uint32_t spaceCharacter = 32 );
            
            word ( const uint32_t * UTF32Text, 
				   const size_t textSize, 
				   const uint32_t spaceCharacter = 32 );
            
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
            
            inline uint32_t spaceCharacter() const
            {
				return this->Separator;
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
            
            inline uint32_t setSpaceCharacter( const uint32_t character )
            {
				if (character != 10) return (this->Separator = character);
				return this->Separator;
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
            
            inline bool startsWith( uint32_t character ) const
            {
				if (!this->CharacterCount) return false;
				return ((character) ? (this->Characters[0] == character) : true);
			}
            
            bool startsWith( word prefix ) const;
            
            inline bool endsWith( uint32_t character ) const
            {
				if (!this->CharacterCount) return false;
				if (!character) return true;
				return (this->Characters[this->CharacterCount - 1] == character);
			}
            
            bool endsWith( word suffix ) const;
            
            inline void removeSuffix( const size_t suffixLength )
            {
				if (suffixLength > this->CharacterCount) return;
				if (suffixLength == this->CharacterCount) 
				{
					this->EncodedSize = this->FollowingSpaces * UTF8EncodedSize(this->Separator);
					this->CharacterCount = 0;
				}
				else
				{
					const size_t newEnd = (this->CharacterCount - (suffixLength + 1));
					for (size_t i = this->CharacterCount - 1; i > newEnd; i--)
					{
						this->EncodedSize -= UTF8EncodedSize(this->Characters[i]);
					}
					this->CharacterCount -= suffixLength;
				}
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
            
            byte * toUTF8( const bool zeroTerminated = true ) const;
            
            uint32_t * toUTF32( const bool zeroTerminated = true ) const;
            
            inline byte * toArray() const
            {
                return this->toUTF8();
            }
            
            inline operator byte * () const
            {
                return this->toUTF8();
            }
			
			inline explicit operator char * () const
			{
				return ((char *)(this->toUTF8()));
			}
			
			inline explicit operator uint32_t * () const
			{
				return this->toUTF32(true);
			}
			
			inline explicit operator int32_t * () const
			{
				return ((int32_t *)(this->toUTF32(true)));
			}
            
            std::string stringified() const;
            
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
        
        size_t EncodedSize;
        
        size_t CharacterCount;
        size_t WordCount;
        size_t LineCount;
        
        uint32_t Separator;
        
        uint16_t LeadingSpaces;
        
        lineBreakType LineBreak;
        
        void pushProperLineBreak( byte * UTF8String, size_t & position ) const;
        
        void loadLeadingSpaces( const byte * UTF8Text, const size_t textSize );
        
        void insertFinalLineBreakIfNeeded( simpleQueue<word> textBuffer );
        
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
            this->EncodedSize = this->CharacterCount = this->WordCount = 0;
            this->LeadingSpaces = 0;
        }
        
        inline bool isEmpty() const
        {
            return (this->EncodedSize == 0);
        }

        inline size_t encodedSize() const
        {
            return this->EncodedSize;
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
        
        inline size_t internalWordCount() const
        {
            return this->WordCount;
        }
        
        inline size_t wordCount() const
        {
            return (this->WordCount - this->LineCount);
        }
        
        inline size_t lineCount() const
        {
			return this->LineCount;
		}
        
        inline word firstWord() const
        {
            if (this->WordCount == 0) return word(this->Separator);
            return this->Words[0];
        }
        
        inline word lastWord() const
        {
            if (this->WordCount == 0) return word(this->Separator);
            return this->Words[this->WordCount - 1];
        }
        
        uint32_t NthCharacter( const size_t N ) const;
        
        inline word NthWord( const size_t N ) const
        {
            if (this->WordCount <= N) return word();
            return this->Words[N];
        }
        
        // NthLine
        
        size_t count( const uint32_t & whichCharacter ) const;
        
        // count word/string
        
        size_t smallestWordSize() const;
        
        size_t largestWordSize() const;
        
        double averageWordSize() const;
        
        size_t mostFrequentWordSize( const bool preferTheSmaller = false ) const;
			
		size_t smallestLineSize( const bool considerEmptyLines = true ) const;
	
		inline size_t smallestNonEmptyLineSize() const
		{
			return this->smallestLineSize(false);
		}
	
		size_t largestLineSize() const;
	
		inline double averageLineSize() const
		{
			if (!(this->LineCount)) return this->CharacterCount;
			return (((double)(this->CharacterCount))/((double)(this->LineCount)));
		}

		inline double averageLineWordCount() const
		{
			if (!(this->LineCount)) return this->WordCount;
			return (((double)(this->WordCount))/((double)(this->LineCount)));
		}
		
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
            if (character != 10) return (this->Separator = character);
            return this->Separator;
        }
        
        inline uint32_t lineBreak() const
        {
			return this->LineBreak;
		}
        
        inline uint32_t setLineBreak( const lineBreakType lineBreak )
        {
            return this->LineBreak = lineBreak;
        }
        
        void trimLeadingSpaces();

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
                
        void trimLeadingLineBreaks();
        
        template <typename function>
        void forEachWord( function && action )
        {
			word buffer;
			for (size_t i = 0, size = this->WordCount, totalChars = 0; i < size; i++)
			{
				if ((this->Words[i].CharacterCount) && (this->Words[i].Characters[0] == 10))
					continue;
				buffer = this->Words[i];
				action(this->Words[i]);
				this->Words[i].PositionInText = totalChars;
				this->Words[i].Separator = this->Separator;
				this->CharacterCount -= (buffer.CharacterCount - this->Words[i].CharacterCount);
				this->CharacterCount -= (buffer.FollowingSpaces - this->Words[i].FollowingSpaces);
				totalChars += (this->Words[i].CharacterCount + this->Words[i].FollowingSpaces);
				this->EncodedSize -= (buffer.EncodedSize - this->Words[i].EncodedSize);
			}
		}
		
		template <typename function>
		void forEachCharacter( function && action )
		{
			uint32_t buffer;
			const size_t wordCount = this->WordCount;
			for (size_t i = 0, j, charCount; i < wordCount; i++)
			{
				this->EncodedSize -= this->Words[i].EncodedSize;
				this->Words[i].EncodedSize = 0;
				for (j = 0, charCount = this->Words[i].CharacterCount; j < charCount; j++)
				{
					buffer = this->Words[i].Characters[j];
					if (buffer != 10) 
					{
						action(this->Words[i].Characters[j]);
						if (this->Words[i].Characters[j] == 10) 
							this->Words[i].Characters[j] = buffer;
					}
					this->Words[i].EncodedSize += UTF8EncodedSize(this->Words[i].Characters[j]);
				}
				this->EncodedSize += this->Words[i].EncodedSize;
				buffer = this->Words[i].Separator;
				action(this->Words[i].Separator);
				if (this->Words[i].Separator == 10) this->Words[i].Separator = buffer;
			}
			buffer = this->Separator;
			action(this->Separator);
			if (this->Separator == 10) this->Separator = buffer;
		}
        
        word * toArrayOfWords() const;
        
        uint32_t * * toArrayOfUTF32Words() const;
        
        byte * * toArrayOfUTF8Words() const;
        
        std::string * toArrayOfWordsAsUTF8Strings() const;
        
        uint32_t * * toArrayOfUTF32Lines() const;
        
        byte * * toArrayOfUTF8Lines() const;
        
		std::string * toArrayOfLinesAsUTF8Strings() const;
	
        byte * toUTF8( const bool zeroTerminated = true ) const;
        
        uint32_t * toUTF32( const bool zeroTerminated = true ) const;
        
        inline byte * toArray() const
        {
            return this->toUTF8(true);
        }
        
        std::string stringified() const;
        
        inline word operator [] ( const size_t N ) const
        {
            return this->NthCharacter(N);
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
        
        inline bool operator < ( const text & another ) const
        {
            return (this->EncodedSize < another.EncodedSize);
        }
        
        inline bool operator > ( const text & another ) const
        {
            return (this->EncodedSize > another.EncodedSize);
        }
        
        inline bool operator <= ( const text & another ) const
        {
            return (this->EncodedSize <= another.EncodedSize);
        }
        
        inline bool operator >= ( const text & another ) const
        {
            return (this->EncodedSize >= another.EncodedSize);
        }

        inline bool operator < ( const size_t size ) const
        {
            return (this->EncodedSize < size);
        }
        
        inline bool operator > ( const size_t size ) const
        {
            return (this->EncodedSize > size);
        }
        
        inline bool operator <= ( const size_t size ) const
        {
            return (this->EncodedSize <= size);
        }
        
        inline bool operator >= ( const size_t size ) const
        {
            return (this->EncodedSize >= size);
        }
        
        inline explicit operator word * () const
        {
			return this->toArrayOfWords();
		}
        
        inline operator byte * () const
        {
			return this->toArray();
		}
		
        inline explicit operator char * () const
        {
			return ((char *)(this->toArray()));
		}
		
		inline explicit operator uint32_t * () const
		{
			return this->toUTF32(true);
		}
		
		inline explicit operator int32_t * () const
		{
			return ((int32_t *)(this->toUTF32(true)));
		}
    };
}

#endif // _RETXT_TEXT_INCLUDED
