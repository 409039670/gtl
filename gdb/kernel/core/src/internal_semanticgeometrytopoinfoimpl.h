#pragma once
#include "topology.h"
#include "internal_topoinfoimpl.h"


begin_gtl_namespace
begin_gdb_namespace

class   SematicGeometryTopoInfoImpl : virtual public TopoInfoImpl, virtual public SemanticGeometryTopoInfo
{
	/** @defgroup SematicGeometryTopoInfoImpl  SematicGeometryTopoInfoImpl-���弸����Ϣ������
	*  @{
	*/
protected:
	/** @name ���ԣ���Ա������
	* @{
	*/

	/** ������Ϣ */
	string	m_sematicName;

	/** ����������Ϣ���ͣ�ÿ�����Ͷ�Ӧһ����� */
	GeometrySharedPtr m_geometry;

	/** @} */ // ���Խ�β

public:

	/** @defgroup SematicGeometryTopoInfoImpl ��������Ա������
	* @{
	*/
	explicit SematicGeometryTopoInfoImpl(FIDITEM fiditem) :
		TopoInfoImpl(fiditem, TOPOINFOTYPE_SEMATIC_GEOMETRY),
		m_sematicName(""){}


	virtual ~SematicGeometryTopoInfoImpl(void);

	/** ��ö����������Ϣ
	* @param
	* @return �����������Ϣ
	*/
	inline string getSemanticName() const { return m_sematicName; }

	/** ���ö����������Ϣ
	* @param name �����������Ϣ
	* @return
	*/
	inline void setSemanticName(const string name)  { m_sematicName = name; }

	/** ���ö�������˼���
	* @param [in]  geo ��������˼���
	* @return void
	*/
	inline void setGeometry(GeometrySharedPtr  geo) { m_geometry = geo; }

	/** ��ö�������˼���
	* @param
	* @return ��������˼���
	*/
	inline GeometrySharedPtr getGeometry()  { return m_geometry; }

	/** ���㵱ǰ���������ڴ�Ĵ�С
	* @param
	* @return ������Ĳ���m_nSize
	*/
	virtual size_t sizeBuffer();

	/** �ӻ����ȡ�����¼
	* @param [in] pBuffer ����
	* @return ��ȡ�ɹ����
	*/
	virtual bool readBuffer(Buffer & buf);

	/** ������д�뻺��
	* @param [in] pBuffer ����
	* @return д��ɹ����
	*/
	virtual bool writeBuffer(Buffer& buf);

	/** ��ָ�����󿽱�����ǰ����
	* @return ��������ָ��
	*/
	virtual bool copy(TopoInfoImpl * topoInfo);

	/** ������������Ϣ����д���ļ�
	* @param [in] f ����
	* @return д��ɹ����
	*/
	virtual void write(std::ostream & f);

	/** ���ļ��ж�ȡ����������Ϣ�����¼
	* @param [in] f ����
	* @return ��ȡ�ɹ����
	*/
	virtual void read(std::istream & f);
};
/////////////////////////////////////////////////////////////////////

end_gdb_namespace
end_gtl_namespace


