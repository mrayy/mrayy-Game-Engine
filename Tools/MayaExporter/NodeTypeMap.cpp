

#include "stdafx.h"
#include "NodeTypeMap.h"
#include "IComponentData.h"



namespace mray
{
namespace exporter
{

std::map<core::string,core::string> NodeTypeMap::s_map;

NodeTypeMap::NodeTypeMap()
{
	s_map["Physical"]="PhysicalComponent";
	s_map["Scene"]="SceneComponent";
	s_map["Collision"]="Shape";

	s_map["ModelComponent"]="CoupledJointComponent";

	s_map["Constraint"]="PhysicalJointDOF6Component";
}

NodeTypeMap NodeTypeMap::s_instance;


const core::string& NodeTypeMap::GetMappedType(const core::string& t)
{
	if(s_map.find(t)==s_map.end())
		return core::string::Empty;
	return s_map[t];
}
IComponentData* NodeTypeMap::CreateComponent(const core::string& t,MayaNode* o)
{
	if(t=="Scene")
		return new SceneComponentData(o);
	if(t=="Physical")
		return new PhysicalComponentData(o);
	return 0;
}

}
}

