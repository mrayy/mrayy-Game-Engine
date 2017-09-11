
#include "stdafx.h"
#include "DateTime.h"
#include "StringConverter.h"
#include "StringUtil.h"
#include <time.h>
#include <sstream>
#include <iomanip>

namespace mray
{
namespace core
{


CTime::CTime() :m_hour(0), m_minute(0), m_second(0)
{
}
CTime::CTime(const CTime& o) : m_hour(o.m_hour), m_minute(o.m_minute), m_second(o.m_second)
{
}

CTime::CTime(int hour, int minute, int second) : m_hour(0), m_minute(0), m_second(0)
{
	SetHour(hour);
	SetMinute(minute);
	SetSecond(second);
}


void CTime::SetTime(int hour, int minute, int second)
{
	SetHour(hour);
	SetMinute(minute);
	SetSecond(second);
}

void CTime::SetHour(int hour)
{
	if (hour < 0)
		return;
	m_hour = hour % 24;
}

void CTime::SetMinute(int minute)
{
	if (minute < 0)
		return;
	m_minute = minute % 60;
}

void CTime::SetSecond(int second)
{
	if (second < 0)
		return;
	m_second = second % 60;
}


int CTime::GetHour(bool _24Hour )const
{
	if (_24Hour)
		return m_hour;
	if (m_hour>12)
		return m_hour - 12;
	return m_hour;
}

bool CTime::IsPM()const
{
	return m_hour > 12;
}

bool CTime::IsAM()const
{
	return !IsPM();
}

int CTime::GetMinute()const
{
	return m_minute;
}

int CTime::GetSecond()const
{
	return m_second;
}

bool CTime::operator<(const CTime& o)const
{
	return *this>o;
}
bool CTime::operator>(const CTime& o)const
{
	if (m_hour > o.m_hour)
		return true;
	else if (m_hour < o.m_hour)
		return false;

	if (m_minute > o.m_minute)
		return true;
	else if (m_minute < o.m_minute)
		return false;


	if (m_second > o.m_second)
		return true;
	else if (m_second < o.m_second)
		return false;

	return true;
}
bool CTime::operator==(const CTime& o)const
{
	return m_hour == o.m_hour &&
		m_minute == o.m_minute &&
		m_second == o.m_second;
}
bool CTime::operator!=(const CTime& o)const
{
	return !(*this == o);
}

core::string CTime::ToString(const CTime& time)
{
	return ToString(time, true, true);
}

core::string CTime::ToString(const CTime& time, bool _24Format, bool seconds)
{
	std::stringstream stream;
	stream << std::setfill('0') << std::setw(2) << time.GetHour(_24Format);
	stream << mT(":") << std::setfill('0') << std::setw(2) << time.GetMinute();
	if(seconds)
		stream << mT(":") << std::setfill('0') << std::setw(2) << time.GetSecond();
	if (!_24Format)
		stream << "-" <<  (time.IsAM() ? "AM" : "PM");
	return stream.str();
}


bool CTime::Parse(const core::string& str, CTime &tm)
{
	std::vector<core::string> tmpArray = core::StringUtil::Split(str, mT(":"));
	int h = 0, m = 0, s = 0;
	if (tmpArray.size() == 3)
	{
		 h = core::StringConverter::toInt(tmpArray[0]);
		 m = core::StringConverter::toInt(tmpArray[1]);

		std::vector<core::string> arr2 = core::StringUtil::Split(tmpArray[2], mT("-"), 2);
		 s = core::StringConverter::toInt(arr2[0]);
		if (arr2.size() == 2)
		{
			if (arr2[1].equals_ignore_case("PM"))
				h += 12;
		}
		tm.SetTime(h, m, s);
		return true;
	}

	if (tmpArray.size() == 2)
	{
		 h = core::StringConverter::toInt(tmpArray[0]);
		std::vector<core::string> arr2 = core::StringUtil::Split(tmpArray[1], mT("-"), 2);
		 m = core::StringConverter::toInt(arr2[0]);
		if (arr2.size() == 2)
		{
			if (arr2[1].equals_ignore_case("PM"))
				h += 12;
		}

		tm.SetTime(h, m, 0);
		return true;
	}

	return false;
}

CTime CTime::Parse(const core::string& str)
{
	CTime ret;
	Parse(str, ret);
	return ret;
}


CTime CTime::Now()
{
	CTime ret;
	time_t  v;
	time(&v);

	struct tm* t = localtime(&v);

	ret.SetTime(t->tm_hour, t->tm_min, t->tm_sec);
	return ret;
}

//////////////////////////////////////////////////////////////////////////

CDate::CDate() :m_year(1988), m_month(1), m_day(15)
{
}
CDate::CDate(const CDate& o) : m_year(o.m_year), m_month(o.m_month), m_day(o.m_day)
{
}

CDate::CDate(int year, int month, int day) : m_year(1988), m_month(1), m_day(15)
{
	SetYear(year);
	SetMonth(month);
	SetDay(day);
}

void CDate::SetDate(int year, int month, int day)
{
	SetYear(year);
	SetMonth(month);
	SetDay(day);
}

void CDate::SetYear(int year)
{
	if (year < 0)
		return;
	m_year = year;
}

void CDate::SetMonth(int month)
{
	if (month <= 0)
		return;
	m_month = month%12;
}

void CDate::SetDay(int day)
{
	if (day <= 0)
		return;
	m_day = day%31;
}


int CDate::GetYear()const
{
	return m_year;
}

int CDate::GetMonth()const
{
	return m_month; 
}

int CDate::GetDay()const
{
	return m_day;
}

bool CDate::operator<(const CDate& o)const
{
	return *this>o;
}
bool CDate::operator>(const CDate& o)const
{
	if (m_year > o.m_year)
		return true;
	else if (m_year < o.m_year)
		return false;

	if (m_month > o.m_month)
		return true;
	else if (m_month < o.m_month)
		return false;


	if (m_day > o.m_day)
		return true;
	else if (m_day < o.m_day)
		return false;
	return true;
}
bool CDate::operator==(const CDate& o)const
{
	return m_year == o.m_year &&
		m_month == o.m_month &&
		m_day == o.m_day;
}
bool CDate::operator!=(const CDate& o)const
{
	return !(*this == o);
}


core::string CDate::ToString(const CDate& date)
{
	std::ostringstream stream;
	stream << date.GetDay() << mT("/") << date.GetMonth() << mT("/") << date.GetYear();
	return stream.str();
}


bool CDate::Parse(const core::string& str, CDate &tm)
{

	std::vector<core::string> tmpArray = core::StringUtil::Split(str, mT("/"));
	if (tmpArray.size() == 3)
	{
		int d = 0, m = 0, y = 0;
		d = core::StringConverter::toInt(tmpArray[0]);
		m = core::StringConverter::toInt(tmpArray[1]);
		y = core::StringConverter::toInt(tmpArray[2]);

		tm.SetDate(y, m, d);
		return true;
	}
	return false;
}

CDate CDate::Parse(const core::string& str)
{
	CDate ret;
	Parse(str, ret);
	return ret;
}

CDate CDate::Now()
{
	CDate ret;
	time_t  v;
	time(&v);

	struct tm* t = localtime(&v);

	ret.SetDate(t->tm_year, t->tm_mon, t->tm_mday);
	return ret;
}


//////////////////////////////////////////////////////////////////////////

DateTime::DateTime()
{
}

DateTime::DateTime(const DateTime& o) : m_date(o.m_date), m_time(o.m_time)
{
}
DateTime::DateTime(const CDate& date, const CTime& time) :m_date(date), m_time(time)
{
}


void DateTime::SetDate(const CDate& date)
{
	m_date = date;
}

void DateTime::SetDate(int year, int month, int day)
{
	m_date.SetDate(year, month, day);
}


void DateTime::SetTime(const CTime& time)
{
	m_time = time;
}

void DateTime::SetTime(int hour, int minute, int second)
{
	m_time.SetTime(hour, minute, second);
}


const CDate& DateTime::GetDate()const
{
	return m_date;
}

const CTime& DateTime::GetTime()const
{
	return m_time;
}
 CDate& DateTime::GetDate()
{
	return m_date;
}

 CTime& DateTime::GetTime()
{
	return m_time;
}

bool DateTime::operator<(const DateTime& o)const
{
	return *this>o;
}
bool DateTime::operator>(const DateTime& o)const
{
	if (m_date > o.m_date)
		return true;
	else if (m_date < o.m_date)
		return false;
	if (m_time > o.m_time)
		return true;
	else if (m_time < o.m_time)
		return false;

	return true;
}
bool DateTime::operator==(const DateTime& o)const
{
	return m_date == o.m_date && m_time == o.m_time;
}
bool DateTime::operator!=(const DateTime& o)const
{
	return !(*this == o);
}

core::string DateTime::ToString(const DateTime& dt)
{
	std::ostringstream stream;
	stream << dt.GetDate().GetDay() << mT("/") << dt.GetDate().GetMonth() << mT("/") << dt.GetDate().GetYear();
	stream << "-";
	stream << dt.GetTime().GetHour() << mT(":") << dt.GetTime().GetMinute() << mT(":") << dt.GetTime().GetSecond();
	return stream.str();
}


bool DateTime::Parse(const core::string& str, DateTime &tm)
{
	return false;
}

DateTime DateTime::Parse(const core::string& str)
{
	DateTime ret;
	Parse(str, ret);
	return ret;
}


DateTime DateTime::Now()
{
	DateTime ret;
	time_t  v;
	time(&v);

	struct tm* t = localtime(&v);

	ret.m_time.SetTime(t->tm_hour, t->tm_min, t->tm_sec);
	ret.m_date.SetDate(t->tm_year+1900, t->tm_mon+1, t->tm_mday);
	return ret;
}

}
}



