


/********************************************************************
	created:	2010/07/08
	created:	8:7:2010   18:22
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\AIListBoxRenderDevice.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	AIListBoxRenderDevice
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef AIListBoxRenderDevice_h__
#define AIListBoxRenderDevice_h__

#include "AIRenderDevice.h"
#include "GUIList.h"

namespace mray
{
namespace GameMod
{

class AIListBoxRenderDevice:public AI::AIRenderDevice
{
private:
protected:
	GUI::GUIList* m_listBox;
public:
	AIListBoxRenderDevice(video::IVideoDevice*dev,GUI::IFont*font,GUI::GUIList* listBox);
	virtual ~AIListBoxRenderDevice();

	virtual void clearTextLines();
	virtual void addTextLine(const core::string&text);
};

}
}


#endif // AIListBoxRenderDevice_h__
