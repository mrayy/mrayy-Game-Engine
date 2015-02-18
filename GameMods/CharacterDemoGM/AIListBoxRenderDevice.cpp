
#include "stdafx.h"
#include "AIListBoxRenderDevice.h"



namespace mray
{
namespace GameMod
{

AIListBoxRenderDevice::AIListBoxRenderDevice(video::IVideoDevice*dev,GUI::IFont*font,GUI::GUIList* listBox)
:AI::AIRenderDevice(dev,font),m_listBox(listBox)
{
}
AIListBoxRenderDevice::~AIListBoxRenderDevice()
{
}

void AIListBoxRenderDevice::clearTextLines()
{
	if(m_listBox)
		m_listBox->clear();
}
void AIListBoxRenderDevice::addTextLine(const core::string&text)
{
	if(m_listBox)
		m_listBox->addItem(text.c_str());
}

}
}
