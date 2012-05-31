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


}; 


#endif	// __EP_RESULTSET_H__
