

#ifndef ___IFONT___
#define ___IFONT___



#include "SColor.h"
#include "IResource.h"
#include "UTFString.h"
//#include <ctype.h>
#include "GCPtr.h"


namespace mray{
namespace GUI{
	class FontAttributes;
	class IGUIRenderer;
	class LanguageFontSpecification;

class MRAY_DLL IFont:public IResource
{
protected:
	core::string m_name;
	virtual uint calcSizeInternal()=0;
public:
	IFont(const core::string &name){
		m_name=name;
	}
	virtual ~IFont(){};


	const core::string&getName()const{return m_name;}

//	virtual void print(math::rectf pos,int fontSize,bool wrap,video::SColor& color,math::rectf*clip,bool center,const mchar*fmt,...)=0;
//	virtual void print(math::rectf pos,int fontSize,bool wrap,video::SColor& color,math::rectf*clip,bool center,bool rtl,const  mchar*fmt,...)=0;
	virtual void print(const math::rectf& pos,FontAttributes*attributs,const math::rectf*clip,const core::UTFString& txt,IGUIRenderer*renderer)=0;

	virtual int getCharFromPos(const math::vector2d& p,FontAttributes*attributs,const math::rectf &rc,const core::UTFString& txt)=0;
	virtual math::Point2df getCharPos(int len,FontAttributes*attributs,const math::rectf &rc,const core::UTFString& txt)=0;
	virtual math::Point2df getTextDimension(const  core::UTFString&txt,FontAttributes*attributs,const math::rectf *rc)=0;
	//	virtual math::Point2df getDimension(const char *txt,int size);
	virtual math::Point2df getCharDimension( uint c,int size)=0;

};

MakeSharedPtrType(IFont);


};//GUI
};//mray

#endif






