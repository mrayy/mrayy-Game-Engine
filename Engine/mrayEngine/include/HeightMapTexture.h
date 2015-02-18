
/********************************************************************
	created:	2009/03/19
	created:	19:3:2009   11:02
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\HeightMapTexture.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	HeightMapTexture
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___HeightMapTexture___
#define ___HeightMapTexture___

#include "IHeightMapLoader.h"
#include "ITexture.h"
#include "GCPtr.h"


namespace mray{
namespace loaders{

class MRAY_DLL HeightMapTexture:public IHeightMapLoader
{
private:
protected:
	math::vector2d m_size;
	std::vector<double> m_heightData;
	core::string m_fileName;
public:
	HeightMapTexture();
	virtual~HeightMapTexture();

	bool setTexture(video::ITexture* tex);
	const core::string& getFileName();

	virtual double getHeight(int x,int y);
	virtual int getWidth();
	virtual int getHeight();

	video::ITexturePtr  GenerateTexture(video::IVideoDevice*device);

	virtual double* getHeightData();
};

}
}


#endif //___HeightMapTexture___
