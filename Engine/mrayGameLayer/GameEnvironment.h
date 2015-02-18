


/********************************************************************
	created:	2010/06/21
	created:	21:6:2010   21:57
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGameLayer\GameEnvironment.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGameLayer
	file base:	GameEnvironment
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GameEnvironment_h__
#define GameEnvironment_h__

#include "CompileConfig.h"
#include <TimeTicket.h>
#include <ISingleton.h>
#include <IHeightMapLoader.h>

namespace mray
{
	namespace video
	{
		class ParsedShaderPP;
	}
	namespace scene
	{
		class SSkyBoxNode;
		class SPatchTerrain;
		class ISceneNode;
	}
	namespace physics
	{
		class IPhysicalNode;
	}
namespace game
{
	class Sun;
	class GameLevel;

class MRAY_GAME_DLL GameEnvironment
{
private:
protected:
	Sun* m_sun;
	scene::SSkyBoxNode* m_skyBox;
	scene::ISceneNode* m_skyBoxNode;
	core::string m_skyName;

	float m_waterLevel;
	scene::ISceneNode* m_waterSurface;

	TimeTicket m_time;
	double m_timeSpeed;

	GameLevel* m_level;

	scene::SPatchTerrain* m_terrain;
	scene::ISceneNode* m_terrainNode;
	physics::IPhysicalNode* m_terrainPHNode;

	video::ParsedShaderPP* m_postProcessingEffect;
	core::string m_postProcessingEffectName;

	void OnTerrainCreated();

public:
	GameEnvironment(GameLevel* level);
	virtual ~GameEnvironment();
	
	Sun* GetSun();

	void SetGameLevel(GameLevel*level);
	GameLevel* GetGameLevel();

	void SetSkybox(const core::string&name);
	scene::SSkyBoxNode* GetSkybox();
	scene::ISceneNode* GetSkyboxNode();

	void GenerateTerrain(GCPtr<loaders::IHeightMapLoader> hm,float lod,float lodDist,const math::vector3d&scale);
	void RemoveTerrain();
	scene::SPatchTerrain* GetTerrain();
	scene::ISceneNode* GetTerrainNode();
	physics::IPhysicalNode* GetTerrainPhysics();

	void SetWaterLevel(float l);
	float GetWaterLevel();

	void SetPostProcessingName(const core::string& path);
	const core::string& GetPostProcessingName();

	video::ParsedShaderPP* GetPostProcessingEffect();

	void SetWaterSurface(scene::ISceneNode* waterSurface);
	scene::ISceneNode* GetWaterSurface();

	void SetTime(const TimeTicket& t);
	const TimeTicket& GetTime();

	void SetTimeSpeed(double s);
	double GetTimeSpeed();

	void Update(float dt);

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};

}
}

#endif // GameEnvironment_h__
