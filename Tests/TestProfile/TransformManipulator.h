

/********************************************************************
	created:	2010/06/16
	created:	16:6:2010   14:39
	filename: 	i:\Programing\GameEngine\mrayEngine\Tools\Editor\TransformManipulator.h
	file path:	i:\Programing\GameEngine\mrayEngine\Tools\Editor
	file base:	TransformManipulator
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef TransformManipulator_h__
#define TransformManipulator_h__

//#include "EditorSceneNode.h"
#include "RTPickBuffer.h"

namespace mray
{
	class ActionsHistory;
namespace scene
{
	class CameraNode;
	class ISceneManager;

class /*MRAY_DLL*/ TransformManipulator
{
public:
	enum EAxisFlags
	{
		EAF_None=0,

		EAF_X=BIT(0),
		EAF_Y=BIT(1),
		EAF_Z=BIT(2),

		EAF_XY=EAF_X | EAF_Y,
		EAF_XZ=EAF_X | EAF_Z,
		EAF_YZ=EAF_Y | EAF_Z,

		EAF_XYZ=EAF_X  | EAF_Y | EAF_Z
	};

	enum EVectorSpace
	{
		EVS_Local,
		EVS_World
	};

	enum EManipulatorType
	{
		EMT_None=0,
		EMT_Translate,
		EMT_Rotate,
		EMT_Scale,
		EMT_Count
	};
	enum EManipulatorComponent
	{
		EMC_None=0,

		EMC_TranslateX	= (BIT(EMT_Translate)<<8) + 0,
		EMC_TranslateY	= (BIT(EMT_Translate)<<8) + 1,
		EMC_TranslateZ	= (BIT(EMT_Translate)<<8) + 2,
		EMC_TranslateXY	= (BIT(EMT_Translate)<<8) + 3,
		EMC_TranslateXZ	= (BIT(EMT_Translate)<<8) + 4,
		EMC_TranslateYZ	= (BIT(EMT_Translate)<<8) + 5,

		EMC_RotateX		= (BIT(EMT_Rotate)<<8) + 0,
		EMC_RotateY		= (BIT(EMT_Rotate)<<8) + 1,
		EMC_RotateZ		= (BIT(EMT_Rotate)<<8) + 2,

		EMC_ScaleX		= (BIT(EMT_Scale)<<8) + 0,
		EMC_ScaleY		= (BIT(EMT_Scale)<<8) + 1,
		EMC_ScaleZ		= (BIT(EMT_Scale)<<8) + 2,
		EMC_ScaleXY		= (BIT(EMT_Scale)<<8) + 3,
		EMC_ScaleXZ		= (BIT(EMT_Scale)<<8) + 4,
		EMC_ScaleYZ		= (BIT(EMT_Scale)<<8) + 5,
		EMC_ScaleXYZ	= (BIT(EMT_Scale)<<8) + 6,

		EMC_Count
	};
	typedef std::list<scene::IMovable*> NodeList;
private:
protected:

	struct ManipulateComponent
	{
		ManipulateComponent():owner(0),node(0),bufferData(0),type(EMC_None),manipulatingFunction(0),isDefault(0)
		{}
		ManipulateComponent(EManipulatorComponent t,scene::ISceneNode* o,scene::IRenderable* n,scene::MeshBufferData*bd,
			void(TransformManipulator::*f)(int x,int y),bool default=false):type(t),owner(o),node(n),bufferData(bd),manipulatingFunction(f),isDefault(default)
		{}
		EManipulatorComponent type;
		scene::ISceneNode* owner;
		scene::IRenderable* node;
		scene::MeshBufferData* bufferData;

		bool isDefault;

		void(TransformManipulator::*manipulatingFunction)(int x,int y);
	};
	std::vector<ManipulateComponent> m_components[EMT_Count];
	

	struct NodeContext
	{
		math::vector3d startPos;
		math::vector3d startScale;
		math::quaternion startRot;
		IMovable* node;
	};
	typedef std::list<NodeContext> NodesContexList;

	GCPtr<scene::ISceneNode> m_manipulatorShape[EMT_Count];

	EManipulatorType m_activeManipulator;
	EManipulatorComponent m_selectedComponent;
	ManipulateComponent* m_selectedManipulator;

	scene::CameraNode* m_camera;
	NodeList m_nodes;
	NodesContexList m_nodesContex;

	bool m_manipulating;
	math::vector2d m_startMousePosition;

	math::vector3d m_startScale;
	math::quaternion m_startRot;
	math::vector3d m_startPos;

	math::vector3d m_totalTranslate;

	GCPtr<video::RTPickBuffer> m_pickBuffer;

	ActionsHistory* m_actionHistory;

	void _startManipulating();

	bool m_localScaling;

protected:
	void TranslateAxis(int x,int y,const math::vector3d&a);
	void TranslatePlane(int x,int y,const math::Plane&p);

	void ScaleAxis(int x,int y,const math::vector3d&a);
	void ScalePlane(int x,int y,const math::Plane&p);

	void RotateAxis(int x,int y,const math::vector3d&a);

	void TranslateX(int x,int y);
	void TranslateY(int x,int y);
	void TranslateZ(int x,int y);
	void TranslateXY(int x,int y);
	void TranslateXZ(int x,int y);
	void TranslateYZ(int x,int y);

	void RotateX(int x,int y);
	void RotateY(int x,int y);
	void RotateZ(int x,int y);

	void ScaleX(int x,int y);
	void ScaleY(int x,int y);
	void ScaleZ(int x,int y);
	void ScaleXY(int x,int y);
	void ScaleXZ(int x,int y);
	void ScaleYZ(int x,int y);
	void ScaleXYZ(int x,int y);



	uint Pick(int x, int y);

	bool SelectAxis(int x,int y);

	void _CheckActiveTransform();
	math::vector3d _GetCenter();
	math::quaternion _GetOrintation();
	math::vector3d _GetScale();

	void _SaveContext();
	void _ReloadContext();
public:
	TransformManipulator();
	virtual ~TransformManipulator();

	void Init(const core::string& modelBasePath,
		ISceneManager* smngr,ViewPort*vp,CameraNode*cam);
	
	void AddNode(scene::IMovable*node);
	void RemoveNode(scene::IMovable*node);
	void ClearNodes();
	const NodeList& GetNodes()const;

	void SetCamera(scene::CameraNode*camera);
	scene::CameraNode* GetCamera();

	void SetActiveManipulator(EManipulatorType m);
	EManipulatorType GetActiveManipulator();

	bool IsManipulating();

	void SetActionHistory(ActionsHistory* h);
	ActionsHistory* GetActionHistory();



	video::RTPickBuffer* GetPickBuffer(){return m_pickBuffer;}

	void Update();

	void OnViewportResize();
	bool OnMousePressed(int x,int y);
	void OnMouseReleased();
	void OnMouseMiddlePressed(int x,int y);
	void OnMouseMoved(int x,int y);
	void OnRMousePressed();
};

}
}
#endif // TransformManipulator_h__

