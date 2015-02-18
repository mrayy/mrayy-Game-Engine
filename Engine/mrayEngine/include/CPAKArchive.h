


#ifndef ___CPAKArchive___
#define ___CPAKArchive___


#include "CLimitedStream.h"
#include "IArchive.h"



namespace mray{
namespace OS{

class MRAY_DLL CPAKArchive:public IArchive
{
	
	// PAK file header
	struct sPakHeader									
	{
		char		szSignature[6];						// PAK Signature should be 'RWPACK'
		float		fVersion;							// Version of PAK file
		ulong		dwNumFTEntries;						// Number of file table entries
		bool		bCypherAddition;					// Should the caesar cypher add (or subtract)
		byte		iCypherValue;						// Random value used for the caesar cypher between 0-255
		char		szUniqueID[10];						// A unique ID for the file (other programs can check it)
		ulong		dwReserved;							// Reserved Value
	};

	// Table Entry per file in the PAK file
	struct sFileTableEntry								
	{
		char				szFileName[30];				// Name of one file in the PAK
		ulong				dwFileSize;					// The size of the file in bytes
		ulong				dwOffset;					// Offset of file in the PAK
		sFileTableEntry*	Next;						// Next file table entry (is a linked list)

		// Constructor
		sFileTableEntry()
		{
			szFileName[0]=0;
			dwFileSize	= 0;
			dwOffset	= 0;
			Next		= NULL;
		}

		// Deconstructor
		~sFileTableEntry()
		{
			dwFileSize	= 0;
			dwOffset	= 0;
			delete Next;
		}
	};
	struct sFileEntry{
		core::string fileName;
		sFileTableEntry entry;
		IStreamPtr file;

		bool operator == (const sFileEntry& o)const{
			return fileName==o.fileName;
		}
		bool operator < (const sFileEntry& o)const{
			return fileName<o.fileName;
		}
	};

	sPakHeader m_Header;
	std::vector<sFileEntry> m_Entrys;

	GCPtr<IStream>m_file;

	void readEntrys();

public:

	CPAKArchive(const  core::string&name);
	virtual~CPAKArchive();

	virtual void unloadArchive();
	virtual bool loadArchive();

	virtual const IStreamPtr& getFile(const  core::string&name,FILE_MODE mode);
	virtual const IStreamPtr& getFile(int index,FILE_MODE mode);

	virtual int getFileIndex(const  core::string&name);

	virtual void getFilesList(bool dirs,TFiles_List & outList);

	virtual int getCount(bool dir);

};


}
}


#endif




