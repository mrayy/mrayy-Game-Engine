

/********************************************************************
	created:	2012/07/14
	created:	14:7:2012   23:15
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\CommunicationData.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar
	file base:	CommunicationData
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___CommunicationData___
#define ___CommunicationData___


namespace mray
{
	namespace OS
	{
		class IStream;
	}
namespace VT
{


	struct ControlInputValues
	{
		ControlInputValues():KinValue(0),RealtimeValue(0)
		{}
		ControlInputValues(float kin,float rt):KinValue(kin),RealtimeValue(rt)
		{
		}
		float KinValue;
		float RealtimeValue;
	};
typedef std::vector<ControlInputValues> CommValueList;

class CommunicationData
{
public:
	struct DataPair
	{
		DataPair(){}
		DataPair(const core::string& target,const ControlInputValues* d,int n):name(target)
		{
			for(int i=0;i<n;++i)
				values.push_back(d[i]);
		}
		core::string name;
		CommValueList values;
	};
protected:

	core::string m_name;
	std::list<DataPair> m_data;
	
public:
	CommunicationData(){}
	virtual~CommunicationData(){}
	void SetTargetName(const core::string& name){m_name=name;}
	const core::string&  GetTargetName()const{return m_name;}

	//src: channel name
	//values: data
	void AddData(const core::string& src,const ControlInputValues* values,int n)
	{
		m_data.push_back(DataPair(src,values,n));
	}
	const std::list<DataPair>& GetData()const
	{
		return m_data;
	}
	void ClearData()
	{
		return m_data.clear();
	}
	
};

class CommunicationDataStreamer
{
public:

	static void WriteData(CommunicationData& d,OS::IStream* stream);
	static void ReadData(CommunicationData&ret,OS::IStream* stream);
};


}
}

#endif
