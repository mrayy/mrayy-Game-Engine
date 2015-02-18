

/********************************************************************
	created:	2010/06/22
	created:	22:6:2010   9:35
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\TimeTicket.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	TimeTicket
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef TimeTicket_h__
#define TimeTicket_h__

#include "compileConfig.h"
#include "mtypes.h"

namespace mray
{

class MRAY_DLL TimeTicket
{
private:
protected:
	uchar m_hour;
	uchar m_min;
	double m_minParts;
public:
	TimeTicket();
	TimeTicket(uchar h,uchar m);
	virtual ~TimeTicket();
	
	void SetTime(uchar hour,uchar min);
	uchar GetHours()const;
	uchar GetMinutes()const;

	double GetTime0To1()const;
	double GetTimeDouble()const;

	TimeTicket operator+(const TimeTicket&o)const;
	TimeTicket operator+(double v)const;
	TimeTicket& operator+=(const TimeTicket&o);
	TimeTicket& operator+=(double v);

	static const TimeTicket Zero;
	static const TimeTicket Max;
};

}
#endif // TimeTicket_h__
