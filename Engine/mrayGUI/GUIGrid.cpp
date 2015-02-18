

#include "GUIElement.h"
#include "GUIGrid.h"
#include "SColor.h"
#include "GUIThemeManager.h"
#include "GUIManager.h"
#include "KeyCode.h"

#include "MouseEvent.h"
#include "GUIEvent.h"
#include "KeyboardEvent.h"
#include "EventMemoryManager.h"

#include "IEventReciver.h"
#include "ILogManager.h"
#include "FontResourceManager.h"

namespace mray{
namespace GUI{

GUIGrid::GUIGrid(GUIManager* manager,int colCount,int rowCount,math::Point2df pos,math::Point2df size,IEventReciver* event,
		GCPtr<GUIElement>  parent,video::SColor color,int id):
GUIElement(manager,parent,pos,size,event,id),columnsCount(colCount),rowsCount(rowCount),font(0),
currCellCol(0),currCellRow(0),firstCellWidth(0)
{
#ifdef ___DEBUG___
	setDebugName("GUIGrid");
#endif
	Color=color;
	Selectable=true;
	
	setTypeStr(mT("Grid"));

	if(colCount==0 || rowCount==0)return;
	columnsCaption.resize(colCount);
	rowsCaption.resize(rowCount);
	columnsWidth.resize(colCount);
//	rowsHeight.resize(rowCount);

	cellsData.resize(rowCount);
	for(int i=0;i<rowCount;++i){
		cellsData[i].resize(colCount);
	}

	int width=floor((float)(MainSize().x)/(float)(columnsCount+1));
	firstCellWidth=width;
	for(int i=0;i<colCount;++i)
		columnsWidth[i]=width;

}
GUIGrid::~GUIGrid()
{
	columnsCaption.clear();
	rowsCaption.clear();
	for(int i=0;i<rowsCount;++i)
		cellsData[i].clear();
	cellsData.clear();
	columnsWidth.clear();
}

void GUIGrid::setRowCount(int count)
{
	rowsCount=count;
	rowsCaption.resize(count);

	cellsData.resize(count);
}
void GUIGrid::setColumnCount(int count)
{
	columnsCount=count;
	columnsCaption.resize(count);
	columnsWidth.resize(count);

	for(int i=0;i<rowsCount;++i){
		cellsData[i].resize(count);
	}

	int width=floor((float)(MainSize().x)/(float)(columnsCount+1));
	firstCellWidth=width;
	for(int i=0;i<count;++i)
		columnsWidth[i]=width;
}
void GUIGrid::addColumn(const  mchar*Caption,int pos){
	if(pos!=-1){
		columnsCaption.insert(Caption,pos);
		for(int i=0;i<rowsCount;++i){
			cellsData[i].insert(core::string(),pos);
		}
	}else{
		columnsCaption.push_back(Caption);
		for(int i=0;i<rowsCount;++i){
			cellsData[i].push_back(core::string());
		}
	}
	columnsCount++;
	columnsWidth.resize(columnsCount);
	int width=floor((float)(MainSize().x)/(float)(columnsCount+1));
	firstCellWidth=width;
	for(int i=0;i<columnsCount;++i)
		columnsWidth[i]=width;

}
void GUIGrid::addRow(const  mchar*Caption,int pos){
	if(pos!=-1){
		rowsCaption.insert(Caption,pos);
		cellsData.insert(std::vector<core::string>(),pos);
		cellsData[pos].resize(columnsCount);
	}else{
		rowsCaption.push_back(Caption);
		cellsData.push_back(std::vector<core::string>());
		cellsData.getLast().resize(columnsCount);
	}
	rowsCount++;
}

void GUIGrid::removeColumn(int pos){
	if(pos>=columnsCount){
		gLogManager.log(mT("GUIGrid::removeColumn()-out of range"),ELL_WARNING);
		return;
	}
	columnsCaption.erase(pos);
	for(int i=0;i<columnsCount;++i){
		cellsData[i].erase(pos);
	}
	columnsCount--;
	if(columnsCount==0)return;
	
	int width=floor((float)(MainSize().x)/(float)(columnsCount+1));
	firstCellWidth=width;
	for(int i=0;i<columnsCount;++i)
		columnsWidth[i]=width;
}
void GUIGrid::removeRow(int pos){
	if(pos>=rowsCount){
		gLogManager.log(mT("GUIGrid::removeRow()-out of range"),ELL_WARNING);
		return;
	}
	rowsCaption.erase(pos);
	cellsData.erase(pos);
	rowsCount--;
}

void GUIGrid::setRowCaption(const  mchar*Caption,int row){
	if(row>=rowsCount){
		gLogManager.log(mT("GUIGrid::setRowCaption()-out of range"),ELL_WARNING);
		return;
	}
	rowsCaption[row]=Caption;
}
void GUIGrid::setColsCaption(const  mchar*Caption,int col){
	if(col>=columnsCount){
		gLogManager.log(mT("GUIGrid::setColsCaption()-out of range"),ELL_WARNING);
		return;
	}
	columnsCaption[col]=Caption;
}

const  mchar*GUIGrid::getRowCaption(int row){
	if(row>=rowsCount){
		gLogManager.log(mT("GUIGrid::getRowCaption()-out of range"),ELL_WARNING);
		return 0;
	}
	return rowsCaption[row].c_str();
}
const  mchar*GUIGrid::getColCaption(int col){
	if(col>=columnsCount){
		gLogManager.log(mT("GUIGrid::getColCaption()-out of range"),ELL_WARNING);
		return 0;
	}
	return columnsCaption[col].c_str();
}


void GUIGrid::setCellData(int r,int c,core::string&str){
	if(r>=rowsCount || c>=columnsCount){
		gLogManager.log(mT("GUIGrid::setCellData()-out of range"),ELL_WARNING);
		return;
	}
	cellsData[r][c]=str;
}
const  mchar*GUIGrid::getCellData(int r,int c){
	if(r>=rowsCount || c>=columnsCount){
		gLogManager.log(mT("GUIGrid::getCellData()-out of range"),ELL_WARNING);
		return 0;
	}
	return cellsData[r][c].c_str();
}

void GUIGrid::clearGrid(){
	columnsCaption.clear();
	rowsCaption.clear();
	cellsData.clear();
	columnsWidth.clear();
}
void GUIGrid::setColumnWidth(int col,int width){
	if(col>=columnsCount){
		gLogManager.log(mT("GUIGrid::setColumnWidth()-out of range"),ELL_WARNING);
		return;
	}
	columnsWidth[col]=width;
}
int	 GUIGrid::getColumnWidth(int col){
	if(col>=columnsCount){
		gLogManager.log(mT("GUIGrid::getColumnWidth()-out of range"),ELL_WARNING);
		return 0;
	}
	return columnsWidth[col];
}
void GUIGrid::clearCellsData(){
	for(int r=0;r<rowsCount;r++)
	for(int c=0;c<columnsCount;c++)
		cellsData[r][c]=mT("");
}


void GUIGrid::draw(float dt)
{
	if(!Visible)return;
	IGUITheme* skin=GUIThemeManager::getInstance().getActiveTheme(); 	
	math::rectf tRect=getRect();
	font=gFontResourceManager.getFontByName(m_FontAttributs.fontName);;
	if(!font)
		return;

	math::Point2df charDim=font->getCharDimension('A',m_FontAttributs.fontSize);
	charDim.y+=5;

	video::SColor bclr=Color();
	bclr.A=AlphaEffect ? Alpha:Color().A;
	
	video::SColor clr;
	
	clr=m_FontAttributs.fontColor;
	if(!Enable)
		m_FontAttributs.fontColor=video::SColor(128,128,128,m_FontAttributs.fontColor.A);

	m_FontAttributs.fontColor.A=AlphaEffect ? Alpha:m_FontAttributs.fontColor.A;
	
	//first draw the top Captions:
	//[-][0][1][2][3]..
	math::rectf currCell;
	currCell.ULPoint=tRect.ULPoint;
	currCell.BRPoint=currCell.ULPoint+math::Point2df(firstCellWidth,charDim.y);
	skin->drawBox(getManager()->GetRenderQueue(),currCell,bclr*0.8,bclr*0.5,&getClipRect());
	font->print(currCell,&m_FontAttributs,0,getRenderString(),guiMngr->GetRenderQueue());
	currCell.ULPoint.x+=firstCellWidth;
	for(int i=0;i<columnsCount && currCell.BRPoint.x-tRect.ULPoint.x<MainSize().x;++i){
		currCell.BRPoint.x+=columnsWidth[i];
		skin->drawBox(getManager()->GetRenderQueue(),currCell,bclr*0.8,bclr*0.5,&getClipRect());
		//skin->drawPanel(currCell,video::SColor(244,244,244,bclr.A));
//		font->print(currCell,&m_FontAttributs,0,columnsCaption[i],guiMngr->GetRenderQueue());
		currCell.ULPoint.x+=columnsWidth[i];
	}
	for(int r=0;r<rowsCount && currCell.BRPoint.y-tRect.ULPoint.y<MainSize().y ;r++){
		currCell.ULPoint.x=tRect.ULPoint.x;
		currCell.ULPoint.y+=charDim.y;
		currCell.BRPoint.y=currCell.ULPoint.y+charDim.y;
		currCell.BRPoint.x=currCell.ULPoint.x+firstCellWidth;

		skin->drawBox(getManager()->GetRenderQueue(),currCell,bclr,bclr*0.5,&getClipRect());
		//skin->drawPanel(currCell,video::SColor(244,244,244,255));
//		font->print(currCell,&m_FontAttributs,0,rowsCaption[r],guiMngr->GetRenderQueue());
		currCell.ULPoint.x+=firstCellWidth;
		for(int c=0;c<columnsCount && currCell.BRPoint.x-tRect.ULPoint.x<MainSize().x;c++){
			currCell.BRPoint.x+=columnsWidth[c];
			skin->drawBox(getManager()->GetRenderQueue(),currCell,bclr,bclr*0.5,&getClipRect());
			//skin->drawPanel(currCell,video::SColor(255,255,255,bclr.A));
//			font->print(currCell,&m_FontAttributs,0,cellsData[r][c],guiMngr->GetRenderQueue());
			currCell.ULPoint.x+=columnsWidth[c];
		}
	}

	m_FontAttributs.fontColor=clr;

	if(guiMngr->hasFocus(this))
	{
		math::rectf rc;
		rc.ULPoint=tRect.ULPoint;
		rc.ULPoint.x+=firstCellWidth;//adding FIRST_CELL_WIDTH for first Caption cell
		for(int i=0;i<currCellCol;++i)//find cell Pos x
			rc.ULPoint.x+=columnsWidth[i];
		
		rc.ULPoint.y+=(currCellRow+1)*charDim.y;
		
		rc.BRPoint.x=rc.ULPoint.x+columnsWidth[currCellCol];
		rc.BRPoint.y=rc.ULPoint.y+charDim.y;

		guiMngr->getDevice()->draw2DRectangle(rc,video::SColor(128,128,255,bclr.A),1,0);
	}
	GUIElement::draw(dt);
}


void GUIGrid::onEvent(const EventPtr& e)
{
	if(!Visible)return;
	if(!Enable)return;
	if(!parentEnabled)return;
	bool ok=0;
	GUIElement::onEvent(e);
	GUI_EVENT_TYPE EventType=GET_NONE;
	math::rectf tRect=getRect();
	tRect.clipRect(clipRect);
	switch(e->getType())
	{
	case ET_Keyboard:
	{
		GCPtr<KeyboardEvent> event=e;

		if(guiMngr->hasFocus(this))
		{
			switch(event->key){
			case KEY_LEFT:
				currCellCol--;
				if(currCellCol<0){
					if(currCellRow>0){
						currCellCol=columnsCount-1;
						currCellRow--;
					}
					else currCellCol=0;
				}
				ok=1;
			break;
			case KEY_RIGHT:
				currCellCol++;
				if(currCellCol>=columnsCount){
					if(currCellRow<rowsCount-1){
						currCellCol=0;
						currCellRow++;
					}
					else currCellCol=columnsCount-1;
				}
				ok=1;
			break;
			case KEY_UP:
				currCellRow--;
				if(currCellRow<0){
					if(currCellCol>0){
						currCellRow=rowsCount-1;
						currCellCol--;
					}
					else currCellRow=0;
				}
				ok=1;
			break;
			case KEY_DOWN:
				currCellRow++;
				if(currCellRow>=rowsCount){
					if(currCellCol<columnsCount-1){
						currCellRow=0;
						currCellCol++;
					}
					else currCellRow=rowsCount-1;
				}
				ok=1;
			break;
			}
		}
		}break;
	case ET_Mouse:
	{
		GCPtr<MouseEvent> event=e;
		if(tRect.IsPointInside(math::vector2d(event->x,event->y))&&
			!event->isRecived())
		{
			if(event->Event==MET_MOVED)
			{
				EventType=GET_Mouse_MOVE;
				if(!guiMngr->hasMouseOn(this))
				{
					guiMngr->setMouseOn(this);
				}
				ok=1;
			}else
			if(event->Event==MET_LEFTDOWN)
			{
				EventType=GET_Mouse_DOWN;
				ok=1;
			}
			else if(event->Event==MET_LEFTUP)
			{
				EventType=GET_Mouse_UP;
				ok=1;
			}
		}else
		{
			showHint(0);
			if(event->Event==MET_LEFTDOWN || 
				event->Event==MET_LEFTUP)
			{
			}
		}
		}break;
	};


	if(ok){
		e->setRecived();
		GCPtr<GUIEvent> event1=EventMemoryManager::getInstance().createEvent(ET_GuiEvent);
		event1->Caller=this;
		event1->Event=EventType;
		guiMngr->onEvent(event1);
		if(m_event)
			m_event->onEvent(event1);
	}
}


GCPtr<GUIElement> GUIGrid::getDuplicate()
{
	GUIGrid*dup=guiMngr->addGrid(columnsCount,rowsCount,getPos(),getSize(),m_event,getParent(),Color(),getId());

	dup->firstCellWidth=firstCellWidth;

	dup->columnsCount=columnsCount;
	dup->rowsCount=rowsCount;

	dup->columnsCaption.resize(columnsCaption.size());
	for(int i=0;i<columnsCaption.size();++i)
		dup->columnsCaption[i]=columnsCaption[i];
	
	dup->rowsCaption.resize(rowsCaption.size());
	for(int i=0;i<rowsCaption.size();++i)
		dup->rowsCaption[i]=rowsCaption[i];

	dup->columnsWidth.resize(columnsWidth.size());
	for(int i=0;i<columnsWidth.size();++i)
		dup->columnsWidth[i]=columnsWidth[i];


	dup->cellsData.resize(cellsData.size());
	for(int i=0;i<cellsData.size();++i)
	{
		dup->cellsData[i].resize(cellsData[i].size());
		for(int j=0;j<cellsData[i].size();j++)
			cellsData[i][j]=cellsData[i][j];
	}

	return dup;
}

FontAttributes* GUIGrid::getFontAttributes(){
	return &m_FontAttributs;
}

}
}
