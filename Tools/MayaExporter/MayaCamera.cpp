
#include "stdafx.h"
#include "MayaCamera.h"


namespace mray
{
namespace exporter
{


MayaCamera::MayaCamera()
{
}

MayaCamera::~MayaCamera()
{
}


MStatus MayaCamera::read(const MFnCamera& camera,ExporterParams& params)
{

	MFnTransform* cameraTransform = NULL;
	// get camera transform
	for (int i=0; i<camera.parentCount(); i++)
	{
		if (camera.parent(i).hasFn(MFn::kTransform))
		{
			cameraTransform = new MFnTransform(camera.parent(i));
			continue;
		}
	}

	if(cameraTransform)
		MayaNode::read(*cameraTransform,params);
	m_isOrtho = camera.isOrtho();

	if (cameraTransform != NULL)
		delete cameraTransform;

	return MS::kSuccess;
}

xml::XMLElement* MayaCamera::saveXML(xml::XMLElement* parent,ExporterParams& params)
{
	xml::XMLElement* ret=MayaNode::saveXML(parent,params);
	ret->addAttribute("Type","Camera");
	ret->addAttribute("IsOrtho",core::StringConverter::toString(m_isOrtho));
	return ret;
}




}
}

