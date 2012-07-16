/*! 
OracleDB Table Column for the EpOraLibrary
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

#include "epColumn.h"
#include <tchar.h>

#include "epOraError.h"
#include "epResultSet.h"
#include "epConnection.h"
using namespace epol;

Column::Column (ResultSet *rs, const TCHAR *name, unsigned short ociDataType, unsigned int maxDataSize, int fetchSize) : SmartObject()
{
	EP_ASSERT (rs && name);

	initialize ();

	m_colName = epl::EpTString (name);
	m_ociType = ociDataType;

	switch (ociDataType)
	{
	case	SQLT_INT:	// integer
	case	SQLT_UIN:	// unsigned int

	case	SQLT_NUM:	// numeric
	case	SQLT_FLT:	// float
	case	SQLT_VNU:	// numeric with length
	case	SQLT_PDN:	// packed decimal
		m_ociType = SQLT_VNU;
		m_colType = DT_NUMBER;
		m_size = sizeof (OCINumber);
		break;

	case	SQLT_DAT:	// date
	case	SQLT_ODT:	// oci date - should not appear?
		m_ociType = SQLT_ODT;
		m_colType = DT_DATE;
		m_size = sizeof (OCIDate);
		break;

	case	SQLT_CHR:	// character string
	case	SQLT_STR:	// zero-terminated string
	case	SQLT_VCS:	// variable-character string
	case	SQLT_AFC:	// ansi fixed char
	case	SQLT_AVC:	// ansi var char
	case	SQLT_VST:	// oci string type
	case	SQLT_LNG:	// long character string type
		m_ociType = SQLT_STR;
		m_colType = DT_TEXT;
		m_size = (maxDataSize + 1) * sizeof(TCHAR);
		break;

	default:
		throw (OraError(EC_UNSUP_ORA_TYPE, __TFILE__, __LINE__, name));
	}

	m_indicators = EP_NEW short [fetchSize];

	if (m_colType == DT_TEXT)
		m_dataLengths = EP_NEW unsigned short [fetchSize];
	else
		m_dataLengths = NULL;

	m_fetchBuffer = EP_NEW char [m_size * fetchSize];

	m_defineHandle = NULL;

	if (!m_indicators || !m_fetchBuffer)
	{
		cleanUp (); 
		throw (OraError(EC_NO_MEMORY, __TFILE__, __LINE__));
	}

	m_resultSet = rs;
}


Column::~Column ()
{
	cleanUp ();
}


void Column::initialize ()
{
	m_colType = DT_UNKNOWN;
	m_ociType = 0;
	m_size = 0;
	m_indicators = NULL;
	m_dataLengths = NULL;
	m_fetchBuffer = NULL;
	m_defineHandle = NULL;
	m_resultSet = NULL;
}


void Column::cleanUp ()
{
	if (m_indicators) 
	{
		EP_DELETE [] m_indicators; 
	}
	m_indicators = NULL;
	if (m_dataLengths) 
		EP_DELETE [] m_dataLengths;
	m_dataLengths = NULL;
	if (m_fetchBuffer) 
		EP_DELETE [] m_fetchBuffer;
	m_fetchBuffer = NULL;
}


bool Column::IsNull () const
{
	EP_ASSERT (m_resultSet);

	unsigned short	rowNo = static_cast <unsigned short> (m_resultSet->m_currentRow % m_resultSet->m_fetchCount);
	return (m_indicators [rowNo] == -1);
}


epl::EpTString Column::ToString () const
{
	EP_ASSERT (m_resultSet);

	unsigned short	row_no = static_cast <unsigned short> (m_resultSet->m_currentRow % m_resultSet->m_fetchCount);
	if (m_colType == DT_TEXT &&	m_indicators [row_no] != -1)
	{
		return epl::EpTString(reinterpret_cast<TCHAR*>(m_fetchBuffer + m_size * row_no));
	}
	else
		throw (OraError(EC_BAD_OUTPUT_TYPE, __TFILE__, __LINE__));
}


double Column::ToDouble () const
{
	EP_ASSERT (m_resultSet);

	unsigned short	rowNo = static_cast <unsigned short> (m_resultSet->m_currentRow % m_resultSet->m_fetchCount);
	if (m_colType == DT_NUMBER &&
		m_indicators [rowNo] != -1)
	{
		double	value;
		int result = ::OCINumberToReal (m_resultSet->m_conn->m_errorHandle,reinterpret_cast <OCINumber *> (m_fetchBuffer) + rowNo, sizeof (double), &value);
		if (result == OCI_SUCCESS)
			return (value);
		else
			throw (OraError(result, m_resultSet->m_conn->m_errorHandle, __TFILE__, __LINE__));
	}
	else
		throw (OraError(EC_BAD_OUTPUT_TYPE, __TFILE__, __LINE__));
}


long
	Column::ToLong () const
{
	EP_ASSERT (m_resultSet);

	unsigned short	rowNo = static_cast <unsigned short> (m_resultSet->m_currentRow % m_resultSet->m_fetchCount);
	if (m_colType == DT_NUMBER &&
		m_indicators [rowNo] != -1)
	{
		long value;
		sword result = OCINumberToInt (m_resultSet->m_conn->m_errorHandle, reinterpret_cast <OCINumber *> (m_fetchBuffer) + rowNo, sizeof (long), OCI_NUMBER_SIGNED, &value);
		if (result == OCI_SUCCESS)
			return (value);
		else
			throw (OraError(result, m_resultSet->m_conn->m_errorHandle, __TFILE__, __LINE__));
	}
	else
		throw (OraError(EC_BAD_OUTPUT_TYPE, __TFILE__, __LINE__));
}


DateTime Column::ToDateTime () const
{
	EP_ASSERT (m_resultSet);

	unsigned short	rowNo = static_cast <unsigned short> (m_resultSet->m_currentRow % m_resultSet->m_fetchCount);
	if (m_colType == DT_DATE &&
		m_indicators [rowNo] != -1)
		return (DateTime (*(reinterpret_cast <OCIDate *> (m_fetchBuffer) + rowNo)));
	else
		throw (OraError(EC_BAD_OUTPUT_TYPE, __TFILE__, __LINE__));
}

