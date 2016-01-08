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
#include "bitset.h"
#include "buffer.h"
#include "material.h"
#include "matrix4x4.h"
#include "indexbuffer.h"
#include "meshproxy.h"



begin_gtl_namespace
begin_gdb_namespace

class MeshBuilder;
typedef std::shared_ptr<MeshBuilder> MeshBuilderSharedPtr;
/** @addtogroup Mesh MeshBuilder-MeshBuilder�������ɹ���
*  @{
*/
class CORE_API MeshBuilder {
protected:
	/** ��ѯ�ӿ�ָ�� */
	MeshProxySharedPtr _proxySptr;
	/** �ֲ��� */
	unsigned long long _layerNumber;
	/** X��Y��Z������ÿ�����еĵ�Ԫ���� */
	unsigned long long _cellNumberPreBlock[3];

	Envelope3d   _totalEnvelope;

	unsigned long long         _propNumberPreCell;//ÿ����Ԫ�����Ը���
	std::vector<std::string>   _propNames;//ÿ�����Ե����ƣ�������Ԫ�ظ�������_propNumberPreCell
	std::vector<double>        _propRanges;//��¼ÿ�����Ե���Сֵ�����ֵ������ĸ���Ϊ2*_propNumberPreCell;

protected://ת������ʾ����ר�õ�һЩ����
	int                        _indexOfProp4Color;//�����ĸ�������ȷ����ɫ 
	ColorSeries                _colorSeries;//��ʾ�����õ�ɫϵ 
public:
	virtual bool open(const Connector& conn) = 0;
	virtual bool create(const Connector& conn) = 0;
	virtual bool append(MeshSharedPtr msp) = 0;//��������������ݿ�
	virtual void close() = 0;
	/**  �жϹ����Ƿ�Ϊ��
	* @param ��
	* @return ���Ϊ�շ���true�����򷵻�false
	*/
	virtual bool isEmpty() = 0;

	virtual MeshProxySharedPtr getProxy() = 0;
	virtual BlockSharedPtr getBlock(const Identifier & id) = 0;
	virtual void setup(std::vector<MeshSharedPtr> & msps,
		unsigned long long xCellNumberPreBlock, unsigned long long yCellNumberPreBlock, unsigned long long zCellNumberPreBlock,
		unsigned long long layerNumber) = 0;
	virtual Envelope3d & calculateEnvelope() = 0;

public:
	MeshBuilder();

	inline unsigned long long getLayerNumber() { return _layerNumber; }
	inline unsigned long long getCellNumberPreBlock(unsigned long long & x, unsigned long long & y, unsigned long long & z) {
		x = _cellNumberPreBlock[0];
		y = _cellNumberPreBlock[1];
		z = _cellNumberPreBlock[2];
		return x*y*z;
	}
	inline void getPropRange(const int i, double & minz, double & maxz){
		minz = _propRanges[i * 2];
		maxz = _propRanges[i * 2 + 1];
	}
	virtual Envelope3d & getEnvelope(){ return _totalEnvelope; }


};
/** @} */


end_gdb_namespace
end_gtl_namespace


