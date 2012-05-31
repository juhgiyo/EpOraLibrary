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



	/*! 
	@class DateTime epDateTime.h
	@brief This is a class representing OracleDB DateTime

	Interface for the OracleDB DateTime.
	*/
	class EP_ORACLELIB DateTime
	{

	public:
		/*!
		Default Constructor

		Create DateTime object
		*/
		inline DateTime () 
		{

		}

		/*!
		Default Constructor

		Create DateTime object with given information
		@param[in] day the value of day
		@param[in] month the value of month
		@param[in] year the value of year
		@param[in] hour the value of hour
		@param[in] minute the value of minute
		@param[in] second the value of second
		*/
		inline DateTime (unsigned char day, MonthsEnum month, signed short year, unsigned char hour = 0, unsigned char minute = 0, unsigned char second = 0)
		{
			m_year=year;
			m_month=month;
			m_day=day;
			m_hour=hour;
			m_minute=minute;
			m_second=second;
		}

		/*!
		Default Constructor

		Create DateTime object with given OciDate object
		@param[in] ociDate the OCI Date object
		*/
		inline DateTime (const OCIDate& ociDate)
		{ 
			operator = (ociDate); 
		}

		/*!
		Return this object's day
		@return the value of day of this object
		*/
		inline unsigned char GetDay () const 
		{ 
			return (m_day); 
		}

		/*!
		Set the value of day with given day value
		@param[in] day the day value to set
		*/
		inline void SetDay (unsigned char day)
		{ 
			m_day = day; 
		};
/*!
		Return this object's month
		@return the value of month of this object
		*/
		inline MonthsEnum GetMonth () const
		{ 
			return m_month; 
		};
		/*!
		Set the value of month with given month value
		@param[in] month the month value to set
		*/
		inline void SetMonth (MonthsEnum month)
		{ 
			m_month = month; 
		};
		
		/*!
		Return this object's year
		@return the value of year of this object
		*/
		inline signed short GetYear () const
		{ 
			return m_year; 
		};
		/*!
		Set the value of year with given year value
		@param[in] year the year value to set
		*/
		inline void SetYear (signed short year)
		{ 
			m_year = year; 
		};

		/*!
		Return this object's hour
		@return the value of hour of this object
		*/
		inline unsigned char GetHour () const
		{ 
			return m_hour; 
		};
		/*!
		Set the value of hour with given hour value
		@param[in] hour the hour value to set
		*/
		inline void SetHour (unsigned char hour)
		{ 
			m_hour = hour; 
		};
		
		/*!
		Return this object's minute
		@return the value of minute of this object
		*/
		inline unsigned char GetMinute () const
		{ 
			return m_minute; 
		};
		
		/*!
		Set the value of minute with given minute value
		@param[in] minute the minute value to set
		*/
		inline void GetMinute (unsigned char minute)
		{ 
			m_minute = static_cast<MonthsEnum>(minute); 
		};
		
		/*!
		Return this object's second
		@return the value of second of this object
		*/
		inline unsigned char GetSecond () const
		{ 
			return m_second; 
		};
		
		/*!
		Set the value of second with given second value
		@param[in] second the second value to set
		*/
		inline void SetSecond (unsigned char second)
		{ 
			m_second = second; 
		};

		/*!
		Set the DateTime value from OCI Date object
		@param[in] ociDate the OCI Date object
		@return this object
		*/
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

		/*!
		Set the given OCI Date as current value
		@param[out] retOciDate the OCI Date object to set as current object
		*/
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
		/// year value
		signed short m_year;
		/// month value
		MonthsEnum	m_month;
		/// day value
		unsigned char m_day;
		/// hour value
		unsigned char m_hour;
		/// minute value
		unsigned char m_minute;
		/// second value
		unsigned char m_second;


	};


}

#endif //__EP_DATETIME_H__