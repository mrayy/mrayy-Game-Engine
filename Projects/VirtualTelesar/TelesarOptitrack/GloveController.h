

/********************************************************************
	created:	2013/05/13
	created:	13:5:2013   22:43
	filename: 	C:\Development\mrayEngine\Projects\VirtualTelesar\TelesarOptitrack\GloveController.h
	file path:	C:\Development\mrayEngine\Projects\VirtualTelesar\TelesarOptitrack
	file base:	GloveController
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GloveController__
#define __GloveController__


#include "fglove.h"

#include <vector>

namespace mray
{

enum FingerIndex
{
	Thumb=0,
	Index=3,
	Middle=6,
	Ring=9,
	Little=12
};
class GloveController
{
protected:
	fdGlove* m_glove;
	int m_hand;
public:
	GloveController(const std::string&pref);
	virtual~GloveController();

	void Init();
	bool Open(int hand);
	void Close();
	void Calibrate();
	void Rescan();
	float GetFingerValue(FingerIndex index);

	void Send();
};

}


#endif
