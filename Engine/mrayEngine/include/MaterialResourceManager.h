
/********************************************************************
	created:	2009/01/05
	created:	5:1:2009   21:08
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\MaterialResourceManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	MaterialResourceManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___MaterialResourceManager___
#define ___MaterialResourceManager___

#include "IResourceManager.h"
#include "ISingleton.h"
#include "RenderMaterial.h"

namespace mray{
class MRAY_DLL MaterialResourceManager:public IResourceManager,public ISingleton<MaterialResourceManager>
{
protected:
	virtual IResourcePtr createInternal(const core::string& name);

	virtual IResourcePtr loadResourceFromFile(OS::IStream* file);

	virtual core::string getDefaultGroup();
public:
	MaterialResourceManager();
	virtual~MaterialResourceManager();

	void parseMaterialScript(OS::IStream* file);
	void parseMaterialXML(OS::IStream* file);
	void exportMaterialXML(const OS::IStreamPtr& file,bool onlyUsed=false);

	void parseMaterialXML(xml::XMLElement*elem);
	xml::XMLElement* exportMaterialXML(xml::XMLElement*elem,bool onlyUsed=false);

	video::RenderMaterialPtr getMaterial(const core::string&name);

	virtual void writeResourceToDist(const core::string&resName,const core::string&fileName);
	virtual void writeResourceToDist(const IResourcePtr& resource,const core::string&fileName);
	virtual void writeResourceToDist(ResourceHandle resource,const core::string&fileName);

};
#define gMaterialResourceManager mray::MaterialResourceManager::getInstance()

}


#endif //___MaterialResourceManager___

