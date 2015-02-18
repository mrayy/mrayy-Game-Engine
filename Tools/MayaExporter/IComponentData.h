


/********************************************************************
	created:	2012/09/13
	created:	13:9:2012   11:08
	filename: 	d:\Development\mrayEngine\Tools\MayaExporter\IComponentData.h
	file path:	d:\Development\mrayEngine\Tools\MayaExporter
	file base:	IComponentData
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___IComponentData___
#define ___IComponentData___



namespace mray
{
namespace exporter
{

	class MayaNode;
	class ExporterParams;

class IComponentData
{
protected:

	MayaNode* m_owner;

public:
	IComponentData(MayaNode* o):m_owner(o)
	{}
	virtual~IComponentData(){}
	
	virtual void LoadData(const MDagPath& dpath,ExporterParams& params)=0;
	virtual xml::XMLElement* Export(xml::XMLElement* parent,ExporterParams& params)=0;
};


class SceneComponentData:public IComponentData
{
protected:
public:
	SceneComponentData(MayaNode* o):IComponentData(o)
	{
	}
	virtual void LoadData(const MDagPath& dpath,ExporterParams& params);
	virtual xml::XMLElement* Export(xml::XMLElement* parent,ExporterParams& params);
};

class PhysicalComponentData:public IComponentData
{
protected:
	bool m_dynamic;
	bool m_kinematic;
	int m_solverIterations;
	float m_mass;
	float m_density;

	class CTargetNode
	{
	public:
	};
	SceneComponentData* m_sceneNode;
public:
	PhysicalComponentData(MayaNode* o):IComponentData(o)
	{
		m_sceneNode=new SceneComponentData(o);;
	}
	virtual~PhysicalComponentData()
	{
		delete m_sceneNode;
	}
	virtual void LoadData(const MDagPath& dpath,ExporterParams& params);
	virtual xml::XMLElement* Export(xml::XMLElement* parent,ExporterParams& params);
};


}
}

#endif
