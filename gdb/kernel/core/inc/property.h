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

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

/** @addtogroup value PROPTYPES ϵͳ֧�ֵ���������
*  @{
*/
static const char*  PROPTYPES[] = {
	/** �������ͣ������κ��������� */
	"PROP_TYPE_INTEGER",
	/**  �����������ͣ�����˫���Ⱥ͵����� */
	"PROP_TYPE_DECIMAL",
	/** �����ַ����ַ������� */
	"PROP_TYPE_CHAR",
	/** �䳤�ַ����ַ������� */
	"PROP_TYPE_VARCHAR",
	/** ���ں�ʱ������ */
	"PROP_TYPE_DATE",
	/** ����������� */
	"PROP_TYPE_BLOB"
};
/** @} */

/** @addtogroup value Property
*  @{
*/
class GV3DCORE_API Property :public Value{
public:
	/** @addtogroup value PropType �������Ͷ���
	*  @{
	*/
	class Type{
	protected:
		/** @name ���Գ�Ա����
		*  @{
		*/
		/** ������������ */
		std::string _name;
		/** �������ͱ��� */
		std::string _code;
		/** �������͵������ַ�����������PROPTYPES�����е�ֵ */
		std::string _type;
		/** �������ͳ��ȣ������ֽڽ��м��㣬˫�ֽڼ�Ϊ�����ֽ� */
		int         _length;
		/** �������͵�С����λ�������ֶ�ֻ������������ΪG3D_TYPE_DECIMAL����Ч */
		int         _decimal;
		/** ���������Ƿ����Ϊ�գ�true��ʾ����Ϊ�գ�false��ʾ����Ϊ�� */
		bool        _nullable;
		/** @} */

	public:
		/** @name ���캯������������
		*  @{
		*/
		Type(){
			_name = "null";
			_code = "unit";
			_type = "G3D_TYPE_VARCHAR";
			_length = 50;
			_decimal = 0;
			_nullable = 0;
		}
		Type(std::string name, std::string code, std::string type, int length, int decimal = 0, bool nullable = false){
			_name = name;
			_code = code;
			_type = type;
			_length = length;
			_decimal = decimal;
			_nullable = nullable;
		}
		Type(const Type & t){ copy(t); }
		/** @} */

		/** @name ������ȺŸ�ֵ����
		*  @{
		*/
		void copy(const Property::Type & t){
			_name = t._name;
			_code = t._code;
			_type = t._type;
			_length = t._length;
			_decimal = t._decimal;
			_nullable = t._nullable;
		}
		Type operator = (const Property::Type & t){
			_name = t._name;
			_code = t._code;
			_type = t._type;
			_length = t._length;
			_decimal = t._decimal;
			_nullable = t._nullable;
			return *this;
		}
		bool operator==(const Property::Type & t){
			if (_name == t._name &&	_code == t._code &&
				_type == t._type &&
				_length == t._length &&
				_decimal == t._decimal &&
				_nullable == t._nullable){
				return true;
			}
			else{
				return false;
			}
		}
		/** @} */

		/** @name �����Ա������ȡ�����ò���������
		*  @{
		*/
		inline std::string getName(){ return _name; }
		inline  void  setName(std::string v){ _name = v; }

		inline std::string getCode(){ return _code; }
		inline void   setCode(std::string v){ _code = v; }

		inline std::string  getType(){ return _type; }
		inline void   setType(std::string v){ _type = v; }

		inline int  getLength(){ return _length; }
		inline void   setLength(int v){ _length = v; }

		inline int  getDecimal(){ return _decimal; }
		inline void   setDecimal(int v){ _decimal = v; }

		inline bool  isNullable(){ return _nullable; }
		inline void   setNullable(bool v){ _nullable = v; }
		/** @} */

	public:
		/**  �ж����������Ƿ�Ϊ��������
		*	 @param ��
		*   @return ��������������������ͣ��򷵻�true�����򷵻�false
		*/
		bool isNumber(){
			if (strcmp(_type.c_str(), PROPTYPES[0]) == 0 || strcmp(_type.c_str(), PROPTYPES[1]) == 0)
				return true;
			else
				return false;
		}
	public:
		/**  ���������Ͷ���д��������
		*	 @param [in,out]  f ������
		*   @return void
		*/
		virtual void  write(std::ostream & f)
		{

			int s = _name.size();
			f.write((char*)(&s), sizeof(int));
			f.write((char*)(_name.c_str()), s);

			s = _code.size();
			f.write((char*)(&s), sizeof(int));
			f.write((char*)(_code.c_str()), s);

			s = _type.size();
			f.write((char*)(&s), sizeof(int));
			f.write((char*)(_type.c_str()), s);

			f.write((char*)(&_length), sizeof(int));

			f.write((char*)(&_decimal), sizeof(int));

			s = _nullable ? 1 : 0;
			f.write((char*)(&s), sizeof(int));
		}
		/**  ���������ж�ȡ�������Ͷ���
		*	 @param [in,out]  f ������
		*   @return void
		*/
		virtual void  read(std::istream & f){
			int s = 0;
			char ca[255];
			f.read((char*)(&s), sizeof(int));
			f.read((char*)(ca), s);
			_name.assign(ca, ca + s);


			f.read((char*)(&s), sizeof(int));
			f.read((char*)(ca), s);
			_code.assign(ca, ca + s);

			f.read((char*)(&s), sizeof(int));
			f.read((char*)(ca), s);
			_type.assign(ca, ca + s);


			f.read((char*)(&_length), sizeof(int));

			f.read((char*)(&_decimal), sizeof(int));

			f.read((char*)(&s), sizeof(int));

			_nullable = (s != 0);
		}
	};
	/** @} */
protected:
	std::string _name;
public:
	Property();
	void copy(const Property & p);
	Property(const Property & p);
	Property(std::string sz, const ValUtils::VALUE & v);
	Property operator = (const Property & p);
public:
	inline std::string getName(){ return _name; }
	inline void setName(std::string & s){ _name = s; }

	virtual void write(Buffer & bs);
	virtual void read(Buffer & bs);
};
/** @} */
//typedef Property::Type PropType;
using PropType = Property::Type;
typedef std::shared_ptr<Value> ValueSharedPtr;
typedef std::shared_ptr<Property> PropertySharedPtr;

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
