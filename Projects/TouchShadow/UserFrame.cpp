

#include "stdafx.h"
#include "UserFrame.h"
#include "UserFrameSchema.h"
#include "CUserData.h"


namespace mray
{

	InteractionArea::InteractionArea(const InteractionAreaSchema& s,UserFrame* o):
owner(o),name(s.GetName()),percentageArea(s.GetArea()),weight(0),intersectionAmount(0),absolute(s.IsAbs()),targetJoint(s.GetTargetJoint())
{
}


UserFrame::UserFrame():m_userID(-1),m_texture(0)
{
}
UserFrame::~UserFrame(){}

void UserFrame::SetBoundingBox(const math::rectf& bb)
{
	m_userRect=bb;
	m_frameData.resize(bb.getWidth()*bb.getHeight());
}

void UserFrame::RefreshTexture()
{
	if(!m_texture)
	{
		m_texture=Engine::getInstance().getDevice()->createEmptyTexture2D(false);
	}
	m_texture->createTexture(math::vector3d(m_userRect.getWidth(),m_userRect.getHeight(),1),video::EPixel_R8G8B8A8);


	video::IHardwarePixelBuffer* buff= m_texture->getSurface(0);
	video::LockedPixelBox box= buff->lock(math::box3d(0,m_texture->getSize()),video::IHardwareBuffer::ELO_Discard);
	uchar* ptr=(uchar*)box.data;
	uchar* srcPtr=&m_frameData[0];
	for(int i=0;i<m_userRect.getWidth();++i)
	{
		for(int j=0;j<m_userRect.getHeight();++j)
		{
			int idx=m_userRect.getWidth()*j+i;
			ptr[4*idx+0]=(srcPtr[idx]!=0)?255:0;
			ptr[4*idx+1]=0;
			ptr[4*idx+2]=0;
			ptr[4*idx+3]=(srcPtr[idx]!=0)?255:0;
		}
	}
	buff->unlock();
}

bool UserFrame::CalculateBoundingBoxes()
{
	for(int i=0;i<m_interactionAreas.size();++i)
	{
		InteractionArea &a=m_interactionAreas[i];
		if(a.absolute || !m_userData)
		{
			a.absArea.ULPoint=m_userRect.ULPoint+a.percentageArea.ULPoint*m_userRect.getSize()*0.01f;
			a.absArea.BRPoint=a.absArea.ULPoint+a.percentageArea.BRPoint*m_userRect.getSize()*0.01f;
		}else
		{
			math::vector2d center;
			const nite::SkeletonJoint& j= m_userData->GetSkeleton().getJoint(a.targetJoint);
			if(j.getPositionConfidence()<0.5)
				return false;
			nite::Status st= m_userData->GetTracker()->convertJointCoordinatesToDepth(j.getPosition().x,j.getPosition().y,j.getPosition().z,&center.x,&center.y);
			if(st==nite::STATUS_OK)
			{
				a.absArea.ULPoint=center+a.percentageArea.ULPoint*m_userRect.getSize()*0.01f;
				a.absArea.BRPoint=center+a.percentageArea.BRPoint*m_userRect.getSize()*0.01f;
			}else
				return false;
		}
		math::rectf& r=a.absArea;
		if(r.ULPoint.x<m_userRect.ULPoint.x)r.ULPoint.x=m_userRect.ULPoint.x;
		if(r.ULPoint.y<m_userRect.ULPoint.y)r.ULPoint.y=m_userRect.ULPoint.y;
		if(r.BRPoint.x>m_userRect.BRPoint.x)r.BRPoint.x=m_userRect.BRPoint.x;
		if(r.BRPoint.y>m_userRect.BRPoint.y)r.BRPoint.y=m_userRect.BRPoint.y;

	}
	return true;
}
void UserFrame::CalculateWeights()
{
	uchar* ptr=&m_frameData[0];
	for(int i=0;i<m_interactionAreas.size();++i)
	{
		m_interactionAreas[i].Reset();
		math::rectf& rc=m_interactionAreas[i].absArea;
		for(int y=rc.ULPoint.y;y<rc.BRPoint.y;++y)
		{
			for(int x=rc.ULPoint.x;x<rc.BRPoint.x;++x)
			{
				int idx=y*rc.getWidth()+x;
				if(ptr[idx]!=0)
					++m_interactionAreas[i].weight;
			}
		}
		printf("%d,%d: %f\n",i,m_interactionAreas[i].weight);
		if(m_interactionAreas[i].weight!=0)
			m_interactionAreas[i].weight=1.0f/m_interactionAreas[i].weight;
	}
}

InteractionArea* UserFrame::GetArea(const core::string&name)
{
	std::map<core::string,int>::iterator it= m_areaMap.find(name);
	if(it==m_areaMap.end())
		return 0;
	return &m_interactionAreas[it->second];
}


void UserFrame::LoadSchema(const UserFrameSchema* s)
{
	m_interactionAreas.clear();
	const std::vector<InteractionAreaSchema>& lst=s->GetAreaSchema();
	std::vector<InteractionAreaSchema>::const_iterator it=lst.begin();
	for (;it!=lst.end();++it)
	{
		m_interactionAreas.push_back(InteractionArea(*it,this));
		m_areaMap[(*it).GetName()]=m_interactionAreas.size()-1;
	}
}


}
