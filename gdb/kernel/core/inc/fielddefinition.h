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
#include "value.h"

begin_gtl_namespace
begin_gdb_namespace

class FieldDefinition {
protected:
	/** @name ���Գ�Ա����
	*  @{
	*/
	/** ����������,��һ�����ݱ���Ҫ�����Ψһ�� */
	std::string _name;
	/** �����б��� */
	std::string _code;
	/**�����б���*/
	std::string _caption;
	/** �������͵������ַ�����GVT�е���Ч���� */
	GVT         _type;
	/** �������ͳ��ȣ������ֽڽ��м��㣬˫�ֽڼ�Ϊ�����ֽ�,<=0��ʾ����Ҫ���������¼�ĳ���
	bool-     1
	int8      1
	int16     2
	int32     4
	int64     8
	uint8     1
	uint16    2
	uint32    4
	uint64    8
	float32   4          -3.40E+38 ~ +3.40E+38
	float64   8          -1.79E+308 ~ +1.79E+308
	date      4*3=12
	datetime  4*7=28
	char8     1-2GB
	char16    2-2GB
	_lengthֻ��Ϊ������Ӧ���͵�������n����n>1��ʱ�򣬱�ʾΪ�ַ������������͵�����
	*/
	int         _length;
	/** �������͵�С����λ��,ֵΪ������ʱ��Ϊ��Чֵ
	*   ֻ�е�����Ϊfloat32��float64��ʱ�������ã�
	*   float32����Чλ��Ϊ7������_decimal[0-6]
	*   float64����Чλ��Ϊ16������_decimal[0-15]
	*/
	int         _decimal;
	/** ���������Ƿ����Ϊ�գ�1-true��ʾ����Ϊ�գ�0-false��ʾ����Ϊ�� */
	int        _nullable;
	/** Ĭ��ֵ*/
	VALUE    *  _defaultValue;
	/**Display justification for field values,OJUndefined = 0, OJLeft = 1, OJRight = 2*/
	int         _justification;
	/**�Ƿ���ԣ�1-true,0-false,default=0*/
	int         _ignore;
	/** @} */

public:
	/** @name ���캯������������
	*  @{
	*/
	FieldDefinition();
	/**
	����һ���򻯵Ĺ��캯����ֻ��Ҫ�����ֶ����ƣ��ֶ����ͺ͸����������Զ�����length��ֵ
	���磺FieldDefinition��"Field1",GVT_CHAR8,50������50���ַ���ȵ��ı��У�length=50
	FieldDefinition��"Field2",GVT_CHAR16,50������50���ַ���ȵĿ��ı��У�length=100
	FieldDefinition��"Field3",GVT_INT64,1������64BITS�������У�length=8
	FieldDefinition��"Field4",GVT_INT64,10������64BITS�����������У�length=80
	FieldDefinition��"Field5",GVT_DATETIME,1������datetime�У�length=28
	*/
	FieldDefinition(std::string name, GVT type, int count = 1);
	/*
	GVT            NAME      SIZE       C/C++                 RANGE
	GVT_BOOL       bool      1          bool
	GVT_INT8       int8      1          signed char
	GVT_INT16      int16     2          short
	GVT_INT32      int32     4          int
	GVT_INT64      int64     8          long long
	GVT_UINT8      uint8     1          unsigned char
	GVT_UINT16     uint16    2          unsigned short
	GVT_UINT32     uint32    4          unsigned int
	GVT_UINT64     uint64    8          unsigned long long
	GVT_FLOAT32    float32   4          float                 -3.40E+38 ~ +3.40E+38
	GVT_FLOAT64    float64   8          double                -1.79E+308 ~ +1.79E+308
	GVT_DATE       date      4*3=12     int [3]
	GVT_DATETIME   datetime  4*7=28     int [7]
	GVT_CHAR8      char8     1-2GB      char
	GVT_CHAR16     char16    2-2GB      wchar_t
	*/
	FieldDefinition(std::string name, std::string  gvt_typename, int count = 1);
	FieldDefinition(std::string name,
		GVT type, int length, int decimal = -1,
		bool nullable = true, const VALUE * defaultVal = 0,
		std::string code = "", std::string caption = "", int justification = 0, bool ignore = false);
	FieldDefinition(const FieldDefinition & t);
	/** @} */

	/** @name ������ȺŸ�ֵ����
	*  @{
	*/
	void copy(const FieldDefinition & t);
	FieldDefinition operator = (const FieldDefinition & t);
	bool operator==(const FieldDefinition & t);
	/** @} */

	/** @name �����Ա������ȡ�����ò���������
	*  @{
	*/
	inline std::string getName() { return _name; }
	inline  void  setName(const std::string& v) { _name = v; }

	inline std::string getCode() { return _code; }
	inline void   setCode(const std::string& v) { _code = v; }

	inline std::string getCation() { return _caption; }
	inline void   setCaption(const std::string& v) { _caption = v; }

	inline std::string  getTypeName() { return ValUtils::getTypeName(_type); }
	inline void   setType(GVT v) { _type = v; }
	inline int    getType() { return (int)_type; }

	inline int  getLength() { return _length; }
	inline void   setLength(int v) { _length = v; }

	inline int  getDecimal() { return _decimal; }
	inline void   setDecimal(int v) { _decimal = v; }

	inline bool  isNullable() { return _nullable; }
	inline void   setNullable(bool v) { _nullable = v; }

	inline bool setDefaultValue(const VALUE & g) {
		if (_defaultValue) {
			_defaultValue = ValUtils::newValue();
		}
		ValUtils::copy(_defaultValue, g);
	}
	inline VALUE* getDefaultValue() { return _defaultValue; }

	inline bool isIgnore() { return _ignore != 0 ? 1 : 0; }
	inline void setIgnore(bool b = true) { _ignore = b ? 1 : 0; }

	inline int getJustification() { return _justification; }
	inline void setJustification(int v = 0) { _justification = v; }
	/** @} */

public:
	/**  �ж����������Ƿ�Ϊ��������
	*	 @param ��
	*   @return ��������������������ͣ��򷵻�true�����򷵻�false
	*/
	inline bool isNumber() { return ValUtils::isNumber(_type); }
	inline bool isArray() { return _length / ValUtils::getTypeSize(_type) > 1; }
	inline bool isText() { return  ValUtils::isChar(_type) && isArray(); }
	inline bool isBlob() { return (!ValUtils::isChar(_type)) && isArray(); }
public:
	/**  ���������Ͷ���д��������
	*	 @param [in,out]  f ������
	*   @return void
	*/
	virtual void  write(std::ostream & f);
	/**  ���������ж�ȡ�������Ͷ���
	*	 @param [in,out]  f ������
	*   @return void
	*/
	virtual void  read(std::istream & f);
};
end_gdb_namespace
end_gtl_namespace


