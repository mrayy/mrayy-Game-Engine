
#include "stdafx.h"
#include "AtmosphericCubeMap.h"

#include "FunctionProfiler.h"
#include "IThreadManager.h"
#include "MutexLocks.h"
#include "ThreadJobManager.h"
#include "AtmosphericCubeMapBuilder.h"

#include "JobPool.h"

namespace mray
{
namespace game
{

	class AtmosphericCubeMapBuilderJob:public JobOrder
	{
		AtmosphericCubeMap* m_owner;
		AtmosphericCubeMap::AtmosphericCubeMapData m_data;
		AtmosphericCubeMapBuilder m_builder;
		OS::IMutex* m_workingMutex; 
		bool m_working;
	public:
		AtmosphericCubeMapBuilderJob(AtmosphericCubeMap* owner)
			:m_working(false),m_builder(m_data)
		{
			m_workingMutex=OS::IThreadManager::getInstance().createMutex();
		}
		~AtmosphericCubeMapBuilderJob()
		{
			delete m_workingMutex;
		}
		AtmosphericCubeMap::AtmosphericCubeMapData &GetData()
		{
			return m_data;
		}
		void SetData(const AtmosphericCubeMap::AtmosphericCubeMapData&data)
		{
			{
				OS::ScopedLock locker(m_workingMutex);
				m_working=true;
			}
			m_data=data;
		}
		bool isWorking()
		{
			OS::ScopedLock locker(m_workingMutex);
			return m_working;
		}
		virtual bool ExecuteJob()
		{
			m_builder.Create();

			{
				OS::ScopedLock locker(m_workingMutex);
				m_working=false;
			}
			return true;
		}
	};

AtmosphericCubeMap::AtmosphericCubeMap():m_threadMngr(0)
{
	
	m_jobPiece=new AtmosphericCubeMapBuilderJob(this);
}
AtmosphericCubeMap::~AtmosphericCubeMap()
{
	delete m_data.lockedBuffers;
	//delete m_jobPiece;
}

void AtmosphericCubeMap::Create(float maxDistance,float sunBrightness,float hg,
								float atmosphereRadius,float planeRadius,
								const math::vector3d& MieVec,const math::vector3d& RaleighVec,
								ushort skyTexSize,ushort extTexSize,float MieC,float RaleighCoef )
{
	m_data.maxDistance=maxDistance;
	m_data.sunBrightness=sunBrightness;
	m_data.MieCoef=MieC;
	m_data.RaleighCoef=RaleighCoef;
	m_data.HG=hg;
	m_data.Mie=MieVec;
	m_data.Raleigh=RaleighVec;
	m_data.planeRadius=planeRadius;
	m_data.atmosphereRadius=atmosphereRadius;
	m_data.extinctionTexSize=extTexSize;
	m_data.cubeMapTexSize=skyTexSize;
	m_data.cubeMapTexDirty=true;
	m_data.extinctionTexDirty=true;
	m_data.lockedBuffers=new std::vector<video::IHardwarePixelBuffer*>();
	Invalidate(true);
}
void AtmosphericCubeMap::_unlockBuffers()
{
	for(int i=0;i<m_data.lockedBuffers->size();++i)
		(*m_data.lockedBuffers)[i]->unlock();
	m_data.lockedBuffers->clear();

}
void AtmosphericCubeMap::Invalidate(bool imediate)
{
	if(!m_data.cubeMapTexDirty && !m_data.extinctionTexDirty)
		return;
	AtmosphericCubeMapBuilderJob*job=((AtmosphericCubeMapBuilderJob*)m_jobPiece.pointer());
	bool working=job->isWorking() ;
	
	bool done=false;
	if(!working || imediate)
	{
		_unlockBuffers();
		job->SetData(m_data);
		m_data.cubeMapTexDirty=m_data.extinctionTexDirty=false;
		if(imediate)
		{
			job->ExecuteJob();
			done=true;
			_unlockBuffers();
			m_data.cubeMap=job->GetData().cubeMap;
		}
	
	}
	if(!working && !done)
	{
		JobPool::getInstance().AddRequest(m_jobPiece,0);
	}
}
void AtmosphericCubeMap::Update()
{
	AtmosphericCubeMapBuilderJob*job=((AtmosphericCubeMapBuilderJob*)m_jobPiece.pointer());
	if(!job->isWorking())
	{
		if(m_data.cubeMapTexDirty || m_data.extinctionTexDirty)
		{
			Invalidate(false);
		}else
		{
			//m_data.cubeMapTexDirty = m_data.extinctionTexDirty = false;
			_unlockBuffers();
		}
	}
}

void AtmosphericCubeMap::SetLightDirection(const math::vector3d&dir)
{
	if(m_data.lightDir==dir)
		return;
	m_data.lightDir=dir;
	m_data.cubeMapTexDirty=true;
}
void AtmosphericCubeMap::SetCoefficients(float MieC,float RaleighCoef,float HG)
{
	if(m_data.MieCoef!=MieC || m_data.RaleighCoef!=RaleighCoef)
		m_data.extinctionTexDirty=true;
	if(m_data.HG!=HG)
		m_data.cubeMapTexDirty=true;
	m_data.MieCoef=MieC;
	m_data.RaleighCoef=RaleighCoef;
	m_data.HG=HG;
}
void AtmosphericCubeMap::SetCoefficientsVectors(const math::vector3d& MieVec,const math::vector3d& RaleighVec)
{
	if(m_data.Mie!=MieVec || m_data.Raleigh!=RaleighVec)
		m_data.extinctionTexDirty=true;
	m_data.Mie=MieVec;
	m_data.Raleigh=RaleighVec;
}
void AtmosphericCubeMap::SetSunBrightness(float v)
{
	if(m_data.sunBrightness!=v)
		m_data.cubeMapTexDirty=true;
	m_data.sunBrightness=v;
}
void AtmosphericCubeMap::SetMaxDistance(float v)
{
	if(m_data.maxDistance!=v)
		m_data.extinctionTexDirty=true;
	m_data.maxDistance=v;
}
void AtmosphericCubeMap::ChangeResolution(ushort skyTexSize,ushort extTexSize)
{
	if(m_data.extinctionTexSize!=extTexSize)
		m_data.extinctionTexDirty=true;
	if(m_data.cubeMapTexSize!=skyTexSize)
		m_data.cubeMapTexDirty=true;
	m_data.extinctionTexSize=extTexSize;
	m_data.cubeMapTexSize=skyTexSize;
}

}
}
