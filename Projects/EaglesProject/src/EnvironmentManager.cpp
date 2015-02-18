
#include "EnvironmentManager.h"
#include "IEaglesManager.h"
#include "userDefinedShaders.h"

#include <VideoValues.h>

namespace mray{
namespace gameMod{

EnvironmentManager::EnvironmentManager(){
	m_enableFog=false;
	m_timeFromStart=0;
	m_time=0;
	m_timeFactor=1;
	m_dayLong=24;

	m_viewDistance=1000;

	m_sun=new Sun();

	m_skyboxManager=new scene::SkyBoxManager();

	m_skyBox=new scene::SSkyBoxNode(0,0,0);

	video::GPUUniform::registerUserSemantic("SunPos",shader_SunPos,true);
}

EnvironmentManager::~EnvironmentManager(){
	m_skyboxManager=0;
}


void EnvironmentManager::attachLightToSun(GCPtr<scene::SLightNode> light){
	m_sun->attachLight(light);
}
void EnvironmentManager::setSunColor(const video::SColor &clr){
	return m_sun->setColor(clr);
}

const video::SColor& EnvironmentManager::getSunColor(){
	return m_sun->getColor();
}


void EnvironmentManager::setAmbientColor(const video::SColor &clr){
	m_ambientColor=clr;
}

const video::SColor& EnvironmentManager::getAmbientColor(){
	return m_ambientColor;
}


void EnvironmentManager::setViewDistance(float dist){
	m_viewDistance=dist;
}
float EnvironmentManager::getViewDistance(){
	return m_viewDistance;
}

void EnvironmentManager::setDayLong(float t){
	m_dayLong=t;
}

float EnvironmentManager::getDayLong(){
	return m_dayLong;
}


void EnvironmentManager::setTime(float t){
	m_time=t;
}

float EnvironmentManager::getTime(){
	return m_time;
}


void EnvironmentManager::setTimeFactor(float f){
	m_timeFactor=f;
}

float EnvironmentManager::getTimeFactor(){
	return m_timeFactor;
}


void EnvironmentManager::enableFog(bool e){
	m_enableFog=e;
	eagles::IEaglesManager::getInstance().getVideoDevice()->enableFog(e);
}
bool EnvironmentManager::isFogEnabled(){
	return m_enableFog;
}
void EnvironmentManager::setFogAttributes(float start,float end,const video::SColor&clr){
	m_fog.start=start;
	m_fog.end=end;
	m_fog.clr=clr;
}

void EnvironmentManager::loadSkyBox(const core::string&path,const core::string&alias){
	m_skyTex=m_skyboxManager->loadSkyFromFolder(path,mT("png"),alias);

	m_skyBox->setTexture(m_skyTex);
	if(m_waterSurface){
		GCPtr<video::SMaterial>mtrl= m_waterSurface->getMaterial();
		if(mtrl){
			IValue*v=mtrl->getValue(mT("envTex"));
			if(v){
				TextureValue*t=dynamic_cast<TextureValue*>(v);
				t->value=m_skyTex;
			}
		}
	}
}
GCPtr<video::ITexture> EnvironmentManager::getSkyTexture(){
	return m_skyTex;
}
GCPtr<scene::ISkyShape> EnvironmentManager::getSky(){
	return m_skyBox;
}
void EnvironmentManager::setWaterSurface(GCPtr<scene::ISceneNode>w){
	m_waterSurface=w;
	if(m_waterSurface){
		GCPtr<video::SMaterial>mtrl= m_waterSurface->getMaterial();
		if(mtrl){
			IValue*v=mtrl->getValue(mT("envTex"));
			if(v){
				TextureValue*t=dynamic_cast<TextureValue*>(v);
				t->value=m_skyTex;
			}
		}
	}
}

void EnvironmentManager::update(float dt){

	GCPtr<video::IVideoDevice>dev=eagles::IEaglesManager::getInstance().getVideoDevice();

	dev->setFog(m_fog.clr,1,m_fog.start,m_fog.end,0.01,0);
	dev->setAmbientColor(m_ambientColor);
}



}
}