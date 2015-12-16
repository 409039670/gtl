#include "internal_featureimpl.h" 
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
FeatureImpl::FeatureImpl()
{
	_id = Identifier::generate();	//Ҫ��ID
	_name = "";						//Ҫ������
	_lod = 1;						//Ҫ��LOD
	_regid = 0;						//Ҫ�ط���ID
	_fclsid = 0;
	_geom.reset();

	//_topo=NULL;

	_state = 0;
	_createTime = 0; //����ʱ��
	_updateTime = 0; //����ʱ��
	_buffersize = 0;

	_lod_info=LODInfo::create();

}

FeatureImpl::FeatureImpl(FeatureClassSharedPtr fc,FID id, std::string name, int lod, REGID regid, int state, 
	GeometrySharedPtr pg, /*C3DSemanticTopology * pt ,*/
	GDBTIME createTime ,GDBTIME updateTime,unsigned long buffersize){
		_id=id;//Ҫ��ID
		_name=name;//Ҫ������
		_lod=lod;//Ҫ��LOD
		_regid=regid;//Ҫ�ط���ID
		_fclsid=fc->getID();//Ҫ�ض����Ӧ��Ҫ����
		
		_geom = pg;

		std::vector<PropType> prop;
		prop = fc->getPropTypes();
		for (vector<PropType>::iterator itor = prop.begin(); itor != prop.end(); itor++){
			_props.push_back("");  //��prop����ֵ
		}

		_state=state;
		_createTime=0; //����ʱ��
		_updateTime=0; //����ʱ��
		_buffersize = buffersize;

		_lod_info=LODInfo::create();
}

FeatureImpl::FeatureImpl(FCLSID fc,FID id, std::string name, int lod, REGID regid, int state, 
	GeometrySharedPtr pg, /*C3DSemanticTopology * pt ,*/
	GDBTIME createTime ,GDBTIME updateTime,unsigned long buffersize){
		_id=id;//Ҫ��ID
		_name=name;//Ҫ������
		_lod=lod;//Ҫ��LOD
		_regid=regid;//Ҫ�ط���ID
		_fclsid=fc;//Ҫ�ض����Ӧ��Ҫ����

		_geom = pg;

		std::vector<PropType>& prop = FeatureClass::findFeatureClass(fc)->getPropTypes();
		for (vector<PropType>::iterator itor = prop.begin(); itor != prop.end(); itor++){
			_props.push_back("");  //��prop����ֵ
		}

		_state=state;
		_createTime=0; //����ʱ��
		_updateTime=0; //����ʱ��
		_buffersize = buffersize;

		_lod_info=LODInfo::create();
}

FeatureImpl::FeatureImpl(FeatureImpl & f){
	_lod_info=LODInfo::create();
	copy(f);
}
void FeatureImpl::copy(const FeatureImpl & f){
	_id=f._id;//Ҫ��ID
	_name=f._name;//Ҫ������
	_lod=f._lod;//Ҫ��LOD
	_regid=f._regid;//Ҫ�ط���ID
	_fclsid=f._fclsid;//Ҫ�ض����Ӧ��Ҫ����

	
	_geom = GeometrySharedPtr(f._geom->clone());
	
	//if (!_topo)
	//{
	//	newTopology();
	//}
	//if(f._topo){
	//	_topo->copy(f._topo);
	//}
	_state=f._state;
	_createTime=f._createTime; //����ʱ��
	_updateTime=f._updateTime; //����ʱ��
	_props.resize(f._props.size());
	std::copy(f._props.begin(),f._props.end(),_props.begin());
	_fclsid = f._fclsid;//Ҫ�ض����Ӧ��Ҫ����
	_code = f._code; //Ҫ�����ͱ���
	_buffersize = f._buffersize;
}
FeatureImpl::~FeatureImpl(){
	/*if(_geom)
		delete _geom;*/
	//if(_topo)
	//	delete _topo;
}


//�ļ���д
void FeatureImpl::write(std::ostream & f){

	f.write((char*)(&_id),sizeof(FID));//1.����ID

	int s = _name.size();//2.��������
	f.write((char*)(&s),sizeof(int));
	f.write((char*)(_name.c_str()),s);

	f.write((char*)(&_state),sizeof(int));//3.����״̬

	f.write((char*)(&_lod),sizeof(int));//4.����LOD����

	f.write((char*)(&_regid),sizeof(REGID));//5.����ID

	f.write((char*)(&_createTime),sizeof(GDBTIME));//7.���󴴽�ʱ��

	f.write((char*)(&_updateTime),sizeof(GDBTIME));//8.�������ʱ��

	_lod_info->write(f);//9.����LOD��Ϣ
	f.write((char*)(& _fclsid),sizeof(FCLSID));//10.Ҫ��������Ϣ


	bool geomExist = false; //11.geometry�����Ƿ���ڱ�ʶ  [sunka, 2010-3-1]
	if (_geom.get()) {
		geomExist = true;
		f.write((char*)(&geomExist),sizeof(bool));
		_geom->write(f);//12.���ζ�����Ϣ
	}
	else{
		f.write((char*)(&geomExist),sizeof(bool));
	}

	////д�����˶�����Ϣ
	//bool topoExist = false; //13.���˶����Ƿ���ڱ�ʶ  [lxm, 2010-3-3]
	//if (_topo) {
	//	topoExist = true;
	//	f.write((char*)(&topoExist),sizeof(bool));
	//	_topo->write(f);//14.���˶�����Ϣ
	//}
	//else{
	//	f.write((char*)(&topoExist),sizeof(bool));
	//}

	s= _props.size();//15.������Ϣ
	f.write((char*)(&s),sizeof(int));
	for(std::vector<std::string>::iterator it = _props.begin();it!=_props.end();it++){
		s = it->size();
		f.write((char*)(&s),sizeof(int));
		f.write((char*)(it->c_str()),s);
	}
}

void FeatureImpl::read(std::istream & f){
	int s = 0;
	char ca[255];

	f.read((char*)(&_id),sizeof(FID));

	f.read((char*)(&s),sizeof(int));
	f.read((char*)(ca),s);
	_name.assign(ca,ca+s);

	f.read((char*)(&_state),sizeof(int));

	f.read((char*)(&_lod),sizeof(int));

	f.read((char*)(&_regid),sizeof(REGID));

	f.read((char*)(&_createTime),sizeof(GDBTIME));

	f.read((char*)(&_updateTime),sizeof(GDBTIME));

	_lod_info->read(f);

	f.read((char*)(&_fclsid),sizeof(FCLSID));

	//�ж�Geometry�Ƿ����  [sunka, 2010-3-1]
	bool geomExist = false;
	f.read((char*)(&geomExist),sizeof(bool));
	if (geomExist) {
		//�����һ��size_t�ĳ���
		f.seekg(sizeof(unsigned int),ios::cur);

		//������������  [sunka, 2010-3-1]
		int nTypeID =0;
		f.read((char*)(&nTypeID),sizeof(int));

		//�Ѷ�ָ������ȥ  [sunka, 2010-3-1]
		f.seekg(-(sizeof(unsigned int)+sizeof(int)),ios::cur);

		//����һ���������Ͳ�ʵ�ֶ�ȡ  [sunka, 2010-3-1]
		_geom = Geometry::create(nTypeID);
		_geom->read(f);
		/*this->attatchGeometry(pGeo);*/
	}	
	else{
		this->attatchGeometry(NULL);
	}

	////�ж����˶����Ƿ����  [sunka, 2010-3-1]
	//bool bTopoExist = false;
	//f.read((char*)(&bTopoExist),sizeof(bool));
	//if(bTopoExist)
	//{
	//	//���ﻹ��Ҫ����[20100303_lxm]
	//	//�����һ��size_t�ĳ���
	//	f.seekg(sizeof(unsigned int),ios::cur);

	//	//������������  [sunka, 2010-3-1]
	//	int nTopoTypeID =0;
	//	f.read((char*)(&nTopoTypeID),sizeof(int));

	//	//�Ѷ�ָ������ȥ  [sunka, 2010-3-1]
	//	f.seekg(-(sizeof(unsigned int)+sizeof(int)),ios::cur);

	//	//����һ���������Ͳ�ʵ�ֶ�ȡ  [sunka, 2010-3-1]
	//	C3DSemanticTopology *pTopo = C3DSemanticTopology::newTopology(nTopoTypeID);
	//	_topo->read(f);
	//	this->attatchTopology(pTopo);
	//}	
	//else{
	//	this->attatchTopology(NULL);
	//}


	f.read((char*)(&s),sizeof(int));
	_props.resize(s);
	for(std::vector<std::string>::iterator it = _props.begin();it!=_props.end();it++){
		f.read((char*)(&s),sizeof(int));
		f.read((char*)(ca),s);
		it->assign(ca,ca+s);
	}
}
//�ڴ��д
size_t FeatureImpl::sizeBuffer()
{
	size_t s = sizeof(FID)//1.����ID
		+sizeof(int)//2.��������ռλ
		+sizeof(char*)*_name.size()//2.��������
		+sizeof(int)//3.����״̬
		+sizeof(int)//4.����LOD����
		+sizeof(REGID)//5.����ID
		+sizeof(GDBTIME)//6.���󴴽�ʱ��
		+sizeof(GDBTIME)//7.�������ʱ��
		+_lod_info->sizeBuffer()//8.����LOD��Ϣ
		+sizeof(int)
		+sizeof(FCLSID)//9.Ҫ������ID
		+sizeof(int);//10���ζ�����Ϣ����
	if (_geom) {
		s+=_geom->sizeBuffer();//11.���ζ�����Ϣ
	}

	//s=s+sizeof(int)//���˶�����Ϣ����
	//+_topo->sizeBuffer();//12.���˶�����Ϣ

	//13.Ҫ�����ͱ���
	s += sizeof(int); //Ҫ�����ͱ���ռλ
	s += sizeof(char)*_code.size();//Ҫ�����ͱ���

	//14.������Ϣ
	s += sizeof(int);//������Ϣ��¼��ռλ
	for(std::vector<std::string>::iterator it = _props.begin();it!=_props.end();it++){
		s += sizeof(int);//������Ϣ����ռλ
		s += sizeof(char*)*(it->size());//������Ϣ
	}

	return s;
}

void FeatureImpl::readBuffer(const Buffer & buf)
{
	int len = 0;

	FID * pfid = (FID *)(((Buffer&)buf).data());
	_id = (FID)*pfid;//1.��������ID
	pfid++;

	//2.������������
	int *pi = (int *)pfid;
	len = *pi;
	pi++;
	char * pc = (char*)pi;
	_name.assign(pc,pc+len);
	pc += len;

	pi = (int *)pc;
	_state = *pi;//3.��������״̬
	pi++;
	_lod = *pi;//4.��������LOD����
	pi++;

	REGID * pregid = (REGID*)pi;
	_regid = *pregid;//5.��������ID
	pregid++;

	GDBTIME * pGdbTime = (GDBTIME*)pregid;
	_createTime = *pGdbTime;//6.�������󴴽�ʱ��
	pGdbTime++;
	_updateTime = *pGdbTime;//7.�����������ʱ��
	pGdbTime++;

	//8.��������LOD��Ϣ
	pi = (int*)pGdbTime;
	len = *pi;
	pi++;
	pc = (char*)pi;
	Buffer lbuf;
	lbuf.set(pc,(size_t)len,true);
	_lod_info->readBuffer(lbuf);
	pc=pc+len;

	//9.����Ҫ��������Ϣ
	FCLSID * pfcid = (FCLSID*)pc;
	_fclsid = *pfcid;
	pfcid++;

	//10.�������ζ�����Ϣ����
	pi = (int*)pfcid;
	len = *pi;
	pi++;

	//11.������ȴ���0  [sunka, 2010-3-2]
	if (len>0) {
		//����Geometry������  [sunka, 2010-3-2]
		int nTypeID = *pi;
		//����һ���������Ͳ�ʵ�ֶ�ȡ  [sunka, 2010-3-1]
		_geom = GeometrySharedPtr(Geometry::create(nTypeID));

		pc = (char*)pi;
		Buffer gbuf;
		size_t s = (size_t)len;
		gbuf.attach(pc,s);
		_geom->readBuffer(gbuf);
		gbuf.detach(s);

		pc=pc+len;
	}
	else{
		pc=(char*)pi;
	}	

	////12.�������˶�����Ϣ
	//pi = (int*)pc;
	//len = *pi;
	//pi++;
	//pc = (char*)pi;
	//Buffer tbuf;
	//tbuf.set(pc,(size_t)len,true);
	//_topo->readBuffer(tbuf);
	//pc=pc+tbuf.size();

	//13.Ҫ�����ͱ���
	pi=(int*)pc;
	len = *pi;
	pi++;
	pc = (char*)pi;
	//������Ȳ�Ϊ0���򿽱������򲻿���  [sunka, 2009-10-24]
	if (len) {
		_code.assign(pc,pc+len);
	}
	else{
		_code = "";
	}

	pc = pc + len;

	//14.����������Ϣ
	pi = (int*)pc;
	if(pi>0){
		_props.resize(*pi);
	}
	else{
		_props.clear();
	}
	pi++;
	for(std::vector<std::string>::iterator it = _props.begin();it!=_props.end();it++){
		len = *pi;
		pi++;
		pc = (char*)pi;
		it->assign(pc,pc+len);
		pc += len;
		pi = (int*)pc;
	}	
}
//������Ϣ�ں������ڴ��д
size_t FeatureImpl::sizeBufferNoProp(){
	size_t s = sizeof(FID)//1.����ID
		+sizeof(int)//2.��������ռλ
		+sizeof(char*)*_name.size()//2.��������
		+sizeof(int)//3.����״̬
		+sizeof(int)//4.����LOD����
		+sizeof(REGID)//5.����ID
		+sizeof(GDBTIME)//6.���󴴽�ʱ��
		+sizeof(GDBTIME)//7.�������ʱ��
		+_lod_info->sizeBuffer()//8.����LOD��Ϣ
		+sizeof(int)
		+sizeof(FCLSID)//9.Ҫ������ID
		+sizeof(int);//���ζ�����Ϣ����
	if (_geom) {
		s+=_geom->sizeBuffer();//11.���ζ�����Ϣ
	}
	//s=s+sizeof(int);//12.���˶�����Ϣ����
	//if(_topo){
	//	s+=_topo->sizeBuffer();//13.���˶�����Ϣ
	//}

	//12.Ҫ�����ͱ���
	s += sizeof(int); //Ҫ�����ͱ���ռλ
	s += sizeof(char)*_code.size();//Ҫ�����ͱ���

	return s;
}

void FeatureImpl::writeBuffer(Buffer & buf){

	size_t s = sizeBuffer();
	int len =0;
	int * pi = (int *) new unsigned char [s];
	buf.attach(pi,s);

	FID * pfid = (FID *)pi;
	*pfid = (FID)_id;//1.д�����ID
	pfid++;

	pi = (int *)pfid;
	len = _name.size();//2.д���������
	*pi = (int)len;
	pi++;
	char * pc = (char*)pi;
	memcpy(pc,_name.c_str(),len);
	pc = pc + len; 

	pi = (int *)pc;
	*pi = _state;//3.д�����״̬
	pi++;
	*pi = _lod;//4.д�����LOD����
	pi++;

	REGID * pregid = (REGID*)pi;
	*pregid = _regid;//5.д�����ID
	pregid++;

	GDBTIME * pGdbTime = (GDBTIME*)pregid;
	*pGdbTime = _createTime;//6.д����󴴽�ʱ��
	pGdbTime++;
	*pGdbTime = _updateTime;//7.д��������ʱ��
	pGdbTime++;

	//8.д�����LOD��Ϣ
	Buffer lbuf;
	 _lod_info->writeBuffer(lbuf);
	len = lbuf.size();
	pi = (int*)pGdbTime;
	*pi = (int)len;
	pi++;
	pc = (char*)pi;
	memcpy(pc,lbuf.data(),len);
	pc=pc+len;

	//9.д��Ҫ������ID
	FCLSID * pfcid = (FCLSID*)pc;
	*pfcid = _fclsid;
	pfcid++;

	//10.���ζ�����Ϣ����
	size_t nSize = 0;
	if (_geom) {
		nSize = _geom->sizeBuffer();	
		pi = (int*)pfcid;
		*pi = (int)nSize;
		pi++;

		//11.д�뼸����Ϣ  [sunka, 2010-3-2]
		pc = (char*)pi;
		Buffer gbuf;
		gbuf.attach(pc, nSize);
		_geom->writeBuffer(gbuf);
		gbuf.detach(nSize);

		pc=pc+nSize;
	}
	else{
		pi = (int*)pfcid;
		*pi = (int)nSize;
		pi++;

		pc = (char*)pi;
	}	

	////12.���˶�����Ϣ
	//Buffer tbuf;
	//_topo->writeBuffer(tbuf);
	//len = tbuf.size();
	//pi = (int*)pc;
	//*pi = (int)len;
	//pi++;
	//pc = (char*)pi;
	//memcpy(pc,tbuf.data(),len);
	//pc=pc+len;

	//13.Ҫ�����ͱ���
	pi = (int*)pc;
	len = _code.size();
	*pi = len;
	pi++;

	pc = (char*)pi;
	memcpy(pc,_code.c_str(),len);
	pc = pc + len; 

	//14.������Ϣ
	pi = (int*)pc;
	*pi = _props.size();
	pi++;
	for(std::vector<std::string>::iterator it = _props.begin();it!=_props.end();it++){
		len = it->size();
		*pi = (int)len;
		pi++;
		pc = (char*)pi;
		memcpy(pc,it->c_str(),len);
		pc = pc + len;
		pi = (int*)pc;
	}
}

void FeatureImpl::readBufferNoProp(const Buffer & buf){

	int len = 0;

	FID * pfid = (FID *)(((Buffer&)buf).data());
	_id = (FID)*pfid;//1.��������ID
	pfid++;

	//2.������������
	int *pi = (int *)pfid;
	len = *pi;
	pi++;
	char * pc = (char*)pi;
	_name.assign(pc,pc+len);
	pc += len;

	pi = (int *)pc;
	_state = *pi;//3.��������״̬
	pi++;
	_lod = *pi;//4.��������LOD����
	pi++;

	REGID * pregid = (REGID*)pi;
	_regid = *pregid;//5.��������ID
	pregid++;

	GDBTIME * pGdbTime = (GDBTIME*)pregid;
	_createTime = *pGdbTime;//6.�������󴴽�ʱ��
	pGdbTime++;
	_updateTime = *pGdbTime;//7.�����������ʱ��
	pGdbTime++;

	//8.��������LOD��Ϣ
	pi = (int*)pGdbTime;
	len = *pi;
	pi++;
	pc = (char*)pi;
	Buffer lbuf;
	lbuf.set(pc,(size_t)len,true);
	_lod_info->readBuffer(lbuf);
	pc=pc+len;

	//9.����Ҫ��������Ϣ
	FCLSID * pfcid = (FCLSID*)pc;
	_fclsid = *pfcid;
	pfcid++;

	//10.�������ζ�����Ϣ����
	pi = (int*)pfcid;
	len = *pi;
	pi++;

	//11.������ȴ���0  [sunka, 2010-3-2]
	if (len>0) {
		//����Geometry������  [sunka, 2010-3-2]
		int nTypeID = *pi;
		//����һ���������Ͳ�ʵ�ֶ�ȡ  [sunka, 2010-3-1]
		_geom = GeometrySharedPtr(Geometry::create(nTypeID));

		pc = (char*)pi;
		Buffer gbuf;
		size_t s = (size_t)len;
		gbuf.attach(pc,s);
		_geom->readBuffer(gbuf);
		gbuf.detach(s);

		pc=pc+len;
	}
	else{
		pc=(char*)pi;
	}	

	////12.�������˶�����Ϣ
	//pi = (int*)pc;
	//len = *pi;
	//pi++;
	//pc = (char*)pi;
	//Buffer tbuf;
	//tbuf.set(pc,(size_t)len,true);
	//_topo->readBuffer(tbuf);
	//pc=pc+tbuf.size();	

	//13.Ҫ�����ͱ���
	pi = (int*)pc;
	len = *pi;
	pi++;
	pc = (char*)pi;
	//������Ȳ�Ϊ0���򿽱������򲻿���  [sunka, 2009-10-24]
	if (len) {
		_code.assign(pc,pc+len);
	}
	else{
		_code = "";
	}
}

//�ж�ָ��LOD�����Geometry�����Ƿ��Ѿ�����  [sunka, 2009-8-27]
bool FeatureImpl::isExist(FLOD flod){
	LODInfo::InfoArray & infs = _lod_info->getLods();
	for (LODInfo::InfoArray::iterator itLevel = infs.begin(); itLevel != infs.end();itLevel++)
	{
		if (itLevel->first.lodLevel == flod && itLevel->second)
		{
			return true;
		}
	}
	return false;
}


void FeatureImpl::writeBufferEndProp(Buffer & buf){

	size_t s = sizeBuffer();
	int len =0;
	int * pi = (int *) new unsigned char [s];
	buf.attach(pi,s);

	FID * pfid = (FID *)pi;
	*pfid = (FID)_id;//1.д�����ID
	pfid++;

	pi = (int *)pfid;
	len = _name.size();//2.д���������
	*pi = (int)len;
	pi++;
	char * pc = (char*)pi;
	memcpy(pc,_name.c_str(),len);
	pc = pc + len; 

	pi = (int *)pc;
	*pi = _state;//3.д�����״̬
	pi++;
	*pi = _lod;//4.д�����LOD����
	pi++;

	REGID * pregid = (REGID*)pi;
	*pregid = _regid;//5.д�����ID
	pregid++;

	GDBTIME * pGdbTime = (GDBTIME*)pregid;
	*pGdbTime = _createTime;//6.д����󴴽�ʱ��
	pGdbTime++;
	*pGdbTime = _updateTime;//7.д��������ʱ��
	pGdbTime++;

	//8.д�����LOD��Ϣ
	Buffer lbuf;
	_lod_info->writeBuffer(lbuf);
	len = lbuf.size();
	pi = (int*)pGdbTime;
	*pi = (int)len;
	pi++;
	pc = (char*)pi;
	memcpy(pc,lbuf.data(),len);
	pc=pc+len;

	//9.д��Ҫ������ID
	FCLSID * pfcid = (FCLSID*)pc;
	*pfcid = _fclsid;
	pfcid++;

	//10.���ζ�����Ϣ����
	size_t nSize = 0;
	if (_geom) {
		nSize = _geom->sizeBuffer();	
		pi = (int*)pfcid;
		*pi = (int)nSize;
		pi++;

		//11.д�뼸����Ϣ  [sunka, 2010-3-2]
		pc = (char*)pi;
		Buffer gbuf;
		gbuf.attach(pc, nSize);
		_geom->writeBuffer(gbuf);
		gbuf.detach(nSize);

		pc=pc+nSize;
	}
	else{
		pi = (int*)pfcid;
		*pi = (int)nSize;
		pi++;

		pc = (char*)pi;
	}	

	////12.���˶�����Ϣ
	//size_t topoSize = 0;
	//if (_topo) {
	//	topoSize = _topo->sizeBuffer();	
	//	pi = (int*)pc;;
	//	*pi = (int)topoSize;
	//	pi++;

	//	//13.д��������Ϣ  [sunka, 2010-3-2]
	//	pc = (char*)pi;
	//	Buffer tbuf;
	//	tbuf.attach(pc, topoSize);
	//	_topo->writeBuffer(tbuf);
	//	tbuf.detach(topoSize);

	//	pc=pc+topoSize;
	//}
	//else{
	//	pi = (int*)pc;
	//	*pi = (int)topoSize;
	//	pi++;

	//	pc = (char*)pi;
	//}	

	//14.Ҫ�����ͱ���
	pi = (int*)pc;
	len = _code.size();
	*pi = len;
	pi++;

	pc = (char*)pi;
	memcpy(pc,_code.c_str(),len);
	pc = pc + len; 

}

//��ȡָ��LOD�����Gometry����  [sunka, 2009-8-28]
GeometrySharedPtr FeatureImpl::getLodGeom(FLOD flod){
	LODInfo::InfoArray & infs = _lod_info->getLods();
	for (LODInfo::InfoArray::iterator itLevel = infs.begin(); itLevel != infs.end();itLevel++)
	{
		if (itLevel->first.lodLevel == flod && itLevel->second)
		{
			return itLevel->second;
		}
	}
	return GeometrySharedPtr();
}

//���ݴ�����������ɱ任������ɶ���ļ��α任
void FeatureImpl::transferCoordinate(double dx, double dy, double dz)
{
	Vertex3d v3d;
	v3d.x = dx;
	v3d.y = dy;
	v3d.z = dz;

	transferCoordinate(v3d);

}
void FeatureImpl::transferCoordinate(Vertex3d& v3d)
{
	Matrix4x4 m4x4 ;
	m4x4.identity();
	//��ƽ����������任����
	m4x4.buildTranslateMatrix(v3d);

	transferCoordinate(m4x4);
}
void FeatureImpl::transferCoordinate(Matrix4x4& m4x4)
{
	_geom->translateFrom(m4x4);
}

const std::vector<std::string>& FeatureImpl::getProps() const
{ 
	return _props;
}

std::vector<std::string>& FeatureImpl::getProps() 
{ 
	return _props;
}

void FeatureImpl::getProps( std::vector<std::string> & sa) {
	sa.resize(_props.size());
	std::copy(_props.begin(),_props.end(),sa.begin());
}

////����ʹ�����ã������ж�
//
//int FeatureImpl::setPropSize(){
//	int nSize = getFeatureClass()->getPropTypes().size();
//	_props.resize(nSize,"");
//	return nSize;
//}


void FeatureImpl::setProps(std::vector<std::string>& prop){
	FeatureClassSharedPtr fc = getFeatureClass();
	std::vector<PropType> & ptv = fc->getPropTypes();
	if (ptv.size()==prop.size())  //�����ֶδ��ڲ���ƥ�������feature����prop����
	{
		int length = 50;
		for(std::vector<PropType>::iterator it = ptv.begin();it != ptv.end();it++){
			length = (*it).getLength();
		}
		_props.clear();  //���prop�ڵ�����
		for (std::vector<std::string>::iterator it1 = prop.begin();it1!=prop.end();it1++){
			int rlength = (*it1).size(); //ʵ�ʳ���
			if (rlength<length)
			{
				_props.push_back(*it1);
			} 
			else
			{
				_props.push_back("null");
			}			
		}
	}
}

bool FeatureImpl::hasSemantic(){
	bool hassemantic = false;
	if(_state & 0x80)//Ҫ�ذ�������
	{
		hassemantic = true;
	}

	return hassemantic;
}

void FeatureImpl::hasSemantic(bool hassemantic){
	if(hassemantic)//Ҫ�ذ�������
	{
		//�˴���������������ṩ���ģ�������[2010-03-29]
		_state = _state | 0x80;
	}
	else
	{
		//�˴���������������ṩ���ģ�������[2010-03-29]
		_state = _state & ~0x80;
	}
}


/** ����Ҫ�ص����Χ��
* @return 
*/
void FeatureImpl::setEnvelope(const Envelope3dRef env3d) { 
	_geom->setEnvelope(env3d); 
}

void FeatureImpl::getEnvelope(double * pv) {
	if (_geom) {
		Envelope3d d;
		_geom->getEnvelope(d);
		
		pv[0]=d.minx; pv[1]=d.maxx;
		pv[2]=d.miny; pv[3]=d.maxy;
		pv[4]=d.minz; pv[5]=d.maxz;
	}		
}

void FeatureImpl::setEnvelope(double pv[6]){
	if (_geom) {
		Envelope3d d;
		

		d.minx=pv[0]; d.maxx=pv[1];
		d.miny=pv[2]; d.maxy=pv[3];
		d.minz=pv[4]; d.maxz=pv[5];
		_geom->setEnvelope(d);
	}
}

// ���������Geometry��Ȼ��󶨵���ǰfeature�� [2009-8-25 sunka add notes]
void FeatureImpl::setGeometry(GeometrySharedPtr g){
	_geom = g;
}

bool FeatureImpl::attatchGeometry (Geometry * g){
	if(!g){
		return false;
	}
	_geom.reset(g);
	return true;
}

bool FeatureImpl::getPropAccessFlag(){return _propreaded;}
void FeatureImpl::setPropAccessFlag(bool propreaded){_propreaded = propreaded;}
GDBTIME FeatureImpl::getCreateTime(){return _createTime;}
void FeatureImpl::setCreateTime(GDBTIME v){_createTime=v;}
GDBTIME FeatureImpl::getUpdateTime(){return _updateTime;}
void FeatureImpl::setUpdateTime(GDBTIME v){_updateTime=v;}
int FeatureImpl::getStatus(){return _state;}
void FeatureImpl::setStatus(int v){_state=v;}

GeometrySharedPtr FeatureImpl::geometry(){ return _geom;}
void FeatureImpl::setFeatureClass(FeatureClassSharedPtr fc) {_fclsid=fc->getID();}
void FeatureImpl::setFeatureClassID(FCLSID fclsid) {_fclsid=fclsid;}
FeatureClassSharedPtr FeatureImpl::getFeatureClass() { return FeatureClass::findFeatureClass(_fclsid);}

unsigned long FeatureImpl::getBufferSize(){ return _buffersize;} //�˺���������ʹ��
void  FeatureImpl::setBufferSize(unsigned long buffersize ){ _buffersize=buffersize;} //�˺���������ʹ��
void FeatureImpl::setID(FID id) { _id=id;}
FID FeatureImpl::getID() {return _id;}	
std::string FeatureImpl::getName() {return _name;}
void FeatureImpl::setName(std::string v) { _name=v;}
int FeatureImpl::getLODLevels(){return _lod_info->getLods().size()/*_lod*/;}
void FeatureImpl::setLODLevels(int v){_lod=v;}
LODInfoSharedPtr FeatureImpl::getLODInfo(){return _lod_info;}
void FeatureImpl::setLODInfo(LODInfoSharedPtr v){_lod_info=v;}
REGID FeatureImpl::getWorkspaceID(){return _regid;}
void FeatureImpl::setWorkspaceID(REGID v){_regid=v;}
void FeatureImpl::setCode(string szCode){_code = szCode;}
std::string FeatureImpl::getCode(){return _code;}
FCLSID FeatureImpl::getFeatureClassID(){return _fclsid;}

void FeatureImpl::copy(FeatureSharedPtr sp){
	copy(*((FeatureImpl*)sp.get()));
}
  

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
