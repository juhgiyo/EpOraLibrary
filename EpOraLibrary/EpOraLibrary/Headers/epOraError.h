/*! 
@file epOraError.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eporalibrary>
@date May 30, 2012
@brief OracleDB Error Interface
@version 1.0

@section LICENSE

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

@section DESCRIPTION

An Interface for OracleDB Error.

@section NOTICE

EpOraLibrary is developed by referencing oraLib 0.0.3, which was developed by 606u <606u@dir.bg> 
<http://606u.dir.bg/>
606u had kindly approved using his/her oraLib freely for EpOraLibrary development.
*/
#ifndef	__EP_ORAERROR_H__
#define	__EP_ORAERROR_H__
#include "epOraLib.h"
#include "epOraDefines.h"
#include "oci.h"
#include <string>

using namespace epl;

namespace epol {

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


}


#endif	// __EP_ORAERROR_H__
