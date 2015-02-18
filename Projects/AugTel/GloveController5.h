

/********************************************************************
	created:	2013/05/13
	created:	13:5:2013   22:43
	filename: 	C:\Development\mrayEngine\Projects\VirtualTelesar\TelesarOptitrack\GloveController5.h
	file path:	C:\Development\mrayEngine\Projects\VirtualTelesar\TelesarOptitrack
	file base:	GloveController5
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GloveController5__
#define __GloveController5__

#include "IFingerDataProvider.h"

#include "fglove.h"

#include <vector>

namespace mray
{
namespace AugTel
{

enum FingerIndex
{
	Thumb=0,
	Index=3,
	Middle=6,
	Ring=9,
	Little=12
};
class GloveController5Impl;

class GloveController5 :public IFingerDataProvider
{
protected:
	GloveController5Impl* m_impl;
public:
	GloveController5(const std::string&pref);
	virtual~GloveController5();

	bool SetHand(int hand);
	bool Start();
	void Stop();
	void Calibrate();
	void Rescan();
	float GetFingerValue(FingerIndex index);
	virtual float GetFingerValue(int finger, int part) ;

	void Update(float dt);

	virtual void SendMessage(const core::string& msg, const core::string& value);
};

}
}


#endif
