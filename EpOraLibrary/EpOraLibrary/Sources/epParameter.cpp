/*! 
OracleDB Parameter for the EpOraLibrary

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
#include "epParameter.h"
#include "epResultSet.h"
#include "epOraError.h"
#include "epStatement.h"
#include "epConnection.h"
#include "epOraDefines.h"
#include <tchar.h>

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epol;

Parameter::Parameter (Statement *to, const TCHAR *name, DataTypesEnum type, unsigned int fetchSize) :SmartObject()		// = FETCH_SIZE
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
	m_indicator = ORADATA_OK;
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
	if (m_bindHandle) 
		m_bindHandle = NULL;

	if (m_resultSet) 
		m_resultSet->ReleaseObj();
	m_resultSet = NULL;

	if (m_rsHandle)
	{
		OCIHandleFree ( m_rsHandle, OCI_HTYPE_STMT);
		m_rsHandle = NULL;
	}

	if (m_fetchBuffer) 
		EP_DELETE [] m_fetchBuffer;
	m_fetchBuffer = NULL;
}


void Parameter::attach (Statement *to, const TCHAR *name, DataTypesEnum type, unsigned int fetchSize)
{
	EP_ASSERT (name && to);

	m_paramName = name;

	setupType (name, type);

	m_indicator = ORADATA_NULL; 
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


void Parameter::setupType (const TCHAR *paramName, DataTypesEnum type)
{
	EP_ASSERT (paramName);

	const TCHAR	*pParamName = paramName;

	if (pParamName [0] == ':')
		pParamName++;

	if (pParamName [0] == PP_ARRAY)
	{
		m_isArray = true;
		pParamName++;
	}

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
		throw (OraError(EC_BAD_PARAM_PREFIX, __TFILE__, __LINE__, paramName));
}


void Parameter::bind (Statement *to)
{
	EP_ASSERT (to);

	int	result;

	m_fetchBuffer = EP_NEW char [m_size];
	switch (m_paramType)
	{
	case	DT_NUMBER:
		OCINumberSetZero (to->m_conn->m_errorHandle, (OCINumber *) m_fetchBuffer);
		break;

	case	DT_DATE:
		OCIDateSysDate ( to->m_conn->m_errorHandle, (OCIDate *) m_fetchBuffer);
		break;

	case	DT_TEXT:
		*((wchar_t *) m_fetchBuffer) = _T('\0');
		break;

	default:
		EP_ASSERT (FALSE);
		throw (OraError(EC_INTERNAL, __TFILE__, __LINE__, _T("Unsupported internal type")));
	}

	m_dataLen = static_cast<short> (m_size);
	result = OCIBindByName (to->m_stmtHandle, &m_bindHandle, to->m_conn->m_errorHandle, (unsigned char *) (m_paramName.data ()), static_cast<unsigned int>(m_paramName.length ()*sizeof(TCHAR)), m_fetchBuffer, m_size, m_ociType, &m_indicator,	&m_dataLen,	NULL,0,	NULL,OCI_DEFAULT);

#if defined(_UNICODE) || defined(UNICODE)
	if (result == OCI_SUCCESS)
	{
		unsigned int value = OCI_UTF16ID;
		result = OCIAttrSet (m_bindHandle, OCI_HTYPE_BIND, &value, sizeof (value), OCI_ATTR_CHARSET_ID, to->m_conn->m_errorHandle);
	}
#endif // defined(_UNICODE) || defined(UNICODE)

	if (result != OCI_SUCCESS)
		throw (OraError(result, to->m_conn->m_errorHandle, __TFILE__, __LINE__, m_paramName.c_str ()));
	m_stmt = to;
}


void Parameter::bindResultSet (Statement *to, unsigned int fetchSize)
{
	EP_ASSERT (to && fetchSize > 0);

	int	result;

	result = OCIHandleAlloc ( to->m_conn->m_envHandle,reinterpret_cast <void **> (&m_rsHandle), OCI_HTYPE_STMT, 0, NULL);

	if (result == OCI_SUCCESS)
	{
		result = OCIBindByName ( to->m_stmtHandle,&m_bindHandle, to->m_conn->m_errorHandle,(text *) m_paramName.data (),static_cast<unsigned int>(m_paramName.length ()*sizeof(TCHAR)), &m_rsHandle, m_size, m_ociType, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
#if defined(_UNICODE) || defined(UNICODE)
		if (result == OCI_SUCCESS)
		{
			unsigned int value = OCI_UTF16ID;
			result = OCIAttrSet (m_bindHandle, OCI_HTYPE_BIND, &value, sizeof (value), OCI_ATTR_CHARSET_ID, to->m_conn->m_errorHandle);
		}
#endif // defined(_UNICODE) || defined(UNICODE)

	}
	else
		throw (OraError(result, to->m_conn->m_envHandle, __TFILE__, __LINE__));

	if (result != OCI_SUCCESS)
		throw (OraError(result, to->m_conn->m_errorHandle, __TFILE__, __LINE__, m_paramName.c_str ()));
	m_stmt = to;
}


Parameter& Parameter::operator = (epl::EpTString text)
{
	EP_ASSERT (m_stmt);

	if (text.length()<=0)
		m_indicator = ORADATA_NULL;
	else if (m_paramType == DT_TEXT)
	{

#if defined(_UNICODE) || defined(UNICODE)
		m_dataLen = static_cast <unsigned short> (text.length() * 2);
#else// defined(_UNICODE) || defined(UNICODE)
		m_dataLen = static_cast <unsigned short> (text.length());
#endif// defined(_UNICODE) || defined(UNICODE)
		////////////////////////////////////////////////////////////////////////////////////////
		if (m_dataLen > m_size)
			m_dataLen = static_cast <unsigned short> ((m_size - 2) & ~1);
		memcpy (m_fetchBuffer, text.c_str(), m_dataLen);

#if defined(_UNICODE) || defined(UNICODE)
		*((wchar_t *) m_fetchBuffer + m_dataLen / 2) = _T('\0');
		m_dataLen += sizeof (wchar_t);
#else// defined(_UNICODE) || defined(UNICODE)
		*((char *) m_fetchBuffer + m_dataLen) = '\0';
		m_dataLen += sizeof (char);
#endif// defined(_UNICODE) || defined(UNICODE)
		m_indicator = ORADATA_OK; 
	}
	else
		throw (OraError(EC_BAD_INPUT_TYPE, __TFILE__, __LINE__));
	return (*this);
}


Parameter& Parameter::operator = (double value)
{
	EP_ASSERT (m_stmt);

	if (m_paramType == DT_NUMBER)
	{
		sword result = OCINumberFromReal (m_stmt->m_conn->m_errorHandle, &value, sizeof (double), reinterpret_cast <OCINumber *> (m_fetchBuffer));
		if (result != OCI_SUCCESS)
			throw (OraError(result, m_stmt->m_conn->m_errorHandle, __TFILE__, __LINE__));
		m_indicator = ORADATA_OK; 
	}
	else
		throw (OraError(EC_BAD_INPUT_TYPE, __TFILE__, __LINE__));
	return (*this);
}


Parameter& Parameter::operator = (long value)
{
	EP_ASSERT (m_stmt);

	if (m_paramType == DT_NUMBER)
	{
		int result = OCINumberFromInt ( m_stmt->m_conn->m_errorHandle, &value, sizeof (long), OCI_NUMBER_SIGNED, reinterpret_cast <OCINumber *> (m_fetchBuffer));
		if (result != OCI_SUCCESS)
			throw (OraError(result, m_stmt->m_conn->m_errorHandle, __TFILE__, __LINE__));
		m_indicator = ORADATA_OK;
	}
	else
		throw (OraError(EC_BAD_INPUT_TYPE, __TFILE__, __LINE__));
	return (*this);
}


Parameter& Parameter::operator = (const DateTime& dateTime)
{
	EP_ASSERT (m_stmt);

	if (m_paramType == DT_DATE)
	{
		dateTime.SetOciDate(*reinterpret_cast <OCIDate*> (m_fetchBuffer));
		m_indicator = ORADATA_OK;
	}
	else
		throw (OraError(EC_BAD_INPUT_TYPE, __TFILE__, __LINE__));
	return (*this);
}


epl::EpTString Parameter::ToString () const
{
	EP_ASSERT (m_stmt);

	if (m_paramType == DT_TEXT && m_indicator != ORADATA_NULL) 
		return epl::EpTString(reinterpret_cast<TCHAR*>(m_fetchBuffer));
	else
		throw (OraError(EC_BAD_OUTPUT_TYPE, __TFILE__, __LINE__));
}


double Parameter::ToDouble () const
{
	EP_ASSERT (m_stmt);

	if (m_paramType == DT_NUMBER && m_indicator != ORADATA_NULL) 
	{
		double	value;
		int result = OCINumberToReal (m_stmt->m_conn->m_errorHandle, reinterpret_cast <OCINumber *> (m_fetchBuffer), sizeof (double), &value);
		if (result == OCI_SUCCESS)
			return (value);
		else
			throw (OraError(result, m_stmt->m_conn->m_errorHandle, __TFILE__, __LINE__));
	}
	else
		throw (OraError(EC_BAD_OUTPUT_TYPE, __TFILE__, __LINE__));
}


long Parameter::ToLong () const
{
	EP_ASSERT (m_stmt);

	if (m_paramType == DT_NUMBER && m_indicator != ORADATA_NULL) 
	{
		long value;
		int result = OCINumberToInt ( m_stmt->m_conn->m_errorHandle, reinterpret_cast <OCINumber *> (m_fetchBuffer), sizeof (long), OCI_NUMBER_SIGNED, &value);
		if (result == OCI_SUCCESS)
			return (value);
		else
			throw (OraError(result, m_stmt->m_conn->m_errorHandle, __TFILE__, __LINE__));
	}
	else
		throw (OraError(EC_BAD_OUTPUT_TYPE, __TFILE__, __LINE__));
}


DateTime Parameter::ToDateTime () const
{
	EP_ASSERT (m_stmt);

	if (m_paramType == DT_DATE && m_indicator != ORADATA_NULL) 
		return (DateTime (*reinterpret_cast <OCIDate *> (m_fetchBuffer)));
	else
		throw (OraError(EC_BAD_OUTPUT_TYPE, __TFILE__, __LINE__));
}


ResultSet& Parameter::ToResultSet ()
{
	EP_ASSERT (m_stmt);

	if (m_paramType == DT_RESULT_SET)
	{
		if (!m_resultSet)
		{
			EP_ASSERT (m_stmt->m_isExecuted);

			m_resultSet = EP_NEW ResultSet (m_rsHandle, m_stmt->m_conn);
			m_resultSet->fetchRows ();
		}
		return (*m_resultSet);
	}
	else
		throw (OraError(EC_BAD_OUTPUT_TYPE, __TFILE__, __LINE__));
}

