

/********************************************************************
	created:	2010/09/01
	created:	1:9:2010   23:34
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ITextDecorateNode.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ITextDecorateNode
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ITextDecorateNode_h__
#define ITextDecorateNode_h__


#include "FontAttributes.h"


namespace mray
{
	class MouseEvent;
	namespace video
	{
		class IVideoDevice;
	}
namespace GUI
{

	class IGUIRenderer;
class IFont;

class TextContextAttributes
{
protected:
	typedef std::map<core::string,core::string> AttributesMap;
	AttributesMap m_attrs;
public:
	FontAttributes fontAttributes;
	IFont* font;
	math::vector2d pos;
	video::IVideoDevice* device;
	TextContextAttributes()
	{
		font=0;
		device=0;
	}

	void SetAttribute(const core::string&attr,const core::string&value);
	core::string GetAttribute(const core::string&attr);
};

class ITextDecorateNode
{
protected:
	typedef std::list<ITextDecorateNode*> NodeList;
	NodeList m_childs;

	math::vector2d m_size;
public:


	ITextDecorateNode();
	virtual~ITextDecorateNode();
	virtual void ParseParameter(const core::UTFString &p){};

	void AddChild(ITextDecorateNode*node){m_childs.push_back(node);}

	virtual const core::string& GetType() = 0;
	virtual void OnEnter(TextContextAttributes*context);
	virtual void Draw(TextContextAttributes*context,IGUIRenderer*renderer,const math::rectf*clip=0);
	virtual void OnExit(TextContextAttributes*context);

	virtual const math::vector2d& GetSize(){return m_size;}

	virtual void CalculateSize(TextContextAttributes*context);

	virtual bool OnEvent(MouseEvent*e);
};

class TextDecorateDummyNode:public ITextDecorateNode
{
	 core::string m_type;
public:
	TextDecorateDummyNode(const core::string& type) :m_type(type)
	{}
	virtual~TextDecorateDummyNode(){}
	virtual const core::string& GetType() { return m_type; }
};

class ITextDecorateCreator
{
protected:
public:
	ITextDecorateCreator()
	{
	}
	virtual~ITextDecorateCreator(){}

	virtual const core::string& GetType() = 0;

	virtual ITextDecorateNode* CreateNode()=0;

};

}
}
#endif // ITextDecorateNode_h__
