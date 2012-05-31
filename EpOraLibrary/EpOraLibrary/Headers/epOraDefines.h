/*! 
@file epOraDefines.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eporalibrary>
@date May 24, 2012
@brief epOraLibrary related Defines Interface
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

An Interface for epOraLibrary related Defines.

@section NOTICE

EpOraLibrary is developed by referencing oraLib 0.0.3, which was developed by 606u <606u@dir.bg> 
<http://606u.dir.bg/>
606u had kindly approved using his/her oraLib freely for EpOraLibrary development.
*/

#ifndef __EP_ORADEFINES_H__
#define __EP_ORADEFINES_H__
#include "epOraLib.h"
#include "oci.h"

namespace epol
{
	enum MonthsEnum
	{
		JAN = 1,
		FEB,
		MAR,
		APR,
		MAY,
		JUN,
		JUL,
		AUG,
		SEP,
		OCT,
		NOV,
		DEC
	};


	// internal data types are 4 only: number, date, text and result set
	enum DataTypesEnum
	{
		DT_UNKNOWN,
		DT_NUMBER,
		DT_DATE,
		DT_TEXT,
		DT_RESULT_SET
	};

	// parameter prefixes (for example: :n1 is a number, :sName is a text)
	enum ParameterPrefixesEnum
	{
		PP_ARRAY = 't',
		PP_NUMERIC = 'n',
		PP_DATE = 'd',
		PP_TEXT = 's',
		PP_RESULT_SET = 'c'
	};

	// maximal length (in characters) of a text input and/or output parameter
	#define MAX_OUTPUT_TEXT_BYTES 4096

	// number of rows to request on each fetch
	#define FETCH_SIZE 200000

	#define ERROR_FORMAT_MAX_MSG_LEN 1024

	// index of the first column in a result set (one of 0, 1)
	#define FIRST_COLUMN_NO 1

	// index of the first parameter in a statement (one of 0, 1)
	#define FIRST_PARAMETER_NO 1


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

	// statement type - select statements and pl/sql blocks are handled with care
	typedef enum _statementTypesEnum
	{
		ST_UNKNOWN,
		ST_SELECT = OCI_STMT_SELECT,
		ST_UPDATE = OCI_STMT_UPDATE,
		ST_DELETE = OCI_STMT_DELETE,
		ST_INSERT = OCI_STMT_INSERT,
		ST_CREATE = OCI_STMT_CREATE,
		ST_DROP = OCI_STMT_DROP,
		ST_ALTER = OCI_STMT_ALTER,
		ST_BEGIN = OCI_STMT_BEGIN,
		ST_DECLARE = OCI_STMT_DECLARE
	}StatementTypesEnum;
}
#endif //__EP_ORADEFINES_H__