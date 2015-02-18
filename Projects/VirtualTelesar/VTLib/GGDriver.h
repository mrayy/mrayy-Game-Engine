

/********************************************************************
	created:	2013/02/26
	created:	26:2:2013   17:17
	filename: 	D:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\GGDriver.h
	file path:	D:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	GGDriver
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	Gravity Grabber Communicator Class
*********************************************************************/
#ifndef _GGDriver_
#define _GGDriver_

#include "IForceFeedbackDriver.h"
#include "IFeedbackControl.h"


namespace mray
{
namespace VT
{


	class GGDriverImpl;


	class GGFeedbackValue:public IFeedbackResult
	{
	public:
		static const int ChannelsCount=18;
		float channels[ChannelsCount];


		GGFeedbackValue()
		{
			for (int i = 0; i<ChannelsCount; ++i)
				channels[i]=0;
		}
		virtual void* GetData()
		{
			return channels;
		}
		virtual core::string ToString()
		{
			core::string ret;
			for (int i = 0; i<ChannelsCount; ++i)
			{
				ret+=core::StringConverter::toString(channels[i])+" ";
			}
			return ret;
		}

	};

class GGDriver:public IForceFeedbackDriver
{
protected:
	GGDriverImpl* m_impl;
public:
	GGDriver();
	virtual~GGDriver();

	virtual void Init(const core::string& ip,int port);
	virtual void Close();
	void SendData(IFeedbackResult* res);

};


}
}

#endif