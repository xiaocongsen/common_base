#include "MeMysql.h"

bool MeMysql::InitMysqlData()
{
    try
    {
        m_Driver = sql::mysql::get_driver_instance();
        if(m_Driver == NULL)
        {
            std::cout << "get driver err\n";
            return false;
        }
        m_Conn = m_Driver->connect(m_Url, m_User, m_Password); //create a conn
        if (!m_Conn)
        {
            return false;
        }
        m_State = m_Conn->createStatement();
        if(!m_State)
        {
            return false;
        }
        m_Conn->setAutoCommit(false);
        m_Conn->setSchema(m_DataBase);
        return true;
    }
    catch (sql::SQLException &e)
    {
        // perror("Get sql driver failed");
        std::cout << e.what() << std::endl;
        sleep(2);
        return false;
    }
    catch (std::runtime_error &e)
    {
        perror("Run error");
        sleep(2);
        return false;
    }
}

bool MeMysql::IsAliveConn()
{
    if (!m_Conn->isValid())
    {
        m_Conn->reconnect();
    }
    return m_Conn->isValid();
}

// int MeMysql::GetColumnCount()
// {
// 	if(m_Res == NULL)
// 	{
//     	return -1;
// 	}
// 	m_RsMeta = m_Res->getMetaData(); 
// 	return m_RsMeta->getColumnCount();
// }

// std::string MeMysql::getColumnName(int index)
// {
// 	if(m_RsMeta == NULL)
// 	{
// 		m_RsMeta = m_Res->getMetaData();
// 	}
// 	return m_RsMeta->getColumnLabel(index+1);;
// }

ResultSet *MeMysql::PerformSelectCmd(char *Cmd)
{
    try
    {
        if (!IsAliveConn())
        {
            std::cout << "m_Conn->isValid() = true" << std::endl;
            return NULL;
        }
        if(!m_State)
        {
            m_State = m_Conn->createStatement();
        }
        return m_State->executeQuery(Cmd);
    }
    catch (sql::SQLException &e)
    {
        perror(e.what());
        sleep(2);
        return NULL;
    }
}

int MeMysql::PerformUpdateCmd(char *Cmd)
{
    try
    {
        if (!IsAliveConn())
        {
            std::cout << "m_Conn->isValid() = true" << std::endl;
            return 0;
            // m_Conn->reconnect();
        }
        if(!m_State)
        {
            m_State = m_Conn->createStatement();
        }
        // std::unique_ptr<sql::Statement> stmt(m_Conn->createStatement());
        return m_State->executeUpdate(Cmd);
    }
    catch (sql::SQLException &e)
    {
        perror(e.what());
        sleep(2);
        return 0;
    }
}

int MeMysql::PerformInstallCmd(char *Cmd)
{
    try
    {
        if (!IsAliveConn())
        {
            std::cout << "m_Conn->isValid() = true" << std::endl;
            return 0;
            // m_Conn->reconnect();
        }
        if(!m_State)
        {
            m_State = m_Conn->createStatement();
        }
        return m_State->executeUpdate(Cmd);
    }
    catch (sql::SQLException &e)
    {
        perror(e.what());
        sleep(2);
        return 0;
    }
}

ResultSet *MeMysql::ExecuteStoredProcedure(char *exeCmd, char *resCmd)
{
    try
    {
        if (!IsAliveConn())
        {
            std::cout << "m_Conn->isValid() = true" << std::endl;
            return NULL;
            // m_Conn->reconnect();
        }
        if(!m_State)
        {
            m_State = m_Conn->createStatement();
        }
        m_State->execute(exeCmd);
        if (resCmd)
        {
            return m_State->executeQuery(resCmd); //SELECT @pop AS _reply
        }
        else
        {
            return NULL;
        }
    }
    catch (sql::SQLException &e)
    {
        perror(e.what());
        sleep(2);
        return NULL;
    }
}

void MeMysql::DestoryConnection()
{
    if (m_Conn)
    {
        try
        {
            m_Conn->close();
        }
        catch (sql::SQLException &e)
        {
            perror(e.what());
        }
        catch (std::exception &e)
        {
            perror(e.what());
        }
        delete m_Conn;
    }
}

MeMysql::~MeMysql()
{
    DestoryConnection();
    // if(m_Res)
	// {
	// 	delete m_Res;
	// 	m_Res = NULL;
	// }
	if(m_State)
	{
		delete m_State;
		m_State = NULL;
	}
	if(m_Conn)
	{
		m_Conn->close();
		delete m_Conn;
		m_Conn = NULL;
	}
}

//execute insert,update,delete sql
// void MeMysql::ExecuteUpdate(const SQLString& sql)
// {
// 	m_Savept = m_Conn->setSavepoint("savept");
// 	try{
// 		m_Conn->setTransactionIsolation(TRANSACTION_SERIALIZABLE); //when change the data,we can't read the data
// 		prep_stmt = m_Conn->prepareStatement(sql);	
// 		int updatecount = prep_stmt->executeUpdate();
// 		if(updatecount == 0)
// 		{
// 			printf("no rows update\n");
// 			return;
// 		}
// 		ExeCommit();
// 	}
//     catch(SQLException &e)
// 	{
// 		std::cout << "ERROR: SQLException in " << __FILE__;
// 		std::cout << " (" << __func__<< ") on line " << __LINE__ << std::endl;
// 		std::cout << "ERROR: " << e.what();
// 		std::cout << " (MySQL error code: " << e.getErrorCode();
// 		std::cout << ", SQLState: " << e.getSQLState() << ")" << std::endl;
// 		RollBack(m_Savept);
//         sleep(2);
// 	}	
// }
