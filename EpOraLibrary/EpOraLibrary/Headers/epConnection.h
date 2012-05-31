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

	/*! 
	@class Connection epConnection.h
	@brief This is a class representing OracleDB Connection

	Interface for the OracleDB Connection class.
	*/
	class EP_ORACLELIB Connection
	{
		// friends
		friend class Statement;
		friend class Parameter;
		friend class ResultSet;
		friend class Column;


	public:
		/*!
		Default Constructor

		Initializes the connection object
		*/
		Connection ();

		/*!
		Default Constructor

		Create an instance of connection and open the connection to OracleDB
		@param[in] serviceName the name of the service
		@param[in] loginID the login ID for OracleDB
		@param[in] password the password for given login ID
		@param[in] envMode the Oracle Environment Mode
		@param[in] nonBlockingMode the flag for whether OracleDB Operation is blocking or not
		*/
		Connection (const TCHAR *serviceName, const TCHAR *loginID,const TCHAR *password, unsigned long envMode = OCI_OBJECT, bool nonBlockingMode = false);

		~Connection ();

		/*!
		Connects to the OracleDB with given information
		@param[in] serviceName the name of the service
		@param[in] loginID the login ID for OracleDB
		@param[in] password the password for given login ID
		@param[in] envMode the Oracle Environment Mode
		@param[in] nonBlockingMode the flag for whether OracleDB Operation is blocking or not
		*/
		void Open (const TCHAR *serviceName,const TCHAR *loginID,const TCHAR *password,unsigned long envMode = OCI_OBJECT, bool nonBlockingMode = false);

		/*!
		Close the connection
		*/
		void Close ();

		/*!
		Execute the given SQL statement
		@param[in] sqlStmt the SQL statement to execute.
		*/
		void Execute (const TCHAR *sqlStmt);

		/*!
		Prepare the SQL statement given for execution, and return the statement instance.
		@param[in] sqlStmt the SQL statement to prepare.
		@return the statement instance with given SQL statement.
		*/
		Statement *Prepare (const TCHAR *sqlStmt);

		/*!
		Execute the SQL statement given, and return the result set.
		@param[in] sqlStmt the SQL statement to execute.
		@return the result set which resulted by executing given SQL statement.
		*/
		ResultSet *Select (const TCHAR *selectStmt);

		/*!
		Commit the changes made.
		*/
		inline void Commit ()
		{ 
			Execute (_T("commit")); 
		}

		/*!
		Roll back the changes made.
		*/
		inline void Rollback ()
		{ 
			Execute (_T("rollback")); 
		}

		/*!
		Return whether connected to OracleDB
		@return true if connected, otherwise false.
		*/
		inline bool IsConnected()
		{
			return m_isOpened;
		}

		/*!
		Return whether mode is blocking or not
		@return true if the mode is blocking, otherwise false.
		*/
		inline bool IsBlocking()
		{
			return m_isBlocking;
		}

	private:
		/*!
		Default Copy Constructor

		*Class cannot be copied
		@param conn the connection object to copy
		*/
		inline Connection (const Connection& conn ) 
		{
			/* could not be copy-constructed */ 
		}

		/*!
		Copy Operator

		*Class cannot be copied
		@param conn the connection object to copy
		*/
		inline Connection& operator = (const Connection& conn) 
		{
			return (*this); /* could not be copy-constructed */ 
		}

		/*!
		Initialize the member variables
		*/
		void initialize ();

		/*!
		Release all resources allocated
		*/
		inline void cleanUp () 
		{
			Close (); 
		};

		/// environment handle
		OCIEnv		*m_envHandle;
		/// server handle
		OCIServer	*m_serverHandle;
		/// error handle
		mutable OCIError	*m_errorHandle;
		/// session handle
		OCISession	*m_sessionHandle;
		/// svc context handle
		OCISvcCtx	*m_svcContextHandle;

		/// flag whether connected to OracleDB or not
		bool		m_isOpened;
		/// flag whether the mode is blocking or not
		bool		m_isBlocking;

	};


}


#endif	// __EP_CONNECTION_H__
