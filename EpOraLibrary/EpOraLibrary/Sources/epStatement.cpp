
#include "epStatement.h"
#include "epResultSet.h"
#include "epParameter.h"
#include "epConnection.h"
#include <assert.h>
#include "epOraError.h"
using namespace epol;


Statement::Statement (Connection &useConnection, const char *sqlStmt, int sqlStmtLen)	// = -1
{
	m_conn = &useConnection;
	initialize ();
	try
	{
		prepare (sqlStmt, sqlStmtLen);
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
		// ignore return code - returns either OCI_SUCCESS or OCI_INVALID_HANDLE
		OCIHandleFree (m_stmtHandle, OCI_HTYPE_STMT);
		m_stmtHandle = NULL;
	}

	// remove bound parameters
	for (Parameters::iterator i=m_parameters.begin (); i!=m_parameters.end (); ++i)
		delete (*i);
	m_parameters.clear ();
}


void Statement::prepare (const char *sqlStmt, int sqlLen)
{
	// prerequisites
	assert (m_conn != NULL && sqlStmt != NULL);
	assert (m_stmtHandle == NULL);	// otherwise old-one will never be released

	int	result;

	// allocate statement handle
	result = OCIHandleAlloc (m_conn->m_envHandle,(void **) &m_stmtHandle,OCI_HTYPE_STMT,0,	NULL);

	if (result == OCI_SUCCESS)
	{
		if (sqlLen == -1) 
			sqlLen = strlen (sqlStmt);

		result = OCIStmtPrepare (m_stmtHandle,m_conn->m_errorHandle,(text *) sqlStmt, sqlLen, OCI_NTV_SYNTAX, OCI_DEFAULT);
	}
	else
		throw (OraError(result, m_conn->m_envHandle, __FILE__, __LINE__));

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
		throw (OraError(result, m_conn->m_errorHandle, __FILE__, __LINE__));
}


void Statement::executePrepared ()
{
	// prerequisites
	assert (m_conn != NULL && m_isPrepared);

	int	result;
	unsigned int iters;

	// direct select statement?
	iters = (m_stmtType == ST_SELECT) ? 0 : 1;
	result = OCIStmtExecute (m_conn->m_svcContextHandle,m_stmtHandle, m_conn->m_errorHandle, iters,	0, NULL, NULL, OCI_DEFAULT);

	if (result == OCI_SUCCESS)
		m_isExecuted = true;
	else
		throw (OraError(result, m_conn->m_errorHandle, __FILE__, __LINE__));
}


Parameter& Statement::Bind (const char *name, DataTypesEnum dateType)
{
	// prerequisites
	assert (name);

	// could throw an exception
	Parameter *param = new Parameter (this, name, dateType, FETCH_SIZE);
	try
	{
		m_parameters.push_back (param);
		m_parametersMap [param->m_paramName] = param;
	}
	catch (...) // STL exception, perhaps
	{
		delete param;
		throw;
	}
	return (*param);
}


ResultSet* Statement::Select ()
{
	// prerequisites
	assert (m_isPrepared && m_stmtType == ST_SELECT);

	Execute ();
	ResultSet *resultSet = new ResultSet (m_stmtHandle,m_conn);
	try
	{
		resultSet->fetchRows();
		return (resultSet);
	}
	catch (...)
	{
		if (resultSet) 
			delete resultSet;
		throw;
	}
}


Parameter& Statement::operator [] (const char *paramName)
{
	ParametersMap::iterator i = m_parametersMap.find (std::string (paramName));
	if (i == m_parametersMap.end ())
		// name not found in parameters
		throw (OraError(EC_PARAMETER_NOT_FOUND, __FILE__, __LINE__, paramName));
	return (*(i->second));
}


Parameter& Statement::operator [] (unsigned short paramIndex)
{
	if (paramIndex < FIRST_PARAMETER_NO || paramIndex > m_parameters.size ())
		// no parameter with such index
		throw (OraError(EC_PARAMETER_NOT_FOUND, __FILE__, __LINE__, "%d", (int) paramIndex));
	return (*(m_parameters.at (paramIndex - FIRST_PARAMETER_NO)));
}



