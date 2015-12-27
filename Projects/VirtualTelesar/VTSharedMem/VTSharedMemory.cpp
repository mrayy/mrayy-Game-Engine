#include "stdafx.h"
#include "shmem.h"
#include "VTSharedMemory.h"
#include "CommunicationData.h"
#include <windows.h>

//#include "StringConverter.h"

namespace mray
{
namespace VT
{

	class VTSharedMemoryImpl
	{

		class JointValueReader
		{
		public:
			JointValueReader()
			{
				m_srcValue[0]=m_srcValue[1]=0;
			}
			int id;
			float* m_srcValue[2];
			AverageWindow* m_targetValue;

			void update(float dt);
		};
		VTSharedMemoryData* m_data;
		core::string m_name;
		std::vector<AverageWindow*> m_values;
		std::vector<core::string> m_schemeNames;
		std::vector<JointValueReader> m_valueReader;
		VTSharedMemory* m_owner;
		bool m_calibrated;
		bool m_connected;
	public:

		VTSharedMemoryImpl(const core::string& name,VTSharedMemory* o);
		virtual~VTSharedMemoryImpl();

		void Start();
		virtual void Close();

		void Update(float dt);

		float GetTemperature(bool left,int index);
		float GetForce(bool left,int index);

		void GetDebugString(std::vector<core::string>& lst);

		const std::vector<core::string>& GetScheme();
		core::string InjectCommand(const core::string& cmd, const core::string& args);

	};

	class AverageWindow
	{
	protected:
		int m_count;
		std::vector<float> m_vals;
		int m_index;
		float m_lastValue;
	public:

		AverageWindow(int cnt)
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

void VTSharedMemoryImpl::JointValueReader::update(float dt)
{
	if(m_srcValue)
		m_targetValue->AddValue(*m_srcValue[0],dt);
}

class VTSharedMemoryData
{
public:
	shmem_joints_t *jointsData;
	shmem* agent;

	VTSharedMemoryData()
	{

		agent=new shmem();
		jointsData=new shmem_joints_t();
	}
	~VTSharedMemoryData()
	{
		delete agent;
		delete jointsData;
	}
};

VTSharedMemoryImpl::VTSharedMemoryImpl(const core::string& n,VTSharedMemory* o)
{
	m_owner=o;
	m_name=n;

	m_connected=false;
	m_calibrated=0;
	m_data=new VTSharedMemoryData();

	struct BodyInfo
	{
		core::string name;
		float* data[2];
	};

	static const int HandMapping[]=
	{
		0,
		1,
		2,
		3,
		4,

		6,
		7,

		9,
		10,

		11,
		12,
		13,

		14,
		15
	};
	BodyInfo infoList[]=
	{
		
		{"JointBody1",&m_data->jointsData->kin_body[0],&m_data->jointsData->rt_body[0]},
		{"JointBody2",&m_data->jointsData->kin_body[1],&m_data->jointsData->rt_body[1]},
		{"JointBody3",&m_data->jointsData->kin_body[2],&m_data->jointsData->rt_body[2]},
		{"JointBody4",&m_data->jointsData->kin_body[3],&m_data->jointsData->rt_body[3]},
		{"JointBody5",&m_data->jointsData->kin_body[4],&m_data->jointsData->rt_body[4]},
		{"JointBody6",&m_data->jointsData->kin_body[5],&m_data->jointsData->rt_body[5]},
		{"JointBody7",&m_data->jointsData->kin_body[6],&m_data->jointsData->rt_body[6]},
		{"JointBody8",&m_data->jointsData->kin_body[7],&m_data->jointsData->rt_body[7]},
		{"JointBody9",&m_data->jointsData->kin_body[8],&m_data->jointsData->rt_body[8]},

		{"JointLShoulderX"	,&m_data->jointsData->kin_arm.left[0],&m_data->jointsData->rt_arm.left[0]},
		{"JointLShoulderY"	,&m_data->jointsData->kin_arm.left[1],&m_data->jointsData->rt_arm.left[1]},
		{"JointLShoulderZ"	,&m_data->jointsData->kin_arm.left[2],&m_data->jointsData->rt_arm.left[2]},
		{"JointLArmX"		,&m_data->jointsData->kin_arm.left[3],&m_data->jointsData->rt_arm.left[3]},
		{"JointLArmZ"		,&m_data->jointsData->kin_arm.left[4],&m_data->jointsData->rt_arm.left[4]},
		{"JointLHandX"		,&m_data->jointsData->kin_arm.left[5],&m_data->jointsData->rt_arm.left[5]},
		{"JointLHandY"		,&m_data->jointsData->kin_arm.left[6],&m_data->jointsData->rt_arm.left[6]},

		{"JointRShoulderX"	,&m_data->jointsData->kin_arm.right[0],&m_data->jointsData->rt_arm.right[0]},
		{"JointRShoulderY"	,&m_data->jointsData->kin_arm.right[1],&m_data->jointsData->rt_arm.right[1]},
		{"JointRShoulderZ"	,&m_data->jointsData->kin_arm.right[2],&m_data->jointsData->rt_arm.right[2]},
		{"JointRArmX"		,&m_data->jointsData->kin_arm.right[3],&m_data->jointsData->rt_arm.right[3]},
		{"JointRArmZ"		,&m_data->jointsData->kin_arm.right[4],&m_data->jointsData->rt_arm.right[4]},
		{"JointRHandX"		,&m_data->jointsData->kin_arm.right[5],&m_data->jointsData->rt_arm.right[5]},
		{"JointRHandY"		,&m_data->jointsData->kin_arm.right[6],&m_data->jointsData->rt_arm.right[6]},

		{"JointLF_1_1"		,&m_data->jointsData->kin_hand.left[HandMapping[0]] ,&m_data->jointsData->rt_hand.left[HandMapping[0]] },
		{"JointLF_1_2"		,&m_data->jointsData->kin_hand.left[HandMapping[1]] ,&m_data->jointsData->rt_hand.left[HandMapping[1]] },
		{"JointLF_1_3"		,&m_data->jointsData->kin_hand.left[HandMapping[2]] ,&m_data->jointsData->rt_hand.left[HandMapping[2]] },
		{"JointLF_1_4"		,&m_data->jointsData->kin_hand.left[HandMapping[3]] ,&m_data->jointsData->rt_hand.left[HandMapping[3]] },
		{"JointLF_1_5"		,&m_data->jointsData->kin_hand.left[HandMapping[4]] ,&m_data->jointsData->rt_hand.left[HandMapping[4]] },
	
		{"JointLF_2_3"		,&m_data->jointsData->kin_hand.left[HandMapping[5]] ,&m_data->jointsData->rt_hand.left[HandMapping[5]] },
		{"JointLF_2_4"		,&m_data->jointsData->kin_hand.left[HandMapping[6]] ,&m_data->jointsData->rt_hand.left[HandMapping[6]] },
	
		{"JointLF_3_3"		,&m_data->jointsData->kin_hand.left[HandMapping[7]] ,&m_data->jointsData->rt_hand.left[HandMapping[7]] },
		{"JointLF_3_4"		,&m_data->jointsData->kin_hand.left[HandMapping[8]] ,&m_data->jointsData->rt_hand.left[HandMapping[8]] },
	
		{"JointLF_4_1"		,&m_data->jointsData->kin_hand.left[HandMapping[9]] ,&m_data->jointsData->rt_hand.left[HandMapping[9]] },
		{"JointLF_4_3"		,&m_data->jointsData->kin_hand.left[HandMapping[10]],&m_data->jointsData->rt_hand.left[HandMapping[10]]},
		{"JointLF_4_4"		,&m_data->jointsData->kin_hand.left[HandMapping[11]],&m_data->jointsData->rt_hand.left[HandMapping[11]]},
	
		{"JointLF_5_3"		,&m_data->jointsData->kin_hand.left[HandMapping[12]],&m_data->jointsData->rt_hand.left[HandMapping[12]]},//12
		{"JointLF_5_4"		,&m_data->jointsData->kin_hand.left[HandMapping[13]],&m_data->jointsData->rt_hand.left[HandMapping[13]]},//13
		 
		{"JointRF_1_1"		,&m_data->jointsData->kin_hand.right[HandMapping[0]] ,&m_data->jointsData->rt_hand.right[HandMapping[0]] },
		{"JointRF_1_2"		,&m_data->jointsData->kin_hand.right[HandMapping[1]] ,&m_data->jointsData->rt_hand.right[HandMapping[1]] },
		{"JointRF_1_3"		,&m_data->jointsData->kin_hand.right[HandMapping[2]] ,&m_data->jointsData->rt_hand.right[HandMapping[2]] },
		{"JointRF_1_4"		,&m_data->jointsData->kin_hand.right[HandMapping[3]] ,&m_data->jointsData->rt_hand.right[HandMapping[3]] },
		{"JointRF_1_5"		,&m_data->jointsData->kin_hand.right[HandMapping[4]] ,&m_data->jointsData->rt_hand.right[HandMapping[4]] },
		{"JointRF_2_3"		,&m_data->jointsData->kin_hand.right[HandMapping[5]] ,&m_data->jointsData->rt_hand.right[HandMapping[5]] },
		{"JointRF_2_4"		,&m_data->jointsData->kin_hand.right[HandMapping[6]] ,&m_data->jointsData->rt_hand.right[HandMapping[6]] },
		{"JointRF_3_3"		,&m_data->jointsData->kin_hand.right[HandMapping[7]] ,&m_data->jointsData->rt_hand.right[HandMapping[7]] },
		{"JointRF_3_4"		,&m_data->jointsData->kin_hand.right[HandMapping[8]] ,&m_data->jointsData->rt_hand.right[HandMapping[8]] },
		{"JointRF_4_1"		,&m_data->jointsData->kin_hand.right[HandMapping[9]] ,&m_data->jointsData->rt_hand.right[HandMapping[9]] },
		{"JointRF_4_3"		,&m_data->jointsData->kin_hand.right[HandMapping[10]],&m_data->jointsData->rt_hand.right[HandMapping[10]]},
		{"JointRF_4_4"		,&m_data->jointsData->kin_hand.right[HandMapping[11]],&m_data->jointsData->rt_hand.right[HandMapping[11]]},
		{"JointRF_5_3"		,&m_data->jointsData->kin_hand.right[HandMapping[12]],&m_data->jointsData->rt_hand.right[HandMapping[12]]},//12
		{"JointRF_5_4"		,&m_data->jointsData->kin_hand.right[HandMapping[13]],&m_data->jointsData->rt_hand.right[HandMapping[13]]},//13
	};

	int count=sizeof(infoList)/sizeof(BodyInfo);
	for (int i=0;i<count;++i)
	{
		m_schemeNames.push_back(infoList[i].name);
		AverageWindow*aw=new AverageWindow(1);
		JointValueReader vr;
		vr.id=i;
		vr.m_srcValue[0]=infoList[i].data[0];
		vr.m_srcValue[1]=infoList[i].data[1];
		vr.m_targetValue=aw;
		m_valueReader.push_back(vr);
		m_values.push_back(aw);
	}
}
VTSharedMemoryImpl::~VTSharedMemoryImpl()
{
	Close();
	delete m_data;
	for(int i=0;i<m_values.size();++i)
		delete m_values[i];
}

void VTSharedMemoryImpl::Start()
{
	m_data->agent->openWrite();
	m_owner->FIRE_LISTENR_METHOD(OnSchemeChanged,(m_owner,m_schemeNames));
}
void VTSharedMemoryImpl::Close()
{
	m_data->agent->Detach();
}

void VTSharedMemoryImpl::Update(float dt)
{
	if (!m_data->agent->data)
		return;
	CommunicationData data;
	data.SetTargetName(m_name);
	ControlInputValues v;

	memcpy(m_data->jointsData,&m_data->agent->data->joints,sizeof(shmem_joints_t));
	if(m_data->agent->data->status.calibration)
	{
		m_calibrated=true;
	}
	if(m_data->agent->data->status.telexistence)
	{
		m_connected=true;
		m_calibrated=true;
	}
	if(m_connected && !m_data->agent->data->status.telexistence)
	{
		m_connected=false;
		m_calibrated=false;
	}

	for(int i=0;i<m_valueReader.size();++i)
	{
		if(m_calibrated)
		{
			m_valueReader[i].update(dt);
		}
		v.KinValue=m_valueReader[i].m_targetValue->GetValue()*math::GRAD_PI32;
		v.RealtimeValue=*m_valueReader[i].m_srcValue[1]*math::GRAD_PI32;
		data.AddData(m_schemeNames[m_valueReader[i].id],&v,1);
	}


	m_owner->FIRE_LISTENR_METHOD(OnDataArrived,(m_owner,&data));
}


float VTSharedMemoryImpl::GetTemperature(bool left,int i)
{
	if(i>=3)
		return 0;
	float v=0;
	if(left)
	{
		v=m_data->agent->data->sensors.temp.left[i];
	}else
		v=m_data->agent->data->sensors.temp.right[i];


	float ret=26.70626*v/65535.0*5.0 - 27.39327;
	return ret;
}
float VTSharedMemoryImpl::GetForce(bool left,int i)
{
	if(i>=3)
		return 0;
	float v=0;
	if(left)
	{
		v=m_data->agent->data->sensors.force.left[i];
	}else
		v=m_data->agent->data->sensors.force.right[i];

	float ret=(35.24273*exp(0.44325*v/65535.0*5.0));
	return ret;
}

void VTSharedMemoryImpl::GetDebugString(std::vector<core::string>& lst)
{
	char tempBuff[512];
	sprintf(tempBuff,"Calibrated : %s",m_data->agent->data->status.calibration?"True":"False");
	lst.push_back(tempBuff);
	if(m_data->agent->data->status.telexistence)
	{
		sprintf(tempBuff,"Mode : Telexistance ");
		lst.push_back(tempBuff);
	}
	else if(m_data->agent->data->status.home)
	{
		sprintf(tempBuff,"Mode : Home");
		lst.push_back(tempBuff);
	}
	else if(m_data->agent->data->status.initial_pose)
	{
		sprintf(tempBuff,"Mode : Initial Pose");
		lst.push_back(tempBuff);
	}
	sprintf(tempBuff,"Thermal : %s",m_data->agent->data->status.thermal?"ON":"OFF");
	lst.push_back(tempBuff);
	sprintf(tempBuff,"System Status : %s",m_data->agent->data->status.emergency?"EMG-Stop":"Active");
	lst.push_back(tempBuff);
}

const std::vector<core::string>& VTSharedMemoryImpl::GetScheme()
{
	return m_schemeNames;
}

core::string VTSharedMemoryImpl::InjectCommand(const core::string& cmd, const core::string& args)
{
	if (!m_data->agent->data)
		return "";
	if (cmd.equals_ignore_case("calib"))
	{
		m_data->agent->data->status.calibration = true;
		m_data->agent->data->status.finger_calibrate = true;
		Update(0.01f);
		Sleep(20);
		m_data->agent->data->status.finger_calibrate = false;
		m_data->agent->data->status.calibration = false;
		return "done";
	}
	if (cmd.equals_ignore_case("done?"))
	{
		if (args.equals_ignore_case("glove"))
		{
			return "no";
		}
		if (args.equals_ignore_case("posture"))
		{
			return "no";
		}
	}
	return "";
}

VTSharedMemory::VTSharedMemory(const core::string& name)
{
	m_impl=new VTSharedMemoryImpl(name,this);
}
VTSharedMemory::~VTSharedMemory()
{
	delete m_impl;
}

void VTSharedMemory::Start()
{
	m_impl->Start();
}
void VTSharedMemory::Close()
{
	m_impl->Close();
}

void VTSharedMemory::Update(float dt)
{
	m_impl->Update(dt);
}

float VTSharedMemory::GetTemperature(bool left,int index)
{
	return m_impl->GetTemperature(left,index);
}
float VTSharedMemory::GetForce(bool left,int index)
{
	return m_impl->GetForce(left,index);
}

void VTSharedMemory::GetDebugString(std::vector<core::string>& lst)
{
	m_impl->GetDebugString(lst);
}

const std::vector<core::string>& VTSharedMemory::GetScheme()
{
	return m_impl->GetScheme();
}

core::string VTSharedMemory::InjectCommand(const core::string& cmd, const core::string& args)
{
	return m_impl->InjectCommand(cmd, args);
}

}
}