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
#include "internal_surface3dimpl.h"
#include "internal_spatialfuns.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
class Triangle3dImpl : virtual public cn::edu::cug::gdb::Triangle3d, virtual public Surface3dImpl
{
public:
	/**  ��ѯ����ӿ�
	* @param  [in] signal  �������ͱ�ʶ
	* @param  [out] p ��Ӧ�Ķ���ָ��
	* @return 1 -- ��ѯ�ɹ���0 -- ��ѯʧ��
	*/
	virtual bool queryInterface(int signal, void ** p){
		switch (signal){
		case cn::edu::cug::gdb::GEOMTYPE_3D_SURFACE:
		{
			*p = (void*)(cn::edu::cug::gdb::Surface3d *) this;
			return true;
		}
		case cn::edu::cug::gdb::GEOMTYPE_3D_TRIANGLE:
		{
			*p = (void*)(cn::edu::cug::gdb::Triangle3d *) this;
			return true;
		}
		default:
		{
			return false;
		}
		}
	}

	virtual void setVertices(cn::edu::cug::gdb::Vertex3d v[3])
	{
		for (int i = 0; i<3; i++)
		{
			m_vVetices[i].x = v[i].x;
			m_vVetices[i].y = v[i].y;
			m_vVetices[i].z = v[i].z;
		}
	}
	virtual void getVertices(cn::edu::cug::gdb::Vertex3d v[3])
	{
		for (int i = 0; i<3; i++)
		{
			v[i].x = m_vVetices[i].x;
			v[i].y = m_vVetices[i].y;
			v[i].z = m_vVetices[i].z;
		}
	}

	virtual void setNormal(const cn::edu::cug::gdb::Vertex3d & v)
	{
		m_vNormal.x = v.x;
		m_vNormal.y = v.y;
		m_vNormal.z = v.z;
	}
	virtual void getNormal(cn::edu::cug::gdb::Vertex3d & v)
	{
		v.x = m_vNormal.x;
		v.y = m_vNormal.y;
		v.z = m_vNormal.z;
	}
	/** ���������ܳ�
	* @return �ܳ�
	*/
	virtual double getPerimeter()
	{
		return 0.0; //$$
	}
	/** @defgroup Triangle3dImpl Triangle3dImpl-������
	*  @{
	*/

protected:
	/** @name ���ԣ���Ա������
	* @{
	*/

	//##ModelId=49E3472801C7
	/** �������� */
	Vertex3d m_vVetices[3];

	//##ModelId=49E3472801D4
	/** ���� */
	Vertex3d m_vNormal;

	/** @} */ // ���Խ�β

public:

	/** @defgroup Triangle3dImpl ��������Ա������
	* @{
	*/


	/** @name ��������������
	*  @{
	*/

	/** ���캯��1
	*/
	Triangle3dImpl()
	{
		m_byteTypeID = GEOTYPE_3D_TRIANGLE;
	}

	/** ���캯��2
	*/
	Triangle3dImpl(Vertex3d& v1, Vertex3d& v2, Vertex3d& v3, Vertex3d& vNormal)
	{
		m_byteTypeID = GEOTYPE_3D_TRIANGLE;
		m_vVetices[0] = v1;
		m_vVetices[1] = v2;
		m_vVetices[2] = v3;
		m_vNormal = vNormal;
	}

	/** ���캯��3
	*/
	Triangle3dImpl(const Vertex3d* vVertices, Vertex3d& vNormal)
	{
		m_byteTypeID = GEOTYPE_3D_TRIANGLE;
		m_vVetices[0] = vVertices[0];
		m_vVetices[1] = vVertices[1];
		m_vVetices[2] = vVertices[2];
		m_vNormal = vNormal;
	}

	/** @} */ // ����������������β



	/** @name ���ݲ���
	*  @{
	*/

	/** ��ö���ķ�����
	* @param
	* @return ����ķ�����
	*/
	Vertex3d GetNormal() const
	{
		return m_vNormal;
	}

	/** ��ö����б�
	* @param [out] vVertices  ������ֵ�Ķ����б�
	* @return
	*/
	void GetVertices(Vertex3d* vVertices) const
	{
		vVertices[0] = m_vVetices[0];
		vVertices[1] = m_vVetices[1];
		vVertices[2] = m_vVetices[2];
	}

	/** ���ö���ķ�����
	* @param [in] vNor ����ķ�����
	* @return
	*/
	void SetNormal(const Vertex3d& vNor)
	{
		m_vNormal = vNor;
	}

	/** ����ע����ɫ
	* @param [in] vVertices �����б�ָ��
	* @return
	*/
	void SetVertices(const Vertex3d* vVertices)
	{
		m_vVetices[0] = vVertices[0];
		m_vVetices[1] = vVertices[1];
		m_vVetices[2] = vVertices[2];
	}

	/** @} */ // ���ݲ������β



	/** @name �������
	*  @{
	*/

	//##ModelId=49EF08490242
	/** �����������
	* @return ���ֵ
	*/
	double Area() const
	{
		Vertex3d v1 = m_vVetices[1] - m_vVetices[0];
		Vertex3d v2 = m_vVetices[2] - m_vVetices[1];
		return 0.5 * (v1 ^ v2).length();
	}

	//##ModelId=49EF08490242
	/** ���������з������
	* @param [in] vDirection �ۿ�����
	* @return ���ֵ
	*/
	double Area(Vertex3d& vDirection) const
	{
		Vertex3d v1 = m_vVetices[0] - m_vVetices[1];
		Vertex3d v2 = m_vVetices[0] - m_vVetices[2];
		Vertex3d v3 = v1 ^ v2;
		if (v3 * vDirection < -1e-6)
			return -v3.length(); // v3��ģ
		else
			return v3.length();
	}


	//##ModelId=49EF0853004E
	/** ���������ܳ�
	* @return �ܳ�
	*/
	// ��Area����������ע�������������Ϊconst��Ա��������ôm_vVetices�ǲ��ܸı�ģ�Ҳ����const��������ô�ǲ��ܴ���DPointToPoint�ġ����DPointToPoint��Ҫ������
	double Perimeter()
	{
		double l1 = DPointToPoint(m_vVetices[0], m_vVetices[1]);
		double l2 = DPointToPoint(m_vVetices[1], m_vVetices[2]);
		double l3 = DPointToPoint(m_vVetices[2], m_vVetices[0]);
		return l1 + l2 + l3;
	}

	/** �����ⲿָ������ά����󵽵�ǰ���ζ�������ƽ��
	* @param [in] vPt	Vertex3d������ά����������
	* @return �㵽��ǰ���ζ�������ƽ��
	*/
	double DPointToGeometryEx(Vertex3d& vPt) const
	{
		Vertex3d tempPt;
		Vertex3d pt0 = m_vVetices[0];
		Vertex3d pt1 = m_vVetices[1];
		Vertex3d pt2 = m_vVetices[2];

		return DPointToTriangleEx(vPt, pt0, pt1, pt2, &tempPt);
	}

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


	/** �麯��������������ʵ��
	* �����ⲿָ������ά�߶ε���ǰ���ζ�������ƽ��
	* @param [in] begvPt	Vertex3d������ά����������
	* @param [in] endvPt	Vertex3d������ά����������
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
	* @param [in] ray	��������
	* @param [out] dLength	������㵽�뼸���彻�����С���룬�����ཻ���ֵȡ�����
	* @return	1 -- �ཻ��0 -- ���ཻ
	*/
	virtual int RayCrossGeometry(RayImpl& ray, double& dLength) const;

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
