

/********************************************************************
	created:	2012/01/26
	created:	26:1:2012   15:54
	filename: 	d:\Development\mrayEngine\Tests\VegetationTest\AtmosphericCubeMapBuilder.h
	file path:	d:\Development\mrayEngine\Tests\VegetationTest
	file base:	AtmosphericCubeMapBuilder
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __AtmosphericCubeMapBuilder__
#define __AtmosphericCubeMapBuilder__

#include "AtmosphericCubeMap.h"

namespace mray
{
namespace game
{

class MRAY_GAME_DLL AtmosphericCubeMapBuilder
{
private:
protected:
	AtmosphericCubeMap::AtmosphericCubeMapData& m_data;
	void _updateCoefficients();

	void _updateExtinction();
	void _updateCubeMap();

	math::vector3d _calculateLin(const math::vector3d& viewDir);
	math::vector3d _calculateExtinctionAmount(float distance);
	float _calculateSkyLength(const math::vector3d& viewDir);
public:
	AtmosphericCubeMapBuilder(AtmosphericCubeMap::AtmosphericCubeMapData&data):m_data(data)
	{}
	virtual~AtmosphericCubeMapBuilder(){}

	void Create();

};

}
}

#endif


