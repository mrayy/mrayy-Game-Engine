

/********************************************************************
created:	2012/05/22
created:	22:5:2012   15:26
filename: 	d:\Development\mrayEngine\Tests\TestOpenCV\GUIWebPageOrganizer.h
file path:	d:\Development\mrayEngine\Tests\TestOpenCV
file base:	GUIWebPageOrganizer
file ext:	h
author:		MHD Yamen Saraiji

purpose:	
*********************************************************************/


#ifndef __GUIWebPageOrganizer__
#define __GUIWebPageOrganizer__


namespace mray
{

namespace GUI
{

	class GUIWebPageOrganizer:public IGUIPanelElement
	{
	protected:

		bool m_fullScreen;
		std::vector<GUIWebPageControl*> m_pages;
		int m_activePage;

		void _SwitchFullscreen()
		{
			m_fullScreen=!m_fullScreen;
			if(m_fullScreen)
			{
				for(int i=0;i<m_pages.size();++i)
				{
					if(i!=m_activePage)
						m_pages[i]->SetVisible(false);
				}
			}else
			{
				for(int i=0;i<m_pages.size();++i)
				{
					m_pages[i]->SetVisible(true);
				}
			}
		}
	public:
		static const GUID ElementType;
		GUIWebPageOrganizer(IGUIManager* creator):IGUIPanelElement(ElementType,creator),m_fullScreen(false)
		{
			m_fullScreen=false;
			m_activePage=0;
		}

		~GUIWebPageOrganizer()
		{
		}

		GUIWebPageControl* GetActivePage()
		{
			return m_pages[m_activePage];
		}

		void toFront(IGUIElementPtr e)
		{
			ElementList::iterator it=m_elements.begin();
			for(;it!=m_elements.end();++it)
			{
				if(*it==e)
				{
					m_elements.erase(it);
					m_elements.push_back(e);
					break;
				}
			}

		}

		bool OnEvent(Event*e)
		{
			bool res=IGUIPanelElement::OnEvent(e);
			if(e->getType()==ET_Keyboard)
			{
				KeyboardEvent* evt=(KeyboardEvent*)e;

				if(evt->Char=='`')
				{
					_SwitchFullscreen();
				}
			}
			if(res)
			{
				for(int i=0;i<m_pages.size();++i)
				{
					if(GetCreator()->GetFocusElement()==m_pages[i])
					{
						m_activePage=i;
						toFront(m_pages[i]);


						break;
					}
				}
			}
			return res;
		}
		void AddNewTab(GCPtr<GUIWebPageControl> ctrl)
		{
			this->AddElement(ctrl);
			m_pages.push_back(ctrl);
		}

		virtual void Update(float dt)
		{
			IGUIElement::Update(dt);
			if(m_pages.size()==0)
				return;
			float speed=10;
			if(m_fullScreen)
			{
				math::rectf trc=math::rectf(0,GetSize());
				math::rectf rc=m_pages[m_activePage]->GetDefaultRegion()->GetRect();

				rc.ULPoint+=(trc.ULPoint-rc.ULPoint)*speed*dt;
				rc.BRPoint+=(trc.BRPoint-rc.BRPoint)*speed*dt;
				m_pages[m_activePage]->SetPosition(rc.ULPoint);
				m_pages[m_activePage]->SetSize(rc.getSize());
			}else
			{
				for(int i=0;i<m_pages.size();++i)
				{
					math::rectf trc(100+i*50,100+i*20,500+i*50,500+i*20);
					math::rectf rc=m_pages[i]->GetDefaultRegion()->GetRect();

					rc.ULPoint+=(trc.ULPoint-rc.ULPoint)*speed*dt;
					rc.BRPoint+=(trc.BRPoint-rc.BRPoint)*speed*dt;
					m_pages[i]->SetPosition(rc.ULPoint);
					m_pages[i]->SetSize(rc.getSize());
				}
			}
		}
		virtual IGUIElement* Duplicate(){return new GUIWebPageOrganizer(GetCreator());}
		virtual FontAttributes* GetFontAttributes(){return 0;}

	};
	const GUID GUIWebPageOrganizer::ElementType(mT("WebPageOrganizer"));

}

#endif
