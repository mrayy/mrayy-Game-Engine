

/********************************************************************
	created:	2010/06/17
	created:	17:6:2010   18:53
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGameLayer\CompileConfig.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGameLayer
	file base:	CompileConfig
	file ext:	h
	author:		Mohamad Yamen SarGAMEji
	
	purpose:	
*********************************************************************/

#ifndef CompileConfig_h__
#define CompileConfig_h__


#ifndef MRAY_GAME_DLL

#ifndef GAME_STATIC_LIB
	#ifdef MRAYGAMELAYER_EXPORTS
		#define MRAY_GAME_DLL __declspec(dllexport)
	#else
		#define MRAY_GAME_DLL __declspec(dllimport)
	#endif

#else
	#define MRAY_GAME_DLL
#endif

#endif


#endif // CompileConfig_h__
