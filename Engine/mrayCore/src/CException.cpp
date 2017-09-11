#include "stdafx.h"


#include "CException.h"
#include "ILogManager.h"
#include "IFileSystem.h"
#include "IErrorDialog.h"
#include "StringConverter.h"


namespace mray{
namespace core{

CException::CException(const char*file,const char*line,const mchar*msg){

	core::string fnamew;
//	char fname[256];
	char_to_string(file,fnamew);
	if(OS::IFileSystem::isExist())
		fnamew=gFileSystem.getShortFileName(fnamew.c_str());
	 
	m_file=fnamew;
	m_line=core::StringConverter::toString(line);
	m_message=msg;
}

void CException::runError(){

	string message;
	message=mT("Fatal Error!!\n\n");
	message+=mT("File :");
	message+=m_file;
	message+=mT("\nLine :");
	message+=m_line;
	message+=mT("\nMessage :");
	message+=m_message;


	//MessageBoxA(0,message.c_str(),"Error",MB_OK | MB_ICONERROR | MB_TASKMODAL);

	if(ILogManager::isExist())
		gLogManager.log(message.c_str(),ELL_ERROR);

}

const mchar*CException::getFileName(){
	return m_file.c_str();
}

const mchar*CException::getLineNamber(){
	return m_line.c_str();
}

const mchar*CException::getMessage(){
	return m_message.c_str();
}

}
}







