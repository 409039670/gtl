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
#include "internal_surface3dimpl.h"



begin_gtl_namespace
begin_gdb_namespace
 
class TriangleMesh3dImpl;

enum
{
	PARAMETERIZEDSURFACE_INVALID,
	PARAMETERIZEDSURFACE_BEZIER,
	PARAMETERIZEDSURFACE_BSPLINE,
	PARAMETERIZEDSURFACE_NURBS,
	PARAMETERIZEDSURFACE_CATMULLROM,
};

//##ModelId=49E347280109
class ParameterizedSurface3dImpl :public Surface3dImpl, virtual public ParameterizedSurface3d
{
	/** @defgroup ParameterizedSurface3dImpl ParameterizedSurface3dImpl-��ά��������
	*  @{
	*/

protected:

	/** @name ���ԣ���Ա������
	* @{
	*/

	/** �������ͣ������� Bezier, B-Spline, NURBS�ȣ��������������������õĲ������� */
	unsigned char m_iParameterizedSurfaceType;

	/** �����������ĵ����飬���ж�̬�ߴ� */
	std::vector<Vertex3d> m_vControlPoints;

	/** �����������u�������� */
	int m_nCtrlPntNumU;

	/** �����������v�������� */
	int m_nCtrlPntNumV;

	/** �����������޶���u�������䣬�Ը������Ա�ʾ */
	std::pair<float, float> m_paParamInterU;

	/** �����������޶���v�������䣬�Ը������Ա�ʾ */
	std::pair<float, float> m_paParamInterV;

	/** �������������õ�u�������������ɾ�����Ҫ��������u����ĵ��� */
	float m_fParamStepU;

	/** �������������õ�v�������������ɾ�����Ҫ��������v����ĵ��� */
	float m_fParamStepV;

	/** @} */ // ���Խ�β

public:
	/**  ��ѯ����ӿ�
	* @param  [in] signal  �������ͱ�ʶ
	* @param  [out] p ��Ӧ�Ķ���ָ��
	* @return 1 -- ��ѯ�ɹ���0 -- ��ѯʧ��
	*/
	virtual bool queryInterface(int signal, void ** p){
		switch (signal){
		case GEOMTYPE_3D_SURFACE:
		{
			*p = (void*)(Surface3d *) this;
			return true;
		}
		case GEOMTYPE_3D_PARAMETERIZEDSURFACE:
		{
			*p = (void*)(ParameterizedSurface3d *) this;
			return true;
		}
		default:
		{
			return false;
		}
		}
	}
public:

	/** @defgroup ParameterizedSurface3dImpl ��������Ա������
	* @{
	*/
	/** ��ò������������ͱ��
	* @param  ��
	* @return �������������ͱ��ֵ
	*/
	virtual unsigned char getParameterizedType() const
	{
		return m_iParameterizedSurfaceType;
	}

	/** ���ò�������������
	* @param [in] iType	��֪��������������ֵ
	* @return ��
	*/
	virtual void setParameterizedType(unsigned char iType)
	{
		m_iParameterizedSurfaceType = iType;
	}

	/** ��ÿ��Ƶ�����
	* @param [out] vControlPoints ������ֵ�Ŀ��Ƶ����������
	* @return
	*/
	virtual void getControlVertices(vector<Vertex3d>& vControlPoints) const
	{
		GetControlVertices(vControlPoints);
	}

	/** ���ÿ��Ƶ�����
	* @param [in] vControlPoints	��֪���Ƶ����������
	* @return
	*/
	virtual void setControlVertices(const vector<Vertex3d>& vControlPoints)
	{
		SetControlVertices(vControlPoints);
	}

	/** @name ��������������
	*  @{
	*/

	/** ���캯��1
	*/
	ParameterizedSurface3dImpl()
	{
		m_byteTypeID = GEOTYPE_3D_PARAMETERIZEDSURFACE;
		m_iParameterizedSurfaceType = PARAMETERIZEDSURFACE_INVALID;
	}

	/** ���캯��2
	*/
	ParameterizedSurface3dImpl(vector<Vertex3d>& vControlPoints, int nCtrlPntNumU, int nCtrlPntNumV, pair<float, float>& paParamInterU, pair<float, float>& paParamInterV,
		float fParamStepU, float fParamStepV, unsigned char iType = PARAMETERIZEDSURFACE_INVALID) :
		m_vControlPoints(vControlPoints), m_nCtrlPntNumU(nCtrlPntNumU), m_nCtrlPntNumV(nCtrlPntNumV), m_paParamInterU(paParamInterU), m_paParamInterV(paParamInterV),
		m_fParamStepU(fParamStepU), m_fParamStepV(fParamStepV)
	{
		m_byteTypeID = GEOTYPE_3D_PARAMETERIZEDSURFACE;
	}

	/** @} */ // ����������������β



	/** @name ���ݲ���
	*  @{
	*/

	/** ����������ͱ��
	* @param
	* @return ��������
	*/
	unsigned char GetType() const
	{
		return m_iParameterizedSurfaceType;
	}

	/** ��������������ĵ�����
	* @param [out] vControlPoints �����������ĵ����������
	* @return
	*/
	void GetControlVertices(vector<Vertex3d>& vControlPoints) const
	{
		vControlPoints = m_vControlPoints;
	}

	/** ����������������õ�u,v��������
	* @param [out] fParamStepU ������ֵ��u��������
	* @param [out] fParamStepV ������ֵ��v��������
	* @return
	*/
	void GetParamStep(float& fParamStepU, float& fParamStepV) const
	{
		fParamStepU = m_fParamStepU;
		fParamStepV = m_fParamStepV;
	}

	/** ��������������޶���u,v��������
	* @param [out] paParamInterU ������ֵ��u��������
	* @param [out] paParamInterV ������ֵ��v��������
	* @return
	*/
	void GetParamInterval(pair<float, float>& paParamInterU, pair<float, float>& paParamInterV) const
	{
		paParamInterU = m_paParamInterU;
		paParamInterV = m_paParamInterV;
	}

	/** �����������Ͳ���
	* @param [in] iType ��֪���������Ͳ���
	* @return
	*/
	void SetType(unsigned char iType)
	{
		m_iParameterizedSurfaceType = iType;
	}

	/** ��������������ĵ�����
	* @param [in] vControlPoints ��֪�������������ĵ����������
	* @return
	*/
	void SetControlVertices(const vector<Vertex3d>& vControlPoints)
	{
		m_vControlPoints = vControlPoints;
	}

	/** ����������������õ�u,v��������
	* @param [out] fParamStepU ��֪��u������������
	* @param [out] fParamStepV ��֪��v������������
	* @return
	*/
	void SetParamStep(float fParamStepU, float fParamStepV)
	{
		m_fParamStepU = fParamStepU;
		m_fParamStepV = fParamStepV;
	}

	/** ��������������޶���u,v��������
	* @param [out] paParamInterU ��֪��u�������������
	* @param [out] paParamInterV ��֪��v�������������
	* @return
	*/
	void SetParamInterval(pair<float, float>& paParamInterU, pair<float, float>& paParamInterV)
	{
		m_paParamInterU = paParamInterU;
		m_paParamInterV = paParamInterV;
	}

	/** @} */ // ���ݲ������β



	/** @name �������
	*  @{
	*/

	/** ������������������������ʽ���ؽ��
	* @param [in] nPntU ��������u����ĵ���
	* @param [in] nPntV ��������v����ĵ���
	* @return �������������ָ��
	*/
	TriangleMesh3dImpl* Generate(int nPntU, int nPntV);

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


