#ifndef MEMYSQL_H
#define MEMYSQL_H

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/exception.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>

using namespace sql;
class MeMysql
{
  public:
	MeMysql(){};
	MeMysql(const char *host, const char *user, const char *passwd, const char *dataDase)
	{
		m_Url = host;
		m_User = user;
		m_Password = passwd;
		m_DataBase = dataDase;
	}
	~MeMysql();
	bool InitMysqlData();
	void ExeCommit() { m_Conn->commit(); }
	void AllRollback() { m_Conn->rollback(); }
	void RollBack(Savepoint *savepoint)
	{
		m_Conn->rollback(savepoint);
		m_Conn->releaseSavepoint(savepoint);
	}
	// void AutoCommit(bool flag) { m_Conn->setAutoCommit(flag); }
	ResultSet *ExecuteStoredProcedure(char *exeCmd, char *resCmd = NULL);
	ResultSet *PerformSelectCmd(char *Cmd);
	int PerformUpdateCmd(char *Cmd);
	int PerformInstallCmd(char *Cmd);

	std::string getColumnName(int index);
	// void ExecuteUpdate(const SQLString &sql);

	// bool IsValueNull(int columnindex)
	// {
	// 	return m_Res->isNull(columnindex) ? true : false;
	// }

	// bool IsValueNull(const std::string &columnname)
	// {
	// 	return m_Res->isNull(columnname) ? true : false;
	// }

	// bool getNext()
	// {
	// 	return m_Res->next();
	// }

	// std::string getValueString(const char *columnname)
	// {
	// 	return m_Res->getString(columnname);
	// }

	// int getValueInt(const char *columnname)
	// {
	// 	int value = m_Res->getInt(columnname);
	// 	return value;
	// }
	
	int GetColumnCount();
	// int GetRowCount()	//获得数据的条数
	// {
	// 	return m_Res->rowsCount();
	// }

  private:
	bool IsAliveConn();
	void DestoryConnection();

  private:
	std::string m_User;
	std::string m_Password;
	std::string m_Url;
	std::string m_DataBase;
	Driver *m_Driver; //sql driver (the sql will free it) sql驱动程序(sql将释放它)
	Connection *m_Conn;
	Statement *m_State;
	// ResultSet *m_Res;
	// ResultSetMetaData *m_RsMeta;

	// sql::PreparedStatement *prep_stmt;
	// Savepoint *m_Savept;
};

#endif //MEMYSQL_H
