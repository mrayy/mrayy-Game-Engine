

#include "stdafx.h"
#include "CommentNode.h"
#include "KMDComment.h"
#include "KMDUser.h"
#include "InternetCacheManager.h"
#include "TextureResourceManager.h"
#include "ShaderResourceManager.h"

#include "NodeRenderer.h"
#include "CSubProject.h"

namespace mray
{
namespace scene
{

CommentNode::CommentNode(kmd::SessionDetails* speaker, kmd::KMDComment* t)
{
	m_hoverValue = 0;
	m_targetProject = speaker;
	m_comment = t;
	if (!m_comment || !m_comment->user)
	{
		m_texture.SetTexture(gTextureResourceManager.loadTexture2D("Faculty.png"));
	}
	else
	{
		video::ITexturePtr tex = gTextureResourceManager.getResource(m_comment->user->imageUrl);
		if (tex.isNull())
		{
			tex = gTextureResourceManager.loadTexture2D(m_comment->user->imageUrl);
		}
		m_texture.SetTexture(tex);
	}

}
CommentNode::~CommentNode()
{

}
kmd::IDType CommentNode::GetCommentID()
{
	if (m_comment)
		return m_comment->ID;
	return 0;
}

void CommentNode::Update(float dt)
{
	const float speed = 1;
#define DECREASE(x,l,v) if(x>(l)){ x-=(v);} if(x<(l)){x=(l);}
#define INCREASE(x,l,v) if(x<(l)){ x+=(v);} if(x>(l)){x=(l);}

	if (IsHovered(true))
	{
		INCREASE(m_hoverValue, 1,speed*dt);
	}
	else
	{
		DECREASE(m_hoverValue, 0,speed*dt);

	}
	if (IsHovered(false))
	{
		INCREASE(m_sizeScaler, 1, speed*dt);
	}
	else
	{
		DECREASE(m_sizeScaler, 0.5, speed*dt);
	}
	
	if (m_alpha < 1)
	{
		m_alpha +=0.1* dt;
		if (m_alpha > 1)
			m_alpha = 1;
	}

	m_phNode->SetRadius(m_sizeScaler*GetSize()*0.5);
}
void CommentNode::Draw(NodeRenderer *r, const math::rectf& rc)
{
	if (rc.IsRectCollide(GetBoundingBox(false)))
		r->AddComment(this, m_sizeScaler);

}

IKMDNode* CommentNode::GetNodeFromPoint(const math::vector2d& pos)
{
	math::rectf rc;
	rc.ULPoint = GetPosition() - m_sizeScaler*GetSize()*0.5;
	rc.BRPoint = GetPosition() + m_sizeScaler*GetSize()*0.5;
	if (rc.IsPointInside(pos))
		return this;
	return 0;
}
math::rectf CommentNode::GetBoundingBox(bool includeChildren)
{
	math::rectf rc = IKMDNode::GetBoundingBox(false);
	if (!includeChildren)
		return rc;
	return rc;
}

}
}
