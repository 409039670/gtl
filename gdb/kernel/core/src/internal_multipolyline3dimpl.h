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
#include "indexbuffer.h"
#include "geometry3d.h"
#include "internal_curve3dimpl.h"


begin_gtl_namespace
begin_gdb_namespace
class MultiPolyline3dImpl : virtual public MultiPolyline3d, virtual public Curve3dImpl
{
	/** @defgroup MultiPolyline3dImpl MultiPolyline3dImpl-��ά����
	*  @{
	*/

protected:
	/** @name ���ԣ���Ա������
	* @{
	*/

	//�߶��������
	//##ModelId=49E6DE6E01B5
	/** @brief ��������� */
	string m_name;

	//g��ɫ
	//##ModelId=49E6DF62006D
	/** @brief g��ɫ */
	unsigned long m_byG;

	//##ModelId=49E73513037A
	/** @brief ȱʡ��ɫ */
	unsigned long m_defaultColor;

	//##ModelId=49E7373F004E
	/** @brief �߿� */
	double m_dLineWidth;

	//##ModelId=49E735280119
	/** @brief ��ɫ���� */
	std::vector<unsigned long> m_color; //��ɫ���� 

	//##ModelId=49E73533036B
	/** @brief �Ƿ�ʹ������ */
	int m_bApplyDotColor;

	//##ModelId=49E7353C0213
	/** @brief ������������ */
	std::vector<int> m_indices; // ��������

	/** @} */ // ���Խ�β

public:

	/** @defgroup MultiPolyline3dImpl ��������Ա������
	* @{
	*/

	/**  ��ѯ����ӿ�
	* @param  [in] signal  �������ͱ�ʶ
	* @param  [out] p ��Ӧ�Ķ���ָ��
	* @return 1 -- ��ѯ�ɹ���0 -- ��ѯʧ��
	*/
	virtual bool queryInterface(int signal, void ** p){
		switch (signal){
		case GEOMTYPE_3D_CURVE:
		{
			*p = (void*)(Curve3d *) this;
			return true;
		}
		case GEOMTYPE_3D_MULTIPOLYLINE:
		{
			*p = (void*)(MultiPolyline3d *) this;
			return true;
		}
		default:
		{
			return false;
		}
		}
	}

	/** ��ȡ�ߵĿ��
	* @param  [in] lineWidth  ���߿��
	* @return ��
	*/
	virtual void setLineWidth(double& lineWidth)
	{
		m_dLineWidth = lineWidth;
	}

	/** ��ȡ�ߵĿ��
	* @param
	* @return �ߵĿ��
	*/
	virtual double getLineWidth()
	{
		return m_dLineWidth;
	}

	/** ��ȡ�ߵĳ���
	* @param
	* @return �߳�
	*/
	virtual double getLength(){ return 0.0; }//$$

	/**  ��ȡ������ߵĸ���
	* @param  ��
	* @return ������ߵĸ���
	*/
	virtual long getPolylineNumber(){ return 0; }	//$$

	/**  ��ȡ���߽ṹ�����
	* @param  [in] ib  ���߽ṹ�����
	* @return ��
	*/
	virtual void getPolylineIndexBuffer(IndexBuffer & ib){}	//$$

	/**  ��ȡ������߸��ԵĽ����Ŀ��
	* @param  [in] ib  ������߸��ԵĽ����Ŀ��
	* @return ��
	*/
	virtual void getVertexNumberPerPolyline(IndexBuffer & ib){}	//$$

	/** @name ��������������
	*  @{
	*/

	/** ���캯��
	*/
	MultiPolyline3dImpl()
	{
		m_byteTypeID = GEOTYPE_3D_MULTIPOLYLINE;
	}

	/** @} */ // ����������������β



	/** @name ���ݲ���
	*  @{
	*/

	//##ModelId=49E6DE6101C5
	/** �����ߵĿ��
	* @param [in] wWidth �߿�
	* @return �߿�
	*/
	void SetWidth(unsigned int wWidth);

	//##ModelId=49E6DE730119
	/** ��ȡ�ߵĿ��
	* @param
	* @return �߿�
	*/
	unsigned int GetWidth();

	//##ModelId=49E6DE92029F
	/** ��ȡ�ߵĳ���
	* @param
	* @return �߳�
	*/
	double Length();

	//##ModelId=49E7362E004E
	/** ��ȡ���߼����ϵ������
	* @param
	* @return �����
	*/
	unsigned int NumberGeometry();

	//##ModelId=49E73663005D
	/** ��ȡ������
	* @param
	* @return ������
	*/
	unsigned int GetDataSize();

	//##ModelId=49E73670034B
	/** ��ȡ���������ߴ�
	* @param
	* @return ���������ߴ�
	*/
	unsigned int GetIndexSize();

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

	/** �ͷŶ������ݲ���ָ��ͱ�������ʼֵ
	* @return �Ƿ�ִ�гɹ�
	*/
	virtual bool Init()
	{
		return true;
	}

	/** @} */ // ����������β

	/** @} */ // ������β

	/** @} */ // ģ���β
};

end_gdb_namespace
end_gtl_namespace


