

/********************************************************************
	created:	2011/06/03
	created:	3:6:2011   14:42
	filename: 	d:\Development\mrayEngine\Projects\BallanceBall\GameCamera.h
	file path:	d:\Development\mrayEngine\Projects\BallanceBall
	file base:	GameCamera
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __GameCamera__
#define __GameCamera__


namespace mray
{
namespace Ball
{
namespace scene
{
	class CameraNode;
}
class IBall;

class GameCamera
{
private:
protected:
	scene::CameraNode* m_node;
	IBall* m_ball;
	float m_radius;
public:
	GameCamera();
	virtual~GameCamera();

	void SetBall(IBall*ball);
	IBall* GetBall();

	void SetCamera(scene::CameraNode*camera);
	scene::CameraNode* GetCamera();

	void Update(float dt);
};


}
}

#endif
