
/********************************************************************
	created:	2009/03/19
	created:	19:3:2009   10:17
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IHeightMapLoader.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IHeightMapLoader
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IHeightMapLoader___
#define ___IHeightMapLoader___

#include "mstring.h"
#include "ITexture.h"

namespace mray{
namespace loaders{

class IHeightMapLoader
{
private:
public:
	IHeightMapLoader(){}
	virtual~IHeightMapLoader(){}

	virtual const core::string& getFileName()=0;

	//0 -> 1 value
	virtual double getHeight(int x,int y)=0;
	virtual int getWidth()=0;
	virtual int getHeight()=0;
	virtual double* getHeightData()=0;

	virtual video::ITexturePtr  GenerateTexture(video::IVideoDevice*device)=0;
};

}
}


#endif //___IHeightMapLoader___
