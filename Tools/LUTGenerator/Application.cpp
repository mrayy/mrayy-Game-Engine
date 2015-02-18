
#include "stdafx.h"
#include "Application.h"
#include "TextureResourceManager.h"


namespace mray
{

Application::Application()
{
}
Application::~Application()
{
}

void Application::init(const OptionContainer &extraOptions)
{
	CMRayApplication::init(extraOptions);
	CMRayApplication::loadResourceFile(mT("Resources.stg"));

//	getSceneManager()->createViewport(mT("MainVP"),0);

	bool generate=0;

	if(generate)
	{
		video::ITexturePtr tex3D=Generate3DLUTTexture();
		tex3D->load(false);
		//video::ITexturePtr tex3D=Generate3DLUTTexture(16);
		m_texture2D=Convert3DTextureTo2DTexture(tex3D);

		gTextureResourceManager.writeResourceToDist(m_texture2D,"base_LUT.png");
		//gTextureResourceManager.writeResourceToDist(tex3D,"LUT.mtex");
	}else
	{
		m_texture2D=gTextureResourceManager.loadTexture2D("LUT.png");
		m_texture2D->setMipmapsFilter(false);
		m_texture2D->load(false);
		video::ITexturePtr tex3D=Convert2DTextureTo3DTexture(m_texture2D,8,4);

		gTextureResourceManager.writeResourceToDist(tex3D,"LUT.mtex");
	}
}
video::ITexturePtr Application::Generate3DLUTTexture()
{
	math::vector3d tex3DSize=32;
 video::ITexturePtr generatedTexture=gTextureResourceManager.createTexture3D("LUT",tex3DSize,video::EPixel_R8G8B8);

	video::IHardwarePixelBuffer* surface=generatedTexture->getSurface(0);
	video::LockedPixelBox box=surface->lock(math::box3d(0,0,0,tex3DSize.x,tex3DSize.y,tex3DSize.z),video::IHardwareBuffer::ELO_Discard);

	struct Pixel
	{
		uchar b,g,r;
	}*p;
	p = (Pixel*)box.data;

	Pixel val;

	int idx=0;

	for(int k=0;k<tex3DSize.z;++k)
	{
		val.b=256*(float)k/(float)tex3DSize.z;
		for(int i=0;i<tex3DSize.x;++i)
		{
			val.g=256*(float)i/(float)tex3DSize.x;
			for(int j=0;j<tex3DSize.y;++j)
			{
				val.r=256*(float)j/(float)tex3DSize.y;

				idx=(k*tex3DSize.y+i)*tex3DSize.x + j;

				p[idx]=val;
			}
		}
	}
	surface->unlock();
	
	/*
	video::ITexturePtr generatedTexture;

	math::vector2di tex2DSize(128,256);

	generatedTexture=gTextureResourceManager.createTexture2D("LUT",tex2DSize,video::EPixel_R8G8B8);

	video::IHardwarePixelBuffer* surface=generatedTexture->getSurface(0);
	video::LockedPixelBox box=surface->lock(math::box3d(0,0,0,tex2DSize.x,tex2DSize.y,1),video::IHardwareBuffer::ELO_Discard);

	struct Pixel
	{
		uchar b,g,r;
	}*p;
	p = (Pixel*)box.data;

	Pixel val;

	int idx=0;

	int sqrtZ=sqrt((float)tex3DSize.z);

	for(int k=0;k<tex3DSize.z;++k)
	{
		val.b=256*(float)k/(float)tex3DSize.z;
		int row=tex3DSize.x*(k/sqrtZ);
		int col=tex3DSize.y*(k%sqrtZ);

		for(int i=0;i<tex3DSize.x;++i)
		{
			val.g=256*(float)i/(float)tex3DSize.x;
			for(int j=0;j<tex3DSize.y;++j)
			{
				val.r=256*(float)j/(float)tex3DSize.y;

				idx=(((row+i))*tex2DSize.x) + (col + j);

				p[idx]=val;
			}
		}
	}
	surface->unlock();
	*/


	return generatedTexture;
	
}

video::ITexturePtr Application::Convert3DTextureTo2DTexture(const video::ITexturePtr& tex)
{
	video::IHardwarePixelBuffer* buffer= tex->getSurface(0);
	int depth=buffer->getDepth();
	int cols=depth;
	int rows=1;
	while(cols%2==0 && cols>rows)
	{
		rows*=2;
		cols=cols/2;
	}

	math::vector2d size(buffer->getWidth()*cols ,buffer->getHeight()*rows);
	
	video::ITexturePtr generatedTexture=gTextureResourceManager.createTexture2D(tex->getResourceName()+"_2D",size,video::EPixel_R8G8B8);

	video::IHardwarePixelBuffer* surface=generatedTexture->getSurface(0);
	video::LockedPixelBox box=surface->lock(math::box3d(0,0,0,size.x,size.y,1),video::IHardwareBuffer::ELO_Discard);
	video::LockedPixelBox boxSrc=buffer->lock(math::box3d(0,0,0,buffer->getWidth(),
		buffer->getHeight(),buffer->getDepth()),video::IHardwareBuffer::ELO_ReadOnly);

	struct Pixel
	{
		uchar b,g,r;
	}*p,*pSrc;
	p = (Pixel*)box.data;
	pSrc = (Pixel*)boxSrc.data;


	int idx=0;
	int surfaceNumber=0;

	int pixels=0;

	OS::IStreamPtr stream=gFileSystem.openFile("idx.txt",OS::TXT_WRITE);
	OS::StreamWriter writer(stream);

	for(int i=0;i<rows;++i)
	{
		int y=i*buffer->getHeight();
		for(int j=0;j<cols;++j)
		{
			int x=j*buffer->getWidth();
			for(int y1=0;y1<buffer->getHeight();++y1)
			{
				for(int x1=0;x1<buffer->getWidth();++x1)
				{
					idx=(y+y1)*size.x+(x+x1);
					int idx2=(surfaceNumber*buffer->getHeight()+y1)*buffer->getWidth()+x1;;
					p[idx]=pSrc[idx2];
					core::string str=(core::StringConverter::toString(idx)+":");
					str+=core::StringConverter::toString(pixels)+"{";
					str+=core::StringConverter::toString((int)pSrc[idx2].r)+",";
					str+=core::StringConverter::toString((int)pSrc[idx2].g)+",";
					str+=core::StringConverter::toString((int)pSrc[idx2].b)+"}";
					writer.writeLine(str);
					++pixels;
				}
			}
			++surfaceNumber;
		}
	}
	stream->close();

	surface->unlock();
//	buffer->unlock();

	return generatedTexture;
}

video::ITexturePtr Application::Convert2DTextureTo3DTexture(const video::ITexturePtr& tex,int rows,int cols)
{
	video::IHardwarePixelBuffer* buffer= tex->getSurface(0);
	int depth=cols*rows;

	math::vector3d size(buffer->getWidth()/cols ,buffer->getHeight()/rows,depth);

	video::ITexturePtr generatedTexture=gTextureResourceManager.createTexture3D(tex->getResourceName()+"_3D",size,tex->getImageFormat());

	video::IHardwarePixelBuffer* surface=generatedTexture->getSurface(0);
	video::LockedPixelBox box=surface->lock(math::box3d(0,0,0,size.x,size.y,size.z),video::IHardwareBuffer::ELO_Discard);
	video::LockedPixelBox boxSrc=buffer->lock(math::box3d(0,0,0,buffer->getWidth(),buffer->getHeight(),1),video::IHardwareBuffer::ELO_ReadOnly);

	uchar*p = (uchar*)box.data;
	uchar*pSrc = (uchar*)boxSrc.data;
	int idx=0;
	int surfaceNumber=0;
	int pixels=0;

	int bpp=1;
	if(tex->getImageFormat()==video::EPixel_R8G8B8)
		bpp=1;
	else if(tex->getImageFormat()==video::EPixel_Float16_RGB)
		bpp=2;

	for(int i=0;i<rows;++i)
	{
		int y=i*size.y;
		for(int j=0;j<cols;++j)
		{
			int x=j*size.x;
			for(int y1=0;y1<size.y;++y1)
			{
				for(int x1=0;x1<size.x;++x1)
				{
					idx=(y+y1)*buffer->getWidth()+(x+x1);
					int idx2=(surfaceNumber*size.y+y1)*size.x+x1;
					for(int i=0;i<3*bpp;++i)
					{
						p[(idx2+i)*bpp]=pSrc[(idx+i)*bpp];
					}
					++pixels;
				}
			}
			++surfaceNumber;
		}
	}

	surface->unlock();
	//	buffer->unlock();

	return generatedTexture;

}


}
