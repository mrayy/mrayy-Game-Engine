


/********************************************************************
	created:	2011/05/05
	created:	5:5:2011   9:00
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\FreeImageCodec.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	FreeImageCodec
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __FreeImageCodec__
#define __FreeImageCodec__


namespace mray
{
namespace loaders
{

class FreeImageCodec
{
private:
protected:
	static int s_FreeImageRefCtr;
public:

	static void IncreaseRef();
	static void DecreaseRef();
	static int GetRefCount(){return s_FreeImageRefCtr;}

};


}
}

#endif
