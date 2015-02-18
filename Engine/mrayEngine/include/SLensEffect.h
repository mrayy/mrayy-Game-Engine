

#ifndef ___SLensEffect___
#define ___SLensEffect___

#include "ISceneNode.h"
#include "IMeshBuffer.h"
#include "SBillboardNode.h"

namespace mray{
namespace scene{


class MRAY_DLL SLensEffect:public IRenderable
{

	std::vector<video::ITexturePtr  >m_rings;

	math::box3d bbox;

	math::vector2d screenPos;

	GCPtr<SBillboardNode> m_renderBoard;

	void drawLens(const video::SColor& color,const math::vector3d& point,float size,int texIndex,IRenderPass*pass);

public:

	SLensEffect(ISceneManager* smngr,const math::vector3d& pos);
	virtual ~SLensEffect();

	void loadDefaultTextures();
	void clearRings();

	

	virtual void preRender(IRenderPass*pass);
	virtual void render(IRenderPass*pass);
	virtual void update(float dt);

	virtual int getMaterialCount(){return 1;}
	virtual const video::RenderMaterialPtr& getMaterial(int i=0){return m_renderBoard->getMaterial(i);}
	
	virtual math::box3d getBoundingBox();

};

}
}



#endif



