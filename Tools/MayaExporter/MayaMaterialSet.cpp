

#include "stdafx.h"
#include "MayaMaterialSet.h"
#include "MayaMaterial.h"
#include "ExporterParams.h"


#include "XMLWriter.h"
#include "IFileSystem.h"
#include "StreamWriter.h"



namespace mray
{
namespace exporter
{

	MayaMaterialSet* MayaMaterialSet::m_instance=0;

MayaMaterialSet::MayaMaterialSet()
{
	m_defaultMaterial = new MayaMaterial();
	m_defaultMaterial->m_type = MT_LAMBERT;
	m_defaultMaterial->m_name = "defaultLambert";
	m_defaultMaterial->m_ambient = MColor(0,0,0,1);
	m_defaultMaterial->m_emissive = MColor(0,0,0,1);
	m_defaultMaterial->m_diffuse = MColor(0.5,0.5,0.5,1);
}
MayaMaterialSet::~MayaMaterialSet()
{
	Clear();
	m_instance=0;
}

void MayaMaterialSet::Clear()
{
	std::map<core::string,MayaMaterial*>::iterator it=m_materials.begin();
	for (;it!=m_materials.end();++it)
	{
		delete it->second;
	}
	m_materials.clear();
}
MayaMaterial* MayaMaterialSet::AddMaterial(const core::string& name,MayaMaterial* m)
{
	bool found = false;
	std::map<core::string,MayaMaterial*>::iterator it=m_materials.find(name);
	if(it==m_materials.end())
	{
		m_materials[name]=m;
		return m;
	}else
	{
		delete m;
		return it->second;
	}
}
MayaMaterial* MayaMaterialSet::GetMaterial(const core::string &name)
{
	std::map<core::string,MayaMaterial*>::iterator it=m_materials.find(name);
	if(it==m_materials.end())
		return 0;
	return it->second;
}
MayaMaterial* MayaMaterialSet::GetDefaultMaterial()
{
	return m_defaultMaterial;
}

xml::XMLElement* MayaMaterialSet::GenerateXML(ExporterParams& params)
{
	xml::XMLElement* root=new xml::XMLElement("Materials");
	std::map<core::string,MayaMaterial*>::iterator it=m_materials.begin();
	for (;it!=m_materials.end();++it)
	{
		it->second->saveXML(root);
	}

	/*
	OS::IStreamPtr matXML=gFileSystem.openFile(core::string(params.basePath.asChar())+"materials.xml",OS::BIN_WRITE);
	xml::XMLWriter writer;
	writer.addElement(root);
	core::string xmlStr=writer.flush();

	OS::StreamWriter sWriter(matXML);
	sWriter.writeString(xmlStr);
	matXML->close();*/
	return root;

}

void MayaMaterialSet::ExportTextures(ExporterParams& params)
{
	std::map<core::string,MayaMaterial*>::iterator it=m_materials.begin();
	for (;it!=m_materials.end();++it)
	{
		it->second->copyTextures(params);
	}
	if(params.materialFileName!="")
	{
		xml::XMLElement* e= GenerateXML(params);

		OS::IStreamPtr matXML=gFileSystem.openFile(core::string(params.basePath.asChar())+params.materialFileName.asChar(),OS::BIN_WRITE);

		xml::XMLWriter writer;
		writer.addElement(e);
		core::string xmlStr=writer.flush();

		OS::StreamWriter sWriter(matXML);
		sWriter.writeString(xmlStr);
		matXML->close();
		delete e;
	}
}

MayaMaterialSet& MayaMaterialSet::GetInstance()
{
	if(!m_instance)
		m_instance=new MayaMaterialSet();
	return *m_instance;
}
MayaMaterialSet* MayaMaterialSet::GetInstancePtr()
{
	return m_instance;
}


}
}


