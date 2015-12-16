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
#include "sharedModel.h"
#include "internal_Point3dImpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

#if defined (_MSC_VER) && (_MSC_VER > 1200)//����VC6�İ汾

typedef SharedModelSharedPtr LinkModelSharedPtr;

#endif//����VC6�İ汾

class  Reference3dImpl :virtual public Reference3d, virtual public Point3dImpl 
{
	/** @defgroup Reference3dImpl Reference3dImpl-��ά����ͼԪ
	*  @{
	*/
protected:
	/** @name ���ԣ���Ա������
	* @{
	*/

	/** @brief ��ת���� */
	Quaternion m_qOrientation;

	/** @brief ���ŷ��� */
	double m_dScale;

	/** @brief �����ļ����� */
	int m_nLinkType;

	/** @brief ��������ID */
	Identifier::raw_type  m_nLinkID;

	/** @brief ������������ */
	string m_strLinkName;

#if defined (_MSC_VER) && (_MSC_VER > 1200)//����VC6�İ汾
	/** @brief ����������ָ�� */ //�ڴ����ڲ��Զ���ͬά��( ��m_nLinkID ������ָ�� )
	LinkModelSharedPtr m_pLinkModel;
#else
	/** @brief ��������ָ�� */	//�ڴ����ⲿ����( ��m_nLinkID ������ָ�� )
	CLinkModel* m_pLinkModel;
#endif//64λID����

	/** @brief ��������(ռ���ڴ�)��С */
	int m_nExtParaSize;

	/** @brief �������� */
	void* m_pExtParaData;

	/** @} */ // ���Խ�β

public:

	/** @defgroup Reference3dImpl ��������Ա������
	* @{
	*/

	/** @name ��������������
	*  @{
	*/

	/** ���캯��
	*/
	Reference3dImpl();
	/** ��������
	*/
	virtual ~Reference3dImpl();

	/** @} */ // ����������������β



	/** @name ���ݲ���
	*  @{
	*/

	/** �����Ԫ��(��ת����)
	* @param
	* @return ��Ԫ��
	*/
	Quaternion GetOrientation() const
	{
		return m_qOrientation;
	}
	/** ������Ԫ��(��ת����)
	* @param [in] orientation ��֪��Ԫ��
	* @return
	*/
	void SetOrientation(Quaternion orientation)
	{
		m_qOrientation = orientation;
	}

	/** ��ù����ļ�����
	* @param
	* @return �����ļ�����
	*/
	int GetLinkType() const
	{
		return m_nLinkType;
	}
	/** ���ù����ļ�����
	* @param [in] nLinkType ��֪�����ļ�����
	* @return
	*/
	void SetLinkType(int nLinkType)
	{
		m_nLinkType = nLinkType;
	}

	/** ������ű���
	* @param
	* @return ���ű���
	*/
	double GetScale() const
	{
		return m_dScale;
	}
	/** �������ű���
	* @param [in] dScale ���ű���
	* @return
	*/
	void SetScale(double dScale)
	{
		m_dScale = dScale;
	}

	/** ��ù�����������
	* @param
	* @return ������������
	*/
	Identifier::raw_type GetLinkID() const
	{
		return m_nLinkID;
	}
	/** ���ù�����������
	* @param [in] nLinkID ������������
	* @return
	*/
	void SetLinkID(Identifier::raw_type nLinkID)
	{
		m_nLinkID = nLinkID;
	}

	/** ��ù�����������
	* @param
	* @return ������������
	*/
	std::string GetLinkName() const
	{
		return m_strLinkName;
	}
	/** ���ù�����������
	* @param [in] strLinkName ������������
	* @return
	*/
	void SetLinkName(std::string strLinkName)
	{
		m_strLinkName = strLinkName;
	}

#if defined (_MSC_VER) && (_MSC_VER > 1200)//����VC6�İ汾
	/** ��ù���ģ�Ͷ�����ָ��
	* @return  ����ģ�Ͷ�����ָ��
	*/
	LinkModelSharedPtr GetLinkModel()
	{
		return m_pLinkModel;
	}

	/** ���ù���ģ�Ͷ�����ָ��
	* @param [in] pLinkModel ����ģ�Ͷ�����ָ��
	* @return
	*/
	void SetLinkModel(LinkModelSharedPtr& pLinkModel)
	{
		m_pLinkModel = pLinkModel;
	}
#else
	/** ��ù���ģ�Ͷ�����ָ��
	* @return  ����ģ�Ͷ�����ָ��
	*/
	CLinkModel* GetLinkModel()
	{
		return m_pLinkModel;
	}

	/** ���ù���ģ�Ͷ�����ָ��
	* @param [in] pLinkModel ����ģ�Ͷ�����ָ��
	* @return
	*/
	void SetLinkModel(CLinkModel* pLinkModel)
	{
		m_pLinkModel = pLinkModel;
	}
#endif//����VC6�İ汾

	/** ����������ݴ�С�������׵�ַ
	* @param [out] nExtParaSize  �������ݴ�С
	* @param [out] pExeParaData  ���������յ�ַ
	* @return
	*/
	void GetExtParaData(int& nExtParaSize, void*& pExeParaData) const;
	/** �����������ݴ�С�������׵�ַ
	* @param [in] nExtParaSize �������ݴ�С
	* @param [in] pExeParaData ���������յ�ַ
	* @return
	*/
	void SetExtParaData(int nExtParaSize, void* pExeParaData);

	/** �����ⲿָ������ά����󵽵�ǰ���ζ�������ƽ��
	* @param [in] vPt	Vertex3d������ά����������
	* @return �㵽��ǰ���ζ�������ƽ��
	*/
	double DPointToGeometryEx(Vertex3d& vPt) const;

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

	/** �麯��������������ʵ��
	* �����ⲿָ���������ε���ǰ���ζ�������ƽ��
	* @param [in] triPt1, triPt2, triPt3Vertex3d������ά����������(��ʾ��������������)
	* @return �߶ε���ǰ���ζ�������ƽ��
	*/
	virtual double D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3);

	virtual int RayCrossGeometry(RayImpl& ray, double& dLength) const;

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

	/**  ��ȡ����ģ��
	* @param  ��
	* @return ����ģ��
	*/
	virtual SharedModelSharedPtr  getSharedModel(){
		return  m_pLinkModel;
	}

	/**  ���ù���ģ��
	* @param  [in] spSharedModel  ����ģ��Ĺ���ָ��
	* @return ��
	*/
	virtual void  setSharedModel(SharedModelSharedPtr  p) {
		m_pLinkModel = p;
	}
	/**  ��ȡ����ģ��ID
	* @param  ��
	* @return ����ģ��ID
	*/
	virtual MDLID  getSharedModelID() {
		return m_nLinkID;
	}

	/**  ��ȡλ������
	* @param  ��
	* @return λ������
	*/
	virtual Vertex3d  getVertex(){
		Vertex3d v;
		v.x = m_vPoint.x;
		v.y = m_vPoint.y;
		v.z = m_vPoint.z;
		return v;
	}

	/** ������ű���
	* @param
	* @return ���ű���
	*/
	virtual double getScale() const
	{
		return m_dScale;
	}

	/** �������ű���
	* @param [in] dScale ���ű���
	* @return
	*/
	virtual void setScale(double dScale)
	{
		m_dScale = dScale;
	}

	/** ��ù�����������
	* @param
	* @return ������������
	*/
	virtual std::string getShareModelName() const
	{
		return m_strLinkName;
	}

	/** ���ù�����������
	* @param [in] strLinkName ������������
	* @return
	*/
	virtual void setShareModelName(std::string strLinkName)
	{
		m_strLinkName = strLinkName;
	}

	/**  ��ѯ����ӿ�
	* @param  [in] signal  �������ͱ�ʶ
	* @param  [out] p ��Ӧ�Ķ���ָ��
	* @return 1 -- ��ѯ�ɹ���0 -- ��ѯʧ��
	*/
	virtual bool queryInterface(int signal, void ** p){
		if (signal == GEOMTYPE_3D_REFERENCE){
			*p = (void*)(Reference3d *) this;
			return true;
		}
		else if (GEOMTYPE_3D_POINT == signal){
			*p = (void*)(Point3d *) this;
			return true;
		}
		else
			return false;
	}
};

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
