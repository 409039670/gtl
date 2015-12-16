/*
* Geosciences Template Library
*
* Copyright (c) 2008
* Zhenwen He (zwhe@hotmail.com)
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Zhenwen He  makes no  representations
* about the suitability of this software for any purpose. 
* It is provided "as is" without express or implied warranty.
*/
//ֱ����GV3dCore��ʵ��
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include "property.h"
#include "buffer.h"
#include "geometry2d.h"
#include "geometry3d.h"
#include "topology.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

#ifndef POINTRADIUS
/** ��Χ�е���С��Чֵ  */
#define  POINTRADIUS 0.000001 
#endif


// ��Щ��Ϣ�洢�����ļ��ĸ����ļ��У����Թ������������������ʹ��
struct FeatureIndexItem{
	FID    _fid; //�����ID
	FCLSID _fclsid; //����
	int    _state; //��ʾ��¼��¼��״̬��0ԭʼ���ݣ�1ɾ����ʾ��2���±�ʾ
	unsigned long _offset; //��¼�ļ�ָ�����ʼλ��:������ļ�ͷ����ƫ�Ƶĳ���
	unsigned long _bufLength; //��¼�ÿ�buf�ĳ���
	unsigned long _bufLengthNoPro;//��¼featureû��������Ϣ�ĳ���
	GDBTIME  _writeTime;	//��¼�ļ�д���ʱ��
	GDBTIME  _updateTime;	//��¼�ļ�д���ʱ��
	double _bound[6];//����İ�Χ��:����������ʹ��
};

//����Vector���Ƿ��Ѿ����ڸ�����Ҫ�ض���  [HZW, 2009-8-18]
struct FeatureItemExist 
{ 
	FeatureIndexItem Target; 
	FeatureItemExist( const FeatureIndexItem& t ):Target(t){}; 
	bool operator()( const FeatureIndexItem *s ) {
		return Target._fid == s->_fid && Target._state == s->_state; 
	} 
}; 




/** @defgroup Feature Feature-Ҫ������� 
*  @{
*/
class  Project;
class  Workspace;
class  Layer;
class  FeatureSet;
class  Feature;
class  FeatureClass;
class  LODInfo;


typedef std::shared_ptr<Project> ProjectSharedPtr;
typedef Project*  ProjectPtr;
typedef Project&  ProjectRef;

typedef std::shared_ptr<Workspace> WorkspaceSharedPtr;
typedef Workspace*  WorkspacePtr;
typedef Workspace&  WorkspaceRef;

typedef std::shared_ptr<Layer> LayerSharedPtr;
typedef Layer*  LayerPtr;
typedef Layer&  LayerRef;

typedef std::shared_ptr<FeatureSet> FeatureSetSharedPtr;
typedef FeatureSet*  FeatureSetPtr;
typedef FeatureSet&  FeatureSetRef;
typedef std::shared_ptr<Feature> FeatureSharedPtr;
typedef Feature&  FeatureRef;
typedef std::weak_ptr<Feature> FeatureWeakPtr;
typedef std::shared_ptr<FeatureClass> FeatureClassSharedPtr;
typedef FeatureClass& FeatureClassRef;
typedef FeatureClass* FeatureClassPtr;
typedef std::shared_ptr<LODInfo> LODInfoSharedPtr;
typedef LODInfo* LODInfoPtr;
typedef LODInfo& LODInfoRef;
typedef std::vector<FeatureSharedPtr > FeatureSharedPtrVector;
typedef std::vector< FeatureWeakPtr >   FeatureWeakPtrVector;
typedef std::shared_ptr<std::vector< FeatureSharedPtr> > FeatureVectorSharedPtr;

typedef Identifier::raw_type FCLSID;
typedef Identifier::raw_type FID;
typedef Identifier::raw_type FSID;
typedef Identifier::raw_type LAYERID;
/** @} */


/** @addtogroup Feature LODInfo 
*  @{
*/
class GV3DCORE_API LODInfo {
public:
	/**LOD�ṹ��*/
	struct LOD_PARAM{
		int			nDrawFlag;		//[�Ƿ���switch���룬�Ժ��ַ�ʽ����]
		double		dSwitchIn;		//��LOD��ε�������Ӿ���
		double		dSwitchOut;		//��LOD��ε���Զ���Ӿ���
		double		nDataSize;		//��LOD��εļ�������������
		int			nTriangleNum;	//��LOD��εļ�������������Ƭ����
		int			nVertexNum;		//��LOD��εļ������ݶ�����
		double		reserve[4];		//�����ֶ�
	};

	/**LOD������Ϣ��LOD BLOB�洢����Ϣ��*/
	struct LOD_INFO {
		int			lodLevel;		//LOD�ļ���
		LOD_PARAM	lodparam;		//LOD������Ϣ
	};

	typedef std::pair<LOD_INFO/*���Ӿ���*/,GeometrySharedPtr> Info;	
	typedef std::vector<Info>   InfoArray;
public:
	virtual  int getCurLevel()=0;
	virtual int getLevel(double dis)=0;
	virtual void setID(FID id)=0;
	virtual FID getID()=0;
	virtual int getLevels()=0;
	virtual InfoArray & getLods()=0;
	virtual bool addInfo(const LOD_INFO &lodinfo, GeometrySharedPtr g)=0;
	virtual size_t sizeBuffer()=0;
	virtual bool writeBuffer(Buffer& buf) =0;
	virtual bool readBuffer(const Buffer & buf)	=0;
	virtual void write(std::ostream & f)=0;
	virtual void read(std::istream & f)=0;
public:
	static LODInfoSharedPtr create();
};
/** @} */

/** @addtogroup Feature FeatureClass 
*  @{
*/
class GV3DCORE_API FeatureClass {
public:
	virtual FCLSID getID()=0;
	virtual void  setID(FCLSID id )=0;
	virtual std::string getName()=0;
	virtual void  setName(std::string v )=0;	
	virtual std::string getCode()=0;
	virtual void  setCode(std::string v )=0;
	virtual int getStatus()=0;
	virtual void  setStatus(int s )=0;	
	virtual void copy(const FeatureClass & fc )=0;
	virtual bool isEqual(const FeatureClass & fc)=0;
	virtual void addPropType(const PropType & pt)=0;
	virtual void addPropType(std::string name,std::string code,std::string type,int length,int decimal=0,bool nullable=false)=0;
	virtual void removePropType(std::string name)=0;	
	virtual std::vector<PropType> & getPropTypes()=0;
	virtual void setLocalFeatureSet(FeatureSetSharedPtr pfs)=0;
	virtual FeatureSetSharedPtr getLocalFeatureSet()=0;
	virtual void write(std::ostream & f)=0;
	virtual void read(std::istream & f)=0;
	virtual size_t sizeBuffer()=0;
	virtual void writeBuffer(Buffer &buf)=0;
	virtual void readBuffer(Buffer &buf)=0;
	virtual FeatureClassSharedPtr clone()=0;
	virtual SREFID  getSpatialReferenceID()=0;
	virtual int  getPartitionNumber()=0;
	virtual void  setSpatialReferenceID(SREFID)=0;
	virtual void  setPartitionNumber(int )=0;
	
public:
	static FeatureClassSharedPtr findFeatureClass(FCLSID fclsid);
	static std::map<FCLSID,FeatureClassSharedPtr> & getFeatureClassMap();
	static void setFeatureClassMap(std::map<FCLSID,FeatureClassSharedPtr> * p);


	static FeatureClassSharedPtr create();
	static FeatureClassSharedPtr create(std::string name,std::string code);
	static FeatureClassSharedPtr create(FCLSID id, std::string name, std::string code);
};
/** @} */

/** @addtogroup Feature Feature  
*  @{
*/
class GV3DCORE_API Feature {
public:
	enum{
		STATUS_DELETED =0x00000001,
		STATUS_HIDED   =0x00000002,
		STATUS_SELECTED=0x00000004
	};
	virtual  void setID(FID id)=0 ;

	virtual FID getID()=0;
    // ��ȡҪ�ض�������

	virtual std::string getName()=0;
    // ����Ҫ�ض�������

	virtual void setName(
        // Ҫ�ض��������
        std::string name)=0;

	virtual REGID getWorkspaceID()=0;
	/** ����Ҫ�ط���ID
	* @param  [in] v Ҫ�ط���ID
	* @return ��
	*/
	virtual void setWorkspaceID(REGID v)=0;

	virtual void setCode(std::string szCode)=0;

	virtual std::string getCode()=0;
	/** ͼ�ε�״ֵ̬,��ʼ��ֵΪ 0x0000;//0λ�� Ϊ1-��ʾ�߼�ɾ�� //1λ�� Ϊ1-��ʾ����	//2λ�� Ϊ1-��ʾѡ��*/	
	/** 3λ�� Ϊ1-��ʾ��ʾ��//7λ�� Ϊ1-��ʾ��ʾ���򣬷�����Ϊ0 //8λ�� Ϊ1-��ʾ��������������Ϣ��������Ϊ0//9��15λ ������*/
	virtual int getStatus()=0;

	virtual void setStatus(int v)=0;

	virtual bool getPropAccessFlag()=0;

	virtual void setPropAccessFlag(bool propreaded)=0;


	virtual int getLODLevels()=0;
 
	virtual void setLODLevels(int v)=0;

	virtual LODInfoSharedPtr getLODInfo()=0;

	virtual void setLODInfo(LODInfoSharedPtr )=0;
	

	virtual void setFeatureClass(FeatureClassSharedPtr fc)=0;
	virtual void setFeatureClassID(FCLSID )=0;

	virtual FeatureClassSharedPtr getFeatureClass()=0;

	virtual FCLSID getFeatureClassID()=0;

	virtual void getProps( std::vector<std::string> & )=0;

	virtual bool setProp(int idex,const std::string & sz)=0;

	virtual const std::vector<std::string> & getProps()const =0;
	virtual std::vector<std::string> & getProps() =0;

	virtual void setProps(std::vector<std::string>&  )=0;

	virtual GeometrySharedPtr getGeometry(int LODLevel=-1)=0;

	virtual void setGeometry(GeometrySharedPtr)=0;

	virtual bool attatchGeometry (Geometry * g)=0;


	virtual void copy(FeatureSharedPtr sp)=0;


	virtual void write(std::ostream & f)=0;

	virtual void read(std::istream & f)=0;

	virtual size_t sizeBuffer()=0;

	virtual void writeBuffer(Buffer & buf)=0;

	virtual void readBuffer(const Buffer & buf)=0;

	/** ��ȡҪ�ص�Buffer��С���˺���������ʹ�ã�����ÿ�μ��㣬����ٶ�
	* @param  �� 
	* @return Ҫ�ص�Buffer����
	*/
	virtual unsigned long getBufferSize()=0;
	/** ����Ҫ�ص�Buffer��С���˺���������ʹ�ã�����ÿ�μ��㣬����ٶ�
	* @param  [in]  Buffer��С
	* @return ��
	*/
	virtual void  setBufferSize(unsigned long buffersize )=0;

	virtual void getEnvelope(Envelope3dRef d)=0;
	virtual void setEnvelope(const Envelope3dRef d)=0;
	virtual void getEnvelope(double * )=0;
	virtual void setEnvelope(double *)=0;

	virtual bool isExist(FLOD f)=0;

	virtual GDBTIME getCreateTime()=0;
	virtual void setCreateTime(GDBTIME v)=0;
	virtual GDBTIME getUpdateTime()=0;
	virtual void setUpdateTime(GDBTIME v)=0;


	/** ��ȡҪ�س�������Ϣ���Buffer��С
	* @param  ��
	* @return Buffer��С
	*/
	virtual size_t sizeBufferNoProp()=0;
	/**��featureȥ��������Ϣ�������д��Buffer
	* @param  [in] buf
	* @return  ��
	*/
	virtual void writeBufferEndProp(Buffer & buf)=0;
	/**��ȡfeatureȥ��������Ϣ���Buffer
	* @param  [in,out] buffer��Ϣ
	* @return  ��
	*/
	virtual void readBufferNoProp(const Buffer & buf)=0;

public:
	/** @name ��̬��Ա����˵��  
	*  @{
	*/
	/** ����Ҫ�ض���
	* @param  ��
	* @return feature ָ��
	*/
	static FeatureSharedPtr create();
	/** ����Ҫ�ص�ID
	* @param  [in] fc  Ҫ����ָ��
	* @param  [in] id1  Ҫ��ID
	* @param  [in] name Ҫ������
	* @param  [in] lod   lod��
	* @param  [in] regid  ����ID
	* @param  [in] state  Ҫ��״̬
	* @param  [in] pg      ������Ϣ
	* @param  [in] creatTime ����ʱ��
	* @param  [in] updateTime ����ʱ�� 
	* @param  [in] buffersize  buffer��С
	* @return ��
	*/
	static FeatureSharedPtr create(FeatureClassSharedPtr fc,FID id1, std::string name, int lod, REGID regid, int state=0, GeometrySharedPtr pg =GeometrySharedPtr(), 
		GDBTIME createTime=0 ,GDBTIME updateTime=0,unsigned long buffersize = 0);	
	static FeatureSharedPtr create(FCLSID fclsid,FID id1, std::string name, int lod, REGID regid, int state=0, GeometrySharedPtr pg =GeometrySharedPtr(), 
		GDBTIME createTime=0 ,GDBTIME updateTime=0,unsigned long buffersize = 0);
	/** ����Ҫ�ض���
	* @param  [in] f Ҫ�ض���
	* @return feature ָ��
	*/
	static FeatureSharedPtr create (FeatureRef f);
	/** @} */ 

};
/** @} */

/** @addtogroup Feature FeatureSet  
*  @{
*/
class GV3DCORE_API FeatureSet {
public:
	virtual FSID getID()=0;
	virtual void setID(FSID id)=0;
	virtual std::string getName()=0;
	virtual void setName(std::string szName)=0;
	virtual void setFeatureClass( FeatureClassSharedPtr)=0;
	virtual FeatureClassSharedPtr   getFeatureClass()=0;
	virtual void setLayerID(LAYERID lyr)=0;
	virtual LAYERID getLayerID()=0;	

	virtual void clear()=0;
	virtual void copy(const FeatureSetSharedPtr  fSet , bool bContent=false)=0;
	virtual void copy(const FeatureSetRef  fSet , bool bContent=false)=0;
	virtual size_t  size() const =0;
	virtual void addFeature(FeatureSharedPtr p)=0;
	virtual void eraseFeature(FID fid)=0;
	virtual FeatureSharedPtr removeFeature(FID fid)=0;
	virtual FeatureSharedPtrVector  & getFeatures()=0;
	
	virtual void setFeatures(FeatureSharedPtrVector &feas ) =0;

	virtual void write(std::ostream & f)=0;
	virtual void read(std::istream & f)=0;

public:
	/**����Ҫ�ض��󣬲���ӵ�Ҫ�ؼ�����
	* @param [in] id  Ҫ��ID
	* @param [in] name  Ҫ������
	* @param [in] lod  Ҫ��Lod
	* @param [in] regid  Ҫ��Ҫ�ط���ID
	* @param [in] state  Ҫ��ͼ�ε�״ֵ̬
	* @param [in] pg  Ҫ�ؼ�����Ϣ
	* @param [in] createTime  Ҫ�ش���ʱ��
	* @param [in] updateTime  Ҫ�ظ���ʱ��
	* @return  Feature
	*/
	virtual FeatureSharedPtr createFeature(FID id, std::string name, int lod, REGID regid, int state=0, GeometrySharedPtr pg=GeometrySharedPtr() ,GDBTIME createTime=0 ,GDBTIME updateTime=0)=0;
	/**����Ҫ�ض��󣬲���ӵ�Ҫ�ؼ�����
	* @param [in] name  Ҫ������
	* @param [in] lod  Ҫ��Lod
	* @param [in] regid  Ҫ��Ҫ�ط���ID
	* @param [in] state  Ҫ��ͼ�ε�״ֵ̬
	* @param [in] pg  Ҫ�ؼ�����Ϣ
	* @param [in] createTime  Ҫ�ش���ʱ��
	* @param [in] updateTime  Ҫ�ظ���ʱ��
	* @return  Feature
	*/
	virtual FeatureSharedPtr createFeature(std::string name, int lod, REGID regid, int state=0, GeometrySharedPtr pg=GeometrySharedPtr() ,/*C3DSemanticTopology * pt=0 ,*/GDBTIME createTime=0 ,GDBTIME updateTime=0)=0;
	/**���ƹ���Ҫ�ض��󣬲���ӵ�Ҫ�ؼ�����
	* @param [in] f Ҫ�ض���	
	* @return  Feature
	*/
	virtual FeatureSharedPtr createFeature(FeatureRef f )=0;
public:

	static FeatureSetSharedPtr create(FeatureClassSharedPtr );
	static FeatureSetSharedPtr create(const FeatureClassRef  fcls );
};
/** @} */

/** @addtogroup Feature Layer  
*  @{
*/
class GV3DCORE_API Layer {
public:
	/** @} */ 
	/** @name ���������븳ֵ���캯�� 
	*  @{
	*/
	virtual void copy(const LayerRef lf )=0;
	/** @} */ 

	/** @name �������û�ò����� 
	*  @{
	*/

	/**��ȡͼ��ID
	* @param  ��
	* @return   ͼ��ID
	*/
	virtual LYRID getID()=0;
	/**����ͼ��ID
	* @param [in] layid ͼ��ID
	* @return ��
	*/
	virtual void  setID(LYRID layid )=0;

	/**��ȡͼ������
	* @param ��
	* @return  ͼ������
	*/
	virtual std::string getName()=0;
	/**����ͼ������
	* @param [in] s ͼ������
	* @return  
	*/
	virtual void setName(std::string s)=0;


	/**��ȡͼ���������ڹ���
	* @param ��
	* @return  prjID ����ID
	*/
	virtual PRJID getProjectID()=0;
	/**����ͼ���������ڵĹ���
	* @param [in] prjID ����ID
	* @return  
	*/
	virtual void  setProjectID(PRJID prjID )=0;

	/**��ȡͼ���Ӧ��Ҫ������ID����
	* @param ��
	* @return  ͼ���Ӧ��Ҫ������ID����
	*/
	virtual vector<FCLSID>& getFeatureClassIDs()=0;
	/**����ͼ���Ӧ��Ҫ������ID����
	* @param [in] fclsids  Ҫ������ID����
	* @return  ��
	*/
	virtual void setFeatureClassIDs(vector<FCLSID>& fclsids)=0;

	/** ��ȡͼ���Ӧ��Ҫ�ؼ��� 
	* @param ��
	* @return  ͼ���Ӧ��Ҫ�ؼ���
	*/
	virtual std::vector<FeatureSetSharedPtr> & getFeatureSetVector()=0;
	/** ��ȡͼ���Ӧ�ĵ�ǰҪ�� 
	* @param ��
	* @return  ͼ���Ӧ�ĵ�ǰҪ��
	*/
	virtual FeatureSetSharedPtr getCurrentFeatureSet()=0;
	/** ����ͼ���Ӧ�ĵ�ǰҪ�� 
	* @param [in] s ��ǰҪ�ؼ�������
	* @return  ��
	*/
	virtual void setCurrentFeatureSet(size_t s)=0;
	/** @} */ 

	/** @name ��������������  
	*  @{
	*/
	/** ��ͼ���Ҫ������ID���������ָ��Ҫ������ID
	* @param [in] fclsid Ҫ������ID
	* @return  ��
	*/
	virtual void addFeatureClassID(FCLSID fclsid) =0;

	/** ��ͼ���Ҫ������ID������ɾ��ָ��Ҫ������ID
	* @param [in] fclsid Ҫ������ID
	*/
	virtual void eraseFeatureClassID(FCLSID fclsid)=0;


	/** ��ȡָ��Ҫ��ID��Ҫ������Ҫ�ؼ������� 
	* @param [in] id Ҫ��ID
	* @return  ʧ�� -1,�ɹ�����Ҫ�ؼ�������
	*/
	virtual size_t indexFeatureSet(FSID id)=0;

	/** ����һ��ָ��Ҫ�����Ҫ�ؼ�
	* @param [in] fc ָ��Ҫ������
	* @return Ҫ�ؼ�ָ��
	*/
	virtual FeatureSetSharedPtr createFeatureSet(const FeatureClassRef fc)=0;
	/** ��ͼ��������һ��Ҫ�ؼ�������
	* @param [in] p Ҫ������ָ��
	* return Ҫ������ָ��
	*/
	virtual FeatureSetSharedPtr addFeatureSet(FeatureSetSharedPtr fs)=0;
	/** ��ͼ���Ҫ�ؼ�����ɾ��ָ��Ҫ�ؼ���ID��Ҫ�ؼ�������
	* @param [in] fid Ҫ��ID
	* return ��
	*/
	virtual void eraseFeatureSet(FSID fsid)=0;
	/** ��ѯͼ��Ҫ�ؼ��ϴ�С
	* @param ��
	* return ͼ��Ҫ�ؼ��ϴ�С
	*/
	virtual size_t  size() const=0;

	/** ����ͼ�����ڵĹ�����ID��Ҫ�ؼ�������
	* @param [in] pws ��ǰ������ָ��
	*/
	virtual void setLocalWorkspace(WorkspaceSharedPtr pws)=0;
	/** ��ȡͼ�����ڵĹ�����
	* @param [in] 
	* @return ��ȡͼ�����ڵĹ�����
	*/
	virtual WorkspaceSharedPtr getLocalWorkspace()=0;

	/** ���ͼ���Ӧ��Ҫ�ؼ��� 
	* @param �� 
	* @return ��
	*/
	virtual void clear()=0;

	virtual FeatureSetSharedPtr getFeatureSet(int i)=0;
	/** @} */ 


	/** @name �ļ�IO����������  
	*  @{
	*/
public:
	/** д�ļ�
	* @param [in] f�����
	* @return ��
	*/
	virtual void write(std::ostream & f)=0;
	/** ���ļ�
	* @param [in] f������
	* @return ��
	*/
	virtual void read(std::istream & f)=0;
	/** �����ļ�
	* @param [in] sz�ļ�·��
	* @return ��
	*/
	virtual void load(std::string & sz)=0;
	/** �����ļ�
	* @param [in] sz�ļ�·��
	* @return ��
	*/
	virtual void save(std::string & sz)=0; 
	/** @} */ 

public:
	virtual bool isEqual(const LayerRef )=0;
	virtual bool isEqual(const LayerSharedPtr )=0;
public:
	static LayerSharedPtr create();
};
/** @} */

/** @addtogroup Feature Workspace  
*  @{
*/
class GV3DCORE_API Workspace {
public:
	virtual WKSPID getID()=0;
	virtual void  setID(WKSPID id )=0;
	virtual std::string getName()=0;
	virtual void  setName(std::string v )=0;
	virtual std::string getCode()=0;
	virtual void  setCode(std::string v )=0;
	virtual void setLocalProject(ProjectSharedPtr )=0;
	virtual ProjectSharedPtr getLocalProject()=0;
	
	virtual void write(std::ostream & f)=0;	
	virtual void read(std::istream & f)=0;	
	virtual void load(std::string & sz)=0;	
	virtual void save(std::string & sz)=0; 
public:
	static WorkspaceSharedPtr create();
};
/** @} */



class GV3DCORE_API ProjectAdapter
{
public:
	virtual void save(string &sz)=0;
	virtual void load(string &sz)=0;
};

typedef std::shared_ptr<ProjectAdapter> ProjectAdapterSharedPtr;

/** @addtogroup Feature Project  
*  @{
*/
class  GV3DCORE_API Project{
public:
	virtual PRJID getID()const=0;
	virtual void  setID(PRJID id )=0;
	virtual std::string getName()=0;
	virtual void  setName(std::string v )=0;
	virtual std::vector<WorkspaceSharedPtr> & getWorkspaces()=0;
	virtual void addWorkspace(WorkspaceSharedPtr p)=0;
	virtual size_t  size() const=0;
	virtual void write(std::ostream & f)=0;
	virtual int read(std::istream & f)=0;
	virtual void load(std::string & sz)=0;
	virtual void save(std::string & sz)=0; 
	virtual void getEnvelope(Envelope3dRef )=0;
	virtual void setEnvelope(Envelope3dRef )=0;
	virtual void setEnvelope( double * p )=0;

	//2013.9.13XiangFly
	virtual void setProjectAdapter(ProjectAdapterSharedPtr prjadapt)=0;
public:
	static ProjectSharedPtr create(int signal =0, ProjectAdapterSharedPtr prjadapt =NULL );
	
;
};
/** @} */





//XiangFly


end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
