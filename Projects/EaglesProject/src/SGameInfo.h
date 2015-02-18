
/********************************************************************
	created:	2009/04/09
	created:	9:4:2009   0:07
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\SGameInfo.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	SGameInfo
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___SGameInfo___
#define ___SGameInfo___

#include <mstring.h>
#include <Point3d.h>

namespace mray{
namespace gameMod{


class SGameInfo
{
private:
protected:
public:
	SGameInfo(){
		objectsCount=0;
	}
	virtual~SGameInfo(){}

public:
	core::string name;
	core::string path;

	core::string terrainHM; //terrain height map

	int objectsCount;

	math::vector3d playerPos;
};

}
}


#endif //___SGameInfo___
