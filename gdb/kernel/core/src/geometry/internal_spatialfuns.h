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
#include "quaternion.h"
#include "matrix4x4.h"
#include "mathex.h"
#include "internal_frustumimpl.h"
#include "internal_rayimpl.h"
#include "internal_aabboximpl.h"
#include <vector>
#include <algorithm>
#include <limits>
#include <deque>
#include <set>
using namespace  std;


begin_gtl_namespace
begin_gdb_namespace

/***********************************************/
// ͨ��ΪGetϵ�л���Ensureϵ�к���
// ע����ƽ�溯�����ж��䷵��ֵ�Ƿ�Ϊ0
/***********************************************/

/** ��ͨ�������ƽ��
* @param [in] p1,p2,p3	�ռ�����
* @param [out] plane	����ƽ��
* @return 1 - ���㲻���ߣ�0 - ���㹲��
*/
 int GetPlaneEquation(Vertex3d& p1, Vertex3d &p2, Vertex3d &p3, PLANE3D& plane);

/** ����֪��������ͨ��һ���ƽ��
* @param [in] vNormal	��֪������
* @param [in] pt	ͨ��һ��
* @param [out] plane	����ƽ��
* @return 1 - ���㲻���ߣ�0 - ���㹲��
*/
 int GetPlaneEquation(Vertex3d &vNormal, Vertex3d &pt, PLANE3D& plane);

/** ��ͨ�������ƽ��,���ҷ������ᱻ��һ��
* @param [in] p1,p2,p3	�ռ�����
* @param [out] plane	����ƽ��
* @return 1 - ���㲻���ߣ�0 - ���㹲��
*/
 int GetPlaneEquationEx(Vertex3d &p1, Vertex3d &p2, Vertex3d &p3, PLANE3D& plane);

/** ����֪ƽ��ķ�����
* @param [in] plane	��֪ƽ��
* @param [out] vNormal	��������
* @return
*/
 void GetPlaneNormal(PLANE3D& plane, Vertex3d& vNormal);

//���ܣ�����һ����һ���ƽ��ķ�����
//������planeΪƽ�棬ptΪһ�㣬vNormalΪ��������

 void GetPlaneNormalEx(PLANE3D& plane, Vertex3d& pt, Vertex3d& vNormal);
/** ����֪ƽ���ϵ�����һ�㣨ʹ��ǰ��Ҫ�ṩ������ӣ�
* @param [in] plane	��֪ƽ��
* @param [out] pt	����һ��
* @return
*/
 void GetOnePointInPlane(PLANE3D& plane, Vertex3d& pt);

/** ����֪ƽ���ϵ�������ĳ�������һ�㣨ʹ��ǰ��Ҫ�ṩ������ӣ�
* @param [in] plane	��֪ƽ��
* @param [out] pt	����һ��
* @param [in] Diffpt �����
* @return
*/
 void GetOnePointInPlane(PLANE3D& plane, Vertex3d& pt, Vertex3d& Diffpt);

/** ����֪���ȵ���������������ʹ��ǰ��Ҫ�ṩ������ӣ�
* @param [out] ray	������������
* @param [in] radius	��֪��������
* @return
*/
 void GetRandomRay(Vertex3d& ray, double radius);

/** ��ĳƽ������֪���ȵ���������������ʹ��ǰ��Ҫ�ṩ������ӣ�
* @param [out] ray	������������
* @param [in] radius	��֪��������
* @param [in] plane	��֪ƽ��
* @return
*/
 void GetRandomRayInPlane(Vertex3d& ray, double radius, PLANE3D& plane);

/** ��ȡһ����ʱ�������ҷ����ཻ����㼯�ķ�����
* @param [in] ptArray	��֪�㼯
* @param [out] vNormal	��������
* @return ��֪ƽ���ϵ�һ�������ߵ�������ţ�-1 - ���е㹲�� ��㼯�����棩
*/
 int GetPointsNormal(vector<Vertex3d>& ptArray, Vertex3d& vNormal);

/** ��ȡһ����ʱ�������ҷ����ཻ�㼯�ķ�����,����㼯������
* @param [in] ptArray	��֪�㼯
* @param [out] vNormal	��������
* @return ��֪ƽ���ϵ�һ�������ߵ�������ţ�-1 - ���е㹲�ߣ�
*/
 int GetPointsNormalEx(vector<Vertex3d>& ptArray, Vertex3d& vNormal);

/** ��������㼯͹��(2D),ydcl
* @param [in] ptArray	��֪�㼯
* @param [out] CHArray	����͹��
* @return
*/
 void GetConvexHull(vector<Vertex3d>& ptArray, vector<Vertex3d>& CHArray); // xx_add_0619
/** ȷ���㼯���
* @param [in] ptArray	��֪�㼯
* @return
*/
 void EnsureClose(vector<Vertex3d>& ptArray);

/** ȷ���㼯����
* @param [in] ptArray	��֪�㼯
* @return
*/
 void EnsureOpen(vector<Vertex3d>& ptArray);

 int EnsureAntiClock(vector<Vertex3d>& Point3dArray, Vertex3d &vDirection);
/** ����������ϵ��WCS����ת�����ֲ����꣨LCS����
* @param [in] CPoint3d &pt : ��Ҫת���ĵ�����
* @param [in] CPoint3d& origin : LCS����ϵ��ԭ����WCS����ϵ�е�ֵ
* @param [in] CPoint3d& vx,CPoint3d& vy,CPoint3d& vz : LCS����ϵ������WCS����ϵ�е�ֵ
* @return
*/
 inline void ChangeXYZAxis(Vertex3d &pt, Vertex3d& origin, Vertex3d& vx, Vertex3d& vy, Vertex3d& vz)
{
	pt -= origin;
	Vertex3d temp;
	temp.x = pt * vx;
	temp.y = pt * vy;
	temp.z = pt * vz;
	pt = temp;
}

/** �Ӿֲ����꣨LCS����ת������������ϵ��WCS����
* @param [in] CPoint3d &pt : ��Ҫת���ĵ�����
* @param [in] CPoint3d& origin : LCS����ϵ��ԭ����WCS����ϵ�е�ֵ
* @param [in] CPoint3d& vx,CPoint3d& vy,CPoint3d& vz : LCS����ϵ������WCS����ϵ�е�ֵ
* @return
*/
 inline void ChangeXYZAxisReverse(Vertex3d &pt, Vertex3d& origin, Vertex3d& vx, Vertex3d& vy, Vertex3d& vz)
{
	pt = vx * pt.x + vy * pt.y + vz * pt.z + origin;
}
//���ܣ��������ֱ����һƽ�洹ֱ��ƽ�淽��
//������pt1,pt2Ϊֱ�ߵ������˵㣬plane1Ϊƽ��1�����ش�ֱƽ��plane2
 void GetPlaneEquationByVerticalPlane(Vertex3d& pt1, Vertex3d& pt2, PLANE3D& plane1, PLANE3D& plane2);

//���ܣ������������һƽ��ƽ�е�ƽ�淽��
//������ptΪ�˵㣬plane1Ϊƽ��1������ƽ��ƽ��plane2
 void GetPlaneEquationByParallelPlane(Vertex3d& pt, PLANE3D& plane1, PLANE3D& plane2);

//���ܣ��������ֱ������һֱ��ƽ�е�ƽ�淽��
//������pt1,pt2,pt3,pt4Ϊ����ֱ�ߵ�4���˵㣬plane1Ϊƽ��1������ƽ��ƽ��plane2
 void GetPlaneEquationByParallelLine(Vertex3d& pt1, Vertex3d& pt2, Vertex3d& pt3, Vertex3d& pt4, PLANE3D& plane);

// ���ܣ����ƽ���Ƿ���Ч���뷨�����ڼ����ά�����Ƿ�Ϊ�任����
 int IsPlaneValid(PLANE3D& plane);
 void EnsureNoRedunant(vector<Vertex3d>& pointArray);

/** �����ƶ��ľ��������������������ڵ������
* @param [in] pointArray : ��Ҫ���Ķ�������
* @param [in] dTOL : �������ʱ���õľ������
* @return
*/
 void EnsureNoRedunantWithTOL(vector<Vertex3d>& pointArray, double dTOL);

/***********************************************/
// ���ں�׺��ΪEx�ĺ������������ϵ�к����У���Dϵ�к����У�����ʾ��þ���ƽ��ֵ��û��������չ����
// ��������뺯�����ṩ��Ӧ��Ex����
/***********************************************/

/** ����֪�ռ�����ľ���
* @param [in] p1,p2  ��֪�ռ�����
* @return  �������
*/
 double DPointToPoint(Vertex3d& pt1, Vertex3d& pt2);

/** ����֪�ռ�����ľ����ƽ��ֵ
* @param [in] p1,p2  ��֪�ռ�����
* @return  ��������ƽ��ֵ
*/
 double DPointToPointEx(Vertex3d& pt3d, Vertex3d& pOrigin);

/**����ĳ�㵽ƽ��ľ���ֵ��
@param pt3d  ��ά�ռ��еĸ����㡣
@param plane  ƽ�档
@return  ���ص㹹��ƽ��ľ���ֵ
*/
 double DPointToPlane(Vertex3d& pt3d, PLANE3D& plane);

/**����ĳ�㵽ƽ��ľ����ƽ��ֵ
@param pt3d  ��ά�ռ��еĸ�����
@param plane  ƽ�档
@return  ���ص㹹��ƽ��ľ����ƽ��ֵ
*/
 double DPointToPlaneEx(Vertex3d& pt3d, PLANE3D& plane);


/** ��㵽ֱ�߶ε���̾���
* @param [in] pt3d  ��֪��
* @param [in] pStartPoint,pEndPoint  ��ֱ֪�߶εĶ˵�
* @param [out] resultPoint			 �������ߵ���һ�˵㣨һ������֪�㣩
* @return  �������
*/
 double DPointToLineSeg(Vertex3d& pt3d, Vertex3d& pStartPoint, Vertex3d& pEndPoint, Vertex3d* pResultPoint = NULL);//xxy//

/** ��㵽ֱ�߶ε���̾����ƽ��ֵ
* @param [in] pt3d  ��֪��
* @param [in] pStartPoint,pEndPoint  ��ֱ֪�߶εĶ˵�
* @param [out] resultPoint			 �������ߵ���һ�˵㣨һ������֪�㣩
* @return  ��������ƽ��ֵ
*/
 double DPointToLineSegEx(Vertex3d& pt3d, Vertex3d& pStartPoint, Vertex3d& pEndPoint, Vertex3d* pResultPoint = NULL);//xxy//

/** ��㵽�����ε���̾���
* @param [in] pt3d  ��֪��
* @param [in] triPt1,triPt2,triPt3  ��֪�����ε���������
* @param [out] resultPoint			 �������ߵ���һ�˵㣨һ������֪�㣩
* @return  �������
*/
 double DPointToTriangle(Vertex3d& pt3d, Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3, Vertex3d* pResultPoint = NULL);//xxy//

/** ��㵽�����ε���̾����ƽ��ֵ
* @param [in] pt3d  ��֪��
* @param [in] triPt1,triPt2,triPt3  ��֪�����ε���������
* @param [out] resultPoint			 �������ߵ���һ�˵㣨һ������֪�㣩
* @return  ��������ƽ��ֵ
*/
 double DPointToTriangleEx(Vertex3d& pt3d, Vertex3d& planePt1, Vertex3d& planePt2, Vertex3d& planePt3, Vertex3d* pResultPoint = NULL);//xxy//

/** ���߶ε������ε���̾���
* @param [in] pt3d  �߶�2�˵�
* @param [in] triPt1,triPt2,triPt3  ��֪�����ε���������
* @param [out] resultPoint			 �������ߵ���һ�˵㣨һ������֪�㣩
* @return  �������
*/
 double DLineSegToTriangle(Vertex3d& linePt1, Vertex3d& linePt2, Vertex3d& planePt1, Vertex3d& planePt2, Vertex3d& planePt3, Vertex3d* pPoint1 = NULL, Vertex3d* pPoint2 = NULL);//xxy//

/** ���߶ε������ε���̾����ƽ��ֵ
* @param [in] pt3d  �߶�2�˵�
* @param [in] triPt1,triPt2,triPt3  ��֪�����ε���������
* @param [out] resultPoint			 �������ߵ���һ�˵㣨һ������֪�㣩
* @return  ��������ƽ��ֵ
*/
 double DLineSegToTriangleEx(Vertex3d& linePt1, Vertex3d& linePt2, Vertex3d& planePt1, Vertex3d& planePt2, Vertex3d& planePt3, Vertex3d* pPoint1 = NULL, Vertex3d* pPoint2 = NULL);//xxy//
//�ռ��������ε������ε���С����/ƽ��
 double DTriangleToTriangleEx(Vertex3d& tri1Pt1, Vertex3d& tri1Pt2, Vertex3d& tri1Pt3, Vertex3d& planePt1, Vertex3d& planePt2, Vertex3d& planePt3, Vertex3d* pPoint1 = NULL, Vertex3d* pPoint2 = NULL);
 double DTriangleToTriangle(Vertex3d& tri1Pt1, Vertex3d& tri1Pt2, Vertex3d& tri1Pt3, Vertex3d& planePt1, Vertex3d& planePt2, Vertex3d& planePt3, Vertex3d* pPoint1 = NULL, Vertex3d* pPoint2 = NULL);

/** ��㵽����ε���̾���
* @param [in] pt3d  ��֪��
* @param [in] vPts  ��֪����εĶ��㼯
* @param [out] resultPoint			 �������ߵ���һ�˵㣨һ������֪�㣩
* @return  �������
*/
 double DPointToPolygon(Vertex3d& pt3d, vector<Vertex3d>& vPts, Vertex3d* pResultPoint = NULL); //xxy//

/** ��㵽����ε���̾����ƽ��ֵ
* @param [in] pt3d  ��֪��
* @param [in] vPts  ��֪����εĶ��㼯
* @param [out] resultPoint			 �������ߵ���һ�˵㣨һ������֪�㣩
* @return  ��������ƽ��ֵ
*/
 double DPointToPolygonEx(Vertex3d& pt3d, vector<Vertex3d>& vPts, Vertex3d* pResultPoint = NULL); //xxy//

 double DPointToLine(Vertex3d& pt, Vertex3d& LineStr, Vertex3d& LineEnd, Vertex3d* pFootPt = NULL); //xxy//
 double DPointToLineEx(Vertex3d& pt, Vertex3d& LineStr, Vertex3d& LineEnd, Vertex3d* pFootPt = NULL); //xxy//

// ���ռ�ֱ�ߵ���̾��루���ཻ��Ϊ0����Ϊ������Ϊ�����߾��룬��ƽ����Ϊ��ࣩ���������̾�����
 double DLineToLine(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, Vertex3d *pPair1 = NULL, Vertex3d *pPair2 = NULL);


// ���ռ��߶ε���̾��루���ཻ��Ϊ0����Ϊ������Ϊ�����߾��룬��ƽ����Ϊ��ࣩ���������̾�����
 double DLineSegToLineSeg(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, Vertex3d *pPair1 = NULL, Vertex3d *pPair2 = NULL);

 double DLineSegToLineSegEx(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, Vertex3d *pPair1 = NULL, Vertex3d *pPair2 = NULL);

 double DLineSegToPolygon(Vertex3d& linePt1, Vertex3d& linePt2, vector<Vertex3d>& ptArray, Vertex3d* pPoint1 = NULL, Vertex3d* pPoint2 = NULL);

 double DLineSegToPolygonEx(Vertex3d& linePt1, Vertex3d& linePt2, vector<Vertex3d>& ptArray, Vertex3d* pPoint1 = NULL, Vertex3d* pPoint2 = NULL);
 double DPointToRay(Vertex3d& pPoint, Vertex3d& pStartPoint, Vertex3d& pEndPoint, Vertex3d* pResultPoint);

 double DPointToRayEx(Vertex3d& pPoint, Vertex3d& pStartPoint, Vertex3d& pEndPoint, Vertex3d* pResultPoint);


 /***********************************************/
 // �ռ��ϵ���жϵķ���ֵ����isϵ�к�Rϵ��
 // ֻ��Ҫ֪�����Ե��жϽ������ֻ��Ҫ��֪��1����0������Isϵ�У����жϰ�����ϵ����������In��Cross��Intersect��
 // ����Ҫ֪����ϸ���жϽ��������0��1����ϸ�Ľ����������Rϵ�У�����ֵ����char��ʾ����Сint����ֵ�ռ�,�����������With
 // ���⣬��Щʱ����Ҫ֪���������жϽ����ֻ��Ҫ֪��ĳһ����Ĺ�ϵ����ô���Դ�Rϵ������ȡIsϵ��,���ÿ����㷨���Ч��
 /***********************************************/

 /**�жϿռ��ĵ��Ƿ���,�������棬��������ĵ���ƽ���ϵĴ��㡣
 @param : [IN] p1	��������ά�ռ��һ��ֻ��p4���ָ���˳�򣩡�
 @param : [IN] p2��	��������ά�ռ�����ֻ��p4���ָ���˳�򣩡�
 @param : [IN] p3	��������ά�ռ������ֻ��p4���ָ���˳�򣩡�
 @param : [IN] p4	��������ά�ռ���ģ�ֻ��p4���ָ���˳�򣩡�
 @param : [OUT] pt	p4��p1��p2��p3ȷ����һƽ���ϵĴ���
 @return : 1 - ����; 0 - �����档
 */
  int IsFourPointsInOnePlane(Vertex3d& p1, Vertex3d& p2, Vertex3d& p3, Vertex3d& p4, Vertex3d* pFootPt = NULL);//xxy//


 /**�жϿռ�һ��S�Ƿ�λ��ĳƽ���ϣ�������ƽ���ϣ����������Ļ�ϵĴ���
 @brief :
 @param : [IN] pt		�ռ�һ��
 @param : [IN] plane		ƽ��
 @param : [OUT] footPt	����
 @return : 1 - ����ƽ���� ; 0 - �㲻��ƽ����
 */
  int IsPointInPlane(Vertex3d& pt, PLANE3D& plane, Vertex3d* pFootPt = NULL);//xxy//

 /**�жϵ����������ε��ڲ�����������)�������ⲿ ��
 @param : [in] pt1  �����εĶ���һ��������Ҫ�󣩡�
 @param : [in] pt2  �����εĶ������������Ҫ�󣩡�
 @param : [in] pt3  �����εĶ�������������Ҫ�󣩡�
 @param : [in] result  ���жϵ㡣
 @param : [in] bOnPlane  �Ƿ���Ѿ���ƽ���ϡ�
 @return:  1  -  �������������ڲ�����������)�� 0 -  �������������ⲿ��
 */
  int IsPointInTri(Vertex3d& pt1, Vertex3d& pt2, Vertex3d& pt3, Vertex3d& vPt, int bOnPlane = 1);

 /**�жϵ����������ε��ڲ�����������)�������ⲿ��
 @param : [in] pt1  �����εĶ���һ��������Ҫ�󣩡�
 @param : [in] pt2  �����εĶ������������Ҫ�󣩡�
 @param : [in] pt3  �����εĶ�������������Ҫ�󣩡�
 @param : [in] result  ���жϵ㡣
 @param : [in] bOnPlane  �Ƿ���Ѿ���ƽ���ϡ�
 @return:  'V'  - ����������һ�������غ�; 'E' - �����������α��ϣ� 'F' -  �������������ڲ�; '0' -  �������������ⲿ
 */
  char RPointWithTri(Vertex3d& pt1, Vertex3d& pt2, Vertex3d& pt3, Vertex3d& vPt, bool bOnPlane = true);


 /**�жϵ����߶��ϻ����߶���
 @param : [in] pt  ���жϵ�
 @param : [in] pt1  �߶����
 @param : [in] pt2  �߶��յ�
 @param : [out] footPt  �����߶��ϵĴ���
 @return:  1  -  �����߶��ϣ� 0 -  �����߶��⡣
 */
  int IsPointInLineSeg(Vertex3d& pt, Vertex3d& pt1, Vertex3d& pt2, Vertex3d* pFootPt = NULL);//xxy//

 /**�жϵ���ֱ���ϻ���ֱ����
 @param : [in] pt  ���жϵ�
 @param : [in] pt1  ֱ��һ��
 @param : [in] pt2  ֱ��һ��
 @param : [out] footPt  ����ֱ���ϵĴ���
 @return:  1  -  ����ֱ���ϣ� 0 -  ����ֱ���⡣
 */
  int IsPointInLine(Vertex3d& pt, Vertex3d& pt1, Vertex3d& pt2, Vertex3d* pFootPt = NULL);//xxy//

 /**�жϵ��Ƿ��ڿռ����Σ�ͬһƽ���ڣ���
 @param : [in] pt  ���жϵ�
 @param : [in] vPts  ֱ��һ��
 @return:  1  -  ���ڶ�����ڣ� 0 -  ���ڶ�����⡣
 */
  int IsPointInPolygon(Vertex3d& pt, vector<Vertex3d>& vPts);

 /**�жϵ���ռ����Σ�ͬһƽ���ڣ��Ĺ�ϵ�����߷���
 @param : [in] ptPoint  ���жϵ�
 @param : [in] vPts  �����
 @param : [in] vPts  ָ��������
 @return:  'o' - �ڶ�����⣻ 'i' - �ڶ�����ڣ�'E' - �ڶ���α߽��ϣ� 'V' - �ڶ���ε���
 */
  char RPointWithPolygon(Vertex3d& ptPoint, vector<Vertex3d>& vPts, Vertex3d *pRay = NULL);

 /**�жϵ�ͶӰ��ռ�����ͶӰ�Ĺ�ϵ�����߷���
 @param : [in] ptPoint  ���жϵ�
 @param : [in] vPts  ֱ��һ��
 @return:  'o' - �ڶ�����⣻ 'i' - �ڶ�����ڣ�'E' - �ڶ���α߽��ϣ� 'V' - �ڶ���ε���
 */
  char RPointWithPolygon2d(Vertex3d& ptPoint, vector<Vertex3d>& vPts);

 /**�ж϶������������XOYͶӰ�ϵĹ�ϵ���������ཻ������
 @param : [in] pt  ���жϵ�
 @param : [in] vPts  ֱ��һ��
 @return:  0 - ���룻1 - �ཻ��2 - ������
 */
  int RPolygonWithPolygon2D(vector<Vertex3d>& ptAs, vector<Vertex3d>& ptBs); //xx_add_0619, // ydcl
 /**�ж��߶���ռ�ƽ��Ĺ�ϵ
 @param : [in] LineStr  �߶����
 @param : [in] LineEnd  �߶��յ�
 @param : [in] plane	   ƽ��
 @param : [out] pt	   �߶���ƽ��Ľ���
 @return:  '0' - The segment lies strictly to one side or the other of the plane��
 '1' - The segment intersects the plane
 'p' - The segment lies wholly within the plane.��
 's' - The startpoint is on the plane
 'e' - The endpoint is on the plane
 */
  char RLineSegWithPlane(Vertex3d& LineStr, Vertex3d& LineEnd, PLANE3D& plane, Vertex3d* pCrossPt = NULL);//xxy//

 /**�ж��߶��Ƿ���ռ�ƽ���ཻ
 @param : [in] LineStr  �߶����
 @param : [in] LineEnd  �߶��յ�
 @param : [in] plane	   ƽ��
 @return:  1 - �߶���ռ�ƽ���ཻ
 0 - �߶���ռ�ƽ�治�ཻ
 */
  int IsLineSegCrossPlane(Vertex3d& LineStr, Vertex3d& LineEnd, PLANE3D& plane);

 /**����ά�ռ��У��ж�ֱ����ƽ��Ĺ�ϵ
 @param : [IN] LineStr  ֱ��һ�㡣
 @param : [IN] LineEnd  ֱ��һ�㡣
 @param : [IN] plane    ƽ��
 @param : [OUT] pt	  ֱ����ƽ��Ľ���
 @return : '1'	- �߶���ƽ���ཻ��'0' - �߶���ƽ��ƽ�У�'2' - �߶���ƽ����
 */
  char RLineWithPlane(Vertex3d& LineStr, Vertex3d& LineEnd, PLANE3D& plane, Vertex3d* pCrossPt = NULL);//xxy//

 /**����ά�ռ��У��ж��߶��Ƿ񴩹�����������ƽ�棬������һ����������������
 @param �� [IN] LineStr			�߶���㡣
 @param �� [IN] LineEnd			�߶��յ㡣
 @param �� [IN] pt1��pt2��pt3	ȷ�������ε������㡣
 @param �� [OUT] pt				����
 @return��  1  - �߶δ���������ƽ�棬�������������Σ� 0 -  ���������
 */
  int IsLineSegCrossTri(Vertex3d& LineStr, Vertex3d& LineEnd, Vertex3d& pt1, Vertex3d& pt2, Vertex3d& pt3, Vertex3d* pCrossPt = NULL);//xxy//

 /**����ά�ռ��У��ж��߶��������εĹ�ϵ
 @param �� [IN] LineStr			�߶���㡣
 @param �� [IN] LineEnd			�߶��յ㡣
 @param �� [IN] pt1��pt2��pt3	ȷ�������ε������㡣
 @param �� [OUT] pt				����//??
 @return��
 'V': the open segment includes a vertex of T.
 'E': the open segment includes a point in the relative interior of an edge of T.
 'F': the open segment includes a point in the relative interior of a face of T.
 '0': the open segment does not intersect triangle T.
 */
  char RLineSegCrossTri(Vertex3d& LineStr, Vertex3d& LineEnd, Vertex3d& pt1, Vertex3d& pt2, Vertex3d& pt3);

 /**�ж�Line1Str��Line1End��Line2Str��Line2End�ֱ𹹳ɵ������߶εĿռ��ϵ��
 @param �� [IN] Line1Str  �߶�1��㡣
 @param �� [IN] Line1End  �߶�1�յ㡣
 @param �� [IN] Line2Str  �߶�2��㡣
 @param �� [IN] Line2End  �߶�2�յ㡣
 @param �� [IN] bOnPlane	 �����Ĺ����ǡ�
 @param �� [OUT] pt       ��õ����߶ν��㡣
 @return �� '1'- ���߶��н���;  '0'- ���߶�����; '3'	- ���߶�ƽ��; '2'-���治�ཻҲ��ƽ�С�
 */
  char RLineSegWithLineSeg(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, int bOnPlane = 0, Vertex3d* pCrossPt = NULL);//xxy//


 /**�ж������������ε��ཻ�����
 @param ��  [IN] LineStr  ������㡣
 @param �� [IN] LineEnd  �����յ㡣
 @param �� [IN] pt1,pt2,pt3  �������������㡣
 @return �� 'V'- ��������������ζ����غ�;  'E'- ��������������α߽��غ�; 'F'- �����������������;
 'v'- ���ߺ�ƽ��Ľ����������ζ����غ�; 'e'- ���ߺ�ƽ��Ľ����������ζ����غ�; 'f'- ���ߺ�ƽ��Ľ��㴩��������;
 '0'- �������
 */
  char RRayCrossTri(Vertex3d& LineStr, Vertex3d& LineEnd, Vertex3d& pt1, Vertex3d& pt2, Vertex3d& pt3);

 // 1 - �ཻ 0 - ���ཻ
  int IsRayCrossEnvelope(RayImpl& ray, const Envelope3d& env, double t0 = 0.0, double t1 = MathEx::dDoubleMax);
  int IsRayCrossEnvelope(RayImpl& r, const AABBoxImpl & env, double t0 = 0.0, double t1 = MathEx::dDoubleMax);

  char RLineSegWithLine(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, int bOnPlane = 0, Vertex3d* pCrossPt = NULL);

 /**�ж�Line1Str��Line1End��Line2Str��Line2End�ֱ𹹳ɵ�����ֱ�ߵĿռ��ϵ��
 @param �� [IN] Line1Str  �߶�1��㡣
 @param �� [IN] Line1End  �߶�1�յ㡣
 @param �� [IN] Line2Str  �߶�2��㡣
 @param �� [IN] Line2End  �߶�2�յ㡣
 @param �� [OUT] pt    ��õ����߶ν��㡣
 @return �� '1'- ��ֱ���н���;  '0'- ��ֱ������; '2'	- ��ֱ��ƽ��;
 */
  char RLineWithLine(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, int bOnPlane = 0, Vertex3d* pIntersect = NULL);
 /** @} */ // ģ���β

 // return : '0' - ����ƽ�е����غ�  '2' �����غ� '1' - �����ཻ
  char RPlaneWithPlane(PLANE3D& plane1, PLANE3D& plane2, Vertex3d& pt1, Vertex3d& pt2);

 // return : '0' - ��������  '2' - �������� '1' - ��������
  char RPointWithPlane(Vertex3d& pt, PLANE3D& plane);

 // return : '0' - box����׵����  '2' - �ཻ '1' - ����׵����
  char RFrustumWithEnvelope(FrustumImpl& frusum, Envelope3d& env);
  char RFrustumWithEnvelope(FrustumImpl& frusum, AABBoxImpl & env);

 // return : '0' - ������׵����  '2' - �ཻ '1' - ����׵����
  char RFrustumWithSphere(FrustumImpl& frusum, Vertex3d& cen, double radius);

  char RFrustumWithTriangle(FrustumImpl& frustum, Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3);

  char RFrustumWithPolygon(FrustumImpl& frustum, vector<Vertex3d>& PtsArray);

 // return : 1 - �����ཻ��0 - �������룻 �����������֮�⣬���������ཻ // ���ﻹ����д�����������ж�sphere֮��ĸ��ֹ�ϵ
  int IsSphereIntersectSphere(Vertex3d& cen1, double radius1, Vertex3d& cen2, double radius2);

 // return : '0' - ����Բ׵����  '2' - �ཻ '1' - ��Բ׵����
  char RConeWithSphere(double dAngle, Vertex3d& vStart, Vertex3d& vDir, Vertex3d& shpCen, double radius);

 // return : 1 - ����Բ׶�ཻ��Բ׶������Ҳ�㣩��0 - ����Բ׶��
  int IsConeIntersectSphere(double dAngle, Vertex3d& vStart, Vertex3d& vDir, Vertex3d& shpCen, double radius);

 // �ж϶�ά�߶��ཻ���
  char RLineSegWithLineSeg2d(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, Vertex3d* pCrossPt = NULL);//xxy//

  char RRectWithPolygon2d(Vertex3d& ptMin, Vertex3d& ptMax, vector<Vertex3d>& vPts);
  char RRayWithLineSeg(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, int bOnPlane, Vertex3d* pCrossPt);

  char RRayWithLine(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, int bOnPlane, Vertex3d* pCrossPt);

  char RRayWithRay(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, int bOnPlane, Vertex3d* pCrossPt);

  char RRayWithPlane(Vertex3d& LineStr, Vertex3d& LineEnd, PLANE3D& plane, Vertex3d* pCrossPt);

  int IsPointInRay(Vertex3d& pt, Vertex3d& pt1, Vertex3d& pt2, Vertex3d* pFootPt);
 // xx_add_for_test_20100321
  void FuzzyPrecision_base(Vertex3d& pos, int nMedian);
  void FuzzyPrecision_base(double& dData, int nMedian);
  void FuzzyPrecision_base(float& dData, int nMedian);
 // xx_add_for_test_20100321



  /***********************************************/
  // ���ں�׺��ΪEx�ĺ����������ֵϵ�к����У���Inpϵ�к����У�����ʾ�������㼯���в�ֵ,�õ��ļ���ֻ�в�ֵ�㣬������ԭ���Ŀ��Ƶ�

  // ע�ⲻͬ�Ĳ�ֵ��ʽ�����ܿ��Ƶ�������ı䣬����BSpline
  // ���еĲ�ֵ��ʽ����ʹ���������ն���ʽ��ֵ����Ϊ�ߴβ���ȡ��������������ֲ������Բ�ǿ�����Բ��õĶ��ǷֶεȾ�ʹβ�ֵ��
  // �����Ƿֶεʹβ�ֵ��������Ҫ���ǲ�ͬ���߶�֮������������⡣C0��ʾλ��������C1��ʾһ�׵�������
  // ���ֶַ����ζ���ʽû����õı�﷽�������Ǹ���ʵ��Ӧ�������ѡ��һ����˵�����ܾ߱����������е�3�����ʣ����������е����ʣ�
  // ��1�����ߵ�ÿһƬ�ζ������ε�
  // ��2�����߶Կ��Ƶ��ֵ��������ͨ�����е���ֵ�㣩
  // ��3�������оֲ�������
  // ��4��������C2������
  /***********************************************/

  // ���Բ�ֵ
  // ����ֵ����ֵ�㣻��ֵ������һ�����Ƶ�ľ��룻��ֵ��֮��2�����Ƶ�ľ��룻��һ�����Ƶ��������
Vertex3d InpByLinearDis(std::vector<Vertex3d>& ptArray, double dStepLength, int &nCurPathNode, double &dDistanceAccumulation, double &dist_std);

  // ���嵯�Բ�ֵ
  // ������һ������vTrans������camPos����������λ��
Vertex3d InpByLinearDis(std::vector<Vertex3d>& ptArray, int &nCurPathNode, Vertex3d vTrans, double& dDistanceAccumulation);

  // ����Hermite��ֵ��Ҳ��Ϊ���β����������� �Կ��Ƶ��ֵ���оֲ������ԣ�����û��C2������
Vertex3d InpByCubicHermite(Vertex3d& P0, Vertex3d& D0, Vertex3d& P1, Vertex3d& D1, double t);

  // ����Hermite��ֵ������KochanekBartels����
Vertex3d InpByKochanekBartels(Vertex3d& P0, Vertex3d& P1, Vertex3d& P2, Vertex3d& P3, double t);

Vertex3d InpByKochanekBartels(std::vector<Vertex3d>& ptArray, int iIndex, double t);

void	InpByKochanekBartels(std::vector<Vertex3d>& ptArray, double dInpDis, std::vector<Vertex3d>& allPtArray, std::vector<int>& indexArray);

  // ����Hermite��ֵ���������������е�����Catmull-Rom����
Vertex3d InpByCatmullRom(Vertex3d& P0, Vertex3d& P1, Vertex3d& P2, Vertex3d& P3, double t);

Vertex3d InpByCatmullRom(std::vector<Vertex3d>& ptArray, int iIndex, double t);

  // ����ֵ����ֵ�����еĵ㼯���������Ƶ㣻���ҷ��ؿ��Ƶ������е������ż���
void	InpByCatmullRom(std::vector<Vertex3d>& ptArray, double dInpDis, std::vector<Vertex3d>& allPtArray, std::vector<int>& indexArray);

  // ����ֵ����ֵ�����в�ֵ�õ��ĵ㼯�����������Ƶ㣻���ҷ���ÿһ�β�ֵ��ĸ�������
  //void	InpByCatmullRomEx(std::vector<Vertex3d>& ptArray, double dInpDis,std::vector<Vertex3d>& inpPtArray,std::vector<int>& indexArray);

  // ��������B�������оֲ������ԣ���C2�����ԣ����ǲ��Կ��Ƶ��ֵ
Vertex3d InpByCubicBSpline(Vertex3d& P0, Vertex3d& P1, Vertex3d& P2, Vertex3d& P3, double t);

Vertex3d InpByCubicBSpline(std::vector<Vertex3d>& ptArray, int iIndex, double t);

void	InpByCubicBSpline(std::vector<Vertex3d>& ptArray, double dStepLength, std::vector<Vertex3d>& allPtArray, std::vector<int>& indexArray);

  // ����Bezier��������C2�����ԣ�ͨ����һ�����һ�����Ƶ㣬û�оֲ�������
Vertex3d InpByCubicBezier(Vertex3d& P0, Vertex3d& P1, Vertex3d& P2, Vertex3d& P3, double t);

Vertex3d InpByCubicBezier(std::vector<Vertex3d>& ptArray, int iIndex, double t);

void	InpByCubicBezier(std::vector<Vertex3d>& ptArray, double dStepLength, std::vector<Vertex3d>& allPtArray, std::vector<int>& indexArray);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// Public interface for Strip and VertexList Optimization based on NVTriStrip from nVidia
////////////////////////////////////////////////////////////////////////////////////////
#define CACHESIZE   32//32 for G80 24 for pre-G80
enum PrimType
{
	PT_LIST,
	PT_STRIP,
	PT_FAN
};
struct PrimitiveGroup //structure for strip
{
	PrimType type;
	unsigned long numIndices;
	unsigned long* indices;

	////////////////////////////////////////////////////////////////////////////////////////
	PrimitiveGroup() : type(PT_STRIP), numIndices(0), indices(NULL) {}
	~PrimitiveGroup()
	{
		if (indices)
			delete[] indices;
		indices = NULL;
	}
};
/** ���ݸ������������б��Mesh�����Ż�
* @param [in] isStrip -- true ����һ������ false ���Ż��б�
* @param [in] in_indices -- ���������ζ��������б�
* @param [in] in_numIndices -- ����Ķ�����������
* @param [out] primGroups -- ���������(�б�)
* @param [out] numGroups -- ���������(�б�)����
* @return true -- �Ż��ɹ�; false -- �Ż�����
*/
bool GenerateStripsEx(bool isStrip, const unsigned long* in_indices, const unsigned long in_numIndices, PrimitiveGroup** primGroups, unsigned long* numGroups);
/** ���ݸ������������б��Mesh�����Ż�
* @param [in] isStrip -- true �������� false ���Ż��б�
* @param [in] isStitch -- true ����һ������(Ĭ��) false ���ɶ������
* @param [in] in_MinStripSize -- ���ɶ������ʱ��������С�����ж������(Ĭ��Ϊ0)
* @param [in] in_indices -- ���������ζ��������б�
* @param [in] in_numIndices -- ����Ķ�����������
* @param [out] primGroups -- ���������(�б�)
* @param [out] numGroups -- ���������(�б�)����
* @return true -- �Ż��ɹ�; false -- �Ż�����
*/
bool GenerateStripsExEx(bool isStrip, bool isStitch, const unsigned long in_MinStripSize, const unsigned long* in_indices, const unsigned long in_numIndices,
	PrimitiveGroup** primGroups, unsigned long* numGroups);

////////////////////////////////////////////////////////////////////////////////////////////////////


end_gdb_namespace
end_gtl_namespace



