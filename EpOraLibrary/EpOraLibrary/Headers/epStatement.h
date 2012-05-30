#ifndef	__EP_STATEMENT_H__
#define	__EP_STATEMENT_H__

#include "epOraLib.h"
#include <vector>
#include <map>
#include "oci.h"
namespace epol {
	
	class ResultSet;
	class Connection;
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


	class Statement
	{
		// friends
		friend class Parameter;
		friend class Connection;

	public:
		// binds a named variable to the statement
		// when type is set to DT_UNKNOWN type is taken from name's prefix
		Parameter &Bind (const char *name, DataTypesEnum dateType = DT_UNKNOWN);

		// executes a prepared statement with no output parameters
		inline void Execute () 
		{
			executePrepared (); 
		}

		// executes a prepared select sql statement and returns the result set
		ResultSet *Select();

		// releases statement
		inline void Release ()
		{
			delete this; 
		}

		// returns a bound parameter by name or index
		Parameter& operator [] (const char *paramName);
		Parameter& operator [] (unsigned short paramIndex);


	private:
		// public not creatable; use connection.execute, .prepare or .select
		// prepares an sql statement for execution
		Statement (Connection &useConnection, const char *sqlStmt, int sqlStmtLen = -1);

		// public not deletable; use release method instead
		~Statement ();

		// private copy-constructor and assignment operator - class could not be copied
		Statement (const Statement& /* st */) 
		{
			/* could not be copy-constructed */ 
		};
		Statement& operator = (const Statement& /* st */) 
		{ 
			return (*this); /* could not be copy-constructed */ 
		};

		// initialize private data
		void initialize ();

		// free resources allocated
		void cleanUp ();

		// prepares an sql statement for execution
		void prepare (const char *sqlStmt, int sqlLen = -1);

		// executes already prepared statement
		void executePrepared ();

		Connection		*m_conn;			// being used
		OCIStmt			*m_stmtHandle;
		std::string		m_sqlStmt;			// being executed
		StatementTypesEnum	m_stmtType;		// of the statement

		typedef std::vector <Parameter *> Parameters;
		typedef std::map <std::string, Parameter *> ParametersMap;
		Parameters		m_parameters;		// an array with bound parameters
		ParametersMap	m_parametersMap;	// a map with parameters against their names

		bool			m_isPrepared;
		bool			m_isExecuted;


	};


};


#endif	// __EP_STATEMENT_H__
