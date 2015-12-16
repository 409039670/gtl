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
#include "internal_Point3dImpl.h"
#include "internal_spatialfuns.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
class MultiPoint3dImpl :virtual public MultiPoint3d, virtual public Point3dImpl
{
public:
	/**  ��ȡ��λ��������
	* @param  ��
	* @return ��λ��������
	*/
	virtual VertexVisitorSharedPtr getVertexVisitor(){
		VertexVisitorSharedPtr p = VertexVisitor::create(m_iVertexType);
		p->set(m_vVertices, m_lVertNum, m_iVertexType);
		return p;
	}


	/**  ��ѯ����ӿ�
	* @param  [in] signal  �������ͱ�ʶ
	* @param  [out] p ��Ӧ�Ķ���ָ��
	* @return 1 -- ��ѯ�ɹ���0 -- ��ѯʧ��
	*/
	virtual bool queryInterface(int signal, void ** p){
		switch (signal){
			case GEOMTYPE_3D_POINT:
			{
				*p = (void*)(Point3d *) this;
				return true;
			}
			case GEOMTYPE_3D_MULTIPOINT:
			{
				*p = (void*)(MultiPoint3d *) this;
				return true;
			}
			default:
			{
				return false;
			}
		}
	}

	/**  ��ȡ�������
	* @param  [out] size  ���Ե��ֽڳ���
	* @param  [out] att �������
	* @return ��
	*/
	virtual void getAttributes(long & size, void ** att){
		size = m_nAttributeSize;
		*att = m_pAttributes;
	}
	/**  ���õ������
	* @param  [in] size  ���Ե��ֽڳ���
	* @param  [in] pAttributes �������
	* @return ��
	*/
	virtual void setAttributes(const long  size, void * const att) {
		m_nAttributeSize = size;
		m_pAttributes = att;
	}

	/** @defgroup Point3dImpl MultiPoint3dImpl-��ά���
	*  @{
	*/

	/** ���ö������ͱ���
	* @param [in] iVertexType	 �������ͱ���
	* @return
	*/
	virtual void setVertexType(unsigned char iVertexType)
	{
		SetVertexType(iVertexType);
	};
	/** ��ö������ͱ���
	* @param ��
	* @return �������ͱ���
	*/
	virtual unsigned char getVertexType() const
	{
		return m_iVertexType;
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
protected:
	/** @name ���ԣ���Ա������
	* @{
	*/

	/** @brief �������ͱ��� */
	unsigned char m_iVertexType;

	/** @brief �������� */
	void *m_vVertices;

	/** @brief ������ */
	long m_lVertNum;

	/** @brief �������������ڴ��С */
	unsigned int m_nAttributeSize;

	/** @brief �������� */
	void *m_pAttributes;

	/** @brief ÿ��������Ը��� */
	long m_lAttrNum;

	/** @} */ // ���Խ�β

public:

	/** @defgroup MultiPoint3dImpl ��������Ա������
	* @{
	*/

	/** @name ��������������
	*  @{
	*/

	/** ���캯��1
	*/
	MultiPoint3dImpl()
	{
		m_byteTypeID = GEOTYPE_3D_MULTIPOINT;

		m_iVertexType = VERTEXTYPE_V3d;
		m_lVertNum = 0;
		m_vVertices = NULL;

		m_nAttributeSize = 0;
		m_pAttributes = NULL;
		m_lAttrNum = 0;
	}

	/** ���캯��2
	*/
	MultiPoint3dImpl(long lVertNum, void* const vVertices) :
		m_lVertNum(lVertNum), m_vVertices(vVertices)
	{
		m_byteTypeID = GEOTYPE_3D_MULTIPOINT;

		m_nAttributeSize = 0;
		m_pAttributes = NULL;
		m_lAttrNum = 0;
	}

	/** ��������
	*/
	virtual ~MultiPoint3dImpl(void)
	{
		VertexVisitor::releaseVertexArray(m_iVertexType, m_vVertices);

		if (m_pAttributes)
		{
			delete m_pAttributes;
			m_pAttributes = NULL;
		}
	}

	/** @} */ // ����������������β


	/** @name ���ݲ���
	*  @{
	*/

	/** ���ö������ͱ���
	* @param [in] iVertexType	 �������ͱ���
	* @return
	*/
	void SetVertexType(unsigned char iVertexType)
	{
		m_iVertexType = iVertexType;
	}

	/** ��ö������ͱ���
	* @param
	* @return �������ͱ���
	*/
	unsigned char GetVertexType() const
	{
		return m_iVertexType;
	}

	/** ������ά��㶥������Ͷ�������
	* @param [in] nVertexNum	��ά��㶥�����
	* @param [in] vVertices	ָ��һ����ά������ָ��
	* @return
	*/
	void SetVertices(long nVertexNum, void* const vVertices)
	{
		m_vVertices = vVertices;
		m_lVertNum = nVertexNum;
	}

	/** �����ά��㶥������
	* @param [out] vVertices ������ֵ��һ����ά�����ָ�������
	* @return
	*/
	void GetVertices(void*& vVertices) const
	{
		vVertices = m_vVertices;
	}

	/** �����ά��㶥������
	* @param [out] vVertices ������ֵ��һ����ά�����ָ�������
	* @param [out] vNum ������ֵ��һ����ά�����ָ�������
	* @return
	*/
	void GetVertices(long& lVertNum, void*& vVertices) const
	{
		lVertNum = m_lVertNum;
		vVertices = m_vVertices;
	}

	void GetVerticesEx(vector<Vertex3d>& vt3d) const;

	/** �����ά��㶥�����
	* @param
	* @return ��ά��㶥�����
	*/
	long GetVerticesNum() const
	{
		return m_lVertNum;
	}

	/** �����������������ڴ��С
	* @param [in] nAttributeSize	�������д�С
	* @return
	*/
	void SetAttrSize(size_t nAttributeSize)
	{
		m_nAttributeSize = nAttributeSize;
	}

	/** ����������������ڴ��С
	* @param [out] nAttributeSize ������ֵ�������д�С
	* @return
	*/
	void GetAttrSize(size_t& nAttributeSize) const
	{
		nAttributeSize = m_nAttributeSize;
	}

	/** ������������
	* @param [in] lAttrNum	ÿ��������Ը���
	* @param [in] pAttributes	��������
	* @return
	*/
	void SetAttribute(long lAttrNum, void* const pAttributes)
	{
		m_lAttrNum = lAttrNum;
		m_pAttributes = pAttributes;
	}

	/** �����������
	* @param [out] nAttributeSize ������ֵ�������д�С
	* @return
	*/
	void GetAttribute(long& lAttrNum, void*& pAttributes) const
	{
		lAttrNum = m_lAttrNum;
		pAttributes = m_pAttributes;
	}

	/** ������������
	* @param [in] pAttributes	��������
	* @return
	*/
	void SetAttribute(void* const pAttributes)
	{
		m_pAttributes = pAttributes;
	}

	/** �����������
	* @param [out] pAttributes	��������
	* @return
	*/
	void GetAttribute(void*& pAttributes) const
	{
		pAttributes = m_pAttributes;
	}

	/** ��������ÿ��������Ը���
	* @param [in] lAttrNum	ÿ��������Ը���
	* @return
	*/
	void SetAttributeCount(long lAttrNum)
	{
		m_lAttrNum = lAttrNum;
	}

	/** ������ȡÿ��������Ը���
	* @param [out] nAttributeSize ������ֵ�������д�С
	* @return
	*/
	void GetAttributeCount(long& lAttrNum) const
	{
		lAttrNum = m_lAttrNum;
	}

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

	/** �����ⲿָ������ά����󵽵�ǰ���ζ�������ƽ��
	* @param [in] vPt	Vertex3d������ά����������
	* @return �㵽��ǰ���ζ�������ƽ��
	*/
	virtual double DPointToGeometryEx(Vertex3d& vPt) const;

	/** �麯��������������ʵ��
	* �����ⲿָ������ά�߶ε���ǰ���ζ�������ƽ��
	* @param [in] vPt	Vertex3d������ά����������
	* @return �㵽��ǰ���ζ�������ƽ��
	*/
	virtual double D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt);

	/** �麯��������������ʵ��
	* �����ⲿָ���������ε���ǰ���ζ�������ƽ��
	* @param [in] triPt1, triPt2, triPt3Vertex3d������ά����������(��ʾ��������������)
	* @return �߶ε���ǰ���ζ�������ƽ��
	*/
	virtual double D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3);

	/** �麯��
	* ͳ�Ƶ�ǰ��������ļ�������
	* @param
	* @return long*:�����ڲ����ٿռ䡢�ⲿ�ͷ�
	*               ����ֵ������ -- ���������������Ч�������������������Ч��ɫ����
	*								�����������������ת��Ϊ�ȼ�����
	*								����������������
	*/
	virtual long* GeoStatistics();

	/** �麯��
	* �жϵ�ǰ����͸����������2D�ϵĹ�ϵ�����룬�ཻ������
	* �ж�
	* @param [in] ptArray		�����
	* @param [in] precisionFlag ���ȱ�־λ 0-������� 1-͹�� 2-����������
	* @return 1-�ཻ���ٽӣ������� 0-���� -1-����
	*/
	virtual int RGeoWithPolygon2D(vector<Vertex3d>& ptArray, int precisionFlag = 0);

	/** �ͷŶ������ݲ���ָ��ͱ�������ʼֵ
	* @return �Ƿ�ִ�гɹ�
	*/
	virtual bool Init();


	/** @} */ // ����������β

	/** @} */ // ������β

	/** @} */ // ģ���β

};

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
