

/********************************************************************
	created:	2010/09/02
	created:	2:9:2010   17:44
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ColorDecorateNode.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ColorDecorateNode
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ColorDecorateNode_h__
#define ColorDecorateNode_h__

#include "ITextDecorateNode.h"

namespace mray
{
namespace GUI
{

class ColorDecorateNode:public ITextDecorateNode
{
public:
	static const core::string Type;
private:
protected:
	video::SColor m_Color;
	video::SColor m_SavedColor;
public:
	ColorDecorateNode();
	virtual ~ColorDecorateNode();

	virtual void ParseParameter(const core::UTFString &p);
	virtual const core::string& GetType() { return Type; }

	virtual void OnEnter(TextContextAttributes*context);
	virtual void OnExit(TextContextAttributes*context);
};

class ColorDecorateNodeCreator:public ITextDecorateCreator
{
public:
	ColorDecorateNodeCreator()
	{
	}
	virtual const core::string& GetType() { return ColorDecorateNode::Type; }
	virtual ITextDecorateNode* CreateNode()
	{
		return new ColorDecorateNode();
	}

};


}
}

#endif // ColorDecorateNode_h__
