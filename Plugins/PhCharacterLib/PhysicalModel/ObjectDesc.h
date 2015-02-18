/********************************************************************
	created:	2010/07/06
	created:	6:7:2010   11:38
	filename: 	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\PhysicalModel\ObjectDesc.h
	file path:	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\PhysicalModel
	file base:	ObjectDesc
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef ObjectDesc_h__
#define ObjectDesc_h__
namespace mray
{
	namespace xml
	{
		class XMLElement;
	}

	namespace PhysicsBasedCharacter
	{
		class ObjectDesc
		{
			public:
				ObjectDesc();
				~ObjectDesc();
				void Serialize(xml::XMLElement*elem);
				core::string type;
				math::vector3d translation;
				math::vector3d rotation;
				float radius;
				float height;
				float width;
				float depth;
		};
	}
}
#endif // ObjectDesc_h__