
#ifndef ___CameraNode___
#define ___CameraNode___





#include "ISceneNode.h"
#include "IViewNode.h"
#include "IRenderPassSubscriber.h"
#include "NormalRenderPass.h"

namespace mray{
namespace scene{

	class CameraNodeDesc;

	enum EProjectionType{
		EPT_Perspective,
		EPT_Ortho
	};

class MRAY_DLL CameraNode:public ISceneNode,public IViewNode,public IRenderPassSubscriber
{
	DECLARE_RTTI
protected:
	float m_fovY;
	float m_ZNear;
	float m_ZFar;
	bool m_useTarget;
	math::vector3d m_target;
	math::vector3d m_vUp;
	float m_aspect;
	bool m_autoUpdateAspect;

	math::vector3d m_transformedTarget;
	math::vector3d m_transformedUp;

	bool m_reflection;
	math::Plane m_reflectionPlane;
	math::matrix4x4 m_reflectionMatrix;

	EProjectionType m_projectionType;

	math::rect<int> m_rWindow;
	math::matrix4x4 m_projectionMatrix;
	math::matrix4x4 m_viewMatrix;

	math::matrix4x4 m_prevViewMatrix;

	math::Frustum m_viewArea;

	virtual void calcProjectionMatrix();
	virtual void calcViewArea();

	IMovable* m_lookAtTarget;

	std::list<ICullable*> m_visibleNodes;

	GCPtr<NormalRenderPass> m_normalRP;


	IViewNode* m_customViewNode;

	virtual void fillProperties();

public:
	DECLARE_PROPERTY_TYPE(FovY,float,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(ZNear,float,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(ZFar,float,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Aspect,float,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Target,math::vector3d,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(UpVector,math::vector3d,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(UseTarget,bool,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(AutoUpdateAspect,bool,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(ProjectionType,EProjectionType,MRAY_DLL);
	
	CameraNode(const  core::string&name,int id,ISceneManager* sceneMngr);
	virtual~CameraNode();

	virtual int GetNodeType();

	virtual void GetRenderPasses(std::list<IRenderPass*>& plst);


	void SetCustomViewNode(IViewNode* vn){m_customViewNode=vn;}
	IViewNode* GetCustomViewNode(){return m_customViewNode;}

	void SetWindow(int x,int y,int w,int h);
	void PositionCamera(math::vector3d pos,math::vector3d target,math::vector3d  up);

	EProjectionType getProjectionType()const{ return m_projectionType;}
	bool setProjectionType(EProjectionType type){ m_projectionType=type;return true;}

	math::vector3d ScreenToWorld(const math::vector2d& pos,float depth);
	math::vector3d WorldToScreen(const math::vector3d& pos);

	bool isReflect()const;
	void enableReflection(const math::Plane& p);
	void disableReflection();
	const math::Plane& getReflectionPlane()const;
	const math::matrix4x4& getReflectionMatrix()const;

	void updateView();

	virtual void preRender();
	virtual void render(IRenderPass*pass);
	virtual void update(float dt);


	void setLookAtTarget(IMovable*m);
	IMovable*getLookAtTarget();

	void setWindowDim(math::Point2di dim);
	void setWindowPos(math::Point2di pos);
	math::Point2di getWindowDim(){return m_rWindow.getSize();}
	math::Point2di getWindowPos(){return m_rWindow.ULPoint;}

	virtual math::vector3d getAbsolutePosition()const;
	virtual math::quaternion getAbsoluteOrintation()const;
	virtual const math::matrix4x4&getProjectionMatrix()const{return m_projectionMatrix;}
	virtual const math::matrix4x4&getViewMatrix()const{return m_viewMatrix;}
	virtual const math::matrix4x4&getPrevViewMatrix()const{return m_prevViewMatrix;}
	virtual math::vector3d getViewPos()const{return getAbsolutePosition();}
	virtual const math::Frustum*getViewFrustrum()const;

	virtual void setProjectionMatrix(const math::matrix4x4& m){m_projectionMatrix=m;}


	math::recti getViewPort();


	const std::list<ICullable*>& getVisibleNodes();
	void clearVisibleNodes();

	bool isCulling(ICullable* node)const;

	bool addVisibleNode(ICullable*node);

	float getFovY()const{return m_fovY;}
	float getAspect()const{return m_aspect;}
	float getZNear()const{return m_ZNear;}
	float getZFar()const{return m_ZFar;}

	bool setUpVector(const math::vector3d& val);
	const math::vector3d &getUpVector()const;
	const math::vector3d &getUpVector();

	const math::vector3d& getTarget();
	const math::vector3d& getTarget()const;
	bool  setTarget(const math::vector3d& t);

	bool setFovY(float fov){
		m_fovY=fov;
		calcProjectionMatrix();
		return true;
	}
	bool setAspect(float asp){
		m_aspect=asp;
		calcProjectionMatrix();
		return true;
	}
	bool setZNear(float znear){
		m_ZNear=znear;
		calcProjectionMatrix();
		return true;
	}
	bool setZFar(float zfar){
		m_ZFar=zfar;
		calcProjectionMatrix();
		return true;
	}

	bool setUseTarget(bool use){m_useTarget=use;return true;}
	bool getIsTargettingEnabled()const{return m_useTarget;}

	bool setAutoUpdateAspect(bool v){m_autoUpdateAspect=v;return true;}
	bool getAutoUpdateAspect()const{return m_autoUpdateAspect;}


	void onAspectChanged(float w,float h);

	virtual void OnVisit(ISceneVisitor*visitor);

	void SaveToDesc(CameraNodeDesc* desc)const;
	void LoadFromDesc(const CameraNodeDesc* desc);
};

MakeSharedPtrType(CameraNode)

}
}


#endif