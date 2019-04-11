#include "fileTypesIndex.hpp"

namespace retxt
{
	const uint16_t _0xXX = 256;
	
	fileTypesIndex::node::node() : 
		Type(fileType::Unknown)
	{
		for (uint16_t i = 0; i < 257; i++) this->Next[i] = nullptr;
	}
	
	fileTypesIndex::node::node( fileType type ) : 
		Type(type)
	{
		for (uint16_t i = 0; i < 257; i++) this->Next[i] = nullptr;
	}

	fileTypesIndex::node::~node()
	{
		for (uint16_t i = 0; i < 257; i++) 
		{
			if (this->Next[i] != nullptr) delete this->Next[i];
		}
	}
	
	void fileTypesIndex::quicklyRegisterFileType( fileType type, 
										          uint16_t * signatureBytes, 
										          byte signatureSize )
	{	
		uint16_t i = 0;
		node * current = this->Root;
		for (signatureSize--; i < signatureSize; i++)
		{
			if ((* current)[signatureBytes[i]] == nullptr)
				current->addNext(signatureBytes[i]);
			current = (*current)[signatureBytes[i]];
		}
		if ((* current)[signatureBytes[i]] != nullptr) 
			(* current)[signatureBytes[i]]->setType(type);
		else
		{
			current->addNext(signatureBytes[i], type);
			this->FileTypesCount++;
		}
	}
	
	// registra assinaturas de arquivos que contém bytes não-printáveis
	void fileTypesIndex::registerBasicBinaryTypes()
	{
		uint16_t * s;
		
		s = new uint16_t[6] {55, 122, 188, 175, 39, 28}; // .7z
			this->quicklyRegisterFileType(fileType::Binary, s, 6);
			delete[] s;
		s = new uint16_t[14] {_0xXX, _0xXX, _0xXX, 0, 0, 0, 144, 42, 42, 65, 67, 69, 42, 42}; // .ace
			this->quicklyRegisterFileType(fileType::Binary, s, 14);
			delete[] s;
		s = new uint16_t[4] {65, 114, 67, 1}; // .arc
			this->quicklyRegisterFileType(fileType::Binary, s, 4);
			delete[] s;
		s = new uint16_t[2] {26, 8}; // .arc
			this->quicklyRegisterFileType(fileType::Binary, s, 2);
			delete[] s;
		s = new uint16_t[2] {26, 9}; // .arc
			this->quicklyRegisterFileType(fileType::Binary, s, 2);
			delete[] s;
		s = new uint16_t[3] {31, 139, 8}; // .gz
			this->quicklyRegisterFileType(fileType::Binary, s, 3);
			delete[] s;
		s = new uint16_t[4] {4, 34, 77, 24}; // .lz4
			this->quicklyRegisterFileType(fileType::Binary, s, 4);
			delete[] s;
		s = new uint16_t[4] {77, 80, 81, 26}; // .mpq
			this->quicklyRegisterFileType(fileType::Binary, s, 4);
			delete[] s;
		s = new uint16_t[7] {82, 97, 114, 33, 26, 7, 0}; // .rar
			this->quicklyRegisterFileType(fileType::Binary, s, 7);
			delete[] s;
		s = new uint16_t[8] {82, 97, 114, 33, 26, 7, 1, 0}; // .rar
			this->quicklyRegisterFileType(fileType::Binary, s, 8);
			delete[] s;
		s = new uint16_t[6] {117, 115, 116, 97, 114, 0}; // .tar
			this->quicklyRegisterFileType(fileType::Binary, s, 6);
			delete[] s;
		s = new uint16_t[8] {117, 115, 116, 97, 114, 0, 48, 48}; // .tar
			this->quicklyRegisterFileType(fileType::Binary, s, 8);
			delete[] s;
		s = new uint16_t[8] {117, 115, 116, 97, 114, 32, 32, 0}; // .tar
			this->quicklyRegisterFileType(fileType::Binary, s, 8);
			delete[] s;
		s = new uint16_t[6] {253, 55, 122, 88, 90, 0}; // .xz
			this->quicklyRegisterFileType(fileType::Binary, s, 6);
			delete[] s;
		s = new uint16_t[4] {80, 75, 3, 4}; // .zip [...]
			this->quicklyRegisterFileType(fileType::Binary, s, 4);
			delete[] s;
		s = new uint16_t[4] {80, 75, 5, 6}; // .zip [...]
			this->quicklyRegisterFileType(fileType::Binary, s, 4);
			delete[] s;
		s = new uint16_t[4] {80, 75, 7, 8}; // .zip [...]
			this->quicklyRegisterFileType(fileType::Binary, s, 4);
			delete[] s;
		s = new uint16_t[8] {68, 67, 77, 1, 80, 65, 51, 48}; // windows update binary delta compression
			this->quicklyRegisterFileType(fileType::Binary, s, 8);
			delete[] s;
			
		s = new uint16_t[10] {73, 73, 42, 0, 16, 0, 0, 0, 67, 82}; // .cr2
			this->quicklyRegisterFileType(fileType::Binary, s, 10);
			delete[] s;
		s = new uint16_t[8] {73, 73, 26, 0, 0, 0, 72, 69}; // .crw
			this->quicklyRegisterFileType(fileType::Binary, s, 8);
			delete[] s;
		s = new uint16_t[4] {0, 0, 2, 0}; // .cur
			this->quicklyRegisterFileType(fileType::Binary, s, 4);
			delete[] s;
		s = new uint16_t[4] {0, 0, 1, 0}; // .ico
			this->quicklyRegisterFileType(fileType::Binary, s, 4);
			delete[] s;
		s = new uint16_t[8] {_0xXX, _0xXX, 78, 71, 13, 10, 26, 10}; // .jng .mng .png
			this->quicklyRegisterFileType(fileType::Binary, s, 8);
			delete[] s;
		s = new uint16_t[5] {56, 66, 80, 83, 0}; // .psd [...]
			this->quicklyRegisterFileType(fileType::Binary, s, 5);
			delete[] s;
		s = new uint16_t[10] {31, 139, 8, 0, 0, 0, 0, 0, 0, 3}; // .svgz
			this->quicklyRegisterFileType(fileType::Binary, s, 10);
			delete[] s;
		s = new uint16_t[4] {73, 73, 42, 0}; // .tiff
			this->quicklyRegisterFileType(fileType::Binary, s, 4);
			delete[] s;
		s = new uint16_t[4] {77, 77, 0, 42}; // .tiff
			this->quicklyRegisterFileType(fileType::Binary, s, 4);
			delete[] s;
		s = new uint16_t[4] {77, 77, 0, 43}; // .tiff
			this->quicklyRegisterFileType(fileType::Binary, s, 4);
			delete[] s;
			
		s = new uint16_t[12] {77, 65, 67, 32, 150, 15, 0, 0, 52, 0, 0, 0}; // .ape
			this->quicklyRegisterFileType(fileType::Binary, s, 12);
			delete[] s;
		s = new uint16_t[8] {102, 76, 97, 67, 0, 0, 0, 34}; // .flac
			this->quicklyRegisterFileType(fileType::Binary, s, 8);
			delete[] s;
		s = new uint16_t[4] {26, 69, 223, 163}; // .mkv .mka .webm [...]
			this->quicklyRegisterFileType(fileType::Binary, s, 4);
			delete[] s;
		s = new uint16_t[8] {0, 0, 0, _0xXX, 102, 116, 121, 112}; // .mp4 .m4a .mov [...]
			this->quicklyRegisterFileType(fileType::Binary, s, 8);
			delete[] s;
		s = new uint16_t[4] {0, 0, 1, 179}; // .mpeg .mpg [...]
			this->quicklyRegisterFileType(fileType::Binary, s, 4);
			delete[] s;
		s = new uint16_t[4] {0, 0, 1, 183}; // .mpeg .mpg [...]
			this->quicklyRegisterFileType(fileType::Binary, s, 4);
			delete[] s;
		s = new uint16_t[4] {77, 80, 43, 7}; // .mpc .mp+
			this->quicklyRegisterFileType(fileType::Binary, s, 4);
			delete[] s;
		s = new uint16_t[6] {78, 69, 83, 77, 26, 1}; // .nsf
			this->quicklyRegisterFileType(fileType::Binary, s, 6);
			delete[] s;
		s = new uint16_t[8] {48, 38, 178, 117, 142, 102, 207, 17}; // .wma .wmv [...]
			this->quicklyRegisterFileType(fileType::Binary, s, 8);
			delete[] s;
			
		s = new uint16_t[15] {24, 70, 73, 67, 72, 73, 69, 82, 32, 71, 85, 73, 84, 65, 82}; // .gp3 .gp4 .gp5 [...]
			this->quicklyRegisterFileType(fileType::Binary, s, 15);
			delete[] s;
		s = new uint16_t[4] {63, 95, 3, 0}; // .hlp
			this->quicklyRegisterFileType(fileType::Binary, s, 4);
			delete[] s;
		s = new uint16_t[16] {27, 0, 84, 0, 117, 0, 120, 0, 71, 0, 117, 0, 105, 0, 116, 0}; // .tg
			this->quicklyRegisterFileType(fileType::Binary, s, 16);
			delete[] s;
			
		s = new uint16_t[8] {100, 101, 120, 10, 48, 51, 53, 0}; // .dex
			this->quicklyRegisterFileType(fileType::Binary, s, 8);
			delete[] s;
		s = new uint16_t[4] {78, 69, 83, 26}; // .nes
			this->quicklyRegisterFileType(fileType::Binary, s, 4);
			delete[] s;
		s = new uint16_t[4] {0, 97, 115, 109}; // .wasm
			this->quicklyRegisterFileType(fileType::Binary, s, 4);
			delete[] s;
		s = new uint16_t[4] {79, 98, 106, 1}; // avro
			this->quicklyRegisterFileType(fileType::Binary, s, 4);
			delete[] s;
			
		s = new uint16_t[11] {79, 84, 84, 79, 0, 11, 0, 128, 0, 3, 0}; // .otf
			this->quicklyRegisterFileType(fileType::Binary, s, 11);
			delete[] s;

		s = new uint16_t[2] {76, 1}; // .o
			this->quicklyRegisterFileType(fileType::Binary, s, 2);
			delete[] s;
	}
        
    void fileTypesIndex::registerBasicNonBinaryTypes()
    {
		uint16_t * s;
		
		s = new uint16_t[5] {60, 63, 120, 109, 108}; // .xml
			this->quicklyRegisterFileType(fileType::Binary, s, 5);
			delete[] s;
			
		s = new uint16_t[5] {123, 92, 114, 116, 102}; // .rtf
			this->quicklyRegisterFileType(fileType::Binary, s, 5);
			delete[] s;
			
		s = new uint16_t[5] {60, 63, 119, 112, 108}; // .wpl
			this->quicklyRegisterFileType(fileType::Binary, s, 5);
			delete[] s;
		s = new uint16_t[10] {91, 112, 108, 97, 121, 108, 105, 115, 116, 93}; // .pls
			this->quicklyRegisterFileType(fileType::Binary, s, 10);
			delete[] s;
	}
	
	fileTypesIndex::fileTypesIndex() : 
		Root(new node()), FileTypesCount(0), MaxSigSize(64)
	{
		this->registerBasicBinaryTypes();
		this->registerBasicNonBinaryTypes();
	}
	
	fileTypesIndex::~fileTypesIndex()
	{
		delete this->Root;
	}
	
	void fileTypesIndex::registerFileType( fileType type, 
										   const char * signatureASCII )
	{
		size_t signatureSize = 0;
		for (uint16_t i = 0; signatureASCII[i] != 0; i++, signatureSize++);
		uint16_t * signatureBytes = new uint16_t[signatureSize];
		for (uint16_t i = 0; signatureASCII[i] != 0; i++)
			signatureBytes[i] = signatureASCII[i];
		this->registerFileType(type, signatureBytes, signatureSize, true);
	}
	
    fileType fileTypesIndex::typeOf( byte * data, size_t dataSize ) const
    {
		node * current = this->Root, * bestCandidate = this->Root;
		for (uint16_t i = 0; i < dataSize; i++)
		{
			if ((* current)[data[i]] != nullptr)
			{
				current = (* current)[data[i]];
				if ((current->Type) != fileType::Unknown) bestCandidate = current;
			}
			else break;
		}
		return bestCandidate->Type;
	}
    
	fileType fileTypesIndex::typeOf( FILE * file ) const
	{
		uint16_t bufferSize = this->MaxSigSize;
		byte signature[bufferSize + 1];
		if (!std::fread((byte *)(signature), sizeof(byte), bufferSize, file)) 
			return fileType::Unknown;
		return this->typeOf(signature, bufferSize);
	}
	
	fileType fileTypesIndex::typeOf( std::string filename ) const
	{
		FILE * fileHandle = std::fopen(filename.data(), "r");
		if (!fileHandle) return fileType::Unknown;
		retxt::fileType type = this->typeOf(fileHandle);
		std::fclose(fileHandle);
		return type;
	}
}
