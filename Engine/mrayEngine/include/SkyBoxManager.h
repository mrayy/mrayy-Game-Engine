
/********************************************************************
	created:	2009/03/24
	created:	24:3:2009   21:30
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\SkyBoxManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	SkyBoxManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___SkyBoxManager___
#define ___SkyBoxManager___

#include "ISingleton.h"
#include "ITexture.h"

namespace mray{
namespace scene{

class MRAY_DLL SkyBoxManager:public ISingleton<SkyBoxManager>
{
private:
protected:

	typedef std::map<core::string,video::ITexturePtr > SkyboxMap;
	SkyboxMap m_textures;

public:
	SkyBoxManager();
	virtual~SkyBoxManager();

	video::ITexturePtr  loadSkyFromFolder(const core::string&path,const core::string&ext,const core::string&name);
	video::ITexturePtr  getTexture(const core::string&name);
	void listResources(std::vector<core::string>&outList);
};

}
}


#endif //___SkyBoxManager___
