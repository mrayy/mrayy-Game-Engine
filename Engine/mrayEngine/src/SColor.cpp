#include "stdafx.h"


#include "SColor.h"




namespace mray{
namespace video{


SColor &SColor::setColorRGBA(RGBA c)
{ 
	R=(c >>24  &0xFF)*math::i255;
	G=((c >>16)&0xFF)*math::i255; 
	B=((c >>8) &0xFF)*math::i255;
	A=((c    ) &0xFF)*math::i255;
	return *this;
}
SColor &SColor::setColorARGB(ARGB c)
{ 
	A=(c >>24  &0xFF)*math::i255;
	R=((c >>16)&0xFF)*math::i255; 
	G=((c >>8) &0xFF)*math::i255;
	B=((c    ) &0xFF)*math::i255;
	return *this;
}
SColor &SColor::setColorBGRA(BGRA c)
{ 
	B=(c >>24  &0xFF)*math::i255;
	G=((c >>16)&0xFF)*math::i255; 
	R=((c >>8) &0xFF)*math::i255;
	A=((c    ) &0xFF)*math::i255;
	return *this;
}
SColor &SColor::setColorABGR(ABGR c)
{ 
	A=(c >>24  &0xFF)*math::i255;
	B=((c >>16)&0xFF)*math::i255; 
	G=((c >>8) &0xFF)*math::i255;
	R=((c    ) &0xFF)*math::i255;
	return *this;
}

RGBA SColor::getColorRGBA()const
{ 
	RGBA ret=0;
	ret =static_cast<byte>(R*255)<<24;
	ret+=static_cast<byte>(G*255)<<16;
	ret+=static_cast<byte>(B*255)<<8;
	ret+=static_cast<byte>(A*255);
	return ret;
}
ARGB SColor::getColorARGB()const
{ 
	ARGB ret=0;
	ret =static_cast<byte>(A*255)<<24;
	ret+=static_cast<byte>(R*255)<<16;
	ret+=static_cast<byte>(G*255)<<8;
	ret+=static_cast<byte>(B*255);
	return ret;
}
BGRA SColor::getColorBGRA()const
{ 
	BGRA ret=0;
	ret =static_cast<byte>(B*255)<<24;
	ret+=static_cast<byte>(G*255)<<16;
	ret+=static_cast<byte>(R*255)<<8;
	ret+=static_cast<byte>(A*255);
	return ret;
}
ABGR SColor::getColorABGR()const
{ 
	ABGR ret=0;
	ret =static_cast<byte>(A*255)<<24;
	ret+=static_cast<byte>(B*255)<<16;
	ret+=static_cast<byte>(G*255)<<8;
	ret+=static_cast<byte>(R*255);
	return ret;
}
SColor SColor::Interpolated(const SColor &c,float i)const
{

	return math::lerp<SColor>(*this,c,i);
	//float inv=(float)(1-i);
	//i*=math::math::i255;
	//return SColor(saturate(c.R*inv +R*i),saturate(c.G*inv +G*i),saturate(c.B*inv +B*i),saturate(c.A*inv +A*i));
}

void SColor::setHSL(float hue, float saturation, float lightness)
{
	// wrap hue
	if (hue > 1.0f)
	{
		hue -= (int)hue;
	}
	else if (hue < 0.0f)
	{
		hue += (int)hue + 1;
	}
	// clamp saturation / lightness
	saturation = math::clamp(saturation,0.0f, 1.0f);
	lightness = math::clamp(lightness, 0.0f, 1.0f);

	if (lightness == 0.0f)
	{   
		// early exit, this has to be black
		R = G = B = 0;
		return;
	}

	if (saturation == 0.0f)
	{   
		// early exit, this has to be grey

		R = G = B = lightness*255;
		return;
	}


	float hueDomain  = hue * 6.0f;
	if (hueDomain >= 6.0f)
	{
		// wrap around, and allow mathematical errors
		hueDomain = 0.0f;
	}
	unsigned short domain = (unsigned short)hueDomain;
	float f1 = lightness * (1 - saturation);
	float f2 = lightness * (1 - saturation * (hueDomain - domain));
	float f3 = lightness * (1 - saturation * (1 - (hueDomain - domain)));

	/*
	f1*=255.0f;
	f2*=255.0f;
	f3*=255.0f;
	lightness*=255.0f;*/
	switch (domain)
	{
	case 0:
		// red domain; green ascends
		R = lightness;
		G = f3;
		B = f1;
		break;
	case 1:
		// yellow domain; red descends
		R = f2;
		G = lightness;
		B = f1;
		break;
	case 2:
		// green domain; blue ascends
		R = f1;
		G = lightness;
		B = f3;
		break;
	case 3:
		// cyan domain; green descends
		R = f1;
		G = f2;
		B = lightness;
		break;
	case 4:
		// blue domain; red ascends
		R = f3;
		G = f1;
		B = lightness;
		break;
	case 5:
		// magenta domain; blue descends
		R = lightness;
		G = f1;
		B = f2;
		break;
	}


}



};//video
};//mray



