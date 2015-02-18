
/********************************************************************
	created:	2009/03/08
	created:	8:3:2009   13:22
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\JobOrder.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	JobOrder
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___JobOrder___
#define ___JobOrder___

#include "CompileConfig.h"
#include "mTypes.h"
#include "IDelegate.h"
#include "GCPtr.h"

namespace mray{

// Job order , used for asynchronous processing and loading
class JobOrder
{
protected:
	uint m_id;
	int m_targetQueue;//-1 for any

public:
	JobOrder():m_id(0),m_targetQueue(-1)
	{}
	virtual~JobOrder(){}
	virtual inline void SetID(uint id){m_id=id;}
	virtual inline uint GetID(){return m_id;}
	virtual bool ExecuteJob()=0;

	void SetTargetQueue(int q){m_targetQueue=q;}
	int GetTargetQueue(){return m_targetQueue;}
};

MakeSharedPtrType(JobOrder);


typedef IDelegate2<void,JobOrder*,bool>* JobLoadCompleteDelegate;

}


#endif //___JobOrder___
