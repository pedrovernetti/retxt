#include "file.hpp"

namespace retxt
{
	using accessStatus = file::accessStatus;
		
	file::file( std::string filePath ) :
		FileHandle(nullptr)
	{
		uint16_t i = 0, length = filePath.size() + 1;
		this->FilePathLength = length;
		if (filePath[length - 1] == fileSeparator) length--;
		this->FilePath = new char[length]{};
		char * thisFilePath = this->FilePath;
		const char * givenFilePath = filePath.data();
		for (; i < length; i++) thisFilePath[i] = givenFilePath[i];
	}
		
	file::~file()
	{
		this->close();
	}
		
	bool file::exists() const
	{
		#if defined(_COMPILING_FOR_POSIX_COMPLIANTS) || defined(_COMPILING_FOR_UNIX)
		struct stat buffer;
		return (!stat(this->FilePath, &buffer));
		#else			
		FILE * fileHandle = std::fopen(this->FilePath, "r");
		if (fileHandle) 
		{
			std::fclose(fileHandle);
			return true;
		}
		return false;
		#endif
	}
	
	bool file::existsAsFile() const
	{
		#if defined(_COMPILING_FOR_POSIX_COMPLIANTS) || defined(_COMPILING_FOR_UNIX)
		struct stat buffer;
		if (stat(this->FilePath, &buffer)) return false;
		return (!S_ISDIR(buffer.st_mode));
		#else
		FILE * fileHandle = std::fopen(this->FilePath, "r+");
		if (fileHandle) 
		{
			std::fclose(fileHandle);
			return true;
		}
		return false;
		#endif
	}
	
	std::string file::name() const
	{
		char * path = this->FilePath;
		size_t i;
		for (i = this->FilePathLength - 1; i > 0; i--)
		{
			if (path[i] == fileSeparator) break;
		}
		return std::string(path + i + 1);
	}
	
	std::string file::containingFolder() const
	{
		char * path = this->FilePath;
		size_t i;
		for (i = this->FilePathLength - 1; i > 0; i--)
		{
			if (path[i] == fileSeparator) break;
		}
		return std::string(path).substr(0, i);
	}
	
	std::string file::extension() const
	{
		char * path = this->FilePath;
		size_t i;
		for (i = this->FilePathLength - 1; i > 0; i--)
		{
			if (path[i] == '.')
			{
				if ((i) && (path[i - 1] == fileSeparator)) i = 0;
				break;
			}
			else if (path[i] == fileSeparator) i = 1;
		}
		if (i) return std::string(path + i + 1);
		return std::string("");
	}
	
	uintmax_t file::size() const
	{
		#if defined(_COMPILING_FOR_POSIX_COMPLIANTS) || defined(_COMPILING_FOR_UNIX)
		struct stat buffer;
		return ((stat(this->FilePath, &buffer)) ? 0 : (buffer.st_size));
		#else
		FILE * fileHandle = this->FileHandle;
		uintmax_t size;
		if (fileHandle)
		{
			uintmax_t position = std::ftell(fileHandle);
			std::fseek(fileHandle, 0, SEEK_END);
			size = (uintmax_t)(std::ftell(fileHandle));
			std::fseek(fileHandle, position, SEEK_SET);
		}
		else if (this->existsAsFile())
		{
			fileHandle = std::fopen(this->FilePath, "r");
			std::fseek(fileHandle, 0, SEEK_END);
			size = (uintmax_t)(std::ftell(fileHandle));
			std::fclose(fileHandle);
		}
		else size = 0;
		return size;
		#endif
	}
	
	accessStatus file::create( bool overwriteIfExists, uintmax_t preallocatedSize )
	{	
		bool exists = this->exists();
		if (exists)
		{
			if (this->existsAsFile())
			{
				if (!overwriteIfExists) return accessStatus::abortedOverwriting;				
				else if (this->FileHandle) std::fclose(this->FileHandle);
			}
			else return accessStatus::directoryNotFile;
		}
		errno = 0;
		FILE * fileHandler = std::fopen(this->FilePath, "w");
		accessStatus status = ((accessStatus)(errno));
		if (fileHandler) 
		{
			if (preallocatedSize)
			{
				std::fseek(fileHandler, (preallocatedSize - 1), SEEK_SET);
				std::fputc(0, fileHandler);
			}
			std::fclose(fileHandler);
		}
		return status;	
	}
		
	accessStatus file::copy( std::string destinationPath, bool overwriteIfExists ) const
	{
		if (((!file::existsAsFile(destinationPath)) && (file::exists(destinationPath))) ||
			((file::existsAsFile(destinationPath)) && (!overwriteIfExists)))
			return accessStatus::abortedOverwriting;
		if (this->existsAsFile())
		{
			bool open = this->FileHandle;
			if (!open) 
			{
				accessStatus status = this->openAsReadOnly();
				if (status) return status;
			}
			FILE * fileHandle = this->FileHandle;
			errno = 0;
			FILE * destination = std::fopen(destinationPath.data(), "w");
			if (errno) return ((accessStatus)(errno));
			uintmax_t size = this->size();				
			std::fseek(destination, (size - 1), SEEK_SET);
			std::fputc(0, destination);
			std::fseek(destination, 0, SEEK_SET);
			for (uintmax_t i = 0; i < size; i++) 
				std::fputc(std::fgetc(fileHandle), destination);
			std::fclose(destination);
			if (!open) this->close();
			return accessStatus::success;
		}
		else if (this->exists()) return accessStatus::directoryNotFile;
		else return accessStatus::nonexistingFile;
	}
		
	accessStatus file::move( std::string destinationPath, bool overwriteIfExists )
	{
		
	}
	
	accessStatus file::remove()
	{
		if (this->FileHandle) std::fclose(this->FileHandle);
		else if (!this->exists()) return accessStatus::nonexistingFile;
		else if (!this->existsAsFile()) return accessStatus::directoryNotFile;
		errno = 0;
		if (std::remove(this->FilePath)) return ((accessStatus)(errno));
		return accessStatus::success;
	}
			
	accessStatus file::open()
	{
		if (this->FileHandle) std::fclose(this->FileHandle);
		else if (!this->exists()) return accessStatus::nonexistingFile;
		else if (!this->existsAsFile()) return accessStatus::directoryNotFile;
		errno = 0;
		this->FileHandle = std::fopen(this->FilePath, "r+");
		if (errno) this->FileHandle = nullptr;
		return ((accessStatus)(errno));
	}
	
	accessStatus file::openAsReadOnly() const
	{
		if (this->FileHandle) std::fclose(this->FileHandle);
		else if (!this->exists()) return accessStatus::nonexistingFile;
		else if (!this->existsAsFile()) return accessStatus::directoryNotFile;
		errno = 0;
		this->FileHandle = std::fopen(this->FilePath, "r");
		if (errno) this->FileHandle = nullptr;
		return ((accessStatus)(errno));
	}
		
	accessStatus file::read( uintmax_t numberOfBytes, byte * readTo ) const
	{
		FILE * const fileHandle = this->FileHandle;
		errno = 0;
		if (fileHandle)
		{
			std::fread(readTo, 1, numberOfBytes, fileHandle);
			return ((accessStatus)(errno));
		}
		return accessStatus::fileNotOpen;
	}
		
	accessStatus file::readAt( uintmax_t position, 
							   uintmax_t numberOfBytes, 
						       byte * data ) const
	{
		FILE * const fileHandle = this->FileHandle;
		errno = 0;
		if (fileHandle)
		{
			std::fseek(fileHandle, position, SEEK_SET);
			std::fread((void *)(data), 1, numberOfBytes, fileHandle);
			return ((accessStatus)(errno));
		}
		return accessStatus::fileNotOpen;
	}
		
	accessStatus file::write( uintmax_t numberOfBytes, byte * data ) const
	{
		FILE * const fileHandle = this->FileHandle;
		errno = 0;
		if (fileHandle)
		{
			std::fwrite((void *)(data), 1, numberOfBytes, fileHandle);
			return ((accessStatus)(errno));
		}
		return accessStatus::fileNotOpen;
	}
		
	accessStatus file::writeAt( uintmax_t position, 
							    uintmax_t numberOfBytes, 
								byte * data ) const
	{
		FILE * const fileHandle = this->FileHandle;
		errno = 0;
		if (fileHandle)
		{
			std::fseek(fileHandle, position, SEEK_SET);
			std::fwrite((void *)(data), 1, numberOfBytes, fileHandle);
			return ((accessStatus)(errno));
		}
		return accessStatus::fileNotOpen;
	}
		
	byte file::operator [] ( uintmax_t position ) const
	{
		FILE * const fileHandle = this->FileHandle;
		if (fileHandle)
		{
			std::fseek(fileHandle, position, SEEK_SET);
			return std::getc(fileHandle); 
		}
		return 0;
	}
	
	file::operator char * () const
	{
		char * filePath = this->FilePath;
		size_t filePathLength = this->FilePathLength;
		char * filePathCopy = new char[filePathLength];
		for (size_t i = 0; i < filePathLength; i++) filePathCopy[i] = filePath[i];
		return filePathCopy;
	}
		
	bool file::exists( const std::string path )
	{
		#if defined(_COMPILING_FOR_POSIX_COMPLIANTS) || defined(_COMPILING_FOR_UNIX)
		struct stat buffer;
		return (!stat(path.data(), &buffer));
		#else			
		FILE * fileHandle = std::fopen(path.data(), "r");
		if (fileHandle) 
		{
			std::fclose(fileHandle);
			return true;
		}
		return false;
		#endif
	}
	
	bool file::existsAsFile( const std::string path )
	{
		#if defined(_COMPILING_FOR_POSIX_COMPLIANTS) || defined(_COMPILING_FOR_UNIX)
		struct stat buffer;
		return ((!stat(path.data(), &buffer)) && (!S_ISDIR(buffer.st_mode)));
		#else
		FILE * fileHandle = std::fopen(path.data(), "r+");
		if (fileHandle) 
		{
			std::fclose(fileHandle);
			return true;
		}
		return false;
		#endif
	}
}
