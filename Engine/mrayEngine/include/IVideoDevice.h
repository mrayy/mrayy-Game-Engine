


/********************************************************************
	created:	2012/01/16
	created:	16:1:2012   14:40
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IVideoDevice.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IVideoDevice
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IVideoDevice__
#define __IVideoDevice__

#include "videoCommon.h"
#include "IRenderTarget.h"
#include "ListenerContainer.h"
#include "ITexture.h"
#include "IHardwareIndexBuffer.h"
//#include "OptionContainer.h"
#include "IDeviceListener.h"

namespace mray
{
	class OptionContainer;
	namespace scene{
		class S3DVertex;

		class IMeshBuffer;
		class SMesh;
		class LightNode;

		enum EVertexCoordType;
		class ViewPort;
	}
namespace video
{
	class SColor;
	class StreamsData;
	class IndexData;
	class IHardwareStreamBuffer;
	class DeviceCapabilites;
	class RenderWindow;
	class RenderPass;
	class IGPUShaderProgram;
	class GPUShader;

class IVideoDevice:public ListenerContainer<IDeviceListener*>
{
private:
protected:
	DECLARE_FIRE_METHOD(OnPreRender,(IVideoDevice* device),(device));
	DECLARE_FIRE_METHOD(OnPostRender,(IVideoDevice* device),(device));
	DECLARE_FIRE_METHOD(OnMaterialUsed,(IVideoDevice* device,RenderPass* v),(device,v));
	DECLARE_FIRE_METHOD(OnTextureUsed,(IVideoDevice* device,TextureUnit* v),(device,v));
	DECLARE_FIRE_METHOD(OnShaderUsed,(IVideoDevice* device,IGPUShaderProgram* v),(device,v));
	DECLARE_FIRE_METHOD(OnDeviceShutdown,(IVideoDevice* device),(device));
public:
	IVideoDevice(){}
	virtual~IVideoDevice(){}

	virtual const core::string& getDeviceType()=0;

	virtual DeviceCapabilites* getCapabilities()=0;

	virtual RenderWindow* CreateRenderWindow(const core::string&name,const math::vector2di& size,bool fullScreen,
		const OptionContainer&params,ulong hWND)=0;

	//! set the initial color
	virtual void setClearColor(const video::SColor &color)=0;
	virtual const video::SColor &getClearColor()=0;
	virtual math::Point2di getRenderTargetSize()=0;


	virtual void setFogParameters(const FogParameters& params)=0;
	virtual const FogParameters& getFogParameters(const FogParameters& params)=0;
	//! enable/disable fog
	virtual void enableFog(bool e)=0;

	virtual bool isFogEnabled()=0;

	virtual void setMultiSampling(bool enable)=0;
	virtual bool getMultiSampling()=0;

	//! shutdown the engine
	virtual void shutdown()=0;

	//! set matrix to certian matrix state
	/*!
	\param transormation state enum 
	\param matrix to set
	\sa ETransformationState
	*/
	virtual void setTransformationState(ETransformationState state,const math::matrix4x4&mat)=0;
	//! return certian matrix state to the out matrix
	virtual void getTransformationState(ETransformationState state,math::matrix4x4&out)=0;


	/*!
	return the number of primitives been drawn
	*/
	virtual int getPrimitiveDrawnCount()=0;
	virtual int getBatchDrawnCount()=0;

	/*!
	begin the scene
	*/
	virtual void begin(bool clearColorBuf=true,bool clearDepthBuf=true)=0;
	/*!
	end the scene
	*/
	virtual void end()=0;


	virtual bool setRenderState(RenderStates flag,bool set)=0;
	virtual bool getRenderState(RenderStates flag) =0;


	virtual void SetClipPlane(int i,const math::Plane&p)=0;
	virtual void AddClipPlane(const math::Plane&p)=0;
	virtual void RemoveClipPlane(int i)=0;
	virtual void ClearClipPlanes()=0;


	virtual ITexturePtr createTexture1D(int size,video::EPixelFormat type,bool internal)=0;
	virtual ITexturePtr createTexture2D(const math::Point2di& size,video::EPixelFormat type,bool internal)=0;
	virtual ITexturePtr createTexture3D(const math::Point3di& size,video::EPixelFormat type,bool internal)=0;
	virtual ITexturePtr createTextureCube(const math::Point2di& size,video::EPixelFormat type,bool internal)=0;

	virtual ITexturePtr createEmptyTexture1D(bool internal)=0;
	virtual ITexturePtr createEmptyTexture2D(bool internal)=0;
	virtual ITexturePtr createEmptyTexture3D(bool internal)=0;
	virtual ITexturePtr createEmptyTextureCube(bool internal)=0;

	//! change view matrix to ortho view
	virtual void set2DMode()=0;
	//! change view matrix to perspective view
	virtual void set3DMode()=0;

	//! set viewport 
	virtual void setViewport(scene::ViewPort* vp)=0;
	//! get viewport 
	virtual scene::ViewPort* getViewport()=0;

	virtual void setScissorTest(bool enable)=0;
	virtual bool getScissorTest()=0;
	virtual void setScissorRect(const math::rectf& rc)=0;
	virtual const math::rectf& getScissorRect()=0;

	virtual void setLineWidth(float lineWidth)=0;
	virtual void setPointSize(float size)=0;


	virtual void draw2DRectangleRot(const math::rect<float> &rc,const math::vector2d &origin,const video::SColor &color,float angle,bool fill=1)=0;
	virtual void draw2DRectangle(const math::rect<float> &rc,const video::SColor &color,bool fill=1,const math::rect<float>*clip=0)=0;
	virtual void draw2DRectangle(const math::rect<float> &rc,const video::SColor &ul,const video::SColor &ur,const video::SColor &br,const video::SColor &bl,bool fill=1,const math::rect<float>*clip=0)=0;

	virtual void draw3DLine(const math::Point3df &start,const math::Point3df &end,const video::SColor& color)=0;
	virtual void draw2DLine(const math::Point2df &start,const math::Point2df &end,const video::SColor& color)=0;

	virtual void draw2DLine(const math::Point2df*points,int n,const video::SColor& color)=0;
	virtual void draw2DShape(const math::Point2df*points,int n,const video::SColor& color,bool fill)=0;
	virtual void draw2DShape(const math::Point2df*points,const video::SColor*clr,int n,bool fill)=0;
	virtual void draw2DShapeTextured(const math::Point2df*points,const math::Point2df*texCoord,int n,const video::SColor&clr,bool fill)=0;

	//draw the image based on the last texture used
	virtual void draw2DImage(const math::rectf &pos/*,video::TextureUnit* texture*/,const video::SColor colors[4],const math::rectf*clip=0,
		const math::rectf *coord=0)=0;

	//! draw 2d image
	//! \param pos:position and size of image
	//! \param texture:image texture //removed
	//! \param clr:global image color
	//! \param clip:clipping math::rectangle
	virtual void draw2DImage(const math::rectf &pos/*,video::TextureUnit* texture*/,const video::SColor &clr,const math::rectf*clip=0,
		const math::rectf *coord=0)=0;

	virtual void draw2DImageRot(const math::rectf &pos,const math::vector2d &origin/*,video::TextureUnit* texture*/,const video::SColor &clr,float angle,
		const  math::rectf *coord=0)=0;

	//! draw 2d image
	//! \param pos:position of image
	//! \param texture:image texture
	//! \param math::rects:std::vector of math::rects to draw from this texture
	//! \param indices:math::rects indices to draw
	//! \param clr:global image color
	//! \param clip:clipping math::rectangle

	virtual void draw2DImage(const math::vector2d &pos,const math::vector2d &size,float spacing/*,video::TextureUnit* texture*/,const std::vector<math::rectf>&rects,
		const std::vector<int>&indices,const video::SColor &clr,const math::rectf*clip=0)=0;

	virtual void draw2DImage(const math::vector2d &pos,const math::vector2d &sizeFactor,float spacing/*,video::TextureUnit* texture*/,const std::vector<math::rectf>&rects,
		const std::vector<math::vector2d>&rectsSize,const std::vector<int>&indices,const video::SColor &clr,const math::rectf*clip=0)=0;


	//! use Pass
	//! \param pass:pass to use
	virtual void useRenderPass(video::RenderPass* p)=0;
	virtual bool useTexture(int stage,video::TextureUnit*tex)=0;
	virtual void disableTexture(int stage)=0;

	virtual bool useShader(GPUShader* shader)=0;
	virtual bool setVertexShader(IGPUShaderProgram* shader)=0;
	virtual bool setFragmentShader(IGPUShaderProgram* shader)=0;
	virtual bool setGeometryShader(IGPUShaderProgram* shader)=0;
	virtual IGPUShaderProgram* getVertexShader()=0;
	virtual IGPUShaderProgram* getFragmentShader()=0;
	virtual IGPUShaderProgram* getGeometryShader()=0;
	virtual void unuseShader()=0;

	//! draw 3d vertex
	//! \param point:vertex position
	//! \param color:vertex color
	//! \param size:vertex size

	virtual void drawVertex(const math::Point3df*points,int n,const video::SColor& color)=0;

	//! draw multiple lines
	//! \param points:std::vector of points
	//! \param n:points count
	//! \param color:lines color
	virtual void drawLine(const math::Point3df*points,int n,const video::SColor &color)=0;

	//! draw mesh buffer 
	virtual void drawSingleMesh(scene::IMeshBuffer* mesh)=0;

	//! draw mesh buffer -the other method-
	virtual void drawBuffer(video::StreamsData*streams ,IndexData* indices,video::EMeshRenderType rType)=0;

	//! add dynamic light
	virtual void addDynamicLight(scene::LightNode*light)=0;
	virtual void disableAllLights()=0;

	virtual int getDynamicLightsCount()=0;
	virtual scene::LightNode* getDynamicLight(int index)=0;

	//! set the global ambient color
	virtual void setAmbientColor(const video::SColor&color)=0;


	//! clear one of back buffers (color,depth,..)
	virtual void clearBuffer(EDeviceBuffer buff)=0;

	virtual IRenderTargetPtr createRenderTarget(const core::string&name,const ITexturePtr& target,
		const IHardwarePixelBufferPtr&  depthBuffer,const IHardwarePixelBufferPtr&   stencilBuffer,bool depthOnly)=0;
	
	virtual bool setRenderTarget(video::IRenderTargetPtr rt,bool clearBackBuffer=true,bool clearZBuffer=true,const video::SColor& clearColor=video::SColor(0,0,0,0),const math::recti* vpRect=0)=0;
	virtual video::IRenderTargetPtr getRenderTarget()=0;

	// create Multi Render Target
	virtual IRenderTargetPtr createMRenderTarget(const core::string&name,const math::Point2di& dim,
		EPixelFormat format,int buffers,bool shareContexts,bool shareObjects)=0;


	virtual void drawStencilShadowVolume(math::vector3d *vertices,int vtxCnt,bool zfail)=0;

	virtual void drawStencilShadow(bool clearStencilBuffer, const video::SColor &shadowColor)=0;

	virtual void SetSceneBlending(EBlendFactor srcFactor, EBlendFactor dstFactor)=0;
	virtual void SetSeparateSceneBlending(EBlendFactor srcFactor, EBlendFactor dstFactor, EBlendFactor srcAlphaFactor, EBlendFactor dstAlphaFactor) = 0;

	virtual float getPixelDepth(int x,int y)=0;
	virtual void getPixels(const math::rect<int> &rc,std::vector<SColor> &outPixels)=0;

	///////

	virtual void enableStream(EMeshStreamType stream,bool e)=0;
	virtual bool isStreamEnabled(EMeshStreamType stream)=0;

	virtual void setStreamMap(video::StreamsData*streams)=0;
	virtual void setStream(video::IHardwareStreamBuffer* stream)=0;
	virtual void endStreams()=0;

	virtual void drawElements(video::IndexData* ib,video::EMeshRenderType rtype)=0;
	virtual void drawElementsUP(const void*indices,IHardwareIndexBuffer::EIndexType type,uint startIndex,int count,video::EMeshRenderType rtype)=0;

	virtual void setDepthBias(float linearBias,float slopeBias)=0;
};

MakeSharedPtrType(IVideoDevice);

}
}

#endif
