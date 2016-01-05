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
#include "describablebuffer.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

class CORE_API IndexBuffer :public DescribableBuffer {
public:
	IndexBuffer();
	IndexBuffer(unsigned int * pi, size_t number, bool b_alloc = false);//���캯�������b_allocΪ�棬���������һ���СΪs���ڴ�飬������d�����ݣ���֮����ָ��d
	void copy(const IndexBuffer & b);//����Buffer���������b��_alloc��ԱΪ�٣��򿽱����Buffer������b����һ���ڴ�飬����Ϊ�ڴ�鿽��
	IndexBuffer(const IndexBuffer & b);//�������캯��
	IndexBuffer operator = (const IndexBuffer & b);//�ȺŸ�ֵ����

	void add(unsigned int i);
	void add(unsigned int i, unsigned int j);
	void add(unsigned int i, unsigned int j, unsigned int k);
	void add(unsigned int i, unsigned int j, unsigned int k, unsigned int m);

	void add(const long * data, const long size);

};


typedef std::shared_ptr<IndexBuffer> IndexBufferSharedPtr;


end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
