

/********************************************************************
	created:	2013/01/31
	created:	31:1:2013   16:18
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\MapObject.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	MapObject
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __MapObject__
#define __MapObject__

#include "ParsedShaderPP.h"


namespace mray
{
	namespace GUI
	{
		class GUIWebPageControl;
	}
namespace TBee
{
	class MapCamera;
	class MapEntity;
	class MapPageListener;

	class MapObject;
	class IMapListener
	{
	public:
		virtual void OnMarkerlClicked(MapObject* map,float x,float y,int id){}
		virtual void OnMarkerlHovered(MapObject* map,float x,float y,int id){}
		virtual void OnMarkerlOut(MapObject* map,float x,float y,int id){}
	};

class MapObject:public ListenerContainer<IMapListener*>
{
protected:
	MapCamera* m_camera;
	GUI::GUIWebPageControl* m_mapWebpage;
	video::ParsedShaderPP::MappedParams* m_blurParam;
	float m_lastDist;
	float m_lastAngle;
	int m_zoomLevel;
	math::vector2d m_panPos;
	MapPageListener* m_mapListener;

	std::vector<MapEntity*> m_ents;
public:
	DECLARE_FIRE_METHOD(OnMarkerlClicked,(MapObject* map,float x,float y,int id),(map,x,y,id));
	DECLARE_FIRE_METHOD(OnMarkerlHovered,(MapObject* map,float x,float y,int id),(map,x,y,id));
	DECLARE_FIRE_METHOD(OnMarkerlOut,(MapObject* map,float x,float y,int id),(map,x,y,id));
public:
	MapObject();
	virtual~MapObject();

	void Init(MapCamera*cam,GUI::GUIWebPageControl* map);


	void AddMarker(float x,float y,int id);
	void DropMarker();
	void ClearMarkers();
	void ShowMarkers();
	void HideMarkers();

	void Reset();

	void ZoomIn();
	void ZoomOut();
	void SetZoom(int lvl);
	int GetZoom();

	bool SelectRobot(Event*evt);
	void MouseMoved(Event*evt);

	void SetEffect(video::ParsedShaderPP* eff);
	video::ParsedShaderPP::MappedParams*GetBlurParam(){return m_blurParam;}

	MapCamera* GetCamera(){return m_camera;}
	GUI::GUIWebPageControl* GetMapControl(){return m_mapWebpage;}

	void SetPos(float x,float y);
	void ChangeDeltaPos(float dx,float dy);
	void SetTilt(float tilt);
	void ZoomCamera(float delta);
	void SetZoomCamera(float zoom);
	void SetBlur(float amount);

	void HandleCallback(const core::stringw& function,const std::vector<core::stringw>& args);
};

}
}


#endif
