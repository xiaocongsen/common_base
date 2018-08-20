#ifndef STDAFX_H
#define STDAFX_H

#include <cmath>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "decimal.h"
#include "LLog.h"
#include "HttpLog.h"

#define MAXNUM 37
#define Max(a, b) (((a) > (b)) ? (a) : (b))
#define Min(a, b) (((a) > (b)) ? (b) : (a))
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

// 位数组定义
#ifndef CHAR_BIT
#define CHAR_BIT (8)
#endif

#define BIT_MASK(idx)     (1 << ((idx) % CHAR_BIT))
#define BIT_IDX(idx)      ((idx) / CHAR_BIT)
#define BIT_LEN(bn)       (((bn) + (CHAR_BIT - 1)) / CHAR_BIT)
#define BIT_SET(ba, idx)  ((ba)[BIT_IDX(idx)] |= BIT_MASK(idx))     //位设置
#define BIT_CLE(ba, idx)  ((ba)[BIT_IDX(idx)] &= (~BIT_MASK(idx)))  //位清除
#define BIT_TST(ba, idx)  ((ba)[BIT_IDX(idx)] & BIT_MASK(idx))

// using namespace dec;
#define Decimal dec::decimal<8,dec::floor_round_policy>

#endif
