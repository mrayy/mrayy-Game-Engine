#include "stdafx.h"


#include "CPakLoader.h"
#include "IArchive.h"
#include "CPakArchive.h"



namespace mray{
namespace OS{


CPakLoader::~CPakLoader(){
}

IArchivePtr CPakLoader::loadArchive(const  core::string&name){
	if(!canLoadArchive(name))return 0;
	return new CPAKArchive(name);
}
core::string CPakLoader::getArchiveType(){
	return mT(".pak");

}
bool CPakLoader::canLoadArchive(const  core::string&filename){
	core::string str(filename);
	str.make_lower();
	return strSearch(str.c_str(),mT(".pak"))!=0;
}


}
}


