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
#include "internal_volume3dimpl.h" 


begin_gtl_namespace
begin_gdb_namespace
class Solid3dImpl : virtual public Solid3d,virtual public Volume3dImpl
{
	/** @defgroup Solid3dImpl Solid3dImpl-��άʵ��
	*  @{
	*/

protected:
	/** @name ���ԣ���Ա������
	* @{
	*/

	/** �߽����б���vector����װ�������ָ�� */
	std::vector<Surface3dImpl*> m_vBoundarySurfaces;

	/** @} */ // ���Խ�β
public:

	/** @defgroup Solid3dImpl ��������Ա������
	* @{
	*/


	/** @name ��������������
	*  @{
	*/

	/** ���캯��1
	*/
	Solid3dImpl()
	{
		m_byteTypeID = GEOTYPE_3D_SOLID;
	}

	/** ���캯��2
	*/
	Solid3dImpl(vector<Surface3dImpl*>& vBoundarySurfaces)
	{
		m_byteTypeID = GEOTYPE_3D_SOLID;
		m_vBoundarySurfaces = vBoundarySurfaces;
	}

	/** ��������
	*/
	virtual ~Solid3dImpl()
	{
		int nCount = m_vBoundarySurfaces.size();
		for (int i = nCount - 1; i>-1; i--)
		{
			if (m_vBoundarySurfaces[i])
				delete m_vBoundarySurfaces[i];
		}
		m_vBoundarySurfaces.clear();
	}

	/** @} */ // ����������������β



	/** @name ���ݲ���
	*  @{
	*/

	/** ��ñ߽����б�
	* @param [out] vBoundarySurfaces  ����ֵ�ı߽����б�
	* @return
	*/
	void GetBoundarySurfaces(vector<Surface3dImpl*>& vBoundarySurfaces) const
	{
		vBoundarySurfaces = m_vBoundarySurfaces;
	}

	/** ���ñ߽����б�
	* @param [in] 	vBoundarySurfaces  ��֪�ı߽����б�
	* @return
	*/
	void SetBoundarySurfaces(const vector<Surface3dImpl*>& vBoundarySurfaces)
	{
		m_vBoundarySurfaces = vBoundarySurfaces;
	}

	/** @} */ // ���ݲ������β



	/** @name �������
	*  @{
	*/

	/** �жϵ��뵱ǰ������Ĺ�ϵ
	* @param [in] vPt	��Ҫ�жϵĵ�
	* @return ����Լ������λ�� 'i'--�ڲ�; 'o'--�ⲿ; 'E'--����; 'V'--����; 'F'--��;
	*/
	char RPointWithGeometry(Vertex3d& vPt) const;

	/** �����ⲿָ������ά����󵽵�ǰ���ζ�������ƽ��
	* @param [in] vPt	Vertex3d������ά����������
	* @return �㵽��ǰ���ζ�������ƽ��
	*/
	double DPointToGeometryEx(Vertex3d& vPt) const;

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
	* @return �����ε���ǰ���ζ�������ƽ��
	*/
	virtual double D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3);


	/** �麯��
	* �ж������Ƿ񴩹���ǰ�����壬��������㵽�뼸���彻�����С����
	* @param [in] ray	����
	* @param [out] dLength	������㵽�뼸���彻�����С���룬�����ཻ���ֵȡ�����
	* @return	1 -- �ཻ��0 -- ���ཻ
	*/
	virtual int RayCrossGeometry(RayImpl& ray, double& dLength) const;

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

	/** �麯��������������ʵ��
	* ���㵱ǰ����ļ�������
	* @return ����ļ�������
	*/
	virtual Vertex3d CalCorePoint();

	/** �ѵ�ǰ����ĳ�Ա�������ݣ������һ��������
	* @param [out] buf	������������
	* @return 1 -- ����ɹ���0 -- ���ʧ��
	*/
	bool writeBuffer(Buffer& buf);

	/** �жϽ�ͷ׵���뵱ǰ�������Ƿ��ཻ
	* @param [in] frustum	����׵��
	* @return	1 -- �ཻ��0 -- ���ཻ
	*/
	int FrustumIntersectGeometry(FrustumImpl& frustum);

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

	/**  ��ѯ����ӿ�
	* @param  [in] signal  �������ͱ�ʶ
	* @param  [out] p ��Ӧ�Ķ���ָ��
	* @return 1 -- ��ѯ�ɹ���0 -- ��ѯʧ��
	*/
	virtual bool queryInterface(int signal, void ** p){
		switch (signal){
		case GEOMTYPE_3D_SOLID:
		{
			*p = (void*)(Solid3d *) this;
			return true;
		}
		case GEOMTYPE_3D_VOLUME:
		{
			*p = (void*)(Volume3d *) this;
			return true;
		}
		default:
		{
			return false;
		}
		}
	}

	virtual void getSurfaces(std::vector<Surface3d*> & f){
		for (std::vector<Surface3dImpl*>::iterator it = m_vBoundarySurfaces.begin(); it != m_vBoundarySurfaces.end(); it++){
			f.push_back((Surface3d*)(*it));
		}
	}
	virtual Surface3d * newSurface(int type);
};

end_gdb_namespace
end_gtl_namespace


