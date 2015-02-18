
/********************************************************************
	created:	2010/04/04
	created:	4:4:2010   20:16
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestAgent\InterstingPointManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestAgent
	file base:	InterstingPointManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___InterstingPointManager___
#define ___InterstingPointManager___

#include <ISingleton.h>

namespace mray{
namespace AI{

class InterstingPointManager:public ISingleton<InterstingPointManager>
{
private:
protected:
	math::vector3d m_interstingPoint;
	bool m_newPoint;
public:
	InterstingPointManager(){
		m_newPoint=false;
	}
	virtual~InterstingPointManager(){}

	void SetInterstingPoint(const math::vector3d &p){
		m_interstingPoint=p;
		m_newPoint=true;
	}
	const math::vector3d& GetInterstingPoint(){
		return m_interstingPoint;
	}

	bool NewPointPresented(){
		return m_newPoint;
	}
	void ClearPoint(){
		m_newPoint=false;
	}

};

}
}



#endif //___InterstingPointManager___
