
#include "stdafx.h"
#include "TextLogDevice.h"


namespace mray
{

TextLogDevice::TextLogDevice()
{
	logEditBox=0;
}
TextLogDevice::~TextLogDevice()
{
}
void TextLogDevice::SetEditBox(CXTListCtrl* edt)
{
	logEditBox=edt;
}
void TextLogDevice::recordMessage(const core::string&msg,ELogLevel level)
{
	if(!logEditBox)
		return;
	core::string levelStr=gLogManager.getLogLevelStr(level);
	int i=logEditBox->GetItemCount();
	logEditBox->InsertItem(i, levelStr.c_str(), 0);
	logEditBox->SetItem(i,1,LVIF_TEXT, msg.c_str(), 0, NULL, NULL, NULL);
	switch(level)
	{
	case ELL_INFO:
		logEditBox->SetRowColor(i, 0, RGB(100,100,255));
		break;
	case ELL_SUCCESS:
		logEditBox->SetRowColor(i, 0, RGB(0,255,0));
		break;
	case ELL_ERROR:
		logEditBox->SetRowColor(i, 0, RGB(255,50,50));
		break;
	case ELL_WARNING:
		logEditBox->SetRowColor(i, 0, RGB(255,120,0));
		break;
	}
}
void TextLogDevice::log(const core::string &msg,const core::string &info,ELogLevel level)
{
	core::string finalMsg;

	int d=m_sections.size();
	core::string tab;
	for(int i=0;i<d;++i)
		tab+=mT("\t");

	finalMsg=tab+msg+mT(":");
	finalMsg+=info;

	recordMessage(finalMsg,level);
}
void TextLogDevice::log(const core::string &msg,ELogLevel level)
{
	core::string finalMsg;

	int d=m_sections.size();
	core::string tab;
	for(int i=0;i<d;++i)
		tab+=mT("\t");


	finalMsg=tab+msg;

	recordMessage(finalMsg,level);
}

void TextLogDevice::startSection(const core::string &name)
{
}
void TextLogDevice::endSection(bool Success)
{
}

void TextLogDevice::close()
{
}

void TextLogDevice::flush()
{
}



}
