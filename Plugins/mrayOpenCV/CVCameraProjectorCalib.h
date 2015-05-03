

#ifndef __CVCAMERAPROJECTORCALIB__
#define __CVCAMERAPROJECTORCALIB__

#include "CVCameraCalib.h"
#include "CVProjectorCalib.h"

namespace mray
{
namespace video
{
	
//credits: https://github.com/kikko/ofxCvCameraProjectorCalibration
class CVCameraProjectorCalib
{
protected:

public:
	CVCameraProjectorCalib();
	virtual ~CVCameraProjectorCalib();

	void load(core::string cameraConfig = "calibrationCamera.yml",
		core::string projectorConfig = "calibrationProjector.yml",
		core::string extrinsicsConfig = "CameraProjectorExtrinsics.yml");
	void setup(int projectorWidth, int projectorHeight);
	void update(cv::Mat camMat);

	void saveExtrinsics(core::string filename) const;
	bool loadExtrinsics(core::string filename);

	bool addProjected(cv::Mat img, cv::Mat processedImg);

	bool setDynamicProjectorImagePoints(cv::Mat img);
	void stereoCalibrate();
	void resetBoards();
	int cleanStereo(float maxReproj);

	std::vector<math::vector2d> getProjected(const std::vector<math::vector3d> & ptsInWorld,
		const cv::Mat & rotObjToCam = cv::Mat::zeros(3, 1, CV_64F),
		const cv::Mat & transObjToCam = cv::Mat::zeros(3, 1, CV_64F));

	CVCameraCalib & getCalibrationCamera() { return calibrationCamera; }
	CVProjectorCalib & getCalibrationProjector() { return calibrationProjector; }

	const cv::Mat & getCamToProjRotation() { return rotCamToProj; }
	const cv::Mat & getCamToProjTranslation() { return transCamToProj; }

protected:

	CVCameraCalib calibrationCamera;
	CVProjectorCalib calibrationProjector;

	cv::Mat rotCamToProj;
	cv::Mat transCamToProj;
};

}
}

#endif
