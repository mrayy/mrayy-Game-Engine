

/********************************************************************
	created:	2013/08/05
	created:	5:8:2013   22:15
	filename: 	C:\Development\mrayEngine\Plugins\mrayFX\Fluid2DTexture.h
	file path:	C:\Development\mrayEngine\Plugins\mrayFX
	file base:	Fluid2DTexture
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __Fluid2DTexture__
#define __Fluid2DTexture__




namespace mray
{
	namespace video
	{
		class ITexture;
		class LockedPixelBox;
	}

namespace FX
{
	class FluidSolver;

	enum EFluidSourceType
	{
		FluidSource_Color,
		FluidSource_Velocity,
		FluidSource_Motion
	};
class Fluid2DTexture
{
protected:
	EFluidSourceType m_type;
	video::ITexture* m_tex;
	FluidSolver* m_solver;

	void _UpdateColor(video::LockedPixelBox* pixels);
	void _UpdateVelocity(video::LockedPixelBox* pixels);
	void _UpdateMotion(video::LockedPixelBox* pixels);
public:
	Fluid2DTexture();
	virtual~Fluid2DTexture();

	void SetTexture(video::ITexture*tex);
	void SetFluidSolver(FluidSolver* solver);
	void SetSourceType(EFluidSourceType type);

	video::ITexture* GetTexture()const{return m_tex;}
	FluidSolver* GetFluidSolver()const{return m_solver;}
	EFluidSourceType GetSourceType()const{return m_type;}

	void UpdateTexture();

};

}
}


#endif
