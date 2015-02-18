

/********************************************************************
	created:	2011/12/04
	created:	4:12:2011   16:32
	filename: 	d:\Development\mrayEngine\Engine\mrayAI\BHRepeatDecorator.h
	file path:	d:\Development\mrayEngine\Engine\mrayAI
	file base:	BHRepeatDecorator
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __BHRepeatDecorator__
#define __BHRepeatDecorator__

#include "IBHDecorator.h"

namespace mray
{
namespace AI
{

class MRAY_AI_DLL BHRepeatDecorator:public IBHDecorator
{
private:
protected:
	int m_repeatCount;
public:
	BHRepeatDecorator():m_repeatCount(1)
	{}
	virtual~BHRepeatDecorator(){}

	void SetIterationCount(int cnt){m_repeatCount=cnt;}
	int GetIterationCount()const{return m_repeatCount;}

	virtual bool Access(float dt)
	{
		if(m_node)
		{
			for(int i=0;i<m_repeatCount;++i)
			{
				if(!m_node->Access(dt))
					return false;
			}
		}
		return true;
	}
};

}
}

#endif

