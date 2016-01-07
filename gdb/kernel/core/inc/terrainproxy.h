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
#include "material.h"
#include "matrix4x4.h"
#include "indexbuffer.h"
#include "connector.h"
#include "terrain.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace



class TerrainProxy;
typedef std::shared_ptr<TerrainProxy>  TerrainProxySharedPtr;

//////////////////////////////////////////////////////////
/** @addtogroup terrain TerrainProxy-���β�ѯ�ӿڡ�
*  @{
*/
class CORE_API TerrainProxy{
public:
	/** ��ȡָ����������;��η�Χ��DEM����
	* @param [in]  box2d			DEM�������㷶Χ
	* @param [in]  nLayerID			DEM�������Ĳ�ID
	* @param [out]  pDemBlocks		DEM�����������ݿ�����
	* @return
	*/
	virtual bool extractDEM(const Envelope2d& box2d, Identifier::raw_type  nLayerID, DEMGridSharedPtr & demObj) = 0;

	/** ��ȷ��ȡָ����������;��η�Χ��DOM����(Ӱ������)
	* @param [in]	box2d			DOM�������㷶Χ
	* @param [in]	nLayerID		DOM�������Ĳ�ID
	* @param [out]  domObj			dom����
	* @return
	*/
	virtual bool extractDOM(const Envelope2d& box2d, Identifier::raw_type  nLayerID, DOMGridSharedPtr & domObj) = 0;

	/** ͨ����ID���кź��кŻ�����ݿ�ID
	* @param [in]	nLayerID		��ID
	* @param [in]	nRow			�к�
	* @param [in]	nCol			�к�
	* @param [out]  blkID			���ݿ�ID
	* @return
	*/
	virtual bool reverseRowColtoBlockID(Identifier::raw_type  nLayerID, int nRow, int nCol, Identifier::raw_type & blkID) = 0;

	/** ͨ�����ݿ�ID��ò�ID���кź��к�
	* @param [in]	blkID			���ݿ�ID
	* @param [out]	nLayerID		��ID
	* @param [out]	nRow			�к�
	* @param [out]  nCol			�к�
	* @return
	*/
	virtual bool reverseBlockIDtoRowCol(Identifier::raw_type blkID, Identifier::raw_type& nLayerID, int& nRow, int& nCol) = 0;

	/** ��ȡָ����ID�ͷ�Χ�����ݿ�ID
	* @param [in]	nLayerID		��ID
	* @param [in]	box2d			��Χ
	* @param [out]  blkIDs			DEM���ݿ�ID����
	* @param [in]	nDataType		DEM����DOM;0:DEM;1:DOM
	* @param [in]	nType			��ȡ����:0:��ȡ��ȫ��BOX�ڲ������ݿ飻1�������ཻ�����ݿ�
	* @return
	*/
	virtual bool getBlockIDsByScope(Identifier::raw_type  nLayerID, const Envelope2d & box2d, std::vector<Identifier::raw_type>& blkIDs, int nDataType, int nType) = 0;

	/** ��õ�����߷ֱ���
	* @param [out]  fRes		������߷ֱ���
	* @param [in]	nDataType	DEM����DOM;0:DEM;1:DOM
	* @return
	*/
	virtual bool getHighestResolution(double& fRes, int nDataType) = 0;

	/** ��õ�����ͷֱ���
	* @param [out]  fRes		DEM��ͷֱ���
	* @param [in]	nDataType	DEM����DOM;0:DEM;1:DOM
	* @return
	*/
	virtual bool getLowestResolution(double& fRes, int nDataType) = 0;

	/** ���DEM���ݿ��������С�߳�
	* @param [out]  fMax		������߷ֱ���
	* @param [out]  fLowestRes	������߷ֱ���
	* @param [in]	nDataType	DEM����DOM;0:DEM;1:DOM
	* @return
	*/
	virtual bool getDEMBlockMaxAndMinZ(Identifier::raw_type blkid, float& blkMaxZ, float& blkMinZ) = 0;

	/** ��õ��δֱַ��ʵ����ݿ�ID����
	* @param [out]  blkIDs		DEM/DOM���ݿ�ID����
	* @param [int]  nDataType	DEM����DOM;0:DEM;1:DOM
	* @return
	*/
	virtual bool getPyramidTopBlockIDs(vector<Identifier::raw_type>& blkIDs, Identifier::raw_type & nLayerID, int nDataType) = 0;


	/** ��ȡָ�����������DEM���ݣ�DemBlock �����ں����ڲ�ͨ�� new ���������ϲ㺯��������ά���������ͷ�
	* @param [in]  nLayerID			DEM��������ID
	* @param [out]  pDemBlocks		DEM�����������ݿ����ݼ���
	* @return
	*/
	virtual bool getDEMDataByLayerID(Identifier::raw_type  nLayerID, vector<DEMBlockSharedPtr>& pDemBlocks) = 0;


	/** ��ȡָ�����ݿ�ID��DEM���ݣ�DemBlock �����ں����ڲ�ͨ�� new ���������ϲ㺯��������ά���������ͷ�
	* @param [in]	blkID		���ݿ�ID
	* @param [out]  pDemBlock	DEM�����������ݿ�����
	* @return
	*/
	virtual bool getDEMDataByBlockID(Identifier::raw_type blkID, DEMBlockSharedPtr& pDemBlock) = 0;

	/** ��ȡָ�����ݿ�ID��ָ��������DEM���ݣ�DemHeight �����ں����ⲿͨ�� new ���������ϲ㺯��������ά���������ͷ�
	* @param [in]	blkID		���ݿ�ID
	* @param [in]	nStartIndex		��ʼ������
	* @param [in]	nLength		��������
	* @param [out]  pDemHeight	DEM�������ĸ߳�����
	* @return
	*/
	virtual bool getDEMDataByIndex(Identifier::raw_type blkID, int nStartIndex, int nLength, float*& pDemHeight) = 0;

	/** ��ȡָ��������������ݿ�ID��DEM���ݣ�DemBlock �����ں����ڲ�ͨ�� new ���������ϲ㺯��������ά���������ͷ�
	* @param [in]	nLayerID	��ID
	* @param [in]	blkIDs		���ݿ�ID
	* @param [out]  pDemBlocks	DEM�����������ݿ�����
	* @return
	*/
	virtual bool getDEMDataByBlockIDs(Identifier::raw_type  nLayerID, vector<Identifier::raw_type> blkIDs, vector<DEMBlockSharedPtr>& pDemBlocks) = 0;


	/** ��ȡָ����������;��η�Χ��DOM���ݣ�ImageBlock �����ں����ڲ�ͨ�� new ���������ϲ㺯��������ά���������ͷ�
	* @param [in]  box2d			DOM�������㷶Χ
	* @param [in]  nLayerID			DOM�������Ĳ�ID
	* @param [out]  pImageBlocks	DOM�����������ݿ�����
	* @param [in]  nType			�������Ķ�ȡ����
	* @return
	*/
	virtual bool getDOMDataByBox(const Envelope2d & box2d, Identifier::raw_type  nLayerID, vector<DOMBlockSharedPtr>& pDomBlocks, int nType) = 0;


	/** ��ȡָ�����������DOM���ݣ�DomBlock �����ں����ڲ�ͨ�� new ���������ϲ㺯��������ά���������ͷ�
	* @param [in]  nLayerID			DOM��������ID
	* @param [out]  pDomBlocks		DOM�����������ݿ����ݼ���
	* @return
	*/
	virtual bool getDOMDataByLayerID(Identifier::raw_type  nLayerID, vector<DOMBlockSharedPtr>& pDomBlocks) = 0;

	//##ModelId=4A15463701CD
	/** ��ȡָ��������������ݿ�ID��DOM���ݣ�ImageBlock �����ں����ڲ�ͨ�� new ���������ϲ㺯��������ά���������ͷ�
	* @param [in]	blkID		DOM���ݿ�ID
	* @param [out]  pImageBlock	DOM�����������ݿ�����
	* @return
	*/
	virtual bool getDOMDataByBlockID(Identifier::raw_type blkID, DOMBlockSharedPtr& pDomBlock) = 0;

	/** ��ȡָ��������������ݿ�ID��DEM���ݣ�DemBlock �����ں����ڲ�ͨ�� new ���������ϲ㺯��������ά���������ͷ�
	* @param [in]	nLayerID	��ID
	* @param [in]	blkIDs		���ݿ�ID
	* @param [out]  pDemBlocks	DOM�����������ݿ�����
	* @return
	*/
	virtual bool getDOMDataByBlockIDs(Identifier::raw_type  nLayerID, vector<Identifier::raw_type> blkIDs, vector<DOMBlockSharedPtr>& pDomBlocks) = 0;

	/** �õ��ϱߵ����ݿ���
	* @param [in]	nCurBlkID			��ǰ���ݿ�ID
	* @param [out]  nUpperBlkID			�ϱ����ݿ�ID
	* @param [int]  nDataType			DEM����DOM;0:DEM;1:DOM
	* @return
	*/
	virtual bool getTopBlockID(Identifier::raw_type nCurBlkID, Identifier::raw_type& nUpperBlkID, int nDataType) = 0;

	/** �õ��±ߵ����ݿ���
	* @param [in]	nCurBlkID			��ǰ���ݿ�ID
	* @param [out]  nLowerBlkID			�±ߵ����ݿ�ID
	* @param [int]  nDataType			DEM����DOM;0:DEM;1:DOM
	* @return
	*/
	virtual bool getBottomBlockIDs(Identifier::raw_type nCurBlkID, vector<Identifier::raw_type>& nLowerBlkIDs, int nDataType) = 0;


	/** �õ��ұߵ����ݿ���
	* @param [in]	nCurBlkID			��ǰ���ݿ�ID
	* @param [out]  nRightBlkID			�ұߵ����ݿ�ID
	* @param [int]  nDataType			DEM����DOM;0:DEM;1:DOM
	* @return
	*/
	virtual bool getRightBlockID(Identifier::raw_type nCurBlkID, Identifier::raw_type& nRightBlkID, int nDataType) = 0;


	/** �õ���ߵ����ݿ���
	* @param [in]	nCurBlkID			��ǰ���ݿ�ID
	* @param [out]  nLeftBlkID			��ߵ����ݿ�ID
	* @param [int]  nDataType			DEM����DOM;0:DEM;1:DOM
	* @return
	*/
	virtual bool getLeftBlockID(Identifier::raw_type nCurBlkID, Identifier::raw_type& nLeftBlkID, int nDataType) = 0;


	/** �õ��������ݿ���
	* @param [in]	nCurLyrID			��ǰ���ݿ����ڲ�ID
	* @param [in]	nCurBlkID			��ǰ���ݿ�ID
	* @param [in]	nChildIDs			��ǰ���ݿ�ĺ������ݿ�ID
	* @param [out]  nChildLyrID			���ӵ����ݿ����ڲ�ID
	* @param [int]  nDataType			DEM����DOM;0:DEM;1:DOM
	* @return
	*/
	virtual bool getChildBlockID(Identifier::raw_type nCurLyrID, Identifier::raw_type nCurBlkID, vector<Identifier::raw_type>& nChildIDs, Identifier::raw_type& nChildLyrID, int nDataType) = 0;

	/** �õ��������ݿ���
	* @param [in]	nCurLyrID			��ǰ���ݿ����ڲ�ID
	* @param [in]	nCurBlkID			��ǰ���ݿ�ID
	* @param [in]	nParentID			��ǰ���ݿ�ĸ������ݿ�ID
	* @param [out]  nParentLyrID		���׵����ݿ����ڲ�ID
	* @param [int]  nDataType			DEM����DOM;0:DEM;1:DOM
	* @return
	*/
	virtual bool getParentBlockID(Identifier::raw_type nCurLyrID, Identifier::raw_type nCurBlkID, Identifier::raw_type& nParentID, Identifier::raw_type& nParentLyrID, int nDataType) = 0;


	/** �õ��������Ĳ���
	* @param [in]	nLayerNum			��ȡ���ܽ���������
	* @param [in]	nDataType			DEM����DOM����: 0 - DEM,1 - DOM
	* @return
	*/
	virtual bool getPyramidLayerNumber(int& nLayerNum, int nDataType) = 0;

	//##ModelId=4A1546370205
	/** �õ�DEM�����������Ϣ
	* @param [in]	nLayerID		��ȡ���ܽ���������
	* @param [in]	m_DemLayerInfo	�������������
	* @return
	*/
	virtual bool getDEMLayerInfo(Identifier::raw_type  nLayerID, DEMLayerInfoSharedPtr& demLayerInfo) = 0;

	//##ModelId=4A1546370208
	/** �õ�DOM�����������Ϣ
	* @param [in]	nLayerID		��ȡ���ܽ���������
	* @param [in]	m_DomLayerInfo	DOM�������������
	* @return
	*/
	virtual bool getDOMLayerInfo(Identifier::raw_type  nLayerID, DOMLayerInfoSharedPtr& domLayerInfo) = 0;

	//##ModelId=4A154637020B
	/** �õ�DEM���ݿ���Ϣ
	* @param [out]	demDBInfo		DEM���ݿ���Ϣ
	* @return
	*/
	virtual bool getDEMDBInfo(DEMDBInfoSharedPtr & demDBInfo) = 0;

	//##ModelId=4A154637020D
	/** �õ�DOM���ݿ���Ϣ
	* @param [out]	domDBInfo		DOM���ݿ���Ϣ
	* @return
	*/
	virtual bool getDOMDBInfo(DOMDBInfoSharedPtr& domDBInfo) = 0;


	/** ��ȡָ�����ݿ�ID��ָ��������DEMԤ�������ݵ�������ݣ�pErrorData �����ں����ⲿͨ�� new ���������ϲ㺯��������ά���������ͷ�
	* @param [in]	blkID				���ݿ�ID
	* @param [in]	nStartIndex			��ʼ������
	* @param [in]	nLength				��������
	* @param [out]  pErrorData			DEMԤ�������ݵ��������
	* @return
	*/
	virtual bool getErrorDataByIndex(Identifier::raw_type blkID, int nStartIndex, int nLength, float*& pErrorData) = 0;

	/** ��ȡָ�����ݿ�ID��Ԥ�������ݵ�boundingbox���ݣ�pBoundingboxData �����ں����ⲿͨ�� new ���������ϲ㺯��������ά���������ͷ�
	* @param [in]	blkID				���ݿ�ID
	* @param [out]  pBoundingboxData	Ԥ�������ݵ�boundingbox����
	* @return
	*/
	virtual bool getBoundingBoxData(Identifier::raw_type blkID, float*& pBoundingboxData) = 0;

	/** �õ��ύԴ�ļ������Ϣ
	* @param [out]	srcInfo		Դ�ļ������Ϣ
	* @param [in]	nDataType	��������
	* @return
	*/
	virtual bool getSourceData(vector<SourceDataInfoSharedPtr>& srcInfo, int nDataType) = 0;

	/** ���չ̶��ֱ��ʺͷ�Χ�ַ�DEM����
	* @param [in]	dfCellSize  �̶��ֱ���
	* @param [in]	szFullPathName	·��
	* @param [in]	Scope	�ַ���Χ
	* @return
	*/
	virtual bool queryDEMFile(double dfCellSize, char* szFullPathName, const Envelope2d&  Scope) = 0;

	/** ���չ̶��ֱ��ʺͷ�Χ�ַ�DOM����
	* @param [in]	dfResolution  �̶��ֱ���
	* @param [in]	szFullPathName	·��
	* @param [in]	Scope	�ַ���Χ
	* @param [in]	nBitCount	Ӱ��λ��
	* @return
	*/
	virtual bool queryDOMFile(double dfResolution, char* szFullPathName, const Envelope2d&  Scope, int nBitCount) = 0;
	/** ���շ�Χ��ѯ�÷�Χ�������ٿ�DEMBlock��DOMBlock,ֻҪ��ķ�Χ��box�ཻ
	* @param [in]	box  ��ѯ��Χ
	* @param [in]	demblocks	DEM��,���ݿ���ڲ��ֿ�
	* @param [in]	domblocks	DOM�飬���ݿ���ڲ��ֿ�
	* @return
	*/
	virtual bool queryByBox(const Envelope3d& box, std::vector<DEMBlockSharedPtr> &demblocks) = 0;
	virtual bool queryByBox(const Envelope3d& box, std::vector<DOMBlockSharedPtr> &domblocks) = 0;
	virtual bool queryByBox(const Envelope3d& box, std::vector<DEMBlockSharedPtr> &demblocks,
		std::vector<DOMBlockSharedPtr> &domblocks) = 0;

	virtual Envelope3d getEnvelope() = 0;
};
/** @} */


end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
