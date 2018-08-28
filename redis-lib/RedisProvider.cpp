#include "RedisProvider.h"
#include "unistd.h"

RedisProvider::RedisProvider(const char *ip, const char *database, const char *passwd, int port) : m_Context(nullptr)
{
    this->m_Ip = ip;
    this->m_DataBase = database;
    this->m_password = passwd;
    this->m_Port = port;
    CreateConnection();
}

bool RedisProvider::CheckConnection()
{
    bool ret = false;

    string pingCmd = "ping";
    string retStr;
    ExeRedisCmd(pingCmd.c_str(), retStr);
    if (retStr == "PONG")
        ret = true;

    return ret;
}

redisReply *RedisProvider::TryExecCmd(const char *Cmd, int tryNum)
{
    int tryCount = 0;
    redisReply *ret = nullptr;
    while (tryCount < tryNum)
    {
        ret = (redisReply *)redisCommand(m_Context, Cmd);
        if (ret != nullptr)
        {
            return ret;
        }

        if (++tryCount == tryNum || (NULL == ret && tryCount == tryNum))
        {
            printf("%d execute command:%s failure\n", __LINE__, Cmd);
            return ret;
        }
        else
        {
            if (!CheckConnection())
            {
                if (!CreateConnection())
                {
                    cout << "reconnect redis fail" << endl;
                    return ret;
                }
            }
            usleep(50);
            cout << "retry cmd:" << Cmd << endl;
        }
    }

    return ret;
}

bool RedisProvider::CreateConnection()
{
    if (this->m_Context != nullptr)
    {
        try
        {
            DestoryConnection();
        }
        catch (exception ex)
        {
            cout << ex.what() << endl;
            return false;
        }
    }

    this->m_Context = redisConnect(m_Ip.c_str(), m_Port);
    if (m_Context->err)
    {
        printf("%d connect redis server failure:%s\n", __LINE__, m_Context->errstr);
        return false;
    }

    std::string cmd;
    if (!m_password.empty())
    {
        cmd = "AUTH ";
        cmd += m_password;

        m_Reply = TryExecCmd(cmd.c_str());
        if (m_Reply == nullptr)
        {
            return false;
        }
        std::cout << "connect to redis:" << m_Reply->str << std::endl;

        freeReplyObject(m_Reply); //free to avoid leaking
        m_Reply = nullptr;
    }
    else
        std::cout << "connect to redis:" << std::endl;

    cmd = "SELECT ";
    cmd += m_DataBase;
    m_Reply = (redisReply *)redisCommand(m_Context, cmd.c_str());
    if (NULL == m_Reply)
    {
        printf("%d execute command:%s failure\n", __LINE__, cmd.c_str());
        return false;
    }
    freeReplyObject(m_Reply); //free to avoid leaking
    m_Reply = nullptr;

    return true;
}

bool RedisProvider::ExeRedisCmd(const char *cmd, std::string &result)
{
    try
    {
        if (NULL == cmd)
        {
            printf("NULL pointer");
            return false;
        }
        m_Reply = TryExecCmd(cmd);
        if (m_Reply == nullptr)
        {
            return false;
        }

        if (m_Reply->type == REDIS_REPLY_NIL)
        {
            result.clear();
            return true;
        }

        //返回执行结果为整型的命令,只有状态为REDIS_REPLY_INTEGER,并且INTEGER是大于0时,才表示这种类型的命令执行成功
        if ((m_Reply->type == REDIS_REPLY_INTEGER) && m_Reply->integer < 0)
        {
            printf("%d execute command:[%s] failure cmd = [%s]\n", __LINE__, m_Reply->str, cmd);
            freeReplyObject(m_Reply); //free to avoid leaking
            m_Reply = nullptr;
            return false;
        }
        if (m_Reply != nullptr && m_Reply->str != nullptr)
            result = m_Reply->str;

        if (result.empty() && m_Reply->elements > 0)
        {
            result = m_Reply->element[0]->str;
        }

        freeReplyObject(m_Reply); //free to avoid leaking
        m_Reply = nullptr;
        return true;
    }
    catch (std::runtime_error &e)
    {
        perror(e.what());
        return false;
    }
}

bool RedisProvider::ExeRedisCmd(const char *cmd)
{
    try
    {
        if (NULL == cmd)
        {
            printf("NULL pointer");
            return false;
        }
        m_Reply = TryExecCmd(cmd);
        if (m_Reply == nullptr)
        {
            return false;
        }

        if (m_Reply->type == REDIS_REPLY_NIL)
        {
            cout << "cmd return nil result" << endl;
            return true;
        }

        //返回执行结果为整型的命令,只有状态为REDIS_REPLY_INTEGER,并且INTEGER是大于0时,才表示这种类型的命令执行成功
        if ((m_Reply->type == REDIS_REPLY_INTEGER) && m_Reply->integer < 0)
        {
            printf("%d execute command:[%s] failure cmd = [%s]\n", __LINE__, m_Reply->str, cmd);
            freeReplyObject(m_Reply); //free to avoid leaking
            m_Reply = nullptr;
            return false;
        }
        freeReplyObject(m_Reply); //free to avoid leaking
        m_Reply = nullptr;
        return true;
    }
    catch (std::runtime_error &e)
    {
        perror(e.what());
        return false;
    }
}

bool RedisProvider::ExeRedisCmd(const char *cmd, int &result)
{
    try
    {
        if (NULL == cmd)
        {
            printf("NULL pointer");
            return false;
        }
        m_Reply = TryExecCmd(cmd);
        if (m_Reply == nullptr)
        {
            return false;
        }

        if (m_Reply->type == REDIS_REPLY_NIL)
        {
            cout << "cmd return nil result" << endl;
            return true;
        }
        //返回执行结果为整型的命令,只有状态为REDIS_REPLY_INTEGER,并且INTEGER是大于0时,才表示这种类型的命令执行成功
        if ((m_Reply->type == REDIS_REPLY_INTEGER) && m_Reply->integer < 0)
        {
            printf("%d execute command:[%s] failure cmd = [%s]\n", __LINE__, m_Reply->str, cmd);
            freeReplyObject(m_Reply); //free to avoid leaking
            m_Reply = nullptr;
            return false;
        }
        result = m_Reply->integer;
        freeReplyObject(m_Reply); //free to avoid leaking
        m_Reply = nullptr;
        return true;
    }
    catch (std::runtime_error &e)
    {
        perror(e.what());
        return false;
    }
}

bool RedisProvider::ExeRedisCmd(const char *cmd, std::vector<std::string> &vreply)
{
    try
    {
        if (NULL == cmd)
        {
            printf("NULL pointer");
            return false;
        }
        m_Reply = TryExecCmd(cmd);
        if (m_Reply == nullptr)
        {
            return false;
        }

        if (m_Reply->type == REDIS_REPLY_NIL)
        {
            cout << "cmd return nil result" << endl;
            return true;
        }

        //返回执行结果为整型的命令,只有状态为REDIS_REPLY_INTEGER,并且INTEGER是大于0时,才表示这种类型的命令执行成功
        if ((m_Reply->type == REDIS_REPLY_INTEGER) && m_Reply->integer < 0)
        {

            printf("%d execute command:[%s] failure cmd = [%s]\n", __LINE__, m_Reply->str, cmd);
            freeReplyObject(m_Reply); //free to avoid leaking
            m_Reply = nullptr;
            return false;
        }

        if (m_Reply->elements > 0)
        {
            for (size_t i = 0; i < m_Reply->elements; i++)
            {
                redisReply *r = (m_Reply->element[i]);

                vreply.push_back(r->str);
                //std::cout<< vreply[i] << std::endl;
            }
        }
        else
        {
            if (m_Reply->str != nullptr)
            {
                vreply.push_back(m_Reply->str);
            }
        }

        freeReplyObject(m_Reply); //free to avoid leaking
        m_Reply = nullptr;
        return true;
    }
    catch (std::runtime_error &e)
    {
        perror(e.what());
        return false;
    }
}

bool RedisProvider::StartTransaction()
{
    string cmd0 = "MULTI";

    return ExeRedisCmd(cmd0.c_str());
}

bool RedisProvider::CommitTransaction()
{
    string cmd3 = "EXEC";

    return ExeRedisCmd(cmd3.c_str());
}

void RedisProvider::DestoryConnection()
{
    //freeReplyObject(m_Reply);
    redisFree(m_Context);
    m_Context = nullptr;
}

bool RedisProvider::ConnectDb(std::string &index)
{
    string cmd = "SELECT " + index;

    return ExeRedisCmd(cmd.c_str());
}

bool RedisProvider::GetAllKeyInDb(vector<string> &keys)
{
    string cmd = "KEYS *";

    if (!ExeRedisCmd(cmd.c_str(), keys))
    {
        return false;
    }

    return true;
}

bool RedisProvider::CheckHashTableExist(string &tablename)
{
    string cmd = "EXISTS " + tablename;

    int num = 0;
    ExeRedisCmd(cmd.c_str(), num);

    return (num > 0);
}

bool RedisProvider::CheckKeyExistsInTable(std::string &table, std::string &key)
{
    string cmd = "HEXISTS " + table + " " + key;

    int num = 0;
    ExeRedisCmd(cmd.c_str(), num);

    return (num > 0);
}

bool RedisProvider::DelTable(string &tablename)
{
    string cmd = "DEL " + tablename;

    return ExeRedisCmd(cmd.c_str());
}

bool RedisProvider::CheckTableExist(std::string &tablename)
{
    string cmd = "EXISTS " + tablename;

    int num = 0;
    ExeRedisCmd(cmd.c_str(), num);

    return num > 0;
}

bool RedisProvider::GetValueFromHashTable(string &tablename, string &key, string &val /*out*/)
{
    string cmd = "HGET " + tablename + " " + key;

    return ExeRedisCmd(cmd.c_str(), val);
}

bool RedisProvider::SetValueInHashTable(string &tablename, string &key, string &newVal)
{
    string cmd = "HMSET " + tablename + " " + key + " " + newVal;

    string ret;
    ExeRedisCmd(cmd.c_str(), ret);

    return (ret == "OK");
}

bool RedisProvider::SetValueInSetTable(string &tablename, string &Val)
{
    string cmd = "SADD " + tablename + " " + Val;

    int num = 0;
    ExeRedisCmd(cmd.c_str(), num);

    return num > 0;
}

bool RedisProvider::SetTableTTL(string &tablename, int timeOut)
{
    string sTimeOut = to_string(timeOut);
    string cmd = "EXPIRE " + tablename + " " + sTimeOut;

    int num = 0;
    ExeRedisCmd(cmd.c_str(), num);

    return num > 0;
}

bool RedisProvider::DelKeyInHashTable(std::string &tablename, std::string &key)
{
    string cmd = "HDEL " + tablename + " " + key;

    return ExeRedisCmd(cmd.c_str());
}

bool RedisProvider::CheckKeyExistsInZset(string &table, string &key)
{
    string cmd = "ZRANGEBYSCORE " + table + " " + key + " " + key;

    vector<string> res;
    bool ret = ExeRedisCmd(cmd.c_str(), res);
    if (ret)
    {
        return res.size() > 0;
    }
    else
        return ret;
}

bool RedisProvider::GetValueFromZset(string &tablename, string &key, string &val)
{
    string cmd = "ZRANGEBYSCORE " + tablename + " " + key + " " + key;

    //cout << "cmd:" << cmd << endl;

    vector<string> res;
    bool ret = ExeRedisCmd(cmd.c_str(), res);
    if (ret)
    {
        if (!res.empty())
        {
            val = res[0];
        }
    }
    return ret;
}

bool RedisProvider::SetValueInZset(string &tablename, string &key, string &newVal)
{
    StartTransaction();
    bool ret = UpdateSortedSetByScore(tablename, key, newVal);
    CommitTransaction();

    return ret;
}

bool RedisProvider::DelKeyInZset(string &tablename, string &key)
{
    string cmd1 = "ZREMRANGEBYSCORE " + tablename + " " + key + " " + key;

    return ExeRedisCmd(cmd1.c_str());
}

bool RedisProvider::LPop(string table)
{
    string cmd = "LPOP " + table;
    return ExeRedisCmd(cmd.c_str());
}

bool RedisProvider::PublishMsg(std::string &topic,const std::string &msg)
{
    string cmd = "PUBLISH " + topic + " " + msg;

    // cout << cmd << endl;

    return ExeRedisCmd(cmd.c_str());
}

bool RedisProvider::UpdateSortedSetByScore(string &table, string &score, string &newInfo)
{
    bool ret = true;

    string cmd1 = "ZREMRANGEBYSCORE " + table + " " + score + " " + score;

    ret = ret && ExeRedisCmd(cmd1.c_str());

    string cmd2 = "ZADD " + table + " " + score + " " + newInfo;

    // cout << cmd2 << endl;

    ret = ret && ExeRedisCmd(cmd2.c_str());

    return ret;
}

bool RedisProvider::GetValueFromZsetByIndex(string &table, int index, string &res)
{
    string sindex = to_string(index);
    string cmd = "ZRANGE " + table + " " + sindex + " " + sindex;

    return ExeRedisCmd(cmd.c_str(), res);
}

RedisProvider::~RedisProvider()
{
    DestoryConnection();
}
