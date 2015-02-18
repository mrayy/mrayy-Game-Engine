
/********************************************************************
	created:	2009/03/07
	created:	7:3:2009   19:34
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IGCCollector.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IGCCollector
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IGCCollector___
#define ___IGCCollector___

#include "ISingleton.h"
#include "GCInfo.h"

namespace mray{

	namespace OS
	{
		class IThread;
		class IMutex;
	}


class MRAY_DLL IGCCollector
{
public:
	virtual ~IGCCollector()
	{

	}
	class IInfoTemplate
	{
	public:
		virtual~IInfoTemplate(){
		}
		virtual void dispose()=0;
	};

	template <class T>
	class GCInfoTemplate:public IInfoTemplate
	{
		T*m_addr;
		GCInfo*m_info;
	public:
		GCInfoTemplate(GCInfo*info){
			m_info=info;
			m_addr=(T*)info->getAddr();
		}
		virtual ~GCInfoTemplate(){
			dispose();
		}
		virtual void dispose(){
			if(m_addr!=0){
				/*if(m_info->isArray()){
				delete [] m_addr;
				}else{*/
				delete m_addr;
				//}
				m_addr=0;
			}
			if(m_info){
				delete m_info;
				m_info=0;
			}
		}
	};
protected:
	virtual void inner_eraseAddress(const void*addr)=0;
	virtual void inner_addToDeadList(IInfoTemplate*info)=0;

	static bool m_isDestroyed;
	static IGCCollector *m_instance;

	virtual GCInfo*_getInfo(const void* addr)=0;
	virtual void _addInfo(GCInfo*info)=0;
public:

	static void Initialize();

	static IGCCollector&getInstance();
	static void shutdown();

	static GCInfo* GetInfo(const void* addr);
	static void AddInfo(GCInfo*info);



	virtual bool isMultiThreaded()=0;
	virtual void SetMultiThreaded(bool mt)=0;

	virtual void SetThread(OS::IThread*t)=0;
	virtual OS::IThread* GetThread()=0;
	virtual void SetMutex(OS::IMutex*memMtx,OS::IMutex*deadMtx)=0;


	template <class T>
	void addToDeadList(GCInfo*info,bool imediateRemove)
	{
		inner_eraseAddress(info->getAddr());

		/**/
		if(!isMultiThreaded() || imediateRemove || m_isDestroyed){
			GCInfoTemplate<T> infoTemp(info);
			infoTemp.dispose();
		}else
		{
			IInfoTemplate*infoTemp=new GCInfoTemplate<T>(info);
			inner_addToDeadList(infoTemp);
		}

	}

	virtual bool collect()=0;

	virtual int activeSize()=0;
	virtual int deadSize()=0;
};

}


#endif //___IGCCollector___
