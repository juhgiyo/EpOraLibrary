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

@section NOTICE

EpOraLibrary is developed by referencing oraLib 0.0.3, which was developed by 606u <606u@dir.bg> 
<http://606u.dir.bg/>
606u had kindly approved using his/her oraLib freely for EpOraLibrary development.
*/
#ifndef	__EP_RESULTSET_H__
#define	__EP_RESULTSET_H__

#include "epOraLib.h"
#include <vector>
#include <map>
#include "epStatement.h"
#include "oci.h"
namespace epol {


	class ResultSet
	{
		// friends
		friend class Connection;
		friend class Statement;
		friend class Parameter;
		friend class Column;


	public:
		// return end-of-data indicator
		inline bool IsEod () const 
		{ 
			return (m_currentRow >= m_rowsFetched && m_isEod); 
		}

		// skip to next row; returns false if current one is the last
		inline bool operator ++ () 
		{
			return (Next ()); 
		};
		bool Next ();

		// access columns in the current row by name or index
		Column& operator [] (const TCHAR *columnName);
		Column& operator [] (unsigned short columnIndex);

		inline void Release () 
		{
			delete this; 
		}

	private:
		// public - not creatable
		ResultSet (OCIStmt *rs, Connection *useConnection,unsigned int fetchSize = FETCH_SIZE);

		// public - not deletable - use release instead
		~ResultSet ();

		// private copy-constructor and assignment operator - class could not be copied
		ResultSet ( const ResultSet& /* rs */) 
		{ 
			/* could not be copy-constructed */ 
		}
		ResultSet& operator = ( const ResultSet& /* rs */) 
		{ 
			return (*this); /* could not be copy-constructed */ 
		}

		// initialize private data
		void initialize ();

		// free resources allocated
		void cleanUp ();

		// attaches a result set object to a statement handle
		void attach (OCIStmt *rs,Connection *useConnection,unsigned int fetchSize = FETCH_SIZE);

		// returns number of columns
		unsigned int columnsCount();

		// returns number of rows fetched so far
		unsigned int rowsCount();

		// attaches a statement - freed, when the result set is released (connection.select)
		void attachStatement(Statement *selectStmt);

		// describe columns of the result set
		void describe ();

		// define columns of the result set - bound buffers to columns
		void define ();

		// fetch new block of rows in the buffers
		void fetchRows ();


		typedef std::vector <Column*>	Columns;
		typedef std::map <EpTString, Column*>	ColumnsMap;
		Columns			m_columns;		// an array with result set columns
		ColumnsMap		m_columnsMap;	// a map with columns against their names

		Connection		*m_conn;			// active connection
		OCIStmt			*m_rsHandle;		// oci statement handle for the result set

		Statement		*m_stmt;			// if created via connection.select
		unsigned short	m_fetchCount;	// number of rows to fetch at once

		unsigned int	m_rowsFetched;	// number of rows fetched so far
		unsigned int	m_currentRow;	// current row (0-based)
		bool			m_isEod;			// end-of-data reached?

		bool			m_isDescribed;	// is result set (columns) described flag
		bool			m_isDefined;		// is result set defined flag

		unsigned int m_nTotRow;

	}; 


}


#endif	// __EP_RESULTSET_H__
