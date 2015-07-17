
#include "stdafx.h"
#include "CVChessBoard.h"
#include "CVUtilities.h"
#include "CVWrappers.h"
#include "Engine.h"


namespace mray
{
namespace video
{
	class CVChessBoardImpl
	{
	public:

		CVChessBoardImpl() :
			subpixelSize(11, 11)
		{
			squares.set(8, 7);
			squareSize = 2.5;
		}
		float squareSize;
		math::vector2di squares;
		cv::Mat grayMat;
		cv::Size subpixelSize;
	};

CVChessBoard::CVChessBoard()
{
	m_impl = new CVChessBoardImpl();
}
CVChessBoard::~CVChessBoard()
{
	delete m_impl;
}


void CVChessBoard::Setup(math::vector2di squares, float squaresize)
{
	m_impl->squares = squares;
	m_impl->squareSize = squaresize;
}
const math::vector2di& CVChessBoard::GetSquares()
{
	return m_impl->squares;
}

void CVChessBoard::Draw(const math::rectf& rc)
{
	video::IVideoDevice* dev = gEngine.getDevice();

	int idx = 0;

	math::vector2d squareSize= rc.getSize() / (m_impl->squares+1);

	for (int i = 0; i <= m_impl->squares.x; i++)
	for (int j = 0; j <= m_impl->squares.y; j++)
	{
		float color = (i % 2 == j % 2) ? 0 : 1;

		math::rectf s;
		s.ULPoint = rc.ULPoint + math::vector2d(i, j)*squareSize;
		s.BRPoint = s.ULPoint + squareSize;

		dev->draw2DRectangle(s, video::SColor(color, color, color, 1));
	}
	dev->draw2DRectangle(math::rectf(rc.ULPoint.x, rc.ULPoint.y, rc.ULPoint.x + squareSize.x / 2, rc.BRPoint.y), 1);
	dev->draw2DRectangle(math::rectf(rc.ULPoint.x, rc.ULPoint.y, rc.BRPoint.x, rc.ULPoint.y + squareSize.y / 2), 1);
	dev->draw2DRectangle(math::rectf(rc.ULPoint.x, rc.BRPoint.y - squareSize.y / 2, rc.BRPoint.x, rc.BRPoint.y), 1);
	dev->draw2DRectangle(math::rectf(rc.BRPoint.x - squareSize.x / 2, rc.ULPoint.y, rc.BRPoint.x, rc.BRPoint.y), 1);

}

void CVChessBoard::GetProjectionPoints(std::vector<math::vector2d>& outList)
{
	for (int i = 0; i < m_impl->squares.x; i++)
	for (int j = 0; j < m_impl->squares.y; j++)
		outList.push_back(math::vector2d(float(j * m_impl->squareSize), float(i * m_impl->squareSize)));
}

bool CVChessBoard::FindInImage(const video::ImageInfo* src, std::vector<math::vector2d>& outList,bool refine)
{

	int chessFlags = CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_ADAPTIVE_THRESH;// CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE;// ;
	cv::Mat img = toCv(src);
	bool found = findChessboardCorners(img, cv::Size(m_impl->squares.x , m_impl->squares.y ), *(vector<Point2f>*)&outList, chessFlags);

	// improve corner accuracy
	if (found) {
		if (img.type() != CV_8UC1) {
			copyGray(img, m_impl->grayMat);
		}
		else {
			m_impl->grayMat = img;
		}

		if (refine) {
			// the 11x11 dictates the smallest image space square size allowed
			// in other words, if your smallest square is 11x11 pixels, then set this to 11x11
			cornerSubPix(m_impl->grayMat, *(vector<Point2f>*)&outList, m_impl->subpixelSize, cv::Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
		}
	}
	return found;

}

}
}

