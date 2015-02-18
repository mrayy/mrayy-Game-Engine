

#ifndef ___CZipArchive___
#define ___CZipArchive___

#include "IArchive.h"


#include <zzip\zzip.h>


namespace mray{
namespace OS{


class MRAY_DLL CZipArchive:public IArchive
{
protected:


	class MRAY_DLL CZipFile:public IStream
	{
	protected:
		ZZIP_FILE *m_zipfile;
		bool m_isOpen;
		virtual uint calcSizeInternal();
	public:
		CZipFile(const  mchar*name,ZZIP_FILE*zipFile,int size);
		virtual ~CZipFile();


		virtual void close();

		virtual bool canRead();
		virtual bool canWrite();
		bool canSeek(){return true;}

		virtual bool isOpen();
		virtual int read(void*_data,size_t _size);
		virtual int write(const void*_data,size_t _size);
		virtual int seek(int bytes,EStreamSeek smode);
		virtual int getPos();;
		virtual bool eof();

	};


	struct sFileEntry{
		core::string fileName;
		core::string filePath;
		core::string dirPath;
		size_t compressSize;
		size_t uncompressSize;

		bool isfolder;
		IStreamPtr fileData;

		bool operator < (const sFileEntry& o)const{
			return fileName<o.fileName;
		}
	};
	zzip_dir* m_zipDir;

	std::vector<sFileEntry> m_files;
	//GCPtr<WinFile> m_file;

	bool checkForError(int err,const mchar*desc);
public:

	CZipArchive(const  core::string&name);
	virtual~CZipArchive();

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




