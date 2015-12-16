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
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include "matrix.h" 

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
class Quaternion;
/** @addtogroup math  Matrix3x3
*  @{
*/
class CORE_API Matrix3x3 : public Matrix
{

public:

	/** @name ���ԣ���Ա������
	* @{
	*/

	/** ��λ���� */
	static const Matrix3x3 IDENTITY3X3;

	/** @} */ // ���Խ�β

public:

	/** @name Vertex3d��������
	*  @{
	*/

	// ���캯��1 ��Ĭ�Ϲ��캯����

	// ���캯��2 �����ι��캯������ʼ���б�
	Matrix3x3(double* const pData);

	/** ���캯��1�����ι��캯������ʼ���б�
	*/
	Matrix3x3();

	// ���캯��3 ���������캯������ʼ���б�
	Matrix3x3(const Matrix3x3& rv);

	// ���캯��4
	Matrix3x3(double fEntry00, double fEntry01, double fEntry02,
		double fEntry10, double fEntry11, double fEntry12,
		double fEntry20, double fEntry21, double fEntry22);

	// ���캯��5
	Matrix3x3(const Vertex3d& rowV1, const Vertex3d& rowV2, const Vertex3d& rowV3);
	/** @} */


	/** @name ��������
	*  @{
	*/
	// The matrix must be orthonormal.  The decomposition is yaw*pitch*roll
	// where yaw is rotation about the Up vector, pitch is rotation about the
	// Right axis, and roll is rotation about the Direction axis.
	// The range of three angles need to be paid attention to:
	// yaw: -180 - 180 ; pitch: -90 - 90�� ; roll: -180 -180
	// ��ŷ���ǵ�ת����ϵ
	int toEulerAnglesXYZ(double& rfYAngle, double& rfPAngle,
		double& rfRAngle) const;
	int toEulerAnglesXZY(double& rfYAngle, double& rfPAngle,
		double& rfRAngle) const;
	int toEulerAnglesYXZ(double& rfYAngle, double& rfPAngle,
		double& rfRAngle) const;
	int toEulerAnglesYZX(double& rfYAngle, double& rfPAngle,
		double& rfRAngle) const;
	int toEulerAnglesZXY(double& rfYAngle, double& rfPAngle,
		double& rfRAngle) const;
	int toEulerAnglesZYX(double& rfYAngle, double& rfPAngle,
		double& rfRAngle) const;

	// ��ŷ����ת��Ϊ�任������������  ����ŷ���ǵ�˳����Զ���Ϊ�Ʋ�ͬ�������ת
	void fromEulerAnglesXYZ(double fYAngle, double fPAngle, double fRAngle);
	void fromEulerAnglesXZY(double fYAngle, double fPAngle, double fRAngle);
	void fromEulerAnglesYXZ(double fYAngle, double fPAngle, double fRAngle);
	void fromEulerAnglesYZX(double fYAngle, double fPAngle, double fRAngle);
	void fromEulerAnglesZXY(double fYAngle, double fPAngle, double fRAngle);
	void fromEulerAnglesZYX(double fYAngle, double fPAngle, double fRAngle);


	void toAxisAngle(Vertex3d& rkAxis, double& rfAngle) const;
	void fromAxisAngle(const Vertex3d& rkAxis, const double& fRadians);
	void fromAxes(const Vertex3d& xAxis, const Vertex3d& yAxis, const Vertex3d& zAxis);

	// ��λ������
	void orthonormalize();

	inline Matrix3x3& operator=(const Matrix3x3& rhm)
	{
		assert(m_iDimM == rhm.m_iDimM);
		for (int i = 0; i<m_iDimM*m_iDimM; i++)
			m_dData[i] = rhm.m_dData[i];
		return (*this);
	}

	// �Ƚ����⣬ע��û��*= ����������
	const Vertex3d operator*(const Vertex3d& rhv) const
	{
		Matrix3x3 mRes = *this;
		return Vertex3d((*this).getRowVector(0) * rhv, (*this).getRowVector(1) * rhv, (*this).getRowVector(2) * rhv);
	}


	/** ������������õ�ǰ�����ָ��������
	* @param [in] nRow	ָ����
	* @return ��ǰ�����ָ��������
	*/
	inline Vertex3d getRowVector(int nRow) const
	{
		assert(nRow < m_iDimM && nRow > -1);
		return Vertex3d((*this)[nRow][0], (*this)[nRow][1], (*this)[nRow][2]);
	}

	/** ������������õ�ǰ�����ָ��������
	* @param [in] nRow	ָ����
	* @return ��ǰ�����ָ��������
	*/
	inline Vertex3d getColVector(int nCol) const
	{
		assert(nCol < m_iDimM && nCol > -1);
		return Vertex3d((*this)[0][nCol], (*this)[1][nCol], (*this)[2][nCol]);
	}


	/** �����������趨��ǰ�����ָ��������
	* @param [in] nRow	ָ����
	* @param [in] vRow	ָ��������
	* @return
	*/
	inline void setRowVector(int nRow, const Vertex3d& vRow)
	{
		assert(nRow < m_iDimM && nRow > -1);
		(*this)[nRow][0] = vRow.x;
		(*this)[nRow][1] = vRow.y;
		(*this)[nRow][2] = vRow.z;
	}


	/** �����������趨��ǰ�����ָ��������
	* @param [in] nRow	ָ����
	* @param [in] vRow	ָ��������
	* @return
	*/
	inline void setRowVector(int nRow, double dEntry0, double dEntry1, double dEntry2)
	{
		setRowVector(nRow, Vertex3d(dEntry0, dEntry1, dEntry2));
	}


	/** �����������趨��ǰ�����ָ��λ�õ�ֵ
	* @param [in] nCol	ָ���� nRow	ָ���� [double] dvalue ���趨��ֵ
	* @return
	*/
	inline void setColVector(int nCol, int nRow, const double& dvalue)
	{
		assert(nCol < m_iDimM && nCol > -1);
		(*this)[nRow][nCol] = dvalue;
	}


	/** �����������趨��ǰ�����ָ��λ�õ�ֵ
	* @param [in] ָ��λ�� [double] dvalue ���趨��ֵ
	* @return
	*/
	inline void setColVector(int location, const double& dvalue)
	{
		assert(location < 0 || location > 8);
		m_dData[location] = dvalue;
	}


	/** �����������趨��ǰ�����ָ��������
	* @param [in] nCol	ָ����
	* @param [in] vCol	ָ��������
	* @return
	*/
	inline void setColVector(int nCol, const Vertex3d& vCol)
	{
		assert(nCol < m_iDimM && nCol > -1);
		(*this)[0][nCol] = vCol.x;
		(*this)[1][nCol] = vCol.y;
		(*this)[2][nCol] = vCol.z;
	}

	inline void setColVector(int nCol, double dEntry0, double dEntry1, double dEntry2)
	{
		setColVector(nCol, Vertex3d(dEntry0, dEntry1, dEntry2));
	}

	/** @}*/  // �����������β
};
/** @} */ // ģ���β


/** @addtogroup math  Matrix3x3ȫ�ַ�������
*  @{
*/
CORE_API Vertex3d operator*(const Vertex3d& lhv, const Matrix3x3& rhm);
CORE_API const Matrix3x3 operator+(const Matrix3x3& lhm, const Matrix3x3& rhm);
CORE_API const Matrix3x3 operator-(const Matrix3x3& lhm, const Matrix3x3& rhm);
CORE_API const Matrix3x3 operator+(const Matrix3x3& lhm, double rhd);
CORE_API const Matrix3x3 operator-(const Matrix3x3& lhm, double rhd);
CORE_API const Matrix3x3 operator*(const Matrix3x3& lhm, const Matrix3x3& rhm);
CORE_API const Matrix3x3 operator*(const Matrix3x3& lhm, double rhd);
CORE_API const Matrix3x3 operator/(const Matrix3x3& lhm, double rhd);
/** @} */

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
