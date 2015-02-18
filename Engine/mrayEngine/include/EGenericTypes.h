


/********************************************************************
	created:	2011/01/21
	created:	21:1:2011   14:55
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\EGenericTypes.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	EGenericTypes
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef EGenericTypes_h__
#define EGenericTypes_h__

#include "mString.h"

namespace mray
{

enum EGenericType
{
	/*EPT_CHAR,		//char
	EPT_WCHAR,		//wchar*/
	EPT_INT,		//int
	EPT_FLOAT,		//float
	EPT_BOOLEAN,	//bool
	EPT_COLOR,		//color
	EPT_STRING,		//string
	EPT_VECTOR2D,	//vector2di
	EPT_VECTOR3D,	//vector3df
	EPT_LINE2D,		//line2d
	EPT_LINE3D,		//line3d
	EPT_BOX3D,		//box3d
	EPT_MATRIX,		//matrix
	EPT_PLANE,		//plane
	EPT_QUATERNION,	//quaternion
	EPT_RECT,		//rectf
	EPT_ITEM_LIST,
	/*EPT_ARRAYi,		//---
	EPT_ARRAYf,		//---*/
	EPT_ENUM,		//---
	EPT_OTHER,		//---
	EPT_UNKOWN,		//---
	EPT_COUNT		//---
};

class MRAY_DLL EGenericTypesUtil
{
public:
	static EGenericType convertTypeStr(const core::string&type);
	static const core::string& convertType(EGenericType type);
};

}
#endif // EGenericTypes_h__
