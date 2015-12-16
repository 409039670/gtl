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

#include "object.h"

begin_gtl_namespace 
template <
	typename T=char,
	typename TRAITS=std::char_traits<T>,
	typename ALLOC=std::allocator<T> >
class Buffer : public Object <T,TRAITS,ALLOC> {
public:
	virtual std::basic_string<T,TRAITS,ALLOC> getClassName(){
		return "Buffer";
	}
	virtual void writeBuffer(void*  & buffer, size_t & count){
		count =  sizeof(uint32_t)  // _size
			+sizeof(uint32_t)//_capacity
			+sizeof(uint32_t)//_growth
			+sizeof(uint32_t)//_curpos
			+sizeof(uint8_t);//_alloc

		if(_alloc){
			count+=_capacity;//
		}
		else{
			count+=sizeof(uint64_t);//
		}
			
		if(buffer==0)
			buffer = new uint8_t[count];

		uint32_t * p = (uint32_t *) buffer;
		*p=_size;
		p++;
		*p=_capacity;
		p++;
		*p=_growth;
		p++;
		*p=_curpos;
		p++;
		uint8_t *p8 =  (uint8_t*)p;

		if(_alloc)
			*p8= 1 ;
		else
			*p8 = 0;

		p8++;

		if(_alloc){
			memcpy(p8,_data,_capacity);
		}
		else{
			uint64_t* p64= (uint64_t*) p8;
			*p64 = (uint64_t) _data;
		}

		
	}
	virtual void readBuffer(const void*  buffer, size_t count=0){
		clear();
		count =  sizeof(uint32_t)  // _size
			+sizeof(uint32_t)//_capacity
			+sizeof(uint32_t)//_growth
			+sizeof(uint32_t)//_curpos
			+sizeof(uint8_t);//_alloc

		if(_alloc){
			count+=_capacity;//
		}
		else{
			count+=sizeof(uint64_t);//
		}
	
		uint32_t * p = (uint32_t *)buffer;
		*p=_size;
		p++;
		*p=_capacity;
		p++;
		*p=_growth;
		p++;
		*p=_curpos;
		p++;
		uint8_t *p8 =  (uint8_t*)p;

		if(*p8 ==1)
			_alloc=true;
		else 
			_alloc=false;

		p8++;

		if(_alloc){
			_data = new uint8_t[_capacity];
			memcpy(_data,p8,_capacity);
		}
		else{
			uint64_t* p64= (uint64_t*) p8;
			if(sizeof(void*)<sizeof(uint64_t)){
				uint32_t address = (uint32_t)p64;
				memcpy(&_data,&address,sizeof(uint32_t));
			}
			else{
				memcpy(&_data,&p64,sizeof(uint64_t));				
			}			
		}
	}
	virtual Object * clone() {
		Buffer * buf = new Buffer();
		buf->copy(*this);
		return buf;
	}
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
	inline void seek(size_t pos){	
		if(_size>0){
			_curpos = pos;
			assert(pos < _size && pos >= 0);
		}			
	}
	/** ��ȡ�������е�ǰָ��λ��
	* @param ��
	* @return �ڴ���ָ��λ�ã���ͷ��ַ��ʼ���㣬��λΪ�ֽ�
	*/
	inline const size_t tell() const {	return _curpos;	}
	/** ��ȡ��ǰλ��ָ��
	* @param ��
	* @return �ڴ��ĵ�ǰ��дָ��
	*/
	inline void * tell() { return (void*)(((char *)_data)+_curpos);}
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
	size_t read(void* cdata, size_t count);
	/** ��Buffer����Ĭ�ϵ�λ�ã����д���ַ���
	* @param [in] sz ����д����ַ���
	* @return
	*/
	void write(const std::basic_string<T,TRAITS,ALLOC> & sz);
	/** ��Buffer����Ĭ�ϵ�λ�ã������ַ���
	* @param [in] sz ����д����ַ���
	* @return
	*/
	void read(std::basic_string<T,TRAITS,ALLOC> & sz);
	/** ��Buffer����Ĭ�ϵ�λ�ã������ַ���
	* @return �ַ���
	*/
	std::basic_string<T,TRAITS,ALLOC> readString();
		
	/** ��ָ�����ļ����ݶ���Buffer��
	* @param [in] pathName �ļ�ȫ��
	* @return
	*/
	bool loadFile(const std::basic_string<T,TRAITS,ALLOC> & pathName);

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
};

/** @} */ 
////////////////////////////////////////////////////////////////////////////////////////////////////

end_gtl_namespace

#include "buffer.inl"