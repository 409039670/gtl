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
#include "internal_trianglemesh3dimpl.h"


begin_gtl_namespace
begin_gdb_namespace
//ʵ�ֶ�ά���ǻ��Ĺ���,�Ѽ����Χ�У���Ҫ����ά�� TriangleMesh3dImpl* mesh

/** ����ͬһƽ���ϵ���ά��ɢ��Vertex3d���ݹ�����ά������
* @param [in] bIsClose -- ������������Ƿ���
* @param [in] normal -- ָ��ƽ��ķ��򣬴���NULL���Զ�����
* @param [in] featurelines -- ����������ߣ�������������û������NULL
* @param [in] holemarks -- �����ڻ��е����ӵ����������ڻ���û������NULL
* @param [in] verts -- ����Ķ�ά��ɢ��
* @param [in out] mesh -- �����������(��ʱ������)
* @return true -- �����ɹ�; false -- ��������
*/
bool TriangulationFrmVerts(bool bIsClose, Vertex3d* normal, vector<int>* featurelines, vector<Vertex3d>* holemarks, vector<Vertex3d> &verts, TriangleMesh3dImpl* mesh);

/** ����ͬһƽ���ϵ���ά��ɢ��VersatileVertex3d���ݹ�����ά������
* @param [in] bIsClose -- ������������Ƿ���
* @param [in] normal -- ָ��ƽ��ķ��򣬴���NULL���Զ�����
* @param [in] featurelines -- ����������ߣ�������������û������NULL
* @param [in] holemarks -- �����ڻ��е����ӵ����������ڻ���û������NULL
* @param [in] verts -- ����Ķ�ά��ɢ��
* @param [in] m_iVertexType -- ���������
* @param [in out] mesh -- �����������(��ʱ������)
* @return true -- �����ɹ�; false -- ��������
*/
bool TriangulationFrmVertsEx(bool bIsClose, Vertex3d* normal, vector<int>* featurelines, vector<Vertex3d>* holemarks, vector<VersatileVertex3d> &verts, unsigned char m_iVertexType, TriangleMesh3dImpl* mesh);

/** ������ά���Ի���������Լ���Ķ�ά������
* @param [in] bIsClose -- true
* @param [in] holemarks -- �����ڻ��е����ӵ����������ڻ���û������NULL
* @param [in] rings -- ����Ķ�ά���Ի�
* @param [in] iVertexType -- ���������
* @param [in out] mesh -- �����������(��ʱ������)
* @param [in] normal -- ���ķ���
* @return true -- �����ɹ�; false -- ��������
*/
bool TriangulationFrmRings(bool bIsClose, vector<Vertex3d> *holemarks, vector<LinearRing3dImpl*>& rings, unsigned char iVertexType, TriangleMesh3dImpl* mesh, Vertex3d* normal = NULL);

/** ���ݵ������Ի������ʷ�Ϊ������
* @param [in] ring -- ��������Ի�
* @param [in] iVertexType -- ���������
* @param [in out] mesh -- �����������(��ʱ������)
* @param [in] normal -- ���ķ���
* @return true -- �����ɹ�; false -- ��������
*/
bool FastTriangulationFrmSimpleRing(LinearRing3dImpl* ring, unsigned char iVertexType, TriangleMesh3dImpl*& mesh, Vertex3d* normal = NULL);

/** ���������Ի�����TOL���������Լ��
* @param [in] ring -- ��������Ի�
* @param [in] TOL -- ������ֵ
* @return
*/
void eliminateRedundancy(LinearRing3dImpl* ring, double TOL = MathEx::TOL);

end_gdb_namespace
end_gtl_namespace


