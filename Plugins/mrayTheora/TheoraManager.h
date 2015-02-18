
/********************************************************************
	created:	2009/05/17
	created:	17:5:2009   21:51
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayTheora\TheoraManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayTheora
	file base:	TheoraManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___TheoraManager___
#define ___TheoraManager___

#include "IVideoSource.h"

#include <GCPtr.h>
#include <IThread.h>
#include <IVideoClipManager.h>

#include <list>


namespace mray{
	namespace sound{
		class ISoundManager;
	}
namespace video{


	class TheoraData;
	class TheoraResourceLoader;
class TheoraManager:public IVideoClipManager,public ISingleton<TheoraManager>
{
private:
protected:

	TheoraData* m_data;

	typedef std::list<IVideoSourcePtr> VideoList;
	VideoList m_videos;

	std::vector<TheoraResourceLoader*> m_loaders;

public:
	TheoraManager(int workersCount=1);
	virtual~TheoraManager();

	IVideoSourcePtr CreateVideo(const core::string& name,EVideoOutputMode output_mode=EVideo_RGB,int numPrecachedOverride=0,bool usePower2Stride=0);
	IVideoSourcePtr CreateVideoFromFile(const core::string& fileName,EVideoOutputMode output_mode=EVideo_RGB,int numPrecachedOverride=0,bool usePower2Stride=0);
	IVideoSourcePtr CreateVideoFromStream(OS::IStream* stream,EVideoOutputMode output_mode=EVideo_RGB,int numPrecachedOverride=0,bool usePower2Stride=0);
	void RemoveVideo(IVideoSource* v);
	void ClearVideos();

	void setSoundManager(sound::ISoundManager*m);
	sound::ISoundManager* getSoundManager();

	void Update(float dt);
};

}
}


#endif //___TheoraManager___
