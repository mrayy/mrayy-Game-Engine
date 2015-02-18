
/********************************************************************
	created:	2013/02/06
	created:	6:2:2013   12:45
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\MapMarker.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	MapMarker
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __MapMarker__
#define __MapMarker__




namespace mray
{
namespace TBee
{

class MapMarker
{
protected:
	math::vector2d m_latLng;
public:
	MapMarker();
	virtual~MapMarker();

	void SetLatLng(float lat,float lng);
	const math::vector2d& GetLatLng();

	math::vector2d Get2DPos();
};

}
}


#endif


