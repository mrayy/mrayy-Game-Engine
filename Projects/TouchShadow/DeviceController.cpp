



#include "stdafx.h"
#include "DeviceController.h"
#include "OpenNICaptureDevice.h"
#include "CUserData.h"
#include "UserDebugData.h"
#include "OpenNIUtils.h"
#include "AppCommon.h"
#include "JointLimb.h"
#include "IDebugDrawManager.h"
#include "IThreadFunction.h"
#include "IThread.h"
#include "IThreadManager.h"
#include "ViewPort.h"
#include "MutexLocks.h"
#include "FontResourceManager.h"

#include "DemoSharedData.h"
#include "InteractionDemo.h"

namespace mray
{


	class DeviceControllerThread:public OS::IThreadFunction
	{
	public:
		DeviceController* m_controller;
	public:
		DeviceControllerThread(DeviceController*s)
		{
			m_controller=s;
		}
		virtual void execute(OS::IThread*caller,void*arg)
		{
			float dt=1.0f/30.0f;
			while(caller->isActive())
			{
				m_controller->Update();
				OS::IThreadManager::getInstance().sleep(1000*dt);
			}
		}
	};


DeviceController::DeviceController()
{
	m_id=0;
	m_device=0;
	m_enableDebug=true;
	for (int i=0;i<EDebugCount;++i)
	{
		m_debugElement[i]=false;
	}
	m_debugElement[EDepth]=true;
	m_debugElement[ESkeleton]=true;
	m_debugElement[EStatus]=true;

	m_tex=Engine::getInstance().getDevice()->createEmptyTexture2D(false);

	m_mutex=OS::IThreadManager::getInstance().createMutex();
	m_interMutex=OS::IThreadManager::getInstance().createMutex();


	m_thread=OS::IThreadManager::getInstance().createThread(new DeviceControllerThread(this));
	m_thread->start(0);

	m_autoCaptureUserFrame=false;
	m_captureUserFrame=false;

	m_guiRender=new GUI::GUIBatchRenderer;
	m_guiRender->SetDevice(Engine::getInstance().getDevice());

	m_interTex=Engine::getInstance().getDevice()->createEmptyTexture2D(false);

	DemoSharedData::Instance()->ActiveUserFrame=&m_referenceFrame;
}

DeviceController::~DeviceController()
{
	Lock();
	OS::IThreadManager::getInstance().killThread(m_thread);
	m_thread->terminate();
	m_tex=0;
	delete m_device;
	Unlock();
	delete m_mutex;
	delete m_interMutex;
}
void DeviceController::Clean()
{
	Lock();

	UsersDataMap::iterator it= m_usersData.begin();
	for (;it!=m_usersData.end();++it)
	{
		delete it->second;
	}
	m_usersData.clear();
	m_referenceFrame=UserFrame();

	Unlock();
}
void DeviceController::Reset()
{
	Lock();

	m_device->Close();
	Clean();
	m_device->Open();
	Unlock();
}

void DeviceController::DrawLimb(nite::UserTracker* pUserTracker, const nite::SkeletonJoint& joint1, const nite::SkeletonJoint& joint2, const video::SColor&  color,bool _2d,
	scene::IDebugDrawManager* dbgMngr)
{
	math::vector3d coordinates[2] ;
	if(_2d)
	{
		pUserTracker->convertJointCoordinatesToDepth(joint1.getPosition().x, joint1.getPosition().y, joint1.getPosition().z, &coordinates[0].x, &coordinates[0].y);
		pUserTracker->convertJointCoordinatesToDepth(joint2.getPosition().x, joint2.getPosition().y, joint2.getPosition().z, &coordinates[1].x, &coordinates[1].y);
	}else
	{
		coordinates[0]=ToVectro3d(joint1.getPosition());
		coordinates[1]=ToVectro3d(joint2.getPosition());
	}

	if (joint1.getPositionConfidence() < 0.1f || joint2.getPositionConfidence() < 0.1f)
	{
		return;
	}

	dbgMngr->AddLine(coordinates[0]/10,coordinates[1]/10,color,10);
	dbgMngr->AddCircle(coordinates[0],10,math::vector3d::ZAxis,color);
	dbgMngr->AddCircle(coordinates[1],10,math::vector3d::ZAxis,color);
}
void DeviceController::DrawSkeleton(nite::UserTracker* pUserTracker, const nite::Skeleton& sk,const video::SColor& clr,bool _2d,
	scene::IDebugDrawManager* dbgMngr)
{
	for(int i=0;i<JointLimb::Limbs.size();++i)
	{
		DrawLimb(pUserTracker, sk.getJoint(JointLimb::Limbs[i].a), sk.getJoint(JointLimb::Limbs[i].b), clr,_2d,dbgMngr);
	}
}
void DeviceController::Display(scene::IDebugDrawManager* dbgMngr,scene::ViewPort* vp)
{
	if(!m_device || !m_device->IsOpen())
		return;
	if(!m_enableDebug)
		return;

	Engine::getInstance().getDevice()->set2DMode();

	{
		if(m_enableDebug)
		{
			video::TextureUnit tex;
			Lock();
			tex.SetTexture(m_tex);
			Engine::getInstance().getDevice()->useTexture(0,&tex);
			Engine::getInstance().getDevice()->draw2DImage(vp->getAbsRenderingViewPort(),video::SColor(1));
			Unlock();
			if(IsUserCaptured())
			{
				math::rectf rc=GetCapturedReference().GetBoundingBox();
				math::vector2d ratio((float)vp->getAbsRenderingViewPort().getWidth()/(float)m_device->GetDepthStream().getVideoMode().getResolutionX(),
					(float)vp->getAbsRenderingViewPort().getHeight()/(float)m_device->GetDepthStream().getVideoMode().getResolutionY());
				rc.ULPoint*=ratio;
				rc.BRPoint*=ratio;
				
				math::rectf refBB=rc;
				Lock();

				tex.SetTexture(m_referenceFrame.GetTexture());
				Engine::getInstance().getDevice()->useTexture(0,&tex);
				Engine::getInstance().getDevice()->draw2DImage(rc,video::SColor(1));
				rc=m_interRect;
				rc.ULPoint*=ratio;
				rc.BRPoint*=ratio;
				tex.SetTexture(m_interTex);
				m_interMutex->lock();
				Engine::getInstance().getDevice()->useTexture(0,&tex);
				Engine::getInstance().getDevice()->draw2DImage(rc,video::SColor(1));
				m_interMutex->unlock();

				Unlock();


				GUI::IFont* font= gFontResourceManager.getDefaultFont();
				

				const IntractAreaList& lst= m_referenceFrame.GetInteractionAreas();
				for(int i=0;i<lst.size();++i)
				{
					rc=lst[i].absArea;
					rc.ULPoint*=ratio;
					rc.BRPoint*=ratio;
					float amount=lst[i].intersectionAmount;
					Engine::getInstance().getDevice()->draw2DRectangle(rc,video::SColor(0,0,amount,amount),true);
					Engine::getInstance().getDevice()->draw2DRectangle(rc,video::SColor(amount,amount,amount,1),false);

					if(font)
					{
						GUI::FontAttributes attr;
						attr.fontSize=18;
						attr.fontColor.Set(1,1,1,1);
						core::string msg=core::StringConverter::toString((int)(amount*100))+"%";
						font->print(rc,&attr,0,msg,m_guiRender);

					}

				}
				m_guiRender->Flush();
			}
		}
	}
#if 0
	const nite::Array<nite::UserData>& users = m_device->GetFrame()->getUsers();
	for (int i = 0; i < users.getSize(); ++i)
	{
		const nite::UserData& user = users[i];
		updateUserState(user, m_device->GetFrame()->getTimestamp());
		if (user.isNew())
		{
			m_device->GetTracker()->startSkeletonTracking(user.getId());
		}
		for(UsersDataMap::iterator it=m_usersData.begin();it!=m_usersData.end();++it)
		{
			CUserData&user=*it->second;
			if (!user.IsLost())
			{
				if (user.GetSkeleton().getState() == nite::SKELETON_TRACKED && m_debugElement[ESkeleton])
				{
					DrawSkeleton(m_device->GetTracker(),user.GetSkeleton(),AppCommon::Colors[(user.GetID()+m_id) % COLORS_COUNT],true,dbgMngr);
				}/*
				if (m_debugElement[ECenterOfMath])
				{
					DrawCenterOfMass(m_device->GetTracker(), user);
				}
				if (m_debugElement[EBoundingBox])
				{
					DrawBoundingBox(user);
				}

				if (m_debugElement[EStatus])
				{
					DrawStatusLabel(m_device->GetTracker(),user.GetDebugString()->debugString.c_str(), user);
				}*/
			}
		}

		if (m_debugElement[EFrameID])
		{
		//	DrawFrameId(m_device->GetFrame()->getFrameIndex());
		}

	}
#endif
}
void DeviceController::updateUserState(const nite::UserData& user, uint64_t ts)
{
	CUserData* usrPtr=0;
	UsersDataMap::iterator it= m_usersData.find(user.getId());
	if (user.isNew() || it==m_usersData.end())
	{
		usrPtr=new CUserData(user);
		usrPtr->SetTracker(m_device->GetTracker());
		m_usersData[user.getId()]=usrPtr;
		usrPtr->GetDebugString()->debugString="New";
		FIRE_LISTENR_METHOD(OnUserDetected,(this,usrPtr));
		printf("[%d] New User: %d\n",m_id,(int)user.getId());
	}else
	{
		usrPtr=it->second;
	}

	if(usrPtr->GetSkeleton().getState()!= user.getSkeleton().getState())
	{
		switch(user.getSkeleton().getState())
		{
		case nite::SKELETON_NONE:
			usrPtr->GetDebugString()->debugString=("Stopped tracking.");
			break;
		case nite::SKELETON_CALIBRATING:
			usrPtr->GetDebugString()->debugString=("Calibrating...");
			printf("[%d] User Calibrating: %d\n",m_id,(int)user.getId());
			break;
		case nite::SKELETON_TRACKED:
			{
				char str[128];
				sprintf(str,"Player:%d",user.getId());
				usrPtr->GetDebugString()->debugString=(str);
				printf("[%d] User Tracking: %d\n",m_id,(int)user.getId());
			}
			break;
		case nite::SKELETON_CALIBRATION_ERROR_NOT_IN_POSE:
		case nite::SKELETON_CALIBRATION_ERROR_HANDS:
		case nite::SKELETON_CALIBRATION_ERROR_LEGS:
		case nite::SKELETON_CALIBRATION_ERROR_HEAD:
		case nite::SKELETON_CALIBRATION_ERROR_TORSO:
			usrPtr->GetDebugString()->debugString=("Calibration Failed... :-|");
			break;
		}
	}
	usrPtr->UpdateState(user);
	usrPtr->TransformByMatrix(m_transMatrix);
}

bool intersected(const math::rectf& r1,const math::rectf& r2)
{
	return !(r2.ULPoint.x > r1.BRPoint.x
		|| r2.BRPoint.x < r1.ULPoint.x
		|| r2.ULPoint.y > r1.BRPoint.y
		|| r2.BRPoint.y < r1.ULPoint.y);
}

math::rectf intersect(const math::rectf &bb,const math::rectf &ref)
{
	math::rectf intersection;
	if(ref.ULPoint.x>bb.ULPoint.x && ref.ULPoint.x<bb.BRPoint.x)
		intersection.ULPoint.x=ref.ULPoint.x;
	else
		intersection.ULPoint.x=bb.ULPoint.x;

	if(ref.ULPoint.y>bb.ULPoint.y && ref.ULPoint.y<bb.BRPoint.y)
		intersection.ULPoint.y=ref.ULPoint.y;
	else
		intersection.ULPoint.y=bb.ULPoint.y;

	if(ref.BRPoint.x>bb.ULPoint.x && ref.BRPoint.x<bb.BRPoint.x)
		intersection.BRPoint.x=ref.BRPoint.x;
	else
		intersection.BRPoint.x=bb.BRPoint.x;

	if(ref.BRPoint.y>bb.ULPoint.y && ref.BRPoint.y<bb.BRPoint.y)
		intersection.BRPoint.y=ref.BRPoint.y;
	else
		intersection.BRPoint.y=bb.BRPoint.y;
	return intersection;


}
bool DeviceController::CheckAgainstReference(UserFrame& frame)
{
	const math::rectf &bb=frame.GetBoundingBox();
	const math::rectf &ref=m_referenceFrame.GetBoundingBox();
	if(!intersected(bb,ref))
		return false;

	m_interRect=intersect(bb,ref);

	//"intersection" holds the shared area between both frames.
	// now check for actual intersection between them using pixel data
	video::IHardwarePixelBuffer* buff=0;
	uchar* ptr=0;
	if(m_enableDebug)
	{
		m_interTex->createTexture(math::vector3d(m_interRect.getWidth(),m_interRect.getHeight(),1),video::EPixel_R8G8B8A8);
		buff= m_interTex->getSurface(0);
		video::LockedPixelBox box= buff->lock(math::box3d(0,m_interTex->getSize()),video::IHardwareBuffer::ELO_Discard);
		ptr=(uchar*)box.data;
		m_interMutex->lock();
	}

	uchar* fData=&frame.GetFrameData()[0];
	uchar* rData=&m_referenceFrame.GetFrameData()[0];

	IntractAreaList& lst= m_referenceFrame.GetInteractionAreas();

	//reset intersections
	for(int area=0;area<lst.size();++area)
	{
		lst[area].intersectionAmount=0;
	}
	for (int j=0;j<m_interRect.getHeight();++j)
	{
		int rY=m_interRect.ULPoint.y-ref.ULPoint.y + j;
		for(int i=0;i<m_interRect.getWidth();++i)
		{
			int rX=m_interRect.ULPoint.x-ref.ULPoint.x + i;
			int fIdx=(m_interRect.ULPoint.y-bb.ULPoint.y + j)*bb.getWidth()+(m_interRect.ULPoint.x-bb.ULPoint.x + i);
			int rIdx=(rY)*ref.getWidth()+(rX);
			int idx=j*m_interRect.getWidth()+i;
			bool isInter=(fData[fIdx]!=0 && rData[rIdx]!=0);
			int hPos=0,vPos=0;
			if(isInter)
			{
				math::vector2d pt(m_interRect.ULPoint.x+i,m_interRect.ULPoint.y+j);
				//search for the target area containing this pixel
				for(int area=0;area<lst.size();++area)
				{
					if(lst[area].absArea.IsPointInside(pt))
						++lst[area].intersectionAmount;
				}
			}
			if(m_enableDebug)
			{
				ptr[4*idx+0]=0;
				ptr[4*idx+1]=isInter?255:0;
				ptr[4*idx+2]=0;
				ptr[4*idx+3]=isInter?255:0;
			}
		}
	}

	//normalize intersections
	for(int i=0;i<lst.size();++i)
	{
		lst[i].intersectRect=intersect(lst[i].absArea,m_interRect);
		lst[i].intersectionAmount*=lst[i].weight;
		lst[i].intersectionAmount=math::clamp(lst[i].intersectionAmount,0.0f,1.0f);
	}
	if(m_enableDebug)
	{
		buff->unlock();
		m_interMutex->unlock();
	}
	return true;
}
void DeviceController::Update()
{

	if(!m_device || !m_device->IsOpen() || !m_device->Update())
		return;

	OS::ScopedLock lock(m_mutex);

	const nite::Array<nite::UserData>& users = m_device->GetFrame()->getUsers();
	for (int i = 0; i < users.getSize(); ++i)
	{
		const nite::UserData& user = users[i];

		updateUserState(user, m_device->GetFrame()->getTimestamp());
		if (user.isNew())
		{
			m_device->GetTracker()->startSkeletonTracking(user.getId());
		}
	}
	if(!m_device->GetFrame()->isValid())
		return;
	{

		openni::VideoFrameRef frame;
		const nite::Array<nite::UserData>& users= m_device->GetFrame()->getUsers();
		const nite::UserMap& userMap = m_device->GetFrame()->getUserMap();
		const nite::UserId* userLabels=userMap.getPixels();

		UserFrame userFrame;

		for(int i=0;i<users.getSize();++i)
		{
			if(users[i].getId()==m_referenceFrame.GetID() && IsUserCaptured() )
				continue;
			UserFrame*userFrameRef=&userFrame;
// 			math::rectf bbox(users[0].getBoundingBox().min.x,users[0].getBoundingBox().min.y,
// 				users[0].getBoundingBox().max.x,users[0].getBoundingBox().max.y);
			math::rectf bbox(users[i].getBoundingBox().min.x,users[i].getBoundingBox().min.y,
				users[i].getBoundingBox().max.x,users[i].getBoundingBox().max.y);
			if(bbox.getWidth()*bbox.getHeight()<50 || !bbox.isValid())
				continue;
			if(m_captureUserFrame)
			{
				userFrameRef=&m_referenceFrame;
			}
			userFrameRef->SetID(users[i].getId());
			userFrameRef->SetBoundingBox(bbox);

			UsersDataMap::iterator it=m_usersData.find(users[i].getId());
			if(it!=m_usersData.end())
				userFrameRef->SetUserData(it->second);
			if(bbox.getWidth()*bbox.getHeight()!=0)
			{
				uchar* frameData=&userFrameRef->GetFrameData()[0];
				for(int y=bbox.ULPoint.y, iy=0;y<bbox.BRPoint.y;++y,++iy)
				{
					for(int x=bbox.ULPoint.x,ix=0;x<bbox.BRPoint.x;++x,++ix)
					{
						int idx=y*userMap.getWidth()+x;
						int idx2=iy*bbox.getWidth()+ix;
						frameData[idx2]=(userLabels[idx]==users[i].getId());

					}
				}

				if(m_captureUserFrame)
				{
					if(DemoSharedData::Instance()->ActiveDemo)
					{
						m_referenceFrame.LoadSchema(DemoSharedData::Instance()->ActiveDemo->GetUserFrame());
					}
					if(!m_referenceFrame.CalculateBoundingBoxes())
						continue;
					if(m_enableDebug)
						m_referenceFrame.RefreshTexture();
					m_referenceFrame.CalculateWeights();
					m_captureUserFrame=false;
				}
			}else
				continue;

			if(IsUserCaptured())
			{
				CheckAgainstReference(userFrame);
			}
		//	break;
		}
		bool drawframe=m_debugElement[EDepth] || m_debugElement[EColor];
		if(drawframe && m_enableDebug)
		{
			if(m_debugElement[EDepth])
				m_device->GetDepthStream().readFrame(&frame);
			else if(m_debugElement[EColor])
				m_device->GetColorStream().readFrame(&frame);
			else
				m_device->GetDepthStream().readFrame(&frame);


			std::vector<float> histo,*ptr=0;
			if(m_debugElement[EDepth])
			{
				histo.resize(10000);
				OpenNIUtils::CalculateHistogram(frame,histo);
				ptr=&histo;
			}
			if(m_tex->getSize().x!=frame.getWidth() || m_tex->getSize().y!=frame.getHeight())
			{
				m_tex->createTexture(math::vector3di(frame.getWidth(),frame.getHeight(),1),video::EPixel_R8G8B8);
			}
			video::IHardwarePixelBuffer* buff= m_tex->getSurface(0);
			video::LockedPixelBox box= buff->lock(math::box3d(0,m_tex->getSize()),video::IHardwareBuffer::ELO_Discard);
			OpenNIUtils::ConvertToTexture(frame,box,ptr);
			buff->unlock();
		}
	}
}

void DeviceController::Lock()
{
	m_mutex->lock();
}
void DeviceController::Unlock()
{
	m_mutex->unlock();
}


void DeviceController::CaptureNextUser()
{
	Lock();
	m_captureUserFrame=true;
	m_referenceFrame.SetID(-1);
	Unlock();
}

}


