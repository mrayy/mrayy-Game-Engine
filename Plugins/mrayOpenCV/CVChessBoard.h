

#ifndef __CVCHESSBOARD__
#define __CVCHESSBOARD__

#include "ImageInfo.h"

namespace mray
{
namespace video
{
	class CVChessBoardImpl;
	
class CVChessBoard
{
protected:

	CVChessBoardImpl* m_impl;
public:
	CVChessBoard();
	virtual ~CVChessBoard();

	void Setup(math::vector2di squares, float squaresize);
	const math::vector2di& GetSquares();

	void Draw(const math::rectf& rc);
	void GetProjectionPoints(std::vector<math::vector2d>& outList);
	bool FindInImage(const video::ImageInfo* src, std::vector<math::vector2d>& outList, bool refine=true);

};

}
}


#endif
