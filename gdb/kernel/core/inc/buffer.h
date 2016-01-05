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

#include <iosfwd>
#include <ios>
#include <istream>
#include <ostream>
#include <string>    // char traits            
#include <cstddef>   // ptrdiff_t
#include <cmath>
#include "assert.h"

#include "bufferstream.h" 

#include "color.h"
#include "vertex2d.h"
#include "vertex3f.h"
#include "vertex3d.h"
#include "vertex4d.h"
#include "vertexvisitor.h"
#include "vertexcollection2d.h"
#include "sphere3d.h"
#include "globalfuns.h"
#include "quaternion.h"
#include "matrix4x4.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

/** @defgroup Buffer Buffer-�ڴ滺����������
*  Buffer��һ���ڴ滺����������,��Ҫ���ڱ�ʾһ���ڴ�顣�ڿռ���Ϣ��������Ҫ�����õ����ࡣ
* ����ʹ�����IBufferIOһ�𹹽��˶����ڴ滺�����Ķ�д���ơ�������Ҫд�����ݿ�Ķ��󣬾���Ҫʵ��IBufferIO��
* �����м��ζ���Geometry��Ҫʵ��IBufferIO�������ݿ���Geometry�Ķ�д������������:
* Buffer buf;
* Geometry g;
* g.writeBuffer(buf);
* writeBlob(buf);//д�����ݿ�
* �������̣�
* buf.clear();
* readBlob(buf);
* g.readBuffer(buf);  
*  @{
*/
class CORE_API Buffer{
protected:
	/*breif ָ���ڴ��ͷ��ַ */
	void * _data;
	/*breif ʹ�ù����ڴ���С */
	size_t _size;
	/*breif �ڴ���С */
	size_t _capacity;
	/*breif �ڴ�������С */
	size_t _growth;
	/*breif �Ƿ���Buffer���Լ�������ڴ棬�������Ϊ�棬��֮Ϊ�� */
	bool _alloc;
	/*breif ��ǰλ�ã�Ĭ��Ϊ0 */
	size_t _curpos;
public:
	/** ��ȡ�ڴ���ͷ��ַ
	* @return �ڴ���ͷ��ַָ��
	*/
	inline  void const *  data() const {return _data;}
	/** ��ȡ�ڴ���ͷ��ַ
	* @return �ڴ���ͷ��ַָ��
	*/
	inline void *  data(){return _data;}
	/** ��ȡ�ڴ���С
	* @return �ڴ��Ĵ�С����λΪ�ֽ�
	*/
	inline size_t size() const {return _size;}
public:
	/** Ĭ�Ϲ��캯��
	* @return ��
	*/
	Buffer(size_t size=0,size_t growth=512);
	
	/** ���Buffer
	* @return ��
	*/
	void clear();
	/** Ĭ����������
	* @return ��
	*/
	~Buffer();
	/** ���캯�������allocΪ�棬���������һ���СΪsize���ڴ�飬������data�����ݣ���֮����ָ��data
	* @param [in] data �ڴ����׵�ַָ��  
	* @param [in] size ����д������ݴ�С
	* @param [in] alloc �Ƿ���Ҫ�����µ��ڴ�
	* @return ��
	*/
	Buffer(void * data,size_t size,bool alloc=false,size_t growth=512);
	/** �������캯��
	* @return ��
	*/
	Buffer(const Buffer & b);
	/** �ȺŸ�ֵ����
	* @return ��
	*/
	Buffer operator = (const Buffer & b);
	/** /����Buffer���������b��_alloc��ԱΪ�٣��򿽱����Buffer������b����һ���ڴ�飬����Ϊ�ڴ�鿽��
	* @param [in] buf Buffer 
	* @return ��
	*/
	void copy(const Buffer & buf);	
	/** //���û����������allocΪ�棬���������һ���СΪsize���ڴ�飬������data�����ݣ���֮����ָ��data
	* @param [in] data �ڴ����׵�ַָ��  
	* @param [in] size ����д������ݴ�С
	* @param [in] alloc �Ƿ���Ҫ�����µ��ڴ�
	* @return ��
	*/
	void set(void * data, size_t ss,bool b_alloc=false);
	/** ���û������е�ǰָ��λ��
	* @param [in] pos �ڴ���ָ��λ�ã���ͷ��ַ��ʼ���㣬��λΪ�ֽ�
	* @return ��
	*/
	inline void setPos(size_t pos){	
		if(_size>0){
			_curpos = pos;
			assert(pos < _size && pos >= 0);
		}			
	}
	/** ��ȡ�������е�ǰָ��λ��
	* @param ��
	* @return �ڴ���ָ��λ�ã���ͷ��ַ��ʼ���㣬��λΪ�ֽ�
	*/
	inline size_t getPos() {	return _curpos;	}
	/** ��ȡ��ǰλ��ָ��
	* @param ��
	* @return �ڴ��ĵ�ǰ��дָ��
	*/
	inline void * getCurPtr() { return (void*)(((char *)_data)+_curpos);}
	/** ��������Buffer��С
	* @param [in] newsize    �µ�Buffer��С
	* @return 
	*/
	void resize(size_t newsize);
	/** ���Ѿ����ڵ��ڴ�齻����Buffer�����й����ⲿ���ܶ�����ڴ���������
	* @param [in] pBlock    �ڴ���׵�ַָ��
	* @param [in] blockSize �ڴ���С����λΪ�ֽ�
	* @return 
	*/
	void attach(void *pBlock,size_t blockSize);
	/** ��Buffer������ڴ��Ĺ���Ȩ���ͷų��������������Ҫ�����ͷ��ڴ�
	* @param [out] s �����ڴ��Ĵ�С����λ���ظ��ڴ���ָ��Ϊ�ֽ�
	* @return ���ظ��ڴ���ָ��
	*/
	void* detach(size_t & s);
	/** �����ڴ�ֵ����ָ����λ��beginpos�����д��count���ַ�
	* @param [in] beginpos ��ʼд��λ��  
	* @param [in] cdata ����д�������ָ��
	* @param [in] count ����д������ݴ�С
	* @return
	*/
	void write(size_t beginpos,void * cdata,size_t count);

	/** �����ڴ�ֵ����Buffer����Ĭ�ϵ�λ�ã����д��count���ַ�
	* д������󣬵�ǰָ������ڴ��ͷ��λ���Զ�����ƶ�countλ
	* @param [in] cdata ����д�������ָ��
	* @param [in] count ����д������ݴ�С
	* @return
	*/
	void write(void * cdata, size_t count);

	/** ��ȡ�ڴ�ֵ����Buffer����Ĭ�ϵ�λ�ã�����ȡcount���ַ�
	* ��ȡ�����󣬵�ǰָ������ڴ��ͷ��λ���Զ�����ƶ�countλ
	* @param [in] cdata ������ֵ������ָ��
	* @param [in] count	����ȡ�����ݴ�С
	* @return
	*/
	void read(void* cdata, size_t count);
	/** ��Buffer����Ĭ�ϵ�λ�ã����д���ַ���
	* @param [in] sz ����д����ַ���
	* @return
	*/
	void write(const std::string & sz);
	/** ��Buffer����Ĭ�ϵ�λ�ã������ַ���
	* @param [in] sz ����д����ַ���
	* @return
	*/
	void read(std::string & sz);
	/** ��Buffer����Ĭ�ϵ�λ�ã������ַ���
	* @return �ַ���
	*/
	std::string readString();
	/** ��Buffer����Ĭ�ϵ�λ�ã�д��һ��Envelope3d����
	* @param [in] envelope ��д���Envelope3d
	* @return
	*/
	void write(Envelope3d & envelope );
	/** ��Buffer����Ĭ�ϵ�λ�ã�����һ��Envelope3d����
	* @param [in] envelope ������Envelope3d
	* @return
	*/
	void read(Envelope3d & envelope );
	/** ��Buffer����Ĭ�ϵ�λ�ã�д��һ��Vertex3dList����
	* @param [in] vlist ��д���Vertex3dList
	* @return
	*/
	void write(Vertex3dList & vlist );
	/** ��Buffer����Ĭ�ϵ�λ�ã�����һ��Vertex3dList����
	* @param [in] vlist ������Vertex3dList
	* @return
	*/
	void read(Vertex3dList & vlist );
	/** ��Buffer����Ĭ�ϵ�λ�ã�д��һ��Color3bList����
	* @param [in] clist ��д���Color3bList
	* @return
	*/
	void write(Color3bList & clist );
	/** ��Buffer����Ĭ�ϵ�λ�ã�����һ��Color3bList����
	* @param [in] clist ������Color3bList
	* @return
	*/
	void read(Color3bList & clist );	
	/** ��ָ�����ļ����ݶ���Buffer��
	* @param [in] pathName �ļ�ȫ��
	* @return
	*/
	bool loadFile(const string & pathName);
};

/** @} */ 
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace

