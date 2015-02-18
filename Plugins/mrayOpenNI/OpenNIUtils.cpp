

#include "stdafx.h"
#include "OpenNIUtils.h"


namespace mray
{

bool OpenNIUtils::ConvertToTexture(openni::VideoFrameRef& stream,video::LockedPixelBox& buff,std::vector<float>* histo)
{
	if(!stream.isValid() ||
		buff.box.getWidth()!=stream.getVideoMode().getResolutionX() ||
	   buff.box.getHeight()!=stream.getVideoMode().getResolutionY() ||
	   buff.format != video::EPixel_R8G8B8 && buff.format != video::EPixel_R8G8B8A8)
	{
		return false;
	}

	bool hasAlpha = (buff.format == video::EPixel_R8G8B8A8);

	bool isDepth=stream.getSensorType()==openni::SENSOR_DEPTH;

	const char* ptr=(const char*)stream.getData();
	const openni::DepthPixel* pDepthRow = (const openni::DepthPixel*)ptr;
	const openni::RGB888Pixel* pColorRow = (const openni::RGB888Pixel*)ptr;
	char* pTexRow = ((char*)buff.data) + (int)(stream.getCropOriginY() * buff.box.getWidth());

	int rowSize = stream.getStrideInBytes();
	if(isDepth)
		rowSize/= sizeof(openni::DepthPixel);
	else
		rowSize/= sizeof(openni::RGB888Pixel);

	int pixelSize = (3 + (hasAlpha ? 1 : 0));

	for (int y = 0; y < stream.getHeight(); ++y)
	{
		const openni::DepthPixel* pDepth = pDepthRow;
		const openni::RGB888Pixel* pColor = pColorRow;
		char* pTex = pTexRow + stream.getCropOriginX();

		for (int x = 0; x < stream.getWidth(); ++x, pTex += pixelSize)
		{
			if (*pDepth != 0)
			{
				if(isDepth)
				{
					int nHistValue = *pDepth;
					if(histo && histo->size())
						nHistValue=(*histo)[math::Min<int>(histo->size()-1,(histo->size())*((float)nHistValue/(float)histo->size()))];
					pTex[0] = nHistValue;
					pTex[1] = nHistValue;
					pTex[2] = nHistValue;
				}else
				{
					pTex[0]=pColor->r;
					pTex[1]=pColor->g;
					pTex[2]=pColor->b;
				}

			}else
			{
				pTex[0]=pTex[1]=pTex[2]=0;
			}
			if (hasAlpha)
			{
				pTex[3] = 255;
			}
			if(isDepth) ++pDepth;
			else ++pColor;
		}

		if(isDepth) pDepthRow += rowSize;
		else pColorRow += rowSize;
		pTexRow +=(int) buff.box.getWidth()*pixelSize;
	}
	return true;
}

bool OpenNIUtils::CalculateHistogram(openni::VideoFrameRef& frame,std::vector<float>& histo)
{
	if(!histo.size())
		return false;
	const openni::DepthPixel* pDepth = (const openni::DepthPixel*)frame.getData();
	// Calculate the accumulative histogram (the yellow display...)
	memset(&histo[0], 0, histo.size()*sizeof(float));
	int restOfRow = frame.getStrideInBytes() / sizeof(openni::DepthPixel) - frame.getWidth();
	int height = frame.getHeight();
	int width = frame.getWidth();

	unsigned int nNumberOfPoints = 0;
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x, ++pDepth)
		{
			if (*pDepth != 0)
			{
				histo[*pDepth]++;
				nNumberOfPoints++;
			}
		}
		pDepth += restOfRow;
	}
	for (int nIndex=1; nIndex<histo.size(); nIndex++)
	{
		histo[nIndex] += histo[nIndex-1];
	}
	if (nNumberOfPoints)
	{
		for (int nIndex=1; nIndex<histo.size(); nIndex++)
		{
			histo[nIndex] = (256 * (1.0f - (histo[nIndex] / nNumberOfPoints)));
		}
	}
	return true;
}

}

