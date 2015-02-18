#include "stdafx.h"


#include "SceneNodeCreator.h"


#include "IFileSystem.h"
#include "IStream.h"
#include "XMLTree.h"
#include "SLODAnimatedMesh.h"
#include "MeshResourceManager.h"

namespace mray
{
namespace scene
{

SLODAnimatedMesh* SceneNodeCreator::addAnimatedMesh(const  core::string&meshFileName,bool instance)
{
	core::string fileName(meshFileName);
	if(fileName.find(mT(".mdx"))!=-1){
		OS::IStreamPtr stream= gFileSystem.openFile(fileName,OS::BIN_READ);
		if(stream){
			xml::XMLTree tree;
			if(tree.load(stream))
			{
				xml::XMLElement*e=tree.getSubElement(mT("ISceneNode"));
				if(e){
					scene::SLODAnimatedMesh* mesh;
					mesh=new scene::SLODAnimatedMesh();
					mesh->loadXMLSettings(e);
					mesh->SetModelFileName(fileName);
					stream->close();
					return mesh;
				}
			}
			stream->close();
		}
	}

	SMesh*m=gMeshResourceManager.loadMesh(meshFileName,instance);
	SLODAnimatedMesh* animMesh=new SLODAnimatedMesh();
	if(m){
		animMesh->addLOD(m,100);
	}

	return animMesh;
}



}
}