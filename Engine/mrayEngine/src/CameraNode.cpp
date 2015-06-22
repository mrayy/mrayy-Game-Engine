
#include "stdafx.h"
#include "CameraNode.h"
#include "IVideoDevice.h"
#include "ISceneManager.h"

#include "IEventReciver.h"
#include "SystemEvent.h"

#include "EventQueue.h"
#include "Engine.h"

#include "mathUtil.h"
#include "ISceneVisitor.h"
#include "EnumManager.h"
#include "ISceneObjectsDesc.h"

namespace mray{
namespace scene{

	
	class CameraEventHandler:public IEventReciver
	{
		std::list<CameraNode*> m_cameras;
	public:
		CameraEventHandler(){
		}

		void addCamera(CameraNode *cam){
			m_cameras.push_back(cam);
		}
		void removeCamera(CameraNode *cam){
			std::list<CameraNode*>::iterator it= m_cameras.begin();
			std::list<CameraNode*>::iterator end= m_cameras.end();
			for (;it!=end;++it)
			{
				if((*it)==cam){
					m_cameras.erase(it);
					return;
				}
			}
		}

		virtual void onEvent(Event*event)
		{
			/*if(event->getType()==ET_SystemEvent){
				SystemEvent* e=dynamic_cast<SystemEvent*>(event);
				if(e->m_eventType==ESET_Resized){
					core::IteratorPair<std::list<CameraNode*>> p(m_cameras);
					for (;!p.done();p++)
					{
						(*p)->onAspectChanged();
					}
				}
			}*/
		}
	};

	CameraEventHandler g_cameraEventHandler;
	int g_cameraEventHandlerInited=false;

CameraNode::CameraNode(const  core::string&name,int id,ISceneManager* sceneMngr)
	:ISceneNode(name,id,sceneMngr),m_reflection(false),m_customViewNode(0),m_autoUpdateAspect(false)
{
	m_useTarget=PropertyTypeUseTarget::instance.GetDefaultValue();
	m_target=PropertyTypeTarget::instance.GetDefaultValue();
	m_vUp=PropertyTypeUpVector::instance.GetDefaultValue();

	m_projectionType=EPT_Perspective;

	m_autoUpdateAspect=PropertyTypeAutoUpdateAspect::instance.GetDefaultValue();

	m_lookAtTarget=0;

	m_fovY=45.0f; 
	m_aspect=4.0f/3.0f;
	m_ZNear=PropertyTypeZNear::instance.GetDefaultValue();
	m_ZFar=PropertyTypeZFar::instance.GetDefaultValue();
	//video::IVideoDevice* dev=m_sceneMngr->getDevice();
	m_rWindow.ULPoint=0;
	/*if(dev)
	{
		m_rWindow.BRPoint.x=dev->getScreenSize().x;
		m_rWindow.BRPoint.y=dev->getScreenSize().y;
		setAspect((float)m_rWindow.BRPoint.x/(float)m_rWindow.BRPoint.y);
	}*/

	calcProjectionMatrix();
	calcViewArea();

	fillProperties();

	m_normalRP=new NormalRenderPass();
	m_autoUpdateProjection = true;
	m_autoUpdateView = true;
/*
	if(!g_cameraEventHandlerInited){
		g_cameraEventHandlerInited=true;
		EventQueue::getInstance().addEventHandler(&g_cameraEventHandler,Event::getEventMaskingValue(ET_SystemEvent));
	}

	g_cameraEventHandler.addCamera(this);*/
}

CameraNode::~CameraNode(){
	g_cameraEventHandler.removeCamera(this);
}
int CameraNode::GetNodeType()
{
	return ENodeType_Camera;
}
void CameraNode::GetRenderPasses(std::list<IRenderPass*>& plst)
{
	plst.push_back(m_normalRP);
}
bool CameraNode::setUpVector(const math::vector3d& val){
	m_vUp=val;
	return true;
}
const math::vector3d &CameraNode::getUpVector()const
{
	if(m_reflection)
	{
		//m_transformedUp=(math::vector4d(m_vUp.x,m_vUp.y,m_vUp.z,1)*m_reflectionMatrix).XYZ();
		return m_transformedUp;
	}
	return m_vUp;
}
const math::vector3d &CameraNode::getUpVector()
{
	if(m_reflection)
	{
		//m_transformedUp=(math::vector4d(m_vUp.x,m_vUp.y,m_vUp.z,1)*m_reflectionMatrix).XYZ();
		m_transformedUp=m_reflectionMatrix*m_vUp;
		return m_transformedUp;
	}
	return m_vUp;
}

const math::vector3d &CameraNode::getTarget()const
{
	if(m_reflection)
	{
		//m_transformedTarget=(math::vector4d(m_target.x,m_target.y,m_target.z,1)*m_reflectionMatrix).XYZ();
		return m_transformedTarget;
	}
	return m_target;
}
const math::vector3d &CameraNode::getTarget()
{
	if(m_reflection)
	{
		//m_transformedTarget=(math::vector4d(m_target.x,m_target.y,m_target.z,1)*m_reflectionMatrix).XYZ();
		m_transformedTarget=m_reflectionMatrix*m_target;
		return m_transformedTarget;
	}
	return m_target;
}
bool  CameraNode::setTarget(const math::vector3d& t){
	m_target=t;
	return true;
}
void CameraNode::fillProperties()
{
	CPropertieDictionary* dic=0;
	if(CreateDictionary(&dic))
	{
		PropertyTypeProjectionType::instance.SetEnum(EnumManager::getInstance().getEnumInfo(mT("EProjectionType")));

		dic->addPropertie(&PropertyTypeProjectionType::instance,mT("Camera"));
		dic->addPropertie(&PropertyTypeFovY::instance,mT("Camera"));	
		dic->addPropertie(&PropertyTypeZNear::instance,mT("Camera"));
		dic->addPropertie(&PropertyTypeZFar::instance,mT("Camera"));
		dic->addPropertie(&PropertyTypeUseTarget::instance,mT("Camera"));
		dic->addPropertie(&PropertyTypeTarget::instance,mT("Camera"));
		dic->addPropertie(&PropertyTypeUpVector::instance,mT("Camera"));
		dic->addPropertie(&PropertyTypeAspect::instance,mT("Camera"));
		dic->addPropertie(&PropertyTypeAutoUpdateAspect::instance,mT("Camera"));
	}
}

math::vector3d CameraNode::ScreenToWorld(const math::vector2d& pos,float depth)
{
	math::matrix4x4 invMat;
	math::vector2d projPt;
	(m_projectionMatrix*m_viewMatrix).getInverse(invMat);
	projPt=(((pos-m_rWindow.ULPoint)*2.0f)/m_rWindow.getSize())-1 ;
	projPt.y=-projPt.y;
	math::vector4d projPos=invMat*math::vector4d(projPt.x,projPt.y,depth,1);
	projPos/=projPos.w;
	return math::vector3d(projPos.x,projPos.y,projPos.z);
}

math::vector3d CameraNode::WorldToScreen(const math::vector3d& pos)
{
	math::matrix4x4 mat=m_projectionMatrix*m_viewMatrix;
	math::vector4d proj=mat*math::vector4d(pos.x,pos.y,pos.z,1);
	proj/=proj.w;
	proj.y=-proj.y;
	proj.x=(proj.x/*+1*/)*m_rWindow.getWidth()*0.5f + m_rWindow.ULPoint.x;
	proj.y=(proj.y/*+1*/)*m_rWindow.getHeight()*0.5f + m_rWindow.ULPoint.y;
	return math::vector3d(proj.x,proj.y,proj.z);
}

bool CameraNode::isReflect()const
{
	return m_reflection;
}
void CameraNode::enableReflection(const math::Plane& p)
{
	m_reflection=true;
	m_reflectionPlane=p;
	m_reflectionMatrix=math::MathUtil::CreateRefelectionMatrix(m_reflectionPlane);
	updateView();
}
void CameraNode::disableReflection()
{
	m_reflection=false;
	updateView();
}
const math::Plane& CameraNode::getReflectionPlane()const
{
	return m_reflectionPlane;
}
const math::matrix4x4& CameraNode::getReflectionMatrix()const
{
	return m_reflectionMatrix;
}

void CameraNode::calcViewArea()
{
	math::matrix4x4 mat = m_projectionMatrix*m_viewMatrix;
	m_viewArea.viewPos = getAbsolutePosition();
	m_viewArea.set(mat);
}
void CameraNode::calcProjectionMatrix()
{
	if (!m_autoUpdateProjection)
		return;
	if(m_projectionType==EPT_Perspective)
		m_projectionMatrix= math::MathUtil::CreateProjectionMatrixPerspectiveFov(math::toRad(m_fovY),m_aspect,m_ZNear,m_ZFar);
	else{
		float w=0.5*m_aspect;//m_rWindow.getHeight()*m_aspect * 0.5f;
		float h=0.5;//m_rWindow.getHeight() * 0.5f;
		m_projectionMatrix=math::MathUtil::CreateProjectionMatrixOrtho(-w,w,-h,h,m_ZNear,m_ZFar);
	}
//	projectionMatrix(0)*=-1;
}

void CameraNode::PositionCamera(math::vector3d pos,math::vector3d vTarget,math::vector3d  up)
{
	setPosition(pos);
	m_target=vTarget;
	m_vUp=up;
//	calcProjectionMatrix();
}
void CameraNode::SetWindow(int x,int y,int w,int h)
{
	m_rWindow.ULPoint.x=x;
	m_rWindow.ULPoint.y=y;
	m_rWindow.BRPoint.x=x+w;
	m_rWindow.BRPoint.y=y+h;

	if(m_autoUpdateAspect)
	{
		onAspectChanged(w,h);
	}
}


void CameraNode::updateView()
{
	m_prevViewMatrix=m_viewMatrix;

	math::vector3d pos=ISceneNode::getAbsolutePosition();

	//math::vector3d vTar;

	if(!m_useTarget){
	//	vTar=target;
	/*	target=pos+math::vector3d(0).TranslateByAngles(math::vector3d(0,0,1),rotate());
		vUp=math::vector3d(0).TranslateByAngles(math::vector3d(0,1,0),rotate());*/

		m_target=pos+ISceneNode::getAbsoluteOrintation()*math::vector3d::ZAxis;
		m_vUp=ISceneNode::getAbsoluteOrintation()*math::vector3d::YAxis;
	}
	else{
		if(m_lookAtTarget){
			m_target=m_lookAtTarget->getAbsolutePosition();
		}
		//here we calculate targetVector depending on rotation angles
		//vTar=pos;
	}
	math::vector3d up=m_vUp;
//	up.Normalize();

//	math::vector3d tg=target;
	//tg.Normalize();

	math::vector3d dir=m_target-pos;
	dir.Normalize();

	float dp=dir.dotProduct(up);
	if ((dp > -1.0001f && dp < -0.9999f) ||
		(dp < 1.0001f && dp > 0.9999f))
		up.x += 0.5f;

	if (m_autoUpdateView)
	{
		m_viewMatrix = math::MathUtil::CreateLookAtMatrix(pos, pos + dir, up);
	}
	//viewMatrix=math::MathUtil::CreateLookAtMatrix(pos,ISceneNode::getOrintation());
	if(m_reflection)
		m_viewMatrix=m_viewMatrix*m_reflectionMatrix;
	calcViewArea();
}
void CameraNode::preRender()
{
	/*
	if(!isVisible())
		return;

	m_visibleNodes.clear();

	//updateView();
*/
	ISceneNode::preRender();

} 

void CameraNode::render(IRenderPass*pass)
{
	video::IVideoDevice*dev=m_sceneMngr->getDevice();
	if(!dev || !isVisible())return;
	dev->setTransformationState(video::TS_PROJECTION,m_projectionMatrix);
	dev->setTransformationState(video::TS_VIEW,m_viewMatrix);


	ISceneNode::render(pass);
}

void CameraNode::update(float dt){

	ISceneNode::update(dt);
}


void CameraNode::setLookAtTarget(IMovable*m){
	m_lookAtTarget=m;
}

IMovable*CameraNode::getLookAtTarget(){
	return m_lookAtTarget;
}


void CameraNode::setWindowDim(math::Point2di dim)
{
	m_rWindow.BRPoint=dim;
//	aspect=(float)dim.x/(float)dim.y;
}
void CameraNode::setWindowPos(math::Point2di pos)
{
	m_rWindow.ULPoint=pos;
}

math::vector3d CameraNode::getAbsolutePosition()const
{
	math::vector3d res=ISceneNode::getAbsolutePosition();
	if(m_reflection){
		//res=(math::vector4d(res.x,res.y,res.z,1)*m_reflectionMatrix).XYZ();
		res=m_reflectionMatrix*res;
	}
	return res;
}
math::quaternion CameraNode::getAbsoluteOrintation()const
{
	math::quaternion res=ISceneNode::getAbsoluteOrintation();
	if(m_reflection)
	{
		math::quaternion q;
		q.fromMatrix(m_reflectionMatrix);
		res=res*q;
	}
	return res;

}

const math::Frustum*CameraNode::getViewFrustrum()const
{
	if(m_customViewNode)
		return m_customViewNode->getViewFrustrum();
	return &m_viewArea;
}


math::recti CameraNode::getViewPort()
{
	/*
	math::Point2di wPos=m_rWindow.ULPoint;
	math::Point2di wSize=m_rWindow.BRPoint;
	math::Point2di wScreen=m_sceneMngr->getDevice()->getScreenSize();
	if(wSize.x>wScreen.x)
		wSize.x=wScreen.x;
	if(wSize.y>wScreen.y)
		wSize.y=wScreen.y;*/

	return m_rWindow;
}


const std::list<ICullable*>& CameraNode::getVisibleNodes()
{
	return m_visibleNodes;
}


bool CameraNode::addVisibleNode(ICullable*node){
	if(isCulling(node))
		return false;

	m_visibleNodes.push_back(node);
	return true;
}

void CameraNode::clearVisibleNodes()
{
	m_visibleNodes.clear();
}

bool CameraNode::isCulling(ICullable *node)const
{
	if(!node)return 1;
	if(node->getCullingType()==SCT_NONE)return false;

	//culling by bounding box

	math::box3d box=node->getTransformedBoundingBox();
/*	node->getAbsoluteTransformation().transformBox(box);

	math::matrix4x4 &mat=node->getAbsoluteTransformation();
	math::vector3d edges[8];
	box.getEdges(edges);
	mat.transformVectPre(edges[0]);
	
	box.reset(edges[0]);
	for(int i=1;i<8;++i){
		mat.transformVectPre(edges[i]);
		box.AddPoint(edges[i]);
	}
*/
	bool c=!m_viewArea.boundingBox.intersectWithBox(box);
	if(!c && m_customViewNode)
		c|=m_customViewNode->isCulling(node);

	if(!c && node->getViewDistance()>0){
		float d=(node->getAbsolutePosition()-getAbsolutePosition()).LengthSQ();
		if(d>=node->getViewDistance()*node->getViewDistance())
			c=true;
	}
/*
	
	if(!c)
	{
		math::vector3d edges[8];
		box.getEdges(edges);
		bool shouldBreak=false;
		//clip against clip planes
		for (int i=0;!shouldBreak && i<math::Frustum::FP_PLANE_COUNT;++i)
		{
			for (int j=0;j<8;++j)
			{
				if(viewArea.planes[i].getVertLocation(edges[j])>0){
					shouldBreak=true;
					break;
				}
			}

		}
	}*/
	

	return c;
}




void CameraNode::onAspectChanged(float w,float h){
	if(!m_autoUpdateAspect)
		return;
	m_aspect=w/h;
//	m_rWindow=m_sceneMngr->getDevice()->getViewportRect();
	calcProjectionMatrix();
}

void CameraNode::OnVisit(ISceneVisitor*visitor)
{
	ISceneNode::OnVisit(visitor);
	visitor->Visit(this);
}
void CameraNode::SaveToDesc(CameraNodeDesc* desc)const
{
	ISceneNode::SaveToDesc(desc);
	desc->fovY=getFovY();
	desc->zNear=getZNear();
	desc->zFar=getZFar();
	desc->aspect=getAspect();
	desc->autoUpdateAspect=getAutoUpdateAspect();
	desc->isPerspective=(getProjectionType()==EPT_Perspective);
	desc->target=getTarget();

}

void CameraNode::LoadFromDesc(const CameraNodeDesc* desc)
{
	ISceneNode::LoadFromDesc(desc);
	setFovY(desc->fovY);
	setZNear(desc->zNear);
	setZFar(desc->zFar);
	setAspect(desc->aspect);
	setAutoUpdateAspect(desc->autoUpdateAspect);
	setProjectionType(desc->isPerspective?EPT_Perspective:EPT_Ortho);
	setTarget(desc->target);
}

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(FovY,CameraNode,float,mT("FovY"),EPBT_Basic,mT("FovY of the camera"),math::PI32/2.5f);
IMPLEMENT_PROPERTY_TYPE_GENERIC(FovY,CameraNode,float,setFovY,getFovY,core::StringConverter::toString,core::StringConverter::toFloat,false)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(ZNear,CameraNode,float,mT("ZNear"),EPBT_Basic,mT("ZNear plane"),1);
IMPLEMENT_PROPERTY_TYPE_GENERIC(ZNear,CameraNode,float,setZNear,getZNear,core::StringConverter::toString,core::StringConverter::toFloat,false)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(ZFar,CameraNode,float,mT("ZFar"),EPBT_Basic,mT("ZFar plane"),3000);
IMPLEMENT_PROPERTY_TYPE_GENERIC(ZFar,CameraNode,float,setZFar,getZFar,core::StringConverter::toString,core::StringConverter::toFloat,false)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(Aspect,CameraNode,float,mT("Aspect"),EPBT_Basic,mT("Aspect of the view port"),1);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Aspect,CameraNode,float,setAspect,getAspect,core::StringConverter::toString,core::StringConverter::toFloat,false)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(Target,CameraNode,vector3d,mT("Target"),EPBT_Struct,mT("Target point to look at"),);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Target,CameraNode,math::vector3d,setTarget,getTarget,core::StringConverter::toString,core::StringConverter::toVector3d,false)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(UpVector,CameraNode,vector3d,mT("UpVector"),EPBT_Struct,mT("Up Vector"),math::vector3d(0,1,0));
IMPLEMENT_PROPERTY_TYPE_GENERIC(UpVector,CameraNode,math::vector3d,setUpVector,getUpVector,core::StringConverter::toString,core::StringConverter::toVector3d,false)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(UseTarget,CameraNode,bool,mT("UseTarget"),EPBT_Basic,mT("Enable lookat target"),0);
IMPLEMENT_PROPERTY_TYPE_GENERIC(UseTarget,CameraNode,bool,setUseTarget,getIsTargettingEnabled,core::StringConverter::toString,core::StringConverter::toBool,false)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(AutoUpdateAspect,CameraNode,bool,mT("AutoUpdateAspect"),EPBT_Basic,mT("Enable lookat target"),0);
IMPLEMENT_PROPERTY_TYPE_GENERIC(AutoUpdateAspect,CameraNode,bool,setAutoUpdateAspect,getAutoUpdateAspect,core::StringConverter::toString,core::StringConverter::toBool,false)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(ProjectionType,CameraNode,enum,mT("ProjectionType"),EPBT_Enum,mT("Projection Type"),EPT_Perspective);

CameraNode::PropertyTypeProjectionType CameraNode::PropertyTypeProjectionType::instance;
EProjectionType CameraNode::PropertyTypeProjectionType::GetValue(CPropertieSet*object)const
{
	CameraNode* o=dynamic_cast<CameraNode*>(object);
	if(!o)
		return m_default;
	return o->getProjectionType();
}
bool CameraNode::PropertyTypeProjectionType::SetValue(CPropertieSet*object,const EProjectionType& v)
{
	CameraNode* o=dynamic_cast<CameraNode*>(object);
	if(!o)return false;
	return o->setProjectionType(v);
}
core::string CameraNode::PropertyTypeProjectionType::toString(CPropertieSet*object)const
{
	if(GetEnum())
		return GetEnum()->ToString(GetValue(object));
	return core::StringConverter::toString((int)GetValue(object),core::string(mT("EProjectionType")));
}
bool CameraNode::PropertyTypeProjectionType::parse(CPropertieSet*reciver,const core::string&str)
{
	if(GetEnum())
	{
		if(!CheckIsValidValue(str))
			return false;
	}
	int v=0;
	if(GetEnum())
		v= GetEnum()->Parse(str);
	else
		core::StringConverter::parse(str,v,mT("EProjectionType"));
	return SetValue(reciver,(EProjectionType)v);
}
bool CameraNode::PropertyTypeProjectionType::isDefault(CPropertieSet*reciver)const
{
	if(GetValue(reciver)==GetDefaultValue())
		return true;
	return false;
}

}
}


