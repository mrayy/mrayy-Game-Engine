

#include "stdafx.h"
#include "GloveController5.h"




namespace mray
{
namespace AugTel
{

	class AverageWindow
	{
	protected:
		int m_count;
		std::vector<float> m_vals;
		int m_index;
		float m_lastValue;
	public:

		AverageWindow(int cnt=30)
		{
			m_lastValue=0;
			m_index=0;
			m_count=cnt;
		}

		float AddValue(float v,float dt)
		{
			dt=math::clamp<float>(dt,1.0f/120.0f,1.0);
			if(m_vals.size()<m_count)
			{
				m_vals.push_back(v);
			}else
			{
				m_vals[m_index]=v;
				m_index=(m_index+1)%m_count;
			}
			float sum=0;
			int maxSamples=math::clamp<int>(m_count*(1-dt),1,m_vals.size());
			for(int i=0;i<maxSamples;++i)
				sum+=m_vals[i];
			return (m_lastValue=sum/(float)maxSamples);
		}
		float GetValue()
		{
			return m_lastValue;
		}
	};

	struct JointSegment
	{
		JointSegment()
		{
		}
		JointSegment(float t,float r)
		{
			this->t=t;
			this->r=r;
		}
		float t;
		float r;
	};
	struct JointParam
	{
		JointParam()
		{
			lastV=0;
			segments.push_back(JointSegment(0,0));
		}

		void Init(int f, const core::string&n, int minL, int maxL)
		{
			finger = f;
			name = n;
			minLim = minL;
			maxLim = maxL;
		}
		int finger;
		core::string name;
		float minLim,maxLim;
		float lastV;
		std::vector<JointSegment> segments;
		void Clamp(){lastV=math::clamp(lastV,minLim,maxLim);}

		void calcValue(float t)
		{
			t=math::clamp(t,0.0f,1.0f);
			int k=1;
			for(int i=1;i<segments.size();++i)
			{
				if(t>segments[i-1].t && t<segments[i].t)
				{
					k=i;
					break;
				}
			}
			float m=(t-segments[k-1].t)/(segments[k].t-segments[k-1].t);
			lastV=math::lerp(segments[k-1].r,segments[k].r,m);
		}
	};
	FingerIndex g_FingerMap[]={Thumb,Index,Middle,Ring,Little};


	class GloveController5Impl
	{
	protected:
		AverageWindow m_fingersVals[5];
#define s_jcount  (5*4)
		JointParam m_JointPrams[s_jcount];
		JointParam* fingersPtr[5][4];
		fdGlove* m_glove;
		int m_hand;
	public:
		GloveController5Impl::GloveController5Impl(const std::string&pref)
		{
			m_glove = 0;
			memset(fingersPtr, 0, sizeof(fingersPtr));
			int i = 0;
			m_JointPrams[i].Init(4, pref + "_1_1", 0, 90);
			m_JointPrams[i].segments.push_back(JointSegment(0.6, 70));
			m_JointPrams[i].segments.push_back(JointSegment(1, 90));
			fingersPtr[0][0] = &m_JointPrams[i];
			++i;
			m_JointPrams[i].Init(4, pref + "_1_2", 0, 90);
			//m_JointPrams[i].segments[0].r = -50;
			m_JointPrams[i].segments.push_back(JointSegment(0.6, -90));
			m_JointPrams[i].segments.push_back(JointSegment(1, -90));
			fingersPtr[0][1] = &m_JointPrams[i];
			++i;
			m_JointPrams[i].Init(4, pref + "_1_3", 0, 90);
			m_JointPrams[i].segments.push_back(JointSegment(0.7, 20));
			m_JointPrams[i].segments.push_back(JointSegment(1, 30));
			fingersPtr[0][2] = &m_JointPrams[i];
			++i;
			m_JointPrams[i].Init(4, pref + "_1_4", 0, 30);
			m_JointPrams[i].segments.push_back(JointSegment(0.7, 20));
			m_JointPrams[i].segments.push_back(JointSegment(1, 30));
			fingersPtr[0][3] = &m_JointPrams[i];
			++i;


			m_JointPrams[i].Init(3, pref + "_2_1", 0, 90);
			m_JointPrams[i].segments.push_back(JointSegment(0.6, 70));
			m_JointPrams[i].segments.push_back(JointSegment(1, 90));
			fingersPtr[1][0] = &m_JointPrams[i];
			++i;
			m_JointPrams[i].Init(3, pref + "_2_2", 0, 90);
			m_JointPrams[i].segments.push_back(JointSegment(0.6, 70));
			m_JointPrams[i].segments.push_back(JointSegment(1, 90));
			fingersPtr[1][1] = &m_JointPrams[i];
			++i;
			m_JointPrams[i].Init(3, pref + "_2_3", 0, 90);
			m_JointPrams[i].segments.push_back(JointSegment(0.6, 70));
			m_JointPrams[i].segments.push_back(JointSegment(1, 90));
			fingersPtr[1][2] = &m_JointPrams[i];
			++i;
			m_JointPrams[i].Init(3, pref + "_2_4", 0, 25);
			m_JointPrams[i].segments.push_back(JointSegment(0.5, 15));
			m_JointPrams[i].segments.push_back(JointSegment(1, 25));
			fingersPtr[1][3] = &m_JointPrams[i];
			++i;


			m_JointPrams[i].Init(2, pref + "_3_1", 0, 90);
			m_JointPrams[i].segments.push_back(JointSegment(0.6, 70));
			m_JointPrams[i].segments.push_back(JointSegment(1, 90));
			fingersPtr[2][0] = &m_JointPrams[i];
			++i;
			m_JointPrams[i].Init(2, pref + "_3_2", 0, 90);
			m_JointPrams[i].segments.push_back(JointSegment(0.6, 70));
			m_JointPrams[i].segments.push_back(JointSegment(1, 90));
			fingersPtr[2][1] = &m_JointPrams[i];
			++i;
			m_JointPrams[i].Init(2, pref + "_3_3", 0, 90);
			m_JointPrams[i].segments.push_back(JointSegment(0.6, 70));
			m_JointPrams[i].segments.push_back(JointSegment(1, 90));
			fingersPtr[2][2] = &m_JointPrams[i];
			++i;
			m_JointPrams[i].Init(2, pref + "_3_4", 0, 40);
			m_JointPrams[i].segments.push_back(JointSegment(0.5, 15));
			m_JointPrams[i].segments.push_back(JointSegment(1, 25));
			fingersPtr[2][3] = &m_JointPrams[i];
			++i;

			m_JointPrams[i].Init(1, pref + "_4_1", 0, 90);
			m_JointPrams[i].segments.push_back(JointSegment(0.6, 70));
			m_JointPrams[i].segments.push_back(JointSegment(1, 90));
			fingersPtr[3][0] = &m_JointPrams[i];
			++i;
			m_JointPrams[i].Init(1, pref + "_4_2", 0, 90);
			m_JointPrams[i].segments.push_back(JointSegment(0.6, 70));
			m_JointPrams[i].segments.push_back(JointSegment(1, 90));
			fingersPtr[3][1] = &m_JointPrams[i];
			++i;
			m_JointPrams[i].Init(1, pref + "_4_3", 0, 90);
			m_JointPrams[i].segments.push_back(JointSegment(0.6, 70));
			m_JointPrams[i].segments.push_back(JointSegment(1, 90));
			fingersPtr[3][2] = &m_JointPrams[i];
			++i;
			m_JointPrams[i].Init(1, pref + "_4_4", 0, 40);
			m_JointPrams[i].segments.push_back(JointSegment(0.5, 15));
			m_JointPrams[i].segments.push_back(JointSegment(1, 25));
			fingersPtr[3][3] = &m_JointPrams[i];
			++i;

			m_JointPrams[i].Init(0, pref + "_5_1", 0, 90);
			m_JointPrams[i].segments.push_back(JointSegment(0.6, 70));
			m_JointPrams[i].segments.push_back(JointSegment(1, 90));
			fingersPtr[4][0] = &m_JointPrams[i];
			++i;
			m_JointPrams[i].Init(0, pref + "_5_2", 0, 90);
			m_JointPrams[i].segments.push_back(JointSegment(0.6, 70));
			m_JointPrams[i].segments.push_back(JointSegment(1, 90));
			fingersPtr[4][1] = &m_JointPrams[i];
			++i;
			m_JointPrams[i].Init(0, pref + "_5_3", 0, 90);
			m_JointPrams[i].segments.push_back(JointSegment(0.6, 70));
			m_JointPrams[i].segments.push_back(JointSegment(1, 90));
			fingersPtr[4][2] = &m_JointPrams[i];
			++i;
			m_JointPrams[i].Init(0, pref + "_5_4", 0, 40);
			m_JointPrams[i].segments.push_back(JointSegment(0.5, 15));
			m_JointPrams[i].segments.push_back(JointSegment(1, 25));
			fingersPtr[4][3] = &m_JointPrams[i];
			++i;

		}

		bool SetHand(int hand)
		{

			m_hand = hand;
			char szPort[128];

			unsigned short aPID[5];
			int nNumFound = 5;
			m_glove = 0;
			fdScanUSB(aPID, nNumFound);
			if (nNumFound > 0)
			{
				for (int i = 0; i < nNumFound; ++i)
				{
					sprintf_s(szPort, "USB%i", i);
					fdGlove*g = fdOpen(szPort);
					if (g == 0)
						continue;
					if (fdGetGloveHand(g) == hand)
					{
						printf("Glove found: %s\n", szPort);
						m_glove = g;
						break;
					}
					else
						fdClose(g);
				}
			}

			if (m_glove == 0)
			{
				printf("Couldn't find any attached gloves!\n");
				return false;
			}

			return true;
		}
		bool Start()
		{
			return SetHand(m_hand);
		}
		void Stop()
		{
			if (!m_glove)
				return;
			fdClose(m_glove);
			m_glove = 0;
		}
		void Calibrate()
		{
			if (!m_glove)
				return;

			fdResetCalibrationAll(m_glove);
		}
		void Rescan()
		{

			Stop();
			SetHand(m_hand);
		}
		float GetFingerValue(FingerIndex index)
		{

			if (!m_glove)
				return 0;
			return fdGetSensorScaled(m_glove, index);
		}
		virtual float GetFingerValue(int finger, int part)
		{
			if (!fingersPtr[finger][part])
				return 0;
			return fingersPtr[finger][part]->lastV;
		}

		void Update(float dt)
		{
			if (!m_glove)
				return;
			float vals[5];
			for (int j = 0; j<5; ++j)
			{
				vals[j] = GetFingerValue(g_FingerMap[j]);
				m_fingersVals[j].AddValue(vals[j], dt);
			}
			for (int i = 0; i<s_jcount; ++i)
			{
				m_JointPrams[i].calcValue(m_fingersVals[m_JointPrams[i].finger].GetValue());
				m_JointPrams[i].Clamp();
			}
		}
		virtual void SendMessage(const core::string& msg, const core::string& value)
		{
			if (msg == "calibrate")
				Calibrate();
			else if (msg=="rescan")
			{
				Rescan();
			}
		}

	};
	GloveController5::GloveController5(const std::string&pref)
	{
		m_impl = new GloveController5Impl(pref);
	}
GloveController5::~GloveController5()
{
	delete m_impl;
}
bool GloveController5::Start()
{
	return true;
}
bool GloveController5::SetHand(int hand)
{
	return m_impl->SetHand(hand);

}
void GloveController5::Stop()
{
	m_impl->Stop();
}
void GloveController5::Calibrate()
{
	m_impl->Calibrate();
}
void GloveController5::Rescan()
{
	m_impl->Rescan();

}


void GloveController5::Update(float dt)
{
	m_impl->Update(dt);
}
float GloveController5::GetFingerValue(FingerIndex index)
{
	return m_impl->GetFingerValue(index);

}
float GloveController5::GetFingerValue(int finger, int part)
{
	return m_impl->GetFingerValue(finger, part);
}
void GloveController5::SendMessage(const core::string& msg, const core::string& value)
{
	m_impl->SendMessage(msg, value);

}

}
}
