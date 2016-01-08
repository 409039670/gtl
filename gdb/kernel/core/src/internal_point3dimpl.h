#pragma once
#include "internal_geometry3dimpl.h"
#include "shadedvertex3d.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
class Point3dImpl :virtual public Point3d, virtual public Geometry3dImpl
{
	/** @defgroup Point3dImpl Point3dImpl-��ά��
	*  @{
	*/
protected:
	/** @name ���ԣ���Ա������
	* @{
	*/

	/** @brief ��ά�� */
	ShadedVertex3d m_vPoint;

	/** @} */ // ���Խ�β
public:

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
		else
			return false;
	}

	/**  ��ȡ��λ������
	* @param  ��
	* @return λ������
	*/
	virtual Vertex3d getPosition(){
		return Vertex3d(m_vPoint.x, m_vPoint.y, m_vPoint.z);
	}

	/**  ���õ�λ������
	* @param  [in] vPosition  ��λ������
	* @return ��
	*/
	virtual void setPosition(const Vertex3d & vPosition){
		m_vPoint.x = vPosition.x;
		m_vPoint.y = vPosition.y;
		m_vPoint.z = vPosition.z;
	}
public:

	/** @defgroup Point3dImpl ��������Ա������
	* @{
	*/

	/** @name ��������������
	*  @{
	*/

	/** ���캯��
	*/
	Point3dImpl();
	/** ��������
	*/
	virtual ~Point3dImpl();

	/** @} */ // ����������������β



	/** @name ���ݲ���
	*  @{
	*/

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

	/** �麯��
	* ͳ�Ƶ�ǰ��������ļ�������
	* @param
	* @return long*:�����ڲ����ٿռ䡢�ⲿ�ͷ�
	*               ����ֵ������ -- ���������������Ч�������������������Ч��ɫ����
	*								�����������������ת��Ϊ�ȼ�����
	*								����������������
	*/
	virtual long* GeoStatistics()
	{
		return NULL;
	}

	/** �麯��
	* �жϵ�ǰ����͸����������2D�ϵĹ�ϵ�����룬�ཻ������
	* �ж�
	* @param [in] ptArray		�����
	* @param [in] precisionFlag ���ȱ�־λ 0-������� 1-͹�� 2-����������
	* @return 1-�ཻ���ٽӣ������� 0-���� -1-����
	*/
	virtual int RGeoWithPolygon2D(vector<Vertex3d>& ptArray, int precisionFlag = 0)
	{
		return -1;
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
