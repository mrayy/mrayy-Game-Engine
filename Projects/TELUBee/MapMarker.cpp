

#include "stdafx.h"
#include "MapMarker.h"



namespace mray
{
namespace TBee
{

MapMarker::MapMarker()
{
}

MapMarker::~MapMarker()
{
}


void MapMarker::SetLatLng(float lat,float lng)
{
	m_latLng.set(lat,lng);
}

const math::vector2d& MapMarker::GetLatLng()
{
	return m_latLng;
}

math::vector2d MapMarker::Get2DPos()
{
	math::vector2d ret;

	ret;

	return ret;
}

}
}
