
#ifndef ___WinCursorControl___
#define ___WinCursorControl___

#include <ICursorControl.h>
#include <GCPtr.h>
#include "compileConfig.h"
//#include <GUIAnimatedSprite.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace mray{

class MRAY_Win32_DLL WinCursorControl:public ICursorControl
{
	math::Point2di pos;
	math::Point2di Border;
	bool visible;
	HWND hWnd;
	//GCPtr<GUI::GUIAnimatedSprite> map;
	HCURSOR hCursor;

	bool useCursorMap;
	bool hotCenter;

	bool m_midScreen;

	bool m_fullscreen;

	math::Point2df invWindowRect;
	math::Point2di windowRect;

	ECursor m_cursor;

public:
	void updatePos();
	WinCursorControl(HWND hWnd,const math::vector2di &winSize,bool fullscreen);
	virtual~WinCursorControl();

	ECursor getCursor();
	void  setCursor(ECursor c);

	void setVisible(bool v);
	void setToMidScreen(bool s);

	math::Point2di getFinalPos(int x,int y);
	math::Point2di getFinalPos(math::Point2di p);

	void setFullscreen(bool fs);

	bool isVisible();
	bool isMidScreen();
	void setPosition(math::Point2di p);
	void setPosition(float x,float y);
	void setPosition(int x,int y);
	void updateBorder();

	virtual void setWindowSize(const math::vector2di&sz);

	//GUI::GUIAnimatedSprite* getMap();
	//void SetMouseMap(GUI::GUIAnimatedSprite* img);
	//void SetMouseMap( mchar*name);
	void CursorMap(bool use);
	math::Point2di getPosition();
	math::Point2df getRelativePosition();
	void useHotCenter(bool b);

	//void drawCursor(float dt);

	void resetHWND(HWND hwnd);


};

};//mray

#endif


