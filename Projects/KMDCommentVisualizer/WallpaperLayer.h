




/********************************************************************
	created:	2013/12/05
	created:	5:12:2013   20:26
	filename: 	C:\Development\mrayEngine\Projects\KMDCommentVisualizer\WallpaperLayer
	file base:	WallpaperLayer
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef WallpaperLayer_h__
#define WallpaperLayer_h__

#include "GenericRenderLayer.h"

namespace mray
{
	namespace GUI
	{
		class GUIPictureWallpaper;
	}
namespace kmd
{
class WallpaperBin;
class WallpaperLayer :public GenericRenderLayer
{
protected:
	WallpaperBin* m_wallBin;
	struct CWallpaperInfo
	{
		enum EStatus
		{
			EFadeIn,
			EIdle,
			EFadeOut
		};
		CWallpaperInfo()
		{
			status = EFadeIn;
			timeOut = 0;
			direction = math::Randomizer::rand01() > 0.5 ? 1 : -1;
		}
		GUI::GUIPictureWallpaper* picture;
		float timeOut;
		EStatus status;
		float direction;
	};

	std::vector<CWallpaperInfo> m_pictures;

	float m_nextImage;
public:
	WallpaperLayer();
	virtual~WallpaperLayer();

	virtual void InitLayer(GUI::IGUIPanelElement* panel);
	virtual void UpdateLayer(float dt);


};

}
}

#endif // WallpaperLayer_h__
