
#include "stdafx.h"
#include "CVChessBoard.h"
#include "CVUtilities.h"
#include "Engine.h"


namespace mray
{
namespace video
{

CVChessBoard::CVChessBoard()
{
	m_squares.set(8, 7);
	m_squareSize = 2.5;
}
CVChessBoard::~CVChessBoard()
{

}
void CVChessBoard::Draw(const math::rectf& rc)
{
	video::IVideoDevice* dev = gEngine.getDevice();

	int idx = 0;

	math::vector2d squareSize= rc.getSize() / (m_squares+1);

	for (int i = 0; i <= m_squares.x; i++)
	for (int j = 0; j <= m_squares.y; j++)
	{
		float color = (i % 2 == j % 2) ? 0 : 1;

		math::rectf s;
		s.ULPoint = rc.ULPoint + math::vector2d(i, j)*squareSize;
		s.BRPoint = s.ULPoint + squareSize;

		dev->draw2DRectangle(s, video::SColor(color, color, color, 1));
	}
	
}

void CVChessBoard::GetProjectionPoints(std::vector<math::vector2d>& outList)
{
	for (int i = 0; i < m_squares.x; i++)
	for (int j = 0; j < m_squares.y; j++)
		outList.push_back(math::vector2d(float(j * m_squareSize), float(i * m_squareSize)));
}

bool CVChessBoard::FindInImage(const video::ImageInfo* src, std::vector<math::vector2d>& outList)
{

	int chessFlags = CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK;// ;
	cv::Mat img = toCv(src);
	bool found = findChessboardCorners(img, cv::Size(m_squares.x , m_squares.y ), *(vector<Point2f>*)&outList, chessFlags);

	return found;

}

}
}

