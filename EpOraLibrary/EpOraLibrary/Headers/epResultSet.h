/*! 
@file epResultSet.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eporalibrary>
@date May 30, 2012
@brief OracleDB Result Set Interface
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
	class ResultSet
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

		/*!
		Release the self object
		*/
		inline void Release () 
		{
			EP_DELETE this; 
		}

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
		~ResultSet ();
		
		/*!
		Default Copy Constructor

		*Class cannot be copied
		@param rs the ResultSet object to copy
		*/
		ResultSet ( const ResultSet& rs) 
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
		typedef std::map <EpTString, Column*>	ColumnsMap;
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
