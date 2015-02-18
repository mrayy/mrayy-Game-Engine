

/********************************************************************
	created:	2011/03/24
	created:	24:3:2011   14:49
	filename: 	i:\Programing\GameEngine\mrayEngine\Engine\mrayEngine\include\FunctionProfileManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\Engine\mrayEngine\include
	file base:	FunctionProfileManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef FunctionProfileManager_h__
#define FunctionProfileManager_h__

#include "ISingleton.h"
#include "mstring.h"
#include "ListenerContainer.h"


namespace mray
{
	class FunctionProfiler;

	class IFunctionProfileListener
	{
	public:
		virtual void OnProfileBegin(FunctionProfiler*p){}
		virtual void OnProfileEnd(FunctionProfiler*p){}
	};
class MRAY_DLL FunctionProfileManager:public ISingleton<FunctionProfileManager>,public ListenerContainer<IFunctionProfileListener*>
{
public:
	struct ProfileSample;
	typedef std::map<uint,ProfileSample*> SampleList;
	struct ProfileSample
	{
		uint id;
		std::string name;
		int samplesCount;
		double totalTime;

		ProfileSample* parent;

		SampleList childs;

		ProfileSample();
		~ProfileSample();
		void Reset();
		void Clear();
	};
protected:
	SampleList m_samples;
	ProfileSample* m_currentSample;

	DECLARE_FIRE_METHOD(OnProfileBegin,(FunctionProfiler*p),(p));
	DECLARE_FIRE_METHOD(OnProfileEnd,(FunctionProfiler*p),(p));
public:
	FunctionProfileManager();

	void BeginProfiling(FunctionProfiler*p);
	void EndProfiling(FunctionProfiler*p);
};

}
#endif // FunctionProfileManager_h__

