/********************************************************************
	created:	2013/10/28
	created:	28:10:2013   23:03
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\GUIOculusDetectImpl.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	GUIOculusDetectImpl
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GUIOculusDetectImpl__
#define __GUIOculusDetectImpl__

#include "GUIOculusDetect.h"



namespace mray
{


class GUIOculusDetectImpl:public GUIOculusDetect
{
public:
	enum EState
	{
		EFound,
		EFailedToConnect
	};
protected:
	EState m_state;
public:

	

	GUIOculusDetectImpl(){}
	virtual~GUIOculusDetectImpl(){}

	void SetState(GUIOculusDetectImpl::EState s);
	GUIOculusDetectImpl::EState GetState(){return m_state;}

	virtual void DoneLoadingElements();

};

}


#endif