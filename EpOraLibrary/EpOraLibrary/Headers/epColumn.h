/*! 
@file epColumn.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eporalibrary>
@date May 30, 2012
@brief OracleDB Table Column Interface
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

An Interface for OracleDB Table Column.

@section NOTICE

EpOraLibrary is developed by referencing oraLib 0.0.3, which was developed by 606u <606u@dir.bg> 
<http://606u.dir.bg/>
606u had kindly approved using his/her oraLib freely for EpOraLibrary development.

*/
#ifndef __EP_COLUMN_H__
#define __EP_COLUMN_H__
#include "epOraLib.h"
#include <string>
#include "epDateTime.h"

using namespace epl;

namespace epol
{
	class EP_ORACLELIB Column
	{
		friend class ResultSet;
	public:
		// returns whether column value is null
		bool IsNull () const;

		EpTString ToString() const;

		double ToDouble() const;

		long ToLong () const;

		DateTime ToDateTime() const;

	private:
		// public not creatable; use connection.select or statement.select
		// attaches this column to a result set; allocates memory for the fetch buffer
		Column (ResultSet *rs, const TCHAR *name, unsigned short ociDataType, unsigned int maxDataSize, int fetchSize = FETCH_SIZE);

		// public not deletable; deleted, when result set is released
		~Column ();

		// initialize data members
		void initialize ();

		// free resources allocated
		void cleanUp ();

		// private copy-constructor and assignment operator - class could not be copied
		Column (const Column& /* col */) 
		{ 
			/* could not be copy-constructed */ 
		}
		Column& operator = (const Column& /* col */) 
		{
			return (*this); /* could not be assigned */ 
		}


		EpTString m_colName;		// in the exact case
		DataTypesEnum m_colType;		// as it will be returned
		unsigned short m_ociType;		// oracle's data type
		int	m_size;			// number of bytes required for

		short* m_indicators;	// an array with indicators: 0 - ok; -1 - null
		unsigned short* m_dataLengths;		// an array with data lengths (for text columns)
		char* m_fetchBuffer;	// where data is returned (fetched)

		OCIDefine* m_defineHandle;	// used for the column
		ResultSet* m_resultSet;	// is the column's owner


	}; 

}


#endif //__EP_COLUMN_H__