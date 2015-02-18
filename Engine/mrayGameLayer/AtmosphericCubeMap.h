

/********************************************************************
	created:	2012/01/25
	created:	25:1:2012   14:56
	filename: 	d:\Development\mrayEngine\Tests\VegetationTest\AtmosphericCubeMap.h
	file path:	d:\Development\mrayEngine\Tests\VegetationTest
	file base:	AtmosphericCubeMap
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __AtmosphericCubeMap__
#define __AtmosphericCubeMap__

#include "CompileConfig.h"
#include "ITexture.h"
#include "JobOrder.h"

namespace mray
{
	class ThreadJobManager;

namespace game
{
class MRAY_GAME_DLL AtmosphericCubeMap
{
private:
public:
	struct AtmosphericCubeMapData
	{
		AtmosphericCubeMapData():
			maxDistance(1),sunBrightness(1),
			MieCoef(0),RaleighCoef(0),HG(0),
			extinctionTexSize(0),planeRadius(1),atmosphereRadius(1),
			cubeMapTexSize(0),extinctionTexDirty(false),cubeMapTexDirty(false)
	{
		lightDir=math::vector3d(-1,-1,-1).Normalize();
	}
		float MieCoef;
		float RaleighCoef;
		float maxDistance;
		float sunBrightness;
		float HG;
		float planeRadius;
		float atmosphereRadius;

		math::vector3d Mie;
		math::vector3d Raleigh;

		math::vector3d BetaMieC;
		math::vector3d BetaRaleighC;
		math::vector3d BetaExtinction;
		math::vector3d lightDir;

		ushort extinctionTexSize;
		ushort cubeMapTexSize;

		video::ITexturePtr extinctionTexture;
		video::ITexturePtr cubeMap;

		bool extinctionTexDirty;
		bool cubeMapTexDirty;

		std::vector<video::IHardwarePixelBuffer*> *lockedBuffers;
	};
protected:
	AtmosphericCubeMapData m_data;

	GCPtr<JobOrder> m_jobPiece;
	ThreadJobManager* m_threadMngr;

protected:
	void _updateCoefficients();

	void _updateExtinction();
	void _updateCubeMap();
	void _unlockBuffers();

	math::vector3d _calculateLin(const math::vector3d& viewDir);
	math::vector3d _calculateExtinctionAmount(float distance);
	float _calculateSkyLength(const math::vector3d& viewDir);
public:
	AtmosphericCubeMap();
	virtual~AtmosphericCubeMap();

	void Create(float maxDistance,float sunBrightness,float hg,
		float atmosphereRadius,float planeRadius,
		const math::vector3d& MieVec,const math::vector3d& RaleighVec,
		ushort skyTexSize=512,ushort extTexSize=1024,float MieC=0.001f,float RaleighCoef = 0.2f);

	void SetCoefficients(float MieC,float RaleighCoef,float HG);
	void SetCoefficientsVectors(const math::vector3d& MieVec,const math::vector3d& RaleighVec);
	void SetSunBrightness(float v);
	void SetMaxDistance(float v);
	void ChangeResolution(ushort skyTexSize,ushort extTexSize);

	void SetLightDirection(const math::vector3d&dir);

	float GetMieC()const{return m_data.MieCoef;}
	float GetRaleighC()const{return m_data.RaleighCoef;}
	float GetHG()const{return m_data.HG;}

	const math::vector3d& GetMieVec()const{return m_data.Mie;}
	const math::vector3d& GetRaleighVec()const{return m_data.Raleigh;}
	float GetSunBrightness()const{return m_data.sunBrightness;}
	float GetMaxDistance()const{return m_data.maxDistance;}
	ushort GetExtinctionTexSize()const{return m_data.extinctionTexSize;}
	ushort GetSkyTexSize()const{return m_data.cubeMapTexSize;}

	void SetThreadManager(ThreadJobManager* mngr)
	{
		m_threadMngr=mngr;
	}

	//call this function in order to rebuild the atmospher map
	void Invalidate(bool imediate=true);

	void Update();

	video::ITextureCRef GetExtinctionTexture()const{return m_data.extinctionTexture;}
	video::ITextureCRef GetSkyBoxTexture()const{return m_data.cubeMap;}
};

}
}

#endif
