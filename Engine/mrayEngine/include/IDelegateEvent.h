

/********************************************************************
	created:	2009/06/02
	created:	2:6:2009   19:02
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IDelegateEvent.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IDelegateEvent
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IDelegateEvent___
#define ___IDelegateEvent___

#include "mString.h"

namespace mray{

	class IDelegateEvent
	{
	protected:
		core::string m_name;
	public:
		IDelegateEvent(const core::string& name):m_name(name)
		{
		}

		void setName(const core::string& name){
			m_name=name;
		}
		const core::string& getName()const
		{
			return m_name;
		}
	};

}


#endif //___IDelegateEvent___
