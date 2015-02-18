
/********************************************************************
	created:	2008/12/23
	created:	23:12:2008   20:30
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\RenderOperation.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	RenderOperation
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___RenderOperation___
#define ___RenderOperation___


namespace mray{
	namespace video
	{
		class RenderPass;
	}
namespace scene{

class MeshBufferData;
class IRenderable;

class RenderOperation
{
public:
	RenderOperation(){
		mesh=0;
		node=0;
		pass=0;
		customRender=false;
	}
	MeshBufferData *mesh;
	video::RenderPass*pass;

	bool customRender;

	IRenderable* node;
};

}
}


#endif //___RenderOperation___


