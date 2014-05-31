/*! 
OracleDB Error for the EpOraLibrary

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
#include "epOraError.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epol;


OraError::OraError (int oraErr,OCIError *errorHandle, const TCHAR *sourceName, long lineNumber, const TCHAR *format,...)
{
	oracleError (oraErr, errorHandle, NULL);

	if (format)
	{
		va_list	va;
		va_start (va, format);
		concatMessage (format, va);
		va_end (va);
	}

	m_type = ET_ORACLE;
	m_winapiCode = 0;
	m_source = sourceName;
	m_lineNo = lineNumber;
}


OraError::OraError (int oraErr, OCIEnv *envHandle,const TCHAR *sourceName, long lineNumber, const TCHAR *format,...)
{
	oracleError (oraErr,NULL,envHandle);

	if (format)
	{
		va_list	va;
		va_start (va, format);
		concatMessage (format, va);
		va_end (va);
	}

	m_type = ET_ORACLE;
	m_winapiCode = 0;
	m_source = sourceName;
	m_lineNo = lineNumber;
}


OraError::OraError (int oralibErr, const TCHAR *sourceName, long lineNumber, const TCHAR *format, ...)
{
	oralibError (oralibErr);

	if (format)
	{
		va_list	va;
		va_start (va, format);
		concatMessage (format, va);
		va_end (va);
	}

	m_type = ET_ORALIB;
	m_oraCode = 0;
	m_winapiCode = 0;
	m_source = sourceName;
	m_lineNo = lineNumber;
}


OraError::OraError (const TCHAR *sourceName, long lineNumber,const TCHAR *format,	...)
{
	winapiError ();

	if (format)
	{
		va_list	va;
		va_start (va, format);
		concatMessage (format, va);
		va_end (va);
	}

	m_type = ET_WINAPI;
	m_code = 0;
	m_oraCode = 0;
	m_source = sourceName;
	m_lineNo = lineNumber;
}



OraError::OraError (const OraError& err):
m_type (err.m_type),
	m_code (err.m_code),
	m_oraCode (err.m_oraCode),
	m_winapiCode (err.m_winapiCode),
	m_description (err.m_description),
	m_source (err.m_source),
	m_lineNo (err.m_lineNo)
{
	m_type = err.m_type;

}


OraError::~OraError ()
{
}


void OraError::oracleError(int oraErr,OCIError *errorHandle, OCIEnv *envHandle)
{
	bool	getDetails = false;

	EP_ASSERT (errorHandle != NULL || envHandle != NULL);

	m_code = oraErr;
	switch (oraErr)
	{
	case	OCI_SUCCESS:
		m_description = _T("(OCI_SUCCESS)");
		break;

	case	OCI_SUCCESS_WITH_INFO:
		m_description = _T("(OCI_SUCCESS_WITH_INFO)");
		getDetails = true;
		break;

	case	OCI_ERROR:
		m_description = _T("(OCI_ERROR)");
		getDetails = true;
		break;

	case	OCI_NO_DATA:
		m_description = _T("(OCI_NO_DATA)");
		getDetails = true;
		break;

	case	OCI_INVALID_HANDLE:
		m_description = _T("(OCI_INVALID_HANDLE)");
		break;

	case	OCI_NEED_DATA:
		m_description = _T("(OCI_NEED_DATA)");
		break;

	case	OCI_STILL_EXECUTING:
		m_description = _T("(OCI_STILL_EXECUTING)");
		getDetails = true;
		break;

	case	OCI_CONTINUE:
		m_description = _T("(OCI_CONTINUE)");
		break;

	default:
		m_description = _T("unknown");
	}
	if (getDetails)
	{
		const int	maxTextLen = 4000;
		char	*errorText = EP_NEW char [maxTextLen];

		if (errorText)
		{
			*errorText = '\0';
			if (errorHandle)
				OCIErrorGet ( errorHandle, 1, NULL, reinterpret_cast<int*>(&m_oraCode), reinterpret_cast<unsigned char *> (errorText), maxTextLen, OCI_HTYPE_ERROR);
			else
				OCIErrorGet ( envHandle, 1, NULL, reinterpret_cast<int*>(&m_oraCode), reinterpret_cast<unsigned char *> (errorText),	maxTextLen, OCI_HTYPE_ENV);

			m_description += _T(" ");
#if defined(_UNICODE) || defined(UNICODE)
			 m_description +=(wchar_t *)errorText;
#else // defined(_UNICODE) || defined(UNICODE)
			 m_description += errorText;
#endif // defined(_UNICODE) || defined(UNICODE)
			EP_DELETE [] errorText;
		}
	}
}


void OraError::oralibError (int oralibErr)
{
	m_code = oralibErr;
	switch (oralibErr)
	{
	case	EC_ENV_CREATE_FAILED:
		m_description = _T("(EC_ENV_CREATE_FAILED) Environment handle creation failed");
		break;

	case	EC_TIMEOUT:
		m_description = _T("(EC_TIMEOUT) Statement took too long to complete and has been aborted");
		break;

	case	EC_NO_MEMORY:
		m_description = _T("(EC_NO_MEMORY) Memory allocation request has failed");
		break;

	case	EC_BAD_PARAM_TYPE:
		m_description = _T("(EC_BAD_PARAM_TYPE) Parameter type is incorrect");
		break;

	case	EC_POOL_NOT_SETUP:
		m_description = _T("(EC_POOL_NOT_SETUP) Connection pool has not been setup yet");
		break;

	case	EC_BAD_INPUT_TYPE:
		m_description = _T("(EC_BAD_INPUT_TYPE) Input data doesn't have expected type");
		break;

	case	EC_BAD_OUTPUT_TYPE:
		m_description = _T("(EC_BAD_OUTPUT_TYPE) Cannot convert to requested type");
		break;

	case	EC_BAD_TRANSFORM:
		m_description = _T("(EC_BAD_TRANSFORM) Requested transformation is not possible");
		break;

	case	EC_BAD_PARAM_PREFIX:
		m_description = _T("(EC_BAD_PARAM_PREFIX) Parameter prefix is not known");
		break;

	case	EC_INTERNAL:
		m_description = _T("(EC_INTERNAL) Internal library error. Please, report to developers");
		break;

	case	EC_UNSUP_ORA_TYPE:
		m_description = _T("(EC_UNSUP_ORA_TYPE) Unsupported Oracle type - cannot be converted to numeric, date or text");
		break;

	case	EC_PARAMETER_NOT_FOUND:
		m_description = _T("(EC_PARAMETER_NOT_FOUND) Name not found in statement's parameters");
		break;

	case	EC_COLUMN_NOT_FOUND:
		m_description = _T("(EC_COLUMN_NOT_FOUND) Result set doesn't contain column with such name");
		break;

	default:
		m_description = _T("unknown");
	}
}

void OraError::winapiError ()
{
	epl::System::FormatLastErrorMessage(m_description,&m_winapiCode);
}




epl::EpTString OraError::Details () const
{
	static const TCHAR *errorTypesText [] =
	{
		_T("unknown"), _T("Oracle"), _T("OraLib"), _T("Win32 API)")
	};

	epl::EpTString retString=_T("Source:");
	retString +=m_source;
	retString += _T("(");
	retString+=m_lineNo;
	retString+=_T(")\n");
	retString+= _T("Error Type: ");
	retString+=errorTypesText [m_type];
	retString+=_T("\nDescription: ");
	retString+= m_description;
	retString+= _T("\n");
	return retString;
}


void OraError::concatMessage (const TCHAR *format, va_list va)
{
	EP_ASSERT (format && va);

	TCHAR *buffer = EP_NEW TCHAR [ERROR_FORMAT_MAX_MSG_LEN];
	if (buffer)
	{
		epl::System::STPrintf_V(buffer,ERROR_FORMAT_MAX_MSG_LEN-1,format,va);
		m_description += _T(": ");
		m_description += buffer;
		EP_DELETE [] buffer;
	}
}

