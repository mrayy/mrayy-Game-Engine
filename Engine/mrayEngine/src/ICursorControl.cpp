#include "stdafx.h"

#include "ICursorControl.h"

namespace mray{
	const mchar* CursorStr[]={
		mT("Arrow"),
		mT("Cross"),
		mT("IBeam"),
		mT("No"),
		mT("SizeAll"),
		mT("SizeNESW"),
		mT("SizeNS"),
		mT("SizeNWSE"),
		mT("SizeWE"),
		mT("UpArrow"),
		mT("WaitArrow"),
		mT("HSplit"),
		mT("VSplit"),
		mT("Hand")
	};
	ICursorControl::ICursorControl(){
	}
	ICursorControl::~ICursorControl(){
	}
	const mchar*ICursorControl::getCursorStr(ECursor c){

		return CursorStr[(int)c];
	}
}
