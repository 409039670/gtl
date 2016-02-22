
#include "indexbuffer.h"






begin_gtl_namespace
begin_gdb_namespace

IndexBuffer::IndexBuffer(){
	_type = DBF_UINT32;
}
IndexBuffer::IndexBuffer(unsigned int * pi,size_t number,bool b_alloc)//���캯�������b_allocΪ�棬���������һ���СΪs���ڴ�飬������d�����ݣ���֮����ָ��d
:DescribableBuffer(pi,number*sizeof(unsigned int),DBF_UINT32,b_alloc)
{

}
void IndexBuffer::copy(const IndexBuffer & b)//����Buffer���������b��_alloc��ԱΪ�٣��򿽱����Buffer������b����һ���ڴ�飬����Ϊ�ڴ�鿽��
{
	DescribableBuffer::copy((const DescribableBuffer&)b);
}
IndexBuffer::IndexBuffer(const IndexBuffer & b)//�������캯��
{
	copy(b);
}
IndexBuffer IndexBuffer::operator = (const IndexBuffer & b)//�ȺŸ�ֵ����
{
	copy(b);
	return * this;
}


void IndexBuffer::add(unsigned int i){
	Buffer::write(&i,sizeof(unsigned int));
}
void IndexBuffer::add(unsigned int i,unsigned int j){
	unsigned int ps [] = {i,j};
	Buffer::write(ps,sizeof(unsigned int)*2);
}
void IndexBuffer::add(unsigned int i,unsigned int j,unsigned int k){
	unsigned int ps [] = {i,j,k};
	Buffer::write(ps,sizeof(unsigned int)*3);
}
void IndexBuffer::add(unsigned int i,unsigned int j,unsigned int k,unsigned int m){
	unsigned int ps [] = {i,j,k,m};
	Buffer::write(ps,sizeof(unsigned int)*4);
}

void IndexBuffer::add(const long * data,const long size){
	unsigned int * ps   = new unsigned int [size];
	for(long i = 0;i<size;i++)
		ps[i]=data[i];
	Buffer::write(ps,sizeof(unsigned int)*size);
	delete [] ps;
}
end_gdb_namespace
end_gtl_namespace


