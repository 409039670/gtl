#pragma once
#include "feature.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace 

class FeatureClassImpl :public std::enable_shared_from_this <FeatureClassImpl>, FeatureClass {

	/** @name ������Ա����˵��
	*  @{
	*/
protected:
	/** Ҫ�����ͱ�� */
	FCLSID   _id;
	/** Ҫ���������� */
	std::string   _name;
	/** Ҫ�����ͱ��� */
	std::string  _code;
	/** �ռ�ο�ϵͳID */
	SREFID  _srefid;
	/**Ҫ�ؼ������� */
	//int     _geom; 
	/** ���α�ķ�������*/
	int   _partitionnum;
	/** ��Ӧ�������ֶμ���*/
	std::vector<PropType> _proptypes;
	/** Ҫ�������ڵ�Ҫ�ؼ���*/
	FeatureSetSharedPtr	_localfeatureset;
	/** Ҫ���������ڵĹ���*/
	PRJID   _prjID;
	/** ����Ҫ�����״̬ */
	int	_fclstype;
	/** Ҫ�����Ͷ�ӦҪ�ؼ��Ϲ������� 0:��Ҫ�����ͽ�Ҫ�ؼ��ϱ�1�����������ͽ�Ҫ�ؼ��ϱ�*/
	int _fsettype;
	/** Ҫ�����Ͷ�Ӧ�������ݱ�������� 0��Ҫ�ؼ��ϵ����Ա���������1��Ҫ�ؼ��ϵ����Ա��Ҫ�ؼ��ϱ���һ��*/
	int _fsetproptype;
	/** @} */

	/** @name ���ó�Ա����˵��
	*  @{
	*/
public:
	/** ��Ԫ�� */
	friend class FeatureUtils;
	/** @} */


public:
	/** @name ��������������
	*  @{
	*/
	FeatureClassImpl();
	FeatureClassImpl(std::string name, std::string code, int  geomtype = 0, SREFID srefid = 0, std::string gTab = "", std::string aTab = "", int partitionnum = 0);
	FeatureClassImpl(FCLSID id, std::string name, std::string code, int geomtype = 0, SREFID srefid = 0, std::string gTab = "", std::string aTab = "", int partitionnum = 0);
	FeatureClassImpl(const FeatureClassImpl & fc);
	/** @} */

	/** ��ȡҪ�����ͱ��
	* @param  ��
	* @return Ҫ�����ͱ��
	*/
	virtual FCLSID getID();
	/** ����Ҫ�����ͱ��
	* @param  [in] id Ҫ�����ͱ��
	* @return  ��
	*/
	virtual void  setID(FCLSID id);
	/** ��ȡҪ����������
	* @param   ��
	* @return  Ҫ����������
	*/
	virtual std::string getName();
	/** ����Ҫ����������
	* @param  [in] v Ҫ����������
	* @return ��
	*/
	virtual void  setName(std::string v);
	/** ����Ҫ�����ͱ���
	* @param   ��
	* @return  Ҫ�����ͱ���
	*/
	virtual std::string getCode();
	/** ��ȡҪ�����ͱ���
	* @param  [in] v Ҫ�����ͱ���
	* @return ��
	*/
	virtual void  setCode(std::string v);
	/** ���ÿռ�ο�ϵͳID
	* @param  [in] srefID �ռ�ο�ϵͳID
	* @return ��
	*/
	inline void  setSpatialReferenceID(SREFID srefID);
	/** ��ȡ�ռ�ο�ϵͳID
	* @param  ��
	* @return �ռ�ο�ϵͳID
	*/
	inline SREFID  getSpatialReferenceID();
	/** ���ü��α�ķ�������
	* @param  [in] v ��������
	* @return  ��
	*/
	inline void  setPartitionNumber(int v);
	/** ��ȡ���α�ķ�������
	* @param   ��
	* @return ���α�ķ�������
	*/
	inline int  getPartitionNumber();
	/** ��ȡҪ���������ڵĹ���
	* @param   ��
	* @return  ����ID
	*/
	inline PRJID getPrjID();
	/** ����Ҫ���������ڵĹ���
	* @param  [in] prjID ����ID
	* @return  ��
	*/
	inline void  setPrjID(PRJID prjID);
	/** ��ȡҪ�����״̬
	* @param  ��
	* @return Ҫ�����״̬
	*/
	inline int getFclsType();
	/** ����Ҫ�����״̬
	* @param  [in] fclstype Ҫ�����״̬
	* @return ��
	*/
	inline void  setFclsType(int fclstype);
	/** Ҫ���࿽������
	* @param  [in] fc ָ��Ҫ���������
	* @return  ��
	*/
	void copy(const FeatureClassImpl & fc);
	/** Ҫ���ำֵ����
	* @param  [in] ָ��Ҫ���������
	* @return Ҫ����
	*/
	FeatureClassImpl operator=(const FeatureClassImpl & fc);
	/** �ж�Ҫ�����Ƿ����
	* @param  [in] ָ��Ҫ���������
	* @return  ��� ����true ����� ����false
	*/
	bool operator==(const FeatureClassImpl & fc);
	/** @} */


	/** @name Ҫ�������Խṹ����������
	*  @{
	*/
	/** ������Խṹ
	* @param  [in] pt���Խṹ������
	* @return ��
	*/
	void addPropType(const PropType & pt);
	/** ������Խṹ
	* @param  [in] name ��������
	* @param  [in] code ���Ա���
	* @param  [in] type ��������
	* @param  [in] length �����ֶεĳ���
	* @param  [in] decimal �Ƿ�ΪС��
	* @param  [in] nullabe �Ƿ�Ϊ��
	* @return ��
	*/
	void addPropType(std::string name, std::string code, std::string type, int length, int decimal = 0, bool nullable = false);
	/** ɾ�����Խṹ
	* @param  [in] name ���Խṹ������
	* @return  ��
	*/
	virtual void removePropType(std::string name);
	/**��ȡ���Խṹ�ļ���
	* @param   ��
	* @return  ��Ӧ�������ֶμ���
	*/
	inline std::vector<PropType> & getPropTypes();
	/** @} */


	/** @name Ҫ�������Ҫ�ؼ�������������Ҫ�ؼ��Ѳ��ã�
	*  @{
	*/
	/**����Ҫ�������ڵ�Ҫ�ؼ���
	* @param  [in] pfs Ҫ�ؼ���ָ��
	* @return  ��
	*/
	inline void setLocalFeatureSet(FeatureSetSharedPtr pfs){ _localfeatureset = pfs; }
	/**��ȡҪ�������ڵ�Ҫ�ؼ���
	* @param   ��
	* @return Ҫ�������ڵ�Ҫ�ؼ���
	*/
	inline FeatureSetSharedPtr getLocalFeatureSet(){ return _localfeatureset; }
	/**��ȡҪ�����͵�Ҫ�ؼ��Ϲ�������
	* @param ��
	* @return  Ҫ�ؼ��Ϲ�������
	*/
	inline int getFsetType();
	/**����Ҫ�����͵�Ҫ�ؼ��Ϲ�������
	* @param [in] v  Ҫ�����͵�Ҫ�ؼ��Ϲ�������
	* @return  ��
	*/
	inline void setFsetType(int v);
	/**��ȡҪ�����͵������ֶν�������
	* @param   ��
	* @return Ҫ�����͵������ֶν�������
	*/
	inline int getFsetPropType();
	/**����Ҫ�����������ֶν�������
	* @param [in] v	�����ֶν�������
	* @return ��
	*/
	inline void setFsetPropType(int v);
	/** @} */


	/** @name �ļ���д������
	*  @{
	*/
	/** д�ļ�
	* @param [in] f�����
	* @return ��
	*/
	virtual void write(std::ostream & f);
	/** ���ļ�
	* @param [in] f������
	* @return ��
	*/
	virtual void read(std::istream & f);
	/** @} */


	/** @name Ҫ����Buffer����������
	*  @{
	*/
	/** ��ȡҪ�����Buffer��С
	* @param  [in] ��
	* @return Buffer��С
	*/
	size_t sizeBuffer();
	/**��Ҫ������Ϣд��Buffer
	* @param  [in] buf
	* @return  ��
	*/
	void writeBuffer(Buffer &buf);
	/**��Buffer��ȡҪ������Ϣ
	* @param  [in,out] buf
	* @return  ��
	*/
	void readBuffer(Buffer &buf);
	/** @} */


public:
	virtual int getStatus() {
		return 0;
	}
	virtual void  setStatus(int s) {

	}

	virtual void copy(const FeatureClass & fc){
		copy((const FeatureClassImpl &)fc);
	}
	virtual bool isEqual(const FeatureClass & fc) {
		return *this == (const FeatureClassImpl &)fc;
	}

	virtual FeatureClassSharedPtr clone() {
		FeatureClassImpl * p = new FeatureClassImpl(*this);
		return FeatureClassSharedPtr((FeatureClass*)p);
	}
};
/** @} */ // End of the group block
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
