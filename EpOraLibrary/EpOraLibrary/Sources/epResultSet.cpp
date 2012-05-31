#include "epResultSet.h"
#include "epColumn.h"
#include "epOraError.h"
#include "epConnection.h"
#include <assert.h>

using namespace epol;

ResultSet::ResultSet (OCIStmt *rs, Connection *useConnection,unsigned int fetchSize)
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
	// free m_columns
	for (Columns::iterator i=m_columns.begin (); i!=m_columns.end (); ++i)
		delete (*i);
	m_columns.clear ();

	// when resultset is created by statement.select - freed by statement object
	// when created by parameter.as_resultset - freed by parameter object
	if (m_rsHandle) 
		m_rsHandle = NULL;

	// statement attached to be released?
	if (m_stmt) 
		m_stmt->Release ();
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
		// get next column info
		OCIParam	*paramHandle = NULL;
		TCHAR		*paramName = NULL;
		unsigned int nameLen = 0;
		unsigned short ociType = 0;
		unsigned int size = 0;

		result = OCIParamGet ( m_rsHandle, OCI_HTYPE_STMT, m_conn->m_errorHandle, reinterpret_cast <void **> (&paramHandle),i + 1);

		if (result == OCI_SUCCESS)
		{
			// column name
			result = OCIAttrGet (paramHandle, OCI_DTYPE_PARAM, &paramName, &nameLen, OCI_ATTR_NAME, m_conn->m_errorHandle);
		}

		if (result == OCI_SUCCESS)
		{
			// oci data type
			result = OCIAttrGet (paramHandle, OCI_DTYPE_PARAM, &ociType, NULL, OCI_ATTR_DATA_TYPE, m_conn->m_errorHandle);
		}

		if (result == OCI_SUCCESS)
		{
			// maximum data size in bytes
			result = OCIAttrGet (paramHandle, OCI_DTYPE_PARAM, &size, NULL, OCI_ATTR_DATA_SIZE, m_conn->m_errorHandle);
		}

		if (paramHandle)
			// ignore result code
			OCIDescriptorFree ( paramHandle, OCI_DTYPE_PARAM);

		// error situation?
		if (result != OCI_SUCCESS)
			throw (OraError(result, m_conn->m_errorHandle, __TFILE__, __LINE__));

		// setup new column, alloc memory for fetch buffer, indicators and data lens;
		// column.constructor could possibly throw an out-of-memory exception
		// in this case resultset will be partially initialized
		Column	*col = new Column (this, reinterpret_cast <const TCHAR *> (paramName), ociType, size, m_fetchCount);

		// add to array AND to map
		// (makes possible to address m_columns by name AND index)
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

	// define all m_columns
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
	// prerequisites
	assert (selectStmt);

	m_stmt = selectStmt;
}


void ResultSet::fetchRows ()
{
	// prerequisites
	assert (m_isDescribed && m_isDefined);

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
	// prerequisites
	assert (m_isDescribed && m_isDefined);

	m_currentRow++;
	if (m_currentRow >= m_rowsFetched)
		if (!m_isEod)
			// fetch new block of rows; fetch_rows will set m_isEod on true
			// when last block if rows has been fetched; will also update m_rowsFetched
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
		// column with such name is not found
		throw (OraError(EC_COLUMN_NOT_FOUND, __TFILE__, __LINE__, columnName));
	return (*(i->second));
}


Column& ResultSet::operator [] (unsigned short columnIndex)
{
	if (columnIndex < FIRST_COLUMN_NO || columnIndex > m_columns.size ())
		// no column with such index
		throw (OraError(EC_COLUMN_NOT_FOUND, __TFILE__, __LINE__, _T("%d"), (int) columnIndex));
	return (*(m_columns.at (columnIndex - FIRST_COLUMN_NO)));
}

