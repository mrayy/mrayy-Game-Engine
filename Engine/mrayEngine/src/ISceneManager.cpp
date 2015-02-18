#include "stdafx.h"

#include "ISceneManager.h"

#include "ISceneVisitor.h"

namespace mray{
namespace scene{

	ISceneManager::ISceneManager(video::IVideoDevice*dev):m_device(dev)
	{
	}
	ISceneManager::~ISceneManager(){}

	video::IVideoDevice* ISceneManager::getDevice(){return m_device;}
	void ISceneManager::setDevice(video::IVideoDevice*dev){m_device=dev;}


	void ISceneManager::OnVisit(ISceneVisitor*visitor)
	{
		visitor->Visit(this);
	}

}
}


