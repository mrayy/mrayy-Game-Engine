

/********************************************************************
	created:	2013/01/26
	created:	26:1:2013   23:36
	filename: 	C:\Development\mrayEngine\Tests\TestOpenCV\GMStyleInfo.h
	file path:	C:\Development\mrayEngine\Tests\TestOpenCV
	file base:	GMStyleInfo
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GMStyleInfo__
#define __GMStyleInfo__



namespace mray
{
namespace google
{

	enum MapTypeStyleElementType
	{
		All,
		Gemoetry,
		GeometryFill,
		GeometryStroke,
		Labels,
		LabelsIcon,
		LabelsText,
		LabelsTextFill,
		LabelsTextStroke,
		Count
	};
	enum MapTypeStyleFeatureType
	{
		Administrative,
		AdministrativeCountry,
		AdministrativeLandParcel,
		AdministrativeLocality,
		AdministrativeNeighborhood,
		AdministrativeProvince,
		All,
		Landscape,
		LandscapeManMade,
		LandscapeNatural,
		LandscapeNaturalLandCover,
		LandscapeNaturalTerrain,
		POI,
		POI_Attraction,
		POI_Business,
		POI_Government,
		POI_Medical,
		POI_Park,
		POI_PlaceOfWorship,
		POI_School,
		POI_SportsComplex,
		Road,
		RoadArterial,
		RoadHighway,
		RoadHighwayControlledAccess,
		RoadLocal,
		Transit,
		TransitLine,
		TransitStation,
		TransitStationAirport,
		TransitStationBus,
		TransitStationRail,
		Water,
		Count
	};

	class GMVisitor;

class GMStyleInfo
{
protected:
public:
	GMStyleInfo();
	virtual~GMStyleInfo();

	MapTypeStyleFeatureType feature;
	MapTypeStyleElementType element;
	bool visible;
	video::SColor hue;
	int saturation;
	int lightness;
	int weight;

	virtual void Visit(GMVisitor* v);

	static const core::string& ToString(MapTypeStyleFeatureType t);
	static const core::string& ToString(MapTypeStyleElementType t);
};

}
}


#endif
