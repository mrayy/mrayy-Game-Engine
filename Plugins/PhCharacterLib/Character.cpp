#include "stdafx.h"
#include "Character.h"
#include "Controlling\TypeResolver.h"
#include ".\PhysicalModel\XMLPhysicalModelParser.h"
#include ".\PhysicalModel\Segment.h"
#include ".\Controlling\ControllingParams.h"
#include ".\Controlling\Controller.h"
#include ".\Environment.h"
#include "CharacterGait.h"
#include "IAnimationMixer.h"
#include "IMovable.h"
#include "CharacterTracking.h"
#include "BoneNode.h"


namespace mray
{
namespace PhysicsBasedCharacter
{
	IMPLEMENT_RTTI(Character,IObject)


Character::Character(CharacterManager* creator):m_tracking(0),m_scale(1),m_model(0),m_duplicat(0),
m_skeleton(0),m_gait(0),m_rootSegment(0),m_creator(creator),m_userData(0)
{

}
bool Character::Create(const core::string& descriptionFile, scene::Skeleton* skeleton,float scale, const math::vector3d &pos, physics::IPhysicManager*phMngr,IObject* data)
{
	m_userData=data;
	m_skeleton=skeleton;
	m_scale=scale;
	xml::XMLTree xmlTree;
	GCPtr<OS::IStream> stream=gFileSystem.openFile(descriptionFile,OS::TXT_READ);
	if(!xmlTree.load(stream)){
		gLogManager.log(mT("Couldn't parse XML Font File :\"")+descriptionFile+mT("\""),ELL_WARNING);
		stream->close();
		return false;
	}

	stream->close();
	xml::XMLElement*e=xmlTree.getSubElement(mT("Character"));

	if(!e)
	{
		gLogManager.log(mT("cann't find Character tag :\"")+descriptionFile+mT("\""),ELL_WARNING);
		return false;
	}

	xml::xmlAttributesMapIT it= e->getAttributesBegin();
	xml::xmlAttributesMapIT end= e->getAttributesEnd();

	core::stringc attrName;
	core::stringc elemName;
	xml::XMLAttribute* att;
/*	math::vector3d pos = core::StringConverter::toVector3d(e->getAttribute(mT("pos"))->value);

	pos.x = 0.01 * pos.x;
	pos.y = 0.01 * pos.y;
	pos.z = 0.01 * pos.z;*/

	float direction = core::StringConverter::toFloat(e->getAttribute(mT("direction"))->value);
	att = e->getAttribute(mT("skeleton"));
	m_model = 0;


	XMLPhysicalModelParser physicalModelParser ;
	for(;it!=end;++it)
	{
		xml::XMLAttribute&attr= it->second;
		core::string_to_char(attr.name,attrName);
		attrName.make_lower();		
		if(attrName==("physical_model"))
		{
			SegmentDesc segmentDesc;
			if(attr.value!=mT(""))
			{
				physicalModelParser.LoadPhysicalModelFile(attr.value, segmentDesc);
			}
			att = e->getAttribute(mT("collision_model"));
			if (att!=0)
			{
				physicalModelParser.LoadCollisionModelFile(att->value, segmentDesc);
			}
			SegmentDesc::BuildFromSkeleton(segmentDesc,skeleton);

			if (m_skeleton!=0)
			{
				m_duplicat = m_skeleton->getDuplicate();
				core::IteratorPair<animation::IAnimationMixer::ATGroupList> it = m_skeleton->getAnimationMixer()->getTrackGroups();

				for(;!it.done();it++)
				{
					m_duplicat->getAnimationMixer()->addTrackGroup((animation::TransAnimationGroup*)(*it).second);
				}

				it = m_duplicat->getAnimationMixer()->getTrackGroups();

				for(;!it.done();it++)
				{
					m_skeleton->getAnimationMixer()->removeTrackGroup((*it).first);
				}
			}
			else
				m_duplicat = 0;

			m_rootSegment = new Segment(this,0);
			segmentDesc.trans = pos;

			math::matrix4x4 refMat;
			math::vector3d xVec,yVec,zVec;

			zVec.set(0,-1,0);
			zVec.Normalize();

			xVec.x = - cos(math::toRad(direction)) ;
			xVec.y = 0;
			xVec.z = - sin(math::toRad(direction)) ;
			xVec.Normalize();

			yVec = zVec.crossProduct(xVec);

			refMat.setComponent(xVec, yVec, zVec);

			math::matrix4x4 refTrans;
			refMat.getTransposeMatrix(refTrans);
			math::quaternion refQ;
			refQ.fromMatrix(refTrans);

//			segmentDesc.rot = Segment::quaternionToVector3D(refQ);
			segmentDesc.rot=math::vector3d(0,direction,0);
			segmentDesc.jointDesc.swing1Limit.low = segmentDesc.rot.y;
			segmentDesc.jointDesc.swing1Limit.high = segmentDesc.rot.y;
			segmentDesc.jointDesc.swing2Limit.low = segmentDesc.rot.x;
			segmentDesc.jointDesc.swing2Limit.high = segmentDesc.rot.x;
			segmentDesc.jointDesc.twistLimit.low.value = segmentDesc.rot.z;
			segmentDesc.jointDesc.twistLimit.high.value = segmentDesc.rot.z;
			m_rootSegment->loadFromDesc(&segmentDesc,phMngr, skeleton,m_scale);					
			SegmentList segmentsList ;
			
			m_rootSegment->getAllChildren(segmentsList);
			for (SegmentList::iterator it = segmentsList.begin(); it != segmentsList.end(); ++it)
			{
				m_segments[(*it)->getName()] = *it;
			}
		}
		else if(attrName==("slots_num"))
		{
			m_numOfSlots = core::StringConverter::toInt(attr.value);
		}
	}

	m_controllingSlots .resize(m_numOfSlots,0);
	m_waitingSlots .resize(m_numOfSlots,0);
	xml::xmlSubElementsMapIT eit=e->getElementsBegin();
	xml::xmlSubElementsMapIT eEnd=e->getElementsEnd();
	for(;eit!=eEnd;++eit)
	{
		if((*eit)->GetType()!=xml::ENT_Element)
			continue;
		xml::XMLElement* e2=dynamic_cast<xml::XMLElement*>(*eit);
		core::string_to_char(e2->getName(),elemName);
		elemName.make_lower();

		if(elemName==("controller"))
		{
			core::string type;
			core::string path;

			xml::xmlAttributesMapIT it2= e2->getAttributesBegin();
			xml::xmlAttributesMapIT end2= e2->getAttributesEnd();

			core::stringc attrName2;
			core::stringc elemName2;


			for(;it2!=end2;it2++)
			{
				xml::XMLAttribute&attr2= it2->second;
				core::string_to_char(attr2.name,attrName2);
				attrName2.make_lower();		
				if(attrName2==("type"))
				{
					type = attr2.value;
				}
				else if(attrName2==("path"))
				{
					path = attr2.value;
				}
			}

			Controller* controller = TypeResolver::resolveController(type);
			controller->load(path, this);
			m_controllers.insert(std::make_pair(controller->getName(), controller));
		}
		else if (elemName==("gait"))
		{
			m_gait = new CharacterGait(e2, this);
		}
	}


	isActive = false;
	if (m_tracking!=0)
	{
		m_tracking->startTracking();
	}
	return true;
}
Character::~Character()
{
	delete m_gait ;
	//delete m_rootSegment;
	delete m_tracking;
	delete m_duplicat;
	m_gait = 0;

	std::map<core::string, Segment*>::iterator it=m_segments.begin();
	for (;it!=m_segments.end();++it)
	{
		delete it->second;
	}
	m_segments.clear();

	for(int i=0;i<m_controllingSlots.size();++i)
	{
		delete m_controllingSlots[i];
	}
	for(int i=0;i<m_waitingSlots.size();++i)
	{
		delete m_waitingSlots[i];
	}
	m_segments.clear();
	m_controllers.clear();
}

void Character::playController(const core::string& controllerName, int slotNum)
{
	Controller* controller = m_controllers.find(controllerName)->second;
	controller->reset();
	if (m_controllingSlots[slotNum]==0)
	{
		m_controllingSlots[slotNum] = controller;
	}
	else
	{
		m_waitingSlots[slotNum] = controller;
		m_controllingSlots[slotNum]->stop();
	}
}

void Character::forcePlayController(const core::string& controllerName, int slotNum)
{
	Controller* controller = m_controllers.find(controllerName)->second;
	controller->reset();
	m_controllingSlots[slotNum] = controller;
	m_waitingSlots[slotNum] = 0;
}

core::string Character::getControllerName(int slotNum)
{
	if (m_controllingSlots[slotNum])
	{
		return m_controllingSlots[slotNum]->getName();
	}
	else
		return mT("");
}

void Character::stopController(int slotNum)
{
	m_controllingSlots[slotNum]->stop();
}

void Character::forceStopController(int slotNum)
{
	m_controllingSlots[slotNum] = 0;
	m_waitingSlots[slotNum] = 0;
}

void Character::swapController(const core::string& controllerName, int slotNum)
{
	Controller* controller = m_controllers.find(controllerName)->second;
	controller->reset();
	if (m_controllingSlots[slotNum]==0)
	{
		m_controllingSlots[slotNum] = controller;
	}
	else
	{
		m_controllingSlots[slotNum]->reset();
		m_waitingSlots[slotNum] = m_controllingSlots[slotNum];
		m_controllingSlots[slotNum] = controller;
	}	
}


void Character::getControllingNames(std::list<core::string>&names)
{
	std::map<core::string,Controller*>::iterator it = m_controllers.begin();
	for (;it!=m_controllers.end();++it)
	{
		names.push_back( it->first);
	}
}
void Character::setParams(const core::string& controllerName, ControllingParams* params)
{
	Controller* controller = m_controllers.find(controllerName)->second;
	controller->setControllingParams(params);
}

Segment* Character::getSegment(const core::string& name)
{
	std::map<core::string, Segment*>::iterator it= m_segments.find(name);
	if(it!=m_segments.end())
		return it->second;
	return 0;
}


void Character::applyForce(const core::string& segmentName, math::vector3d& force, float dt)
{
	getSegment(segmentName)->applyGlobalForce(force);
}

Segment* Character::getRoot()
{
	return m_rootSegment;
}

CharacterGait* Character::getGait()
{
	return m_gait;
}

void Character::activate()
{
	//if (!isActive)
	{			
		isActive = true;
		for (std::map<core::string, Segment*>::iterator it = m_segments.begin(); it!=m_segments.end();++it)
		{
			//it->second->getActor()->raiseBodyFlag(NxBodyFlag::NX_BF_DISABLE_GRAVITY);
			it->second->getActor()->SetBodyFlag(physics::EBF_Kinematic,false);				
		}
	}

}

void Character::deactivate()
{
	isActive = false;
	for (std::map<core::string, Segment*>::iterator it = m_segments.begin(); it!=m_segments.end();++it)
	{
		//it->second->getActor()->raiseBodyFlag(NxBodyFlag::NX_BF_DISABLE_GRAVITY);
		it->second->getActor()->SetBodyFlag(physics::EBF_Kinematic,true);				
	}
}
void Character::update(float dt)
{
	//MessageBox(0,"Finished","",0);

	m_rootSegment->update(dt);
	if (m_model!=0)
	{
		math::vector3d pos=m_rootSegment->getGlobalTranslation()-m_rootSegment->GetBone()->getAbsolutePosition()*m_scale;
		m_model->setPosition(pos);
		//m_model->setOrintation(m_rootSegment->getGlobalOrientation() * skeletonToCharacter);
	}

	if (m_duplicat!=0)
	{
		m_duplicat->update(dt);
	}
	if (isActive)
	{
		if (m_gait!=0)
		{
			m_gait->update(dt);
		}
		if (m_tracking!=0)
		{
			m_tracking->update(dt);
		}
		for (int i=0;i<m_numOfSlots;i++)
		{
			if (m_controllingSlots[i]!=0 && m_controllingSlots[i]->isFinished())
			{
				m_controllingSlots[i] = 0;					
			}

			if (m_controllingSlots[i] == 0)
			{
				m_controllingSlots[i] = m_waitingSlots[i];
				m_waitingSlots[i] = 0;
			}

			if (m_controllingSlots[i] != 0)
			{
				m_controllingSlots[i]->update(dt);
			}
		}
	}

}
void Character::Visualize(scene::IDebugDrawManager* renderer)
{
	m_rootSegment->Visualize(renderer);
}

void Character::OnContact(physics::IPhysicalNode* node)
{
	FIRE_LISTENR_METHOD(OnCharacterContact,(this,node));
}

}
}