#pragma once
#include "internal_topoitemimpl.h"
 


begin_gtl_namespace
begin_gdb_namespace
 

class  TopoInfoImpl : virtual public TopoInfo
{
	/** @defgroup TopoInfoImpl TopoInfoImpl-����������Ϣ�������
	*  @{
	*/
protected:
	/** @name ���ԣ���Ա������
	* @{
	*/

	//ԭʼ��Ҫ����Ϣ
	FIDITEM m_originalItem;

	/** ����������Ϣ���ͣ�ÿ�����Ͷ�Ӧһ����� */
	int m_type;

	/** @} */ // ���Խ�β

public:


	virtual Identifier::raw_type getFeatureClassID() const {
		return m_originalItem.second;
	}
	virtual Identifier::raw_type getFeatureID() const  {
		return m_originalItem.first;
	}

	virtual void setFeatureClassID(Identifier::raw_type d){
		m_originalItem.second = d;
	}
	virtual void setFeatureID(Identifier::raw_type d){
		m_originalItem.first = d;
	}

	virtual bool copy(const TopoInfo & topoInfo){
		TopoInfoImpl* p = dynamic_cast<TopoInfoImpl*>((TopoInfo*)(&topoInfo));
		return copy(p);
	}


	/** @defgroup TopoInfoImpl ��������Ա������
	* @{
	*/
	TopoInfoImpl(FIDITEM fiditem, int type = 0);
	virtual ~TopoInfoImpl(void);

	/** ��ö��������������Ϣ���ͱ��
	* @param
	* @return ���������������Ϣ���ͱ��
	*/
	inline int getType() const { return m_type; }

	/** ���ö��������������Ϣ���ͱ��
	* @param [in]  type ���������������Ϣ���ͱ��
	* @return void
	*/
	inline void setType(int type) { m_type = type; }

	/** ���ԭʼ��Ҫ����Ϣ
	* @param
	* @return ������������ͱ��
	*/
	inline FIDITEM getOriginalItem()  { return m_originalItem; }

	/** ����ԭʼ��Ҫ����Ϣ
	* @param [in]  type �������ͱ��
	* @return void
	*/
	inline void setOriginalItem(FIDITEM & item) { m_originalItem = item; }

	/** ���㵱ǰ���������ڴ�Ĵ�С
	* @param
	* @return ������Ĳ���m_nSize
	*/
	virtual size_t sizeBuffer() = 0;

	/** �ӻ����ȡ�����¼
	* @param [in] pBuffer ����
	* @return ��ȡ�ɹ����
	*/
	virtual bool readBuffer(Buffer & buf) = 0;

	/** ������д�뻺��
	* @param [in] pBuffer ����
	* @return д��ɹ����
	*/
	virtual bool writeBuffer(Buffer& buf) = 0;

	/** ��ָ�����󿽱�����ǰ����
	* @return ��������ָ��
	*/
	virtual bool copy(TopoInfoImpl * topoInfo);

	/** ������������Ϣ����д���ļ�
	* @param [in] f ����
	* @return д��ɹ����
	*/
	virtual void write(std::ostream & f) = 0;

	/** ���ļ��ж�ȡ����������Ϣ�����¼
	* @param [in] f ����
	* @return ��ȡ�ɹ����
	*/
	virtual void read(std::istream & f) = 0;

};
end_gdb_namespace
end_gtl_namespace


