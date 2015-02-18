




/********************************************************************
	created:	2013/12/05
	created:	5:12:2013   20:26
	filename: 	C:\Development\mrayEngine\Plugins\mrayOpenCV\CVCalibration
	file base:	CVCalibration
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	The implementation has been imported from openframeworks original implementation
*********************************************************************/

#ifndef CVCalibration_h__
#define CVCalibration_h__

#include "mString.h"
#include "opencv/cv.h"

using namespace cv;
namespace mray
{
namespace video
{


	class Intrinsics {
	public:
		// kinect is 6.66mm(H) x 5.32mm(V)
		void setup(Mat cameraMatrix, cv::Size imageSize, cv::Size sensorSize = cv::Size(0, 0));
		void setImageSize(cv::Size imgSize);
		Mat getCameraMatrix() const;
		cv::Size getImageSize() const;
		cv::Size getSensorSize() const;
		cv::Point2d getFov() const;
		double getFocalLength() const;
		double getAspectRatio() const;
		Point2d getPrincipalPoint() const;
		void loadProjectionMatrix(float nearDist = 10., float farDist = 10000., cv::Point2d viewportOffset = cv::Point2d(0, 0)) const;
	protected:
		Mat cameraMatrix;
		cv::Size imageSize, sensorSize;
		cv::Point2d fov;
		double focalLength, aspectRatio;
		Point2d principalPoint;
	};

	enum CalibrationPattern { CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID };

	class CVCalibration  {
	public:
		CVCalibration();

		void save(core::string filename, bool absolute = false) const;
		void load(core::string filename, bool absolute = false);
		void reset();

		void setPatternType(CalibrationPattern patternType);
		void setPatternSize(int xCount, int yCount);
		void setSquareSize(float squareSize);
		/// set this to the pixel size of your smallest square. default is 11
		void setSubpixelSize(int subpixelSize);

		bool add( Mat& img);
		bool clean(float minReprojectionError = 2.f);
		bool calibrate();
		bool calibrateFromDirectory(core::string directory);
		bool findBoard(Mat& img, OutputArray  pointBuf, bool refine = true);
		void setIntrinsics(Intrinsics& distortedIntrinsics, Mat& distortionCoefficients);

		void undistort( Mat& img, int interpolationMode = INTER_NEAREST);
		void undistort(const Mat& src, Mat& dst, int interpolationMode = INTER_NEAREST);

		math::vector2d undistort(const math::vector2d& src) const;
		void undistort(std::vector<math::vector2d>& src, std::vector<math::vector2d>& dst) const;

		bool getTransformation(CVCalibration& dst, Mat& rotation, Mat& translation);

		float getReprojectionError() const;
		float getReprojectionError(int i) const;

		const Intrinsics& getDistortedIntrinsics() const;
		const Intrinsics& getUndistortedIntrinsics() const;
		Mat getDistCoeffs() const;

		// if you want a wider fov, say setFillFrame(false) before load() or calibrate()
		void setFillFrame(bool fillFrame);

		int size() const;
		cv::Size getPatternSize() const;
		float getSquareSize() const;
		static std::vector<Point3f> createObjectPoints(cv::Size patternSize, float squareSize, CalibrationPattern patternType);

		void customDraw();
		void draw(int i) const;
		void draw3d() const;
		void draw3d(int i) const;

		bool isReady();
		std::vector<_OutputArray > imagePoints;

	protected:
		CalibrationPattern patternType;
		cv::Size patternSize, addedImageSize, subpixelSize;
		float squareSize;
		Mat grayMat;

		Mat distCoeffs;

		std::vector<Mat> boardRotations, boardTranslations;
		std::vector<std::vector<Point3f> > objectPoints;

		float reprojectionError;
		std::vector<float> perViewErrors;

		bool fillFrame;
		Mat undistortBuffer;
		Mat undistortMapX, undistortMapY;

		void updateObjectPoints();
		void updateReprojectionError();
		void updateUndistortion();

		Intrinsics distortedIntrinsics;
		Intrinsics undistortedIntrinsics;

		bool ready;
	};

}
}
#endif // CVCalibration_h__
