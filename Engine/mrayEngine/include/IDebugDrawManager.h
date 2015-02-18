

/********************************************************************
	created:	2011/02/03
	created:	3:2:2011   14:08
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IDebugDrawManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IDebugDrawManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IDebugDrawManager_h__
#define IDebugDrawManager_h__



#include "SColor.h"
#include "mstring.h"

namespace mray
{
namespace scene
{
	class IViewNode;

class IDebugDrawManager
{
private:
protected:
public:
	IDebugDrawManager(){}
	virtual ~IDebugDrawManager(){}
	
	virtual void AddLine(const math::vector3d& start,const math::vector3d& end,const video::SColor& clr,float lineWidth=1,float duration=0,bool depthTest=true )=0;
	virtual void AddArrow(const math::vector3d& start,const math::vector3d& end,const video::SColor& clr,float lineWidth=1,float duration=0,bool depthTest=true )=0;
	virtual void AddCross(const math::vector3d& pos,float size,const video::SColor& clr,float duration=0,bool depthTest=true )=0;
	virtual void AddSphere(const math::vector3d& pos,float radius,const video::SColor& clr,bool wireframe=true,float duration=0,bool depthTest=true )=0;
	virtual void AddCircle(const math::vector3d& pos,float radius,const math::vector3d& dir,const video::SColor& clr,float duration=0,bool depthTest=true )=0;
	virtual void AddAxis(const math::matrix4x4& transform,float size,float duration=0,bool depthTest=true )=0;
	virtual void AddMesh(const math::vector3d* verts,const ushort* indicies,int vCnt,int iCnt,const video::SColor& clr,bool wireframe=true,float duration=0,bool depthTest=true )=0;
	virtual void AddAABB(const math::box3d& aabb,const video::SColor& clr,float duration=0,bool depthTest=true )=0;
	virtual void AddOBB(const math::matrix4x4& transform,const math::vector3d& scale,const video::SColor& clr,float duration=0,bool depthTest=true )=0;

	virtual void AddString(const math::vector3d& pos,const core::string& txt,const video::SColor& clr,float duration=0,bool depthTest=true )=0;

	//start building the primitives
	virtual void StartDraw(IViewNode* cam)=0;
	//end building and drawing the primitives
	virtual void EndDraw()=0;

	virtual void Update(float dt)=0;

};

}
}


#endif // IDebugDrawManager_h__
