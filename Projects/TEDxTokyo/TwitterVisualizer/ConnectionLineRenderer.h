

/********************************************************************
	created:	2014/04/04
	created:	4:4:2014   1:13
	filename: 	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer\ConnectionLineRenderer.h
	file path:	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer
	file base:	ConnectionLineRenderer
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __ConnectionLineRenderer__
#define __ConnectionLineRenderer__


#include "MeshRenderableNode.h"

namespace mray
{
namespace scene
{

class ConnectionLineRenderer :public MeshRenderableNode
{
protected:
	scene::MeshBufferData* m_buffer;
	struct ConnectionIfo
	{
		math::vector3d start;
		math::vector3d end;
		video::SColor clrs[2];
	};

	bool m_dirty;
	std::list<ConnectionIfo> m_connections;
public:
	ConnectionLineRenderer();
	virtual~ConnectionLineRenderer();

	void Reset();
	void AddConnection(const math::vector3d& a, const math::vector3d& b, const video::SColor& aclr, const video::SColor& bclr);

	virtual void update(float dt);
};

}
}


#endif
