


/********************************************************************
created:	2012/02/14
created:	14:2:2012   9:01
filename: 	d:\Development\mrayEngine\Tests\VegetationTest\XMLSceneLoader.h
file path:	d:\Development\mrayEngine\Tests\VegetationTest
file base:	XMLSceneLoader
file ext:	h
author:		MHD Yamen Saraiji

purpose:	
*********************************************************************/


#ifndef __XMLSceneLoader__
#define __XMLSceneLoader__


namespace mray
{
	class XMLSceneLoader
	{
	private:
	protected:
		scene::ISceneManager* m_manager;
		physics::IPhysicManager* m_phManager;
		scene::ISceneNode* loadNode(xml::XMLElement* elem,scene::ISceneNode*parent,std::vector<scene::ISceneNode*>& snodes,std::vector<physics::IPhysicalNode*>& phNodes);
		physics::IPhysicalNode* loadPhNode(xml::XMLElement* elem,scene::ISceneNode* parent);
	public:
		XMLSceneLoader(scene::ISceneManager* manager,physics::IPhysicManager* phManager);
		virtual~XMLSceneLoader(){}

		virtual bool Load(OS::IStream* stream,scene::ISceneNode*parent,
			std::vector<scene::ISceneNode*>& snodes,std::vector<physics::IPhysicalNode*>& phNodes);

	};


}

#endif