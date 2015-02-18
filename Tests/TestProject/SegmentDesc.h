

/********************************************************************
	created:	2009/10/16
	created:	16:10:2009   15:47
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject\SegmentDesc.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject
	file base:	SegmentDesc
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___SegmentDesc___
#define ___SegmentDesc___

#include "DOFLimit.h"
#include "SegShapeDesc.h"
#include "SegInertiaDesc.h"
namespace mray{

namespace xml
{
	class XMLElement;
}




class SegmentDesc
{
private:
protected:
	void parseChildren(xml::XMLElement*elem);
public:
	SegmentDesc();
	virtual~SegmentDesc();

	void Serialize(xml::XMLElement*elem);
	void Clear();

public:
	typedef std::list<SegmentDesc*> SegmentList;

	

	core::string	name;
	//Holds Model Name..
	core::string	model;

	math::vector3d	trans;
	math::vector3d	rot;

	SegInertiaDesc	inertiaDesc;
	DOFLimit		jointDesc;
	SegShapeDesc	shapeDesc;
	

	SegmentList	children;

};

}


#endif //___SegmentDesc___
