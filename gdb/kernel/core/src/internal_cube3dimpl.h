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
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
class  Cube3dImpl
	: virtual public cn::edu::cug::gdb::Cube3d, public Voxel3dImpl
{
	/** @defgroup Cube3dImpl Cube3dImpl-��ά����
	*  @{
	*/

protected:
	/** @name ���ԣ���Ա������
	* @{
	*/

	/** �������飨8������������Լ������ */
	Vertex3d m_vVertices[8];

	/** @} */  //���Խ�β

public:

	/** @defgroup Cube3dImpl ��������Ա������
	* @{
	*/
	/**  ��ѯ����ӿ�
	* @param  [in] signal  �������ͱ�ʶ
	* @param  [out] p ��Ӧ�Ķ���ָ��
	* @return 1 -- ��ѯ�ɹ���0 -- ��ѯʧ��
	*/
	virtual bool queryInterface(int signal, void ** p){
		switch (signal){
		case cn::edu::cug::gdb::GEOMTYPE_3D_VOLUME:
		{
			*p = (void*)(cn::edu::cug::gdb::Volume3d *) this;
			return true;
		}
		case cn::edu::cug::gdb::GEOMTYPE_3D_VOXEL:
		{
			*p = (void*)(cn::edu::cug::gdb::Voxel3d *) this;
			return true;
		}
		case cn::edu::cug::gdb::GEOMTYPE_3D_CUBE:
		{
			*p = (void*)(cn::edu::cug::gdb::Cube3d *) this;
			return true;
		}
		default:
		{
			return false;
		}
		}
	}
	/** ��ȡ����
	* @param
	* @return ����ֵ
	*/
	virtual double getLength() const
	{
		return Length();
	}

	/** ��ȡ���
	* @param
	* @return ���ֵ
	*/
	virtual double getWidth() const
	{
		return Width();
	}

	/** ��ȡ�߶�
	* @param
	* @return �߶�ֵ
	*/
	virtual double getHeight() const
	{
		return Height();
	}

	/** @name ��������������
	*  @{
	*/
	Cube3dImpl()
	{
		m_byteTypeID = GEOTYPE_3D_CUBE;
		m_iVolumeTypeID = VOXEL_CUBE;
	}
	/** @} */ // ����������������β


	/** @name ���ݲ���
	*  @{
	*/
	/** ��ö�������
	* @param [out] vVertices	���궥�������׵�ַ������
	* @return
	*/
	void SetVertices(Vertex3d*& vVertices)
	{
		for (int i = 0; i<8; i++)
		{
			m_vVertices[i] = vVertices[i];
		}
	}

	/** ��ö�������
	* @param [out] vVertices	���궥�������׵�ַ������
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

	//##ModelId=49E3472800DA
	/** ��ȡ����
	* @param
	* @return ����ֵ
	*/
	double Length() const;

	//##ModelId=49E3472800DB
	/** ��ȡ���
	* @param
	* @return ���ֵ
	*/
	double Width() const;

	//##ModelId=49E3472800DC
	/** ��ȡ�߶�
	* @param
	* @return �߶�ֵ
	*/
	double Height() const;

	//##ModelId=49ECAD70036B
	/** ��ȡ���ĵ�
	* @param
	* @return ���ĵ�
	*/
	Vertex3d Center() const;

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

	/** @} */ //������β

	/** @} */ // ģ���β
};

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
