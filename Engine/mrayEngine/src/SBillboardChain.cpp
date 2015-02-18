#include "stdafx.h"

#include "SBillboardChain.h"
#include "IViewNode.h"
#include "ISceneManager.h"
#include "IVideoDevice.h"
#include "MeshBufferData.h"
#include "RenderTechnique.h"
#include "RenderPass.h"

namespace mray{
namespace scene{

SBillboardChain::SBillboardChain():MeshRenderableNode(0),m_fStartWidth(1),m_fEndWidth(1),m_fInterval(10)
{
	SetMaxPointCount(2);
	_init();
	m_dirty=true;
}

SBillboardChain::SBillboardChain(int segments,float startWidth,float endWidth):
MeshRenderableNode(0),m_fStartWidth(startWidth),m_fEndWidth(endWidth),m_fInterval(10)
{
	SetMaxPointCount(segments);
	m_currV=0;
	m_deltaV=10.0f/(float)segments;

	_init();
	m_dirty=true;
}
void SBillboardChain::_init()
{
	//m_customRender=true;
	SetUseIdentityMatrix(true);
	MeshBufferData* meshD=m_mesh->addNewBuffer();
	IMeshBuffer* mesh=meshD->getMeshBuffer();

	int segments=m_vPoses.size();
	segments+=segments%2;
	int vCount=2*(segments);


	mesh->createStream(0,video::EMST_Position,video::ESDT_Point3f,vCount,video::IHardwareStreamBuffer::EUT_Dynamic,true);
	mesh->createStream(0,video::EMST_Normal,video::ESDT_Point3f,vCount,video::IHardwareStreamBuffer::EUT_Dynamic,true);
	mesh->createStream(0,video::EMST_Binormal,video::ESDT_Point3f,vCount,video::IHardwareStreamBuffer::EUT_Dynamic,true);
	mesh->createStream(0,video::EMST_Tangent,video::ESDT_Point3f,vCount,video::IHardwareStreamBuffer::EUT_Dynamic,true);

	video::IHardwareStreamBuffer* uvStream=mesh->createStream(0,video::EMST_Texcoord,
		video::ESDT_Point2f,vCount,video::IHardwareStreamBuffer::EUT_Dynamic,true);

	//mesh->getMaterial()->setRenderState(video::RS_Wireframe,video::ES_Use);
	//mesh->getMaterial()->shader=0;
	int icnt=6*(segments-1);
	video::IHardwareIndexBuffer*indBuff=mesh->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit,icnt,video::IHardwareBuffer::EUT_Static,true);
	/*
	video::IndexData* indices= mesh->getIndexData();

	ushort*idx=(ushort*)indBuff->lock(0,0,video::IHardwareBuffer::ELO_Discard);
 	int n=0;
	for(int i=0;i<(icnt);++i){
		int x=2*i;
 		idx[n+0]=n+0;
		idx[n+1]=n+1;
 		n+=2;
 	}
	indBuff->unlock();
	
 	m_vPoses.resize(2*segments);*/
	m_startIndex=0;
	m_fTime=0;
	m_currIndex=0;


	mesh->setRenderType(video::MR_TRIANGLE_STRIP);

	
}
SBillboardChain::~SBillboardChain(){
}

void SBillboardChain::_create()
{

	int segments=m_vPoses.size();
	int vCount=2*(segments);

	MeshBufferData* meshD=m_mesh->getBufferData(0);
	IMeshBuffer* mesh=meshD->getMeshBuffer();
	video::IndexData*indD=mesh->getIndexData();

	video::IHardwareStreamBuffer* posBuf= mesh->getStream(0,video::EMST_Position);
	video::IHardwareStreamBuffer* normBuf=mesh->getStream(0,video::EMST_Normal);
	video::IHardwareStreamBuffer* binBuf=mesh->getStream(0,video::EMST_Binormal);
	video::IHardwareStreamBuffer* tanBuf=mesh->getStream(0,video::EMST_Tangent);
	video::IHardwareStreamBuffer* texBuf=mesh->getStream(0,video::EMST_Texcoord);
	posBuf->SetLength(vCount);
	normBuf->SetLength(vCount);
	binBuf->SetLength(vCount);
	tanBuf->SetLength(vCount);
	texBuf->SetLength(vCount);


	int icnt=6*(segments-1);
	indD->indexCount=icnt;
	indD->indexBuffer->resize(icnt*sizeof(ushort));

	ushort*idx=(ushort*)indD->indexBuffer->lock(0,0,video::IHardwareBuffer::ELO_Discard);
	int n=0;
	for(int i=0;i<(icnt);++i){
		int x=2*i;
		idx[n+0]=n+0;
		idx[n+1]=n+1;
		n+=2;
	}
	indD->indexBuffer->unlock();

	math::vector2df *tc=(math::vector2df *)texBuf->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);

	//m_vPoses.resize(1);
	for(int i=0;i<segments;++i){
		float t=4*(float)i/(float)segments;
		tc[i*2]=math::vector2d(t,0);
		tc[i*2+1]=math::vector2d(t,1);

	}
	texBuf->unlock();
}

void SBillboardChain::setRecordInterval(float interval){
	m_fInterval=interval;
}
float SBillboardChain::getRecordInterval(){
	return m_fInterval;
}


void SBillboardChain::SetStartWidth(float w)
{
	m_fStartWidth=w;
}
void SBillboardChain::SetEndWidth(float w)
{
	m_fEndWidth=w;
}
void SBillboardChain::SetMaxPointCount(int c)
{
	m_vPoses.resize(c);
}

void SBillboardChain::SetPoints(const math::vector3d* pts,int n)
{
	m_vPoses.resize(n);
	for(int i=0;i<n;++i)
		m_vPoses[i]=pts[i];
}

bool SBillboardChain::SetPoint(int i,const math::vector3d& pt)
{
	if(i>=m_vPoses.size())
		return false;
	m_vPoses[i]=pt;
	return true;
}

const math::vector3d& SBillboardChain::GetPoint(int i)
{
	if(i>=m_vPoses.size())
		return math::vector3d::Zero;
	return m_vPoses[i];
}

bool SBillboardChain::preRender(IRenderPass*pass){

	int segments=m_vPoses.size();
	//video::IVideoDevice*dev=getDevice();
	IViewNode* cam=pass->getViewNode();
	if(!cam /*|| ! dev*/)
		return true;

	if(m_dirty)
	{
		m_dirty=false;
		_create();
	}

	IMeshBuffer* mesh=m_mesh->getBuffer(0);
	video::IndexData* indices= mesh->getIndexData();

	video::IHardwareStreamBuffer* vStream=mesh->getStream(0,video::EMST_Position);
	math::vector3df *vert=(math::vector3df *)vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);

	video::IHardwareStreamBuffer* nStream=mesh->getStream(0,video::EMST_Normal);
	math::vector3df *norm=(math::vector3df *)nStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);

	video::IHardwareStreamBuffer* bStream=mesh->getStream(0,video::EMST_Binormal);
	math::vector3df *binorm=(math::vector3df *)bStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);

	video::IHardwareStreamBuffer* tStream=mesh->getStream(0,video::EMST_Tangent);
	math::vector3df *tangent=(math::vector3df *)tStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);

	video::IHardwareStreamBuffer* uvStream=mesh->getStream(0,video::EMST_Texcoord);
	math::vector2df *tc=(math::vector2df *)uvStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);


	const math::matrix4x4 &mat=cam->getViewMatrix();
	//Bf=Cf
	//(mat(0,0), mat(0,1), mat(0,2));
	//math::vector3d camPos=cam->getAbsolutePosition();
	//math::vector3d frontVec=cam->target()-camPos;
 	int cnt=m_currIndex-m_startIndex+1;
 	indices->indexCount=2*cnt-2;
// 	if(cnt>0)
// 		m_vPoses[0]=getAbsolutePosition();
	int n=0;

	float currV=m_currV-(cnt-1)*m_deltaV;

	math::vector3d eye=mat.getAxisVector(2);
	for(int i=0;i<cnt;i+=2){
		//eye vector to beam
		//E=Cp-Bs
		int a=(i+m_startIndex);
		int b=(a+1);

		math::vector3d vStartPoint=m_vPoses[a];
		math::vector3d vEndPoint=m_vPoses[b];

		//beam vector defined by endPoints
		//B = Be - Bs
		math::vector3d beam=vStartPoint-vEndPoint;
		beam.Normalize();
// 
// 		//Cross Produt of beam and eye
// 		//P=B x E
// 		math::vector3d pBeam = eye.crossProduct(beam);
// 
// 		//find up and right vectors
// 		//Bu=(Bf x P) / |Bf x P|
// 		//Br=Bf x Bu
// 		math::vector3d upVec=frontVec.crossProduct(pBeam);
// 		upVec.Normalize();
// 
//		math::vector3d rightVec=frontVec.crossProduct(upVec);
		//Cross Produt of beam and eye
		//P=B x E
		math::vector3d upVec = eye.crossProduct(beam);
		//upVec.set(0,1,0);
		math::vector3d rightVec = upVec.crossProduct(beam);

		//find up and right vectors
		//Bu=(Bf x P) / |Bf x P|
		//Br=Bf x Bu
//		math::vector3d rightVec=eye.crossProduct(upVec);
//		upVec.Normalize();

//		math::vector3d rightVec=frontVec.crossProduct(upVec);
/*
		//calc matrix
		matrix4x4 mBeam;
		
		mBeam(0,0)=rightVec.x;
		mBeam(0,1)=rightVec.y;
		mBeam(0,2)=rightVec.z;

		mBeam(1,0)=upVec.x;
		mBeam(1,1)=upVec.y;
		mBeam(1,2)=upVec.z;

		mBeam(2,0)=frontVec.x;
		mBeam(2,1)=frontVec.y;
		mBeam(2,2)=frontVec.z;

		//create a matrix for each end point

		matrix4x4 mSPoint=mBeam;

		mSPoint(3,0)=vStartPoint.x;
		mSPoint(3,1)=vStartPoint.y;
		mSPoint(3,2)=vStartPoint.z;
		
		matrix4x4 mEPoint=mBeam;

		mEPoint(3,0)=vEndPoint.x;
		mEPoint(3,1)=vEndPoint.y;
		mEPoint(3,2)=vEndPoint.z;
		//mSPoint.transformVect(math::vector3d(-w,0,0),mesh->currFrame[i*2].Pos);
		//mSPoint.transformVect(math::vector3d( w,0,0),mesh->currFrame[i*2+2].Pos);

		//mEPoint.transformVect(math::vector3d(-w,0,0),mesh->currFrame[i*2+3].Pos);
		//mEPoint.transformVect(math::vector3d( w,0,0),mesh->currFrame[i*2+1].Pos);
*/

		float t=1-(float)i/(float)segments;

		tc[n+0]=math::vector2d(currV,0);
		tc[n+1]=math::vector2d(currV,1);
		tc[n+2]=math::vector2d(currV+m_deltaV,0);
		tc[n+3]=math::vector2d(currV+m_deltaV,1);

		currV+=m_deltaV;

		float w=m_fStartWidth*(1-t) + m_fEndWidth*t;
		vert[n+0] =vStartPoint - upVec*w  ;
		vert[n+1] =vStartPoint + upVec*w;
		vert[n+2] =vEndPoint - upVec*w;
		vert[n+3] =vEndPoint + upVec*w;

		norm[n+0]=rightVec;
		norm[n+1]=rightVec;
		norm[n+2]=rightVec;
		norm[n+3]=rightVec;

		binorm[n+0]=upVec;
		binorm[n+1]=upVec;
		binorm[n+2]=upVec;
		binorm[n+3]=upVec;

		tangent[n+0]=beam;
		tangent[n+1]=beam;
		tangent[n+2]=beam;
		tangent[n+3]=beam;

		n+=4;
		if(i==cnt-2){
		}
	}
	vStream->unlock();
	nStream->unlock();
	bStream->unlock();
	tStream->unlock();
	uvStream->unlock();
	
	mesh->recalculateBoundingBox();
	//dev->setTransformationState(video::TS_WORLD,math::matrix4x4::Identity);
	//dev->drawMesh(m_mesh);

	return MeshRenderableNode::preRender(pass);


}
void SBillboardChain::update(float dt){
	// update positions
	return;
	int segments=m_vPoses.size();
	m_fTime+=dt;
	if(m_fTime>m_fInterval){
		m_fTime=0;
//  		if(m_vPoses.size()<segments)
//  			m_vPoses.increaseUsed(1);
// 		math::vector3d*p=m_vPoses.pointer();
// 		for(int i=m_vPoses.size()-1;i>0;i--){
// 			p[i]=p[i-1];
// 			//m_vPoses[i]=lerp<math::vector3d>(m_vPoses[i+1] , m_vPoses[i-1],fSpeed);
		// 		}
		++m_currIndex;

		m_currV+=m_deltaV;

		if(m_currIndex>=2*segments)
		{
			memcpy(&m_vPoses[0],&m_vPoses[segments],segments*sizeof(math::vector3d));
			m_currIndex=segments-1;
			m_startIndex=0;
		}else if(m_currIndex>=segments)
			++m_startIndex;

//		m_vPoses[m_currIndex]=absTransformation.getTranslation();
		
// 		if(m_vPoses.size()>=segments+1)
// 			m_vPoses.resize(segments);
// 		m_vPoses.push_back(getAbsolutePosition());
		
		//m_vPoses[m_vPoses.size()-1]=m_vPoses[m_vPoses.size()-1]+(m_vPoses[m_vPoses.size()-2] - m_vPoses[m_vPoses.size()-1])*fSpeed;

	}

	m_vPoses[m_currIndex]=m_owner->getAbsolutePosition();

	MeshRenderableNode::update(dt);
}

}
}
