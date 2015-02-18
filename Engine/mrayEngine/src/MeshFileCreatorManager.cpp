#include "stdafx.h"

#include "MeshFileCreatorManager.h"
#include "IMeshFileCreator.h"
#include "StringUtil.h"
#include "MeshResourceManager.h"
#include "MeshRenderableNode.h"
#include "SLODAnimatedMesh.h"
#include "XMLTree.h"

#include "IFileSystem.h"

namespace mray
{
namespace scene
{

	class MDLMeshFileCreator:public IMeshFileCreator
	{
	public:
		MDLMeshFileCreator():IMeshFileCreator(mT("mdl"))
		{
		}
		virtual MeshRenderableNode* LoadFromFile(const core::string&str,bool instanced)
		{
			GCPtr<scene::SMesh> mesh= gMeshResourceManager.loadMesh(str,instanced);
			if(!mesh.pointer())return 0;
			MeshRenderableNode* node=new MeshRenderableNode(mesh);
			return node;
		}
	};

	class MDXMeshFileCreator:public IMeshFileCreator
	{
	public:
		MDXMeshFileCreator():IMeshFileCreator(mT("mdx"))
		{
		}
		virtual MeshRenderableNode* LoadFromFile(const core::string&str,bool instanced)
		{
			xml::XMLTree tree;
			GCPtr<OS::IStream> stream=gFileSystem.openFile(str);
			if(!stream)return 0;
			if(!tree.load(stream))return 0;
			SLODAnimatedMesh* node=new SLODAnimatedMesh();
			xml::XMLElement* e=tree.getSubElement(mT("SLODAnimatedMesh"));
			if(e)
				node->loadXMLSettings(e);
			return node;
		}
	};


MeshFileCreatorManager::MeshFileCreatorManager()
{
	RegisterCreator(new MDLMeshFileCreator());
	RegisterCreator(new MDXMeshFileCreator());
}
MeshFileCreatorManager::~MeshFileCreatorManager()
{
	CreatorMap::iterator it= m_creators.begin();
	for(;it!=m_creators.end();++it)
		delete it->second;
	m_creators.clear();
}

void MeshFileCreatorManager::RegisterCreator(IMeshFileCreator*c)
{
	core::string t=core::StringUtil::ToLower(c->GetType());
	m_creators[t]=c;
}
void MeshFileCreatorManager::RemoveCreator(const core::string& type)
{
	core::string t=core::StringUtil::ToLower(type);
	CreatorMap::iterator it= m_creators.find(t);
	if(it==m_creators.end())return;
	delete it->second;
	m_creators.erase(it);
}

MeshRenderableNode* MeshFileCreatorManager::LoadFromFile(const core::string&str,bool instanced)
{
	core::string ext,path;
	core::StringUtil::SplitPathExt(str,path,ext);
	ext=core::StringUtil::ToLower(ext);
	CreatorMap::iterator it= m_creators.find(ext);
	if(it==m_creators.end())
		return 0;
	return it->second->LoadFromFile(str,instanced);
}



}
}

