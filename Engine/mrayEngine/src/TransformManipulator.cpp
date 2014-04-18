#include "stdafx.h"

#include "TransformManipulator.h"

#include "MeshResourceManager.h"
#include "CameraNode.h"
#include "ISceneManager.h"
#include "ILogManager.h"
#include "DefaultColors.h"
#include "SceneHelper.h"
#include "ActionsHistory.h"

namespace mray
{
namespace scene
{

	class TransformManipulatorAction:public IActionHistoryRecord
	{
		TransformManipulator::EManipulatorType m_transformationType;
		
		math::vector3d m_v1,m_v2;
		math::quaternion m_q1,m_q2;
		
		IMovable* targetNode;
	public:
		TransformManipulatorAction(IMovable* node,TransformManipulator::EManipulatorType transType,const math::vector3d& v1,
			const math::vector3d& v2):targetNode(node),m_transformationType(transType),m_v1(v1),m_v2(v2)
		{
		}
		TransformManipulatorAction(IMovable* node,TransformManipulator::EManipulatorType transType,const math::quaternion& q1,const math::quaternion& q2):
		targetNode(node),m_transformationType(transType),m_q1(q1),m_q2(q2)
		{
		}
		virtual core::string GetDescription()
		{
			switch (m_transformationType)
			{
			case TransformManipulator::EMT_Translate:
				return mT("Translation");
			case TransformManipulator::EMT_Rotate:
				return mT("Rotation");
			case TransformManipulator::EMT_Scale:
				return mT("Scale");
			}
			return mT("Transformation");
		}
		virtual void Redo()
		{
			switch (m_transformationType)
			{
			case TransformManipulator::EMT_Translate:
				targetNode->setPosition(m_v2);
				break;
			case TransformManipulator::EMT_Rotate:
				targetNode->setOrintation(m_q2);
				break;
			case TransformManipulator::EMT_Scale:
				targetNode->setScale(m_v2);
				break;
			}
		}
		virtual void Undo()
		{
			switch (m_transformationType)
			{
			case TransformManipulator::EMT_Translate:
				targetNode->setPosition(m_v1);
				break;
			case TransformManipulator::EMT_Rotate:
				targetNode->setOrintation(m_q1);
				break;
			case TransformManipulator::EMT_Scale:
				targetNode->setScale(m_v1);
				break;
			}
		}
	};

TransformManipulator::TransformManipulator()
{
	m_activeManipulator=EMT_None;

	m_camera=0;
	m_node=0;
	m_selectedManipulator=0;
	m_manipulating=false;
	m_actionHistory=0;

}
TransformManipulator::~TransformManipulator()
{
}

void TransformManipulator::Init(const core::string& translateModel,const core::string& rotateModel,const core::string& scaleModel,
								scene::ISceneManager*smngr,scene::CameraNode*cam)
{
	m_activeManipulator=EMT_Translate;

	m_selectedManipulator=0;

	m_camera=cam;
	math::vector2d vpSize=smngr->getDevice()->getViewportRect().getSize();
	m_pickBuffer=new video::RTPickBuffer(smngr->getDevice(),vpSize,vpSize);

	//EventQueue::getInstance().addEventHandler(this,BIT(ET_Mouse));

	GCPtr<scene::SMesh> meshT,meshR,meshS;
	
	meshT=gMeshResourceManager.loadMesh(translateModel,true);
	if(!meshT)
	{
		gLogManager.log(mT("Translate manipulator model couldn't be load: ")+ translateModel,ELL_ERROR);
		return;
	}
	m_manipulatorShape[EMT_Translate]=new scene::EditorSceneNode(meshT,mT("TranslateManipulator"),0);

	meshR=gMeshResourceManager.loadMesh(rotateModel,true);
	if(!meshR)
	{
		gLogManager.log(mT("Rotate manipulator model couldn't be load: ")+ rotateModel,ELL_ERROR);
		return;
	}
	m_manipulatorShape[EMT_Rotate]=new scene::EditorSceneNode(meshR,mT("RotateManipulator"),0);

	meshS=gMeshResourceManager.loadMesh(scaleModel,true);
	if(!meshS)
	{
		gLogManager.log(mT("Translate manipulator model couldn't be load: ")+ scaleModel,ELL_ERROR);
		return;
	}
	m_manipulatorShape[EMT_Scale]=new scene::EditorSceneNode(meshS,mT("ScaleManipulator"),0);

	m_manipulatorShape[EMT_Translate]->Visible=false;
	m_manipulatorShape[EMT_Rotate]->Visible=false;
	m_manipulatorShape[EMT_Scale]->Visible=false;

	smngr->addSceneNode(m_manipulatorShape[EMT_Translate]);
	smngr->addSceneNode(m_manipulatorShape[EMT_Rotate]);
	smngr->addSceneNode(m_manipulatorShape[EMT_Scale]);

	const core::string ComponentsNames[]=
	{
		mT("XAxis"),
		mT("YAxis"),
		mT("ZAxis"),

		mT("XYPlane"),
		mT("XZPlane"),
		mT("YZPlane"),

		mT("XYZAxis")
	};
	GCPtr<video::SMaterial> AxisMaterial[]=
	{
		new video::SMaterial(),
		new video::SMaterial(),
		new video::SMaterial(),

		new video::SMaterial(),
		new video::SMaterial(),
		new video::SMaterial(),

		new video::SMaterial(),
	};

	bool defaultAxis[EManipulatorComponent::EMC_Count]=
	{
		false,
		false,
		false,
		false,
		true,
		false,

		false,
		true,
		false,

		false,
		false,
		false,
		false,
		false,
		false,
		true
	};

	EManipulatorComponent ComponentsIDs[EManipulatorComponent::EMC_Count]=
	{
		EMC_TranslateX,
		EMC_TranslateY,
		EMC_TranslateZ,
		EMC_TranslateXY,
		EMC_TranslateXZ,
		EMC_TranslateYZ,

		EMC_RotateX,
		EMC_RotateY,
		EMC_RotateZ,

		EMC_ScaleX,
		EMC_ScaleY,
		EMC_ScaleZ,
		EMC_ScaleXY,
		EMC_ScaleXZ,
		EMC_ScaleYZ,
		EMC_ScaleXYZ
	};
	void(TransformManipulator::*mFunctions[])(int x,int y)=
	{
		&TransformManipulator::TranslateX,
		&TransformManipulator::TranslateY,
		&TransformManipulator::TranslateZ,
		&TransformManipulator::TranslateXY,
		&TransformManipulator::TranslateXZ,
		&TransformManipulator::TranslateYZ,

		&TransformManipulator::RotateX,
		&TransformManipulator::RotateY,
		&TransformManipulator::RotateZ,

		&TransformManipulator::ScaleX,
		&TransformManipulator::ScaleY,
		&TransformManipulator::ScaleZ,
		&TransformManipulator::ScaleXY,
		&TransformManipulator::ScaleXZ,
		&TransformManipulator::ScaleYZ,
		&TransformManipulator::ScaleXYZ
	};

	for(int i=0;i<7;++i)
	{
		AxisMaterial[i]->setRenderState(video::RS_Lighting,video::ES_DontUse);
		AxisMaterial[i]->setRenderState(video::RS_CullFace,video::ES_DontUse);
		AxisMaterial[i]->setRenderState(video::RS_ZTest,video::ES_DontUse);
		AxisMaterial[2]->Alpha=0;
	}
	AxisMaterial[0]->ambientColor=AxisMaterial[0]->diffuseColor=video::DefaultColors::Red;	//X
	AxisMaterial[1]->ambientColor=AxisMaterial[1]->diffuseColor=video::DefaultColors::Green;//Y
	AxisMaterial[2]->ambientColor=AxisMaterial[2]->diffuseColor=video::DefaultColors::Blue; //Z

	AxisMaterial[3]->ambientColor=AxisMaterial[3]->diffuseColor=video::SColor(255,255,0,255);//XY
	AxisMaterial[4]->ambientColor=AxisMaterial[4]->diffuseColor=video::SColor(255,0,255,255);//XZ
	AxisMaterial[5]->ambientColor=AxisMaterial[5]->diffuseColor=video::SColor(0,255,255,255);//YZ

	AxisMaterial[6]->ambientColor=AxisMaterial[6]->diffuseColor=video::DefaultColors::White;  //XYZ

	int index;

	for(int i=0;i<6;++i)
	{
		index=meshT->getBufferIndex(ComponentsNames[i].c_str());
		if(index==-1)
		{
			gLogManager.log(mT("Translate manipulator's ")+ComponentsNames[i]+mT(" couldn't be found: ")+ translateModel,ELL_ERROR);
			return;
		}
		meshT->getBufferData(index)->setMaterial(AxisMaterial[i]);
		m_components[EMT_Translate].
			push_back(ManipulateComponent(ComponentsIDs[i],m_manipulatorShape[EMT_Translate], meshT->getBufferData(index),mFunctions[i],defaultAxis[i]));
	}

	for(int i=0;i<3;++i)
	{
		index=meshR->getBufferIndex(ComponentsNames[i].c_str());
		if(index==-1)
		{
			gLogManager.log(mT("Rotate manipulator's ")+ComponentsNames[i]+mT(" couldn't be found: ")+ rotateModel,ELL_ERROR);
			return;
		}
		meshR->getBufferData(index)->setMaterial(AxisMaterial[i]);
		m_components[EMT_Rotate].
			push_back(ManipulateComponent(ComponentsIDs[i+6],m_manipulatorShape[EMT_Rotate], meshR->getBufferData(index),mFunctions[i+6],defaultAxis[i+6]));
	}
	for(int i=0;i<7;++i)
	{
		index=meshS->getBufferIndex(ComponentsNames[i].c_str());
		if(index==-1)
		{
			gLogManager.log(mT("Scale manipulator's ")+ComponentsNames[i]+mT(" couldn't be found: ")+ scaleModel,ELL_ERROR);
			return;
		}
		meshS->getBufferData(index)->setMaterial(AxisMaterial[i]);
		m_components[EMT_Scale].
			push_back(ManipulateComponent(ComponentsIDs[i+9],m_manipulatorShape[EMT_Scale], meshS->getBufferData(index),mFunctions[i+9],defaultAxis[i+9]));
	}
}

void TransformManipulator::SetNode(scene::IMovable*node)
{
	m_node=node;
	m_manipulatorShape[EMT_Translate]->Visible=(m_activeManipulator==EMT_Translate)&&m_node;
	m_manipulatorShape[EMT_Rotate]->Visible=(m_activeManipulator==EMT_Rotate)&&m_node;
	m_manipulatorShape[EMT_Scale]->Visible=(m_activeManipulator==EMT_Scale)&&m_node;

	if(m_activeManipulator!=EMT_None){
		for(int i=0;i<m_components[m_activeManipulator].size();++i)
		{
			if(m_components[m_activeManipulator][i].isDefault)
			{
				m_selectedManipulator=&m_components[m_activeManipulator][i];
				break;
			}
		}
	}
}
scene::IMovable* TransformManipulator::GetNode()
{
	return m_node;
}

void TransformManipulator::SetCamera(scene::CameraNode*camera)
{
	m_camera=camera;
}
scene::CameraNode* TransformManipulator::GetCamera()
{
	return m_camera;
}


void TransformManipulator::SetActiveManipulator(TransformManipulator::EManipulatorType m)
{
	m_activeManipulator=m;
	if(m_activeManipulator==EMT_None){
		m_manipulating=false;
	}else{
		for(int i=0;i<m_components[m_activeManipulator].size();++i)
		{
			if(m_components[m_activeManipulator][i].isDefault)
			{
				m_selectedManipulator=&m_components[m_activeManipulator][i];
				break;
			}
		}
	}


	m_manipulatorShape[EMT_Translate]->Visible=(m_activeManipulator==EMT_Translate)&&m_node;
	m_manipulatorShape[EMT_Rotate]->Visible=(m_activeManipulator==EMT_Rotate)&&m_node;
	m_manipulatorShape[EMT_Scale]->Visible=(m_activeManipulator==EMT_Scale)&&m_node;
}
TransformManipulator::EManipulatorType TransformManipulator::GetActiveManipulator()
{
	return m_activeManipulator;
}

bool TransformManipulator::IsManipulating()
{
	return m_manipulating;
}


void TransformManipulator::SetActionHistory(ActionsHistory* h)
{
	m_actionHistory=h;
}
ActionsHistory* TransformManipulator::GetActionHistory()
{
	return m_actionHistory;
}

void TransformManipulator::Update()
{
	if(m_activeManipulator==EMT_None){
		return;
	}
	if(!m_node)
		return;

	math::matrix4x4 viewProjection,invViewProjection;
/*
	viewProjection=m_camera->getViewMatrix();
	math::vector3d unitVector(1,1,1);
	unitVector.Normalize();
	viewProjection.getInverse(invViewProjection);
	invViewProjection.transformVectPre(unitVector);
	float len=unitVector.Length();*/

	float len=(m_camera->getAbsolutePosition()-m_node->getAbsolutePosition()).Length();
	if(len>0)
	{
		len*=0.15;
//		len=1.0f/len;
		m_manipulatorShape[EMT_Translate]->scale=len;
		m_manipulatorShape[EMT_Rotate]->scale=len;
		m_manipulatorShape[EMT_Scale]->scale=len;


		m_manipulatorShape[EMT_Translate]->position=m_node->getAbsolutePosition();
		m_manipulatorShape[EMT_Rotate]->position=m_node->getAbsolutePosition();
		m_manipulatorShape[EMT_Scale]->position=m_node->getAbsolutePosition();

		//m_manipulatorShape[EMT_Translate]->setOrintation(m_node->getAbsoluteOrintation());
		m_manipulatorShape[EMT_Rotate]->setOrintation(m_node->getAbsoluteOrintation());
		m_manipulatorShape[EMT_Scale]->setOrintation(m_node->getAbsoluteOrintation());
	}

	
	if(!m_manipulating)
		return;


}

bool TransformManipulator::SelectAxis(int x,int y)
{
	if(!m_node)
		return false;
	if(m_activeManipulator==EMT_None){
		return false;
	}
	EManipulatorComponent comp=(EManipulatorComponent)Pick(x,y);
	if(comp==EMC_None)
	{
		return false;
	}
	m_selectedComponent=comp;

	m_startMousePosition.x=x;
	m_startMousePosition.y=y;

	int selectedAxis=0;

	if(m_selectedComponent & BIT(EMT_Translate)<<8)
	{
		selectedAxis = m_selectedComponent & ~(BIT(EMT_Translate)<<8);
		//m_activeManipulator = EMT_Translate;
	}else if(m_selectedComponent & BIT(EMT_Rotate)<<8)
	{
		selectedAxis = m_selectedComponent & ~(BIT(EMT_Rotate)<<8);
		//m_activeManipulator = EMT_Rotate;
	}else if(m_selectedComponent & BIT(EMT_Scale)<<8)
	{
		selectedAxis = m_selectedComponent & ~(BIT(EMT_Scale)<<8);
		//m_activeManipulator = EMT_Scale;
	}

	for(int i=0;i<m_components[m_activeManipulator].size();++i)
	{
		m_components[m_activeManipulator][i].bufferData->setVisible(false);
	}
	m_components[m_activeManipulator][selectedAxis].bufferData->setVisible(true);
	m_selectedManipulator=&m_components[m_activeManipulator][selectedAxis];
	m_manipulating=true;
	return true;
}
bool TransformManipulator::OnMousePressed(int x, int y)
{
	m_selectedManipulator=0;
	if(!SelectAxis(x,y))
	{
		m_manipulating=false;
		return false;
	}


	m_startScale=m_node->getScale();
	m_startRot=m_node->getOrintation();
	m_startPos=m_node->getPosition();

	return true;
}

void TransformManipulator::OnMouseMiddlePressed(int x, int y)
{
	SelectAxis(x,y);
	if(!m_selectedManipulator || !m_node)
		return;
	for(int i=0;i<m_components[m_activeManipulator].size();++i)
	{
		m_components[m_activeManipulator][i].bufferData->setVisible(false);
	}
	m_selectedManipulator->bufferData->setVisible(true);
	m_manipulating=true;

	m_startScale=m_node->getScale();
	m_startRot=m_node->getOrintation();
	m_startPos=m_node->getPosition();

	m_startMousePosition.x=x;
	m_startMousePosition.y=y;
}
void TransformManipulator::OnMouseReleased()
{
	if(!m_manipulating || m_activeManipulator==EMT_None || !m_node  || !m_selectedManipulator){
		return;
	}
	if(m_actionHistory)
	{
		switch(m_activeManipulator)
		{
		case EMT_Translate:
			m_actionHistory->PushAction(new TransformManipulatorAction(m_node,EMT_Translate,m_startPos,m_node->getPosition()));
			break;
		case EMT_Rotate:
			m_actionHistory->PushAction(new TransformManipulatorAction(m_node,EMT_Rotate,m_startRot,m_node->getOrintation()));
			break;
		case EMT_Scale:
			m_actionHistory->PushAction(new TransformManipulatorAction(m_node,EMT_Scale,m_startScale,m_node->getScale()));
			break;
		}
	}
	m_manipulating=false;
	for(int i=0;i<m_components[m_activeManipulator].size();++i)
	{
		m_components[m_activeManipulator][i].bufferData->setVisible(true);
	}
}


void TransformManipulator::OnMouseMoved(int x, int y)
{
	if(!m_manipulating || !m_selectedManipulator || !m_node)
		return;

	(this->*m_selectedManipulator->manipulatingFunction)(x,y);
}

void TransformManipulator::OnRMousePressed()
{
	if(!m_manipulating || !m_selectedManipulator || !m_node)
		return;
	m_node->setScale(m_startScale);
	m_node->setOrintation(m_startRot);
	m_node->setPosition(m_startPos);
	OnMouseReleased();
}
uint TransformManipulator::Pick(int x, int y)
{
	m_pickBuffer->Clear();
	m_pickBuffer->SetViewMatrix(m_camera->getViewMatrix());
	m_pickBuffer->SetProjectionMatrix(m_camera->getProjectionMatrix());

	for(int i=0;i<m_components[m_activeManipulator].size();++i)
	{
		m_pickBuffer->PushPickID(m_components[m_activeManipulator][i].type);
		m_pickBuffer->AddRenderObject(m_components[m_activeManipulator][i].bufferData,m_components[m_activeManipulator][i].owner);
	}

	m_pickBuffer->Render();

	return m_pickBuffer->GetID(x,y);
}


//////////////////////////////////////////////////////////////////////////

void TransformManipulator::TranslateAxis(int x,int y,const math::vector3d&a)
{
	math::vector2d delta=(math::vector2d(x,y)-m_startMousePosition)*0.1;
	math::vector3d axis=/*m_node->getAbsoluteOrintation()**/a;
	math::vector3d start=scene::SceneHelper::getInstance().getScreenCoordFromPos(m_startPos,m_camera);
	math::vector3d end=scene::SceneHelper::getInstance().getScreenCoordFromPos(m_startPos+axis,m_camera);
	math::vector2d startP(start.x,start.y);
	math::vector2d endP(end.x,end.y);
	math::vector2d dir=(endP-startP).Normalize();

	float l=(delta.dotProduct(dir));

	//m_totalTranslate=diff
	
	m_startMousePosition.x=x;
	m_startMousePosition.y=y;
	m_node->translate(a*l,scene::TS_World);
}

void TransformManipulator::TranslatePlane(int x,int y,const math::Plane&p)
{

	math::line3d ray1,ray2;
	ray1=scene::SceneHelper::getInstance().getRayFromScreenCoord(m_startMousePosition,m_camera);
	ray2=scene::SceneHelper::getInstance().getRayFromScreenCoord(math::vector2d(x,y),m_camera);

	math::vector3d r1Dir=ray1.getVector();
	math::vector3d r2Dir=ray2.getVector();
	math::vector3d v1,v2;
	if(!p.getIntersectionWithLine(ray1.pStart,r1Dir,v1))
		return;
	if(!p.getIntersectionWithLine(ray2.pStart,r2Dir,v2))
		return;
	math::vector3d diff=v2-v1;

	m_startMousePosition.x=x;
	m_startMousePosition.y=y;
	m_node->translate(diff,scene::TS_World);
}

void TransformManipulator::ScaleAxis(int x,int y,const math::vector3d&a)
{
	math::vector2d delta=(math::vector2d(x,y)-m_startMousePosition)*0.1;
	math::vector3d axis=m_node->getAbsoluteOrintation()*a;
	math::vector3d start=scene::SceneHelper::getInstance().getScreenCoordFromPos(m_startPos,m_camera);
	math::vector3d end=scene::SceneHelper::getInstance().getScreenCoordFromPos(m_startPos+axis,m_camera);
	math::vector2d startP(start.x,start.y);
	math::vector2d endP(end.x,end.y);
	math::vector2d dir=(endP-startP).Normalize();

	float l=(delta.dotProduct(dir));

	//m_totalTranslate=diff
	m_node->setScale(m_startScale+a*l);
}
void TransformManipulator::ScalePlane(int x,int y,const math::Plane&p)
{

	math::line3d ray1,ray2;
	ray1=scene::SceneHelper::getInstance().getRayFromScreenCoord(m_startMousePosition,m_camera);
	ray2=scene::SceneHelper::getInstance().getRayFromScreenCoord(math::vector2d(x,y),m_camera);

	math::vector3d r1Dir=ray1.getVector();
	math::vector3d r2Dir=ray2.getVector();
	math::vector3d v1,v2;
	if(!p.getIntersectionWithLine(ray1.pStart,r1Dir,v1))
		return;
	if(!p.getIntersectionWithLine(ray2.pStart,r2Dir,v2))
		return;

	math::vector3d startToPos=v1-m_node->getAbsolutePosition();
	math::vector3d endToPos=v2-m_node->getAbsolutePosition();

	float l1=startToPos.Length();
	float l2=endToPos.Length();

	float ratio=(l1==0)? 1 : l2/l1;
	math::vector3d diff=(v2-v1)*ratio*0.2;

	m_node->setScale(m_startScale+diff);

}


void TransformManipulator::RotateAxis(int x,int y,const math::vector3d&a)
{
	math::vector3d pos=m_node->getAbsolutePosition();
	math::Plane p(pos,a);

	math::line3d ray1,ray2;
	ray1=scene::SceneHelper::getInstance().getRayFromScreenCoord(m_startMousePosition,m_camera);
	ray2=scene::SceneHelper::getInstance().getRayFromScreenCoord(math::vector2d(x,y),m_camera);

	math::vector3d r1Dir=ray1.getVector();
	math::vector3d r2Dir=ray2.getVector();
	math::vector3d v1,v2;
	if(!p.getIntersectionWithLine(ray1.pStart,r1Dir,v1))
		return;
	if(!p.getIntersectionWithLine(ray2.pStart,r2Dir,v2))
		return;

	math::vector3d originToStart=(v1-pos).Normalize();
	math::vector3d originToEnd=(v2-pos).Normalize();

	math::vector3d rotCrossStart=a.crossProduct(originToStart).Normalize();
	math::vector3d rotCrossEnd=a.crossProduct(originToEnd).Normalize();

	math::vector3d startCrossEnd=originToStart.crossProduct(originToEnd).Normalize();

	float d=originToStart.dotProduct(originToEnd);
	d=math::clamp<float>(d,-1,1);
	float rotAngle=math::toDeg(acos(d))*math::sign(a.dotProduct(startCrossEnd));

	math::quaternion rot(rotAngle,a);
	rot.Normalize();

	m_startMousePosition.x=x;
	m_startMousePosition.y=y;
	m_node->rotate(rot,scene::TS_Local);
}

void TransformManipulator::TranslateX(int x,int y)
{
	TranslateAxis(x,y,math::vector3d::XAxis);
}
void TransformManipulator::TranslateY(int x,int y)
{
	TranslateAxis(x,y,math::vector3d::YAxis);
}
void TransformManipulator::TranslateZ(int x,int y)
{
	TranslateAxis(x,y,math::vector3d::ZAxis);
}
void TransformManipulator::TranslateXY(int x,int y)
{
	math::vector3d pos=m_node->getPosition();
	math::vector3d axis1=/*m_node->getAbsoluteOrintation()**/math::vector3d::XAxis;
	math::vector3d axis2=/*m_node->getAbsoluteOrintation()**/math::vector3d::YAxis;
	math::Plane plane(pos,pos+axis1,pos+axis2);
	TranslatePlane(x,y,plane);
}
void TransformManipulator::TranslateXZ(int x,int y)
{
	math::vector3d pos=m_node->getPosition();
	math::vector3d axis1=/*m_node->getAbsoluteOrintation()**/math::vector3d::XAxis;
	math::vector3d axis2=/*m_node->getAbsoluteOrintation()**/math::vector3d::ZAxis;
	math::Plane plane(pos,pos+axis1,pos+axis2);
	TranslatePlane(x,y,plane);
}
void TransformManipulator::TranslateYZ(int x,int y)
{
	math::vector3d pos=m_node->getPosition();
	math::vector3d axis1=/*m_node->getAbsoluteOrintation()**/math::vector3d::YAxis;
	math::vector3d axis2=/*m_node->getAbsoluteOrintation()**/math::vector3d::ZAxis;
	math::Plane plane(pos,pos+axis1,pos+axis2);
	TranslatePlane(x,y,plane);
}

void TransformManipulator::RotateX(int x,int y)
{
	RotateAxis(x,y,math::vector3d::XAxis);
}
void TransformManipulator::RotateY(int x,int y)
{
	RotateAxis(x,y,math::vector3d::YAxis);
}
void TransformManipulator::RotateZ(int x,int y)
{
	RotateAxis(x,y,math::vector3d::ZAxis);
}

void TransformManipulator::ScaleX(int x,int y)
{
	ScaleAxis(x,y,math::vector3d::XAxis);
}
void TransformManipulator::ScaleY(int x,int y)
{
	ScaleAxis(x,y,math::vector3d::YAxis);
}
void TransformManipulator::ScaleZ(int x,int y)
{
	ScaleAxis(x,y,math::vector3d::ZAxis);
}
void TransformManipulator::ScaleXY(int x,int y)
{
	math::vector3d pos=m_node->getPosition();
	math::vector3d axis1=m_node->getAbsoluteOrintation()*math::vector3d::XAxis;
	math::vector3d axis2=m_node->getAbsoluteOrintation()*math::vector3d::YAxis;
	math::Plane plane(pos,pos+axis1,pos+axis2);
	ScalePlane(x,y,plane);
}
void TransformManipulator::ScaleXZ(int x,int y)
{
	math::vector3d pos=m_node->getPosition();
	math::vector3d axis1=m_node->getAbsoluteOrintation()*math::vector3d::XAxis;
	math::vector3d axis2=m_node->getAbsoluteOrintation()*math::vector3d::ZAxis;
	math::Plane plane(pos,pos+axis1,pos+axis2);
	ScalePlane(x,y,plane);
}
void TransformManipulator::ScaleYZ(int x,int y)
{
	math::vector3d pos=m_node->getPosition();
	math::vector3d axis1=m_node->getAbsoluteOrintation()*math::vector3d::YAxis;
	math::vector3d axis2=m_node->getAbsoluteOrintation()*math::vector3d::ZAxis;
	math::Plane plane(pos,pos+axis1,pos+axis2);
	ScalePlane(x,y,plane);
}
void TransformManipulator::ScaleXYZ(int x,int y)
{
	float diff=(x-m_startMousePosition.x)*0.02;
	m_node->setScale(m_startScale+m_startScale*diff);
}

void TransformManipulator::OnViewportResize()
{
	math::vector2d vpSize=m_camera->getViewPort().getSize();
	m_pickBuffer->SetViewportSize(vpSize);
	m_pickBuffer->SetRenderTargetSize(vpSize/2);
}

}
}