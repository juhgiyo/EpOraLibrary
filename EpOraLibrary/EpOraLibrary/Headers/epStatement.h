/*! 
@file epStatement.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eporalibrary>
@date May 30, 2012
@brief OracleDB SQL Statement Interface
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

An Interface for OracleDB SQL Statement.
*/
#ifndef	__EP_STATEMENT_H__
#define	__EP_STATEMENT_H__

#include "epOraLib.h"
#include "epOraDefines.h"
#include <vector>
#include <map>
#include "oci.h"


namespace epol {
	
	class ResultSet;
	class Connection;

	/*! 
	@class Statement epStatement.h
	@brief This is a class representing OracleDB SQL Statement 

	Interface for the OracleDB SQL Statement.
	*/
	class EP_ORACLELIB Statement:public epl::SmartObject
	{
		// friends
		friend class Parameter;
		friend class Connection;

	public:

		/*!
		Binds a named variable to the statement
		*When type is set to DT_UNKNOWN type is taken from name's prefix
		@param[in] name the name of the variable
		@param[in] type the Data type for the parameter
		@return the parameter binded
		*/
		Parameter &Bind (const TCHAR *name,DataTypesEnum type = DT_UNKNOWN);

		/*!
		Executes the prepared statement with no output parameters
		*/
		inline void Execute () 
		{
			executePrepared (); 
		}

		/*!
		Executes the prepared Select SQL statement and returns the result set
		@return the result set returned by executing the SQL statement
		*/
		ResultSet *Select();


		/*!
		Return the flag whether statement is prepared or not
		@return true if prepared otherwise false
		*/
		bool IsPrepared()
		{
			return m_isPrepared;
		}

		/*!
		Return the flag whether statement is executed or not
		@return true if executed otherwise false
		*/
		bool IsExecuted()
		{
			return m_isExecuted;
		}

		/*!
		Return the type of the statement
		@return The statement type
		*/
		StatementTypesEnum GetStmtType()
		{
			return m_stmtType;
		}

		/*!
		Return the parameter binded with given name
		@param paramName the name of the paramter binded to get
		@return the parameter binded with given name
		*/
		Parameter& operator [] (const TCHAR *paramName);

		/*!
		Return the parameter binded at given index
		@param paramIndex the index of the paramter binded to get
		@return the parameter binded at given index
		*/
		Parameter& operator [] (unsigned short paramIndex);
	private:
		/*!
		Default Constructor

		*Cannot be created publicly
		*Use Connection::Execute/Prepare/Select
		Prepare the statement for execution
		@param[in] useConnection the connection object to use
		@param[in] sqlStmt the SQL statement
		*/
		Statement (Connection &useConnection, const TCHAR *sqlStmt);

		/*!
		Default Destructor
		
		Use Statement::Release instead
		*Cannot be deleted publicly
		*/
		virtual ~Statement ();

		/*!
		Default Copy Constructor

		*Class cannot be copied
		@param stmt the Statement object to copy
		*/
		Statement (const Statement& stmt) :SmartObject(stmt)
		{
			/* could not be copy-constructed */ 
		};

		/*!
		Copy Operator

		*Class cannot be copied
		@param stmt the Statement object to copy
		*/
		Statement& operator = (const Statement& stmt) 
		{ 
			return (*this); /* could not be copy-constructed */ 
		};

		/*!
		Initialize member variables
		*/
		void initialize ();

		/*!
		Release all resources allocated
		*/
		void cleanUp ();

		/*!
		Prepare the SQL statement given for execution
		@param[in] sqlStmt the SQL statement to prepare
		*/
		void prepare (const TCHAR *sqlStmt);

		/*!
		Execute the prepared statement
		*/
		void executePrepared ();



		/// Type definition for Parameters
		typedef std::vector <Parameter *> Parameters;
		/// Type definition for ParametersMap
		typedef std::map <epl::EpTString, Parameter *> ParametersMap;
		/// array with bound parameters
		Parameters		m_parameters;	
		/// a map with paramters against their names
		ParametersMap	m_parametersMap;	

		/// the connection object used
		Connection		*m_conn;	
		/// the statement handle
		OCIStmt			*m_stmtHandle;
		/// the type of the statement
		StatementTypesEnum	m_stmtType;

		/// the flag whether the statement is prepared or not
		bool			m_isPrepared;
		/// the flag whether the statement is executed or not
		bool			m_isExecuted;


	};


}


#endif	// __EP_STATEMENT_H__
