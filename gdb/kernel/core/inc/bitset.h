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
#include <memory>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <set>
#include "time.h"
#include "config.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

/** @defgroup Bitset  Bitsetλ����
*  @{
*/
class GV3DCORE_API Bitset{
	std::vector<unsigned char> _data;//���λ����[0]λ�ã�ÿλ��Ĭ��ֵΪ0
public:
	//�����Ѿ����õ�λ��
	size_t count();

	bool get(size_t pos);
	//����ָ��λ��Ϊ0��1��true��ʾ1��false��ʾ0�����pos�������鳤�ȣ����Զ���չ
	void set(size_t pos, bool b = true);
	//��λ����ת�������������λ����[0]λ��
	//���������д�ŵ�1011���򷵻�11�������Ƿ���13
	unsigned long long to_ullong();
	//����ָ��λ�õ�ֵ�����pos����λ���鳤�ȣ��Զ���չ
	bool operator [] (const size_t pos);
	//ֱ������ת���ɶ����ƣ���ֵ��Bitset�����λ����[0]λ��
	Bitset operator = (unsigned long long ull);
	//Ĭ�Ϲ���
	Bitset();
	//ֱ������ת���ɶ����ƣ���ֵ��Bitset�����λ����[0]λ��
	Bitset(unsigned long long n);
	//�������캯��
	Bitset(const Bitset & b);
	//����λ���λ���࣬�������ŵ���1011���򱾺���ִ�к�Ϊ1101
	void reverse();
	//�������nλ�������µ�Bitset
	Bitset left(size_t n);
	//�����ұ�nλ�������µ�Bitset
	Bitset right(size_t n);

	//��������Bitset�Ƿ����
	bool operator == (const Bitset & b);
	//�ж�b������λ�����Ƿ��Ǳ����λ�����ǰ׺
	bool has_prefix(const Bitset & b);
	//�жϱ����λ�����Ƿ���b��ǰ׺
	bool is_prefix(unsigned long long & b);
	//
	void clear();

	void resize(size_t newSize);

	void set(const unsigned char * flags, unsigned long long s);
};
/** @} */
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
