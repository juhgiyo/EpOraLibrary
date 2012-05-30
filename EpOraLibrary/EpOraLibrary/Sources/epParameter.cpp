#include "epParameter.h"
#include "epResultSet.h"
#include "epOraError.h"
#include "epStatement.h"
#include <assert.h>
#include "epConnection.h"
#include <tchar.h>

using namespace epol;

Parameter::Parameter (Statement *to, const char *name, DataTypesEnum type, unsigned int fetchSize)		// = FETCH_SIZE
{
	initialize ();
	try
	{
		attach (to, name, type, fetchSize);
	}
	catch (...)
	{
		cleanUp ();
		throw;
	}
}


Parameter::~Parameter ()
{
	cleanUp ();
}


void Parameter::initialize ()
{
	m_paramType = DT_UNKNOWN;
	m_ociType = 0;
	m_size = 0;
	m_indicator = 0;
	m_dataLen = 0;
	m_fetchBuffer = NULL;
	m_isArray = false;
	m_stmt = NULL;
	m_bindHandle = NULL;
	m_resultSet = NULL;
	m_rsHandle = NULL;
}


void Parameter::cleanUp ()
{
	// set all to null to be save to call cleanup more than once for a single instance

	// bind handle cannot be freed
	if (m_bindHandle) 
		m_bindHandle = NULL;

	// resultset class for bound cursor variable
	if (m_resultSet) 
		delete m_resultSet;
	m_resultSet = NULL;

	if (m_rsHandle)
	{
		// free statement handle if result set
		OCIHandleFree ( m_rsHandle, OCI_HTYPE_STMT);
		m_rsHandle = NULL;
	}

	if (m_fetchBuffer) 
		delete [] m_fetchBuffer;
	m_fetchBuffer = NULL;
}


void Parameter::attach (Statement *to, const char *name, DataTypesEnum type, unsigned int fetchSize)
{
	// prerequisites
	assert (name && to);

	m_paramName = name;

	// setup type, m_ociType, size, is_input, m_isArray
	setupType (name, type);

	m_indicator = -1; // null by default
	m_dataLen = 0;

	m_fetchBuffer = NULL;

	m_stmt = NULL;
	m_bindHandle = NULL;
	m_rsHandle = NULL;
	m_resultSet = NULL;

	if (m_paramType == DT_RESULT_SET)
		bindResultSet(to, fetchSize);
	else
		bind (to);
}


void Parameter::setupType (const char *paramName, DataTypesEnum type)
{
	// prerequisites
	assert (paramName);

	const char	*pParamName = paramName;

	if (pParamName [0] == ':')
		pParamName++;

	// an array?
	if (pParamName [0] == PP_ARRAY)
	{
		m_isArray = true;
		pParamName++;
	}

	// determine type
	if (type == DT_NUMBER || (type == DT_UNKNOWN && pParamName [0] == PP_NUMERIC))
	{
		m_paramType = DT_NUMBER;
		m_ociType = SQLT_VNU;
		m_size = sizeof (OCINumber);
	}
	else if (type == DT_DATE || (type == DT_UNKNOWN && pParamName [0] == PP_DATE))
	{
		m_paramType = DT_DATE;
		m_ociType = SQLT_ODT;
		m_size = sizeof (OCIDate);
	}
	else if (type == DT_TEXT || (type == DT_UNKNOWN && pParamName [0] == PP_TEXT))
	{
		m_paramType = DT_TEXT;
		m_ociType = SQLT_STR;
		m_size = MAX_OUTPUT_TEXT_BYTES;
	}
	else if (type == DT_RESULT_SET || (type == DT_UNKNOWN && pParamName [0] == PP_RESULT_SET))
	{
		m_paramType = DT_RESULT_SET;
		m_ociType = SQLT_RSET;
		m_size = sizeof (OCIBind *);
	}
	else
		// unrecognized data type
		throw (OraError(EC_BAD_PARAM_PREFIX, __FILE__, __LINE__, paramName));
}


void Parameter::bind (Statement *to)
{
	// prerequisites
	assert (to);

	int	result;

	// allocate and initialize memory for the result
	m_fetchBuffer = new char [m_size];
	switch (m_paramType)
	{
	case	DT_NUMBER:
		OCINumberSetZero (to->m_conn->m_errorHandle, (OCINumber *) m_fetchBuffer);
		break;

	case	DT_DATE:
		OCIDateSysDate ( to->m_conn->m_errorHandle, (OCIDate *) m_fetchBuffer);
		break;

	case	DT_TEXT:
		*((wchar_t *) m_fetchBuffer) = L'\0';
		break;

	default:
		assert (FALSE);
		throw (OraError(EC_INTERNAL, __FILE__, __LINE__, "Unsupported internal type"));
	}

	// bind
	m_dataLen = static_cast<sb2> (m_size);
	result = OCIBindByName (to->m_stmtHandle, &m_bindHandle, to->m_conn->m_errorHandle, (text *) (m_paramName.data ()), m_paramName.length (), m_fetchBuffer, m_size, m_ociType, &m_indicator,	&m_dataLen,	NULL,0,	NULL,OCI_DEFAULT);

#if defined (UNICODE)
	// request/pass text parameters as unicode (2.0)
	if (result == OCI_SUCCESS)
	{
		unsigned int value = OCI_UCS2ID;
		result = OCIAttrSet (m_bindHandle, OCI_HTYPE_BIND, &value, sizeof (value), OCI_ATTR_CHARSET_ID, to->m_conn->m_errorHandle);
	}
#endif // UNICODE defined?

	if (result != OCI_SUCCESS)
		throw (OraError(result, to->m_conn->m_errorHandle, __FILE__, __LINE__, m_paramName.c_str ()));
	m_stmt = to;
}


void Parameter::bindResultSet (Statement *to, unsigned int fetchSize)
{
	// prerequisites
	assert (to && fetchSize > 0);

	int	result;

	// allocate a statement handle for the result set
	result = OCIHandleAlloc ( to->m_conn->m_envHandle,reinterpret_cast <void **> (&m_rsHandle), OCI_HTYPE_STMT, 0, NULL);

	// bind statement handle as result set
	if (result == OCI_SUCCESS)
		result = OCIBindByName ( to->m_stmtHandle,&m_bindHandle, to->m_conn->m_errorHandle,(text *) m_paramName.data (),m_paramName.length (), &m_rsHandle, m_size, m_ociType, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
	else
		throw (OraError(result, to->m_conn->m_envHandle, __FILE__, __LINE__));

	if (result != OCI_SUCCESS)
		throw (OraError(result, to->m_conn->m_errorHandle, __FILE__, __LINE__, m_paramName.c_str ()));
	m_stmt = to;
}


Parameter& Parameter::operator = (Pstr text)
{
	// prerequisites
	assert (m_stmt);

	if (!text)
		m_indicator = -1; // set to null
	else if (m_paramType == DT_TEXT)
	{

		////////////////////////////////////////////////////////////////////////////////////////
		//주석해제(수정)
#if defined (UNICODE)
		m_dataLen = static_cast <unsigned short> (wcslen (text) * 2);
#else
		m_dataLen = static_cast <unsigned short> (strlen (text));
#endif
		////////////////////////////////////////////////////////////////////////////////////////
		m_dataLen = static_cast <unsigned short> (_tcslen (text));
		if (m_dataLen > m_size)
			m_dataLen = static_cast <unsigned short> ((m_size - 2) & ~1);
		memcpy (m_fetchBuffer, text, m_dataLen);

#if defined (UNICODE)
		// zero-terminate
		*((wchar_t *) m_fetchBuffer + m_dataLen / 2) = L'\0';
		// data len should include terminating zero
		m_dataLen += sizeof (wchar_t);
#else
		*((char *) m_fetchBuffer + m_dataLen) = '\0';
		m_dataLen += sizeof (char);
#endif
		m_indicator = 0; // not null
	}
	else
		throw (OraError(EC_BAD_INPUT_TYPE, __FILE__, __LINE__));
	return (*this);
}


Parameter& Parameter::operator = (double value)
{
	// prerequisites
	assert (m_stmt);

	if (m_paramType == DT_NUMBER)
	{
		sword result = OCINumberFromReal (m_stmt->conn->m_errorHandle, &value, sizeof (double), reinterpret_cast <OCINumber *> (m_fetchBuffer));
		if (result != OCI_SUCCESS)
			throw (OraError(result, m_stmt->conn->m_errorHandle, __FILE__, __LINE__));
		m_indicator = 0; // not null
	}
	else
		throw (OraError(EC_BAD_INPUT_TYPE, __FILE__, __LINE__));
	return (*this);
}


Parameter& Parameter::operator = (long value)
{
	// prerequisites
	assert (m_stmt);

	if (m_paramType == DT_NUMBER)
	{
		int result = OCINumberFromInt ( m_stmt->conn->m_errorHandle, &value, sizeof (long), OCI_NUMBER_SIGNED, reinterpret_cast <OCINumber *> (m_fetchBuffer));
		if (result != OCI_SUCCESS)
			throw (OraError(result, m_stmt->conn->m_errorHandle, __FILE__, __LINE__));
		m_indicator = 0; // not null
	}
	else
		throw (OraError(EC_BAD_INPUT_TYPE, __FILE__, __LINE__));
	return (*this);
}


Parameter& Parameter::operator = (const DateTime& dateTime)
{
	// prerequisites
	assert (m_stmt);

	if (m_paramType == DT_DATE)
	{
		d.SetOciDate(*reinterpret_cast <OCIDate*> (m_fetchBuffer));
		m_indicator = 0; // not null
	}
	else
		throw (OraError(EC_BAD_INPUT_TYPE, __FILE__, __LINE__));
	return (*this);
}


Pstr Parameter::ToString () const
{
	// prerequisites
	assert (m_stmt);

	if (m_paramType == DT_TEXT
		&& m_indicator != -1) // not null?
		return (reinterpret_cast <Pstr> (m_fetchBuffer));
	else
		throw (OraError(EC_BAD_OUTPUT_TYPE, __FILE__, __LINE__));
}


double Parameter::ToDouble () const
{
	// prerequisites
	assert (m_stmt);

	if (m_paramType == DT_NUMBER && m_indicator != -1) // not null?)
	{
		double	value;
		int result = OCINumberToReal (m_stmt->m_conn->m_errorHandle, reinterpret_cast <OCINumber *> (m_fetchBuffer), sizeof (double), &value);
		if (result == OCI_SUCCESS)
			return (value);
		else
			throw (OraError(result, m_stmt->m_conn->m_errorHandle, __FILE__, __LINE__));
	}
	else
		throw (OraError(EC_BAD_OUTPUT_TYPE, __FILE__, __LINE__));
}


long Parameter::ToLong () const
{
	// prerequisites
	assert (m_stmt);

	if (m_paramType == DT_NUMBER && m_indicator != -1) // not null?)
	{
		long value;
		int result = OCINumberToInt ( m_stmt->m_conn->m_errorHandle, reinterpret_cast <OCINumber *> (m_fetchBuffer), sizeof (long), OCI_NUMBER_SIGNED, &value);
		if (result == OCI_SUCCESS)
			return (value);
		else
			throw (OraError(result, m_stmt->m_conn->m_errorHandle, __FILE__, __LINE__));
	}
	else
		throw (OraError(EC_BAD_OUTPUT_TYPE, __FILE__, __LINE__));
}


DateTime Parameter::ToDateTime () const
{
	// prerequisites
	assert (m_stmt);

	if (m_paramType == DT_DATE && m_indicator != -1) // not null?)
		return (DateTime (*reinterpret_cast <OCIDate *> (m_fetchBuffer)));
	else
		throw (OraError(EC_BAD_OUTPUT_TYPE, __FILE__, __LINE__));
}


ResultSet& Parameter::ToResultSet ()
{
	// prerequisites
	assert (m_stmt);

	if (m_paramType == DT_RESULT_SET)
	{
		if (!m_resultSet)
		{
			// statement should be executed first!
			assert (m_stmt->m_isExecuted);

			// initialize
			// both could throw an exception
			m_resultSet = new ResultSet (m_rsHandle, m_stmt->m_conn);
			m_resultSet->fetchRows ();
		}
		return (*m_resultSet);
	}
	else
		throw (OraError(EC_BAD_OUTPUT_TYPE, __FILE__, __LINE__));
}

