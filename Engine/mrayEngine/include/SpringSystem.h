
/********************************************************************
	created:	2008/10/24
	created:	24:10:2008   16:24
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\mrayCEGUI\SpringSystem.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\mrayCEGUI
	file base:	SpringSystem
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	simple spring mesh , simulates things like cloth
*********************************************************************/

#ifndef ___SpringSystem___
#define ___SpringSystem___

#include "IDeformable.h"
#include <mArray.h>

namespace mray{
namespace physics{

struct MRAY_DLL SpringLink{
	SpringLink(){
		damping=2;
	}
	SpringLink(int v1,int v2,float coeff,float minLength,float maxLength){
		vertConnection[0]=v1;
		vertConnection[1]=v2;
		springCoefficient=coeff;
		this->minLength=minLength;
		this->maxLength=maxLength;
		damping=0.9;
	}
	int vertConnection[2];	//vertices's id
	float springCoefficient;	
	float minLength,maxLength;			//spring length
	float damping;
};

class MRAY_DLL SpringSystem:public IDeformable
{
public:

protected:
	std::vector<SpringLink> m_springs;

	math::vector3d m_force;
	math::vector3d m_gravity;
	int m_iterations;

public:
	SpringSystem(IPhysicObject*mesh);
	virtual~SpringSystem();


	void setSpringsCount(int cnt);
	int getSpringsCount();

	void addSpring(int v1,int v2,float coeff,float minLength,float maxLength);
	void addSpring(SpringLink&spring);

	void removeSpring(int i);
	void clear();

	void setForce(const math::vector3d &f);
	void setGravity(const math::vector3d &f);

	math::vector3d getForce();
	math::vector3d getGravity();

	SpringLink*getSpring(int i);
	SpringLink*getSprings();

	virtual void affect(IPhysicObject*o,float dt);
};

}
}

#endif //___SpringSystem___

