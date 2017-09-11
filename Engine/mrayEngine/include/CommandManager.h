

/********************************************************************
	created:	2009/05/04
	created:	4:5:2009   20:05
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\CommandManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	CommandManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___CommandManager___
#define ___CommandManager___

#include "compileconfig.h"
#include "ISingleton.h"
#include "GCPtr.h"

#include "ICommand.h"
#include "IDelegate.h"


namespace mray{

	typedef std::map<core::string,GCPtr<ICommand>> CommandList;

class MRAY_DLL CommandManager
{
private:
protected:
	CommandList m_commands;
	core::string m_lastMsg;
	std::vector<core::string> m_args;
public:

	DelegateEvent1<const core::string&> MessageLog;

	CommandManager();
	virtual~CommandManager();

	void addCommand(GCPtr<ICommand> cmd);
	
	bool execCommand(const core::string&cmd);

	void removeCommand(GCPtr<ICommand> cmd);
	void removeCommand(const core::string&name);

	const core::string&getLastMessage();

	const CommandList& getCommands();

	// enable/disable command
	void enableCommand(const core::string&name,bool e);
};

}


#endif //___CommandManager___
