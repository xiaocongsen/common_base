#include "RabbitMqInOut.h"

RabbitMqInOut::RabbitMqInOut(ConfigData *cfg)
{
    m_nPort = cfg->fport;
    m_strHost = cfg->fip;
    m_strUserName = cfg->fuser_name;
    m_strPassWord = cfg->fpass_word;
    m_strQueueName = cfg->fqueue_name;
    m_strExchangeName = cfg->fexchange_name;
    m_strRoutingkey = cfg->frouting_key;
    m_vhost = cfg->fvirtual;
}

bool RabbitMqInOut::CreateRecvChannel(const std::string &exchange_type, bool exclusive, u_int16_t message_prefetch_count)
{
    try
    {
        if (m_Channel == nullptr)
        {
            m_Channel = AmqpClient::Channel::Create(m_strHost, m_nPort, m_strUserName, m_strPassWord, m_vhost); //创建到服务器的连接
            m_Channel->DeclareExchange(m_strExchangeName, exchange_type, false, true);
            m_Channel->DeclareQueue(m_strQueueName, false, true, false, false);
            m_Channel->BindQueue(m_strQueueName, m_strExchangeName, m_strRoutingkey);
            m_strConsumerTag = m_Channel->BasicConsume(m_strQueueName, "", true, false, exclusive, message_prefetch_count);
        }
        return true;
    }
    catch (std::runtime_error &e)
    {
        perror("CreateRecvChannel error");
        BACKTRACE("---------------------------------\n");
        return false;
    }
}

bool RabbitMqInOut::CreateSendChannel(const std::string &exchange_type)
{
    try
    {
        if (m_Channel == nullptr)
        {
            m_Channel = AmqpClient::Channel::Create(m_strHost, m_nPort, m_strUserName, m_strPassWord, m_vhost); //创建到服务器的连接
            m_Channel->DeclareExchange(m_strExchangeName, exchange_type, false, true);
            m_Channel->DeclareQueue(m_strQueueName, false, true, false, false);
            m_Channel->BindQueue(m_strQueueName, m_strExchangeName, m_strRoutingkey);
        }
    }
    catch (std::runtime_error &e)
    {
        perror("CreateSendChannel error");
        BACKTRACE("---------------------------------\n");
        return false;
    }
    return true;
}

void RabbitMqInOut::PurgeThisQueue() //删除队列数据
{
    m_Channel->PurgeQueue(m_strQueueName);
}

void RabbitMqInOut::CloseChannel()
{
    try
    {
        if (m_Channel.get() && !m_strConsumerTag.empty())
        {
            m_Channel->BasicCancel(m_strConsumerTag);
        }
    }
    catch (std::runtime_error &e)
    {
        perror("CloseChannel error");
        return;
    }
}

RabbitMqInOut::~RabbitMqInOut()
{
    CloseChannel();
}

void RabbitMqInOut::MqPopUnblock(std::string &Message, int timeout)
{
    try
    {
        if (m_Channel->BasicConsumeMessage(m_strConsumerTag, envelope, timeout))
        {
            Message = envelope->Message()->Body();
        }
        else
        {
            Message.clear();
        }
    }
    catch (std::runtime_error &e)
    {
        Message.clear();
        perror("GetChannelMessageTimeout error");
        exit(0);
    }
}

void RabbitMqInOut::MqPop(std::string &Message)
{
    try
    {
        envelope = m_Channel->BasicConsumeMessage(m_strConsumerTag);
        Message = envelope->Message()->Body();
    }
    catch (std::runtime_error &e)
    {
        perror("GetChannelMessage error");
        Message.clear();
        exit(0);
    }
}

void RabbitMqInOut::MqPush(std::string message)
{
    try
    {
        m_Channel->BasicPublish(m_strExchangeName, m_strRoutingkey, AmqpClient::BasicMessage::Create(message)); //第一个是exchange名称，第二个参数是routing_key（此处可理解为消息会被送往的队列）。
    }
    catch (std::runtime_error &e)
    {
        perror("run error");
        return;
    }
}

void RabbitMqInOut::ReplyAck(bool isAck)
{
    try
    {
        if (isAck)
            m_Channel->BasicAck(envelope);
        else
            m_Channel->BasicReject(envelope, true, true);
    }
    catch (std::runtime_error &e)
    {
        perror("run error");
        return;
    }
}
