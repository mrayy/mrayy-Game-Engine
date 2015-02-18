
#ifndef ___SSKEYBOX_NODE___
#define ___SSKEYBOX_NODE___


#include "SMesh.h"
#include "ISkyShape.h"
#include "ITexture.h"

namespace mray{
namespace scene{

class MRAY_DLL SSkyBoxNode:public ISkyShape
{
	/*
	S3DVertex vertices[6 * 4];
	Point2df TCoords  [6 * 4];
	int indices[4];
	RenderMaterial *material[6];
	box3d box;
	*/
public:
	SSkyBoxNode();
	virtual ~SSkyBoxNode();

	void SetTexture(video::ITextureCRef skyTex);
	
	virtual bool render(IRenderPass*pass);

};





}
}


#endif