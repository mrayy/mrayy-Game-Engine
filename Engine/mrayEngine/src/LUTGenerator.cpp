

#include "stdafx.h"
#include "LUTGenerator.h"
#include "IVideoDevice.h"
#include "ColorCurve.h"


namespace mray
{
namespace video
{

LUTGenerator::LUTGenerator()
{
}
LUTGenerator::~LUTGenerator()
{
}


video::ITexturePtr LUTGenerator::GenerateLUT(IVideoDevice* device,const ColorCurve*r,const ColorCurve*g,const ColorCurve*b)
{
	math::vector3di tex3DSize(32);
	video::ITexturePtr generatedTexture=device->createTexture3D(tex3DSize,video::EPixel_R8G8B8,true);
	generatedTexture->setMipmapsFilter(false);

	video::IHardwarePixelBuffer* surface=generatedTexture->getSurface(0);
	video::LockedPixelBox box=surface->lock(math::box3d(0,0,0,tex3DSize.x,tex3DSize.y,tex3DSize.z),video::IHardwareBuffer::ELO_Discard);

	struct Pixel
	{
		uchar b,g,r;
	}*p;
	p = (Pixel*)box.data;

	Pixel val;

	int idx=0;
	//generate vectors

	std::vector<float> rSamples;
	std::vector<float> gSamples;
	std::vector<float> bSamples;
	std::vector<int> rCnt;
	std::vector<int> gCnt;
	std::vector<int> bCnt;
	rSamples.resize(tex3DSize.x,0);
	gSamples.resize(tex3DSize.y,0);
	bSamples.resize(tex3DSize.z,0);
	rCnt.resize(tex3DSize.x,0);
	gCnt.resize(tex3DSize.y,0);
	bCnt.resize(tex3DSize.z,0);

	int samplesCount=512;
	float invS=1.0f/(float)samplesCount;
	for(int i=0;i<samplesCount;++i)
	{
		float t=(float)i*invS;
		math::vector2d rS=r->GetValue(t);
		math::vector2d gS=g->GetValue(t);
		math::vector2d bS=b->GetValue(t);

		rS.x=math::clamp(rS.x,0.f,1.f);rS.y=math::clamp(rS.y,0.f,1.f);
		gS.x=math::clamp(gS.x,0.f,1.f);gS.y=math::clamp(gS.y,0.f,1.f);
		bS.x=math::clamp(bS.x,0.f,1.f);bS.y=math::clamp(bS.y,0.f,1.f);

		int idx=math::Min<int>(rS.x*tex3DSize.x,tex3DSize.x-1);
		rSamples[idx]+=rS.y;
		rCnt[idx]++;

		idx=math::Min<int>(gS.x*tex3DSize.y,tex3DSize.y-1);
		gSamples[idx]+=gS.y;
		gCnt[idx]++;

		idx=math::Min<int>(bS.x*tex3DSize.z,tex3DSize.z-1);
		bSamples[idx]+=bS.y;
		bCnt[idx]++;
	}
	for(int i=0;i<tex3DSize.x;++i)
	{
		if(rCnt[i]>0)
			rSamples[i]/=(float)rCnt[i];
		if(gCnt[i]>0)
			gSamples[i]/=(float)gCnt[i];
		if(bCnt[i]>0)
			bSamples[i]/=(float)bCnt[i];
	}

	for(int k=0;k<tex3DSize.z;++k)
	{
		val.r=bSamples[k]*256;
		for(int i=0;i<tex3DSize.x;++i)
		{
			val.g=rSamples[i]*256;
			for(int j=0;j<tex3DSize.y;++j)
			{
				val.b=gSamples[j]*256;
				idx=(k*tex3DSize.y+i)*tex3DSize.x + j;

				p[idx]=val;
			}
		}
	}
	surface->unlock();

	return generatedTexture;
}

}
}

