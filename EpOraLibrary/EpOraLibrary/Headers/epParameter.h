#ifndef	__EP_PARAMETER_H__
#define	__EP_PARAMETER_H__

#include "epOraLib.h"
#include "epDateTime.h"
#include <string>

namespace epol {


	class ResultSet;

	class EP_ORACLELIB Parameter
	{
		// friends
		friend class Statement;

	public:
		// sets parameter value to null
		inline void ToNull () 
		{ 
			m_indicator = -1; 
		}

		// sets parameter value to some text
		Parameter& operator = (Pstr text);

		// sets parameter value to some double
		Parameter& operator = (double value);

		// sets parameter value to some long
		Parameter& operator = (long value);

		// sets parameter value to some date/time
		Parameter& operator = (const DateTime& dateTime);

		// returns whether parameter value is null
		inline bool IsNull () const 
		{
			return (m_indicator == -1); 
		};

		// returns parameter value as a text
		inline operator Pstr () const 
		{
			return (ToString ()); 
		};
		Pstr ToString () const;

		// returns parameter value as a double
		inline operator double () const 
		{ 
			return (ToDouble ()); 
		};
		double ToDouble () const;

		// returns parameter value as a long
		inline operator long () const 
		{ 
			return (ToLong ()); 
		};
		long ToLong () const;

		// returns parameter value as a date/time helper object
		inline operator DateTime () const 
		{ 
			return (ToDateTime ()); 
		};
		DateTime ToDateTime () const;

		// returns a resultset for a cursor bound variable
		inline operator ResultSet& () 
		{
			return (ToResultSet ()); 
		};
		ResultSet& ToResultSet ();

	private:
		// public not creatable; use statement.bind instead
		// attaches parameter to a statement
		// when type is set to DT_UNKNOWN type is taken from name's prefix
		Parameter (Statement *to, const char *name, DataTypesEnum type = DT_UNKNOWN, unsigned int fetchSize = FETCH_SIZE);

		~Parameter ();

		// private copy-constructor and assignment operator - class could not be copied
		Parameter ( const Parameter& /* var */) 
		{ 
			/* could not be copy-constructed */ 
		};
		Parameter& operator = (const Parameter& /* var */) 
		{ 
			return (*this); /* could not be copy-constructed */ 
		};

		// initialize private data
		void initialize ();

		// free resources allocated
		void cleanUp ();

		// attaches parameter to a statement
		// when type is set to DT_UNKNOWN type is taken from name's prefix
		void attach (Statement *to, const char *name, DataTypesEnum type = DT_UNKNOWN, unsigned int fetchSize = FETCH_SIZE);

		// sets-up name, type, oci_type and size, depending on type value
		// when type is set to DT_UNKNOWN type is taken from name's prefix
		void setupType (const char *paramName, DataTypesEnum type = DT_UNKNOWN);

		// binds an input and/or output parameter to the statement to
		void bind (Statement *to);

		// binds a result set - fetch_size rows will be retrieved on each step
		void bindResultSet (Statement *to,unsigned int fetchSize = FETCH_SIZE);


		std::string		m_paramName;		// in the exact case, including leading ':'
		DataTypesEnum	m_paramType;		// as it will be returned
		unsigned short				m_ociType;		// oracle's data type
		unsigned short				m_size;			// number of bytes required for

		short				m_indicator;		// 0 - ok; -1 - null
		unsigned short				m_dataLen;		// number of bytes returned (used for text)
		char			*m_fetchBuffer;	// where data is returned

		bool			m_isArray;		// with values?

		Statement		*m_stmt;			// parameter is bound to
		OCIBind			*m_bindHandle;
		OCIStmt			*m_rsHandle;		// if parameter is a result set
		ResultSet		*m_resultSet;	// if parameter is a result set



	}; // parameter class


};


#endif	// __EP_PARAMETER_H__
