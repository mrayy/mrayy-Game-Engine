
/********************************************************************
	created:	2009/03/29
	created:	29:3:2009   18:42
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\Sun.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject
	file base:	Sun
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___Sun___
#define ___Sun___

#include "CompileConfig.h"
#include <SColor.h>
#include <Point3d.h>
#include <LightNode.h>
#include "IObject.h"

namespace mray
{
namespace game
{
	class GameLevel;

class MRAY_GAME_DLL Sun:public IObject
{
	DECLARE_RTTI()
private:
protected:

	GameLevel* m_level;

	video::SColor m_color;
	GCPtr<scene::LightNode> m_light;

	scene::IMovable* m_attachedNode;

	math::vector3d m_dir;

	bool m_enableShadow;
	float m_shadowProjFov;
	float m_shadowProjAspect;
	float m_shadowProjNear;
	float m_shadowProjFar;

	math::vector2di m_shadowMapSize;

	float m_vertical;
	float m_horizontal;

	void _RecalcPosition();
public:
	Sun(GameLevel*lvl);
	virtual~Sun();

	void SetHorizontal(const float& v);
	void SetVertical(const float& v);

	void SetShadowMapSize(const math::vector2di& sz);
	void EnableShadow(const bool &e);
	void SetShadowProjectionParams(float fov,float aspect,float znear,float zfar);

	void SetShadowProjFov(const float& v);
	void SetShadowProjAspect(const float& v);
	void SetShadowProjNear(const float& v);
	void SetShadowProjFar(const float& v);


	const float& GetVertical();
	const float& GetHorizontal();

	const bool& _isShadowEnable();

	const float& GetShadowProjFov();
	const float& GetShadowProjAspect();
	const float& GetShadowProjNear();
	const float& GetShadowProjFar();

	const math::vector2di& GetShadowMapSize();


	const math::vector3d& GetDirection();

	void SetColor(const video::SColor &clr);
	const video::SColor& GetColor();

	scene::LightNode* GetLight();

	void AttachNode(scene::IMovable*node);
	scene::IMovable* GetAttachedNode();

	void Update(float dt);

	virtual xml::XMLElement*  loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};

}
}


#endif //___Sun___
