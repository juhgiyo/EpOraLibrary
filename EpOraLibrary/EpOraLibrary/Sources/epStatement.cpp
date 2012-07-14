/*! 
OracleDB SQL Statement for the EpOraLibrary
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
*/
#include "epStatement.h"
#include "epResultSet.h"
#include "epParameter.h"
#include "epConnection.h"
#include "epOraError.h"
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
		unsigned int sqlLen = epl::System::TcsLen(sqlStmt);

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