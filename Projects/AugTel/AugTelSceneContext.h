
#ifndef AugTelSceneContext_h__
#define AugTelSceneContext_h__



namespace mray
{
	namespace game
	{
		class GameEntityManager;
	}
	namespace TBee
	{
		class ICameraVideoSource;
	}

namespace AugTel
{
	class HeadMount;

class AugTelSceneContext
{
public:
	AugTelSceneContext():entManager(0),headNode(0),videoSource(0)
	{}
	virtual ~AugTelSceneContext(){}

	game::GameEntityManager* entManager;
	HeadMount* headNode;
	TBee::ICameraVideoSource* videoSource;
	scene::CameraNode* cameras[2];
};

}

}

#endif // AugTelSceneContext_h__

