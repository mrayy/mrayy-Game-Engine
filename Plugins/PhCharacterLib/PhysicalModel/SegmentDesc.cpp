
#include "stdafx.h"
#include "SegmentDesc.h"
#include "BoneNode.h"


namespace mray{
namespace PhysicsBasedCharacter
{
	
	SegmentDesc::SegmentDesc():down(math::vector3d::ZAxis),back(math::vector3d::XAxis),autoBuildShapes(false),radius(1)
	{
	}
	SegmentDesc::~SegmentDesc()
	{
		Clear();
	}
	
	void SegmentDesc::Serialize(xml::XMLElement*elem)
	{
	
		if(!elem)
			return;
		xml::xmlAttributesMapIT it= elem->getAttributesBegin();
		xml::xmlAttributesMapIT end= elem->getAttributesEnd();
	
		core::stringc attrName;
		core::stringc elemName;
	
		for(;it!=end;++it){
			xml::XMLAttribute&attr= it->second;
			core::string_to_char(attr.name,attrName);
			attrName.make_lower();		
			if(attrName=="name")
			{
				name=attr.value;
			}
			else if(attrName=="bone")
			{
				bone=attr.value;
			}
			else if(attrName=="autobuild")
			{
				autoBuildShapes=core::StringConverter::toBool(attr.value);
			}
			else if(attrName=="radius")
			{
				radius=core::StringConverter::toFloat(attr.value);
			}
			else if(attrName=="trans")
			{
					trans = core::StringConverter::toVector3d(attr.value);
					trans.x = trans.x * 0.01;
					trans.y = trans.y * 0.01;
					trans.z = trans.z * 0.01;
			}
			else if(attrName=="rot")
			{
				rot=core::StringConverter::toVector3d(attr.value);
			}
			else if(attrName=="down")
			{
				down=core::StringConverter::toVector3d(attr.value);
			}			
			else if(attrName=="back")
			{
				back=core::StringConverter::toVector3d(attr.value);
			}
		}
	
		children.clear();
	
		xml::xmlSubElementsMapIT eit=elem->getElementsBegin();
		xml::xmlSubElementsMapIT eEnd=elem->getElementsEnd();
		for(;eit!=eEnd;++eit)
		{
			if((*eit)->GetType()!=xml::ENT_Element)
				continue;
			xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*eit);
			core::string_to_char(e->getName(),elemName);
			elemName.make_lower();
	
			if(elemName=="joint"){
				jointDesc.Serialize(e);
			}else if (elemName=="inertia")
			{
				inertiaDesc.Serialize(e);
			}else if (elemName=="shape")
			{
				shapeDesc.Serialize(e);
			}
			//parse children
			else if(elemName=="segment"){
				SegmentDesc *desc=new SegmentDesc();
				desc->Serialize(e);
				children.push_back(desc);
			} 
		}
	}
	
	
	void SegmentDesc::parseChildren(xml::XMLElement*elem){
		core::stringc elemName;
	
		xml::xmlSubElementsMapIT it=elem->getElementsBegin();
		xml::xmlSubElementsMapIT end=elem->getElementsEnd();
		for(;it!=end;++it){
			if((*it)->GetType()!=xml::ENT_Element)
				continue;
			xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*it);
			core::string_to_char(e->getName(),elemName);
			elemName.make_lower();
			if(elemName=="segment"){
				SegmentDesc* desc=new SegmentDesc();
				desc->Serialize(e);
				children.push_back(desc);
			}
		}
	}
	void SegmentDesc::Clear(){
		SegmentList::iterator it=children.begin();
		for(;it!=children.end();++it){
			delete *it;
		}
		children.clear();
	}

	scene::BoneNode* GetNextChild(scene::BoneNode* bone,const SegmentObjectMap &bones)
	{
		scene::BonesList bonesVec;
		scene::BoneNode *b=bone;
		scene::BoneNode *best=0;
		do
		{
			bonesVec=b->getSubBones();
			if(bonesVec.size()>1 || bonesVec.size()==0)
			{
				if(b==bone)
				{
					scene::BonesList::iterator it=bonesVec.begin();
					for(;it!=bonesVec.end();++it)
					{
						if(bones.find((*it)->getNodeName())!=bones.end())
							return *it;
					}
				}
				return 0;
			}
			b=*bonesVec.begin();
			if(bones.find(b->getNodeName())!=bones.end())
				return b;
		}while(true);
		return 0;
	}

	void SegmentDesc::BuildFromSkeleton(SegmentDesc &desc,scene::Skeleton* s)
	{

		SegmentObjectMap map = desc.getAllChildren();
		SegmentObjectMap::iterator it=map.begin();
		for(;it!=map.end();++it)
		{
			if(!it->second->autoBuildShapes)
				continue;
			scene::BoneNode* b= s->getBone(it->first);
			if(!b)continue;
			scene::BoneNode*bb= GetNextChild(b,map);
			it->second->BuildFromBone(b,bb);
		}
	}

	void SegmentDesc::BuildFromBone(scene::BoneNode* bone,scene::BoneNode* bone2)
	{

		ObjectDesc* desc=new ObjectDesc();
		desc->type="objects";
		desc->radius=radius;//desc->height*0.1;;
		if(bone2)
		{
			desc->type="cylinder";
			desc->height=bone->getAbsoluteBasePosition().getDist(bone2->getAbsoluteBasePosition());
			
			desc->height=desc->height-desc->radius*2;
			if(desc->height<=0)desc->height=0.001;
			desc->translation.x=desc->height/2;
			desc->rotation.z=90;
		}
		else
		{
			desc->type="sphere";
		//	desc->radius=0.05;
		}
		shapeDesc.objects.push_back(desc);
	}
	void SegmentDesc::_getAllChildren(SegmentObjectMap& objects)
	{
		if (bone!=mT(""))
		{
			objects[bone]=this;
		}
		for (SegmentList::iterator it=children.begin(); it!=children.end();it++)
		{				
			(*it)->_getAllChildren(objects);
		}

	}
	SegmentObjectMap SegmentDesc::getAllChildren()
	{
		SegmentObjectMap temp;
		_getAllChildren(temp);
		return temp;
	}

}

}