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
#include "internal_TriangleMesh3dImpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
typedef struct tagRINGTOPOLOGY //it's a kind of red-black tree
{
	//
	LinearRing3dImpl* m_ptrRing;
	//
	struct tagRINGTOPOLOGY* m_Parent;
	typedef std::vector<tagRINGTOPOLOGY*> _vectorRINGTOP;
	_vectorRINGTOP m_vectorChildRingPtr;
	//
	tagRINGTOPOLOGY()
	{
		m_ptrRing = NULL;
		m_Parent = NULL;
		m_vectorChildRingPtr.clear();
	}
	~tagRINGTOPOLOGY()
	{

	}
}RINGTOP;
typedef std::vector<RINGTOP*> _vectorRINGTOP;

class Polygon3dImpl : virtual public Polygon3d, virtual public Surface3dImpl
{
	/** @defgroup Polygon3dImpl Polygon3dImpl-��ά�����
	*  @{
	*/

protected:
	/** @name ���ԣ���Ա������
	* @{
	*/

	/** ���Ի����飬��vectorװ��ָ��  */
	typedef std::vector<LinearRing3dImpl*> _vector3DLinearRing;
	_vector3DLinearRing  m_vRings;

	//�ݴ�ת����������
	TriangleMesh3dImpl* m_pTriangleMesh;

	/** @} */ // ���Խ�β
public:
	virtual void getRings(std::vector<LinearRing3d*> & rings){
		for (std::vector<LinearRing3dImpl*>::iterator it = m_vRings.begin(); it != m_vRings.end(); it++)
			rings.push_back((LinearRing3d*)(*it));
	}
	virtual void addRing(LinearRing3d * pRing){
		LinearRing3dImpl * p = dynamic_cast<LinearRing3dImpl *>(pRing);
		if (p) {
			if (m_pTriangleMesh)
			{
				delete m_pTriangleMesh;
				m_pTriangleMesh = NULL;
			}
			m_vRings.push_back(p);
		}
	}
	virtual void setRings(std::vector<LinearRing3d*> & rings){
		std::vector<LinearRing3dImpl*> rs;
		for (std::vector<LinearRing3d*>::iterator it = rings.begin(); it != rings.end(); it++)
		{
			LinearRing3dImpl * p = dynamic_cast<LinearRing3dImpl *>(*it);
			rs.push_back(p);
		}
		SetRings(rs);
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
			case GEOMTYPE_3D_POLYGON:
			{
				*p = (void*)(Polygon3d *) this;
				return true;
			}
			default:
			{
				return false;
			}
		}
	}
	virtual TriangleMesh3d * asTriangleMesh();
public:

	/** @defgroup Polygon3dImpl ��������Ա������
	* @{
	*/


	/** @name ��������������
	*  @{
	*/

	/** ���캯��1
	*/
	Polygon3dImpl()
	{
		m_byteTypeID = GEOTYPE_3D_POLYGON;
		m_pTriangleMesh = NULL;
	}

	/** ���캯��2
	*/
	Polygon3dImpl(vector<LinearRing3dImpl*>& vRings)
	{
		m_byteTypeID = GEOTYPE_3D_POLYGON;
		m_vRings = vRings;
		m_pTriangleMesh = NULL;
	}

	/** ��������
	*/
	virtual ~Polygon3dImpl()
	{
		int nCount = m_vRings.size();
		for (int i = nCount - 1; i>-1; i--)
		{
			if (m_vRings[i])
				delete m_vRings[i];
		}
		m_vRings.clear();

		if (m_pTriangleMesh)
		{
			delete m_pTriangleMesh;
			m_pTriangleMesh = NULL;
		}
	}

	/** @} */ // ����������������β



	/** @name ���ݲ���
	*  @{
	*/

	/** ������Ի�����
	* @param [out] vRings ������ֵ�����Ի����������
	* @return
	*/
	void GetRings(vector<LinearRing3dImpl*>& vRings) const
	{
		vRings = m_vRings;
	}

	/** �������Ի�����
	* @param [in] vRings ��֪�����Ի����������
	* @return
	*/
	void SetRings(const vector<LinearRing3dImpl*>& vRings)
	{
		if (m_pTriangleMesh)
		{
			delete m_pTriangleMesh;
			m_pTriangleMesh = NULL;
		}

		//////////////////////[2011-11-7,15:53 lizhanglin]//////////////+
		//TODO: ���¶�m_vRings���и�ֵ
		if (vRings.size()>0)
		{
			int nCount = m_vRings.size();
			for (int i = nCount - 1; i>-1; i--)
			{
				if (m_vRings[i])
					delete m_vRings[i];
			}
			m_vRings.clear();

			for (int i = 0; i < vRings.size(); i++)
			{
				LinearRing3dImpl* p3DLinearRing = new LinearRing3dImpl;
				p3DLinearRing->copy(vRings[i]);
				m_vRings.push_back(p3DLinearRing);
			}
		}
		//////////////////////[2011-11-7,15:53 lizhanglin]//////////////-

		// ���� [2011-11-7 lizhanglin]
		//m_vRings = vRings;
	}

	/** @} */ // ���ݲ������β



	/** @name �������
	*  @{
	*/
	/** �жϵ��뵱ǰ������Ĺ�ϵ
	* @param [in] vPt	��Ҫ�жϵĵ�
	* @return ����Լ������λ�� 'i'--�ڲ�; 'o'--�ⲿ; 'E'--����; 'V'--����;
	*/
	char  RPointWithGeometry(Vertex3d& vPt) const;

	/** �����ⲿָ������ά����󵽵�ǰ���ζ�������ƽ��
	* @param [in] vPt	Vertex3d������ά����������
	* @return �㵽��ǰ���ζ�������ƽ��
	*/
	double DPointToGeometryEx(Vertex3d& vPt);

	/** �麯��
	* �ж������Ƿ񴩹���ǰ�����壬��������㵽�뼸���彻�����С����
	* @param [in] ray	��������
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

	/** �ѵ�ǰ����ĳ�Ա�������ݣ������һ��������
	* @param [out] buf	������������
	* @return 1 -- ����ɹ���0 -- ���ʧ��
	*/
	bool writeBuffer(Buffer& buf);

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

	/** �麯��������������ʵ��
	* ���㵱ǰ����ļ�������
	* @return ����ļ�������
	*/
	virtual Vertex3d CalCorePoint();

	/** �ѵ�ǰ��Polygonת��Ϊ���������磬�Ѱ������ʺͰ�Χ��(֧�ֶ���⻷�������֧�ֻ����ཻ�����)
	* @param [in] nMode	0--Ĭ��(ֻ����������) 1--�������������Ż��������� 2--����������ͬʱ��������������
	* @param [in] nType	0--Ĭ��(�ڴ������ͷ�) 1--����Mesh���ڴ����ⲿ�ͷ�
	* @param [in] bIsFoldingTest	0--Ĭ��(���л������ཻ����) 1--(�����л������ཻ����)
	* @return ��ǰpolygon�����ɵ���������,������ɹ��򷵻�NULL(!!notice!!��Ҫ��������TriangleMesh3dImpl���󣬷��������ڴ�й¶)
	*/
	TriangleMesh3dImpl* ConvertToMesh(const int nMode, const int nType, bool bIsFoldingTest = true);

private:
	/** ����polygon��ring�����˽ṹ
	* @param [in] ring -- ��Ҫ�����linearing top--���˽ṹ
	* @return true -- ���˽����ɹ�; false -- ���˴���
	*/
	bool topologyRing(LinearRing3dImpl* ring, RINGTOP* top);

	/** �ж� linearRing refRing �Ƿ���� linearRing testRing (�����ǻ��ཻ���)
	* @param [in]
	* @return true -- refRing����testRing; false -- refRing������testRing
	*/
	bool IsContain(LinearRing3dImpl* testRing, LinearRing3dImpl* refRing);

	/** ����polygon��ring���˽ṹ�Ľڵ�
	* @param [in] ring -- ��Ҫ�����linearing top--���˽ṹ
	* @return true -- ���˽����ɹ�; false -- ���˴���
	*/
	bool insertTopNode(LinearRing3dImpl* ring, RINGTOP* top);

	/** ��polygon���ҳ����е����ӵ�
	* @param [in] topPolygon -- polygon�еĻ����˽ṹ
	* @param [out] holemarks -- �����ӵ��б�
	*/
	void getPolygonholemarks(RINGTOP* topPolygon, vector<Vertex3d> &holemarks, Vertex3d& refNormal);

	/** ������˽ṹ
	* @param [in] topPolygon -- polygon�еĻ����˽ṹ
	*/
	void freetopPolygon(RINGTOP* topPolygon);

	/** �Ƚ�����linearRing�����Χ�еĴ�С������a�Ƿ�С��b
	* @param [in] ������Ҫ�ȽϵĻ�
	* @return true -- a<b; false -- a>=b
	*/
	static bool compLinearRingSize(LinearRing3dImpl*, LinearRing3dImpl*);

	/** �ж�ring�еı��Ƿ����ཻ
	* @param [in] verts -- ring�е����ж���
	* @return true -- ���ཻ; false -- �����ཻ
	*/
	bool isRingSelfIntersect(vector<Vertex3d>& verts);

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
