

#ifndef ___GUIProfile___
#define ___GUIProfile___

#include "mString.h"


#include "SColor.h"
#include "GCPtr.h"
#include "ImageSet.h"

namespace mray{
namespace GUI{

	

class MRAY_DLL GUIProfile
{
public:
	class GUIRegion{
	public:
		int id;
		//core::string name;
		core::string targetImage;
		
		math::rectf sourceRect;
		math::rectf texCoords;
		bool operator <(const GUIRegion&o)const{
			return id<o.id;
		}
		const math::rectf& GetSrcRect()const
		{
			return sourceRect;
		}
		const math::rectf& GetTextureRect()const
		{
			return texCoords;
		}
	};


	GUIProfile(const core::string&name);
	virtual~GUIProfile();

	const core::string&getName();

	void addRegion(const GUIRegion& r);

	//GUIRegion* getRegion(const core::string& name);
	GUIRegion* getRegion(int id);
	int getRegionCount();

	video::ImageSet* GetImageSet();
	const core::string& GetImageSetName();

	void SetImageSetName(const core::string& name);


protected:
	core::string m_imageSetName;
	video::ImageSetPtr m_imageSet;
	core::string m_name;
	typedef std::map<int,GUIRegion> RegionMap;
	RegionMap m_regions;
};

MakeSharedPtrType(GUIProfile)

}
}


#endif

