

/********************************************************************
	created:	2010/08/18
	created:	18:8:2010   21:58
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\Waypoint.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	Waypoint
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef Waypoint_h__
#define Waypoint_h__

#include "compileConfig.h"
#include <quaternion.h>
#include <mstring.h>

namespace mray
{
namespace AI
{

	class WaypointPath;
class MRAY_AI_DLL Waypoint
{
private:
protected:

	core::string m_name;
	uint m_id;

	math::vector3d m_pos;
	math::quaternion m_orintation;

	WaypointPath* m_owner;

public:
	Waypoint();
	virtual~Waypoint();

	void SetOwner(WaypointPath*o);
	WaypointPath* GetOwner();

	const core::string& GetName()const;
	void SetName(const core::string&name);

	uint GetID();
	void SetID(const uint&id);

	const math::vector3d& GetPosition()const;
	const math::quaternion& GetOrintation()const;

	void SetPosition(const math::vector3d& v);
	void SetOrintation(const math::quaternion& v);

};

}
}


#endif // Waypoint_h__
