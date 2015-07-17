
#include "stdafx.h"
#include "SessionRenderer.h"

#include "SessionContainer.h"
#include "CSubProject.h"
#include "SessionDetails.h"

#include "ProjectNode.h"
#include "CommentNode.h"
#include "IThreadManager.h"
#include "NodeRenderer.h"
#include "KMDComment.h"
#include "SceneCamera.h"
#include "AppData.h"
#include "ViewPort.h"

namespace mray
{
namespace scene
{


SessionRenderer::SessionRenderer()
{
	m_sessions = 0;
	m_activeSpeaker = 0;
	{
		m_physics = new physics::PhManager;// msa::physics::World2D();
		//m_physics->setGravity(0);
	}
	m_hoverItem = 0;
	m_nodeRenderer = new NodeRenderer();
	m_dataMutex = OS::IThreadManager::getInstance().createMutex();
	m_camera = new SceneCamera;

	m_selectedItem = 0;
	m_alpha = 1;

	m_speakerDistance = 400;
	m_CommentsDistance = 80;
	gAppData.subProjectChange.AddListener(this);

}

SessionRenderer::~SessionRenderer()
{
	delete m_physics;
	delete m_dataMutex;
	delete m_nodeRenderer;
	delete m_camera;
}

void SessionRenderer::OnPointerMoved(const math::vector2d& pos)
{
}



void SessionRenderer::SetRenderingVP(const math::rectf& vp)
{
	m_camera->SetViewPort(vp);
}

void SessionRenderer::SetSessions(kmd::SessionContainer*sessions)
{
	m_sessions = sessions;

	if (!m_sessions)
		return;

	const std::vector<kmd::SessionDetails*>& slist=m_sessions->GetSessions();

	int speakerCount = 0;
	for (int i = 0; i < slist.size(); ++i)
	{
		speakerCount += slist[i]->GetProjects().size();
	}
	std::vector<ProjectNode*> spList;
	float rad = 400;
	math::vector2d pos;
	for (int i = 0; i < slist.size();++i)
	{
		ProjectNode* s = new ProjectNode(slist[i]);


		//msa::physics::Particle2D* n = new msa::physics::Particle2D(pos);
		//m_physics->addParticle(n);
		//n->makeFixed();

		physics::PhNode* n = m_physics->CreateNode();
		n->SetPosition(pos);
		n->SetFixed(true);
		//msa::physics::Spring2D* spr = m_physics->makeSpring(root, n,  0.005, rad);
		n->SetRadius(20);
		s->SetSize(n->GetRadius()*2);

		s->SetPhysics(n);
		m_projects[s->GetProject()] = s;
		m_projectsSeq.push_back(s);

		spList.push_back(s);
		m_renderNodes.push_back(s);

		pos.y += m_speakerDistance;
		pos.x = math::Randomizer::randRange(-200, 200);

		const std::vector<kmd::CSubProject*>& speakers = slist[i]->GetProjects();
		for (int j = 0; j < speakers.size(); ++j)
		{

		}
	}

}

void SessionRenderer::AddComments(const std::vector<kmd::KMDComment*> &Comments)
{
	for (int i = 0; i < Comments.size(); ++i)
	{
		if (!Comments[i]->project)
			continue;
		ProjectMap::iterator it = m_projects.find(Comments[i]->project);
		if (it != m_projects.end())
		{
			_AddCommentNode(Comments[i], it->second);
		}
	}
}

void SessionRenderer::_AddCommentNode(kmd::KMDComment* t, ProjectNode*speaker)
{
	m_dataMutex->lock();
	if (m_Comments.find(t->ID) != m_Comments.end())
	{
		m_dataMutex->unlock();
		return;
	}
	CommentNode* node = new CommentNode(speaker->GetProject(), t);

	m_Comments[node->GetCommentID()] = node;

	float sz = 15;
	//msa::physics::Particle2D* n = new msa::physics::Particle2D();
	physics::PhNode* n = m_physics->CreateNode();
	n->SetRadius(sz);

	//n->SetPosition(pos);
	n->SetFixed(false);
	//m_physics->addParticle(n);
	node->SetPhysics(n);
	node->SetSize(n->GetRadius() * 2);

	speaker->AddComment(node);

	physics::PhNode *ph = speaker->GetPhysics();
	math::vector2d pos = ph->GetPosition();
	float a = math::Randomizer::rand01() * 360;
	physics::PhNode* nph = node->GetPhysics();
	float r = math::Randomizer::rand01() * 50 + 50 + nph->GetRadius() + ph->GetRadius();
	float r2 = r + 300;
	pos.x += math::cosd(a) * r2;
	pos.y += math::sind(a) * r2;
	nph->SetPosition(pos);
	//msa::physics::Spring2D* spr = m_physics->makeSpring(ph, nph, 0.0001, r);
	m_physics->CreateSpring(ph, nph, 1, r);

	m_dataMutex->unlock();
}

void SessionRenderer::_AddCommentsNodes(const std::vector<CommentNode*> &nodes)
{
	if (!nodes.size())
		return;
#if 0
	gAppData.soundManager->playSound("sounds//CommentArrived.mp3", 0, true, 100, false, sound::ESNDT_2D);
#endif
	m_dataMutex->lock();
	for (int i = 0; i < nodes.size(); ++i)
	{
		
		m_Comments[nodes[i]->GetCommentID()] = nodes[i];

		float sz = 25;
		//msa::physics::Particle2D* n = new msa::physics::Particle2D();
		physics::PhNode* n = m_physics->CreateNode();
		n->SetRadius(sz );
		//m_physics->addParticle(n);
		nodes[i]->SetPhysics(n);
		//m_projects.find(nodes[i]->GetSubProjectID());
	}

	for (int i = 0; i < nodes.size(); ++i)
	{
		 IKMDNode* target = 0;
		
		{
			int s = math::Randomizer::rand(m_projects.size());
			ProjectMap::iterator  it = m_projects.begin();
			std::advance(it, s);
			it->second->AddComment(nodes[i]);
			target = it->second;
		}

		 physics::PhNode *ph = target->GetPhysics();
		 math::vector2d pos = ph->GetPosition();
		 float a = math::Randomizer::rand01() * 360;
		 physics::PhNode* nph = nodes[i]->GetPhysics();
		 float r = math::Randomizer::rand01() * 50 + 50 + nph->GetRadius() + ph->GetRadius();
		 float r2 = r + 300;
		 pos.x += math::cosd(a) * r2;
		 pos.y += math::sind(a) * r2;
		 nph->SetPosition(pos);
		 //msa::physics::Spring2D* spr = m_physics->makeSpring(ph, nph, 0.0001, r );
		 m_physics->CreateSpring(ph, nph, 0.01, r);


	}
	//m_renderNodes.insert(m_renderNodes.end(), nodes.begin(), nodes.end());
	m_dataMutex->unlock();
}

void SessionRenderer::_OnSubProjectChange(kmd::CSubProject*s)
{
	if (m_activeSpeaker && s->GetSession() == m_activeSpeaker->GetProject())
		return;
	ProjectMap::iterator it = m_projects.find(s->GetSession());
	if (it == m_projects.end())
		return;
	m_activeSpeaker = it->second;
	SetSelectedItem(m_activeSpeaker);
}


IKMDNode* SessionRenderer::GetNodeFromPosition(const math::vector2d& pos)
{
	math::vector2d p = m_camera->ConvertToWorldSpace(pos);
	m_dataMutex->lock();
	IKMDNode* ret = 0;
	ProjectMap::iterator  it = m_projects.begin();

	for (; it != m_projects.end();++it)
	{
		ret= it->second->GetNodeFromPoint(p);
		if (ret)
			break;;
	}
	m_dataMutex->unlock();
	return ret;
}


void SessionRenderer::Update(float dt)
{
	//if (dt > 0.01)
	m_dataMutex->lock();
	m_physics->Update(dt);
	ProjectMap::iterator  it = m_projects.begin();

	for (; it != m_projects.end(); ++it)
	{
		it->second->Update(dt);
	}
	m_dataMutex->unlock();
	//m_translation.x += 20 * dt;
	//SetTransformation(m_translation, 0, 1);

	if (m_selectedItem)
		m_camera->FrameBox(m_selectedItem->GetBoundingBox(true));
	else
		m_camera->FrameBox(CalcAllBox());
	m_camera->Update(dt);
}
void SessionRenderer::Draw(float alpha)
{
	video::IVideoDevice* dev = Engine::getInstance().getDevice();

	float a = alpha*m_alpha;

	math::matrix4x4 oldT;
	dev->getTransformationState(video::TS_WORLD,oldT);
	m_camera->ApplyTransformation();
	m_dataMutex->lock();
	m_nodeRenderer->Clear();
	m_nodeRenderer->SetClippingVP(m_camera->GetWorldSpaceViewPort());
	ProjectMap::iterator  it, it2;
	for (int i = 0; i<(int)m_projectsSeq.size()-1; ++i)
	{
		m_nodeRenderer->AddSpeakerSpeaker(m_projectsSeq[i], m_projectsSeq[i+1]);
	}
	it = m_projects.begin();
	for (; it != m_projects.end(); ++it)
	{
		it->second->Draw(m_nodeRenderer,m_camera->GetWorldSpaceViewPort());
	}
	m_dataMutex->unlock();

	if (gAppData.Debugging)
	{
		dev->unuseShader();
		if (m_selectedItem)
		{
			dev->draw2DRectangle(m_selectedItem->GetBoundingBox(true), video::SColor(1,1,1,a), false);
		}
	}
	m_nodeRenderer->RenderAll(this, a);
	dev->setTransformationState(video::TS_WORLD, oldT);
}

math::rectf SessionRenderer::CalcAllBox()
{
	math::rectf rc;
	ProjectMap::iterator  it = m_projects.begin();
	for (; it != m_projects.end(); ++it)
	{
		math::rectf r = it->second->GetBoundingBox(true);
		if (it == m_projects.begin())
			rc = r;
		else
		{
			rc.addPoint(r.ULPoint);
			rc.addPoint(r.BRPoint);
		}
	}
	return rc;
}

void SessionRenderer::SetHoverdItem(IKMDNode* node)
{
	if (m_hoverItem)
		m_hoverItem->OnHoverOff();
	m_hoverItem = node;
	if (m_hoverItem)
		m_hoverItem->OnHoverOn();

}

void SessionRenderer::SetSelectedItem(IKMDNode* node)
{
	if (m_selectedItem)
		m_selectedItem->OnSelectedOff();
	m_selectedItem = node;
	if (!m_selectedItem)
		m_selectedItem = m_activeSpeaker;
	if (m_selectedItem)
		m_selectedItem->OnSelectedOn();

}

}
}
