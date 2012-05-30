/*! 
@file epOraLib.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
@date May 24, 2012
@brief EP Oracle Library Interface
@version 2.0

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

An Interface for EP Oracle Library.

*/

#ifndef __EP_ORACLE_LIB_H__
#define __EP_ORACLE_LIB_H__

#ifdef EP_ORACLELIB_DLL_EXPORT

#define EP_ORACLELIB __declspec(dllexport)

#else   //EP_ORACLELIB_DLL_EXPORT

#ifdef EP_ORACLE_DLL_IMPORT

#define EP_ORACLELIB __declspec(dllimport)

#else   //EP_ORACLELIB_DLL_IMPORT

#define EP_ORACLELIB

#endif  //EP_ORACLELIB_DLL_IMPORT
#endif  //EP_ORACLELIB_DLL_EXPORT


// internal data types are 4 only: number, date, text and result set
enum DataTypesEnum
{
	DT_UNKNOWN,
	DT_NUMBER,
	DT_DATE,
	DT_TEXT,
	DT_RESULT_SET
};


// number of rows to request on each fetch
#define FETCH_SIZE 200000


#endif //__EP_ORACLE_LIB_H__