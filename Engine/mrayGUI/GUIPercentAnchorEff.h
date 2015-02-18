
/********************************************************************
	created:	2009/04/12
	created:	12:4:2009   14:05
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\GUIPercentAnchorEff.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	GUIPercentAnchorEff
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GUIPercentAnchorEff___
#define ___GUIPercentAnchorEff___

#include "GUIAnchorEffector.h"

namespace mray{
namespace GUI{

class MRAY_DLL GUIPercentAnchorEff:public GUIAnchorEffector
{
private:
protected:
	math::vector2d m_posPercent;
	virtual void fillProperties();
public:
	DECLARE_RTTI;

	rwProperty<GUIPercentAnchorEff,math::vector2d> PosPercent;

	GUIPercentAnchorEff(EPosAnchor anch,const math::vector2d &posPercent);
	virtual~GUIPercentAnchorEff();

	virtual void effect(GUIElement* elem,float dt);

	void setPosPercent(const math::vector2d&v){m_posPercent=v;}
	const math::vector2d&getPosPercent(){return m_posPercent;}
};

}
}


#endif //___GUIPercentAnchorEff___
