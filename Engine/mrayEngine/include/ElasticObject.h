
/********************************************************************
	created:	2008/10/24
	created:	24:10:2008   21:10
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\mrayCEGUI\ElasticObject.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\mrayCEGUI
	file base:	ElasticObject
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef ___ElasticObject___
#define ___ElasticObject___

#include "IPhysicObject.h"
#include "SpringSystem.h"
#include <mArray.h>

namespace mray{
namespace physics{

class MRAY_DLL ElasticObject:public SpringSystem
{
protected:
	math::vector3d m_center;
	struct SEVector{
		math::vector3d vec;
		float length;
		float lengthSQ;
	};
	std::vector<SEVector> m_Lengths;
	float m_stiffness;

public:
	ElasticObject(IPhysicObject*mesh);
	virtual~ElasticObject();

	virtual void setPhysicObject(IPhysicObject*mesh);

	virtual void affect(IPhysicObject*o,float dt);
};

}
}

#endif //___ElasticObject___

