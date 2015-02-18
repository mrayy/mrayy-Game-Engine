


#ifndef ___XML_GUITheme___
#define ___XML_GUITheme___

#include "GUITheme.h"

namespace mray{
namespace OS{
	class IStream;
}
namespace GUI{


class MRAY_DLL XML_GUITheme
{
protected:

	static const core::string ThemeElement;
	static const core::string ProfileElement;
	static const core::string RegionElement;

	static IGUITheme* readThemeElement(xml::XMLElement*attrs);
	static GUIProfile* readProfileElement(xml::XMLElement*attrs);
	static void readRegionElement(xml::XMLElement*attrs,GUIProfile*);
	static void readRegionDivElement(xml::XMLElement*attrs,GUIProfile*);

public:

	static IGUITheme* loadXML(OS::IStream *stream);

};

}
}





#endif




