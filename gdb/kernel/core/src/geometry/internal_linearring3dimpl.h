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
#include "internal_curve3dimpl.h"
#include "internal_rayimpl.h"
#include "internal_frustumimpl.h"


begin_gtl_namespace
begin_gdb_namespace
class LinearRing3dImpl :public Curve3dImpl , virtual public LinearRing3d
{
	/** @defgroup LinearRing3dImpl LinearRing3dImpl-�߻�
	*  @{
	*/

protected:

	/** @name ���ԣ���Ա������
	* @{
	*/
	/** @brief �ڻ������⻷�ı�־ */
	int m_bInOrOutRing;

	/** @} */ // ���Խ�β
public:

	/**  ��ȡ�߻���־
	* @param  ��
	* @return 1 -- �ڻ���־��0 -- �⻷��־
	*/
	virtual int getRingFlag() {
		return m_bInOrOutRing;
	}
	/**  �����߻���־
	* @param  [in] nFlag �߻���־ (1 -- �ڻ���־��0 -- �⻷��־)
	* @return ��
	*/
	virtual void setRingFlag(int f){
		m_bInOrOutRing = f;
	}

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
		case GEOMTYPE_3D_LINEARRING:
		{
			*p = (void*)(LinearRing3d *) this;
			return true;
		}
		default:
		{
			return false;
		}
		}
	}

	/** ��ȡ�ߵĳ���
	* @param
	* @return �߳�
	*/
	virtual double getLength(){ return 0.0; }	//$$

public:

	/** @defgroup LinearRing3dImpl ��������Ա������
	* @{
	*/


	/** @name ��������������
	*  @{
	*/

	/** ���캯��1
	*/
	LinearRing3dImpl()
	{
		m_lVertNum = 0;
		m_vVertices = NULL;
		m_bInOrOutRing = 0; // �⻷
		m_byteTypeID = GEOTYPE_3D_LINEARRING;
		m_iVertexType = VERTEXTYPE_V3d;
	}

	/** ���캯��2
	*/
	LinearRing3dImpl(int nVertexNum, Vertex3d* const vVertices, int bInOrOutRing = 0)
		: m_bInOrOutRing(bInOrOutRing)
	{
		m_byteTypeID = GEOTYPE_3D_LINEARRING;
		m_iVertexType = VERTEXTYPE_V3d;
		m_lVertNum = nVertexNum;
		m_vVertices = vVertices;
	}

	/** ��������
	*/
	virtual ~LinearRing3dImpl()
	{
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

	/** �����߻��������������ֵ
	* @param [in] lVertNum	�������
	* @param [in] vVertices	ָ��һ����������ָ��
	* @return
	*/
	void SetVertices(long lVertNum, void* const vVertices)
	{
		m_lVertNum = lVertNum;
		m_vVertices = vVertices;
	}

	/** ��õ�ǰ����Ķ������е�ָ��
	* @param [in] vVertices ����ֵ�Ķ������е�ָ��
	* @return
	*/
	void GetVertices(void*& vVertices) const
	{
		vVertices = m_vVertices;
	}

	/** ��õ�ǰ����Ķ������
	* @param
	* @return ��ǰ����Ķ������
	*/
	long GetVerticesNum() const
	{
		return m_lVertNum;
	}

	/** ��ö����б�Ͷ����б���
	* @param [out] vVertexList	�����б�
	* @param [out] lVertNum	�����б���
	* @return
	*/
	void GetVertices(void*& vVertexList, long& lVertNum) const
	{
		vVertexList = m_vVertices;
		lVertNum = m_lVertNum;
	}

	/** ��ö����б�2
	* @param [out]  vt3d �����б�
	* @return
	*/
	void GetVerticesEx(vector<Vertex3d>& vt3d) const;

	/** ������ڻ������⻷�ı�־ֵ
	* @param
	* @return 1 -- �ڻ���־��0 -- �⻷��־
	*/
	int GetInOutRingFlag()
	{
		return m_bInOrOutRing;
	}

	/** �������ڻ������⻷�ı�־
	* @param [in] bFlag	��־ֵ
	* @return
	*/
	void SetInOutRingFlag(int bFlag)
	{
		m_bInOrOutRing = bFlag;
	}

	/** @} */ // ���ݲ������β


	/** @name �������
	*  @{
	*/

	/** �����ⲿָ������ά����󵽵�ǰ���ζ�������ƽ��
	* @param [in] vPt	Vertex3d������ά����������
	* @return �㵽��ǰ���ζ�������ƽ��
	*/
	double DPointToGeometryEx(Vertex3d& vPt) const;

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
	* �ж������Ƿ񴩹���ǰ�����壬��������㵽�뼸���彻�����С����
	* @param [in] ray ����
	* @param [out] dLength	������㵽�뼸���彻�����С���룬�����ཻ���ֵȡ�����
	* @return	1 -- �ཻ��0 -- ���ཻ
	*/
	virtual int RayCrossGeometry(RayImpl& ray, double& dLength) const;

	/** �ж��ⲿָ����ά������뵱ǰ���ζ���������εĿռ��ϵ
	* @param [in]  vPt Vertex3d������ά����������
	* @return:  'o' - �ڶ�����⣻ 'i' - �ڶ�����ڣ�'E' - �ڶ���α߽��ϣ� 'V' - �ڶ���ε���
	*/
	char  RPointWithGeometry(Vertex3d& vPt) const;

	/** �麯��
	* �жϽ�ͷ׵���뵱ǰ�������Ƿ��ཻ
	* @param [in] frustum	����׵��
	* @return	1 -- �ཻ��0 -- ���ཻ
	*/
	virtual int FrustumIntersectGeometry(FrustumImpl& frustum);

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


	//int FrustumIntersectGeometry(FrustumImpl& frustum);

	/** @} */ // ����������β

	/** @} */ // ������β

	/** @} */ // ģ���β
};

end_gdb_namespace
end_gtl_namespace


