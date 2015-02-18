/********************************************************************
	created:	2009/06/08
	created:	8:6:2009   17:55
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IViewNode.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IViewNode
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IViewNode___
#define ___IViewNode___



namespace mray{
	namespace math
	{
		class Frustum;
	}
namespace scene
{

	class ICullable;

class IViewNode
{
private:
protected:
public:

	virtual const math::matrix4x4& getProjectionMatrix()const=0;
	virtual const math::matrix4x4& getViewMatrix()const=0;
	virtual const math::matrix4x4& getPrevViewMatrix()const=0;
	virtual const math::Frustum* getViewFrustrum()const=0;
	virtual math::vector3d  getViewPos()const=0;
	virtual bool isCulling(ICullable* node)const=0; 

};

}
}


#endif //___IViewNode___

