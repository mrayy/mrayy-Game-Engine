


/********************************************************************
	created:	2011/11/20
	created:	20:11:2011   16:10
	filename: 	d:\Development\mrayEngine\Engine\mrayD3D9Device\D3D9Texture.h
	file path:	d:\Development\mrayEngine\Engine\mrayD3D9Device
	file base:	D3D9Texture
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __D3D9Texture__
#define __D3D9Texture__

#include "compileConfig.h"
#include "ITexture.h"

namespace mray
{
namespace video
{

class MRAY_D3D9_DLL D3D9Texture:public ITexture
{
private:
protected:
	LPDIRECT3DTEXTURE9 m_pTexture;
public:
	D3D9Texture(ETextureType type,bool internal);
	virtual~D3D9Texture();

	virtual void createTexture(const math::vector3di&sz,EPixelFormat pf);

	virtual int getSurfaceCount();

	virtual ETextureType getType();

	virtual void setRenderTarget(bool rt);
	virtual bool isRenderTarget();

	virtual void onUse();


	virtual void setMipmapsFilter(bool e);

	virtual void setBilinearFilter(bool e);

	virtual void setTrilinearFilter(bool e);

	virtual void setAnisotropicFilter(bool e);

	virtual IHardwarePixelBuffer* getSurface(int face);
	virtual const math::vector3di& getSize();

	virtual const core::string& getDeviceType()const;

	virtual ITexture*getDuplicate() ;

	virtual bool isPBuffer(){return false;}
};

}
}

#endif
