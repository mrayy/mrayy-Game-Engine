

#include "GUICellSprite.h"
#include "ITexture.h"

namespace mray{
namespace GUI{


GUICellSprite::GUICellSprite():GUIRotImage(),
	m_rows(0),m_cols(0),m_maxFrames(0),m_cell(0),
	Rows(this,&GUICellSprite::setRows,&GUICellSprite::getRows,EPT_INT,mT("Rows")),
	Cols(this,&GUICellSprite::setCols,&GUICellSprite::getCols,EPT_INT,mT("Cols")),
	MaxFrames(this,&GUICellSprite::setMaxFrames,&GUICellSprite::getMaxFrames,EPT_INT,mT("MaxFrames")),
	Cell(this,&GUICellSprite::setCell,&GUICellSprite::getCell,EPT_INT,mT("Cell"))
{
	setTypeStr(mT("GUICellSprite"));
}

GUICellSprite::GUICellSprite(GUIManager* manager,video::ITexturePtr  tex,int rowCnt,int colCnt,int framesCnt, 
									 math::vector2d pos,math::vector2d size,IEventReciver* event,GCPtr<GUIElement> parent,int id):
GUIRotImage(manager,pos,size,parent,id),m_rows(rowCnt),m_cols(colCnt),m_maxFrames(framesCnt),m_cell(0),
Rows(this,&GUICellSprite::setRows,&GUICellSprite::getRows,EPT_INT,mT("Rows")),
Cols(this,&GUICellSprite::setCols,&GUICellSprite::getCols,EPT_INT,mT("Cols")),
MaxFrames(this,&GUICellSprite::setMaxFrames,&GUICellSprite::getMaxFrames,EPT_INT,mT("MaxFrames")),
Cell(this,&GUICellSprite::setCell,&GUICellSprite::getCell,EPT_INT,mT("Cell"))
{
	setTypeStr(mT("GUICellSprite"));
	setTexture(tex);

}


GUICellSprite::~GUICellSprite(){
}



void GUICellSprite::draw(float dt){
	if(!Visible)return;
	if(!MaxFrames)return;
	if(m_textureUnit.pointer()!=0 && Rows!=0 && Cols!=0){

		float USize=(float)1.0f/(float)Cols;
		float VSize=(float)1.0f/(float)Rows;
		int x=m_cell % Cols;
		int y=m_cell / Cols;
		float currU=USize*x;
		float currV=VSize*y;

		math::rectf tcoords;
		tcoords.ULPoint.x=currU;
		tcoords.ULPoint.y=currV;
		tcoords.BRPoint.x=currU+USize;
		tcoords.BRPoint.y=currV+VSize;

		TexCoords=tcoords;
	}

	GUIRotImage::draw(dt);
}

void GUICellSprite::setRows(const int & rowCnt){
	m_rows=rowCnt;
	if(m_rows==0)m_rows=1;
}
void GUICellSprite::setCols(const int & colCnt){
	m_cols=colCnt;
	if(m_cols==0)m_cols=1;
}

}
}



