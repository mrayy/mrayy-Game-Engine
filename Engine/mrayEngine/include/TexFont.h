
#ifndef ___TexFont___
#define ___TexFont___


#include "BaseFont.h"


namespace mray{
namespace GUI{

class MRAY_DLL TexFont:public BaseFont
{
protected:


public:
	TexFont(const core::string &name);

	virtual~TexFont();


//	void loadFont(OS::IStream* file);
	void LoadFont(xml::XMLElement* elem);
	
};

}
}


#endif