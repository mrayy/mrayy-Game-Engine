


/********************************************************************
	created:	2012/02/09
	created:	9:2:2012   17:59
	filename: 	d:\Development\mrayEngine\Tools\MayaExporter\MayaCamera.h
	file path:	d:\Development\mrayEngine\Tools\MayaExporter
	file base:	MayaCamera
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __MayaCamera__
#define __MayaCamera__

#include "MayaNode.h"

namespace mray
{
namespace exporter
{
	class ExporterParams;

class MayaCamera:public MayaNode
{
private:
protected:
	bool m_isOrtho;

public:
	MayaCamera();
	virtual~MayaCamera();

	MStatus read(const MFnCamera& camera,ExporterParams& params);

	virtual xml::XMLElement* saveXML(xml::XMLElement* parent,ExporterParams& params);
};


}
}

#endif
