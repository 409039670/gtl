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
#include "geometry3d.h"
#include "internal_geometry3dimpl.h"


begin_gtl_namespace
begin_gdb_namespace

class Curve3dImpl : virtual public Curve3d, virtual public Geometry3dImpl
{
public:
	/** @defgroup Curve3dImpl Curve3dImpl-��ά����
	*  @{
	*/

	/** @name ���ԣ���Ա������
	* @{
	*/

protected:
	/** @brief �������ͱ��� */
	unsigned char m_iVertexType;
	/** @brief �������� */
	void *m_vVertices;
	/** @brief ������ */
	long m_lVertNum;

	/** @} */ // ���Խ�β
	/** @defgroup Curve3dImpl ��������Ա������
	* @{
	*/
public:
	/**  ��ȡ����λ�����������
	* @param  ��
	* @return ����λ�����������ָ��
	*/
	virtual VertexVisitorSharedPtr getVertexVisitor(){
		VertexVisitorSharedPtr p = VertexVisitor::create(m_iVertexType);
		p->set(m_vVertices, m_lVertNum, m_iVertexType);
		return p;
	}
	/** ��ö������ͱ���
	* @param ��
	* @return �������ͱ���
	*/
	virtual unsigned char getVertexType() const
	{
		return m_iVertexType;
	}
	/** ���ö������ͱ���
	* @param [in] iVertexType	 �������ͱ���
	* @return
	*/
	virtual void setVertexType(unsigned char iVertexType)
	{
		m_iVertexType = iVertexType;
	}
	/** �����ά�������
	* @param
	* @return ��ά�������
	*/
	virtual long getVerticesNum() const
	{
		return m_lVertNum;
	}
	/** ������ά��������Ͷ�������
	* @param [in] nVertexNum	��ά�������
	* @param [in] vVertices	ָ��һ����ά������ָ��
	* @return
	*/
	virtual void setVertices(long nVertexNum, void* const vVertices)
	{
		m_vVertices = vVertices;
		m_lVertNum = nVertexNum;
	}
	/** �����ά��㶥������
	* @param [out] vVertices ������ֵ��һ����ά�����ָ�������
	* @param [out] vNum ������ֵ��һ����ά�����ָ�������
	* @return
	*/
	virtual void getVertices(long& lVertNum, void*& vVertices) const
	{
		vVertices = m_vVertices;
		lVertNum = m_lVertNum;
	}
	/** ��ȡ�ߵĳ���
	* @param
	* @return �߳�
	*/
	virtual double getLength(){ return 0.0; }	//$$

	/**  ��ѯ����ӿ�
	* @param  [in] signal  �������ͱ�ʶ
	* @param  [out] p ��Ӧ�Ķ���ָ��
	* @return 1 -- ��ѯ�ɹ���0 -- ��ѯʧ��
	*/
	virtual bool queryInterface(int signal, void ** p){
		switch (signal){
		case GEOMTYPE_3D_CURVE:
		{
			*p = (void*)(Curve3d *) this;
			return true;
		}
		default:
		{
			return false;
		}
		}
	}

	/** @name ��������������
	*  @{
	*/

	/** Ĭ�Ϲ��캯��
	*/
	Curve3dImpl::Curve3dImpl()
	{
		m_byteTypeID = GEOTYPE_3D_CURVE;
		m_lVertNum = 0;
		m_vVertices = NULL;
		m_iVertexType = VERTEXTYPE_V3d;
	}
	/** ��������
	*/
	virtual ~Curve3dImpl()
	{
		VertexVisitor::releaseVertexArray(m_iVertexType, m_vVertices);
	}
	/** @} */ // ����������������β



	/** @name ���ݲ���
	*  @{
	*/

	/** @} */ // ���ݲ������β



	/** @name �������
	*  @{
	*/

	/** ����һ����ǰ�����������ڴ�ռ�
	* @param
	* @return ���ٵĵ�ǰ������ڴ�ռ���׵�ַ
	*/
	GeometryImpl* CreateObject();

	/** �����ⲿ������4ά����ʵ�ֵ�ǰ����ļ��α任
	* @param [in] m4x4	������4ά����
	* @return
	*/
	void TranslateFrom(Matrix4x4& m4x4);

	/** ���㵱ǰ���������ڴ�Ĵ�С
	* @param
	* @return ��ǰ���������ڴ��С
	*/
	size_t sizeBuffer();

	/** �ӻ������н�����ǰ���󣬲�����Ա������ֵ
	* @param [in] buf	������������
	* @return 1 -- �����ɹ���0 -- ����ʧ��
	*/
	bool readBuffer(Buffer & buf);

	/** �ѵ�ǰ����ĳ�Ա�������ݣ������һ��������
	* @param [out] buf	������������
	* @return 1 -- ����ɹ���0 -- ���ʧ��
	*/
	bool writeBuffer(Buffer& buf);

	/** �麯��������������ʵ��
	* ���㵱ǰ����ļ�������
	* @return ����ļ�������
	*/
	virtual Vertex3d CalCorePoint();

	/** �麯��
	* ͳ�Ƶ�ǰ��������ļ�������
	* @param
	* @return long*:�����ڲ����ٿռ䡢�ⲿ�ͷ�
	*               ����ֵ������ -- ���������������Ч�������������������Ч��ɫ����
	*								�����������������ת��Ϊ�ȼ�����
	*								����������������
	*/
	virtual long* GeoStatistics()
	{
		return NULL;
	}

	/** �麯��
	* �жϵ�ǰ����͸����������2D�ϵĹ�ϵ�����룬�ཻ������
	* �ж�
	* @param [in] ptArray		�����
	* @param [in] precisionFlag ���ȱ�־λ 0-������� 1-͹�� 2-����������
	* @return 1-�ཻ���ٽӣ������� 0-���� -1-����
	*/
	virtual int RGeoWithPolygon2D(vector<Vertex3d>& ptArray, int precisionFlag = 0)
	{
		return -1;
	}

	/** �ͷŶ������ݲ���ָ��ͱ�������ʼֵ
	* @return �Ƿ�ִ�гɹ�
	*/
	virtual bool Init();

	/** @} */ // ����������β

	/** @} */ // ������β

	/** @} */ // ģ���β
};
end_gdb_namespace
end_gtl_namespace


