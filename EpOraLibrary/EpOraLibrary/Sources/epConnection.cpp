#include "epConnection.h"
#include <assert.h>
#include "epOraError.h"
#include "epStatement.h"
#include "epResultSet.h"

using namespace epol;


Connection::Connection ()
{
	initialize ();
}


Connection::Connection (const TCHAR *serviceName, const TCHAR *loginID,const TCHAR *password, unsigned long envMode, bool nonBlockingMode)
{
	initialize ();
	try
	{
		Open (serviceName,	loginID, password, envMode, nonBlockingMode);
	}
	catch (...)
	{
		Close ();
		throw;
	}
}


Connection::~Connection ()
{
	cleanUp (); // calls close
}


void Connection::initialize ()
{
	m_envHandle = NULL;
	m_serverHandle = NULL;
	m_errorHandle = NULL;
	m_sessionHandle = NULL;
	m_svcContextHandle = NULL;

	m_isOpened = false;
	m_isAvailable = false;
	m_isBlocking = false;
}


void Connection::Open (const TCHAR *serviceName,const TCHAR *loginID,const TCHAR *password,unsigned long envMode, bool nonBlockingMode)
{
	// prerequisites
	assert (serviceName && loginID && password);
	assert (!m_isOpened);

	int	result;

	// allocate an environment handle
#if defined(_UNICODE) || defined(UNICODE)
	result = OCIEnvNlsCreate(&m_envHandle,envMode,NULL,NULL,NULL,0,NULL,NULL,OCI_UTF16ID,OCI_UTF16ID);
#else //defined(_UNICODE) || defined(UNICODE)
	result = OCIEnvCreate ( &m_envHandle, envMode, NULL, NULL, NULL, NULL, 0, NULL);
#endif //defined(_UNICODE) || defined(UNICODE)

	// allocate a server handle
	if (result == OCI_SUCCESS)
		result = OCIHandleAlloc (m_envHandle, (void **) &m_serverHandle, OCI_HTYPE_SERVER, 0,NULL);	
	else
		throw (OraError(EC_ENV_CREATE_FAILED, __TFILE__, __LINE__));

	// allocate an error handle
	if (result == OCI_SUCCESS)
		result = OCIHandleAlloc (m_envHandle, (void **) &m_errorHandle, OCI_HTYPE_ERROR, 0, NULL);

	// create a server context
	if (result == OCI_SUCCESS)
		result = OCIServerAttach (m_serverHandle, m_errorHandle, (text *) serviceName, System::TcsLen(serviceName)*sizeof(TCHAR), OCI_DEFAULT);
	else
		throw (OraError(result, m_envHandle, __TFILE__, __LINE__));

	// allocate a service handle
	if (result == OCI_SUCCESS)
		result = OCIHandleAlloc ( m_envHandle, (void **) &m_svcContextHandle, OCI_HTYPE_SVCCTX, 0, NULL);
	else
		throw (OraError(result, m_errorHandle, __TFILE__, __LINE__));

	// set the server attribute in the service context handle
	if (result == OCI_SUCCESS)
		result = OCIAttrSet ( m_svcContextHandle, OCI_HTYPE_SVCCTX, m_serverHandle, sizeof (OCIServer *), OCI_ATTR_SERVER, m_errorHandle);
	else
		throw (OraError(result, m_envHandle, __TFILE__, __LINE__));

	// allocate a user session handle
	if (result == OCI_SUCCESS)
		result = OCIHandleAlloc ( m_envHandle, (void **) &m_sessionHandle, OCI_HTYPE_SESSION, 0, NULL);
	else
		throw (OraError(result, m_errorHandle, __TFILE__, __LINE__));

	// set username and password attributes in user session handle
	if (result == OCI_SUCCESS)
		result = OCIAttrSet ( m_sessionHandle, OCI_HTYPE_SESSION, (void *) loginID, System::TcsLen (loginID)*sizeof(TCHAR), OCI_ATTR_USERNAME, m_errorHandle);
	else
		throw (OraError(result, m_envHandle, __TFILE__, __LINE__));

	if (result == OCI_SUCCESS)
		result = OCIAttrSet ( m_sessionHandle, OCI_HTYPE_SESSION, (void *) password, System::TcsLen (password)*sizeof(TCHAR), OCI_ATTR_PASSWORD, m_errorHandle);

	// start the session
	if (result == OCI_SUCCESS)
		result = OCISessionBegin ( m_svcContextHandle, m_errorHandle, m_sessionHandle, OCI_CRED_RDBMS, OCI_DEFAULT);

	// set the user session attribute in the service context handle
	if (result == OCI_SUCCESS)
		result = OCIAttrSet (m_svcContextHandle, OCI_HTYPE_SVCCTX, m_sessionHandle, sizeof (OCISession *), OCI_ATTR_SESSION, m_errorHandle);

	// switch to non-blocking mode?
	if (result == OCI_SUCCESS && nonBlockingMode)
	{
		unsigned char attr_value;

		attr_value = 1;
		result = OCIAttrSet ( m_serverHandle, OCI_HTYPE_SERVER, &attr_value, sizeof (attr_value), OCI_ATTR_NONBLOCKING_MODE, m_errorHandle);
	}

	if (result == OCI_SUCCESS)
	{
		m_isOpened = true;
		m_isAvailable = true;
		m_isBlocking = !nonBlockingMode;
	}
	else
		throw (OraError(result, m_errorHandle, __TFILE__, __LINE__));
}


void Connection::Close ()
{
	int	result;

	// no prerequisites

	// just in case switch server to blocking mode
	if (m_serverHandle != NULL)
	{
		unsigned char attrValue;

		attrValue = 0;
		result = OCIAttrSet ( m_serverHandle, OCI_HTYPE_SERVER, &attrValue, sizeof (attrValue), OCI_ATTR_NONBLOCKING_MODE, m_errorHandle);
	}
	else
		result = OCI_SUCCESS;

	// end session
	if (m_svcContextHandle != NULL && m_errorHandle != NULL)
	{
		if (m_sessionHandle != NULL)
			result = OCISessionEnd (m_svcContextHandle, m_errorHandle, m_sessionHandle, OCI_DEFAULT);
		else
			result = OCI_SUCCESS;

		// detach from the oracle server
		if (result == OCI_SUCCESS)
			result = OCIServerDetach (m_serverHandle, m_errorHandle, OCI_DEFAULT);
	}
	else
		result = OCI_SUCCESS;

	// free handles
	if (result == OCI_SUCCESS && m_svcContextHandle != NULL)
		result = OCIHandleFree ( m_svcContextHandle, OCI_HTYPE_SVCCTX);

	if (result == OCI_SUCCESS && m_sessionHandle != NULL)
	{
		m_svcContextHandle = NULL;
		result = OCIHandleFree ( m_sessionHandle, OCI_HTYPE_SESSION);
	}

	if (result == OCI_SUCCESS && m_errorHandle != NULL)
	{
		m_sessionHandle = NULL;
		result = OCIHandleFree ( m_errorHandle, OCI_HTYPE_ERROR);
	}

	if (result == OCI_SUCCESS && m_serverHandle != NULL)
	{
		m_errorHandle = NULL;
		result = OCIHandleFree ( m_serverHandle, OCI_HTYPE_SERVER);
	}


	if (result == OCI_SUCCESS && m_envHandle != NULL)
	{
		m_serverHandle = NULL;
		result = OCIHandleFree (m_envHandle, OCI_HTYPE_ENV);
		if (result == OCI_SUCCESS)
			m_envHandle = NULL;
	}

	if (result == OCI_SUCCESS)
	{
		m_isOpened = false;
		m_isAvailable = false;
		m_isBlocking = false;
	}
}


void Connection::Execute (const TCHAR *sqlStmt)
{
	// prerequisites
	assert (sqlStmt);

	Statement st (*this, sqlStmt);
	st.executePrepared ();
}


Statement* Connection::Prepare (const TCHAR *sqlStmt)	
{
	// prerequisites
	assert (sqlStmt);

	return (new Statement (*this, sqlStmt));
}


ResultSet* Connection::Select (const TCHAR *selectStmt)
{
	// prerequisites
	assert (selectStmt);

	Statement	*statement = Prepare (selectStmt);
	try
	{
		ResultSet	*result = statement->Select ();
		result->attachStatement (statement);
		return (result);
	}
	catch (...)
	{
		statement->Release ();
		throw;
	}
}
