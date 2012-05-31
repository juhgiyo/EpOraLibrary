/*! 
@file epDateTime.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eporalibrary>
@date May 30, 2012
@brief OracleDB DateTime Interface
@version 1.0

@section LICENSE

Copyright (C) 2012  Woong Gyu La <juhgiyo@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

@section DESCRIPTION

An Interface for OracleDB DateTime.

@section NOTICE

EpOraLibrary is developed by referencing oraLib 0.0.3, which was developed by 606u <606u@dir.bg> 
<http://606u.dir.bg/>
606u had kindly approved using his/her oraLib freely for EpOraLibrary development.
*/
#ifndef __EP_DATETIME_H__
#define __EP_DATETIME_H__

#include "epOraLib.h"
#include "epOraDefines.h"
#include <oci.h>

namespace epol {




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


}

#endif //__EP_DATETIME_H__