


/********************************************************************
	created:	2014/04/09
	created:	9:4:2014   21:32
	filename: 	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer\RTFTextParser.h
	file path:	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer
	file base:	RTFParser
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __RTFParser__
#define __RTFParser__




namespace mray
{
namespace GUI
{


	class IRTFTextParserListener
	{
	public:
		virtual void OnColor(const video::SColor& clr){}
		virtual void OnSize(int size){}
	};

class RTFTextParser
{
protected:
public:
	RTFTextParser();
	virtual~RTFTextParser();
};

}
}

#endif
