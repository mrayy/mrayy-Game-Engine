
#include "stdafx.h"

#include <io.h>
#include <direct.h>  



#include <gl\ExtGL.h>

#include "GLDev.h"
#include <Engine.h>
#include "Win32Context.h"
#include <SMesh.h>
#include <RenderMaterial.h>
#include <gl\glext.h>
#include <macros.h>
#include <CShaderFactory.h>

#include <IFileSystem.h>
#include <VideoLoggerSystem.h>

#include <ITimer.h>

#include <matrix3x3.h>
#include <LightNode.h>

#include <CShaderFactory.h>
#include "SGLCGShaderFactory.h"
#include "SGLSLShaderFactory.h"
//#include "GLRenderTarget.h"

#include "GLStreamBuffer.h"
#include "GLIndexBuffer.h"
#include <SoftwareBufferManager.h>
#include <SoftwareIndexBuffer.h>
#include <SoftwareStreamBuffer.h>
#include <nvapi.h>
#include <IThreadManager.h>

#include "GLPBufferTexture.h"
#include "GLMRenderTarget.h"
#include "GLFBORenderTarget.h"
#include "MeshBufferData.h"

#include "Win32GLSupport.h"

#include "GLWin32RenderWindow.h"
#include "ViewPort.h"

#include "MathUtil.h"
#include "MeshAnimatorManager.h"
#include "ShaderResourceManager.h"
#include "TraceManager.h"
#include "ShaderSemanticTable.h"

#include "RenderTechnique.h"
#include "RenderPass.h"


namespace mray{
namespace video{

#define VBO_BUFFER_OFFSET(x) ((char*)NULL +(x))


	GLenum g_ETextureClamMap[]={
		GL_REPEAT,
		GL_CLAMP,
		GL_CLAMP_TO_EDGE,
		GL_CLAMP_TO_BORDER,
		GL_MIRRORED_REPEAT
	};

GLenum getGLBlend(EBlendFactor f){
	switch(f){
	case EBF_ZERO:return GL_ZERO;
	case EBF_ONE:return GL_ONE;
	case EBF_SRC_COLOR:return GL_SRC_COLOR;
	case EBF_ONE_MINUS_SRC_COLOR:return GL_ONE_MINUS_SRC_COLOR;
	case EBF_DST_COLOR:return GL_DST_COLOR;
	case EBF_ONE_MINUS_DST_COLOR:return GL_ONE_MINUS_DST_COLOR;
	case EBF_SRC_ALPHA:return GL_SRC_ALPHA;
	case EBF_ONE_MINUS_SRC_ALPHA:return GL_ONE_MINUS_SRC_ALPHA;
	case EBF_DST_ALPHA:return GL_DST_ALPHA;
	case EBF_ONE_MINUS_DST_ALPHA:return GL_ONE_MINUS_DST_ALPHA;
	case EBF_SRC_ALPHA_SATURATE:return GL_SRC_ALPHA_SATURATE;
	}
	return 0;
}

GLenum getAlphaFunction(EAlphaFunc f){
	switch(f){
	case EAF_Never:return GL_NEVER;
	case EAF_Less:return GL_LESS;
	case EAF_Equal:return GL_EQUAL;
	case EAF_LessEqual:return GL_LEQUAL;
	case EAF_Greater:return GL_GREATER;
	case EAF_NotEqual:return GL_NOTEQUAL;
	case EAF_GreaterEqual:return GL_GEQUAL;
	case EAF_Always:return GL_ALWAYS;
	}
	return 0;
}

GLenum getElementType(EStreamDataType type){
	switch(type)
	{
	case ESDT_Point1f:
	case ESDT_Point2f:
	case ESDT_Point3f:
	case ESDT_Point4f:
		return GL_FLOAT;
	case ESDT_Point1s:
	case ESDT_Point2s:
	case ESDT_Point3s:
	case ESDT_Point4s:
		return GL_SHORT;
	case ESDT_Color4b:
		return GL_UNSIGNED_BYTE;
	}
	return 0;
}

class GLThreadListener:public OS::MultiThreadListener
{
	GLDev*m_dev;
public:
	GLThreadListener(GLDev*dev){
		m_dev=dev;
	}

	virtual void preThreadStart(){
		m_dev->preThreadStart();
	}
	virtual void postThreadStart(){
		m_dev->postThreadStart();
	}
	virtual void registerThread(){
		m_dev->registerThread();
	}
	virtual void unregisterThread(){
		m_dev->unregisterThread();
	}
};

const core::string GLDev::m_deviceType=mT("OpenGL");

GLDev::GLDev():mrayDev(),
		m_renderTarget(0),
		fogEnable(0),m_activeContext(0),m_mainContext(0),m_enableScissorRect(false)
{
	m_glInited=0;

	m_activeViewport=0;
	m_lastTextureType=0;

//	m_rtViewPort=false;

	m_currentPBuffer=0;
	m_currentPBufferTexture=0;

	gLogManager.startSection(mT("OpenGL"));

	m_caps=new DeviceCapabilites();
/*
	NVAPI_INTERFACE res;
	res=NvAPI_Initialize();
	if(res==NVAPI_OK ){
		NvDisplayHandle  hNvDisplay; 
		NV_DISPLAY_DRIVER_VERSION *  pVersion  ;
		NvAPI_GetDisplayDriverVersion ( hNvDisplay, pVersion ) ;

	}*/

	m_frontFace=0;
	m_lineWidth=0;
	m_pointSize=0;
	m_activeMatrixMode=0;
	m_alphaFunc=0;
	m_alphaFuncClamp=0;

	m_support=GLSupport::GetInstance();
	((Win32GLSupport*)m_support)->SetDevice(this);
	((Win32GLSupport*)m_support)->InitaliseWGL();
	
	m_vertexPointerSet=0;
	currDrawingObjects=0;
	m_hardwareBuffManager=0;
	fogEnable=0;

	m_fogParams.color.Set(0.5,0.5,0.5,1);
	m_fogParams.type=EFog_Linear;
	m_fogParams.start=10;
	m_fogParams.end=1000;
	m_fogParams.density=1;
	m_fogParams.useFogCoord=false;

	setFogParameters(m_fogParams);

	setAmbientColor(0);

	lastLightAdded=0;

	currentRenderMode=RM_NONE;


	gShaderResourceManager.addShaderLoader(new SGLCGShaderFactory());
	gShaderResourceManager.addShaderLoader(new SGLSLShaderFactory());



	m_RenderStates[RS_Wireframe]=m_DefaultRenderStates[RS_Wireframe]=ES_DontUse;
	m_RenderStates[RS_Smooth]=m_DefaultRenderStates[RS_Smooth]		=ES_Use;
	m_RenderStates[RS_Lighting]=m_DefaultRenderStates[RS_Lighting]	=ES_DontUse;
	m_RenderStates[RS_ZTest]=m_DefaultRenderStates[RS_ZTest]		=ES_Use;
	m_RenderStates[RS_ZWrite]=m_DefaultRenderStates[RS_ZWrite]		=ES_Use;
	m_RenderStates[RS_Points]=m_DefaultRenderStates[RS_Points]		=ES_DontUse;
	m_RenderStates[RS_CullFace]=m_DefaultRenderStates[RS_CullFace]	=ES_Use;
	m_RenderStates[RS_Fog]=m_DefaultRenderStates[RS_Fog]			=ES_DontUse;


	m_threadListener=new GLThreadListener(this);
	OS::IThreadManager::getInstance().registerThreadListener(m_threadListener);

	gLogManager.endSection(true);
}

GLDev::~GLDev()
{
	OS::IThreadManager::getInstance().removeThreadListener(m_threadListener);

	shutdown();

	delete m_mainContext;

	colorArrayBuffer.clear();

	/**/
}


void GLDev::_initGLCaps()
{
	loadExtensions();

	if(m_caps->isFeatureSupported(EDF_VertexBufferObject)  )
		m_hardwareBuffManager=new GLHardwareBufferManager();
	else
		m_hardwareBuffManager=new SoftwareBufferManager();

	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
	_SetFrontFace( GL_CCW );
	_SetGLState(GL_POINT_SMOOTH,true);
	glCullFace(GL_BACK);
	setScissorTest(true);

	if(m_caps->isFeatureSupported(EDF_MultiSample))
		_SetGLState(GL_MULTISAMPLE_ARB,true);


	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);
	SetSceneBlending(EBF_SRC_ALPHA, EBF_ONE_MINUS_SRC_ALPHA);

	setRenderState(RS_Smooth,true);



	dynamicLights.resize(m_caps->getMaxLightsUnits());

	m_TextureID.resize(m_caps->getMaxTextureUnits());
	currTextures.resize(m_caps->getMaxTextureUnits());


}


void GLDev::_SetGLState(GLenum st,bool s)
{
// 	if(_GetGLState(st)==s)
// 		return;
	m_states[st]=s;
	if(s)
		glEnable(st);
	else glDisable(st);
}

void GLDev::_SetGLClientState(GLenum st,bool s)
{

 	stdext::hash_map<GLenum,bool>::iterator it=m_states.find(st);
 	if(it!=m_states.end() && it->second==s)
 		return;
	m_states[st]=s;
	if(s)
		glEnableClientState(st);
	else glDisableClientState(st);
}
bool GLDev::_GetGLState(GLenum st)
{

	stdext::hash_map<GLenum,bool>::iterator it=m_states.find(st);
	if(it==m_states.end())
	{
		bool ret=glIsEnabled(st);
		m_states[st]=ret;
		return ret;
	}
	return it->second;
}
void GLDev::_SetMatrixMode(GLenum m)
{
// 	if(m_activeMatrixMode==m)
// 		return;
	m_activeMatrixMode=m;
	glMatrixMode(m);
}

void GLDev::loadExtensions()
{
	traceFunction(video);
	const char*str=(const char*)glGetString(GL_VENDOR);
	if(str){
		gLogManager.log(mT("Opengl Vendor is "),core::StringConverter::toString(str),ELL_INFO);
		if(strstr(str,"NVIDIA"))
			m_caps->setVendor(EGPU_NVidia);
		else if(strstr(str,"ATI"))
			m_caps->setVendor(EGPU_ATI);
		else if(strstr(str,"Intel"))
			m_caps->setVendor(EGPU_Intel);
		else if(strstr(str,"S3"))
			m_caps->setVendor(EGPU_S3);
		else if(strstr(str,"Matrox"))
			m_caps->setVendor(EGPU_Matrox);
		else if(strstr(str,"3DLabs"))
			m_caps->setVendor(EGPU_3DLabs);
	}
	str=(const char*)glGetString(GL_VERSION);
	if(str){
		if(atof(str)>=1.2)
			gLogManager.log(mT("Opengl Version is "),core::StringConverter::toString((char*)glGetString(GL_VERSION)),ELL_INFO);
		else
			gLogManager.log(mT("Opengl Version is less than 1.2"),mT("Warning!!"),ELL_WARNING);
	}
	str=(const char*)glGetString(GL_RENDERER);
	if(str){
		core::string ss=core::StringConverter::toString(str);
		gLogManager.log(mT("Opengl Renderer is "),ss,ELL_INFO);
		m_caps->setDeviceName(ss);
	}
	//gLogManager.log(mT("Opengl Extensions is "),core::StringConverter::toString((char*)glGetString(GL_EXTENSIONS)),ELL_INFO);

	initGLExt();


	GLint stencil;
	glGetIntegerv(GL_STENCIL_BITS,&stencil);
	if(stencil){
		m_caps->setFeature(EDF_StencilBuffer,true);
		m_caps->setStencilBufferBits(stencil);
	}
	m_caps->setFeature(EDF_Texture3D,true);


	if(GLEW_VERSION_1_4 || GLEW_SGIS_generate_mipmap){
		m_caps->setFeature(EDF_AutoMipmap,true);
	}
	if(GLEW_VERSION_1_3 || GLEW_ARB_texture_env_combine || GLEW_EXT_texture_env_combine)
	{
		m_caps->setFeature(EDF_Blending,true);
	}
	if(GLEW_ARB_multitexture){
		m_caps->setFeature(EDF_MultiTexture, true);
	}
	if(GLEW_ARB_multisample){
		m_caps->setFeature(EDF_MultiSample, true);
	}

	if (GLEW_ARB_fragment_shader){
		m_caps->setFeature(EDF_FragmentProgram, true);
		m_caps->addShaderProfile(mT("ps_1_1"));
		m_caps->addShaderProfile(mT("ps_1_2"));
		m_caps->addShaderProfile(mT("ps_1_3"));
		m_caps->addShaderProfile(mT("ps_1_4"));
	}

	if (GLEW_NV_register_combiners2 &&
		GLEW_NV_texture_shader)
	{
		m_caps->setFeature(EDF_FragmentProgram, true);
		m_caps->addShaderProfile(mT("fp20"));
	}
	if(GLEW_ARB_vertex_program){
		m_caps->setFeature(EDF_VertexProgram, true);
		m_caps->addShaderProfile(mT("vp1"));
#ifdef _USE_GLEE_
		if(GLEE_NV_vertex_program2_option){
#else
		if(GLEW_NV_vertex_program2_option){
#endif
			m_caps->addShaderProfile(mT("vp30"));
		}
#ifdef _USE_GLEE_
		if(GLEE_NV_vertex_program3){
#else
		if(GLEW_NV_vertex_program2_option){
#endif
			m_caps->addShaderProfile(mT("vp40"));
		}
#ifdef _USE_GLEE_
		if(GLEE_NV_vertex_program4){
#else
		if(GLEW_NV_vertex_program4){
#endif
			m_caps->addShaderProfile(mT("gp40vp"));
			m_caps->addShaderProfile(mT("gp_vp"));
		}
	}

	if(GLEW_VERSION_2_0 && GLEW_EXT_geometry_shader4){
		m_caps->setFeature(EDF_GeometryProgram, true);
		m_caps->addShaderProfile(mT("nvgp4"));

		//Also add the CG profiles
		m_caps->addShaderProfile(mT("gpu_gp"));
		m_caps->addShaderProfile(mT("gp4gp"));
	}

	if (GLEW_ARB_fragment_program){
		m_caps->setFeature(EDF_FragmentProgram, true);
		m_caps->addShaderProfile(mT("arbfp1"));
#ifdef _USE_GLEE_
		if(GLEE_NV_fragment_program_option)
#else
		if(GLEW_NV_fragment_program_option)
#endif
			m_caps->addShaderProfile(mT("fp30"));
#ifdef _USE_GLEE_
		if(GLEE_NV_fragment_program2)
#else
		if(GLEW_NV_fragment_program2)
#endif
			m_caps->addShaderProfile(mT("fp40"));
		m_caps->addShaderProfile(mT("arbfp1"));
	}
	// load common extensions
	GLint tmp;
	glGetIntegerv(GL_MAX_LIGHTS, &tmp);
	m_caps->setMaxLightsUnits(tmp);

	if(GLEW_VERSION_1_3 || 	GLEW_ARB_multitexture){
		glGetIntegerv(GL_MAX_TEXTURE_UNITS, &tmp);/*
		GLint arb=0;
		glGetIntegerv(GL_MAX_TEXTURE_EPixel_UNITS,&arb);
		if(arb>tmp)
			tmp=arb;*/
		m_caps->setMaxTextureUnits(tmp);
	}

	if (GLEW_ARB_vertex_program){
		m_caps->setFeature(EDF_GeometryProgram,true);
	}
	if (GLEW_ARB_shading_language_100){
		m_caps->addShaderProfile(mT("glsl"));
	}
	if (GLEW_EXT_texture_filter_anisotropic)
		m_caps->setFeature(EDF_Anisotropy, true);

	if (GLEW_ATI_separate_stencil)
		m_caps->setFeature(EDF_SeperateStencilBuffer, true);

	if (GLEW_SGIS_generate_mipmap)
		m_caps->setFeature(EDF_MIPMAP_AUTO_UPDATE, true);

	if (GLEW_ARB_texture_compression)
		m_caps->setFeature(EDF_DXT_Compression, true);

	if (GLEW_ARB_texture_non_power_of_two)
		m_caps->setFeature(EDF_TextureNP2, true);

	if (GLEW_EXT_packed_depth_stencil)
		m_caps->setFeature(EDF_PackedDepthStencil, true);

	if (GLEW_EXT_separate_specular_color)
		m_caps->setFeature(EDF_SeparateSpecularColor, true);

	if (GLEW_ARB_texture_float || GLEW_ATI_texture_float)
		m_caps->setFeature(EDF_FloatTexture, true);

	if (GLEW_ARB_texture_env_dot3)
		m_caps->setFeature(EDF_DOT3, true);

	if (GLEW_EXT_texture_cube_map)
		m_caps->setFeature(EDF_CubeMapping, true);

	if (GLEW_NV_point_sprite || GLEW_ARB_point_sprite)
		m_caps->setFeature(EDF_PointSprites, true);

	if (GLEW_NV_point_sprite)
		m_caps->setFeature(EDF_StencilBuffer, true);

	if (GLEW_ARB_vertex_buffer_object)
		m_caps->setFeature(EDF_VertexBufferObject, true);

	if (GLEW_ARB_occlusion_query)
		m_caps->setFeature(EDF_OcclusionQuery, true);

	if (GLEW_EXT_framebuffer_object){
		m_caps->setFeature(EDF_FrameBufferObject, true);
	}
	if(GLEW_VERSION_2_0 || 
		GLEW_ARB_draw_buffers ||
		GLEW_ATI_draw_buffers)
	{
		GLint buf;
		glGetIntegerv(GL_MAX_DRAW_BUFFERS_ARB, &buf);
		m_caps->setNumMultiRenderTargets(buf);

		m_caps->setFeature(EDF_RenderToTarget, true);
	}
	float maxAnis=0;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnis);
	m_caps->setMaxAnisotropy(maxAnis);

	currTextures.resize(m_caps->getMaxTextureUnits());
	for(int i=0;i<currTextures.size();++i)
		currTextures[i]=0;
	
	if (!glActiveTextureARB || !glClientActiveTextureARB)
	{
		gLogManager.log(mT("couldn't load multitexture extension.."),ELL_WARNING);
	}
}
DeviceCapabilites* GLDev::getCapabilities(){
	return m_caps;
}

RenderWindow* GLDev::CreateRenderWindow(const core::string&name,const math::vector2di& size,bool fullScreen,
										const OptionContainer&params,ulong hWND)
{

	RenderWindow* ret=m_support->CreateRenderWindow(name,size,fullScreen,params,hWND);
	if(!m_glInited)
	{
		InitializeContext(ret);

		_oneTimeInit();
		if(m_activeContext)
			m_activeContext->setCurrent();

		_initGLCaps();
		m_glInited=true;
	}

	if(ret)
		gLogManager.log(mT("Window Created: ")+name,ELL_SUCCESS);
	return ret;
}
void GLDev::InitializeContext(RenderWindow* primary)
{
	GLWin32RenderWindow* wnd=(GLWin32RenderWindow*)primary;
	m_mainContext= wnd->GetContext();
	m_activeContext=m_mainContext;

	if(m_activeContext)
		m_activeContext->setCurrent();

	m_support->InitExtensions();

//	glewContextInit();
}




void GLDev::setFogParameters(const FogParameters& params)
{
	m_fogParams=params;
	GLint fogType=GL_LINEAR;
	switch(params.type)
	{
	case EFog_Linear:
		fogType=GL_LINEAR;
		break;
	case EFog_Exp:
		fogType=GL_EXP;
		break;
	case EFog_Exp2:
		fogType=GL_EXP2;
		break;
	}

	glFogi(GL_FOG_MODE, fogType);
#ifdef GL_EXT_fog_coord
	glFogi(GL_FOG_COORDINATE_SOURCE, GL_FRAGMENT_DEPTH);
#endif

	if(params.type==EFog_Linear)
	{
		glFogf(GL_FOG_START, params.start);
		glFogf(GL_FOG_END, params.end);
	}
	else{
		if(params.useFogCoord){
			glFogi(GL_FOG_COORD_SRC, GL_FOG_COORD);
			glFogi(GL_FOG_MODE, GL_EXP2);
		}else{
			glFogi(GL_FOG_MODE, GL_EXP);
			glFogi(GL_FOG_COORD_SRC, GL_FRAGMENT_DEPTH);
		}
		glFogf(GL_FOG_DENSITY, params.density);
	}

	GLfloat data[4] = {params.color.R, params.color.G, params.color.B, params.color.A};
	glFogfv(GL_FOG_COLOR, data);
}
const FogParameters& GLDev::getFogParameters(const FogParameters& params)
{
	return m_fogParams;
}

void GLDev::enableFog(bool e)
{
	fogEnable=e;
	_SetGLState(GL_FOG,e);
}
bool GLDev::isFogEnabled(){
	return fogEnable;
}
void GLDev::setMultiSampling(bool enable)
{
	if(!getCapabilities()->isFeatureSupported(EDF_MultiSample))
		return;

	_SetGLState(GL_MULTISAMPLE_ARB,enable);
}
bool GLDev::getMultiSampling()
{
	if(!getCapabilities()->isFeatureSupported(EDF_MultiSample))
		return false;
	GLboolean v=false;
	glGetBooleanv(GL_MULTISAMPLE_ARB,&v);
	return v!=0;
}


void GLDev::setLineWidth(float lineWidth){
	if(m_lineWidth==lineWidth)
		return;
	m_lineWidth=lineWidth;
	glLineWidth(lineWidth);
}
void GLDev::setPointSize(float size){
	if(m_pointSize==size)
		return;
	m_pointSize=size;
	glPointSize(size);
}


void GLDev::setCurrentPBuffer(GLPBuffer*b){
	m_currentPBuffer=b;
}
GLPBuffer* GLDev::getCurrentPBuffer(){
	return m_currentPBuffer;
}
void GLDev::setCurrentPBufferTexture(GLPBufferTexture*b){
	m_currentPBufferTexture=b;
}
GLPBufferTexture* GLDev::getCurrentPBufferTexture(){
	return m_currentPBufferTexture;
}

void GLDev::shutdown()
{
	{
		std::list<GLContext*>::iterator it=m_contexts.begin();
		for(;it!=m_contexts.end();++it)
		{
			(*it)->deleteContext();
			delete *it;
		}
		m_contexts.clear();
	}

	FIRE_LISTENR_METHOD(OnDeviceShutdown,(this));

}

void GLDev::begin(bool clearColorBuf,bool clearDepthBuf)
{
	mrayDev::begin(clearColorBuf,clearDepthBuf);
	FIRE_LISTENR_METHOD(OnPreRender,(this));
	GLbitfield mode=0;
	if(clearColorBuf)
		mode|=GL_COLOR_BUFFER_BIT;
	if(clearDepthBuf)
		mode|=GL_DEPTH_BUFFER_BIT;
	glClear(mode);
	setScissorTest(true);
}

void GLDev::end()
{
	setScissorTest(false);
	FIRE_LISTENR_METHOD(OnPostRender,(this));
//	float currtime=gTimer.getActualTime();

//	core::CFPS::getInstance().regFrame(currtime);

	//benchmarkAddSample((OS::ITimer::getInstance().getActualTime()-m_startTime));

   // glDisable(GL_SCISSOR_TEST);

/*
	static float ltime=OS::gTimer.getActualTime();
	if(currtime - ltime>100){
		ltime=currtime;
	}
	*/
}
void GLDev::draw2DRectangle(const math::rect<float> &src,const video::SColor& color,bool fill,const math::rect<float>*clip)
{
	math::rect<float> rc=src;
	if(clip)
		rc.clipRect(*clip);
	if(!rc.isValid()) 
		return;

// 	glPushMatrix();
// 	glLoadIdentity();
	
	disableTexture(0);
	disableTexture(1);
	disableTexture(2);
	if(fill)
		glBegin(GL_QUADS);
	else
		glBegin(GL_LINE_LOOP);

	glColor4f(color.R,color.G,color.B,color.A);
	glTexCoord2f(0,0);
	glVertex2f(rc.ULPoint.x,(rc.ULPoint.y));

	glTexCoord2f(1,0);
	glVertex2f(rc.BRPoint.x,(rc.ULPoint.y));

	glTexCoord2f(1,1);
	glVertex2f(rc.BRPoint.x,(rc.BRPoint.y));

	glTexCoord2f(0,1);
	glVertex2f(rc.ULPoint.x,(rc.BRPoint.y));

	glEnd();
	m_batchesDrawn++;
	// 	glPopMatrix();
}

void GLDev::draw2DRectangleRot(const math::rect<float> &pos,const math::vector2d &origin,const video::SColor &clr,float angle,bool fill){

	if(!pos.isValid() )
		return;

	//	set2DMode();
	//useTexture(0,texture);
// 	glPushMatrix();
// 	glLoadIdentity();

	math::vector2d size=pos.getSize();

	// 	glTranslatef(rc.ULPoint.x+size.x,rc.ULPoint.y+size.y,0);
	// 	glRotatef(angle,0,0,1);

	math::vector2d points[4];
	if(angle==0){
		points[0]=math::vector2d(pos.ULPoint.x		 ,pos.ULPoint.y);
		points[1]=math::vector2d(pos.ULPoint.x+size.x,pos.ULPoint.y);
		points[2]=math::vector2d(pos.ULPoint.x+size.x,pos.ULPoint.y+size.y);
		points[3]=math::vector2d(pos.ULPoint.x		 ,pos.ULPoint.y+size.y);
	}else{
		math::matrix3x3 m;
		m.setAngle(angle);
		m.translate(pos.ULPoint+origin);
		points[0]=m*math::vector2d(-origin.x,-origin.y);
		points[1]=m*math::vector2d( size.x-origin.x,-origin.y);
		points[2]=m*math::vector2d( size.x-origin.x, size.y-origin.y);
		points[3]=m*math::vector2d(-origin.x, size.x-origin.y);
	}
	//setTransformationState(TS_TEXTURE0,texture->getTransformationMatrix());
	//useTexture(0,texture);

	if(fill)
		glBegin(GL_QUADS);
	else
		glBegin(GL_LINE_LOOP);
	
		glColor4f(clr.R,clr.G,clr.B,clr.A);
		glVertex3f(points[0].x,points[0].y,0);

		glVertex3f(points[1].x,points[1].y,0);

		glVertex3f(points[2].x,points[2].y,0);

		glVertex3f(points[3].x,points[3].y,0);
	glEnd();
	m_batchesDrawn++;

/*	if(!rc.isValid()) 
		return;

// 	glPushMatrix();
// 	glLoadIdentity();
	
	math::vector2d size=rc.getSize();
// 	glTranslatef(rc.ULPoint.x+size.x,rc.ULPoint.y+size.y,0);
// 	glRotatef(angle,0,0,1);

	math::vector2d pos[4];
	if(angle==0){
		pos[0]=math::vector2d(rc.ULPoint.x		 ,rc.ULPoint.y);
		pos[1]=math::vector2d(rc.ULPoint.x+size.x,rc.ULPoint.y);
		pos[2]=math::vector2d(rc.ULPoint.x+size.x,rc.ULPoint.y+size.y);
		pos[3]=math::vector2d(rc.ULPoint.x		 ,rc.ULPoint.y+size.y);
	}else{
		math::matrix3x3 m;
		m.setAngle(angle);
		m.translate(math::vector2d(rc.ULPoint.x+size.x*0.5,rc.ULPoint.y+size.y*0.5));
		pos[0]=m*math::vector2d(-size.x*0.5,-size.y*0.5);
		pos[1]=m*math::vector2d( size.x*0.5,-size.y*0.5);
		pos[2]=m*math::vector2d( size.x*0.5, size.y*0.5);
		pos[3]=m*math::vector2d(-size.x*0.5, size.y*0.5);
	}
	
	glDisable(GL_TEXTURE_2D);
	if(fill)
		glBegin(GL_QUADS);
	else
		glBegin(GL_LINE_LOOP);
		glColor4f(color.R,color.G,color.B,color.A);
		glVertex2f(pos[0].x,pos[0].y);
		glVertex2f(pos[1].x,pos[1].y);
		glVertex2f(pos[2].x,pos[2].y);
		glVertex2f(pos[3].x,pos[3].y);
	glEnd();
//	glPopMatrix();*/
}

void GLDev::draw2DRectangle(const math::rect<float> &src,const video::SColor &ul,const video::SColor &ur,const video::SColor &br,
							const video::SColor &bl,bool fill,const math::rect<float>*clip)
{
	math::rect<float> rc=src;
	if(clip)
		rc.clipRect(*clip);
	if(!rc.isValid()) 
		return;
// 	glPushMatrix();
// 	glLoadIdentity();
	disableTexture(0);
	if(fill)
		glBegin(GL_QUADS);
	else
		glBegin(GL_LINE_LOOP);

	//	colors[3]=1;
		glColor4f(ul.R,ul.G,ul.B,ul.A);
		glVertex2f(rc.ULPoint.x,(rc.ULPoint.y));

		glColor4f(ur.R,ur.G,ur.B,ur.A);
		glVertex2f(rc.BRPoint.x,(rc.ULPoint.y));

		glColor4f(br.R,br.G,br.B,br.A);
		glVertex2f(rc.BRPoint.x,(rc.BRPoint.y));

		glColor4f(bl.R,bl.G,bl.B,bl.A);
		glVertex2f(rc.ULPoint.x,(rc.BRPoint.y));
	glEnd();
	m_batchesDrawn++;

// 	glPopMatrix();
}

void GLDev::draw2DImage(const math::rectf &src,/*video::TextureUnit* texture,*/const video::SColor colors[4],const math::rectf*clip,const  math::rectf *tcoord)
{
	math::rectf rc=src;
	math::rectf coord(0,0,1,1);
	if(tcoord)
		coord=*tcoord;
	if(clip){
		math::rectf clipped=rc;
		math::vector2d coordSz=coord.getSize();
		clipped.clipRect(*clip);
		math::rectf tmp;

		tmp.ULPoint=clipped.ULPoint-rc.ULPoint;
		tmp.BRPoint=clipped.BRPoint-rc.ULPoint;

		tmp.ULPoint/=rc.getSize();
		tmp.BRPoint/=rc.getSize();

		tmp.ULPoint*=coordSz;
		tmp.BRPoint*=coordSz;

		coord.ULPoint+=tmp.ULPoint;
		coord.BRPoint-=coordSz-tmp.BRPoint;

		rc=clipped;
	}
	if(!rc.isValid()) 
		return;

	//	set2DMode();
//	useTexture(0,texture);
// 	glPushMatrix();
// 	glLoadIdentity();
	glBegin(GL_QUADS);
	
		glColor4f(colors[0].R,colors[0].G,colors[0].B,colors[0].A);
		glTexCoord2f(coord.ULPoint.x,coord.ULPoint.y);
		glVertex2f(rc.ULPoint.x,(rc.ULPoint.y));

		glColor4f(colors[1].R,colors[1].G,colors[1].B,colors[1].A);
		glTexCoord2f(coord.BRPoint.x,coord.ULPoint.y);
		glVertex2f(rc.BRPoint.x,(rc.ULPoint.y));

		glColor4f(colors[2].R,colors[2].G,colors[2].B,colors[2].A);
		glTexCoord2f(coord.BRPoint.x,coord.BRPoint.y);
		glVertex2f(rc.BRPoint.x,(rc.BRPoint.y));

		glColor4f(colors[3].R,colors[3].G,colors[3].B,colors[3].A);
		glTexCoord2f(coord.ULPoint.x,coord.BRPoint.y);
		glVertex2f(rc.ULPoint.x,(rc.BRPoint.y));
	glEnd();
	m_batchesDrawn++;
//	glPopMatrix();
}

void GLDev::draw2DImage(const math::rectf &src/*,video::TextureUnit* texture*/,const video::SColor &clr,const math::rectf*clip,
						  const math::rectf *tcoord)
{

	math::rectf rc=src;
	math::rectf coord(0,0,1,1);
	if(tcoord)
		coord=*tcoord;
	if(clip){
		math::rectf clipped=rc;
		math::vector2d coordSz=coord.getSize();
		clipped.clipRect(*clip);
		math::rectf tmp;

		tmp.ULPoint=clipped.ULPoint-rc.ULPoint;
		tmp.BRPoint=clipped.BRPoint-rc.ULPoint;

		tmp.ULPoint/=rc.getSize();
		tmp.BRPoint/=rc.getSize();

		tmp.ULPoint*=coordSz;
		tmp.BRPoint*=coordSz;

		coord.ULPoint+=tmp.ULPoint;
		coord.BRPoint-=coordSz-tmp.BRPoint;

	//	float wr=1.0f/rc.getWidth();
	//	float hr=1.0f/rc.getHeight();
	//	coord.ULPoint=clipped.ULPoint-rc.ULPoint;
	//	coord.BRPoint=clipped.BRPoint-rc.ULPoint;
		
	//	coord.BRPoint.x*=wr;
	//	coord.BRPoint.y*=hr;
	//	coord.ULPoint.x*=wr;
	//	coord.ULPoint.y*=hr;
		rc=clipped;
	}
	if(!rc.isValid()) 
		return;

	ushort tindices[6]={
		0,1,2,0,2,3
	};

//	useTexture(0,texture);

	_SetGLClientState(GL_VERTEX_ARRAY,true);
	_SetGLClientState(GL_TEXTURE_COORD_ARRAY,true);

	glColor4f(clr.R,clr.G,clr.B,clr.A);

	math::vector2d points[4]={
		math::vector2d(rc.ULPoint.x,rc.ULPoint.y),
		math::vector2d(rc.BRPoint.x,rc.ULPoint.y),
		math::vector2d(rc.BRPoint.x,rc.BRPoint.y),
		math::vector2d(rc.ULPoint.x,rc.BRPoint.y),
	};
	math::vector2d tc[4]={
		math::vector2d(coord.ULPoint.x,coord.ULPoint.y),
		math::vector2d(coord.BRPoint.x,coord.ULPoint.y),
		math::vector2d(coord.BRPoint.x,coord.BRPoint.y),
		math::vector2d(coord.ULPoint.x,coord.BRPoint.y),
	};
	glVertexPointer(2,GL_FLOAT,sizeof(math::vector2d),points);
	glTexCoordPointer(2,GL_FLOAT,sizeof(math::vector2d),tc);

	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,tindices);


	_SetGLClientState(GL_TEXTURE_COORD_ARRAY,false);
	_SetGLClientState(GL_VERTEX_ARRAY,false);
	m_batchesDrawn++;

}


void GLDev::draw2DImageRot(const math::rectf &pos,const math::vector2d &origin/*,video::TextureUnit* texture*/,const video::SColor &clr,float angle,
		 const math::rectf *tcoord)
{
	math::rectf coord(0,0,1,1);
	if(tcoord)
		coord=*tcoord;
	if(!pos.isValid() )
		return;

	//	set2DMode();
	//useTexture(0,texture);
// 	glPushMatrix();
// 	glLoadIdentity();

	math::vector2d size=pos.getSize();

	// 	glTranslatef(rc.ULPoint.x+size.x,rc.ULPoint.y+size.y,0);
	// 	glRotatef(angle,0,0,1);

	math::vector2d points[4];
	if(angle==0){
		points[0]=math::vector2d(pos.ULPoint.x		 ,pos.ULPoint.y);
		points[1]=math::vector2d(pos.ULPoint.x+size.x,pos.ULPoint.y);
		points[2]=math::vector2d(pos.ULPoint.x+size.x,pos.ULPoint.y+size.y);
		points[3]=math::vector2d(pos.ULPoint.x		 ,pos.ULPoint.y+size.y);
	}else{
		math::matrix3x3 m;
		m.setAngle(angle);
		m.translate(pos.ULPoint+origin);
		points[0]=m*math::vector2d(-origin.x,-origin.y);
		points[1]=m*math::vector2d( size.x-origin.x,-origin.y);
		points[2]=m*math::vector2d( size.x-origin.x, size.y-origin.y);
		points[3]=m*math::vector2d(-origin.x, size.y-origin.y);
	}
	//setTransformationState(TS_TEXTURE0,texture->getTransformationMatrix());
	//useTexture(0,texture);
	glBegin(GL_QUADS);
	
		glColor4f(clr.R,clr.G,clr.B,clr.A);
		glTexCoord2f(coord.ULPoint.x,coord.ULPoint.y);
		glVertex3f(points[0].x,points[0].y,0);

		glTexCoord2f(coord.BRPoint.x,coord.ULPoint.y);
		glVertex3f(points[1].x,points[1].y,0);

		glTexCoord2f(coord.BRPoint.x,coord.BRPoint.y);
		glVertex3f(points[2].x,points[2].y,0);

		glTexCoord2f(coord.ULPoint.x,coord.BRPoint.y);
		glVertex3f(points[3].x,points[3].y,0);
	glEnd();
	m_batchesDrawn++;

//	glPopMatrix();
}

void GLDev::draw2DImage(const math::vector2d &pos,const math::vector2d& size,float spacing/*,video::TextureUnit* texture*/,const std::vector<math::rectf>&rects,
	const std::vector<int>&indices,const video::SColor &clr,const math::rectf*clip)
{
	math::rectf rc;
	math::rectf p;
	p.ULPoint=pos;

	for(int i=0;i<indices.size();++i){
		int idx=indices[i];
		rc=rects[idx];
		p.BRPoint=p.ULPoint+size;
		draw2DImage(p,/*texture,*/clr,clip,&rc);
		p.ULPoint.x+=size.x+spacing;

	}
}
void GLDev::draw2DImage(const math::vector2d &pos,const math::vector2d &sizeFactor,float spacing,/*video::TextureUnit* texture,*/const std::vector<math::rectf>&rects,
	const std::vector<math::vector2d>&rectsSize,const std::vector<int>&indices,const video::SColor &clr,const math::rectf*clip)
{
	math::rectf rc;
	math::rectf p;
	p.ULPoint=pos;

	ushort tindices[4]={
		0,1,2,3
	};

	//useTexture(0,texture);
// 	glPushMatrix();
// 	glLoadIdentity();

 	_SetGLClientState(GL_VERTEX_ARRAY,true);
 	_SetGLClientState(GL_TEXTURE_COORD_ARRAY,true);

	glColor4f(clr.R,clr.G,clr.B,clr.A);
	for(int i=0;i<indices.size();++i){
		int idx=indices[i];
		rc=rects[idx];
		math::vector2d rSize=sizeFactor*rectsSize[idx];
		p.BRPoint=p.ULPoint+rSize;

		math::rectf coord=rc;
		if(clip){
			math::rectf clipped=rc;
			math::vector2d coordSz=coord.getSize();
			clipped.clipRect(*clip);
			math::rectf tmp;

			tmp.ULPoint=clipped.ULPoint-rc.ULPoint;
			tmp.BRPoint=clipped.BRPoint-rc.ULPoint;

			tmp.ULPoint/=rc.getSize();
			tmp.BRPoint/=rc.getSize();

			tmp.ULPoint*=coordSz;
			tmp.BRPoint*=coordSz;

			coord.ULPoint+=tmp.ULPoint;
			coord.BRPoint-=coordSz-tmp.BRPoint;

			rc=clipped;
		}
		if(!rc.isValid()) 
			continue;

		math::vector2d points[4]={
			math::vector2d(p.ULPoint.x,p.ULPoint.y),
			math::vector2d(p.BRPoint.x,p.ULPoint.y),
			math::vector2d(p.BRPoint.x,p.BRPoint.y),
			math::vector2d(p.ULPoint.x,p.BRPoint.y),
		};
		math::vector2d tc[4]={
			math::vector2d(coord.ULPoint.x,coord.ULPoint.y),
			math::vector2d(coord.BRPoint.x,coord.ULPoint.y),
			math::vector2d(coord.BRPoint.x,coord.BRPoint.y),
			math::vector2d(coord.ULPoint.x,coord.BRPoint.y),
		};
 		glVertexPointer(2,GL_FLOAT,sizeof(math::vector2d),points);
 		glTexCoordPointer(2,GL_FLOAT,sizeof(math::vector2d),tc);

		glDrawElements(GL_QUADS,4,GL_UNSIGNED_SHORT,tindices);
		//glDrawArrays(GL_QUADS,0,1);
		//draw2DImage(p,texture,clr,clip,&rc);
		m_batchesDrawn++;

		p.ULPoint.x+=rSize.x+spacing;
	}


	//	glPopMatrix();
	_SetGLClientState(GL_TEXTURE_COORD_ARRAY,false);
 	_SetGLClientState(GL_VERTEX_ARRAY,false);
}
void GLDev::draw3DLine(const math::vector3d &start,const math::vector3d &end,const video::SColor& color)
{

//	glPushMatrix();

	disableTexture(0);

	glBegin(GL_LINE_STRIP);
	
	glColor4f(color.R,color.G,color.B,color.A);
	glVertex3f(start.x,start.y,start.z);
	glVertex3f(end.x,end.y,end.z);

	glEnd();
	m_batchesDrawn++;

//	glPopMatrix();
}

void GLDev::draw2DLine(const math::vector2d &start,const math::vector2d &end,const video::SColor& color)
{

// 	glPushMatrix();
// 	glLoadIdentity();

	disableTexture(0);

	glBegin(GL_LINE_STRIP);
	glColor4f(color.R,color.G,color.B,color.A);
	
	glVertex2f(start.x,start.y);
	glVertex2f(end.x,end.y);

	glEnd();
	m_batchesDrawn++;

//	glPopMatrix();
}
void GLDev::draw2DLine(const math::Point2df*points,int n,const video::SColor& color){
// 	glPushMatrix();
// 	glLoadIdentity();

	disableTexture(0);
	glColor4f(color.R,color.G,color.B,color.A);
/*
	glBegin(GL_LINES);
		for(int i=0;i<n;++i)
		{
			glVertex2d(points[i].x,points[i].y);
		}
	glEnd();
	*/
	_SetGLClientState(GL_VERTEX_ARRAY,true);

	glVertexPointer(2,GL_FLOAT,sizeof(math::Point2df),points);
		glDrawArrays(GL_LINE_STRIP,0,n);
	_SetGLClientState(GL_VERTEX_ARRAY,false);
	m_batchesDrawn++;


//	glPopMatrix();
}
void GLDev::draw2DShape(const math::Point2df*points,int n,const video::SColor& color,bool fill)
{
	disableTexture(0);
	glColor4f(color.R,color.G,color.B,color.A);
	_SetGLClientState(GL_VERTEX_ARRAY,true);

	int type=fill?GL_POLYGON:GL_LINE_LOOP;

	glVertexPointer(2,GL_FLOAT,sizeof(math::Point2df),points);
	glDrawArrays(type,0,n);
	_SetGLClientState(GL_VERTEX_ARRAY,false);
}

void GLDev::draw2DShape(const math::Point2df*points,const video::SColor*clr,int n,bool fill)
{
	disableTexture(0);
	_SetGLClientState(GL_VERTEX_ARRAY,true);
	_SetGLClientState(GL_COLOR_ARRAY,true);

	int type=fill?GL_POLYGON:GL_LINE_LOOP;

	glVertexPointer(2,GL_FLOAT,sizeof(math::Point2df),points);
	glColorPointer(4,GL_FLOAT,sizeof(video::SColor),clr);
	glDrawArrays(type,0,n);
	_SetGLClientState(GL_VERTEX_ARRAY,false);
	_SetGLClientState(GL_COLOR_ARRAY,false);
	m_batchesDrawn++;

}

void GLDev::draw2DShapeTextured(const math::Point2df*points,const math::Point2df*texCoord,int n,const video::SColor&clr,bool fill)
{
	glColor4f(clr.R,clr.G,clr.B,clr.A);
	_SetGLClientState(GL_VERTEX_ARRAY,true);
	
	_SetGLClientState(GL_TEXTURE_COORD_ARRAY,true);

	int type=fill?GL_POLYGON:GL_LINE_LOOP;

	glVertexPointer(2,GL_FLOAT,sizeof(math::Point2df),points);
	glTexCoordPointer(2,GL_FLOAT,sizeof(math::Point2df),texCoord);

	glDrawArrays(type,0,n);
	_SetGLClientState(GL_VERTEX_ARRAY,false);
	_SetGLClientState(GL_TEXTURE_COORD_ARRAY,false);
	m_batchesDrawn++;

}

math::Point2di GLDev::getRenderTargetSize()
{
//	if(currentRendertargetSize.x==0)
//		return gSize;
	return m_viewportRect.getSize();
}


bool GLDev::setRenderState(RenderStates flag,bool set){
#define SetRS(RS)\
	_SetGLState(RS,set);
	//if(mrayDev::setRenderState(flag,set))
	{
		switch (flag)
		{
		case RS_CullFace:
			SetRS(GL_CULL_FACE);
			break;
		case RS_Fog:
			SetRS(GL_FOG);
			break;
		case RS_Lighting:
			SetRS(GL_LIGHTING);
			break;
		case RS_ZTest:
			SetRS(GL_DEPTH_TEST);
			break;
		case RS_ZWrite:
			glDepthMask(set);
			break;
		case RS_Blend:
			SetRS(GL_BLEND);
			break;
		case RS_AlphaTest:
			SetRS(GL_ALPHA_TEST);
			break;
		case RS_Smooth:
			if(set)
				glShadeModel(GL_SMOOTH);
			else glShadeModel(GL_FLAT);
			break;
		case RS_Points:
			if(set){
				glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
			}else{
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			}
			break;
		case RS_Wireframe:
			if(set){
				glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			}else{
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			}
			break;
		}
	}

	return true;
}
bool GLDev::getRenderState(RenderStates flag){
	return m_RenderStates[flag]==ES_Use;
}


void GLDev::setViewport(scene::ViewPort* vp)
{
	mrayDev::setViewport(vp);
	if(!vp)
		return;
	//vp->getRenderTarget();
	GLWin32RenderWindow* wnd=(GLWin32RenderWindow*) vp->GetRenderWindow();
	if(wnd)
		switchContext( wnd->GetContext());

	const math::recti& rc = vp->getAbsRenderingViewPort();
	setViewportRect(rc);
}

void GLDev::setViewportRect(const math::rectf& rc)
{
	//currentRendertargetSize=rc.getSize();

	if(m_viewportRect==rc)
		return;

	m_viewportRect=rc;
	GLsizei x, y, w, h;
	w=rc.getWidth();
	h=rc.getHeight();

	x=rc.ULPoint.x;
	y=rc.ULPoint.y;
	glViewport(x,y,w,h);
	setScissorRect(rc);

	_onUpdateViewport();
}
const math::rectf&  GLDev::getViewportRect()
{
	return m_viewportRect;
}
void GLDev::_onUpdateViewport(){
	if( currentRenderMode==RM_2D)
	{
		math::matrix4x4 mat;
		math::vector2di size=getRenderTargetSize();

		mat=math::MathUtil::CreateProjectionMatrixOrtho(0,size.x*0.5,0,size.y*0.5,-1,1);
		//mat.setTranslation(math::vector3d(-1,1,0));
		setTransformationState(TS_PROJECTION,mat);
		mat.makeIdentity();
		setTransformationState(TS_WORLD,mat);
		mat.setTranslation(math::vector3d(-0.5*size.x, -0.5*size.y, 0.0));
		//mat.setTranslation(math::vector3d(0.375, 0.375, 0.0));
		setTransformationState(TS_VIEW,mat);
	}
}


void GLDev::setScissorTest(bool enable)
{
	if(m_enableScissorRect==enable)
		return;
	m_enableScissorRect=enable;
	_SetGLState(GL_SCISSOR_TEST,m_enableScissorRect);
}
bool GLDev::getScissorTest()
{
	return m_enableScissorRect;
}
void GLDev::setScissorRect(const math::rectf& rc)
{
	if(m_scissorRect== rc)
		return;
	//scissor rect defined by the Lower Left point, and size
	//it is necessary to map it to be Upper Left Point
	m_scissorRect= rc;
	if (m_renderTarget )
		glScissor(m_scissorRect.ULPoint.x,m_scissorRect.ULPoint.y,m_scissorRect.getWidth(),m_scissorRect.getHeight());
	else
		glScissor(m_scissorRect.ULPoint.x, m_viewportRect.getHeight() - m_scissorRect.getHeight() - m_scissorRect.ULPoint.y, m_scissorRect.getWidth(), m_scissorRect.getHeight());
}
const math::rectf& GLDev::getScissorRect()
{
	return m_scissorRect;
}
void GLDev::set2DMode()
{
//	if( currentRenderMode!=RM_2D)
	{
		if(currentRenderMode==RM_3D)
		{
			m_3DProjMat=Matrices[TS_PROJECTION];
			m_3DViewMat=Matrices[TS_VIEW];
		}

		currentRenderMode=RM_2D;

		
		math::matrix4x4 mat;
		
		disableTexture(3);
		disableTexture(2);
		disableTexture(1);
		disableTexture(0);

		if(glActiveTextureARB)
			glActiveTextureARB(GL_TEXTURE0_ARB);

		unuseShader();


		setTransformationState(TS_TEXTURE3,mat);
		setTransformationState(TS_TEXTURE2,mat);
		setTransformationState(TS_TEXTURE1,mat);
		setTransformationState(TS_TEXTURE0,mat);


		_SetMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		//_SetMatrixMode(GL_PROJECTION);
		//glLoadIdentity();

		math::vector2di size=getRenderTargetSize();

// 		if (m_renderTarget)
// 			mat = math::MathUtil::CreateProjectionMatrixOrtho(0, size.x*0.5, size.y*0.5, 0, -1, 1);
// 		else
			mat = math::MathUtil::CreateProjectionMatrixOrtho(0, size.x*0.5, 0, size.y*0.5, -1, 1);
		//mat.setTranslation(math::vector3d(-1,1,0));
		setTransformationState(TS_PROJECTION,mat);
		//createGLMatrix(glMat,mat);
		//glLoadMatrixf(glMat);
// 		_SetMatrixMode(GL_MODELVIEW);
// 		glLoadIdentity();
// 		glTranslatef (0.375, 0.375, 0.0); 
		mat.makeIdentity();
		setTransformationState(TS_WORLD,mat);
		
		mat.setTranslation(math::vector3d(-0.5*size.x, -0.5*size.y, 0.0));
 		setTransformationState(TS_VIEW,mat);
		
	
// 		if(currMaterial)
// 			matRenderers[(int)currMaterial->matRenderType]->unset();

		setRenderState(RS_ZTest,false);
		setRenderState(RS_ZWrite,false);
		setRenderState(RS_Fog,false);
		setRenderState(RS_Wireframe,false);
		setRenderState(RS_Lighting,false);
		setRenderState(RS_CullFace,false);
		setRenderState(RS_AlphaTest,false);
		setRenderState(RS_Blend,true);
		_SetAlphaFunc(GL_LEQUAL,1);
/*
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_MODULATE);
		glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_TEXTURE);
		glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_EXT, GL_PRIMARY_COLOR_EXT);
				

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
		glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
		glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
		glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PRIMARY_COLOR_EXT);
		*/
		SetSceneBlending(EBF_SRC_ALPHA, EBF_ONE_MINUS_SRC_ALPHA);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


		setLineWidth(1);
		setPointSize(1);

	}
	
}

void GLDev::set3DMode()
{
	

	if(currentRenderMode!=RM_3D){
		currentRenderMode=RM_3D;
		/*float glmat[16];
		
		createGLMatrix(glmat, m_3DProjMat);
		//glmat[12]*=-1;
		_SetMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glmat);
		
		createGLMatrix(glmat, m_3DViewMat * Matrices[TS_WORLD]);
		_SetMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glmat);
*/

		setTransformationState(TS_PROJECTION,m_3DProjMat);
		setTransformationState(TS_VIEW,m_3DViewMat);
		// Reset Texture Stages
		glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_MODULATE );


		setRenderState(RS_Fog,false);
		setRenderState(RS_Wireframe,false);
		setRenderState(RS_Lighting,false);
		setRenderState(RS_CullFace,false);
		setRenderState(RS_AlphaTest,false);
		setRenderState(RS_Blend,false);
		setRenderState(RS_ZTest,true);
		setRenderState(RS_ZWrite,true);
		//setRenderState(RS_AlphaTest,true);
		setRenderState(RS_Fog,fogEnable);

		glCullFace(GL_BACK);


// 		if(currMaterial)
// 			useMaterial(currMaterial,1);

	}

}

void GLDev::createGLMatrix(float mat[16],const math::matrix4x4&m)
{
	int x=0;
	for(int i=0;i<4;++i)
		for(int j=0;j<4;++j)
		{
			mat[x]=m.m4x4[j][i];
			++x;
		}
}

void GLDev::setTransformationState(ETransformationState state,const math::matrix4x4&mat)
{
	//if(currentRenderMode==RM_3D)
	mrayDev::setTransformationState(state, mat);
	float glmat[16];
	Matrices[state]=mat;

// 	if(!m_currShader.isNull())
// 		return;

// 	if(m_currShader)
// 		return;

	switch(state)
	{
	case TS_VIEW:
	case TS_WORLD:
		createGLMatrix(glmat,Matrices[TS_VIEW]*Matrices[TS_WORLD]);
		_SetMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glmat);
		break;
	case TS_PROJECTION:
		createGLMatrix(glmat,mat);
		//glmat[12]*=-1;
		_SetMatrixMode(GL_PROJECTION);
		// Invert transformed y
		if (m_renderTarget)
		{
			glmat[1] = -glmat[1];
			glmat[5] = -glmat[5];
			glmat[9] = -glmat[9];
			glmat[13] = -glmat[13];
		}
		glLoadMatrixf(glmat);
		_SetMatrixMode(GL_MODELVIEW);
		break;
	case TS_TEXTURE0:
	case TS_TEXTURE1:
	case TS_TEXTURE2:
	case TS_TEXTURE3:

		if(m_caps->isFeatureSupported(EDF_MultiTexture))
			glActiveTextureARB( GL_TEXTURE0_ARB + (state-TS_TEXTURE0));
		createGLMatrix(glmat,mat);
		_SetMatrixMode(GL_TEXTURE);
		glLoadMatrixf(glmat);

		break;
	};
}
void GLDev::getTransformationState(ETransformationState state,math::matrix4x4&out)
{
	out= Matrices[state];
}
video::ITexturePtr  GLDev::createEmptyTexture1D(bool internal)
{
	return new video::GLTexture(ETT_1DTex,internal);
}
video::ITexturePtr  GLDev::createEmptyTexture2D(bool internal)
{
	return new video::GLTexture(ETT_2DTex,internal);
}
video::ITexturePtr  GLDev::createEmptyTexture3D(bool internal)
{
	return new video::GLTexture(ETT_3DTex,internal);
}
video::ITexturePtr  GLDev::createEmptyTextureCube(bool internal)
{
	return new video::GLTexture(ETT_CubeTex,internal);
}
video::ITexturePtr GLDev::createTexture1D(int size,video::EPixelFormat type,bool internal)
{
	video::ITexturePtr tex= createEmptyTexture1D(internal);
	//tex->setTexFileName(name);
	tex->createTexture(math::vector3di(size,1,1),type);
	return tex;
}
video::ITexturePtr GLDev::createTexture2D(const math::Point2di &size,video::EPixelFormat type,bool internal)
{
	video::ITexturePtr tex= createEmptyTexture2D(internal);
	//tex->setTexFileName(name);
	tex->createTexture(math::vector3di(size.x,size.y,1),type);
	return tex;
}
video::ITexturePtr GLDev::createTexture3D(const math::Point3di &size,video::EPixelFormat type,bool internal)
{
	video::ITexturePtr tex= createEmptyTexture3D(internal);
	//tex->setTexFileName(name);
	tex->createTexture(size,type);
	return tex;
}
video::ITexturePtr GLDev::createTextureCube(const math::Point2di &size,video::EPixelFormat type,bool internal)
{
	video::ITexturePtr tex= createEmptyTextureCube(internal);
	//tex->setTexFileName(name);
	tex->createTexture(math::vector3d(size.x,size.y,1),type);
	return tex;
}


void GLDev::useRenderPass(video::RenderPass* mtrl)
{
	FIRE_LISTENR_METHOD(OnMaterialUsed,(this,mtrl));

	mrayDev::useRenderPass(mtrl);
	if(!mtrl ){
		unuseShader();
		for(int i=0;i<m_caps->getMaxTextureUnits();++i){
			disableTexture(i);
		}/*
		for(int i=0;i<RS_RenderStates_Count;++i){
			setRenderState((RenderStates)i,m_DefaultRenderStates[(RenderStates)i]);
		}*/
		return;
	}
	//if(mtrl==currMaterial && !reset)return;
	float color[4];
//	glEnable(GL_DEPTH_TEST);
	
	

	if (mtrl->getRenderState(RS_Blend)){
		SetSceneBlending(mtrl->GetSourceBlendFactor(), mtrl->GetDestinationBlendFactor());
	}
	_SetAlphaFunc(getAlphaFunction(mtrl->GetAlphaFunction()),mtrl->GetAlphaReferenceValue());

//	reset=1;
	

//	for(int i=MaxTextureUnits-1;i>=0;i--){


	for(int i=0;i<m_caps->getMaxTextureUnits();++i){
		if(mtrl->getTexture(i))
		{
			TextureUnit*tex=mtrl->getTextureUnit(i);
			useTexture(i,tex);
		}
		else{
			disableTexture(i);
		}
	}
	glActiveTextureARB(GL_TEXTURE0_ARB );
/*
	if(!currMaterial || (int)currMaterial->matRenderType<matRenderers.size() ){
		if(currMaterial)
			matRenderers[(int)currMaterial->matRenderType]->unset();
		if(matRenderers[(int)mtrl->matRenderType]->preUse()){
			matRenderers[(int)mtrl->matRenderType]->set(mtrl);
		}
	}
	*/
	if(mtrl->getRenderShader())
	{
		useShader(mtrl->getRenderShader());
	}else{
		unuseShader();
	}

	//if(m_currShader.isNull())
	{
		mtrl->GetAmbient().ToArray(color);
		color[3]=mtrl->GetAlpha();
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,color);

		mtrl->GetDiffuse().ToArray(color);
		color[3]=mtrl->GetAlpha();
		glColor4fv(color);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,color);

		mtrl->GetEmissive().ToArray(color);
		color[3]=mtrl->GetAlpha();
		glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,color);
		
		if(mtrl->GetShininess()){
	#ifdef GL_EXT_separate_specular_color
		/*	if(SeparateSpecularColorExt)
				glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);*/
	#endif
			glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,mtrl->GetShininess());
			glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mtrl->GetSpecular().ptr());
		}
	#ifdef GL_EXT_separate_specular_color
	/*	else{
			if(SeparateSpecularColorExt)
			glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
		}*/

	#endif
	}

	EState state;
	for(int i=0;i<RS_RenderStates_Count;++i){
		state=mtrl->getRenderState((RenderStates)i);
		bool s=state==ES_Use? true:false;
		if(state==ES_Unknown)
			s=m_RenderStates[(RenderStates)i];
		setRenderState((RenderStates)i,s);
	}
	_SetFrontFace(mtrl->GetCullFaceDirection()==ECFM_CW ? GL_CW : GL_CCW);

	setLineWidth(mtrl->GetThickness());
	setPointSize(mtrl->GetThickness());



//	currMaterial=mtrl;
}

bool GLDev::useTexture(int stage,video::TextureUnit*texUnit){

	FIRE_LISTENR_METHOD(OnTextureUsed,(this,texUnit));
	ITexture*tex=0;
	if(texUnit)
		tex=texUnit->GetTexture();

	if(tex)
		tex->onUse();

	if(!mrayDev::useTexture(stage,texUnit))
		return true;

	if(!tex){
		disableTexture(stage);
		return true;
	}

	if(tex->getDeviceType()!=m_deviceType){
		gLogManager.log(mT("GLDevice::useTexture() - using non GLTexture!"),tex->getResourceName(),ELL_ERROR);
	}

	if(m_currentPBufferTexture){
		m_currentPBufferTexture->unuse();
		m_currentPBufferTexture=0;
	}
	int id=0;
	GLenum target=0;

	video::GLTexture* gtex=dynamic_cast<video::GLTexture*>(tex);
	id=gtex->getTextureID();
	target=gtex->getTarget();

	if(currTextures[stage] && tex && currTextures[stage]->getType()!=tex->getType())
		useTextureByID(currTextures[stage]->getType(),stage,0);//disable old texture type
	currTextures[stage]=tex;
	if(texUnit && tex){
		math::matrix4x4 mat;
		texUnit->updateMatrix();
		mat=texUnit->getTransformationMatrix();
		setTransformationState((ETransformationState)(TS_TEXTURE0+stage),mat);
		bool res=useTextureByID(target,stage ,id);//try to bind the texture now

		//if successed
		if(res){
			if(m_caps->isFeatureSupported(EDF_Anisotropy))
				glTexParameterf(target,GL_TEXTURE_MAX_ANISOTROPY_EXT,tex->getAnisotropicFilter() ? m_caps->getMaxAnisotropy() : 1.0f);

			glTexParameteri(target,GL_TEXTURE_MIN_FILTER, tex->getMipmapsFilter() ? GL_LINEAR_MIPMAP_LINEAR:GL_LINEAR);
			glTexParameteri(target,GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, texUnit->GetEdgeColor().ptr());
			for(int i=0;i<ETGT_Count;++i){
				bool e=texUnit->isEnableGenMode((ETextureGenType)i);
				if(e){
					//_SetGLState(GL_TEXTURE_GEN_S+i,true);
					glEnable(GL_TEXTURE_GEN_S+i);
					ETextureGenMode mode =texUnit->getTextureGenMode((ETextureGenType)i);
					switch(mode){
					case ETGM_EyeLinear:
						glTexGeni(GL_S+i, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
						break;
					case ETGM_ObjectLinear:
						glTexGeni(GL_S+i, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
						break;
					case ETGM_SphereMap:
						glTexGeni(GL_S+i, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
						break;
					}
				}else{
					//_SetGLState(GL_TEXTURE_GEN_S+i,false);
					glDisable(GL_TEXTURE_GEN_S+i);
				}
			}

			ETextureClamp c=texUnit->getTextureClamp(ETW_WrapR);
			glTexParameterf(target, GL_TEXTURE_WRAP_R, g_ETextureClamMap[c]);
			c=texUnit->getTextureClamp(ETW_WrapS);
			glTexParameterf(target, GL_TEXTURE_WRAP_S, g_ETextureClamMap[c]);
			c=texUnit->getTextureClamp(ETW_WrapT);
			glTexParameterf(target, GL_TEXTURE_WRAP_T, g_ETextureClamMap[c]);
		}
		return res;
	}
	else {
		for(int i=0;i<ETGT_Count;++i){
			glDisable(GL_TEXTURE_GEN_S+i);
		}
		disableTexture(stage);
		return true;
	}

	return true;
}


bool GLDev::useTextureByID(GLenum target,int stage,int TexID){
	if(stage>=m_caps->getMaxTextureUnits())return 0;
	if(glActiveTextureARB)
		glActiveTextureARB(GL_TEXTURE0_ARB+stage);
/*
	GLenum tt;
	switch (type)
	{
	case ETT_1DTex:
		tt=GL_TEXTURE_1D;
		break;
	case ETT_2DTex:
		tt=GL_TEXTURE_2D;
		break;
	case ETT_3DTex:
		tt=GL_TEXTURE_3D;
		break;
	case ETT_CubeTex:
		tt=GL_TEXTURE_CUBE_MAP_ARB;
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_GEN_R);
		break;
	}*/
	if(m_lastTextureType!=0 && target!=m_lastTextureType)
		_SetGLState(m_lastTextureType,false);

	m_lastTextureType=target;
	//_SetGLState(target,TexID!=0);
	if(TexID)
		glEnable(target);
	else glDisable(target);

	if(m_TextureID[stage]==TexID)return 1;
	glBindTexture(target,TexID);

	m_TextureID[stage]=TexID;
	return 1;
}

void GLDev::disableTexture(int stage){
	mrayDev::disableTexture(stage);
	FIRE_LISTENR_METHOD(OnTextureUsed,(this,0));
	if(stage>=m_caps->getMaxTextureUnits())return;
	if(glActiveTextureARB)
		glActiveTextureARB(GL_TEXTURE0_ARB+stage);

	currTextures[stage]=0;

	glDisable(GL_TEXTURE_1D);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_3D);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
// 	_SetGLState(GL_TEXTURE_1D,false);
// 	_SetGLState(GL_TEXTURE_2D,false);
// 	_SetGLState(GL_TEXTURE_3D,false);
// 	_SetGLState(GL_TEXTURE_CUBE_MAP_ARB,false);
//  	glBindTexture(GL_TEXTURE_1D,0);
//  	glBindTexture(GL_TEXTURE_2D,0);
//  	glBindTexture(GL_TEXTURE_3D,0);
//  	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB,0);
	m_TextureID[stage]=0;
}

void GLDev::drawVertex(const math::Point3df*points,int n,const video::SColor& color)
{
	_SetGLClientState(GL_VERTEX_ARRAY,true);
	glVertexPointer(3,GL_FLOAT,sizeof(math::vector3d),points);
	glColor4f(color.R,color.G,color.B,color.A);
	glDrawArrays(GL_POINTS,0,n);
	_SetGLClientState(GL_VERTEX_ARRAY,false);
}
void GLDev::drawLine(const math::vector3d*points,int n,const video::SColor& color)
{
	_SetGLClientState(GL_VERTEX_ARRAY,true);
	glVertexPointer(3,GL_FLOAT,sizeof(math::vector3d),points);
	glColor4f(color.R,color.G,color.B,color.A);
	glDrawArrays(GL_LINE_STRIP,0,n);
	_SetGLClientState(GL_VERTEX_ARRAY,false);
}

void GLDev::setStream(video::IHardwareStreamBuffer* stream){
}
void GLDev::setStreamMap(StreamsData*streams){

	m_vertexPointerSet=true;

	m_useAttributes.resize(0);

	video::StreamsMapIT it=streams->streams.begin();
	for(;it!=streams->streams.end();++it){
		video::IHardwareStreamBuffer* s=it->second;
		const MeshStreamKey& prop=it->first;
		if(!isStreamEnabled(prop.type))continue;

		void*pdata=0;
		if(s->getDeviceType()==m_deviceType){
			pdata=0;
		}else{
			SoftwareStreamBuffer* buff=(SoftwareStreamBuffer*)s;
			pdata=buff->getData(0);
		}
		pdata=static_cast<char*>(pdata)+streams->start*s->getElementSize();

		bool validAttr=0;
		if(m_vertexShader)
		{
			validAttr=m_vertexShader->isAttributeValid(prop.type,prop.index);
		}
		if(s->getDeviceType()==m_deviceType){
			video::GLStreamBuffer* sb=(video::GLStreamBuffer*)s;
			glBindBufferARB(GL_ARRAY_BUFFER_ARB,sb->getBufferID());
		}

		if(validAttr){
			int index=m_vertexShader->getAttributeIndex(prop.type,prop.index);
			glVertexAttribPointerARB(index,s->getComponentsCount(),
				getElementType(s->getElementData()),
				false,//Normalization
				0,pdata);
			glEnableVertexAttribArrayARB(index);

			m_useAttributes.push_back(index);
		}else{
			switch(prop.type){
				case video::EMST_Position:
					glVertexPointer(s->getComponentsCount(),GL_FLOAT,s->getElementSize(),pdata);
					_SetGLClientState(GL_VERTEX_ARRAY,true);
					
					break;
				case video::EMST_Normal:
					_SetGLClientState(GL_NORMAL_ARRAY,true);
						
					glNormalPointer(GL_FLOAT,s->getElementSize(),pdata);
					
					break;
				case video::EMST_BlendIndicies:
					glClientActiveTextureARB(GL_TEXTURE0_ARB+scene::MeshAnimatorManager::getInstance().GetGPUBoneIDStream());
					
					glTexCoordPointer(s->getComponentsCount(),GL_FLOAT,s->getElementSize(),pdata);

					_SetGLClientState(GL_TEXTURE_COORD_ARRAY,true);
					break;
				case video::EMST_BlendWeights:
					glClientActiveTextureARB(GL_TEXTURE0_ARB+scene::MeshAnimatorManager::getInstance().GetGPUBoneWeightStream());
					
					glTexCoordPointer(s->getComponentsCount(),GL_FLOAT,s->getElementSize(),pdata);

					_SetGLClientState(GL_TEXTURE_COORD_ARRAY,true);
					break;
				case video::EMST_Tangent:
// 					glEnableClientState(GL_TANGENT_ARRAY_EXT);
// 					glTangentPointerEXT(GL_FLOAT,s->getElementSize(),s->lock(0,0,IHardwareBuffer::ELO_ReadOnly));
					break;
				case video::EMST_Binormal:
// 					glEnableClientState(GL_BINORMAL_ARRAY_EXT);
// 					glBinormalPointerEXT(GL_FLOAT,s->getElementSize(),s->lock(0,0,IHardwareBuffer::ELO_ReadOnly));
					break;
				case video::EMST_Color:
					_SetGLClientState(GL_COLOR_ARRAY,true);
					
					glColorPointer(s->getComponentsCount(),GL_FLOAT,s->getElementSize(),pdata);
					
					break;
				case video::EMST_Texcoord:
					//glActiveTextureARB(GL_TEXTURE0_ARB+prop.index);
					glClientActiveTextureARB(GL_TEXTURE0_ARB+prop.index);
					m_activeTexture.push_back(prop.index);
					
					glTexCoordPointer(s->getComponentsCount(),GL_FLOAT,s->getElementSize(),pdata);

					_SetGLClientState(GL_TEXTURE_COORD_ARRAY,true);
					break;
			}
		}
	}

/*
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);


	glVertexPointer(3,GL_FLOAT,sizeof(scene::S3DVertex),
		&(&vertices[0])->Pos);
		glColorPointer(4,GL_UNSIGNED_BYTE,sizeof(scene::S3DVertex),
	&(&vertices[0])->color);
	
	glNormalPointer(GL_FLOAT,sizeof(scene::S3DVertex),
		&(&vertices[0])->Normal);

	if(MultiTextureExt){
		switch(vertexType){
		case scene::EVCT_STANDARD:
			glActiveTextureARB(GL_TEXTURE1_ARB);
			glClientActiveTextureARB(GL_TEXTURE1_ARB);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2,GL_FLOAT,sizeof(scene::S3DVertex),&(vertices[0].TCoord));

			glClientActiveTextureARB(GL_TEXTURE0_ARB);
			break;
		case scene::EVCT_COORD2:
			glActiveTextureARB(GL_TEXTURE1_ARB);
			glClientActiveTextureARB(GL_TEXTURE1_ARB);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2,GL_FLOAT,sizeof(scene::S3DVertex),&(vertices[0].TCoord2));

			glClientActiveTextureARB(GL_TEXTURE0_ARB);
			break;
		case scene::EVCT_TANGENTS:
			glClientActiveTextureARB(GL_TEXTURE1_ARB);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(3,GL_FLOAT,sizeof(scene::S3DVertex),&(vertices[0].Tangent));

			glClientActiveTextureARB(GL_TEXTURE2_ARB);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(3,GL_FLOAT,sizeof(scene::S3DVertex),&(vertices[0].Binormal));

			glClientActiveTextureARB(GL_TEXTURE0_ARB);
			break;
		}
	}
	glTexCoordPointer(2,GL_FLOAT,sizeof(scene::S3DVertex),&(vertices[0].TCoord));*/
}
void GLDev::endStreams(){
	_SetGLClientState(GL_VERTEX_ARRAY,false);
	_SetGLClientState(GL_NORMAL_ARRAY,false);
	_SetGLClientState(GL_COLOR_ARRAY,false);
	_SetGLClientState(GL_TANGENT_ARRAY_EXT,false);
	_SetGLClientState(GL_BINORMAL_ARRAY_EXT,false);
	_SetGLClientState(GL_TEXTURE_COORD_ARRAY,false);
	m_vertexPointerSet=false;

	for(int i=0;i<m_useAttributes.size();++i){
		glDisableVertexAttribArrayARB(m_useAttributes[i]);
	}
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,0);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
	m_useAttributes.resize(0);

	for(int i=m_activeTexture.size()-1;i>=0;--i)
	{
		glClientActiveTextureARB(GL_TEXTURE0_ARB+m_activeTexture[i]);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
	}
	m_activeTexture.clear();
}
void GLDev::drawElements(video::IndexData* ib,video::EMeshRenderType rtype){

	if(!ib->indexBuffer)
		return;

	mrayDev::drawElements(ib,rtype);

	void* pBuff=0;
	if(ib->indexBuffer->getDeviceType()==m_deviceType){
		GLIndexBuffer* buff=dynamic_cast<GLIndexBuffer*>(ib->indexBuffer.pointer());
		if(buff){
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,buff->getBufferID());
			pBuff=VBO_BUFFER_OFFSET(ib->firstIndex * buff->getIndexSize());
		}
	}else{
		SoftwareIndexBuffer* buff=dynamic_cast<SoftwareIndexBuffer*>(ib->indexBuffer.pointer());
		pBuff=buff->getData(ib->firstIndex * buff->getIndexSize());
	}
	int cnt=ib->indexCount;

	GLenum primType;

	switch(rtype)
	{
	case video::MR_POINTS:
		primType=GL_POINTS;
		cnt=0;
		break;
	case video::MR_LINES:
		primType=GL_LINES;
		cnt=0;
		break;
	case video::MR_LINE_LOOP:
		primType=GL_LINE_LOOP;
		cnt=0;
		break;
	case video::MR_LINE_STRIP:
		primType=GL_LINE_STRIP;
		cnt=0;
		break;
	case video::MR_TRIANGLES:
		primType=GL_TRIANGLES;
		cnt/=3;
		break;
	case video::MR_TRIANGLE_FAN:
		primType=GL_TRIANGLE_FAN;
		cnt-=2;
		break;
	case video::MR_TRIANGLE_STRIP:
		primType=GL_TRIANGLE_STRIP;
		cnt-=2;
		break;
	case video::MR_QUADS:
		primType=GL_QUADS;
		cnt/=2;
		break;
	case video::MR_QUAD_STRIP:
		primType=GL_QUAD_STRIP;
		break;
	case video::MR_POLYGON:
		primType=GL_POLYGON;
		cnt=1;
		break;
	};
	m_primitivesDrawn+=cnt;

	GLenum idxType=ib->indexBuffer->getIndexType()==IHardwareIndexBuffer::EIT_16Bit ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;

	glDrawElements(primType,ib->indexCount,idxType,pBuff);
//	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,0);
//	glFlush();
}
void GLDev::drawElementsUP(const void*indices,IHardwareIndexBuffer::EIndexType type,uint startIndex,int count,video::EMeshRenderType rtype){

	mrayDev::drawElementsUP(indices,type,startIndex,count,rtype);
	if(!m_vertexPointerSet){
		gLogManager.log(mT("calling drawElements() without setting vertex pointer!"),ELL_ERROR);
		return;
	}
	int cnt=count;

	GLenum idxType=type==IHardwareIndexBuffer::EIT_16Bit ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;


	switch(rtype)
	{
	case video::MR_POINTS:glDrawArrays(GL_POINTS,0,count);
		cnt=0;
		break;
	case video::MR_LINES:glDrawElements(GL_LINES,count,idxType,indices);
		cnt=0;
		break;
	case video::MR_LINE_LOOP:glDrawElements(GL_LINE_LOOP,count,idxType,indices);
		cnt=0;
		break;
	case video::MR_LINE_STRIP:glDrawElements(GL_LINE_STRIP,count,idxType,indices);
		cnt=0;
		break;
	case video::MR_TRIANGLES:glDrawElements(GL_TRIANGLES,count,idxType,indices);
		cnt/=3;
		break;
	case video::MR_TRIANGLE_FAN:glDrawElements(GL_TRIANGLE_FAN,count+2,idxType,indices);
		cnt-=2;
		break;
	case video::MR_TRIANGLE_STRIP:glDrawElements(GL_TRIANGLE_STRIP,count,idxType,indices);
		cnt-=2;
		break;
	case video::MR_QUADS:glDrawElements(GL_QUADS,count,idxType,indices);
		cnt/=2;
		break;
	case video::MR_QUAD_STRIP:glDrawElements(GL_QUAD_STRIP,count+2,idxType,indices);
		break;
	case video::MR_POLYGON:glDrawElements(GL_POLYGON,count,idxType,indices);
		break;
	};
	m_primitivesDrawn+=cnt;

//	glFlush();
}
void GLDev::drawBuffer(StreamsData* streams,IndexData* indices,video::EMeshRenderType rType)
{
	setStreamMap(streams);
	drawElements(indices,rType);
	endStreams();
}

void GLDev::drawSingleMesh(scene::IMeshBuffer* mesh)
{
	
//	glPushMatrix();
/*
	useMaterial(mesh->getMaterial());
	if(mesh->getMaterial()->shader){
		if(mesh->getShaderCallback())
			mesh->getShaderCallback()->setConstants(mesh->getMaterial()->shader);
	}*/
/*
		math::matrix4x4 m2;
		math::matrix4x4 m3;
		m2.setTranslation(mesh->position);
		m2.setRotationDegrees(mesh->rotate);
		if(mesh->scale!=1)
		{
			m3.setScale(mesh->scale);
			m2*=m3;
		}*/
/*		math::matrix4x4 m1;
		getTransformationState(TS_WORLD,m1);

		setTransformationState(TS_WORLD,m1*mesh->getAbsoluteTransformation());

	
		setTranslation(mesh->position);
		setRotation(mesh->rotate);
		setScale(mesh->scale);
*/
		drawBuffer(mesh->getStreamsData(),mesh->getIndexData(),mesh->getRenderType());
		
//	unuseShader();
//	glPopMatrix();
}


const core::string& GLDev::getDeviceType(){
	return m_deviceType;
}

void GLDev::setDepthBias(float linearBias,float slopeBias){

	if (linearBias != 0 || slopeBias != 0)
	{
		_SetGLState(GL_POLYGON_OFFSET_FILL,true);
		_SetGLState(GL_POLYGON_OFFSET_POINT,true);
		_SetGLState(GL_POLYGON_OFFSET_LINE,true);
		glPolygonOffset(-slopeBias, -linearBias);
	}
	else
	{
		_SetGLState(GL_POLYGON_OFFSET_FILL,false);
		_SetGLState(GL_POLYGON_OFFSET_POINT,false);
		_SetGLState(GL_POLYGON_OFFSET_LINE,false);
	}
}

void GLDev::setClearColor(const video::SColor &color)
{
	mrayDev::setClearColor(color);
	if(m_clearColor==color)
		return;
	m_clearColor=color;
	glClearColor(m_clearColor.R,m_clearColor.G,m_clearColor.B,m_clearColor.A);
}
const video::SColor &GLDev::getClearColor(){
	return m_clearColor;
}


void GLDev::addDynamicLight(scene::LightNode*light)
{
	mrayDev::addDynamicLight(light);
	if(lastLightAdded==m_caps->getMaxLightsUnits())return;
	dynamicLights[lastLightAdded] = light;
	
	int lightId=GL_LIGHT0+lastLightAdded;

	lastLightAdded++;

	//if(m_currShader.isNull() && 0)
	{
		_SetMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		float mat[16];
		createGLMatrix(mat, Matrices[TS_VIEW]);
		glLoadMatrixf(mat);

		math::vector3d p=light->getAbsolutePosition();
		float data[4];
		if(light->getType()==scene::LT_PointLight)
		{
			data[0]=p.x;
			data[1]=p.y;
			data[2]=p.z;
			data[3]=1;
			glLightfv(lightId,GL_POSITION,data);
		}else
		{
			data[0]=-p.x;
			data[1]=-p.y;
			data[2]=-p.z;
			data[3]=0;
			glLightfv(lightId,GL_POSITION,data);
			data[0]=-light->getTarget().x;
			data[1]=-light->getTarget().y;
			data[2]=-light->getTarget().z;
			data[3]=1;
			glLightfv(lightId,GL_SPOT_DIRECTION,data);
			
			glLightf(lightId,GL_SPOT_CUTOFF,180);
			glLightf(lightId,GL_SPOT_EXPONENT,0);
		}

		data[0]=light->getAmbient().R;
		data[1]=light->getAmbient().G;
		data[2]=light->getAmbient().B;
		data[3]=light->getAmbient().A;
		glLightfv(lightId,GL_AMBIENT,data);
		
		data[0]=light->getDiffuse().R;
		data[1]=light->getDiffuse().G;
		data[2]=light->getDiffuse().B;
		data[3]=light->getDiffuse().A;
		glLightfv(lightId,GL_DIFFUSE,data);
		
		data[0]=light->getSpecular().R;
		data[1]=light->getSpecular().G;
		data[2]=light->getSpecular().B;
		data[3]=light->getSpecular().A;
		glLightfv(lightId,GL_SPECULAR,data);

		glLightf(lightId,GL_CONSTANT_ATTENUATION,0);
		if(light->getRadius()>0)
			glLightf(lightId,GL_LINEAR_ATTENUATION,1.0f/light->getRadius());
		glLightf(lightId,GL_QUADRATIC_ATTENUATION,0);

		_SetGLState(lightId,true);


		glPopMatrix();
	}
}

void GLDev::disableAllLights()
{
	mrayDev::disableAllLights();
	for(int i=0;i<lastLightAdded;++i)
		_SetGLState(GL_LIGHT0+i,false);
	lastLightAdded=0;
}

int GLDev::getDynamicLightsCount(){
	return lastLightAdded;
}
scene::LightNode* GLDev::getDynamicLight(int index){
	if(index<0 || index>=dynamicLights.size())return 0;
	return dynamicLights[index];
}
void GLDev::setAmbientColor(const video::SColor& color)
{
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,color.ptr());
}


void GLDev::clearBuffer(EDeviceBuffer buff)
{
	int buff2;
	switch(buff){
		case EDB_DEPTH:
			buff2=GL_DEPTH_BUFFER_BIT;
			break;
		case EDB_COLOR:
			buff2=GL_COLOR_BUFFER_BIT;
			break;
		case EDB_ACCUM:
			buff2=GL_ACCUM_BUFFER_BIT;
			break;
		case EDB_STENCIL:
			buff2=GL_STENCIL_BUFFER_BIT;
			break;
		default:return;
	}
	glClear(buff2);
}

video::IRenderTargetPtr GLDev::createMRenderTarget(const core::string&name,const math::Point2di& dim,
											   EPixelFormat format,int buffers,bool shareContexts,bool shareObjects)
{
	GCPtr<video::GLMRenderTarget>rTarget=0;

	static int renderTargetNum=0;

#ifdef GL_EXT_framebuffer_object
	if(m_caps->isFeatureSupported(EDF_RenderToTarget)){
		rTarget=new video::GLMRenderTarget(this,0,buffers);
		rTarget->initialize(dim,shareContexts,shareObjects);
	}
	else
#endif
	{
		gLogManager.log(mT("couldn't create render target, Render Targets Not Supported by this video card!!"),ELL_WARNING);
		return 0;
	}
	//gTextureResourceManager.addResource(rTarget,core::string(mT("renderTarget"))+renderTargetNum);
	++renderTargetNum;

	return rTarget;
}
video::IRenderTargetPtr GLDev::createRenderTarget(const core::string&name,const ITexturePtr& target,
		const IHardwarePixelBufferPtr& depthBuffer,const IHardwarePixelBufferPtr& stencilBuffer,bool depthOnly)
{
	video::IRenderTargetPtr rTarget=0;

	static int renderTargetNum=0;

#ifdef GL_EXT_framebuffer_object
	if(m_caps->isFeatureSupported(EDF_FrameBufferObject)){
		//TODO : 
		GCPtr<video::GLFBORenderTarget>fpoTarget=new video::GLFBORenderTarget(name);
		fpoTarget->initialize(target,depthBuffer,stencilBuffer,depthOnly);
		rTarget=fpoTarget;
	}
	else
#endif
	{
		gLogManager.log(mT("couldn't create render target, Render Targets Not Supported by this video card!!"),ELL_WARNING);
		return 0;
	}
	//gTextureResourceManager.addResource(rTarget,core::string(mT("renderTarget"))+renderTargetNum);
	++renderTargetNum;

	return rTarget;
}

bool GLDev::setRenderTarget(video::IRenderTargetPtr rt,bool clearBackBuffer,bool clearZBuffer,const video::SColor &clearColor,const math::recti* vpRect){

	if(rt==m_renderTarget && !rt.isNull())
		return false;
	mrayDev::setRenderTarget(rt,clearBackBuffer,clearZBuffer,clearColor);
	bool ret=1;
	if(m_renderTarget){
			m_renderTarget->unbind();
			/*
		else{
			glBindTexture(GL_TEXTURE_2D,renderTargetTexture->getTextureID());
			// copy viewport to texture
			glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,renderTargetTexture->getSize().x,renderTargetTexture->getSize().y);
		}*/
	}

	bool changed = false;
		
	if(rt){
		if (!m_renderTarget)
			changed = true;
		m_renderTarget = rt;

		m_renderTarget->bind();

/*		currentRendertargetSize=rt->getSize();
		if(!m_rtViewPort){
			m_tmpViewPort=m_viewPort;
			m_rtViewPort=true;
		}*/
		//glViewport(0,0,currentRendertargetSize.x,currentRendertargetSize.y);
		if(vpRect)
			setViewportRect(*vpRect);
		else
			setViewportRect(math::recti(0, 0, rt->GetSize().x, rt->GetSize().y));

		rt->clear(clearColor,clearBackBuffer,clearZBuffer);
	}
	else
	{
		if (m_renderTarget)
			changed = true;
	/*	if(m_renderTarget){
			m_rtViewPort=false;
			m_renderTarget = 0;
			currentRendertargetSize = 0;
			//glViewport(0,0,m_tmpViewPort.getSize().x,m_tmpViewPort.getSize().y);
			setViewportRect(m_tmpViewPort);
		}*/
		if(m_activeViewport)
		{
			setViewportRect(m_activeViewport->getAbsViewPort());
		}

		glClearColor(clearColor.R,clearColor.G,clearColor.B,clearColor.A);
		GLbitfield mode=0;
		if(clearBackBuffer)
			mode|=GL_COLOR_BUFFER_BIT;
		if(clearZBuffer)
			mode|=GL_DEPTH_BUFFER_BIT;
		glClear(mode);
		m_renderTarget=0;
	}
	if (changed)
	{
		//update projection matrix
		setTransformationState(TS_PROJECTION,Matrices[TS_PROJECTION]);
	}
// 	if (currentRenderMode == RM_2D)
// 		set2DMode();

	return ret;
}
video::IRenderTargetPtr GLDev::getRenderTarget()
{
	return m_renderTarget;
}
void GLDev::drawStencilShadowVolume(math::vector3d *vertices,int vtxCnt,bool zfail){

	if (!vtxCnt)//|| !StencilBuffer
		return;

	// unset last 3d material
	/*
	if (currentRenderMode == RM_3D && currMaterial && currMaterial->matRenderType >= 0
		 && currMaterial->matRenderType < matRenderers.size())
	{
		matRenderers[currMaterial->matRenderType]->unset();
	}
	*/

	// store current OpenGL state
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT |
		GL_POLYGON_BIT | GL_STENCIL_BUFFER_BIT);

	_SetGLState(GL_LIGHTING,false);
	_SetGLState(GL_FOG,false);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE); // no depth buffer writing
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ); // no color buffer drawing
	_SetGLState(GL_STENCIL_TEST,true);
	_SetGLState(GL_CULL_FACE,true);

	_SetGLClientState(GL_VERTEX_ARRAY,true);
	glVertexPointer(3,GL_FLOAT,sizeof(math::vector3d),&vertices[0]);
	glStencilMask(0xff);
	glStencilFunc(GL_ALWAYS, 0, 0xff);
	
	if (!zfail)
	{
		// ZPASS Method

		glCullFace(GL_BACK);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
		glDrawArrays(GL_TRIANGLES,0,vtxCnt);

		glCullFace(GL_FRONT);
		glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
		glDrawArrays(GL_TRIANGLES,0,vtxCnt);
	}
	else
	{
		// ZFAIL Method

		glCullFace(GL_BACK);
		glStencilOp(GL_KEEP, GL_INCR,GL_KEEP);
		glDrawArrays(GL_TRIANGLES,0,vtxCnt);

		glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);
		glCullFace(GL_FRONT);
		glDrawArrays(GL_TRIANGLES,0,vtxCnt);
	}


	_SetGLClientState(GL_VERTEX_ARRAY,false); //not stored on stack
	glPopAttrib();
}

void GLDev::drawStencilShadow(bool clearStencilBuffer,const  video::SColor& shadowColor)
{
	
	/*
	if (!StencilBuffer)
		return;
		*/

	disableTexture(3);
	disableTexture(2);
	disableTexture(1);
	disableTexture(0);

	// store attributes
	glPushAttrib( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT | GL_POLYGON_BIT | GL_STENCIL_BUFFER_BIT );

	_SetGLState( GL_LIGHTING,false );
	_SetGLState(GL_FOG,false);
	glDepthMask(GL_FALSE);

	glShadeModel( GL_FLAT );
	glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );

	_SetGLState(GL_BLEND,true);
	SetSceneBlending(EBF_SRC_ALPHA,EBF_ONE_MINUS_SRC_ALPHA);

	_SetGLState( GL_STENCIL_TEST,true );
	glStencilFunc(GL_NOTEQUAL, 0, 0xff);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	// draw a shadow rectangle covering the entire screen using stencil buffer
	_SetMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glBegin(GL_QUADS);

	glColor4f (shadowColor.R, shadowColor.G , shadowColor.B, shadowColor.A );
	glVertex3f(-1.1f,-1.1f,0.9f);

	glVertex3f(-1.1f, 1.1f,0.9f);

	glVertex3f( 1.1f, 1.1f,0.9f);

	glVertex3f( 1.1f,-1.1f,0.9f);

	glEnd();

	if (clearStencilBuffer){
		glClear(GL_STENCIL_BUFFER_BIT);
		//glClearStencil(0);
	}

	// restore settings
	glPopMatrix();
	glPopAttrib();
}



float GLDev::getPixelDepth(int x,int y){
	float depth;
	glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	return depth;

}
void GLDev::getPixels(const math::rect<int> &rc,std::vector<SColor> &outPixels){
	outPixels.resize(rc.getWidth()*rc.getHeight());
	glReadPixels(rc.ULPoint.x,rc.ULPoint.y,rc.getWidth(),rc.getHeight(),
		GL_RGBA, GL_4_BYTES, &outPixels[0]);
}

void GLDev::loadXMLSettings(xml::XMLElement* elem){
}

xml::XMLElement* GLDev::exportXMLSettings(xml::XMLElement* elem){
	
	return 0;
}

void GLDev::preThreadStart(){
	if(m_activeContext)
		m_activeContext->endCurrent();
}
void GLDev::postThreadStart()
{
	if(m_activeContext)
		m_activeContext->setCurrent();
}

void GLDev::registerThread(){
	if(!m_activeContext)
		return;
	GLContext*c= m_activeContext->duplicate();
	m_contexts.push_back(c);
	c->setCurrent();
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);        
	_SetGLState(GL_COLOR_SUM,true);
	_SetGLState(GL_DITHER,false);

}

void GLDev::SetSceneBlending(EBlendFactor srcFactor, EBlendFactor dstFactor)
{
	/*
	if (srcFactor == EBF_ONE && dstFactor == EBF_ZERO)
		glDisable(GL_BLEND);
	else */glEnable(GL_BLEND);

	glBlendFunc(getGLBlend(srcFactor), getGLBlend(dstFactor));
}

void GLDev::SetSeparateSceneBlending(EBlendFactor srcFactor, EBlendFactor dstFactor, EBlendFactor srcAlphaFactor, EBlendFactor dstAlphaFactor)
{
// 	if (srcFactor == EBF_ZERO && dstFactor == EBF_ONE &&
// 		srcAlphaFactor == EBF_ZERO && dstAlphaFactor == EBF_ONE)
// 	{
// 		glDisable(GL_BLEND);
// 	}
// 	else
	{
		//		glEnable(GL_BLEND);
		if (glBlendFuncSeparate)
			glBlendFuncSeparate(getGLBlend(srcFactor), getGLBlend(dstFactor), getGLBlend(srcAlphaFactor), getGLBlend(dstAlphaFactor));
		 else if(glBlendFuncSeparateEXT)
			glBlendFuncSeparateEXT(getGLBlend(srcFactor), getGLBlend(dstFactor), getGLBlend(srcAlphaFactor), getGLBlend(dstAlphaFactor));
		
	}
}

void GLDev::_SetAlphaFunc(GLenum func,GLclampf c)
{
// 	if(m_alphaFunc==func && m_alphaFuncClamp==c)
// 		return;

	m_alphaFunc=func;
	m_alphaFuncClamp=c;
	glAlphaFunc(func,c);
}
void GLDev::_SetFrontFace(GLenum f)
{

	if (m_renderTarget)
	{
		f = (f==GL_CW )? GL_CCW : GL_CW;
	}

	if(m_frontFace==f)
		return;

	m_frontFace=f;
	glFrontFace(f);
}
void GLDev::unregisterThread(){
}

void GLDev::removeContext(GLContext*c){
	if(c==m_activeContext){
		if(m_mainContext!=m_activeContext)
		{
			switchContext(m_mainContext);
		}else
		{
			m_activeContext->endCurrent();
			m_activeContext=0;
			m_mainContext=0;
		}
	}
	else if(c==m_mainContext)
	{
		switchContext(0);
		m_mainContext=0;
		//m_mainContext=m_activeContext;
	}
}
void GLDev::switchContext(GLContext*c)
{
	if(m_activeContext==c )
		return;
	
	if(m_vertexShader)m_vertexShader->unuse();
	if(m_fragmentShader)m_fragmentShader->unuse();
	if(m_geomShader)m_geomShader->unuse();

	if(m_activeContext)
		m_activeContext->endCurrent();

	m_activeContext=c;
	if(!c)
	{
		return;
	}
	m_activeContext->setCurrent();
	if(!m_activeContext->IsInited())
	{
		_oneTimeInit();
		m_activeContext->SetInited();
	}
	if(m_vertexShader)m_vertexShader->use();
	if(m_fragmentShader)m_fragmentShader->use();
	if(m_geomShader)m_geomShader->use();
}
void GLDev::_oneTimeInit()
{
	if (GLEW_VERSION_1_2)
	{
		// Set nicer lighting model -- d3d9 has this by default
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);        
	}
	if (GLEW_VERSION_1_4)
	{
		_SetGLState(GL_COLOR_SUM,true);
		_SetGLState(GL_DITHER,false);
	}

	// Check for FSAA
	// Enable the extension if it was enabled by the GLSupport
	if (m_support->CheckExtension(mT("GL_ARB_multisample")))
	{
		int fsaa_active = false;
		glGetIntegerv(GL_SAMPLE_BUFFERS_ARB,(GLint*)&fsaa_active);
		if(fsaa_active)
		{
			_SetGLState(GL_MULTISAMPLE_ARB,true);
			gVideoLoggerSystem.log(mT("Using FSAA from GL_ARB_multisample extension."),ELL_INFO);
		}            
	}
}

//////////////////////////////////////////////////////////////////////////
mrayGLDeviceFactory::mrayGLDeviceFactory()
{
}
mrayGLDeviceFactory::~mrayGLDeviceFactory()
{
}
GCPtr<IVideoDevice> mrayGLDeviceFactory::create()
{
	return new GLDev();
}

bool mrayGLDeviceFactory::canCreate(const core::string& type){
	core::string s(type);
	return s.equals_ignore_case(getType());
}
/*
bool mrayGLDeviceFactory::canCreate(EDeviceType type){
	return type==getDeviceType();
}*/

core::string mrayGLDeviceFactory::getType(){
	return GLDev::m_deviceType;
}
/*EDeviceType mrayGLDeviceFactory::getDeviceType(){
	return m_deviceType;
}*/
core::string mrayGLDeviceFactory::getDescription(){
	return mT("OpenGL Device Renderer");
}

void mrayGLDeviceFactory::getConfigParams(OptionContainer& params)
{
	GLSupport::GetInstance()->getConfigParams(params);
}

}
}