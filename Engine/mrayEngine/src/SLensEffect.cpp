

#include "SLensEffect.h"
#include "ISceneManager.h"
#include "SceneHelper.h"
#include "TextureResourceManager.h"
#include "ITexture.h"

namespace mray{
namespace scene{


SLensEffect::SLensEffect(ISceneManager* smngr,const math::vector3d& pos):
	ISceneNode(mT(""),0)
{

	if(smngr){
		SetSceneManager(smngr);
		setDevice(smngr->getDevice());
		loadDefaultTextures();
	}

	setNodeType(EST_EFFECT);

	bbox=math::box3d(-2,2);


	m_renderBoard=new  SBillboardNode(0,1,0,mT(""),0);
	m_renderBoard->SetSceneManager(smngr);
	m_renderBoard->setDevice(getDevice());
	m_renderBoard->setCullingType(scene::SCT_NONE);

	m_renderBoard->getMaterial(0)->setRenderState(video::RS_ZWrite,video::ES_DontUse);
	m_renderBoard->getMaterial(0)->setRenderState(video::RS_ZTest,video::ES_DontUse);
	m_renderBoard->getMaterial(0)->setRenderState(video::RS_Fog,video::ES_DontUse);
//	m_renderBoard->getMaterial()->matRenderType=video::MRT_ADDCOLOR;
	m_renderBoard->getMaterial(0)->setMaterialRenderer(video::MRT_ADDALPHA);
}

SLensEffect::~SLensEffect(){

	clearRings();
}

void SLensEffect::drawLens(const video::SColor& color,const math::vector3d& point,float size,int texIndex,IRenderPass*pass){
	if(texIndex<0 || texIndex>=m_rings.size())return;

	m_renderBoard->setTexture(m_rings[texIndex]);
	m_renderBoard->setSize(size);
	if(m_renderBoard->getMaterial(0)){
		m_renderBoard->getMaterial(0)->Alpha=color.A;
		m_renderBoard->getMaterial(0)->diffuseColor=
			m_renderBoard->getMaterial(0)->ambientColor=color;
	}
	m_renderBoard->position=point;
	m_renderBoard->updateAbsoluteTransformation();

	m_renderBoard->render(pass);
}

void SLensEffect::loadDefaultTextures(){
	clearRings();

	core::string baseName=mT("flare\\flare");
	for(int i=0;i<7;++i){
		core::string texName=baseName+i;
		texName+=mT(".tga");
		video::ITexturePtr  tex= gTextureResourceManager.loadTexture2D(texName);
		if(tex){
			m_rings.push_back(tex);
		//	tex->addRef();
		}
	}
}

void SLensEffect::clearRings(){
	m_rings.clear();
}



void SLensEffect::preRender(IRenderPass*pass){
	if(!getSceneManager())return;
	getSceneManager()->addNodeToRender(this,NRT_TRANSPARENT);
	
	ISceneNode::preRender(pass);
}

void SLensEffect::render(IRenderPass*pass){
	CameraNode*cam=getSceneManager()->getActiveCamera();
	if(!cam)return;

	math::vector3d lensCamPos=cam->getAbsolutePosition()+position();

	math::vector3d screenPos=SceneHelper::getInstance().getScreenCoordFromPos(lensCamPos,cam);

	math::vector2d screenSize=cam->getViewPort().getSize();
//	screenSize*=0.5f;
	math::vector3d normalizedScreenPos=screenPos;
	normalizedScreenPos.x/=screenSize.x;
	normalizedScreenPos.y/=screenSize.y;
	normalizedScreenPos.z=normalizedScreenPos.z ==0 ? 1 : 1.0f/normalizedScreenPos.z;
	normalizedScreenPos=normalizedScreenPos * 2 - 1;

	if(normalizedScreenPos.x<-1.4 || normalizedScreenPos.x>1.4 || normalizedScreenPos.y<-1.4 || normalizedScreenPos.y>1.4 ){
		return;
	}
/*
	if(normalizedScreenPos.x>=-1 && normalizedScreenPos.x<=1 
		&& normalizedScreenPos.y>=-1 && normalizedScreenPos.y<=1 ){
		float depth=device->getPixelDepth(screenPos.x,screenSize.y-screenPos.y);
		if(normalizedScreenPos.z>depth)return;
	}*/

	float Alpha=1;

	if(normalizedScreenPos.x<-1) 
		Alpha += 5*(1.0f + normalizedScreenPos.x);
	else if(normalizedScreenPos.x>1) 
		Alpha -= 5*(normalizedScreenPos.x - 1.0f );
	
	if(normalizedScreenPos.y<-1) 
		Alpha += 5*(1.0f + normalizedScreenPos.y);
	else if(normalizedScreenPos.y>1) 
		Alpha -= 5*(normalizedScreenPos.y - 1.0f );

	if(Alpha <= 0){
		return;
	}


	math::vector3d pos=lensCamPos;
	math::vector3d tocam=cam->getAbsolutePosition() - pos;
	float toCamLength=tocam.Length();


	math::vector3d ptInt=cam->target()-cam->getAbsolutePosition();
	ptInt.Normalize();
	ptInt*=toCamLength;
	ptInt+=cam->getAbsolutePosition();
	
	math::vector3d lightToInt=ptInt-pos;
	float lightToIntLength= lightToInt.Length();


	





#ifndef INTERPOLATE
#define INTERPOLATE(p) pos+lightToInt*p
//	getDevice()->setTransformationState(TS_WORLD,getAbsoluteTransformation());//math::matrix4x4());
	

	drawLens(video::SColor(110,110,110,255*Alpha),INTERPOLATE(0)	,toCamLength*scale().x*1.2	,4,pass);
	drawLens(video::SColor(255,255,255,255*Alpha),INTERPOLATE(0)	,toCamLength*scale().x*0.5	,0,pass);
	drawLens(video::SColor(75,25,0,255*Alpha)	 ,INTERPOLATE(0)	,toCamLength*scale().x*0.4	,1,pass);
	drawLens(video::SColor(17,17,26,25*Alpha)	 ,INTERPOLATE(0.4)	,toCamLength*scale().x*0.4	,5,pass);
	drawLens(video::SColor(12,17,26,25*Alpha)	 ,INTERPOLATE(0.32)	,toCamLength*scale().x*0.1	,5,pass);
	drawLens(video::SColor(25,17,17,25*Alpha)	 ,INTERPOLATE(0.46)	,toCamLength*scale().x*0.2	,5,pass);
	drawLens(video::SColor(15,75,47,33*Alpha)	 ,INTERPOLATE(0.8)	,toCamLength*scale().x*0.12	,5,pass);
	drawLens(video::SColor(25,128,128,255*Alpha) ,INTERPOLATE(0.95)	,toCamLength*scale().x*0.02	,6,pass);
	drawLens(video::SColor(2,128,128,255*Alpha)	 ,INTERPOLATE(1.15)	,toCamLength*scale().x*0.06	,4,pass);
	drawLens(video::SColor(7,75,33,255*Alpha)	 ,INTERPOLATE(1.3)	,toCamLength*scale().x*0.2	,3,pass);
	drawLens(video::SColor(25,75,33,180*Alpha)	 ,INTERPOLATE(1.25)	,toCamLength*scale().x*0.1	,3,pass);
	drawLens(video::SColor(75,50,33,180*Alpha)	 ,INTERPOLATE(1.32)	,toCamLength*scale().x*0.05	,5,pass);
	drawLens(video::SColor(12,25,50,128*Alpha)	 ,INTERPOLATE(1.38)	,toCamLength*scale().x*0.05	,5,pass);
	drawLens(video::SColor(25,50,25,100*Alpha)	 ,INTERPOLATE(1.5)	,toCamLength*scale().x*0.3	,2,pass);
	drawLens(video::SColor(25,50,25,100*Alpha)	 ,INTERPOLATE(1.7)	,toCamLength*scale().x*0.5	,2,pass);

#undef INTERPOLATE
#endif


//	buffer->material->Alpha=oldAlpha;

}

void SLensEffect::update(float dt){
	math::vector3d lensCamPos=position();
	if(getSceneManager() && getSceneManager()->getActiveCamera())
		lensCamPos+=getSceneManager()->getActiveCamera()->getAbsolutePosition();
	bbox.reset(lensCamPos);
	bbox.MinP-=1;
	bbox.MaxP+=1;

	ISceneNode::update(dt);
}

math::box3d SLensEffect::getBoundingBox(){
	return bbox;
}

}
}






