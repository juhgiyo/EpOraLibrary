#ifndef	__EP_ORAERROR_H__
#define	__EP_ORAERROR_H__
#include "epOraLib.h"
#include "oci.h"
#include <string>

using namespace epl;
namespace epol {


	// error codes thrown from the library
	typedef enum _errorCodesEnum
	{
		EC_OCI_ERROR = -1,
		EC_ENV_CREATE_FAILED = 1000,
		EC_TIMEOUT,
		EC_NO_MEMORY,
		EC_BAD_PARAM_TYPE,
		EC_POOL_NOT_SETUP,
		EC_BAD_INPUT_TYPE,
		EC_BAD_OUTPUT_TYPE,
		EC_BAD_TRANSFORM,
		EC_BAD_PARAM_PREFIX,
		EC_UNSUP_ORA_TYPE,
		EC_PARAMETER_NOT_FOUND,
		EC_COLUMN_NOT_FOUND,
		EC_INTERNAL
	}ErrorCodesEnum;


	// error types
	typedef enum _errorTypesEnum
	{
		ET_UNKNOWN = 0,
		ET_ORACLE,
		ET_ORALIB,
		ET_WINAPI
	}ErrorTypesEnum;


	class OraError
	{
	public:
		// oracle error via error handle
		OraError (int oraErr,OCIError *errorHandle, const TCHAR *sourceName = NULL, long lineNumber = -1, const TCHAR *format = NULL, ...);

		// oracle error via environment handle
		OraError (int oraErr, OCIEnv *envHandle,const TCHAR *sourceName = NULL, long lineNumber = -1, const TCHAR *format = NULL,...);

		// library error
		OraError (int oralibErr, const TCHAR *sourceName = NULL, long lineNumber = -1, const TCHAR *format = NULL,...);

		// winapi error
		OraError (const TCHAR *sourceName = NULL, long lineNumber = -1,const TCHAR *format = NULL, ...);

		// copy constructor
		OraError (const OraError& err);

		~OraError ();

		// return error details (in a format, similar to display method output)
		EpTString Details() const;

	private:

		// private assignment operator - class could not be copied
		OraError& operator = (const OraError& /* err */) 
		{
			return (*this); /* could not be copy-constructed */ 
		};

		// sets-up an oracle error details
		void oracleError (int oraErr,OCIError *errorHandle, OCIEnv *envHandle);

		// sets-up a library error details
		void oralibError (int oralibErr);

		// sets-up a winapi call error details
		void winapiError (void);

		// formats printf-like message and concats it to the description
		void concatMessage (const TCHAR *format, va_list va);

		ErrorTypesEnum	m_type;		// type
		int		m_code;			// error code if library error or -1 if Oracle error
		unsigned int m_oraCode;		// Oracle's error code - ORA-xxxxx
		unsigned long	m_winapiCode;	// win32 api error code (Windows platform only!)

		EpTString	m_description;	// error description as a text
		EpTString	m_source;			// source file, where error was thrown (optional)
		long		m_lineNo;		// line number, where error was thrown (optional)

	};


};


#endif	// __EP_ORAERROR_H__
