/*
* Geosciences Template Library
*
* Copyright (c) 2008
* Zhenwen He (zwhe@hotmail.com)
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Zhenwen He  makes no  representations
* about the suitability of this software for any purpose.
* It is provided "as is" without express or implied warranty.
*/
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>
#include <cmath>
#include "buffer.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace


/** @defgroup value ValUtils
*  @{
*/
class GV3DCORE_API ValUtils{
public:

	enum GVT{
		GVT_UNKN = 0,   //δ֪����

		GVT_BEGIN_CHAR,
		GVT_CHAR,   //�ַ�����
		GVT_WCHAR,   //�ַ�����
		GVT_END_CHAR,

		GVT_BEGIN_NUMBER,//��ʼ���������͵Ķ���

		GVT_BEGIN_INTEGER,
		GVT_UCHAR,   //�ֽ���������
		GVT_SHORT,  //2���ֽ�����
		GVT_USHORT,
		GVT_INT,//4���ֽ�����
		GVT_UINT,
		GVT_LONG,
		GVT_ULONG,
		GVT_ULONGLONG,
		GVT_END_INTEGER,

		GVT_BEGIN_DECIMAL,
		GVT_FLOAT,  //4���ֽڸ�����
		GVT_DOUBLE,//8���ֽڸ�����
		GVT_END_DECIMAL,

		GVT_END_NUMBER,//��ʼ���������͵Ķ���

		GVT_MAX   //�������ֵ
	};

	struct VALUE{
		GVT       type;//ֵ����
		int       count;//ֵ�ĸ���
		void *    pvalue;//ֵ�ڴ��
	};

protected://��������ת��
	static bool _numb_to_numb(VALUE & g, GVT pt);
	static bool _numb_to_char(VALUE & g, GVT pt);
	static bool _char_to_char(VALUE & g, GVT pt);
	static bool _char_to_numb(VALUE & g, GVT pt);
public:
	static void write(Buffer & bs, VALUE & g);
	static void read(Buffer & bs, VALUE & g);
	static bool changeType(VALUE & g, GVT pt);
	static void initial(VALUE & v);
	static void clear(VALUE & g);
	static bool isDecimal(GVT pt);
	static bool isInteger(GVT pt);
	static bool isNumber(GVT pt);
	static bool isChar(GVT pt);
	static bool isDecimal(VALUE & g);
	static bool isInteger(VALUE & g);
	static bool isNumber(VALUE & g);
	static bool isChar(VALUE & g);
	static bool isArray(VALUE & g);
	static int getTypeSize(GVT t);
	static double toNumber(VALUE & g);
	static int toNumber(VALUE & g, double ** da, int * n);
	static std::string toString(VALUE & g);
	static std::wstring toWString(VALUE & g);
	static int compare(VALUE &  v1, VALUE &v2);
	static int getSize(const VALUE & v);
	static void copy(VALUE * g, const VALUE & v);

	static void initialChar(VALUE * pv, char * cc, int n = 1);
	static void initialChar(VALUE * pv, char  cc);
	static void initialUChar(VALUE * pv, unsigned char * cc, int n = 1);
	static void initialUChar(VALUE * pv, unsigned char  cc);
	static void initialWChar(VALUE * pv, wchar_t * cc, int n = 1);
	static void initialWChar(VALUE * pv, wchar_t  cc);
	static void initialShort(VALUE * pv, short * cc, int n = 1);
	static void initialShort(VALUE * pv, short  cc);
	static void initialUShort(VALUE * pv, unsigned short * cc, int n = 1);
	static void initialUShort(VALUE * pv, unsigned short  cc);
	static void initialInt(VALUE * pv, int * cc, int n = 1);
	static void initialInt(VALUE * pv, int  cc);
	static void initialUInt(VALUE * pv, unsigned int * cc, int n = 1);
	static void initialUInt(VALUE * pv, unsigned int  cc);
	static void initialLong(VALUE * pv, long * cc, int n = 1);
	static void initialLong(VALUE * pv, long  cc);
	static void initialULong(VALUE * pv, unsigned long * cc, int n = 1);
	static void initialULong(VALUE * pv, unsigned long cc);
	static void initialULongLong(VALUE * pv, unsigned long long c);
	static void initialFloat(VALUE * pv, float * cc, int n = 1);
	static void initialFloat(VALUE * pv, float  cc);
	static void initialDouble(VALUE * pv, double * cc, int n = 1);
	static void initialDouble(VALUE * pv, double  cc);

	static void stringToWString(std::string & s, std::wstring & sd);
	//ֻ�д���ͬһ���뷽ʽ������²�����ȷ�ģ����������ϵ��ͬ���򱾺���������ȷ��ִ��
	static void wstringToString(std::wstring & s, std::string & sd);

	static bool doubleEqual(double d1, double d2);
	static bool floatEqual(float d1, float d2);

};
/** @} */

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
