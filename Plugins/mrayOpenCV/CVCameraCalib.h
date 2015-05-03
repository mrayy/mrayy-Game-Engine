

#ifndef __CVCAMERACALIB__
#define __CVCAMERACALIB__

#include "CVCalibration.h"

namespace mray
{
namespace video
{

//credits: https://github.com/kikko/ofxCvCameraProjectorCalibration
class CVCameraCalib :public CVCalibration
{
protected:
	std::vector<cv::Point3f> candidateObjectPts;
public:
	CVCameraCalib();
	virtual ~CVCameraCalib();

	void computeCandidateBoardPose(const vector<cv::Point2f> & imgPts, cv::Mat& boardRot, cv::Mat& boardTrans);
	bool backProject(const cv::Mat& boardRot64, const cv::Mat& boardTrans64,
		const vector<cv::Point2f>& imgPt,
		vector<cv::Point3f>& worldPt);
	void setupCandidateObjectPoints();
	vector<cv::Point3f> getCandidateObjectPoints() { return candidateObjectPts; }
};

}
}

#endif
