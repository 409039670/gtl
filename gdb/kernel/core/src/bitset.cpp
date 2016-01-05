
#include "bitset.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

//�����Ѿ����õ�λ��
size_t Bitset::count(){return _data.size();}

bool Bitset::get(size_t pos){
	if(pos>=_data.size())
		set(pos,false);
	return _data[pos]==0?false:true;
}
//����ָ��λ��Ϊ0��1��true��ʾ1��false��ʾ0�����pos�������鳤�ȣ����Զ���չ
void Bitset::set(size_t pos, bool b ){
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
unsigned long long Bitset::to_ullong() {
	size_t s = _data.size();
	unsigned long long ss=0;
	for(size_t i=0;i<s;i++){
		ss=ss<<1;	
		ss+=_data[i];
	}
	return ss;
}

//����ָ��λ�õ�ֵ�����pos����λ���鳤�ȣ��Զ���չ
bool Bitset::operator [] (const size_t pos){
	if(pos>=_data.size())
		set(pos,false);
	return _data[pos]==0?false:true;		
}
//ֱ������ת���ɶ����ƣ���ֵ��Bitset�����λ����[0]λ��
Bitset Bitset::operator = (unsigned long long ull){		
	return Bitset(ull);
}
//Ĭ�Ϲ���
Bitset::Bitset(){
}
//ֱ������ת���ɶ����ƣ���ֵ��Bitset�����λ����[0]λ��
Bitset::Bitset(unsigned long long n){
	std::vector<unsigned char> v;
	for(size_t i=0;n>0;n/=2){
		v.push_back(n%2==1);
	}
	_data.resize(v.size());
	std::copy(v.rbegin(),v.rend(),_data.begin());
}
//�������캯��
Bitset::Bitset(const Bitset & b){
	_data.resize(b._data.size());
	std::copy(b._data.begin(),b._data.end(),_data.begin());
}
//����λ���λ���࣬�������ŵ���1011���򱾺���ִ�к�Ϊ1101
void Bitset::reverse(){
	std::vector<unsigned char> v(_data.size());
	std::copy(_data.rbegin(),_data.rend(),v.begin());		
	std::copy(v.begin(),v.end(),_data.begin());

}
//�������nλ�������µ�Bitset
Bitset Bitset::left(size_t n){
	Bitset id;
	id._data.resize(n,0);
	std::copy(_data.begin(),_data.size()>n ? _data.begin()+n:_data.end(),id._data.begin());
	return id;
}

//�����ұ�nλ�������µ�Bitset
Bitset Bitset::right(size_t n){
	Bitset id;
	id._data.resize(n,0);
	std::copy(_data.rbegin(),_data.size()>n ? _data.rbegin()+n:_data.rend(),id._data.begin());
	id.reverse();
	return id;
}

//��������Bitset�Ƿ����
bool Bitset::operator == (const Bitset & b) {
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
bool Bitset::has_prefix(const Bitset & b){
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
bool Bitset::is_prefix(unsigned long long & ull){
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
void Bitset::clear(){
	_data.clear();
}


void Bitset::resize(size_t newSize){
	_data.resize(newSize,0);
}

void Bitset::set(const unsigned char * flags,unsigned long long s){
	resize(s);
	memcpy((void*)_data.data(),flags,s);
}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
