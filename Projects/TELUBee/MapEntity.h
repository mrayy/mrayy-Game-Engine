

/********************************************************************
	created:	2013/02/06
	created:	6:2:2013   3:23
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\MapEntity.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	MapEntity
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __MapEntity__
#define __MapEntity__




namespace mray
{
namespace TBee
{

class MapEntity
{
protected:
	MapMarker m_marker;
	scene::IMovable* m_ent;
public:
	MapEntity();
	virtual~MapEntity();

	MapMarker& GetMarker();
	virtual void Update(float dt);
};

}
}


#endif
