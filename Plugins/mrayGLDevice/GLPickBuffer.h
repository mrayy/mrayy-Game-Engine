

/********************************************************************
	created:	2010/06/13
	created:	13:6:2010   14:06
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGLDevice\GLPickBuffer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGLDevice
	file base:	GLPickBuffer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GLPickBuffer_h__
#define GLPickBuffer_h__

#include "CompileConfig.h"
#include "IPickBuffer.h"
#include <stack>
#include <vector>

namespace mray
{
namespace video
{

	class mrayDev;

class MRAY_GL_DLL GLPickBuffer:public IPickBuffer
{
private:
protected:
#define  SELECT_BUFFER_SIZE 128

	struct IDRenderObject
	{
		IDRenderObject(uint id)
		{
			this->id=id;
		}
		uint id;

		struct ROperation
		{
			ROperation():node(0),bd(0)
			{}
			ROperation(scene::IRenderable*n,scene::MeshBufferData*b):node(n),bd(b)
			{}
			scene::IRenderable*node;
			scene::MeshBufferData*bd;
		};
		std::vector<ROperation> objs;
	};
	typedef std::stack<IDRenderObject> IDObjectsMap;

	IDObjectsMap m_objects;

	uint m_selectBuffer[SELECT_BUFFER_SIZE];

	math::matrix4x4 m_viewMatrix;
	math::matrix4x4 m_projMatrix;

	uint m_hitsCount;

	mrayDev* m_device;

	uint ProcessHits();

public:
	GLPickBuffer(mrayDev*device);
	virtual ~GLPickBuffer();
	
	void Clear();

	void SetViewMatrix(const math::matrix4x4& m);
	void SetProjectionMatrix(const math::matrix4x4& m);

	void PushPickID(uint id);

	void AddRenderObject(scene::MeshBufferData*bd,scene::IRenderable*obj);

	void PopPickID();

	void Render();

	uint GetID(int x,int y);

};

}
}


#endif // GLPickBuffer_h__
