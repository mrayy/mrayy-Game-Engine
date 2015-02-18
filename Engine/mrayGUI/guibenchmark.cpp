

#include "GUIBenchMark.h"
#include "GUIManager.h"
#include "CBenchMark.h"

namespace mray{
namespace GUI{


GUIBenchMark::GUIBenchMark(GUIManager* manager,math::vector2d pos,math::vector2d size,GCPtr<GUIElement>  parent,video::SColor color)
:GUIList(manager,mT(""),pos,size,0,parent,color)
{
	setTypeStr(mT("BenchMark"));
}

GUIBenchMark::~GUIBenchMark()
{
}

void GUIBenchMark::pushChilds(IBenchmarkItem*elem,int t){
	SItem item;
	item.tab=t;
	std::list<GCPtr<IBenchmarkItem>>::iterator it= elem->subItems.begin();
	for(;it!=elem->subItems.end();++it){
		
		item.name=(*it)->m_name;
		item.percentage=(*it)->m_percentage;
		item.time=(*it)->m_time;
		items.push_back(item);
		pushChilds((*it),t+1);
	}
}
void GUIBenchMark::draw(float dt){
	if(!Visible)return;
	items.resize(0);
	

	core::IteratorPair<CBenchMark::BenchmarkItemsList> p= CBenchMark::getInstance().getItems();
	SItem item;
	item.tab=0;
	for(;!p.done();p++){
		item.name=(*p)->m_name;
		item.percentage=(*p)->m_percentage;
		item.time=(*p)->m_time;
		items.push_back(item);
		pushChilds(*p,1);
		
	}
	itemList.resize(0);
	core::string str;

	for(int i=0;i<items.size();++i){
		str="";
		for(int j=0;j<items[i].tab;j++)
			str+=mT("-  ");
		str+=items[i].name;
		str+=mT(" : time = ");
		str+=core::StringConverter::toString(items[i].time*0.001f,6);
		str+=mT("sec , percentage = ");
		str+=core::StringConverter::toString(items[i].percentage,4);
		str+=mT(" %%");
		itemList.push_back(listItem(str.c_str(), video::ImageSetTexture(),0));
		if(i<items.size()-1 && items[i+1].tab==0){
			itemList.push_back(listItem(mT(""),video::ImageSetTexture(),0));
		}
	}
	GUIList::draw(dt);

	///////////
	/*
	GCPtr<GUI::IFont>font=guiMngr->Font();
	core::string str;
	rectf *clp=&getClipRect();
	rectf rc=getRect();
	device->draw2DRectangle(rc,Color,1,clp);
	device->draw2DRectangle(rc,video::SColor(0,0,0,255),0,clp);
	float ySZ=font->getDimension("A",fontSize).y;
	for(int i=0;i<items.size();++i){
		str="";
		for(int j=0;j<items[i].tab;j++)
			str+="-  ";
		str+=items[i].name;
		str+=" : time = ";
		str+=items[i].time;
		str+="ms , percentage = ";
		str+=items[i].percentage;
		str+=" %%";
		font->print(rc,fontSize,0,fontColor,clp,0,str.c_str());
		rc.ULPoint.y+=ySZ + 3;
		if(i<items.size()-1 && items[i].tab==0){
			font->print(rc,fontSize,0,fontColor,clp,0," ");
			rc.ULPoint.y+=ySZ + 3;
		}
	}*/

}

}
}



