

#ifndef __CVCHESSBOARD__
#define __CVCHESSBOARD__

#include "ImageInfo.h"

namespace mray
{
namespace video
{
	
class CVChessBoard
{
protected:

	float m_squareSize;
	math::vector2di m_squares;
public:
	CVChessBoard();
	virtual ~CVChessBoard();

	void Setup(math::vector2di squares, float squaresize)
	{
		m_squares = squares; 
		m_squareSize = squaresize;
	}
	const math::vector2di& GetSquares(){ return m_squares; }

	void Draw(const math::rectf& rc);
	void GetProjectionPoints(std::vector<math::vector2d>& outList);
	bool FindInImage(const video::ImageInfo* src,std::vector<math::vector2d>& outList);

};

}
}


#endif
