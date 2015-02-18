
#include "stdafx.h"
#include "RenderTechnique.h"

#include "DeviceCapabilites.h"
#include "Engine.h"

#include "RenderPass.h"
#include "StringUtil.h"


namespace mray
{
namespace video
{
	IMPLEMENT_PROPERTY_TYPE_HEADER(Name,RenderTechnique,string,mT("Name"),EPBT_Basic,mT("Technique Name"),mT(""));
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Name,RenderTechnique,core::string,SetName,GetName,,,false)

	IMPLEMENT_PROPERTY_TYPE_HEADER(RequiredProfiles,RenderTechnique,string,mT("RequiredProfiles"),EPBT_Basic,mT("Required Profiles"),mT(""));
	IMPLEMENT_PROPERTY_TYPE_GENERIC(RequiredProfiles,RenderTechnique,core::string,SetRequiredProfiles,GetRequiredProfiles,,,false)


	RenderTechnique::RenderTechnique(RenderMaterial*o)
	{
		m_isSupported=true;
		m_owner=o;

		CPropertieDictionary *dic=0;
		if(CreateDictionary(&dic))
		{
			dic->addPropertie(&PropertyTypeName::instance);
			dic->addPropertie(&PropertyTypeRequiredProfiles::instance);
		}
	}

	RenderTechnique::~RenderTechnique()
	{
		ClearPasses();
	}


	bool RenderTechnique::SetRequiredProfiles(const core::string& profiles)
	{
		m_profiles=profiles;
		std::vector<core::string> arr=core::StringUtil::Split(m_profiles,mT(" ,"));
		video::DeviceCapabilites*caps=Engine::getInstance().getDevice()->getCapabilities();
		m_isSupported=true;
		for(int i=0;i<arr.size();++i)
		{
			if(arr[i]!=mT("") && !caps->isShaderProfileSupported(arr[i]))
			{
				m_isSupported=false;
				break;
			}
		}
		return true;
	}
	const core::string& RenderTechnique::GetRequiredProfiles()
	{
		return m_profiles;
	}
	bool RenderTechnique::SetName(const core::string& name)
	{
		m_name=name;
		return true;
	}

	const core::string& RenderTechnique::GetName()const
	{
		return m_name;
	}


	RenderPass* RenderTechnique::CreatePass(const core::string&name)
	{
		RenderPass* p=new RenderPass(this);
		p->SetName(name);
		AddPass(p);
		return p;
	}
	const RenderPassList& RenderTechnique::GetPasses()const
	{
		return m_passes;
	}

	RenderPass* RenderTechnique::GetPassAt(int i)
	{
		if(i>=m_passes.size())
			return 0;
		return m_passes[i];
	}

	RenderPass* RenderTechnique::GetPassByName(const core::string& name)
	{
		for(int i=0;i<m_passes.size();++i)
		{
			if(name.equals_ignore_case(m_passes[i]->GetName()))
				return m_passes[i];
		}
		return 0;
	}

	void RenderTechnique::AddPass(RenderPass*p)
	{
		p->SetOwnerTechnique(this);
		m_passes.push_back(p);
	}

	void RenderTechnique::RemovePass(RenderPass*p)
	{
		for(RenderPassList::iterator it=m_passes.begin();it!=m_passes.end();++it)
		{
			if(*it==p)
			{
				m_passes.erase(it);
				break;
			}
		}
	}

	void RenderTechnique::RemovePassAt(int index)
	{
		if(index>=m_passes.size())
			return;
		RenderPassList::iterator it=m_passes.begin();
		it+=index;
		delete *it;
		m_passes.erase(it);
	}

	void RenderTechnique::ClearPasses()
	{

		for(int i=0;i<m_passes.size();++i)
		{
			delete m_passes[i];
		}
		m_passes.clear();
	}

	RenderTechnique* RenderTechnique::Duplicate(){
		RenderTechnique* m=new RenderTechnique(m_owner);
		m->SetName(m_name);
		m->SetRequiredProfiles(m_profiles);
		m->SetDepthShader(m_depthShader);
		m->m_isSupported=m_isSupported;
		for(int i=0;i<m_passes.size();++i)
		{
			m->AddPass( m_passes[i]->Duplicate());
		}
		return m;
	}



	xml::XMLElement* RenderTechnique::loadXMLSettings(xml::XMLElement* elem)
	{
		IObject::loadFromXML(elem);
		xml::XMLElement*e;
		e=elem->getSubElement(mT("RenderShader"));
		if(e)
		{
			GPUShaderPtr shader=new GPUShader();
			shader->loadXMLSettings(e);
			SetDepthShader(shader);
		}
		e=elem->getSubElement(mT("Pass"));
		while(e)
		{
			core::string name=e->getValueString("Name");
			video::RenderPass* p= GetPassByName(name);
			bool loaded=false;
			if(!p)
			{
				p=new RenderPass(this);
			}else
				loaded=true;
			p->loadXMLSettings(e);
			if(!loaded)
				AddPass(p);
			e=e->nextSiblingElement(mT("Pass"));
		}
		return elem;
	}

	xml::XMLElement*  RenderTechnique::exportXMLSettings(xml::XMLElement* elem)
	{
		xml::XMLElement* e=new xml::XMLElement(mT("Technique"));
		IObject::exportToXML(e);
		elem->addSubElement(e);
		if(m_depthShader)
		{
			xml::XMLElement* e2=new xml::XMLElement(mT("DepthShader"));
			e->addSubElement(e2);
			m_depthShader->exportXMLSettings(e2);
			//node->addAttribute(mT("DepthShader"),m_shadowShader->GetName());
		}

		for(int i=0;i<m_passes.size();++i)
		{
			m_passes[i]->exportXMLSettings(e);
		}
		return e;
	}



}
}
