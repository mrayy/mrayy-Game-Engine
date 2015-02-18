#include "stdafx.h"


#include "TimeTicket.h"




namespace mray
{
const TimeTicket TimeTicket::Zero(0,0);
const TimeTicket TimeTicket::Max(23,59);


TimeTicket::TimeTicket():m_hour(0),m_min(0),m_minParts(0)
{
}
TimeTicket::TimeTicket(uchar h,uchar m):m_hour(h),m_min(m),m_minParts(0)
{
}
TimeTicket::~TimeTicket()
{
}

void TimeTicket::SetTime(uchar hour,uchar min)
{
	m_hour=hour;
	m_min=min;
	m_minParts=0;
}
uchar TimeTicket::GetHours()const
{
	return m_hour;
}
uchar TimeTicket::GetMinutes()const
{
	return m_min;
}

TimeTicket TimeTicket::operator+(const TimeTicket&o)const
{
	TimeTicket t(*this);
	t+=o;
	return t;
}
TimeTicket TimeTicket::operator+(double v)const
{
	TimeTicket t(*this);
	t+=v;
	return t;
}
TimeTicket& TimeTicket::operator+=(const TimeTicket&o)
{
	TimeTicket t;
	t.m_hour=((m_hour+o.m_hour)+(m_min+o.m_min)/60)%24;
	t.m_min=((int)((m_min+o.m_min)+(m_minParts+o.m_minParts)/100))%60;
	t.m_minParts=(m_minParts+o.m_minParts)-(int)(m_minParts+o.m_minParts);
	*this=t;
	return *this;
}
TimeTicket& TimeTicket::operator+=(double v)
{
	m_minParts=(m_minParts+v)-(int)(m_minParts+v);
	m_min+=((int)v)/100;
	m_hour=(m_hour+m_min/60)%24;
	m_min=m_min%60;
	return *this;
}


double TimeTicket::GetTime0To1()const
{
	return math::Min(1.0,GetTimeDouble()/Max.GetTimeDouble());
}
double TimeTicket::GetTimeDouble()const
{
	return ((m_hour*24.0+m_min)*60.0)+m_minParts;
}

}