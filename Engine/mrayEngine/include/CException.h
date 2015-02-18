

#ifndef ___CException___
#define ___CException___


#include "mString.h"


namespace mray{
namespace core{

class MRAY_DLL CException{

	string m_file;
	string m_line;
	string m_message;
public:

	CException(const char*file,const char*line,const mchar*msg);

	void runError();
	const mchar*getFileName();
	const mchar*getLineNamber();
	const mchar*getMessage();

};

}
}



#endif




