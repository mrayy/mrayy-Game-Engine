


/********************************************************************
	created:	2010/08/28
	created:	28:8:2010   21:45
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\TerrainUtility.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	TerrainUtility
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef TerrainUtility_h__
#define TerrainUtility_h__

#include "CompileConfig.h"



namespace mray
{
namespace scene
{

	class SPatchTerrain;
class MRAY_DLL TerrainUtility
{
private:
protected:
	SPatchTerrain* m_terrain;
public:
	TerrainUtility(SPatchTerrain*terr);
	virtual ~TerrainUtility();

	math::vector2d GetTerrainSize();
	SPatchTerrain* GetTerrain();
	
	math::vector3d* LockVerticies();
	void Unlock();
};

}
}


#endif // TerrainUtility_h__
