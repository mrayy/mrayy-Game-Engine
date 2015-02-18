

/********************************************************************
	created:	2010/04/07
	created:	7:4:2010   17:16
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestAgent\AIRenderDevice.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestAgent
	file base:	AIRenderDevice
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef AIRenderDevice_h__
#define AIRenderDevice_h__

#include "IRenderDevice.h"

namespace mray
{
namespace AI
{

class AIRenderDevice:public IRenderDevice
{
private:
protected:
	video::IVideoDevice* m_device;
	GUI::IFont* m_font;
	GCPtr<scene::IMeshBuffer> m_mesh;

	core::array<core::string> m_txtLines;

public:
	AIRenderDevice(video::IVideoDevice*dev,GUI::IFont*font);
	virtual ~AIRenderDevice();

	const core::array<core::string> &getTextLines();

	virtual void drawLine(const math::line3d&l,const video::SColor&c);

	virtual void drawText(const math::vector2d&p,const core::string&text,const video::SColor&c);

	virtual void clearTextLines();

	virtual void addTextLine(const core::string&text);

	virtual void renderLines(const math::vector3d*points,int ptCnt,const ushort*idx,int idxCnt,const video::SColor&c);

	virtual void renderTriangles(const math::vector3d*points,int ptCnt,const ushort*idx,int idxCnt,const video::SColor&c);
};

}
}

#endif // AIRenderDevice_h__
