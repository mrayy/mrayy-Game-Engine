

/********************************************************************
	created:	2010/08/28
	created:	28:8:2010   21:47
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\CircleTerrainBrush.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	CircleTerrainBrush
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef CircleTerrainBrush_h__
#define CircleTerrainBrush_h__

#include "CompileConfig.h"
#include "ITerrainBrush.h"

namespace mray
{
namespace scene
{

class MRAY_DLL CircleTerrainBrush:public ITerrainBrush
{
private:
protected:
	float m_outerRadius;
	float m_innerRadius;
	float m_falloff;

	inline float CalcWeight(const math::vector3d&p1,const math::vector3d&p2);
public:
	CircleTerrainBrush();
	virtual ~CircleTerrainBrush();

	void SetOuterRadius(float r);
	void SetInnerRadius(float r);
	void SetFalloff(float f);

	virtual void Apply(TerrainUtility* m,const math::vector3d&pos,float strength,ETerrainModificationMode mode);
};

}
}


#endif // CircleTerrainBrush_h__
