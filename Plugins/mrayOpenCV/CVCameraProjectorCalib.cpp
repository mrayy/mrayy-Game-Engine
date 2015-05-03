

#include "stdafx.h"
#include "CVCameraProjectorCalib.h"

using namespace std;

namespace mray
{
namespace video
{


void CVCameraProjectorCalib::load(core::string cameraConfig, core::string projectorConfig, core::string extrinsicsConfig){
	calibrationCamera.load(cameraConfig);
	calibrationProjector.load(projectorConfig);
	loadExtrinsics(extrinsicsConfig);
}

void CVCameraProjectorCalib::setup(int projectorWidth, int projectorHeight){

	calibrationCamera.setPatternSize(8, 5);
	calibrationCamera.setSquareSize(1.25);
	calibrationCamera.setPatternType(CHESSBOARD);

	calibrationProjector.setImagerSize(projectorWidth, projectorHeight);
	calibrationProjector.setPatternSize(4, 5);
	calibrationProjector.setPatternPosition(500, 250);
	calibrationProjector.setSquareSize(40);
	calibrationProjector.setPatternType(ASYMMETRIC_CIRCLES_GRID);
}

void CVCameraProjectorCalib::saveExtrinsics(core::string filename) const {

	core::string path;
	gFileSystem.getCorrectFilePath(filename, path);
	if (path == "")
		path = gFileSystem.getAppPath() + filename;
	cv::FileStorage fs(path, cv::FileStorage::WRITE);
	fs << "Rotation_Vector" << rotCamToProj;
	fs << "Translation_Vector" << transCamToProj;
}

bool CVCameraProjectorCalib::loadExtrinsics(core::string filename) {

	core::string path;
	gFileSystem.getCorrectFilePath(filename, path);
	if (path == "")
		return false;
	cv::FileStorage fs(path, cv::FileStorage::READ);
	fs["Rotation_Vector"] >> rotCamToProj;
	fs["Translation_Vector"] >> transCamToProj;
	return true;
}

std::vector<math::vector2d> CVCameraProjectorCalib::getProjected(const std::vector<math::vector3d> & pts,
	const cv::Mat & rotObjToCam,
	const cv::Mat & transObjToCam){
	cv::Mat rotObjToProj, transObjToProj;

	cv::composeRT(rotObjToCam, transObjToCam,
		rotCamToProj, transCamToProj,
		rotObjToProj, transObjToProj);

	std::vector<math::vector2d> out;
	projectPoints(cv::Mat(pts),
		rotObjToProj, transObjToProj,
		calibrationProjector.getDistortedIntrinsics().getCameraMatrix(),
		calibrationProjector.getDistCoeffs(),
		out);
	return out;
}

bool CVCameraProjectorCalib::addProjected(cv::Mat img, cv::Mat processedImg){

	vector<cv::Point2f> chessImgPts;

	bool bPrintedPatternFound = calibrationCamera.findBoard(img, chessImgPts, true);

	if (bPrintedPatternFound) {

		vector<cv::Point2f> circlesImgPts;
		bool bProjectedPatternFound = cv::findCirclesGrid(processedImg, calibrationProjector.getPatternSize(), circlesImgPts, cv::CALIB_CB_ASYMMETRIC_GRID);

		if (bProjectedPatternFound){

			vector<cv::Point3f> circlesObjectPts;
			cv::Mat boardRot;
			cv::Mat boardTrans;
			calibrationCamera.computeCandidateBoardPose(chessImgPts, boardRot, boardTrans);
			calibrationCamera.backProject(boardRot, boardTrans, circlesImgPts, circlesObjectPts);

			calibrationCamera.imagePoints.push_back(chessImgPts);
			calibrationCamera.getObjectPoints().push_back(calibrationCamera.getCandidateObjectPoints());
			calibrationCamera.getBoardRotations().push_back(boardRot);
			calibrationCamera.getBoardTranslations().push_back(boardTrans);

			calibrationProjector.imagePoints.push_back(calibrationProjector.getCandidateImagePoints());
			calibrationProjector.getObjectPoints().push_back(circlesObjectPts);

			return true;
		}
	}
	return false;
}

bool CVCameraProjectorCalib::setDynamicProjectorImagePoints(cv::Mat img){

	vector<cv::Point2f> chessImgPts;
	bool bPrintedPatternFound = calibrationCamera.findBoard(img, chessImgPts, true);

	if (bPrintedPatternFound) {

		cv::Mat boardRot;
		cv::Mat boardTrans;
		calibrationCamera.computeCandidateBoardPose(chessImgPts, boardRot, boardTrans);

		const auto & camCandObjPts = calibrationCamera.getCandidateObjectPoints();
		Point3f axisX = camCandObjPts[1] - camCandObjPts[0];
		Point3f axisY = camCandObjPts[calibrationCamera.getPatternSize().width] - camCandObjPts[0];
		Point3f pos = camCandObjPts[0] - axisY * (calibrationCamera.getPatternSize().width - 2);

		vector<Point3f> auxObjectPoints;
		for (int i = 0; i < calibrationProjector.getPatternSize().height; i++) {
			for (int j = 0; j < calibrationProjector.getPatternSize().width; j++) {
				auxObjectPoints.push_back(pos + axisX * float((2 * j) + (i % 2)) + axisY * i);
			}
		}

		Mat Rc1, Tc1, Rc1inv, Tc1inv, Rc2, Tc2, Rp1, Tp1, Rp2, Tp2;
		Rp1 = calibrationProjector.getBoardRotations().back();
		Tp1 = calibrationProjector.getBoardTranslations().back();
		Rc1 = calibrationCamera.getBoardRotations().back();
		Tc1 = calibrationCamera.getBoardTranslations().back();
		Rc2 = boardRot;
		Tc2 = boardTrans;

		Mat auxRinv = Mat::eye(3, 3, CV_32F);
		Rodrigues(Rc1, auxRinv);
		auxRinv = auxRinv.inv();
		Rodrigues(auxRinv, Rc1inv);
		Tc1inv = -auxRinv*Tc1;
		Mat Raux, Taux;
		composeRT(Rc2, Tc2, Rc1inv, Tc1inv, Raux, Taux);
		composeRT(Raux, Taux, Rp1, Tp1, Rp2, Tp2);

		vector<Point2f> followingPatternImagePoints;
		projectPoints(Mat(auxObjectPoints),
			Rp2, Tp2,
			calibrationProjector.getDistortedIntrinsics().getCameraMatrix(),
			calibrationProjector.getDistCoeffs(),
			followingPatternImagePoints);

		calibrationProjector.setCandidateImagePoints(followingPatternImagePoints);
	}
	return bPrintedPatternFound;
}

void CVCameraProjectorCalib::stereoCalibrate(){

	const auto & objectPoints = calibrationProjector.getObjectPoints();

	vector<vector<cv::Point2f> > auxImagePointsCamera;
	for (int i = 0; i < objectPoints.size(); i++) {
		vector<cv::Point2f> auxImagePoints;
		projectPoints(cv::Mat(objectPoints[i]),
			calibrationCamera.getBoardRotations()[i],
			calibrationCamera.getBoardTranslations()[i],
			calibrationCamera.getDistortedIntrinsics().getCameraMatrix(),
			calibrationCamera.getDistCoeffs(),
			auxImagePoints);

		auxImagePointsCamera.push_back(auxImagePoints);
	}

	cv::Mat projectorMatrix = calibrationProjector.getDistortedIntrinsics().getCameraMatrix();
	cv::Mat projectorDistCoeffs = calibrationProjector.getDistCoeffs();
	cv::Mat cameraMatrix = calibrationCamera.getDistortedIntrinsics().getCameraMatrix();
	cv::Mat cameraDistCoeffs = calibrationCamera.getDistCoeffs();

	cv::Mat fundamentalMatrix, essentialMatrix;
	cv::Mat rotation3x3;

	cv::stereoCalibrate(objectPoints,
		auxImagePointsCamera,
		calibrationProjector.imagePoints,
		cameraMatrix, cameraDistCoeffs,
		projectorMatrix, projectorDistCoeffs,
		calibrationCamera.getDistortedIntrinsics().getImageSize(),
		rotation3x3, transCamToProj,
		essentialMatrix, fundamentalMatrix);

	cv::Rodrigues(rotation3x3, rotCamToProj);
}

void CVCameraProjectorCalib::resetBoards(){
	calibrationCamera.resetBoards();
	calibrationProjector.resetBoards();
}

int CVCameraProjectorCalib::cleanStereo(float maxReproj){
	int removed = 0;
	for (int i = calibrationProjector.size() - 1; i >= 0; i--) {
		if (calibrationProjector.getReprojectionError(i) > maxReproj) {
			calibrationProjector.remove(i);
			calibrationCamera.remove(i);
			removed++;
		}
	}
	return removed;
}

}
}

