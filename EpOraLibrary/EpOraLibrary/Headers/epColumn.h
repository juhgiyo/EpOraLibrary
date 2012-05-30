#ifndef __EP_COLUMN_H__
#define __EP_COLUMN_H__
#include "epOraLib.h"
#include "epDateTime.h"

namespace epol
{
	class EP_ORACLELIB Column
	{
		friend class ResultSet;
	public:
		// returns whether column value is null
		bool IsNull () const;

		// returns column value as a text
		inline operator Pstr () const { return (ToString ()); };
		Pstr ToString() const;

		// returns column value as a double
		inline operator double () const { return (ToDouble ()); };
		double ToDouble() const;

		// returns column value as a long
		inline operator long () const { return (ToLong ()); };
		long ToLong () const;

		// returns column value as a date/time helper class
		inline operator DateTime () const { return (ToDateTime ()); };
		DateTime ToDateTime() const;

	private:
		// public not creatable; use connection.select or statement.select
		// attaches this column to a result set; allocates memory for the fetch buffer
		Column (ResultSet *rs, const char *name, unsigned int nameLen, unsigned short ociDataType, unsigned int maxDataSize, int fetchSize = FETCH_SIZE);

		// public not deletable; deleted, when result set is released
		~Column ();

		// initialize data members
		void initialize ();

		// free resources allocated
		void cleanup ();

		// private copy-constructor and assignment operator - class could not be copied
		Column (const Column& /* col */) { /* could not be copy-constructed */ };
		Column& operator = (const Column& /* col */) 
		{
			return (*this); /* could not be assigned */ 
		};


		std::string m_colName;		// in the exact case
		DataTypesEnum m_colType;		// as it will be returned
		unsigned short m_ociType;		// oracle's data type
		int	m_size;			// number of bytes required for

		short* m_indicators;	// an array with indicators: 0 - ok; -1 - null
		unsigned short* m_dataLengths;		// an array with data lengths (for text columns)
		char* m_fetchBuffer;	// where data is returned (fetched)

		OCIDefine* m_defineHandle;	// used for the column
		ResultSet* m_resultSet;	// is the column's owner


	}; // column class

}


#endif //__EP_COLUMN_H__