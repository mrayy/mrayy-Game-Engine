


/********************************************************************
	created:	2011/10/19
	created:	19:10:2011   15:06
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IAsyncLoadRequest.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IAsyncLoadRequest
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IAsyncLoadRequest__
#define __IAsyncLoadRequest__


namespace mray
{

class IAsyncLoadRequest
{
private:
protected:
	bool m_loaded;
public:
	IAsyncLoadRequest():m_loaded(false)
	{}
	virtual~IAsyncLoadRequest(){}

	virtual bool StartLoading()=0;
	virtual void OnFailedLoading()=0;
	bool IsLoaded(){return m_loaded;}

};


}

#endif