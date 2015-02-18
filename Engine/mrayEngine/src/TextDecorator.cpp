#include "stdafx.h"

#include "TextDecorator.h"

#include "TextDecorateNodeFactory.h"
#include "TextDecorateNode.h"


namespace mray
{
namespace GUI
{



//////////////////////////////////////////////////////////////////////////


TextDecorator::TextDecorator()
{
}
TextDecorator::~TextDecorator()
{
}
TextDecorator::EPredictionResult TextDecorator::GetPrediction(const utf32*ptr)
{
	if(*ptr=='<')
	{
		ptr++;
		if(!*ptr)
			return ENormalText;
		if(*ptr=='/')
			return ECloseTag;
		return EOpenTag;
	}
	return ENormalText;
}

const utf32* TextDecorator::_ParserText(const utf32*ptr, ITextDecorateNode* parent)
{

	enum EState
	{
		ENormal,
		ETagOpen,
		ETagClose,
		ETagEnder,
		EInStartTag,
		EInEndTag
	};

	EState state=ENormal;

	bool tagStart=false;
	bool tagEnd=false;
	core::UTFString tagName;
	core::UTFString tagValue;
	core::UTFString text;

	bool readingValue=false;
	bool skipChar=false;

	ITextDecorateNode* node=0;

	while(*ptr)
	{
		utf32 c = *ptr;
		if(!skipChar)
		{
			switch (c)
			{
			case '<':
				{
					EPredictionResult r=GetPrediction(ptr);
					if(r!=ENormalText && text!=core::UTFString::Empty)
					{
						TextDecorateNode*d=new TextDecorateNode(text.GetAsString());
						parent->AddChild(d);
						text = core::UTFString::Empty;
					}
					if(r==EOpenTag)
					{
						state=ETagOpen;
						if(!tagStart)
							tagStart=true;
						tagName = core::UTFString::Empty;
						tagValue = core::UTFString::Empty;
						readingValue=false;
					}else if(r==ECloseTag)
					{
						++ptr;
						state=EInEndTag;
						if(!tagEnd)
							tagEnd=true;
						tagName = core::UTFString::Empty;
					}else
						state=ENormal;
				}
				break;
			case '>':
				if(state==EInEndTag)
				{
					return ptr;
				}else if(state==EInStartTag)
				{
					//create Node
					node=TextDecorateNodeFactory::getInstance().CreateNode(tagName.GetAsString());
					node->ParseParameter(tagValue);
					if(parent)
						parent->AddChild(node);

					ptr=_ParserText(ptr+1,node);
				}
				state=ETagClose;
				break;
			case '\\':
				if(state==ENormal && !skipChar){
					skipChar=true;
					continue;
				}
				break;
			case '=':
				if(state==EInStartTag){
					readingValue=true;
				}
				break;
			default:
				if(state==ETagOpen)
					state=EInStartTag;

				if(state==EInStartTag ){
					if(!readingValue)
						tagName+=c;
					else tagValue+=c;
				}else if(state==EInEndTag)
				{
					tagName+=c;
				}else{
					state=ENormal;
				}

			}
		}
		if(state==ENormal)
			text+=c;
		skipChar=false;

		++ptr;
	}
	return ptr;
}

ITextDecorateNode* TextDecorator::ParseText(const core::UTFString&text)
{
	const utf32*ptr=text.c_str();

	TextDecorateDummyNode* root=new TextDecorateDummyNode(mT("Root"));

	_ParserText(ptr,root);
	return root;
}

}
}
