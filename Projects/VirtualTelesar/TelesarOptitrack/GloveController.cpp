

#include "stdafx.h"
#include "GloveController.h"
#include "Communicator.h"




namespace mray
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
	}g_fingersVals[5];

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
	JointParam g_JointPrams[5+2+2+2+2];
	/*
	JointParam g_JointPrams[]=
	{
		{pref+"_1_1",-90*2,0,0,0,0,	-90,0,0},
		{pref+"_1_2",-80*2,0,0,0,0,	-80,0,0},
		{pref+"_1_3",90,0,0,0,0,		0,10,0},
		{pref+"_1_4",45,0,0,0,0,		0,21,0},
		{pref+"_1_5",-20,0,0,0,0,		-8,0,0},

		{pref+"_2_3",0,90,0,0,0,		0,55,0},
		{pref+"_2_4",0,40,0,0,0,		0,6,0},

		{pref+"_3_3",0,0,90,0,0,		0,60,0},
		{pref+"_3_4",0,0,-10,0,0,		-11,0,0},

	//	{pref+"_4_1",0,50,50,50,50, 0,8,0},
		{pref+"_4_3",0,0,0,90,0,		0,60,0},
		{pref+"_4_4",0,0,0,-40,0,		-6,0,0},

		{pref+"_5_3",0,0,0,0,120,		0,60,0},
		{pref+"_5_4",0,0,0,0,40,		0,13,0}
	};*/
	FingerIndex g_FingerMap[]={Thumb,Index,Middle,Ring,Little};

	int g_JointCount=sizeof(g_JointPrams)/sizeof(JointParam);
GloveController::GloveController(const std::string&pref)
{
	m_glove=0;

	int i=0;
	g_JointPrams[i].name=pref+"_1_1";
	g_JointPrams[i].finger=0;
	g_JointPrams[i].minLim=-90;
	g_JointPrams[i].maxLim=0;
	g_JointPrams[i].segments.push_back(JointSegment(0.6,-90));
	g_JointPrams[i].segments.push_back(JointSegment(1,-90));
	++i;
	g_JointPrams[i].name=pref+"_1_2";
	g_JointPrams[i].finger=0;
	g_JointPrams[i].minLim=-90;
	g_JointPrams[i].maxLim=0;
	g_JointPrams[i].segments[0].r=-50;
	g_JointPrams[i].segments.push_back(JointSegment(0.4,-90));
	g_JointPrams[i].segments.push_back(JointSegment(0.8,-80));
	g_JointPrams[i].segments.push_back(JointSegment(1,-60));
	++i;
	g_JointPrams[i].name=pref+"_1_3";
	g_JointPrams[i].finger=0;
	g_JointPrams[i].minLim=0;
	g_JointPrams[i].maxLim=30;
	g_JointPrams[i].segments.push_back(JointSegment(0.7,20));
	g_JointPrams[i].segments.push_back(JointSegment(1,30));
	++i;
	g_JointPrams[i].name=pref+"_1_4";
	g_JointPrams[i].finger=0;
	g_JointPrams[i].minLim=0;
	g_JointPrams[i].maxLim=80;
	g_JointPrams[i].segments.push_back(JointSegment(0.5,40));
	g_JointPrams[i].segments.push_back(JointSegment(0.7,60));
	g_JointPrams[i].segments.push_back(JointSegment(1,80));
	++i;
	g_JointPrams[i].name=pref+"_1_5";
	g_JointPrams[i].finger=0;
	g_JointPrams[i].minLim=0;
	g_JointPrams[i].maxLim=30;
	g_JointPrams[i].segments.push_back(JointSegment(0.5,20));
	g_JointPrams[i].segments.push_back(JointSegment(1,30));
	++i;

	g_JointPrams[i].name=pref+"_2_3";
	g_JointPrams[i].finger=1;
	g_JointPrams[i].minLim=0;
	g_JointPrams[i].maxLim=90;
	g_JointPrams[i].segments.push_back(JointSegment(0.6,70));
	g_JointPrams[i].segments.push_back(JointSegment(1,90));
	++i;
	g_JointPrams[i].name=pref+"_2_4";
	g_JointPrams[i].finger=1;
	g_JointPrams[i].minLim=0;
	g_JointPrams[i].maxLim=25;
	g_JointPrams[i].segments.push_back(JointSegment(0.5,15));
	g_JointPrams[i].segments.push_back(JointSegment(1,25));
	++i;

	g_JointPrams[i].name=pref+"_3_3";
	g_JointPrams[i].finger=2;
	g_JointPrams[i].minLim=0;
	g_JointPrams[i].maxLim=90;
	g_JointPrams[i].segments.push_back(JointSegment(0.6,70));
	g_JointPrams[i].segments.push_back(JointSegment(1,90));
	++i;
	g_JointPrams[i].name=pref+"_3_4";
	g_JointPrams[i].finger=2;
	g_JointPrams[i].minLim=0;
	g_JointPrams[i].maxLim=40;
	g_JointPrams[i].segments.push_back(JointSegment(0.5,15));
	g_JointPrams[i].segments.push_back(JointSegment(1,25));
	++i;

	g_JointPrams[i].name=pref+"_4_3";
	g_JointPrams[i].finger=3;
	g_JointPrams[i].minLim=0;
	g_JointPrams[i].maxLim=90;
	g_JointPrams[i].segments.push_back(JointSegment(0.6,70));
	g_JointPrams[i].segments.push_back(JointSegment(1,90));
	++i;
	g_JointPrams[i].name=pref+"_4_4";
	g_JointPrams[i].finger=3;
	g_JointPrams[i].minLim=0;
	g_JointPrams[i].maxLim=40;
	g_JointPrams[i].segments.push_back(JointSegment(0.5,15));
	g_JointPrams[i].segments.push_back(JointSegment(1,25));
	++i;

	g_JointPrams[i].name=pref+"_5_3";
	g_JointPrams[i].finger=4;
	g_JointPrams[i].minLim=0;
	g_JointPrams[i].maxLim=90;
	g_JointPrams[i].segments.push_back(JointSegment(0.6,70));
	g_JointPrams[i].segments.push_back(JointSegment(1,90));
	++i;
	g_JointPrams[i].name=pref+"_5_4";
	g_JointPrams[i].finger=4;
	g_JointPrams[i].minLim=0;
	g_JointPrams[i].maxLim=40;
	g_JointPrams[i].segments.push_back(JointSegment(0.5,15));
	g_JointPrams[i].segments.push_back(JointSegment(1,25));
	++i;


}
GloveController::~GloveController()
{
	Close();
}
void GloveController::Init()
{
	for(int i=0;i<g_JointCount;++i)
		Communicator::Instance()->AddSchemeItem(g_JointPrams[i].name);
}
bool GloveController::Open(int hand)
{
	m_hand=hand;
	char szPort[128];

	unsigned short aPID[5];
	int nNumFound = 5;
	m_glove=0;
	fdScanUSB(aPID,nNumFound);
	if(nNumFound>0)
	{
		for(int i=0;i<nNumFound;++i)
		{
			sprintf(szPort,"USB%i",i);
			fdGlove*g= fdOpen(szPort);
			if(g==0)
				continue;
			if(fdGetGloveHand(g)==hand)
			{
				printf("Glove found: %s\n", szPort);
				m_glove=g;
				break;
			}else
				fdClose(g);
		}
	}

	if(m_glove==0)
	{
		printf("Couldn't find any attached gloves!\n");
		return false;
	}

	return true;

}
void GloveController::Close()
{
	if(!m_glove)
		return;
	fdClose(m_glove);
	m_glove=0;
}
void GloveController::Calibrate()
{
	if(!m_glove)
		return;

	fdResetCalibrationAll(m_glove);
}
void GloveController::Rescan()
{
	Close();
	Open(m_hand);

}


void GloveController::Send()
{
	float vals[5];
	for(int j=0;j<5;++j)
	{
		vals[j]=GetFingerValue(g_FingerMap[j]);
		g_fingersVals[j].AddValue(vals[j],0.001f);
	}
	for(int i=0;i<g_JointCount;++i)
	{/*
		g_JointPrams[i].lastV=0;
		for(int j=0;j<5;++j)
		{
			if(g_JointPrams[i].r[j]!=0)
				g_JointPrams[i].lastV+=math::clamp<float>(fabs(g_fingersVals[j].GetValue()-0.1)/0.9f,0,1)*g_JointPrams[i].r[j];
		}*/
		g_JointPrams[i].calcValue(g_fingersVals[g_JointPrams[i].finger].GetValue());
		g_JointPrams[i].Clamp();
		Communicator::Instance()->AddSendValue(g_JointPrams[i].name,&g_JointPrams[i].lastV,1);
	}
}
float GloveController::GetFingerValue(FingerIndex index)
{
	if(!m_glove)
		return 0;
	return fdGetSensorScaled(m_glove,index);

}

}

