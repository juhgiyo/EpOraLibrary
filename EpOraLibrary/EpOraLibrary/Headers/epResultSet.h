/*! 
@file epResultSet.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eporalibrary>
@date May 30, 2012
@brief OracleDB Result Set Interface
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

An Interface for OracleDB Result Set.
*/
#ifndef	__EP_RESULTSET_H__
#define	__EP_RESULTSET_H__

#include "epOraLib.h"
#include <vector>
#include <map>
#include "epStatement.h"
#include "oci.h"
namespace epol {

	/*! 
	@class ResultSet epResultSet.h
	@brief This is a class representing OracleDB Result Set 

	Interface for the OracleDB Result Set.
	*/
	class EP_ORACLELIB  ResultSet:public epl::SmartObject
	{
		// friends
		friend class Connection;
		friend class Statement;
		friend class Parameter;
		friend class Column;


	public:
		/*!
		Return the End-Of-Data indicator
		@return true if at end of data otherwise false.
		*/
		inline bool IsEod () const 
		{ 
			return (m_currentRow >= m_rowsFetched && m_isEod); 
		}

		/*!
		Traverse to the next row
		@return true if successfully traversed, false if at last row
		*/
		inline bool operator ++ () 
		{
			return (Next ()); 
		};

		/*!
		Traverse to the next row
		@return true if successfully traversed, false if at last row
		*/
		bool Next ();

		/*!
		Get the column by column name within current row
		@param[in] columnName the name of the column to get
		@return the column with given name within current row
		*/
		Column& operator [] (const TCHAR *columnName);

		/*!
		Get the column by column index within current row
		@param[in] columnIndex the index of the column to get
		@return the column at given index within current row
		*/
		Column& operator [] (unsigned short columnIndex);


	private:
		/*!
		Default Constructor

		*Cannot be created publicly
		@param[in] rs the OCI statement handle.
		@param[in] useConnection the connection object to use
		@param[in] fetchSize the size of fetching
		*/
		ResultSet (OCIStmt *rs, Connection *useConnection,unsigned int fetchSize = FETCH_SIZE);

		/*!
		Default Destructor
		
		Use ResultSet::Release instead
		*Cannot be deleted publicly
		*/
		virtual ~ResultSet ();
		
		/*!
		Default Copy Constructor

		*Class cannot be copied
		@param rs the ResultSet object to copy
		*/
		ResultSet ( const ResultSet& rs): SmartObject(rs) 
		{ 
			/* could not be copy-constructed */ 
		}

		/*!
		Copy Operator

		*Class cannot be copied
		@param rs the ResultSet object to copy
		*/
		ResultSet& operator = ( const ResultSet& rs ) 
		{ 
			return (*this); /* could not be copy-constructed */ 
		}

		/*!
		Initialize member variables
		*/
		void initialize ();

		/*!
		Release all resources allocated
		*/
		void cleanUp ();

		/*!
		Attach the result set object to given statement handle
		@param[in] rs the OCI statement handle.
		@param[in] useConnection the connection object to use
		@param[in] fetchSize the size of fetching
		*/
		void attach (OCIStmt *rs,Connection *useConnection,unsigned int fetchSize = FETCH_SIZE);

		/*!
		Returns number of columns
		@return the number of columns
		*/
		unsigned int columnsCount();

		/*!
		Returns number of rows fetched so far
		@return the number of rows fetched so far
		*/
		unsigned int rowsCount();

		/*!
		Attach the given statement object to this ResultSet
		*Released when the Result Set is released 
		@param[in] selectStmt the statement to be attached
		*/
		void attachStatement(Statement *selectStmt);

		/*!
		Describe columns of the result set
		*/
		void describe ();

		/*!
		Define columns of the result set 
		*Bound buffers to columns
		*/
		void define ();

		/*!
		Fetch new block of rows in the buffers
		*/
		void fetchRows ();


		/// Type Definition for Columns
		typedef std::vector <Column*>	Columns;
		/// Type Definition for ColumnsMap
		typedef std::map <epl::EpTString, Column*>	ColumnsMap;
		/// array with Result Set columns
		Columns			m_columns;		
		/// map with columns against their names
		ColumnsMap		m_columnsMap;

		/// active connection
		Connection		*m_conn;
		/// OCI statement handle for the Result Set
		OCIStmt			*m_rsHandle;

		/// Statement Object when created via Connection::Select
		Statement		*m_stmt;
		/// the number of rows to fetch at once
		unsigned short	m_fetchCount;

		/// the number of rows fetched so far
		unsigned int	m_rowsFetched;
		/// current row index (0 based)
		unsigned int	m_currentRow;
		/// flag for end-of-data
		bool			m_isEod;

		/// flag for whether Result Set is described or not
		bool			m_isDescribed;
		/// flag for whether Result Set is defined or not
		bool			m_isDefined;
		/// the total number of rows
		unsigned int m_nTotRow;

	}; 


}


#endif	// __EP_RESULTSET_H__
