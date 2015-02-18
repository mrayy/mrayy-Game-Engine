


/********************************************************************
	created:	2011/11/20
	created:	20:11:2011   15:49
	filename: 	d:\Development\mrayEngine\Engine\mrayD3D9Device\compileConfig.h
	file path:	d:\Development\mrayEngine\Engine\mrayD3D9Device
	file base:	compileConfig
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __compileConfig__
#define __compileConfig__


#ifndef MRAY_D3D9_DLL
#	ifndef MRAY_D3D9_LIB
#		ifdef MRAY_D3D9_EXPORTS
#			define MRAY_D3D9_DLL __declspec(dllexport)
#		else
#			define MRAY_D3D9_DLL __declspec(dllimport)
#		endif
#	else
#		define MRAY_D3D9_DLL
#	endif
#endif

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

#endif
