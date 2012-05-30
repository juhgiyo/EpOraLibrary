#ifndef __EP_DATETIME_H__
#define __EP_DATETIME_H__

#include "epOraLib.h"
#include <oci.h>

namespace epol {


	enum MonthsEnum
	{
		JAN = 1,
		FEB,
		MAR,
		APR,
		MAY,
		JUN,
		JUL,
		AUG,
		SEP,
		OCT,
		NOV,
		DEC
	};


	class EP_ORACLELIB DateTime
	{

	public:
		inline DateTime () {};

		inline DateTime (unsigned char day, MonthsEnum month, signed short year, unsigned char hour = 0, unsigned char minute = 0, unsigned char second = 0)
		{
			m_year=year;
			m_month=month;
			m_day=day;
			m_hour=hour;
			m_minute=minute;
			m_second=second;
		};

		inline DateTime (const OCIDate& ociDate)
		{ 
			operator = (ociDate); 
		};

		inline unsigned char GetDay () const 
		{ 
			return (m_day); 
		};

		inline void SetDay (unsigned char day)
		{ 
			m_day = day; 
		};

		inline MonthsEnum GetMonth () const
		{ 
			return m_month; 
		};
		
		inline void SetMonth (MonthsEnum month)
		{ 
			m_month = month; 
		};

		inline signed short GetYear () const
		{ 
			return m_year; 
		};
		
		inline void SetYear (signed short year)
		{ 
			m_year = year; 
		};

		inline unsigned char GetHour () const
		{ 
			return m_hour; 
		};
		
		inline void SetHour (unsigned char hour)
		{ 
			m_hour = hour; 
		};

		inline unsigned char GetMinute () const
		{ 
			return m_month; 
		};
		
		inline void GetMinute (unsigned char minute)
		{ 
			m_month = static_cast<MonthsEnum>(minute); 
		};

		inline unsigned char GetSecond () const
		{ 
			return m_second; 
		};
		
		inline void SetSecond (unsigned char second)
		{ 
			m_second = second; 
		};

		inline DateTime& operator = (const OCIDate& ociDate)
		{
			m_year = ociDate.OCIDateYYYY;
			m_month = (MonthsEnum) (ociDate.OCIDateMM);
			m_day = ociDate.OCIDateDD;
			m_hour = ociDate.OCIDateTime.OCITimeHH;
			m_minute = ociDate.OCIDateTime.OCITimeMI;
			m_second = ociDate.OCIDateTime.OCITimeSS;
			return (*this);
		};

		inline void SetOciDate(OCIDate& retOciDate) const
		{
			retOciDate.OCIDateYYYY = m_year;
			retOciDate.OCIDateMM = static_cast<ub1>(m_month);
			retOciDate.OCIDateDD = m_day;
			retOciDate.OCIDateTime.OCITimeHH = m_hour;
			retOciDate.OCIDateTime.OCITimeMI = m_minute;
			retOciDate.OCIDateTime.OCITimeSS = m_second;
		};

	private:
		signed short m_year;
		MonthsEnum	m_month;
		unsigned char m_day;
		unsigned char m_hour;
		unsigned char m_minute;
		unsigned char m_second;


	};


}; 

#endif //__EP_DATETIME_H__