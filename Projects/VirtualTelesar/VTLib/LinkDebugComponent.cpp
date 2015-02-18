

#include "stdafx.h"
#include "LinkDebugComponent.h"
#include "IPhysical6DOFJointComponent.h"
#include "PhysicsComponent.h"
#include "SceneComponent.h"
#include "ISceneNode.h"
#include "IRenderable.h"
#include "RenderMaterial.h"
#include "RenderTechnique.h"
#include "RenderPass.h"
#include "TextureResourceManager.h"
#include "MaterialResourceManager.h"

#include "DebugRenderSettings.h"
#include "IGUIButton.h"
#include "IGUIManager.h"
#include "IGUIPanelElement.h"
#include "IGUIStackPanel.h"
#include "CoupledJointComponent.h"

namespace mray
{
namespace VT
{

	

	class LinkDebugComponentData
	{
	public:
		LinkDebugComponentData()
		{
			targetInput=ELink_Realtime;
			cascade=true;
			targetAlpha=0.5;
			thresholdAngle=0.5;
			color.Set(1,0,0,1);
			useKinematics=true;
		}
		~LinkDebugComponentData()
		{

			for (int i=0;i<m_links.size();++i)
			{
				delete m_links[i];
			}

			AttachmentMap::iterator it=m_attachments.begin();
			for (;it!=m_attachments.end();++it)
			{
				delete it->second;
			}
		}

		struct ChainInfo;
		struct LinkInfo
		{
			CoupledJointInfo* controller;
			game::IPhysical6DOFJointComponent* link;
			scene::ISceneNode* parent;
			game::IPhysicsComponent* comp;
			std::vector<scene::ISceneNode*> nodes;
			math::matrix4x4 rotationMat;
			video::RenderMaterial* material;
			float alpha;
			bool visible;

			ChainInfo* chainPtr;

			LinkInfo()
			{
				alpha=0;
				link=0;
				parent=0;
				nodes.clear();
			}
		};

		struct ChainInfo
		{
			ChainInfo* parent;
			LinkInfo* link;
			std::vector<ChainInfo*> children;

		};

		float targetAlpha;
		float thresholdAngle;
		video::SColor color;
		bool cascade;
		ELinkDebugTargetInput targetInput;
		bool useKinematics;

		typedef std::map<LinkInfo*,ChainInfo*> AttachmentMap;
		AttachmentMap m_attachments;
		std::vector<LinkInfo*> m_links;
		std::vector<ChainInfo*> m_roots;

		void _UpdateLink(ChainInfo* l);
		void _ProcessNode(game::IPhysicsComponent* comp,LinkInfo &ifo);
		void Init(LinkDebugComponent *o);
#if 0
		void Init(LinkDebugComponent *o)
		{

			video::RenderMaterial* m=gMaterialResourceManager.getMaterial("LinkDebugMtrl");//->Duplicate();
			m->GetActiveTechnique()->GetPassAt(0)->SetAlpha(targetAlpha);
			m->GetActiveTechnique()->GetPassAt(0)->SetDiffuse(color);

			std::map<game::IPhysicsComponent*,LinkInfo*> nodes;

			//iterate and get all scene nodes from the component
			const std::list<IObjectComponent*>& lst=o->GetOwnerComponent()->GetComponentsList();
			std::list<IObjectComponent*>::const_iterator it=lst.begin();
			for(;it!=lst.end();++it)
			{
				game::IPhysical6DOFJointComponent* c=dynamic_cast<game::IPhysical6DOFJointComponent*>(*it);
				if(c)
				{

					if(!c->IsInited())
						c->InitComponent();

					if(!c->GetTargetComponentA() || !c->GetTargetComponentB())
						continue;

					LinkInfo *alink=0,*blink=0;

					std::map<game::IPhysicsComponent*,LinkInfo*>::iterator it2=nodes.find(c->GetTargetComponentA());
					if(it2==nodes.end())
					{
						alink=new LinkInfo();
						//alink->link=c;
						_ProcessNode(c->GetTargetComponentA(),*alink);
						nodes[c->GetTargetComponentA()]=alink;
						m_links.push_back(alink);
					}else
					{
						alink=it2->second;
						//alink->link=c;
					}
					it2=nodes.find(c->GetTargetComponentB());
					if(it2==nodes.end())
					{
						blink=new LinkInfo();
						blink->link=c;
						_ProcessNode(c->GetTargetComponentB(),*blink);
						nodes[c->GetTargetComponentB()]=blink;
						m_links.push_back(blink);
					}else
					{
						blink=it2->second;
						blink->link=c;
					}


					ChainInfo* chain=0;
					AttachmentMap::iterator it3=m_attachments.find(alink);
					if(it3!=m_attachments.end())
					{
						chain=it3->second;
					}else
					{
						chain=new ChainInfo();
						chain->link=alink;
						m_attachments[alink]=chain;
						alink->chainPtr=chain;
					}
					alink->parent->addChild(blink->parent);

					it3=m_attachments.find(blink);

					if(it3!=m_attachments.end())
					{
						it3->second->parent=chain;
						chain->children.push_back(it3->second);
					}else
					{
						ChainInfo* bchain=new ChainInfo();
						bchain->link=blink;
						m_attachments[blink]=bchain;
						blink->chainPtr=bchain;
						chain->children.push_back(bchain);
						bchain->parent=chain;
					}

				}
			}

			math::matrix4x4 mat[2];
			//fix the placement of joints
			for(int j=0;j<m_links.size();++j)
			{
				LinkInfo* link=m_links[j];
				if(!link->link)
				{
					//make this link as a root
					AttachmentMap::iterator it= m_attachments.find(link);
					if(it!=m_attachments.end())
						m_roots.push_back(it->second);
					link->parent->setPosition(link->comp->GetPhysicalNode()->getGlobalPosition());
					link->parent->setOrintation(link->comp->GetPhysicalNode()->getGlobalOrintation());
					continue;
				}
				physics::IPhysicalJoint3DDesc*desc= link->link->GetJointDesc();
				for(int i=0;i<2;++i)
				{

					desc->localAxis[i].Normalize();
					desc->localNormal[i].Normalize();
					//calculate parent matrix
					math::vector3d tang=desc->localAxis[i].crossProduct(desc->localNormal[i]);
					mat[i].setComponent(desc->localAxis[i],desc->localNormal[i],tang);
					//	mat[i]=mat[i].getTransposed();
					mat[i].setTranslation(desc->localAnchor[i]);
				}
				//mat[0]=mat[0]*mat[1];
				math::quaternion q(mat[0]);
				q.Normalize();
				link->rotationMat=mat[0];
				link->parent->setOrintation(q);
				link->parent->setPosition(mat[0].getTranslation());
			}
		}
#endif

		void Update(float dt)
		{

			for(int i=0;i<m_roots.size();++i)
			{
				_UpdateLink(m_roots[i]);
			}
		}
	};

	void LinkDebugComponentData::Init(LinkDebugComponent *o)
	{

		video::RenderMaterial* m=gMaterialResourceManager.getMaterial("LinkDebugMtrl");//->Duplicate();
		m->GetActiveTechnique()->GetPassAt(0)->SetAlpha(targetAlpha);
		m->GetActiveTechnique()->GetPassAt(0)->SetDiffuse(color);

		std::map<game::IPhysicsComponent*,LinkInfo*> nodes;

		//iterate and get all scene nodes from the component
		const std::list<IObjectComponent*>& lst=o->GetOwnerComponent()->GetComponentsList();
		std::list<IObjectComponent*>::const_iterator it=lst.begin();
		for(;it!=lst.end();++it)
		{
			CoupledJointComponent* cj=dynamic_cast<CoupledJointComponent*>(*it);
			if(!cj)
				continue;
			cj->InitComponent();
			const std::vector<CoupledJointInfo*>& jlst=cj->GetAttachedJoints();
			for (std::vector<CoupledJointInfo*>::const_iterator jit=jlst.begin();
				jit!=jlst.end();++jit)
			{
				game::IPhysical6DOFJointComponent* c=dynamic_cast<game::IPhysical6DOFJointComponent*>((*jit)->GetJoint());
				if(!c)
					continue;

				if(!c->IsInited())
					c->InitComponent();

				if(!c->GetTargetComponentA() || !c->GetTargetComponentB())
					continue;

				LinkInfo *alink=0,*blink=0;

				std::map<game::IPhysicsComponent*,LinkInfo*>::iterator it2=nodes.find(c->GetTargetComponentA());
				if(it2==nodes.end())
				{
					alink=new LinkInfo();
					//alink->link=c;
					_ProcessNode(c->GetTargetComponentA(),*alink);
					nodes[c->GetTargetComponentA()]=alink;
					m_links.push_back(alink);
				}else
				{
					alink=it2->second;
					//alink->link=c;
				}
				it2=nodes.find(c->GetTargetComponentB());
				if(it2==nodes.end())
				{
					blink=new LinkInfo();
					_ProcessNode(c->GetTargetComponentB(),*blink);
					nodes[c->GetTargetComponentB()]=blink;
					m_links.push_back(blink);
				}else
				{
					blink=it2->second;
				}
				blink->controller=*jit;
				blink->link=c;



				ChainInfo* chain=0;
				AttachmentMap::iterator it3=m_attachments.find(alink);
				if(it3!=m_attachments.end())
				{
					chain=it3->second;
				}else
				{
					chain=new ChainInfo();
					chain->link=alink;
					m_attachments[alink]=chain;
					alink->chainPtr=chain;
				}
				alink->parent->addChild(blink->parent);

				it3=m_attachments.find(blink);

				if(it3!=m_attachments.end())
				{
					it3->second->parent=chain;
					chain->children.push_back(it3->second);
				}else
				{
					ChainInfo* bchain=new ChainInfo();
					bchain->link=blink;
					m_attachments[blink]=bchain;
					blink->chainPtr=bchain;
					chain->children.push_back(bchain);
					bchain->parent=chain;
				}

			}
		}

		math::matrix4x4 mat[2];
		//fix the placement of joints
		for(int j=0;j<m_links.size();++j)
		{
			LinkInfo* link=m_links[j];
			if(!link->link)
			{
				//make this link as a root
				AttachmentMap::iterator it= m_attachments.find(link);
				if(it!=m_attachments.end())
					m_roots.push_back(it->second);
				game::PhysicsComponent* c=dynamic_cast<game::PhysicsComponent*>(link->comp);
				if(c)
				{
					link->parent->setPosition(c->GetPhysicalNode()->getGlobalPosition());
					link->parent->setOrintation(c->GetPhysicalNode()->getGlobalOrintation());
				}else if(link->comp)
				{
					link->parent->setPosition(link->comp->GetPosition());
					link->parent->setOrintation(link->comp->GetRotation());
				}
				continue;
			}
			physics::IPhysicalJoint3DDesc*desc= link->link->GetJointDesc();
			for(int i=0;i<1;++i)
			{

// 				desc->localAxis[i].Normalize();
// 				desc->localNormal[i].Normalize();
				//calculate parent matrix
				math::vector3d tang=desc->localAxis[i].crossProduct(desc->localNormal[i]);
				mat[i].setComponent(desc->localAxis[i],desc->localNormal[i],tang);
				//	mat[i]=mat[i].getTransposed();
				mat[i].setTranslation(desc->localAnchor[i]);
			}
			//mat[0]=mat[0]*mat[1];
		//	math::quaternion q(mat[0]);
		//	q.Normalize();
			link->rotationMat=mat[0];
		//	link->parent->setOrintation(q);
			link->parent->setPosition(mat[0].getTranslation());
		//	link->parent->setOrintation(link->comp->GetRotation());
		}
	}

	
	void LinkDebugComponentData::_ProcessNode(game::IPhysicsComponent* comp,LinkInfo &ifo)
	{
		ifo.comp=comp;
	
		const std::list<game::PhysicalComponentAttachment*> &lst=comp->GetAttachedNodes();
		std::list<game::PhysicalComponentAttachment*>::const_iterator it2=lst.begin();
		ifo.parent=0;
		video::RenderMaterial* m=gMaterialResourceManager.getMaterial("LinkDebugMtrl");//->Duplicate();
		ifo.material=m;
		//go over the attached nodes
		for (;it2!=lst.end();++it2)
		{
			game::SceneComponent* comp=(*it2)->GetTargetNode();

			if(!comp->IsInited())
				comp->InitComponent();
			scene::ISceneNode* snode=comp->GetSceneNode();
			if(!ifo.parent)
			{
				ifo.parent=snode->getSceneManager()->createSceneNode(comp->GetName()+"#DBG_Parent");
			}
			scene::ISceneNode* node2=snode->getSceneManager()->createSceneNode(snode->getNodeName()+"#DBG");
			const scene::AttachNodesList& mlst= snode->GetAttachedNodes();
			scene::AttachNodesList::const_iterator it3=mlst.begin();

			/*m->GetActiveTechnique()->GetPassAt(0)->setMaterialRenderer(video::MRT_TRANSPARENT);

			=new video::RenderMaterial();

			video::RenderPass* p= m->CreateTechnique("default")->CreatePass("default");
			p->SetDiffuse(video::SColor(1,0,0,1));
			p->SetAlpha(0.5);
			//p->setRenderShader(ShaderResourceManager::getInstance());*/
			for (;it3!=mlst.end();++it3)
			{
				scene::MeshRenderableNode* rnode= new scene::MeshRenderableNode((*it3)->getMesh()->duplicateContextOnly());
				if(m==0)
				{
					m=(*it3)->getMaterial(0)->Duplicate();
					m->GetActiveTechnique()->GetPassAt(0)->SetDiffuse(video::SColor(1,0,0,0.5));
					m->GetActiveTechnique()->GetPassAt(0)->setMaterialRenderer(video::MRT_TRANSPARENT);
					m->GetActiveTechnique()->GetPassAt(0)->SetAlpha(0.5);
					m->GetActiveTechnique()->GetPassAt(0)->setTexture(gTextureResourceManager.loadTexture2D("white.bmp"),0);
				}
				for(int j=0;j<rnode->getMaterialCount();++j)
					rnode->setMaterial(m,j);
				node2->AttachNode(rnode);

				ifo.nodes.push_back(node2);
			}

		//	node2->setPosition(0);//snode->getAbsolutePosition());
		//	node2->setOrintation(0);//snode->getAbsoluteOrintation());

			ifo.parent->addChild(node2);

		}
	}
	void LinkDebugComponentData::_UpdateLink(ChainInfo* l)
	{
		LinkInfo* link=l->link;
		if(link->link)
		{
			bool pVis=false;
			if(l->parent)
				pVis=l->parent->link->visible;
			float controlValue;
			if(useKinematics && false)
				controlValue=link->controller->GetValue().KinValue;
			else
				controlValue=link->controller->GetValue().RealtimeValue;

			math::vector3d controlVec=link->link->TransformToJointSpaceAngles(math::vector3d(controlValue,0,0));
			math::vector3d targetRotation=link->rotationMat.transformNormal( controlVec);
			math::vector3d diff=targetRotation-link->link->GetCurrentAngles();
			float diffAbs=fabs(diff.x)+fabs(diff.y)+fabs(diff.z);
			math::quaternion v(targetRotation);
			math::quaternion v2= link->link->GetCurrentAngles();
			link->alpha=(1-(v*math::vector3d::ZAxis).dotProduct(v2*math::vector3d::ZAxis));
			link->visible=diffAbs>thresholdAngle || pVis && cascade;
			if(link->visible)
			{
				int i=0;
			}
	
			for (int i=0;i<link->nodes.size();++i)
			{
				link->nodes[i]->setVisible(link->visible);
			}
			//link->material->GetActiveTechnique()->GetPassAt(0)->SetAlpha(math::Min<float>(1,40*(link->alpha+pAlpha)));

			//	math::vector3d transV=link->rotationQuat*v;
			link->parent->setOrintation(v);

		}else
		{
			link->visible=false;
			for (int i=0;i<link->nodes.size();++i)
			{
				link->nodes[i]->setVisible(link->visible);
			}

			//link->material->GetActiveTechnique()->GetPassAt(0)->SetAlpha(0);

			//	math::vector3d transV=link->rotationQuat*v;
			game::PhysicsComponent* c=dynamic_cast<game::PhysicsComponent*>(link->comp);
			if(c)
			{
				link->parent->setOrintation(c->GetPhysicalNode()->getGlobalOrintation());
			}

		}
		//update child links 
		for(int j=0;j<l->children.size();++j)
		{
			_UpdateLink(l->children[j]);
		}
	}


	IMPLEMENT_PROPERTY_TYPE_HEADER(Cascade,LinkDebugComponent,bool,mT("Cascade"),EPBT_Basic,mT(""),0);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Cascade,LinkDebugComponent,bool,SetCascade,GetCascade,core::StringConverter::toString,core::StringConverter::toBool,false);

	IMPLEMENT_PROPERTY_TYPE_HEADER(UseKinematic,LinkDebugComponent,bool,mT("UseKinematic"),EPBT_Basic,mT(""),true);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(UseKinematic,LinkDebugComponent,bool,SetUseKinematic,GetUseKinematic,core::StringConverter::toString,core::StringConverter::toBool,false);

	IMPLEMENT_PROPERTY_TYPE_HEADER(ThresholdAngle,LinkDebugComponent,float,mT("ThresholdAngle"),EPBT_Basic,mT(""),0);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(ThresholdAngle,LinkDebugComponent,float,SetThresholdAngle,GetThresholdAngle,core::StringConverter::toString,core::StringConverter::toFloat,false);

	IMPLEMENT_PROPERTY_TYPE_HEADER(TargetAlpha,LinkDebugComponent,float,mT("TargetAlpha"),EPBT_Basic,mT(""),0);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(TargetAlpha,LinkDebugComponent,float,SetTargetAlpha,GetTargetAlpha,core::StringConverter::toString,core::StringConverter::toFloat,false);

	IMPLEMENT_PROPERTY_TYPE_HEADER(Color,LinkDebugComponent,video::SColor,mT("Color"),EPBT_Basic,mT(""),video::SColor(1,0,0,1));
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Color,LinkDebugComponent,video::SColor,SetColor,GetColor,core::StringConverter::toString,core::StringConverter::toColor,false);


	LinkDebugComponent::LinkDebugComponent(game::GameEntityManager*)
	{
		m_data=new LinkDebugComponentData();
		CPropertieDictionary* dic;
		if(CreateDictionary(&dic))
		{
			dic->addPropertie(&PropertyTypeTargetAlpha::instance);
			dic->addPropertie(&PropertyTypeThresholdAngle::instance);
			dic->addPropertie(&PropertyTypeColor::instance);
			dic->addPropertie(&PropertyTypeCascade::instance);
		}
	}
	LinkDebugComponent::~LinkDebugComponent()
	{
		delete m_data;
	}

	bool LinkDebugComponent::InitComponent()
	{
		if(!IVTComponent::InitComponent())
			return false;
		m_data->Init(this);
		if(false && DebugRenderSettings::guiManager)
		{
			GUI::IGUIStackPanel *stackPanel=(GUI::IGUIStackPanel*)DebugRenderSettings::guiManager->CreateElement(GUI::IGUIStackPanel::ElementType);
			GUI::IGUIButton* slider=(GUI::IGUIButton*)DebugRenderSettings::guiManager->CreateElement(GUI::IGUIButton::ElementType);
		
			DebugRenderSettings::CreatedElements[GetName()]=stackPanel;
			slider->SetSize(math::vector2d(30,30));
			slider->SetText(core::string("+"));
			slider=(GUI::IGUIButton*)DebugRenderSettings::guiManager->CreateElement(GUI::IGUIButton::ElementType);
			stackPanel->AddElement(slider);

			slider->SetSize(math::vector2d(30,30));
			slider->SetText(core::string("-"));
			stackPanel->AddElement(slider);

			stackPanel->SetStackDirection(GUI::IGUIStackPanel::EHorizontal);
			stackPanel->SetSize(math::vector2d(180,180));
			if(DebugRenderSettings::rootElement)
				DebugRenderSettings::rootElement->AddElement(stackPanel);
		}
		return true;
	}


	bool LinkDebugComponent::SetCascade(bool v)
	{
		m_data->cascade=v;
		return true;
	}
	bool LinkDebugComponent::GetCascade()
	{
		return m_data->cascade;
	}
	bool LinkDebugComponent::SetUseKinematic(bool v)
	{
		m_data->useKinematics=v;
		return true;
	}
	bool LinkDebugComponent::GetUseKinematic()
	{
		return m_data->useKinematics;
	}
	bool LinkDebugComponent::SetThresholdAngle(float v)
	{
		m_data->thresholdAngle=v;
		return true;
	}

	float LinkDebugComponent::GetThresholdAngle()
	{
		return m_data->thresholdAngle;
	}


	bool LinkDebugComponent::SetTargetAlpha(float v)
	{
		m_data->targetAlpha=v;
		return true;
	}

	float LinkDebugComponent::GetTargetAlpha()
	{
		return m_data->targetAlpha;
	}


	bool LinkDebugComponent::SetColor(const video::SColor& v)
	{
		m_data->color=v;
		return true;
	}

	const video::SColor& LinkDebugComponent::GetColor()
	{
		return m_data->color;
	}


	void LinkDebugComponent::PreUpdate()
	{
	}
	void LinkDebugComponent::Update(float dt)
	{
		m_data->Update(dt);
	}
	void LinkDebugComponent::DebugRender(scene::IDebugDrawManager* renderer)
	{
	}

	void LinkDebugComponent::OnDestroy()
	{
	}




}
}


