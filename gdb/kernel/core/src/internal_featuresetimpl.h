#pragma once
#include "feature.h"



begin_gtl_namespace
begin_gdb_namespace

/** @defgroup FeatureSetImpl Ҫ�ؼ��Ͻӿ�
*  @{
*/
class  FeatureSetImpl :public std::enable_shared_from_this<FeatureSetImpl>, public FeatureSet
{
protected:
	/**Ҫ�ؼ���ID*/
	FSID  _fsid;

	/**Ҫ�ؼ�������*/
	std::string  _name;
	/**Ҫ�ض����Ӧ��Ҫ����*/
	FeatureClassSharedPtr _fclass;
	/**Ҫ�ؼ���*/
	FeatureSharedPtrVector _fset;//Ҫ��
	/**Ҫ�ؼ������ڵ�ͼ��*/
	LYRID       _localLayer;



	/** @name ��������������
	*  @{
	*/
	FeatureSetImpl(const FeatureClass & fcls);
	FeatureSetImpl(FeatureClassSharedPtr fcls);
	/** @} */
public:

	/** @name ��������������
	*  @{
	*/
	FeatureSetImpl();
	void clear();
	void copy(const FeatureSetSharedPtr fSet, bool bContent = false);
	void copy(const FeatureSetRef fSet, bool bContent);
	~FeatureSetImpl();
	/** @} */

	/** @name ���������
	*  @{
	*/
	Feature & operator() (size_t s);
	const Feature & operator()  (size_t s) const;
	inline size_t  size() const { return _fset.size(); }
	/** @} */

public:
	/** @name ö�ٳ�Ա����˵��
	*  @{
	*/
	/**
	* SINGLE_FILE_DB = 0,�������̴��Ϊһ��GDB�ļ������ڵ��ļ�ϵͳ�ĸ�ʽ��Ҫ���о���Ҫ�ض���Ķ�д
	* MULTI_FILE_DB = 1,�������̴��Ϊ����ļ��������ļ�ϵͳ�����ݿ��ʽ�������о���Ҫ�ض���Ķ�д
	* ORA_FILE_DB = 2,���ڹ�ϵ���ݿ�Ĵ洢,Ҫ���о���Ҫ�ض���Ķ�д
	*/
	enum DatasetType{
		SINGLE_FILE_DB = 0,
		MULTI_FILE_DB,
		ORA_FILE_DB
	};
	/** @} */

	/** @name ��̬��Ա����˵��
	*  @{
	*/
	/** ��ȡ���ݼ�����
	* @param  ��
	* @return ���ݼ�����
	*/
	static DatasetType getDatasetType();

	/** �������ݼ�����
	* @param  [in]  dst ���ݼ�����
	* @return ��
	*/
	static void setDatasetType(DatasetType dst);
	/** @} */


public:
	/** @name �������û�ò�����
	*  @{
	*/

	/**��ȡҪ�ؼ���ID
	* @param  ��
	* @return   Ҫ�ؼ���ID
	*/
	inline FSID getID(){ return _fsid; }
	/**����Ҫ�ؼ���ID
	* @param [in] id Ҫ�ؼ���ID
	* @return  ��
	*/
	inline void setID(FSID id){ _fsid = id; }

	/** ��ȡҪ�ؼ�������
	* @param  ��
	* @return  Ҫ�ؼ�������
	*/
	inline std::string getName() { return _name; }
	/** ����Ҫ�ؼ��ϵ�����
	* @param  [in] szName Ҫ�ؼ�������
	* @return ��
	*/
	inline void setName(std::string szName) { _name = szName; }


	/** ����Ҫ�ؼ��϶�Ӧ��Ҫ����
	* @param  [in] f Ҫ�ؼ��϶�Ӧ��Ҫ����
	* @return ��
	*/
	inline void setFeatureClass(FeatureClassSharedPtr  f);

	/** ����Ҫ�ؼ��϶�Ӧ��Ҫ����
	* @param  [in] f Ҫ�ؼ��϶�Ӧ��Ҫ����
	* @return ��
	*/
	inline void attachFeatureClass(FeatureClass* f);
	/** ��ȡҪ�ؼ��϶�Ӧ��Ҫ����
	* @param  ��
	* @return Ҫ�ؼ��϶�Ӧ��Ҫ����
	*/
	inline FeatureClassSharedPtr   getFeatureClass();
	/** @} */

	/** @name ��������������
	*  @{
	*/
	/**��Ҫ�ؼ��������ָ��Ҫ��ID��Ҫ�ض���
	* @param [in] p Ҫ�ض���
	* @return  ��
	*/
	void addFeature(FeatureSharedPtr p);
	///**��Ҫ�ؼ�����ɾ��ָ��Ҫ��ID��Ҫ������
	//* @param [in] fid  Ҫ��ID
	//* @return  ��
	//*/
	//inline void removeAFeature(FID fid);
	/**��Ҫ�ؼ�����ɾ��ָ��Ҫ��ID��Ҫ������
	* @param [in] fid  Ҫ��ID
	* @return  Ҫ�ض���ָ��
	*/
	FeatureSharedPtr removeFeature(FID fid);
	/** ��ȡҪ�ؼ����е�Ҫ�ؼ�
	* @param  ��
	* @return Ҫ�ؼ����е�Ҫ�ؼ�
	*/
	inline FeatureSharedPtrVector& getFeatures(){ return _fset; }
	/** ����Ҫ�ؼ������ڵ�ͼ��
	* @param  [in] lyr Ҫ�ؼ������ڵ�ͼ��
	* @return ��
	*/
	virtual void setFeatures(FeatureSharedPtrVector &feas){ _fset = feas; }

	inline void setLayerID(LYRID lyr){ _localLayer = lyr; }
	/** ��ȡҪ�ؼ������ڵ�ͼ��
	* @param  ��
	* @return Ҫ�ؼ������ڵ�ͼ��
	*/
	inline LYRID getLayerID(){ return _localLayer; }
	/**��Ҫ�ؼ�����ɾ��ָ��Ҫ��ID��Ҫ������
	* @param [in] fid  Ҫ��ID
	* @return
	*/
	void eraseFeature(FID fid);

	/** @} *



	/** @name �ļ�IO����������
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

	virtual FeatureSharedPtr createFeature(FID id, std::string name, int lod, REGID regid,
		int state = 0, GeometrySharedPtr pg = GeometrySharedPtr(), GDBTIME createTime = 0, GDBTIME updateTime = 0);
	virtual FeatureSharedPtr createFeature(std::string name, int lod, REGID regid,
		int state = 0, GeometrySharedPtr pg = GeometrySharedPtr(), GDBTIME createTime = 0, GDBTIME updateTime = 0);
	virtual FeatureSharedPtr createFeature(FeatureRef f);
public:
	friend class FeatureSet;

};
end_gdb_namespace
end_gtl_namespace


