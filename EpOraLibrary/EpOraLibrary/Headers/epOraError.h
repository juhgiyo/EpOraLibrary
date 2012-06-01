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
*/
#ifndef	__EP_ORAERROR_H__
#define	__EP_ORAERROR_H__
#include "epOraLib.h"
#include "epOraDefines.h"
#include "oci.h"
#include <string>

using namespace epl;

namespace epol {

	/*! 
	@class OraError epOraError.h
	@brief This is a class representing OraError 

	Interface for the OraError.
	*/
	class OraError
	{
	public:
		/*!
		Default Constructor

		Create the Error object with given information via error handle
		@param[in] oraErr the Oracle error code
		@param[in] errorHandle the Oracle error handle
		@param[in] sourceName the name of the source where error occurred
		@param[in] lineNumber the line of the source where error occurred
		@param[in] format the string format for error message
		*/
		OraError (int oraErr,OCIError *errorHandle, const TCHAR *sourceName = NULL, long lineNumber = -1, const TCHAR *format = NULL, ...);

		/*!
		Default Constructor

		Create the Error object with given information via environment handle
		@param[in] oraErr the Oracle error code
		@param[in] envHandle the Oracle environment handle
		@param[in] sourceName the name of the source where error occurred
		@param[in] lineNumber the line of the source where error occurred
		@param[in] format the string format for error message
		*/
		OraError (int oraErr, OCIEnv *envHandle,const TCHAR *sourceName = NULL, long lineNumber = -1, const TCHAR *format = NULL,...);

		/*!
		Default Constructor

		Create the Error object with given information via EpOraLibrary error
		@param[in] oralibErr the EpOraLibrary error code
		@param[in] sourceName the name of the source where error occurred
		@param[in] lineNumber the line of the source where error occurred
		@param[in] format the string format for error message
		*/
		OraError (int oralibErr, const TCHAR *sourceName = NULL, long lineNumber = -1, const TCHAR *format = NULL,...);

		/*!
		Default Constructor

		Create the Error object with given information via Windows API error
		@param[in] sourceName the name of the source where error occurred
		@param[in] lineNumber the line of the source where error occurred
		@param[in] format the string format for error message
		*/
		OraError (const TCHAR *sourceName = NULL, long lineNumber = -1,const TCHAR *format = NULL, ...);

		/*!
		Default Copy Constructor

		@param err the Error object to copy
		*/
		OraError (const OraError& err);

		/*!
		Default Destructor
		
		Release the Error object
		*/
		~OraError ();

		/*!
		Return error details
		*In a format, similar to display method output
		@return the detailed error in text
		*/
		EpTString Details() const;

	private:

		/*!
		Copy Operator

		*Class cannot be copied
		@param err the Error object to copy
		*/
		OraError& operator = (const OraError& err ) 
		{
			return (*this); /* could not be copy-constructed */ 
		};

		/*!
		Set-up given Oracle error details
		@param[in] oraErr the Oracle Error Code
		@param[in] errorHandle the Oracle error handle
		@param[in] envHandle the Oracle environment handle
		*/
		void oracleError (int oraErr,OCIError *errorHandle, OCIEnv *envHandle);

		/*!
		Set-up given EpOraLibrary error details
		@param[in] oralibErr the EpOraLibrary Error Code
		*/
		void oralibError (int oralibErr);

		/*!
		Set-up given WinAPI error details
		*/
		void winapiError ();

		/*!
		Format printf-like message and concats it to the description
		@param[in] format the format of the message
		@param[in] va the variadic argument
		*/
		void concatMessage (const TCHAR *format, va_list va);

		/// the type of the error
		ErrorTypesEnum	m_type;	
		/// the error code if library error or -1 if Oracle error
		int		m_code;		
		/// Oracle's error code (ORA-xxxxx)
		unsigned int m_oraCode;
		/// Windows API error code
		unsigned long	m_winapiCode;
		/// the description of error in text
		EpTString	m_description;
		/// the source file where the error occurred
		EpTString	m_source;			
		/// the line number where the error occurred
		long		m_lineNo;

	};


}


#endif	// __EP_ORAERROR_H__
