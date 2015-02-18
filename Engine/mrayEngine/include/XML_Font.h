


#ifndef ___XML_Font___
#define ___XML_Font___

#include "IFont.h"
#include "IStream.h"

namespace mray{
namespace GUI{


class MRAY_DLL XML_Font
{
protected:
	IFont* readFontElement(xml::XMLElement*attrs);

public:

	XML_Font()
	{
	}

	IFont* loadXML(OS::IStream* stream);
};

}
}





#endif




