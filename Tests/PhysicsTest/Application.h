


/********************************************************************
	created:	2012/06/26
	created:	26:6:2012   14:53
	filename: 	d:\Development\mrayEngine\Tests\PhysicsTest\Application.h
	file path:	d:\Development\mrayEngine\Tests\PhysicsTest
	file base:	Application
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __Application__
#define __Application__

#include "CMRayApplication.h"
#include "GUIBatchRenderer.h"
#include "GUIManager.h"
#include "GameEntity.h"
#include "GameEntityManager.h"
#include "ViewPort.h"
#include "MeshBufferData.h" 
#include "mraynet.h"
#include "IUDPClient.h"
#include "OptiTrackClient.h"
#include "VertConstGeoInstancing.h"

#include "DirectShowVideoGrabber.h"

#include "CarController.h"


namespace mray
{


	class OptiTrackController:public animation::IOptiTrackClientListener
	{
		int m_id;
		scene::IMovable*m_node;
	public:
		OptiTrackController(int id,scene::IMovable*node);
		void OnOptiTrackData(animation::OptiTrackClient*client,const animation::OptiTrackRigidBody& body);
	};


	
	class PositionEstimator
	{
		typedef math::vector3d T;
		std::vector<T> m_samples;
		T m_mean;
		T m_variance;
	public:

		void AddSample(const T& v)
		{
			m_samples.push_back(v);
		}
		void Calculate()
		{
			m_mean=m_samples[0];
			for(int i=1;i<m_samples.size();++i)
			{
				m_mean+=m_samples[i];
			}
			m_mean/=(float)m_samples.size();
			m_variance=math::sqr(m_samples[0]-m_mean);
			for(int i=1;i<m_samples.size();++i)
			{
				m_variance+=math::sqr(m_samples[i]-m_mean);
			}
			m_variance/=(float)m_samples.size();
			m_variance.x=sqrt(m_variance.x);
			m_variance.y=sqrt(m_variance.y);
			m_variance.z=sqrt(m_variance.z);
		}
		const T& Mean()const		
		{
			return m_mean;
		}
		const T& Variance()const		
		{
			return m_variance;
		}
	};

	class CarTrackController:public animation::IOptiTrackClientListener
	{
	protected:
		int m_id;
		CarController* m_car;

		PositionEstimator m_headEstimator;
		PositionEstimator m_rotateEstimator;
		float m_initTime;
		bool m_initing;
	public:

		math::vector3d m_lastSpeed;
		math::vector3d m_lastAngles;

		CarTrackController(int id,CarController*node);
		void OnOptiTrackData(animation::OptiTrackClient*client,const animation::OptiTrackRigidBody& body);
	};

class Application:public CMRayApplication,public animation::IOptiTrackClientListener
{
private:
protected:
	GCPtr<scene::LightNode> m_light;
	GCPtr<scene::CameraNode> m_camera;
	GCPtr<GUI::GUIBatchRenderer> m_guiRender;
	GCPtr<physics::IPhysicManager> m_phManager;
	GCPtr<GUI::GUIManager> m_guiManager;
	GCPtr<game::GameEntityManager> m_gameManager;
	scene::ViewPort* m_viewPort;

	GCPtr<animation::OptiTrackClient> m_optiTrack;
	GCPtr<OptiTrackController> m_camController;
	GCPtr<network::INetwork> m_network;
	GCPtr<network::IUDPClient> m_udpClient;
	GCPtr<OS::IThread> m_videoThread;
	GCPtr<video::ITexture> m_videoTex;

	GCPtr<video::CMultiPassPP> m_imageProcessor;
	GCPtr<video::IRenderTarget> m_targetImage;
	GCPtr<scene::IMeshBuffer> m_mbuffer;

	GCPtr<video::GPUShader> m_renderShader;


	GCPtr<CarController> m_carController;
	GCPtr<CarTrackController> m_optiCarController;

	//typedef std::map<scene::SMesh*, GCPtr<scene::VertConstGeoInstancing>> InstanceMap;
	typedef std::vector<GCPtr<scene::VertConstGeoInstancing>> InstanceMap;
	InstanceMap m_instances;
	GCPtr<scene::VertConstGeoInstancing> m_allInstances;
	void _AddInstanceModel(scene::MeshRenderableNode* buff);

	math::matrix4x4 m_proj,m_view;

	GCPtr<video::DirectShowVideoGrabber> m_grabber;
public:


	Application();
	virtual~Application();


	game::GameEntity* CreateGameEntity(const core::string& modelPath,const core::string& collision,const math::vector3d& pos);
	virtual void onEvent(Event* event);

	void CreatePhysicsSystem();

	virtual void init(const OptionContainer &extraOptions);

	virtual void draw(scene::ViewPort* vp);
	virtual void WindowPostRender(video::RenderWindow* wnd);
	virtual void update(float dt);
	virtual void onDone();

	void _NewImageRecieved(video::ImageInfo* image);

	void OnOptiTrackData(animation::OptiTrackClient*client,const animation::OptiTrackRigidBody& body);

};


}

#endif
