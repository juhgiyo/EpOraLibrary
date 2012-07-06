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
*/
#ifndef __EP_COLUMN_H__
#define __EP_COLUMN_H__
#include "epOraLib.h"
#include <string>
#include "epDateTime.h"

using namespace epl;

namespace epol
{
	/*! 
	@class Column epColumn.h
	@brief This is a class representing OracleDB Table Column 

	Interface for the OracleDB Table Column class.
	*/
	class EP_ORACLELIB Column
	{
		friend class ResultSet;
	public:
		/*!
		Returns whether column value is NULL
		@return true if column value is NULL otherwise false.
		*/
		bool IsNull () const;

		/*!
		Returns the column data by converting it to String
		@return column data in string format.
		*/
		EpTString ToString() const;

		/*!
		Returns the column data by converting it to double
		@return column data in double format.
		*/
		double ToDouble() const;

		/*!
		Returns the column data by converting it to long
		@return column data in long format.
		*/
		long ToLong () const;

		/*!
		Returns the column data by converting it to DateTime
		@return column data in DateTime format.
		*/
		DateTime ToDateTime() const;

		/*!
		Release the self object
		@remark Column is not deleted by the client, but when ResultSet is released instead
		*/
		inline void Release ()
		{
			
		}

	private:

		/*!
		Default Constructor

		*Cannot be created publicly
		*Use Connection::Select or Statement::Select
		@param[in] rs the result set that holds this column
		@param[in] name the name of the column
		@param[in] ociDataType the data type of this column
		@param[in] maxDataSize the maximum data size of data type given
		@param[in] fetchSize the size of fetching
		*/
		Column (ResultSet *rs, const TCHAR *name, unsigned short ociDataType, unsigned int maxDataSize, int fetchSize = FETCH_SIZE);

		/*!
		Default Destructor
		
		Deleted when result set is released.
		*Cannot be deleted publicly
		*/
		~Column ();

		/*!
		Initialize member variables
		*/
		void initialize ();

		/*!
		Release all resources allocated
		*/
		void cleanUp ();

		/*!
		Default Copy Constructor

		*Class cannot be copied
		@param col the column object to copy
		*/
		Column (const Column&  col ) 
		{ 
			/* could not be copy-constructed */ 
		}

		/*!
		Copy Operator

		*Class cannot be copied
		@param col the column object to copy
		*/
		Column& operator = (const Column& col) 
		{
			return (*this); /* could not be assigned */ 
		}


		/// the name of the column
		EpTString m_colName;	
		/// the type of the column
		DataTypesEnum m_colType;
		/// the Oracle's data type
		unsigned short m_ociType;
		/// the number of bytes required for data
		int	m_size;

		/// array with indicators
		short* m_indicators;	
		/// array with data lengths (for text columns)
		unsigned short* m_dataLengths;	
		/// the buffer which holds the data (fetched)
		char* m_fetchBuffer;	

		/// handle for column
		OCIDefine* m_defineHandle;
		/// the owner result set of this column
		ResultSet* m_resultSet;	


	}; 

}


#endif //__EP_COLUMN_H__