/*! 
@file epStatement.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eporalibrary>
@date May 30, 2012
@brief OracleDB SQL Statement Interface
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

An Interface for OracleDB SQL Statement.

@section NOTICE

EpOraLibrary is developed by referencing oraLib 0.0.3, which was developed by 606u <606u@dir.bg> 
<http://606u.dir.bg/>
606u had kindly approved using his/her oraLib freely for EpOraLibrary development.

*/
#ifndef	__EP_STATEMENT_H__
#define	__EP_STATEMENT_H__

#include "epOraLib.h"
#include "epOraDefines.h"
#include <vector>
#include <map>
#include "oci.h"

using namespace epl;
namespace epol {
	
	class ResultSet;
	class Connection;

	class Statement
	{
		// friends
		friend class Parameter;
		friend class Connection;

	public:

		// executes a prepared statement with no output parameters
		inline void Execute () 
		{
			executePrepared (); 
		}

		// executes a prepared select sql statement and returns the result set
		ResultSet *Select();

		// releases statement
		inline void Release ()
		{
			delete this; 
		}

	private:
		// public not creatable; use connection.execute, .prepare or .select
		// prepares an sql statement for execution
		Statement (Connection &useConnection, const TCHAR *sqlStmt);

		// public not deletable; use release method instead
		~Statement ();

		// private copy-constructor and assignment operator - class could not be copied
		Statement (const Statement& /* st */) 
		{
			/* could not be copy-constructed */ 
		};
		Statement& operator = (const Statement& /* st */) 
		{ 
			return (*this); /* could not be copy-constructed */ 
		};

		// initialize private data
		void initialize ();

		// free resources allocated
		void cleanUp ();

		// prepares an sql statement for execution
		void prepare (const TCHAR *sqlStmt);

		// executes already prepared statement
		void executePrepared ();

		Connection		*m_conn;			// being used
		OCIStmt			*m_stmtHandle;
		std::string		m_sqlStmt;			// being executed
		StatementTypesEnum	m_stmtType;		// of the statement

		bool			m_isPrepared;
		bool			m_isExecuted;


	};


}


#endif	// __EP_STATEMENT_H__
