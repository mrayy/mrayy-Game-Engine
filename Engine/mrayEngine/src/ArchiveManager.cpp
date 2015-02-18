#include "stdafx.h"


#include "ArchiveManager.h"
#include "IArchive.h"
#include "IArchiveLoader.h"
#include "IFileSystem.h"
#include "TraceManager.h"

#include "common.h"

namespace mray{



namespace OS{

ArchiveManager::ArchiveManager(){
	addArchiveLoader(new CPakLoader());
	addArchiveLoader(new CZipLoader());
}
ArchiveManager::~ArchiveManager(){
	{
		ArchiveList::iterator it=m_Archives.begin();
		for(;it!=m_Archives.end();++it){
			it->second=0;
		}
	}

	{
		ArchiveLoaders::iterator it=m_ArchiveLoaders.begin();
		for(;it!=m_ArchiveLoaders.end();++it){
			it->second=0;
		}
	}
	m_Archives.clear();
	m_ArchiveLoaders.clear();
}
	
IArchivePtr ArchiveManager::addArchive(const  core::string&filename){
	traceFunction(eOS);
	core::string str(filename);
	
	IArchivePtr a=getArchive(filename);
	if(a)
		return a;

	gFileSystem.getCorrectFilePath(filename,str);

	ArchiveLoaders::iterator it= m_ArchiveLoaders.begin();
	for(;it!=m_ArchiveLoaders.end();++it){
		if(it->second->canLoadArchive(filename)){
			a=it->second->loadArchive(str.c_str());
			if(a)
				m_Archives.insert(ArchiveList::value_type(filename,a));
			
			return a;
		}
	}
	
	return IArchivePtr::Null;
}
void ArchiveManager::removeArchive(const  core::string&filename)
{
	ArchiveList::iterator it= m_Archives.find(filename);

	if(it!=m_Archives.end())
	{
		it->second=0;
		m_Archives.erase(it);
	}

}
const IArchivePtr&  ArchiveManager::getArchive(const  core::string&archive){
	ArchiveList::iterator it= m_Archives.find(archive);

	if(it!=m_Archives.end()){
		
		return it->second;
	}
	return IArchivePtr::Null;
}



const IStreamPtr& ArchiveManager::openFile(const  core::string&filename,const  core::string&archive,FILE_MODE mode){

	GCPtr<IArchive>ar;
	ar=getArchive(archive);
	if(!ar)
		ar=addArchive(archive);
	if(!ar)return IStreamPtr::Null;
	return ar->getFile(filename,mode);
}
const IStreamPtr& ArchiveManager::searchForFile(const  core::string&filename,FILE_MODE mode){
	ArchiveList::iterator it= m_Archives.begin();
	for(;it!=m_Archives.end();++it){
		const IStreamPtr& f=it->second->getFile(filename,mode);
		if(f){
			return f;
		}
	}
	return IStreamPtr::Null;
}



void ArchiveManager::addArchiveLoader(GCPtr<IArchiveLoader> loader){
	m_ArchiveLoaders.insert(ArchiveLoaders::value_type(loader->getArchiveType(),loader));
}
void ArchiveManager::removeArchiveLoader(const  core::string&type){
	m_ArchiveLoaders.erase(type);
}


}
}


