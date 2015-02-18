#include "stdafx.h"


#include "ShaderSemanticTable.h"
#include "LightNode.h"
#include "RenderPass.h"
#include "IViewNode.h"
#include "ITexture.h"



namespace mray{
namespace video{


ShaderSemanticTable::ShaderSemanticTable(){
	m_ViewProjCalc=true;
	m_WorldViewCalc=true;
	m_WorldViewProjCalc=true;

	m_invWorldCalc=true;
	m_invViewCalc=true;
	m_invProjCalc=true;

	m_invViewProjCalc=true;
	m_invWorldViewCalc=true;
	m_invWorldViewProjCalc=true;
	m_time=0;
	m_dtime=0;
	m_viewNode=0;
	m_pass=0;
	m_renderTarget=0;

	m_skeleton=0;
	m_worldMatricies=0;
	m_worldMatriciesCount=0;
}
ShaderSemanticTable::~ShaderSemanticTable(){
}


void ShaderSemanticTable::setWorldMatricies(const math::matrix4x4* m,int cnt)
{
	m_worldMatricies=m;
	m_worldMatriciesCount=cnt;
}
void ShaderSemanticTable::setWorldMatrix(const math::matrix4x4&m){
	m_WorldMatrix=m;

	m_WorldViewCalc=false;
	m_WorldViewProjCalc=false;
	m_invWorldCalc=false;
	m_invWorldViewCalc=false;
	m_invWorldViewProjCalc=false;

}
void ShaderSemanticTable::setViewMatrix(const math::matrix4x4&m){
	m_ViewMatrix=m;
	//m.getTransposeMatrix(m_ViewMatrix);

	m_WorldViewCalc=false;
	m_WorldViewProjCalc=false;
	m_invWorldViewCalc=false;
	m_invWorldViewProjCalc=false;
	m_ViewProjCalc=false;
	m_invViewProjCalc=false;


}
void ShaderSemanticTable::setProjectionMatrix(const math::matrix4x4&m){
	m_ProjectionMatrix=m;

	m_WorldViewProjCalc=false;
	m_invWorldViewProjCalc=false;
	m_ViewProjCalc=false;
	m_invViewProjCalc=false;
}

void ShaderSemanticTable::setPrevWorldMatrix(const math::matrix4x4&m)
{
	m_PrevWorldMatrix=m;
}

void ShaderSemanticTable::setTextureMatrix(int stage,const math::matrix4x4&m){
	
	m_textureMatrix.SetVariable(stage,m);
}

void ShaderSemanticTable::setTextureStage(video::TextureUnit* tex,int stage){

	int c=m_textures.GetVariablesCount();
	m_textures.SetVariable(stage,tex);
	for(int i=c;i<stage;++i)
		m_textures.SetVariable(i,0);
}

void ShaderSemanticTable::setRenderPass(video::RenderPass* m){
	m_pass=m;
}

void ShaderSemanticTable::addLight(scene::LightNode* light){
	if(light->isCastShadows())
		m_lights.AddVariableFront(light);
	else
		m_lights.AddVariableBack(light);
}
void ShaderSemanticTable::disableLights(){
	m_lights.Clear();
}

void ShaderSemanticTable::setViewNode(scene::IViewNode* v){
	m_viewNode=v;
	if(m_viewNode)
	{
		m_PrevViewProjMatrix=v->getProjectionMatrix()*v->getPrevViewMatrix();
		m_PrevViewProjMatrix.getInverse(m_InvPrevViewProjMatrix);
	}
}

void ShaderSemanticTable::setTime(double t){
	m_time=t;
}
void ShaderSemanticTable::setDTime(float t){
	m_dtime=t;
}

void ShaderSemanticTable::setRenderTarget(IRenderTarget* rt){
	m_renderTarget=rt;
}

void ShaderSemanticTable::setViewport(int x,int y,int w,int h){
	m_viewRect.ULPoint.x=x;
	m_viewRect.ULPoint.y=y;

	m_viewRect.BRPoint.x=x+w;
	m_viewRect.BRPoint.y=y+h;
}

void ShaderSemanticTable::setClearColor(const video::SColor &c){
	m_clearColor=c;
}


///

const math::matrix4x4* ShaderSemanticTable::getWorldMatricies()
{
	if(!m_worldMatricies)
		return &m_WorldMatrix;
	return m_worldMatricies;
}

int ShaderSemanticTable::getWorldMatriciesCount()
{
	if(!m_worldMatricies)
		return 1;
	return m_worldMatriciesCount;
}

math::matrix4x4&  ShaderSemanticTable::getWorldMatrix(){
	return m_WorldMatrix;
}
math::matrix4x4&  ShaderSemanticTable::getViewMatrix(){
	return m_ViewMatrix;
}
math::matrix4x4&  ShaderSemanticTable::getProjectionMatrix(){
	return m_ProjectionMatrix;
}


math::matrix4x4& ShaderSemanticTable::getViewProjMatrix(){

	if(!m_ViewProjCalc){
		m_ViewProjCalc=true;
		//math::matrix4x4 tmp;
		m_ViewProjMatrix=m_ProjectionMatrix*m_ViewMatrix;
		//tmp.getTransposeMatrix(m_ViewProjMatrix);
	}
	return m_ViewProjMatrix;
}
math::matrix4x4& ShaderSemanticTable::getWorldViewMatrix(){

	if(!m_WorldViewCalc){
		m_WorldViewCalc=true;
		//math::matrix4x4 tmp;
		m_WorldViewMatrix=m_ViewMatrix*m_WorldMatrix;
	}
	return m_WorldViewMatrix;
}
math::matrix4x4& ShaderSemanticTable::getWorldViewProjectionMatrix(){
	if(!m_WorldViewProjCalc){
		m_WorldViewProjCalc=true;
		m_WorldViewProjectionMatrix=m_ProjectionMatrix*getWorldViewMatrix();
		//tmp.getTransposeMatrix(m_WorldViewProjectionMatrix);
	}
	return m_WorldViewProjectionMatrix;
}

math::matrix4x4&  ShaderSemanticTable::getInvWorldMatrix(){
	if(!m_invWorldCalc){
		m_invWorldCalc=true;
		m_WorldMatrix.getInverse(m_InvWorldMatrix);
	}
	return m_InvWorldMatrix;
}
math::matrix4x4&  ShaderSemanticTable::getInvViewMatrix(){
	if(!m_invViewCalc){
		m_invViewCalc=true;
		m_ViewMatrix.getInverse(m_InvViewMatrix);
	}
	return m_InvViewMatrix;
}
math::matrix4x4&  ShaderSemanticTable::getInvProjectionMatrix(){
	if(!m_invProjCalc){
		m_invProjCalc=true;
		m_ProjectionMatrix.getInverse(m_InvProjectionMatrix);
	}
	return m_InvProjectionMatrix;
}


math::matrix4x4& ShaderSemanticTable::getInvViewProjMatrix(){

	if(!m_invViewProjCalc){
		m_invViewProjCalc=true;
		//math::matrix4x4 tmp;
		getViewProjMatrix().getInverse(m_InvViewProjMatrix);
	}
	return m_InvViewProjMatrix;
}
math::matrix4x4& ShaderSemanticTable::getInvWorldViewMatrix(){
	if(!m_invWorldViewCalc){
		m_invWorldViewCalc=true;
		getWorldViewMatrix().getInverse(m_InvWorldViewMatrix);
	}
	return m_InvWorldViewMatrix;
}
math::matrix4x4& ShaderSemanticTable::getInvWorldViewProjectionMatrix(){
	if(!m_invWorldViewProjCalc){
		m_invWorldViewProjCalc=true;
		getWorldViewProjectionMatrix().getInverse(m_InvWorldViewProjectionMatrix);
	}
	return m_InvWorldViewProjectionMatrix;
}
const math::matrix4x4& ShaderSemanticTable::getPrevWorldMatrix()
{
	return m_PrevWorldMatrix;
}
const math::matrix4x4& ShaderSemanticTable::getPrevViewMatrix()
{
	if(m_viewNode)
		return m_viewNode->getPrevViewMatrix();
	return m_ViewMatrix;;
}
const math::matrix4x4& ShaderSemanticTable::getPrevViewProjMatrix()
{
	return m_PrevViewProjMatrix;
}
const math::matrix4x4& ShaderSemanticTable::getInvPrevViewProjMatrix()
{
	return m_InvPrevViewProjMatrix;
}
math::matrix4x4 ShaderSemanticTable::getPrevWorldViewMatrix()
{
	if(m_viewNode)
		return m_viewNode->getPrevViewMatrix()*m_PrevWorldMatrix;
	else
		return m_ViewMatrix*m_PrevWorldMatrix;
}
math::matrix4x4 ShaderSemanticTable::getInvPrevWorldViewMatrix()
{
	math::matrix4x4 m;
	getPrevWorldViewMatrix().getInverse(m);
	return m;
}
math::matrix4x4 ShaderSemanticTable::getPrevWorldViewProjMatrix()
{
	return m_PrevViewProjMatrix*m_PrevWorldMatrix;
}
math::matrix4x4 ShaderSemanticTable::getInvPrevWorldViewProjMatrix()
{
	math::matrix4x4 m;
	(m_PrevViewProjMatrix*m_PrevWorldMatrix).getInverse(m);
	return m;
}

const math::matrix4x4&  ShaderSemanticTable::getTextureMatrix(int stage){
	math::matrix4x4*m=m_textureMatrix.GetVariable(stage);
	if(!m)
		return math::matrix4x4::Identity;
	return *m;	
}

video::TextureUnit* ShaderSemanticTable::getTextureStage(int stage){
	video::TextureUnit**t=m_textures.GetVariable(stage);
	if(!t)
		return 0;
	if(!*t)
		return 0;
	return (*t);
}

video::RenderPass* ShaderSemanticTable::getRenderPass(){
	return m_pass;
}

int ShaderSemanticTable::getLightsCount(){
	return m_lights.GetVariablesCount();
}
scene::LightNode* ShaderSemanticTable::getLight(int id){
	scene::LightNode**l=m_lights.GetVariable(id);
	if(!l)
		return 0;
	return *l;
}
const math::vector3d& ShaderSemanticTable::getLightDir(int id){
	scene::LightNode**l=m_lights.GetVariable(id);
	if(!l)
		return math::vector3d::Zero;

	return (*l)->getDir();
}

scene::IViewNode* ShaderSemanticTable::getViewNode(){
	return m_viewNode;
}

double ShaderSemanticTable::getTime(){
	return m_time;
}
float ShaderSemanticTable::getDTime(){
	return m_dtime;
}

IRenderTarget* ShaderSemanticTable::getRenderTarget(){
	return m_renderTarget;
}

const math::recti& ShaderSemanticTable::getViewport(){
	return m_viewRect;
}

const video::SColor &ShaderSemanticTable::getClearColor(){
	return m_clearColor;
}

video::ITexture* ShaderSemanticTable::getShadowMap(int id){
	scene::LightNode*light=getLight(id);
	if(!light)
		return 0;
	if(!light->getShadowMap())
		return 0;
	return light->getShadowMap()->GetColorTexture();
}

math::vector2di ShaderSemanticTable::getShadowMapSize(int id){
	video::ITexture*map=getShadowMap(id);
	if(!map)
		return 1;
	return math::vector2di(map->getSize().x,map->getSize().y);
}

const math::matrix4x4& ShaderSemanticTable::getLightViewProj(int id){
	scene::LightNode*light=getLight(id);
	if(!light)
		return math::matrix4x4::Identity;
	return light->getShadowViewProjMatrix();
}

}
}
