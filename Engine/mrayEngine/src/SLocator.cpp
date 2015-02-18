#include "stdafx.h"
 
#include "SLocator.h"

#include "IVideoDevice.h"
 
namespace mray{
namespace scene{

//normal
SLocator::SLocator(const  mchar* name,IMovable* parent):ISceneNode(name,0,0),frames(0)
{
	//	setName(name);
}

bool SLocator::getFramePos(float frame,int sframe,int eframe,math::vector3d &outPos){

	return math::getValFromFrame<math::vector3d>(&frames->position[0],frames->position.size(),frame,sframe,eframe,outPos);
}
bool SLocator::getFrameOrintation(float frame,int sframe,int eframe,math::quaternion &outOrint){

	if(sframe<0 || sframe>=frames->orintation.size() || 
		eframe<0 || eframe>=frames->orintation.size())
		return 0;
	/*
	math::quaternion q1(frames->rotate[sframe]);
	math::quaternion q2(frames->rotate[eframe]);
	orintation.Slerp(q1,q2,frame*0.01f);
	math::matrix4x4 mat;
	orintation.toMatrix(mat);
	outRot=mat.getRotation();
*/
	outOrint.Slerp(frames->orintation[sframe],frames->orintation[eframe],frame*0.01f);
	return 1;

} 

bool SLocator::getFrameScale(float frame,int sframe,int eframe,math::vector3d &outScale){
	return math::getValFromFrame<math::vector3d>(&frames->scale[0],frames->scale.size(),frame,sframe,eframe,outScale);
} 

//blending

bool SLocator::getFramePos(float frame,int sframe,int eframe,float blendAmount,int blendFrame,
					  math::vector3d &outPos){


	return math::getBlendValFromFrame<math::vector3d>(&frames->position[0],frames->position.size(),frame,sframe,eframe,
		blendAmount,blendFrame,outPos);
}
bool SLocator::getFrameOrintation(float frame,int sframe,int eframe,float blendAmount,int blendFrame,
						 math::quaternion &outOrint){

	if(sframe<0 || sframe>=frames->orintation.size() || 
		eframe<0 || eframe>=frames->orintation.size())
		return 0;
	/*
	math::quaternion q1(frames->rotate[sframe]);
	math::quaternion q2(frames->rotate[eframe]);
	math::quaternion q3(frames->rotate[blendFrame]);
	orintation.Slerp(q1,q2,frame);
	orintation.Slerp(orintation,q3,blendAmount);
	orintation.toEulerAngles(outRot);*/

	outOrint.Slerp(frames->orintation[sframe],frames->orintation[eframe],frame*0.01f);
	outOrint.Slerp(outOrint,frames->orintation[blendFrame],blendAmount*0.01f);
	return 1;
}
bool SLocator::getFrameScale(float frame,int sframe,int eframe,float blendAmount,int blendFrame,
						math::vector3d &outScale){
							
	return math::getBlendValFromFrame<math::vector3d>(&frames->scale[0],frames->scale.size(),frame,sframe,eframe,
		blendAmount,blendFrame,outScale);
}

//normal

void SLocator::setCurrPos(float frame,int sframe,int eframe){
	math::vector3d pos;
	math::getValFromFrame<math::vector3d>(&frames->position[0],frames->position.size(),frame,
		sframe,eframe,pos);
	setPosition(pos);
}

void SLocator::setCurrOrintation(float frame,int sframe,int eframe){
/*	math::getValFromFrame<math::vector3d>(frames->rotate.pointer(),frames->rotate.size(),frame,
		sframe,eframe,rotate);*/
	math::quaternion q;
	getFrameOrintation(frame,sframe,eframe,q);
	setOrintation(q);

}

void SLocator::setCurrScale(float frame,int sframe,int eframe){
	math::vector3d s;
	math::getValFromFrame<math::vector3d>(&frames->scale[0],frames->scale.size(),frame,
		sframe,eframe,s);
	setScale(s);
}
void SLocator::setCurrFrame(float frame,int sframe,int eframe){
	if(!frames)return;
	setCurrPos(frame,sframe,eframe);
	setCurrOrintation(frame,sframe,eframe);
	setCurrScale(frame,sframe,eframe);
}
/// blending

void SLocator::setCurrPos(float frame,int sframe,int eframe,float blendAmount,int blendFrame){
	math::vector3d pos;
	math::getBlendValFromFrame<math::vector3d>(&frames->position[0],frames->position.size(),frame,sframe,eframe,
		blendAmount,blendFrame,pos);
	setPosition(pos);
}

void SLocator::setCurrOrintation(float frame,int sframe,int eframe,float blendAmount,int blendFrame){
	math::quaternion q;
	getFrameOrintation(frame,sframe,eframe,blendAmount,blendFrame,q);
	setOrintation(q);

}

void SLocator::setCurrScale(float frame,int sframe,int eframe,float blendAmount,int blendFrame){
	math::vector3d s;
	math::getBlendValFromFrame<math::vector3d>(&frames->scale[0],frames->scale.size(),frame,sframe,eframe,
		blendAmount,blendFrame,s);
	setScale(s);
}
void SLocator::setCurrFrame(float frame,int sframe,int eframe,float blendAmount,int blendFrame){
	setCurrPos(frame,sframe,eframe,blendAmount,blendFrame);
	setCurrOrintation(frame,sframe,eframe,blendAmount,blendFrame);
	setCurrScale(frame,sframe,eframe,blendAmount,blendFrame);
}


const math::matrix4x4&SLocator::getRelativeTransformation()const
{/*
	math::matrix4x4 mat;
	orintation.toMatrix(mat);
	mat.setTranslation(position);
	math::matrix4x4 m2;
	if(scale!=1){
		m2.setScale(scale);
		mat*=m2;
	}

	orintation.toEulerAngles(rotate);*/
	return ISceneNode::getRelativeTransformation();
//	return mat;
}

void SLocator::updateAbsoluteTransformation(){
	return ISceneNode::updateAbsoluteTransformation();
	/*
	if(parent && sceneMngr && parent!=(GCPtr<ISceneNode> )sceneMngr)
	{

		absTransformation=parent->getAbsoluteTransformation()*getRelativeTransformation();
	}
	else
		absTransformation=getRelativeTransformation();*/
}



void SLocator::render(IRenderPass*pass){

	//ISceneNode::render(pass);
}

GCPtr<SLocator>SLocator::getDuplicate(){
	GCPtr<SLocator>loc=new SLocator(getNodeName().c_str(),getParent());
	loc->SetSceneManager(getSceneManager());
	loc->frames=frames;
	loc->setPosition(getPosition());
	loc->setOrintation(getOrintation());;
	loc->setScale(getScale());

	return loc;
}

}
}