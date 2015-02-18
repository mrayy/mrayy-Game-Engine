

/********************************************************************
	created:	2011/12/04
	created:	4:12:2011   12:30
	filename: 	d:\Development\mrayEngine\Tools\BHUIBuilderLib\BHCanvasManager.h
	file path:	d:\Development\mrayEngine\Tools\BHUIBuilderLib
	file base:	BHCanvasManager
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __BHCanvasManager__
#define __BHCanvasManager__

#include "CanvasManager.h"
#include "ImageSet.h"

namespace mray
{
namespace canvas
{

class BHCanvasManager:public CanvasManager
{
private:
protected:
	video::ImageSet* m_symbolsImageSet;
public:
	BHCanvasManager(video::IVideoDevice* dev);
	virtual~BHCanvasManager();

	video::ImageSet* GetSymbols(){return m_symbolsImageSet;}

	virtual IConnectionCurve* CreateConnection(IConnector* source,IConnector* destination);
};


}
}

#endif
