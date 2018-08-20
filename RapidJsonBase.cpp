#include "RapidJsonBase.h"

RapidJsonBase::RapidJsonBase(std::string buff)
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
        *(std::string *)data = s.GetString();
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
