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
#include "internal_surface3dimpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
class Polygon3dImpl;
class  Rectangle3dImpl : virtual public cn::edu::cug::gdb::Rectangle3d, virtual public Surface3dImpl
{
	/** @defgroup Rectangle3dImpl Rectangle3dImpl-��ά����
	*  @{
	*/

protected:
	/** @name ���ԣ���Ա������
	* @{
	*/

	/** �������� */
	Vertex3d m_vVertices[4];

	/** @} */ // ���Խ�β


public:

	/** @defgroup Rectangle3dImpl ��������Ա������
	* @{
	*/
	/** ����γ���
	* @return ����ֵ
	*/
	virtual double getLength(){ return 0.0; }	//$$

	/** ��ȡ���ο��
	* @return ���ֵ
	*/
	virtual double getWidth(){ return 0.0; }	//$$

	/** @name ��������������
	*  @{
	*/

	/** @} */ // ����������������β



	/** @name ���ݲ���
	*  @{
	*/

	/** ��ö�������
	* @param [out] vVertices ������ֵ�Ķ�������ָ�������
	* @return
	*/
	void GetVertices(Vertex3d*& vVertices)
	{
		vVertices = m_vVertices;
	}

	/** @} */ // ���ݲ������β



	/** @name �������
	*  @{
	*/

	/** ����γ���
	* @return ����ֵ
	*/
	double Length(){ return 0.0; }//$$

	/** ��ȡ���ο��
	* @return ���ֵ
	*/
	double Width(){ return 0.0; }//$$

	/** ��ȡ�������
	* @return ���
	*/
	double Area(){ return 0.0; }//$$

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

	/** �麯��������������ʵ��
	* �����ⲿָ������ά�߶ε���ǰ���ζ�������ƽ��
	* @param [in] begvPt	Vertex3d������ά����������
	* @param [in] endvPt	Vertex3d������ά����������
	* @return �㵽��ǰ���ζ�������ƽ��
	*/
	virtual double D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt);

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
	virtual int RGeoWithPolygon2D(vector<Vertex3d>& ptArray, int precisionFlag = 0)
	{
		return -1;// ydcl
	}

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
