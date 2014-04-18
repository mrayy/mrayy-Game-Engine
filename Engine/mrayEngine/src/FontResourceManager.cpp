#include "stdafx.h"



#include "FontResourceManager.h"
#include "IVideoDevice.h"
#include "ILogManager.h"
#include "TraceManager.h"
#include "TexFont.h"
#include "IFont.h"
#include "IFileSystem.h"
#include "common.h"
#include "FTFont.h"
#include "Engine.h"
#include "DynamicFontGenerator.h"


namespace mray{


	
////////////////////////


class glyphXMLFontLoader:public IFontXMLParser
{
	static core::string type;
public:
	virtual const mchar* getType(){
		return type.c_str();
	}
	virtual bool canLoad(const mchar* t){
		return type.equals_ignore_case(t);
	}
	virtual GUI::IFont* parseAttributes(xml::XMLElement*attrs){
		traceFunction(eGUI);
		xml::XMLAttribute*a=attrs->getAttribute(mT("Name"));
		if(!a){
			gLogManager.log(mT("Error While Parsing Font XML File, No Name for the Font"),ELL_ERROR);
		}
		GUI::TexFont* font=new GUI::TexFont(a->value);
		font->LoadFont(attrs);

		

		return font;
	}
};
core::string glyphXMLFontLoader::type=mT("glyph");

////////////////////////


class truetypeXMLFontLoader:public IFontXMLParser
{
	static core::string type;
public:
	virtual const mchar* getType(){
		return type.c_str();
	}
	virtual bool canLoad(const mchar* t){
		return type.equals_ignore_case(t);
	}
	virtual GUI::IFont* parseAttributes(xml::XMLElement*attrs){
		traceFunction(eGUI);
		xml::XMLAttribute*a=attrs->getAttribute(mT("Name"));
		if(!a){
			gLogManager.log(mT("Error While Parsing Font XML File, No Name for the Font"),ELL_ERROR);
		}
		uint size=16;
		uint res=96;
		xml::XMLAttribute*attr=attrs->getAttribute(mT("Size"));
		if(attr){
			size=core::StringConverter::toUInt(attr->value);
		}
		attr=attrs->getAttribute(mT("Resolution"));
		if(attr){
			res=core::StringConverter::toUInt(attr->value);
		}

		GUI::FTFont* font=new GUI::FTFont(a->value,size,res);
		//OS::IStreamPtr s=gFileSystem.openFile(source->value);
		font->loadFont(attrs);



		return font;
	}
};
core::string truetypeXMLFontLoader::type=mT("trueType");

///////////////////////////////////////////



core::string FontResourceManager::getDefaultGroup(){
	return mT("Fonts");
}

FontResourceManager::FontResourceManager(video::IVideoDevice*device)
:IResourceManager(mT("Fonts Resource Manager")),m_device(device)
{
	addXMLParser(new glyphXMLFontLoader());
	addXMLParser(new truetypeXMLFontLoader());
}

FontResourceManager::~FontResourceManager(){
	m_parsers.clear();
	m_fontList.clear();
	m_defaultFont=0;
}
void FontResourceManager::setDevice(video::IVideoDevice* dev){
	m_device=dev;
}
IResourcePtr FontResourceManager::createInternal(const core::string& name){
	traceFunction(eGUI);
	GCPtr<GUI::IFont> font;
	font=loadResourceFromFile(gFileSystem.createBinaryFileReader(name.c_str()));


	if(font.isNull())
	{
		gLogManager.log(mT("Failed to load font: ")+name,ELL_WARNING);
	}
	return font;
}
IResourcePtr FontResourceManager::loadResourceFromFile(OS::IStream* file){
	traceFunction(eGUI);
	if(!file)return 0;

	GUI::IFont* font=m_fontParser.loadXML(file);
	
	if(font){
		m_fontList.insert(FontList::value_type(font->getName(),font));
	}
	
	return font;
}


const GUI::IFontPtr& FontResourceManager::getDefaultFont(){
	return m_defaultFont;
}
void FontResourceManager::setDefaultFont(const GUI::IFontPtr& f){
	m_defaultFont=f;
	m_fontList[mT("Default")]=f;
}

GUI::IFontPtr FontResourceManager::loadFont(const core::string& name){
	GUI::IFontPtr font=getOrCreate(name);
	return font;
}
const GUI::IFontPtr& FontResourceManager::getFontByName(const core::string& name){
	FontList::iterator it=m_fontList.find(name);
	if(it!=m_fontList.end())
		return it->second;
	return m_defaultFont;
}


void FontResourceManager::addXMLParser(const IFontXMLParserPtr& p){
	core::string t=p->getType();
	t.make_lower();
	m_parsers.insert(FontParsersMap::value_type(t,p));
}
const IFontXMLParserPtr& FontResourceManager::getXMLParser(const core::string&type){
	core::string t=type;
	t.make_lower();
	FontParsersMap::iterator it=m_parsers.find(t);
	if(it==m_parsers.end())return IFontXMLParserPtr::Null;
	return it->second;
}
void FontResourceManager::removeXMLParser(const core::string&type){
	core::string t=type;
	t.make_lower();
	m_parsers.erase(t);
}
void FontResourceManager::removeAll()
{
	IResourceManager::removeAll();
	m_fontList.clear();
	m_defaultFont=0;
}
void FontResourceManager::writeResourceToDist(const core::string&resName,const core::string&fileName){
	gLogManager.log(mT("FontResourceManager::writeResourceToDist() - not supported!"),ELL_WARNING);
}
void FontResourceManager::writeResourceToDist(const IResourcePtr& resource,const core::string&fileName){
	gLogManager.log(mT("FontResourceManager::writeResourceToDist() - not supported!"),ELL_WARNING);
}
void FontResourceManager::writeResourceToDist(ResourceHandle resource,const core::string&fileName){
	gLogManager.log(mT("FontResourceManager::writeResourceToDist() - not supported!"),ELL_WARNING);
}










}


