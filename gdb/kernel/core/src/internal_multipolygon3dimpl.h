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
#include "internal_linearring3dimpl.h"
#include "internal_surface3dimpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

class TriangleMesh3dImpl;

class MultiPolygon3dImpl : virtual public MultiPolygon3d, virtual public Surface3dImpl
{
	/** @defgroup MultiPolygon3dImpl MultiPolygon3dImpl-��ά����
	*  @{
	*/

protected:
	/** @name ���ԣ���Ա������
	* @{
	*/

	/** ����θ��� */
	long m_lPolygonNum;

	/** ÿ������εĶ����� */
	long* m_pPolygonVertexNums;

	/** ����ζ��������б� */
	long* m_pPolygonVertexIndexs;

	//�ݴ�ת����������
	TriangleMesh3dImpl* pTempMesh;
	TriangleMesh3dImpl* m_pTriangleMesh;

	/* @brief ��ʱ�߻�����*/
	std::vector<LinearRing3dImpl*> m_vRings;

	/** @} */ // ���Խ�β
public:
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
		case GEOMTYPE_3D_MULTIPOLYGON:
		{
			*p = (void*)(MultiPolygon3d *) this;
			return true;
		}
		default:
		{
			return false;
		}
		}
	}
	virtual Surface3d * asSurface() {
		return dynamic_cast<Surface3d*>((Surface3dImpl*)this);
	}
	virtual long getPolygonNumber() {
		return m_lPolygonNum;
	}
	virtual void getPolygonIndexBuffer(IndexBuffer & ib) {
		long sum = 0;
		for (int i = 0; i<m_lPolygonNum; i++){
			sum += m_pPolygonVertexNums[i];
		}
		ib.clear();
		ib.add(m_pPolygonVertexIndexs, sum);
	}
	virtual void getVertexNumberPerPolygon(IndexBuffer & ib){
		ib.clear();
		ib.add(m_pPolygonVertexNums, m_lPolygonNum);
	}

	// ����������Ϣ������������θ��� ÿ������εĶ����� ����ζ��������б�
	virtual void setPolyTopos(const long PolygonNumber, const std::vector<long> & VertexNumberPrePolygon, const std::vector<long> & VertexIndices)
	{
		if (PolygonNumber <1 || VertexNumberPrePolygon.size()<1 || VertexIndices.size()<1)
		{
			return;
		}

		if (m_pPolygonVertexNums)
		{
			delete[]m_pPolygonVertexNums;
			m_pPolygonVertexNums = NULL;
		}
		if (m_pPolygonVertexIndexs)
		{
			delete[]m_pPolygonVertexIndexs;
			m_pPolygonVertexIndexs = NULL;
		}

		m_lPolygonNum = PolygonNumber;
		m_pPolygonVertexNums = new long[VertexNumberPrePolygon.size()];
		std::copy(VertexNumberPrePolygon.begin(), VertexNumberPrePolygon.end(), m_pPolygonVertexNums);

		m_pPolygonVertexIndexs = new long[VertexIndices.size()];
		std::copy(VertexIndices.begin(), VertexIndices.end(), m_pPolygonVertexIndexs);
	}

public:

	/** @defgroup MultiPolygon3dImpl ��������Ա������
	* @{
	*/


	/** @name ��������������
	*  @{
	*/

	/** ���캯��
	*/
	MultiPolygon3dImpl()
	{
		m_byteTypeID = GEOTYPE_3D_MULTIPOLYGON;
		m_lPolygonNum = 0;
		m_pPolygonVertexNums = NULL;
		m_pPolygonVertexIndexs = NULL;
		pTempMesh = NULL;
		m_pTriangleMesh = NULL;
	}

	/** ��������
	*/
	virtual ~MultiPolygon3dImpl()
	{
		if (m_pPolygonVertexNums)
		{
			delete[]m_pPolygonVertexNums;
			m_pPolygonVertexNums = NULL;
		}
		if (m_pPolygonVertexIndexs)
		{
			delete[]m_pPolygonVertexIndexs;
			m_pPolygonVertexIndexs = NULL;
		}
		if (m_pTriangleMesh)
		{
			delete m_pTriangleMesh;
			m_pTriangleMesh = NULL;
		}
		if (pTempMesh)
		{
			delete pTempMesh;
			pTempMesh = NULL;
		}

		for (int i = 0; i < m_vRings.size(); i++)
		{
			if (m_vRings[i])
			{
				delete m_vRings[i];
				m_vRings[i] = NULL;
			}
			m_vRings.clear();
		}
	}

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

	/** ���ö���θ���
	* @param [in] lPolygonNum	����θ���
	* @return
	*/
	void SetPolygonNum(long lPolygonNum)
	{
		m_lPolygonNum = lPolygonNum;
	}

	/** ��ö���θ���
	* @param [out] m_lPolygonNum ����θ���
	* @return
	*/
	long GetPolygonNum() const
	{
		return m_lPolygonNum;
	}

	/** ����MultiPolygon�ж���θ�����ÿ������ζ�����
	* @param [in] pPolygonVertexNums  ÿ������ζ�����
	* @param [in] lPolygonNum ����θ���
	* @return
	*/
	void SetPolygonVertexNums(long* const pPolygonVertexNums, long lPolygonNum)
	{
		m_lPolygonNum = lPolygonNum;
		m_pPolygonVertexNums = pPolygonVertexNums;
	}

	/** ���MultiPolygon�ж���θ�����ÿ������ζ�����
	* @param [out] pPolygonVertexNums ÿ������ζ�����
	* @param [out] lPolygonNum ����θ���
	* @return
	*/
	void GetPolygonVertexNums(long*& pPolygonVertexNums, long& lPolygonNum) const
	{
		lPolygonNum = m_lPolygonNum;
		pPolygonVertexNums = m_pPolygonVertexNums;
	}

	/** ����MultiPolygon�ж���θ�����ÿ������ζ�����
	* @param [in] pPolygonVertexNums  ÿ������ζ�����
	* @param [in] lPolygonNum ����θ���
	* @return
	*/
	void SetPolygonVertexNumsEx(vector<long> const pPolygonVertexNums)
	{
		m_lPolygonNum = pPolygonVertexNums.size();
		if (m_pPolygonVertexNums != NULL)
		{
			delete m_pPolygonVertexNums;
			m_pPolygonVertexNums = NULL;
		}

		m_pPolygonVertexNums = new long[m_lPolygonNum];
		for (long i = 0; i<m_lPolygonNum; i++)
		{
			m_pPolygonVertexNums[i] = pPolygonVertexNums.at(i);
		}
	}

	/** ���MultiPolygon�ж���θ�����ÿ������ζ�����
	* @param [out] pPolygonVertexNums ÿ������ζ�����
	* @param [out] lPolygonNum ����θ���
	* @return
	*/
	void GetPolygonVertexNumsEx(vector<long>& pPolygonVertexNums) const
	{
		pPolygonVertexNums.reserve(m_lPolygonNum);
		for (long i = 0; i<m_lPolygonNum; i++)
		{
			pPolygonVertexNums.push_back(m_pPolygonVertexNums[i]);
		}
	}

	/** ���ö���������š�ÿ���������������������θ���
	* @param [in] pPolygonVertexIndexs  �������������
	* @param [in] pPolygonVertexNums ÿ���������������
	* @param [in] lPolygonNum ����θ���
	* @return
	*/
	void SetPolygonVertexIndexs(long* const pPolygonVertexIndexs, long* const pPolygonVertexNums, long lPolygonNum)
	{
		m_lPolygonNum = lPolygonNum;
		m_pPolygonVertexNums = pPolygonVertexNums;
		m_pPolygonVertexIndexs = pPolygonVertexIndexs;
	}

	/** ��ö���������š�ÿ���������������������θ���
	* @param [out] pPolygonVertexIndexs  �������������
	* @param [out] pPolygonVertexNums ÿ���������������
	* @param [out] lPolygonNum ����θ���
	* @return
	*/
	void GetPolygonVertexIndexs(long*& const pPolygonVertexIndexs, long*& const pPolygonVertexNums, long& lPolygonNum) const
	{
		lPolygonNum = m_lPolygonNum;
		pPolygonVertexNums = m_pPolygonVertexNums;
		pPolygonVertexIndexs = m_pPolygonVertexIndexs;
	}

	/** ��ö������������
	* @return
	*/
	long GetPolygonIndexNum() const
	{
		long NUM = 0;
		long i = 0;
		for (i = 0; i < m_lPolygonNum; i++)
		{
			NUM += m_pPolygonVertexNums[i];
		}
		return NUM;
	}

	/** ��ö���������š�ÿ���������������������θ���
	* @param [out] pPolygonVertexIndexs  �������������
	* @param [out] pPolygonVertexNums  ÿ���������������
	* @return
	*/
	void GetPolygonVertexIndexsEx(vector<long>& pPolygonVertexIndexs, vector<long>& pPolygonVertexNums) const
	{
		pPolygonVertexNums.reserve(m_lPolygonNum);
		long i = 0;
		for (i = 0; i < m_lPolygonNum; i++)
		{
			pPolygonVertexNums.push_back(m_pPolygonVertexNums[i]);
		}

		long Num = GetPolygonIndexNum();
		pPolygonVertexIndexs.reserve(Num);
		for (i = 0; i < Num; i++)
		{
			pPolygonVertexIndexs.push_back(m_pPolygonVertexIndexs[i]);
		}
	}

	/** �ѵ�ǰ��MultiPolygonת��Ϊ����������_WTS_0408
	* @param [in] nMode	0--Ĭ��(ֻ����������) 1--����������ͬʱ�������������� ����ǰֻ��Ϊ0��
	* @param [in] nType	0--Ĭ��(���й����ڴ�) 1--�ⲿ�������ɵ��ڴ�
	* @return ��ǰpolygon�����ɵ���������,������ɹ��򷵻�NULL(!!notice!!��Ҫ��������TriangleMesh3dImpl�����ڴ�)
	*/
	TriangleMesh3dImpl* ConvertToMesh(const int nMode, const int nType);

	/** ת����ǰmultipolygonΪ���Ring_WTS_0408
	* @param [in]
	* @param [out] m_pRings ��ָ������
	* @return �ɹ�����TRUE
	*/
	bool getRings(std::vector<LinearRing3dImpl*>& m_pRings);

	//##ModelId=49E6F2CE00EA
	/** ��ȡ�������
	* @param
	* @return ���
	*/
	double Area();

	//##ModelId=49E6F3AA002E
	/** ��ȡ�����弸���ϵ������
	* @param
	* @return �����
	*/
	unsigned int NumberGeometry();

	//##ModelId=49E7368D01E4
	/** ��ȡ���������ߴ�
	* @param
	* @return ���������ߴ�
	*/
	unsigned int GetIndexSize();

	//##ModelId=49E736FC0109
	/** ��ȡ��������������
	* @param
	* @return ��������������
	*/
	unsigned int GetDataSize();

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
