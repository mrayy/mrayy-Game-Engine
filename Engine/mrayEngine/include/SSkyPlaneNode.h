

#ifndef ___SSkyPlaneNode___
#define ___SSkyPlaneNode___

#include "SMesh.h"
#include "ISkyShape.h"
#include "ITexture.h"

namespace mray{
namespace scene{

class SceneManager;

class MRAY_DLL SSkyPlaneNode:public ISkyShape
{	
	int m_uSegments;
	int m_vSegments;
	double m_texPercentage;
	float m_hight;
	float m_size;
	math::vector2d m_speed;

	void setup(const video::ITexturePtr&  tex);

	void fillProperties();
public:


/*	rwProperty<SSkyPlaneNode,int> USegments;
	rwProperty<SSkyPlaneNode,int> VSegments;
	rwProperty<SSkyPlaneNode,double> TexPercentage;
	rwProperty<SSkyPlaneNode,float> Hight;
	rwProperty<SSkyPlaneNode,float> Size;
	rwProperty<SSkyPlaneNode,math::vector2d> Speed;*/

	SSkyPlaneNode(const video::ITexturePtr&  texture,int uSegments,int vSegments,
		double texPercentage,float hight,float size);
	virtual ~SSkyPlaneNode();


	const int& getUSegments() { return m_uSegments; }
	void setUSegments(const int &val) { m_uSegments = val; }

	const int& getVSegments() { return m_vSegments; }
	void setVSegments(const int &val) { m_vSegments = val; }

	const double& getTexPercentage() { return m_texPercentage; }
	void setTexPercentage(const double &val) { m_texPercentage = val; }

	const float& getHight() { return m_hight; }
	void setHight(const float &val) { m_hight = val; }

	const float& getSize() { return m_size; }
	void setSize(const float &val) { m_size = val; }

	const math::vector2d& getSpeed() { return m_speed; }
	void setSpeed(const math::vector2d &val) { m_speed = val; }


	virtual void update(float dt);
/*
	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
*/
};

}
}

#endif




