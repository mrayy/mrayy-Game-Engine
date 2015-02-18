
#ifndef ___GUICellSprite___
#define ___GUICellSprite___


#include "GUIRotImage.h"

namespace mray{
namespace GUI{


class MRAY_DLL GUICellSprite:public GUIRotImage
{
protected:
	int m_rows;
	int m_cols;
	int m_maxFrames;
	int m_cell;

	const int& getRows() { return m_rows; }
	void setRows(const int &val) ;

	const int& getCols() { return m_cols; }
	void setCols(const int &val);

	const int& getMaxFrames() { return m_maxFrames; }
	void setMaxFrames(const int &val) { m_maxFrames = val; }

	const int& getCell() { return m_cell; }
	void setCell(const int &val) { m_cell = val; }

public:
	DECLARE_RTTI;

	rwProperty<GUICellSprite,int> Rows;
	rwProperty<GUICellSprite,int> Cols;
	rwProperty<GUICellSprite,int> MaxFrames;
	rwProperty<GUICellSprite,int> Cell;

	GUICellSprite();

	GUICellSprite(GUIManager* manager,video::ITexturePtr  tex,int rowCnt,int colCnt,int framesCnt,
		math::Point2df pos,math::Point2df size,IEventReciver* event=0,GCPtr<GUIElement> parent=0,int id=0);
	virtual ~GUICellSprite();

	virtual void draw(float dt);


};

}
}






#endif

