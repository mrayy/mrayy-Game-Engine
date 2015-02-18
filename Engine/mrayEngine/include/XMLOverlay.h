
/********************************************************************
	created:	2009/04/10
	created:	10:4:2009   10:24
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\XMLOverlay.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	XMLOverlay
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___XMLOverlay___
#define ___XMLOverlay___

#include "IStream.h"

namespace mray{
namespace GUI{

	class GUIOverlay;

class MRAY_DLL XMLOverlay
{
private:
protected:

	OS::IStreamPtr m_stream;

public:
	XMLOverlay();
	virtual~XMLOverlay();

	GUIOverlay* loadOverlay(xml::XMLElement*elem);
	GUIOverlay* loadXML(OS::IStream* stream);

};

}
}


#endif //___XMLOverlay___
