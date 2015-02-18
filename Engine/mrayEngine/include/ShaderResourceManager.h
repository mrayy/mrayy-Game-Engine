
/********************************************************************
	created:	2008/12/24
	created:	24:12:2008   12:27
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\ShaderResourceManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	ShaderResourceManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ShaderResourceManager___
#define ___ShaderResourceManager___

#include "IResourceManager.h"
#include "ISingleton.h"
#include "IGPUShaderProgram.h"

namespace mray{
	namespace video
	{
		class CShaderFactory;
		class IGPUShaderFactory;
	}

class MRAY_DLL ShaderResourceManager:public IResourceManager,public ISingleton<ShaderResourceManager>
{
	video::IVideoDevice*m_device;
	video::CShaderFactory*m_gpuFactory;

	core::string m_defaultShaderType;
	video::IGPUShaderProgramPtr m_nullShader;

	video::IGPUShaderProgramPtr m_defaultShader;

	bool m_supportShaders;
protected:
	virtual IResourcePtr  createInternal(const core::string& name);

	virtual IResourcePtr  loadResourceFromFile(OS::IStream* file);


	virtual core::string getDefaultGroup();
public:
	ShaderResourceManager(video::IVideoDevice*device);
	virtual~ShaderResourceManager();

	void parseShaderXML(xml::XMLElement*elem);
	video::IGPUShaderProgramPtr parseShaderXMLElement(xml::XMLElement*elem,bool isAddResource);
	xml::XMLElement* exportShaderXML(xml::XMLElement*elem,bool onlyUsed=false);

	void parseShaderXML(OS::IStream* file);
	void exportShaderXML(const OS::IStreamPtr& file,bool onlyUsed=false);

	void setDevice(video::IVideoDevice* dev);
	void addShaderLoader(video::IGPUShaderFactory* loader);

	video::IGPUShaderProgramPtr loadShaderFromFile(const core::string&path,video::EShaderProgramType programType,
		const char*entryPoint, const video::ShaderPredefList& predef, const core::string&type = mT(""));

	video::IGPUShaderProgramPtr loadShaderFromProgram(const core::string&name,const core::string&program, video::EShaderProgramType programType,
		const char*entryPoint, const video::ShaderPredefList& predef, const core::string&type = mT(""));

	video::IGPUShaderProgramPtr createEmptyShader(video::EShaderProgramType programType, const core::string&type);

	void setDefaultShaderType(const core::string&type);
	const core::string& getDefaultShaderType();

	const video::IGPUShaderProgramPtr& getDefaultShader();
	void setDefaultShader(const video::IGPUShaderProgramPtr&shader);


	virtual void writeResourceToDist(const core::string&resName,const core::string&fileName);
	virtual void writeResourceToDist(const IResourcePtr& resource,const core::string&fileName);
	virtual void writeResourceToDist(ResourceHandle resource,const core::string&fileName);

};
#define gShaderResourceManager mray::ShaderResourceManager::getInstance()


}


#endif //___ShaderResourceManager___

