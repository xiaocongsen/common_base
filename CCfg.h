#ifndef CCFG_H
#define CCFG_H

#include <string.h>
#include <unordered_map>

static const int CfgKeyValueLen = 128;         // 配置文件名值对字符串的最大长度
static const int MaxFullLen = 2048;            // 支持的最大全路径文件名长度
static const int MaxLogBuffLen = 1024 * 128;   // 支持一次写入的最大日志长度
static const int WriteErrTime = 5;             // 连续写日志文件多少次失败后，重新关闭&打开文件
static const int TryTimes = 10;                // 连续尝试打开多少次日志文件失败后，关闭日志功能
static const int MinConnectPort = 2000;        // socket主动建立连接的最小端口号，防止和系统端口号冲突

static const int StrIntLen = 32;
typedef char strInt_t[StrIntLen];   // 整型
// new & delete 宏定义，防止内存操作失败抛出异常
#define NEW(pointer, ClassType)     \
    try                             \
	{                               \
		pointer = new ClassType;    \
	}                               \
	catch(...)                      \
	{                               \
		pointer = NULL;             \
	}

#define DELETE(pointer)             \
    try                             \
	{                               \
		delete pointer;             \
		pointer = NULL;             \
	}                               \
	catch(...)                      \
	{                               \
		pointer = NULL;             \
	}
struct Key2Value
{
	char key[CfgKeyValueLen];
	char value[CfgKeyValueLen];
	Key2Value *pNext;

	Key2Value(const char *k, unsigned int kLen, const char *val, unsigned int vLen) : pNext(NULL)
	{
		if (kLen >= (unsigned int)CfgKeyValueLen)
			kLen = CfgKeyValueLen - 1;
		if (vLen >= (unsigned int)CfgKeyValueLen)
			vLen = CfgKeyValueLen - 1;

		memcpy(key, k, kLen);
		key[kLen] = '\0';

		memcpy(value, val, vLen);
		value[vLen] = '\0';
	}

	~Key2Value()
	{
		key[0] = '\0';
		value[0] = '\0';
		pNext = NULL;
	}
};

// 配置文件格式如下：
/*
配置文件说明、注释信息等等
key = value  // 注释信息
key1 = value
......
*/

class CCfg
{
  public:
	~CCfg();

  public:
	static CCfg *initCfgFile(const char *pCfgFile); 				// 进程启动时先调用此函数初始化配置文件
	static CCfg *reLoadCfg();																// 重新加载更新配置项

	// 获取配置项值
	static const char *getValue(const char *key);
	static int getIntValue(const char *key);
	static const char *getKey(const char *value);
	static const char *getKey(const int value);

  public:
	const char *get(const char *key);
	const char *getKeyByValue(const char *value);
	const char *getKeyByValue(const int value);

	void reLoad();

  public:
	static int createDir(const char *pCfgFile);
	void output(); // only for test
	
  private:
	int createFile(const char *pCfgFile, const char *key2value[], const int len);
	void createKV(const char *pCfgFile, bool isUpdate = false);
	void destroyKV();
	void add(const char *key2value, bool isUpdate = false);
	const char *getItem(const char *value, const bool isValue = true);

  private:
	// 禁止拷贝、赋值
	CCfg();
	CCfg(const CCfg &);
	CCfg &operator=(const CCfg &);
	CCfg(const char *pCfgFile);

  private:
	Key2Value *m_KeyVal;
	char m_fileName[MaxFullLen];
};

#endif // CCFG_H
