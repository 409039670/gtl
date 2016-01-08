#include "feature.h"



begin_gtl_namespace
begin_gdb_namespace 

/** @defgroup ProjectImpl ���̹�����
*  @{
*/
class ProjectImpl : public std::enable_shared_from_this<ProjectImpl>, public Project{
protected:

	/**����ID*/
	PRJID                     _prjid;
	/**��������*/
	std::string               _name;
	/**���̱߽�*/
	double                    _bound[6];
	/**��������*/
	std::vector<WorkspaceSharedPtr> _regs;
	//RTree<FID,double,3,double>  _rtree;//����������Χ�ڵ�RTree������Ĭ��Ϊ����	
	//SpatialIndex * _indextree;
	/**��ǰ����������Ĭ�ϲ����ڵ�ǰ��*/
	size_t  _cur_index;
	/**���̵Ķ���α߽� */
	Vertex3dVector              _polybound;

public:
	/** @name �������Բ���������
	*  @{
	*/
	/** ��ȡ��ǰ������
	* @param  ��
	* @return ��ǰ������
	*/
	WorkspaceSharedPtr  getCurWorkspace();
	/** ��ȡ��ǰ����ID
	* @param  ��
	* @return ��ǰ����ID
	*/
	inline PRJID getID()const;
	/** ���õ�ǰ����ID
	* @param  [in] id ��ǰ����ID
	* @return ��
	*/
	inline void  setID(PRJID id);
	/** ��ȡ��ǰ��������
	* @param  ��
	* @return ��ǰ��������
	*/
	inline std::string getName();
	/** ���õ�ǰ��������
	* @param  [in] v ��ǰ��������
	* @return ��
	*/
	inline void  setName(std::string v);
	/** ��ȡ��ǰ���̰�Χ��
	* @param  [out] vl ��ǰ���̰�Χ��
	* @return ��
	*/
	inline void	getBound(Envelope3d & vl);
	virtual void getEnvelope(Envelope3dRef vl){
		getBound(vl);
	}
	/** ���õ�ǰ���̰�Χ��
	* @param  [in] vl ��ǰ���̰�Χ��
	* @return ��
	*/
	inline void setBound(Envelope3d & vl);
	virtual void setEnvelope(Envelope3dRef vl){
		setBound(vl);
	}
	/** ���õ�ǰ���̰�Χ��
	* @param  [in] d[6] ��ǰ���̰�Χ��
	* @return ��
	*/
	inline void setBound(double d[6]);
	virtual void setEnvelope(double * p){
		setBound(p);
	}
	/** ��ȡ��ǰ���̰�Χ��
	* @param  [out] pv ��ǰ���̰�Χ��ָ��
	* @return ��
	*/
	inline void getBound(double * pv);
	/** ��ȡ��ǰ�������й�����
	* @param  ��
	* @return ��ǰ�������й���������
	*/
	inline std::vector<WorkspaceSharedPtr> & getWorkspaces();
	/** ��ǰ������ӹ�����
	* @param [in] p ������ָ��
	* @return ��
	*/
	void addWorkspace(WorkspaceSharedPtr p);
	/** ��չ��̣�ʹ����������Ϊ��
	* @param ��
	* @return ��
	*/
	void clear();
	/** ��ȡ���̵Ķ���α߽�
	* @param ��
	* @return ���̵Ķ���α߽�
	*/
	inline Vertex3dVector & getPolyBound();

	/** ���ù��̵Ķ���α߽�
	* @param [in] plybound ���̶���α߽�
	* @return ��
	*/
	inline void setPolyBound(Vertex3dVector & plybound);
	/** @} */


public:
	/** @name ��������������
	*  @{
	*/
	ProjectImpl();
	ProjectImpl(PRJID id, std::string name, double * bound);
	void copy(const ProjectImpl &);
	ProjectImpl(const ProjectImpl &);
	ProjectImpl operator=(const ProjectImpl &impl);
	~ProjectImpl();
	/** @} */

	/** @name ���������
	*  @{
	*/
	/** ����ָ��������
	* @param [in] s ��������
	* @return ������
	*/
	Workspace & operator() (size_t s);
	/** ����ָ��������
	* @param [in] s ��������
	* @return ��������const���ã�
	*/
	const Workspace & operator() (size_t s) const;
	/** ��ȡ����������
	* @param ��
	* @return �����������
	*/
	inline size_t  size() const { return _regs.size(); }
	/** @} */

	/** @name �ļ���д������
	*  @{
	*/
public:
	/** д�ļ�
	* @param [in] f �����
	* @return ��
	*/
	virtual void write(std::ostream & f);
	/** ���ļ�
	* @param [in] f ������
	* @return ��
	*/
	virtual int read(std::istream & f);//xushicong 2009-10-14 ����ֵ��0��������2���ļ���ȡ����
	/** ���ļ����ع���
	* @param [in] sz����·��
	* @return ��
	*/
	virtual void load(std::string & sz);
	/** �����̱��浽�ļ�
	* @param [in] sz����·��
	* @return ��
	*/
	virtual void save(std::string & sz);

	virtual void setProjectAdapter(ProjectAdapterSharedPtr prjadapt){};

	/** @} */
};
/** @} */


//�ֲ�ʽ���ݿ�Ĺ��̶���ʵ�֣���ӦProject::create(signal),signal =1;
class DDBProjectImpl :public ProjectImpl
{
protected:
	ProjectAdapterSharedPtr m_projadapt;
public:
	DDBProjectImpl(ProjectAdapterSharedPtr _projadapt);
	DDBProjectImpl(PRJID id, std::string name, double * bound);
	void copy(const DDBProjectImpl &);
	DDBProjectImpl(const DDBProjectImpl &);
	DDBProjectImpl operator=(const DDBProjectImpl &impl);

	~DDBProjectImpl();
public:
	//��д���̵Ķ�д
	virtual void load(std::string & sz);
	virtual void save(std::string & sz);
	virtual void setProjectAdapter(ProjectAdapterSharedPtr prjadapt);

};


end_gdb_namespace
end_gtl_namespace


