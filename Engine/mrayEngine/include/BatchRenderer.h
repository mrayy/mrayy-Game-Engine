/********************************************************************
	created:	2014/04/10
	created:	10:4:2014   1:37
	filename: 	C:\Development\mrayEngine\Engine\mrayEngine\include\BatchRenderer.h
	file path:	C:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	BatchRenderer
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __BatchRenderer__
#define __BatchRenderer__




namespace mray
{
namespace scene
{
	class MeshBufferData;

class BatchRenderer
{
protected:
public:
	BatchRenderer();
	virtual~BatchRenderer();

	void AddMesh(scene::MeshBufferData* buffer,const math::matrix4x4* trans);
};

}
}


#endif