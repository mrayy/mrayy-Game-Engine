

#include "stdafx.h"
#include "Fluid2DTexture.h"
#include "FluidSolver.h"
#include "ITexture.h"

namespace mray
{
namespace FX
{

Fluid2DTexture::Fluid2DTexture()
{
	m_type=FluidSource_Color;
	m_tex=0;
	m_solver=0;
}
Fluid2DTexture::~Fluid2DTexture()
{
}


void Fluid2DTexture::_UpdateColor(video::LockedPixelBox* pixels)
{
	int w=pixels->box.getWidth();
	int h=pixels->box.getHeight();
	uchar* ptr=(uchar*) pixels->data;
	video::SColor clr;
	for (int i=0;i<w;++i)
	{
		for (int j=0;j<h;++j)
		{
			clr=m_solver->GetColorAtCell(i,j);

			int r=math::Min<uchar>(clr.R*255,255);
			int g=math::Min<uchar>(clr.G*255,255);
			int b=math::Min<uchar>(clr.B*255,255);
			ptr[0]=r;
			ptr[1]=g;
			ptr[2]=b;
			ptr[3]=255;
			ptr+=4;
		}
	}
}
void Fluid2DTexture::_UpdateVelocity(video::LockedPixelBox* pixels)
{
	math::vector2d vel;
	int w=pixels->box.getWidth();
	int h=pixels->box.getHeight();

	int fw=m_solver->GetWidth();
	int fh=m_solver->GetHeight();

	uchar* ptr=(uchar*) pixels->data;
	for (int i=0;i<w;++i)
	{
		for (int j=0;j<h;++j)
		{
			vel= m_solver->GetVelocityAtCell(i,j);
			float s=fabs(vel.x)*fw+fabs(vel.y)*fh;
			s=math::Min(1.0f,s);
			ptr[0]=ptr[1]=ptr[2]=math::Min<uchar>(255,s*255);
			ptr[3]=255;
			ptr+=4;
		}
	}
}
void Fluid2DTexture::_UpdateMotion(video::LockedPixelBox* pixels)
{
	math::vector2d vel;
	int w=pixels->box.getWidth();
	int h=pixels->box.getHeight();
	int fw=m_solver->GetWidth();
	int fh=m_solver->GetHeight();
	uchar* ptr=(uchar*) pixels->data;
	for (int i=0;i<w;++i)
	{
		for (int j=0;j<h;++j)
		{
			vel= m_solver->GetVelocityAtCell(i,j);
			vel.x=fabs(vel.x)*fw;
			vel.y=fabs(vel.y)*fh;
			float s=fabs(vel.x)*fw+fabs(vel.y)*fh;
			s=math::Min(1.0f,s);
			vel.x=math::Min(1.0f,vel.x);
			vel.y=math::Min(1.0f,vel.y);
			ptr[0]=math::Min<uchar>(255,vel.x*255);
			ptr[1]=math::Min<uchar>(255,vel.y*255);
			ptr[2]=0;
			ptr[3]=255;
			ptr+=4;
		}
	}
}

void Fluid2DTexture::SetTexture(video::ITexture*tex)
{
	m_tex=tex;
}
void Fluid2DTexture::SetFluidSolver(FluidSolver* solver)
{
	m_solver=solver;
}
void Fluid2DTexture::SetSourceType(EFluidSourceType type)
{
	m_type=type;
}

void Fluid2DTexture::UpdateTexture()
{
	if(!m_solver || !m_tex)
		return;
	if(!m_solver->IsReady())
		return;
	const FluidSolverDesc& desc=m_solver->GetDesc();
	if(m_tex->getSize().x!=desc.resolution.x || m_tex->getSize().y!=desc.resolution.y)
	{
		m_tex->setMipmapsFilter(false);
		m_tex->createTexture(math::vector3d(desc.resolution.x,desc.resolution.y,1),video::EPixel_R8G8B8A8);
	}
	video::IHardwarePixelBuffer* surf= m_tex->getSurface(0);
	video::LockedPixelBox lockBox= surf->lock(math::box3d(0,m_tex->getSize()),video::IHardwareBuffer::ELO_Discard);
	m_solver->Lock();
	switch(m_type)
	{
	case FluidSource_Color:_UpdateColor(&lockBox);break;
	case FluidSource_Velocity:_UpdateVelocity(&lockBox);break;
	case FluidSource_Motion:_UpdateMotion(&lockBox);break;
	}
	m_solver->Unlock();
	surf->unlock();

}


}
}

