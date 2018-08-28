#ifndef RABBITMQINOUT_H
#define RABBITMQINOUT_H

#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <iostream>
#include <stdio.h>
#include "LLog.h"
#include "stdafx.h"

class RabbitMqInOut
{
public:
  RabbitMqInOut(){};
  RabbitMqInOut(ConfigData *cfg);
  ~RabbitMqInOut();
  bool CreateRecvChannel(const std::string &exchange_type, bool exclusive, u_int16_t message_prefetch_count);
  bool CreateSendChannel(const std::string &exchange_type);
  void CloseChannel();
  void MqPopUnblock(std::string &Message, int timeout);
  void ReplyAck(bool isAck);
  void MqPop(std::string &Message);
  void MqPush(std::string message);
  void PurgeThisQueue();

private:
  int m_nPort;
  std::string m_strHost;
  std::string m_strUserName;
  std::string m_strPassWord;
  std::string m_strQueueName;    //队列名
  std::string m_strExchangeName; //Exchange名 路由名
  std::string m_strRoutingkey;   //路由的key
  std::string m_strConsumerTag;  //订阅的标签
  std::string m_vhost;
  AmqpClient::Channel::ptr_t m_Channel; //连接通道
  AmqpClient::Envelope::ptr_t envelope; //信封 回消息用
};
#endif //RABBITMQINOUT_H