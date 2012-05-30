#ifndef	__EP_ORAERROR_H__
#define	__EP_ORAERROR_H__
#include "epOraLib.h"
#include "oci.h"
#include <string>
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
		OraError (int oraErr,OCIError *errorHandle, const char *sourceName = NULL, long lineNumber = -1, const char *format = NULL, ...);

		// oracle error via environment handle
		OraError (int oraErr, OCIEnv *envHandle,const char *sourceName = NULL, long lineNumber = -1, const char *format = NULL,...);

		// library error
		OraError (int oralibErr, const char *sourceName = NULL, long lineNumber = -1, const char *format = NULL,...);

#if	defined (_WIN32)
		// winapi error
		OraError (const char *sourceName = NULL, long lineNumber = -1,const char *format = NULL, ...);
#endif

		// copy constructor
		OraError (const OraError& err);

		~OraError ();

		// return error details (in a format, similar to display method output)
		std::string	Details() const;

#if	!defined (ORALIB_NO_ERROR_DISPLAY)
		// dumps error details to cout
		void	Display();
#endif

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

#if	defined (_WIN32)
		// sets-up a winapi call error details
		void winapiError (void);
#endif

		// formats printf-like message and concats it to the description
		void concatMessage (const char *format, va_list va);

		ErrorTypesEnum	m_type;		// type
		int		m_code;			// error code if library error or -1 if Oracle error
		unsigned int m_oraCode;		// Oracle's error code - ORA-xxxxx
#if	defined (_WIN32)
		unsigned long	m_winapiCode;	// win32 api error code (Windows platform only!)
#endif
		std::string	m_description;	// error description as a text
		std::string	m_source;			// source file, where error was thrown (optional)
		long		m_lineNo;		// line number, where error was thrown (optional)

	};


};


#endif	// __EP_ORAERROR_H__
