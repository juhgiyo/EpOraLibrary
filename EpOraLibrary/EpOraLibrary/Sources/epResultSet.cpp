/*! 
OracleDB Result Set for the EpOraLibrary
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
#include "epResultSet.h"
#include "epColumn.h"
#include "epOraError.h"
#include "epConnection.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)


using namespace epol;

ResultSet::ResultSet (OCIStmt *rs, Connection *useConnection,unsigned int fetchSize):SmartObject()
{
	initialize ();
	try
	{
		attach (rs,	useConnection, fetchSize);
	}
	catch (...)
	{
		cleanUp ();
		throw;
	}
}


ResultSet::~ResultSet ()
{
	cleanUp ();
}


void ResultSet::initialize ()
{
	m_conn = NULL;
	m_rsHandle = NULL;
	m_stmt = NULL;
	m_fetchCount = 0;
	m_rowsFetched = 0;
	m_currentRow = 0;
	m_isEod = false;
	m_isDescribed = false;
	m_isDefined = false;
}


void ResultSet::cleanUp ()
{
	for (Columns::iterator i=m_columns.begin (); i!=m_columns.end (); ++i)
		(*i)->ReleaseObj();
	m_columns.clear ();

	if (m_rsHandle) 
		m_rsHandle = NULL;

	if (m_stmt) 
		m_stmt->ReleaseObj();
	m_stmt = NULL;
}


void ResultSet::attach (OCIStmt *rs,Connection *useConnection,unsigned int fetchSize)	// = FETCH_SIZE
{
	m_conn = useConnection;
	m_rsHandle = rs;
	m_stmt = NULL;
	m_fetchCount = fetchSize;

	m_rowsFetched = 0;
	m_currentRow = 0;
	m_isEod = false;

	m_isDescribed = false;
	m_isDefined = false;

	describe ();
	define ();
}


unsigned int ResultSet::columnsCount ()
{
	int	result;
	unsigned int count;

	count = 0;
	result = OCIAttrGet (m_rsHandle, OCI_HTYPE_STMT, &count, NULL, OCI_ATTR_PARAM_COUNT, m_conn->m_errorHandle);
	if (result == OCI_SUCCESS)
		return (count);
	else
		throw (OraError(result, m_conn->m_errorHandle, __TFILE__, __LINE__));
}


unsigned int ResultSet::rowsCount ()
{
	int	result;
	unsigned int		count;

	count = 0;
	result = OCIAttrGet (m_rsHandle, OCI_HTYPE_STMT, &count, NULL, OCI_ATTR_ROW_COUNT, m_conn->m_errorHandle);
	if (result == OCI_SUCCESS)
	{
		m_nTotRow += count;
		return (count);
	}
	else
		throw (OraError(result, m_conn->m_errorHandle, __TFILE__, __LINE__));
}


void ResultSet::describe ()
{
	int		result;
	unsigned int count, i;

	count = columnsCount();
	m_columns.reserve(count);
	for (i=0; i<count; i++)
	{
		OCIParam	*paramHandle = NULL;
		TCHAR		*paramName = NULL;
		unsigned int nameLen = 0;
		unsigned short ociType = 0;
		unsigned short size = 0;

		result = OCIParamGet ( m_rsHandle, OCI_HTYPE_STMT, m_conn->m_errorHandle, reinterpret_cast <void **> (&paramHandle),i + 1);

		if (result == OCI_SUCCESS)
		{
			result = OCIAttrGet (paramHandle, OCI_DTYPE_PARAM, &paramName, &nameLen, OCI_ATTR_NAME, m_conn->m_errorHandle);
		}

		if (result == OCI_SUCCESS)
		{
			result = OCIAttrGet (paramHandle, OCI_DTYPE_PARAM, &ociType, NULL, OCI_ATTR_DATA_TYPE, m_conn->m_errorHandle);
		}

		if (result == OCI_SUCCESS)
		{
			result = OCIAttrGet (paramHandle, OCI_DTYPE_PARAM, &size, NULL, OCI_ATTR_DATA_SIZE, m_conn->m_errorHandle);
		}

		if (paramHandle)
			OCIDescriptorFree ( paramHandle, OCI_DTYPE_PARAM);

		if (result != OCI_SUCCESS)
			throw (OraError(result, m_conn->m_errorHandle, __TFILE__, __LINE__));

		Column	*col = EP_NEW Column (this, reinterpret_cast <const TCHAR *> (paramName), ociType, size, m_fetchCount);

		m_columns.push_back (col);
		m_columnsMap [col->m_colName] = col;
	}

	m_isDescribed = true;
	m_isDefined = false;
}


void ResultSet::define()
{
	Columns::iterator	i;
	int	result;
	unsigned int position;

	position = 1;
	result = OCI_SUCCESS;
	for (i=m_columns.begin (); result == OCI_SUCCESS && i!=m_columns.end (); ++i)
	{
		result = OCIDefineByPos (m_rsHandle, &((*i)->m_defineHandle), m_conn->m_errorHandle, position++, (*i)->m_fetchBuffer, (*i)->m_size,	(*i)->m_ociType, (*i)->m_indicators, (*i)->m_dataLengths, NULL, OCI_DEFAULT);

#if defined(_UNICODE) || defined(UNICODE)
		if (result == OCI_SUCCESS && (*i)->m_colType == DT_TEXT)
		{

			unsigned int value = OCI_UTF16ID;
			result = OCIAttrSet ( (*i)->m_defineHandle, OCI_HTYPE_DEFINE, &value, sizeof (value), OCI_ATTR_CHARSET_ID, m_conn->m_errorHandle);
		}
#endif //defined(_UNICODE) || defined(UNICODE)

		if (result != OCI_SUCCESS)
			throw (OraError(result, m_conn->m_errorHandle, __TFILE__, __LINE__, (*i)->m_colName.c_str ()));
	}
	m_isDefined = true;
}


void ResultSet::attachStatement (Statement *selectStmt)
{
	EP_ASSERT (selectStmt);

	m_stmt = selectStmt;
}


void ResultSet::fetchRows ()
{
	EP_ASSERT (m_isDescribed && m_isDefined);

	int	result;
	unsigned int oldRowsCount = m_rowsFetched;

	result = OCIStmtFetch (m_rsHandle, m_conn->m_errorHandle, m_fetchCount, OCI_FETCH_NEXT, OCI_DEFAULT);
	if (result == OCI_SUCCESS || result == OCI_NO_DATA || result == OCI_SUCCESS_WITH_INFO)
	{
		m_rowsFetched = rowsCount ();
		if (m_rowsFetched - oldRowsCount != m_fetchCount)
			m_isEod = true;
	}
	else
		throw (OraError(result, m_conn->m_errorHandle, __TFILE__, __LINE__));
}


bool ResultSet::Next ()
{
	EP_ASSERT (m_isDescribed && m_isDefined);

	m_currentRow++;
	if (m_currentRow >= m_rowsFetched)
		if (!m_isEod)
			fetchRows();
		else
			return (false);
	if (m_currentRow >= m_rowsFetched)
		return (false);

	return (true);
}


Column& ResultSet::operator [] (const TCHAR *columnName)
{
	ColumnsMap::iterator i = m_columnsMap.find (columnName);
	if (i == m_columnsMap.end ())
		throw (OraError(EC_COLUMN_NOT_FOUND, __TFILE__, __LINE__, columnName));
	return (*(i->second));
}


Column& ResultSet::operator [] (unsigned short columnIndex)
{
	if (columnIndex < FIRST_COLUMN_NO || columnIndex > m_columns.size ())
		throw (OraError(EC_COLUMN_NOT_FOUND, __TFILE__, __LINE__, _T("%d"), (int) columnIndex));
	return (*(m_columns.at (columnIndex - FIRST_COLUMN_NO)));
}

