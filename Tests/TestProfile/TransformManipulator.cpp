#include "stdafx.h"

#include "TransformManipulator.h"

#include "MeshResourceManager.h"
#include "CameraNode.h"
#include "ISceneManager.h"
#include "ILogManager.h"
#include "DefaultColors.h"
#include "SceneHelper.h"
#include "ActionsHistory.h"
#include "Viewport.h"
#include "MeshBufferData.h"

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
	m_selectedManipulator=0;
	m_manipulating=false;
	m_actionHistory=0;
	m_localScaling=true;

}
TransformManipulator::~TransformManipulator()
{
}

void TransformManipulator::Init(const core::string& modelBasePath,
								ISceneManager* smngr,ViewPort*vp,CameraNode*cam)
{
	m_activeManipulator=EMT_Translate;

	m_selectedManipulator=0;

	m_camera=cam;
	math::vector2d vpSize=vp->getAbsViewPort().getSize();
	m_pickBuffer=new video::RTPickBuffer(Engine::getInstance().getDevice(),vpSize,vpSize);

	//EventQueue::getInstance().addEventHandler(this,BIT(ET_Mouse));

	GCPtr<scene::SMesh> meshT,meshR,meshS;
	GCPtr<scene::IRenderable> rendT,rendR,rendS;
	
	m_manipulatorShape[EMT_Translate]=smngr->createSceneNode(mT("TranslateManipulator"));

	m_manipulatorShape[EMT_Rotate]=smngr->createSceneNode(mT("RotateManipulator"));

	m_manipulatorShape[EMT_Scale]=smngr->createSceneNode(mT("ScaleManipulator"));

	m_manipulatorShape[EMT_Translate]->setVisible(false);
	m_manipulatorShape[EMT_Rotate]->setVisible(false);
	m_manipulatorShape[EMT_Scale]->setVisible(false);

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

	bool defaultAxis[EMC_Count]=
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

	EManipulatorComponent ComponentsIDs[EMC_Count]=
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
		//AxisMaterial[i]->setRenderState(video::RS_ZTest,video::ES_DontUse);
		AxisMaterial[2]->SetAlpha(0);
	}
	AxisMaterial[0]->SetAmbient(video::DefaultColors::Red);
	AxisMaterial[0]->SetDiffuse(video::DefaultColors::Red);	//X
	AxisMaterial[1]->SetAmbient(video::DefaultColors::Green);
	AxisMaterial[1]->SetDiffuse(video::DefaultColors::Green);//Y
	AxisMaterial[2]->SetAmbient(video::DefaultColors::Blue);
	AxisMaterial[2]->SetDiffuse(video::DefaultColors::Blue); //Z

	AxisMaterial[3]->SetAmbient(video::DefaultColors::Yellow);
	AxisMaterial[3]->SetDiffuse(video::DefaultColors::Yellow);//XY
	AxisMaterial[4]->SetAmbient(video::DefaultColors::Fuchsia);
	AxisMaterial[4]->SetDiffuse(video::DefaultColors::Fuchsia);//XZ
	AxisMaterial[5]->SetAmbient(video::DefaultColors::Cyan);
	AxisMaterial[5]->SetDiffuse(video::DefaultColors::Cyan);//YZ

	AxisMaterial[6]->SetAmbient(video::DefaultColors::White);
	AxisMaterial[6]->SetDiffuse(video::DefaultColors::White);  //XYZ

	int index;


	for(int i=0;i<6;++i)
	{
		core::string path=modelBasePath+mT("\\Translate\\")+ComponentsNames[i]+mT(".mdl");
		meshT=gMeshResourceManager.loadMesh(path,true);
		if(!meshT)
		{
			gLogManager.log(mT("Manipulator model couldn't be load: \"")+path+mT("\" "),ELL_ERROR);
			continue;
		}
		index=meshT->getBufferIndex((ComponentsNames[i]+mT("Shape")).c_str());
		if(index==-1)
		{
			gLogManager.log(mT("Translate manipulator's ")+ComponentsNames[i]+mT(" couldn't be found "),ELL_ERROR);
			return;
		}
		meshT->getBufferData(index)->setMaterial(AxisMaterial[i]);

		rendT=new MeshRenderableNode(meshT);
		rendT->SetTargetRenderGroup(scene::RGH_Transparent+1);
		rendT->SetHasCustomRenderGroup(true);
		m_manipulatorShape[EMT_Translate]->AttachNode(rendT);
		m_components[EMT_Translate].
			push_back(ManipulateComponent(ComponentsIDs[i],m_manipulatorShape[EMT_Translate], rendT,
			meshT->getBufferData(index),mFunctions[i],defaultAxis[i]));
	}

	for(int i=0;i<3;++i)
	{
		core::string path=modelBasePath+mT("\\Rotate\\")+ComponentsNames[i]+mT(".mdl");
		meshR=gMeshResourceManager.loadMesh(path,true);
		if(!meshR)
		{
			gLogManager.log(mT("Manipulator model couldn't be load: \"")+path+mT("\" "),ELL_ERROR);
			continue;
		}
		index=meshR->getBufferIndex((ComponentsNames[i]+mT("Shape")).c_str());
		if(index==-1)
		{
			gLogManager.log(mT("Rotate manipulator's ")+ComponentsNames[i]+mT(" couldn't be found"),ELL_ERROR);
			return;
		}
		meshR->getBufferData(index)->setMaterial(AxisMaterial[i]);
		rendR=new MeshRenderableNode(meshR);
		rendR->SetTargetRenderGroup(scene::RGH_Transparent+1);
		rendR->SetHasCustomRenderGroup(true);
		m_manipulatorShape[EMT_Rotate]->AttachNode(rendR);
		m_components[EMT_Rotate].
			push_back(ManipulateComponent(ComponentsIDs[i+6],m_manipulatorShape[EMT_Rotate],rendR,
			meshR->getBufferData(index),mFunctions[i+6],defaultAxis[i+6]));
	}
	for(int i=0;i<7;++i)
	{
		core::string path=modelBasePath+mT("\\Scale\\")+ComponentsNames[i]+mT(".mdl");
		meshS=gMeshResourceManager.loadMesh(path,true);
		if(!meshS)
		{
			gLogManager.log(mT("Manipulator model couldn't be load: \"")+path+mT("\" "),ELL_ERROR);
			continue;
		}
		index=meshS->getBufferIndex((ComponentsNames[i]+mT("Shape")).c_str());
		if(index==-1)
		{
			gLogManager.log(mT("Scale manipulator's ")+ComponentsNames[i]+mT(" couldn't be found "),ELL_ERROR);
			return;
		}
		meshS->getBufferData(index)->setMaterial(AxisMaterial[i]);
		rendS=new MeshRenderableNode(meshS);
		rendS->SetTargetRenderGroup(scene::RGH_Transparent+1);
		rendS->SetHasCustomRenderGroup(true);
		m_manipulatorShape[EMT_Scale]->AttachNode(rendS);
		m_components[EMT_Scale].
			push_back(ManipulateComponent(ComponentsIDs[i+9],m_manipulatorShape[EMT_Scale], rendS,
			meshS->getBufferData(index),mFunctions[i+9],defaultAxis[i+9]));
	}
}

void TransformManipulator::AddNode(scene::IMovable*node)
{
	NodeList::iterator it=m_nodes.begin();
	for (;it!=m_nodes.end();++it)
	{
		if(*it==node)
			return;
	}
	NodeContext c;
	c.node=node;
	m_nodes.push_back(node);
	m_nodesContex.push_back(c);
	_CheckActiveTransform();
}
void TransformManipulator::RemoveNode(scene::IMovable*node)
{
	NodeList::iterator it=m_nodes.begin();
	for (;it!=m_nodes.end();++it)
	{
		if(*it==node)
		{
			m_nodes.erase(it);
			break;
		}
	}
	NodesContexList::iterator it2=m_nodesContex.begin();
	for (;it2!=m_nodesContex.end();++it2)
	{
		if((*it2).node==node)
		{
			m_nodesContex.erase(it2);
			break;
		}
	}
	_CheckActiveTransform();
}
void TransformManipulator::ClearNodes()
{
	m_nodes.clear();
	m_nodesContex.clear();
	_CheckActiveTransform();
}
const TransformManipulator::NodeList& TransformManipulator::GetNodes()const
{
	return m_nodes;
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

	_CheckActiveTransform();

// 	m_manipulatorShape[EMT_Translate]->setVisible((m_activeManipulator==EMT_Translate)&&m_node;
// 	m_manipulatorShape[EMT_Rotate]->Visible=(m_activeManipulator==EMT_Rotate)&&m_node;
// 	m_manipulatorShape[EMT_Scale]->Visible=(m_activeManipulator==EMT_Scale)&&m_node;
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
	if(!m_nodes.size())
		return;

	math::matrix4x4 viewProjection,invViewProjection;
/*
	viewProjection=m_camera->getViewMatrix();
	math::vector3d unitVector(1,1,1);
	unitVector.Normalize();
	viewProjection.getInverse(invViewProjection);
	invViewProjection.transformVectPre(unitVector);
	float len=unitVector.Length();*/

	//find center of nodes
	math::vector3d center=_GetCenter();

	math::quaternion orintation;
	if(m_nodes.size()==1)
	{
		orintation=(*m_nodes.begin())->getAbsoluteOrintation();
	}

	float len=(m_camera->getAbsolutePosition()-center).Length();
	if(len>0)
	{
		len*=0.15;
//		len=1.0f/len;
		m_manipulatorShape[EMT_Translate]->setScale(len);
		m_manipulatorShape[EMT_Rotate]->setScale(len);
		m_manipulatorShape[EMT_Scale]->setScale(len);


		m_manipulatorShape[EMT_Translate]->setPosition(center);
		m_manipulatorShape[EMT_Rotate]->setPosition(center);
		m_manipulatorShape[EMT_Scale]->setPosition(center);

		//m_manipulatorShape[EMT_Translate]->setOrintation(m_node->getAbsoluteOrintation());
		m_manipulatorShape[EMT_Rotate]->setOrintation(orintation);
		if(m_localScaling)
			m_manipulatorShape[EMT_Scale]->setOrintation(orintation);
		else
			m_manipulatorShape[EMT_Scale]->setOrintation(math::quaternion::Identity);
	}

	
	if(!m_manipulating)
		return;


}

bool TransformManipulator::SelectAxis(int x,int y)
{
	if(!m_nodes.size())
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
void TransformManipulator::_CheckActiveTransform()
{
	m_manipulatorShape[EMT_Translate]->setVisible((m_activeManipulator==EMT_Translate)&&m_nodes.size());
	m_manipulatorShape[EMT_Rotate]->setVisible((m_activeManipulator==EMT_Rotate)&&m_nodes.size());
	m_manipulatorShape[EMT_Scale]->setVisible((m_activeManipulator==EMT_Scale)&&m_nodes.size());

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

math::vector3d TransformManipulator::_GetCenter()
{
	math::vector3d center;
	if(m_nodes.size()>0)
	{
		NodeList::iterator it=m_nodes.begin();
		for (;it!=m_nodes.end();++it)
		{
			center+=(*it)->getAbsolutePosition();
		}
		center/=(float)m_nodes.size();
	}
	return center;
}
math::quaternion TransformManipulator::_GetOrintation()
{
	if(m_nodes.size()>0)
		return math::quaternion::Identity;
	return (*m_nodes.begin())->getAbsoluteOrintation();
}
math::vector3d TransformManipulator::_GetScale()
{
	if(m_nodes.size()>0)
		return 1;
	return (*m_nodes.begin())->getScale();
}
void TransformManipulator::_SaveContext()
{
	NodesContexList::iterator it=m_nodesContex.begin();
	for (;it!=m_nodesContex.end();++it)
	{
		NodeContext& c= (*it);
		c.startPos=c.node->getPosition();
		c.startRot=c.node->getAbsoluteOrintation();
		c.startScale=c.node->getScale();
	}
}
void TransformManipulator::_ReloadContext()
{
	NodesContexList::iterator it=m_nodesContex.begin();
	for (;it!=m_nodesContex.end();++it)
	{
		NodeContext& c= (*it);
		c.node->setPosition(c.startPos);
		c.node->setOrintation(c.startRot);
		c.node->setScale(c.startScale);
	}
}
void TransformManipulator::_startManipulating()
{
	_SaveContext();

 	m_startScale=_GetScale();
 	m_startRot=_GetOrintation();
 	m_startPos=_GetCenter();
	m_manipulating=true;
}
bool TransformManipulator::OnMousePressed(int x, int y)
{
	m_selectedManipulator=0;
	if(!SelectAxis(x,y))
	{
		m_manipulating=false;
		return false;
	}

	_startManipulating();
	return true;
}

void TransformManipulator::OnMouseMiddlePressed(int x, int y)
{
	SelectAxis(x,y);
	if(!m_selectedManipulator || !m_nodes.size())
		return;
	for(int i=0;i<m_components[m_activeManipulator].size();++i)
	{
		m_components[m_activeManipulator][i].bufferData->setVisible(false);
	}
	m_selectedManipulator->bufferData->setVisible(true);
	
	_startManipulating();

	m_startMousePosition.x=x;
	m_startMousePosition.y=y;
}
void TransformManipulator::OnMouseReleased()
{
	if(!m_manipulating || m_activeManipulator==EMT_None || !m_nodes.size()  || !m_selectedManipulator){
		return;
	}
	if(m_actionHistory)
	{
		NodesContexList::iterator it=m_nodesContex.begin();
		for (;it!=m_nodesContex.end();++it)
		{
			switch(m_activeManipulator)
			{
			case EMT_Translate:
				m_actionHistory->PushAction(new TransformManipulatorAction((*it).node,EMT_Translate,(*it).startPos,(*it).node->getPosition()));
				break;
			case EMT_Rotate:
				m_actionHistory->PushAction(new TransformManipulatorAction((*it).node,EMT_Rotate,(*it).startRot,(*it).node->getOrintation()));
				break;
			case EMT_Scale:
				m_actionHistory->PushAction(new TransformManipulatorAction((*it).node,EMT_Scale,(*it).startScale,(*it).node->getScale()));
				break;
			}
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
	if(!m_manipulating || !m_selectedManipulator || !m_nodes.size())
		return;

	(this->*m_selectedManipulator->manipulatingFunction)(x,y);
}

void TransformManipulator::OnRMousePressed()
{
	if(!m_manipulating || !m_selectedManipulator || !m_nodes.size())
		return;
	_ReloadContext();
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
		MeshBufferData* bd= m_components[m_activeManipulator][i].node->getMesh()->getBufferData(0);
		m_pickBuffer->AddRenderObject(m_components[m_activeManipulator][i].bufferData,m_components[m_activeManipulator][i].node);
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

	math::vector3d diff=a*l;

	NodeList::iterator it=m_nodes.begin();
	for (;it!=m_nodes.end();++it)
	{
		(*it)->translate(diff,scene::TS_World);
	}
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
	NodeList::iterator it=m_nodes.begin();
	for (;it!=m_nodes.end();++it)
	{
		(*it)->translate(diff,scene::TS_World);
	}
}

void TransformManipulator::ScaleAxis(int x,int y,const math::vector3d&a)
{
	math::vector2d delta=(math::vector2d(x,y)-m_startMousePosition)*0.1;
	math::vector3d axis;
	if(m_localScaling)
		axis=a;
	else
		axis=_GetOrintation()*a;
	math::vector3d start=scene::SceneHelper::getInstance().getScreenCoordFromPos(m_startPos,m_camera);
	math::vector3d end=scene::SceneHelper::getInstance().getScreenCoordFromPos(m_startPos+axis,m_camera);
	math::vector2d startP(start.x,start.y);
	math::vector2d endP(end.x,end.y);
	math::vector2d dir=(endP-startP).Normalize();

	float l=(delta.dotProduct(dir));

	//m_totalTranslate=diff
	NodesContexList::iterator it=m_nodesContex.begin();
	for (;it!=m_nodesContex.end();++it)
	{
		(*it).node->setScale((*it).startScale+a*l);
	}
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

	NodesContexList::iterator it=m_nodesContex.begin();
	for (;it!=m_nodesContex.end();++it)
	{
		NodeContext& c=*it;
		math::vector3d startToPos=v1-c.node->getAbsolutePosition();
		math::vector3d endToPos=v2-c.node->getAbsolutePosition();

		float l1=startToPos.Length();
		float l2=endToPos.Length();

		float ratio=(l1==0)? 1 : l2/l1;
		math::vector3d diff=(v2-v1)*ratio*0.2;

		c.node->setScale(c.startScale+diff);
	}

}


void TransformManipulator::RotateAxis(int x,int y,const math::vector3d&a)
{
	math::vector3d pos=_GetCenter();
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
	NodesContexList::iterator it=m_nodesContex.begin();
	for (;it!=m_nodesContex.end();++it)
	{
		(*it).node->rotate(rot,scene::TS_Local);
	}
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
	math::vector3d pos=_GetCenter();
	math::vector3d axis1=/*m_node->getAbsoluteOrintation()**/math::vector3d::XAxis;
	math::vector3d axis2=/*m_node->getAbsoluteOrintation()**/math::vector3d::YAxis;
	math::Plane plane(pos,pos+axis1,pos+axis2);
	TranslatePlane(x,y,plane);
}
void TransformManipulator::TranslateXZ(int x,int y)
{
	math::vector3d pos=_GetCenter();
	math::vector3d axis1=/*m_node->getAbsoluteOrintation()**/math::vector3d::XAxis;
	math::vector3d axis2=/*m_node->getAbsoluteOrintation()**/math::vector3d::ZAxis;
	math::Plane plane(pos,pos+axis1,pos+axis2);
	TranslatePlane(x,y,plane);
}
void TransformManipulator::TranslateYZ(int x,int y)
{
	math::vector3d pos=_GetCenter();
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
	math::vector3d pos=_GetCenter();
	math::vector3d axis1,axis2;
	if(m_localScaling)
	{
		axis1=math::vector3d::XAxis;
		axis2=math::vector3d::YAxis;
	}else
	{
		axis1=_GetOrintation()*math::vector3d::XAxis;
		axis2=_GetOrintation()*math::vector3d::YAxis;
	}
	math::Plane plane(pos,pos+axis1,pos+axis2);
	ScalePlane(x,y,plane);
}
void TransformManipulator::ScaleXZ(int x,int y)
{
	math::vector3d pos=_GetCenter();
	math::vector3d axis1,axis2;
	if(m_localScaling)
	{
		axis1=math::vector3d::XAxis;
		axis2=math::vector3d::ZAxis;
	}else
	{
		axis1=_GetOrintation()*math::vector3d::XAxis;
		axis2=_GetOrintation()*math::vector3d::ZAxis;
	}
	math::Plane plane(pos,pos+axis1,pos+axis2);
	ScalePlane(x,y,plane);
}
void TransformManipulator::ScaleYZ(int x,int y)
{
	math::vector3d pos=_GetCenter();
	math::vector3d axis1,axis2;
	if(m_localScaling)
	{
		axis1=math::vector3d::YAxis;
		axis2=math::vector3d::ZAxis;
	}else
	{
		 axis1=_GetOrintation()*math::vector3d::YAxis;
		 axis2=_GetOrintation()*math::vector3d::ZAxis;
		
	}
	math::Plane plane(pos,pos+axis1,pos+axis2);
	ScalePlane(x,y,plane);
}
void TransformManipulator::ScaleXYZ(int x,int y)
{
	float diff=(x-m_startMousePosition.x)*0.02;
	NodesContexList::iterator it=m_nodesContex.begin();
	for (;it!=m_nodesContex.end();++it)
	{
		(*it).node->setScale((*it).startScale+m_startScale*diff);
	}
}

void TransformManipulator::OnViewportResize()
{
	math::vector2d vpSize=m_camera->getViewPort().getSize();
	m_pickBuffer->SetViewportSize(vpSize);
	m_pickBuffer->SetRenderTargetSize(vpSize/2);
}

}
}