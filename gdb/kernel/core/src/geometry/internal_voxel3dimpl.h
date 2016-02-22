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
#include "internal_volume3dimpl.h"


begin_gtl_namespace
begin_gdb_namespace
enum
{
	VOXEL_INVALID,
	VOXEL_CUBE,
	VOXEL_PRISM,
	VOXEL_TETRAHEDRON,
};

 
 
class Voxel3dImpl : virtual public Voxel3d, virtual public Volume3dImpl
{
	/** @defgroup Voxel3dImpl Voxel3dImpl-��ά��Ԫ�����ࣩ
	*  @{
	*/
protected:

	/** @name ���ԣ���Ա������
	* @{
	*/

	/** ��Ԫ���ͣ�ÿ����Ԫ���;�ָ��һ����� */
	unsigned char m_iVolumeTypeID;

	/** @} */ // ���Խ�β


public:

	/** @defgroup Voxel3dImpl ��������Ա������
	* @{
	*/


	/** @name ��������������
	*  @{
	*/

	/** ���캯��
	*/
	Voxel3dImpl()
	{
		m_byteTypeID = GEOTYPE_3D_VOXEL;
		m_iVolumeTypeID = VOXEL_INVALID;
	}

	/** @} */ // ����������������β



	/** @name ���ݲ���
	*  @{
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
		default:
		{
			return false;
		}
		}
	}
	/** ��ȡ��������
	* @param  ��
	* @return �������
	*/
	virtual double getVolume(){ return 0.0; }	//$$
	/** �����Ԫ���ͱ��
	* @param
	* @return  ��Ԫ���ͱ��
	*/
	virtual unsigned char getVolumeTypeID()
	{
		return m_iVolumeTypeID;
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


