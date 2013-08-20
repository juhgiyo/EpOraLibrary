/*! 
@file epConnection.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eporalibrary>
@date May 30, 2012
@brief OracleDB Connection Interface
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

An Interface for OracleDB Connection.
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
		Connection (const TCHAR *serviceName, const TCHAR *loginID,const TCHAR *password, unsigned long envMode = OCI_THREADED|OCI_OBJECT, bool nonBlockingMode = false);

		~Connection ();

		/*!
		Connects to the OracleDB with given information
		@param[in] serviceName the name of the service
		@param[in] loginID the login ID for OracleDB
		@param[in] password the password for given login ID
		@param[in] envMode the Oracle Environment Mode
		@param[in] nonBlockingMode the flag for whether OracleDB Operation is blocking or not
		*/
		void Open (const TCHAR *serviceName,const TCHAR *loginID,const TCHAR *password,unsigned long envMode = OCI_THREADED|OCI_OBJECT, bool nonBlockingMode = false);

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
		@param[in] selectStmt the SQL statement to execute.
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
