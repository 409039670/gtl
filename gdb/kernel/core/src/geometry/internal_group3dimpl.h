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
#include "internal_geometry3dimpl.h"


begin_gtl_namespace
begin_gdb_namespace
class  Group3dImpl :  public Geometry3dImpl, virtual public Group3d
{
	/** @defgroup Group3dImpl Group3dImpl-���������
	*  @{
	*/

protected:
	/** @name ���ԣ���Ա������
	* @{
	*/

	/** ���ζ��󼯺ϣ���vectorΪ��������ȡ���ζ����ָ�� */
	std::vector<Geometry3dImpl*> m_vGeoCollection;

	/** @} */ // ���Խ�β

public:

	/** @defgroup Group3dImpl ��������Ա������
	* @{
	*/


	/** @name ��������������
	*  @{
	*/

	/** ���캯��1
	*/
	Group3dImpl()
	{
		m_byteTypeID = GEOTYPE_3D_GROUP;
	}

	/** ���캯��2
	*/
	Group3dImpl(const vector<Geometry3dImpl*>& geoCollection)
	{
		m_byteTypeID = GEOTYPE_3D_GROUP;
		m_vGeoCollection = geoCollection;
	}

	/** ���캯��3
	*/
	Group3dImpl(Geometry3dImpl* const geoCollection, int nNum)
	{
		m_byteTypeID = GEOTYPE_3D_GROUP;
		m_vGeoCollection.reserve(nNum);
		for (int i = 0; i<nNum; i++)
			m_vGeoCollection.push_back(&geoCollection[i]);
	}

	/** ��������
	*/
	virtual ~Group3dImpl()
	{
		Clear();
	}

	/** @} */ // ����������������β



	/** @name ���ݲ���
	*  @{
	*/
	/**  ��ȡ�ӵ�Ԫ�б�
	* @param  [in] f  �ӵ�Ԫ�б�
	* @param  [out] f �ӵ�Ԫ�б�
	* @return ��
	*/
	virtual void getElements(std::vector<Geometry*> & f){
		for (std::vector<Geometry3dImpl*>::iterator it = m_vGeoCollection.begin(); it != m_vGeoCollection.end(); it++){
			f.push_back((Geometry*)(*it));
		}
	}
	/** ��洢���ζ���ָ��������������һ��Ԫ��
	* @param [in] pGeometry	����ӵļ��ζ����ָ��
	* @return Ԫ���������е����
	*/
	virtual int addElement(Geometry* const pGeometry)
	{
		return Insert(dynamic_cast<Geometry3dImpl*>(pGeometry));
	}
	/** ��մ洢���ζ���ָ����������ͷ�������Ԫ��ָ�������ڴ�(��ʱ���ͷ��ڴ�)
	* @param  ��
	* @return ��
	*/
	virtual void removeAll()
	{
		RemoveAll();
	}
	/** ��մ洢���ζ���ָ����������ͷ�������Ԫ��ָ�������ڴ�(��ʱ���ͷ��ڴ�)
	* @param  ��
	* @return ��
	*/
	virtual void clear()
	{
		Clear();
	}
	/**  ��ѯ����ӿ�
	* @param  [in] signal  �������ͱ�ʶ
	* @param  [out] p ��Ӧ�Ķ���ָ��
	* @return 1 -- ��ѯ�ɹ���0 -- ��ѯʧ��
	*/
	virtual bool queryInterface(int signal, void ** p){
		switch (signal){
		case GEOMTYPE_3D_GROUP:
		{
			*p = (void*)(Group3d *) this;
			return true;
		}
		default:
		{
			return false;
		}
		}
	}

	/** ���ü��������
	* @param [in] color	��֪���������
	* @return
	*/
	void SetCollection(const vector<Geometry3dImpl*>& geoCollection)
	{
		m_vGeoCollection = geoCollection;
	}

	/** ��洢���ζ���ָ����������������
	* @param [in] geoCollection	ָ��һ�����ζ����ָ��
	* @param [in] nNum	����ӵļ��ζ������
	* @return
	*/
	void SetCollection(Geometry3dImpl* const geoCollection, int nNum)
	{
		m_vGeoCollection.clear();
		m_vGeoCollection.reserve(nNum);
		for (int i = 0; i<nNum; i++)
			m_vGeoCollection.push_back(&geoCollection[i]);
	}

	/** ��ô洢���ζ���ָ������������
	* @param  geoCollection  ����ֵ�Ĵ洢���ζ���ָ������������
	* @return
	*/
	void GetCollection(vector<Geometry3dImpl*>& geoCollection) const
	{
		geoCollection = m_vGeoCollection;
	}

	/** ��洢���ζ���ָ��������������һ��Ԫ��
	* @param [in] pGeometry	����ӵļ��ζ����ָ��
	* @return
	*/
	int Insert(Geometry3dImpl* const pGeometry)
	{
		m_vGeoCollection.push_back(pGeometry);
		m_eEnvelope.Union(pGeometry->getEnvelope());
		return 1;
	}

	/** ��մ洢���ζ���ָ����������ͷ�������Ԫ��ָ�������ڴ�(��ʱ���ͷ��ڴ�)
	* @param
	* @return
	*/
	void RemoveAll()
	{
		m_vGeoCollection.clear();
	}

	/** ��մ洢���ζ���ָ����������ͷ�������Ԫ��ָ�������ڴ�(��ʱ���ͷ��ڴ�)
	* @param
	* @return
	*/
	void Clear()
	{
		int nCount = m_vGeoCollection.size();
		for (int i = nCount - 1; i>-1; i--)
		{
			if (m_vGeoCollection[i])
			{
				delete m_vGeoCollection[i];
			}
		}
		RemoveAll();
	}

	/** @} */ // ���ݲ������β


	/** @name �������
	*  @{
	*/

	/** �ж��ⲿָ������ά������뵱ǰ���ζ���Ŀռ��ϵ
	* @param [in]  vPt Vertex3d������ά����������
	* @return 1--���ڵ�ǰ���ζ�����ڲ���߽磻0--���ڵ�ǰ���ζ����ⲿ
	*/
	int IsPointInGeometry(Vertex3d& vPt) const;

	/** �����ⲿָ������ά����󵽵�ǰ���ζ�������ƽ��
	* @param [in] vPt	Vertex3d������ά����������
	* @return ָ���㵽��ǰ���ζ�������ƽ��
	*/
	double DPointToGeometryEx(Vertex3d& vPt) const;

	/** �麯��
	* �ж������Ƿ񴩹���ǰ�����壬��������㵽�뼸���彻�����С����
	* @param [in] vPt1	�������
	* @param [in] vPt2	���߷������Ե�(��vPt1��ͬ�������߷���)
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
	* @return ��ǰ���������ڴ�Ĵ�С
	*/
	size_t sizeBuffer();

	/** �ӻ������н�����ǰ���󣬲�����Ա������ֵ
	* @param [in] buf	������Buffer���������
	* @return 1 -- �����ɹ���0 -- ����ʧ��
	*/
	bool readBuffer(Buffer & buf);

	/** �ѵ�ǰ����ĳ�Ա�������ݣ������һ��������
	* @param [out] buf	������Buffer������
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
	* @return �߶ε���ǰ���ζ�������ƽ��
	*/
	virtual double D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt);

	/** �麯��������������ʵ��
	* �����ⲿָ���������ε���ǰ���ζ�������ƽ��
	* @param [in] triPt1, triPt2, triPt3Vertex3d������ά����������(��ʾ��������������)
	* @return �����ε���ǰ���ζ�������ƽ��
	*/
	virtual double D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3);

	/** �麯��
	* �жϽ�ͷ׵���뵱ǰ�������Ƿ��ཻ
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

};

end_gdb_namespace
end_gtl_namespace


