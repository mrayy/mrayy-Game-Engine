

/********************************************************************
	created:	2010/09/01
	created:	1:9:2010   22:23
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\TextDecorator.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	TextDecorator
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	decorate a text using xml like tags
				example:
					"<color='255,0,0'><Italic>Hello world</Italic></color>"
					will produce "Hello world" with red color and italic style
				tags can be extended by adding more complex tags,like hyperlinks
*********************************************************************/

#ifndef TextDecorator_h__
#define TextDecorator_h__

#include "mString.h"
#include "ITextDecorateNode.h"
#include "CompileConfig.h"

namespace mray
{
namespace GUI
{

class MRAY_DLL TextDecorator
{
private:
protected:

	enum EPredictionResult
	{
		EOpenTag,
		ECloseTag,
		ENormalText
	};

	EPredictionResult GetPrediction(const utf32*ptr);

	const utf32* _ParserText(const utf32*ptr, ITextDecorateNode* parent);
public:
	TextDecorator();
	virtual ~TextDecorator();

	ITextDecorateNode* ParseText(const core::UTFString&text);
};

}
}

#endif // TextDecorator_h__
