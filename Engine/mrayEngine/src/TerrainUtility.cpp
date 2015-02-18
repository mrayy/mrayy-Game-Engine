#include "stdafx.h"


#include "TerrainUtility.h"
#include "SPatchTerrain.h"

namespace mray
{
namespace scene
{

TerrainUtility::TerrainUtility(SPatchTerrain*terr)
{
	m_terrain=terr; 
}
TerrainUtility::~TerrainUtility()
{
}

math::vector2d TerrainUtility::GetTerrainSize()
{
	return m_terrain->GetSegementsCount();

}

SPatchTerrain* TerrainUtility::GetTerrain()
{
	return m_terrain;
}



math::vector3d* TerrainUtility::LockVerticies()
{
	return (math::vector3d*)m_terrain->getStream(0,video::EMST_Position)->lock(0,0,video::IHardwareBuffer::ELO_Normal);
}
void TerrainUtility::Unlock()
{
	m_terrain->getStream(0,video::EMST_Position)->unlock();
}


}
}


