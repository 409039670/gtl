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
#include "geometry3d.h"
#include "internal_point3dimpl.h"


begin_gtl_namespace
begin_gdb_namespace
class Annotation3dImpl : public Point3dImpl, public Annotation3d
{
public:

	/** ���ƽ������ֵ
	* @param
	* @return	ƽ������ֵ
	*/
	virtual Vertex3d getPosition() const{
		return Vertex3d(m_vPoint.x, m_vPoint.y, m_vPoint.z);
	}
	/** �趨ƽ������ֵ
	* @param [in] pos	��ά��
	* @return
	*/
	virtual void setPosition(Vertex3d pos){
		m_vPoint.x = pos.x;
		m_vPoint.y = pos.y;
		m_vPoint.z = pos.z;
	}

	/**  ��ȡ��λ����
	* @param  ��
	* @return ��λ����
	*/
	virtual Quaternion getOrientation(){
		return Quaternion(m_qOrientation.x(), m_qOrientation.y(), m_qOrientation.z(), m_qOrientation.w());
	}
	/**  ���÷�λ����
	* @param  [in] q  ��λ����
	* @return ��
	*/
	virtual void setOrientation(const Quaternion & q) {
		SetOrientation(Quaternion(q.w(), q.x(), q.y(), q.z()));
	}

	/**  ��ȡע������
	* @param  ��
	* @return ע������
	*/
	virtual long getAnnotationType(){
		return GetAnnoType();
	}
	/**  ����ע������
	* @param  [in] t  ע������
	* @return ��
	*/
	virtual void setAnnotationType(long t){
		SetAnnoType(t);
	}

	/**  ��ȡע�Ƿ��
	* @param  ��
	* @return ע�Ƿ��
	*/
	virtual unsigned char getAnnotationStyle() {
		return m_byteStyle;
	}
	/**  ����ע�Ƿ��
	* @param  [in] c  ע�Ƿ��
	* @return ��
	*/
	virtual void setAnnotationStyle(unsigned char c){
		m_byteStyle = c;
	}

	/**  ��ȡע���ִ�
	* @param  [in] ��
	* @return ע���ִ�
	*/
	virtual std::string getAnnotationString(){
		return m_strAnnoString;
	}
	/**  ����ע���ִ�
	* @param  [in] s  ע���ִ�
	* @return ��
	*/
	virtual void setAnnotationString(const std::string &  s){
		SetAnnoString(s);
	}

	/**  ��ȡ��������
	* @param  ��
	* @return ��������
	*/
	virtual std::string getFontName(){
		return GetFontName();
	}
	/**  ������������
	* @param  [in] s  ��������
	* @return ��
	*/
	virtual void setFontName(const std::string & s){
		SetFontName(s);
	}

	/** ע���ָߺ��ֿ���16Ϊ�ֿ���16λΪ�ָ�;0.01mmΪ��λ */
	/**  ��ȡע���ָ�
	* @param  ��
	* @return ע���ָ�
	*/
	virtual long getHeight() {
		return GetHeight();
	}
	/**  ����ע���ָ�
	* @param  [in] h  ע���ָ�
	* @return ��
	*/
	virtual void setHeight(long h){
		SetHeight(h);
	}

	/** ע���ִ����ֿ��Ե���ע����Ч���Բ������Ч;0.01mmΪ��λ */
	/**  ��ȡע���ִ��Ŀ��
	* @param  ��
	* @return ע���ִ��Ŀ��
	*/
	virtual long getWidth(){
		return GetWidth();
	}
	/**  ����ע���ִ��Ŀ��
	* @param  [in] w  ע���ִ��Ŀ��
	* @return ��
	*/
	virtual void setWidth(long w){
		SetWidth(w);
	}

	/** ע���ּ��;0.01mmΪ��λ */
	/**  ��ȡע���ּ��
	* @param  ��
	* @return ע���ּ��
	*/
	virtual long getSpaces(){
		return GetSpaces();
	}
	/**  ����ע���ּ��
	* @param  [in] s  ע���ּ��
	* @return ��
	*/
	virtual void setSpaces(long s){
		SetSpaces(s);
	}

	/** ע����ɫ */
	/**  ��ȡע����ɫ
	* @param  ��
	* @return ע����ɫ
	*/
	virtual Color4f getColor() {
		Color4f c4f;
		Color::convert(GetColor(), c4f);
		return c4f;
	}
	/**  ����ע����ɫ
	* @param  [in] clAnnotation  ע����ɫ
	* @return ��
	*/
	virtual void setColor(const Color4f & c4f){
		long c;
		Color::convert(c4f, c);
		SetColor(c);
	}


	/**  ��ѯ����ӿ�
	* @param  [in] signal  �������ͱ�ʶ
	* @param  [out] p ��Ӧ�Ķ���ָ��
	* @return 1 -- ��ѯ�ɹ���0 -- ��ѯʧ��
	*/
	virtual bool queryInterface(int signal, void ** p){
		if (GEOMTYPE_3D_POINT == signal){
			*p = (void*)(Point3d *) this;
			return true;
		}
		else if (GEOMTYPE_3D_ANNOTATION == signal)
		{
			*p = (void*)(Annotation3d *)this;
			return true;
		}
		else
			return false;
	}
	/** @defgroup Annotation3dImpl Annotation3dImpl-��άע��
	*  @{
	*/
protected:

	/** @name ���ԣ���Ա������
	* @{
	*/

	/** @brief ��ת���� */
	Quaternion m_qOrientation;

	/** @brief ע������ */
	long m_lAnnoType;

	/** @brief ע�Ƿ�� */
	unsigned char m_byteStyle;

	/** @brief ע���ַ��� */
	string m_strAnnoString;

	/** @brief ע���ָߺ��ֿ���16Ϊ�ֿ���16λΪ�ָ�;0.01mmΪ��λ */
	string m_strFontName;

	/** @brief ע���ָߺ��ֿ���16Ϊ�ֿ���16λΪ�ָ�;0.01mmΪ��λ */
	long m_lHeight;

	/** @brief ע���ִ����ֿ��Ե���ע����Ч���Բ������Ч;0.01mmΪ��λ */
	long m_lWidth;

	/** @brief ע���ּ��;0.01mmΪ��λ */
	long m_lSpaces;

	/** @brief ע����ɫ */
	long m_lColor;

	/** @} */ // ���Խ�β


	/** @defgroup Annotation3dImpl ��������Ա������
	* @{
	*/
public:

	/** @name ��������������
	*  @{
	*/

	/** Ĭ�Ϲ��캯��
	*/
	Annotation3dImpl(void);
	/** Ĭ����������
	*/
	virtual ~Annotation3dImpl(void);

	/** @} */ // ����������������β


	/** @name ���ݲ���
	*  @{
	*/

	/** ���ƽ������ֵ
	* @param
	* @return	ƽ������ֵ
	*/
	Vertex3d GetPosition() const;
	/** �趨ƽ������ֵ
	* @param [in] pos	��ά��
	* @return
	*/
	void SetPosition(Vertex3d pos);

	/** �����Ԫ�������α任�е���ת����
	* @param
	* @return	��Ԫ��ֵ
	*/
	Quaternion GetOrientation() const;
	/** �趨��Ԫ��
	* @param [in] orientation	��֪��Ԫ��
	* @return
	*/
	void SetOrientation(Quaternion orientation);

	/** ���ע������
	* @param
	* @return	ע������
	*/
	long GetAnnoType() const;
	/** ����ע������
	* @param [in] lAnnoType	��֪ע������
	* @return
	*/
	void SetAnnoType(long lAnnoType);

	/** ����ַ�������
	* @param
	* @return	�ַ�������
	*/
	std::string GetAnnoString() const;
	/** ����ַ�������
	* @param [in] annoString	��֪�ַ���
	* @return
	*/
	void SetAnnoString(std::string annoString);

	/** ���ע������
	* @param
	* @return	ע������
	*/
	string GetFontName() const;
	/** ����ע������
	* @param [in] fontName	��֪ע������
	* @return
	*/
	void SetFontName(string fontName);

	/** ���ע�Ƿ��
	* @param
	* @return ע�Ƿ��
	*/
	unsigned char GetStyle() const;
	/** ����ע�Ƿ��
	* @param [in] bStyle	��֪ע�Ƿ��
	* @return
	*/
	void SetStyle(unsigned char bStyle);

	/** ���ע���ָ�
	* @param
	* @return ע���ָ�
	*/
	long GetHeight() const;
	/** ����ע���ָ�
	* @param [in] lHeight	��֪�ָ�
	* @return
	*/
	void SetHeight(long lHeight);

	/** ���ע���ֿ�
	* @param
	* @return ע���ֿ�
	*/
	long GetWidth() const;
	/** ����ע���ָ�
	* @param [in] width	��֪�ֿ�
	* @return
	*/
	void SetWidth(long width);

	/** ���ע���ּ��
	* @param
	* @return ע���ּ��
	*/
	long GetSpaces() const;
	/** ����ע���ּ��
	* @param [in] lSpace	��֪�ּ��
	* @return
	*/
	void SetSpaces(long lSpace);

	/** ���ע����ɫ
	* @param
	* @return ע����ɫ
	*/
	long GetColor() const;
	/** ����ע����ɫ
	* @param [in] color	��֪ע����ɫ
	* @return
	*/
	void SetColor(long color);

	/** @} */ // ���ݲ������β

public:

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

	/** �麯��������������ʵ��
	* ���㵱ǰ����ļ�������
	* @return ����ļ�������
	*/
	virtual Vertex3d CalCorePoint();

	/** �麯��������������ʵ��
	* �����ⲿָ������ά�߶ε���ǰ���ζ�������ƽ��
	* @param [in] vPt	Vertex3d������ά����������
	* @return �߶ε���ǰ���ζ�������ƽ��
	*/
	virtual double D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt);


	/** �麯��������������ʵ��
	* �����ⲿָ���������ε���ǰ���ζ�������ƽ��
	* @param [in] triPt1, triPt2, triPt3Vertex3d������ά����������(��ʾ��������������)
	* @return �߶ε���ǰ���ζ�������ƽ��
	*/
	virtual double D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3);

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
	virtual int RGeoWithPolygon2D(vector<Vertex3d>& ptArray, int precisionFlag = 0);

	/** �ͷŶ������ݲ���ָ��ͱ�������ʼֵ
	* @return �Ƿ�ִ�гɹ�
	*/
	virtual bool Init();

	/** @} */ // ����������β

	/** @} */ // ������β

	/** @} */ // ģ���β
};
 
end_gdb_namespace
end_gtl_namespace


