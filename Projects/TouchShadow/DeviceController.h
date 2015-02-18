

/********************************************************************
	created:	2013/08/17
	created:	17:8:2013   23:44
	filename: 	C:\Development\mrayEngine\Projects\TouchShadow\DeviceController.h
	file path:	C:\Development\mrayEngine\Projects\TouchShadow
	file base:	DeviceController
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __DeviceController__
#define __DeviceController__

#include "ListenerContainer.h"
#include "UserFrame.h"
#include "GUIBatchRenderer.h"

namespace mray
{
	class OpenNICaptureDevice;
	class CUserData;
	class DeviceController;

	class IDeviceControllerListener
	{
	public:
		virtual void OnUserDetected(DeviceController* src,CUserData* user){}
	};

	enum EDebugElement
	{
		EDepth,
		EColor,
		ESkeleton,
		ECenterOfMath,
		EBoundingBox,
		EStatus,
		EFrameID,

		EDebugCount
	};

	class DeviceController:public ListenerContainer<IDeviceControllerListener*>
	{
	protected:
		OpenNICaptureDevice* m_device;
		typedef std::map<nite::UserId,CUserData*> UsersDataMap;
		UsersDataMap m_usersData;

		bool m_enableDebug;
		int m_id;

		video::ITexturePtr m_tex;

		bool m_debugElement[EDebugCount];

		math::matrix4x4 m_transMatrix;	

		OS::IThread *m_thread;
		OS::IMutex* m_mutex;

		UserFrame m_referenceFrame;
		bool m_captureUserFrame;
		bool m_autoCaptureUserFrame;
		math::rectf m_interRect;

		GCPtr<video::ITexture> m_interTex;
		OS::IMutex* m_interMutex;

		GCPtr<GUI::GUIBatchRenderer> m_guiRender;

	protected:

		void DrawLimb(nite::UserTracker* pUserTracker, const nite::SkeletonJoint& joint1, const nite::SkeletonJoint& joint2, const video::SColor&  color,bool _2d,
			scene::IDebugDrawManager* dbgMngr);
		void DrawSkeleton(nite::UserTracker* pUserTracker, const nite::Skeleton& sk,const video::SColor& clr,bool _2d,
			scene::IDebugDrawManager* dbgMngr);

		void updateUserState(const nite::UserData& user, uint64_t ts);
		bool CheckAgainstReference(UserFrame& frame);

		DECLARE_FIRE_METHOD(OnUserDetected,(DeviceController* src,CUserData* user),(src,user));

	public:
		DeviceController();
		virtual~DeviceController();

		void SetID(int id){m_id=id;}
		int GetID(){return m_id;}
		void SetDevice(OpenNICaptureDevice* device){m_device=device;}
		OpenNICaptureDevice* GetDevice(){return m_device;}
		void Update();
		void Display(scene::IDebugDrawManager* dbgMngr,scene::ViewPort* vp);

		void Clean();
		void Reset();

		void SetTransformationMatrix(const math::matrix4x4& m){m_transMatrix=m;}

		void EnableDebugging(bool e){Lock();m_enableDebug=e;Unlock();};
		bool IsEnableDebugging(){return m_enableDebug;}
		void SetDebugElement(EDebugElement d,bool e){m_debugElement[d]=e;}

		void Lock();
		void Unlock();

		void CaptureNextUser();
		void EnableAutoUpdateFrame(bool a)
		{
			m_autoCaptureUserFrame=a;
			if(!IsUserCaptured())
				CaptureNextUser();
		}
		bool IsAutoUpdateFrameEnabled(){return m_autoCaptureUserFrame;}
		bool IsUserCaptured(){return !m_captureUserFrame && m_referenceFrame.GetID()!=-1;}
		UserFrame& GetCapturedReference(){return m_referenceFrame;}
	};


}


#endif
