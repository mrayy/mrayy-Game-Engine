/*
 utilities are used internally by ofxCv, and make it easier to write code that
 can work with OpenCv and openFrameworks data.
 
 useful functions from this file:
 - imitate and copy
 - toCv and toOf
 */

#pragma once

#include "opencv2/opencv.hpp"
#include "ITexture.h"
#include "PixelUtil.h"

namespace mray {
namespace video {
	
	using namespace cv;
	
	// these functions are for accessing Mat, ofPixels and ofImage consistently.
	// they're very important for imitate().
	
	// width, height
	template <class T> inline int getWidth(T& src) {return src.getWidth();}
	template <class T> inline int getHeight(T& src) {return src.getHeight();}
	inline int getWidth(Mat& src) {return src.cols;}
	inline int getHeight(Mat& src) {return src.rows;}
	template <class T> inline bool getAllocated(T& src) {
		return getWidth(src) > 0 && getHeight(src) > 0;
	}
	
	// depth
    inline int getDepth(int cvImageType) {
        return CV_MAT_DEPTH(cvImageType);
    }
	inline int getDepth(Mat& mat) {
		return mat.depth();
	}
	inline int getDepth(video::EPixelFormat type) {
		switch (type) {
		case EPixel_R8G8B8A8:
		case EPixel_R8G8B8:
		case EPixel_X8R8G8B8:
		case EPixel_B8G8R8:
		case EPixel_B8G8R8A8:
		case EPixel_X8B8G8R8:
		case EPixel_LUMINANCE8:
		case EPixel_Alpha8:
			return CV_8U;

		case EPixel_Float32_R:
		case EPixel_Float32_RGB:
		case EPixel_Float32_RGBA:
		case EPixel_Float32_GR:
			return CV_32F;
		default: return 0;
		}
	}
    inline int getDepth(ITexture* tex) {
        // avoid "texture not allocated" warning
        if(!tex) {
            return CV_8U;
        }
		return getDepth( tex->getImageFormat());
    }
	/*template <class T> inline int getDepth(ofPixels_<T>& pixels) {
		switch(pixels.getBytesPerChannel()) {
			case 4: return CV_32F;
			case 2: return CV_16U;
			case 1: default: return CV_8U;
		}
	}
	template <> inline int getDepth(ofPixels_<signed short>& pixels) {
		return CV_16S;
	}
	template <> inline int getDepth(ofPixels_<signed char>& pixels) {
		return CV_8S;
	}
	template <class T> inline int getDepth(ofBaseHasPixels_<T>& img) {
		return getDepth(img.getPixelsRef());
	}*/

	inline int getChannels(video::EPixelFormat type) {

		const PixelDescriptor& d = PixelUtil::getPixelDescription(type);
		return d.componentsCount;
	}
	// channels
	inline int getChannels(int cvImageType) {
		return CV_MAT_CN(cvImageType);
	}/*
	inline int getChannels(ofImageType imageType) {
		switch(imageType) {
			case OF_IMAGE_COLOR_ALPHA: return 4;
			case OF_IMAGE_COLOR: return 3;
			case OF_IMAGE_GRAYSCALE: default: return 1;
		}
	}*/
	inline int getChannels(const Mat& mat) {
		return mat.channels();
	}
	/*
    inline int getChannels(ofTexture& tex) {
        // avoid "texture not allocated" warning
        if(!tex.isAllocated()) {
            return GL_RGB;
        }
        int type = tex.getTextureData().glTypeInternal;
        switch(type) {
            case GL_RGBA: return 4;
            case GL_RGB: return 3;
            case GL_LUMINANCE_ALPHA: return 2;
            case GL_LUMINANCE: return 1;
                
#ifndef TARGET_OPENGLES
            case GL_RGBA8: return 4;
            case GL_RGB8: return 3;
            case GL_LUMINANCE8: return 1;
            case GL_LUMINANCE8_ALPHA8: return 2;
                
            case GL_RGBA32F_ARB: return 4;
            case GL_RGB32F_ARB: return 3;
            case GL_LUMINANCE32F_ARB: return 1;
#endif
            default: return 0;
        }
    }
	template <class T> inline int getChannels(ofPixels_<T>& pixels) {
		return pixels.getNumChannels();
	}
	template <class T> inline int getChannels(ofBaseHasPixels_<T>& img) {
		return getChannels(img.getPixelsRef());
	}*/
	
	// image type
	inline int getCvImageType(int channels, int cvDepth ) {
		return CV_MAKETYPE(cvDepth, channels);
	}
	/*
	inline int getCvImageType(ofImageType imageType, int cvDepth = CV_8U) {
		return CV_MAKETYPE(cvDepth, getChannels(imageType));
	}
	template <class T> inline int getCvImageType(T& img) {
		return CV_MAKETYPE(getDepth(img), getChannels(img));
	}*/
	inline int getCvImageType(EPixelFormat fmt) {
		return CV_MAKETYPE(getDepth(fmt), getChannels(fmt));
	}
	inline int getImageType(int cvImageType) {
		return getChannels(cvImageType);
	}
    inline EPixelFormat getGlImageType(int cvImageType) {
        int channels = getChannels(cvImageType);
        int depth = getDepth(cvImageType);
        switch(depth) {
            case CV_8U:
                switch(channels) {
				case 1: return EPixel_LUMINANCE8;
					case 3: return EPixel_R8G8B8;
					case 4: return EPixel_R8G8B8A8;
                }
#ifndef TARGET_OPENGLES
            case CV_32F:
                switch(channels) {
					case 1: return EPixel_Unkown;
					case 3: return EPixel_Float32_RGB;
					case 4: return EPixel_Float32_RGBA;
                }
#endif
        }
		return EPixel_Unkown;
    }
	
	// allocation
	// only happens when necessary
	
    inline void allocate(ImageInfo* img, int width, int height, int cvType) {
		img->createData(math::vector3di(width, height, 1), getGlImageType(cvType));
    }
	inline void allocate(Mat& img, int width, int height, int cvType) {
		int iw = getWidth(img), ih = getHeight(img);
		int it = getCvImageType(getDepth(img),getChannels(img));
		if(iw != width || ih != height || it != cvType) {
			img.create(height, width, cvType);
		}
	}
	// ofVideoPlayer/Grabber can't be allocated, so we assume we don't need to do anything
	/*inline void allocate(ofVideoPlayer& img, int width, int height, int cvType) {}
	inline void allocate(ofVideoGrabber& img, int width, int height, int cvType) {}*/
	
	// imitate() is good for preparing buffers
	// it's like allocate(), but uses the size and type of the original as a reference
	// like allocate(), the image being allocated is the first argument	
	
	// this version copies size, but manually specifies mirror's image type
	template < class O> void imitate(Mat& mirror, O& original, int mirrorCvImageType) {
		int ow = getWidth(original), oh = getHeight(original);
		allocate(mirror, ow, oh, mirrorCvImageType);
	}
	
	// this version copies size and image type
	template < class O> void imitate(Mat& mirror, O& original) {
		imitate(mirror, original, getCvImageType(getDepth(original), getChannels(original)));
	}
	
	// maximum possible values for that depth or matrix
	float getMaxVal(int cvDepth);
	float getMaxVal(const Mat& mat);
	int getTargetChannelsFromCode(int conversionCode);
    
	// toCv functions
	// for conversion functions, the signature reveals the behavior:
	// 1       Type& argument // creates a shallow copy of the data
	// 2 const Type& argument // creates a deep copy of the data
	// 3       Type  argument // creates a deep copy of the data
	// style 1 is used when possible (for Mat conversion). style 2 is used when
	// dealing with a lot of data that can't/shouldn't be shallow copied. style 3
	// is used for small objects where the compiler can optimize the copying if
	// necessary. the reference is avoided to make inline toCv/toOf use easier.
	
	Mat toCv(const Mat& mat);
	Mat toCv(const math::matrix4x4& mat);
	
	Mat toCv(const ImageInfo* pix);
	Point2f toCv(const math::vector2d& vec);
	Point3f toCv(const math::vector3d& vec);
	cv::Rect toCv(const math::rectf&rect);
	vector<cv::Point2f> toCv(const std::vector<math::vector2d>& points);
	vector<cv::Point3f> toCv(const std::vector<math::vector3d>& points);
	Scalar toCv(const video::SColor& color);
	
	// cross-toolkit, cross-bitdepth copying
	template <class S, class D>
	void copy(S& src, D& dst, int dstDepth) {
		imitate(dst, src, getCvImageType(getChannels(src), dstDepth));
		Mat srcMat = toCv(src), dstMat = toCv(dst);
		if(srcMat.type() == dstMat.type()) {
			srcMat.copyTo(dstMat);
		} else {
			double alpha = getMaxVal(dstMat) / getMaxVal(srcMat);
			srcMat.convertTo(dstMat, dstMat.depth(), alpha);
		}
	}
	
	// most of the time you want the destination to be the same as the source. but
	// sometimes your destination is a different depth, and copy() will notice and
	// do the conversion for you.
	template <class S, class D>
	void copy(S& src, D& dst) {
		int dstDepth;
		if(getAllocated(dst)) {
			dstDepth = getDepth(dst);
		} else {
			dstDepth = getDepth(src);
		}
		copy(src, dst, dstDepth);
	}
	
	// toOf functions
	math::vector2d fromCV(Point2f point);
	math::vector3d fromCV(Point3f point);
	math::rectf fromCV(cv::Rect rect);
	bool fromCV(const Mat& src, math::matrix4x4& dst);

	template <class T>
	void fromCV(Mat mat, ImageInfo* pixels) {
		pixels->setData(mat.ptr<T>(), math::vector3di(mat.cols, mat.rows, 1), getGlImageType(mat.channels()));
	}
}
}