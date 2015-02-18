#include "stdafx.h"


#include "EGenericTypes.h"


namespace mray
{

	core::string EGenericTypesStrTable[]={
		/*
		mT("char"),
		mT("wchar"),*/
		mT("int"),
		mT("float"),
		mT("bool"),
		mT("color"),
		mT("string"),
		mT("string"),
		mT("vector2d"),
		mT("vector3d"),
		mT("line2d"),
		mT("line3d"),
		mT("box3d"),
		mT("matrix"),
		mT("plane"),
		mT("quaternion"),
		mT("rect"),/*
		mT("arrayi"),
		mT("arrayf"),*/
		mT("other"),
		mT("unkown"),
		mT("")
	};

	EGenericType EGenericTypesUtil::convertTypeStr(const core::string&type){
		core::string typeStr(type);
		for(int i=0;i<EPT_COUNT;++i){
			if(typeStr==EGenericTypesStrTable[i]){
				return (EGenericType)i;
			}
		}
		return EPT_UNKOWN;
	}

	const core::string& EGenericTypesUtil::convertType(EGenericType type){
		return EGenericTypesStrTable[(int)type];
	}
}