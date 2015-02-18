


#ifndef LoadingScreen_h__
#define LoadingScreen_h__

#include "IGUIManager.h"
#include "GUILoadingScreen.h"

namespace mray
{
	namespace TBee
	{
		class TBRobotInfo;
	}
namespace AugTel
{
	class VideoRenderElement;
	
class LoadingScreen
{
protected:
	VideoRenderElement* m_video;
	GUI::IGUIManager* m_guiManager;
	GUI::IGUIPanelElement* m_guiroot;
	GUI::GUILoadingScreen* m_screenLayout;
	bool m_done;
	float m_startTime;
public:
	LoadingScreen();
	virtual ~LoadingScreen();

	void Init();

	void Start(TBee::TBRobotInfo* robot);

	void Draw(const math::rectf& rc);

	void Update(float dt);

	void End();

	bool IsDone();

	
};

}
}


#endif // LoadingScreen_h__
