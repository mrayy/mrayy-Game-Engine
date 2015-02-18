


/********************************************************************
	created:	2010/11/04
	created:	4:11:2010   22:45
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ImageSetTexture.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ImageSetTexture
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ImageSetTexture_h__
#define ImageSetTexture_h__

#include "mstring.h"
#include "compileConfig.h"

namespace mray
{
	namespace xml
	{
		class XMLElement;
	}
namespace video
{

	class ImageSet;
	class ImageRegion;

class MRAY_DLL ImageSetTexture
{
private:
protected:
	core::string m_imageSetName;
	core::string m_imageName;

	ImageSet* m_imageSet;
	ImageRegion* m_image;
public:
	ImageSetTexture();
	ImageSetTexture(const core::string&set,const core::string&image);

	virtual ~ImageSetTexture();
	
	void SetImageSetName(const core::string&name);
	const core::string& GetImageSetName()const;

	void SetImageName(const core::string&name);
	const core::string& GetImageName()const;

	ImageSet* GetImageSet()const;
	ImageRegion* GetImage()const;

	void Deserialize(xml::XMLElement*elem);
	xml::XMLElement* Serialize(xml::XMLElement*elem);
};

}
}


#endif // ImageSetTexture_h__
