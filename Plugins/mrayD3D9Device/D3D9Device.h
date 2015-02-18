
/********************************************************************
	created:	2011/11/20
	created:	20:11:2011   15:50
	filename: 	d:\Development\mrayEngine\Engine\mrayD3D9Device\D3D9Device.h
	file path:	d:\Development\mrayEngine\Engine\mrayD3D9Device
	file base:	D3D9Device
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __D3D9Device__
#define __D3D9Device__



#include "compileConfig.h"
#include <mrayDev.h>

#include "D3DDriverList.h"
#include <IDeviceFactory.h>

namespace mray
{
namespace video
{

	class D3DHardwareBufferManager;

class MRAY_D3D9_DLL D3D9Device:public mrayDev
{
public:
	static const core::string m_deviceType;

	static const int MAX_LIGHTS=8;
	static const int MAX_TEXTURE_STAGES=16;
protected:

	struct D3DTextureStageInfo
	{
		uint coordIndex;

		IDirect3DTexture9 *pTex;
		IDirect3DBaseTexture9* pVertexTex;
	};

	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pDevice;

	D3DMULTISAMPLE_TYPE m_fsaaType;
	DWORD m_fsaaQuality;

	HINSTANCE m_hInst;

	D3DDriverList m_driverList;
	D3D9Driver* m_activeDriver;
	
	D3DHardwareBufferManager* m_hardwareBuffer;

	bool m_deviceLost;

	scene::LightNode* m_lights[MAX_LIGHTS];
	D3DTextureStageInfo m_texStages[MAX_TEXTURE_STAGES];
	uint m_currentLights;

	FogParameters m_fogParams;

	void _initD3D();
public:
	D3D9Device();
	~D3D9Device();

	bool IsDeviceLost();
	void RestoreLostDevice();

	void _notifyDeviceLost();

	void getConfigParams(OptionContainer& params);

	//////////////////////////////////////////////////////////////////////////
	RenderWindow* CreateRenderWindow(const core::string&name,const math::vector2di& size,bool fullScreen,
		const OptionContainer&params,ulong hWND);

	void InitializeContext(RenderWindow* primary);


	const core::string& getDeviceType();

	DeviceCapabilites* getCapabilities();

	virtual void setMultiSampling(bool enable);
	virtual bool getMultiSampling();

	const video::SColor &getClearColor();
	math::Point2di getRenderTargetSize();



	void setFogParameters(const FogParameters& params);
	const FogParameters& getFogParameters(const FogParameters& params);

	void enableFog(bool e);

	bool isFogEnabled();


	void shutdown();
	void resizeWindow(int w,int h);
	virtual void setTransformationState(ETransformationState state,const math::matrix4x4&mat);
	void getTransformationState(ETransformationState state,math::matrix4x4&out);


	void begin(bool clearColorBuf,bool clearDepthBuf);
	void end();

	bool setRenderState(RenderStates flag,bool set);
	bool getRenderState(RenderStates flag) ;


	ITexturePtr createTexture1D(int size,video::EPixelFormat type,bool internal);
	ITexturePtr createTexture2D(const math::Point2di& size,video::EPixelFormat type,bool internal);
	ITexturePtr createTexture3D(const math::Point3di& size,video::EPixelFormat type,bool internal);
	ITexturePtr createTextureCube(const math::Point2di& size,video::EPixelFormat type,bool internal);

	ITexturePtr createEmptyTexture1D(bool internal);
	ITexturePtr createEmptyTexture2D(bool internal);
	ITexturePtr createEmptyTexture3D(bool internal);
	ITexturePtr createEmptyTextureCube(bool internal);

	//! change view matrix to ortho view
	void set2DMode();
	//! change view matrix to perspective view
	void set3DMode();

	//! set 3D viewport size
	void setViewportRect(const math::recti&rc);
	//! get 3D viewport size
	math::recti getViewportRect();

	void setLineWidth(float lineWidth);
	void setPointSize(float size);


	void draw2DRectangleRot(const math::rect<float> &rc,const math::vector2d &origin,const video::SColor &color,float angle,bool fill);
	void draw2DRectangle(const math::rect<float> &rc,const video::SColor &color,bool fill,const math::rect<float>*clip);
	void draw2DRectangle(const math::rect<float> &rc,const video::SColor &ul,const video::SColor &ur,const video::SColor &br,const video::SColor &bl,bool fill,const math::rect<float>*clip);

	void draw3DLine(const math::Point3df &start,const math::Point3df &end,const video::SColor& color);
	void draw2DLine(const math::Point2df &start,const math::Point2df &end,const video::SColor& color);

	void draw2DLine(const math::Point2df*points,int n,const video::SColor& color);
	virtual void draw2DShape(const math::Point2df*points,int n,const video::SColor& color,bool fill);
	virtual void draw2DShape(const math::Point2df*points,const video::SColor*clr,int n,bool fill);
	virtual void draw2DShapeTextured(const math::Point2df*points,const math::Point2df*texCoord,int n,const video::SColor&clr,bool fill);

	void draw2DImage(const math::rectf &pos,const video::SColor colors[4],const math::rectf*clip,
		const math::rectf *coord);

	//! draw 2d image
	//! \param pos:position and size of image
	//! \param texture:image texture
	//! \param clr:global image color
	//! \param clip:clipping math::rectangle
	void draw2DImage(const math::rectf &pos,const video::SColor &clr,const math::rectf*clip,
		const math::rectf *coord);

	void draw2DImageRot(const math::rectf &pos,const math::vector2d &origin,const video::SColor &clr,float angle,
		const  math::rectf *coord);

	//! draw 2d image
	//! \param pos:position of image
	//! \param texture:image texture
	//! \param math::rects:std::vector of math::rects to draw from this texture
	//! \param indices:math::rects indices to draw
	//! \param clr:global image color
	//! \param clip:clipping math::rectangle

	virtual void draw2DImage(const math::vector2d &pos,const math::vector2d &size,float spacing/*,video::TextureUnit* texture*/,const std::vector<math::rectf>&rects,
		const std::vector<int>&indices,const video::SColor &clr,const math::rectf*clip=0);

	virtual void draw2DImage(const math::vector2d &pos,const math::vector2d &sizeFactor,float spacing/*,video::TextureUnit* texture*/,const std::vector<math::rectf>&rects,
		const std::vector<math::vector2d>&rectsSize,const std::vector<int>&indices,const video::SColor &clr,const math::rectf*clip=0);




	//! use material
	//! \param mtrl:material to use
	//! \param reset: reset material stage to this material 
	void useMaterial(video::SMaterial* mtrl,bool reset);
	bool useTexture(int stage,video::TextureUnit*tex);
	void disableTexture(int stage);

	bool useShader(const GPUShaderPtr& shader);
	void unuseShader();
	const GPUShaderPtr& getActiveShader();

	//! draw 3d vertex
	//! \param point:vertex position
	//! \param color:vertex color
	//! \param size:vertex size

	void drawVertex(const math::vector3d &point,const video::SColor &color,float size);

	//! draw multiple lines
	//! \param points:std::vector of points
	//! \param n:points count
	//! \param color:lines color
	void drawLine(const math::Point3df*points,int n,const video::SColor &color);


	//! draw mesh and it's sub mesh buffers
	void drawMesh(scene::SMesh* mesh);
	//! draw mesh buffer 
	void drawSingleMesh(scene::IMeshBuffer* mesh);

	//! draw mesh buffer -the other method-
	void drawBuffer(video::StreamsData*streams ,IndexData* indices,video::EMeshRenderType rType);

	//! add dynamic light
	void addDynamicLight(scene::LightNode*light);
	void disableAllLights();

	int getDynamicLightsCount();
	scene::LightNode* getDynamicLight(int index);

	//! set the global ambient color
	void setAmbientColor(const video::SColor& color);


	//! clear one of back buffers (color,depth,..)
	void clearBuffer(EDeviceBuffer buff);

	IRenderTargetPtr createRenderTarget(const core::string&name,const ITexturePtr& target,
		const IHardwarePixelBufferPtr&  depthBuffer,const IHardwarePixelBufferPtr&   stencilBuffer,bool depthOnly);

	bool setRenderTarget(IRenderTargetPtr rt,bool clearBackBuffer,bool clearZBuffer,const video::SColor &clearColor);

	// create Multi Render Target
	IRenderTargetPtr createMRenderTarget(const core::string&name,const math::Point2di& dim,
		EPixelFormat format,int buffers,bool shareContexts,bool shareObjects);


	void drawStencilShadowVolume(math::vector3d *vertices,int vtxCnt,bool zfail);

	void drawStencilShadow(bool clearStencilBuffer, const video::SColor &shadowColor);


	float getPixelDepth(int x,int y);
	void getPixels(const math::rect<int> &rc,std::vector<SColor> &outPixels);

	void loadXMLSettings(xml::XMLElement* elem);
	xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);

	///////

	void enableStream(EMeshStreamType stream,bool e);
	bool isStreamEnabled(EMeshStreamType stream);

	void setStreamMap(video::StreamsData*streams);
	void setStream(video::IHardwareStreamBuffer* stream);
	void endStreams();

	void drawElements(video::IndexData* ib,video::EMeshRenderType rtype);
	void drawElementsUP(const void*indices,IHardwareIndexBuffer::EIndexType type,uint startIndex,int count,video::EMeshRenderType rtype);

	void setDepthBias(float linearBias,float slopeBias);

};

class mrayD3DDeviceFactory:public IDeviceFactoryCreater
{
public:

	mrayD3DDeviceFactory();
	virtual~mrayD3DDeviceFactory();

	GCPtr<IVideoDevice> create();

	bool canCreate(const core::string&type);

	core::string getType();
	//EDeviceType getDeviceType();
	core::string getDescription();

	void getConfigParams(OptionContainer& params);
};

}
}

#endif