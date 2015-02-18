
/********************************************************************
	created:	2009/08/11
	created:	11:8:2009   9:10
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGLDevice\GLTexture.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGLDevice
	file base:	GLTexture
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GLTexture___
#define ___GLTexture___

#include <vector>
#include <ITexture.h>
#include "CompileConfig.h"

namespace mray{
namespace video{

class MRAY_GL_DLL GLTexture:public ITexture
{
private:
protected:
	uint m_TextureID;

	bool m_TextureChanged;

	uint m_target;

	bool m_dirty;


	virtual void preLoadInternal(bool async);
	virtual void unloadInternal();
	void unloadInternal(bool removeTexture);


	//GCPtr<ImageInfo> m_imageInfo;

	typedef std::vector<IHardwarePixelBuffer*> SurfaceList;
	SurfaceList m_surfaces;


	virtual uint calcSizeInternal();

	virtual void _innerCreateTexture();
public:
	GLTexture(ETextureType type,bool internal);
	virtual~GLTexture();

	uint getTarget(){return m_target;}

	uint getTextureID();

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
/*
	virtual bool createTexture(const math::vector3di &size,EPixelFormat EPixelFormat);
	virtual void setData(const void*data,const math::vector3di& size,EPixelFormat type,int face);
*/

	virtual const core::string& getDeviceType()const;

	virtual ITexture*getDuplicate() ;

	virtual bool isPBuffer(){return false;}

};

}
}

#endif //___GLTexture___
