#ifndef	__EP_CONNECTION_H__
#define	__EP_CONNECTION_H__

#include "epOraLib.h"
#include "oci.h"
namespace epol 
{
	class Statement;
	class ResultSet;

	class EP_ORACLELIB Connection
	{
		// friends
		friend class Statement;
		friend class Parameter;
		friend class ResultSet;
		friend class Column;


	public:
		Connection ();

		// create an instance and open the connection
		Connection (const TCHAR *serviceName, const TCHAR *loginID,const TCHAR *password, unsigned long envMode = OCI_OBJECT, bool nonBlockingMode = false);

		~Connection ();

		// connects to an Oracle server
		void Open (const TCHAR *serviceName,const TCHAR *loginID,const TCHAR *password,unsigned long envMode = OCI_OBJECT, bool nonBlockingMode = false);

		// closes the connection
		void Close ();

		// executes a sql statement with no result
		void Execute (const TCHAR *sqlStmt);

		// prepares (and returns) a sql statement for execution
		Statement *Prepare (const TCHAR *sqlStmt);

		// executes a select sql statement and return the result set
		ResultSet *Select (const TCHAR *selectStmt);

		// commits changes
		inline void Commit ()
		{ 
			Execute (_T("commit")); 
		};

		// rollbacks changes
		inline void rollback ()
		{ 
			Execute (_T("rollback")); 
		};


	private:
		// private copy-constructor and assignment operator - class could not be copied
		inline Connection (const Connection& /* cn */) { /* could not be copy-constructed */ };
		inline Connection& operator = (const Connection& /* cn */) 
		{
			return (*this); /* could not be copy-constructed */ 
		};

		// initialize private data
		void initialize ();

		// free resources allocated
		inline void cleanUp () 
		{
			Close (); 
		};


		OCIEnv		*m_envHandle;
		OCIServer	*m_serverHandle;
		mutable OCIError	*m_errorHandle;	// because it could be changed by most oracle APIs
		OCISession	*m_sessionHandle;
		OCISvcCtx	*m_svcContextHandle;

		bool		m_isOpened;
		bool		m_isAvailable;			// (used for connection pooling)?
		bool		m_isBlocking;			// mode (a call could return OCI_STILL_EXECUTING)


	}; // connection class


}; // oralib namespace


#endif	// __EP_CONNECTION_H__
