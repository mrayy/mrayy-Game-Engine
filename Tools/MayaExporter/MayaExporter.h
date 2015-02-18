

/********************************************************************
	created:	2012/02/09
	created:	9:2:2012   10:29
	filename: 	d:\Development\mrayEngine\Tools\MayaExporter\MayaExporter.h
	file path:	d:\Development\mrayEngine\Tools\MayaExporter
	file base:	MayaExporter
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __MayaExporter__
#define __MayaExporter__

#include <maya/MPxCommand.h>
#include <maya/MFnPlugin.h>

#include "ExporterParams.h"

namespace mray
{
namespace exporter
{

	class MayaMesh;
	class MayaNode;
	class MayaCamera;

class MayaExporter: public MPxCommand
{
protected:
	MSelectionList m_selList;
	ExporterParams m_params;

	std::vector<MayaNode*> m_nodes;

	std::vector<MayaMesh*> m_meshs;

	std::vector<MayaCamera*> m_cameras;

	MStatus ReadNode(MDagPath& dPath,MayaNode* parent);

	xml::XMLElement* GenerateXMLNodes();
	xml::XMLElement* GenerateXMLComponents();

	void ParseScene();
	void ExportScene();
public:
	static const MString commandName;
	MayaExporter();
	virtual~MayaExporter();

	static void*creator();
	MStatus doIt(const MArgList& args);
	bool isUndoable() const;
	
	void exit();

};

}
}

#endif

