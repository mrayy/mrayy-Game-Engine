


/********************************************************************
	created:	2012/02/09
	created:	9:2:2012   10:43
	filename: 	d:\Development\mrayEngine\Tools\MayaExporter\ExporterParams.h
	file path:	d:\Development\mrayEngine\Tools\MayaExporter
	file base:	ExporterParams
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __ExporterParams__
#define __ExporterParams__

#include <IHardwareBuffer.h>

#define CM2MM 10.0
#define CM2CM 1.0
#define CM2M  0.01
#define CM2IN 0.393701
#define CM2FT 0.0328084
#define CM2YD 0.0109361

namespace mray
{
namespace exporter
{
	class MayaSubmesh;

	typedef enum
	{
		NPT_CURFRAME,
		NPT_BINDPOSE
	} NeutralPoseType;

	typedef enum
	{
		TS_TEXCOORD,
		TS_TANGENT
	} TangentSemantic;

	class AnimationTrackDesc
	{
	public:
		MString name;
		int start;
		int end;
		float sampling;
	};

class ExporterParams
{
private:
protected:
	static const MString arg_exportAll;
	static const MString arg_exportMesh;
	static const MString arg_exportMaterial;
	static const MString arg_exportSkeletons;
	static const MString arg_exportSkeletonAnim;
	static const MString arg_exportAnimCurves;
	static const MString arg_exportCameras;
	static const MString arg_exportLights;
	static const MString arg_exportWorldCoords;
	static const MString arg_exportTangent;
	static const MString arg_scaler;
	static const MString arg_materialPrefix;
	static const MString arg_vba;
	static const MString arg_texcoords;
	static const MString arg_vertColor;
	static const MString arg_vertNormal;
	static const MString arg_worldCoord;
	static const MString arg_sharedGeom;
	static const MString arg_basePath;
	static const MString arg_exportTexture;

public:
	bool exportAll;
	bool exportMesh,exportMaterial,exportAnimCurves,exportSkeletonAnim,exportSkeletons,exportCameras,exportLights;
	bool exportWorldCoords,exportTangents;
	bool exportVertNorms,exportVertColor,exportTexcoords,exportVBA;
	bool useSharedGeom;
	bool exportTextures;

	bool lightingOff;

	double scaler;

	NeutralPoseType neutralPoseType;

	MString basePath,meshFileName,skeletonFileName,materialFileName,animFileName,camerasFileName,matPrefix;

	video::IHardwareBuffer::EUsageType indexBufferUsage;
	video::IHardwareBuffer::EUsageType streamsBufferUsage;

	std::vector<AnimationTrackDesc> animationTracks;

public:
	ExporterParams()
	{
		reset();
	}
	void reset()
	{
		exportAll=false;
		exportMesh=false;
		exportMaterial=false;
		exportAnimCurves=false;
		exportCameras=false;
		exportLights=false;
		exportWorldCoords=false;
		exportSkeletonAnim=false;
		exportSkeletons=false;
		useSharedGeom=false;
		exportVertNorms=false;
		exportVertColor=false;
		exportTexcoords=false;
		exportVBA=false;
		lightingOff=false;
		exportTangents=false;
		exportTextures=false;

		indexBufferUsage=video::IHardwareBuffer::EUT_StaticWriteOnly;
		streamsBufferUsage=video::IHardwareBuffer::EUT_WriteOnly;

		neutralPoseType = NPT_CURFRAME;

		scaler=1.0f;
		meshFileName="";
		skeletonFileName="";
		materialFileName="";
		animFileName="";
		camerasFileName="";
		matPrefix="";

		animationTracks.clear();
	}
	virtual~ExporterParams();


	// method to pars arguments and set parameters
	void parseArgs(const MArgList &args);

	static MString fixName(const MString&str);
};

}
}

#endif
