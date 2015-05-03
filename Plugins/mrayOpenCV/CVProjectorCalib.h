

#ifndef __CVPROJECTORCALIB__
#define __CVPROJECTORCALIB__

#include "CVCalibration.h"

namespace mray
{
namespace video
{
	
	//credits: https://github.com/kikko/ofxCvCameraProjectorCalibration
class CVProjectorCalib:public CVCalibration
{
protected:
	cv::Size imagerSize;
	cv::Point2f patternPosition;

private:
	vector<cv::Point2f> candidateImagePoints;
public:
	CVProjectorCalib();
	virtual ~CVProjectorCalib();

	void setImagerSize(int width, int height);
	void setPatternPosition(float x, float y);
	void setStaticCandidateImagePoints();
	void setCandidateImagePoints(vector<cv::Point2f> pts);
	const vector<cv::Point2f> & getCandidateImagePoints() const { return candidateImagePoints; }

};

}
}

#endif
