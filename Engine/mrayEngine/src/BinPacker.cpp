


#include "stdafx.h"
#include "BinPacker.h"

#include <algorithm>


namespace mray
{
namespace core
{
// ---------------------------------------------------------------------------

bool BinPacker::Pack(
	const std::vector<math::vector2df>&          rects,
	std::vector< BinPackerOutRect >& packs,
	const math::vector2d & packSize,
	bool allowRotation)
{

	Clear();



	m_packSize = packSize;



	// Add rects to member array, and check to make sure none is too big

	for (size_t i = 0; i < rects.size(); i ++) {

		if (rects[i].x > m_packSize.x || rects[i].y > m_packSize.y) {

			//assert(!"All rect dimensions must be <= the pack size");

			return false;
		}

		m_rects.push_back(Rect(rects[i], i));

	}



	// Sort from greatest to least area
	std::sort(m_rects.rbegin(), m_rects.rend());



	// Pack
	while (m_numPacked < (int)m_rects.size()) {
		int i = m_packs.size();
		m_packs.push_back(Rect(m_packSize));
		m_roots.push_back(i);
		Fill(i, allowRotation);
	}



	// Write out
	packs.resize(m_roots.size());
	for (size_t i = 0; i < m_roots.size(); ++i) {
		AddPackToArray(m_roots[i], packs);
	}



	// Check and make sure all rects were packed

	for (size_t i = 0; i < m_rects.size(); ++i) {
		if (!m_rects[i].packed) {
			//assert(!"Not all rects were packed");
			return false;
		}
	}
	return true;
}

bool BinPacker::AddToPack(const math::vector2df& rect, BinPackerOutRect& pack, bool allowRotation)
{

	if (rect.x > m_packSize.x || rect.y > m_packSize.y) {

		//assert(!"All rect dimensions must be <= the pack size");

		return false;
	}
	int i = m_rects.size();

	//add the rect
	m_rects.push_back(Rect(rect, i));
	//find empty pack

	for (int p = 0; p < m_packs.size(); ++p)
	{
		if (!m_packs[p].packed && Fits(m_rects[i], m_packs[p], allowRotation))
		{
			Split(p, i);
			pack = m_packs[p].rectInfo;
			return true;
		}
	}

	return false;
}

// ---------------------------------------------------------------------------

void BinPacker::Clear()

{

	m_packSize = 0;

	m_numPacked = 0;

	m_rects.clear();

	m_packs.clear();

	m_roots.clear();

}

// ---------------------------------------------------------------------------

int BinPacker::Fill(int pack, bool allowRotation)

{

	if (!PackIsValid(pack))
		return 0;



	int i = pack;

	int ret = 0;


	// For each rect

	for (size_t j = 0; j < m_rects.size(); ++j) {

		// If it's not already packed

		if (!m_rects[j].packed) {

			// If it fits in the current working area

			if (Fits(m_rects[j], m_packs[i], allowRotation)) {

				// Store in lower-left of working area, split, and recurse

				++m_numPacked;

				Split(i, j);
				ret = 1;
				ret += Fill(m_packs[i].children[1], allowRotation);

				ret += Fill(m_packs[i].children[0], allowRotation);

				return ret;

			}

		}

	}
	return ret;
}

// ---------------------------------------------------------------------------

void BinPacker::Split(int pack, int rect)

{

	assert(PackIsValid(pack));

	assert(RectIsValid(rect));



	int i = pack;

	int j = rect;



	// Split the working area either horizontally or vertically with respect

	// to the rect we're storing, such that we get the largest possible child

	// area.



	Rect left = m_packs[i];

	Rect right = m_packs[i];

	Rect bottom = m_packs[i];

	Rect top = m_packs[i];

	float w = m_rects[j].size.x;
	float h = m_rects[j].size.y;

	left.rectInfo.pos.y += h;
	left.size.x = w;
	left.size.y -= h;
	right.rectInfo.pos.x += w;
	right.size.x -= w;



	bottom.rectInfo.pos.x += m_rects[j].size.x;

	bottom.size.y = m_rects[j].size.y;

	bottom.size.x -= m_rects[j].size.x;

	top.rectInfo.pos.y += m_rects[j].size.y;

	top.size.y -= m_rects[j].size.y;



	float maxLeftRightArea = left.GetArea();

	if (right.GetArea() > maxLeftRightArea) {

		maxLeftRightArea = right.GetArea();

	}



	float maxBottomTopArea = bottom.GetArea();

	if (top.GetArea() > maxBottomTopArea) {

		maxBottomTopArea = top.GetArea();

	}



	if (maxLeftRightArea > maxBottomTopArea) {

		if (left.GetArea() > right.GetArea()) {

			m_packs.push_back(left);

			m_packs.push_back(right);

		}
		else {

			m_packs.push_back(right);

			m_packs.push_back(left);

		}

	}
	else {

		if (bottom.GetArea() > top.GetArea()) {

			m_packs.push_back(bottom);

			m_packs.push_back(top);

		}
		else {

			m_packs.push_back(top);

			m_packs.push_back(bottom);

		}

	}



	// This pack area now represents the rect we've just stored, so save the

	// relevant info to it, and assign children.

	m_packs[i].size = m_rects[j].size;

	m_packs[i].rectInfo.ID = m_rects[j].rectInfo.ID;

	m_packs[i].rectInfo.rotated = m_rects[j].rectInfo.rotated;

	m_packs[i].children[0] = m_packs.size() - 2;

	m_packs[i].children[1] = m_packs.size() - 1;

	m_packs[i].packed = true;

	// Done with the rect

	m_rects[j].packed = true;

}

// ---------------------------------------------------------------------------

bool BinPacker::Fits(Rect& rect1, const Rect& rect2, bool allowRotation)

{

	// Check to see if rect1 fits in rect2, and rotate rect1 if that will

	// enable it to fit.



	if (rect1.size.x <= rect2.size.x && rect1.size.y <= rect2.size.y) {

		return true;

	}
	else if (allowRotation && rect1.size.y <= rect2.size.x && rect1.size.x <= rect2.size.y) {

		rect1.Rotate();

		return true;

	}
	else {

		return false;

	}

}

// ---------------------------------------------------------------------------

void BinPacker::AddPackToArray(int pack, std::vector<BinPackerOutRect>& array) const

{

	if (!PackIsValid(pack))
		return;



	int i = pack;

	if (m_packs[i].rectInfo.ID != -1) {

		array.push_back(m_packs[i].rectInfo);



		if (m_packs[i].children[0] != -1) {

			AddPackToArray(m_packs[i].children[0], array);

		}

		if (m_packs[i].children[1] != -1) {

			AddPackToArray(m_packs[i].children[1], array);

		}

	}

}

// ---------------------------------------------------------------------------

bool BinPacker::RectIsValid(int i) const

{

	return i >= 0 && i < (int)m_rects.size();

}

// ---------------------------------------------------------------------------

bool BinPacker::PackIsValid(int i) const

{

	return i >= 0 && i < (int)m_packs.size();

}
}
}