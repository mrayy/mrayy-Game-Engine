


#include "stdafx.h"
#include "IComponentData.h"
#include "MayaNode.h"
#include "ExporterParams.h"



namespace mray
{
namespace exporter
{


void SceneComponentData::LoadData(const MDagPath& dpath,ExporterParams& params)
{
}
xml::XMLElement* SceneComponentData::Export(xml::XMLElement* parent,ExporterParams& params)
{
	xml::XMLElement* ret=new xml::XMLElement("SceneComponent");
	if(parent)
		parent->addSubElement(ret);

	ret->addAttribute("Name",m_owner->GetName().asChar());

	{
		xml::XMLElement* e=new xml::XMLElement("SceneNodeDesc");
		ret->addSubElement(e);
		e->addAttribute("Name",m_owner->GetName().asChar());
		e->addAttribute("Position",core::StringConverter::toString(m_owner->GetPosition()));
		e->addAttribute("Rotation",core::StringConverter::toString(m_owner->GetRotation()));
		e->addAttribute("SCale",core::StringConverter::toString(m_owner->GetScale()));
	}
	xml::XMLElement* comps=new xml::XMLElement("Components");
	ret->addSubElement(comps);
	{
		xml::XMLElement* e=new xml::XMLElement("ModelComponent");
		comps->addSubElement(e);
		e->addAttribute("ModelPath",(m_owner->GetName()+".mdx").asChar());
	}
	return comps;
}


void PhysicalComponentData::LoadData(const MDagPath& dpath,ExporterParams& params)
{
	m_sceneNode->LoadData(dpath,params);
	MPlug plug;


	MFnTransform* nodeTransform = NULL;
	MFnMesh mesh(dpath);
	// get camera transform
	for (int i=0; i<mesh.parentCount(); i++)
	{
		if (mesh.parent(i).hasFn(MFn::kTransform))
		{
			nodeTransform = new MFnTransform(mesh.parent(i));
			continue;
		}
	}

	if(!nodeTransform)
		return;
	plug = nodeTransform->findPlug("IsDynamic");
	plug.getValue(m_dynamic);
	plug = nodeTransform->findPlug("Kinematic");
	plug.getValue(m_kinematic);
	plug = nodeTransform->findPlug("Mass");
	plug.getValue(m_mass);
	plug = nodeTransform->findPlug("Density");
	plug.getValue(m_density);
	plug = nodeTransform->findPlug("SolverIterations");
	plug.getValue(m_solverIterations);
}
xml::XMLElement* PhysicalComponentData::Export(xml::XMLElement* parent,ExporterParams& params)
{
	if(m_sceneNode)
		m_sceneNode->Export(parent,params);
	xml::XMLElement* ret=new xml::XMLElement("PhysicalComponent");
	if(parent)
		parent->addSubElement(ret);

	ret->addAttribute("Name",m_owner->GetName().asChar());
	ret->addAttribute("Position",core::StringConverter::toString(m_owner->GetPosition()));
	ret->addAttribute("Rotation",core::StringConverter::toString(m_owner->GetRotation()));

	xml::XMLElement* physicalE=new xml::XMLElement("PhysicalNode");
	ret->addSubElement(physicalE);
	if(m_dynamic)
	{
		xml::XMLElement* bodyE=new xml::XMLElement("PhysicalBody");
		physicalE->addSubElement(bodyE);
		bodyE->addAttribute("Mass",core::StringConverter::toString(m_mass));
		bodyE->addAttribute("Kinematic",core::StringConverter::toString(m_kinematic));
	}
	xml::XMLElement* shapesE=new xml::XMLElement("PhysicalBody");
	physicalE->addSubElement(shapesE);

	xml::XMLElement* comps=new xml::XMLElement("Components");
	ret->addSubElement(comps);
	{
		xml::XMLElement* e=new xml::XMLElement("ModelComponent");
		comps->addSubElement(e);
		e->addAttribute("ModelPath",(m_owner->GetName()+".mdx").asChar());
	}
	return comps;
}

}
}