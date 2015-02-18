

/********************************************************************
	created:	2010/09/04
	created:	4:9:2010   22:01
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ImageDecorateNode.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ImageDecorateNode
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ImageDecorateNode_h__
#define ImageDecorateNode_h__

#include "ITextDecorateNode.h"
#include "TextureUnit.h"

namespace mray
{
namespace GUI
{
class ImageDecorateNode:public ITextDecorateNode
{
public:
	static const core::string Type;
private:
protected:
	GCPtr<video::TextureUnit> m_image;
public:
	ImageDecorateNode();
	virtual ~ImageDecorateNode();
	virtual const core::string& GetType() { return Type; }

	virtual void ParseParameter(const core::UTFString &p);//parameter should be like: "imageName,width,height" or "imageName,size"

	virtual void Draw(TextContextAttributes*context,const math::rectf*clip);

	virtual void CalculateSize(TextContextAttributes*context);
};

class ImageDecorateNodeCreator:public ITextDecorateCreator
{
public:
	ImageDecorateNodeCreator()
	{
	}
	virtual const core::string& GetType() { return ImageDecorateNode::Type; }
	virtual ITextDecorateNode* CreateNode()
	{
		return new ImageDecorateNode();
	}

};

}
}
#endif // ImageDecorateNode_h__
