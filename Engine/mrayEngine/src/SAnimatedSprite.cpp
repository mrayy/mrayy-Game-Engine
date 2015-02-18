#include "stdafx.h"

#include "SAnimatedSprite.h"
#include "IVideoDevice.h"
#include "MeshBufferData.h"
#include "RenderTechnique.h"
#include "RenderPass.h"


namespace mray{
namespace scene{

SAnimatedSprite::SAnimatedSprite(const video::ITexturePtr& tex,int rowCnt,int colCnt):
MeshRenderableNode(0),rows(rowCnt),cols(colCnt),frame(0)
{
	if(rows==0)rows=1;

	StartUp();
	setTexture(tex);
}
SAnimatedSprite::~SAnimatedSprite(){
}

void SAnimatedSprite::StartUp(){

	IMeshBuffer*mBuff=m_mesh->addNewBuffer()->getMeshBuffer();


	video::IHardwareStreamBuffer* vStream=mBuff->createStream(0,video::EMST_Position,
		video::ESDT_Point3f,4,video::IHardwareStreamBuffer::EUT_Static,true);
	video::IHardwareStreamBuffer* nStream=mBuff->createStream(0,video::EMST_Normal,
		video::ESDT_Point3f,4,video::IHardwareStreamBuffer::EUT_Static,true);
	video::IHardwareStreamBuffer* uvStream=mBuff->createStream(0,video::EMST_Texcoord,
		video::ESDT_Point2f,4,video::IHardwareStreamBuffer::EUT_Dynamic,true);

	math::vector3df *vert=(math::vector3df *)vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	math::vector3df *norm=(math::vector3df *)nStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	math::vector2df *tc=(math::vector2df *)uvStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);



	math::box3d b;

	b.MinP.set(-0.5,0,-0.5);
	b.MaxP.set(0.5,0,0.5);

	mBuff->setBoundingBox(b);

	vert[0]=math::vector3d(-0.5,-0.5,0);
	vert[1]=math::vector3d(0.5,-0.5,0);
	vert[2]=math::vector3d(0.5,0.5,0);
	vert[3]=math::vector3d(-0.5,0.5,0);

	norm[0]=math::vector3d(0,0,1);
	norm[1]=math::vector3d(0,0,1);
	norm[2]=math::vector3d(0,0,1);
	norm[3]=math::vector3d(0,0,1);

	tc[0]=math::vector2d(0,0);
	tc[1]=math::vector2d(1,0);
	tc[2]=math::vector2d(1,1);
	tc[3]=math::vector2d(0,1);

	uint Indices[6];

	Indices[0]=0;
	Indices[1]=2;
	Indices[2]=1;
	Indices[3]=0;
	Indices[4]=3;
	Indices[5]=2;
	video::IHardwareIndexBuffer*indBuff=mBuff->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit,6,video::IHardwareIndexBuffer::EUT_StaticWriteOnly,false);
	indBuff->writeData(0,indBuff->getSizeInBytes(),Indices);


	mBuff->setRenderType(video::MR_TRIANGLES);



}


void SAnimatedSprite::setTexture(const video::ITexturePtr& tex){
	m_mesh->getBufferData(0)->getMaterial()->GetActiveTechnique()->GetPassAt(0)->setTexture(tex,0);
//	if(tex)tex->subRef();
}
video::ITexture *SAnimatedSprite::getTexture(){
	return m_mesh->getBufferData(0)->getMaterial()->GetActiveTechnique()->GetPassAt(0)->getTexture(0);
}
void SAnimatedSprite::setRowsCount(int rowCnt){
	rows=rowCnt;
	if(rows==0)rows=1;
}
int SAnimatedSprite::getRowsCount(){
	return rows;
}
void SAnimatedSprite::setColsCount(int colCnt){
	cols=colCnt;
	if(cols==0)cols=1;
}
int SAnimatedSprite::getColsCount(){
	return cols;
}
void SAnimatedSprite::setFrame(float f){
	frame=f;
}
float SAnimatedSprite::getFrame(){
	return frame;
}

void SAnimatedSprite::update(float dt){
	if(!m_mesh)return;
	MeshBufferData* mbuffD=m_mesh->getBufferData(0);
	IMeshBuffer* mbuff=mbuffD->getMeshBuffer();
	video::ITexture*t=getTexture();
	if(t!=0 && rows!=0 && cols!=0){
		
		frame=fmod(frame,100.0f);
		int currTex=(float)(frame/100.0f)*rows*cols;
		float USize=(float)1.0f/(float)cols;
		float VSize=(float)1.0f/(float)rows;
		int x=currTex % cols;
		int y=currTex / cols;
		float currU=USize*x;
		float currV=1-VSize*y;

		video::IHardwareStreamBuffer* uvStream=mbuff->getStream(0,video::EMST_Texcoord);
		math::vector2df *tc=(math::vector2df *)uvStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
		

		tc[0]=math::vector2d(currU,currV);
		tc[1]=math::vector2d(currU+USize,currV);
		tc[2]=math::vector2d(currU+USize,currV+VSize);
		tc[3]=math::vector2d(currU,currV+VSize);
	}
	MeshRenderableNode::update(dt);	
}

}
}
