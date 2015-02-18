

/********************************************************************
	created:	2013/05/07
	created:	7:5:2013   21:02
	filename: 	C:\Development\mrayEngine\Engine\mrayEngine\GUITreeNode.h
	file path:	C:\Development\mrayEngine\Engine\mrayEngine
	file base:	GUITreeNode
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GUITreeNode__
#define __GUITreeNode__


#include "ImageSet.h"
#include <vector>

namespace mray
{
	class MouseEvent;

namespace GUI
{
	class IFont;
	class IGUITreeBox;
	class FontAttributes;
	class GUITreeNode;
	typedef std::vector<GUITreeNode*> GUITreeNodeList;

	struct GUITreeBoxDrawInfo
	{
		FontAttributes* fontAttrs;
		IGUITreeBox* owner;
		math::vector2d pos;
		math::rectf rc;
		IFont* font;
	};
class MRAY_DLL GUITreeNode
{
protected:

	core::string m_text;
	video::SColor m_backcolor;
	bool m_expanded;
	video::ImageSetPtr m_imageSet;
	core::string m_imageIndex;

	GUITreeNodeList m_nodes;


public:
	GUITreeNode():m_expanded(false),m_backcolor(1,1,1,0)
	{}
	virtual~GUITreeNode()
	{
		for (int i=0;i<m_nodes.size();++i)
		{
			delete m_nodes[i];
		}
	}


	GUITreeNode* AddItem(const core::string& text)
	{
		GUITreeNode* node=new GUITreeNode();
		node->SetText(text);

		m_nodes.push_back(node);
		return node;
	}
	void RemoveItem(GUITreeNode* node)
	{
		GUITreeNodeList::iterator it= m_nodes.begin();
		for (;it!=m_nodes.end();++it)
		{
			if(*it==node)
			{
				m_nodes.erase(it);
				break;
			}
		}
	}
	void ClearItems()
	{
		for(int i=0;i<m_nodes.size();++i)
		{
			delete m_nodes[i];
		}
		m_nodes.clear();
	}

	const core::string& GetText()const{return m_text;}
	void SetText(const core::string& t){m_text=t;}


	const video::SColor& GetBackColor()const{return m_backcolor;}
	void SetBackColor(const video::SColor& c){m_backcolor=c;}

	bool IsExpanded(){return m_expanded;}
	void SetExpanded(bool b,bool children)
	{
		m_expanded=b;
		if(children)
		{
			for(int i=0;i<m_nodes.size();++i)
				m_nodes[i]->SetExpanded(b,true);
		}
	}
	
	video::ImageSetCRef GetImageSet(){return m_imageSet;}
	void SetImageSet(video::ImageSetCRef s){m_imageSet=s;}
	
	const core::string& GetImageIndex(){return m_imageIndex;}
	void SetImageIndex(	const core::string& idx){m_imageIndex=idx;}

	const GUITreeNodeList& GetNodes()const{return m_nodes;}

	GUITreeNodeList& GetNodes(){return m_nodes;}

	virtual void Draw(GUITreeBoxDrawInfo& ifo);
	virtual bool OnMouseEvent(MouseEvent* e,GUITreeBoxDrawInfo& ifo);

public:
	void* userData;

};

}
}


#endif
