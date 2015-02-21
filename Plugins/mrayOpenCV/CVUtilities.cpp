
#include "stdafx.h"
#include "CVUtilities.h"
#include <limits>

// vs2010 support (this should be added to the OF core)
#if (_MSC_VER)
#include <stdint.h>
#endif

namespace mray {
	namespace video {
	
	using namespace cv;
	using namespace std;
	Mat toCv(const Mat& mat)
	{
		return mat;
	}

	Mat toCv(const math::matrix4x4& mat) {
		return Mat(4,4,CV_32FC3,(float*)mat.getMatPointer());
	}
	
	Point2f toCv(const math::vector2d& vec) {
		return Point2f(vec.x, vec.y);
	}
	Size toCvSz(const math::vector2d& vec) {
		return Size(vec.x, vec.y);
	}
	
	Point3f toCv(const math::vector3d& vec) {
		return Point3f(vec.x, vec.y, vec.z);
	}
	
	cv::Rect toCv(const math::rectf& rect) {
		return cv::Rect(rect.ULPoint.x, rect.ULPoint.y, rect.getWidth(), rect.getHeight());
	}

	Mat toCv(const ImageInfo* pix) {
		return Mat(pix->Size.y, pix->Size.x, getCvImageType(pix->format), pix->imageData, 0);
	}
	
	vector<cv::Point2f> toCv(const vector<math::vector2d>& points) {
		vector<cv::Point2f> out(points.size());
		for(int i = 0; i < points.size(); i++) {
			out[i].x = points[i].x;
			out[i].y = points[i].y;
		}
		return out;		
	}
	
	vector<cv::Point3f> toCv(const vector<math::vector3d>& points) {
		vector<cv::Point3f> out(points.size());
		for(int i = 0; i < points.size(); i++) {
			out[i].x = points[i].x;
			out[i].y = points[i].y;
			out[i].z = points[i].z;
		}
		return out;		
	}


	math::vector2d fromCV(Point2f point) {
		return math::vector2d(point.x, point.y);
	}
	
	math::vector3d fromCV(Point3f point) {
		return math::vector3d(point.x, point.y, point.z);
	}
	
	math::rectf fromCV(cv::Rect rect) {
		return math::rectf(rect.x, rect.y, rect.x + rect.width, rect.x+rect.height);
	}
	bool fromCV(const Mat& src, math::matrix4x4& dst)
	{
		if (src.cols != 4 || src.rows != 4)
			return false;

		dst.loadMatrix((float*)src.ptr<float>());
		return true;
	}

	void fromCV(Mat mat, ImageInfo* pixels, bool refOnly) {
		if (!refOnly)
			pixels->setData(mat.ptr<uchar>(), math::vector3di(mat.cols, mat.rows, 1), getGlImageType(mat.type()));
		else
		{
			pixels->autoDel = false;
			pixels->Size = math::vector3di(mat.cols, mat.rows, 1);
			pixels->format = getGlImageType(mat.type());
			pixels->imageData = mat.ptr<uchar>();
		}
	}
	float getMaxVal(int cvDepth) {
		switch(cvDepth) {
			case CV_8U: return numeric_limits<uint8_t>::max();
			case CV_16U: return numeric_limits<uint16_t>::max();
				
			case CV_8S: return numeric_limits<int8_t>::max();
			case CV_16S: return numeric_limits<int16_t>::max();
			case CV_32S: return numeric_limits<int32_t>::max();
				
			case CV_32F: return 1;
			case CV_64F: default: return 1;
		}
	}
	
	float getMaxVal(const Mat& mat) {
		return getMaxVal(mat.depth());
	}
	
	// for some reason, cvtColor handles this info internally rather than having
	// a single helper function. so we have to create a helper function to aid
	// in doing the allocationg ofxCv::convertColor()
#define mkcase(x, y) {case x: return y;}
	int getTargetChannelsFromCode(int conversionCode) {
		switch(conversionCode) {
				mkcase(CV_RGB2RGBA,4)	mkcase(CV_RGBA2RGB,3) mkcase(CV_RGB2BGRA,4)
				mkcase(CV_RGBA2BGR,3) mkcase(CV_BGR2RGB,3) mkcase(CV_BGRA2RGBA,4)
				mkcase(CV_BGR2GRAY,1) mkcase(CV_RGB2GRAY,1) mkcase(CV_GRAY2RGB,3)
				mkcase(CV_GRAY2RGBA,4) mkcase(CV_BGRA2GRAY,1) mkcase(CV_RGBA2GRAY,1)
				mkcase(CV_BGR5652BGR,3) mkcase(CV_BGR5652RGB,3) mkcase(CV_BGR5652BGRA,4)
				mkcase(CV_BGR5652RGBA,4) mkcase(CV_BGR5652GRAY,1) mkcase(CV_BGR5552BGR,3)
				mkcase(CV_BGR5552RGB,3) mkcase(CV_BGR5552BGRA,4) mkcase(CV_BGR5552RGBA,4)
				mkcase(CV_BGR5552GRAY,1) mkcase(CV_BGR2XYZ,3) mkcase(CV_RGB2XYZ,3)
				mkcase(CV_XYZ2BGR,3) mkcase(CV_XYZ2RGB,3) mkcase(CV_BGR2YCrCb,3)
				mkcase(CV_RGB2YCrCb,3) mkcase(CV_YCrCb2BGR,3) mkcase(CV_YCrCb2RGB,3)
				mkcase(CV_BGR2HSV,3) mkcase(CV_RGB2HSV,3) mkcase(CV_BGR2Lab,3)
				mkcase(CV_RGB2Lab,3) mkcase(CV_BayerGB2BGR,3) mkcase(CV_BayerBG2RGB,3)
				mkcase(CV_BayerGB2RGB,3) mkcase(CV_BayerRG2RGB,3) mkcase(CV_BGR2Luv,3)
				mkcase(CV_RGB2Luv,3) mkcase(CV_BGR2HLS,3) mkcase(CV_RGB2HLS,3)
				mkcase(CV_HSV2BGR,3) mkcase(CV_HSV2RGB,3) mkcase(CV_Lab2BGR,3)
				mkcase(CV_Lab2RGB,3) mkcase(CV_Luv2BGR,3) mkcase(CV_Luv2RGB,3)
				mkcase(CV_HLS2BGR,3) mkcase(CV_HLS2RGB,3) mkcase(CV_BayerBG2RGB_VNG,3)
				mkcase(CV_BayerGB2RGB_VNG,3) mkcase(CV_BayerRG2RGB_VNG,3)
				mkcase(CV_BayerGR2RGB_VNG,3) mkcase(CV_BGR2HSV_FULL,3)
				mkcase(CV_RGB2HSV_FULL,3) mkcase(CV_BGR2HLS_FULL,3)
				mkcase(CV_RGB2HLS_FULL,3) mkcase(CV_HSV2BGR_FULL,3)
				mkcase(CV_HSV2RGB_FULL,3) mkcase(CV_HLS2BGR_FULL,3)
				mkcase(CV_HLS2RGB_FULL,3) mkcase(CV_LBGR2Lab,3) mkcase(CV_LRGB2Lab,3)
				mkcase(CV_LBGR2Luv,3) mkcase(CV_LRGB2Luv,3) mkcase(CV_Lab2LBGR,4)
				mkcase(CV_Lab2LRGB,4) mkcase(CV_Luv2LBGR,4) mkcase(CV_Luv2LRGB,4)
				mkcase(CV_BGR2YUV,3) mkcase(CV_RGB2YUV,3) mkcase(CV_YUV2BGR,3)
				mkcase(CV_YUV2RGB,3)
			default: return 0;
		}
	}
}
}