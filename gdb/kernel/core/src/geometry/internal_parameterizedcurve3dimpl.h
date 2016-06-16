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
#include <utility>
#include "internal_curve3dimpl.h"


begin_gtl_namespace
begin_gdb_namespace
enum
{
	PARAMETERIZEDCURVE_INVALID,
	PARAMETERIZEDCURVE_BEZIER,
	PARAMETERIZEDCURVE_BSPLINE,
	PARAMETERIZEDCURVE_NURBS,
	PARAMETERIZEDCURVE_CATMULLROM,
};
//##ModelId=49E3472700AB
class  ParameterizedCurve3dImpl : public Curve3dImpl, virtual public ParameterizedCurve3d
{
	/** @defgroup ParameterizedCurve3dImpl ParameterizedCurve3dImpl-��ά��������
	*  @{
	*/

protected:
	/** @name ���ԣ���Ա������
	* @{
	*/

	/** �������ͣ������� Bezier, B-Spline, NURBS�ȣ��������������������õĲ������� */
	unsigned char m_iParameterizedCurveType;

	/** ���Ƶ� */
	std::vector<Vertex3d>	m_vControlPoints;

	/** �����������޶��Ĳ������䣬�Ը������Ա�ʾ */
	std::pair<float, float> m_paParamInterval;

	/** �������������õĲ������������ɾ����������ߵ�ĸ�����Ҳ���ɸ������������ */
	float m_fParamStep;

	/** @} */ // ���Խ�β


public:

	/** @defgroup ParameterizedCurve3dImpl ��������Ա������
	* @{
	*/
	/** ����������ͱ��
	* @param
	* @return �������ͱ��ֵ
	*/
	virtual unsigned char getParameterizedType() const
	{
		return m_iParameterizedCurveType;
	}

	/** ������������
	* @param [in] iType	��֪��������ֵ
	* @return ��
	*/
	virtual void setParameterizedType(unsigned char iType)
	{
		m_iParameterizedCurveType = iType;
	}

	/** ��ÿ��Ƶ�����
	* @param [out] vControlPoints ������ֵ�Ŀ��Ƶ����������
	* @return
	*/
	virtual void getControlVertices(vector<Vertex3d>& vControlPoints) const
	{
		vControlPoints = m_vControlPoints;
	}

	/** ���ÿ��Ƶ�����
	* @param [in] vControlPoints	��֪���Ƶ����������
	* @return
	*/
	virtual void setControlVertices(const vector<Vertex3d>& vControlPoints)
	{
		m_vControlPoints = vControlPoints;
	}

	/** @name ��������������
	*  @{
	*/

	/** ���캯��1
	*/
	ParameterizedCurve3dImpl()
	{
		m_byteTypeID = GEOTYPE_3D_PARAMETERIZEDCURVE;
	}
	/** ���캯��2
	*/
	ParameterizedCurve3dImpl(vector<Vertex3d>& vControlPoints, pair<float, float>& paParamInterval, float fParamStep, unsigned char iType = PARAMETERIZEDCURVE_INVALID)
		:m_vControlPoints(vControlPoints), m_paParamInterval(paParamInterval), m_fParamStep(fParamStep), m_iParameterizedCurveType(iType)
	{
		m_byteTypeID = GEOTYPE_3D_PARAMETERIZEDCURVE;
	}

	/** @} */ // ����������������β



	/** @name ���ݲ���
	*  @{
	*/

	/** ����������ͱ��
	* @param
	* @return �������ͱ��ֵ
	*/
	unsigned char GetType() const
	{
		return m_iParameterizedCurveType;
	}

	/** ��ÿ��Ƶ�����
	* @param [out] vControlPoints ������ֵ�Ŀ��Ƶ����������
	* @return
	*/
	void GetControlVertices(vector<Vertex3d>& vControlPoints) const
	{
		vControlPoints = m_vControlPoints;
	}

	/** ����������������õĲ�������
	* @param
	* @return ��������ֵ
	*/
	float GetParamStep() const
	{
		return m_fParamStep;
	}

	/** ��������������޶��Ĳ�������
	* @param [out] paParamInterval
	* @return �����������޶��Ĳ������䷶Χ
	*/
	void GetParamInterval(pair<float, float>& paParamInterval) const
	{
		paParamInterval = m_paParamInterval;
	}

	/** ������������
	* @param [in] iType	��֪��������ֵ
	* @return
	*/
	void SetType(unsigned char iType)
	{
		m_iParameterizedCurveType = iType;
	}

	/** ���ÿ��Ƶ�����
	* @param [in] vControlPoints	��֪���Ƶ����������
	* @return
	*/
	void SetControlVertices(const vector<Vertex3d>& vControlPoints)
	{
		m_vControlPoints = vControlPoints;
	}

	/** �����������������õĲ�������
	* @param [in] fParamStep  ���趨�Ĳ���ֵ
	* @return
	*/
	void SetParamStep(float fParamStep)
	{
		m_fParamStep = fParamStep;
	}

	/** ���������������޶��Ĳ�������
	* @param [in] paParamInterval	��֪�������������
	* @return
	*/
	void SetParamInterval(const pair<float, float>& paParamInterval)
	{
		m_paParamInterval = paParamInterval;
	}

	/** @} */ // ���ݲ������β



	/** @name �������
	*  @{
	*/

	/** ��������
	* @param [in] nPnt �����ϵĵ���
	* @return ���ߵ�����
	*/
	Vertex3d* Generate(int nPnt);

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


