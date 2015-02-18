
#ifndef ___GUIGRID___
#define ___GUIGRID___
	
#include "GUIElement.h"
#include "SColor.h"
#include "IFont.h"
#include "mArray.h"

namespace mray{
namespace GUI{

class MRAY_DLL GUIGrid:public GUIElement
{
	typedef std::vector<core::string> arrStr;
	GCPtr<GUI::IFont> font;

	std::vector<core::string>	columnsCaption;
	std::vector<core::string>	rowsCaption;
	std::vector<arrStr>	cellsData;

	std::vector<int>		columnsWidth;

	FontAttributes m_FontAttributs;

	int firstCellWidth;

	int currCellCol;
	int currCellRow;

	int columnsCount;
	int rowsCount;
public:
	DECLARE_RTTI;

	GUIGrid(GUIManager* manager,int colCount,int rowCount,math::Point2df pos,math::Point2df size,IEventReciver* event=0,
		GCPtr<GUIElement>  parent=0,video::SColor color=240,int id=0);
	virtual~GUIGrid();
	
	void setRowCount(int count);
	void setColumnCount(int count);

	void addColumn(const  mchar*Caption,int pos=-1);
	void addRow(const  mchar*Caption,int pos=-1);

	void removeColumn(int pos);
	void removeRow(int pos);

	void setRowCaption(const  mchar*Caption,int row);
	void setColsCaption(const  mchar*Caption,int col);

	const  mchar*getRowCaption(int row);
	const  mchar*getColCaption(int col);

	int getColumnsCount(){return columnsCount;}
	int getRowsCount(){return rowsCount;}

	void setCellData(int r,int c,core::string&str);
	const  mchar*getCellData(int r,int c);

	void clearGrid();
	void clearCellsData();

	void setColumnWidth(int col,int width);
	int	 getColumnWidth(int col);

	
	virtual void draw(float dt);
	virtual void onEvent(const EventPtr&  event);


	virtual FontAttributes* getFontAttributes();

	virtual GCPtr<GUIElement> getDuplicate();
};

}
}


#endif