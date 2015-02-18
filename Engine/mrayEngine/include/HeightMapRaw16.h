
/********************************************************************
	created:	2009/03/19
	created:	19:3:2009   10:19
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\HeightMapRaw16.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	HeightMapRaw16
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___HeightMapRaw16___
#define ___HeightMapRaw16___

#include "IHeightMapLoader.h"
#include "IStream.h"

#include "GCPtr.h"
#include "ITexture.h"

namespace mray{
namespace loaders{

class MRAY_DLL HeightMapRaw16:public IHeightMapLoader
{
private:
protected:
	int m_length;
	std::vector<double> m_heightData;
	core::string m_fileName;
public:

	HeightMapRaw16();
	virtual~HeightMapRaw16();

	const core::string& getFileName();
	bool loadHeightMap(OS::IStreamPtr file);

	virtual double getHeight(int x,int y);
	virtual int getWidth();
	virtual int getHeight();

	video::ITexturePtr  GenerateTexture(video::IVideoDevice*device);

	virtual double* getHeightData();
};

}
}


#endif //___HeightMapRaw16___
