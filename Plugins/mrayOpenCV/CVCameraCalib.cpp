

#include "stdafx.h"
#include "CVCameraCalib.h"


namespace mray
{
namespace video
{

	CVCameraCalib::CVCameraCalib()
	{

	}
	CVCameraCalib::~CVCameraCalib()
	{

	}

void CVCameraCalib::setupCandidateObjectPoints(){
	candidateObjectPts.clear();
	for (int i = 0; i < patternSize.height; i++) {
		for (int j = 0; j < patternSize.width; j++) {
			candidateObjectPts.push_back(cv::Point3f(float(j * squareSize), float(i * squareSize), 0));
		}
	}
}

void CVCameraCalib::computeCandidateBoardPose(const vector<cv::Point2f> & imgPts, cv::Mat& boardRot, cv::Mat& boardTrans){
	cv::solvePnP(candidateObjectPts, imgPts,
		distortedIntrinsics.getCameraMatrix(),
		distCoeffs,
		boardRot, boardTrans);
}

// some crazy stuff, no idea what's happening there. So thanks Alvaro & Niklas!
bool CVCameraCalib::backProject(const Mat& boardRot64,
	const Mat& boardTrans64,
	const vector<Point2f>& imgPt,
	vector<Point3f>& worldPt) {
	if (imgPt.size() == 0) {
		return false;
	}
	else
	{
		Mat imgPt_h = Mat::zeros(3, imgPt.size(), CV_32F);
		for (int h = 0; h < imgPt.size(); ++h) {
			imgPt_h.at<float>(0, h) = imgPt[h].x;
			imgPt_h.at<float>(1, h) = imgPt[h].y;
			imgPt_h.at<float>(2, h) = 1.0f;
		}
		Mat Kinv64 = getUndistortedIntrinsics().getCameraMatrix().inv();
		Mat Kinv, boardRot, boardTrans;
		Kinv64.convertTo(Kinv, CV_32F);
		boardRot64.convertTo(boardRot, CV_32F);
		boardTrans64.convertTo(boardTrans, CV_32F);

		// Transform all image points to world points in camera reference frame
		// and then into the plane reference frame
		Mat worldImgPt = Mat::zeros(3, imgPt.size(), CV_32F);
		Mat rot3x3;
		Rodrigues(boardRot, rot3x3);

		Mat transPlaneToCam = rot3x3.inv()*boardTrans;

		for (int i = 0; i < imgPt.size(); ++i) {
			Mat col = imgPt_h.col(i);
			Mat worldPtcam = Kinv*col;
			Mat worldPtPlane = rot3x3.inv()*(worldPtcam);

			float scale = transPlaneToCam.at<float>(2) / worldPtPlane.at<float>(2);
			Mat worldPtPlaneReproject = scale*worldPtPlane - transPlaneToCam;

			Point3f pt;
			pt.x = worldPtPlaneReproject.at<float>(0);
			pt.y = worldPtPlaneReproject.at<float>(1);
			pt.z = 0;
			worldPt.push_back(pt);
		}
	}
	return true;
}

}
}

