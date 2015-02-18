

/********************************************************************
	created:	2010/09/02
	created:	2:9:2010   17:49
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\SizeDecorateNode.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	SizeDecorateNode
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef SizeDecorateNode_h__
#define SizeDecorateNode_h__


#include "ITextDecorateNode.h"

namespace mray
{
namespace GUI
{

class SizeDecorateNode:public ITextDecorateNode
{
public:
	static const core::string Type;
private:
protected:
	float m_Size;
	float m_SavedSize;
public:
	SizeDecorateNode();
	virtual ~SizeDecorateNode();

	virtual void ParseParameter(const core::UTFString &p);
	virtual const core::string& GetType() { return Type; }

	virtual void OnEnter(TextContextAttributes*context);
	virtual void OnExit(TextContextAttributes*context);
};

class SizeDecorateNodeCreator:public ITextDecorateCreator
{
public:
	SizeDecorateNodeCreator()
	{
	}
	virtual const core::string& GetType() { return SizeDecorateNode::Type; }
	virtual ITextDecorateNode* CreateNode()
	{
		return new SizeDecorateNode();
	}

};

}
}
#endif // SizeDecorateNode_h__
