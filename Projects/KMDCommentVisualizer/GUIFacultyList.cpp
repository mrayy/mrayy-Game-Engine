

#include "stdafx.h"
#include "GUIFacultyList.h"
#include "GUIProfilePicture.h"

#include "GUIOverlayManager.h"
#include "GUIOverlay.h"
#include "GUIElementRegion.h"
#include "SessionDetails.h"
#include "AppData.h"

namespace mray
{
namespace GUI
{

	const GUID GUIFacultyList::ElementType = "GUIFacultyList";

	IMPLEMENT_RTTI(GUIFacultyList, IGUIPanelElement);

GUIFacultyList::GUIFacultyList(IGUIManager* m)
:IGUIPanelElement(ElementType, m), GUIDockedElement(this)
{
	GUI::GUIOverlay* o = GUI::GUIOverlayManager::getInstance().LoadOverlay("GUIFacultyListLayout.GUI");
	if (o)
	{
		o->CreateElements(m, this, this, this);

		Advisers->SetUseScroll(false);
		Committee->SetUseScroll(false);

	}
}

GUIFacultyList::~GUIFacultyList()
{
}


void GUIFacultyList::_ProcessList(const std::vector<kmd::KMDUser*>& p, std::vector<GUI::GUIProfilePicture*> &target, GUIStackPanel* o)
{
	float w = o->GetOffset();
	float h = o->GetDefaultRegion()->GetRect().getHeight();
	for (int i = 0; i < p.size(); ++i)
	{
		GUI::GUIProfilePicture* prof;
		if(target.size()>i)
		{
			prof = target[i];
		}
		else
		{
			prof = new GUI::GUIProfilePicture(GetCreator());
			target.push_back(prof);
			o->AddElement(prof);
		}
		prof->SetSize(h - 10);
		prof->SetPosition((0, 5));
		prof->SetImage(p[i]->GetTexture());
		prof->SetVisible(true);

		w += target[i]->GetSize().x;
		w += o->GetOffset();
	}
	for (int i = p.size(); i < target.size(); ++i)
	{
		target[i]->SetVisible(false);
	}
	o->SetSize(math::vector2d(w, h));
}


void GUIFacultyList::SetSubProject(kmd::CSubProject* p)
{
	const std::vector<kmd::KMDUser*>& adv = p->GetSession()->GetProjectProfessors();
	const std::vector<kmd::KMDUser*>& com = p->GetSession()->GetComitteeProfessors();

	_ProcessList(adv, m_advisers, Advisers);
	_ProcessList(com, m_committee, Committee);
}

void GUIFacultyList::Update(float dt)
{
	if (!IsVisible())
		return;
	UpdateDocking();
	IGUIPanelElement::Update(dt);
}
void GUIFacultyList::LoadXMLSettings(xml::XMLElement*e)
{
	IGUIPanelElement::LoadXMLSettings(e);

	math::vector2d sz;
	sz = this->GetSize();
	sz.y = core::StringConverter::toInt(gAppData.GetValue("UI", "FacultyHeight", core::StringConverter::toString(sz.y)));
	this->SetSize(sz);

	AdvisersLbl->GetFontAttributes()->fontSize = sz.y / 5;
	committeeLbl->GetFontAttributes()->fontSize = sz.y / 5;
}

IMPLEMENT_ELEMENT_FACTORY(GUIFacultyList);

}
}
