#include "stdafx.h"


#include "mrayDev.h"
#include "FPSCalc.h"
#include "ITimer.h"
#include "ShaderSemanticTable.h"
#include "BenchmarkItem.h"
#include "Engine.h"
#include "ViewPort.h"

namespace mray{
namespace video{


mrayDev::mrayDev()
{
	m_vertexShader=0;
	m_geomShader=0;
	m_fragmentShader=0;
	m_benchmarkItem=new BenchmarkItem(mT("Device"));


	for(int i=0;i<RS_RenderStates_Count;++i)
		m_dirtyRenderStates[i]=true;
	for (int i=0;i<EMST_StreamsCount;++i)
	{
		m_enabledStreams[i]=true;
	}

	m_clipPlanesDirty=false;

	Engine::getInstance().getRootBenchmarking()->addSubItem(m_benchmarkItem);
}

mrayDev::~mrayDev()
{
	Engine::getInstance().getRootBenchmarking()->removeSubItem(m_benchmarkItem);
	delete m_benchmarkItem;
}


void mrayDev::begin(bool clearColorBuf,bool clearDepthBuf)
{
	resetStatistics();
}

void mrayDev::setClearColor(const video::SColor &color){
	ShaderSemanticTable::getInstance().setClearColor(color);
}

void mrayDev::setTransformationState(ETransformationState state,const math::matrix4x4&mat){
	switch(state){
		case TS_WORLD:
			ShaderSemanticTable::getInstance().setWorldMatrix(mat);
			break;
		case TS_VIEW:
			ShaderSemanticTable::getInstance().setViewMatrix(mat);
			break;
		case TS_PROJECTION:

			if (ShaderSemanticTable::getInstance().getRenderTarget())
			{
				//flip y axis
				math::matrix4x4 m(mat);
				m.f12 = -m.f12;
				m.f22 = -m.f22;
				m.f32 = -m.f32;
				m.f42 = -m.f42;
				ShaderSemanticTable::getInstance().setProjectionMatrix(m);
			}
			else
			{
				ShaderSemanticTable::getInstance().setProjectionMatrix(mat);
			}
			break;
		case TS_TEXTURE0:
		case TS_TEXTURE1:
		case TS_TEXTURE2:
		case TS_TEXTURE3:
			ShaderSemanticTable::getInstance().setTextureMatrix(state-TS_TEXTURE0,mat);
			break;
	}
}

void mrayDev::addDynamicLight(scene::LightNode*light){
	ShaderSemanticTable::getInstance().addLight(light);
}
void mrayDev::disableAllLights(){
	ShaderSemanticTable::getInstance().disableLights();
}
bool mrayDev::setRenderTarget(IRenderTargetPtr rt,bool clearBackBuffer,bool clearZBuffer,const video::SColor &clearColor){
	ShaderSemanticTable::getInstance().setRenderTarget(rt);
	return true;
}

void mrayDev::useRenderPass(video::RenderPass* mtrl){
	ShaderSemanticTable::getInstance().setRenderPass(mtrl);
}
bool mrayDev::useTexture(int stage,video::TextureUnit*tex){
	if(stage>=currTextures.size())
		currTextures.resize(stage+1);
	if(tex && currTextures[stage]==tex->GetTexture())
		return false;
	ShaderSemanticTable::getInstance().setTextureStage(tex,stage);
	return true;
}
void mrayDev::disableTexture(int stage){
	ShaderSemanticTable::getInstance().setTextureStage(0,stage);
}


bool mrayDev::useShader(GPUShader* shader)
{
	if(!shader)
	{
		unuseShader();
		return true;
	}
	setVertexShader(shader->GetVertexShader());
	setGeometryShader(shader->GetGeometryShader());
	setFragmentShader(shader->GetFragmentShader());
	return true;
}
bool mrayDev::setVertexShader(IGPUShaderProgram* shader){

	if(m_vertexShader==shader)
		return false;
	if(m_vertexShader)
		m_vertexShader->unuse();
	m_vertexShader=shader;
	if(m_vertexShader)
		m_vertexShader->use();

	FIRE_LISTENR_METHOD(OnShaderUsed,(this,shader));
	return true;
}
bool mrayDev::setFragmentShader(IGPUShaderProgram* shader){

	if(m_fragmentShader==shader)
		return false;
	if(m_fragmentShader)
		m_fragmentShader->unuse();
	m_fragmentShader=shader;
	if(m_fragmentShader)
		m_fragmentShader->use();
	FIRE_LISTENR_METHOD(OnShaderUsed,(this,shader));
	return true;
}
bool mrayDev::setGeometryShader(IGPUShaderProgram* shader){

	if(m_geomShader==shader)
		return false;
	if(m_geomShader)
		m_geomShader->unuse();
	m_geomShader=shader;
	if(m_geomShader)
		m_geomShader->use();
	FIRE_LISTENR_METHOD(OnShaderUsed,(this,shader));
	return true;
}
void mrayDev::unuseShader(){
	setVertexShader(0);
	setGeometryShader(0);
	setFragmentShader(0);
}

void mrayDev::setViewport(scene::ViewPort* vp)
{
	m_activeViewport=vp;
	math::rectf rc;
	if(m_activeViewport)
		rc=m_activeViewport->getAbsViewPort();
	ShaderSemanticTable::getInstance().setViewport(rc.ULPoint.x,rc.ULPoint.y,rc.getWidth(),rc.getHeight());
}

void mrayDev::loadXMLSettings(xml::XMLElement* elem){
}
xml::XMLElement*  mrayDev::exportXMLSettings(xml::XMLElement* elem){
	return 0;
}


bool  mrayDev::setRenderState(RenderStates flag,bool set){

	if(!m_dirtyRenderStates[flag] && m_RenderStates[flag]==(set ? ES_Use : ES_DontUse))
		return false;

	m_RenderStates[flag]=set ? ES_Use : ES_DontUse;
	m_dirtyRenderStates[flag]=false;

	return true;
}
bool mrayDev::getRenderState(RenderStates flag){
	return m_RenderStates[flag]==ES_Use;
}

void mrayDev::SetClipPlane(int i,const math::Plane&p)
{
	if(i>=m_clipPlanes.size())
		m_clipPlanes.resize(i+1);
	m_clipPlanes[i]=p;
	m_clipPlanesDirty=true;
}
void mrayDev::AddClipPlane(const math::Plane&p)
{
	m_clipPlanes.push_back(p);
	m_clipPlanesDirty=true;
}
void mrayDev::RemoveClipPlane(int i)
{
	if(i>=m_clipPlanes.size())
		return;
	m_clipPlanes.erase(m_clipPlanes.begin()+i);
	m_clipPlanesDirty=true;
}
void mrayDev::ClearClipPlanes()
{
	m_clipPlanes.clear();
	m_clipPlanesDirty=true;
}

void mrayDev::drawElements(video::IndexData* ib,video::EMeshRenderType rtype){
	m_batchesDrawn++;
}
void mrayDev::drawElementsUP(const void*indices,IHardwareIndexBuffer::EIndexType type,uint startIndex,int count,video::EMeshRenderType rtype){
	m_batchesDrawn++;
}


void mrayDev::enableStream(EMeshStreamType stream,bool e)
{
	m_enabledStreams[stream]=e;
}
bool mrayDev::isStreamEnabled(EMeshStreamType stream)
{
	return m_enabledStreams[stream];
}

}
}



