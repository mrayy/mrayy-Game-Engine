
#ifndef BINPACKER_H

#define BINPACKER_H



#include <vector>
#include "compileconfig.h"

namespace mray
{
namespace core
{

	struct BinPackerOutRect
	{
		BinPackerOutRect() :ID(0), rotated(false)
		{}
		int ID;
		math::vector2d pos;
		bool rotated;
	};
class MRAY_DLL BinPacker
{
public:

	// The input and output are in terms of vectors of ints to avoid

	// dependencies (although I suppose a public member struct could have been

	// used). The parameters are:



	// rects : An array containing the width and height of each input rect in

	// sequence, i.e. [w0][h0][w1][h1][w2][h2]... The IDs for the rects are

	// derived from the order in which they appear in the array.



	// packs : After packing, the outer array contains the packs (therefore

	// the number of packs is packs.size()). Each inner array contains a

	// sequence of sets of 4 ints. Each set represents a rectangle in the

	// pack. The elements in the set are 1) the rect ID, 2) the x position

	// of the rect with respect to the pack, 3) the y position of the rect

	// with respect to the pack, and 4) whether the rect was rotated (1) or

	// not (0). The widths and heights of the rects are not included, as it's

	// assumed they are stored on the caller's side (they were after all the

	// input to the function).



	// allowRotation : when true (the default value), the packer is allowed

	// the option of rotating the rects in the process of trying to fit them

	// into the current working area.



	bool Pack(
		const std::vector<math::vector2df>&          rects,
		std::vector< BinPackerOutRect >& packs,
		const math::vector2d&                              packSize,
		bool                             allowRotation = true
		);

	bool AddToPack(const math::vector2df& rect, BinPackerOutRect& pack, bool allowRotation);

private:

	struct Rect
	{
		Rect(const math::vector2d& size, int ID = -1)
			: size( size), packed(false)
		{
			rectInfo.ID = ID;
			children[0] = -1;
			children[1] = -1;
		}



		Rect(const math::vector2d& pos, const math::vector2d& size, int ID = 1)
			: size(size),  packed(false)
		{
			rectInfo.ID = ID;
			rectInfo.pos = pos;
			children[0] = -1;
			children[1] = -1;
		}



		float GetArea() const {

			return size.x*size.y;

		}



		void Rotate() {
			
			std::swap(size.x, size.y);
			rectInfo.rotated = !rectInfo.rotated;
		}


		bool operator<(const Rect& rect) const {
			return GetArea() < rect.GetArea();
		}


		math::vector2d size;

		BinPackerOutRect rectInfo;

		int  children[2];

		bool packed;

	};



	void Clear();

	int Fill(int pack, bool allowRotation);

	void Split(int pack, int rect);

	bool Fits(Rect& rect1, const Rect& rect2, bool allowRotation);

	void AddPackToArray(int pack, std::vector<BinPackerOutRect>& rc) const;



	bool RectIsValid(int i) const;

	bool PackIsValid(int i) const;



	math::vector2d               m_packSize;

	int               m_numPacked;

	std::vector<Rect> m_rects;

	std::vector<Rect> m_packs;

	std::vector<int>  m_roots;

};

}
}

#endif // #ifndef BINPACKER_H




