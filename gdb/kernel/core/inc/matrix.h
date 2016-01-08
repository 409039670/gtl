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
#include "config.h"
#include "vertex3d.h"
#include "mathex.h"


begin_gtl_namespace
begin_gdb_namespace
 

/** @addtogroup math  Matrix-���󣨻��ࣩ 
    *  @{
    */
class CORE_API Matrix 
{

protected:

	
	/** @name ���ԣ���Ա������
	* @{
	*/
	
	/** �������� */
	double* m_dData;
	
	/** ����ά�� */
	 int  m_iDimM;

	/** @} */ // ���Խ�β

public:
	
	/** @name ����������
	*  @{
	*/
	/** ���캯��1�����ι��캯������ʼ���б�
	*/
	Matrix(int iDim,double* const pData = NULL);
	/** ���캯��3 ���������캯������ʼ���б�
	*/
	Matrix(const Matrix& rv);

	// ��������
	virtual ~Matrix()
	{
		if(m_dData)
			delete m_dData;
	}

	/** @}*/  // �������������β

    /** @name �������
    *  @{
    */

	/** ���㵱ǰ����������
	* @param 
	* @return 
    */
	int   inverse();

	//##ModelId=49EEE8070138
	/** ���㵱ǰ�����ת�þ���
	* @param 
	* @return 
    */
	void   flip();

	//##ModelId=49ECA67F00FA
	/** ���õ�λ����
	* @param 
	* @return 
    */
	void   loadIdentity();



	/** @}*/  // ����������β

	/** @name ���ݲ���
    *  @{
    */

	/** �������������ض�Ԫ������"="�����ڶԵ�ǰ����ĸ�ֵ��
	* @param [in] rhm	���ڸ�ֵ�ľ���
	* @return ��ֵ��õ��ľ���
    */
	inline Matrix& operator=(const Matrix& rhm)
	{
		assert(m_iDimM == rhm.m_iDimM);
		for(int i=0; i<m_iDimM*m_iDimM; i++)
			m_dData[i] = rhm.m_dData[i];
		return (*this);
	}

	/** �������������ض�Ԫ������"=="�������жϵ�ǰ������ƶ������Ƿ���ȡ�
	* @param [in] rhm	���ڸ�ֵ�ľ���
	* @return 1 -- ��ȣ�0 -- �����
    */
	inline int operator==(const Matrix& rhm)
	{
		assert(m_iDimM == rhm.m_iDimM);
		for(int i=0; i<m_iDimM*m_iDimM; i++)
			if(fabs(m_dData[i] - rhm.m_dData[i]) > TOLERANCE)
				return 0;
		return 1;
	}

	/** �������������ض�Ԫ������"��="�������жϵ�ǰ������ƶ������Ƿ���ȡ�
	* @param [in] rhm	���ڸ�ֵ�ľ���
	* @return 1 -- ����ȣ�0 -- ���
    */
	inline int operator!=(const Matrix& rhm)
	{
		return !((*this) == rhm);
	}


	/** �������������ض�Ԫ������"^="�����ڶԾ���ĸ��ϸ�ֵ���ӷ���
	* @param [in] rhm	��ǰ���ڸ��ϸ�ֵ�ľ���
	* @return 
    */
	inline void operator+=(const Matrix& rhm)
	{
		assert(m_iDimM == rhm.m_iDimM);
		for(int i=0; i<m_iDimM*m_iDimM; i++)
			m_dData[i] += rhm.m_dData[i];
	}

	//##ModelId=49EEE40403C8
	/** �������������ض�Ԫ������"+="�����ڶԾ���ĸ��ϸ�ֵ�����ӣ�
	* @param [in] rhd	��ǰ���ڸ��ϸ�ֵ����
	* @return 
    */
	inline void operator+=(double rhd)
	{
		for(int i=0; i<m_iDimM*m_iDimM; i++)
			m_dData[i] += rhd;
	}

	
	/** �������������ض�Ԫ������"^="�����ڶԾ���ĸ��ϸ�ֵ��������
	* @param [in] rhm	��ǰ���ڸ��ϸ�ֵ�ľ���
	* @return 
    */
	inline void operator-=(const Matrix& rhm)
	{
		assert(m_iDimM == rhm.m_iDimM);
		for(int i=0; i<m_iDimM*m_iDimM; i++)
			m_dData[i] -= rhm.m_dData[i];
	}

	
	/** ��������������һԪ������"^="������ȡ�þ�����෴ֵ��
	* @param 
	* @return ������෴ֵ
	*/
	inline Matrix operator-() const
	{
		Matrix res(*this);
		res *= -1;
		return res;
	}
	
	/** �������������ض�Ԫ������"^="�����ڶԾ���ĸ��ϸ�ֵ��������
	* @param [in] rhd	��ǰ���ڸ��ϸ�ֵ����
	* @return 
    */
	inline void operator-=(double rhd)
	{
		for(int i=0; i<m_iDimM*m_iDimM; i++)
			m_dData[i] -= rhd;
	}

	/** �������������ض�Ԫ������"*="�����ڶԾ���ĸ��ϸ�ֵ���˷���
	* @param [in] rhd	��ǰ���ڸ��ϸ�ֵ�ľ���
	* @return 
    */
	void operator*=(const Matrix& rhm);
	
	/** �������������ض�Ԫ������"^="�����ڶԾ���ĸ��ϸ�ֵ�����ˣ�
	* @param [in] rhd	��ǰ���ڸ��ϸ�ֵ����
	* @return 
    */
	inline void operator*=(double rhd)
	{
		for(int i=0; i<m_iDimM*m_iDimM; i++)
			m_dData[i] *= rhd;
	}

	/** �������������ض�Ԫ������"^="�����ڶԾ���ĸ��ϸ�ֵ��������
	* @param [in] rhd	��ǰ���ڸ��ϸ�ֵ����
	* @return 
	*/
	inline void operator/=(double rhd)
	{
		if(fabs(rhd) < TOLERANCE)
			return;
		for(int i=0; i<m_iDimM*m_iDimM; i++)
			m_dData[i] /= rhd;
	}

	/** �������������ض�Ԫ������"[]"�����ڻ�þ���ָ��������Ӧ��ֵ
	* @param [in] rhd	��ǰ���ڸ��ϸ�ֵ����
	* @return 
	*/
	inline double* operator[](int iIndex) const
	{
		assert(iIndex >= 0 && iIndex < m_iDimM);
		return &m_dData[iIndex*m_iDimM];
	}
	/** @}*/  // ���ݲ������β

	/** @name ��������
    *  @{
    */

	/** ���õ�ǰ����Ϊ����󣬼�����Ԫ��ֵΪ0
	* @param 
	* @return 
    */
	void  setNull()
	{
		memset(m_dData,0,m_iDimM*m_iDimM*sizeof(double));
	}
	
	/** �жϵ�ǰ�����Ƿ�Ϊ�����
	* @param 
	* @return	1 -- �����0 - ������� 
    */
	int  isNull()
	{
		for(int i=0; i<m_iDimM*m_iDimM; i++)
			if(fabs(m_dData[i]) > TOLERANCE)
				return 0;
		return 1;
	}

	/** ��þ����ά��
	* @param 
	* @return �����ά��
    */
	int getDimension() const
	{
		return m_iDimM;
	}

	/** ���λ�ڹ̶����е�Ԫ��ֵ
	* @param [in] nRow		�̶���	
	* @param [in] nCol		�̶���
	* @return λ�ڹ̶����е�Ԫ��ֵ
    */
	double getData(int nRow, int nCol) const
	{
		return m_dData[nRow*m_iDimM + nCol];
	}
	/** @}*/  // �����������β

	/** ���Ԫ��ֵ
	* @return
    */
	double* getData() const
	{
		return m_dData;
	}
	/** @}*/  // �����������β

};
/** @}*/

/** @addtogroup math  Matrix-ȫ�ֲ����������� 
    *  @{
    */
 CORE_API const Matrix operator+(const Matrix& lhm, const Matrix& rhm);
 CORE_API const Matrix operator-(const Matrix& lhm, const Matrix& rhm);
 CORE_API const Matrix operator+(const Matrix& lhm, double rhd);
 CORE_API const Matrix operator-(const Matrix& lhm, double rhd);
 CORE_API const Matrix operator*(const Matrix& lhm, const Matrix& rhm);
 CORE_API const Matrix operator*(const Matrix& lhm, double rhd);
 CORE_API const Matrix operator/(const Matrix& lhm, double rhd);
 /** @}*/

end_gdb_namespace
end_gtl_namespace


