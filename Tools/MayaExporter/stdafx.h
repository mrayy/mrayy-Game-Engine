// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define PRECISION 0.0001

// standard libraries
#include <math.h>
#include <vector>
#include <set>
#include <cassert>
#include <fstream>

#include <vector>
#include <list>
#include <map>
#include <algorithm>


#ifdef MAC_PLUGIN
#include <ext/hash_map>
#else
#include <hash_map>
#endif

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

#include <XMLElement.h>
#include <StringConverter.h>

// Maya API
#include <maya/MAngle.h>
#include <maya/MFnTransform.h>
#include <maya/MItDag.h>
#include <maya/MFnCamera.h>
#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MObject.h>
#include <maya/MObjectArray.h>
#include <maya/MIntArray.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnVectorArrayData.h>
#include <maya/MArgList.h>
#include <maya/MStatus.h>
#include <maya/MDagPath.h>
#include <maya/MFnMesh.h>
#include <maya/MFnLambertShader.h>
#include <maya/MFnPhongShader.h>
#include <maya/MFnBlinnShader.h>
#include <maya/MFnIkJoint.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MMatrix.h>
#include <maya/MFnSkinCluster.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MDagPathArray.h>
#include <maya/MPointArray.h>
#include <maya/MItGeometry.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MTime.h>
#include <maya/MAnimControl.h>
#include <maya/MAnimUtil.h>
#include <maya/MRenderUtil.h>
#include <maya/MQuaternion.h>
#include <maya/MFnBlendShapeDeformer.h>
#include <maya/MBoundingBox.h>
#include <maya/MDagModifier.h>

