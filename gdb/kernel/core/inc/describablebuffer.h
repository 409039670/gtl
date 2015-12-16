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
#include "buffer.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace


class CORE_API DescribableBuffer: public Buffer{
public:
	enum DBF{
		DBF_VOID,

		DBF_INT8,
		DBF_INT16,
		DBF_INT32,
		DBF_INT64,

		DBF_UINT8,
		DBF_UINT16,
		DBF_UINT32,
		DBF_UINT64,



		DBF_FLOAT,
		DBF_3FLOAT,
		DBF_4FLOAT,
		DBF_5FLOAT,
		DBF_6FLOAT,
		DBF_7FLOAT,
		DBF_8FLOAT,
		DBF_9FLOAT,
		DBF_10FLOAT,
		DBF_16FLOAT,

		DBF_DOUBLE,
		DBF_3DOUBLE,
		DBF_4DOUBLE,
		DBF_5DOUBLE,
		DBF_6DOUBLE,
		DBF_7DOUBLE,
		DBF_8DOUBLE,
		DBF_9DOUBLE,
		DBF_10DOUBLE,
		DBF_16DOUBLE


	};

protected:
	DBF  _type;
public:
	static int sizeType(DBF t );
	inline int numberElements() {return size()/sizeType(_type);}
	inline DBF getType() const { return _type;}

public:
	DescribableBuffer();
	DescribableBuffer(void * d,size_t s,DBF t = DBF_VOID,bool b_alloc=false);//���캯�������b_allocΪ�棬���������һ���СΪs���ڴ�飬������d�����ݣ���֮����ָ��d
	void copy(const DescribableBuffer & b);//����Buffer���������b��_alloc��ԱΪ�٣��򿽱����Buffer������b����һ���ڴ�飬����Ϊ�ڴ�鿽��
	DescribableBuffer(const DescribableBuffer & b);//�������캯��
	DescribableBuffer operator = (const DescribableBuffer & b);//�ȺŸ�ֵ����
	bool changeType(DBF t);
	void get(std::vector<double> & dv);
	void get(int * pc, double ** pp);//��Ҫ�û��Լ��ͷ�*pp
	void get(std::vector<float> & fv);
	void get(int * pc, float ** pp);//��Ҫ�û��Լ��ͷ�*pp
	void get(std::vector<int> & fv);
	void get(int * pc, int ** pp);//��Ҫ�û��Լ��ͷ�*pp
};
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
