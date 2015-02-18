
#include "stdafx.h"
#include "CommunicationData.h"
#include "StreamWriter.h"
#include "StreamReader.h"


namespace mray
{
namespace VT
{
	
	void CommunicationDataStreamer::WriteData(CommunicationData& d,OS::IStream* stream)
	{
		const std::list<CommunicationData::DataPair>& data=d.GetData();
		OS::StreamWriter wrtr(stream);
		wrtr.binWriteString(d.GetTargetName());
		wrtr.binWriteInt(data.size());

		std::list<CommunicationData::DataPair>::const_iterator it=data.begin();
		for (;it!=data.end();++it)
		{
			const CommunicationData::DataPair& dp=*it;
			wrtr.binWriteString(dp.name);
			wrtr.binWriteInt(dp.values.size());
			for(int i=0;i<dp.values.size();++i)
			{
				wrtr.binWriteFloat(dp.values[i].KinValue);
				wrtr.binWriteFloat(dp.values[i].RealtimeValue);
			}
		}

	}
	void CommunicationDataStreamer::ReadData(CommunicationData&ret,OS::IStream* stream)
	{
		OS::StreamReader rdr(stream);
		ret.ClearData();
		ret.SetTargetName(rdr.binReadStringC());
		int cnt=rdr.binReadInt();
		ControlInputValues v;
		for(int i=0;i<cnt;++i)
		{
			CommunicationData::DataPair dp;
			
			dp.name= rdr.binReadStringC();
			int vc=rdr.binReadInt();

			
			for(int j=0;j<vc;++j)
			{
				v.KinValue=(rdr.binReadFloat());
				v.RealtimeValue=(rdr.binReadFloat());

				dp.values.push_back(v);
			}
			ret.AddData(dp.name,&dp.values[0],dp.values.size());
		}
	}
}
}


