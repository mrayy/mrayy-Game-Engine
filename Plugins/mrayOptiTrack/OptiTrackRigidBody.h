


/********************************************************************
	created:	2012/06/28
	created:	28:6:2012   15:58
	filename: 	d:\Development\mrayEngine\Engine\mrayOptiTrack\OptiTrackRigidBody.h
	file path:	d:\Development\mrayEngine\Engine\mrayOptiTrack
	file base:	OptiTrackRigidBody
	file ext:	h
	author:		Written by Charith Fernando , modefied by Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __OptiTrackRigidBody__
#define __OptiTrackRigidBody__


namespace mray
{
namespace animation
{

class OptiTrackRigidBody
{
private:
protected:
	int m_id;
	std::vector<math::vector3d> m_posSamples;
	std::vector<math::quaternion> m_oriSamples;
	math::vector3d m_position;
	math::quaternion m_orintation;
	core::string m_name;
public:
	OptiTrackRigidBody():m_id(-1){}
	virtual~OptiTrackRigidBody(){}

	void SetName(const core::string& name){m_name=name;}
	const core::string& GetName()const{return m_name;}

	inline void SetID(int id){m_id=id;}
	inline int GetID()const{return m_id;}

	 void SetPosition(const math::vector3d& pos);
	inline const math::vector3d& GetPosition()const {return m_position;}

	 void SetOrintation(const math::quaternion& o);
	inline const math::quaternion& GetOrintation()const {return m_orintation;}

};


}
}

#endif
