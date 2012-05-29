#ifndef __EP_COLUMN_H__
#define __EP_COLUMN_H__
#include "epOraLib.h"
#include "epDateTime.h"

namespace epol
{
	class EP_ORACLELIB Column
	{
		friend class resultset;
	public:
		// returns whether column value is null
		bool is_null (void) const;

		// returns column value as a text
		inline operator Pstr (void) const { return (ToString ()); };
		Pstr ToString() const;

		// returns column value as a double
		inline operator double (void) const { return (ToDouble ()); };
		double ToDouble() const;

		// returns column value as a long
		inline operator long (void) const { return (ToLong ()); };
		long ToLong () const;

		// returns column value as a date/time helper class
		inline operator DateTime (void) const { return (ToDateTime ()); };
		DateTime ToDateTime() const;

		// (column is not deleted by the client, but when result set is released instead)
		inline void release (void) { };

	private:
		// public not creatable; use connection.select or statement.select
		// attaches this column to a result set; allocates memory for the fetch buffer
		column (ResultSet *rs, const char *name, ub4 name_len, ub2 oci_data_type, ub4 max_data_size, int fetch_size = FETCH_SIZE);

		// public not deletable; deleted, when result set is released
		~column ();

		// initialize data members
		void initialize (void);

		// free resources allocated
		void cleanup (void);

		// private copy-constructor and assignment operator - class could not be copied
		column (
			IN const column& /* col */) { /* could not be copy-constructed */ };
		column& operator = (
			IN const column& /* col */) { return (*this); /* could not be assigned */ };


		std::string m_colName;		// in the exact case
		DataTypesEnum m_colType;		// as it will be returned
		ub2 m_ociType;		// oracle's data type
		int	m_size;			// number of bytes required for

		sb2* m_indicators;	// an array with indicators: 0 - ok; -1 - null
		ub2* m_dataLength;		// an array with data lengths (for text columns)
		char* m_fetchBuffer;	// where data is returned (fetched)

		OCIDefine* m_defineHandle;	// used for the column
		ResultSet* m_resultSet;	// is the column's owner


	}; // column class

}


#endif //__EP_COLUMN_H__