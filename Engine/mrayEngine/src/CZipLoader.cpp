#include "stdafx.h"


#include "CZipLoader.h"
#include "IArchive.h"
#include "CZipArchive.h"

#include "StringUtil.h"


namespace mray{
namespace OS{

CZipLoader::~CZipLoader(){
}

IArchivePtr CZipLoader::loadArchive(const  core::string&name){
	if(!canLoadArchive(name))return 0;
	return new CZipArchive(name);
}
core::string CZipLoader::getArchiveType(){
	return mT(".zip");

}
bool CZipLoader::canLoadArchive(const  core::string&filename){
	core::string str=core::StringUtil::ToLower(filename);
	return strSearch(str.c_str(),mT(".zip"))!=0;
}


}
}


