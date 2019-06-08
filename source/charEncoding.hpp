#pragma once
#ifndef _RETXT_CHARENCODING_INCLUDED
#define _RETXT_CHARENCODING_INCLUDED

#include "globalUtilities.hpp"

namespace // internal parts
{
	constexpr byte isASCIIPunctuation[16] =
	{
		0b00000000, 0b00000000, 0b00000000, 0b00000000, 
		0b01100001, 0b11001111, 0b00000000, 0b00111011, 
		0b00000000, 0b00000000, 0b00000000, 0b00010100, 
		0b00000000, 0b00000000, 0b00000000, 0b00010100,
	};

	constexpr byte isASCIISymbol[16] =
	{
		0b00000000, 0b00000000, 0b00000000, 0b00000000, 
		0b00011110, 0b00110000, 0b00000000, 0b00001110, 
		0b10000000, 0b00000000, 0b00000000, 0b00001011, 
		0b10000000, 0b00000000, 0b00000000, 0b00001010
	};
		
	constexpr uint16_t ISO88597ToUnicodeConversionTable[30] =
	{
		160, 8216, 8217, 163, 8364, 8367, 166, 167, 168, 169, 890,  171, 172, 173, 26, 8213,
		176, 177, 178, 179, 900, 901, 902, 183, 904, 905, 906, 187, 908, 189
	};
	
	constexpr uint16_t CP1250ToUnicodeConversionTable[128] =
	{
		8364, 26, 8218, 26, 8222, 8230, 8224, 8225, 26, 8240, 352, 8249, 346, 356, 381, 377,
		26, 8216, 8217, 8220, 8221, 8226, 8211, 8212, 26, 8482, 353, 8250, 347, 357, 382, 378,
		160, 711, 728, 321, 164, 260, 166, 167, 168, 169, 350, 171, 172, 173, 174, 379,
		176, 177, 731, 322, 180, 181, 182, 183, 184, 261, 351, 187, 317, 733, 318, 380,
		340, 193, 194, 258, 196, 313, 262, 199, 268, 201, 280, 203, 282, 205, 206, 270,
		272, 323, 327, 211, 212, 336, 214, 215, 344, 366, 218, 368, 220, 221, 354, 223,
		341, 225, 226, 259, 228, 314, 263, 231, 269, 233, 281, 235, 283, 237, 238, 271,
		273, 324, 328, 243, 244, 337, 246, 247, 345, 367, 250, 369, 252, 253, 355, 729
	};
	
	constexpr uint16_t CP1251ToUnicodeConversionTable[64] =
	{
		1026, 1027, 8218, 402, 8222, 8230, 8224, 8225, 8364, 8240, 1033, 8249, 1034, 1036, 1035, 1039,
		1106, 8216, 8217, 8220, 8221, 8226, 8211, 8212, 26, 8482, 1113, 8250, 1114, 1116, 1115, 1119,
		160, 1038, 1118, 1032, 164, 1168, 166, 167, 1025, 169, 1028, 171, 172, 173, 174, 1031,
		176, 177, 1030, 1110, 1169, 181, 182, 183, 1105, 8470, 1108, 187, 1112, 1029, 1109, 1111
	};
	
	constexpr uint16_t CP1252ToUnicodeConversionTable[32] =
	{
		8364, 26, 8218, 402, 8222, 8230, 8224, 8225, 710, 8240, 352, 8249, 338, 26, 381, 26,
		26, 8216, 8217, 8220, 8221, 8226, 8211, 8212, 732, 8482, 353, 8250, 339, 26, 382, 376
	};
	
	constexpr uint16_t CP1253ToUnicodeConversionTable[64] =
	{
		8364, 26, 8218, 402, 8222, 8230, 8224, 8225, 26, 8240, 26, 8249, 26, 26, 26, 26,
		26, 8216, 8217, 8220, 8221, 8226, 8211, 8212, 26, 8482, 26, 8250, 26, 26, 26, 26,
		160, 901, 902, 163, 164, 165, 166, 167, 168, 169, 26, 171, 172, 173, 174, 8213,
		176, 177, 178, 179, 900, 181, 182, 183, 904, 905, 906, 187, 908, 189, 910, 911
	};
	
	constexpr uint16_t CP1255ToUnicodeConversionTable[64] =
	{
		8364, 26, 8218, 402, 8222, 8230, 8224, 8225, 710, 8240, 26, 8249, 26, 26, 26, 26,
		26, 8216, 8217, 8220, 8221, 8226, 8211, 8212, 732, 8482, 26, 8250, 26, 26, 26, 26,
		160, 161, 162, 163, 8362, 165, 166, 167, 168, 169, 215, 171, 172, 173, 174, 175,
		176, 177, 178, 179, 900, 181, 182, 183, 184, 185, 247, 187, 188, 189, 190, 191
	};
	
	constexpr uint16_t CP1256ToUnicodeConversionTable[128] =
	{
		8364, 1662, 8218, 402,  8222, 8230, 8224, 8225, 710,  8240, 1657, 8249, 338,  1670, 1688, 1672,
		1711, 8216, 8217, 8220, 8221, 8226, 8211, 8212, 1705, 8482, 1681, 8250, 339,  8204, 8205, 1722,
		160,  1548, 162,  163,  164,  165,  166,  167,  168,  169,  1726, 171,  172,  173,  174,  175,
		176,  177,  178,  179,  180,  181,  182,  183,  184,  185,  1563, 187,  188,  189,  190,  1567,
		1729, 1569, 1570, 1571, 1572, 1573, 1574, 1575, 1576, 1577, 1578, 1579, 1580, 1581, 1582, 1583,
		1584, 1585, 1586, 1587, 1588, 1589, 1590, 215,  1591, 1592, 1593, 1594, 1600, 1601, 1602, 1603,
		224,  1604, 226,  1605, 1606, 1607, 1608, 231,  232,  233,  234,  235,  1609, 1610, 238,  239,
		1611, 1612, 1613, 1614, 244,  1615, 1616, 247,  1617, 249,  1618, 251,  252,  8206, 8207, 1746
	};
	
	constexpr uint16_t CP1257ToUnicodeConversionTable[128] =
	{
		8364, 26, 8218, 26, 8222, 8230, 8224, 8225, 26, 8240, 26, 8249, 26, 168, 711, 184,
		26, 8216, 8217, 8220, 8221, 8226, 8211, 8212, 26, 8482, 26, 8250, 26, 175, 731, 26,
		160, 26,  162, 163, 164, 26,  166, 167, 216, 169, 342, 171, 172, 173, 174, 198,
		176, 177, 178, 179, 900, 181, 182, 183, 248, 185, 343, 187, 188, 189, 190, 230,
		260, 302, 256, 262, 196, 197, 280, 274, 268, 201, 377, 278, 290, 310, 298, 315,
		352, 323, 325, 211, 332, 213, 214, 215, 370, 321, 346, 362, 220, 379, 381, 223,
		261, 303, 257, 263, 228, 229, 281, 275, 269, 233, 378, 279, 291, 311, 299, 316,
		353, 324, 326, 243, 333, 245, 246, 247, 371, 322, 347, 363, 252, 380, 382, 729
	};
	
	constexpr uint16_t CP1258ToUnicodeConversionTable[128] =
	{
		8364, 26, 8218, 402, 8222, 8230, 8224, 8225, 710, 8240, 26, 8249, 338, 26, 26, 26,
		26, 8216, 8217, 8220, 8221, 8226, 8211, 8212, 732, 8482, 26, 8250, 339, 26, 26, 376,
		160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
		176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
		192, 193, 194, 258, 196, 197, 198, 199, 200, 201, 202, 203, 768, 205, 206, 207,
		272, 209, 777, 211, 212, 416, 214, 215, 216, 217, 218, 219, 220, 431, 771, 223,
		224, 225, 226, 259, 228, 229, 230, 231, 232, 233, 234, 235, 769, 237, 238, 239,
		273, 241, 803, 243, 244, 417, 246, 247, 248, 249, 250, 251, 252, 432, 8363, 255,
	};
	
    constexpr uint16_t EBCDICToUnicodeConversionTable[256] = 
    {
        0,   1,   2,   3,   26,  9,   10,  127, 26,  26,  26,  11,  12,  13,  14,  15,
        16,  17,  18,  19,  26,  10,  8,   26,  24,  25,  26,  145, 28,  29,  30,  31, 
        26,  152, 28,  818, 26,  10,  23,  27,  26,  26,  26,  26,  26,  5,   6,   7, 
        26,  26,  22,  133, 26,  26,  8,   4,   26,  26,  12,  146, 20,  21,  0,   26, 
        32,  26,  26,  26,  26,  26,  26,  26,  26,  26,  162, 46,  60,  40,  43,  124, 
        38,  26,  26,  26,  26,  26,  26,  26,  26,  26,  33,  36,  42,  41,  59,  172, 
        45,  47,  26,  26,  26,  26,  26,  26,  26,  26,  166, 44,  37,  95,  62,  63, 
        26,  26,  26,  26,  26,  26,  26,  26,  26,  96,  58,  35,  64,  39,  61,  34,
        26,  97,  98,  99,  100, 101, 102, 103, 104, 105, 26,  26,  26,  26,  26,  177,
        26,  106, 107, 108, 109, 110, 111, 112, 113, 114, 26,  26,  26,  26,  26,  26,
        26,  126, 115, 116, 117, 118, 119, 120, 121, 122, 26,  26,  26,  26,  26,  26,
        94,  26,  26,  26,  26,  26,  26,  26,  26,  26,  91,  93,  26,  26,  26,  26,
        123, 65,  66,  67,  68,  69,  70,  71,  72,  73,  26,  26,  26,  26,  26,  26,
        125, 74,  75,  76,  77,  78,  79,  80,  81,  82,  26,  26,  26,  26,  26,  26,
        92,  26,  83,  84,  85,  86,  87,  88,  89,  90,  26,  26,  26,  26,  26,  26,
        48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  26,  26,  26,  26,  26,  159
    };
        
    constexpr uint32_t codepointFrom2UTF8Bytes( const byte * bytes, const size_t position )
    {
        return (((bytes[position] & 0x1F) << 6) | 
                ((bytes[position + 1]) & 0x3F));
    }
    
    constexpr uint32_t codepointFrom3UTF8Bytes( const byte * bytes, const size_t position )
    {
        return (((bytes[position] & 0xF) << 12) | 
                ((bytes[position + 1] & 0x3F) <<  6) | 
                (bytes[position + 2] & 0x3F));
    }
    
    constexpr uint32_t codepointFrom4UTF8Bytes( const byte * bytes, const size_t position )
    {
        return (((bytes[position] & 0x7) << 18) | 
                ((bytes[position + 1] & 0x3F) << 12) | 
                ((bytes[position + 2] & 0x3F) <<  6) | 
                (bytes[position + 3] & 0x3F));
    }
    
    constexpr void codepointTo2UTF8Bytes( const uint32_t codepoint, 
										  byte * string, 
										  size_t position )
    {
        string[position++] = (((codepoint >> 6) & 0x1F) | 0xC0);
        string[position] = ((codepoint & 0x3F) | 0x80);
    }
    
    constexpr void codepointTo3UTF8Bytes( const uint32_t codepoint, 
										  byte * string, 
										  size_t position )
    {
        string[position++] = (((codepoint >> 12) & 0xF) | 0xE0);
        string[position++] = (((codepoint >> 6) & 0x3F) | 0x80);
        string[position] = ((codepoint & 0x3F) | 0x80);
    }
    
    constexpr void codepointTo4UTF8Bytes( const uint32_t codepoint, 
										  byte * string, 
										  size_t position )
    {
        string[position++] = (((codepoint >> 18) & 0x7) | 0xF0);
        string[position++] = (((codepoint >> 12) & 0x3F) | 0x80);
        string[position++] = (((codepoint >> 6) & 0x3F) | 0x80);
        string[position] = ((codepoint & 0x3F) | 0x80);
    }
}

namespace retxt
{
    enum characterEncoding : byte
    {
        USASCII = 1,
        UTF8,
        UTF16,
        ISO8859_1,
        ISO8859_2,
        ISO8859_3,
        ISO8859_4,
        ISO8859_5,
        ISO8859_6,
        ISO8859_7,
        ISO8859_8,
        ISO8859_9,
        ISO8859_10,
        ISO8859_11,
        ISO8859_12,
        ISO8859_13,
        ISO8859_14,
        ISO8859_15,
        ISO8859_16,
        CP1250,
        CP1251,
        CP1252,
        CP1253,
        CP1254,
        CP1255,
        CP1256,
        CP1257,
        CP1258,
        CP932,
        ShiftJIS,
        EBCDIC
    };
    
    enum characterClass : uint32_t
    {
		// Class				// Value (20bits)         // Unicode equivalent category
		Unassigned				= 0b00000000000000000000, // Cn
			NonCharacter 		= 0b10000000000000000000, // Cn~ [non-charactes]
		Control					= 0b00000000000000000001, // Cc
			HorizontalTab		= 0b00000000000000001101, // Cc~ [0x9]
			ControlAndNewline	= 0b00000000000000010101, // Cc~ [0xA..0xD, 0x85]
			ControlAndFormat	= 0b00000000000000000011, // Cc~
		Format					= 0b00000000000000000010, // Cf | Cc~
		Whitespace				= 0b00000000000000000100, // Zs | Zl | Zp | Cc~
			HorizontalSpace		= 0b00000000000000001100, // Zs | Cc~ [0x9]
			Newline				= 0b00000000000000010100, // Cc~ [0xA..0xD, 0x85] | Zl | Zp			
		Punctuation				= 0b00000000000000100000, // Po~ | Pc | Pd | Pe | Pf | Pi | Ps
		Number					= 0b00000000000001000000, // No | Nd | Nl
			Digit				= 0b00000000000011000000, // Nd
			LetterNumber		= 0b00000000000101000000, // Nl
				LowercaseNumber	= 0b00000000001101000000, // Nl~
				UppercaseNumber	= 0b00000000110101000000, // Nl~
		Letter					= 0b00000000000100000000, // Lo | Ll | Lt | Lu | Nl
			LowercaseLetter 	= 0b00000000001100000000, // Ll
			TitlecaseLetter		= 0b00000000010100000000, // Lt
				UppercaseLetter = 0b00000000110100000000, // Lu
		Modifier				= 0b00000001000000000000, // Mc | Me | Mn | Lm | Sk
			NonSpacingModifier	= 0b00000011000000000000, // Me | Mn 
			SpacingModifier		= 0b00000101000000000000, // Mc | Lm | Sk~
				ModifierLetter  = 0b00000101000100000000, // Lm
		Symbol					= 0b00001000000000000000, // So | Sc | Sm | Po~ | Sk~
			MathSymbol			= 0b00011000000000000000, // Sm
			LetterAsSymbol		= 0b00001000000100000000, // Lo~ | Ll~ | Lu~ | So~
				LowercaseSymbol = 0b00001000001100000000, // Lo~ | Ll~ | So~
				UppercaseSymbol = 0b00001000110100000000, // Lo~ | Lu~ | So~
		Private					= 0b00100000000000000000, // Co
		Surrogate				= 0b01000000000000000000, // Cs
		ByteOrderMark			= 0b00000000000000000011, // Cf~ [0xFEFF]
    };
    
    enum lineBreakType : byte
    {
		LFLineBreak				= 0b00000001,
		CRLineBreak 			= 0b00000010,
		RSLineBreak				= 0b00000100,
		CRLFLineBreak			= 0b00001011,
		LFCRLineBreak			= 0b00010011,
		FFLineBreak				= 0b00100000,
		NELLineBreak			= 0b01000000,
		LSLineBreak				= 0b10000000,
		
		UnixLineBreak			= 0b00000001,
		WindowsLineBreak		= 0b00001011,
		
		#ifdef _COMPILING_FOR_UNIX
		DefaultLineBreak		= 0b00000001
		#else
		DefaultLineBreak		= 0b00001011
		#endif
	};
	
	inline void pushProperLineBreak( lineBreakType lineBreak,
									 byte * UTF8String, 
									 size_t & position )
	{
		if (lineBreak == lineBreakType::RSLineBreak)
			UTF8String[position] = 30;
		if (lineBreak == lineBreakType::CRLFLineBreak)
			UTF8String[position++] = 13;
		if (lineBreak | lineBreakType::LFLineBreak)
			UTF8String[position++] = 10;
		if ((lineBreak | lineBreakType::CRLineBreak) &&
			(lineBreak != lineBreakType::CRLFLineBreak))
			UTF8String[position] = 13;
	}
	
	namespace ASCII
	{
		constexpr uint32_t nullCharacter = 0;
		constexpr uint32_t NUL = 0;
		constexpr uint32_t backspace = 8;
		constexpr uint32_t BS = 8;
		constexpr uint32_t horizontalTab = 9;
		constexpr uint32_t HT = 9;
		constexpr uint32_t lineFeed = 10;
		constexpr uint32_t LF = 10;
		constexpr uint32_t carriageReturn = 13;
		constexpr uint32_t CR = 13;
		constexpr uint32_t substituteCharacter = 26;
		constexpr uint32_t SUB = 26;
		constexpr uint32_t escapeCharacter = 27;
		constexpr uint32_t ESC = 27;
		constexpr uint32_t space = 32;
		constexpr uint32_t SP = 32;
		constexpr uint32_t deleteCharacter = 127;
		constexpr uint32_t DEL = 127;
		
		constexpr bool isControlCharacter( const char character )
		{
			return ((((byte)(character)) < 32) || (character == 127));
		}

		constexpr bool isNonGraphicControlCharacter( const char character )
		{
			return ((((byte)(character)) < 9) || (character == 127) ||
					((character > 11) && (character < 32)));
		}

		constexpr bool isWhitespace( const char character )
		{
			return ((character == 32) || ((character > 8) && (character < 14)));
		}
		
		constexpr bool isHorizontalWhitespace( const char character )
		{
			return ((character == 32) || (character == 9));
		}
		
		constexpr bool isDigit( const char character )
		{
			return ((character >= 48) && (character <= 57));
		}
		
		constexpr bool isHexadecimalDigit( const char character )
		{
			return (((character >= 48) && (character <= 57)) ||
					((character >= 65) && (character <= 70)) ||
					((character >= 97) && (character <= 102)));
		}
		
		constexpr bool isUppercaseLetter( const char character )
		{
			return ((character >= 65) && (character <= 90));
		}
		
		constexpr bool isLowercaseLetter( const char character )
		{
			return ((character >= 97) && (character <= 122));
		}
			
		constexpr bool isLetter( const char character )
		{
			return (isUppercaseLetter(character) || 
					isLowercaseLetter(character));
		}
		
        constexpr bool isPunctuation( const uint32_t codepoint )
        {
			if (codepoint & 128) return false;
			return ((isASCIIPunctuation[codepoint / 8] << (codepoint % 8)) & 128);
		}
        
        constexpr bool isSymbol( const uint32_t codepoint )
        {
			if (codepoint & 128) return false;
			return ((isASCIISymbol[codepoint / 8] << (codepoint % 8)) & 128);
		}
        
		constexpr bool isGraphicCharacter( const char character )
		{
			return (((character > 31) && (character < 127)) || 
					((character > 8) && (character < 12)));
		}

		constexpr bool isNonCharacter( const char character )
		{
			return (((byte)(character)) > 127);
		}
			
		constexpr bool lowercase( const char character )
		{
			return ((isUppercaseLetter(character)) ? 
						(character + 32) : character);
		}
		
		constexpr bool uppercase( const char character )
		{
			return ((isLowercaseLetter(character)) ? 
						(character - 32) : character);
		}
	}
    
    namespace unicode
    {
		constexpr uint32_t nullCharacter = 0;
		constexpr uint32_t NUL = 0;
		constexpr uint32_t backspace = 8;
		constexpr uint32_t BS = 8;
		constexpr uint32_t horizontalTab = 9;
		constexpr uint32_t HT = 9;
		constexpr uint32_t lineFeed = 10;
		constexpr uint32_t LF = 10;
		constexpr uint32_t carriageReturn = 13;
		constexpr uint32_t CR = 13;
		constexpr uint32_t substituteCharacter = 26;
		constexpr uint32_t SUB = 26;
		constexpr uint32_t escapeCharacter = 27;
		constexpr uint32_t ESC = 27;
		constexpr uint32_t space = 32;
		constexpr uint32_t SP = 32;
		constexpr uint32_t deleteCharacter = 127;
		constexpr uint32_t DEL = 127;
		constexpr uint32_t byteOrderMark = 65279;
		constexpr uint32_t BOM = 65279;
		constexpr uint32_t replacementCharacter = 65533;
		
		constexpr uint32_t fromASCII( const char character )
		{
			return ((character & 128) ? 26 : character);
		}
		
		constexpr uint32_t fromISO88591( const byte character )
		{
			return character;
		}
		
		uint32_t fromISO88592( const byte character );
		
		uint32_t fromISO88593( const byte character );
		
		uint32_t fromISO88594( const byte character );
		
		uint32_t fromISO88595( const byte character );
		
		uint32_t fromISO88596( const byte character );
		
		constexpr uint32_t fromISO88597( const byte character )
		{
			if (character < 160) return character;
			if (character > 190) return ((character == 255) ? 26 : (character + 720));
			return ISO88597ToUnicodeConversionTable[character - 160];
		}
		
		uint32_t fromISO88598( const byte character );
		
		uint32_t fromISO88599( const byte character );
		
		uint32_t fromISO885910( const byte character );
		
		uint32_t fromISO885911( const byte character );
		
		uint32_t fromISO885913( const byte character );
		
		uint32_t fromISO885914( const byte character );
		
		uint32_t fromISO885915( const byte character );
		
		uint32_t fromISO885916( const byte character );
		
		constexpr uint32_t fromWindows1250( const byte character )
		{
			if (character < 128) return character;
			return CP1250ToUnicodeConversionTable[character - 128];
		}
		
		constexpr uint32_t fromWindows1251( const byte character )
		{
			if (character < 128) return character;
			if (character > 191) return (character + 848);
			return CP1251ToUnicodeConversionTable[character - 128];
		}
		
		constexpr uint32_t fromWindows1252( const byte character )
		{
			if ((character < 128) || (character > 159)) return character;
			return CP1252ToUnicodeConversionTable[character - 128];
		}
		
		constexpr uint32_t fromWindows1253( const byte character )
		{
			if (character < 128) return character;
			if (character > 189) return ((character == 255) ? 26 : (character + 720));
			return CP1253ToUnicodeConversionTable[character - 128];
		}
		
		uint32_t fromWindows1254( const byte character );
		
		uint32_t fromWindows1255( const byte character );
		
		constexpr uint32_t fromWindows1256( const byte character )
		{
			if (character < 128) return character;
			return CP1256ToUnicodeConversionTable[character - 128];
		}
		
		constexpr uint32_t fromWindows1257( const byte character )
		{
			if (character < 128) return character;
			return CP1257ToUnicodeConversionTable[character - 128];
		}
		
		constexpr uint32_t fromWindows1258( const byte character )
		{
			if (character < 128) return character;
			return CP1258ToUnicodeConversionTable[character - 128];
		}
		
		constexpr uint32_t fromUCS2( const uint16_t character )
		{
			return character;
		}
		
		constexpr uint32_t fromUCS4( const uint32_t character )
		{
			return character;
		}
		
		constexpr uint32_t fromEBCDIC( const byte character )
		{
			return EBCDICToUnicodeConversionTable[character];
		}
        
        constexpr uint16_t planeOf( const uint32_t codepoint )
        {
            return (codepoint >> 16);
        }
        
        constexpr bool belongsToBasicMultilingualPlane( const uint32_t codepoint )
        {
            return (codepoint < 0x10000);
        }
        
        constexpr bool belongsToSupplementaryMultilingualPlane( const uint32_t codepoint )
        {
            return ((codepoint >> 16) == 1);
        }
        
        constexpr bool belongsToSupplementaryIdeographicPlane( const uint32_t codepoint )
        {
            return ((codepoint >> 16) == 2);
        }
        
        constexpr bool belongsToUnassignedPlanes( const uint32_t codepoint )
        {
            return ((codepoint >= 0x30000) && (codepoint <= 0xDFFFF));
        }
        
        constexpr bool belongsToSupplementarySpecialPurposePlane( const uint32_t codepoint )
        {
            return ((codepoint >> 16) == 14);
        }
        
        constexpr bool belongsToPrivateUsePlanes( const uint32_t codepoint )
        {
            return ((codepoint >= 0xF0000) && (codepoint <= 0x10FFFF));
        }
        
        constexpr bool belongsToNoValidPlane( const uint32_t codepoint )
        {
            return (codepoint > 0x10FFFF);
        }
        
        characterClass classOf( const uint32_t codepoint );
        
        constexpr bool isControlCharacter( const uint32_t codepoint )
        {
			return ((codepoint < 32) || ((codepoint > 126) && (codepoint < 160)));
		}
        
        bool isNonGraphicControlCharacter( const uint32_t codepoint );
        
        bool isFormatCharacter( const uint32_t codepoint );
        
        bool isWhitespace( const uint32_t codepoint );
        
        bool isHorizontalWhitespace( const uint32_t codepoint );
        
        bool isLineBreak( const uint32_t codepoint );
        
		bool isDigit( const uint32_t codepoint );
		
		constexpr bool isHexadecimalDigit( const uint32_t codepoint )
		{
			return ((codepoint > 127) ? false : (ASCII::isHexadecimalDigit(codepoint)));
		}
		
        bool isUppercaseLetter( const uint32_t codepoint );
		
		bool isLowercaseLetter( const uint32_t codepoint );
        
        bool isLetter( const uint32_t codepoint );
        
        bool isPunctuation( const uint32_t codepoint );
        
        bool isNonSpacingModifier( const uint32_t codepoint );
        
        bool isSymbol( const uint32_t codepoint );
        
        bool isGraphicCharacter( const uint32_t codepoint );
        
        constexpr bool isSurrogate( const uint32_t codepoint )
        {
            return ((codepoint >= 983040) && (codepoint <= 1048573));
        }
        
        constexpr bool isControlPicture( const uint32_t codepoint )
        {
            if ((codepoint >= 9216) && (codepoint <= 9254)) return true; 
            return false;
        }
        
        constexpr bool isTagCharacter( const uint32_t codepoint )
        {
            return ((codepoint >= 917536) && (codepoint <= 917630));
        }
        
        bool isUnassigned( const uint32_t codepoint );
        
        constexpr bool isPrivateCharacter( const uint32_t codepoint )
        {
            return (((codepoint >= 983040) && (codepoint <= 1048573)) ||
                    ((codepoint >= 57344) && (codepoint <= 63743)));
        }
        
        bool isNonCharacter( const uint32_t codepoint );
        
        uint32_t uppercase( const uint32_t codepoint );
        
        uint32_t lowercase( const uint32_t codepoint );
        
        uint32_t titlecase( const uint32_t codepoint );
			
		uint32_t plain( const uint32_t codepoint );
		
        namespace UTF8
        {
            constexpr bool isOutsideEncodingRange( const uint32_t codepoint )
            {
                return (codepoint > 2097151);
            }
            
            constexpr byte bytesNeededToEncode( const uint32_t codepoint )
            {
                if (codepoint < 128) return 1;
                if (codepoint < 2048) return 2;
                if (codepoint < 65536) return 3;
                if (codepoint < 2097152) return 4;
                if (codepoint < 67108864) return 5;
                return 6;
            }
            
            constexpr bool isLeadingByte( const byte character )
            {
                return ((character >= 192) && (character <= 247));
            }
            
            constexpr bool isContinuationByte( const byte character )
            {
                return ((character & 0xC0) == 0x80);
            }
            
            constexpr byte sequenceSize( const byte firstByte )
            {
                if (firstByte >= 224) return ((firstByte >> 4) - 11);
                if (firstByte >= 192) return 2;
                return 1;
            }
            
            uint32_t decode2Bytes( const byte * string, 
                                   const size_t position, 
                                   const size_t stringSize );
            
            uint32_t decode3Bytes( const byte * string, 
                                   const size_t position, 
                                   const size_t stringSize );
            
            uint32_t decode4Bytes( const byte * string, 
                                   const size_t position, 
                                   const size_t stringSize );
            
            uint32_t decode( const byte * string, 
                             const size_t position, 
                             const size_t stringSize );
            
            inline uint32_t decode( const std::string & string, 
                                    const size_t position = 0 )
            {
                return decode((const byte *)(string.data()), position, string.size());
            }
            
            bool encodeAs2Bytes( const uint32_t character, 
                                 byte * string,
                                 const size_t position,
                                 const size_t stringSize );

            bool encodeAs3Bytes( const uint32_t character, 
                                 byte * string,
                                 const size_t position,
                                 const size_t stringSize );

            bool encodeAs4Bytes( const uint32_t character, 
                                 byte * string,
                                 const size_t position,
                                 const size_t stringSize );

            byte encode( const uint32_t character, 
                         byte * string,
                         const size_t position,
                         const size_t stringSize );
            
            std::string encode( const uint32_t character );
            
            bool isValid( const byte * string, 
                          const size_t position, 
                          const size_t stringSize );

            bool isValid( const byte * string, const size_t stringSize );
            
            inline bool isValid( std::string string )
            {
                return isValid((const byte *)(string.data()), string.size());
            }
            
            void replaceInvalidBytes( byte * string, const size_t stringSize );
            
            inline void replaceInvalidBytes( std::string & string )
            {
				replaceInvalidBytes(const_cast<byte *>((const byte *)(string.data())), 
								    string.size());
			}
        }
    }
}

#endif // _RETXT_CHARENCODING_INCLUDED
