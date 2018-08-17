#include "RabbitMqInOut.h"

RabbitMqInOut::RabbitMqInOut(std::string host, int port, std::string username, std::string password, std::string queue_name,std::string exchange_Name,std::string routing_key,std::string vhost)
{
    m_nPort = port;
    m_strHost = host;
    m_strUserName = username;
    m_strPassWord = password;
    m_strQueueName = queue_name;
    m_strExchangeName = exchange_Name;
    m_strRoutingkey = routing_key;
    m_vhost = vhost;
}

bool RabbitMqInOut::CreateRecvChannel(bool exclusive, u_int16_t message_prefetch_count)
{
    try
    {
        m_Channel = AmqpClient::Channel::Create(m_strHost, m_nPort, m_strUserName, m_strPassWord,m_vhost); //创建到服务器的连接
        m_Channel->DeclareExchange(m_strExchangeName, AmqpClient::Channel::EXCHANGE_TYPE_TOPIC, false, true);
        m_Channel->DeclareQueue(m_strQueueName, false, true, false, false);
        m_Channel->BindQueue(m_strQueueName, m_strExchangeName, m_strRoutingkey);
        m_strConsumerTag = m_Channel->BasicConsume(m_strQueueName, "", true, false, exclusive,message_prefetch_count);
        return true;
    }
    catch (std::runtime_error &e)
    {
        perror("CreateRecvChannel error");
        return false;
    }
}

bool RabbitMqInOut::CreateSendChannel()
{
    try
    {
        m_Channel = AmqpClient::Channel::Create(m_strHost, m_nPort, m_strUserName, m_strPassWord,m_vhost); //创建到服务器的连接
        m_Channel->DeclareExchange(m_strExchangeName, AmqpClient::Channel::EXCHANGE_TYPE_TOPIC, false, true);
        m_Channel->DeclareQueue(m_strQueueName, false, true, false, false);
        m_Channel->BindQueue(m_strQueueName, m_strExchangeName, m_strRoutingkey);
    }
    catch (std::runtime_error &e)
    {
        perror("CreateSendChannel error");
        return false;
    }
    return true;
}

void RabbitMqInOut::PurgeThisQueue()
{
    m_Channel->PurgeQueue(m_strQueueName);
    sleep(2);
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

void RabbitMqInOut::GetChannelMessageTimeout(std::string& Message)
{
    try
    {
        if(m_Channel->BasicConsumeMessage(m_strConsumerTag,envelope,2)) 
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

void RabbitMqInOut::GetChannelMessage(std::string& Message)
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

void RabbitMqInOut::SendChannelMessage(std::string message)
{
    try
    {
        m_Channel->BasicPublish(m_strExchangeName,m_strRoutingkey, AmqpClient::BasicMessage::Create(message)); //第一个是exchange名称，第二个参数是routing_key（此处可理解为消息会被送往的队列）。
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
        if(isAck)
        m_Channel->BasicAck(envelope);
        else
        m_Channel->BasicReject(envelope,true,true);
    }
    catch (std::runtime_error &e)
    {
        perror("run error");
        return;
    }
}
