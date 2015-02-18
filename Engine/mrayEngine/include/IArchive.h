

#ifndef ___IArchive___
#define ___IArchive___



#include "IStream.h"

#include "GCPtr.h"


namespace mray{
namespace OS{

typedef std::vector<core::string> TFiles_List;

class MRAY_DLL IArchive
{
	core::string m_name;
public:

	IArchive(const  core::string&name):m_name(name)
	{
	}

	virtual~IArchive(){}

	virtual void unloadArchive()=0;
	virtual bool loadArchive()=0;

	virtual const IStreamPtr& getFile(const  core::string&name,FILE_MODE mode)=0;
	virtual const IStreamPtr& getFile(int index,FILE_MODE mode)=0;

	virtual int getFileIndex(const  core::string&name)=0;

	virtual int getCount(bool dir)=0;

	virtual void getFilesList(bool dirs,TFiles_List & outList)=0;

	const  mchar*getName(){return m_name.c_str();}

};

MakeSharedPtrType(IArchive);


}
}




#endif




