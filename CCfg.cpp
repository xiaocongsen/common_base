
/* author : limingfan
 * date : 2014.10.27
 * description : 配置文件解析读取配置项
 */

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include "CCfg.h"

CCfg::CCfg(const char *pCfgFile) : m_KeyVal(NULL)
{
	m_fileName[0] = '\0';

	if (pCfgFile == NULL || *pCfgFile == '\0' || strlen(pCfgFile) >= (unsigned int)MaxFullLen) // 文件名无效
	{
		return;
	}

	if (access(pCfgFile, F_OK | W_OK) != 0) // 不存在则创建
	{
		if (createDir(pCfgFile) == 0) // 创建目录
		{
				const char *key2value[] = {
				"/* 默认配置文件 */\n\n"
				"ThreadCount = 3\n"
				"DBIP = 192.168.1.201\n"
				"DBPort = 3306\n"
				"USER = root\n"
				"PASSWD = Aa123456\n"
				"DataBaseName = beta\n"
				"LogUil = 192.168.1.203:5000/programExceptionLog\n"
			};
			createFile(pCfgFile, key2value, (int)(sizeof(key2value) / sizeof(key2value[0]))); // 创建配置文件
		}
	}

	createKV(pCfgFile); // 创建名值对

	strcpy(m_fileName, pCfgFile);
}

CCfg::~CCfg()
{
	destroyKV();
	m_fileName[0] = '\0';
}

int CCfg::createDir(const char *pCfgFile)
{
	int rc = 0;
	int dirNameSize = 0;
	char dirName[MaxFullLen] = {0};
	const char *pFind = strchr(pCfgFile, '/');
	while (pFind != NULL)
	{
		// 目录名处理
		dirNameSize = pFind - pCfgFile;
		if (dirNameSize > 0)
		{
			memcpy(dirName, pCfgFile, dirNameSize);
			dirName[dirNameSize] = '\0';

			rc = access(dirName, F_OK | W_OK);
			if (rc != 0) // 不存在则创建
			{
				rc = mkdir(dirName, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); // 保持和手动创建的目录权限一致
				if (rc != 0)												// 创建目录失败直接退出
				{
					break;
				}
			}
		}

		pFind = strchr(++pFind, '/'); // 继续查找下一级目录名
	}

	return rc;
}

int CCfg::createFile(const char *pCfgFile, const char *key2value[], const int len)
{
	int rc = -1;
	FILE *pf = fopen(pCfgFile, "w");
	if (pf != NULL)
	{
		rc = 0;
		for (int i = 0; i < len; i++)
		{
			add(key2value[i]);

			if (fputs(key2value[i], pf) == EOF)
			{
				rc = -1;
				break;
			}
		}
		fclose(pf);
	}

	return rc;
}

void CCfg::createKV(const char *pCfgFile, bool isUpdate)
{
	FILE *pf = fopen(pCfgFile, "r");
	if (pf != NULL)
	{
		char keyValue[CfgKeyValueLen] = {0};
		while (fgets(keyValue, CfgKeyValueLen - 1, pf) != NULL)
		{
			add(keyValue, isUpdate);
		}
		fclose(pf);
	}
}

void CCfg::destroyKV()
{
	Key2Value *delKV = NULL;
	while (m_KeyVal != NULL)
	{
		delKV = m_KeyVal;
		m_KeyVal = m_KeyVal->pNext;
		DELETE(delKV);
	}
}

void CCfg::add(const char *key2value, bool isUpdate)
{
	while (*key2value == ' ')
		key2value++; // 先过滤掉前空白符号

	if (*key2value == '/')
		return; // 忽略注释信息

	const char *pFind = strchr(key2value, '=');
	if (pFind == NULL)
		return;

	// Key值
	const char *last = pFind - 1;
	while (*last == ' ')
		last--; // 过滤掉后空白符号
	int keyLen = last - key2value + 1;

	// Value值
	pFind++;
	while (*pFind == ' ')
		pFind++; // 过滤掉前空白符号

	int valueLen = 0;
	last = pFind;
	char cVal = *last;
	while (cVal != '\0')
	{
		if (cVal == ' ' || cVal == ';' || cVal == '\n' || cVal == '\r')
			break; // 判断是否遇到结束符号
		cVal = *(++last);
		valueLen++;
	}

	// Key&Value值
	if (keyLen > 0 && valueLen > 0)
	{
		// 先检查是否是更新的配置项
		Key2Value *pUpKV = m_KeyVal;
		while (isUpdate && pUpKV != NULL)
		{
			if (strlen(pUpKV->key) == (unsigned int)keyLen && memcmp(pUpKV->key, key2value, keyLen) == 0)
			{
				// 更新配置项
				if (valueLen >= CfgKeyValueLen)
					valueLen = CfgKeyValueLen - 1;
				memcpy(pUpKV->value, pFind, valueLen);
				pUpKV->value[valueLen] = '\0';
				return;
			}
			pUpKV = pUpKV->pNext;
		}
		// 新增加的配置项
		Key2Value *kv = new Key2Value(key2value, keyLen, pFind, valueLen);
		kv->pNext = m_KeyVal;
		m_KeyVal = kv;
	}
}

const char *CCfg::getItem(const char *item, const bool isValue)
{
	if (item == NULL || *item == '\0')
		return NULL;

	Key2Value *pFind = m_KeyVal;
	while (pFind != NULL)
	{
		if (isValue)
		{
			if (strcmp(pFind->key, item) == 0)
				return pFind->value;
		}
		else
		{
			if (strcmp(pFind->value, item) == 0)
				return pFind->key;
		}
		pFind = pFind->pNext;
	}
	return NULL;
}

const char *CCfg::get(const char *key)
{
	return getItem(key);
}

const char *CCfg::getKeyByValue(const char *value)
{
	return getItem(value, false);
}

const char *CCfg::getKeyByValue(const int value)
{
	strInt_t strValue = {0};
	snprintf(strValue, StrIntLen - 1, "%d", value);
	return getItem(strValue, false);
}

void CCfg::reLoad()
{
	if (m_fileName[0] != '\0')
	{
		createKV(m_fileName, true); // 重新读取创建配置项
									// output();
	}
}

// only for test
void CCfg::output()
{
	Key2Value *pOutput = m_KeyVal;
	while (pOutput != NULL)
	{
		Log("kv = %p, key = %s, value = %s\n", pOutput, pOutput->key, pOutput->value);
		pOutput = pOutput->pNext;
	}
}

// 进程启动时先调用此函数初始化配置文件
static CCfg *pCfgObj = NULL;
CCfg *CCfg::initCfgFile(const char *pCfgFile)
{
	static CCfg cfgObj(pCfgFile); // 全局唯一
	pCfgObj = &cfgObj;
	return pCfgObj;
}

const char *CCfg::getValue(const char *key)
{
	return pCfgObj->get(key);
}

int CCfg::getIntValue(const char *key)
{
	return atoi(pCfgObj->get(key));
}

const char *CCfg::getKey(const char *value)
{
	return pCfgObj->getKeyByValue( value);
}

const char *CCfg::getKey(const int value)
{
	return pCfgObj->getKeyByValue( value);
}

CCfg *CCfg::reLoadCfg()
{
	pCfgObj->reLoad();
	return pCfgObj;
}
