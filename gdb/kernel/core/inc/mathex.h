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
#include "vertex3d.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

/** @defgroup math  Math-��ѧ��
*  @{
*/

class GV3DCORE_API MathEx
{
public:


	/** @name ���ԣ���Ա������
	* @{
	*/

	/** ��Ա���� */
	static const double  TOL;
	static const double	 TOL_F;
	static const double  ONE_PI;
	static const double  TWO_PI;
	static const double  HALF_PI;
	static const double  dDegToRad;
	static const double  dRadToDeg;
	static const double  dDoubleMax;
	static const double  dDoubleMin;

	/** @} */ // ���Խ�β



	/** @name ��������������
	*  @{
	*/
protected:
	/** ���캯��
	*/
	MathEx(void);
	/** ��������
	*/
	~MathEx(void);
	/** @} */

	/** @name �������
	*  @{
	*/
public:

	/** �жϷ���
	* @param [in] iValue ������ֵ
	* @return ����
	*/
	static inline int isign(int iValue){ return (iValue > 0 ? +1 : (iValue < 0 ? -1 : 0)); }
	/** �жϷ���
	* @param [in] dValue ������ֵ
	* @return ����
	* -1 ��ʾΪ��
	* --1 ��ʾΪ��
	* -0 ��ʾΪ0
	*/
	static double sign(double dValue);
	/** ���㷴����ֵ
	* @param [in] fValue ��������
	* @return �Ƕ�
	*/
	static double acos(double fValue);

	/** ���㷴����ֵ
	* @param [in] fValue ��������
	* @return �Ƕ�
	*/
	static double asin(double fValue);

	/** ���㷴����ֵ
	* @param [in] dValue ��������
	* @return �Ƕ�
	*/
	static inline double atan(double fValue) { return (::atan(fValue)); }

	/** �������Һ���
	* @param [in] dValue ����
	* @return ����ֵ
	*/
	static inline double atan2(double fY, double fX) { return (::atan2(fY, fX)); }

	/** �������Һ���
	* @param [in] dValue ����
	* @return ����ֵ
	*/
	static  double radianToDegree(double angle);

	/** �������Һ���
	* @param [in] dValue ����
	* @return ����ֵ
	*/
	static double degreeToRadian(double angle);

	/** ʹ�ñ�׼ģ��⣨STL���ķ���ʽ����������Դ��wikipedia
	* @param BidirectionalIterator:	�б�ĵ���������
	* @param Compare:��������ıȽϺ���
	* @return
	*/
	template< typename BidirectionalIterator, typename Compare >
	static void quick_sortEx(BidirectionalIterator first, BidirectionalIterator last, Compare cmp) {
		if (first != last) {
			BidirectionalIterator left = first;
			BidirectionalIterator right = last;
			BidirectionalIterator pivot = left++;

			while (left != right) {
				if (cmp(*left, *pivot)) {
					++left;
				}
				else {
					while ((left != right) && cmp(*pivot, *right))
						right--;
					std::iter_swap(left, right);
				}
			}

			if (cmp(*pivot, *left))
				--left;
			std::iter_swap(first, left);

			quick_sortEx(first, left, cmp);
			quick_sortEx(right, last, cmp);
		}
	}
	/** ʹ�ñ�׼ģ��⣨STL���ķ���ʽ����������Դ��wikipedia
	* @param BidirectionalIterator:	�б�ĵ�����
	* @return
	*/
	template< typename BidirectionalIterator >
	static void quick_sort(BidirectionalIterator first, BidirectionalIterator last) {
		quick_sortEx(first, last,
			std::less_equal< typename std::iterator_traits< BidirectionalIterator >::value_type >()
			);
	}
	/** @} */
};
/** @} */



end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
