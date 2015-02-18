/********************************************************************
	created:	2009/09/03
	created:	3:9:2009   21:41
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ViewportListener.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	ViewportListener
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ViewportListener___
#define ___ViewportListener___

#include "GCPtr.h"

namespace mray{
namespace video{
	class IRenderTarget;
	class IPostProcessing;
}
namespace scene{
	class CameraNode;
	class ViewPort;

class IViewportListener
{
private:
protected:
public:

	virtual void onRenderBegin(ViewPort*vp){};

	virtual void onRenderDone(ViewPort*vp){};

	virtual void onSetCamera(ViewPort*vp,CameraNode* cam){};

	virtual void onSetRenderTarget(ViewPort*vp,video::IRenderTarget* rt){};

	virtual void onSetPostProcessing(ViewPort*vp,video::IPostProcessing *pp){};
};

MakeSharedPtrType(ViewportListener)

}
}


#endif //___ViewportListener___

