

#include "stdafx.h"
#include "InputKeyMap.h"
#include "KeyboardEvent.h"


namespace mray
{

namespace controllers
{

	core::string InputKeyMap::CommandInfo::ToString(bool withInfo)const 
	{
		core::string ret;
		if (ctrl)
		{
			ret += "Ctrl + ";
		}
		if (shift)
		{
			ret += "Shift + ";
		}
		ret += getCodeString(code);
		if (withInfo && info!="")
		{
			ret += " : " + info;
		}
		return ret;
	}

InputKeyMap::InputKeyMap()
{

}

InputKeyMap::~InputKeyMap()
{

}


bool InputKeyMap::RegisterKey(uint cmd, EKEY_CODE key, bool ctrl, bool shift, const core::string& info)
{
	if (cmd == 0)return false;
	int index = GetCommandIndex(cmd);
	if (index != -1)
		return false;


	CommandInfo c;
	c.cmd=cmd;
	c.code = key;
	c.ctrl = ctrl;
	c.shift = shift;
	c.info = info;
	m_commands.push_back(c);

	return true;
}

bool InputKeyMap::UpdateKey(uint cmd, EKEY_CODE key, bool ctrl, bool shift, const core::string& info)
{
	if (cmd == 0)return false;
	int index = GetCommandIndex(cmd);
	if (index == -1)
		return false;


	CommandInfo c;
	c.cmd = cmd;
	c.code = key;
	c.ctrl = ctrl;
	c.shift = shift;
	c.info = info;
	m_commands[index]=(c);

	return true;
}

void InputKeyMap::RemoveKey(uint cmd)
{
	int index = GetCommandIndex(cmd);
	if (index == -1)
		return;

	m_commands.erase(m_commands.begin() + index);

}
int InputKeyMap::GetCommandIndex(uint cmd)
{
	for (int i = 0; i < m_commands.size(); ++i)
	{
		if (m_commands[i].cmd == cmd)
			return i;
	}
	return -1;
}

uint InputKeyMap::GetCommand(const KeyboardEvent* e)
{
	for (int i = 0; i < m_commands.size(); ++i)
	{
		CommandInfo &c = m_commands[i];
		if (e->key == c.code && 
			(e->ctrl == c.ctrl) &&
			( e->shift==c.shift))
		{
			return c.cmd;
		}
	}
	return 0;
}

void InputKeyMap::ClearKeys()
{
	m_commands.clear();
}


}
}


