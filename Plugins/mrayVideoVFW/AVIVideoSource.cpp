#include "stdafx.h"



#include "AVIVideoSource.h"
#include "ILogManager.h"
#include "IFileSystem.h"
#include "Engine.h"
#include "mraySystem.h"


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vfw.h>

namespace mray{
namespace video{



struct AVIVideoSourceData
{
	AVIVideoSourceData():pavi(0),pgf(0)
	{
	}
	static int AVIRef;
	PAVISTREAM pavi;
	AVISTREAMINFO psi;
	BITMAPINFOHEADER pmih;
	PGETFRAME pgf;
	HBITMAP hBitmap;
	HDRAWDIB hdd;
	HDC hdc;
};

int AVIVideoSourceData::AVIRef=0;

void flipIt(void* buffer,math::Point2di size)										// Flips The Red And Blue Bytes (256x256)
{
	void* b = buffer;											// Pointer To The Buffer
	int sz=size.x*size.y;
	__asm														// Assembler Code To Follow
	{
		mov ecx, sz												// Counter Set To Dimensions Of Our Memory Block
		mov ebx, b												// Points ebx To Our Data (b)
		label:													// Label Used For Looping
			mov al,[ebx+0]										// Loads Value At ebx Into al
			mov ah,[ebx+2]										// Loads Value At ebx+2 Into ah
			mov [ebx+2],al										// Stores Value In al At ebx+2
			mov [ebx+0],ah										// Stores Value In ah At ebx
			
			add ebx,3											// Moves Through The Data By 3 Bytes
			dec ecx												// Decreases Our Loop Counter
			jnz label											// If Not math::Zero Jump Back To Label
	}
}


AVIVideoSource::AVIVideoSource():m_frame(0)
{
	m_data=new AVIVideoSourceData();
	m_hasNewFrame=false;
	m_Texture.autoDel=true;
	m_pixelFormat=video::EPixel_R8G8B8;
	SetFrameSize(320,240);


	m_frame=0;
	m_lastFrame=0;
	m_invLastFrame=0;
	m_loop=0;
	m_data->hdd = DrawDibOpen();
	m_data->hdc=CreateCompatibleDC(0);

	m_speedFactor=1;
	
  	m_data->pavi=0;

	m_status=EVS_Stopped;

}

AVIVideoSource::~AVIVideoSource(){
	destory();
	DrawDibClose(m_data->hdd);											// Closes The DrawDib Device Context
	delete m_data;

}

uint AVIVideoSource::calcSizeInternal(){
	return sizeof(AVIVideoSource);
}

void AVIVideoSource::destory(){
	if(!m_data->pavi)return;
	DeleteObject(m_data->hBitmap);										// Delete The Device Dependant Bitmap Object
	AVIStreamGetFrameClose(m_data->pgf);								// Deallocates The GetFrame Resources
	AVIStreamRelease(m_data->pavi);										// Release The Stream
	AVIVideoSourceData::AVIRef--;
	if(AVIVideoSourceData::AVIRef==0)
		AVIFileExit();
}

bool AVIVideoSource::loadVideo(const  core::string&fileName){

	destory();
	core::string fName;
	gFileSystem.getCorrectFilePath(fileName,fName);

	if(fName==mT(""))return false;

	if(AVIVideoSourceData::AVIRef==0){
		// Opens The AVIFile Library
		AVIFileInit();
	}
	AVIVideoSourceData::AVIRef++;
	
	// Opens The AVI Stream
	if(AVIStreamOpenFromFile(&m_data->pavi,fName.c_str(),streamtypeVIDEO,0,OF_READ,0)){
		gLogManager.log(mT("Failed to load avi file "),fileName,ELL_WARNING);
		return false;
	}

	
	// Reads Information About The Stream Into psi
	AVIStreamInfo(m_data->pavi, &m_data->psi, sizeof(m_data->psi));
	// Width Is Right Side Of Frame Minus Left
	m_SrcSize.x=m_data->psi.rcFrame.right-m_data->psi.rcFrame.left;
	// Height Is Bottom Of Frame Minus Top
	m_SrcSize.y=m_data->psi.rcFrame.bottom-m_data->psi.rcFrame.top;

	m_lastFrame=AVIStreamLength(m_data->pavi);							// The Last Frame Of The Stream
	if(m_lastFrame>0)
		m_invLastFrame=1.0f/(float)m_lastFrame;

	m_mpf=(float)AVIStreamSampleToTime(m_data->pavi,1);// *m_invLastFrame;		// Calculate Rough Milliseconds Per Frame

	m_data->pmih.biSize = sizeof (BITMAPINFOHEADER);					// Size Of The BitmapInfoHeader
	m_data->pmih.biPlanes = 1;											// Bitplanes	
	m_data->pmih.biBitCount = 24;										// Bits Format We Want (24 Bit, 3 Bytes)
	m_data->pmih.biWidth = m_DstSize.x;											// Width We Want
	m_data->pmih.biHeight = m_DstSize.y;										// Height We Want 
	m_data->pmih.biCompression = BI_RGB;								// Requested Mode = RGB



	m_data->hBitmap = CreateDIBSection (m_data->hdc, (BITMAPINFO*)(&m_data->pmih), DIB_RGB_COLORS, (void**)(&data), NULL, NULL);
	SelectObject (m_data->hdc, m_data->hBitmap);								// Select hBitmap Into Our Device Context (hdc)

	m_data->pgf=AVIStreamGetFrameOpen(m_data->pavi, NULL);						// Create The PGETFRAME	Using Our Request Mode
	if (m_data->pgf==NULL)
	{
		// An Error Occurred Opening The Frame
		gLogManager.log(mT("Failed to open avi file "),fileName,ELL_WARNING);
		return false;
	}

	m_Texture.setData(data,math::vector3d(m_DstSize.x,m_DstSize.y,1),m_pixelFormat);

	return true;
}

bool AVIVideoSource::setFrame(float frame){
	if(m_frame==frame)
	{
		m_hasNewFrame=false;
		return false;
	}
	m_frame=frame;

	if(m_loop || m_frame<0){
		if(m_frame>m_lastFrame)
			m_frame=fmod(m_frame,m_lastFrame);
	}else
		if(m_frame>m_lastFrame)
			m_frame=m_lastFrame;

//	uchar*data=(uchar*)m_Texture->lock(0,0,video::IHardwareStreamBuffer::ELO_ReadOnly);

	LPBITMAPINFOHEADER lpbi;									// Holds The Bitmap Header Information
	lpbi = (LPBITMAPINFOHEADER)AVIStreamGetFrame(m_data->pgf,(int)m_frame);	// Grab Data From The AVI Stream
	if(!lpbi)
		return false;
	char*pdata=(char *)lpbi+lpbi->biSize+lpbi->biClrUsed * sizeof(RGBTRIPLE);	// Pointer To Data Returned By AVIStreamGetFrame

	// Convert Data To Requested Bitmap Format
	DrawDibDraw (m_data->hdd, m_data->hdc, 0, 0, m_DstSize.x, m_DstSize.y, lpbi, pdata, 0, 0, m_SrcSize.x, m_SrcSize.y, 0);

	flipIt(data,m_DstSize);												// Swap The Red And Blue Bytes (GL Compatability)

	// Update The Texture
	m_Texture.setData(data,math::vector3d(m_DstSize.x,m_DstSize.y,1),m_pixelFormat);

	m_hasNewFrame=true;
	return true;
}
float AVIVideoSource::getDuration(){
	if(m_speedFactor==0)return 0;
	return (float)m_lastFrame/(m_mpf*m_speedFactor);
}

float AVIVideoSource::getCurrFrame(){
	return m_frame;
}
float AVIVideoSource::getPercentage(){
	return m_frame*100.0f*m_invLastFrame;
}
void AVIVideoSource::setLoop(bool loop){
	m_loop=loop;
}
bool AVIVideoSource::isLoop(){
	return m_loop;
}

bool AVIVideoSource::isDone(){
	return m_frame==m_lastFrame;
}

bool AVIVideoSource::GrabFrame(){
// 	if(m_status==EVS_Playing)
// 	{
// 		return setFrame(m_frame+dt*m_mpf*m_speedFactor);
// 	}
//	return false;
	return m_hasNewFrame;
}

void AVIVideoSource::SetFrameSize(int w,int h)
{
	m_DstSize.x=w;
	m_DstSize.y=h;

}

const ImageInfo* AVIVideoSource::GetLastFrame(){
	return &m_Texture;
}

void AVIVideoSource::SetImageFormat(video::EPixelFormat fmt)
{
}
video::EPixelFormat AVIVideoSource::GetImageFormat()
{
	return m_pixelFormat;
}


void AVIVideoSource::setSpeedFactor(float speed){
	m_speedFactor=speed;
}
float AVIVideoSource::getSpeedFactor(){
	return m_speedFactor;
}


void AVIVideoSource::play(){
	m_status=EVS_Playing;
}
void AVIVideoSource::stop(){
	m_status=EVS_Stopped;
	setFrame(0);
}
void AVIVideoSource::pause(){
	if(m_status==EVS_Playing)
		m_status=EVS_Paused;
}
EVideoStatus AVIVideoSource::getStatus(){
	return m_status;
}

}
}



