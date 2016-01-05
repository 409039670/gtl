#pragma once
#include "feature.h" 
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace 
/** @defgroup Builder Ҫ�ض���ӿ�
*  @{
*/
class FeatureImpl : public Feature {
	/** @name ������Ա����˵��
	*  @{
	*/
protected:
	/** Ҫ��ID*/
	FID    _id;
	/** Ҫ������*/
	std::string   _name;
	/** ͼ�ε�״ֵ̬,��ʼ��ֵΪ 0x0000;//0λ�� Ϊ1-��ʾ�߼�ɾ�� //1λ�� Ϊ1-��ʾ����	//2λ�� Ϊ1-��ʾѡ��*/
	/** 3λ�� Ϊ1-��ʾ��ʾ��//7λ�� Ϊ1-��ʾ��ʾ���򣬷�����Ϊ0 //8λ�� Ϊ1-��ʾ��������������Ϣ��������Ϊ0//9��15λ ������*/
	int   _state;
	/** Ҫ��LOD���ܼ���*/
	int  _lod;
	/** LOD��Ϣ*/
	LODInfoSharedPtr  _lod_info;
	/** SDO_GEOMETRY��Ϊ�˴���R������*/
	//SDO_GEOMETRY* _orclBound;
	/** Ҫ�ط���ID*/
	WKSPID  _regid;
	//double        _bound[6];//X����С���Y����С���Z����С���
	/** ָ�򼸺���Ϣ*/
	GeometrySharedPtr   _geom;
	//Topology*     _topo;//ָ��������Ϣ������	
	/** ����ʱ��*/
	GDBTIME  _createTime;
	/** ����ʱ��*/
	GDBTIME  _updateTime;
	/** Ҫ�ض����Ӧ��Ҫ����*/
	FCLSID  _fclsid;
	/** ���Ҫ�ص�����*/
	std::vector<std::string> _props;
	/** Ҫ�����ͱ���*/
	string		  _code;
	/** ȥ��Ҫ��ʱ�Ƿ��ж�ȡ���� 0û�ж���1��ȡ��*/
	bool _propreaded;
	/** ��buffer�Ĵ�С*/
	unsigned long    _buffersize;
	/** @} */


public:
	/** @name ��������������
	*  @{
	*/
	FeatureImpl();
	FeatureImpl(FeatureClassSharedPtr fc, FID id1, std::string name, int lod, REGID regid, int state = 0, GeometrySharedPtr pg = GeometrySharedPtr(),
		GDBTIME createTime = 0, GDBTIME updateTime = 0, unsigned long buffersize = 0);
	FeatureImpl(FCLSID fc, FID id1, std::string name, int lod, REGID regid, int state = 0, GeometrySharedPtr pg = GeometrySharedPtr(),
		GDBTIME createTime = 0, GDBTIME updateTime = 0, unsigned long buffersize = 0);
	FeatureImpl(FeatureImpl & f);
	void copy(const FeatureImpl & f);

	void copy(FeatureSharedPtr sp);
	virtual ~FeatureImpl();
	/** @} */

public:
	/** @name Ҫ��Feature����������
	*  @{
	*/
	/** ����Ҫ�ص�ID
	* @param  [in] id  Ҫ��ID
	* @return ��
	*/
	inline void setID(FID id);
	/** ��ȡҪ�ص�ID
	* @param  ��
	* @return Ҫ��ID
	*/
	inline FID getID();
	/** ��ȡҪ�ص�����
	* @param   ��
	* @return Ҫ������
	*/
	inline std::string getName();
	/** ����Ҫ�ص�����
	* @param  [in]  v Ҫ������
	* @return ��
	*/
	inline void setName(std::string v);
	/** ��ȡҪ�ص�LOD�ܼ�����
	* @param  ��
	* @return Ҫ�ص�LOD�ܼ�����
	*/
	inline int getLODLevels();
	/** ����Ҫ�ص�LOD�ܼ�����
	* @param  [in]  v �ܼ�����
	* @return ��
	*/
	inline void setLODLevels(int v);
	/** ��ȡҪ�ص�LOD��Ϣ
	* @param  ��
	* @return Ҫ�ص�LOD��Ϣ
	*/
	inline LODInfoSharedPtr getLODInfo();
	/** ����Ҫ�ص�LOD��Ϣ
	* @param  [in]  v LOD��Ϣ
	* @return  ��
	*/
	inline void setLODInfo(LODInfoSharedPtr v);
	/** ��ȡҪ�صķ���ID
	* @param  ��
	* @return Ҫ�ص�LOD��Ϣ
	*/
	inline REGID getWorkspaceID();
	/** ����Ҫ�ط���ID
	* @param  [in] v Ҫ�ط���ID
	* @return ��
	*/
	inline void setWorkspaceID(REGID v);
	/** ����Ҫ�ر���
	* @param  [in] szCode Ҫ�ط���ID
	* @return ��
	*/
	inline void setCode(string szCode);
	/** ��ȡҪ�ر���
	* @param  ��
	* @return Ҫ�ر���
	*/
	inline std::string getCode();
	/** ��ȡҪ�ص����Χ��
	* @param  ��
	* @return Ҫ�ص����Χ��
	*/

	virtual void getEnvelope(Envelope3dRef d){
		_geom->getEnvelope(d);
	}
	/** ����Ҫ�ص����Χ��
	* @param   env3d Ҫ�ص����Χ��
	* @return ��
	*/
	virtual void setEnvelope(const Envelope3dRef env3d);
	/** ��ȡҪ�ص����Χ��
	* @param  [out] pv Ҫ�ص����Χ��
	* @return ��
	*/

	virtual void getEnvelope(double *d);
	/** ����Ҫ�ص����Χ��
	* @param  [in] d[6] Ҫ�ص����Χ��
	* @return ��
	*/
	virtual void setEnvelope(double *d);
	/** ��ȡҪ�ش���ʱ��
	* @param  ��
	* @return GDBTIME ����ʱ��
	*/
	inline GDBTIME getCreateTime();
	/** ����Ҫ�ش���ʱ��
	* @param  [in] v ����ʱ��
	* @return ��
	*/
	inline void setCreateTime(GDBTIME v);
	/** ��ȡҪ�ظ���ʱ��
	* @param   ��
	* @return  ����ʱ��
	*/
	inline GDBTIME getUpdateTime();
	/** ����Ҫ�ظ���ʱ��
	* @param  [in] v ����ʱ��
	* @return ��
	*/
	inline void setUpdateTime(GDBTIME v);
	/** ��ȡҪ��ͼ�ε�״ֵ̬
	* @param   ��
	* @return Ҫ��ͼ�ε�״ֵ̬
	*/
	inline int getStatus();
	/** ����Ҫ��ͼ�ε�״ֵ̬
	* @param  [in]  v ͼ�ε�״ֵ̬
	* @return ��
	*/
	inline void setStatus(int v);
	/** ����Ҫ��������Ҫ����
	* @param  [in] fc Ҫ����ָ��
	* @return ��
	*/
	inline void setFeatureClass(FeatureClassSharedPtr fc);
	inline void setFeatureClassID(FCLSID);
	/** ��ȡҪ��������Ҫ����
	* @param  ��
	* @return Ҫ����ָ��
	*/
	inline FeatureClassSharedPtr getFeatureClass();
	/** ��ȡҪ��������Ҫ����ID
	* @param   ��
	* @return Ҫ����ID
	*/
	inline FCLSID getFeatureClassID();
	/** ��ȡ��Ҫ���Ƿ������������Ϣ_����Ϣ��λ�洢��_state������
	* @param  ��
	* @return Ҫ����ID
	*/
	inline bool hasSemantic();
	/** ���ø�Ҫ���Ƿ������������Ϣ_����Ϣ��λ�洢��_state������
	* @param  [in] hassemantic ��Ҫ���Ƿ������������Ϣ
	* @return true �� false ��
	*/
	inline void hasSemantic(bool hassemantic);
	/** @} */


	/** @name Ҫ��Feature���Բ���������
	*  @{
	*/
	/** ��ȡҪ�ص������ֶμ���
	* @param  ��
	* @return Ҫ�ص������ֶμ���
	*/
	virtual const std::vector<std::string>& getProps() const;
	virtual   std::vector<std::string>& getProps();
	virtual void getProps(std::vector<std::string> & sa);
	///** ����Ҫ�ص������ֶμ��ϴ�С
	//* @param   ��
	//* @return �����ֶμ��ϴ�С
	//*/
	//inline int setPropSize();
	/** ����Ҫ�ص������ֶμ���
	* @param  [in]  prop �����ֶμ���
	* @return ��
	*/
	inline void setProps(std::vector<std::string>& prop);
	/** ��ȡ��ȡҪ��ʱ���Ƿ��ж�ȡ����
	* @param  ��
	* @return true ��ȡ�� false û�ж�
	*/
	inline bool getPropAccessFlag();
	/** ��ȡ��ȡҪ��ʱ���Ƿ��ж�ȡ����
	* @param  ��
	* @return true ��ȡ�� false û�ж�
	*/
	inline void setPropAccessFlag(bool propreaded);

	virtual bool setProp(int idex, const std::string & sz){
		if (_props.size() <= idex)
			return false;
		else{
			_props[idex] = sz;
			return true;
		}
	}

	/** @} */



public:
	/** @name ������Ϣ������
	*  @{
	*/
	/** ��ȡҪ�صļ�����Ϣ
	* @param  ��
	* @return Geometry ������Ϣ
	*/
	inline GeometrySharedPtr geometry();

	/** ���������Geometry��Ȼ��󶨵���ǰfeature��
	* @param  [in] g  Geometryָ��
	* @return true �󶨳ɹ� false ��ʧ��
	*/
	inline void setGeometry(GeometrySharedPtr g);
	/**�滻Ҫ�صļ�����Ϣ
	* @param  [in] g  Geometryָ��
	* @return true �󶨳ɹ� false ��ʧ��
	*/
	inline bool attatchGeometry(GeometryPtr g);
	/** �ж�ָ��LOD�����Geometry�����Ƿ��Ѿ�����
	* @param  [in] flod  Ҫ��LOD��ID
	* @return true �󶨳ɹ� false ��ʧ��
	*/
	virtual bool isExist(FLOD flod);
	/** ��ȡָ��LOD�����Geometry
	* @param  [in] flod  Ҫ��LOD��ID
	* @return  ������Ϣָ��
	*/
	GeometrySharedPtr getLodGeom(FLOD flod);
	/** @} */

	virtual GeometrySharedPtr getGeometry(int LODLevel = -1){
		if (LODLevel <= 0)
			return _geom;
		else
			return getLodGeom(LODLevel);
	}

public:
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

public:
	/** @name �ڴ��д������
	*  @{
	*/
	/** ��ȡҪ�ص�Buffer��С
	* @param   ��
	* @return Buffer��С
	*/
	size_t sizeBuffer();
	/**��feature��Ϣд��Buffer
	* @param  [in] buf
	* @return  ��
	*/
	void writeBuffer(Buffer & buf);
	/**��Buffer��ȡfeature��Ϣ
	* @param  [in] buf
	* @return  ��
	*/
	void readBuffer(const Buffer & buf);
	/** ��ȡҪ�س�������Ϣ���Buffer��С
	* @param  ��
	* @return Buffer��С
	*/
	size_t sizeBufferNoProp();
	/**��featureȥ��������Ϣ�������д��Buffer
	* @param  [in] buf
	* @return  ��
	*/
	void writeBufferEndProp(Buffer & buf);
	/**��ȡfeatureȥ��������Ϣ���Buffer
	* @param  [in,out] buffer��Ϣ
	* @return  ��
	*/
	void readBufferNoProp(const Buffer & buf);
	/** ��ȡҪ�ص�Buffer��С���˺���������ʹ��
	* @param  ��
	* @return Ҫ�ص�Buffer����
	*/
	inline unsigned long getBufferSize();
	/** ����Ҫ�ص�Buffer��С���˺���������ʹ��
	* @param  [in]  Buffer��С
	* @return ��
	*/
	inline void  setBufferSize(unsigned long buffersize);
	/** @} */


	/** @name Ҫ�صļ��ζ������������
	*  @{
	*/
	/** ���ݴ�����������ɱ任������ɶ���ļ��α任
	* @param  [in]  dx  xֵ
	* @param  [in]  dy  yֵ
	* @param  [in]  dz  zֵ
	* @return ��
	*/
	void transferCoordinate(double dx, double dy, double dz);
	/** ���ݴ�����������ɱ任������ɶ���ļ��α任
	* @param  [in]  v3d  ����ֵ
	* @return ��
	*/
	void transferCoordinate(Vertex3d& v3d);
	/** ���ݴ�����������ɱ任������ɶ���ļ��α任
	* @param  [in]  m4x4  ����ֵ
	* @return ��
	*/
	void transferCoordinate(Matrix4x4& m4x4);
	/** @} */


	/** @name ���ó�Ա����˵��
	*  @{
	*/
public:
	/**  ��������*/
	friend class FeatureSet;
	friend class Feature;
	/** @} */
};
/** @} */ // End of the group block

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
