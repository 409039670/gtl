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
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>


#include "Selector.h"
#include "TemporarySet.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace


class QResult;

/** @defgroup proxy Proxy-���ݲ�ѯ�ӿ�  
    *  @{
    */
class GV3DCORE_API Proxy {	
	
	/** @name �������������� 
		*  @{
		*/
protected:	
	Proxy();
public:
	virtual ~Proxy();
	/** @} */ 
	/** @name ������Ա����˵��  
	*  @{
	*/
protected:
	/** ��ʱ��ָ�� */
	TemporarySets * _tempsets;
	/** @} */ 

protected:
	/** @name ������Ա����˵��  
	*  @{
	*/

public:
	virtual void setTemporarySets(TemporarySets * p) {_tempsets = p;}

	/** ��ȡ��ʱҪ�ؼ�
	* @param  ��
	* @return tempSets ��ʱҪ�ؼ�������
	*/	
	virtual TemporarySets& getTemporarySets()  { return * _tempsets;}
	/** @} */ 

		/** @name ���еĲ�ѯ�������� 
	*  @{
	*/
	/** ��ѯָ����Χ���ڵ�����Ҫ�ض���
	* @param  [in] bound ��Χ�з�Χ
	* @param  [in] fcid Ҫ����ID����
	* @param  [out] objs ��ѯ��������Ҫ�ض��󼯺�
	* @param  [in] readProps �Ƿ��ȡҪ�ض��������
	* @param  [in] r ���˰뾶���������İ�Χ�а뾶С��r�򲻵���
	* @param  [in] removes�����������е�FID����Ҫ���е���
	* @return �����ѯ�ɹ�������true�����򷵻�false
	*/
	virtual bool queryByBox(const Envelope3dRef bound, 		
		const std::vector<FCLSID> & fcid,
		std::vector<FeatureSharedPtr> & objs,
		const bool readProps = true,const double &r = 0.0,std::vector<FID> * removes=0)=0;

	/** ��ѯָ����Χ���ڵ�����Ҫ�ض���
	* @param  [in] bound ��Χ�з�Χ
	* @param  [in] fcid Ҫ����ID����
	* @param  [in] readProps �Ƿ��ȡҪ�ض��������
	* @return ��ѯ����Ĺ���ָ��
	*/
	virtual std::shared_ptr<QResult> queryByBox(const Envelope3dRef bound, 		
		const std::vector<FCLSID> & fcid, const bool readProps = true)=0;

	/** ��ѯָ����ѡ������Ҫ�ض���
	* @param  [in] point ѡ��Ķ���
	* @param  [in] fcid Ҫ����ID����
	* @param  [out] objs ��ѯ��������Ҫ�ض��󼯺�
	* @param  [in] readProps �Ƿ��ȡҪ�ض��������
	* @return �����ѯ�ɹ�������true�����򷵻�false
	*/
	virtual bool queryByPoint(const Vertex3dRef point, 
		const std::vector<FCLSID> & fcid, 
		std::vector<FeatureSharedPtr> & objs, const bool readProps = true)=0;	

	/** ��ѯ��ͨ��������Ҫ�ض���
	* @param  [in] mpline ѡ�����
	* @param  [in] fcid Ҫ����ID����
	* @param  [out] objs ��ѯ��������Ҫ�ض��󼯺�
	* @param  [in] readProps �Ƿ��ȡҪ�ض��������
	* @return �����ѯ�ɹ�������true�����򷵻�false
	*/	
	virtual bool queryByLine(const Vertex3dVector & mpline ,
		const std::vector<FCLSID> & fcid, 
		std::vector<FeatureSharedPtr> & objs, const bool readProps = true)=0;

	/** ��ѯ��ͨ��������Ҫ�ض���
	* @param  [in] mpline ѡ�����
	* @param  [in] fcid Ҫ����ID����
	* @param  [in] readProps �Ƿ��ȡҪ�ض��������
	* @return ��ѯ����Ĺ���ָ��
	*/
	virtual std::shared_ptr<QResult> queryByLine(const Vertex3dVector & mpline ,
		const std::vector<FCLSID> & fcid, const bool readProps = true)=0;

	/** ��ѯָ������η�Χ�ڵ�����Ҫ�ض���
	* @param  [in] mpgon ѡ��Ķ���η�Χ
	* @param  [in] fcid Ҫ����ID����
	* @param  [out] objs ��ѯ��������Ҫ�ض��󼯺�
	* @param  [in] readProps �Ƿ��ȡҪ�ض��������
	* @return �����ѯ�ɹ�������true�����򷵻�false
	*/
	virtual bool queryByPolygon(const Vertex3dVector & mpgon,
		const std::vector<FCLSID> & fcid, 
		std::vector<FeatureSharedPtr> & objs, const bool readProps = true)=0;

	/** ��ѯָ������η�Χ�ڵ�����Ҫ�ض���
	* @param  [in] mpgon ѡ��Ķ���η�Χ
	* @param  [in] fcid Ҫ����ID����
	* @param  [in] readProps �Ƿ��ȡҪ�ض��������
	* @return ��ѯ����Ĺ���ָ��
	*/
	virtual std::shared_ptr<QResult> queryByPolygon(const Vertex3dVector & mpgon,
		const std::vector<FCLSID> & fcid, const bool readProps = true)=0;

	/** ���ݶ���� FID ��ѯָ������
	* @param  [in] fid ��Ҫ���Ҷ����FID
	* @param  [in] fcid ��Ҫ���Ҷ����Ҫ�����FCLSID
	* @param  [out] obj ��ѯ�õ���Ҫ�ض���
	* @param  [in] readProps �Ƿ��ȡҪ�ض��������
	* @return �����ѯ�ɹ�������true�����򷵻�false
	*/
	virtual bool queryByFeatureID(const FID &fid, const FCLSID &fcid,  FeatureSharedPtr & obj, const bool readProps = true)=0; 

	/** ���ݶ�������fids��ѯ�������
	* @param  [in] fids ��Ҫ���Ҷ�������FID����
	* @param  [in] fcid ��Ҫ���Ҷ���ĸ�Ҫ�����FCLSID
	* @param  [out] objs ��ѯ�õ���Ҫ�ض���
	* @param  [in] readProps �Ƿ��ȡҪ�ض��������
	* @return �����ѯ�ɹ�������true�����򷵻�false
	*/
	virtual bool queryByFeatureIDs(const std::vector<FID> &fids,const FCLSID &fcid,
		vector <FeatureSharedPtr> &objs, const bool readProps = true)=0;

	/** ����Ҫ�����ȡID
	* @param  [in] fcid ��Ҫ���ҵ�Ҫ�����FCLSID
	* @param  [out] obj �õ���Ҫ�ض���
	* @param  [in] readProps �Ƿ��ȡҪ�ض��������
	* @return �����ѯ�ɹ�������true�����򷵻�false
	*/
	virtual bool queryByFeatureClassID(const FCLSID &fcid, vector<FeatureSharedPtr> & obj,const bool readProps = true)=0; 

	/** ����Ҫ����ID��ȡ����ID����
	* @param  [in] fcid ��Ҫ���ҵ�Ҫ�����FCLSID
	* @param  [out] fids ��ѯ�õ���Ҫ�ض���ID����
	* @return �����ѯ�ɹ�������true�����򷵻�false
	*/
	virtual bool queryFeatureIDsByFeatureClassID(const FCLSID &fcid, vector<FID> & fids)=0; 

	/** ��ѯָ��Ҫ��ID��ָ��LOD�����Ҫ�ض���
	* @param  [in] fid ��Ҫ���ҵ�Ҫ�ض���id
	* @param  [in] flod ��Ҫ���ҵ�Ҫ�ض����ָ��LOD����
	* @param  [in] fcid ��Ҫ���ҵ�Ҫ�ض�������Ҫ�����FCLSID
	* @return ���ҵ���Ҫ�ض���
	*/
	virtual FeatureSharedPtr loadLOD( const FID &fid, const FLOD &flod, const FCLSID &fcid)=0;
	
	/** ��ѯָ��Ҫ��ID��ָ��LOD�����Ҫ�ض���
	* @param  [in] fid ��Ҫ���ҵ�Ҫ�ض���id
	* @param  [in] flod ��Ҫ���ҵ�Ҫ�ض����ָ��LOD����
	* @param  [in] fcid ��Ҫ���ҵ�Ҫ�ض�������Ҫ�����FCLSID
	* @param  [out] obj ���ҵ���Ҫ�ض���
	* @return ��
	*/
	virtual void loadLOD( const FID &fid, const FLOD &flod, const FCLSID &fcid, FeatureSharedPtr & obj)=0;

	/** ��ѯָ��Ҫ��ID��ָ��LOD�����Ҫ�ض��󼯺�(flod����Ϊconst����)
	* @param  [in] fid ��Ҫ���ҵ�Ҫ�ض���id����
	* @param  [in] flod ��Ҫ���ҵ�Ҫ�ض����ָ��LOD����
	* @param  [in] fcid ��Ҫ���ҵ�Ҫ�ض�������Ҫ�����FCLSID
	* @param  [out] objs ���ҵ���Ҫ�ض��󼯺�
	* @return ��
	*/
	virtual void loadLOD( const std::vector<FID> & fid, const FLOD & flod, const FCLSID &fcid,
		std::vector<FeatureSharedPtr> &objs)=0;

	/** ��ѯָ��Ҫ������ID��ָ��LOD�����Ҫ�ض��󼯺�(flod����Ϊ��const����)
	* @param  [in] fcid ��Ҫ���ҵ�Ҫ�ض�������Ҫ�����FCLSID����
	* @param  [in] flod ��Ҫ���ҵ�Ҫ�ض����ָ��LOD����
	* @param  [out] objs ���ҵ���Ҫ�ض��󼯺�
	* @return ��
	*/
	virtual void loadLOD( const std::vector<FCLSID> & fcid,
		const FLOD & flod, 
		std::vector<FeatureSharedPtr> &objs)=0;

	/** ��ѯָ��Ҫ�ض��󼯺�
	* @param  [in] objs ���ҵ���Ҫ�ض��󼯺�
	* @return ��
	*/
	virtual void loadLOD( std::vector<FeatureSharedPtr> &objs)=0;

	/** ��ȡ���������е�Ҫ�����ͼ���
	* @param  [out] fcls_set ���ҵ�������Ҫ�����ͼ���
	* @return ��
	*/
	virtual void getAllFeatureClass(std::vector<FeatureClassSharedPtr> & fcls_set)=0;

	/** ����Ҫ����ID��ȡҪ����
	* @param  [in] fcid ��Ҫ���ҵ�Ҫ������ID
	* @param  [out] fcls ���ҵ���Ҫ������
	* @return �����ѯ�ɹ����򷵻�true�����򷵻�false
	*/
	virtual bool getFeatureClass(const FCLSID &fcid, FeatureClassSharedPtr &fcls)=0; 

	/** ��������л�ȡ�������
	* @param  [in] texid ��Ҫ���ҵ�����ID
	* @return ���ҵ����������
	*/
	virtual TextureSharedPtr getTexture(const TEXID &texid ,int iLevel = 0 )=0;

	/** ��������л�ȡ�������
	* @param  [in] texids ��Ҫ���ҵ���������ID����
	* @param  [out] Textures ���ҵ�������
	* @return ��
	*/
	virtual void getTexture(const vector<TEXID> &texids , vector<TextureSharedPtr> &Textures,int iLevel = 0 )=0;
	
	/** �Ӳ��ʿ��л�ȡ���ʶ���
	* @param  [in] matid ��Ҫ���ҵ�����ID
	* @return ���ҵ��Ĳ��ʶ���
	*/
	virtual MaterialSharedPtr getMaterial(const MATID &matid )=0;

	/** �Ӳ��ʿ��л�ȡ���ʶ���
	* @param  [in] matids ��Ҫ���ҵ����в���ID����
	* @param  [out] Mats ���ҵ���������
	* @return ��
	*/
	virtual void getMaterial(const vector<MATID> &matids , vector<MaterialSharedPtr> &Mats )=0;

	/** �ӹ���ģ�Ϳ��л�ȡ����ģ����Ϣ
	* @param  [in] mdlid ��Ҫ���ҵĹ���ģ��ID
	* @return ���ҵ��Ĺ���ģ��
	*/
	virtual SharedModelSharedPtr getSharedModel(const MDLID  &mdlid)=0;

	/** �ӹ���ģ�Ϳ��л�ȡ����ģ����Ϣ
	* @param  [in] Modleids ��Ҫ���ҵ����й���ģ��ID����
	* @param  [out] Modles ���ҵ���������
	* @return ��
	*/
	virtual void getSharedModel(const vector<MDLID> &Modleids , vector<SharedModelSharedPtr> &Modles )=0;

	/** �ж�ָ����ʱ���Ժ�ָ����FIDҪ�ض����Ƿ�ı䣬����ı䷵��TRUE�����ҷ���һ���µ�Ҫ�ض��󣬷��򷵻�FALSE������������Ϊ��	
	* @param  [in] t ָ����ʱ��
	* @param  [in] fid ָ��Ҫ�ض���ID
	* @param  [in] outpf ��ȡ���Ѿ����µ�Ҫ�ض���
	* @return ��������ɹ����򷵻�true�����򷵻�false
	*/
	virtual bool isUpdated(const time_t &t , const FID &fid,FeatureSharedPtr & outpf)=0;

	
	/** ��ȡ����texture��ID
	* @param  [in] g_texID ��ȡ��������texture��ID
	* @return ��������ɹ����򷵻�true�����򷵻�false
	*/
	virtual bool getAllTextureID( std::vector<TEXID> &g_texID)=0;

	/** ��ȡ����material��ID
	* @param [in]  g_matID ��ȡ��������material��ID
	* @return ��������ɹ����򷵻�true�����򷵻�false
	*/
	virtual bool getAllMaterialID( std::vector<MATID> &g_matID) = 0;

	/** ��ȡ���й���ģ��ID
	* @param [in]  g_mdlID ��ȡ�������й���ģ�͵�ID
	* @return ��������ɹ����򷵻�true�����򷵻�false
	*/
	virtual bool getAllSharedModelID( std::vector<MDLID> &g_mdlID) = 0;

	/** ͨ��SQL����ѯҪ�ض���
	* @param [in]  fclsid ����Ҫ���ҵ�Ҫ�ض�������ID
	* @param [in]  sql ��ѯ�����SQL���
	* @param [out]  fResult ��ȡ��������Ҫ�ض��󼯺�
	* @return ��������ɹ����򷵻�true�����򷵻�false
	*/
	virtual bool queryBySQL(const FCLSID &fclsid,std::string sql,std::vector<FeatureSharedPtr> &fResult) = 0;

	/** ��ȡ��������Χ��
	* @param ��
	* @return ��ȡ�Ĺ��̰�Χ��
	*/
	virtual Envelope3d getEnvelope()= 0;

	/**  ���ݴ����FCLSID��ֵ��ȡ����features�ĸ���
	 *	 @param [in]      fclsid   Ҫ�����͵�ΨһID
	 *   @return ��ȡָ�����͵�Ҫ�ض������    
	 */
	virtual int getFeatureCount(const FCLSID &fclsid) = 0;

	/**  ��ȡ����������ģ������ͳ����Ϣ
	*	 @param [in]      info    �ռ����ݿ�ͳ����Ϣ��               
	*                     info[0] Ҫ�������������Ϊ m
	*                     info[1] Ҫ������1��Ҫ�ظ���
	*                     info[2] Ҫ������2��Ҫ�ظ���
	*                     ...... 
	*                     info[m] Ҫ������m��Ҫ�ظ���
	*                     info[m+1] ��������
	*                               �������Ч������������
	*                               �������Ч��ɫ����
	*								���������
	*                               ��������
	*                               ת��Ϊ�ȼ�������Ƭ����
	*                               ������
	*								������
	*   @return ����ͳ����Ϣ    
	*/
	virtual int getStatisticsInfo(std::vector<unsigned long long > & info) = 0;
	/** @} */ 


	/** @name ���˲�����غ���
	*  @{
	*/	
	/** ��ȡ����TOPO�ڵ㣬Ϊ����������
	* @param  [out] nodes �������˵Ľ��
	* @return 0 -- ��ʾʧ�ܣ�1 -- ��ʾ���سɹ�
	*/
	virtual int getAllTopos(std::vector<TopoItemSharedPtr>& nodes) = 0;

	/** ��ȡ����TOPO������Ϣ��Ϊ����������
	* @param  [out] infos ����������Ϣ
	* @return 0 -- ��ʾʧ�ܣ�1 -- ��ʾ���سɹ�
	*/
	virtual int getAllTopos(std::vector<TopoInfoSharedPtr>&infos) = 0;
	
	/** ��ѯָ��Ҫ��ID��ָ�������������͵��������˽�㼯��
	* @param [in]		fid			Ҫ��ID
	* @param [in]		type		�������˲�ѯ����, Ϊ0ʱ��ʾ��������
	* @param [in]		fclsids		��Ҫ��ѯ�����������˹�ϵ��Ҫ������
	* @param [out]	 topologyItems	���ص��������˵Ľ��
	* @param [in]		queryFlag	��ѯ��ʶ
	* @return  0 -- ��ʾʧ�ܣ�1 -- ��ʾ���سɹ�
	*/	
	virtual long queryTopos(const FID &fid, const int &type, const vector<FCLSID>& fclsids, vector<TopoItemSharedPtr>& topologyItems, int queryFlag = 0) = 0;
	
	/** ��ѯָ��Ҫ��ID��ָ�������������͵��������˽�㼯��
	* @param [in]		fid			Ҫ��ID
	* @param [in]		type		�������˲�ѯ����, Ϊ0ʱ��ʾ��������
	* @param [out]	topologyItems	���ص��������˵Ľ��
	* @param [in]		queryFlag	��ѯ��ʶ
	* @return  0 -- ��ʾʧ�ܣ�1 -- ��ʾ���سɹ�
	*/	
	virtual long queryTopos(const FID& fid, const int &type, vector<TopoItemSharedPtr>& topologyItems, int queryFlag = 0) = 0;
	
	/** ��ѯָ��Ҫ��ID��ָ�����͵�����������Ϣ����
	* @param [in]		fid			Ҫ��ID
	* @param [in]		type		����������Ϣ����
	* @param [out]	topologyInfo		���ص�����������Ϣ����
	* @return  0 -- ��ʾʧ�ܣ�1 -- ��ʾ���سɹ�
	*/	
	virtual long queryTopo(const FID &fid, const int &type, TopoInfoSharedPtr& topologyInfo) = 0;
	

	/** ��ѯָ��Ҫ��ID��ָ�������������͵��������˽�㼯��
	* @param [in]		fids		Ҫ��ID����
	* @param [in]		type		�������˲�ѯ����
	* @param [out]	topologyInfos	���ص�����������Ϣ���󼯺�
	* @return  0 -- ��ʾʧ�ܣ�1 -- ��ʾ���سɹ�
	*/	
	virtual long queryTopos(const vector<FID>& fids,const  int &type, vector<TopoInfoSharedPtr>& topologyInfos) = 0;
	/** ��ȡ��������м����ڲ�����  
	* @param  [in] fid ����ȡ�������ID
	* @param  [out] levelSet �����𼯺�
	* @return true  �ɹ�,false ʧ��
	*/
	virtual bool getAllTextureLevel(const TEXID &texid,std::set<int> &levelSet) = 0;



	virtual void getSharedModelIDs(GeometrySharedPtr p,std::vector<MDLID> & )=0;
	virtual void getMaterilIDs( GeometrySharedPtr pGeom,std::vector<MATID> &)=0;
	/** @} */ 

	/** @name ������Ա����˵��  
	*  @{
	*/
protected:      

	/** ��ѯָ����Χ���ڵ����в��ڻ�����Ķ��� 
	* @param  [in] bound Ҫ��ѯ�İ�Χ��
	* @param  [in] fcid Ҫ��ӵ�FeatureClass����ָ�����
	* @param  [out] objs ��ѯ��������Ҫ�ض���
	* @param  [in] readProps �Ƿ��ȡ����
	* @return ���ִ�гɹ��򷵻�true,���򷵻�false
	*/
	virtual bool queryByBound(const Envelope3dRef bound, 	
		const std::vector<FCLSID> & fcid,
		std::vector<FeatureSharedPtr> & objs,const bool readProps = true)=0;

	virtual double  getDiagonal(double d[]);
	/** @} */ 
};
/** @} */  


/** @addtogroup proxy QResult-��ѯ���,�������Proxy�ڲ����д������ⲿ���ܴ���,ֻ��ʹ����ָ��
*  @{
*/
class GV3DCORE_API QResult{	
	/** @name ˽�г�Ա����˵��  
	*  @{
	*/
	/** ��Proxy */
	Proxy * _proxy;
    /** @} */ 

protected:
	/** @name ������Ա����˵��  
	*  @{
	*/
	/** ���ڻ����С���ƣ�ʣ��Ĳ�ѯ��Ҫ�ض���ID���� */
	std::vector<pair<FID,FCLSID> > _remainder;
	/** ��ǰ����ֵ */
	size_t                         _cur_index;
	/** ���ص�Ҫ�ض����б� */
	std::vector<FeatureSharedPtr>  _result;
	/** @} */ 
public:
	/** @name �������������� 
	*  @{
	*/
	QResult(Proxy * p){
		_proxy = p;	
		_cur_index=-1;
	}
	~QResult(){
		_proxy = NULL;
		_cur_index=-1;
	}
	/** @} */

	/** @name ������е����к���  
	*  @{
	*/	  
	/** �����ȡ����ȡ������
	* @param  ��
	* @return ��ȡ��Ҫ�ض���
	*/
	FeatureSharedPtr next(){
		size_t s1=_result.size();
		size_t s2 = _remainder.size();
		FeatureSharedPtr feaSptr;
		if(s1==0 && s2 ==0) 
			return feaSptr;
		_cur_index++;

		if(_cur_index<s1){//���ڴ���
			return _result[_cur_index];
		}
		else if(_cur_index>=s1 && _cur_index<s1+s2){
			std::vector<pair<FID,FCLSID> >::iterator  _itrCur = _remainder.begin()+(_cur_index-s1);
			//�������������ָ��������
			_proxy->queryByFeatureID(_itrCur->first, _itrCur->second, feaSptr);
			return 			feaSptr;
		}
		else{
			return feaSptr;
		}
	}

	/** ��ȡ����ȡ���ݵ����ݴ�С
	* @param  ��
	* @return ��ȡ��Ҫ�ض������ݵĴ�С
	*/
	inline size_t size(){return _result.size()+_remainder.size();}

	/** ��ս����
	* @param  ��
	* @return ��
	*/
	inline void clear(){
		_remainder.clear();
		_result.clear();
		_cur_index=-1;
	}

	/** �����Ѿ���ȡ�����ݣ��ڲ�����
	* @param  [in] ResultIn ��ѯ����Ҫ�ض��󼯺�
	* @return ��
	*/
	void setResult(std::vector<FeatureSharedPtr> & ResultIn){ 
		//�������ǰ������  [sunka, 2009-9-22]
		_result.clear();
		_result.resize(ResultIn.size());
		//����������
		std::copy(ResultIn.begin(),ResultIn.end(),_result.begin());
	}

	
	/** ��Ӵ���ȡ�����ݣ��ڲ�����  
	* @param  [in] fid ����ȡҪ�ض���ID
	* @param  [in] fclsid ����ȡҪ����ID
	* @return ��
	*/
	void addRemainder(FID fid,FCLSID fclsid){
		_remainder.push_back(pair<FID,FCLSID>(fid,fclsid));
	};
	
public:
	/** ��Ԫ��proxy */
	friend class Proxy;
	/** @} */ 
};
typedef std::shared_ptr<QResult> QResultSharedPtr;
/** @} */ 
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
