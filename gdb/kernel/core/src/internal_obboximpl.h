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

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
class OBBoxImpl
{
	/** @defgroup OBBoxImpl OBBoxImpl-�����Χ��
	*  @{
	*/

protected:
	/** @name ���ԣ���Ա������
	* @{
	*/

	//##ModelId=49E347290280
	/** ��С�� */
	Vertex3d m_VMin;

	//##ModelId=49E347290290
	/** ���� */
	Vertex3d m_VMax;

	//##ModelId=49E34729029F
	/** �����Ծ����ʾ */
	Matrix3x3 m_vOrientation;

	/** @} */ // ���Խ�β

public:

	/** @name ��������������
	*  @{
	*/

	/** ���캯��1
	*/
	OBBoxImpl()
	{
		const double big_number = 1e37f;
		m_VMin.x = m_VMin.y = m_VMin.z = big_number;
		m_VMax.x = m_VMax.y = m_VMax.z = -big_number;
	}

	/** ���캯��2
	*/
	OBBoxImpl(const Vertex3d& vmin, const Vertex3d& vmax, const Matrix3x3& matrix)
	{
		m_VMin = vmin;
		m_VMax = vmax;
		m_vOrientation = matrix;
	}

	/** ���캯��3
	*/
	OBBoxImpl(const OBBoxImpl& box)
	{
		m_VMax = box.m_VMax;
		m_VMin = box.m_VMin;
		m_vOrientation = box.m_vOrientation;
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

	/** ����AABB����󶥵�λ��(X,Y,Z��С��
	* @return AABB����󶥵�λ��
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

	/** ���AABB�ĳ���
	* @return AABB�ĳ���
	*/
	Matrix3x3 GetOrientation() const
	{
		return m_vOrientation;
	}

	/** ����AABB�ĳ���
	* @param [in] matrix AABB�ĳ���
	*/
	void SetOrientation(const Matrix3x3& matrix)
	{
		m_vOrientation = matrix;
	}

	/** @} */ // ���ݲ������β


	/** @name �������
	*  @{
	*/

	//##ModelId=49E347290271
	/** ����OBB�ĳ���
	* @return OBB�ĳ���
	*/
	double Length() const;

	//##ModelId=49E3472902B0
	/** ����OBB�Ŀ��
	* @return OBB�ĳ���
	*/
	double Width() const;

	//##ModelId=49E3472902B1
	/** ����OBB�Ŀ��
	* @return OBB�ĸ߶�
	*/
	double Height() const;

	//##ModelId=49EC8E1F035B
	/** ����OBB�����ĵ�λ��
	* @return OBB�����ĵ�
	*/
	Vertex3d CenterPoint() const;


	//##ModelId=49EF0C1401B5
	/** ���㵱ǰOBB�봫��OBB�Ƿ��ཻ
	* @return 1 -- �ཻ��0 -- ���ཻ
	*/
	int IsOBBoxIntersect(const OBBoxImpl& obb) const;

	//##ModelId=49EF0C1401B5
	/** ���㴫����Ƿ�λ�ڵ�ǰOBB��
	* @return 1 -- �����ڣ�0 -- ��������
	*/
	int IsPointInOBB(const Vertex3d& vPt) const;


	/** @}*/  // ����������β

	/** @} */ // ģ���β
};

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
