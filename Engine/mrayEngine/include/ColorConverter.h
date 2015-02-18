



#ifndef ___COLORCONVERTER___
#define ___COLORCONVERTER___

#include "mTypes.h"

#include "ImageInfo.h"

namespace mray{
namespace video{

class MRAY_DLL ColorConverter
{
public:

	static inline float normal32(int x,int y,int cd,int width,int height,uchar*ptr);


	//mono -> 16 bit (1A5R5G5B)
	static void convert1BitTo16Bit(const uchar*clrIn,uchar*clrOut,const math::vector2di& size,
		int linePad,bool flip);

	//flip 16 bit image
	static void colorFlip16Bit(const uchar*clrIn,uchar*clrOut,const math::vector2di& size,
		int linePad);

	//convert 24 bit -> 24 bit with some changes
	//flip:flip image upside down
	//bgr :convert image fro rgb to bgr
	static void convert24BitTo24Bit(const uchar*clrIn,uchar*clrOut,const math::vector2di& size,
		int linePad,bool flip,bool bgr);
	static void convert24BitTo24Bit(video::ImageInfo* image,const math::vector2di& size,
		int linePad,bool flip,bool bgr);

	//convert 32 bit -> 32 bit with some changes
	//flip:flip image upside down
	//bgr :convert image fro rgb to bgr
	static void convert32BitTo32Bit(const uchar*clrIn,uchar*clrOut,const math::vector2di& size,
		int linePad,bool flip,bool bgr);
	static void convert32BitTo32Bit(video::ImageInfo* image,const math::vector2di& size,
		int linePad,bool flip,bool bgr);

	//convert 32 bit -> 24 bit with some changes
	//flip:flip image upside down
	//bgr :convert image fro rgb to bgr
	static void convert32BitTo24Bit(const uchar*clrIn,uchar*clrOut,const math::vector2di& size,
		int linePad,bool flip,bool bgr);
	static void convert32BitTo24Bit(video::ImageInfo* tex,const math::vector2di& size,
		int linePad,bool flip,bool bgr);

	//convert 8 bit -> A8R8G8B8 bit with resize
	static void convert8BitTo32BitAndResize(const uchar*clrIn,uchar*clrOut,const math::vector2di& size,
		math::Point2di newSize);

	//convert 8 bit -> R8G8B8 bit with resize
	static void convert8BitTo24BitAndResize(const uchar*clrIn,uchar*clrOut,const math::vector2di& size,
		math::Point2di newSize);

	//convert 16 bit -> A8R8G8B8 bit with resize
	static void convert16BitTo32BitAndResize(const uchar*clrIn,uchar*clrOut,const math::vector2di& size,
		math::Point2di newSize);

	//convert 24 bit -> A8R8G8B8 bit with resize
	static void convert24BitTo32BitAndResize(const uchar*clrIn,uchar*clrOut,const math::vector2di& size,
		math::Point2di newSize);


	//convert 24 bit -> Grey scale image
	static void convert24BitToGreyscale(const uchar*clrIn,uchar*clrOut,const math::vector2di& size);

	//convert 32 bit -> Grey scale image
	static void convert32BitToGreyscale(const uchar*clrIn,uchar*clrOut,const math::vector2di& size);

	static void resizeImage(const video::ImageInfo* tIn,video::ImageInfo *tOut,const math::Point2di& newSize);
	static void resizeImage(video::ImageInfo* image,const math::Point2di& newSize);

	static void changeGamma(video::ImageInfo* image,float factor);

	static void createNormalMap(video::ImageInfo* image,float amplitude);

};


};//video
};//mray

#endif








