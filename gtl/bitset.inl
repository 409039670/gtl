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

begin_gtl_namespace
/////////////////////////////////////////////////////////////////////////////
//�����Ѿ����õ�λ��
template< typename ELEMENT,	typename TRAITS,	typename ALLOC >
size_t Bitset<ELEMENT,TRAITS,ALLOC>::count()
{
	return _data.size(); 
	
}

template< typename ELEMENT, typename TRAITS, typename ALLOC >
bool Bitset<ELEMENT,TRAITS,ALLOC>::get(size_t pos){ 
	if(pos>=_data.size())
		set(pos,false);
	return _data[pos]==0?false:true; 	 
}
//����ָ��λ��Ϊ0��1��true��ʾ1��false��ʾ0�����pos�������鳤�ȣ����Զ���չ
template< typename ELEMENT, typename TRAITS, typename ALLOC >
void Bitset<ELEMENT,TRAITS,ALLOC>::set(size_t pos, bool b ){
	if(pos>= _data.size()){
		size_t i = _data.size();
		while(i<pos){
			_data.push_back(0);
			i++;
		}
		_data.push_back(b?1:0);
	}
	else{
		_data[pos]=b?1:0;
	}
}
//��λ����ת�������������λ����[0]λ��
//���������д�ŵ�1011���򷵻�11�������Ƿ���13
template< typename ELEMENT, typename TRAITS, typename ALLOC >
unsigned long long Bitset<ELEMENT,TRAITS,ALLOC>::to_ullong() {
	size_t s = _data.size();
	unsigned long long ss=0;
	for(size_t i=0;i<s;i++){
		ss=ss<<1;	
		ss+=_data[i];
	}
	return ss;
}

//����ָ��λ�õ�ֵ�����pos����λ���鳤�ȣ��Զ���չ
template< typename ELEMENT, typename TRAITS, typename ALLOC >
bool Bitset<ELEMENT,TRAITS,ALLOC>::operator [] (const size_t pos){
	if(pos>=_data.size())
		set(pos,false);
	return _data[pos]==0?false:true;		
}
//ֱ������ת���ɶ����ƣ���ֵ��Bitset�����λ����[0]λ��
template< typename ELEMENT, typename TRAITS, typename ALLOC >
Bitset<ELEMENT,TRAITS,ALLOC> Bitset<ELEMENT,TRAITS,ALLOC>::operator = (unsigned long long ull){		
	return Bitset<ELEMENT,TRAITS,ALLOC>(ull);
}
//Ĭ�Ϲ���
template< typename ELEMENT, typename TRAITS, typename ALLOC >
Bitset<ELEMENT,TRAITS,ALLOC>::Bitset(){
}
//ֱ������ת���ɶ����ƣ���ֵ��Bitset�����λ����[0]λ��
template< typename ELEMENT, typename TRAITS, typename ALLOC >
Bitset<ELEMENT,TRAITS,ALLOC>::Bitset(unsigned long long n){
	std::vector<unsigned char> v;
	for(size_t i=0;n>0;n/=2){
		v.push_back(n%2==1);
	}
	_data.resize(v.size());
	std::copy(v.rbegin(),v.rend(),_data.begin());
}
//�������캯��
template< typename ELEMENT, typename TRAITS, typename ALLOC >
Bitset<ELEMENT,TRAITS,ALLOC>::Bitset(const Bitset & b){
	_data.resize(b._data.size());
	std::copy(b._data.begin(),b._data.end(),_data.begin());
}
//����λ���λ���࣬�������ŵ���1011���򱾺���ִ�к�Ϊ1101
template< typename ELEMENT, typename TRAITS, typename ALLOC >
void Bitset<ELEMENT,TRAITS,ALLOC>::reverse(){
	std::vector<unsigned char> v(_data.size());
	std::copy(_data.rbegin(),_data.rend(),v.begin());		
	std::copy(v.begin(),v.end(),_data.begin());

}
//�������nλ�������µ�Bitset
template< typename ELEMENT, typename TRAITS, typename ALLOC >
Bitset<ELEMENT,TRAITS,ALLOC> Bitset<ELEMENT,TRAITS,ALLOC>::left(size_t n){
	Bitset<ELEMENT,TRAITS,ALLOC> id;
	id._data.resize(n,0);
	std::copy(_data.begin(),_data.size()>n ? _data.begin()+n:_data.end(),id._data.begin());
	return id;
}

//�����ұ�nλ�������µ�Bitset
template< typename ELEMENT, typename TRAITS, typename ALLOC >
Bitset<ELEMENT,TRAITS,ALLOC> Bitset<ELEMENT,TRAITS,ALLOC>::right(size_t n){
	Bitset<ELEMENT,TRAITS,ALLOC> id;
	id._data.resize(n,0);
	std::copy(_data.rbegin(),_data.size()>n ? _data.rbegin()+n:_data.rend(),id._data.begin());
	id.reverse();
	return id;
}

//��������Bitset�Ƿ����
template< typename ELEMENT, typename TRAITS, typename ALLOC >
bool Bitset<ELEMENT,TRAITS,ALLOC>::operator == (const Bitset & b) {
	size_t s = b._data.size();
	if(s!=_data.size())
		return false;

	for(std::vector<unsigned char>::const_iterator it1=_data.begin(),it2=b._data.begin();
		it1!=_data.end(); it1++,it2++){
			if(*it1!=*it2)
				return false;
	}
	return true;
}
//�ж�b������λ�����Ƿ��Ǳ����λ�����ǰ׺
template< typename ELEMENT, typename TRAITS, typename ALLOC >
bool Bitset<ELEMENT,TRAITS,ALLOC>::has_prefix(const Bitset & b){
	size_t s = b._data.size();
	if(s>_data.size()){
		return false;
	}
	else{
		for(std::vector<unsigned char>::const_iterator it1=_data.begin(),it2=b._data.begin();
		it2!=b._data.end(); it1++,it2++){
			if(*it1!=*it2)
				return false;
		}
	}	
	return true;
}
//�ж�b������λ�����Ƿ��Ǳ����λ�����ǰ׺
template< typename ELEMENT, typename TRAITS, typename ALLOC >
bool Bitset<ELEMENT,TRAITS,ALLOC>::is_prefix(unsigned long long & ull){
	Bitset b (ull);
	size_t s = b._data.size();
	if(s<_data.size()){
		return false;
	}
	else{
		for(std::vector<unsigned char>::const_iterator it1=_data.begin(),it2=b._data.begin();
		it1!=_data.end(); it1++,it2++){
			if(*it1!=*it2)
				return false;
		}
	}	
	return true;
}

//�ж�b������λ�����Ƿ��Ǳ����λ�����ǰ׺
template< typename ELEMENT, typename TRAITS, typename ALLOC >
void Bitset<ELEMENT,TRAITS,ALLOC>::clear(){
	_data.clear();
}

template< typename ELEMENT, typename TRAITS, typename ALLOC >
void Bitset<ELEMENT,TRAITS,ALLOC>::resize(size_t newSize){
	_data.resize(newSize,0);
}

template< typename ELEMENT, typename TRAITS, typename ALLOC >
void Bitset<ELEMENT,TRAITS,ALLOC>::set(const unsigned char * flags,unsigned long long s){
	resize(s);
	memcpy((void*)_data.data(),flags,s);
}

template< typename ELEMENT, typename TRAITS, typename ALLOC >
std::basic_string<ELEMENT,TRAITS,ALLOC> Bitset<ELEMENT,TRAITS,ALLOC>::to_string()
{
	size_t s = _data.size();
	std::basic_string<ELEMENT,TRAITS,ALLOC> sz;
	sz.resize(s);
	for(int i=0;i<s;i++)
	{
		if(_data[i]==0)
			sz[i]='0';
		else
			sz[i]='1';
	}

	return sz;
}

end_gtl_namespace
