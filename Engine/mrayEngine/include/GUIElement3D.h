


/********************************************************************
	created:	2013/01/28
	created:	28:1:2013   14:22
	filename: 	C:\Development\mrayEngine\Engine\mrayEngine\include\GUIElement3D.h
	file path:	C:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	GUIElement3D
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GUIElement3D__
#define __GUIElement3D__

#include "MeshRenderableNode.h"


namespace mray
{
	namespace video
	{
		class RenderPass;
	}
namespace scene
{
	class IGUIElementRT;

class MRAY_DLL GUIElement3D:public MeshRenderableNode
{
protected:
	IGUIElementRT* m_element;
	video::RenderPass* m_rendePass;
public:
	GUIElement3D();
	virtual~GUIElement3D();
	virtual void setOwner(ISceneNode*o);
	void SetElement(IGUIElementRT* e);

	virtual bool preRender(IRenderPass*pass);
};

}
}


#endif
