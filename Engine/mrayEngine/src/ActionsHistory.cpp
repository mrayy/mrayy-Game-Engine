#include "stdafx.h"

#include "ActionsHistory.h"

namespace mray
{

ActionsHistory::ActionsHistory()
{
}
ActionsHistory::~ActionsHistory()
{
	Clear();
}

void ActionsHistory::Clear()
{
	while(!m_undoStack.empty())
	{
		delete m_undoStack.top();
		m_undoStack.pop();
	}
	while(!m_redoStack.empty())
	{
		delete m_redoStack.top();
		m_redoStack.pop();
	}
}
void ActionsHistory::SetMaxUndoActions(int c)
{
	m_maxUndoActions=c;
}
int ActionsHistory::GetMaxUndoActions()
{
	return m_maxUndoActions;
}

void ActionsHistory::PushAction(IActionHistoryRecord*record)
{
	m_undoStack.push(record);
}

IActionHistoryRecord* ActionsHistory::GetNextUndo()
{
	if(m_undoStack.empty())
		return 0;
	return m_undoStack.top();
}
IActionHistoryRecord* ActionsHistory::GetNextRedo()
{
	if(m_redoStack.empty())
		return 0;
	return m_redoStack.top();
}

bool ActionsHistory::Undo()
{
	if(m_undoStack.empty())
		return false;
	IActionHistoryRecord* action=m_undoStack.top();
	action->Undo();
	m_redoStack.push(action);
	return true;
}
bool ActionsHistory::Rndo()
{
	if(m_redoStack.empty())
		return false;
	IActionHistoryRecord* action=m_redoStack.top();
	action->Redo();
	m_undoStack.push(action);
	return true;
}

}
