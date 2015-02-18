

#include "stdafx.h"
#include "BHCanvasManager.h"
#include "DirectedConnector.h"
#include "ImageSetResourceManager.h"

namespace mray
{
namespace canvas
{

BHCanvasManager::BHCanvasManager(video::IVideoDevice* dev)
:CanvasManager(dev)
{
	m_symbolsImageSet=gImageSetResourceManager.loadImageSet(mT("BHTreeSymbols.imageset"));
}
BHCanvasManager::~BHCanvasManager()
{
}

IConnectionCurve* BHCanvasManager::CreateConnection(IConnector* source,IConnector* destination)
{
	DirectedConnector* curve=new DirectedConnector(this);
	AddCurve(curve);
	curve->SetSource(source);
	curve->SetDestination(destination);

	return curve;
}


}
}


