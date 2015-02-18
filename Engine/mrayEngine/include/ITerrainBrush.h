

/********************************************************************
	created:	2010/08/28
	created:	28:8:2010   21:46
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ITerrainBrush.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ITerrainBrush
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ITerrainBrush_h__
#define ITerrainBrush_h__


#include "CompileConfig.h"

namespace mray
{
namespace scene
{

	class TerrainUtility;

	enum ETerrainModificationMode
	{
		ETMM_Add,
		ETMM_Sub,
		ETMM_SetValue,
		ETMM_Smooth,
		ETMM_Noise
	};

class MRAY_DLL ITerrainBrush
{
private:
protected:
	float ApplyAtVertex(float height,float value,ETerrainModificationMode mode);
public:
	ITerrainBrush(){}
	virtual ~ITerrainBrush(){}
	
	virtual void Apply(TerrainUtility* m,const math::vector3d&pos,float strength,ETerrainModificationMode mode=ETMM_Add)=0;
};

}
}


#endif // ITerrainBrush_h__
