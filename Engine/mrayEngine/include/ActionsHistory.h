

/********************************************************************
	created:	2010/06/24
	created:	24:6:2010   11:42
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ActionsHistory.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ActionsHistory
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ActionsHistory_h__
#define ActionsHistory_h__


#include "mstring.h"
#include <stack>

namespace mray
{
	class IActionHistoryRecord
	{
	public:
		virtual core::string GetDescription()=0;
		virtual void Redo()=0;
		virtual void Undo()=0;
	};

class MRAY_DLL ActionsHistory
{
private:
protected:
	typedef std::stack<IActionHistoryRecord*> ActionStack;
	ActionStack m_undoStack;
	ActionStack m_redoStack;
	int m_maxUndoActions;
public:
	ActionsHistory();
	virtual ~ActionsHistory();
	
	void Clear();
	void SetMaxUndoActions(int c);
	int GetMaxUndoActions();

	virtual void PushAction(IActionHistoryRecord*record);

	virtual IActionHistoryRecord* GetNextUndo();
	virtual IActionHistoryRecord* GetNextRedo();

	virtual bool Undo();
	virtual bool Rndo();
};

}
#endif // ActionsHistory_h__
