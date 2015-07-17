
#include "stdafx.h"
#include "NodeRenderer.h"
#include "CommentNode.h"
#include "ProjectNode.h"
#include "CSubProject.h"
#include "SessionDetails.h"
#include "ConnectionLineRenderer.h"

#include "ShaderResourceManager.h"
#include "SessionRenderer.h"

namespace mray
{
namespace scene
{

	const core::string NodeRenderer::ProjectNodeShader =
		"float4 main_fp(float2 texCoord : TEXCOORD0, "
		"uniform sampler2D texA : register(s0)) : COLOR "
		"{"
		"vec4 clr=tex2D(texA,texCoord);"
		"float2 v=(texCoord-0.5);"
		"float dst=dot(v,v);"
		"dst=dst>0.25?0:1;"
		"return float4(clr.rgb,dst);"
		"}";
	const core::string NodeRenderer::CommentNodeShader =
		"half4 main_fp(float2 texCoord : TEXCOORD0, "
		"uniform sampler2D texA : register(s0),uniform float Gray,uniform float Alpha) : COLOR "
		"{"
		"half4 clr=tex2D(texA,texCoord);"
		"half2 v=(texCoord-0.5);"
		"half dst=dot(v,v);"
		"dst=dst>0.25?0:1;"
		"return half4(clr.rgb,dst);"
		"}";
	const core::string NodeRenderer::ConnectionLineShader =
		"float4 main_fp(uniform float alpha) : COLOR "
		"{"
		"return float4(1,1,1,alpha);"
		"}";
NodeRenderer::NodeRenderer()
{
	m_connRenderer = new ConnectionLineRenderer();
	m_ProjectNodeShader = gShaderResourceManager.loadShaderFromProgram("ProjectNodeShader", ProjectNodeShader, video::EShader_FragmentProgram, "main_fp",video::ShaderPredefList(), mT("cg"));
	gShaderResourceManager.addResource(m_ProjectNodeShader, "ProjectNodeShader");

	m_CommentNodeShader = gShaderResourceManager.loadShaderFromProgram("CommentNodeShader", CommentNodeShader, video::EShader_FragmentProgram, "main_fp", video::ShaderPredefList(), mT("cg"));
	gShaderResourceManager.addResource(m_ProjectNodeShader, "CommentNodeShader");

	m_connectionShader = gShaderResourceManager.loadShaderFromProgram("ConnectionLineShader",ConnectionLineShader, video::EShader_FragmentProgram, "main_fp", video::ShaderPredefList(), mT("cg"));
	gShaderResourceManager.addResource(m_ProjectNodeShader, "ConnectionLineShader");
}

NodeRenderer::~NodeRenderer()
{
	delete m_connRenderer;
}


void NodeRenderer::Clear()
{
	m_speakerConn.clear();
	m_CommentsConn.clear();
	m_Comments.clear();
	m_speakers.clear();
	m_connRenderer->Reset();
}
void NodeRenderer::_renderConnections(SessionRenderer *r, float alpha)
{
	m_connRenderer->update(0);
	Engine::getInstance().getDevice()->useTexture(0, 0);
	Engine::getInstance().getDevice()->useTexture(1, 0);	
	Engine::getInstance().getDevice()->unuseShader();
//	Engine::getInstance().getDevice()->setFragmentShader(m_connectionShader);
	Engine::getInstance().getDevice()->setLineWidth(3);
	if(false)
	{
		Engine::getInstance().getDevice()->drawSingleMesh(m_connRenderer->getMesh()->getBuffer(0));
		return;
	}
	alpha = 1;
	video::IVideoDevice* dev = Engine::getInstance().getDevice();
	math::vector2d points[2];
	for (int i = 0; i < m_CommentsConn.size(); ++i)
	{
		points[0]=(m_CommentsConn[i].a->GetPosition());
		points[1] = (m_CommentsConn[i].b->GetPosition());
		float len = points[0].getDist(points[1]);
		len = 1-math::Min<float>(len, 250) / 250.0f;
		dev->draw2DLine(points, 2, video::SColor(1, 1, 1, alpha*len*len	));
	}
	Engine::getInstance().getDevice()->setLineWidth(5);
	for (int i = 0; i < m_speakerConn.size(); ++i)
	{
		points[0] = (m_speakerConn[i].a->GetPosition());
		points[1] = (m_speakerConn[i].b->GetPosition());
		dev->draw2DLine(points, 2, video::SColor(1, 0, 0, 0.5*alpha));
	}

}

void NodeRenderer::_renderSpeakers(SessionRenderer *r, float alpha)
{
	video::IVideoDevice* dev = Engine::getInstance().getDevice();
	dev->setFragmentShader(0);
	video::TextureUnit tex;
	math::vector2d pos;
	tex.SetTexture(gTextureResourceManager.loadTexture2D("GlowBG.png"));
	dev->useTexture(0, &tex);

	for (int i = 0; i < m_speakers.size(); ++i)
	{
		scene::ProjectNode* speaker = m_speakers[i].node;

		pos = speaker->GetPosition();

		float scale = speaker->GetSize()*m_speakers[i].glow;
		math::rectf rc(pos, pos);
		rc.ULPoint -= scale*0.8;
		rc.BRPoint += scale*0.8;


		dev->draw2DImage(rc, video::SColor(1,1,1,m_speakers[i].glow*alpha));
	}
	dev->setFragmentShader(m_ProjectNodeShader);

	for (int i = 0; i < m_speakers.size(); ++i)
	{
		scene::ProjectNode* speaker = m_speakers[i].node;

		tex.SetTexture(speaker->GetProject()->GetTexture());
		dev->useTexture(0, &tex);
		dev->draw2DImage(speaker->GetBoundingBox(false), 1);
	}
	dev->setFragmentShader(0);
}

void NodeRenderer::_renderComments(SessionRenderer *r, float alpha)
{
	video::IVideoDevice* dev = Engine::getInstance().getDevice();
	dev->setFragmentShader(m_CommentNodeShader);
	math::vector2d pos;

	for (int i = 0; i < m_Comments.size(); ++i)
	{
		scene::CommentNode* Comment = m_Comments[i].node;

		pos = Comment->GetPosition();

		float g = 1 - Comment->GetHoverValue();
		float a = Comment->GetAlpha()*alpha;
		m_CommentNodeShader->setConstant("Gray", &g, 1);
		m_CommentNodeShader->setConstant("Alpha", &a, 1);

		float scale = Comment->GetSize()*m_Comments[i].scale;
		math::rectf rc(Comment->GetPosition(), Comment->GetPosition());
		rc.ULPoint -= scale*0.5;
		rc.BRPoint += scale*0.5;

		dev->useTexture(0, &Comment->GetImage());
		dev->draw2DImage(rc, 1);
	}
	dev->setFragmentShader(0);

}

void NodeRenderer::RenderAll(SessionRenderer *r, float alpha)
{
	_renderConnections(r,alpha);
	_renderSpeakers(r, alpha);
	_renderComments(r, alpha);
	Engine::getInstance().getDevice()->unuseShader();
}


void NodeRenderer::AddSpeakerSpeaker(ProjectNode*a, ProjectNode* b)
{/*
	math::rectf combRC = a->GetBoundingBox(false);
	combRC.addPoint(b->GetBoundingBox(false).ULPoint);
	combRC.addPoint(b->GetBoundingBox(false).BRPoint);
	if (!m_clipRect.IsRectCollide(combRC))
		return;*/
	SpeakerConnectionInfo c;
	c.a = a;
	c.b = b;
	m_speakerConn.push_back(c);
	math::vector2d pa = a->GetPosition();
	math::vector2d pb = b->GetPosition();
	m_connRenderer->AddConnection(math::vector3d(pa.x, pa.y, 0), math::vector3d(pb.x, pb.y, 0), video::SColor(1, 0, 0, 0.5), video::SColor(1, 0, 0, 1));
}

void NodeRenderer::AddSpeakerCommentNode(ProjectNode*a, CommentNode* b)
{
	if (!m_clipRect.IsRectCollide(a->GetBoundingBox(false))
		&& !m_clipRect.IsRectCollide(b->GetBoundingBox(false)))
		return;
	NodeConnectionInfo c;
	c.a = a;
	c.b = b;
	m_CommentsConn.push_back(c);

	math::vector2d pa = a->GetPosition();
	math::vector2d pb = b->GetPosition();
	m_connRenderer->AddConnection(math::vector3d(pa.x, pa.y, 0), math::vector3d(pb.x, pb.y, 0), video::SColor(1, 0, 0, 1), video::SColor(1, 1, 1, 1));
}

void NodeRenderer::AddComment(CommentNode* node,float scale)
{
	CommentNodeInfo ifo;
	ifo.node = node;
	ifo.scale= scale;
	m_Comments.push_back(ifo);
}

void NodeRenderer::AddSpeaker(ProjectNode* node,float glow)
{
	ProjectNodeInfo ifo;
	ifo.node = node;
	ifo.glow = glow;
	m_speakers.push_back(ifo);

}

}
}



