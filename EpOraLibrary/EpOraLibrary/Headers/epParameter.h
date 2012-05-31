/*! 
@file epParameter.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eporalibrary>
@date May 30, 2012
@brief OracleDB Parameter Interface
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

An Interface for OracleDB Parameter.

@section NOTICE

EpOraLibrary is developed by referencing oraLib 0.0.3, which was developed by 606u <606u@dir.bg> 
<http://606u.dir.bg/>
606u had kindly approved using his/her oraLib freely for EpOraLibrary development.
*/
#ifndef	__EP_PARAMETER_H__
#define	__EP_PARAMETER_H__

#include "epOraLib.h"
#include "epDateTime.h"
#include <string>

using namespace epl;

namespace epol {


	class ResultSet;

	class EP_ORACLELIB Parameter
	{
		// friends
		friend class Statement;

	public:

		// returns whether parameter value is null
		inline bool IsNull () const 
		{
			return (m_indicator == -1); 
		};

		EpTString ToString () const;

		double ToDouble () const;

		// returns parameter value as a long
		long ToLong () const;

		// returns parameter value as a date/time helper object
		DateTime ToDateTime () const;

		// returns a resultset for a cursor bound variable
		ResultSet& ToResultSet ();

	private:
		// public not creatable; use statement.bind instead
		// attaches parameter to a statement
		// when type is set to DT_UNKNOWN type is taken from name's prefix
		Parameter (Statement *to, const TCHAR *name, DataTypesEnum type = DT_UNKNOWN, unsigned int fetchSize = FETCH_SIZE);

		~Parameter ();

		// private copy-constructor and assignment operator - class could not be copied
		Parameter ( const Parameter& /* var */) 
		{ 
			/* could not be copy-constructed */ 
		};
		Parameter& operator = (const Parameter& /* var */) 
		{ 
			return (*this); /* could not be copy-constructed */ 
		};

		// initialize private data
		void initialize ();

		// free resources allocated
		void cleanUp ();

		// attaches parameter to a statement
		// when type is set to DT_UNKNOWN type is taken from name's prefix
		void attach (Statement *to, const TCHAR *name, DataTypesEnum type = DT_UNKNOWN, unsigned int fetchSize = FETCH_SIZE);

		// sets-up name, type, oci_type and size, depending on type value
		// when type is set to DT_UNKNOWN type is taken from name's prefix
		void setupType (const TCHAR *paramName, DataTypesEnum type = DT_UNKNOWN);

		// binds an input and/or output parameter to the statement to
		void bind (Statement *to);

		// binds a result set - fetch_size rows will be retrieved on each step
		void bindResultSet (Statement *to,unsigned int fetchSize = FETCH_SIZE);


		// sets parameter value to null
		inline void toNull () 
		{ 
			m_indicator = -1; 
		}

		// sets parameter value to some text
		Parameter& operator = (EpTString text);

		// sets parameter value to some double
		Parameter& operator = (double value);

		// sets parameter value to some long
		Parameter& operator = (long value);

		// sets parameter value to some date/time
		Parameter& operator = (const DateTime& dateTime);


		EpTString		m_paramName;		// in the exact case, including leading ':'
		DataTypesEnum	m_paramType;		// as it will be returned
		unsigned short				m_ociType;		// oracle's data type
		unsigned short				m_size;			// number of bytes required for

		short				m_indicator;		// 0 - ok; -1 - null
		unsigned short				m_dataLen;		// number of bytes returned (used for text)
		char			*m_fetchBuffer;	// where data is returned

		bool			m_isArray;		// with values?

		Statement		*m_stmt;			// parameter is bound to
		OCIBind			*m_bindHandle;
		OCIStmt			*m_rsHandle;		// if parameter is a result set
		ResultSet		*m_resultSet;	// if parameter is a result set



	}; // parameter class


}


#endif	// __EP_PARAMETER_H__
