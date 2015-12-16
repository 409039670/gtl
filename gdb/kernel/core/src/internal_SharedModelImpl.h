#pragma once
#include "sharedModel.h"

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
 
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
 
class SharedModelImpl : public SharedModel
{
	/** @defgroup CMdlInfo CMdlInfo-����ģ�͹�����
	*  @{
	*/
protected:
	/** @name ���ԣ���Ա������
	* @{
	*/

	/**����ģ��ID */
	Identifier::raw_type m_mdlid;

	/**����ģ������ */
	//�д�����
	string m_mdlname;

	/**����ģ������ */
	int m_mdltype;

	/**����ģ�͹����Ĳ���ID */
	vector<Identifier::raw_type> m_linkmatids;

	/**����ģ�͵ļ������� */
	//��ʱ��TrackleFile��������
	GeometrySharedPtr m_geom;

	/** @} */ // ���Խ�β

public:

	/** @name ��������
	* @{
	*/

	/** Ĭ�Ϲ��캯��
	* @param [in]
	*/
	SharedModelImpl();

	/** ���캯��
	* @param [in] mdlid			����ģ��ID
	* @param [in] mdlname		����ģ������
	* @param [in] mdltype		����ģ������
	*/
	SharedModelImpl(Identifier::raw_type mdlid, string mdlname, int mdltype);

	/** ��������
	* @param [in]
	*/
	~SharedModelImpl();

		/** ������д�뻺��
	* @param [in] buf ����
	* @return д��ɹ����
	*/
	virtual bool writeBuffer(Buffer& buf);

	/** �ӻ����ȡ�����¼
	* @param [in] pBuffer ����
	* @return ��ȡ�ɹ����
	*/
	virtual bool readBuffer(Buffer& buf);

	/** ���������ռ�ڴ��ֽ���
	* @param
	* @return ������Ĳ���m_nSize
	*/
	virtual size_t sizeBuffer();


	/** @} */ // ����������β


	/** @} */ // ģ���β
public:
	/** ��ȡ����ģ��ID
	* @return ����ģ��ID
	*/
	virtual Identifier::raw_type getID() {
		return m_mdlid;
	}

	/** ���ù���ģ��ID
	* @param [in] mdlid ����ģ��ID
	*/
	virtual void setID(Identifier::raw_type mdlid){
		m_mdlid = mdlid;
	}

	/** ��ȡ����ģ������
	* @return ����ģ������
	*/
	virtual string getName() {
		return m_mdlname;
	}

	/** ���ù���ģ������
	* @param [in] mdlname ����ģ������
	*/
	virtual void setName(string mdlname) {
		m_mdlname = mdlname;
	}

	/** ��ȡ����ģ������
	* @return ����ģ������
	*/
	virtual int getType() {
		return m_mdltype;
	}

	/** ���ù���ģ������
	* @param [in] mdltype ����ģ������
	*/
	virtual void setType(int mdltype){
		m_mdltype = mdltype;
	}


	/** ��ȡ����ģ�͹����Ĳ���ID
	* @return ����ģ�͹����Ĳ���ID
	*/
	virtual vector<Identifier::raw_type>& getMaterialIDs(){
		return m_linkmatids;
	}

	/** ���ù���ģ�͹����Ĳ���ID
	* @param [in] linkmatids ����ģ�͹����Ĳ���ID
	*/
	virtual void setMaterialIDs(vector<Identifier::raw_type>& linkmatids) {
		m_linkmatids.resize(linkmatids.size());
		std::copy(linkmatids.begin(), linkmatids.end(), m_linkmatids.begin());
	}

	/** ���ù���ģ�͵ļ�������
	* @param [in] p ����ģ�͹����ļ�������
	*/
	virtual void setGeometry(GeometrySharedPtr p) {
		m_geom = p;
	}

	/** ��ȡ����ģ�͵ļ�������
	* @return C3DGeometry������,����ģ�͹����ļ�������
	*/
	virtual GeometrySharedPtr getGeometry() const  {
		return   m_geom;
	}
};
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace



