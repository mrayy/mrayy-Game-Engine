

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#include <stdio.h>
#include <tchar.h>



// TODO: reference additional headers your program requires here

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <mrayEngine.h>
#include <mrayWinApp.h>
#include <mrayPhysX.h>

#include <GameEntity.h>
#include <GameEntityManager.h>
#include <GameGlobals.h>

#include <ImageSetResourceManager.h>
#include <FontResourceManager.h>
#include <IFileSystem.h>


#include <GUIThemeManager.h>
#include <CFPS.h>
#include <GUIBatchRenderer.h>