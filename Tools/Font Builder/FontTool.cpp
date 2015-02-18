//This Program is used to Generate BMP fonts
//it was modified by Yamen Saraiji
//for Contact : 
//Email : mrayyamen@gmail.com
//Thanks for using
 
#include "stdafx.h"
#include "resource.h"
#include "stdio.h"
#include "stdlib.h"
#include <gl\glu.h>

#include <string>


wchar_t *exportTypes[] = {L"Bmp",L"Tga",0};

struct SCharData
{
	wchar_t character;
	int width;
	int height;

	int posX;
	int posY;
};


#ifdef _MSC_VER
#	pragma pack( push, packing )
#	pragma pack( 1 )
#	define PACK_STRUCT
#elif defined( __GNUC__ )
#	define PACK_STRUCT	__attribute__((packed))
#else
#	error compiler not supported
#endif

typedef unsigned short ushort;
typedef unsigned int  uint;
typedef unsigned char  uchar;

struct SBMPHeader
{
	ushort	Id;					//	BM - Windows 3.1x, 95, NT, 98, 2000, ME, XP
										//	BA - OS/2 Bitmap Array
										//	CI - OS/2 Color Icon
										//	CP - OS/2 Color Pointer
										//	IC - OS/2 Icon
										//	PT - OS/2 Pointer
	uint	FileSize;
	uint	Reserved;
	uint	BitmapDataOffset;
	uint	BitmapHeaderSize;	// should be 28h for windows bitmaps or
										// 0Ch for OS/2 1.x or F0h for OS/2 2.x
	uint Width;
	uint Height;
	ushort Planes;
	ushort BPP;					// 1: Monochrome bitmap
										// 4: 16 color bitmap
										// 8: 256 color bitmap
										// 16: 16bit (high color) bitmap
										// 24: 24bit (true color) bitmap
										// 32: 32bit (true color) bitmap

	uint  Compression;			// 0: none (Also identified by BI_RGB)
										// 1: RLE 8-bit / pixel (Also identified by BI_RLE4)
										// 2: RLE 4-bit / pixel (Also identified by BI_RLE8)
										// 3: Bitfields  (Also identified by BI_BITFIELDS)

	uint  BitmapDataSize;		// Size of the bitmap data in bytes. This number must be rounded to the next 4 byte boundary.
	uint  PixelPerMeterX;
	uint  PixelPerMeterY;
	uint  Colors;
	uint  ImportantColors;
}PACK_STRUCT ;


#ifdef _MSC_VER
#	pragma pack( pop, packing )
#endif


#include <mstring.h>

using namespace mray;
using namespace core;

int CALLBACK EnumFamCallBack(CONST LOGFONT *lplf, CONST TEXTMETRIC *lptm, DWORD FontType, LPARAM lpData) 
{ 
	SendDlgItemMessage ((HWND)lpData, IDC_LIST1, LB_ADDSTRING, 0, (LONG)lplf->lfFaceName); 
	return 1;
}

void updateFontPreview(HWND hWnd)
{
	int sizeSel = SendDlgItemMessage(hWnd, IDC_LIST2, LB_GETCURSEL, 0, 0);
	int sel = SendDlgItemMessage(hWnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
	int bold = SendDlgItemMessage(hWnd, IDC_CHECK1, BM_GETCHECK, 0, 0);
	int italic = SendDlgItemMessage(hWnd, IDC_CHECK2, BM_GETCHECK, 0, 0);
	int ArabicChars=SendDlgItemMessage(hWnd, IDC_CHECK3, BM_GETCHECK, 0, 0);
	if (sel != LB_ERR && sizeSel != LB_ERR)
	{
		wchar_t str[1024]; 

		SendDlgItemMessage(hWnd, IDC_LIST2, LB_GETTEXT, sizeSel, (LPARAM) (LPCTSTR)str);
		int fontSize = strToInt(str);

		SendDlgItemMessage(hWnd, IDC_LIST1, LB_GETTEXT, sel, (LPARAM) (LPCTSTR)str);

		HDC dc = GetDC(hWnd);

		HFONT font = CreateFont(
			-MulDiv(fontSize, GetDeviceCaps(dc, LOGPIXELSY), 72),
			0,0,0,
			bold ? FW_BOLD : 0,
			italic,
			0,0,0,0,0,0,0,
			str);

		ReleaseDC(hWnd, dc);

		RECT r;
		HWND preview = GetDlgItem(hWnd, IDC_PREVIEW);
		GetClientRect(preview, &r);

		LOGBRUSH lbrush;
		lbrush.lbColor = GetSysColor(COLOR_3DFACE);
		lbrush.lbHatch = 0;
		lbrush.lbStyle = BS_SOLID;
		
		HBRUSH brush = CreateBrushIndirect(&lbrush);
		HPEN pen = CreatePen(PS_NULL, 0, 0);

		dc = GetDC(preview);

		HGDIOBJ oldfont = SelectObject(dc, font);
		HGDIOBJ oldpen = SelectObject(dc, pen);
		HGDIOBJ oldbrush = SelectObject(dc, brush);

		const wchar_t *text = L"ABCDEF abcdef 12345 ﾇﾈﾌﾏ 褂ﾒ";

		SetBkMode(dc, TRANSPARENT);
		SetBkColor(dc, GetSysColor(COLOR_3DFACE));
		Rectangle(dc, 0,0, r.right, r.bottom);

		DrawText(dc, text, wcslen(text), &r, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

		SelectObject(dc, oldfont);
		SelectObject(dc, oldpen);
		SelectObject(dc, oldbrush);

		ReleaseDC(preview, dc);

		DeleteObject(font);
		DeleteObject(brush);
		DeleteObject(pen);
	}
}

double BiCubicR(double x)
{
   double xp2,xp1,xm1;
   double r = 0;

   xp2 = x + 2;
   xp1 = x + 1;
   xm1 = x - 1;

   if (xp2 > 0)
      r += xp2 * xp2 * xp2;
   if (xp1 > 0)
      r -= 4 * xp1 * xp1 * xp1;
   if (x > 0)
      r += 6 * x * x * x;
   if (xm1 > 0)
      r -= 4 * xm1 * xm1 * xm1;

   return(r / 6.0);
}

void FillFontList(HWND hWnd)
{
	// Fill list with font names

	SendDlgItemMessage(hWnd, IDC_LIST1, LB_RESETCONTENT, 0, 0); 
	HDC dc = GetDC(hWnd);
	EnumFonts(dc, 0, &EnumFamCallBack, (LPARAM)hWnd);
	ReleaseDC(hWnd, dc);

	SendDlgItemMessage (hWnd, IDC_LIST1, LB_SETCURSEL, 0, 0);

	// Fill list with font sizes

	wchar_t buf[128];
	int sc[] = {4,6,8,9,10,11,12,14,16,18,20,22,24,26,28,36,48,56,68,72,0};
	for (int i=0; sc[i] != 0; ++i)
		SendDlgItemMessage (hWnd, IDC_LIST2, LB_ADDSTRING, 0, (LONG)_itow(sc[i], buf, 10)); 

	SendDlgItemMessage (hWnd, IDC_LIST2, LB_SETCURSEL, 4, 0);

	// Fill texture size list

	int tc[] = {256,512,1024,2048,0};
	for (int t=0; tc[t] != 0; ++t)
	{
		wsprintf(buf, L"%d pixels width", tc[t]);
		SendDlgItemMessage (hWnd, IDC_COMBO1, CB_ADDSTRING, 0, (LONG)buf);
	}
	SendDlgItemMessage (hWnd, IDC_COMBO1, CB_SETCURSEL, 0, 0);

	for (int t=0; t<2; ++t)
	{
		SendDlgItemMessage (hWnd, IDC_COMBO2, CB_ADDSTRING, 0, (LONG)exportTypes[t]);
	}

	SendDlgItemMessage (hWnd, IDC_COMBO2, CB_SETCURSEL, 0, 0);
}



inline int getTextureSizeFromSurfaceSize(int size)
{
	int ts = 0x01;
	while(ts < size)
		ts <<= 1;

	return ts;
}



void copyFontToClipBoard(HWND hWnd, int lastChar)
{
	int surfaceSel = SendDlgItemMessage(hWnd, IDC_COMBO1, CB_GETCURSEL, 0, 0);
	int exportSel = SendDlgItemMessage(hWnd, IDC_COMBO2, CB_GETCURSEL, 0, 0);
	int sizeSel = SendDlgItemMessage(hWnd, IDC_LIST2, LB_GETCURSEL, 0, 0);
	int sel = SendDlgItemMessage(hWnd, IDC_LIST1, LB_GETCURSEL, 0, 0);	
	int bold = SendDlgItemMessage(hWnd, IDC_CHECK1, BM_GETCHECK, 0, 0);
	int italic = SendDlgItemMessage(hWnd, IDC_CHECK2, BM_GETCHECK, 0, 0);
	int ArabicChars=SendDlgItemMessage(hWnd, IDC_CHECK3, BM_GETCHECK, 0, 0);
	int EnableResize=SendDlgItemMessage(hWnd, IDC_CHECK4, BM_GETCHECK, 0, 0);
	int AlphaLayer=SendDlgItemMessage(hWnd, IDC_CHECK6, BM_GETCHECK, 0, 0);
	
//	if(EnableResize)sizeSel=72;

	if (sel == LB_ERR || sizeSel == LB_ERR || surfaceSel == CB_ERR)
		return;

	unsigned int i;
	int firstChar = 32;
	int characterCount = 256;

	if(ArabicChars)
		characterCount+=35;
	SCharData *asciiTable = new SCharData[characterCount];
	
	wchar_t* testString = new wchar_t[characterCount + 1];

	// create the out table
	//English
	int currChar=0;
	if(!ArabicChars){
		//ASCII chars
		for (i=32; i<=256; ++i)
		{
			testString[currChar]=(wchar_t)i;
			currChar++;
		}
	}else{
		//UniCode

		//English chars
		for (i=32; i<=127; ++i)
		{
			testString[currChar]=(wchar_t)i;
			currChar++;
		}
		//Digits
		for (i=1632; i<=1641; ++i)
		{
			testString[currChar]=(wchar_t)i;
			currChar++;
		}
		//Arabic Chars
		for (i=1575; i<=1610; ++i)
		{
			testString[currChar]=(wchar_t)i;
			currChar++;
		}
		//Arabic Chars
		for (i=65152; i<=65276; ++i)
		{
			testString[currChar]=(wchar_t)i;
			currChar++;
		}

	}
	testString[currChar] = 0;

	characterCount=currChar;
	// We use the font render preview area as dc

	HWND preview = GetDlgItem(hWnd, IDC_PREVIEW);
	HDC dc = GetDC(preview);

	wchar_t str[1024]; 

	SendDlgItemMessage(hWnd, IDC_LIST2, LB_GETTEXT, sizeSel, (LPARAM) (LPCTSTR)str);
	int fontSize = strToInt(str);

	SendDlgItemMessage(hWnd, IDC_COMBO1, CB_GETLBTEXT, surfaceSel, (LPARAM) (LPCTSTR)str);
	int outSurfaceDim = strToInt(str);

	SendDlgItemMessage(hWnd, IDC_LIST1, LB_GETTEXT, sel, (LPARAM) (LPCTSTR)str);


	//Prepare the font
	LOGFONTW logFont;
	HFONT font;


	if(ArabicChars)//writing arabic charset
	{ 
		font = CreateFont(
			-MulDiv(fontSize, GetDeviceCaps(dc, LOGPIXELSY), 72),
			0,0,0,
			bold ? FW_BOLD : 0,
			italic,
			0,0,ARABIC_CHARSET,0,0,0,0,
			str);
	}
	else
	{

		font = CreateFont(
			-MulDiv(fontSize, GetDeviceCaps(dc, LOGPIXELSY), 72),
			fontSize,0,0,
			bold ? FW_BOLD : 0,
			italic,
			0,0,0,0,0,0,0,
			str);
	}

	 SelectObject(dc, font);
	// calculate text extents.

	SIZE size;
	size.cx = 0;
	size.cy = 0;
	int posx = 0;
	int posy = 0;

	SIZE charSz;
	charSz.cx=fontSize+5;
	charSz.cy=fontSize+5;
	int col=0;
	int row=0;

	int maxSzX=0;
	int maxSzY=0;

	int extra=0;

	int surfaceWidth=512;
	int surfaceHeight=512;

	//get max dim of chars
	for (i=0; i<characterCount; ++i)
	{
		GetTextExtentPoint32W(dc, &testString[i], 1, &size);
		ABC abc;
		if(GetCharABCWidthsW(dc,testString[i],testString[i],&abc))
		{
			size.cx=abc.abcB;
		}

		asciiTable[i].width = size.cx+extra;
		asciiTable[i].height = size.cy+extra;

		maxSzX=max(maxSzX,asciiTable[i].width);
		maxSzY=max(maxSzY,asciiTable[i].height);

	}

	int currX=0;
	int currY=0;
	for (i=0; i<characterCount; ++i)
	{
		if(currX+asciiTable[i].width>surfaceWidth)
		{
			currX=0;
			currY+=maxSzY;
		}

		asciiTable[i].posX = currX;
		asciiTable[i].posY = currY;
		asciiTable[i].character = testString[i];	
		currX+=asciiTable[i].width;

	/*	col++;

		if (col >= 16)
		{
			col=0;
			row++;
		}*/
	}
/*
	int surfaceHeight = max(maxSzX,maxSzY)*16;;
	int surfaceWidth  = surfaceHeight;*/


	// draw characters

	HBITMAP bmp = CreateCompatibleBitmap(dc, surfaceWidth, surfaceHeight);
	HDC bmpdc = CreateCompatibleDC(dc);

	LOGBRUSH lbrush;
	lbrush.lbColor = RGB(255,255,255);
	lbrush.lbHatch = 0;
	lbrush.lbStyle = BS_SOLID;
	
	HBRUSH brush = CreateBrushIndirect(&lbrush);
	HPEN pen = CreatePen(PS_NULL, 0, 0);

	HGDIOBJ oldbmp = SelectObject(bmpdc, bmp);
	HGDIOBJ oldbmppen = SelectObject(bmpdc, pen);
	HGDIOBJ oldbmpbrush = SelectObject(bmpdc, brush);
	HGDIOBJ oldbmpFont = SelectObject(bmpdc, font);



	Rectangle(bmpdc, 0,0,surfaceWidth,surfaceHeight);
	SetBkMode(bmpdc, TRANSPARENT);
	SetBkColor(bmpdc,RGB(255,255,255));

	SCharData* d;


	//create bmp rectangle
	int offX=0;
	currChar=0;
	WCHAR ch[2];
	for (currChar=0; currChar<characterCount; ++currChar)
	{
		d = &asciiTable[currChar];
		wsprintfW(ch,L"%c",d->character);
	//	GetTextExtentPoint32W(dc, &d->character, 1, &size);
	//	offX=(maxSz-size.cx)/2;
		TextOutW(bmpdc, d->posX+extra*0.5, d->posY+extra*0.5, &d->character, 1);
		
	}


	BITMAP bmpInfo;

	//get image info
	GetObject(bmp,sizeof(BITMAP),&bmpInfo);
	DWORD *buffer;
	
	int pixel=AlphaLayer ? 4:3;

	int line=(bmpInfo.bmWidth*pixel+pixel)& 0xfffffffc;
	int dataSize=line*(bmpInfo.bmHeight)*pixel;
	buffer=new DWORD[dataSize];

	//get image data
	GetBitmapBits(bmp,dataSize,buffer);
	

	DWORD tmp;
	int top=0;
	int bottom=surfaceHeight-1;
	//flip image vertical
	for(int j=0;j<surfaceHeight/2;j++)
	{

		for(int i=0;i<surfaceWidth;++i)
		{
			tmp=buffer[surfaceWidth*top+i];
			buffer[surfaceWidth*top+i]=buffer[surfaceWidth*bottom+i];
			buffer[surfaceWidth*bottom+i]=tmp;
		}
		bottom--;
		top++;
	}


	if(AlphaLayer){
		for(int i=0;i<surfaceWidth*surfaceHeight;++i)
		{
			mray::byte r=GetRValue(buffer[i]);
			buffer[i] |=(byte)(255-r)<<24;
		}
	}

	
	//video::CTexture*tex=TextureResourceManager;
	//tex->createTexture(surfaceWidth,surfaceHeight,AlphaLayer==1 ? video::EPixel_R8G8B8A8 : video::EPixel_R8G8B8);
	/*
	int EPixel_size;
	if(AlphaLayer){
		EPixel_size=4*surfaceWidth*surfaceHeight;
	}else
		EPixel_size=3*surfaceWidth*surfaceHeight;
	mray::byte*data=(mray::byte*)(buffer);//(byte*)tex->lock();

	int escape=AlphaLayer==1 ? 4:3;

	for(int i=0;i<surfaceWidth*surfaceHeight;)
	{
		data[escape*i + 0]=GetRValue(buffer[i]);
		data[escape*i + 1]=GetGValue(buffer[i]);
		data[escape*i + 2]=GetBValue(buffer[i]);
		if(AlphaLayer==1){
			data[escape*i + 3]=255-data[escape*i + 0];
		}
		i+=escape;
	}
*/
	//rescale image size
/*
	if(surfaceWidth!=outSurfaceDim && EnableResize)
	{
		video::ColorConverter::resizeTexture(tex,vector2di(outSurfaceDim));
			
	}*/
	SendDlgItemMessage(hWnd, IDC_LIST1, LB_GETTEXT, sel, (LPARAM) (LPCTSTR)str);

	std::wstring fontName=str;
	wchar_t str2[512];
	wcsncpy(str2,str,512);
	
	wcscat(str,L"_font.bmp");
//	wcscat(str,exportTypes[exportSel]);
	
	wcscat(str2,L"_attr.fnt");

	
	//prepare header --- bad way :( ---
	SBMPHeader header;
	header.Id=0x4d42;
	header.FileSize=0;
	header.BitmapHeaderSize=40;
	header.BitmapDataOffset=54;
	header.Reserved=0;
	header.Width=surfaceWidth;
	header.Height=surfaceHeight;
	header.Planes=1;
	header.BPP=AlphaLayer ? 32:24;
	header.Compression=0;
	header.BitmapDataSize=dataSize;
	header.PixelPerMeterX=0;
	header.PixelPerMeterY=0;
	header.Colors=0;
	header.ImportantColors=0;

	

	FILE* fp=_wfopen(str,L"wb");
	if(!fp)
	{
		MessageBox(hWnd,L"ﾎﾘﾃ: 矼 桄・ﾃ蕚ﾇﾁ ﾇ矼矍",L"ﾎﾘﾃ",0);
	}
	else
	{
		fwrite(&header,1,sizeof(header),fp);
		fwrite(buffer,1,dataSize,fp);

		fclose(fp);
	}
/*
	if(!texWriter->write(tex,str))
	{
		
		MessageBox(hWnd,L"ﾎﾘﾃ: 矼 桄・ﾃ蕚ﾇﾁ ﾇ矼矍",L"ﾎﾘﾃ",0);
	}*/
	//export chars attributes
	float invW=1.0f/(float)surfaceWidth;
	float invH=1.0f/(float)surfaceHeight;
	FILE*AttrFile=_wfopen(str2,L"w");
	if(AttrFile){
		fwprintf(AttrFile,L"<Font Name=\"%s\" Type=\"glyph\" Texture=\"%s\" >\n",fontName.c_str(),str);

		for(int i=0;i<characterCount;++i){
			int x=asciiTable[i].posX;
			int y=asciiTable[i].posY;
			fwprintf(AttrFile,L"\t<Char C=\"%d\" U=\"%f\" V=\"%f\" dU=\"%f\" dV=\"%f\"/>\n",asciiTable[i].character,
				max(0,x)*invW,max(0,y)*invH,asciiTable[i].width*invW,asciiTable[i].height*invH);
		}
		fwprintf(AttrFile,L"</Font>");
	/*
		fwprintf(AttrFile,L"%s\n",str);
		fprintf(AttrFile,"%d\n",surfaceWidth);
		fprintf(AttrFile,"%d\n",surfaceHeight);
		fprintf(AttrFile,"%d\n",characterCount);
		for(int i=0;i<characterCount;++i){
			int x=asciiTable[i].posX;
			int y=asciiTable[i].posY;
			fwprintf(AttrFile,L"%d ",asciiTable[i].character);
			fprintf(AttrFile,"%d ",max(0,x));
			fprintf(AttrFile,"%d ",max(0,y));
			fprintf(AttrFile,"%d ",asciiTable[i].width);
			fprintf(AttrFile,"%d\n",asciiTable[i].height);
		}*/
		fclose(AttrFile);
	}

	delete [] asciiTable;
	delete [] testString;
	delete [] buffer;

	SelectObject(bmpdc, oldbmp);
	SelectObject(bmpdc, oldbmppen);
	SelectObject(bmpdc, oldbmpbrush);


	ReleaseDC(preview, dc);

	DeleteDC(bmpdc);
	DeleteObject(font);
	DeleteObject(brush);
	DeleteObject(pen);
	DeleteObject(bmp);
}


BOOL CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) 
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		{
			int wNotifyCode = HIWORD(wParam);
			int wID = LOWORD(wParam);
			HWND hwndCtl = (HWND)lParam;

			switch(wID)
			{
			case IDOK:
			case IDCANCEL:
				EndDialog(hwndDlg, wNotifyCode);
				PostQuitMessage(0);
				return TRUE;
			case IDC_CHECK1:
			case IDC_CHECK2:
			case IDC_LIST1:
			case IDC_LIST2: 
				updateFontPreview(hwndDlg);
				return TRUE;
			case IDC_BUTTON2:
				MessageBox(hwndDlg, L"ﾊ・ﾊﾚﾏ涖 衵・ﾇ睇ﾏﾇﾉ 聿 ﾇﾌ・ﾊ趁榘 聶ﾝﾇﾊ 葯ﾘ栁 硼ﾊ・ﾇﾓﾊﾎﾏﾇ肄ﾇ ﾝ偆n "\
					L"M-RAY Engine\n"\
					L"Email: mrayyamen@gmail.com",
					L"About FontTool", MB_OK | MB_ICONINFORMATION);
				return TRUE;
			case IDC_BUTTON1:
				copyFontToClipBoard(hwndDlg, 256);
				return TRUE;
			}
		}
		break;
	}

	return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow )
{
//	core::PRINTER::openLogger(L"log.txt");
	/*

	texWriter=new loaders::TexWriter(0);
	texWriter->addWriter(new loaders::CBmpWriter());
	texWriter->addWriter(new loaders::CTargaWriter());*/

	HWND hWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, DialogProc);

	ShowWindow(hWnd , SW_SHOW);

	FillFontList(hWnd);

	MSG msg;
	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else{
			WaitMessage();
		}
	}
	while (msg.message != WM_QUIT);

	return 0;
}



