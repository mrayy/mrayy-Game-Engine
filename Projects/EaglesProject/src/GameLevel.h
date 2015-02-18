
/********************************************************************
	created:	2009/04/05
	created:	5:4:2009   23:11
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\GameLevel.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	GameLevel
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GameLevel___
#define ___GameLevel___


#include "GameEntity.h"
#include "AirPlane.h"
#include "PlaneCamera.h"
#include "HumanController.h"
#include "CollisionManager.h"
#include "InGamePP.h"

#include <GUIManager.h>
#include <ISoundManager.h>
#include <SceneManager.h>
#include <GCPtr.h>
#include <ITerrain.h>
#include <IEventReciver.h>
#include <GUIWindow.h>
#include <GUILoadingBar.h>
#include <GUIImage.h>

namespace mray{
namespace gameMod{


class GameLevel:public IEventReciver
{
private:
protected:

	math::box3d m_boundingBox;

	GCPtr<CollisionManager> m_collisionManager;

	GCPtr<scene::ISceneNode> m_boundingBoxShape;

	GCPtr<scene::SCameraNode> m_freeCamera;
	GCPtr<PlaneCamera> m_planeCamera;

	GCPtr<GUI::GUIManager> m_guiMngr;
	GCPtr<GUI::GUIManager> m_oldGuiMngr;

	GCPtr<sound::ISoundManager> m_sndMngr;
	GCPtr<sound::ISoundManager> m_oldSndMngr;

	GCPtr<sound::ISound> m_windSound;

	GCPtr<HumanController> m_controller;

	GCPtr<scene::SceneManager> m_sceneMngr;
	GCPtr<scene::SceneManager> m_oldSceneMngr;

	GCPtr<GUI::GUIStaticText> m_planeHeightTxt;

	GCPtr<InGamePP> m_gamePP;

	core::string m_name;
	GCPtr<scene::ITerrain> m_terrain;

	GCPtr<AirPlane> m_player;

	GCPtr<GUI::GUIWindow> m_menuWnd;

	GCPtr<GUI::GUIStaticText> m_warningText;
	GCPtr<GUI::GUILoadingBar> m_warningBar;

	GCPtr<GUI::GUIImage> m_dangerImage;

	EntityList m_gameEntities;

	int m_state;

	float m_waterHeight;

	bool m_pause;
public:
	GameLevel();
	virtual~GameLevel();

	void setName(const core::string&name);
	const core::string& getName();

	GCPtr<GUI::GUIManager> getGUIManager(){return m_guiMngr;}
	GCPtr<scene::SceneManager> getSceneManager(){return m_sceneMngr;}
	GCPtr<sound::ISoundManager> getSoundManager(){return m_sndMngr;}

	void addSceneNode(GCPtr<scene::ISceneNode>node);

	void setLevelBoundingBox(const math::box3d&bb);

	void setPlayer(const core::string& type,const core::string& missile,const math::vector3d&pos,
		const math::vector3d&rot,const math::vector3d&scale);

	void addAntiPlane(const core::string& type,const core::string& missile,const math::vector3d&pos,
		const math::vector3d&rot,const math::vector3d&scale);

	void setTerrain(GCPtr<scene::ITerrain> terr);
	GCPtr<scene::ITerrain> getTerrain();

	void addEntity(GCPtr<GameEntity> ent,bool toTerrain=true);
	void addEntity(const core::string& oclass,int type,const math::vector3d&pos,
		const math::vector3d&rot,const math::vector3d&scale);


	core::IteratorPair<EntityList> getEntities();

	void createWaterSurface(float height,int detail,const math::vector2d& size);

	void setPlayer(GCPtr<AirPlane> plane);
	GCPtr<AirPlane> getPlayer();

	void onEnter();
	void onExit();

	void toggleCamera();

	float getHeight(float x,float z);

	void setPlayerPlane(const math::vector3d&pos,const math::vector3d&rot,const core::string& type);

	void update(float dt);

	void onEvent(GCPtr<Event> e);

	int getState(){return m_state;}


	bool isPaused(){
		return m_pause;
	}

};

}
}


#endif //___GameLevel___
