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
#include "buffer.h"
#include "geometry.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

class TopoInfo;
class TopoItem;

typedef std::shared_ptr<TopoItem>  TopoItemSharedPtr;
typedef std::shared_ptr<TopoInfo>  TopoInfoSharedPtr;
/** @defgroup topology SEMAN_TOPO_TYPE-�������˹�ϵ���� 
*  @{
*/
enum SEMAN_TOPO_TYPE
{
	SEMANTOPOTYPE_COPOSITION = 1,			//��ɹ�ϵ
	SEMANTOPOTYPE_CONNECTIVITY,			//��ͨ��ϵ
	SEMANTOPOTYPE_INCLUSION,			//������ϵ
	SEMANTOPOTYPE_SYNTOPY,				//�ڽӹ�ϵ
	SEMANTOPOTYPE_ASSOCIATION,			//������ϵ
	SEMANTOPOTYPE_INTERSECTION,			//�ཻ��ϵ
};
/** @} */ 
/** @addtogroup topology TopoItem-A - B�������˹�ϵ 
*  @{
*/
class CORE_API TopoItem{
public:
	virtual int getType() const =0;
	virtual void setType(int t)=0;

	virtual Identifier::raw_type getAFeatureClassID() const =0;
	virtual Identifier::raw_type getBFeatureClassID() const =0;
	virtual Identifier::raw_type getAFeatureID() const =0;
	virtual Identifier::raw_type getBFeatureID() const =0;

	virtual void setAFeatureClassID(Identifier::raw_type )=0;
	virtual void setBFeatureClassID(Identifier::raw_type)=0;
	virtual void setAFeatureID(Identifier::raw_type)=0;
	virtual void setBFeatureID(Identifier::raw_type)=0;

	virtual std::pair<FID,FCLSID> getAItem()=0;
	virtual std::pair<FID,FCLSID> getBItem()=0;


	virtual void setAItem(std::pair<FID,FCLSID> & )=0;
	virtual void setBItem(std::pair<FID,FCLSID> & )=0;

	virtual bool copy(const TopoItem & tt)=0;
	virtual bool isEqual(const TopoItem & tt)=0;

	static TopoItemSharedPtr newTopoItem();
	static TopoItemSharedPtr newTopoItem( int topoType, FID fidMain, FCLSID fclsidMain, FID fidSub, FCLSID fclsidSub);
};
/** @} */ 
/** @addtogroup topology TOPO_INFO_TYPE-A-B����������Ϣ���� 
*  @{
*/
enum TOPO_INFO_TYPE
{
	TOPOINFOTYPE_SEMATIC_GEOMETRY = 1,	//���弸��
	TOPOINFOTYPE_SEMATIC_ACTION,				//���嶯��
};
/** @} */ 
/** @addtogroup topology TopoInfo 
*  @{
*/
class  CORE_API TopoInfo
{
public:
	virtual int getType() const=0;
	virtual void setType( int type)=0;

	virtual Identifier::raw_type getFeatureClassID() const =0;
	virtual Identifier::raw_type getFeatureID() const =0;

	virtual void setFeatureClassID(Identifier::raw_type )=0;
	virtual void setFeatureID(Identifier::raw_type)=0;

	virtual std::pair<FID,FCLSID> getOriginalItem() =0;
	virtual void setOriginalItem( std::pair<FID,FCLSID> & ) =0;

	virtual size_t sizeBuffer() = 0;
	virtual bool readBuffer( Buffer & buf) = 0;
	virtual bool writeBuffer(Buffer& buf) = 0;
	virtual bool copy(const TopoInfo & topoInfo)=0;
	virtual void write(std::ostream & f) = 0;
	virtual void read(std::istream & f) = 0;
public:

	/** ��̬�������ڴ���ָ�����͵����˶���
	*  ���ݴ���Ķ������ͣ��½�һ����Ӧ����������Ϣ��ָ�룬�����ٿռ�
	* @param [in] signal	
	* -1	TOPOINFOTYPE_SEMATIC_GEOMETRY,			//���弸��
	* -2	TOPOINFOTYPE_SEMATIC_ACTION,			//���嶯��
	*/
	static TopoInfoSharedPtr newTopoInfo(int signal); 

	/** �½����˶��󣨸���Buffer�е�typeʵ�ֶ���ָ��Ĵ������ڴ�Ŀ��٣�
	*��Buffer�ж�ȡ���ݣ��½�һ�����˶���,����readBufferʵ�ָ����ڴ������������Ŀ���
	* @return �����ָ��
	*/
	static TopoInfoSharedPtr newTopoInfo( Buffer & buf);

};
typedef std::shared_ptr<TopoInfo> TopoInfoSharedPtr;
typedef std::shared_ptr<TopoItem> TopoItemSharedPtr;
/** @} */ 
/** @addtogroup topology SemanticGeometryTopoInfo 
*  @{
*/
class CORE_API SemanticGeometryTopoInfo: virtual public TopoInfo {
public:
	/** ��ö����������Ϣ	
	* @param 
	* @return �����������Ϣ	
	*/
	virtual string getSemanticName() const=0;

	/** ���ö����������Ϣ	
	* @param name �����������Ϣ
	* @return 	
	*/
	virtual void setSemanticName(const string name)=0;

	/** ���ö�������˼���
	* @param [in]  geo ��������˼���
	* @return void
	*/
	virtual void setGeometry( GeometrySharedPtr   geo)=0;

	/** ��ö�������˼���
	* @param 
	* @return ��������˼���
	*/
	virtual GeometrySharedPtr getGeometry() =0;
};
/** @} */ 
/** @addtogroup topology SemanticActionTopoInfo 
*  @{
*/
class CORE_API SemanticActionTopoInfo: virtual public TopoInfo{
	virtual std::string getName() =0;
};
/** @} */ 
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
