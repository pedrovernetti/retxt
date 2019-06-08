#pragma once
#ifndef _RETXT_TEXT_INCLUDED
#define _RETXT_TEXT_INCLUDED

#include "globalUtilities.hpp"
#include "basicDataStructures.hpp"
#include "charEncoding.hpp"

namespace // internal parts
{	
	constexpr byte UTF8EncodedSize( const uint32_t codepoint )
	{
		if (codepoint < 128) return 1;
		if (codepoint < 2048) return 2;
		if (codepoint < 65536) return 3;
		if (codepoint < 2097152) return 4;
		if (codepoint < 67108864) return 5;
		return 6;
	}
            
    constexpr uint32_t characterFrom2UTF8Bytes( const byte * bytes, const size_t position )
    {
        return (((bytes[position] & 0x1F) << 6) | 
                ((bytes[position + 1]) & 0x3F));
    }
    
    constexpr uint32_t characterFrom3UTF8Bytes( const byte * bytes, const size_t position )
    {
        return (((bytes[position] & 0xF) << 12) | 
                ((bytes[position + 1] & 0x3F) <<  6) | 
                (bytes[position + 2] & 0x3F));
    }
    
    constexpr uint32_t characterFrom4UTF8Bytes( const byte * bytes, const size_t position )
    {
        return (((bytes[position] & 0x7) << 18) | 
                ((bytes[position + 1] & 0x3F) << 12) | 
                ((bytes[position + 2] & 0x3F) <<  6) | 
                (bytes[position + 3] & 0x3F));
    }
    
    constexpr void characterTo2UTF8Bytes( const uint32_t codepoint, 
										  byte * string, 
										  size_t & position )
    {
        string[position++] = (((codepoint >> 6) & 0x1F) | 0xC0);
        string[position++] = ((codepoint & 0x3F) | 0x80);
    }
    
    constexpr void characterTo3UTF8Bytes( const uint32_t codepoint, 
										  byte * string, 
										  size_t & position )
    {
        string[position++] = (((codepoint >> 12) & 0xF) | 0xE0);
        string[position++] = (((codepoint >> 6) & 0x3F) | 0x80);
        string[position++] = ((codepoint & 0x3F) | 0x80);
    }
    
    constexpr void characterTo4UTF8Bytes( const uint32_t codepoint, 
										  byte * string, 
										  size_t & position )
    {
        string[position++] = (((codepoint >> 18) & 0x7) | 0xF0);
        string[position++] = (((codepoint >> 12) & 0x3F) | 0x80);
        string[position++] = (((codepoint >> 6) & 0x3F) | 0x80);
        string[position++] = ((codepoint & 0x3F) | 0x80);
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
    
    template <typename generic>
    bool isCloseToLF( const generic * characters, 
					  const size_t totalCharacters, 
					  const size_t position )
    {
        return ((((position + 1) < totalCharacters) && (characters[position + 1] == 10)) ||
                ((position > 0) && (characters[position + 1] == 10)));
	}
	
	constexpr bool isJunkCharacter( const retxt::characterClass charClass )
	{
		// Unassigned, Surrogate, Non-character
		return ((!charClass) || (charClass & 0b11000000000000000000));
	}
	
	constexpr bool isStandaloneCharacter( const retxt::characterClass charClass )
	{
		// Control, Format, Punctuation, Symbol
		return (charClass & 0b00001000000000100011); 
	}
	
		//HorizontalSpace			= 0b00000000000000001000,				
		//Newline					= 0b00000000000000010000,
		//Number					= 0b00000000000001000000,
		//Digit						= 0b00000000000010000000,
		//Letter					= 0b00000000000100000000, 		
		//Modifier					= 0b00000001000000000000,		
		//Private			 		= 0b00100000000000000000, 
		
	template <typename generic>
	bool descendingMultiplicityCriterion( const std::pair<generic, size_t> first,
										  const std::pair<generic, size_t> last )
	{
		return (first.second > last.second);
	}
}

namespace retxt
{    
    class text : public stringifiable
    {
        public:
        
        enum wordType : byte
        {
			Empty = 0,
			Literal = 1,
			Number = 2,
			Punctuation = 3,
			Symbol = 4,
			Whitespace = 5,
			Newline = 6,
			Format = 7,
			ControlCode = 8,
			Junk = 9
		};
        
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
            public: wordType Type = wordType::Empty; protected: // TODO
            
            void load( const byte * string, const size_t stringSize = ((size_t)(-1)) );
        
            void load( const uint32_t * string, const size_t stringSize = ((size_t)(-1)) );
            
            word( const uint32_t * UTF32String,  
				  const size_t stringSize,
                  const size_t positionInText,
                  const uint16_t followingSpaces,
                  const uint32_t spaceCharacter = 32 );
            
            word( const uint32_t character, 
                  const size_t positionInText,
                  const uint16_t followingSpaces,
                  const uint32_t spaceCharacter = 32 );
            
            public:
            
            word( const word & another );
            
            word( const uint32_t spaceCharacter = 32 );
            
            word( const byte * UTF8Text, const uint32_t spaceCharacter = 32 );
            
            word( const byte * UTF8Text, 
                  const size_t textSize, 
                  const uint32_t spaceCharacter = 32 );
            
            word( const char * UTF8Text, const uint32_t spaceCharacter = 32 );
            
            word( const char * UTF8Text, 
                  const size_t textSize, 
                  const uint32_t spaceCharacter = 32 );
            
            word ( const uint32_t * UTF32Text, 
				   const size_t textSize, 
				   const uint32_t spaceCharacter = 32 );
            
            word( std::string UTF8Text, const uint32_t spaceCharacter = 32 );
            
            ~word();
            
            inline void clear()
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
            
            inline size_t size() const
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
            
            inline uint32_t setSpaceCharacter( const uint32_t character )
            {
				if (character != 10) return (this->Separator = character);
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
            
            inline void toTitlecase()
            {
				if (this->CharacterCount == 0) return;
				this->Characters[0] = unicode::titlecase(this->Characters[0]);
                for (size_t i = 1, size = this->CharacterCount; i < size; i++)
                    this->Characters[i] = unicode::lowercase(this->Characters[i]);
			}
            
            word essentialCopy() const;
            
            byte * toUTF8( const bool zeroTerminated = true ) const;
            
            uint32_t * toUTF32( const bool zeroTerminated = true ) const;
            
            inline byte * toArray() const
            {
                return this->toUTF8();
            }
            
            std::string stringified() const;
            
            inline uint32_t & operator [] ( const size_t position ) const
            {
                return this->Characters[position];
            }
            
            inline word & operator () ( const uint32_t UTF32Character ) // TODO
            {
				this->clear();
				
			}
            
            inline word & operator () ( const byte * UTF8Text )
            {
                this->clear();
                this->load(UTF8Text);
                return (*this);
            }
            
            inline word & operator () ( const char * UTF8Text )
            {
                this->clear();
                this->load((const byte *)(UTF8Text));
                return (*this);
            }
            
            inline word & operator () ( const uint32_t * UTF32Text )
            {
                this->clear();
                this->load(UTF32Text);
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
				return this->operator()(UTF8Text);
            }
        
            inline word & operator = ( const byte * UTF8Text )
            {
                return this->operator()(UTF8Text);
            }
        
            inline word & operator = ( const uint32_t * UTF32Text )
            {
                return this->operator()(UTF32Text);
            }
        
            inline word & operator = ( std::string UTF8Text )
            {
                return this->operator()(UTF8Text);
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
        };
        
        protected:
        
        word * Words;
        
        size_t * LineBreaks;
        
        mutable size_t EncodedSize;
        
        size_t CharacterCount;
        size_t WordCount;
        size_t LineCount;
        
        uint32_t Separator;
        
        uint16_t LeadingSpaces;
        
        lineBreakType LineBreak;
        
        void pushProperLineBreak( byte * UTF8String, size_t & position ) const;

		uint32_t * decode( const byte * UTF8Text, const size_t textSize );
		
		void analyze( const uint32_t * UTF32Text, const size_t textSize );
		
		void tokenize( const uint32_t * UTF8Text, simpleQueue<word> & textBuffer );
        
        void loadFromQueue( simpleQueue<word> & textBuffer );
        
        void load( const byte * UTF8Text, const size_t textSize = ((size_t)(-1)) );
				
        void load( const uint32_t * UTF32Text, const size_t textSize = ((size_t)(-1)) );
				
        void trimWordsFollowingSpaces( bool leaveOne );
        
        public:
        
        text( const text & another );
        
        text( const uint32_t spaceCharacter = 32, 
			  const lineBreakType lineBreak = lineBreakType::CRLFLineBreak );
        
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
        
        text( const uint32_t * UTF32Text,
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
            if (this->LineBreaks != nullptr) delete[] this->LineBreaks;
            this->Words = nullptr;
            this->LineBreaks = nullptr;
            this->EncodedSize = this->CharacterCount = this->WordCount = this->LineCount = 0;
            this->LeadingSpaces = 0;
        }
        
        inline bool isEmpty() const
        {
            return (this->CharacterCount == 0);
        }

        inline size_t encodedSize() const
        {
			if (this->EncodedSize >= this->CharacterCount) return this->EncodedSize;
			
			std::cout << "%%%\n";
			const size_t wordCount = this->WordCount;
			const byte separatorSize = UTF8EncodedSize(this->Separator);
			this->EncodedSize = (separatorSize * this->LeadingSpaces);
			for (size_t i = 0, j, charCount; i < wordCount; i++)
			{
				for (j = 0, charCount = this->Words[i].CharacterCount; j < charCount; j++)
				{
					this->EncodedSize += UTF8EncodedSize(this->Words[i].Characters[j]);
					this->EncodedSize += (separatorSize * this->Words[i].FollowingSpaces);
				}
			}
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
        
        inline size_t size() const
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
		
		inline double hasMeaning() const
		{
			std::map<std::string, size_t> bagOfWords;
			for (size_t i = 0, total = this->WordCount; i < total; i++)
			{
				if (this->Words[i].Characters == 0) continue;
				if (this->Words[i].Type != wordType::Literal) continue;
				bagOfWords[this->Words[i].stringified()/*.essentialCopy()*/]++;
			}
			std::vector<std::pair<std::string, size_t>> v(bagOfWords.begin(), bagOfWords.end());
			return ((double)(this->WordCount) / (double)(v.size()));
			std::sort(v.begin(), v.end(), descendingMultiplicityCriterion<std::string>);
			uint32_t * differences = new uint32_t[v.size()];
			for (size_t i = 1, total = v.size(); i < total; i++)
				differences[i - 1] = (v.at(i - 1).second - v.at(i).second);
			double meaning = 0;
			for (size_t i = 0, total = (v.size() - 2); i < total; i++)
			{
				std::cout << (differences[i] - differences[i + 1]) << " ";
				meaning += (differences[i] - differences[i + 1]);
			}
			return (meaning / (v.size() - 2));
		}
        
        word * NMostFrequentWords( size_t N, wordType soughtType = wordType::Literal ) const
        {
			std::map<std::string, size_t> bagOfWords;
			for (size_t i = 0, total = this->WordCount; i < total; i++)
			{
				if (this->Words[i].Characters == 0) continue;
				if (this->Words[i].Type != soughtType) continue;
				bagOfWords[this->Words[i].stringified()/*.essentialCopy()*/]++;
			}
			std::vector<std::pair<std::string, size_t>> v(bagOfWords.begin(), bagOfWords.end());
			
			const size_t total = bagOfWords.size();
			std::pair<std::string, size_t> * listedBag = 
				new std::pair<std::string, size_t>[total];
			auto element = bagOfWords.begin();
			for (size_t i = 0; i < total; listedBag[i] = (*element), i++, element++);			
			std::sort(listedBag, &(listedBag[total]), descendingMultiplicityCriterion<std::string>);
			
			if (N > total) N = total;
			
			word * words = new word[N];
			for (size_t i = 0; i < N; words[i](listedBag[i].first), i++);
			return words;
        }

        /*word * NLargestWords( const size_t N ) const
        {
            simpleQueue<word> words;
        }
        
        inline pair<uint32_t, size_t> * charactersFrequency() const
        {
            
        }
        
        inline std::pair<word, size_t> * bagOfWords() const
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
            for (size_t i = 0, total = this->WordCount; i < total; i++)
                this->Words[i].toUppercase();
            this->EncodedSize = 0;
        }
        
        inline void toLowercase()
        {
            if (this->WordCount == 0) return;
            for (size_t i = 0, total = this->WordCount; i < total; i++)
                this->Words[i].toLowercase();
            this->EncodedSize = 0;
        }
        
        inline void toTitlecase()
        {
			if (this->WordCount == 0) return;
            for (size_t i = 0, total = this->WordCount; i < total; i++)
                this->Words[i].toTitlecase();
            this->EncodedSize = 0;
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
        
        inline lineBreakType lineBreak() const // TODO
        {
			return this->LineBreak;
		}
        
        inline lineBreakType setLineBreak( const lineBreakType lineBreak ) // TODO
        {
			this->LineBreak = lineBreak;
            return this->LineBreak;
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
        void forEachWord( function && action ) // TODO: confirmar segurança
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
			}
			this->EncodedSize = 0;
		}
		
		template <typename function>
		void forEachCharacter( function && action ) // TODO: confirmar segurança
		{
			uint32_t buffer;
			const size_t wordCount = this->WordCount;
			for (size_t i = 0, j, charCount; i < wordCount; i++)
			{
				for (j = 0, charCount = this->Words[i].CharacterCount; j < charCount; j++)
				{
					buffer = this->Words[i].Characters[j];
					if (buffer != 10) 
					{
						action(this->Words[i].Characters[j]);
						if (this->Words[i].Characters[j] == 10) 
							this->Words[i].Characters[j] = buffer;
					}
				}
				buffer = this->Words[i].Separator;
				action(this->Words[i].Separator);
				if (this->Words[i].Separator == 10) this->Words[i].Separator = buffer;
			}
			buffer = this->Separator;
			action(this->Separator);
			if (this->Separator == 10) this->Separator = buffer;
			this->EncodedSize = 0;
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
	
		void refactor();
		
        inline uint32_t operator [] ( const size_t N ) const
        {
            return this->NthCharacter(N);
        }
        
        text & operator () ( const text & another );
        
        inline text & operator () ( const byte * UTF8Text )
        {
            this->clear();
            this->load(UTF8Text);
            return (*this);
        }
        
        inline text & operator () ( const char * UTF8Text )
        {
            this->clear();
            this->load((const byte *)(UTF8Text));
            return (*this);
        }
        
        inline text & operator () ( const uint32_t * UTF32Text )
        {
			this->clear();
			this->load(UTF32Text);
			return (*this);
		}
        
        inline text & operator () ( std::string UTF8Text )
        {
            this->clear();
            this->load((const byte *)(UTF8Text.data()), UTF8Text.size());
            return (*this);
        }
        
        inline text & operator = ( const text & another )
        {
			return this->operator()(another);
		}
        
        inline text & operator = ( const byte * UTF8Text )
        {
			return this->operator()(UTF8Text);
        }
        
        inline text & operator = ( const char * UTF8Text )
        {
			return this->operator()(UTF8Text);
        }
        
        inline text & operator = ( const uint32_t * UTF32Text )
        {
			return this->operator()(UTF32Text);
		}
        
        inline text & operator = ( std::string & UTF8Text )
        {
            return this->operator()(UTF8Text);
        }
        
        inline bool operator < ( const text & another ) const // TODO: lexicographic
        {
            return (this->CharacterCount < another.CharacterCount);
        }
        
        inline bool operator > ( const text & another ) const // TODO: lexicographic
        {
            return (this->CharacterCount > another.CharacterCount);
        }
        
        inline bool operator <= ( const text & another ) const // TODO: lexicographic
        {
            return (this->CharacterCount <= another.CharacterCount);
        }
        
        inline bool operator >= ( const text & another ) const // TODO: lexicographic
        {
            return (this->CharacterCount >= another.CharacterCount);
        }
        
        bool operator == ( const text & another ) const; // TODO: lexicographic

        inline bool operator < ( const size_t size ) const
        {
            return (this->CharacterCount < size);
        }
        
        inline bool operator > ( const size_t size ) const
        {
            return (this->CharacterCount > size);
        }
        
        inline bool operator <= ( const size_t size ) const
        {
            return (this->CharacterCount <= size);
        }
        
        inline bool operator >= ( const size_t size ) const
        {
            return (this->CharacterCount >= size);
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
