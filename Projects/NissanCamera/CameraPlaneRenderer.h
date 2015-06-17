

#ifndef __CAMERAPLANERENDERER__
#define __CAMERAPLANERENDERER__

#include "ParsedShaderPP.h"
#include "TBeeCommon.h"

namespace mray
{
	namespace TBee
	{
		class ICameraVideoSource;
		class TelubeeCameraConfiguration;
	}
namespace NCam
{
	
class CameraPlaneRenderer
{
protected:
	struct SurfaceMeshParams
	{
		bool plane;
		float hfov, aspect;
		int segments;
		float radius;
		math::vector3d scale[2];
	};

	struct DistortionParams
	{
		DistortionParams() :distortionVal(0.5), cylindricalRatio(0.1), yOffset(0)
		{}

		float distortionVal;
		float cylindricalRatio;
		float yOffset;
	};

	DistortionParams m_distortionParams;

	scene::MeshRenderableNode* m_surface[2];
	SurfaceMeshParams m_surfaceParams;
	math::vector3d m_cameraOffsets;


	scene::ISceneNode* m_headNode;
	scene::CameraNode* m_camera[2];
	scene::ISceneNode* m_screenNode[2];
	video::RenderPass* m_screenMtrl[2];
	video::RenderPass* m_wireframePass[2];


	TBee::ICameraVideoSource* m_videoSource;
	float m_displayFov;
	float m_displayWidth;
	float m_screenDistance;
	bool m_sphericalPlane;

	bool m_useLensCorrection;
	GCPtr<video::ParsedShaderPP> m_lensCorrectionPP;
	TBee::TelubeeCameraConfiguration *m_cameraConfiguration;
	bool m_camConfigDirty;

	float CalcDisplayFoV(float headDistance);
public:
	CameraPlaneRenderer();
	virtual ~CameraPlaneRenderer();

	virtual bool OnEvent(Event* e);

	virtual void Init(scene::ISceneNode* headNode, scene::CameraNode* leftCam, scene::CameraNode* rightCam);

	virtual void Start();
	virtual void Stop();

	virtual void SetTransformation(const math::vector3d& pos, const math::vector3d &angles);
	virtual void RescaleMesh(int index, const math::vector3d &scaleFactor);

	void GenerateSurface(bool plane, float hfov,float aspectRatio, int segments, float cameraScreenDistance);

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
