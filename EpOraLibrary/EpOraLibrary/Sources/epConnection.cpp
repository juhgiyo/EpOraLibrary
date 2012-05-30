#include "epConnection.h"
#include <assert.h>
#include "epError.h"
#include "epStatement.h"
#include "epResultSet.h"

namespace epol {


	Connection::Connection ()
	{
		initialize ();
	}


	Connection::Connection (const char *serviceName, const char *loginID,const char *password, unsigned long envMode, bool nonBlockingMode)
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
		cleanup (); // calls close
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


	void Connection::Open (const char *serviceName,const char *loginID,const char *password,unsigned long envMode, bool nonBlockingMode)
	{
		// prerequisites
		assert (serviceName && loginID && password);
		assert (!m_isOpened);

		int	result;

		// allocate an environment handle
		result = OCIEnvCreate ( &m_envHandle, envMode, NULL, NULL, NULL, NULL, 0, NULL);

		// allocate a server handle
		if (result == OCI_SUCCESS)
			result = OCIHandleAlloc (m_envHandle, (void **) &m_serverHandle, OCI_HTYPE_SERVER, 0,NULL);	
		else
			throw (oralib::error (EC_ENV_CREATE_FAILED, __FILE__, __LINE__));

		// allocate an error handle
		if (result == OCI_SUCCESS)
			result = OCIHandleAlloc (m_envHandle, (void **) &m_errorHandle, OCI_HTYPE_ERROR, 0, NULL);

		// create a server context
		if (result == OCI_SUCCESS)
			result = OCIServerAttach (m_serverHandle, m_errorHandle, (text *) serviceName, strlen (serviceName), OCI_DEFAULT);
		else
			throw (oralib::error (result, m_envHandle, __FILE__, __LINE__));

		// allocate a service handle
		if (result == OCI_SUCCESS)
			result = OCIHandleAlloc ( m_envHandle, (void **) &m_svcContextHandle, OCI_HTYPE_SVCCTX, 0, NULL);
		else
			throw (oralib::error (result, m_errorHandle, __FILE__, __LINE__));

		// set the server attribute in the service context handle
		if (result == OCI_SUCCESS)
			result = OCIAttrSet ( m_svcContextHandle, OCI_HTYPE_SVCCTX, m_serverHandle, sizeof (OCIServer *), OCI_ATTR_SERVER, m_errorHandle);
		else
			throw (oralib::error (result, m_envHandle, __FILE__, __LINE__));

		// allocate a user session handle
		if (result == OCI_SUCCESS)
			result = OCIHandleAlloc ( m_envHandle, (void **) &m_sessionHandle, OCI_HTYPE_SESSION, 0, NULL);
		else
			throw (oralib::error (result, m_errorHandle, __FILE__, __LINE__));

		// set username and password attributes in user session handle
		if (result == OCI_SUCCESS)
			result = OCIAttrSet ( m_sessionHandle, OCI_HTYPE_SESSION, (text *) loginID, strlen (loginID), OCI_ATTR_USERNAME, m_errorHandle);
		else
			throw (oralib::error (result, m_envHandle, __FILE__, __LINE__));

		if (result == OCI_SUCCESS)
			result = OCIAttrSet ( m_sessionHandle, OCI_HTYPE_SESSION, (text *) password, strlen (password), OCI_ATTR_PASSWORD, m_errorHandle);

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
			throw (oralib::error (result, m_errorHandle, __FILE__, __LINE__));
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


	void Connection::Execute (const char *sqlStmt, int sqlStmtLen)
	{
		// prerequisites
		assert (sqlStmt);

		Statement st (*this, sqlStmt, sqlStmtLen);
		st.ExecutePrepared ();
	}


	Statement* Connection::Prepare (const char *sqlStmt,int sqlStmtLen)	
	{
		// prerequisites
		assert (sqlStmt);

		return (new Statement (*this, sqlStmt, sqlStmtLen));
	}


	ResultSet* Connection::Select (const char *selectStmt, int selectStmtLen)
	{
		// prerequisites
		assert (select);

		Statement	*statement = Prepare (selectStmt, selectStmtLen);
		try
		{
			ResultSet	*result = statement->Select ();
			result->AttachStatement (statement);
			return (result);
		}
		catch (...)
		{
			statement->Release ();
			throw;
		}
	}


};	
