#include "stdafx.h"



#include "ColorConverter.h"
#include "ILogManager.h"
#include "SColor.h"

#include "mraySystem.h"
#include "TraceManager.h"
#include "common.h"
#include "PixelUtil.h"

namespace mray{
namespace video{


SColor averageValue(uchar*data,int x,int y,int width,int factorX,int factorY,int components){
	float sum[4]={0};

	for(int i=0;i<factorY;++i){
		for(int j=0;j<factorX;j++){
			int index=(width*(y+i)+(x+j))*components;
			for(int k=0;k<components;k++)
				sum[k]+=data[index+k];
		}
	}
	float if2=1.0f/(float)(factorX*factorY);
	for(int k=0;k<components;k++){
		sum[k]*=if2;
		sum[k]=math::clamp(sum[k],0.0f,255.0f);
	}
	if(components==3)
		sum[3]=255;
	if(components==1)
		sum[1]=sum[2]=sum[3]=sum[0];
	return SColor(sum[0]*math::i255,sum[1]*math::i255,sum[2]*math::i255,sum[3]*math::i255);
}

float ColorConverter::normal32(int x,int y,int cd,int width,int height,uchar*ptr){
	if(x<0)x=width-1;
	if(x>=width)x=0;
	if(y<0)y=height-1;
	if(y>=height)y=0;

	int idx = (y*width + x)*cd;
	//a r g b
	//uint v=(ptr[y*pitch+x +0]<<24) | (ptr[y*pitch+x +0]<<16) | (ptr[y*pitch+x +1]<<8) | ptr[y*pitch+x +2];
	return (ptr[idx + 0] + ptr[idx + 1] + ptr[idx + 2]) / 3.0f;//red
}

void ColorConverter::convert1BitTo16Bit(const uchar*clrIn,uchar*clrOut,const math::vector2di& size,
		int linePad,bool flip)
{
	if(!clrIn || !clrOut)return;

	//if flip image flag then jump to the last of image

	for(int y=0;y<size.y;y++)
	{
		int shift=7;
		if(flip)
			clrOut-=size.x;
		for(int x=0;x<size.x;x++)
		{
			clrOut[x]=*clrIn>>shift & 0x01 ? (uchar)0xffff : (uchar)0x0000;
			if((--shift)<0)
			{
				shift=7;
				clrIn++;//process next Pixel
			}
		}

		if(shift!=7)
			clrIn++;
		if(!flip)
			clrOut+=size.x;
		clrIn+=linePad;
	}
}

void ColorConverter::colorFlip16Bit(const uchar*clrIn,uchar*clrOut,const math::vector2di& size,
		int linePad)
{
	if(!clrIn || !clrOut)return;

	// jump to the last of image
	clrOut+=size.x*size.y;

	for(int y=0;y<size.y;y++)
	{
		clrOut-=size.x;
		mraySystem::memCopy(clrOut,clrIn,size.x*sizeof(uchar));
		clrIn++;
		clrIn+=linePad;
	}
}

void ColorConverter::convert24BitTo24Bit(const uchar*clrIn,uchar*clrOut,const math::vector2di& size,
	int linePad,bool flip,bool bgr)
{
	if(!clrIn || !clrOut)return;
	int lineWidth=3*size.x;

	//if flip image flag then jump to the last of image
	if(flip) 
		clrOut+=lineWidth*size.y;

	for(int y=0;y<size.y;y++)
	{
		if(flip)
			clrOut-=lineWidth;
		if(bgr){
			for(int x=0;x<lineWidth;x+=3)
			{
				clrOut[x  ]=clrIn[x+2];
				clrOut[x+1]=clrIn[x+1];
				clrOut[x+2]=clrIn[x  ];
			}
		}
		else
		{
			mraySystem::memCopy(clrOut,clrIn,lineWidth*sizeof(uchar));
		}

		if(!flip)
			clrOut+=lineWidth;
		clrIn+=lineWidth+linePad;
	}
}

void ColorConverter::convert24BitTo24Bit(video::ImageInfo*img,const math::vector2di& size,
									 int linePad,bool flip,bool bgr)
{
	if(!img ||  !img->imageData)return;
	uchar*temp=new uchar[img->getPitch()*img->Size.y];
	convert24BitTo24Bit(img->imageData,temp,size,linePad,flip,bgr);
	img->setData(temp,img->Size,img->format);
	delete [] temp;
}

void ColorConverter::convert32BitTo32Bit(const uchar*clrIn,uchar*clrOut,const math::vector2di& size,
	int linePad,bool flip,bool bgr)
{
	if(!clrIn || !clrOut)return;
	int lineWidth=4*size.x;

	//if flip image flag then jump to the last of image
	if(flip) 
		clrOut+=lineWidth*size.y;

	for(int y=0;y<size.y;y++)
	{
		if(flip)
			clrOut-=lineWidth;
		if(bgr){
			for(int x=0;x<lineWidth;x+=4)
			{
				clrOut[x  ]=clrIn[x+2];
				clrOut[x+1]=clrIn[x+1];
				clrOut[x+2]=clrIn[x  ];
				clrOut[x+3]=clrIn[x+3];
			}
		}
		else
		{
			mraySystem::memCopy(clrOut,clrIn,lineWidth*sizeof(uchar));
		}

		if(!flip)
			clrOut+=lineWidth;
		clrIn+=lineWidth+linePad;
	}
}
void ColorConverter::convert32BitTo32Bit(video::ImageInfo*img,const math::vector2di& size,
	int linePad,bool flip,bool bgr){

	if(!img || !img->imageData)return;
	uchar*temp=new uchar[img->getPitch()*img->Size.y];
	convert32BitTo32Bit((uchar*)img->imageData,temp,size,linePad,flip,bgr);
	img->setData(temp,img->Size,img->format);
	delete [] temp;
}


void ColorConverter::convert32BitTo24Bit(const uchar*clrIn,uchar*clrOut,const math::vector2di& size,
	int linePad,bool flip,bool bgr)
{
	if(!clrIn || !clrOut)return;
	int lineWidth=4*size.x;
	int lineWidth2=3*size.x;

	//if flip image flag then jump to the last of image
	if(flip) 
		clrOut+=lineWidth*size.y;

	for(int y=0;y<size.y;y++)
	{
		if(flip)
			clrOut-=lineWidth;
		for(int x=0,k=0;x<lineWidth;x+=4,k+=3)
		{
			if(bgr){
				clrOut[k  ]=clrIn[x+2];
				clrOut[k+2]=clrIn[x  ];
			}else{
				clrOut[k  ]=clrIn[x  ];
				clrOut[k+2]=clrIn[x+2];
			}
			clrOut[k+1]=clrIn[x+1];
		}
		

		if(!flip)
			clrOut+=lineWidth2;
		clrIn+=lineWidth+linePad;
	}
}
void ColorConverter::convert32BitTo24Bit(video::ImageInfo*img,const math::vector2di& size,
	int linePad,bool flip,bool bgr){

	if(!img || !img->imageData)return;
	uchar*temp=new uchar[img->getPitch()*img->Size.y];
	convert32BitTo24Bit((uchar*)img->imageData,temp,size,linePad,flip,bgr);
	img->setData(temp,img->Size,img->format);
	delete [] temp;
}



void ColorConverter::convert8BitTo32BitAndResize(const uchar*clrIn,uchar*clrOut,const math::vector2di& size,
									 math::Point2di newSize)
{
	if(!clrIn || !clrOut)return;
	if(newSize ==0)return;
	float xStep=(float)size.x/(float)newSize.x;
	float yStep=(float)size.y/(float)newSize.y;

	float sy;
	int t;
	for(int x=0;x<newSize.x;x++)
	{
		sy=0;

		for(int y=0;y<newSize.y;y++)
		{
			t=clrIn[(int)(((int)sy)*size.x+x*xStep)];
			clrOut[(int)(y*newSize.x+x)*4+0]=t;
			clrOut[(int)(y*newSize.x+x)*4+1]=t;
			clrOut[(int)(y*newSize.x+x)*4+2]=t;
			clrOut[(int)(y*newSize.x+x)*4+3]=255;
			sy+=yStep;
		}
	}
}
void ColorConverter::convert8BitTo24BitAndResize(const uchar*clrIn,uchar*clrOut,const math::vector2di& size,
	math::Point2di newSize){
	if(!clrIn || !clrOut)return;
	if(newSize ==0)return;
	float xStep=(float)size.x/(float)newSize.x;
	float yStep=(float)size.y/(float)newSize.y;

	float sy;
	int t;
	for(int x=0;x<newSize.x;x++)
	{
		sy=0;

		for(int y=0;y<newSize.y;y++)
		{
			t=clrIn[(int)(((int)sy)*size.x+x*xStep)];
			clrOut[(int)(y*newSize.x+x)*4+0]=t;
			clrOut[(int)(y*newSize.x+x)*4+1]=t;
			clrOut[(int)(y*newSize.x+x)*4+2]=t;
			sy+=yStep;
		}
	}
}

void ColorConverter::convert16BitTo32BitAndResize(const uchar*clrIn,uchar*clrOut,const math::vector2di& size,
									 math::Point2di newSize)
{
	if(!clrIn || !clrOut)return;
	if(newSize ==0)return;
	float xStep=(float)size.x/(float)newSize.x;
	float yStep=(float)size.y/(float)newSize.y;

	float sy;
	for(int x=0;x<newSize.x;x++)
	{
		sy=0;

		for(int y=0;y<newSize.y;y++)
		{
/*
			t=clrIn[(int)(((int)sy)*size.x+x*xStep)];
			t = (((t >> 15)&0x1)<<31) |	(((t >> 10)&0x1F)<<19) |
				(((t >> 5)&0x1F)<<11) |	(t&0x1F)<<3;
			clrOut[(int)(y*newSize.y+x)]=t;*/
			sy+=yStep;
		}
	}
}
void ColorConverter::convert24BitTo32BitAndResize(const uchar*clrIn,uchar*clrOut,const math::vector2di& size,
	math::Point2di newSize)
{
	if(!clrIn || !clrOut)return;
	if(newSize ==0)return;
	float xStep=(float)size.x/(float)newSize.x;
	float yStep=(float)size.y/(float)newSize.y;

	float sy;
	int a=0;
	int b=0;
	for(int x=0;x<newSize.x;x++)
	{
		sy=0;

		for(int y=0;y<newSize.y;y++)
		{
			clrOut[(int)(y*newSize.x+x)*4+0]=1*clrIn[(int)(((int)sy)*size.x+x*xStep)*3+0];
			clrOut[(int)(y*newSize.x+x)*4+1]=1*clrIn[(int)(((int)sy)*size.x+x*xStep)*3+1];
			clrOut[(int)(y*newSize.x+x)*4+2]=1*clrIn[(int)(((int)sy)*size.x+x*xStep)*3+2];
			clrOut[(int)(y*newSize.x+x)*4+3]=255;
			/*
			t=clrIn[(int)(((int)sy)*size.x+x*xStep)];
			clrOut[(int)(y*newSize.y+x)]=t;*/
			sy+=yStep;
		}
	}
}


void ColorConverter::convert24BitToGreyscale(const uchar*clrIn,uchar*clrOut,const math::vector2di& size)
{
	if(!clrIn || !clrOut)return;


	struct rgb
	{
		uchar r,g,b;
	}*p;

	p=(rgb*)clrIn;
	for(int i=0;i<size.x*size.y;++i)
	{
		clrOut[i]=(uchar)(0.45 * p->r + 0.35 * p->g + 0.2 * p->b);
		//clrOut[i]=(uchar)(0.299 * p->r + 0.587 * p->g + 0.114 * p->b);
		//clrOut[i]=(uchar)(0.2125 * p->r + 0.7154 * p->g + 0.0721 * p->b);
		p++;
	}
}
void ColorConverter::convert32BitToGreyscale(const uchar*clrIn,uchar*clrOut,const math::vector2di& size)
{
	if(!clrIn || !clrOut)return;

	struct rgba
	{
		uchar r,g,b,a;
	}*p,*o;

	p=(rgba*)clrIn;
	o=(rgba*)clrOut;
	uchar grey;
	for(int i=0;i<size.x*size.y;++i)
	{
		grey=(uchar)(0.45 * p->r + 0.35 * p->g + 0.2 * p->b);
		o->r=o->b=o->g=grey;
		o->a=p->a;
		//clrOut[i]=(uchar)(0.299 * p->r + 0.587 * p->g + 0.114 * p->b);
		//clrOut[i]=(uchar)(0.2125 * p->r + 0.7154 * p->g + 0.0721 * p->b);
		p++;
		o++;
	}
}


//not bad method to rescale images
void ColorConverter::resizeImage(const video::ImageInfo* tIn,video::ImageInfo* tOut,const math::Point2di& newSize)
{
	if (!tIn || !tOut)return;
	tOut->createData(math::vector3d(newSize.x, newSize.y, 1), tIn->format);

	float dw = (float)tIn->Size.x / (float)newSize.x;
	float dh = (float)tIn->Size.y / (float)newSize.y;

	int depth = video::PixelUtil::getPixelDescription(tIn->format).componentsCount;

	if (dw == 1 && dh == 1)//just copy
	{
		memcpy(tOut->imageData, tIn->imageData, tIn->imageDataSize);
		return;
	}
	uchar*buffer = (uchar*)tOut->imageData;
	uchar*inData = (uchar*)tIn->imageData;
	int width = tIn->Size.x;
	//	int x=0,y=0;
	//	byte clrArr[4];
	for (int i = 0; i<newSize.x; ++i){
		//		x=i*dw;
		for (int j = 0; j<newSize.y; j++){
			//		y=j*dh;
			int index = (j*newSize.x + i)*depth;
			//averageValue(inData,x,y,width,dw,dh,depth).ToByteArrayRGBA(clrArr);
			int nearestMatch = (((int)(j * dh) * (tOut->Size.x)) + ((int)(i *dw)))*depth;
			if (depth == 1)
				buffer[index] = inData[nearestMatch];
			else if (depth == 3){
				buffer[index + 0] = inData[nearestMatch + 0];
				buffer[index + 1] = inData[nearestMatch + 1];
				buffer[index + 2] = inData[nearestMatch + 2];
			}
			else{
				buffer[index + 0] = inData[nearestMatch + 0];
				buffer[index + 1] = inData[nearestMatch + 1];
				buffer[index + 2] = inData[nearestMatch + 2];
				buffer[index + 3] = inData[nearestMatch + 3];
			}
		}
	}
	return;
}


//not bad method to rescale images
void ColorConverter::resizeImage(video::ImageInfo*img,const math::Point2di& newSize)
{
	if(!img)return;
	float dw=(float)img->Size.x/(float)newSize.x;
	float dh=(float)img->Size.y/(float)newSize.y;

	int depth=video::PixelUtil::getPixelDescription(img->format).componentsCount;
	
	if(dw==1 && dh==1)//just copy
	{
		return;
	}

	uchar*buffer=new uchar[newSize.x*newSize.y*depth];

	uchar*inData=(uchar*)img->imageData;
	int width=img->Size.x;
//	int x=0,y=0;
//	byte clrArr[4];
	for(int i=0;i<newSize.x;++i){
//		x=i*dw;
		for(int j=0;j<newSize.y;j++){
	//		y=j*dh;
			int index = (j*newSize.x + i)*depth;
			//averageValue(inData,x,y,width,dw,dh,depth).ToByteArrayRGBA(clrArr);
			int nearestMatch = (((int)(j * dh) * (img->Size.x )) + ((int)(i *dw) ))*depth;
			if(depth==1)
				buffer[index]=inData[nearestMatch];
			else if(depth==3){
				buffer[index + 0] = inData[nearestMatch+0];
				buffer[index + 1] = inData[nearestMatch + 1];
				buffer[index + 2] = inData[nearestMatch + 2];
			}else{
				buffer[index + 0] = inData[nearestMatch + 0];
				buffer[index + 1] = inData[nearestMatch + 1];
				buffer[index + 2] = inData[nearestMatch + 2];
				buffer[index + 3] = inData[nearestMatch + 3];
			}
		}
	}
	img->setData(buffer,math::vector3d(newSize.x,newSize.y,1),img->format);
	delete [] buffer;
	return;
}


void ColorConverter::changeGamma(video::ImageInfo*img,float factor){

#define DoJop(val)\
	val*=fmul;\
	if(val>1 && (temp=(1.0f/val))<scale)scale=temp;

	int size=img->Size.x*img->Size.y;

	float fmul=factor*math::i255;

	float r,g,b;
	float scale=1;
	float temp=0;
	byte*p=(byte*)img->imageData;

	for(int i=0;i<size;++i){
		scale=1;
		switch(video::PixelUtil::getPixelDescription(img->format).componentsCount){
			case 1:
				r=(float)p[0];
				DoJop(r);
				scale*=255;
				r*=255;
				r=math::clamp<float>(r,0,255);
				p[0]=(byte)r;
				p++;
				break;
			case 2:
				return;
				break;
			case 3:
				r=(float)p[0];
				g=(float)p[1];
				b=(float)p[2];
				DoJop(r);
				DoJop(g);
				DoJop(b);
				scale*=255;
				r*=255;
				g*=255;
				b*=255;
				r=math::clamp<float>(r,0,255);
				g=math::clamp<float>(g,0,255);
				b=math::clamp<float>(b,0,255);
				p[0]=(byte)r;
				p[1]=(byte)g;
				p[2]=(byte)b;
				p+=3;
				break;
			case 4:
				r=(float)p[0];
				g=(float)p[1];
				b=(float)p[2];
				DoJop(r);
				DoJop(g);
				DoJop(b);
				scale*=255;
				r*=255;
				g*=255;
				b*=255;
				r=math::clamp<float>(r,0,255);
				g=math::clamp<float>(g,0,255);
				b=math::clamp<float>(b,0,255);
				p[0]=(byte)r;
				p[1]=(byte)g;
				p[2]=(byte)b;
				p+=4;
				break;
		}

	}
}

void ColorConverter::createNormalMap(video::ImageInfo*img,float amplitude){
	
	traceFunction(eVideo);
	if(!img){
		gLogManager.log(mT("Null imgture!"),ELL_ERROR);
	}
	if(video::PixelUtil::getPixelDescription(img->format).componentsCount!=4){
		//convert to 32-bit image
		int cmp=video::PixelUtil::getPixelDescription(img->format).componentsCount;
		if(cmp!=1 && cmp!=2 && cmp!=3 && cmp!=4)
			return;
		if(cmp!=4){
			uchar*buff=new uchar[img->Size.x*img->Size.y*4];
			math::vector2di sz(img->Size.x,img->Size.y);
			switch(cmp){
				case 1:
					convert8BitTo32BitAndResize((uchar*)img->imageData,buff,sz,sz);
					break;
				case 2:
					convert16BitTo32BitAndResize((uchar*)img->imageData,buff,sz,sz);
					break;
				case 3:
					convert24BitTo32BitAndResize((uchar*)img->imageData,buff,sz,sz);
					break;
			};
			delete[] img->imageData;
			img->imageData = buff;
			img->Size = img->Size;
			img->format=EPixel_R8G8B8A8;
		}
	}
	uchar*ptr=(uchar*)img->imageData;
	if(!ptr)return;

	math::Point2di d(img->Size.x,img->Size.y);
	amplitude/=255.0f;
	
	float vh=(float)d.y/(float)d.x;
	float hh=(float)d.x/(float)d.y;

	int pitch=img->getPitch();
	int cd=video::PixelUtil::getPixelDescription(img->format).componentsCount;

	uchar*in=new uchar[pitch*d.y];
	mraySystem::memCopy(in,ptr,sizeof(uchar)*d.y*pitch);


	for(int x=0;x<d.x;x++){
		for(int y=0;y<d.y;y++){
			math::vector3d h1((x-1)*hh,normal32(x-1,y,cd,d.x,d.y,in)*amplitude,y*vh);
			math::vector3d h2((x+1)*hh,normal32(
				x+1,y,cd,d.x,d.y,in)*amplitude,y*vh);

			math::vector3d v1(x*hh,normal32(x,y+1,cd,d.x,d.y,in)*amplitude,(y-1)*vh);
			math::vector3d v2(x*hh,normal32(x,y-1,cd,d.x,d.y,in)*amplitude,(y+1)*vh);

			math::vector3d v=v1-v2;
			math::vector3d h=h1-h2;

			math::vector3d n=v.crossProduct(h);
			n.Normalize();
			n*=0.5;
			n+=0.5;
			n*=255;

			uchar height=normal32(x,y,cd,d.x,d.y,in);
			ptr[(y*d.x+x)*4 +0]=(uchar)n.x;
			ptr[(y*d.x+x)*4 +1]=(uchar)n.z;
			ptr[(y*d.x+x)*4 +2]=(uchar)n.y;
			ptr[(y*d.x+x)*4 +3]=height;
		}
	}

	delete [] in;

}










}
}

