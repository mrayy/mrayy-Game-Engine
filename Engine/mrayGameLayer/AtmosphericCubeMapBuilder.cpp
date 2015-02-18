
#include "stdafx.h"
#include "AtmosphericCubeMapBuilder.h"
#include "TextureResourceManager.h"

namespace mray
{
namespace game
{

void AtmosphericCubeMapBuilder::Create()
{
	if(m_data.extinctionTexDirty)
	{
		_updateExtinction();
		m_data.extinctionTexDirty=false;
		m_data.cubeMapTexDirty=true;
	}
	if(m_data.cubeMapTexDirty)
	{
		_updateCubeMap();
		m_data.cubeMapTexDirty=false;
	}
}

void AtmosphericCubeMapBuilder::_updateCoefficients()
{
	m_data.BetaMieC=m_data.Mie*m_data.MieCoef;
	m_data.BetaRaleighC=m_data.Raleigh*m_data.RaleighCoef;

	m_data.BetaExtinction=m_data.BetaRaleighC+m_data.BetaMieC;
}

void AtmosphericCubeMapBuilder::_updateExtinction()
{

	if(m_data.extinctionTexture.isNull())
	{
		m_data.extinctionTexture=gTextureResourceManager.createEmptyTexture1D(mT("Atmo_ext"),true);
	}
	if(m_data.extinctionTexture->getSize().x!=m_data.extinctionTexSize)
		m_data.extinctionTexture->createTexture(math::vector3d(m_data.extinctionTexSize,1,1),video::EPixel_Float32_R);

	_updateCoefficients();

	video::IHardwarePixelBuffer* pixelBuf= m_data.extinctionTexture->getSurface(0);
	if(!pixelBuf->isLocked())
	{
		pixelBuf->lock(math::box3d(0,0,0,m_data.extinctionTexSize,1,1),video::IHardwareBuffer::ELO_Discard);
		m_data.lockedBuffers->push_back(pixelBuf);
	}
	const video::LockedPixelBox& lockBox=pixelBuf->getLockedBox();

	float * ptr=(float*)lockBox.data;

	float expValue=(m_data.BetaExtinction.x+m_data.BetaExtinction.y+m_data.BetaExtinction.z)/3.0f;
	float farthest=1-expf(-expValue*m_data.maxDistance*70.0f);
	for(int i=0;i<m_data.extinctionTexSize;++i)
	{
		float s=((float)i)*m_data.maxDistance/((float)(m_data.extinctionTexSize-1));
		float check=1-expf(-expValue*s*20.0f);

		check=1-check/farthest;
		check=math::clamp(check,0.f,1.f);//*255.0f;
		*ptr++=check;
	}
	//pixelBuf->unlock();
}
void AtmosphericCubeMapBuilder::_updateCubeMap()
{
	if(m_data.cubeMap.isNull())
	{
		m_data.cubeMap=gTextureResourceManager.createTextureCube(mT("Atmo_cubeMap"),m_data.cubeMapTexSize,video::EPixel_Float32_RGB);
	}
	if(m_data.cubeMap->getSize().x!=m_data.cubeMapTexSize)
		m_data.cubeMap->createTexture(math::vector3d(m_data.cubeMapTexSize,m_data.cubeMapTexSize,1),video::EPixel_Float32_RGB);

	float size=0.5f * (float)(m_data.cubeMapTexSize-1);
	float w,h;
	for(int face=0;face<6;++face)
	{
		video::IHardwarePixelBuffer* pixelBuf= m_data.cubeMap->getSurface(face);
		if(!pixelBuf->isLocked())
		{
			pixelBuf->lock(math::box3d(0,0,0,m_data.cubeMapTexSize,m_data.cubeMapTexSize,1),
				video::IHardwareBuffer::ELO_Discard);
			m_data.lockedBuffers->push_back(pixelBuf);
		}
		const video::LockedPixelBox& lockBox=pixelBuf->getLockedBox();

		float* ptr=(float*)lockBox.data;

		for(int y=0;y<m_data.cubeMapTexSize;++y)
		{
			h=y;
			h-=size;
			for(int x=0;x<m_data.cubeMapTexSize;++x)
			{
				w=x;
				w-=size;
				math::vector3d normal;
				switch(face)
				{
				case video::ECF_X_Positive:
					normal=math::vector3d(size,-h,-w);
					break;
				case video::ECF_X_Negative:
					normal=math::vector3d(-size,-h,w);
					break;
				case video::ECF_Y_Positive:
					normal=math::vector3d(w,size,h);
					break;
				case video::ECF_Y_Negative:
					normal=math::vector3d(w,-size,-h);
					break;
				case video::ECF_Z_Positive:
					normal=math::vector3d(w,-h,size);
					break;
				case video::ECF_Z_Negative:
					normal=math::vector3d(-w,-h,-size);
					break;
				}
				math::vector3d clr=_calculateLin(normal);
				clr.x=math::clamp(clr.x,0.f,1.f);
				clr.y=math::clamp(clr.y,0.f,1.f);
				clr.z=math::clamp(clr.z,0.f,1.f);
				//clr*=255.0f;

				*ptr++=/*(uchar)*/clr.x;
				*ptr++=/*(uchar)*/clr.y;
				*ptr++=/*(uchar)*/clr.z;
				//*ptr++=255;
			}
		}
		//pixelBuf->unlock();
	}
}

math::vector3d AtmosphericCubeMapBuilder::_calculateExtinctionAmount(float distance)
{
	math::vector3d ret;
	ret.x=expf(-m_data.BetaExtinction.x*distance);
	ret.y=expf(-m_data.BetaExtinction.y*distance);
	ret.z=expf(-m_data.BetaExtinction.z*distance);
	return ret;
}
float AtmosphericCubeMapBuilder::_calculateSkyLength(const math::vector3d& viewDir)
{
	math::vector3d dir=viewDir;
	dir.Normalize();
	float b,c;

	b=2.0f*m_data.planeRadius*dir.y;
	c=m_data.planeRadius*m_data.planeRadius - m_data.atmosphereRadius*m_data.atmosphereRadius;

	return (-b+sqrtf(b*b - 4.0f*c))/2.0f;
}
math::vector3d AtmosphericCubeMapBuilder::_calculateLin(const math::vector3d& viewDir)
{
	// Get the cosine of the angle between the light direction and view direction.

	float cosmTheta = ((-m_data.lightDir).dotProduct(viewDir))/viewDir.Length();//(*m_data.lightDir.Length());

	// Calculate the raleigh scattering at this angle
	// fR(mTheta) = 3/16pi * (1 + cos^2(mTheta))

	static const float coeff1=3.0f/(16.0f*math::PI32);
	float fR=coeff1*(2.0f + 0.5f*cosmTheta*cosmTheta);

	// Now the mie scattering
	float denom=sqrtf(1.0f + m_data.HG*m_data.HG +2.0f*m_data.HG*cosmTheta);
	float fM=(1.0f-m_data.HG)/(4.0f*math::PI32*denom*denom*denom);

	// Now the full Bsc(mTheta)
	math::vector3d betaScattering=m_data.BetaRaleighC*fR + m_data.BetaMieC*fM;

	float skyLength=_calculateSkyLength(-m_data.lightDir);
	math::vector3d sunExt=_calculateExtinctionAmount(skyLength);

	math::vector3d sunClr=sunExt * m_data.sunBrightness;

	float viewLength=_calculateSkyLength(viewDir);
	math::vector3d viewExt=_calculateExtinctionAmount(viewLength);

	math::vector3d finalClr=sunClr * betaScattering/m_data.BetaExtinction * (math::vector3d(1)-viewExt);

	float exposure=-1.0f;
	finalClr.x=1.0f-expf(-finalClr.x);//*exposure);
	finalClr.y=1.0f-expf(-finalClr.y);//*exposure);
	finalClr.z=1.0f-expf(-finalClr.z);//*exposure);

	return finalClr;

}

}
}
