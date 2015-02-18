

/********************************************************************
	created:	2010/06/13
	created:	13:6:2010   15:16
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IPickBuffer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IPickBuffer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IPickBuffer_h__
#define IPickBuffer_h__

#include "mtypes.h"



namespace mray
{
	namespace scene
	{
		class IRenderable;
		class MeshBufferData;
	}
namespace video
{

class IPickBuffer
{
private:
protected:
public:
	IPickBuffer(){}
	virtual ~IPickBuffer(){}

	virtual void Clear()=0;

	virtual void SetViewMatrix(const math::matrix4x4& m)=0;
	virtual void SetProjectionMatrix(const math::matrix4x4& m)=0;

	virtual void PushPickID(uint id)=0;

	virtual void AddRenderObject(scene::MeshBufferData*bd,scene::IRenderable*obj)=0;

	virtual void PopPickID()=0;

	virtual void Render()=0;

	virtual uint GetID(int x,int y)=0;
};

}
}

#endif // IPickBuffer_h__
