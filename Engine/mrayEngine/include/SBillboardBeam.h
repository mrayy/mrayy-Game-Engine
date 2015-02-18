#ifndef ___SBillboardBeam___
#define ___SBillboardBeam___


#include "MeshRenderableNode.h"


namespace mray{
namespace scene{

class MRAY_DLL SBillboardBeam:public MeshRenderableNode
{
protected:

	math::vector3d m_StartPoint;
	math::vector3d m_EndPoint;

	float m_Width;
	bool m_useWorldSpace;
	bool m_dirty;

	void _init();
public:
	SBillboardBeam();
	SBillboardBeam(const math::vector3d& start,const math::vector3d& end,float width);
	~SBillboardBeam();
	
	void setWidth(float width);
	float getWidth();

	void SetUseWorldSpace(bool u);
	bool GetUseWorldSpace();

	void setStartEndPoints(const math::vector3d& start,const math::vector3d& end);
	void setStartPoint(const math::vector3d& end);
	void setEndPoint(const math::vector3d& start);

	const math::vector3d& getStartPoint();
	const math::vector3d& getEndPoint();

	virtual bool render(IRenderPass*pass);

};





}
}


#endif