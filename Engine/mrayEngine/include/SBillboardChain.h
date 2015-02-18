#ifndef ___SBillboardChain___
#define ___SBillboardChain___


#include "MeshRenderableNode.h"



namespace mray{
namespace scene{

class MRAY_DLL SBillboardChain:public MeshRenderableNode
{
protected:
	float m_fStartWidth;
	float m_fEndWidth;
	//! array to keep track of points positions
	std::vector<math::vector3d> m_vPoses;
	float m_fInterval;
	float m_fTime;
	int m_currIndex;
	int m_startIndex;

	float m_currV;
	float m_deltaV;

	bool m_dirty;
	void _create();

	void _init();
public:
	SBillboardChain();
	SBillboardChain(int segments,float startWidth,float endWidth);
	virtual~SBillboardChain();

	void SetStartWidth(float w);
	void SetEndWidth(float w);
	float GetStartWidth(){return m_fStartWidth;}
	float GetEndWidth(){return m_fEndWidth;}

	int GetPointsCount(){return m_vPoses.size();}
	void SetMaxPointCount(int c);
	void SetPoints(const math::vector3d* pts,int n);
	bool SetPoint(int i,const math::vector3d& pt);
	const std::vector<math::vector3d>& GetPoints(){return m_vPoses;}
	const math::vector3d& GetPoint(int i);

	virtual bool preRender(IRenderPass*pass);
	//virtual bool render(IRenderPass*pass);
	virtual void update(float dt);

	void setRecordInterval(float interval);
	float getRecordInterval();
};





}
}


#endif