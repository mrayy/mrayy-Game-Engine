

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

	struct JointParam
	{
		core::string name;
		float r[5];
		float minLim,maxLim;
		float lastV;
		void Clamp(){lastV=math::clamp(lastV,minLim,maxLim);}
	};
	JointParam g_JointPrams[]=
	{
		{"JointRF_1_1",-90*2,0,0,0,0,	-90,0,0},
		{"JointRF_1_2",-80*2,0,0,0,0,	-80,0,0},
		{"JointRF_1_3",90,0,0,0,0,		0,10,0},
		{"JointRF_1_4",45,0,0,0,0,		0,21,0},
		{"JointRF_1_5",-20,0,0,0,0,		-8,0,0},

		{"JointRF_2_3",0,90,0,0,0,		0,55,0},
		{"JointRF_2_4",0,40,0,0,0,		0,6,0},

		{"JointRF_3_3",0,0,90,0,0,		0,60,0},
		{"JointRF_3_4",0,0,-10,0,0,		-11,0,0},

	//	{"JointRF_4_1",0,50,50,50,50, 0,8,0},
		{"JointRF_4_3",0,0,0,90,0,		0,60,0},
		{"JointRF_4_4",0,0,0,-40,0,		-6,0,0},

		{"JointRF_5_3",0,0,0,0,120,		0,60,0},
		{"JointRF_5_4",0,0,0,0,40,		0,13,0}
	};
	FingerIndex g_FingerMap[]={Thumb,Index,Middle,Ring,Little};

	int g_JointCount=sizeof(g_JointPrams)/sizeof(JointParam);
GloveController::GloveController()
{
	m_glove=0;
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
bool GloveController::Open()
{
	char szPort[128];

	unsigned short aPID[5];
	int nNumFound = 5;
	m_glove=0;
	fdScanUSB(aPID,nNumFound);
	if(nNumFound>0)
	{
		sprintf(szPort,"USB%i",0);
		fdGlove*g= fdOpen(szPort);
		if(g)
		{
			printf("Glove found: %s\n", szPort);
			m_glove=g;
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

	char szPort[128];

	unsigned short aPID[5];
	int nNumFound = 5;
	fdScanUSB(aPID,nNumFound);
	if(nNumFound>0)
	{
		sprintf(szPort,"USB%i",0);
		fdGlove*g= fdOpen(szPort);
		if(g)
		{
			printf("Glove found: %s\n", szPort);
			m_glove=g;
		}
	}

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
	{
		g_JointPrams[i].lastV=0;
		for(int j=0;j<5;++j)
		{
			if(g_JointPrams[i].r[j]!=0)
				g_JointPrams[i].lastV+=math::clamp<float>(fabs(g_fingersVals[j].GetValue()-0.1)/0.9f,0,1)*g_JointPrams[i].r[j];
		}
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

