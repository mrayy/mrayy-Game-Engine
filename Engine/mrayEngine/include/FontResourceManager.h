

#ifndef ___FontResourceManager___
#define ___FontResourceManager___

#include "IResourceManager.h"
#include "ISingleton.h"
#include "IStream.h"
#include "GCPtr.h"
#include "IFont.h"
#include "XML_Font.h"


namespace mray{
namespace video{	
	class IVideoDevice;
}

class IFontXMLParser
{
public:
	virtual const mchar* getType()=0;
	virtual bool canLoad(const mchar* type)=0;
	virtual GUI::IFont* parseAttributes(xml::XMLElement*attrs)=0;
};

MakeSharedPtrType(IFontXMLParser);

class MRAY_DLL FontResourceManager:public IResourceManager,public ISingleton<FontResourceManager>
{
	typedef std::map<core::string,IFontXMLParserPtr> FontParsersMap;
	typedef std::map<core::string,GUI::IFontPtr> FontList;

	FontList m_fontList;
	video::IVideoDevice*m_device;
	GUI::XML_Font m_fontParser;

	FontParsersMap m_parsers;

	GUI::IFontPtr m_defaultFont;

protected:
	virtual IResourcePtr createInternal(const core::string& name);
	
	virtual IResourcePtr loadResourceFromFile(OS::IStream* file);

	virtual core::string getDefaultGroup();
	
public:
	FontResourceManager(video::IVideoDevice*device);
	virtual~FontResourceManager();

	const GUI::IFontPtr& getDefaultFont();
	void setDefaultFont(const GUI::IFontPtr& f);

	int loadFontsFromDir(const core::string& dir);
	GUI::IFontPtr loadFont(const core::string& name);
	const GUI::IFontPtr& getFontByName(const core::string& name);

	void addFont(GUI::IFontCRef f);

	void setDevice(video::IVideoDevice* dev);

	void addXMLParser(const IFontXMLParserPtr& p);
	const IFontXMLParserPtr& getXMLParser(const core::string&type);
	void removeXMLParser(const core::string&type);
	
	void removeAll();

	
	virtual void writeResourceToDist(const core::string&resName,const core::string&fileName);
	virtual void writeResourceToDist(const IResourcePtr& resource,const core::string&fileName);
	virtual void writeResourceToDist(ResourceHandle resource,const core::string&fileName);

};
#define gFontResourceManager mray::FontResourceManager::getInstance()

}

#endif


