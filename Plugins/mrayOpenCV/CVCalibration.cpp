
#include "stdafx.h"
#include "CVCalibration.h"
#include "CVWrappers.h"
#include "ILogManager.h"


namespace mray {
namespace video {

using namespace cv;

void Intrinsics::setup(Mat cameraMatrix, cv::Size imageSize, cv::Size sensorSize) {
	this->cameraMatrix = cameraMatrix;
	this->imageSize = imageSize;
	this->sensorSize = sensorSize;
	calibrationMatrixValues(cameraMatrix, imageSize, sensorSize.width, sensorSize.height,
		fov.x, fov.y, focalLength, principalPoint, aspectRatio);

}

void Intrinsics::setImageSize(cv::Size imgSize) {
	imageSize = imgSize;
}

Mat Intrinsics::getCameraMatrix() const {
	return cameraMatrix;
}

cv::Size Intrinsics::getImageSize() const {
	return imageSize;
}

cv::Size Intrinsics::getSensorSize() const {
	return sensorSize;
}

cv::Point2d Intrinsics::getFov() const {
	return fov;
}

double Intrinsics::getFocalLength() const {
	return focalLength;
}

double Intrinsics::getAspectRatio() const {
	return aspectRatio;
}

Point2d Intrinsics::getPrincipalPoint() const {
	return principalPoint;
}

void Intrinsics::loadProjectionMatrix(float nearDist, float farDist, cv::Point2d viewportOffset) const {

/*	ofViewport(viewportOffset.x, viewportOffset.y, imageSize.width, imageSize.height);
	ofSetMatrixMode(OF_MATRIX_PROJECTION);
	ofLoadIdentityMatrix();
	float w = imageSize.width;
	float h = imageSize.height;
	float fx = cameraMatrix.at<double>(0, 0);
	float fy = cameraMatrix.at<double>(1, 1);
	float cx = principalPoint.x;
	float cy = principalPoint.y;

	ofMatrix4x4 frustum;
	frustum.makeFrustumMatrix(
		nearDist * (-cx) / fx, nearDist * (w - cx) / fx,
		nearDist * (cy) / fy, nearDist * (cy - h) / fy,
		nearDist, farDist);
	ofMultMatrix(frustum);

	ofSetMatrixMode(OF_MATRIX_MODELVIEW);
	ofLoadIdentityMatrix();

	ofMatrix4x4 lookAt;
	lookAt.makeLookAtViewMatrix(ofVec3f(0, 0, 0), ofVec3f(0, 0, 1), ofVec3f(0, -1, 0));
	ofMultMatrix(lookAt);*/
}

CVCalibration::CVCalibration() :
	patternType(CHESSBOARD),
	patternSize(cv::Size(10, 7)), // based on Chessboard_A4.pdf, assuming world units are centimeters
	subpixelSize(cv::Size(11, 11)),
	squareSize(2.5),
	reprojectionError(0),
	fillFrame(true),
	ready(false) {

}

void CVCalibration::save(core::string filename, bool absolute) const {
	if (!ready){
		gLogManager.log("CVCalibration::save() failed, because your calibration isn't ready yet!",ELL_WARNING);
		return;
	}
	core::string path;
	gFileSystem.getCorrectFilePath(filename, path);

	FileStorage fs(path, FileStorage::WRITE);
	cv::Size imageSize = distortedIntrinsics.getImageSize();
	cv::Size sensorSize = distortedIntrinsics.getSensorSize();
	Mat cameraMatrix = distortedIntrinsics.getCameraMatrix();
	fs << "cameraMatrix" << cameraMatrix;
	fs << "imageSize_width" << imageSize.width;
	fs << "imageSize_height" << imageSize.height;
	fs << "sensorSize_width" << sensorSize.width;
	fs << "sensorSize_height" << sensorSize.height;
	fs << "distCoeffs" << distCoeffs;
	fs << "reprojectionError" << reprojectionError;
	fs << "features" << "[";
	for (int i = 0; i < (int)imagePoints.size(); i++) {
		fs << "[:" << imagePoints[i].getMat() << "]";
	}
	fs << "]";
}

void CVCalibration::load(core::string filename, bool absolute) {
	imagePoints.clear();
	core::string path;
	gFileSystem.getCorrectFilePath(filename,path);

	FileStorage fs(path, FileStorage::READ);
	cv::Size imageSize, sensorSize;
	Mat cameraMatrix;
	fs["cameraMatrix"] >> cameraMatrix;
	fs["imageSize_width"] >> imageSize.width;
	fs["imageSize_height"] >> imageSize.height;
	fs["sensorSize_width"] >> sensorSize.width;
	fs["sensorSize_height"] >> sensorSize.height;
	fs["distCoeffs"] >> distCoeffs;
	fs["reprojectionError"] >> reprojectionError;
	FileNode features = fs["features"];
	for (FileNodeIterator it = features.begin(); it != features.end(); it++) {
		vector<Point2f> cur;
		(*it) >> cur;
		imagePoints.push_back(cur);
	}
	addedImageSize = imageSize;
	distortedIntrinsics.setup(cameraMatrix, imageSize, sensorSize);
	updateUndistortion();
	ready = true;
}
void CVCalibration::setIntrinsics(Intrinsics& distortedIntrinsics, Mat& distortionCoefficients){
	this->distortedIntrinsics = distortedIntrinsics;
	this->distCoeffs = distortionCoefficients;
	this->addedImageSize = distortedIntrinsics.getImageSize();
	updateUndistortion();
	this->ready = true;
}
void CVCalibration::reset(){
	this->ready = false;
	this->reprojectionError = 0.0;
	this->imagePoints.clear();
	this->objectPoints.clear();
	this->perViewErrors.clear();
}
void CVCalibration::setPatternType(CalibrationPattern patternType) {
	this->patternType = patternType;
}
void CVCalibration::setPatternSize(int xCount, int yCount) {
	patternSize = cv::Size(xCount, yCount);
}
void CVCalibration::setSquareSize(float squareSize) {
	this->squareSize = squareSize;
}
void CVCalibration::setFillFrame(bool fillFrame) {
	this->fillFrame = fillFrame;
}
void CVCalibration::setSubpixelSize(int subpixelSize) {
	subpixelSize = MAX(subpixelSize, 2);
	this->subpixelSize = cv::Size(subpixelSize, subpixelSize);
}
bool CVCalibration::add( Mat& img) {
	addedImageSize = img.size();

	_OutputArray pointBuf;

	// find corners
	bool found = findBoard(img, pointBuf);

	if (found)
		imagePoints.push_back(pointBuf);
// 	else
// 		gLogManager.log( "CVCalibration::add() failed, maybe your patternSize is wrong or the image has poor lighting?",ELL_WARNING);
	return found;
}
bool CVCalibration::findBoard(Mat& img, OutputArray pointBuf, bool refine) {
	bool found = false;+
	if (patternType == CHESSBOARD) {
		// no CV_CALIB_CB_FAST_CHECK, because it breaks on dark images (e.g., dark IR images from kinect)
		int chessFlags = CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE;// ;

		found = findChessboardCorners(img, patternSize, pointBuf, chessFlags);

		// improve corner accuracy
		if (found) {
			if (img.type() != CV_8UC1) {
				copyGray(img, grayMat);
			}
			else {
				grayMat = img;
			}

			if (refine) {
				// the 11x11 dictates the smallest image space square size allowed
				// in other words, if your smallest square is 11x11 pixels, then set this to 11x11
				cornerSubPix(grayMat, pointBuf, subpixelSize, cv::Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
			}
		}
	}
#ifdef USING_OPENCV_2_3
	else {
		int flags = (patternType == CIRCLES_GRID ? CALIB_CB_SYMMETRIC_GRID : CALIB_CB_ASYMMETRIC_GRID); // + CALIB_CB_CLUSTERING
		found = findCirclesGrid(img, patternSize, pointBuf, flags);
	}
#endif
	return found;
}
bool CVCalibration::clean(float minReprojectionError) {
	int removed = 0;
	for (int i = size() - 1; i >= 0; i--) {
		if (getReprojectionError(i) > minReprojectionError) {
			objectPoints.erase(objectPoints.begin() + i);
			imagePoints.erase(imagePoints.begin() + i);
			removed++;
		}
	}
	if (size() > 0) {
		if (removed > 0) {
			return calibrate();
		}
		else {
			return true;
		}
	}
	else {
		gLogManager.log( "CVCalibration::clean() removed the last object/image point pair",ELL_WARNING);
		return false;
	}
}
bool CVCalibration::calibrate() {
	if (size() < 1) {
		gLogManager.log( "CVCalibration::calibrate() doesn't have any image data to calibrate from.",ELL_WARNING);
		if (ready) {
			gLogManager.log("CVCalibration::calibrate() doesn't need to be called after CVCalibration::load().",ELL_WARNING);
		}
		return ready;
	}

	Mat cameraMatrix = Mat::eye(3, 3, CV_64F);
	distCoeffs = Mat::zeros(8, 1, CV_64F);

	updateObjectPoints();

	int calibFlags = 0;
	float rms = calibrateCamera(objectPoints, imagePoints, addedImageSize, cameraMatrix, distCoeffs, boardRotations, boardTranslations, calibFlags);
	gLogManager.log( "calibrateCamera() reports RMS error of " + core::StringConverter::toString(rms),ELL_INFO,EVL_Heavy);

	ready = checkRange(cameraMatrix) && checkRange(distCoeffs);

	if (!ready) {
		gLogManager.log("CVCalibration::calibrate() failed to calibrate the camera",ELL_WARNING);
	}

	distortedIntrinsics.setup(cameraMatrix, addedImageSize);
	updateReprojectionError();
	updateUndistortion();

	return ready;
}

bool CVCalibration::isReady(){
	return ready;
}

bool CVCalibration::calibrateFromDirectory(core::string directory) {
	/*
	ofDirectory dirList;
	ofImage cur;
	dirList.listDir(directory);
	for (int i = 0; i < (int)dirList.size(); i++) {
		cur.loadImage(dirList.getPath(i));
		if (!add(toCv(cur))) {
			gLogManager.log( "CVCalibration::add() failed on " + dirList.getPath(i));
		}
	}*/
	return calibrate();
}
void CVCalibration::undistort( Mat& img, int interpolationMode) {
	img.copyTo(undistortBuffer);
	undistort(undistortBuffer, img, interpolationMode);
}
void CVCalibration::undistort(const Mat& src, Mat& dst, int interpolationMode) {
	remap(src, dst, undistortMapX, undistortMapY, interpolationMode);
}

math::vector2d CVCalibration::undistort(const math::vector2d& src) const {
	math::vector2d dst;
	Mat matSrc = Mat(1, 1, CV_32FC2,(float*) &src.x);
	Mat matDst = Mat(1, 1, CV_32FC2, &dst.x);;
	undistortPoints(matSrc, matDst, distortedIntrinsics.getCameraMatrix(), distCoeffs);
	return dst;
}

void CVCalibration::undistort(std::vector<math::vector2d>& src, std::vector<math::vector2d>& dst) const {
	int n = src.size();
	dst.resize(n);
	Mat matSrc = Mat(n, 1, CV_32FC2, &src[0].x);
	Mat matDst = Mat(n, 1, CV_32FC2, &dst[0].x);
	undistortPoints(matSrc, matDst, distortedIntrinsics.getCameraMatrix(), distCoeffs);
}

bool CVCalibration::getTransformation(CVCalibration& dst, Mat& rotation, Mat& translation) {
	//if(imagePoints.size() == 0 || dst.imagePoints.size() == 0) {
	if (!ready) {
		gLogManager.log( "getTransformation() requires both Calibration objects to have just been calibrated",ELL_WARNING);
		return false;
	}
	if (imagePoints.size() != dst.imagePoints.size() || patternSize != dst.patternSize) {
		gLogManager.log("getTransformation() requires both Calibration objects to be trained simultaneously on the same board", ELL_WARNING);
		return false;
	}
	Mat fundamentalMatrix, essentialMatrix;
	Mat cameraMatrix = distortedIntrinsics.getCameraMatrix();
	Mat dstCameraMatrix = dst.getDistortedIntrinsics().getCameraMatrix();
	// uses CALIB_FIX_INTRINSIC by default
	stereoCalibrate(objectPoints,
		imagePoints, dst.imagePoints,
		cameraMatrix, distCoeffs,
		dstCameraMatrix, dst.distCoeffs,
		distortedIntrinsics.getImageSize(), rotation, translation,
		essentialMatrix, fundamentalMatrix);
	return true;
}
float CVCalibration::getReprojectionError() const {
	return reprojectionError;
}
float CVCalibration::getReprojectionError(int i) const {
	return perViewErrors[i];
}
const Intrinsics& CVCalibration::getDistortedIntrinsics() const {
	return distortedIntrinsics;
}
const Intrinsics& CVCalibration::getUndistortedIntrinsics() const {
	return undistortedIntrinsics;
}
Mat CVCalibration::getDistCoeffs() const {
	return distCoeffs;
}
int CVCalibration::size() const {
	return imagePoints.size();
}
cv::Size CVCalibration::getPatternSize() const {
	return patternSize;
}
float CVCalibration::getSquareSize() const {
	return squareSize;
}
void CVCalibration::customDraw() {
	for (int i = 0; i < size(); i++) {
		draw(i);
	}
}
void CVCalibration::draw(int i) const {
	/*
	ofPushStyle();
	ofNoFill();
	ofSetColor(ofColor::red);
	for (int j = 0; j < (int)imagePoints[i].size(); j++) {
		ofCircle(toOf(imagePoints[i][j]), 5);
	}
	ofPopStyle();*/
}
// this won't work until undistort() is in pixel coordinates
/*
void CVCalibration::drawUndistortion() const {
vector<ofVec2f> src, dst;
cv::Point2i divisions(32, 24);
for(int y = 0; y < divisions.y; y++) {
for(int x = 0; x < divisions.x; x++) {
src.push_back(ofVec2f(
ofMap(x, -1, divisions.x, 0, addedImageSize.width),
ofMap(y, -1, divisions.y, 0, addedImageSize.height)));
}
}
undistort(src, dst);
ofMesh mesh;
mesh.setMode(OF_PRIMITIVE_LINES);
for(int i = 0; i < src.size(); i++) {
mesh.addVertex(src[i]);
mesh.addVertex(dst[i]);
}
mesh.draw();
}
*/
void CVCalibration::draw3d() const {
	for (int i = 0; i < size(); i++) {
		draw3d(i);
	}
}
void CVCalibration::draw3d(int i) const {
/*	ofPushStyle();
	ofPushMatrix();
	ofNoFill();

	applyMatrix(makeMatrix(boardRotations[i], boardTranslations[i]));

	ofSetColor(ofColor::fromHsb(255 * i / size(), 255, 255));

	ofDrawBitmapString(core::StringConverter::toString((i), 0, 0);

	for (int j = 0; j < (int)objectPoints[i].size(); j++) {
		ofPushMatrix();
		ofTranslate(toOf(objectPoints[i][j]));
		ofCircle(0, 0, .5);
		ofPopMatrix();
	}

	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
	for (int j = 0; j < (int)objectPoints[i].size(); j++) {
		ofVec3f cur = toOf(objectPoints[i][j]);
		mesh.addVertex(cur);
	}
	mesh.draw();

	ofPopMatrix();
	ofPopStyle(); */
}
void CVCalibration::updateObjectPoints() {
	vector<Point3f> points = createObjectPoints(patternSize, squareSize, patternType);
	objectPoints.resize(imagePoints.size(), points);
}
void CVCalibration::updateReprojectionError() {
	vector<Point2f> imagePoints2;
	int totalPoints = 0;
	double totalErr = 0;

	perViewErrors.clear();
	perViewErrors.resize(objectPoints.size());

	for (int i = 0; i < (int)objectPoints.size(); i++) {
		projectPoints(Mat(objectPoints[i]), boardRotations[i], boardTranslations[i], distortedIntrinsics.getCameraMatrix(), distCoeffs, imagePoints2);
		double err = norm(imagePoints[i].getMat(), Mat(imagePoints2), CV_L2);
		int n = objectPoints[i].size();
		perViewErrors[i] = sqrt(err * err / n);
		totalErr += err * err;
		totalPoints += n;
		gLogManager.log("view " + core::StringConverter::toString(i) + " has error of " + core::StringConverter::toString(perViewErrors[i]), ELL_INFO);
	}

	reprojectionError = sqrt(totalErr / totalPoints);

	gLogManager.log("all views have error of " + core::StringConverter::toString(reprojectionError), ELL_INFO);
}
void CVCalibration::updateUndistortion() {
	Mat undistortedCameraMatrix = getOptimalNewCameraMatrix(distortedIntrinsics.getCameraMatrix(), distCoeffs, distortedIntrinsics.getImageSize(), fillFrame ? 0 : 1);
	initUndistortRectifyMap(distortedIntrinsics.getCameraMatrix(), distCoeffs, Mat(), undistortedCameraMatrix, distortedIntrinsics.getImageSize(), CV_16SC2, undistortMapX, undistortMapY);
	undistortedIntrinsics.setup(undistortedCameraMatrix, distortedIntrinsics.getImageSize());
}

vector<Point3f> CVCalibration::createObjectPoints(cv::Size patternSize, float squareSize, CalibrationPattern patternType) {
	vector<Point3f> corners;
	switch (patternType) {
	case CHESSBOARD:
	case CIRCLES_GRID:
		for (int i = 0; i < patternSize.height; i++)
		for (int j = 0; j < patternSize.width; j++)
			corners.push_back(Point3f(float(j * squareSize), float(i * squareSize), 0));
		break;
	case ASYMMETRIC_CIRCLES_GRID:
		for (int i = 0; i < patternSize.height; i++)
		for (int j = 0; j < patternSize.width; j++)
			corners.push_back(Point3f(float(((2 * j) + (i % 2)) * squareSize), float(i * squareSize), 0));
		break;
	}
	return corners;
}
}
}