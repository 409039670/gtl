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
#include "internal_voxel3dimpl.h"


begin_gtl_namespace
begin_gdb_namespace

class Tetrahedron3dImpl
	: virtual public Tetrahedron3d, virtual public Voxel3dImpl
{
	/** @defgroup Tetrahedron3dImpl Tetrahedron3dImpl-������
	*  @{
	*/

protected:
	/** @name ���ԣ���Ա������
	* @{
	*/

	/** �������飨4������������Լ������ */
	Vertex3d m_vVertices[4];

	/** @} */ // ���Խ�β

public:

	/** @defgroup Tetrahedron3dImpl ��������Ա������
	* @{
	*/
	/**  ��ѯ����ӿ�
	* @param  [in] signal  �������ͱ�ʶ
	* @param  [out] p ��Ӧ�Ķ���ָ��
	* @return 1 -- ��ѯ�ɹ���0 -- ��ѯʧ��
	*/
	virtual bool queryInterface(int signal, void ** p){
		switch (signal){
		case GEOMTYPE_3D_VOLUME:
		{
			*p = (void*)(Volume3d *) this;
			return true;
		}
		case GEOMTYPE_3D_VOXEL:
		{
			*p = (void*)(Voxel3d *) this;
			return true;
		}
		case GEOMTYPE_3D_TETRAHEDRON:
		{
			*p = (void*)(Tetrahedron3d *) this;
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
	Tetrahedron3dImpl()
	{
		m_byteTypeID = GEOTYPE_3D_TETRAHEDRON;
		m_iVolumeTypeID = VOXEL_TETRAHEDRON;
	}
	/** @} */ // ����������������β



	/** @name ���ݲ���
	*  @{
	*/

	/** ��ö�������
	* @param [out]  vVertices  ������ֵ�Ķ�������ָ��
	* @return
	*/
	void GetVertices(Vertex3d*& vVertices)
	{
		vVertices = m_vVertices;
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
	virtual int RGeoWithPolygon2D(vector<Vertex3d>& ptArray, int precisionFlag = 0)
	{
		return -1; // ydcl
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


