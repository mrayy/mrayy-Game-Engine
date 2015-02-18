// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers



// TODO: reference additional headers your program requires here

//math lib
#include <point2d.h>
#include <point3d.h>
#include <Box3D.h>
#include <ExtrudedPolygon.h>
#include <Frustum.h>
#include <GenericMatrix.h>
#include <Intersection.h>
#include <line2d.h>
#include <line3d.h>
#include <MathUtil.h>
#include <matrix3x3.h>
#include <matrix4x4.h>
#include <mMath.h>
#include <Plane.h>
#include <quaternion.h>
#include <Polygon2D.h>
#include <randomizer.h>
#include <Ray3d.h>
#include <Rect.h>
#include <Sphere.h>
#include <triangle3d.h>

#include <vector>
#include <list>
#include <map>
#include <algorithm>

#include <berkelium/Berkelium.hpp>