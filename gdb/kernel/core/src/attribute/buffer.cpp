#include "config.h"
#include "buffer.h"


begin_gtl_namespace
begin_gdb_namespace

Buffer::Buffer(size_t size ,size_t growth){//Ĭ�Ϲ��캯��
	_data=0;	
	_curpos=0;	
	_growth=growth;
	if(size>0){
		_size=size;
		_alloc=true;
		_capacity=_size;
		_data = new char [_capacity];
	}
	else{
		_capacity=0;
		_size=0;
		_alloc=false;
	}
}
void Buffer::clear(){//���Buffer
	if(_alloc && _data){
		delete  [] ((unsigned char*)_data);
	}
	_data = 0;
	_size=0;
	_alloc=false;
	_curpos=0;
	_capacity=0;
	_growth=512;
}
Buffer::~Buffer(){//Ĭ����������
	clear();
}
Buffer::Buffer(void * d,size_t s,bool b_alloc,size_t growth){//���캯�������b_allocΪ�棬���������һ���СΪs���ڴ�飬������d�����ݣ���֮����ָ��d
	_size=s;

	_capacity=s;

	_growth=growth;

	_alloc=b_alloc;
	if(_alloc){
		_data = new char[_size];
		memcpy(_data,d,_size);
	}
	else{
		_data=d;
	}
	_curpos=0;
}
void Buffer::copy(const Buffer & b){//����Buffer���������b��_alloc��ԱΪ�٣��򿽱����Buffer������b����һ���ڴ�飬����Ϊ�ڴ�鿽��
	set(b._data,b._size,b._alloc);
	_curpos=0;
}
Buffer::Buffer(const Buffer & b){//�������캯��
	_data = 0;
	_size=0;
	_alloc=false;
	_curpos=0;
	_capacity=0;
	_growth=512;
	copy(b);
}
Buffer Buffer::operator = (const Buffer & b){//�ȺŸ�ֵ����
	copy(b);
	return * this;
}
void Buffer::set(void * pv, size_t ss,bool b_alloc){//���û����������b_allocΪ�棬���������һ���СΪss���ڴ�飬������pv�����ݣ���֮����ָ��pv
	clear();
	_alloc = b_alloc;
	if(_alloc){
		_data = new char[ss];
		_size = ss;
		_capacity=ss; 
		if(pv)
			memcpy(_data,pv,ss);
	}
	else{
		_data = pv;
		_size = ss;
		_capacity=ss;
	}
	_curpos=0;
}


//void Buffer::attach(void * pv, unsigned long long ss){
//	clear();
//	_data = pv;
//	_size = ss;
//	_alloc=true;
//}
//void* Buffer::detach(unsigned long long & s){		
//	s=_size;
//	_alloc=false;
//	void* p =_data;
//	_data = 0;
//	_curpos=0;
//	return p;
//}

void Buffer::attach(void * pv, size_t ss){
	clear();
	_data = pv;
	_size = ss;
	_capacity=ss;
	_alloc=true;
}
void* Buffer::detach(size_t & s){		
	s=_size;
	_alloc=false;
	void* p =_data;
	_data = 0;
	_curpos=0;
	_size=0;
	_capacity=0;
	return p;
}

void Buffer::write(size_t beginpos,void * cdata,size_t count)//��ָ����λ��beginpos�����д��count���ַ�
{
	if(count==0||cdata==0)return ;

	if(beginpos+count<=_capacity){
		memcpy(((char*)_data)+beginpos,cdata,count);
		//��չʹ�ù����ڴ��С����
		if(_size<beginpos+count)
			_size = beginpos+count;
	}
	else{//��Ҫ���������ڴ�����
		size_t new_capacity = ((beginpos+count)/_growth+1)*_growth;
		char * p = new char[new_capacity];
		memcpy(p,_data,beginpos);
		delete [] ((char*)_data);
		memcpy(p+beginpos,cdata,count);
		//char * p = new char [beginpos+count];
		//memcpy(p,_data,beginpos);
		//memcpy(p+beginpos,cdata,count);		
		_data = p;
		_size = beginpos+count;
		_capacity = new_capacity;

		// ���ڵ�����delete [] ((char*)_data); _data = p;  ��ˣ�����ڴ�����attach��ʽ�õ��ģ�
		//���ⲿ�Ĺ������޷�ʵ�����ڴ������,�����������Ĵ�������������clear���� [7-29-2010 �￨]
		_alloc = true;
	}
	//�ƶ���ǰָ��λ��
	_curpos = beginpos + count;
}

void Buffer::write(void * cdata, size_t count)
{
	if(count==0||cdata==0)return ;

	if(_curpos+count<=_capacity){
		memcpy(((char*)_data)+_curpos,cdata,count);
		//�ƶ���ǰָ��λ��
		_curpos += count;
		//��չʹ���ڴ�Ĵ�С
		if(_size<=_curpos)
			_size = _curpos;
	}
	else{
		size_t new_capacity = ((_curpos+count)/_growth+1)*_growth;
		char * p = new char[new_capacity];
		memcpy(p,_data,_curpos);
		delete [] ((char*)_data);
		memcpy(p+_curpos,cdata,count);

		///**�޸�_curpos����Ϊ��ֵ[2009-09-17cuiyt]*/
		//_size = _curpos+count;
		//char * p = new char [_size];
		//memcpy(p,_data,_curpos);
		//memcpy(p+_curpos,cdata,count);
		//delete [] ((char*)_data);
		_data = p;
		_curpos += count;	
		//��չʹ���ڴ�Ĵ�С
		if(_size<=_curpos)
			_size = _curpos;

		_capacity = new_capacity;

		// ���ڵ�����delete [] ((char*)_data); _data = p;  ��ˣ�����ڴ�����attach��ʽ�õ��ģ�
		//���ⲿ�Ĺ������޷�ʵ�����ڴ������,�����������Ĵ�������������clear���� [7-29-2010 �￨]
		_alloc = true;
	}
}

void Buffer::read(void * cdata, size_t count)
{
	if(_curpos+count<=_size){
		memcpy(cdata,((char*)_data)+_curpos,count);
		_curpos += count;
	}
	else{
		assert(0);
	}
}

void Buffer::write(const std::string & sz){
	int s = (int) sz.size();
	write((void*)&s,sizeof(int));
	if(s>0){
		const char * sc = sz.c_str();
		write((void*)sc,s*sizeof(char));
	}
}

std::string Buffer::readString(){
	int s =0;
	read((void*)&s,sizeof(int));
	std::string sz;
	if(s>0){
		char * cc = new char[s];
		read((void*)cc,s*sizeof(char));
		sz.assign(cc,s);
		delete [] cc;
	}
	return sz;
}
void Buffer::read(std::string & sz){
	int s =0;
	read((void*)&s,sizeof(int));
	if(s>0){
		char * cc = new char[s];
		read((void*)cc,s*sizeof(char));
		sz.assign(cc,s);
		delete [] cc;
	}
}
void Buffer::write(Envelope3d & vl){
	double * pv = new double [6];
	pv[0]=vl.minx;pv[1]=vl.maxx;
	pv[2]=vl.miny;pv[3]=vl.maxy;
	pv[4]=vl.minz;pv[5]=vl.maxz;
	write((void*)pv,sizeof(double)*6);
	delete [] pv;
}
void Buffer::read(Envelope3d & vl){
	const double * pv =(const double*) getCurPtr();
	vl.minx = *pv;
	pv++;
	vl.maxx= *pv;
	pv++;
	vl.miny=*pv;
	pv++;
	vl.maxy=*pv;
	pv++;
	vl.minz=*pv;
	pv++;
	vl.maxz=*pv;
}
void Buffer::write(Vertex3dList & vl){
	size_t s = 3*vl.size()*sizeof(double)+sizeof(int) ;
	char * pc  = new char [s];
	int * pi = (int*)pc;
	*pi = (int)vl.size();
	pi++;
	double * p =(double*) pi;
	for(Vertex3dList::iterator it = vl.begin();it!=vl.end();it++){
		*p=it->x;
		p++;
		*p=it->y;
		p++;
		*p=it->z;
		p++;
	}	
	write((void*)pc,s);
	delete [] pc;
}
void Buffer::read(Vertex3dList & vl){
	const int * pi = (const int * ) getCurPtr();
	vl.resize(*pi);
	pi++;
	const double * p = (const double*)pi;
	for(Vertex3dList::iterator it = vl.begin();it!=vl.end();it++){
		it->x=*p;
		p++;
		it->y=*p;
		p++;
		it->z=*p;
		p++;
	}
}
void Buffer::write(Color3bList & vl){
	size_t s = 3*vl.size()*sizeof(unsigned char)+sizeof(int) ;
	char * pc  = new char [s];
	int * pi = (int*)pc;
	*pi = (int) vl.size();
	pi++;
	unsigned char * p =(unsigned char*) pi;
	for(Color3bList::iterator it = vl.begin();it!=vl.end();it++){
		*p=it->r;
		p++;
		*p=it->g;
		p++;
		*p=it->b;
		p++;
	}
	write((void*)pc,s);
	delete [] pc;
}
void Buffer::read(Color3bList & vl){
	const int * pi = (const int * ) getCurPtr();
	vl.resize(*pi);
	pi++;
	const unsigned char * p = (const unsigned char*)pi;
	for(Color3bList::iterator it = vl.begin();it!=vl.end();it++){
		it->r=*p;
		p++;
		it->g=*p;
		p++;
		it->b=*p;
		p++;
	}
}

void Buffer::resize(size_t newsize){
	if(_capacity==0 && _data==0)
	{
		_data =(void*) new char[newsize];
		_capacity=newsize;
		_size=newsize;
		return;
	}
	if(newsize>_capacity){
		char * p = new char[newsize];
		memcpy(p,_data,_capacity);
		delete [] ((char*)_data);
		_data = p;
		_capacity=newsize;
		_size=newsize;
	}
	else{
		if(newsize>=_size){
			_size = newsize;
		}
		else{
			_size=newsize;
			_curpos = min(_curpos,_size);
		}
	}
}

/** ��ָ�����ļ����ݶ���Buffer��
	* @param [in] pathName �ļ�ȫ��
	* @return
	*/
bool Buffer::loadFile(const string & pathName)
{

	try{
		FILE * fp = fopen(pathName.c_str(), "rb");
		fseek(fp,0,2);
		size_t s= ftell(fp);		
		resize(s);
		fseek(fp,0,0);
		fread(_data,1,s,fp);
		fclose(fp);
		return true;
	}
	catch(...)
	{
		return false;
	}
}

end_gdb_namespace
end_gtl_namespace


