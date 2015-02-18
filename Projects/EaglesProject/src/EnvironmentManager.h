
/********************************************************************
	created:	2009/03/29
	created:	29:3:2009   18:36
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\EnvironmentManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject
	file base:	EnvironmentManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___EnvironmentManager___
#define ___EnvironmentManager___

#include <ISingleton.h>
#include <GCPtr.h>
#include <SSkyBoxNode.h>
#include <SkyBoxManager.h>
#include "Sun.h"

namespace mray{
namespace gameMod{

struct FogAttributes
{
public:
	FogAttributes(){
		start=0;
		end=0;
	}

	float start;
	float end;
	video::SColor clr;
};

class EnvironmentManager:public ISingleton<EnvironmentManager>
{
private:
protected:

	float m_timeFromStart;
	float m_time;
	float m_timeFactor;
	float m_dayLong;

	GCPtr<Sun> m_sun;
	GCPtr<scene::SkyBoxManager> m_skyboxManager;
	GCPtr<scene::SSkyBoxNode> m_skyBox;
	GCPtr<video::ITexture> m_skyTex;
	GCPtr<scene::ISceneNode> m_waterSurface;

	FogAttributes m_fog;
	bool m_enableFog;

	float m_viewDistance;

	video::SColor m_ambientColor;
public:
	EnvironmentManager();
	virtual~EnvironmentManager();

	void attachLightToSun(GCPtr<scene::SLightNode> light);
	void setSunColor(const video::SColor &clr);
	const video::SColor& getSunColor();

	void setAmbientColor(const video::SColor &clr);
	const video::SColor& getAmbientColor();

	void enableFog(bool e);
	bool isFogEnabled();
	void setFogAttributes(float start,float end,const video::SColor&clr);

	void setViewDistance(float dist);
	float getViewDistance();

	//day time
	void setDayLong(float t);
	float getDayLong();

	void setTime(float t);
	float getTime();

	void setTimeFactor(float f);
	float getTimeFactor();

	void setWaterSurface(GCPtr<scene::ISceneNode>w);

	void loadSkyBox(const core::string&path,const core::string&alias);
	GCPtr<scene::ISkyShape> getSky();
	GCPtr<video::ITexture> getSkyTexture();

	virtual void update(float dt);
};

}
}


#endif //___EnvironmentManager___
