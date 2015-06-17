



/********************************************************************
	created:	2014/05/21
	created:	21:5:2014   19:26
	filename: 	F:\Development\mrayEngine\Projects\TelubeeLib\NodeHeadController.h
	file path:	F:\Development\mrayEngine\Projects\TelubeeLib
	file base:	NodeHeadController
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __NodeHeadController__
#define __NodeHeadController__




#include "IHeadController.h"


namespace mray
{
	namespace scene
	{
		class IMovable;
	}

namespace TBee
{
class NodeHeadController :public IHeadController
{
protected:
	scene::IMovable* m_node;
	math::quaternion m_initialOrientation;
public:
	NodeHeadController();
	virtual~NodeHeadController();

	void SetNode(scene::IMovable* n){ m_node = n; }
	scene::IMovable* GetNode(){ return m_node ; }

	void SetInitialOrientation(math::quaternion &init){ m_initialOrientation = init; }
	virtual void Recalibrate();

	virtual bool GetHeadOrientation(math::quaternion& v, bool abs);
	virtual bool GetHeadPosition(math::vector3d& v, bool abs);
};

}
}


#endif
