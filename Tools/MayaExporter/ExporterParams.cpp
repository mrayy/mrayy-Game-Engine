
#include "stdafx.h"
#include "ExporterParams.h"


namespace mray
{
namespace exporter
{

	const MString ExporterParams::arg_exportAll			="-all";
	const MString ExporterParams::arg_exportMesh		="-mesh";
	const MString ExporterParams::arg_exportMaterial	="-material";
	const MString ExporterParams::arg_exportAnimCurves	="-anim";
	const MString ExporterParams::arg_exportSkeletons	="-skel";
	const MString ExporterParams::arg_exportSkeletonAnim="-skelAnim";
	const MString ExporterParams::arg_exportCameras		="-cameras";
	const MString ExporterParams::arg_exportLights		="-lights";
	const MString ExporterParams::arg_exportWorldCoords	="-world";
	const MString ExporterParams::arg_exportTangent		="-tangents";
	const MString ExporterParams::arg_scaler			="-units";
	const MString ExporterParams::arg_materialPrefix	="-matPrefix";
	const MString ExporterParams::arg_vba				="-vba";
	const MString ExporterParams::arg_texcoords			="-texcoords";
	const MString ExporterParams::arg_vertColor			="-vertColor";
	const MString ExporterParams::arg_vertNormal		="-vertNormal";
	const MString ExporterParams::arg_worldCoord		="-worldCoord";
	const MString ExporterParams::arg_sharedGeom		="-sharedGeom";
	const MString ExporterParams::arg_basePath			="-exportPath";
	const MString ExporterParams::arg_exportTexture			="-exportTextures";

	ExporterParams::~ExporterParams()
	{
	}

void ExporterParams::parseArgs(const MArgList &args)
{
	reset();

	MStatus stat;
	float sc=1.0f;
	// Parse arguments from command line
	for (uint i=0; i < args.length(); i++ )
	{
		if(arg_exportAll==args.asString(i,&stat) && stat==MS::kSuccess)
			exportAll=true;
		else if(arg_exportWorldCoords==args.asString(i,&stat) && stat==MS::kSuccess)
			exportWorldCoords=true;
		else if(arg_scaler==args.asString(i,&stat) && stat==MS::kSuccess)
		{
			MString lengthUnit = args.asString(++i,&stat);
			if (MString("pref") == lengthUnit)
			{
				MGlobal::executeCommand("currentUnit -q -l",lengthUnit,false);
			}
			if (MString("mm") == lengthUnit)
				scaler = CM2MM;
			else if (MString("cm") == lengthUnit)
				scaler = CM2CM;
			else if (MString("m") == lengthUnit)
				scaler = CM2M;
			else if (MString("in") == lengthUnit)
				scaler = CM2IN;
			else if (MString("ft") == lengthUnit)
				scaler = CM2FT;
			else if (MString("yd") == lengthUnit)
				scaler = CM2YD;
		}
		else if(MString("-scaler")==args.asString(i,&stat) && stat==MS::kSuccess)
		{
			sc = args.asDouble(++i,&stat);
		}
		else if (arg_exportMesh == args.asString(i,&stat) && (MS::kSuccess == stat))
		{
			exportMesh = true;
			meshFileName = args.asString(++i,&stat);
		}
		else if (arg_exportMaterial == args.asString(i,&stat) && (MS::kSuccess == stat))
		{
			exportMaterial = true;
			materialFileName = args.asString(++i,&stat);
		}
		else if (arg_materialPrefix == args.asString(i,&stat) && (MS::kSuccess == stat))
			matPrefix = args.asString(++i,&stat);
		else if (arg_exportSkeletons == args.asString(i,&stat) && (MS::kSuccess == stat))
		{
			exportSkeletons = true;
		//	skeletonFileName = args.asString(++i,&stat);
		}else if (arg_exportTexture == args.asString(i,&stat) && (MS::kSuccess == stat))
		{
			exportTextures= true;
			//	skeletonFileName = args.asString(++i,&stat);
		}
		else if (arg_exportSkeletonAnim == args.asString(i,&stat) && (MS::kSuccess == stat))
			exportSkeletonAnim = true;
		else if(arg_vba==args.asString(i,&stat) && stat==MS::kSuccess)
			exportVBA=true;
		else if(arg_texcoords==args.asString(i,&stat) && stat==MS::kSuccess)
			exportTexcoords=true;
		else if(arg_vertColor==args.asString(i,&stat) && stat==MS::kSuccess)
			exportVertColor=true;
		else if(arg_vertNormal==args.asString(i,&stat) && stat==MS::kSuccess)
			exportVertNorms=true;
		else if(arg_worldCoord==args.asString(i,&stat) && stat==MS::kSuccess)
			exportWorldCoords=true;
		else if(arg_exportTangent==args.asString(i,&stat) && stat==MS::kSuccess)
			exportTangents=true;
		//else if(arg_sharedGeom==args.asString(i,&stat) && stat==MS::kSuccess)
		//	useSharedGeom=true;
		else if(arg_basePath==args.asString(i,&stat) && stat==MS::kSuccess)
			basePath=args.asString(++i,&stat);
		else if(arg_exportAnimCurves==args.asString(i,&stat) && stat==MS::kSuccess)
		{
			exportAnimCurves=true;
			AnimationTrackDesc anim;
			anim.name=args.asString(++i);
			anim.start=args.asInt(++i);
			anim.end=args.asInt(++i);
			anim.sampling=args.asDouble(++i);
			animationTracks.push_back(anim);
		}
	}

	scaler *= sc;

}

MString ExporterParams::fixName(const MString&str)
{
	core::string ret=str.asChar();
	ret.replaceChar(':','_');
	ret.replaceChar('|','_');
	return ret.c_str();;
}

}
}