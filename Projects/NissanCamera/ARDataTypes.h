
#ifndef ARDataTypes_h__
#define ARDataTypes_h__



namespace mray
{
namespace NCam
{

	enum class EARObjectType
	{
		EARMesh,
		EARPredef,
		EARString,
		EARVehicle
	};

	// element values for DispObjCommonAttribute.origin (masks)
#define ORIGMASK_WINDOW 0x2 		//  Window coordinate (origin BottomLeft)
	//#define ORIGMASK_WORLD  0x0 	//  World  (east:+X, north :+Y, up :+Z)
	// i.e. right-handed system 
#define ORIGMASK_OWNCAR 0x1 		// Car (left:+X, rear:+Y, up:+Z)
	// origin is x: middle of car, y: front of car, z: ground
#define ORIGMASK_2D_NORMALIZE 0x1 // Window, unit is not pixel but normalized to window size. (i.e. 1 is window width/height)
#define ORIGMASK_3D_TO_2D	0x4		// coordinate is 3D, but use 2D coordinate after transformation for rendering

	enum EARMeshType
	{
		ELines=video::MR_LINES,
		ELineLoop = video::MR_LINE_LOOP,
		ELineStrip = video::MR_LINE_STRIP,
		ETriangles = video::MR_TRIANGLES,
		ETriangleStrip = video::MR_TRIANGLE_STRIP,
		ETriangleFan = video::MR_TRIANGLE_FAN,
		EQuads= video::MR_QUADS
	};


	enum class EARCoordinates
	{
		EWorldSpace = 0,	//3D world coord -> display in 3D
		EOwnCar = (ORIGMASK_OWNCAR),		//3D car relative coord -> display in 3D
		EWindow3DWorld = (ORIGMASK_3D_TO_2D),	//3D world coord -> get 2D coord after transformation and display
		EWindow3DOwnCar = (ORIGMASK_3D_TO_2D | ORIGMASK_OWNCAR),	//3D car relative coord -> get 2D coord ...
		EWindow2DPixel = (ORIGMASK_WINDOW),	// 2D Pixel coord  -> display in window
		EWindow2DNormalized = (ORIGMASK_WINDOW | ORIGMASK_2D_NORMALIZE)// 2D "normalized" coord -> display in window

	};
	enum class EARNormalType
	{
		ENone,
		EVertex,
		EOverall,
		EAutoSmooth,
		EAutoFlat
	};
	enum class EARColorType
	{
		ENone,
		EVertex,
		EOverall
	};
	enum class EARFontSize
	{
		EPixel,
		ENormalized
	};

	class IARObject
	{
	public:
		uint id;
		EARObjectType objectType;
		math::vector3d pos;
		math::vector3d scale;
		math::vector3d dir;
		EARCoordinates coordinates;
		bool isVehicle;
		IARObject(EARObjectType type) :objectType(type), scale(1), isVehicle(false)
		{}
	};
	class ARMesh :public IARObject
	{
	public:
		ARMesh() :IARObject(EARObjectType::EARMesh)
		{}

		uint windowID;
		float lineWidth;
		bool cullface;

		EARNormalType normalType;
		EARColorType colorType;
		
		EARMeshType meshType;
		std::vector<math::vector3d> verticies;
		std::vector<math::vector3d> normals;
		std::vector<video::SColor> colors;
	};
	class ARPredef :public IARObject
	{
	public:
		ARPredef() :IARObject(EARObjectType::EARPredef)
		{}
		math::vector3d scale;
		core::string name;
	};
	class ARString :public IARObject
	{
	public:
		ARString() :IARObject(EARObjectType::EARString)
		{}

		EARFontSize fontSizeType;
		float fontSize;
		core::string fontType;
		std::vector<video::SColor> colors;

		core::string text;
	};

	class ARVehicle :public IARObject
	{
	public:
		ARVehicle() :IARObject(EARObjectType::EARVehicle)
		{}

	};


	class ARGroup
	{
	public:
		~ARGroup()
		{
			for (int i = 0; i < objects.size();++i)
			{
				delete objects[i];
			}
		}
		uint groupID;
		uint seqID;
		std::vector<IARObject*> objects;
	};

	enum EARCommandType
	{
		EArData,
		EDeleteGroup
	};
	class IARCommand
	{
	public:
		IARCommand(EARCommandType t) :type(t)
		{}
		EARCommandType type;
	};

	class ARCommandAddData :public IARCommand
	{
	public:
		ARCommandAddData() :IARCommand(EArData)
		{}

		ARGroup* group;
	};

	class ARCommandDeleteGroup :public IARCommand
	{
	public:
		ARCommandDeleteGroup() :IARCommand(EDeleteGroup)
		{}

		uint groupID;
	};


}
}

#endif // ARDataTypes_h__
