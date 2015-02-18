
/********************************************************************
	created:	2008/11/21
	created:	21:11:2008   15:19
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\CommonProperties.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	CommonProperties
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/
/*
#ifndef ___CommonProperties___
#define ___CommonProperties___

#include "CPropertie.h"

namespace mray{
namespace video{
	class SColor;
}

class intProp:rwProperty<int>
{
public:
	intProp(int value,const mchar*name=mT(""),const  mchar*info=mT(""));
};
class floatProp:rwProperty<float>
{
public:
	floatProp(float value,const mchar*name=mT(""),const  mchar*info=mT(""));
};
class charProp:rwProperty<char>
{
public:
	charProp(char value,const mchar*name=mT(""),const  mchar*info=mT(""));
};
class wcharProp:rwProperty<wchar_t>
{
public:
	wcharProp(wchar_t value,const mchar*name=mT(""),const  mchar*info=mT(""));
};
class boolProp:rwProperty<bool>
{
public:
	boolProp(bool value,const mchar*name=mT(""),const  mchar*info=mT(""));
};

class colorProp:rwProperty<video::SColor>
{
public:
	colorProp(video::SColor value,const mchar*name=mT(""),const  mchar*info=mT(""));
};

class stringProp:rwProperty<core::string>
{
public:
	stringProp(core::string value,const mchar*name=mT(""),const  mchar*info=mT(""));
};



#ifdef UNICODE
	typedef wcharProp mcharProp;
#else
	typedef charProp mcharProp;
#endif

}


#endif //___CommonProperties___


*/