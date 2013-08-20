/*! 
@file epParameter.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eporalibrary>
@date May 30, 2012
@brief OracleDB Parameter Interface
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

An Interface for OracleDB Parameter.
*/
#ifndef	__EP_PARAMETER_H__
#define	__EP_PARAMETER_H__

#include "epOraLib.h"
#include "epDateTime.h"
#include <string>



namespace epol {


	class ResultSet;

	/*! 
	@class Parameter epParameter.h
	@brief This is a class representing OracleDB Parameter 

	Interface for the OracleDB Parameter.
	*/
	class EP_ORACLELIB Parameter: public epl::SmartObject
	{
		// friends
		friend class Statement;

	public:

		/*!
		Return whether the parameter value is NULL
		@return true if the parameter value is NULL, otherwise false
		*/
		inline bool IsNull () const 
		{
			return (m_indicator == ORADATA_NULL); 
		};

		/*!
		Returns the parameter data by converting it to String
		@return parameter data in string format.
		*/
		epl::EpTString ToString () const;

		/*!
		Returns the parameter data by converting it to double
		@return parameter data in double format.
		*/
		double ToDouble () const;

		/*!
		Returns the parameter data by converting it to long
		@return parameter data in long format.
		*/
		long ToLong () const;

		/*!
		Returns the parameter data by converting it to DateTime
		@return parameter data in DateTime format.
		*/
		DateTime ToDateTime () const;

		/*!
		Return the ResultSet for a cursor bound variable
		*/
		ResultSet& ToResultSet ();

		/*!
		Set the parameter value to null
		*/
		inline void ToNull () 
		{ 
			m_indicator = ORADATA_NULL; 
		}

		/*!
		Set the parameter value to given text
		@param[in] text the text to set for parameter value
		@return the reference to current Parameter
		*/
		Parameter& operator = (epl::EpTString text);

		/*!
		Set the parameter value to given double value
		@param[in] value the double value to set for parameter value
		@return the reference to current Parameter
		*/
		Parameter& operator = (double value);

		/*!
		Set the parameter value to given long value
		@param[in] value the long value to set for parameter value
		@return the reference to current Parameter
		*/
		Parameter& operator = (long value);

		/*!
		Set the parameter value to given DateTime value
		@param[in] dateTime the DateTime value to set for parameter value
		@return the reference to current Parameter
		*/
		Parameter& operator = (const DateTime& dateTime);

		/*!
		Return the name of the parameter
		@return the name of the parameter
		*/
		inline epl::EpTString GetName()
		{
			return m_paramName;
		}

		/*!
		Return the type of the parameter
		@return the type of the parameter
		*/
		inline DataTypesEnum GetType()
		{
			return m_paramType;
		}

		/*!
		Return the OCI type of the parameter
		@return the OCI type of the parameter
		*/
		inline unsigned short GetOciType()
		{
			return m_ociType;
		}
		
	private:
		/*!
		Default Constructor

		Attach the parameter to given statement
		*Cannot be created publicly
		*Use Statement::Bind instead
		*When type is set to DT_UNKNOWN type is taken from name's prefix
		@param[in] to the statement object that this parameter will bind to
		@param[in] name the name of the parameter
		@param[in] type the type of the parameter value
		@param[in] fetchSize the size of fetching
		*/
		Parameter (Statement *to, const TCHAR *name, DataTypesEnum type = DT_UNKNOWN, unsigned int fetchSize = FETCH_SIZE);

		/*!
		Default Destructor
		
		*Cannot be deleted publicly
		*/
		virtual ~Parameter ();

		/*!
		Default Copy Constructor

		*Class cannot be copied
		@param var the Parameter object to copy
		*/
		Parameter ( const Parameter&  var ) :SmartObject(var)
		{ 
			/* could not be copy-constructed */ 
		};

		/*!
		Copy Operator

		*Class cannot be copied
		@param var the Parameter object to copy
		*/
		Parameter& operator = (const Parameter& var ) 
		{ 
			return (*this); /* could not be copy-constructed */ 
		};

		/*!
		Initialize member variables
		*/
		void initialize ();

		/*!
		Release all resources allocated
		*/
		void cleanUp ();

		/*!
		Attach the parameter to given statement
		*When type is set to DT_UNKNOWN type is taken from name's prefix
		@param[in] to the statement object that this parameter will bind to
		@param[in] name the name of the parameter
		@param[in] type the type of the parameter value
		@param[in] fetchSize the size of fetching
		*/
		void attach (Statement *to, const TCHAR *name, DataTypesEnum type = DT_UNKNOWN, unsigned int fetchSize = FETCH_SIZE);

		/*!
		Set-up name, type, oci_type and size, depending on type value
		*when type is set to DT_UNKNOWN type is taken from name's prefix
		@param[in] paramName the name of the parameter
		@param[in] type the type of the parameter
		*/
		void setupType (const TCHAR *paramName, DataTypesEnum type = DT_UNKNOWN);

		/*!
		Bind an input and/or output parameter to the given statement.
		@param[in] to the statement object to bind to
		*/
		void bind (Statement *to);

		/*!
		Bind the Result Set
		@param[in] to the statement, which this object will bind to
		@param[in] fetchSize the number of rows, that will be retrieved on each step
		*/
		void bindResultSet (Statement *to,unsigned int fetchSize = FETCH_SIZE);

		/// The name of the parameter (in the exact case, including leading ':')
		epl::EpTString		m_paramName;
		/// the type of the parameter
		DataTypesEnum	m_paramType;
		/// the Oracle's data type
		unsigned short	m_ociType;
		/// the number of bytes required for fetching
		unsigned short	m_size;	

		/// the flag whether the parameter data is NULL or not
		short m_indicator;
		/// the number of bytes returned (used for text)
		unsigned short m_dataLen;
		/// the buffer for fetching
		char *m_fetchBuffer;

		/// the flag whether the paramet is array or not
		bool m_isArray;

		/// the statement object that this parameter will bind to
		Statement *m_stmt;
		/// the bind handle
		OCIBind	*m_bindHandle;
		/// Oracle statement handle
		OCIStmt	*m_rsHandle;
		/// Result Set handle
		ResultSet *m_resultSet;

	}; // parameter class


}


#endif	// __EP_PARAMETER_H__
