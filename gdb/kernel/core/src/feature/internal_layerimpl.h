#include "feature.h"



begin_gtl_namespace
begin_gdb_namespace
class LayerImpl : public std::enable_shared_from_this<LayerImpl>, public gdb::Layer//�涨��һ��Ҫ������ֻ������һ��ͼ��
{

	/** @name ������Ա����˵��
	*  @{
	*/

protected:
	/** @name ���ԣ���Ա������
	* @{
	*/

	/** ͼ��id */
	LYRID _layid;

	/** ͼ������ */
	std::string _name;

	/** ͼ�������Ҫ������ID���� */
	std::vector<FCLSID>	m_fclsids;

	/** ͼ���Ӧ��Ҫ�ؼ��� */
	std::vector<FeatureSetSharedPtr> _set;

	/** ͼ��ĵ�ǰҪ�ؼ������� */
	int    _cur_index;

	/** ͼ�����ڵĹ��� */
	WorkspaceSharedPtr _localworkspace;//ͼ�����ڵĹ���

	/** ͼ�����ڵĹ��� */
	PRJID  _prjID;
	/** @} */
public:
	/** @name ��������������
	*  @{
	*/
	LayerImpl();
	~LayerImpl();
	LayerImpl(/*std::string name,*/LYRID layid);
	/** @} */
	/** @name ���������븳ֵ���캯��
	*  @{
	*/
	void copy(const LayerRef);
	LayerImpl(const LayerImpl &);
	LayerImpl operator=(const LayerImpl &);
	/** @} */

	/** @name �������û�ò�����
	*  @{
	*/

	/**��ȡͼ��ID
	* @param  ��
	* @return   ͼ��ID
	*/
	inline LYRID getID(){ return _layid; }
	/**����ͼ��ID
	* @param [in] layid ͼ��ID
	* @return ��
	*/
	inline void  setID(LYRID layid){ _layid = layid; }

	/**��ȡͼ������
	* @param ��
	* @return  ͼ������
	*/
	inline std::string getName(){ return _name; }
	/**����ͼ������
	* @param [in] s ͼ������
	* @return
	*/
	inline void setName(std::string s){ _name = s; }


	/**��ȡͼ���������ڹ���
	* @param ��
	* @return  prjID ����ID
	*/
	inline PRJID getProjectID(){ return _prjID; }
	/**����ͼ���������ڵĹ���
	* @param [in] prjID ����ID
	* @return
	*/
	inline void  setProjectID(PRJID prjID){ _prjID = prjID; }

	/**��ȡͼ���Ӧ��Ҫ������ID����
	* @param ��
	* @return  ͼ���Ӧ��Ҫ������ID����
	*/
	inline vector<FCLSID>& getFeatureClassIDs() { return m_fclsids; }
	/**����ͼ���Ӧ��Ҫ������ID����
	* @param [in] fclsids  Ҫ������ID����
	* @return  ��
	*/
	inline void setFeatureClassIDs(vector<FCLSID>& fclsids) { m_fclsids = fclsids; }

	/** ��ȡͼ���Ӧ��Ҫ�ؼ���
	* @param ��
	* @return  ͼ���Ӧ��Ҫ�ؼ���
	*/
	inline std::vector<FeatureSetSharedPtr> & getFeatureSetVector(){ return _set; }
	/** ��ȡͼ���Ӧ�ĵ�ǰҪ��
	* @param ��
	* @return  ͼ���Ӧ�ĵ�ǰҪ��
	*/
	inline FeatureSetSharedPtr getCurrentFeatureSet(){
		FeatureSetSharedPtr p;
		if (_cur_index<0) return p;
		return _set[_cur_index];
	}
	/** ����ͼ���Ӧ�ĵ�ǰҪ��
	* @param [in] s ��ǰҪ�ؼ�������
	* @return  ��
	*/
	inline void setCurrentFeatureSet(size_t s){ _cur_index = (int)s; }
	/** @} */

	/** @name ��������������
	*  @{
	*/
	/** ��ͼ���Ҫ������ID���������ָ��Ҫ������ID
	* @param [in] fclsid Ҫ������ID
	* @return  ��
	*/
	inline void addFeatureClassID(FCLSID fclsid) { m_fclsids.push_back(fclsid); }

	/** ��ͼ���Ҫ������ID������ɾ��ָ��Ҫ������ID
	* @param [in] fclsid Ҫ������ID
	*/
	void eraseFeatureClassID(FCLSID fclsid);


	/** ��ȡָ��Ҫ��ID��Ҫ������Ҫ�ؼ�������
	* @param [in] id Ҫ��ID
	* @return  ʧ�� -1,�ɹ�����Ҫ�ؼ�������
	*/
	size_t indexFeatureSet(FSID id);

	/** ����һ��ָ��Ҫ�����Ҫ�ؼ�
	* @param [in] fc ָ��Ҫ������
	* @return Ҫ�ؼ�ָ��
	*/
	FeatureSetSharedPtr createFeatureSet(const FeatureClassRef fc);
	/** ��ͼ��������һ��Ҫ�ؼ�������
	* @param [in] p Ҫ������ָ��
	* return Ҫ������ָ��
	*/
	FeatureSetSharedPtr addFeatureSet(FeatureSetSharedPtr fs);
	/** ��ͼ���Ҫ�ؼ�����ɾ��ָ��Ҫ�ؼ���ID��Ҫ�ؼ�������
	* @param [in] fid Ҫ��ID
	* return ��
	*/
	void eraseFeatureSet(FSID fsid);
	/** ���������
	* @param
	*/
	FeatureSetSharedPtr  operator() (size_t s);
	/** ���������
	* @param
	*/
	bool operator ==(const LayerImpl& lay);
	/** ���������
	* @param
	*/
	const FeatureSet&   operator() (size_t s) const;
	/** ��ѯͼ��Ҫ�ؼ��ϴ�С
	* @param ��
	* return ͼ��Ҫ�ؼ��ϴ�С
	*/
	inline size_t  size() const { return _set.size(); }

	/** ����ͼ�����ڵĹ�����ID��Ҫ�ؼ�������
	* @param [in] pws ��ǰ������ָ��
	*/
	inline void setLocalWorkspace(WorkspaceSharedPtr pws){ _localworkspace = pws; }
	/** ��ȡͼ�����ڵĹ�����
	* @param [in]
	* @return ��ȡͼ�����ڵĹ�����
	*/
	inline WorkspaceSharedPtr getLocalWorkspace(){ return _localworkspace; }

	/** ���ͼ���Ӧ��Ҫ�ؼ���
	* @param ��
	* @return ��
	*/
	void clear();

	virtual FeatureSetSharedPtr getFeatureSet(int i)
	{
		return _set[i];
	}
	/** @} */


	/** @name �ļ�IO����������
	*  @{
	*/
public:
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
	/** �����ļ�
	* @param [in] sz�ļ�·��
	* @return ��
	*/
	virtual void load(std::string & sz);
	/** �����ļ�
	* @param [in] sz�ļ�·��
	* @return ��
	*/
	virtual void save(std::string & sz);
	/** @} */


	virtual bool isEqual(const LayerRef  lr) {
		return (*this) == (LayerImpl&)lr;
	}
	virtual bool isEqual(const LayerSharedPtr l) {
		return (*this) == (LayerImpl&)(*l.get());
	}
};
/** @} */ // End of the group block

end_gdb_namespace
end_gtl_namespace


