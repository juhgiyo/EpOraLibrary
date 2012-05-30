#if	defined (_WIN32) // Windows platform
#	include <windows.h>
#endif
#include "epOraError.h"
#include <assert.h>

using namespace epol;


OraError::OraError (int oraErr,OCIError *errorHandle, const char *sourceName, long lineNumber, const char *format,...)
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


OraError::OraError (int oraErr, OCIEnv *envHandle,const char *sourceName, long lineNumber, const char *format,...)
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


OraError::OraError (int oralibErr, const char *sourceName, long lineNumber, const char *format, ...)
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
OraError::OraError (const char *sourceName, long lineNumber,const char *format,	...)
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
#if	defined (_WIN32)
	m_winapiCode (err.m_winapiCode),
#endif
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
		m_description = "(OCI_SUCCESS)";
		break;

	case	OCI_SUCCESS_WITH_INFO:
		m_description = "(OCI_SUCCESS_WITH_INFO)";
		getDetails = true;
		break;

	case	OCI_ERROR:
		m_description = "(OCI_ERROR)";
		getDetails = true;
		break;

	case	OCI_NO_DATA:
		m_description = "(OCI_NO_DATA)";
		getDetails = true;
		break;

	case	OCI_INVALID_HANDLE:
		m_description = "(OCI_INVALID_HANDLE)";
		break;

	case	OCI_NEED_DATA:
		m_description = "(OCI_NEED_DATA)";
		break;

	case	OCI_STILL_EXECUTING:
		m_description = "(OCI_STILL_EXECUTING)";
		getDetails = true;
		break;

	case	OCI_CONTINUE:
		m_description = "(OCI_CONTINUE)";
		break;

	default:
		m_description = "unknown";
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
				OCIErrorGet ( errorHandle, 1, NULL, reinterpret_cast<int*>(&m_oraCode), reinterpret_cast<text *> (errorText), maxTextLen, OCI_HTYPE_ERROR);
			else
				OCIErrorGet ( envHandle, 1, NULL, reinterpret_cast<int*>(&m_oraCode), reinterpret_cast<text *> (errorText),	maxTextLen, OCI_HTYPE_ENV);

			m_description += " ";
			m_description += errorText;
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
		m_description = "(EC_ENV_CREATE_FAILED) Environment handle creation failed";
		break;

	case	EC_TIMEOUT:
		m_description = "(EC_TIMEOUT) Statement took too long to complete and has been aborted";
		break;

	case	EC_NO_MEMORY:
		m_description = "(EC_NO_MEMORY) Memory allocation request has failed";
		break;

	case	EC_BAD_PARAM_TYPE:
		m_description = "(EC_BAD_PARAM_TYPE) Parameter type is incorrect";
		break;

	case	EC_POOL_NOT_SETUP:
		m_description = "(EC_POOL_NOT_SETUP) Connection pool has not been setup yet";
		break;

	case	EC_BAD_INPUT_TYPE:
		m_description = "(EC_BAD_INPUT_TYPE) Input data doesn't have expected type";
		break;

	case	EC_BAD_OUTPUT_TYPE:
		m_description = "(EC_BAD_OUTPUT_TYPE) Cannot convert to requested type";
		break;

	case	EC_BAD_TRANSFORM:
		m_description = "(EC_BAD_TRANSFORM) Requested transformation is not possible";
		break;

	case	EC_BAD_PARAM_PREFIX:
		m_description = "(EC_BAD_PARAM_PREFIX) Parameter prefix is not known";
		break;

	case	EC_INTERNAL:
		m_description = "(EC_INTERNAL) Internal library error. Please, report to developers";
		break;

	case	EC_UNSUP_ORA_TYPE:
		m_description = "(EC_UNSUP_ORA_TYPE) Unsupported Oracle type - cannot be converted to numeric, date or text";
		break;

	case	EC_PARAMETER_NOT_FOUND:
		m_description = "(EC_PARAMETER_NOT_FOUND) Name not found in statement's parameters";
		break;

	case	EC_COLUMN_NOT_FOUND:
		m_description = "(EC_COLUMN_NOT_FOUND) Result set doesn't contain column with such name";
		break;

	default:
		m_description = "unknown";
	}
}


#if	defined (_WIN32) // Windows platform only
void OraError::winapiError ()
{
	unsigned long dwRet;
	char *errorText = NULL;

	// get last error's code
	m_winapiCode = GetLastError ();

	// get error message from the system
	dwRet = FormatMessage (
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,					// message source
		m_winapiCode,			// message id
		MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT),	// language id - default language
		(LPTSTR) &errorText,	// address of pointer to output buffer
		0,						// max size of output buffer
		NULL);					// va_list arguments

	if (errorText)
	{
		m_description = errorText;
		LocalFree ((HLOCAL) errorText);
	}
}
#endif // defined (_WIN32)


#if	!defined (ORALIB_NO_ERROR_DISPLAY)
void OraError::Display ()
{
	static const char *errorTypesText [] =
	{
		"unknown", "Oracle", "OraLib", "Win32 API"
	};

// 	std::cout
// 		<< "Source: " << m_source << ", line " << m_lineNo << "\n"
// 		<< "Error type: " << errorTypesText [m_type] << "\n"
// 		<< "Code: " << m_code << "\n"
// 		<< "Oracle error code: " << m_oraCode << "\n"
// 		<< "Win32 API error code: " << m_winapiCode << "\n"
// 		<< "Description: " << m_description << "\n";
}
#endif // !defined (ORALIB_NO_ERROR_DISPLAY)


std::string OraError::Details () const
{
	static const char *errorTypesText [] =
	{
		"unknown", "Oracle", "OraLib", "Win32 API"
	};

	return (
		"Error type: " + std::string (errorTypesText [m_type]) + "\n"
		"Description: " + m_description + "\n");
}


void OraError::concatMessage (const char *format, va_list va)
{
	// prerequisites
	assert (format && va);

	char *buffer = new char [ERROR_FORMAT_MAX_MSG_LEN];
	if (buffer)
	{
		// _vsnprintf - printf-like, with max buffer length and va_list input
		_vsnprintf_s(buffer, ERROR_FORMAT_MAX_MSG_LEN, ERROR_FORMAT_MAX_MSG_LEN - 1, format, va);
		m_description += ": ";
		m_description += buffer;
		delete [] buffer;
	}
}

