
/********************************************************************
	created:	2009/03/19
	created:	19:3:2009   10:19
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\HeightMapRaw8.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	HeightMapRaw8
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___HeightMapRaw8___
#define ___HeightMapRaw8___

#include "IHeightMapLoader.h"
#include "IStream.h"

#include "GCPtr.h"
#include "ITexture.h"

namespace mray{
namespace loaders{

class MRAY_DLL HeightMapRaw8:public IHeightMapLoader
{
private:
protected:
	int m_length;
	std::vector<double> m_heightData;
	core::string m_fileName;
public:

	HeightMapRaw8();
	virtual~HeightMapRaw8();

	bool loadHeightMap(OS::IStreamPtr file);
	const core::string& getFileName();

	virtual double getHeight(int x,int y);
	virtual int getWidth();
	virtual int getHeight();

	video::ITexturePtr  GenerateTexture(video::IVideoDevice*device);

	virtual double* getHeightData();
};

}
}


#endif //___HeightMapRaw8___
