
/********************************************************************
	created:	2012/07/25
	created:	25:7:2012   21:58
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\GUIGraphPlotElement.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	GUIGraphPlotElement
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___GUIGraphPlotElement___
#define ___GUIGraphPlotElement___

#include "IGUIElement.h"

namespace mray
{
namespace GUI
{

class GUIGraphPlotElement:public GUI::IGUIElement
{
protected:
	struct GraphInfo
	{
	public:
		GraphInfo()
		{
			minVal=0;
			maxVal=0;
			currSample=0;
			lastVal=0;
		}
		core::string name;
		video::SColor color;
		std::vector<float> values;
		float lastVal;
		float minVal;
		float maxVal;
		std::vector<math::vector2d> points;
		int currSample;
	};
	typedef std::map<core::string,GraphInfo*> GraphList;
	GraphList m_graphs;
	int m_maxSamplesCount;

	GUI::FontAttributes m_font;
	float m_max;
	float m_min;

public:
	static const GUID ElementType;
public:
	GUIGraphPlotElement(GUI::IGUIManager* crtr);
	virtual~GUIGraphPlotElement();
	
	void AddGraph(const core::string &name,const video::SColor& clr);
	void RemoveGraph(const core::string &name);
	void AddValue(const core::string &name,float v);
	void Clear();


	bool HasGraph(const core::string& name);

	void ResetValues();

	void SetMaxSamplesCount(int cnt);

	virtual void Draw(const math::rectf*vp);
	virtual void Update(float dt);
};

}
}

#endif

