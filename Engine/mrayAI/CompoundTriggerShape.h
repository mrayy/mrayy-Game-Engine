
/********************************************************************
	created:	2010/03/17
	created:	17:3:2010   17:06
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\CompoundTriggerShape.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	CompoundTriggerShape
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___CompoundTriggerShape___
#define ___CompoundTriggerShape___


#include "ITriggerShape.h"


namespace mray{
namespace AI{

class CompoundTriggerShape:public ITriggerShape
{
private:
protected:
	typedef std::list<ITriggerShape*> ShapesList;
	ShapesList m_shapes;
	math::box3d m_bbox;
public:
	CompoundTriggerShape(){}
	virtual~CompoundTriggerShape(){
		m_shapes.clear();
	}

	void AddShape(ITriggerShape*shape)
	{
		if(m_shapes.getSize()==0)m_bbox=shape->GetAABB();
		else m_bbox.AddBox(shape->GetAABB());

		m_shapes.push_back(shape);
	}

	virtual const math::box3d& GetAABB(){return m_bbox;}
	virtual bool IsActorInside(AIActor*actor){
		ShapesList::iterator it=m_shapes.begin();
		for(;it!=m_shapes.end();++it){
			if((*it)->IsActorInside(actor))
				return true;
		}
		return false;
	}
};

}
}


#endif //___CompoundTriggerShape___
