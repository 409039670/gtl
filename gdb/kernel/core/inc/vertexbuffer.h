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

class CORE_API VertexBuffer :public DescribableBuffer {
public:
	enum VBF{
		VBF_3FLOAT_XYZ = DescribableBuffer::DBF_3FLOAT,
		VBF_4FLOAT_XYZW = DescribableBuffer::DBF_4FLOAT,
		VBF_5FLOAT_XYZUV = DescribableBuffer::DBF_5FLOAT,
		VBF_6FLOAT_XYZRGB = DescribableBuffer::DBF_6FLOAT,
		VBF_7FLOAT_XYZRGBA = DescribableBuffer::DBF_7FLOAT,
		VBF_8FLOAT_XYZUVRGB = DescribableBuffer::DBF_8FLOAT,
		VBF_9FLOAT_XYZUVRGBA = DescribableBuffer::DBF_9FLOAT,
		VBF_10FLOAT_XYZWUVRGBA = DescribableBuffer::DBF_10FLOAT,

		VBF_3DOUBLE_XYZ = DescribableBuffer::DBF_3DOUBLE,
		VBF_4DOUBLE_XYZW = DescribableBuffer::DBF_4DOUBLE,
		VBF_5DOUBLE_XYZUV = DescribableBuffer::DBF_5DOUBLE,
		VBF_6DOUBLE_XYZRGB = DescribableBuffer::DBF_6DOUBLE,
		VBF_7DOUBLE_XYZRGBA = DescribableBuffer::DBF_7DOUBLE,
		VBF_8DOUBLE_XYZUVRGB = DescribableBuffer::DBF_8DOUBLE,
		VBF_9DOUBLE_XYZUVRGBA = DescribableBuffer::DBF_9DOUBLE,
		VBF_10DOUBLE_XYZWUVRGBA = DescribableBuffer::DBF_10DOUBLE,

		//VBF_8FLOAT_XYZWRGBA=VBF_8FLOAT_XYZUVRGB,
		//VBF_8DOUBLE_XYZWRGBA=VBF_8DOUBLE_XYZUVRGB
	};

public:
	VertexBuffer(VBF t = VBF_3FLOAT_XYZ);
	VertexBuffer(void * d, size_t s, VBF t = VBF_3FLOAT_XYZ, bool b_alloc = false);//���캯�������b_allocΪ�棬���������һ���СΪs���ڴ�飬������d�����ݣ���֮����ָ��d
	void copy(const VertexBuffer & b);//����Buffer���������b��_alloc��ԱΪ�٣��򿽱����Buffer������b����һ���ڴ�飬����Ϊ�ڴ�鿽��
	VertexBuffer(const VertexBuffer & b);//�������캯��
	VertexBuffer operator = (const VertexBuffer & b);//�ȺŸ�ֵ����
	void set(void * d, size_t s, VBF t, bool b_alloc);//���캯�������b_allocΪ�棬���������һ���СΪs���ڴ�飬������d�����ݣ���֮����ָ��d

	void add(double  x, double y, double z);
	void add(double  x, double y, double z, double w);
	void add(double  x, double y, double z, float u, float v);
	void add(double  x, double y, double z, float r, float g, float b);
	void add(double  x, double y, double z, float r, float g, float b, float a);
	void add(double  x, double y, double z, double w, float r, float g, float b, float a);
	void add(double  x, double y, double z, float u, float v, float r, float g, float b, float a);
	void add(double  x, double y, double z, double w, float u, float v, float r, float g, float b, float a);

	inline  bool isFloat();
};
typedef std::shared_ptr<VertexBuffer> VertexBufferSharedPtr;
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
