

/********************************************************************
	created:	2012/02/09
	created:	9:2:2012   16:21
	filename: 	d:\Development\mrayEngine\Tools\MayaExporter\MayaMaterialSet.h
	file path:	d:\Development\mrayEngine\Tools\MayaExporter
	file base:	MayaMaterialSet
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __MayaMaterialSet__
#define __MayaMaterialSet__


namespace mray
{
namespace exporter
{
	class MayaMaterial;
	class ExporterParams;

class MayaMaterialSet
{
private:
protected:
	static MayaMaterialSet* m_instance;

	MayaMaterial* m_defaultMaterial;
	std::map<core::string,MayaMaterial*> m_materials;

public:
	MayaMaterialSet();
	virtual~MayaMaterialSet();

	void Clear();
	MayaMaterial* AddMaterial(const core::string& name,MayaMaterial* m);
	MayaMaterial* GetMaterial(const core::string &name);
	MayaMaterial* GetDefaultMaterial();

	xml::XMLElement* GenerateXML(ExporterParams& params);
	void ExportTextures(ExporterParams& params);

	static MayaMaterialSet& GetInstance();
	static MayaMaterialSet* GetInstancePtr();

};

}
}

#endif

