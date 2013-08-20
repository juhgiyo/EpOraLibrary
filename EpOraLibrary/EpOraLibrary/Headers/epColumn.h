/*! 
@file epColumn.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eporalibrary>
@date May 30, 2012
@brief OracleDB Table Column Interface
@version 1.0

@section LICENSE

The MIT License (MIT)

Copyright (c) 2012-2013 Woong Gyu La <juhgiyo@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

@section DESCRIPTION

An Interface for OracleDB Table Column.
*/
#ifndef __EP_COLUMN_H__
#define __EP_COLUMN_H__
#include "epOraLib.h"
#include <string>
#include "epDateTime.h"



namespace epol
{
	/*! 
	@class Column epColumn.h
	@brief This is a class representing OracleDB Table Column 

	Interface for the OracleDB Table Column class.
	*/
	class EP_ORACLELIB Column: public epl::SmartObject
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
		epl::EpTString ToString() const;

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
		Column (ResultSet *rs, const TCHAR *name, unsigned short ociDataType, unsigned int maxDataSize, unsigned int fetchSize = FETCH_SIZE);

		/*!
		Default Destructor
		
		Deleted when result set is released.
		*Cannot be deleted publicly
		*/
		virtual ~Column ();

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
		Column (const Column&  col ) :SmartObject(col)
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
		epl::EpTString m_colName;	
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