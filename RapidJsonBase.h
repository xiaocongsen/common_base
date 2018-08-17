#ifndef RAPIDJSON_BASE_H
#define RAPIDJSON_BASE_H

#include <iostream>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include "stdafx.h"

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
  RapidJsonBase(std::string buff);
  ~RapidJsonBase() {}
  void InitDocument();
  void GetTypeData(void *data, DATATYPE type, const char *Val);

private:
  std::string m_strData;
  rapidjson::Document m_document;
};
#endif //RAPIDJSON_HANDLE_H