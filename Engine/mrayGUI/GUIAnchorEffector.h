

#ifndef ___GUIAnchorEffector___
#define ___GUIAnchorEffector___

#include "GUIEffector.h"
#include "Point2d.h"

namespace mray{
namespace GUI{

enum EPosAnchor{
	EPAnchor_U,
	EPAnchor_B,
	EPAnchor_L,
	EPAnchor_R,
	EPAnchor_MidX,
	EPAnchor_MidY,
	EPAnchor_UL,
	EPAnchor_UR,
	EPAnchor_BL,
	EPAnchor_BR,
	EPAnchor_CENTER
};
class MRAY_DLL GUIAnchorEffector:public GUIEffector
{
public:
	DECLARE_RTTI;

	rwProperty<GUIAnchorEffector,EPosAnchor> Anchor;
	rwProperty<GUIAnchorEffector,math::vector2d> Offset;

	GUIAnchorEffector(EPosAnchor anch,const math::vector2d& offset=0);

	virtual void effect(GUIElement* elem,float dt);

	void setAnchor(const EPosAnchor&v){m_anchor=v;}
	const EPosAnchor& getAnchor(){return m_anchor;}

	void setOffset(const math::vector2d&v){m_offset=v;}
	const math::vector2d& getOffset(){return m_offset;}

protected:

	virtual void fillProperties();

	EPosAnchor m_anchor;
	math::vector2d m_offset;
};


}
}

#endif




