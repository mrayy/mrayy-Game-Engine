

#ifndef ___ICullable___
#define ___ICullable___


#include "IMovable.h"
#include "TypedProperty.h"

namespace mray{
namespace scene{

enum ENodeCullingType{
	//! has no culling
	SCT_NONE,
	//! bounding box
	SCT_BBOX,
	//! Bounding Sphere
	SCT_BSPHERE
};

class MRAY_DLL ICullable:public IMovable
{
protected:
	ENodeCullingType m_cullingType;
	float m_viewDist;
	DECLARE_RTTI
public:
	DECLARE_PROPERTY_TYPE(ViewDist,float,MRAY_DLL);

	//DECLARE_RTTI

	ICullable();


	bool setCullingType(ENodeCullingType type){m_cullingType=type;return true;}
	ENodeCullingType getCullingType()const{return m_cullingType;}

	virtual~ICullable(){}


	bool setViewDistance(const float& val){m_viewDist=val;return true;}
	float getViewDistance()const{return m_viewDist;}

	virtual void OnVisit(ISceneVisitor*visitor);
};

MakeSharedPtrType(ICullable);

typedef std::list<ICullablePtr> CullableList;

}
}



#endif




