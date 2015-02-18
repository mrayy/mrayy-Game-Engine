


/********************************************************************
	created:	2011/12/04
	created:	4:12:2011   12:03
	filename: 	d:\Development\mrayEngine\Tools\BHUIBuilderLib\IBHCanvasObject.h
	file path:	d:\Development\mrayEngine\Tools\BHUIBuilderLib
	file base:	IBHCanvasObject
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IBHCanvasObject__
#define __IBHCanvasObject__

#include <ICanvasObject.h>
#include "BHConnector.h"

namespace mray
{
	namespace AI
	{
		class IBHNode;
	}
namespace canvas
{

	class TextShape;

class IBHCanvasObject:public ICanvasObject
{
private:
protected:

	BHConnector* m_input;
	ConnectorList m_output;

	TextShape* m_label;

	virtual void _BuildShapes()=0;
	virtual void _BuildConnectors();
public:
	IBHCanvasObject();
	virtual~IBHCanvasObject();

	virtual void OnSetNode();
	virtual AI::IBHNode* GetNode()=0;


	virtual void AddConnector(IConnector* s);
	virtual void ClearConnectors();
};

}
}

#endif
