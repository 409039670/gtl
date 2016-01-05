#include "mesh.h"  

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

Block::Block(unsigned long long cx,unsigned long long cy, unsigned long long cz){
	_identifier=Identifier::generate();
	_xCellNumber=cx;//X�����߼���Ԫ��
	_yCellNumber=cy;//Y�����߼���Ԫ��
	_zCellNumber=cz;//Z�����߼���Ԫ��   
	/** �ϼ������X����Ŀ�ʼ�ͽ�����Ԫ�±꣬��0��ʼ,Ϊ�뿪���䣬Ĭ���������_endX = _beginX+_xCellNumber */
	_beginX=0;
	/** �ϼ������Y����Ŀ�ʼ�ͽ�����Ԫ�±꣬��0��ʼ��Ϊ�뿪���䣬Ĭ���������_endY = _beginY+_yCellNumber */
	_beginY=0;
	/** �ϼ������Z����Ŀ�ʼ�ͽ�����Ԫ�±꣬��0��ʼ��Ϊ�뿪���䣬Ĭ���������_endZ = _beginZ+_zCellNumber */
	_beginZ=0;

	_layerID= 0;
}
Block::~Block(){
	_cells.clear();
}
void Block::output(ofstream & f){
	f<<_identifier.get()<<" "<<_layerID<<" " 
		<<_xCellNumber<<" "<<_yCellNumber<<" "<<_yCellNumber<<" "<<_zCellNumber<<" "
		<<_beginX<<" "<<_beginY<<" "<<_beginZ<<std::endl;
	f<<_envelope.minx<<" "<<_envelope.maxx<<" "
		<<_envelope.miny<<" "<<_envelope.maxy<<" "
		<<_envelope.minz<<" "<<_envelope.maxz<<std::endl;

	f<<_envelope.minx<<" "<<_envelope.maxx<<" "
		<<_envelope.miny<<" "<<_envelope.maxy<<" "
		<<_envelope.minz<<" "<<_envelope.maxz<<std::endl;

	f<< _cells.size() << std::endl;
	for(size_t i =0;i<_cells.size();i++)
	{
		_cells[i]->output(f);
	}
}
void Block::output(std::vector<BlockSharedPtr> & blks,const std::string &filename){
	std::locale _loc = std::locale::global(std::locale(""));//���û���Ϊϵͳ����  
	
	ofstream ofs;
	ofs.open(filename.c_str(),std::ios_base::in|std::ios_base::trunc);

	if (ofs)
	{		
		size_t s = blks.size();
		ofs<<s<<std::endl;
		for(int i=0;i<s;i++){
			blks[i]->output(ofs);
		}
	}
	ofs.close();
	std::locale::global(_loc);//��ԭ����  
}
/** ��ȡ����д�뵽Buffer����ռ�õ��ֽ��������ڴ�Buffer�����й������ζ���
    * @param  [in] �� 
    * @return ���ؼ��ζ���д�뵽Buffer����ռ�õ��ֽ���
	*/
size_t Block::sizeBuffer() {
	size_t s=0;

	s+=sizeof(double)*6;//�߽�

	s+= sizeof(unsigned long long)*9;//ID+X/Y/Z���������ϵĵ�Ԫ��������ʼֵ+��Ԫ�ܸ���+LayerID

	for(size_t i =0; i<_cells.size();i++){
		s+= _cells[i]->sizeBuffer();
	}
	
	return s;
}
/** ��Buffer�ж�ȡ��Ϣ��䱾���ζ���
    * @param  [in,out] buf Buffer & ���������������� 
    * @return �ɹ�����true�����򷵻�false
*/
bool Block::readBuffer( Buffer & buf){

	buf.read(_identifier.getBytes(),sizeof(unsigned long long));

	buf.read(&_layerID,sizeof(unsigned long long));

	//Buffer::read(_envelope,buf);
	double v[6];	
	buf.read((void*)v,sizeof(double)*6);
	_envelope.set(v);

	buf.read(&_xCellNumber,sizeof(unsigned long long));
	buf.read(&_yCellNumber,sizeof(unsigned long long));
	buf.read(&_zCellNumber,sizeof(unsigned long long));

	buf.read(&_beginX,sizeof(unsigned long long));
	buf.read(&_beginY,sizeof(unsigned long long));
	buf.read(&_beginZ,sizeof(unsigned long long));

	unsigned long long s = 0;//_cells.size();
	buf.read(&s,sizeof(unsigned long long));
	_cells.clear();

	for(size_t i=0;i<s;i++){
		CellSharedPtr cp (new Cell());
		cp->readBuffer(buf);
		_cells.push_back(cp);
	}

	return true;
}
/** ���������Ϣд��Buffer�� 
    * @param  [in,out] buf Buffer & ����������������
    * @return �ɹ�����true�����򷵻�false
*/
bool Block::writeBuffer(Buffer& buf) {

	buf.write(_identifier.getBytes(),sizeof(unsigned long long));

	buf.write(&_layerID,sizeof(unsigned long long));

	//Buffer::write(_envelope,buf);
	double v[6];
	_envelope.get(v);
	buf.write((void*)v,sizeof(double)*6);

	buf.write(&_xCellNumber,sizeof(unsigned long long));
	buf.write(&_yCellNumber,sizeof(unsigned long long));
	buf.write(&_zCellNumber,sizeof(unsigned long long));

	buf.write(&_beginX,sizeof(unsigned long long));
	buf.write(&_beginY,sizeof(unsigned long long));
	buf.write(&_beginZ,sizeof(unsigned long long));

	unsigned long long s = _cells.size();
	buf.write(&s,sizeof(unsigned long long));

	for(size_t i=0;i<s;i++){
		_cells[i]->writeBuffer(buf);
	}

	return true;
}
/* ���������Ϣд��������У������ļ���д�򻺳���д���� 
    * @param  [in,out] f  std::ostream &����׼�������������
    * @return �ɹ�����true�����򷵻�false
*/
void Block::write(std::ostream & f){
	Buffer buf;
	writeBuffer(buf);
	unsigned long long s = buf.size();
	f.write((const char*)&s,sizeof(unsigned long long));
	f.write((const char*) buf.data(),s);
}
/** �ӱ�׼����������ȡ�������Ϣ  
    * @param  [in,out] f  std::istream &����׼��������������
    * @return �ɹ�����true�����򷵻�false
*/
void Block::read(std::istream & f){
	unsigned long long  s = 0;
	f.read((char*)&s,sizeof(unsigned long long));
	char *  p = new char[s];
	Buffer buf;
	buf.attach(p,s);
	f.read(p,s);
	readBuffer(buf);	
}

void Block::clear(){
	_xCellNumber=0;//X�����߼���Ԫ��
	_yCellNumber=0;//Y�����߼���Ԫ��
	_zCellNumber=0;//Z�����߼���Ԫ��   
	/** �ϼ������X����Ŀ�ʼ�ͽ�����Ԫ�±꣬��0��ʼ,Ϊ�뿪���䣬Ĭ���������_endX = _beginX+_xCellNumber */
	_beginX=0;
	/** �ϼ������Y����Ŀ�ʼ�ͽ�����Ԫ�±꣬��0��ʼ��Ϊ�뿪���䣬Ĭ���������_endY = _beginY+_yCellNumber */
	_beginY=0;
	/** �ϼ������Z����Ŀ�ʼ�ͽ�����Ԫ�±꣬��0��ʼ��Ϊ�뿪���䣬Ĭ���������_endZ = _beginZ+_zCellNumber */
	_beginZ=0;

	_cells.clear();

	_envelope = Envelope3d();

	_layerID=0;

}

void Block::addCell(CellSharedPtr c){
	if(! c.get()) return ;

	_cells.push_back(c);
	Envelope3d e3d ;
	c->getEnvelope(e3d);
	_envelope.extend(e3d);
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
