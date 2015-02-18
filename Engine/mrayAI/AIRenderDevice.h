

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

#include "CompileConfig.h"
#include "IRenderDevice.h"
#include "marray.h"

namespace mray
{
namespace video
{
	class IVideoDevice;
}
namespace GUI
{
	class IFont;
	class IGUIRenderer;
}
namespace scene
{
	class IDebugDrawManager;
}
namespace AI
{

class MRAY_AI_DLL AIRenderDevice:public IRenderDevice
{
private:
protected:

	std::vector<core::string> m_txtLines;
	scene::IDebugDrawManager *m_debugManager;
public:
	AIRenderDevice(scene::IDebugDrawManager *debugManager);
	virtual ~AIRenderDevice();

	scene::IDebugDrawManager* GetDebugManager(){return m_debugManager;}

	const std::vector<core::string> &getTextLines();

	virtual void drawLine(const math::vector3d&s,const math::vector3d&e,const video::SColor&c);

	virtual void drawText(const math::vector3d&p,const core::string&text,const video::SColor&c);

	virtual void clearTextLines();

	virtual void addTextLine(const core::string&text);

	virtual void renderTriangles(const math::vector3d*points,int ptCnt,const ushort*idx,int idxCnt,const video::SColor&c);
};

}
}

#endif // AIRenderDevice_h__
