/*! 
@file epOraDefines.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eporalibrary>
@date May 24, 2012
@brief epOraLibrary related Defines Interface
@version 1.0

@section LICENSE

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

@section DESCRIPTION

An Interface for epOraLibrary related Defines.
*/

#ifndef __EP_ORADEFINES_H__
#define __EP_ORADEFINES_H__
#include "epOraLib.h"
#include "oci.h"

namespace epol
{
	/// Enumerator for Month
	enum MonthsEnum
	{
		/// January
		JAN = 1,
		/// Febuary
		FEB,
		/// March
		MAR,
		/// April
		APR,
		/// May
		MAY,
		/// June
		JUN,
		/// July
		JUL,
		/// August
		AUG,
		/// September
		SEP,
		/// October
		OCT,
		/// November
		NOV,
		/// December
		DEC
	};


	/// Enumerator for Internal data types
	enum DataTypesEnum
	{
		/// Unknown data type
		DT_UNKNOWN,
		/// Number type
		DT_NUMBER,
		/// Date type
		DT_DATE,
		/// Text type
		DT_TEXT,
		/// ResultSet type
		DT_RESULT_SET
	};

	/// Enumberator for Parameter prefixes
	//* (Ex: :n1 is a number, :sName is a text)
	enum ParameterPrefixesEnum
	{
		/// Array prefix
		PP_ARRAY = 't',
		/// Numeric prefix
		PP_NUMERIC = 'n',
		/// Date prefix
		PP_DATE = 'd',
		/// Text prefix
		PP_TEXT = 's',
		/// ResultSet prefix
		PP_RESULT_SET = 'c'
	};

	/*!
	@def MAX_OUTPUT_TEXT_BYTES
	@brief maximal length of a text input and/or output parameter

	Macro for the maximal length (in characters) of a text input and/or output parameter.
	*/
	#define MAX_OUTPUT_TEXT_BYTES 4096

	/*!
	@def FETCH_SIZE
	@brief number of rows to request on each fetch

	Macro for the number of rows to request on each fetch.
	*/
	#define FETCH_SIZE 200000

	/*!
	@def ERROR_FORMAT_MAX_MSG_LEN
	@brief The maximum length of the error message

	Macro for the maximum length of the error message.
	*/
	#define ERROR_FORMAT_MAX_MSG_LEN 1024

	/*!
	@def FIRST_COLUMN_NO
	@brief The index of the first column in a result set

	Macro for the index of the first column in a result set (one of 0, 1)
	*/
	#define FIRST_COLUMN_NO 0

	/*!
	@def FIRST_PARAMETER_NO
	@brief The index of the first parameter in a statement

	Macro for the index of the first parameter in a statement (one of 0, 1)
	*/
	#define FIRST_PARAMETER_NO 0

	/// Enumberator for error codes thrown from EpOraLibrary
	typedef enum _errorCodesEnum
	{
		/// OCI Error
		EC_OCI_ERROR = -1,
		/// Environment Creation Failed
		EC_ENV_CREATE_FAILED = 1000,
		/// Time out Error
		EC_TIMEOUT,
		/// Allocation failed Error
		EC_NO_MEMORY,
		/// Bad parameter Error
		EC_BAD_PARAM_TYPE,
		/// Pool not setup Error
		EC_POOL_NOT_SETUP,
		/// Bad input type Error
		EC_BAD_INPUT_TYPE,
		/// Bad output type Error
		EC_BAD_OUTPUT_TYPE,
		/// Bad transform Error
		EC_BAD_TRANSFORM,
		/// Bad parameter prefix Error
		EC_BAD_PARAM_PREFIX,
		/// UnSup Oracle type Error
		EC_UNSUP_ORA_TYPE,
		/// Parameter not found Error
		EC_PARAMETER_NOT_FOUND,
		/// Column not found error
		EC_COLUMN_NOT_FOUND,
		/// Internal error
		EC_INTERNAL
	}ErrorCodesEnum;

	/// Enumberator for error type
	typedef enum _errorTypesEnum
	{
		/// Unknown error type
		ET_UNKNOWN = 0,
		/// Oracle error type
		ET_ORACLE,
		/// EpOraLibrary error type
		ET_ORALIB,
		/// WinAPI error type
		ET_WINAPI
	}ErrorTypesEnum;

	/// Enumberator for statement type 
	// *Select statements and pl/sql blocks are handled with care
	typedef enum _statementTypesEnum
	{
		/// Unknown type
		ST_UNKNOWN,
		/// Select type
		ST_SELECT = OCI_STMT_SELECT,
		/// Update type
		ST_UPDATE = OCI_STMT_UPDATE,
		/// Delete type
		ST_DELETE = OCI_STMT_DELETE,
		/// Insert type
		ST_INSERT = OCI_STMT_INSERT,
		/// Create type
		ST_CREATE = OCI_STMT_CREATE,
		/// Drop type
		ST_DROP = OCI_STMT_DROP,
		/// Alter type
		ST_ALTER = OCI_STMT_ALTER,
		/// Begin type
		ST_BEGIN = OCI_STMT_BEGIN,
		/// Declare type
		ST_DECLARE = OCI_STMT_DECLARE
	}StatementTypesEnum;

	/*!
	@def ORADATA_NULL
	@brief The value of the NULL for Oracle Data

	Macro for the value of the NULL for Oracle Data
	*/
	#define ORADATA_NULL -1

	/*!
	@def ORADATA_OK
	@brief The value of the OK for Oracle Data

	Macro for the value of the OK for Oracle Data
	*/
	#define ORADATA_OK 0
}
#endif //__EP_ORADEFINES_H__