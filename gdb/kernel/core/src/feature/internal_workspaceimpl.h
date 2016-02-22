#include "feature.h"



begin_gtl_namespace
begin_gdb_namespace 

/** @defgroup  ������������
*  @{
*/
class WorkspaceImpl : public std::enable_shared_from_this<WorkspaceImpl>, public Workspace{
	/** @name ������Ա����˵��
	*  @{
	*/
protected:
	/**������/����ID���ڹ��̷�Χ����Ψһ��*/
	WKSPID   _regid;
	/**����������*/
	std::string _name;
	/**����������*/
	std::string _code;
	/**��Χ�б߽����,����˳��Ϊ��X����С���Y����С���Z����С���*/
	double _bound[6];
	/**���������*/
	Vertex3dVector  _polybound;
	/**�ռ�ο�����ϵͳID*/
	SREFID  _srefid;
	/**�����ļ���*/
	int     _level;
	/**ͼ��ļ���*/
	std::vector< LayerSharedPtr > _set;
	/**��ǰͼ��������Ĭ�ϲ����ڵ�ǰ��*/
	int  _cur_index;//
	/**���������ڵĹ���*/
	ProjectSharedPtr  _localproject;
	/** @} */
public:
	/** @name �������û�ò�����
	*  @{
	*/
	/**��ȡ������ID
	* @param  ��
	* @return   ������ID
	*/
	inline WKSPID getID(){ return _regid; }
	/**���ù�����ID
	* @param [in] id ������ID
	* @return ��
	*/
	inline void  setID(WKSPID id){ _regid = id; }

	/**��ȡ����������
	* @param ��
	* @return  ����������
	*/
	inline std::string getName(){ return _name; }
	/**���ù���������
	* @param [in] v ����������
	* @return
	*/
	inline void  setName(std::string v){ _name = v; }
	/**��ȡ����������
	* @param ��
	* @return  ����������
	*/
	inline std::string getCode(){ return _code; }
	/**���ù���������
	* @param [in] v ����������
	* @return
	*/
	inline void  setCode(std::string v){ _code = v; }

	/**��ȡ��Χ�б߽����
	* @param [out] vl ��Χ�б߽����
	* @return  ��
	*/
	inline void getBound(Envelope3d & vl) {
		vl.minx = _bound[0]; vl.maxx = _bound[1];
		vl.miny = _bound[2]; vl.maxy = _bound[3];
		vl.minz = _bound[4]; vl.maxz = _bound[5];
	}
	/**���ð�Χ�б߽����
	* @param [in] vl ��Χ�б߽����
	* @return  ��
	*/
	inline void setBound(Envelope3d & vl) {
		_bound[0] = vl.minx; _bound[1] = vl.maxx;
		_bound[2] = vl.miny; _bound[3] = vl.maxy;
		_bound[4] = vl.minz; _bound[5] = vl.maxz;
	}
	/**��ȡ���������
	* @param ��
	* @return  ���������
	*/
	inline Vertex3dVector & getPolyBound(){ return _polybound; }
	/**���ö��������
	* @param [in] plybound ���������
	* @return  ��
	*/
	inline void setPolyBound(Vertex3dVector & plybound) {
		_polybound.resize(plybound.size());
		std::copy(plybound.begin(), plybound.end(), _polybound.begin());
	}
	/**��ȡ�ռ�ο�����ϵͳID
	* @param ��
	* @return  �ռ�ο�����ϵͳID
	*/
	inline SREFID getSRefID(){ return _srefid; }
	/**���ÿռ�ο�����ϵͳID
	* @param [in] s �ռ�ο�����ϵͳID
	* @return  ��	*/
	inline void setSRefID(SREFID s){ _srefid = s; }
	/**��ȡ�����ļ���
	* @param ��
	* @return  �����ļ���
	*/
	inline int getLevel(){ return _level; }
	/**���÷����ļ���
	* @param [in] l �����ļ���
	* @return  ��	*/
	inline void setLevel(int l){ _level = l; }
	/**��ȡ��ǰͼ������
	* @param  ��
	* @return ��ǰͼ������
	*/
	inline LayerSharedPtr  getCurLayer(){
		LayerSharedPtr p;
		if (_cur_index<0)
			return p;
		return  _set[_cur_index];
	}
	/**���õ�ǰͼ��������δʵ��
	* @param [in] layername ��ǰͼ������
	* @return  ��
	*/
	void setCurLayer(std::string layername);
	/**���õ�ǰͼ������
	* @param [in] s ��ǰͼ������
	* @return  ��
	*/
	void setCurLayer(size_t s);
	/**���ù��������ڵĹ���
	* @param [in] pProj ���������ڵĹ���ָ��
	* @return  ��
	*/
	inline void setLocalProject(ProjectSharedPtr  pProj){ _localproject = pProj; }//����
	/**��ȡ���������ڵĹ���
	* @param ��
	* @return  ���������ڵĹ���ָ��
	*/
	inline ProjectSharedPtr getLocalProject(){ return _localproject; }//��ȡ���������ڵĹ���
	/**��ȡ��Χ�б߽����
	* @param [out] pv ��Χ�б߽����
	* @return  ��
	*/
	inline void getBound(double * pv) {
		memcpy((void*)pv, _bound, sizeof(double) * 6);
	}
	/**���ð�Χ�б߽����
	* @param [in] d[6] ��Χ�б߽����
	* @return  ��
	*/
	inline void setBound(double d[6]){
		memcpy((void*)_bound, d, sizeof(double) * 6);
	}


	/** @} */

public:

	/** @name ��������������
	*  @{
	*/
	WorkspaceImpl();
	~WorkspaceImpl();
	void clear();
	/** @} */
	/** @name ���������븳ֵ���캯��
	*  @{
	*/
	void copy(const WorkspaceImpl &);
	WorkspaceImpl(const WorkspaceImpl &);
	WorkspaceImpl operator=(const WorkspaceImpl &);
	WorkspaceImpl(WKSPID id, std::string name, std::string code, double * bound, int level = 0, SREFID sid = 0);

	/** @} */


	/** ���������
	* @param
	*/
	LayerSharedPtr operator() (size_t s);
	/** ���������
	* @param
	*/
	const LayerRef operator() (size_t s) const;
	/** ���������
	* @param
	*/
	bool operator==(const WorkspaceImpl &);


	/** @name ��������������
	*  @{
	*/

	/** ���һ��ͼ��
	* @param [in] p ͼ��ָ��
	* @return ��
	*/
	void addLayer(std::shared_ptr<Layer> p);

	/** ����Ҫ������ID����ȡָ����Ҫ�ؼ���;
	* @param [in] fclsid Ҫ����ID
	* @return Ҫ�ؼ�ָ��
	*/
	FeatureSetSharedPtr findFeatureSet(FCLSID fclsid);

	/** ��ȡͼ�㣬δʵ��
	* @param [in] name ͼ������
	* @return ͼ��ָ��
	*/
	inline std::shared_ptr<Layer> getLayer(std::string name){
		std::shared_ptr<Layer> p;
		return p;
	}
	/** ��ȡͼ��ļ���
	* @param ��
	* @return ͼ��ļ���
	*/
	inline std::vector< std::shared_ptr<Layer> > & getLayers(){ return _set; }

	/** ����ͼ�㣬δʵ��
	* @param [in] name ͼ������
	* @return ͼ��ID
	*/
	size_t indexLayer(std::string layername);
	/** ��ȡͼ��ļ��ϴ�С
	* @param ��
	* @return ͼ��ļ��ϴ�С
	*/
	inline size_t  size() const { return _set.size(); }


	/** ���������������ж����IDͳһ����Ϊ��ǰ��������ID
	* @param ��
	* @return ��
	*/
	void unifyID();


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
};
/** @} */ // End of the group block

 
end_gdb_namespace
end_gtl_namespace


