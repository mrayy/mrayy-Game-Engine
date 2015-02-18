/********************************************************************
	created:	2009/11/06
	created:	6:11:2009   14:02
	filename: 	d:\Project\Character Animation\19-10\Project\SegInertiaDesc.h
	file path:	d:\Project\Character Animation\19-10\Project
	file base:	SegInertiaDesc
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef ___SegInertiaDesc___
#define ___SegInertiaDesc___

namespace mray{

	namespace xml
	{
		class XMLElement;
	}

namespace PhysicsBasedCharacter
{
	class SegInertiaDesc
	{
	public:
		SegInertiaDesc(void)
		{
			density = 0;
			mass = 0;
			shape = false;
		}
		virtual ~SegInertiaDesc(void){}
		void Serialize(xml::XMLElement*elem);
	public:
		float density;
		float mass;
		math::vector3d moments;
		math::vector3d trans;
		math::vector3d rot;	
		bool shape;
	};
}
}
#endif ___SegInertiaDesc___