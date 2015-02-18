


/********************************************************************
	created:	2012/01/08
	created:	8:1:2012   15:44
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\LUTGenerator.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	LUTGenerator
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __LUTGenerator__
#define __LUTGenerator__

#include "ITexture.h"

namespace mray
{
namespace video
{

	class ColorCurve;
	class IVideoDevice;

class MRAY_DLL LUTGenerator
{
private:
protected:
public:
	LUTGenerator();
	virtual~LUTGenerator();


	video::ITexturePtr GenerateLUT(IVideoDevice* device,const ColorCurve*r,const ColorCurve*g,const ColorCurve*b);
};

}
}

#endif
