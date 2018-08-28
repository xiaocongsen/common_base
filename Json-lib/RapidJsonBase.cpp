#include "RapidJsonBase.h"

RapidJsonBase::RapidJsonBase(string buff)
{
    m_strData = buff;
}

void RapidJsonBase::InitDocument()
{
    if (m_document.Parse(m_strData.c_str()).HasParseError())
    {
        throw "HasParseError";
    }
    if (!m_document.IsObject())
    {
        throw "IsObject = false";
    }
}

void RapidJsonBase::GetTypeData(void *data, DATATYPE type, const char *Val)
{
    if (!m_document.HasMember(Val))
    {
        throw Val;
    }
    rapidjson::Value &s = m_document[Val];
    switch (type)
    {
    case DATATYPE::eBOOL:
    {
        *(bool *)data = s.GetBool();
        break;
    }
    case DATATYPE::eDOUBLE:
    {
        if (!m_document[Val].IsString())
        {
            throw Val;
        }
        *(Decimal *)data = Decimal(s.GetString());
        break;
    }
    case DATATYPE::eINT:
    {
        if (!m_document[Val].IsNumber())
        {
            throw Val;
        }
        *(int *)data = s.GetInt();
        break;
    }
    case DATATYPE::eCHAR:
    {
        if (!m_document[Val].IsNumber())
        {
            throw Val;
        }
        *(char *)data = s.GetInt();
        break;
    }
    case DATATYPE::eSTRING:
    {
        if (!m_document[Val].IsString())
        {
            throw Val;
        }
        *(string *)data = s.GetString();
        break;
    }
    case DATATYPE::eUINT64:
    {
        if (!m_document[Val].IsNumber())
        {
            throw Val;
        }
        *(uint64_t *)data = s.GetUint64();
        break;
    }
    default:
    {
        Log("error");
        throw Val;
    }
    }
}



bool RapidJsonBase::ParseInt(const char* key, int& val)
{
    if (!m_document.HasMember(key))
    {
        cout << "Not Exist " << key << endl;
        return false;
    }
    rapidjson::Value &s = m_document[key];
    val = s.GetInt();
    return true;
}

bool RapidJsonBase::ParseDecimal(const char* key, Decimal& val)
{
   if (!m_document.HasMember(key))
   {
       cout << "Not Exist " << key << endl;
       return false;
   }
   rapidjson::Value &s = m_document[key];
   if (!m_document[key].IsNumber())
   {
       if (m_document[key].IsString())
       {
           val = Decimal(s.GetString());
       }
       else
       {
           cout << key << " Is not double and string" << endl;
           return false;
       }
   }
   else
   {
       val = Decimal(s.GetDouble());
   }

   return true;
}

bool RapidJsonBase::ParseDouble(const char* key, double& val)
{
    if (!m_document.HasMember(key))
    {
        cout << "Not Exist " << key << endl;
        return false;
    }
    rapidjson::Value &s = m_document[key];
    if (!m_document[key].IsNumber())
    {
        if (!m_document[key].IsString())
        {
            cout << key << " IsString = false" << endl;
            return false;
        }

        cout << key << " IsDouble = false" << endl;
        return false;
    }
    val = s.GetDouble();
    return true;

}

bool RapidJsonBase::ParseBool(const char* key, bool &val)
{
    if (!m_document.HasMember(key))
    {
        cout << "Not Exist " << key << endl;
        return false;
    }
    rapidjson::Value &s = m_document[key];
    val = s.GetBool();
    return true;
}

bool RapidJsonBase::ParseString(const char* key, string& val)
{
    if (!m_document.HasMember(key))
    {
        //cout << "Not Exist " << key << endl;
        return false;
    }
    rapidjson::Value &s = m_document[key];

    val = s.GetString();
    return true;
}

bool RapidJsonBase::ParseArray(const char* key, rapidjson::Value &res)
{
    if (!m_document.HasMember(key))
    {
        //cout << "Not Exist " << key << endl;
        return false;
    }
    rapidjson::Value &r = m_document[key];

    if(!r.IsArray())
    {
        cout << "Not an array" << endl;
        return false;
    }

    res = r;

    return true;
}

bool RapidJsonBase::ParseInt(rapidjson::Value &inVal, const char *key, int64_t &val)
{
    if (!inVal.HasMember(key))
    {
        cout << "Not Exist " << key << endl;
        return false;
    }
    rapidjson::Value &s = inVal[key];
    val = s.GetInt64();
    return true;
}

bool RapidJsonBase::ParseDouble(rapidjson::Value &inVal, const char *key, double &val)
{
    if (!inVal.HasMember(key))
    {
        cout << "Not Exist " << key << endl;
        return false;
    }
    rapidjson::Value &s = inVal[key];
    if (!inVal[key].IsNumber())
    {
        if (!inVal[key].IsString())
        {
            cout << key << " IsString = false" << endl;
            return false;
        }

        cout << key << " IsDouble = false" << endl;
        return false;
    }
    val = s.GetDouble();
    return true;
}

bool RapidJsonBase::ParseDecimal(rapidjson::Value &inVal, const char *key, Decimal &val)
{
    if (!inVal.HasMember(key))
    {
        cout << "Not Exist " << key << endl;
        return false;
    }
    rapidjson::Value &s = inVal[key];
    if (!inVal[key].IsNumber())
    {
        if (inVal[key].IsString())
        {
            val = Decimal(s.GetString());
        }
        else
        {
            cout << key << " Is not double and string" << endl;
            return false;
        }
    }
    else
    {
        val = Decimal(s.GetDouble());
    }

    return true;
}

bool RapidJsonBase::ParseBool(rapidjson::Value &inVal, const char *key, bool &val)
{
    if (!inVal.HasMember(key))
    {
        cout << "Not Exist " << key << endl;
        return false;
    }
    rapidjson::Value &s = inVal[key];
    val = s.GetBool();
    return true;
}

bool RapidJsonBase::ParseString(rapidjson::Value &inVal, const char *key, string &val)
{
    if (!inVal.HasMember(key))
    {
        //cout << "Not Exist " << key << endl;
        return false;
    }
    rapidjson::Value &s = inVal[key];

    val = s.GetString();
    return true;
}

bool RapidJsonBase::ParseArray(rapidjson::Value &inVal, const char *key, rapidjson::Value &res)
{
    if (!inVal.HasMember(key))
    {
        //cout << "Not Exist " << key << endl;
        return false;
    }
    rapidjson::Value &r = inVal[key];

    if(!r.IsArray())
    {
        cout << "Not an array" << endl;
        return false;
    }

    res = r;

    return true;
}
