#include "stdafx.h"

#include "CircleTerrainBrush.h"
#include "TerrainUtility.h"
#include "spatchterrain.h"


namespace mray
{
namespace scene
{

CircleTerrainBrush::CircleTerrainBrush()
{
	m_outerRadius=1;
	m_innerRadius=0;
	m_falloff=1;
}
CircleTerrainBrush::~CircleTerrainBrush()
{
}


float CircleTerrainBrush::CalcWeight(const math::vector3d&p1,const math::vector3d&p2)
{

	float len=p1.getDistSQ(p2);
	if(len<=m_innerRadius*m_innerRadius)
		return 1;
	else if(len>=m_outerRadius*m_outerRadius)
		return 0;
	else
	{
		len=sqrt(len);
		return 1-(len-m_innerRadius)/(m_outerRadius-m_innerRadius);
	}
}

void CircleTerrainBrush::SetOuterRadius(float r)
{
	m_innerRadius=math::Min(r,m_innerRadius);
	m_outerRadius=math::Max(m_innerRadius,r);
}
void CircleTerrainBrush::SetInnerRadius(float r)
{
	m_outerRadius=math::Max(m_outerRadius,r);
	m_innerRadius=math::Min(r,m_outerRadius);
}
void CircleTerrainBrush::SetFalloff(float f)
{
	m_falloff=math::clamp(f,0.0f,1.0f);
}

void CircleTerrainBrush::Apply(TerrainUtility* m,const math::vector3d& pos,float strength,ETerrainModificationMode mode)
{
	math::vector3d*vtx= m->LockVerticies();

	//compute min and max bounding box of the brush in XZ plane
	math::vector3d minP=pos-m_outerRadius;
	math::vector3d maxP=pos+m_outerRadius;

	int segments=m->GetTerrain()->GetSegementsCount();

	//convert it to terrain space
	minP=m->GetTerrain()->TransformVectorToLocal(minP);
	maxP=m->GetTerrain()->TransformVectorToLocal(maxP);
	minP.x=math::Max(0,(int)minP.x);
	minP.z=math::Max(0,(int)minP.z);

	maxP.x=math::Min(segments-1,(int)maxP.x);
	maxP.z=math::Min(segments-1,(int)maxP.z);

	float weight;

	//math::vector3d tPos=m->GetTerrain()->TransformVector(pos);
	math::vector3d tPos(pos);
	tPos.y=0;

	for(int i=minP.x;i<maxP.x;++i)
	{
		for(int j=minP.z;j<maxP.z;++j)
		{
			int idx=i*segments+j;
			math::vector3d vPos=vtx[idx];
			vPos.y=0;
			weight=CalcWeight(tPos,vPos);
			if(weight>0){
				vtx[idx].y=ApplyAtVertex(vtx[idx].y,strength*weight,mode);
				m->GetTerrain()->OnHeightChanged(idx,vtx[idx].y);
			}
		}
	}


	m->Unlock();
}


}
}
