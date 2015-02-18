/********************************************************************
	created:	2013/10/15
	created:	15:10:2013   17:16
	filename: 	C:\Development\mrayEngine\Engine\mrayGameLayer\SkyBoxComponent.h
	file path:	C:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	SkyBoxComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __SkyBoxComponent__
#define __SkyBoxComponent__

#include "IGameComponent.h"
#include "ITexture.h"


namespace mray
{
	namespace scene
	{
		class SSkyBoxNode;
		class ISceneNode;
		class ISceneManager;
	}
namespace game
{


class MRAY_GAME_DLL SkyBoxComponent:public IGameComponent
{
	DECLARE_RTTI;
protected:
	scene::ISceneManager* m_sceneMngr;
	scene::SSkyBoxNode* m_skyBoxNode;
	scene::ISceneNode* m_node;
	video::ITexturePtr m_skyTex;
	core::string m_skyboxPath;
public:
	DECLARE_PROPERTY_TYPE(SkyBoxPath,core::string,);
public:
	SkyBoxComponent(GameEntityManager*);
	virtual~SkyBoxComponent();

	virtual bool InitComponent();

	bool SetSkyBoxPath(const core::string& p);
	const core::string& GetSkyBoxPath();

	virtual bool IsUnique()const{return true;}

	scene::ISceneNode* GetSceneNode(){return m_node;}
	scene::SSkyBoxNode* GetSkyBox(){return m_skyBoxNode;}
	
	virtual xml::XMLElement*  loadXMLSettings(xml::XMLElement* elem);
};
DECLARE_GAMECOMPONENT_FACTORY(SkyBoxComponent);

}
}


#endif
