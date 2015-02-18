#include "stdafx.h"


#include "CPAKArchive.h"
#include "IFileSystem.h"
#include "ILogManager.h"

#include "StringUtil.h"


namespace mray{
namespace OS{

CPAKArchive::CPAKArchive(const  core::string&name):IArchive(name),m_file(0){
	loadArchive();
}
CPAKArchive::~CPAKArchive(){
	unloadArchive();
}

void CPAKArchive::unloadArchive(){
	m_Entrys.clear();
	m_file=0;
}
bool CPAKArchive::loadArchive(){
	m_file=gFileSystem.openFile(getName(),BIN_READ);
	if(!m_file || !m_file->isOpen())return false;

	m_file->seek(0,OS::ESeek_Set);

	m_file->read(&m_Header,sizeof(m_Header));

	if(m_Header.szSignature[0]!='R' || m_Header.szSignature[1]!='W' ||
		m_Header.szSignature[2]!='P' || m_Header.szSignature[3]!='A' ||
		m_Header.szSignature[4]!='C' || m_Header.szSignature[5]!='K' ||
		m_Header.fVersion!=1.0f){

		gLogManager.log(mT("CPAKArchive::loadArchive() - Invalid Pack Signature!"),ELL_WARNING);
		return false;
	}


	readEntrys();


	return true;
}

void CPAKArchive::readEntrys(){
	m_Entrys.resize(m_Header.dwNumFTEntries);

	//m_Entrys.set_sorted(false);

	core::string filename;

	int eSize=sizeof(sFileTableEntry);

	int val=m_Header.iCypherValue;
	if(m_Header.bCypherAddition)
		val*=-1;

	for(int i=0;i<m_Header.dwNumFTEntries;++i){
		m_file->read((void*)&m_Entrys[i].entry,eSize);
		byte*ptr=(byte*)&m_Entrys[i].entry;
		if(val!=0){
			for(int k=0;k<eSize;k++){
				(*ptr++)+=val;
			}
		}
		m_Entrys[i].entry.Next=0;
		core::char_to_string(m_Entrys[i].entry.szFileName,filename);
		m_Entrys[i].fileName=core::StringUtil::ToLower(filename);
		m_Entrys[i].file=new CLimitedStream(m_file, m_Entrys[i].entry.dwFileSize+1,m_Entrys[i].entry.dwOffset,
			m_Entrys[i].fileName.c_str(),BIN_READ);
	}
}

const IStreamPtr& CPAKArchive::getFile(const  core::string&name,FILE_MODE mode){
	sFileEntry e;
	e.fileName==core::StringUtil::ToLower(name);
	std::vector<sFileEntry>::iterator it= std::find(m_Entrys.begin(),m_Entrys.end(),e);
	//int i=m_Entrys.binary_search(e);
	if(it==m_Entrys.end())return IStreamPtr::Null;
	(*it).file->setMode(mode);
	return (*it).file;

}
const IStreamPtr& CPAKArchive::getFile(int index,FILE_MODE mode){
	if(index<0 || index >=m_Entrys.size())return IStreamPtr::Null;

	m_Entrys[index].file->setMode(mode);
	return m_Entrys[index].file;
}

int CPAKArchive::getFileIndex(const  core::string&name){
	for(int i=0;i<m_Entrys.size();++i)
		if(m_Entrys[i].fileName.equals_ignore_case(name))
			return i;
	return -1;
}

int CPAKArchive::getCount(bool dir){
	return m_Entrys.size();
}

void CPAKArchive::getFilesList(bool dirs,TFiles_List & outList){
}


}
}



