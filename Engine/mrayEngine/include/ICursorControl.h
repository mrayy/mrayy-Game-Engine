
/********************************************************************
	created:	2009/02/28
	created:	28:2:2009   0:10
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ICursorControl.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ICursorControl
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ICursorControl___
#define ___ICursorControl___


#include "ISingleton.h"


namespace mray{/*
	namespace GUI{
		class GUIAnimatedSprite;
	}*/

	enum ECursor
	{
		EC_Arrow,
		EC_Cross,
		EC_IBeam,
		EC_No,
		EC_SizeAll,
		EC_SizeNESW,
		EC_SizeNS,
		EC_SizeNWSE,
		EC_SizeWE,
		EC_UpArrow,
		EC_WaitArrow,
		EC_HSplit,
		EC_VSplit,
		EC_Hand

	};
class MRAY_DLL ICursorControl:public ISingleton<ICursorControl>
{

public:
	ICursorControl();
	virtual~ICursorControl();

	virtual ECursor getCursor()=0;
	virtual void  setCursor(ECursor c)=0;

	virtual void setVisible(bool v)=0;
	virtual void setToMidScreen(bool s)=0;

	virtual math::Point2di getFinalPos(int x,int y)=0;
	virtual math::Point2di getFinalPos(math::Point2di p)=0;

	virtual void updatePos()=0;

	virtual bool isVisible()=0;
	virtual bool isMidScreen()=0;
	virtual void setPosition(math::Point2di p)=0;
	virtual void setPosition(float x,float y)=0;
	virtual void setPosition(int x,int y)=0;
	virtual void updateBorder()=0;

	virtual void setFullscreen(bool fs)=0;

//	virtual GUI::GUIAnimatedSprite* getMap()=0;
//	virtual void SetMouseMap(GUI::GUIAnimatedSprite* img)=0;
//	virtual void SetMouseMap( mchar*name)=0;
	virtual void CursorMap(bool use)=0;
	virtual math::Point2di getPosition()=0;
	virtual math::Point2df getRelativePosition()=0;
	virtual void useHotCenter(bool b)=0;

	virtual void setWindowSize(const math::vector2di&sz)=0;

//	virtual void drawCursor(float dt)=0;

	static const mchar*getCursorStr(ECursor c);
};
#define gCursorControl mray::ICursorControl::getInstance()


}


#endif //___ICursorControl___

