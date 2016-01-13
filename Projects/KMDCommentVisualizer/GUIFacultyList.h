

#ifndef GUIFacultyList_h__
#define GUIFacultyList_h__

#include "GUIFacultyListLayout.h"
#include "CSubProject.h"
#include "GUIDockedElement.h"

namespace mray
{
namespace GUI
{
	class GUIProfilePicture;

class GUIFacultyList :public IGUIPanelElement, public GUIFacultyListLayout,public GUIDockedElement
{
	DECLARE_RTTI;
public:
	static const GUID ElementType;
protected:

	std::vector<GUI::GUIProfilePicture*> m_advisers;
	std::vector<GUI::GUIProfilePicture*> m_committee;


	void _ProcessList(const std::vector<kmd::KMDUser*>& p, std::vector<GUI::GUIProfilePicture*> &target, GUIStackPanel* o);

public:
	GUIFacultyList(IGUIManager* m);
	virtual ~GUIFacultyList();

	void SetSubProject(kmd::CSubProject* p);

	virtual void Update(float dt);

	virtual void LoadXMLSettings(xml::XMLElement*e);
};
DECLARE_ELEMENT_FACTORY(GUIFacultyList);

}
}
#endif // GUIFacultyList_h__
