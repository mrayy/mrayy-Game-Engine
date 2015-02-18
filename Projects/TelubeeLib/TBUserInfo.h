

/********************************************************************
	created:	2013/01/29
	created:	29:1:2013   12:42
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\TBUserInfo.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	TBUserInfo
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __TBUserInfo__
#define __TBUserInfo__




namespace mray
{
namespace TBee
{

class TBUserInfo
{
protected:
public:
	TBUserInfo(){}
	virtual~TBUserInfo(){}

	long ID;
	core::string name;
	core::string email;

};

}
}


#endif

