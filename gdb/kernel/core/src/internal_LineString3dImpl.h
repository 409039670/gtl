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
#include "internal_Curve3dImpl.h"
#include "internal_rayimpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

class LineString3dImpl : virtual public LineString3d, virtual public Curve3dImpl
{
public:

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
		case GEOMTYPE_3D_LINESTRING:
		{
			*p = (void*)(LineString3d *) this;
			return true;
		}
		default:
		{
			return false;
		}
		}
	}
	/** @defgroup LineString3dImpl LineString3dImpl-��ά����
	*  @{
	*/
	/**  ��ȡ���߿��
	* @param  [in] lineWidth  ���߿��
	* @param  [out] lineWidth  ���߿��
	* @return ��
	*/
	virtual void getLineWidth(double& lineWidth){
		lineWidth = m_dLineWidth;
	}
	/**  �������߿��
	* @param  [in] lineWidth  ���߿��
	* @return
	*/
	virtual void setLineWidth(double& lineWidth){
		m_dLineWidth = lineWidth;
	}

	/** ��ȡ�ߵĳ���
	* @param
	* @return �߳�
	*/
	virtual double getLength(){ return 0.0; }	//$$

protected:
	/** @name ���ԣ���Ա������
	* @{
	*/

	/** @brief �߿� */
	double m_dLineWidth;

	/** @} */ // ���Խ�β

public:

	/** @defgroup LineString3dImpl ��������Ա������
	* @{
	*/


	/** @name ��������������
	*  @{
	*/


	/** ���캯��1
	*/
	LineString3dImpl()
	{
		m_lVertNum = 0;
		m_vVertices = NULL;
		m_byteTypeID = GEOTYPE_3D_LINESTRING;
		m_iVertexType = VERTEXTYPE_V3d;
		m_dLineWidth = 0.0;
	}

	/** ���캯��2
	*/
	LineString3dImpl(int nVertNum, void* const vVertices, unsigned char vertexType, double dLineWidth)
		: m_dLineWidth(dLineWidth)
	{
		m_byteTypeID = GEOTYPE_3D_LINESTRING;
		m_lVertNum = nVertNum;
		m_vVertices = vVertices;
		m_iVertexType = vertexType;
	}

	/** ��������
	*/
	virtual ~LineString3dImpl()
	{
	}

	/** @} */ // ����������������β



	/** @name ���ݲ���
	*  @{
	*/

	/** ������ά���߶�������Ͷ�������
	* @param [in] nVertexNum	��ά���߶������
	* @param [in] vVertices	ָ��һ����ά������ָ��
	* @return
	*/
	void SetVertices(int nVertexNum, void* const vVertices, unsigned char iVertexType)
	{
		m_vVertices = vVertices;
		m_lVertNum = nVertexNum;
		m_iVertexType = iVertexType;
	}

	/** �����ά���߶�������
	* @param [out] vVertices ������ֵ��һ����ά�����ָ�������
	* @return
	*/
	void GetVertices(void*& vVertices) const
	{
		vVertices = m_vVertices;
	}

	void GetVerticesEx(vector<Vertex3d>& vt3d) const;

	/** �����ά���߶������
	* @param
	* @return ��ά���߶������
	*/
	long GetVerticesNum() const
	{
		return m_lVertNum;
	}

	/** ��ö������ͱ���
	* @param
	* @return �������ͱ���
	*/
	unsigned char GetVertexType() const
	{
		return m_iVertexType;
	}

	/** �����߿�
	* @param [in] dLineWidth	�߿�
	* @return
	*/
	void SetLineWidth(double dLineWidth)
	{
		m_dLineWidth = dLineWidth;
	}

	/** ����߿�
	* @param
	* @return �߿�
	*/
	double GetLineWidth() const
	{
		return m_dLineWidth;
	}

	/** @} */ // ���ݲ������β

	virtual int RayCrossGeometry(RayImpl& ray, double& dLength) const;

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
