/*! 
OracleDB Error for the EpOraLibrary
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
#include "epOraError.h"

using namespace epol;


OraError::OraError (int oraErr,OCIError *errorHandle, const TCHAR *sourceName, long lineNumber, const TCHAR *format,...)
{
	// sets-up ora_code and description
	oracleError (oraErr, errorHandle, NULL);

	// concat user-specified details
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
	// sets-up ora_code and description
	oracleError (oraErr,NULL,envHandle);

	// concat user-specified details
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
	// sets-up code and description
	oralibError (oralibErr);

	// concat user-specified details
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


#if	defined (_WIN32) // Windows platform only
OraError::OraError (const TCHAR *sourceName, long lineNumber,const TCHAR *format,	...)
{
	// sets-up winapi_code and description
	winapiError ();

	// concat user-specified details
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
#endif // defined (_WIN32)


// copy constructor
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

	// prerequisites
	assert (errorHandle != NULL || envHandle != NULL);

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

	// get detailed error description
	if (getDetails)
	{
		const int	maxTextLen = 4000;
		char	*errorText = new char [maxTextLen];

		if (errorText)
		{
			*errorText = '\0';
			if (errorHandle)
				OCIErrorGet ( errorHandle, 1, NULL, reinterpret_cast<int*>(&m_oraCode), reinterpret_cast<unsigned char *> (errorText), maxTextLen, OCI_HTYPE_ERROR);
			else
				OCIErrorGet ( envHandle, 1, NULL, reinterpret_cast<int*>(&m_oraCode), reinterpret_cast<unsigned char *> (errorText),	maxTextLen, OCI_HTYPE_ENV);

			m_description += _T(" ");
#if defined(_UNICODE) || defined(UNICODE)
			m_description += System::MultiByteToWideChar(errorText);
#else // defined(_UNICODE) || defined(UNICODE)
			m_description += errorText;
#endif // defined(_UNICODE) || defined(UNICODE)
			delete [] errorText;
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
	System::FormatLastErrorMessage(m_description,&m_winapiCode);
}




EpTString OraError::Details () const
{
	static const TCHAR *errorTypesText [] =
	{
		_T("unknown"), _T("Oracle"), _T("OraLib"), _T("Win32 API)")
	};

	EpTString retString=_T("Source:");
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
	// prerequisites
	EP_ASSERT (format && va);

	TCHAR *buffer = new TCHAR [ERROR_FORMAT_MAX_MSG_LEN];
	if (buffer)
	{
		System::STPrintf_V(buffer,ERROR_FORMAT_MAX_MSG_LEN-1,format,va);
		m_description += _T(": ");
		m_description += buffer;
		delete [] buffer;
	}
}

