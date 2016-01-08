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
#include "terrainproxy.h"



begin_gtl_namespace
begin_gdb_namespace

class  TerrainBuilder;
typedef std::shared_ptr<TerrainBuilder>  TerrainBuilderSharedPtr;
/** @addtogroup terrain TerrainBuilder-�������ݿ����ָ�롡
*  @{
*/
class CORE_API TerrainBuilder
{
protected:
	/** ��DEM���ݿ�
	* @param [in]  dbinfo		DEM���ݿ��¼��Ϣ���ļ�ϵͳΪ�ļ���·�������ݿ�ϵͳΪ��¼���û���¼�����ݿ����Ƶȣ�
	* @return
	*/
	virtual bool openDEMDB(const Connector & conn) = 0;


	/** ��DOM���ݿ�
	* @param [in]  dbinfo		DOM���ݿ��¼��Ϣ���ļ�ϵͳΪ�ļ���·�������ݿ�ϵͳΪ��¼���û���¼�����ݿ����Ƶȣ�
	* @return
	*/
	virtual bool openDOMDB(const Connector & conn) = 0;


	/** �ر�DEM���ݿ�
	* @param [in]  demid		DEM���ݿ�ID
	* @return
	*/
	virtual bool closeDEMDB() = 0;


	/** �ر�DOM���ݿ�
	* @param [in]  domid		DOM���ݿ�ID
	* @return
	*/
	virtual bool closeDOMDB() = 0;

	/** ����DEM���ݿ�
	* @param [in]  demDBName 		DEM���ݿ�����
	* @param [in]  demDatainfo 		DEM����������Ϣ
	* @param [in]  scope 			DEM���ݿⷶΧ
	* @return
	*/
	virtual bool createDEMDB(const Connector & conn, DEMDBInfoSharedPtr demdbinfo) = 0;

	/** ����DOM���ݿ�
	* @param [in]  domDBName 		DOM���ݿ�����
	* @param [in]  domDatainfo 		DOM����������Ϣ
	* @param [in]  scope 			DOM���ݿⷶΧ
	* @return
	*/
	virtual bool createDOMDB(const Connector & conn, DOMDBInfoSharedPtr domdbinfo) = 0;

	/** ����DEM�½�������
	* @param [in]  nLayerID			DEM��������ID
	* @param [in]  szLayerName		DEM������
	* @param [in]  LayerInfo		DEM����Ϣ
	* @return
	*/
	virtual bool newDEMLayer(Identifier::raw_type  nLayerID, DEMLayerInfoSharedPtr LayerInfo, int indexPosition = -1) = 0;


	/** ����DOM�½�������
	* @param [in]  nLayerID			DOM��������ID
	* @param [in]  szLayerName		DOM������
	* @param [in]  LayerInfo		DOM����Ϣ
	* @return
	*/
	virtual bool newDOMLayer(Identifier::raw_type  nLayerID, DOMLayerInfoSharedPtr LayerInfo, int indexPosition = -1) = 0;

	/**���DEM���ݿ�
	* @param [in]  demid			DEM���ݿ�ID
	* @return
	*/
	virtual bool clearDEMDBData() = 0;

	/**���DOM���ݿ�
	* @param [in]  domdbid			DOM���ݿ�ID
	* @return
	*/
	virtual bool clearDOMDBData() = 0;

	/** ɾ��DEM���ݿ�
	* @param [in]  demid			DEM���ݿ�ID
	* @return
	*/
	virtual bool deleteDEMDB() = 0;

	/** ɾ��DOM���ݿ�
	* @param [in]  domid			DOM���ݿ�ID
	* @return
	*/
	virtual bool deleteDOMDB() = 0;



	/** ƽ��DEM���ݿ�
	* @param [in]	x		x����ƽ�Ƴ���
	* @param [in]	y		y����ƽ�Ƴ���
	* @return
	*/
	virtual bool translateDEMDB(double x, double y) = 0;

	/** ƽ��DOM���ݿ�
	* @param [in]	x		x����ƽ�Ƴ���
	* @param [in]	y		y����ƽ�Ƴ���
	* @return
	*/
	virtual bool translateDOMDB(double x, double y) = 0;


public:
	/**  �жϹ����Ƿ�Ϊ��
	* @param ��
	* @return ���Ϊ�շ���true�����򷵻�false
	*/
	virtual bool isEmpty() = 0;

	/** ��ȡ��ǰ���ID
	* @param [in]  ��
	* @return ��ǰ���ID
	*/
	virtual Identifier::raw_type getCurrentLayerID() = 0;

	/** ���õ�ǰ���ID
	* @param [in]  int layerID
	* @return
	*/
	virtual bool setCurrentLayerID(Identifier::raw_type layerID) = 0;

	/** �ر����ݿ�
	* @param [in]  ��
	* @return
	*/
	virtual bool close() = 0;

	/** �����ݿ�
	* @param [in]  Connector: conn
	* @return
	*/
	virtual bool open(const Connector &) = 0;

	/** �������ݿ�
	* @param [in]  Connector: conn
	* @return
	*/
	virtual bool create(const Connector &, DEMDBInfoSharedPtr &, DOMDBInfoSharedPtr &) = 0;
	/** ����DEM���ݿ�
	* @param [in]  Connector: conn
	* @return
	*/
	virtual bool create(const Connector &, DEMDBInfoSharedPtr &) = 0;

	/** ����DOM���ݿ�
	* @param [in]  Connector: conn
	* @return
	*/
	virtual bool create(const Connector &, DOMDBInfoSharedPtr &) = 0;

	/** ��ȡ���β�ѯָ�����
	* @return
	*/
	virtual TerrainProxySharedPtr getProxy() = 0;



	/** ��ȡDEM���ݿ���Ϣ
	* @param [out]  demdbinfo 		DEM���ݿ���Ϣ
	* @return
	*/
	virtual DEMDBInfoSharedPtr getDEMDBInfo() = 0;

	/** ��ȡDEM���ݿ���Ϣ
	* @param [out]  domdbinfo 		DOM���ݿ���Ϣ
	* @return
	*/
	virtual DOMDBInfoSharedPtr getDOMDBInfo() = 0;

	/** ɾ��һ����������
	* @param [in]  nLayerID			��������ID
	* @return
	*/
	virtual bool deleteLayer(Identifier::raw_type  nLayerID) = 0;


	/** �ύ����DEM�������ݿ�
	* @param [in]  blkdata			DEM����
	* @param [in]  bEnd				�Ƿ����һ�����ݿ�
	* @return
	*/
	virtual bool submitDEMBlock(DEMBlockSharedPtr blkdata) = 0;


	/** �ύ���DEM�������ݿ�
	* @param [in]  nLayerID			DEM��������ID
	* @param [in]  blkdatas			DEM���ݼ���
	* @return
	*/
	virtual bool submitDEMBlocks(Identifier::raw_type  nLayerID, std::vector<DEMBlockSharedPtr>& blkdatas) = 0;


	/** �ύDEM����
	* @param [in]  szFileName		��ǰ�ݽ���DEM�ļ�·�������ƣ����ڼ�¼ԭʼDEM������Դ
	* @param [in]  demObject		��ǰ�ݽ���DEM����
	* @param [in]  nUpdateType		�µݽ����������ԭ���������ص�ʱ�Ĵ���ʽ0:����Ϊ��ֵ ��1:������ֵ�ں�; 2���滻����������ֵ
	* @return
	*/
	virtual bool submitDEM(DEMGridSharedPtr demObj) = 0;

	/** �ύ����DOM�������ݿ�
	* @param [in]  blkdata			DOM����
	* @return
	*/
	virtual bool submitDOMBlock(DOMBlockSharedPtr blkdata) = 0;

	/** �ύ���DOM�������ݿ�
	* @param [in]  nLayerID			DOM��������ID
	* @param [in]  blkdatas			DOM���ݼ���
	* @return
	*/
	virtual bool submitDOMBlocks(Identifier::raw_type  nLayerID, std::vector<DOMBlockSharedPtr>& blkdatas) = 0;


	/** �ύDOM����
	* @param [in]  nLayerID			DOM��������ID
	* @param [in]  szFileName		��ǰ�ݽ���DOM�ļ�·��������
	* @param [in]  imageData			��ǰ�ݽ���DOM����ָ��
	* @param [in]  DomInfo			��ǰ�ݽ���DOM������Ϣ
	* @param [in]  scope			��ǰ�ݽ���DOM��Χ
	* @param [in]  nUpdateType		�µݽ����������ԭ���������ص�ʱ�Ĵ���ʽ0:����Ϊ��ֵ ��1:������ֵ�ں�; 2���滻����������ֵ
	* @return
	*/
	virtual bool submitDOM(DOMGridSharedPtr domObj) = 0;

	/** ����DEM���ݿⷶΧΪ������
	* @param [in]	srcDemScope			ԭʼDEM���ݿⷶΧ
	* @param [out]	adjustDemScope		�������DEM���ݿⷶΧ
	* @return
	*/
	virtual bool adjustDEMDBScopeToMinSquare(const Envelope2d& srcDemScope, const Envelope2d& adjustDemScope) = 0;

	/** ����DEM�ĸ�����࣬ʹDEM���ݿ�ı߳�������Ϊ2N+1
	* @param [in]	adjustDemScope		�������DEM���ݿⷶΧ
	* @param [in]	srcCellsize			ԭʼDEM���ݿ�������
	* @param [out]	adjustCellsize		�������DEM���ݿ�������
	* @return
	*/
	virtual bool adjustDEMCellSizeTo2N(const Envelope2d& adjustScope, double srcCellsize, double& adjustCellsize) = 0;

	/** ����DEM�ķ�Χ��ʹDEM���ݿ�ı߳�������Ϊ2N+1
	* @param [in]	srcCellsize			ԭʼDEM���ݿ�������
	* @param [in]	srcDemScope			�������DEM���ݿⷶΧ
	* @param [out]	adjustDemScope		�������DEM���ݿⷶΧ
	* @return
	*/
	virtual bool adjustDEMDBScopeTo2NSquare(double cellsize, const Envelope2d& srcDemScope, const Envelope2d& adjustDemScope) = 0;

	/** ����ָ���������㷶Χ
	* @param [in]  nLayerID			DEM��������ID
	* @param [out] demLayerScope	������ķ�Χ
	* @return
	*/
	virtual bool adjustLayerScope(Identifier::raw_type  nLayerID, const Envelope2d& demLayerScope, int& colNum, int& rowNum) = 0;
	/** �õ�connector
	* @return
	*/
	virtual  Connector & getConnector() = 0;
public:
	//�����ж�DEM��DOMĳ���Ƿ�������
	virtual bool isDOMLayerEmpty(Identifier::raw_type layid) = 0;
	virtual bool isDEMLayerEmpty(Identifier::raw_type layid) = 0;
	//����DEM������
	virtual bool createDEMPyramid() = 0;
	virtual bool createDOMPyramid() = 0;


	virtual double getZValue(Identifier::raw_type layerID, unsigned long long row, unsigned long long col) = 0;
	virtual void setZValue(Identifier::raw_type layerID, unsigned long long row, unsigned long long col, double z) = 0;

	virtual DEMLayerInfoSharedPtr findDEMLayerInfo(Identifier::raw_type layerID) = 0;
	virtual DOMLayerInfoSharedPtr findDOMLayerInfo(Identifier::raw_type layerID) = 0;

	virtual DEMBlockSharedPtr getDEMBlock(Identifier::raw_type layid, int rowBlock, int colBlock) = 0;
	virtual DOMBlockSharedPtr getDOMBlock(Identifier::raw_type layid, int rowBlock, int colBlock) = 0;
	virtual DEMBlockSharedPtr getDEMBlock(Identifier::raw_type layid, Identifier::raw_type blockid) = 0;
	virtual DOMBlockSharedPtr getDOMBlock(Identifier::raw_type layid, Identifier::raw_type blockid) = 0;
	/** ͨ�����ݿ����������ݿ��е��кź��кţ��������ݿ��ΨһID
	* @param [in]	block_row			���ݿ����������ݿ��е��к�
	* @param [in]	block_col			���ݿ����������ݿ��е��к�
	* @return ���ݿ��ΨһID
	*/
	static unsigned long long calculate(int block_row, int block_col)
	{
		unsigned long long id = 0;
		id = block_row;
		id = id << 32;
		id += block_col;
		return id;
	}
	/** ͨ�����ݿ��ΨһID���������ݿ����������ݿ��е��кź��к�
	* @param [in]	id      			���ݿ��ΨһID
	* @param [in]	block_row			���ݿ����������ݿ��е��к�
	* @param [in]	block_col			���ݿ����������ݿ��е��к�
	* @return
	*/
	static void calculate(unsigned long long id, int & block_row, int & block_col)
	{
		block_col = (int)id;
		block_row = (int)(id >> 32);
	}
	/** ͨ��ĳ�������������ݿ��еĵ�Ԫ���кź��кţ�ÿ�����ݿ�����ĵ�Ԫ���������������������������ݿ��ΨһID�����õ������ݿ��е��кź��к�
	* @param [in]	row      			ĳ�������ݿ����к�
	* @param [in]	col      			ĳ�������ݿ����к�
	* @param [in]	rowsPreblock      	ÿ�����ݿ�����ĵ�Ԫ������
	* @param [in]	colsPreblock      	ÿ�����ݿ�����ĵ�Ԫ������
	* @param [in,out]	block_id      		���ݿ��ΨһID
	* @param [in,out]	row_in_block		ĳ�������ݿ��е��к�
	* @param [in,out]	col_in_block		ĳ�������ݿ��е��к�
	* @return
	*/
	static void calculate(unsigned long long row, unsigned long long col,
		int rowsPreblock, int colsPreblock,
		unsigned long long & block_id, int & row_in_block, int & col_in_block)
	{
		int  block_row = (int)(row / (rowsPreblock - 1));
		int  block_col = (int)(col / (colsPreblock - 1));
		block_id = calculate(block_row, block_col);
		row_in_block = row % (rowsPreblock - 1);
		col_in_block = col % (colsPreblock - 1);
	}
	/** ͨ��ĳ���������ֵ���������������ݿ��еĵ�Ԫ���кź��к�
	* @param [in]	edb2d        	���ݿ�ķ�Χ
	* @param [in]	cal2d     	    ���ڼ��������ݿ�����ʼ����ֹ���еĺ���
	* @param [in]	cellSize      	��Ԫ���С
	* @param [int,out]	beg_row      			�����ݿ����к�
	* @param [int,out]	beg_col      			�����ݿ����к�
	* @param [int,out]	end_row      			�����ݿ����к�
	* @param [int,out]	end_col      			�����ݿ����к�
	* @return                           �ɹ�����true���򷵻�false
	*/
	static bool  calculate(Envelope2d &edb2d, Envelope2d &cal2d, int cellSize, int & beg_row, int & beg_col,
		int & end_row, int & end_col){
		if (Envelope2d::test(cal2d, edb2d) == 0)
			return false;
		else{
			beg_col = (int)((cal2d.minx - edb2d.minx) / cellSize);
			beg_row = (int)((cal2d.miny - edb2d.miny) / cellSize);
			end_col = (int)((cal2d.maxx - edb2d.minx) / cellSize);
			end_row = (int)((cal2d.maxy - edb2d.miny) / cellSize);
			return true;
		}
	}
};
/** @} */


end_gdb_namespace
end_gtl_namespace


