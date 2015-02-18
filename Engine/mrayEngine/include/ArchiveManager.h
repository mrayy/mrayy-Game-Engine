
#ifndef ___ArchiveManager___
#define ___ArchiveManager___

#include "ISingleton.h"
#include "GCPtr.h"
#include "IStream.h"

#include "CPakLoader.h"
#include "CZipLoader.h"
#include "IArchive.h"
#include "IArchiveLoader.h"

namespace mray{
namespace OS{


class MRAY_DLL ArchiveManager:public ISingleton<ArchiveManager>
{
	typedef std::map<core::string,GCPtr<IArchive>> ArchiveList;
	typedef std::map<core::string,GCPtr<IArchiveLoader>> ArchiveLoaders;

	ArchiveList m_Archives;
	ArchiveLoaders m_ArchiveLoaders;

public:
	ArchiveManager();
	virtual~ArchiveManager();
	
	IArchivePtr addArchive(const  core::string&filename);
	const IArchivePtr& getArchive(const  core::string&archive);
	void removeArchive(const  core::string&filename);

	const IStreamPtr& openFile(const  core::string&filename,const  core::string&archive,FILE_MODE mode);
	const IStreamPtr&  searchForFile(const  core::string&filename,FILE_MODE mode);

	void addArchiveLoader(GCPtr<IArchiveLoader> loader);
	void removeArchiveLoader(const  core::string&type);
};

}
}



#endif


