


/********************************************************************
	created:	2012/02/20
	created:	20:2:2012   11:53
	filename: 	d:\Development\mrayEngine\Engine\mrayNVPrefHUD\GUILineStream.h
	file path:	d:\Development\mrayEngine\Engine\mrayNVPrefHUD
	file base:	GUILineStream
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __GUILineStream__
#define __GUILineStream__

#include "IGUIElement.h"
#include "mrayNVPerfSDK.h"
#include "FontAttributes.h"
#include <map>

namespace mray
{
namespace GUI
{

class GUILineStream:public IGUIElement
{
	DECLARE_RTTI;

private:
protected:

	struct TraceData
	{
		int index;
		core::UTFString title;
		video::SColor clr;
		std::vector<float> samples;
		std::vector<math::vector2d> points;
	};

	typedef std::map<int,TraceData*> TracteMap;
	TracteMap m_traces;

	int m_currentTraceSample;
	int m_maxSamplesCount;

	FontAttributes m_fontAttrs;

	void _UpdatePoints();
	bool m_dirty;
public:
	static const GUID ElementType;

	GUILineStream(IGUIManager*mngr);
	virtual~GUILineStream();

	void SetMaxSamplesCount(int scount);
	int	 GetMaxSamplesCount()const;

	void AddTrace(int idx,const video::SColor& clr,const core::UTFString& title);
	void RemoveTrace(int idx);
	bool HasTrace(int idx);
	void ClearTraces();

	void SetTraceColor(int idx,const video::SColor& clr);
	void SetTraceValue(int idx,float v);
	void SetTraceTitle(int idx,const core::UTFString& title);

	virtual void Draw();

	virtual FontAttributes* GetFontAttributes()
	{
		return &m_fontAttrs;
	}
	virtual IGUIElement* Duplicate();
};

}
}

#endif
