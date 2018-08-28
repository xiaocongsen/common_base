#ifndef RAPIDJSON_BASE_H
#define RAPIDJSON_BASE_H

#include <iostream>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include "stdafx.h"
using namespace std;

enum DATATYPE
{
    eINT = 1,
    eDOUBLE,
    eBOOL,
    eSTRING,
    eCHAR,
    eUINT64
};


class RapidJsonBase
{
public:
  RapidJsonBase(string buff);
  ~RapidJsonBase() {}
  void InitDocument();
  void GetTypeData(void *data, DATATYPE type, const char *Val);



    bool ParseInt(const char* key, int& val);

    bool ParseDouble(const char* key, double& val);

    bool ParseDecimal(const char* key, Decimal& val);

    bool ParseBool(const char* key, bool& val);

    bool ParseString(const char* key, string& val);

    bool ParseArray(const char* key, rapidjson::Value& res);

    static bool ParseInt(rapidjson::Value& inVal, const char* key, std::int64_t& val);

    static bool ParseDouble(rapidjson::Value& inVal, const char* key, double& val);

    static bool ParseDecimal(rapidjson::Value& inVal, const char* key, Decimal& val);

    static bool ParseBool(rapidjson::Value& inVal, const char* key, bool& val);

    static bool ParseString(rapidjson::Value& inVal, const char* key, string& val);

    static bool ParseArray(rapidjson::Value& inVal, const char* key, rapidjson::Value& res);
private:
  string m_strData;
  rapidjson::Document m_document;
};
#endif //RAPIDJSON_HANDLE_H
