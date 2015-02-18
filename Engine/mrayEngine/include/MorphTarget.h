
/********************************************************************
	created:	2009/03/16
	created:	16:3:2009   0:14
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\MorphTarget.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	MorphTarget
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___MorphTarget___
#define ___MorphTarget___

#include "GCptr.h"

namespace mray{
namespace scene{
	class IMeshBuffer;

class MorphTarget 
{
	float m_percent;
	scene::IMeshBuffer*m_target;
public:
	MorphTarget(){
		m_percent=0;
		m_target=0;
	}
	MorphTarget(scene::IMeshBuffer*buff,float p){
		m_percent=p;
		m_target=buff;
	}
	virtual~MorphTarget(){
	}
	float getPercent(){return m_percent;}
	void setPercent(float p){
		m_percent=p;
	}

	IMeshBuffer* getMesh(){return m_target;}
	void setMesh(IMeshBuffer*m){m_target=m;}
};

MakeSharedPtrType(MorphTarget)

}

}


#endif //___MorphTarget___
