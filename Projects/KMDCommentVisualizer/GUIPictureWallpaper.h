




/********************************************************************
	created:	2013/12/05
	created:	5:12:2013   20:26
	filename: 	C:\Development\mrayEngine\Projects\KMDCommentVisualizer\GUIPictureWallpaper
	file base:	GUIPictureWallpaper
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GUIPictureWallpaper_h__
#define GUIPictureWallpaper_h__

#include "GUIStaticImage.h"

namespace mray
{
namespace GUI
{

class GUIPictureWallpaper :public GUIStaticImage
{
	DECLARE_RTTI;
public:
	static const GUID ElementType;
protected:
	float m_frameSize;
public:
	GUIPictureWallpaper(IGUIManager* m);
	virtual ~GUIPictureWallpaper();

	virtual void Draw(const math::rectf*vp);

	virtual void Update(float dt);
};
DECLARE_ELEMENT_FACTORY(GUIPictureWallpaper);

}
}

#endif // GUIPictureWallpaper_h__
