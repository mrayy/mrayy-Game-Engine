


/********************************************************************
	created:	2014/04/30
	created:	30:4:2014   17:41
	filename: 	F:\Development\mrayEngine\Engine\mrayEngine\DateTime.h
	file path:	F:\Development\mrayEngine\Engine\mrayEngine
	file base:	DateTime
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __DateTime__
#define __DateTime__


#include "mstring.h"

namespace mray
{
namespace core
{

class MRAY_CORE_DLL CTime
{
protected:
	int m_hour;
	int m_minute;
	int m_second;

public:

	CTime();
	CTime(const CTime& o);
	CTime(int hour, int minute, int second);

	void SetTime(int hour, int minute, int second);
	void SetHour(int hour);
	void SetMinute(int minute);
	void SetSecond(int second);

	int GetHour(bool _24Hour=true)const;
	bool IsPM()const;
	bool IsAM()const;
	int GetMinute()const;
	int GetSecond()const;

	bool operator<(const CTime& o)const;
	bool operator>(const CTime& o)const;
	bool operator==(const CTime& o)const;
	bool operator!=(const CTime& o)const;


	static core::string ToString(const CTime& time);
	static core::string ToString(const CTime& time, bool _24Format,bool seconds=true);

	static bool Parse(const core::string& str, CTime &tm);
	static CTime Parse(const core::string& str);

	static CTime Now();
};

class MRAY_CORE_DLL CDate
{
protected:
	int m_year;
	int m_month;
	int m_day;
public:

	CDate();
	CDate(const CDate& o);
	CDate(int year, int month, int day);

	void SetDate(int year, int month, int day);
	void SetYear(int year);
	void SetMonth(int month);
	void SetDay(int day);

	int GetYear()const;
	int GetMonth()const;
	int GetDay()const;


	bool operator<(const CDate& o)const;
	bool operator>(const CDate& o)const;
	bool operator==(const CDate& o)const;
	bool operator!=(const CDate& o)const;

	static core::string ToString(const CDate& date);

	static bool Parse(const core::string& str, CDate &tm);
	static CDate Parse(const core::string& str);
	
	static CDate Now();
};

class MRAY_CORE_DLL DateTime
{
protected:
	CDate m_date;
	CTime m_time;
public:
	DateTime();
	DateTime(const DateTime& o);
	DateTime(const CDate& date,const CTime& time);

	void SetDate(const CDate& date);
	void SetDate(int year, int month, int day);

	void SetTime(const CTime& time);
	void SetTime(int hour, int minute, int second);

	const CDate& GetDate()const;
	const CTime& GetTime()const;
	CDate& GetDate();
	CTime& GetTime();


	bool operator<(const DateTime& o)const;
	bool operator>(const DateTime& o)const;
	bool operator==(const DateTime& o)const;
	bool operator!=(const DateTime& o)const;

	static core::string ToString(const DateTime& dt);

	static bool Parse(const core::string& str, DateTime &tm);
	static DateTime Parse(const core::string& str);
	
	static DateTime Now();
};

}
}

#endif
