#pragma once
#include "feature.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace 

/** @defgroup LODInfoImpl Lod��Ϣ������
*  @{
*/
class LODInfoImpl :public LODInfo {
public:
	/*
	CameraPosition ----------------D0-----------------D1-------------------D2-------------------D3-------------------D4-----------------
	G0                 G1                   G2                   G3                   G4
	*/

	/** @name ������Ա����˵��
	*  @{
	*/
protected:
	/**Ҫ�ض���ID*/
	FID             _fid;
	/**��ǰ����*/
	int             _cur_level;
	/**Lod��Ϣ��*/
	std::vector<Info>  _infos;
	/**buffer�Ĵ�С*/
	unsigned long _buffersize;

	/** @} */
public:
	/** @name ��������������
	*  @{
	*/
	LODInfoImpl();
	~LODInfoImpl();
	/** @} */


	/** @name �������û�ò�����
	*  @{
	*/

	/**��ȡ��ǰ����
	* @param  ��
	* @return   ��ǰ����
	*/
	inline int getCurLevel(){
		return _cur_level;
	}
	/**���ݾ����ж�LOD����
	* @param  [in] dis ����
	* @return   LOD����,-1����ʾ
	*/
	inline int getLevel(double dis){//
		for (size_t j = 0; j<_infos.size(); j++){
			if (dis < _infos[j].first.lodparam.dSwitchOut) {
				return _infos[j].first.lodLevel;
			}
		}
		return -1;
	}
	/**����Ҫ�ض���ID
	* @param [in] id Ҫ�ض���ID
	* @return  ��
	*/
	inline void setID(FID id) { _fid = id; }
	/**��ȡҪ�ض���ID
	* @param  ��
	* @return   Ҫ�ض���ID
	*/
	inline FID getID() { return _fid; }

	/**��ȡbuffer�Ĵ�С
	* @param  ��
	* @return   buffer�Ĵ�С
	*/
	inline unsigned long getBufferSize(){ return _buffersize; }
	/**����buffer�Ĵ�С
	* @param [in] buffersize buffer�Ĵ�С
	* @return ��
	*/
	inline void  setBufferSize(unsigned long buffersize){ _buffersize = buffersize; }

	/**��ȡ�ܵļ�����
	* @param  ��
	* @return   �ܵļ�����
	*/
	inline int getLevels(){
		return	_infos.size();
	}
	/**��ȡLOD��Ϣ
	* @param  ��
	* @return   LOD��Ϣ
	*/
	inline std::vector<Info> & getLods(){
		return _infos;
	}

	/**��Ӽ��ζ���
	* @param  [in] lodinfo LOD��Ϣ
	* @param  [in] g ���ζ���ָ��
	* @return  �ɹ�true��ʧ��false
	*/
	bool addInfo(const LOD_INFO &lodinfo, GeometrySharedPtr g);




	/**��featureʹ�ã�ֻ��LOD�ļ���ÿ������Ķ�Ӧ�Ĳ�������д���ļ�:����Ϣ����Geometry������
	* @param  ��
	* @return  ��Ҫ��buffer��С
	*/
	virtual size_t sizeBuffer();
	/** @} */

	/** @name �ļ�IO����������
	*  @{
	*/

	/**��lod��Ϣд��Buffer
	* @param  [in] buf
	* @return  �ɹ�true��ʧ��false
	*/
	virtual bool writeBuffer(Buffer& buf);
	/**��Bufferlod��ȡlod��Ϣ
	* @param  [in] buf
	* @return  �ɹ�true��ʧ��false
	*/
	virtual bool readBuffer(const Buffer & buf);

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
};
/** @} */ // End of the group block


 
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
