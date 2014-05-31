/*! 
OracleDB SQL Statement for the EpOraLibrary

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
*/
#include "epStatement.h"
#include "epResultSet.h"
#include "epParameter.h"
#include "epConnection.h"
#include "epOraError.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epol;


Statement::Statement (Connection &useConnection, const TCHAR *sqlStmt):SmartObject()
{
	m_conn = &useConnection;
	initialize ();
	try
	{
		prepare (sqlStmt);
	}
	catch (...)
	{
		cleanUp ();
		throw;
	}
}


Statement::~Statement ()
{
	cleanUp ();
}


void Statement::initialize ()
{
	m_stmtHandle = NULL;
	m_isPrepared = false;
	m_isExecuted = false;
	m_stmtType = ST_UNKNOWN;
}


void Statement::cleanUp ()
{
	if (m_stmtHandle)
	{
		OCIHandleFree (m_stmtHandle, OCI_HTYPE_STMT);
		m_stmtHandle = NULL;
	}
	for (Parameters::iterator i=m_parameters.begin (); i!=m_parameters.end (); ++i)
		(*i)->ReleaseObj();
	m_parameters.clear ();
	m_parametersMap.clear();	
}


void Statement::prepare (const TCHAR *sqlStmt)
{
	// prerequisites
	EP_ASSERT (m_conn != NULL && sqlStmt != NULL);
	EP_ASSERT (m_stmtHandle == NULL);	

	int	result;

	result = OCIHandleAlloc (m_conn->m_envHandle,(void **) &m_stmtHandle,OCI_HTYPE_STMT,0,	NULL);

	if (result == OCI_SUCCESS)
	{
		unsigned int sqlLen = epl::EpTString(sqlStmt).length();

		result = OCIStmtPrepare (m_stmtHandle,m_conn->m_errorHandle,(text *) sqlStmt, sqlLen*sizeof(TCHAR), OCI_NTV_SYNTAX, OCI_DEFAULT);
	}
	else
		throw (OraError(result, m_conn->m_envHandle, __TFILE__, __LINE__));

	if (result == OCI_SUCCESS)
	{
		unsigned short stmtType = 0;
		result = OCIAttrGet (m_stmtHandle, OCI_HTYPE_STMT, &stmtType, NULL, OCI_ATTR_STMT_TYPE, m_conn->m_errorHandle);
		// returns 0 (ST_UNKNOWN) if sql statement is wrong
		m_stmtType = (StatementTypesEnum) stmtType;
	}

	if (result == OCI_SUCCESS)
	{
		m_isPrepared = true;
		m_isExecuted = false;
	}
	else
		throw (OraError(result, m_conn->m_errorHandle, __TFILE__, __LINE__));
}


void Statement::executePrepared ()
{
	EP_ASSERT (m_conn != NULL && m_isPrepared);

	int	result;
	unsigned int iters;

	iters = (m_stmtType == ST_SELECT) ? 0 : 1;
	result = OCIStmtExecute (m_conn->m_svcContextHandle,m_stmtHandle, m_conn->m_errorHandle, iters,	0, NULL, NULL, OCI_DEFAULT);

	if (result == OCI_SUCCESS)
		m_isExecuted = true;
	else
		throw (OraError(result, m_conn->m_errorHandle, __TFILE__, __LINE__));
}


ResultSet* Statement::Select ()
{
	EP_ASSERT (m_isPrepared && m_stmtType == ST_SELECT);

	Execute ();
	ResultSet *resultSet = EP_NEW ResultSet (m_stmtHandle,m_conn);
	try
	{
		resultSet->fetchRows();
		return (resultSet);
	}
	catch (...)
	{
		if (resultSet) 
			resultSet->ReleaseObj();
		throw;
	}
}


Parameter& Statement::Bind (const TCHAR *name,DataTypesEnum type)
{
	// prerequisites
	EP_ASSERT (name);

	// could throw an exception
	Parameter *param = new Parameter (this,	name,type,FETCH_SIZE);
	try
	{
		m_parameters.push_back (param);
		m_parametersMap [param->m_paramName] = param;
	}
	catch (...) // STL exception, perhaps
	{
		param->ReleaseObj();
		throw;
	}
	return (*param);
}


Parameter& Statement::operator [] (const TCHAR *paramName)
{
	ParametersMap::iterator i = m_parametersMap.find (epl::EpTString(paramName));
	if (i == m_parametersMap.end ())
		throw (OraError (EC_PARAMETER_NOT_FOUND, __TFILE__, __LINE__, paramName));
	return (*(i->second));
}


Parameter& Statement::operator [] (unsigned short paramIndex)
{
	if (paramIndex < FIRST_PARAMETER_NO ||	paramIndex > m_parameters.size ())
		throw (OraError(EC_PARAMETER_NOT_FOUND, __TFILE__, __LINE__, _T("%d"), (int) paramIndex));
	return (*(m_parameters.at (paramIndex - FIRST_PARAMETER_NO)));
}