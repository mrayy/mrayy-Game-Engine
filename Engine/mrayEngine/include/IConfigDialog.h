
/********************************************************************
	created:	2009/02/27
	created:	27:2:2009   23:20
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IConfigDialog.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IConfigDialog
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IConfigDialog___
#define ___IConfigDialog___



//#include "OptionContainer.h"
#include "ISingleton.h"
#include "SOptionElement.h"

namespace mray{
	class OptionContainer;

class IConfigDialog:public ISingleton<IConfigDialog>
{
public:

	IConfigDialog(){}
	virtual ~IConfigDialog(){}

	virtual bool run(OptionContainer& options)=0;
};

}


#endif //___IConfigDialog___
