#pragma once
#ifndef _RETXT_FILE_INCLUDED
#define _RETXT_FILE_INCLUDED

#include "globalUtilities.hpp"

#if defined(_COMPILING_FOR_POSIX_COMPLIANTS) || defined(_COMPILING_FOR_UNIX)
#include <sys/stat.h>
#elif defined(_COMPILING_FOR_WINDOWS)
#include <windows.h>
#endif

namespace retxt
{
	#if defined(_COMPILING_FOR_LINUX) || defined(_COMPILING_FOR_ANDROID) || defined(_COMPILING_FOR_AMIGAOS)
	constexpr char fileSeparator = '/';
	#elif defined(_COMPILING_FOR_WINDOWS) || defined(_COMPILING_FOR_WINDOWSCE) || defined(_COMPILING_FOR_WIN16) || defined(_COMPILING_FOR_DOS) || defined(_COMPILING_FOR_OS2)
	constexpr char fileSeparator = '\\';
	#elif defined(_COMPILING_FOR_UNIX)
	constexpr char fileSeparator = '/';
	#else
	constexpr char fileSeparator = 0;
	#endif
	
	class file
	{
		public:
		
		enum accessStatus : byte
		{
			success = 0,
			operationNotPermitted = 1,
			nonexistingFile = 2,
			nonexistingProcess = 3,
			interruptedSystemCall = 4,
			IOError = 5,
			directoryNotFile = 6,
			abortedOverwriting = 7,
			fileNotOpen = 8
		};
		
		protected:
		
		mutable FILE * FileHandle;
		char * FilePath;
		uint16_t FilePathLength;
		
		public:
		
		file( std::string filePath );
		
		~file();
		
		bool exists() const;
		
		bool existsAsFile() const;
		
		inline std::string path() const
		{
			return std::string(this->FilePath);
		}
		
		std::string name() const;
		
		std::string containingFolder() const;
		
		std::string extension() const;
		
		uintmax_t size() const;
		
		accessStatus create( bool overwriteIfExists = false, 
							 uintmax_t preallocatedSize = 0 );
		
		accessStatus copy( std::string destinationPath, bool overwriteIfExists = false ) const;
		
		accessStatus move( std::string destinationPath, bool overwriteIfExists = false );
		
		accessStatus remove();
		
		template <typename function>
		accessStatus shred( function && randFunction, 
							byte iterations = 1, 
							bool finalZeroOverwrite = false )
		{
			if (this->FileHandle) std::fclose(this->FileHandle);
			else if (!this->exists()) return accessStatus::nonexistingFile;
			else if (!this->existsAsFile()) return accessStatus::directoryNotFile;
			errno = 0;
			FILE * fileHandle = std::fopen(this->FilePath, "r+");
			if (!fileHandle) return ((accessStatus)(errno));
			uintmax_t size = this->size();
			uint16_t stepSize = ((512 > size) ? size : 512);
			uintmax_t steps = (size / stepSize), lastStepSize = (size % stepSize);
			for (byte iteration = 0; iteration < iterations; iteration++)
			{
				for (uintmax_t i = 0; i < size; i++) std::fputc(randFunction(), fileHandle);
			}
			if (finalZeroOverwrite)
			{
				for (uintmax_t i = 0; i < size; i++) std::fputc(0, fileHandle);
			}
			std::fclose(this->FileHandle);
			this->FileHandle = nullptr;
			errno = 0;
			std::remove(this->FilePath);
			return ((accessStatus)(errno));
		}
		
		inline bool isOpen() const
		{
			return (this->FileHandle);
		}
		
		accessStatus open();
		
		accessStatus openAsReadOnly() const;
		
		inline void close() const
		{
			if (this->FileHandle) std::fclose(this->FileHandle);
			this->FileHandle = nullptr;
		}
		
		inline bool seek( uintmax_t position ) const
		{
			if (std::fseek(this->FileHandle, position, SEEK_SET)) return false;
			return true;
		}
		
		inline bool seekFromCurrentPosition( uintmax_t position ) const
		{
			if (std::fseek(this->FileHandle, position, SEEK_CUR)) return false;
			return true;
		}
		
		inline bool seekEnd() const
		{
			if (std::fseek(this->FileHandle, 0, SEEK_END)) return false;
			return true;
		}
		
		inline bool quicklyRead( uintmax_t numberOfBytes, byte * readTo ) const
		{
			return (std::fread(readTo, 1, numberOfBytes, this->FileHandle) == numberOfBytes);
		}
		
		accessStatus read( uintmax_t numberOfBytes, byte * readTo ) const;
		
		accessStatus readAt( uintmax_t position, 
							 uintmax_t numberOfBytes, 
							 byte * data ) const;
		
		inline bool quicklyWrite( uintmax_t numberOfBytes, byte * readTo ) const
		{
			return (std::fwrite(readTo, 1, numberOfBytes, this->FileHandle) == numberOfBytes);
		}
		
		accessStatus write( uintmax_t numberOfBytes, byte * data ) const;
		
		accessStatus writeAt( uintmax_t position, 
							  uintmax_t numberOfBytes, 
							  byte * data ) const;
		
		inline byte getByte() const
		{
			return ((this->FileHandle) ? ((byte)(std::getc(this->FileHandle))) : 0);
		}
		
		byte operator [] ( uintmax_t position ) const;
		
		operator char * () const;
		
		inline operator std::string () const
		{
			return std::string(this->FilePath);
		}
		
		inline operator FILE * () const
		{
			return this->FileHandle;
		}
		
		static bool exists( const std::string path );
		
		static bool existsAsFile( const std::string path );
	};
}

#endif // _RETXT_FILE_INCLUDED
