#pragma once
#include "internal_topoinfoimpl.h"


begin_gtl_namespace
begin_gdb_namespace

class SemanticActionTopoInfoImpl :virtual public TopoInfoImpl, virtual public SemanticActionTopoInfo
{
public:
	explicit SemanticActionTopoInfoImpl(FIDITEM fiditem) :
		TopoInfoImpl(fiditem, TOPOINFOTYPE_SEMATIC_ACTION)
	{

	}
	 
	~SemanticActionTopoInfoImpl();
	virtual std::string getName();

	/** ���㵱ǰ���������ڴ�Ĵ�С
	* @param
	* @return ������Ĳ���m_nSize
	*/
	virtual size_t sizeBuffer() { return 0; }

	/** �ӻ����ȡ�����¼
	* @param [in] pBuffer ����
	* @return ��ȡ�ɹ����
	*/
	virtual bool readBuffer(Buffer & buf) { return false; }

	/** ������д�뻺��
	* @param [in] pBuffer ����
	* @return д��ɹ����
	*/
	virtual bool writeBuffer(Buffer& buf) { return false; }

	/** ��ָ�����󿽱�����ǰ����
	* @return ��������ָ��
	*/
	virtual bool copy(TopoInfoImpl * topoInfo){ return false; }

	/** ������������Ϣ����д���ļ�
	* @param [in] f ����
	* @return д��ɹ����
	*/
	virtual void write(std::ostream & f) {}

	/** ���ļ��ж�ȡ����������Ϣ�����¼
	* @param [in] f ����
	* @return ��ȡ�ɹ����
	*/
	virtual void read(std::istream & f) {}
};

end_gdb_namespace
end_gtl_namespace



