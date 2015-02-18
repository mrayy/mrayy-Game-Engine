// MayaExporter.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "MayaExporter.h"
#include "MayaMesh.h"
#include "MayaMaterialSet.h"
#include "MayaCamera.h"

#include "WinFileSystem.h"
#include "Engine.h"
#include "WinOSystem.h"
#include "XMLWriter.h"
#include "IFileSystem.h"
#include "StreamWriter.h"
#include "SoftwareBufferManager.h"

//#include "MRAYMayaNode.h"

namespace mray
{
namespace exporter
{


const MString MayaExporter::commandName("mrayExporter");


MayaExporter::MayaExporter()
{
	MGlobal::displayInfo("Original Author of the exporter: Francesco Giordana");
	MGlobal::displayInfo("Conveted to MRAY Engine by: MHD Yamen Saraiji");

	new OS::WinFileSystem();
	new Engine(new OS::WinOSystem());
	new video::SoftwareBufferManager();
}
MayaExporter::~MayaExporter()
{
	this->exit();
}

// Routine for creating the plug-in
void* MayaExporter::creator()
{
	return new MayaExporter();
}

// It tells that this command is not undoable
bool MayaExporter::isUndoable() const
{
	MGlobal::displayInfo("Command is not undoable");
	return false;
}
void MayaExporter::exit()
{
	MGlobal::setActiveSelectionList(m_selList);

	for(int i=0;i<m_nodes.size();++i)
		delete m_nodes[i];
	m_nodes.clear();

	/*
	for(int i=0;i<m_cameras.size();++i)
		delete m_cameras[i];
		*/
	m_cameras.clear();
	m_meshs.clear();
	delete MayaMaterialSet::GetInstancePtr();
	//delete OS::IFileSystem::getInstancePtr();
	delete Engine::getInstancePtr();
	delete video::SoftwareBufferManager::getInstancePtr();
}

MStatus MayaExporter::doIt(const MArgList& args)
{
	MGlobal::getActiveSelectionList(m_selList);
	m_params.parseArgs(args);
	
	ParseScene();
	ExportScene();

	return MS::kSuccess;
}
void MayaExporter::ParseScene()
{
	MStatus stat;

	if(m_params.exportAll)
	{
		//export the scene
		MItDag dagIt;
		MFnDagNode worldDag(dagIt.root());
		MDagPath worldPath;
		worldDag.getPath(worldPath);
		ReadNode(worldPath,0);
	}else
	{
		MItSelectionList iter(m_selList);
		for ( ; !iter.isDone(); iter.next())
		{								
			MDagPath dagPath;
			stat = iter.getDagPath(dagPath);
			stat = ReadNode(dagPath,0); 
		}				
	}
}

xml::XMLElement* MayaExporter::GenerateXMLNodes()
{
	xml::XMLElement* root=new xml::XMLElement("Nodes");
	for(int i=0;i<m_nodes.size();++i)
	{
		m_nodes[i]->saveXML(root,m_params);
	}
	/*
	for(int i=0;i<m_meshs.size();++i)
	{
		m_meshs[i]->saveXML(root,m_params);
	}*/
	/*
	OS::IStreamPtr sceneXML=gFileSystem.openFile(core::string(m_params.basePath.asChar())+"scene.xml",OS::BIN_WRITE);

	xml::XMLWriter writer;
	writer.addElement(root);
	core::string xmlStr=writer.flush();

	OS::StreamWriter sWriter(sceneXML);
	sWriter.writeString(xmlStr);
	sceneXML->close();
	*/
	return root;
}
xml::XMLElement* MayaExporter::GenerateXMLComponents()
{
	xml::XMLElement* root=new xml::XMLElement("Components");
	for(int i=0;i<m_nodes.size();++i)
	{
		m_nodes[i]->saveXMLAsComponent(root,m_params);
	}
	/*
	for(int i=0;i<m_meshs.size();++i)
	{
		m_meshs[i]->saveXML(root,m_params);
	}*/
	/*
	OS::IStreamPtr sceneXML=gFileSystem.openFile(core::string(m_params.basePath.asChar())+"scene.xml",OS::BIN_WRITE);

	xml::XMLWriter writer;
	writer.addElement(root);
	core::string xmlStr=writer.flush();

	OS::StreamWriter sWriter(sceneXML);
	sWriter.writeString(xmlStr);
	sceneXML->close();
	*/
	return root;
}
void MayaExporter::ExportScene()
{
	if(m_params.exportTextures)
		MayaMaterialSet::GetInstance().ExportTextures(m_params);
//	xml::XMLElement* materials= MayaMaterialSet::GetInstance().GenerateXML(m_params);
//	xml::XMLElement* root=new xml::XMLElement("Scene");
	//root->addSubElement(materials);
//	root->addSubElement(nodes);

	{
		xml::XMLElement* root=GenerateXMLNodes();

		OS::IStreamPtr sceneXML=gFileSystem.openFile(core::string(m_params.basePath.asChar())+"scene.xml",OS::BIN_WRITE);

		xml::XMLWriter writer;
		writer.addElement(root);
		core::string xmlStr=writer.flush();

		OS::StreamWriter sWriter(sceneXML);
		sWriter.writeString(xmlStr);
		sceneXML->close();
		delete root;
	}
	{
		xml::XMLElement* root=GenerateXMLComponents();

		OS::IStreamPtr sceneXML=gFileSystem.openFile(core::string(m_params.basePath.asChar())+"components.xml",OS::BIN_WRITE);

		xml::XMLWriter writer;
		writer.addElement(root);
		core::string xmlStr=writer.flush();

		OS::StreamWriter sWriter(sceneXML);
		sWriter.writeString(xmlStr);
		sceneXML->close();
		delete root;
	}
}


//////////////////////////////////////////////////////////////////////////
MStatus MayaExporter::ReadNode(MDagPath& dPath,MayaNode* parent)
{
	MStatus stat;
	MString message;
	MayaNode* node=parent;

	message = "Analysing Node: " + dPath.fullPathName();
	MGlobal::displayInfo(message);
	if((m_params.exportAll || m_params.exportMesh || m_params.exportMaterial || m_params.exportSkeletons) 
		&& dPath.hasFn(MFn::kMesh) && dPath.childCount()==0)
	{
		MFnMesh meshFn(dPath);
		if(!meshFn.isIntermediateObject())
		{
			MayaMesh* mesh=new MayaMesh();
			stat=mesh->read(dPath,m_params);
			if(stat==MS::kSuccess)
			{
				m_meshs.push_back(mesh);
				node=mesh;
				if(parent)
					parent->AddChild(mesh);
				else m_nodes.push_back(mesh);
			}
			else
			{
				delete mesh;
			}
		}
	}

	if((m_params.exportAll || m_params.exportCameras) 
		&& dPath.hasFn(MFn::kCamera) && !dPath.hasFn(MFn::kShape))
	{

		MFnCamera cameraFn(dPath);
		if(!cameraFn.isIntermediateObject())
		{
			MayaCamera* cam=new MayaCamera();
			stat=cam->read(cameraFn,m_params);
			if(stat==MS::kSuccess)
			{
				m_cameras.push_back(cam);
				node=cam;

				if(parent)
					parent->AddChild(cam);
				else m_nodes.push_back(cam);
			}
			else
			{
				delete cam;
			}
		}
	}


	if(node==parent && dPath.hasFn(MFn::kTransform) && !dPath.hasFn(MFn::kMesh))
	{
		MayaNode* n=new MayaNode();
		bool d=dPath.hasFn(MFn::kMesh);
		MFnTransform trans(dPath);
		if(!trans.isIntermediateObject())
		{
			stat=n->read(trans,m_params);
			if(stat==MS::kSuccess)
			{
				if(!parent)
					m_nodes.push_back(n);

				if(parent)
					parent->AddChild(n);
				node=n;
			}else
				delete n;
		}
	}
	std::cout.flush();
	for(uint i=0;i<dPath.childCount();++i)
	{
		MObject child=dPath.child(i);
		MDagPath cp=dPath;
		stat=cp.push(child);
		if(stat!=MS::kSuccess)
		{
			message= MString("Error Retriving Path for child: ") + i +" of node: "+dPath.fullPathName();

			MGlobal::displayInfo(message);
			return MS::kFailure;
		}
		stat=ReadNode(cp,node);
		if(stat!=MS::kSuccess)
			return MS::kFailure;
	}
	return MS::kSuccess;
}


}
}



// Routine for registering the command within Maya
MStatus initializePlugin( MObject obj )
{
	MStatus   status;
	MFnPlugin plugin( obj, "MRAYExporter", "1.0", "Any");
	status = plugin.registerCommand( mray::exporter::MayaExporter::commandName, mray::exporter::MayaExporter::creator );
	if (!status) {
		status.perror("registerCommand");
		return status;
	}

//	plugin.registerNode("MRAYNode",mray::exporter::MRAYMayaNode::id,mray::exporter::MRAYMayaNode::creator,mray::exporter::MRAYMayaNode::initialize);

	return status;
}

// Routine for unregistering the command within Maya
MStatus uninitializePlugin( MObject obj)
{
	MStatus   status;
	MFnPlugin plugin( obj );
	status = plugin.deregisterCommand(mray::exporter::MayaExporter::commandName );
	if (!status) {
		status.perror("deregisterCommand");
		return status;
	}
//	plugin.deregisterNode(mray::exporter::MRAYMayaNode::id);

	return status;
}

