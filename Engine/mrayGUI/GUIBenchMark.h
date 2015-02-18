

#ifndef ___GUIBenchMark___
#define ___GUIBenchMark___

#include "GUIList.h"
#include "IBenchmarkItem.h"

namespace mray{
namespace GUI{

class MRAY_DLL GUIBenchMark:public GUIList
{
	struct SItem{
		int tab;
		core::string name;
		float time;
		float percentage;
	};
	std::vector<SItem> items;
	void pushChilds(IBenchmarkItem*elem,int t);
public:
	DECLARE_RTTI;

	GUIBenchMark(GUIManager* manager,math::Point2df pos,math::Point2df size,GCPtr<GUIElement>  parent=0,video::SColor color=240);
	virtual ~GUIBenchMark();

	virtual void draw(float dt);
};

}
}


#endif

