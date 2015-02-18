

/********************************************************************
	created:	2010/11/04
	created:	4:11:2010   14:18
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ImageSet.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ImageSet
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ImageSet_h__
#define ImageSet_h__


#include "mString.h"
#include "TextureUnit.h"


namespace mray
{
namespace xml
{
	class XMLElement;
}

namespace video
{

class ImageSet;


class MRAY_DLL ImageRegion
{
	ImageSet* m_owner;
	core::string m_name;
	math::rectf m_srcRect;
	math::rectf m_textureRect;
public:
	ImageRegion(ImageSet* owner);
	virtual~ImageRegion();

	void SetName(const core::string&name);
	const core::string& GetName();

	void SetSrcRect(const math::rectf& rc);
	const math::rectf& GetSrcRect();

	void SetTextureRect(const math::rectf& rc);
	const math::rectf& GetTextureRect();

	ImageSet* GetOwner();

	void Deserialize(xml::XMLElement*elem);
	xml::XMLElement* Serialize(xml::XMLElement*elem);
};

class MRAY_DLL ImageSet:public IResource
{
private:
protected:

	core::string m_name;
	core::string m_textureName;
	TextureUnitPtr m_texture;

	typedef std::map<core::string,ImageRegion*> RegionMap;
	RegionMap m_regions;

	virtual void loadInternal();
	virtual void unloadInternal();
	virtual uint calcSizeInternal();
public:
	ImageSet();
	virtual ~ImageSet();

	void SetName(const core::string&name);
	const core::string& GetName();

	ImageRegion* CreateRegion(const core::string&name);

	TextureUnit* GetTexture();
	const core::string& GetTextureName();
	void SetTextureName(const core::string&tex);

	void RemoveRegion(const core::string&name);
	ImageRegion* GetRegion(const core::string&name);

	void ClearRegions();

	void Deserialize(xml::XMLElement*elem);
	xml::XMLElement* Serialize(xml::XMLElement*elem);
};

MakeSharedPtrType(ImageSet);

}
}


#endif // ImageSet_h__
