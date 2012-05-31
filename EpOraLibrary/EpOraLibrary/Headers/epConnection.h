/*! 
@file epConnection.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eporalibrary>
@date May 30, 2012
@brief OracleDB Connection Interface
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

An Interface for OracleDB Connection.

@section NOTICE

EpOraLibrary is developed by referencing oraLib 0.0.3, which was developed by 606u <606u@dir.bg> 
<http://606u.dir.bg/>
606u had kindly approved using his/her oraLib freely for EpOraLibrary development.
*/
#ifndef	__EP_CONNECTION_H__
#define	__EP_CONNECTION_H__

#include "epOraLib.h"
#include "oci.h"
namespace epol 
{
	class Statement;
	class ResultSet;

	class EP_ORACLELIB Connection
	{
		// friends
		friend class Statement;
		friend class Parameter;
		friend class ResultSet;
		friend class Column;


	public:
		Connection ();

		// create an instance and open the connection
		Connection (const TCHAR *serviceName, const TCHAR *loginID,const TCHAR *password, unsigned long envMode = OCI_OBJECT, bool nonBlockingMode = false);

		~Connection ();

		// connects to an Oracle server
		void Open (const TCHAR *serviceName,const TCHAR *loginID,const TCHAR *password,unsigned long envMode = OCI_OBJECT, bool nonBlockingMode = false);

		// closes the connection
		void Close ();

		// executes a sql statement with no result
		void Execute (const TCHAR *sqlStmt);

		// prepares (and returns) a sql statement for execution
		Statement *Prepare (const TCHAR *sqlStmt);

		// executes a select sql statement and return the result set
		ResultSet *Select (const TCHAR *selectStmt);

		// commits changes
		inline void Commit ()
		{ 
			Execute (_T("commit")); 
		};

		// rollbacks changes
		inline void rollback ()
		{ 
			Execute (_T("rollback")); 
		};


	private:
		// private copy-constructor and assignment operator - class could not be copied
		inline Connection (const Connection& /* cn */) 
		{
			/* could not be copy-constructed */ 
		};
		inline Connection& operator = (const Connection& /* cn */) 
		{
			return (*this); /* could not be copy-constructed */ 
		};

		// initialize private data
		void initialize ();

		// free resources allocated
		inline void cleanUp () 
		{
			Close (); 
		};


		OCIEnv		*m_envHandle;
		OCIServer	*m_serverHandle;
		mutable OCIError	*m_errorHandle;	// because it could be changed by most oracle APIs
		OCISession	*m_sessionHandle;
		OCISvcCtx	*m_svcContextHandle;

		bool		m_isOpened;
		bool		m_isAvailable;			// (used for connection pooling)?
		bool		m_isBlocking;			// mode (a call could return OCI_STILL_EXECUTING)

	};


}


#endif	// __EP_CONNECTION_H__
