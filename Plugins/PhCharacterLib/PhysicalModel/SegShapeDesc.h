/********************************************************************
	created:	2009/11/06
	created:	6:11:2009   9:19
	filename: 	d:\Project\Character Animation\19-10\Project\SegShapeDesc.h
	file path:	d:\Project\Character Animation\19-10\Project
	file base:	SegShapeDesc
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/
#ifndef __SegShapeDesc__
#define __SegShapeDesc__

#include "ObjectDesc.h"

namespace mray{

	namespace xml
	{
		class XMLElement;
	}

namespace PhysicsBasedCharacter
{
	class SegShapeDesc
	{
	public:
		SegShapeDesc(void)
		{
			length = 0;
			radius = 0;
		}
	
		virtual ~SegShapeDesc(void);
	
		void Serialize(xml::XMLElement*elem);
	public:
		core::string	type;
		core::string	model;
		core::string	attach;
		float			length;
		float			radius;
		math::vector3d	extent;
		math::vector3d	trans;
		std::list<ObjectDesc*> objects;
	};
}
}
#endif // __SegShapeDesc__

