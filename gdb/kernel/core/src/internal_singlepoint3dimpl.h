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
#include "internal_point3dimpl.h"


begin_gtl_namespace
begin_gdb_namespace
class  SinglePoint3dImpl : virtual public SinglePoint3d, virtual public  Point3dImpl
{

public:

	/**  ��ȡ�����ɫ
	* @param  ��
	* @return �����ɫ
	*/
	virtual Color4f getColor() {
		return Color::create(m_vPoint.cR, m_vPoint.cG, m_vPoint.cB, 1.0f);
	}
	/**  ���õ����ɫ
	* @param  [in] c4f  �����ɫ
	* @return ��
	*/
	virtual void setColor(const Color4f & c4f){
		m_vPoint.cR = c4f.r;
		m_vPoint.cG = c4f.g;
		m_vPoint.cB = c4f.b;
	}

	/**  ��ȡ��ķ���
	* @param  ��
	* @return ��ķ���
	*/
	virtual Vertex3d getNormal(){
		return Vertex3d(m_vPoint.nx, m_vPoint.ny, m_vPoint.nz);
	}
	/**  ���õ�ķ���
	* @param  [in] v  ��ķ�������
	* @return ��
	*/
	virtual void setNormal(const Vertex3d & v){
		m_vPoint.nx = v.x;
		m_vPoint.ny = v.y;
		m_vPoint.nz = v.z;
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
		case GEOMTYPE_3D_SINGLEPOINT:
		{
			*p = (void*)(SinglePoint3d *) this;
			return true;
		}
		default:
		{
			return false;
		}
		}
	}

	/**  ��ȡ��Ĵ�С
	* @param  ��
	* @return ��Ĵ�С
	*/
	virtual double getPointSize() {
		return m_dPointSize;
	}
	/**  ���õ�Ĵ�С
	* @param  [in] s  ��Ĵ�С
	* @return ��
	*/
	virtual void setPointSize(double s) {
		m_dPointSize = s;
	}

	/** @defgroup Point3dImpl SinglePoint3dImpl-��ά��
	*  @{
	*/

protected:
	/** @name ���ԣ���Ա������
	* @{
	*/

	/** @brief ��ά���С */
	double m_dPointSize;

	/** @} */ // ���Խ�β

public:

	/** @defgroup SinglePoint3dImpl ��������Ա������
	* @{
	*/

	/** @name ��������������
	*  @{
	*/

	/** ���캯��
	*/
	SinglePoint3dImpl(void);

	/** ��������
	*/
	virtual ~SinglePoint3dImpl(void);

	/** @} */ // ����������������β


	/** @name ���ݲ���
	*  @{
	*/

	/** ���õ�����
	* @param [in] vPoint	������
	* @return
	*/
	void SetVertex(ShadedVertex3d vPoint)
	{
		m_vPoint = vPoint;
	}

	/** ��ö������ͱ���
	* @param
	* @return ������
	*/
	ShadedVertex3d GetVertex() const
	{
		return m_vPoint;
	}

	/** ���õ��С
	* @param [in] vPoint	���С
	* @return
	*/
	void SetPointSize(double dPointSize)
	{
		m_dPointSize = dPointSize;
	}

	/** ��õ��С
	* @param
	* @return ���С
	*/
	double GetPointSize() const
	{
		return m_dPointSize;
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
end_gtl_namespace


