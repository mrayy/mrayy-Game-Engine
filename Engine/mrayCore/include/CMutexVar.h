

/********************************************************************
	created:	2010/10/15
	created:	15:10:2010   21:12
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\CMutexVar.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	CMutexVar
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef CMutexVar_h__
#define CMutexVar_h__

#include "IMutex.h"

namespace mray
{
namespace OS
{

template <typename T>
class MRAY_CORE_DLL  CMutexVar
{
	IMutex* m_mutex;
	T m_var;
public:
	CMutexVar() :m_mutex(0)
	{	}
	CMutexVar(IMutex* m):m_mutex(m)
	{	}
	CMutexVar(IMutex* m,T v):m_var(v)
	{	
	}
	virtual~CMutexVar(){
		if(m_mutex)
			delete m_mutex;
	}
	void setMutex(IMutex* m){
		m_mutex=m;
	}
	void lock(){
		if(m_mutex)
			m_mutex->lock();
	}
	T &operator= (const T&other){
		m_var=other;
		return m_var;
	}
	T &operator() (){
		return m_var;
	}
	const T &operator() () const{
		return m_var;
	}
	void unlock(){
		if(m_mutex)
			m_mutex->unlock();
	}
};

}
}
#endif // CMutexVar_h__

