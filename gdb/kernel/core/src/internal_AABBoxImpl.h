
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
#include "envelope3d.h"
#include "mathex.h"
#include "matrix4x4.h"
#include "internal_OBBoxImpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
class  AABBoxImpl
{
	/** @defgroup AABBoxImpl AABBoxImpl-�����Χ��
	*  @{
	*/

protected:

	/** @name ���ԣ���Ա������
	* @{
	*/
	/** ��С�� */
	Vertex3d m_VMin;

	/** ���� */
	Vertex3d m_VMax;



	/** @} */ // ���Խ�β

public:

	int m_nRecPlaneIndex;

	/** @defgroup AABBoxImpl ��������Ա������
	* @{
	*/


	/** @name ��������������
	*  @{
	*/

	/** ���캯��1
	*/
	AABBoxImpl()
	{
		const double big_number = 1e37f;
		m_VMin.x = m_VMin.y = m_VMin.z = big_number;
		m_VMax.x = m_VMax.y = m_VMax.z = -big_number;
		m_nRecPlaneIndex = -1;
	}

	/** ���캯��2
	*/
	AABBoxImpl(const Vertex3d& vmin, const Vertex3d& vmax)
	{
		m_VMin = vmin;
		m_VMax = vmax;
		m_nRecPlaneIndex = -1;
	}

	/** ���캯��3
	*/
	AABBoxImpl(double minx, double miny, double minz, double maxx, double maxy, double maxz)
	{
		m_VMin.x = minx; m_VMin.y = miny; m_VMin.z = minz;
		m_VMax.x = maxx; m_VMax.y = maxy; m_VMax.z = maxz;
		m_nRecPlaneIndex = -1;
	}

	/** ���캯��4
	*/
	AABBoxImpl(const AABBoxImpl& box)
	{
		m_VMax = box.m_VMax;
		m_VMin = box.m_VMin;
		m_nRecPlaneIndex = box.m_nRecPlaneIndex;
	}

	/** @} */ // ����������������β



	/** @name ���ݲ���
	*  @{
	*/

	/** ���AABB����С����λ��(X,Y,Z��С��
	* @return AABB����С����λ��
	*/
	Vertex3d GetMin() const
	{
		return m_VMin;
	}

	/** ���AABB����󶥵�λ��(X,Y,Z���
	* @return AABB����󶥵�λ��
	*/
	Vertex3d GetMax() const
	{
		return m_VMax;
	}

	/** �õ�AABB�����������
	* @param [in] vCorners AABB��������
	*/
	void GetVertices(std::vector<Vertex3d>& vCorners) const
	{
		vCorners.reserve(8);
		for (int i = 0; i<8; i++)
		{
			Vertex3d pt = FetchCornerPoint(i);
			vCorners.push_back(pt);
		}
	}
	/** ����AABB����С����λ��(X,Y,Z��С��
	* @param [in] maxV AABB����󶥵�λ��
	*/
	void SetMax(const Vertex3d& maxV)
	{
		m_VMax = maxV;
	}

	/** ����AABB����С����λ��(X,Y,Z��С��
	* @param [in] minV AABB����С����λ��
	*/
	void SetMin(const Vertex3d& minV)
	{
		m_VMin = minV;
	}

	/** @} */ // ���ݲ������β



	/** @name �������
	*  @{
	*/

	/** ����AABB�ĳ���
	* @return AABB�ĳ���
	*/
	double Length() const;

	/** ����AABB�Ŀ��
	* @return AABB�Ŀ��
	*/
	double Width() const;

	/** ����AABB�ĸ߶�
	* @return AABB�ĸ߶�
	*/
	double Height() const;

	/** ����AABB�Խ��ߵĳ���
	* @return AABB�Խ��ߵĳ���
	*/
	double Diagonal() const
	{
		//return m_VMin.distance(m_VMax);
		Vertex3d v = m_VMax - m_VMin;
		return v.length();
	}
	/** ����AABB��x����ĳ���
	* @return AABB��x����ĳ���
	*/
	double XSize() const
	{
		return m_VMax.x - m_VMin.x;
	}

	/** ����AABB��y����ĳ���
	* @return AABB��y����ĳ���
	*/
	double YSize() const
	{
		return m_VMax.y - m_VMin.y;
	}

	/** ����AABB��z����ĳ���
	* @return AABB��z����ĳ���
	*/
	double ZSize() const
	{
		return m_VMax.z - m_VMin.z;
	}

	double MaxX() const{ return m_VMax.x; }
	double MinX() const{ return m_VMin.x; }
	double MaxY() const{ return m_VMax.y; }
	double MinY() const{ return m_VMin.y; }
	double MaxZ() const{ return m_VMax.z; }
	double MinZ() const{ return m_VMin.z; }

	/** ����AABB�����
	* @return AABB�����
	*/
	double Volume() const
	{
		return XSize() * YSize() * ZSize();
	}

	/** ����AABB�����ĵ�λ��
	* @return AABB�����ĵ�λ��
	*/
	Vertex3d CenterPoint() const
	{
		return (m_VMin + m_VMax) / 2;
	}

	/** ȡ��AABB�İ˸�����֮һ
	* @param [in] iIndex
	*  - 0	Xmin,Ymin,Zmin
	*  - 1	Xmax,Ymin,Zmin
	*  - 2	Xmin,Ymax,Zmin
	*  - 3  Xmax,Ymax,Zmin
	*  - 4	Xmin,Ymin,Zmax
	*  - 5	Xmax,Ymin,Zmax
	*  - 6	Xmin,Ymax,Zmax
	*  - 7  Xmax,Ymax,Zmax
	* @return AABB�İ˸�����֮һ
	*/
	Vertex3d FetchCornerPoint(int iIndex) const;

	/** �����봫��������AABB�ϵĵ�
	* @param [in] vVer �����
	* @return �봫��������AABB�ĵ�
	*/
	Vertex3d GetClosestVertexToAnotherPt(Vertex3d& vVer) const;

	/** ���㴫����ά�߶ε�AABB����С����
	* @param [in] begVer,endVer�����߶ζ˵�
	* @return �봫��������AABB�ĵ�
	*/
	double GetClosestLineDisToAABBBox(Vertex3d& begVer, Vertex3d& endVer) const;

	/** ���㴫�������ε�AABB����С����
	* @param [in] begVer,endVer�����߶ζ˵�
	* @return �봫��������AABB�ĵ�
	*/
	double GetClosestTriangleDisToAABBBox(Vertex3d& triPts1, Vertex3d& triPts2, Vertex3d& triPts3) const;

	/** �Ե�ǰAABB��������ȷ����������㣬������AABB�ڣ�����Ҫ����
	* @param [in] pnt �����
	* @return ���ź��AABB
	*/
	AABBoxImpl Union(const Vertex3d& pnt);

	/** �Ե�ǰAABB��������ȷ����������㼯�����㼯��AABB�ڣ�����Ҫ����
	* @param [in] pnts ����㼯
	* @param [in] nPntNum �����ĸ���
	* @return ���ź��AABB
	*/
	AABBoxImpl Union(const Vertex3d* pnts, int nPntNum);
	AABBoxImpl Union(const std::vector<Vertex3d>& pnts);

	/** �Ե�ǰAABB��������ȷ������������һ��AABBoxImpl��������AABB�ڣ�����Ҫ����
	* @param [in] otherEnvlp �����AABBoxImpl
	* @return ���ź��AABB
	*/
	AABBoxImpl Union(const AABBoxImpl& otherEnvlp);

	/** ���ֵ�ǰAABB�����ĵ㲻�䣬��ԭ�л����ϣ��ֱ���x��y��z�����᷽������һ������
	* @param [in] dx x��������
	* @param [in] dy y��������
	* @param [in] dz z��������
	* @return ���ź��AABB
	*/
	AABBoxImpl Inflate(double dx, double dy, double dz)
	{
		return Inflate(Vertex3d(dx, dy, dz));
	}

	/** ���ֵ�ǰAABB�����ĵ㲻�䣬��ԭ�л����ϣ��ֱ���x��y��z�����᷽������һ������
	* @param [in] vVec x��y��z�����������ϣ�������ʾ��
	* @return ���ź��AABB
	*/
	AABBoxImpl Inflate(const Vertex3d& vVec)
	{
		m_VMin -= vVec;
		m_VMax += vVec;
		Vertex3d vMin = m_VMin;
		Vertex3d vMax = m_VMax;
		m_VMin.x = min(vMin.x, vMax.x);
		m_VMin.y = min(vMin.y, vMax.y);
		m_VMin.z = min(vMin.z, vMax.z);

		m_VMax.x = max(vMin.x, vMax.x);
		m_VMax.y = max(vMin.y, vMax.y);
		m_VMax.z = max(vMin.z, vMax.z);

		return *this;
	}

	/** ���ֵ�ǰAABB�����ĵ㲻�䣬��ԭ�л����ϣ��ֱ���x��y��z�����᷽���Сһ������
	* @param [in] dx x��������
	* @param [in] dy y��������
	* @param [in] dz z��������
	* @return ��С���AABB
	*/
	AABBoxImpl Deflate(double dx, double dy, double dz)
	{
		return Deflate(Vertex3d(dx, dy, dz));
	}

	/** ���ֵ�ǰAABB�����ĵ㲻�䣬��ԭ�л����ϣ��ֱ���x��y��z�����᷽���Сһ������
	* @param [in] vVec x��y��z�����������ϣ�������ʾ��
	* @return ��С���AABB
	*/
	AABBoxImpl Deflate(const Vertex3d& vVec)
	{
		return Inflate(-vVec);
	}

	/** ����AABBΪ�գ�������С���λ����Ϊ������������λ����Ϊ�������
	*/
	inline void Empty()
	{
		const double big_number = 1e37f;
		m_VMin.x = m_VMin.y = m_VMin.z = big_number;
		m_VMax.x = m_VMax.y = m_VMax.z = -big_number;
	}


	/** �жϵ�ǰAABB�Ƿ�Ϊ��
	* @return �Ƿ�Ϊ��
	*/
	int IsEmpty() const
	{
		//	return ((m_VMin.x > m_VMax.x) || (m_VMin.y > m_VMax.y) || (m_VMin.z > m_VMax.z)); //xx_0128
		return (((m_VMin.x - m_VMax.x) > MathEx::TOL) ||
			((m_VMin.y - m_VMax.y) > MathEx::TOL) ||
			((m_VMin.z - m_VMax.z) > MathEx::TOL));
	}

	/** �Ե�ǰAABBƽ��һ������
	* @param [in] vVec ƽ������
	* @return	ƽ�ƺ��AABB
	*/
	AABBoxImpl Offset(const Vertex3d& vVec)
	{
		m_VMax += vVec;
		m_VMin += vVec;
		return *this;
	}

	/** ����ǰAABB�봫���AABB�����󽻣����õ��󽻺��AABB
	* @param [in] otherbox �����AABB
	* @param [out] retBox �󽻺�õ���AABB
	* @return	1 -- �ཻ��0 -- ���룻2 -- ������ 3 -- ��������
	*/
	int  Intersect(const AABBoxImpl& otherbox, AABBoxImpl& retBox) const;

	/** �ж�һ�����Ƿ��ڰ�Χ����
	* @param [in] vPt �����
	* @return	1 -- �ڰ�Χ���ڣ�0 -- �ڰ�Χ���⣻
	*/
	int IsPointInBox(const Vertex3d& vPt) const;

	/** ��ǰAABBBOX �봫���AABBBOX �Ƿ��ཻ
	* @param [in] inBox �����������
	* @return	1 -- �ڰ�Χ���ڣ�0 -- �ڰ�Χ���⣻
	*/
	int IsBoxIntersectWithGivenBox(const AABBoxImpl& inBox) const;

	/** ��ǰAABBBOX �봫���AABBBOX �ڸ��������ͶӰ�Ƿ��ཻ
	* @param [in] inBox		������������
	* @param [in] proAxis	����ͶӰ��,ȡֵ��x����y����z��
	* @return	1 -- ͶӰ�ཻ�����ٽӣ���������0 -- ͶӰ���ཻ��
	*/
	int IsBoxIntersectWithGivenBox2D(const AABBoxImpl& inBox, char proAxis = 'z', AABBoxImpl* intersectBox = NULL) const;

	/** ���ز�����"=",���ڶԵ�ǰAABB�ĸ�ֵ
	* @param [in] rha �����AABB
	* @return ��ֵ���AABB
	*/
	AABBoxImpl& operator=(const AABBoxImpl& rha)
	{
		m_VMax = rha.m_VMax;
		m_VMin = rha.m_VMin;
		return (*this);
	}

	/** ���ز�����"==",�жϴ���AABB�Ƿ��뵱ǰAABB���
	* @param [in] otherbox �����AABB
	* @return 1 -- ��ȣ�0 -- ����
	*/
	int operator==(const AABBoxImpl& otherbox) const
	{
		return m_VMax == otherbox.m_VMax && m_VMin == otherbox.m_VMin;
	}

	
	/** ��AABB���ĵ���ת���γɵ��µ�AABB
	* @param [in] matrix ��ʾ��ת��4*4��λ����
	* @return ��AABB���ĵ���ת��õ���AABB
	*/
	AABBoxImpl  ComputeNewAABB(const Matrix4x4& matrix)
	{
		matrix.transformAffine(m_VMin);
		matrix.transformAffine(m_VMax);
		return *this;
	}

	/** �жϵ�ǰAABB������������2D�ϵĹ�ϵ���������ཻ������
	* @param [in] ptArray ���������
	* @return 1 -- ͶӰ�ཻ�����ٽӣ���������0 -- ͶӰ���ཻ��
	*/
	int IsBoxintersectWithPolygon2D(vector<Vertex3d>& ptArray);

	/** @} */ // ����������β

	/** @} */ // ������β

	/** @} */ // ģ���β
};
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
