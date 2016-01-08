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

#include "geometry.h"


begin_gtl_namespace
begin_gdb_namespace

class SharedModel;
typedef std::shared_ptr<SharedModel> SharedModelSharedPtr;
typedef SharedModel* SharedModelPtr;

/** @addtogroup Geometry SharedModel-����ģ����
*  @{
*/
class CORE_API SharedModel{
public:
	/** ��ȡ����ģ��ID
	* @return ����ģ��ID
	*/
	virtual Identifier::raw_type getID() = 0;

	/** ���ù���ģ��ID
	* @param [in] mdlid ����ģ��ID
	*/
	virtual void setID(Identifier::raw_type mdlid) = 0;

	/** ��ȡ����ģ������
	* @return ����ģ������
	*/
	virtual string getName() = 0;

	/** ���ù���ģ������
	* @param [in] mdlname ����ģ������
	* @return void
	*/
	virtual void setName(string mdlname) = 0;

	/** ��ȡ����ģ������
	* @return int: ����ģ������
	*/
	virtual int getType() = 0;

	/** ���ù���ģ������
	* @param [in] mdltype ����ģ������
	*/
	virtual void setType(int mdltype) = 0;

	/** ��ȡ����ģ�͹����Ĳ���ID
	* @return ����ģ�͹����Ĳ���ID
	*/
	virtual vector<Identifier::raw_type>& getMaterialIDs() = 0;

	/** ���ù���ģ�͹����Ĳ���ID
	* @param [in] linkmatids ����ģ�͹����Ĳ���ID
	*/
	virtual void setMaterialIDs(vector<Identifier::raw_type>& linkmatids) = 0;

	/** ���ù���ģ�͵ļ�������
	* @param [in] p ����ģ�͹����ļ�������
	*/
	virtual void setGeometry(GeometrySharedPtr p) = 0;

	/** ��ȡ����ģ�͵ļ�������
	* @return C3DGeometry������,����ģ�͹����ļ�������
	*/
	virtual GeometrySharedPtr getGeometry() const = 0;

	/** ������д�뻺��
	* @param [in] buf ����
	* @return д��ɹ����
	*/
	virtual bool writeBuffer(Buffer& buf) = 0;

	/** �ӻ����ȡ�����¼
	* @param [in] pBuffer ����
	* @return ��ȡ�ɹ����
	*/
	virtual bool readBuffer(Buffer& buf) = 0;

	/** ���������ռ�ڴ��ֽ���
	* @param
	* @return ������Ĳ���m_nSize
	*/
	virtual size_t sizeBuffer() = 0;
public:
	static SharedModelSharedPtr create();
	static SharedModelSharedPtr create(Identifier::raw_type d, std::string name, int type);

};
/** @} */




end_gdb_namespace
end_gtl_namespace


