#pragma once
#ifndef _RETXT_FILETYPESINDEX_INCLUDED
#define _RETXT_FILETYPESINDEX_INCLUDED

#include "globalUtilities.hpp"

namespace retxt
{
	enum fileType : byte
	{
		Unknown = 0,		// unknown-type files
		Binary = 1,			// binary files, like jpg, mkv, mp3, zip, ...
		Readable = 2,		// readable non-text files, like obj, m3u, pnm, csv, xml...
		Code = 3,			// source code or script files, like cpp, py, sh, ...
		FormattedText = 4,	// text files with formatting elements, like html, rtf, srt, ...
		Text = 5			// text files, like txt, log, ...
	};
	
	extern const uint16_t _0xXX;
	
    class fileTypesIndex
    {
        protected:
        
        class node
        {
            public:
            
            fileType Type;
            node * Next[257];
            
            node();
            
            node( fileType type );
        
            ~node();
            
            inline const fileType type() const
            {
                return this->Type;
            }
            
            inline void setType( fileType type )
            {
                this->Type = type;
            }
            
            inline void addNext( uint16_t value )
            {
                this->Next[value] = new node();
            }
            
            inline void addNext( uint16_t value, fileType type )
            {
                this->Next[value] = new node(type);
            }
            
            inline node * operator [] ( uint16_t i )
            {
                return this->Next[i];
            }
        };
        
        node * Root;
        uint16_t FileTypesCount;
        byte MaxSigSize;
    
        void quicklyRegisterFileType( fileType type, 
								      uint16_t * signature, 
									  byte signatureSize );
        
        void registerBasicBinaryTypes();
        
        void registerBasicNonBinaryTypes();
        
        public:
        
        fileTypesIndex();
        
        ~fileTypesIndex();
    
        inline uint16_t fileTypesCount() const
        {
            return this->FileTypesCount;
        }
        
        inline byte maximumSignatureSize() const
        {
            return this->MaxSigSize;
        }
        
        inline void setMaximumSignatureSize( byte size )
        {
            this->MaxSigSize = size;
        }
    
        inline void registerFileType( fileType type, 
							   uint16_t * signature, 
                               byte signatureSize, 
                               bool deleteAfter = true )
		{
			if ((signatureSize == 0) || (signatureSize > this->MaxSigSize)) return;
			this->quicklyRegisterFileType(type, signature, signatureSize);
			if (deleteAfter) delete [] signature;
		}
    
        void registerFileType( fileType type, 
                               const char * signature_ASCII );
                               
        fileType typeOf( byte * data, size_t dataSize ) const;
    
        fileType typeOf( FILE * file ) const;
        
        fileType typeOf( std::string filename ) const;
    };
};

#endif // _RETXT_FILETYPESINDEX_INCLUDED
