




/********************************************************************
	created:	2013/12/05
	created:	5:12:2013   20:26
	filename: 	C:\Development\mrayEngine\Projects\KMDCommentVisualizer\WallpaperBin
	file base:	WallpaperBin
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef WallpaperBin_h__
#define WallpaperBin_h__

namespace mray
{
namespace kmd
{

class WallpaperBin
{
protected:
	std::vector<core::string> m_picturesNames;
	std::vector<video::ITexturePtr> m_textures;

	int m_nextPicture;
public:
	WallpaperBin();
	virtual~WallpaperBin();

	void LoadPicturesFromDir(const core::string& dir);

	std::vector<core::string> GetPicturesNames();
	std::vector<video::ITexturePtr> GetPictures();

	video::ITexturePtr GetNextRandomPicture();

};

}
}
#endif // WallpaperBin_h__
