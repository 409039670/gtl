
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
#include "internal_GeometryImpl.h"
#include "internal_rayimpl.h"
#include "internal_frustumimpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

class Geometry3dImpl 	: virtual public GeometryImpl
{
	/** @defgroup Geometry3dImpl Geometry3dImpl-��ά���Σ�������ࣩ
	*  @{
	*/

protected:
	/** @name ���ԣ���Ա������
	* @{
	*/

	/** �Ƿ�Ϊһ�����ж���Ŀ�¡ʵ�� */
	int m_bClone;

	/** ����ģ����򣩰�Χ�� */
	AABBoxImpl m_eEnvelope;

	/** @} */ // ���Խ�β

public:

	/** @defgroup Geometry3dImpl ��������Ա������
	* @{
	*/


	/** @name ��������������
	*  @{
	*/

	/** Ĭ�Ϲ��캯��
	*/
	Geometry3dImpl();
	/** Ĭ����������
	*/
	virtual ~Geometry3dImpl();

	/** @} */ // ����������������β



	/** @name ���ݲ���
	*  @{
	*/

	/** ���õ�ǰ������������
	* @param [in] MaxBox	��֪��AABBoxImpl����
	* @return
	*/
	void setEnvelope(const AABBoxImpl & envelope3d)
	{
		m_eEnvelope = envelope3d;
	}

	/** ��õ�ǰ�����Ƿ�Ϊһ�����ж���Ŀ�¡ʵ���ı�ʶ
	* @param
	* @return 1--�ǿ�¡ʵ�� 0--���ǿ�¡ʵ��
	*/
	int IsCloned()
	{
		return m_bClone;
	}

	/** ��õ�ǰ������������
	* @param
	* @return ��ǰ������������
	*/
	AABBoxImpl & getEnvelope()
	{
		return m_eEnvelope;
	}

	/** @} */ // ���ݲ������β

	/** @name �������
	*  @{
	*/

	/** ���㵱ǰ�������ⲿָ�����ζ������Сŷʽ����
	* @param [in] pGeometry �ⲿָ�����ζ����ָ��
	* @return �����ζ������Сŷʽ�������ֵ
	*/
	virtual double ComputeMinDis(const Geometry3dImpl* pGeometry) const;

	/** �ж��ⲿָ����ά������뵱ǰ���ζ���������εĿռ��ϵ
	* @param [in]  vPt Vertex3d������ά����������
	* @return '0'--���ڵ�ǰ���ζ������������У�'o'--�㲻�ڵ�ǰ���ζ�������������
	*/
	virtual  char RPointWithGeometry(Vertex3d& vPt) const
	{
		/* If query point is outside bounding box, finished. */
		srand((int)time(0));
		if (m_eEnvelope.IsPointInBox(vPt))
			return '0';
		else
			return 'o';
	}

	/** �ж��ⲿָ����ά������뵱ǰ���ζ���Ŀռ��ϵ
	* @param [in]  vPt Vertex3d������ά����������
	* @return 1--���ڵ�ǰ���ζ�����ڲ���߽磻0--���ڵ�ǰ���ζ����ⲿ
	*/
	virtual int  IsPointInGeometry(Vertex3d& vPt) const
	{
		char cRes = RPointWithGeometry(vPt);
		if (cRes == 'V' || cRes == 'E' || cRes == 'F' || cRes == 'i')
			return 1;
		else
			return 0;
	}

	/** �麯��������������ʵ��
	* �����ⲿָ������ά����󵽵�ǰ���ζ�������ƽ��
	* @param [in] vPt	Vertex3d������ά����������
	* @return �㵽��ǰ���ζ�������ƽ��
	*/
	virtual double DPointToGeometryEx(Vertex3d& vPt) const
	{
		return 1e+14;
	}

	/** �����ⲿָ������ά�����͵�ǰ���ζ���ľ���
	* @param [in]  vPt Vertex3d������ά����������
	* @return ��͵�ǰ����ľ���
	*/
	double DPointToGeometry(Vertex3d& vPt) const
	{
		return sqrt(DPointToGeometryEx(vPt));
	}

	/** �麯��������������ʵ��
	* �����ⲿָ������ά�߶ε���ǰ���ζ�������ƽ��
	* @param [in] begvPt	Vertex3d������ά����������
	* @param [in] endvPt	Vertex3d������ά����������
	* @return �㵽��ǰ���ζ�������ƽ��
	*/
	virtual double D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt)
	{
		return 1e+14;
	}

	/** �����ⲿָ������ά�߶κ͵�ǰ���ζ���ľ���
	* @param [in] begvPt	Vertex3d������ά����������
	* @param [in] endvPt	Vertex3d������ά����������
	* @return ��͵�ǰ����ľ���
	*/
	double D3DLineSegToGeometry(Vertex3d& begvPt, Vertex3d& endvPt)
	{
		return sqrt(D3DLineSegToGeometryEx(begvPt, endvPt));
	}

	/** �麯��������������ʵ��
	* �����ⲿָ���������ε���ǰ���ζ�������ƽ��
	* @param [in] triPt1, triPt2, triPt3Vertex3d������ά����������(��ʾ��������������)
	* @return �㵽��ǰ���ζ�������ƽ��
	*/
	virtual double D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3)
	{
		return 1e+14;
	}

	/** �麯��������������ʵ��
	* ���㵱ǰ����ļ�������
	* @return ����ļ�������
	*/
	virtual Vertex3d CalCorePoint()
	{
		Vertex3d corePt;
		return corePt;
	}

	/** �����ⲿָ���������κ͵�ǰ���ζ���ľ���
	* �����ⲿָ���������ε���ǰ���ζ�������ƽ��
	* @param [in] triPt1, triPt2, triPt3Vertex3d������ά����������(��ʾ��������������)
	* @return ��͵�ǰ����ľ���
	*/
	double D3DTriangleToGeometry(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3)
	{
		return sqrt(D3DTriangleToGeometryEx(triPt1, triPt2, triPt3));
	}

	/** �麯��
	* ͳ�Ƶ�ǰ��������ļ�������
	* @param
	* @return long*:�����ڲ����ٿռ䡢�ⲿ�ͷ�
	*               ����ֵ������ -- ���������������Ч�������������������Ч��ɫ����
	*								�����������������ת��Ϊ�ȼ�������Ƭ����
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
	virtual int RGeoWithPolygon2D(vector<Vertex3d>& ptArray, int precisionFlag = 0);

	/** �麯��
	* �ж������Ƿ񴩹���ǰ�����壬��������㵽�뼸���彻�����С����
	* @param [in] ray	��������
	* @param [out] dLength	������㵽�뼸���彻�����С���룬�����ཻ���ֵȡ�����
	* @return	1 -- �ཻ��0 -- ���ཻ
	*/
	virtual int RayCrossGeometry(RayImpl& ray, double& dLength) const;

	/** �麯��
	* �жϽ�ͷ׵���뵱ǰ�������Ƿ��ཻ
	* @param [in] frustum	����׵��
	* @return	1 -- �ཻ��0 -- ���ཻ
	*/
	virtual int FrustumIntersectGeometry(FrustumImpl& frustum);

	/** �麯��
	* �жϽ�ͷ׵���뵱ǰ�������Ƿ��ཻ
	* @param [in] frustum	����׵��
	* @return	1 -- �ཻ��0 -- ���ཻ
	*/
	virtual int FrustumIntersectGeometryEx(FrustumImpl& frustum, vector<GeometryImpl*>& pGeometrys)
	{
		pGeometrys.push_back(this);
		return FrustumIntersectGeometry(frustum);
	}

	/**�жϽ�ͷ׵���뵱ǰ�������������Ƿ��ཻ
	* @param [in] frustum	����׵��
	* @return	1 -- �ཻ��0 -- ���ཻ
	*/
	int FrustumIntersectAABB(FrustumImpl& frustum);

	/** �麯��
	* �ж������Ƿ񴩹���ǰ�����壬��������㵽�뼸���彻�����С�����ƽ��
	* @param [in] vPt1	�������
	* @param [in] vPt2	���߷������Ե�(��vPt1��ͬ�������߷���)
	* @param [out] dLength	������㵽�뼸���彻�����С�����ƽ���������ཻ���ֵȡ�����
	* @return	1 -- �ཻ��0 -- ���ཻ
	*/
	virtual int RayCrossGeometryEx(Vertex3d& vPt1, Vertex3d& vPt2, double& dLength, Geometry3dImpl*& pSubGeometry)
	{
		pSubGeometry = this;
		Vertex3d vDir = vPt2 - vPt1;
		vDir.normalize();
		RayImpl ray(vPt1, vDir);
		return RayCrossGeometry(ray, dLength);
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

	/** �ͷŶ������ݲ���ָ��ͱ�������ʼֵ
	* @return �Ƿ�ִ�гɹ�
	*/
	virtual bool Init();

	/** �麯��
	* �ж������Ƿ񴩹���ǰ�����壬��������㵽�뼸���彻�����С����
	* @param [in] ray	��������
	* @param [out] dLength	������㵽�뼸���彻�����С���룬�����ཻ���ֵȡ�����
	* @return	1 -- �ཻ��0 -- ���ཻ
	*/
	virtual int rayCross(const RaySharedPtr & ray, double& length) const{
		return RayCrossGeometry(*((RayImpl*) ray.get()), length);
	}

	/** �麯��
	/** �жϽ�ͷ׵���뵱ǰ�������Ƿ��ཻ
	* @param [in] frustum	����׵��
	* @return	1 -- �ཻ��0 -- ���ཻ
	*/
	virtual int frustumIntersect(const FrustumSharedPtr & frustum){
		return FrustumIntersectGeometry(*((FrustumImpl*) frustum.get()));
	}
	/** @} */ // ����������β

	/** @} */ // ������β

	/** @} */ // ģ���β
};
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace


