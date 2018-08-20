#ifndef ME_REDIS_H
#define ME_REDIS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <hiredis/hiredis.h>
#include <iostream>
#include <vector>

using namespace std;
class MeRedis
{
public:
  MeRedis(const char *host, const char *database, const char *passwd, int port);
  ~MeRedis();
  bool CheckConnection();
  bool CreateConnection();
  void DestoryConnection();

  redisReply *TryExecCmd(const char *Cmd, int retryNum = 3);
  bool ExeRedisCmd(const char *Cmd);
  bool ExeRedisCmd(const char *Cmd, std::string &result);
  bool ExeRedisCmd(const char *Cmd, int &result);
  bool ExeRedisCmd(const char *cmd, std::vector<std::string> &vreply);

  bool StartTransaction();

  bool CommitTransaction();

  bool ConnectDb(std::string &index);

  bool GetAllKeyInDb(vector<string> &keys);

  ////////////////////////////////////////////////////////////////////////////////
  /* Hash Table Methods*/
  bool CheckHashTableExist(std::string &tablename);

  bool CheckKeyExistsInTable(std::string &table, std::string &key);

  bool DelTable(std::string &tablename);

  bool CheckTableExist(std::string &tablename);

  //val is the output
  bool GetValueFromHashTable(std::string &tablename /*in*/, std::string &key /*in*/, string &val /*out*/);

  bool SetValueInHashTable(std::string &tablename, std::string &key, std::string &newVal);

  bool DelKeyInHashTable(std::string &tablename, std::string &key);

  /////////////////////////////////////////////////////////////////////////////////

  /* ZSet Table Methods*/

  bool CheckKeyExistsInZset(std::string &table, std::string &key);

  //val is the output
  bool GetValueFromZset(std::string &tablename /*in*/, std::string &key /*in*/, string &val /*out*/);

  bool SetValueInZset(std::string &tablename, std::string &key, std::string &newVal);

  bool DelKeyInZset(std::string &tablename, std::string &key);

  bool LPop(string table);

  bool PublishMsg(std::string &topic, std::string &msg);

  bool UpdateSortedSetByScore(string &table, string &score, string &newInfo);

  bool GetValueFromZsetByIndex(string &table, int index, string &res);

public:
  std::string m_DataBase;
  std::string m_password;
  std::string m_Ip;
  int m_Port;

  redisContext *m_Context;
  redisReply *m_Reply;
};

#endif //ME_REDIS_H
