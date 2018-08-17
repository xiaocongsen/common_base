#ifndef RABBITMQINOUT_H
#define RABBITMQINOUT_H

#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <iostream>
#include <stdio.h>

class RabbitMqInOut
{
      public:
        RabbitMqInOut(){};
        RabbitMqInOut(std::string host, int port, std::string username, std::string password, std::string queue_name,std::string exchange_Name,std::string routing_key, std::string vhost);
        ~RabbitMqInOut();
        bool CreateRecvChannel(bool exclusive, u_int16_t message_prefetch_count);
        bool CreateSendChannel();
        void CloseChannel();
        void GetChannelMessage(std::string& Message);
        void GetChannelMessageTimeout(std::string& Message);
        void ReplyAck(bool isAck);
        void SendChannelMessage(std::string message);
        void PurgeThisQueue();
      private:
        int m_nPort;
        std::string m_strHost;
        std::string m_strUserName;
        std::string m_strPassWord;
        std::string m_strQueueName;       //队列名
        std::string m_strExchangeName;    //Exchange名 路由名
        std::string m_strRoutingkey;      //路由的key
        std::string m_strConsumerTag;     //订阅的标签
        std::string m_vhost;
        AmqpClient::Channel::ptr_t m_Channel; //连接通道
        AmqpClient::Envelope::ptr_t envelope; //信封 回消息用
};
#endif //RABBITMQINOUT_H