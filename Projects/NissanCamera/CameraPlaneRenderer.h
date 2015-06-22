

#ifndef __CAMERAPLANERENDERER__
#define __CAMERAPLANERENDERER__

#include "ParsedShaderPP.h"
#include "TBeeCommon.h"
#include "OffAxisProjection.h"

namespace mray
{
	namespace TBee
	{
		class ICameraVideoSource;
		class TelubeeCameraConfiguration;
	}
namespace NCam
{
	class CarObjects;

class CameraPlaneRenderer
{
protected:
	struct SurfaceMeshParams
	{
		float  aspect;		//Remote Camera Texture Aspect Ratio
		float fovScaler;	//Render Plane scaler value based on Remote Camera FoV and plane distance
		float distance;		//Render Plane distance from the camera
	};


	scene::MeshRenderableNode* m_surface[2];
	SurfaceMeshParams m_surfaceParams;
	math::vector3d m_cameraOffsets;

	video::OffAxisProjection m_offAxisProj[2];

	CarObjects* m_car;

	video::RenderPass* m_screenMtrl[2];
	video::RenderPass* m_wireframePass[2];


	TBee::ICameraVideoSource* m_videoSource;
	float m_displayFov;
	math::vector2d m_displaySize;
	float m_screenDistance;

	bool m_useLensCorrection;
	GCPtr<video::ParsedShaderPP> m_lensCorrectionPP;
	TBee::TelubeeCameraConfiguration *m_cameraConfiguration;
	bool m_camConfigDirty;

	void _UpdateCameraProj();
	void _UpdateHead(const math::vector3d& pos, const math::vector3d &angles);
	void _UpdateCameraPlane();
	float CalcDisplayFoV(float headDistance);
	void _UpdateCameraPlaneScaler();
	void _SetCameraPlaneDistance(float d);
public:
	CameraPlaneRenderer();
	virtual ~CameraPlaneRenderer();

	virtual bool OnEvent(Event* e);

	virtual void Init(CarObjects* car);

	virtual void Start();
	virtual void Stop();

	virtual void SetTransformation(const math::vector3d& pos, const math::vector3d &angles);

	void GenerateSurface(float aspectRatio, float cameraScreenDistance);

	virtual void Update(float dt);

	void PreRender(const math::rectf& rc, int index);
	void PostRender(int index);
	void DebugRender(GUI::IGUIRenderer* r);


	math::vector3d GetCameraOffset(){ return m_cameraOffsets; }
	void SetCameraOffset(const math::vector3d& v){ m_cameraOffsets=v; }

	TBee::ICameraVideoSource* GetVideoSource(){ return  m_videoSource; }

	float GetDisplayFov(){ return m_displayFov; }

	void LoadFromXML(xml::XMLElement* e);
};

}
}


#endif
