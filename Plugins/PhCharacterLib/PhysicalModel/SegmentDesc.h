

/********************************************************************
	created:	2009/10/16
	created:	16:10:2009   15:47
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject\SegmentDesc.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject
	file base:	SegmentDesc
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef ___SegmentDesc___
#define ___SegmentDesc___

#include "DOFLimit.h"
#include "SegShapeDesc.h"
#include "SegInertiaDesc.h"

namespace mray
{
namespace scene
{
	class BoneNode;
}

namespace xml
{
	class XMLElement;
}




namespace PhysicsBasedCharacter
{
	class SegmentDesc;
	typedef std::map<core::string,SegmentDesc*> SegmentObjectMap;
	class SegmentDesc
	{
	public:
		typedef std::list<SegmentDesc*> SegmentList;
	private:
	protected:
		void parseChildren(xml::XMLElement*elem);
		void _getAllChildren(SegmentObjectMap& objects);
	public:

		SegmentDesc();
		virtual~SegmentDesc();
		SegmentObjectMap getAllChildren();
		void Serialize(xml::XMLElement*elem);
		void Clear();

		void BuildFromBone(scene::BoneNode* bone,scene::BoneNode* bone2);
		static void BuildFromSkeleton(SegmentDesc &desc,scene::Skeleton* s);
	
	public:
	
		bool autoBuildShapes;
		float radius;

		core::string	name;
		core::string	bone;

		math::vector3d	trans;
		math::vector3d	rot;
		math::vector3d	down;
		math::vector3d	back;

		SegInertiaDesc	inertiaDesc;
		DOFLimit		jointDesc;
		SegShapeDesc	shapeDesc;


		SegmentList	children;
	
	};
}
}


#endif //___SegmentDesc___
