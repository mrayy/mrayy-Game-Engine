

#include "stdafx.h"
#include "D3D9Device.h"
#include "DefaultColors.h"
#include "D3DHardwareBufferManager.h"
#include "SOptionElement.h"
#include "OptionContainer.h"
#include "D3DVideoMode.h"


namespace mray
{
namespace video
{


D3D9Device::D3D9Device():mrayDev()
{
	m_pDevice=0;
	m_activeDriver=0;
	m_hardwareBuffer=0;
	m_deviceLost=false;

	for(int i=0;i<MAX_LIGHTS;++i)
		m_lights[i]=0;

	for(int i=0;i<MAX_TEXTURE_STAGES;++i)
	{
		m_texStages[i].coordIndex=0;
		m_texStages[i].pTex=0;
		m_texStages[i].pVertexTex=0;
	}

	m_fsaaType=D3DMULTISAMPLE_NONE;
	m_fsaaQuality=0;
	m_currentLights=0;


	if((m_pD3D=Direct3DCreate9(D3D_SDK_VERSION))==0)
	{
		FATAL_ERROR(true,"Failed to create Direct3D9 Object!");
	}

	m_driverList.Enumerate(m_pD3D);
}
D3D9Device::~D3D9Device()
{
	shutdown();

	delete m_hardwareBuffer;

	SAFE_RELEASE(m_pD3D);
}

void D3D9Device::_initD3D()
{
}

void D3D9Device::getConfigParams(OptionContainer& params)
{

	SOptionElement& driversList= params[mT("Rendering Device")];
	SOptionElement& videoMode= params[mT("Resolution")];
	SOptionElement& fullScreen= params[mT("Fullscreen")];
	SOptionElement& colorDepth= params[mT("Bits")];
	SOptionElement& vsync= params[mT("VSync")];
	SOptionElement& FSAA= params[mT("Multisample")];
	
	driversList.valueSet.clear();

	D3DDriverList::iterator it= m_driverList.begin();
	for(;it!=m_driverList.end();++it)
	{
		driversList.valueSet.insert((*it)->DriverDescription());
		if(it==m_driverList.begin())
		{
			driversList.value=(*it)->DriverDescription();
			videoMode.valueSet.clear();

			const D3DVideoModeList& vmode=(*it)->GetVideoModes();

			for(int i=0;i<vmode.size();++i)
			{
				videoMode.valueSet.insert(vmode[i].GetDescription());
			}
		}
	}

	fullScreen.valueSet.insert(mT("True"));
	fullScreen.valueSet.insert(mT("False"));

	vsync.valueSet.insert(mT("True"));
	vsync.valueSet.insert(mT("False"));

	colorDepth.valueSet.insert(mT("16"));
	colorDepth.valueSet.insert(mT("24"));
	colorDepth.valueSet.insert(mT("32"));


	FSAA.valueSet.insert(mT("0"));
	FSAA.valueSet.insert(mT("1"));
	FSAA.valueSet.insert(mT("2"));
	FSAA.valueSet.insert(mT("4"));
	FSAA.valueSet.insert(mT("8"));
	FSAA.valueSet.insert(mT("16"));
}

const core::string& D3D9Device::getDeviceType()
{
	return m_deviceType;
}

DeviceCapabilites* D3D9Device::getCapabilities()
{
	return 0;
}


const video::SColor &D3D9Device::getClearColor()
{
	return video::DefaultColors::Blue;
}
math::Point2di D3D9Device::getRenderTargetSize()
{
	return 0;
}


void D3D9Device::enableFog(bool e)
{
}

bool D3D9Device::isFogEnabled()
{
	return false;
}


void D3D9Device::shutdown()
{
}
void D3D9Device::resizeWindow(int w,int h)
{
}
void D3D9Device::setTransformationState(ETransformationState state,const math::matrix4x4&mat)
{
}
void D3D9Device::getTransformationState(ETransformationState state,math::matrix4x4&out)
{
}


void D3D9Device::begin(bool clearColorBuf,bool clearDepthBuf)
{
}
void D3D9Device::end()
{
}

bool D3D9Device::setRenderState(RenderStates flag,bool set)
{
	return false;
}
bool D3D9Device::getRenderState(RenderStates flag) 
{
	return false;
}


ITexturePtr D3D9Device::createTexture1D(int size,video::EPixelFormat type,bool internal)
{
	return 0;
}
ITexturePtr D3D9Device::createTexture2D(const math::Point2di& size,video::EPixelFormat type,bool internal)
{
	return 0;
}
ITexturePtr D3D9Device::createTexture3D(const math::Point3di& size,video::EPixelFormat type,bool internal)
{
	return 0;
}
ITexturePtr D3D9Device::createTextureCube(const math::Point2di& size,video::EPixelFormat type,bool internal)
{
	return 0;
}

ITexturePtr D3D9Device::createEmptyTexture1D(bool internal)
{
	return 0;
}
ITexturePtr D3D9Device::createEmptyTexture2D(bool internal)
{
	return 0;
}
ITexturePtr D3D9Device::createEmptyTexture3D(bool internal)
{
	return 0;
}
ITexturePtr D3D9Device::createEmptyTextureCube(bool internal)
{
	return 0;
}

void D3D9Device::set2DMode()
{
}

void D3D9Device::set3DMode()
{
}

void D3D9Device::setViewportRect(const math::recti&rc)
{
}

math::recti D3D9Device::getViewportRect()
{
	return math::recti(0,0,0,0);
}

void D3D9Device::setLineWidth(float lineWidth)
{
}
void D3D9Device::setPointSize(float size)
{
}


void D3D9Device::draw2DRectangleRot(const math::rect<float> &rc,const math::vector2d &origin,const video::SColor &color,float angle,bool fill)
{
}
void D3D9Device::draw2DRectangle(const math::rect<float> &rc,const video::SColor &color,bool fill,const math::rect<float>*clip)
{
}
void D3D9Device::draw2DRectangle(const math::rect<float> &rc,const video::SColor &ul,const video::SColor &ur,const video::SColor &br,const video::SColor &bl,bool fill,const math::rect<float>*clip)
{
}

void D3D9Device::draw3DLine(const math::Point3df &start,const math::Point3df &end,const video::SColor& color)
{
}
void D3D9Device::draw2DLine(const math::Point2df &start,const math::Point2df &end,const video::SColor& color)
{
}

void D3D9Device::draw2DLine(const math::Point2df*points,int n,const video::SColor& color)
{
}


void D3D9Device::draw2DImage(const math::rectf &pos,const video::SColor colors[4],const math::rectf*clip,
							 const math::rectf *coord)
{
}

void D3D9Device::draw2DImage(const math::rectf &pos,const video::SColor &clr,const math::rectf*clip,
							 const math::rectf *coord)
{
}

void D3D9Device::draw2DImageRot(const math::rectf &pos,const math::vector2d &origin,const video::SColor &clr,float angle,
								const  math::rectf *coord)
{
}


void D3D9Device::draw2DImage(const math::vector2d &pos,const math::vector2d &size,float spacing,const std::vector<math::rectf>&rects,
							 const std::vector<int>&indices,const video::SColor &clr,const math::rectf*clip)
{
}

void D3D9Device::draw2DImage(const math::vector2d &pos,const math::vector2d &sizeFactor,float spacing,const std::vector<math::rectf>&rects,
							 const std::vector<math::vector2d>&rectsSize,const std::vector<int>&indices,const video::SColor &clr,const math::rectf*clip)
{
}

void D3D9Device::useMaterial(video::SMaterial* mtrl,bool reset)
{
}
bool D3D9Device::useTexture(int stage,video::TextureUnit*tex)
{
	return 0;
}
void D3D9Device::disableTexture(int stage)
{
}

bool D3D9Device::useShader(const GPUShaderPtr& shader)
{
	return 0;
}
void D3D9Device::unuseShader()
{
}
const GPUShaderPtr& D3D9Device::getActiveShader()
{
	return 0;
}

void D3D9Device::drawVertex(const math::vector3d &point,const video::SColor &color,float size)
{
}

void D3D9Device::drawLine(const math::Point3df*points,int n,const video::SColor &color)
{
}


//! draw mesh and it's sub mesh buffers
void D3D9Device::drawMesh(scene::SMesh* mesh)
{
}
//! draw mesh buffer 
void D3D9Device::drawSingleMesh(scene::IMeshBuffer* mesh)
{
}

//! draw mesh buffer -the other method-
void D3D9Device::drawBuffer(video::StreamsData*streams ,IndexData* indices,video::EMeshRenderType rType)
{
}




//! add dynamic light
void D3D9Device::addDynamicLight(scene::LightNode*light)
{
}
void D3D9Device::disableAllLights()
{
}

int D3D9Device::getDynamicLightsCount()
{
	return 0;
}
scene::LightNode* D3D9Device::getDynamicLight(int index)
{
	return 0;
}

//! set the global ambient color
void D3D9Device::setAmbientColor(const video::SColor& color)
{
}


//! clear one of back buffers (color,depth,..)
void D3D9Device::clearBuffer(EDeviceBuffer buff)
{
}

IRenderTargetPtr D3D9Device::createRenderTarget(const core::string&name,const ITexturePtr& target,
												const IHardwarePixelBufferPtr&  depthBuffer,const IHardwarePixelBufferPtr&   stencilBuffer,bool depthOnly)
{
	return 0;
}

bool D3D9Device::setRenderTarget(IRenderTargetPtr rt,bool clearBackBuffer,bool clearZBuffer,const video::SColor &clearColor)
{
	return 0;
}
// create Multi Render Target
IRenderTargetPtr D3D9Device::createMRenderTarget(const core::string&name,const math::Point2di& dim,
												 EPixelFormat format,int buffers,bool shareContexts,bool shareObjects)
{
	return 0;
}


void D3D9Device::drawStencilShadowVolume(math::vector3d *vertices,int vtxCnt,bool zfail)
{
}

void D3D9Device::drawStencilShadow(bool clearStencilBuffer, const video::SColor &shadowColor)
{
}


float D3D9Device::getPixelDepth(int x,int y)
{
	return 0;
}
void D3D9Device::getPixels(const math::rect<int> &rc,std::vector<SColor> &outPixels)
{
}

void D3D9Device::loadXMLSettings(xml::XMLElement* elem)
{
}
xml::XMLElement*  D3D9Device::exportXMLSettings(xml::XMLElement* elem)
{
	return 0;
}

///////

void D3D9Device::enableStream(EMeshStreamType stream,bool e)
{
}
bool D3D9Device::isStreamEnabled(EMeshStreamType stream)
{
	return 0;
}

void D3D9Device::setStreamMap(video::StreamsData*streams)
{
}
void D3D9Device::setStream(video::IHardwareStreamBuffer* stream)
{
}
void D3D9Device::endStreams()
{
}

void D3D9Device::drawElements(video::IndexData* ib,video::EMeshRenderType rtype)
{
}
void D3D9Device::drawElementsUP(const void*indices,IHardwareIndexBuffer::EIndexType type,uint startIndex,int count,video::EMeshRenderType rtype)
{
}

void D3D9Device::setDepthBias(float linearBias,float slopeBias)
{
}


//////////////////////////////////////////////////////////////////////////
mrayD3DDeviceFactory::mrayD3DDeviceFactory()
{
}
mrayD3DDeviceFactory::~mrayD3DDeviceFactory()
{
}
GCPtr<IVideoDevice> mrayD3DDeviceFactory::create()
{
	return new D3D9Device();
}

bool mrayD3DDeviceFactory::canCreate(const core::string& type){
	core::string s(type);
	return s.equals_ignore_case(getType());
}
/*
bool mrayGLDeviceFactory::canCreate(EDeviceType type){
return type==getDeviceType();
}*/

core::string mrayD3DDeviceFactory::getType(){
	return D3D9Device::m_deviceType;
}
/*EDeviceType mrayGLDeviceFactory::getDeviceType(){
return m_deviceType;
}*/
core::string mrayD3DDeviceFactory::getDescription(){
	return mT("D3D9 Device Renderer");
}

void mrayD3DDeviceFactory::getConfigParams(OptionContainer& params)
{
	D3D9Device*dev=new D3D9Device();

	dev->getConfigParams(params);

	delete dev;
}

}
}

