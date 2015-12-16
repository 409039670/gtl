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
#include <deque>
#include "geometry3d.h"
#include "internal_Surface3dImpl.h"
#include "internal_LinearRing3dImpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
class Polygon3dImpl;
class TriangleMesh3dImpl
	: virtual public TriangleMesh3d, virtual public Surface3dImpl
{
	/** @defgroup TriangleMesh3dImpl TriangleMesh3dImpl-����������
	*  @{
	*/
private:
	/** �����α�����ʾ�б� ��0-û�б��� 1-������������Ϊ�����θ��� */
	unsigned int* m_uiTriangleMark;

	/** ��������м�¼�ı߽�ı�(�洢ÿ�����������������)*/
	std::vector<long> m_vectBoundaryEdges;

	/**�洢�ñ��Ƿ��Ѿ�������0-û�б��� 1-������,�����еĳ��ȵ��ڱ߽糤�ȵ�һ��*/
	unsigned int* m_uiBoundEdgeMark;

protected:
	/** ������������ */
	long* m_lTriangleList;

	/** �������������ȣ�����ָ�򶥵��б�����Ϊ�����θ�����3�� */
	long m_lTriListSize;

	/** ���������������� */
	long* m_lStripIndices;

	/** �����������������ȣ�����ָ�򶥵��б�������Ϊ�����θ�������2 */
	long m_lStripListSize;

	/** @} */ // ���Խ�β
public:
	/**  ��ȡ������������Žṹ
	* @param  [in] ib  ������������Žṹbuffer
	* @param  [out] ib  ������������Žṹbuffer
	* @return ��
	*/
	virtual void getTriangleIndexBuffer(IndexBuffer & ib){
		ib.clear();
		ib.add(m_lTriangleList, m_lTriListSize);
	}
	/**  ��ȡ����������������Žṹ
	* @param  [in] ib  ����������������Žṹbuffer
	* @param  [out] ib ����������������Žṹbuffer
	* @return ��
	*/
	virtual void getTriangleStripIndexBuffer(IndexBuffer & ib){
		ib.clear();
		ib.add(m_lStripIndices, m_lStripListSize);
	}
	/**  ��ѯ����ӿ�
	* @param  [in] signal  �������ͱ�ʶ
	* @param  [out] p ��Ӧ�Ķ���ָ��
	* @return 1 -- ��ѯ�ɹ���0 -- ��ѯʧ��
	*/
	virtual bool queryInterface(int signal, void ** p){
		switch (signal){
		case GEOMTYPE_3D_SURFACE:
		{
			*p = (void*)(Surface3d *) this;
			return true;
		}
		case GEOMTYPE_3D_TRIANGLEMESH:
		{
			*p = (void*)(TriangleMesh3d *) this;
			return true;
		}
		default:
		{
			return false;
		}
		}
	}

	/** ������������������������
	* @param [in] lTriangleList  ��������������
	* @param [in] lTriListSize ��������������
	* @return
	*/
	virtual void setTriangles(long* const lTriangleList, long lTriListSize)
	{
		SetTriangles(lTriangleList, lTriListSize);
	}
public:

	/** @defgroup TriangleMesh3dImpl ��������Ա������
	* @{
	*/


	/** @name ��������������
	*  @{
	*/

	/** ���캯��
	*/
	TriangleMesh3dImpl();
	/** ��������
	*/
	virtual ~TriangleMesh3dImpl();

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

	/** ���ö����б�
	* @param [in]  vVertexList	�����б�
	* @return
	*/
	void SetVertices(void* const vVertexList)
	{
		m_vVertices = vVertexList;
	}

	/** ���ö����б�
	* @param [in] vVertexList	�����б�
	* @param [in] lVertNum	�����б���
	* @return
	*/
	void SetVertices(void* const vVertexList, long lVertNum)
	{
		m_vVertices = vVertexList;
		m_lVertNum = lVertNum;
	}

	/** ���ö����б���
	* @param [in] lVertNum	 �����б���
	* @return
	*/
	void SetVerticesNum(long lVertNum)
	{
		m_lVertNum = lVertNum;
	}

	/** ��ö����б�1
	* @param [out] vVertexList �����б�
	* @return
	*/
	void GetVertices(void*& vVertexList) const
	{
		vVertexList = m_vVertices;
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

	/** ��ö����б���
	* @param
	* @return
	*/
	long GetVerticesNum() const
	{
		return m_lVertNum;
	}

	/** ��ö����б�2
	* @param [out]  vt3d �����б�
	* @return
	*/
	void GetVerticesEx(vector<Vertex3d>& vt3d) const;

	/** �������������
	* @param [out] tr3d  ��������������
	* @return
	*/
	void GetTriangles(vector<long>& tr3d)
	{
		tr3d.reserve(m_lTriListSize);
		for (int i = 0; i<m_lTriListSize; i++)
		{
			tr3d.push_back(m_lTriangleList[i]);
		}
	}

	/** �����������������
	* @param [out] st3d  ������������������
	* @return
	*/
	void GetStrips(vector<long>& st3d)
	{
		st3d.reserve(m_lStripListSize);
		for (int i = 0; i<m_lStripListSize; i++)
		{
			st3d.push_back(m_lStripIndices[i]);
		}
	}

	/** ������������������������
	* @param [in] lTriangleList  ��������������
	* @param [in] lTriListSize ��������������
	* @return
	*/
	void SetTriangles(long* const lTriangleList, long lTriListSize)
	{
		m_lTriListSize = lTriListSize;
		m_lTriangleList = lTriangleList;
	}

	/** �����������������������
	* @param [out] lTriangleList ��������������
	* @param [out] lTriListSize ��������������
	* @return
	*/
	void GetTriangles(long*& lTriangleList, long& lTriListSize) const
	{
		lTriListSize = m_lTriListSize;
		lTriangleList = m_lTriangleList;
	}

	/** ����������������������������
	* @param [in] lTriangleList  ������������������
	* @param [in] lTriListSize ������������������
	* @return
	*/
	void SetStrips(long* const lStripIndices, long lStripListSize)
	{
		m_lStripIndices = lStripIndices;
		m_lStripListSize = lStripListSize;
	}

	/** ���������������������������
	* @param [out] lTriangleList ������������������
	* @param [out] lTriListSize ������������������
	* @return
	*/
	void GetStrips(long*& lStripIndices, long& lStripListSize) const
	{
		lStripIndices = m_lStripIndices;
		lStripListSize = m_lStripListSize;
	}

	/** �ѵ�ǰ��TriangleMeshת��ΪPolygon�ṹ���Ѱ������ʺͰ�Χ��(�����ǰ�������⻷�����)
	* @param [in] nMode	0--(���ɾ�������⻷Polygon�ṹ) 1--(���ٽ�ÿ��������ת��Ϊһ��Polygon)
	* @param [out] resPolySoup -- ת�������ɵ�polygon����(!!Notice!!ʹ�ú�Ҫ������Polygon�����������ڴ�й¶)
	* @return true -- ת���ɹ� false -- ת��ʧ��
	*/
	bool ConvertToPoly(const int nMode, vector<Polygon3dImpl*> &resPolySoup);

private:

	/** �������ӵ㽫�����ν��з���
	* @param [in] lTri ��lTri�������Σ�!!notice!!�������ζ���������
	* @return true -- ����ɹ� false -- ����ʧ��
	*/
	bool GrowTriangleGroup(long lTri);

	/** ��m_lBoundaryEdges�еı߽����飬����polygon����
	* @param [out] newPly ���ɵĶ���ζ���
	* @return true -- ����ɹ� false -- ����ʧ��
	*/
	bool TrackBoundaries(Polygon3dImpl*& newPly);

	/** �������ӱ�����Ѱ�ҹ��������Σ��ҵ�������v1v2��������oriTri�Ĺ��������Σ�
	* @param [in] vNormal �����淨��
	* @param [in] oriTri ��������������
	* @param [in] v1 �����α߽綥��1����
	* @param [in] v2 �����α߽綥��2����
	* @return true -- �ҵ����������� false -- û���ҵ�����������
	*/
	bool TrackTriangleByEdge(Vertex3d vNormal, long oriTri, long v1, long v2);

	/** �������Ӷ�������Ѱ�ұ�
	* @param [in] toVert to����
	* @param [in] frmVert from����
	* @param [in] m_vectRing һ�����еĶ���
	* @return
	*/
	void TrackEdgeByVert(long toVert, long frmVert, std::deque<long>& m_vectRing);

	/** ���ݶ������������µ�LinearRing
	* @param [in] m_vectRingVerts ����������
	* @param [in] m_iVertexType ��������
	* @param [in] m_OutorIn ��������(0--�⻷ 1--�ڻ�)
	* @return ���ɵ��µ�LinearRing��ָ��
	*/
	LinearRing3dImpl* GenNewLinearRing(std::deque<long>& m_vectRingVerts, unsigned char m_iVertexType, int m_OutorIn);

	/** �����������������������η���
	* @param [in] lTri ��lTri�������Σ�!!notice!!�������ζ���������
	* @return ��õķ���
	*/
	Vertex3d GetTriNormal(long lTri);
public:
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

	//##ModelId=49E3472801C5
	/** ������������
	* @return �������ɹ����
	*/
	int Strip();

	//##ModelId=49EF1556000F
	/**
	*	��������
	*/
	int Fan();

	/** �����������������Ż�(�����������ζ������������ֵ���������θ���+2�������ʹ��)
	* @return �б��Ż��ɹ����
	*/
	int OptimizeList();

	/** �жϵ��뵱ǰ������Ĺ�ϵ
	* @param [in] vPt	��Ҫ�жϵĵ�
	* @return ����Լ������λ�� 'i'--�ڲ�; 'o'--�ⲿ; 'E'--����; 'V'--����; 'F'--��;
	*/
	char RPointWithGeometry(Vertex3d& vPt) const;

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
	* @param [in] precisionFlag ���ȱ�־λ 0-������� 1-��͹�� 2-����������
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
