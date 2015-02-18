#include "stdafx.h"


#include "WinCursorControl.h"
//#include <GUIManager.h>
//#include <GUIAnimatedSprite.h>
//#include <TextureResourceManager.h>

namespace mray{




WinCursorControl::WinCursorControl(HWND hWnd,const math::vector2di &winSize,bool fullscreen):
	visible(1),hotCenter(0),useCursorMap(0),m_fullscreen(fullscreen)
{
	m_midScreen=false; 
	updateBorder();
/*	map=new GUIImage(math::vector2d(0,0),math::vector2d(25,25));
	map->SetDevice(hWindow->getDevice());
	map->SetManager(hWindow->getGUIMngr());
	map=hWindow->getGUIMngr()->Items.addAnimatedSprite(0,1,1,1,0,math::vector2d(0),math::vector2d(25));
	map->removeFromParent();
	map->setVisible(0);*/

	hWnd=hWnd;
	windowRect=winSize;

	if(windowRect.x)
		invWindowRect.x=1.0f/(float)(windowRect.x);
	if(windowRect.y)
		invWindowRect.y=1.0f/(float)(windowRect.y);
//		hCursor=GetCursor();

	setCursor(EC_Arrow);
}

WinCursorControl::~WinCursorControl()
{
}
void WinCursorControl::resetHWND(HWND hwnd){
	this->hWnd=hwnd;
}

void WinCursorControl::updatePos()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(hWnd,&pt);/**/
	RECT rc;
	GetWindowRect(hWnd,&rc);
	pos.x=pt.x;//-rc.left-Border.x;
	pos.y=pt.y;//-rc.top-Border.y;
	/*
	if(useCursorMap){
		if(hotCenter)
			map->MainPos=math::vector2d(pos.x-map->getWidth()/2,pos.y-map->getHeight()/2);
		else
			map->MainPos=math::vector2d(pos.x,pos.y);
	}*/
	if(isMidScreen()){
		setPosition(windowRect/2);
	}
}

ECursor WinCursorControl::getCursor(){
	return m_cursor;
}
void  WinCursorControl::setCursor(ECursor c){
	m_cursor=c;
/*	if(map){
		map->setCycle(getCursorStr(m_cursor));
	}*/
}

void WinCursorControl::setVisible(bool v){
	if(visible && !v)ShowCursor(0);
	if(!visible &&  v)ShowCursor(1);
	visible=v;
}
void WinCursorControl::setToMidScreen(bool s){
	m_midScreen=s;
}


math::Point2di WinCursorControl::getFinalPos(int x,int y){
	RECT rc;
	if(GetWindowRect(hWnd,&rc))
		return math::Point2di(x+rc.left+Border.x,y+rc.top+Border.y);
	return 0;
}
math::Point2di WinCursorControl::getFinalPos(math::Point2di p){
	return getFinalPos(p.x,p.y);
}


bool WinCursorControl::isVisible(){
	return visible;
}
bool WinCursorControl::isMidScreen(){
	return m_midScreen;
}
void WinCursorControl::setPosition(math::Point2di p){
	setPosition(p.x,p.y);
}
void WinCursorControl::setPosition(float x,float y){
	setPosition((int)(x*windowRect.x),(int)(y*windowRect.y));
}
void WinCursorControl::setPosition(int x,int y)
{
	RECT rc;
	if(GetWindowRect(hWnd,&rc))
		SetCursorPos(x+rc.left+Border.x,y+rc.top+Border.y);
	pos.x=x;
	pos.y=y;
}

void WinCursorControl::setFullscreen(bool fs){
	m_fullscreen=fs;
}

void WinCursorControl::updateBorder()
{
	if(m_fullscreen)
	{
		Border.x=0;
		Border.y=0;
	}else{	
		Border.x=GetSystemMetrics(SM_CXDLGFRAME);
		Border.y=GetSystemMetrics(SM_CYCAPTION)+GetSystemMetrics(SM_CYDLGFRAME);
	}
}
/*
GUI::GUIAnimatedSprite* WinCursorControl::getMap(){
	return map;
}

void WinCursorControl::SetMouseMap(GUI::GUIAnimatedSprite* img){
	map=img;
	if(img){
		img->removeFromParent();
	}
}

void WinCursorControl::SetMouseMap( mchar*name)
{
	map->setTexture(gTextureResourceManager.loadTexture2D(name));
}*/
void WinCursorControl::CursorMap(bool use){
	if(!use && useCursorMap)
		ShowCursor(1);
	if(use && !useCursorMap)
		ShowCursor(0);
	useCursorMap=use;
	//map->Visible=use;
}

math::Point2di WinCursorControl::getPosition(){
	updatePos();
	return pos;
}

math::vector2d WinCursorControl::getRelativePosition(){
	updatePos();
	return math::vector2d(pos.x*invWindowRect.x,pos.y*invWindowRect.y);
}
void WinCursorControl::useHotCenter(bool b){
	hotCenter=b;
}
/*
void WinCursorControl::drawCursor(float dt)
{
	if(!visible || !useCursorMap)return;
	map->draw(dt);
	//map->getDevice()->Draw2DCircle(map->getPos(),3,255);
}*/


void WinCursorControl::setWindowSize(const math::vector2di&sz){
	windowRect=sz;
}



}