

/********************************************************************
	created:	2013/08/05
	created:	5:8:2013   13:39
	filename: 	C:\Development\mrayEngine\Plugins\mrayFX\FluidSolver.h
	file path:	C:\Development\mrayEngine\Plugins\mrayFX
	file base:	FluidSolver
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __FluidSolver__
#define __FluidSolver__


#include "SColor.h"


namespace mray
{
namespace FX
{

	struct FluidSolverDesc
	{
	public:
		FluidSolverDesc():resolution(64,64),solverIterations(10),viscocity(0.001),colorDiffusion(0)
			,wrapX(true),wrapY(true),doRGB(true),fadeSpeed(0.05),doVorticityConfinement(false)
		{

		}
		math::vector2di resolution;
		int solverIterations;
		float viscocity;
		float colorDiffusion;
		bool wrapX;
		bool wrapY;
		bool doRGB;
		float fadeSpeed;
		bool doVorticityConfinement;

	};

	class FluidSolverImpl;

class FluidSolver
{
protected:
	FluidSolverImpl* m_impl;

public:
	FluidSolver();
	virtual~FluidSolver();

	void SetupFromDesc(const FluidSolverDesc& d);
	const FluidSolverDesc& GetDesc()const;
	FluidSolverDesc& GetEditDesc();

	bool IsReady();

	void Resize(int xRes,int yRes);
	void Reset();
	void Clean();

	int GetWidth()const;
	int GetHeight()const;

	void SetViscocity(float vis);
	void SetColorDiffusion(float dif);
	void SetWrap(bool x,bool y);
	void SetSolverIterations(int n);

	void Update(float dt);

	float GetAverageDensity()const;
	float GetUniformity()const;
	float GetAverageSpeed()const;

	int GetIndexForCell(int x,int y)const;
	int GetIndexForPos(float x,float y)const;

	void Lock();
	void Unlock();

	math::vector2d GetVelocityAtIndex(int idx)const;
	math::vector2d GetVelocityAtCell(int x,int y)const{return GetVelocityAtIndex(GetIndexForCell(x,y));}
	math::vector2d GetVelocityAtPos(float x,float y)const{return GetVelocityAtIndex(GetIndexForPos(x,y));}

	video::SColor GetColorAtIndex(int idx)const;
	video::SColor GetColorAtCell(int x,int y)const{return GetColorAtIndex(GetIndexForCell(x,y));}
	video::SColor GetColorAtPos(float x,float y)const{return GetColorAtIndex(GetIndexForPos(x,y));}


	void AddForceAtIndex(int idx,const math::vector2d&f);
	void AddForceAtCell(int x,int y,const math::vector2d&f){return AddForceAtIndex(GetIndexForCell(x,y),f);}
	void AddForceAtPos(float x,float y,const math::vector2d&f){return AddForceAtIndex(GetIndexForPos(x,y),f);}

	void AddColorAtIndex(int idx,const video::SColor&c);
	void AddColorAtCell(int x,int y,const video::SColor&c){return AddColorAtIndex(GetIndexForCell(x,y),c);}
	void AddColorAtPos(float x,float y,const video::SColor&c){return AddColorAtIndex(GetIndexForPos(x,y),c);}

};

}
}


#endif
