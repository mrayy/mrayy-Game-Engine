

/********************************************************************
	created:	2012/08/08
	created:	8:8:2012   17:55
	filename: 	d:\Development\mrayEngine\Tools\MayaExporter\MRAYMayaNode.h
	file path:	d:\Development\mrayEngine\Tools\MayaExporter
	file base:	MRAYMayaNode
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___MRAYMayaNode___
#define ___MRAYMayaNode___


#include <maya/MPxNode.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include "api_macros.h"

namespace mray
{
namespace exporter
{

class MRAYMayaNode:public MPxNode
{
protected:
public:
	MRAYMayaNode()
	{}
	virtual~MRAYMayaNode(){}
	

	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );
	static  void*		creator();
	static  MStatus		initialize();

	static  MObject		type;
	static  MTypeId		id;
};

MTypeId	MRAYMayaNode::id( 0x81000 );
MObject	MRAYMayaNode::type;        

void* MRAYMayaNode::creator()
{
	return new MRAYMayaNode;
}

MStatus MRAYMayaNode::initialize()
{
	MFnTypedAttribute tattr;
	MFnEnumAttribute attr;
	//type=attr.create("Type","tp");
	attr.addField("Type1",0);
	attr.addField("Type2",1);
	attr.addField("Type3",2);
	attr.addField("Type4",3);

	type=tattr.create("Type","tp",MFnData::Type::kString);

	//MFnNumericAttribute nAttr;
	MStatus				stat;

	//type = nAttr.create( "type", "tp", MFnNumericData::kDouble, 0.0 );
	attr.setStorable(true);
	attr.setHidden(false);
	ADD_ATTRIBUTE(type);
	return MS::kSuccess;
}
MStatus MRAYMayaNode::compute( const MPlug& plug, MDataBlock& data)
{
	return MS::kSuccess;
}

}
}

#endif
