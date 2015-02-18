#ifndef ___GLDEV___
#define ___GLDEV___


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include <mrayDev.h>
#include <RenderMaterial.h>
#include <IDeviceFactory.h>
#include <MultiThreadListener.h>
#include "GLTexture.h"
#include "GLHardwareBufferManager.h"
#include "GLContext.h"

#include "DeviceCapabilites.h"
#include "hash_map"

#include <gl\gl.h>

namespace mray{
namespace scene{
	class SMesh;
	class LightNode;
}

namespace video{
	class ITexture;

	class CShaderFactory;
	class GLPBuffer;
	class GLPBufferTexture;

	class GLSupport;

/*!
   Engine main class
*/
class MRAY_GL_DLL GLDev:public mrayDev
{
public:
	static const core::string m_deviceType;
protected:

	stdext::hash_map<GLenum,bool> m_states;

	int lastLightAdded;

	EState m_DefaultRenderStates[RS_RenderStates_Count];

	GCPtr<OS::MultiThreadListener> m_threadListener;

//	std::list<video::ITexturePtr > Textures;

	GLPBuffer* m_currentPBuffer;
	GLPBufferTexture* m_currentPBufferTexture;

	GLenum m_lastTextureType;

	FogParameters m_fogParams;

	bool fogEnable;


	GCPtr<IHardwareBufferManager> m_hardwareBuffManager;

	std::vector<int> m_TextureID;

	math::rectf m_viewportRect;
	/*
	bool m_rtViewPort;//Render Target Viewport is set?
	*/

	std::vector<uint> m_useAttributes;

	GLContext* m_activeContext;
	GLContext* m_mainContext;

	std::list<GLContext*> m_contexts;

	std::vector<scene::LightNode*> dynamicLights;

	std::vector<byte> colorArrayBuffer;

	math::matrix4x4 Matrices[TS_COUNT];

	math::matrix4x4 m_3DProjMat;
	math::matrix4x4 m_3DViewMat;


	void createGLMatrix(float mat[16],const math::matrix4x4&m);

	bool m_vertexPointerSet;

	SColor m_clearColor;

	GCPtr<GPUShader> m_depthShader;

	GLSupport* m_support;

	GCPtr<DeviceCapabilites> m_caps;

	video::IRenderTargetPtr  m_renderTarget;
	//math::Point2di currentRendertargetSize;

	float m_lineWidth;
	float m_pointSize;

	bool m_glInited;

	std::vector<int> m_activeTexture;

	bool m_enableScissorRect;
	math::rectf m_scissorRect;

	GLenum m_activeMatrixMode;
	GLenum m_alphaFunc;
	GLclampf m_alphaFuncClamp; 
	GLenum m_frontFace;


protected:
	void loadExtensions();
	void _onUpdateViewport();
	void _oneTimeInit();
	void _initGLCaps();
public:
	GLDev();
	virtual~GLDev();

	const core::string& getDeviceType();
	DeviceCapabilites* getCapabilities();

	RenderWindow* CreateRenderWindow(const core::string&name,const math::vector2di& size,bool fullScreen,
		const OptionContainer&params,ulong hWND);

	virtual void InitializeContext(RenderWindow* primary);

	void setCurrentPBuffer(GLPBuffer*b);
	GLPBuffer* getCurrentPBuffer();
	void setCurrentPBufferTexture(GLPBufferTexture*b);
	GLPBufferTexture* getCurrentPBufferTexture();

	//! set the initial color
	void setClearColor(const video::SColor &color);
	const video::SColor &getClearColor();

	math::Point2di getRenderTargetSize();

	bool  setRenderState(RenderStates flag,bool set);
	bool getRenderState(RenderStates flag);

	void setFogParameters(const FogParameters& params);
	const FogParameters& getFogParameters(const FogParameters& params);

	//! enable/disable fog
	void enableFog(bool e);
	bool isFogEnabled();
	

	void setMultiSampling(bool enable);
	bool getMultiSampling();

	//! shutdown the engine
	void shutdown();

	//! set matrix to certian matrix state
	/*!
	  \param transormation state enum 
	  \param matrix to set
	  \sa ETransformationState
	*/
	void setTransformationState(ETransformationState state,const math::matrix4x4&mat);
	//! return certian matrix state to the out matrix
	void getTransformationState(ETransformationState state,math::matrix4x4&out);

	/*!
	    begin the scene
	  */
	void begin(bool clearColorBuf=true,bool clearDepthBuf=true);
	/*!
	    end the scene
	  */
	void end();

	video::ITexturePtr createTexture1D(int size,video::EPixelFormat type,bool internal);
	video::ITexturePtr createTexture2D(const math::Point2di &size,video::EPixelFormat type,bool internal);
	video::ITexturePtr createTexture3D(const math::Point3di &size,video::EPixelFormat type,bool internal);
	video::ITexturePtr createTextureCube(const math::Point2di &size,video::EPixelFormat type,bool internal);

	video::ITexturePtr createEmptyTexture1D(bool internal);
	video::ITexturePtr createEmptyTexture2D(bool internal);
	video::ITexturePtr createEmptyTexture3D(bool internal);
	video::ITexturePtr createEmptyTextureCube(bool internal);

	//! change view matrix to ortho view
	void set2DMode();
	//! change view matrix to perspective view
	void set3DMode();

	//! set  viewport size
	void setViewport(scene::ViewPort* vp);
	void setViewportRect(const math::rectf& rc);
	const math::rectf&  getViewportRect();

	void setScissorTest(bool enable);
	bool getScissorTest();
	void setScissorRect(const math::rectf& rc);
	const math::rectf& getScissorRect();

	void setLineWidth(float lineWidth);
	void setPointSize(float size);

	void draw2DRectangleRot(const math::rect<float> &rc,const math::vector2d &origin,const video::SColor& color,float angle,bool fill=1);
	void draw2DRectangle(const math::rect<float> &rc,const video::SColor& color,bool fill=1,const math::rect<float>*clip=0);
	void draw2DRectangle(const math::rect<float> &rc,const video::SColor &ul,const video::SColor &ur,const video::SColor &br,const video::SColor &bl,bool fill=1,const math::rect<float>*clip=0);
	
	void draw3DLine(const math::Point3df &start,const math::Point3df &end,const video::SColor& color);
	void draw2DLine(const math::Point2df &start,const math::Point2df &end,const video::SColor& color);
	
	void draw2DLine(const math::Point2df*points,int n,const video::SColor& color);
	void draw2DShape(const math::Point2df*points,int n,const video::SColor& color,bool fill);
	void draw2DShape(const math::Point2df*points,const video::SColor*clr,int n,bool fill);
	void draw2DShapeTextured(const math::Point2df*points,const math::Point2df*texCoord,int n,const video::SColor&clr,bool fill);

	
	void draw2DImage(const math::rectf &pos,const video::SColor colors[4],const math::rectf*clip=0,
		 const math::rectf *coord=0);

	//! draw 2d image
	//! \param pos:position and size of image
	//! \param texture:image texture
	//! \param clr:global image color
	//! \param clip:clipping math::rectangle
	void draw2DImage(const math::rectf &pos/*,video::TextureUnit* texture*/,const video::SColor &clr,const math::rectf*clip=0,
		 const math::rectf *coord=0);
	
	void draw2DImageRot(const math::rectf &pos,const math::vector2d &origin/*,video::TextureUnit* texture*/,const video::SColor &clr,float angle,
		 const math::rectf *coord=0);
	
	//! draw 2d image
	//! \param pos:position of image
	//! \param texture:image texture
	//! \param math::rects:std::vector of math::rects to draw from this texture
	//! \param indices:math::rects indices to draw
	//! \param clr:global image color
	//! \param clip:clipping math::rectangle

	void draw2DImage(const math::vector2d& pos,const math::vector2d& size,float spacing/*,video::TextureUnit* texture*/,const std::vector<math::rectf>&rects,
		const std::vector<int>&indices,const video::SColor& clr,const math::rectf*clip=0);

	void draw2DImage(const math::vector2d& pos,const math::vector2d &sizeFactor,float spacing/*,video::TextureUnit* texture*/,const std::vector<math::rectf>&rects,
		const std::vector<math::vector2d>&rectsSize,const std::vector<int>&indices,const video::SColor& clr,const math::rectf*clip=0);
	
	//! use material
	//! \param mtrl:material to use
	//! \param reset: reset material stage to this material 
	void useRenderPass(video::RenderPass* mtrl);
	bool useTexture(int stage,video::TextureUnit*tex);
	bool useTextureByID(GLenum target,int stage,int TexID);
	void disableTexture(int stage);
//	void Event(SEvent&event);
	
	//! draw 3d vertex
	//! \param point:vertex position
	//! \param color:vertex color
	//! \param size:vertex size

	void drawVertex(const math::Point3df*points,int n,const video::SColor& color);
	
	//! draw multiple lines
	//! \param points:std::vector of points
	//! \param n:points count
	//! \param color:lines color
	void drawLine(const math::Point3df*points,int n,const video::SColor& color);

	
	//! draw mesh and it's sub mesh buffers
	void drawMesh(scene::SMesh* mesh);
	//! draw mesh buffer 
	void drawSingleMesh(scene::IMeshBuffer* mesh);
	
	//! draw mesh buffer -the other method-
	void drawBuffer(StreamsData* streams,IndexData* indices,video::EMeshRenderType rType);


	//! add dynamic light
	void addDynamicLight(scene::LightNode*light);
	void disableAllLights();

	int getDynamicLightsCount();
	scene::LightNode* getDynamicLight(int index);

	//! set the global ambient color
	void setAmbientColor(const video::SColor& color);

	void SetSeparateSceneBlending(EBlendFactor srcFactor, EBlendFactor dstFactor, EBlendFactor srcAlphaFactor, EBlendFactor dstAlphaFactor);
	void SetSceneBlending(EBlendFactor srcFactor, EBlendFactor dstFactor);

	//! add new material renderer


	//! clear one of back buffers (color,depth,..)
	void clearBuffer(EDeviceBuffer buff);

	video::IRenderTargetPtr createMRenderTarget(const core::string&name,const math::Point2di& dim,
		EPixelFormat format,int buffers,bool shareContexts,bool shareObjects);
	video::IRenderTargetPtr createRenderTarget(const core::string&name,const ITexturePtr& target,
		const IHardwarePixelBufferPtr& depthBuffer,const IHardwarePixelBufferPtr& stencilBuffer,bool depthOnly);

	bool setRenderTarget(video::IRenderTargetPtr rt,bool clearBackBuffer=true,bool clearZBuffer=true,const video::SColor& clearColor=video::SColor(0,0,0,0),const math::recti* vpRect=0);
	video::IRenderTargetPtr getRenderTarget();

	void drawStencilShadowVolume(math::vector3d *vertices,int vtxCnt,bool zfail);

	void drawStencilShadow(bool clearStencilBuffer, const video::SColor &shadowColor);
	
	float getPixelDepth(int x,int y);
	void getPixels(const math::rect<int> &rc,std::vector<SColor> &outPixels);

	void loadXMLSettings(xml::XMLElement* elem);
	xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);

	///////
	void setStreamMap(StreamsData*streams);
	void setStream(video::IHardwareStreamBuffer* stream);
	void endStreams();

	void drawElements(video::IndexData* ib,video::EMeshRenderType rtype);
	void drawElementsUP(const void*indices,IHardwareIndexBuffer::EIndexType type,uint startIndex,int count,video::EMeshRenderType rtype);

	void setDepthBias(float linearBias,float slopeBias);


	void _SetGLState(GLenum st,bool s);
	void _SetGLClientState(GLenum st,bool s);
	bool _GetGLState(GLenum st);
	void _SetMatrixMode(GLenum m);
	void _SetAlphaFunc(GLenum func,GLclampf c);
	void _SetFrontFace(GLenum f);
	//////////////////////////////////////////////////////////////////////////

	void preThreadStart();
	void postThreadStart();
	void registerThread();
	void unregisterThread();
	void removeContext(GLContext*c);
	void switchContext(GLContext*c);

private:




};


class mrayGLDeviceFactory:public IDeviceFactoryCreater
{
public:

	mrayGLDeviceFactory();
	virtual~mrayGLDeviceFactory();

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