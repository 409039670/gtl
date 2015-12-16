#include "internal_featureclassimpl.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
FeatureClassImpl::FeatureClassImpl(){
	_id = Identifier::generate();
	_name = "";
	_code = "";
	_srefid = 0;
	//_gTab="";  
	//_aTab="";  
	_partitionnum = 0;
	_fclstype = 0;
}
FeatureClassImpl::FeatureClassImpl(std::string name, std::string code, int  geomtype, SREFID srefid, std::string gTab, std::string aTab, int partitionnum){
	_id = Identifier::generate();
	_name = name;
	_code = code;
	//_geom=geomtype;
	_srefid = srefid;
	/*_gTab=gTab;
	_aTab=aTab;  */
	_partitionnum = partitionnum;
	_fclstype = 0;
}
FeatureClassImpl::FeatureClassImpl(FCLSID id, std::string name, std::string code, int geomtype, SREFID srefid, std::string gTab, std::string aTab, int partitionnum) {
	_id = id;
	_name = name;
	_code = code;
	//_geom=geomtype;
	_srefid = srefid;
	//_gTab=gTab;  
	//_aTab=aTab;  
	_partitionnum = partitionnum;
	_fclstype = 0;
}
FeatureClassImpl::FeatureClassImpl(const FeatureClassImpl & fc){
	copy(fc);
}
void FeatureClassImpl::copy(const FeatureClassImpl & fc){
	_id = fc._id;
	_name = fc._name;
	_code = fc._code;
	//_geom=fc._geom;
	_srefid = fc._srefid;
	/*_gTab=fc._gTab;
	_aTab=fc._aTab;  */
	_partitionnum = fc._partitionnum;
	_proptypes.resize(fc._proptypes.size());
	std::copy(fc._proptypes.begin(), fc._proptypes.end(), _proptypes.begin());
	_localfeatureset = fc._localfeatureset;
	_fclstype = fc._fclstype;
}
FeatureClassImpl FeatureClassImpl::operator=(const FeatureClassImpl & fc){
	copy(fc);
	return *this;
}
void FeatureClassImpl::addPropType(const PropType & pt){
	_proptypes.insert(_proptypes.end(), pt);
}
void FeatureClassImpl::addPropType(std::string name, std::string code, std::string type, int length, int decimal, bool nullable){
	_proptypes.insert(_proptypes.end(), PropType(name, code, type, length, decimal, nullable));
}

void FeatureClassImpl::removePropType(std::string name){
	for (std::vector<PropType>::iterator it = _proptypes.begin(); it<_proptypes.end(); it++){
		if (name == it->getName()){
			_proptypes.erase(it);
			break;
		}
	}
}

bool FeatureClassImpl::operator==(const FeatureClassImpl & fc)
{
	bool bRet = false;
	// �ж�Ҫ����������Ϣ�Ƿ����
	if (_name == fc._name && _code == fc._code /*&& _geom == fc._geom*/ && _srefid == fc._srefid){
		bRet = true;
	}
	else{
		return false;
	}

	// �ж�Ҫ���������ֶ��Ƿ����
	if (_proptypes.size() != fc._proptypes.size())
	{
		return false;
	}

	size_t nPos = 0;
	bool bVal = true;
	for (std::vector<PropType>::iterator it = _proptypes.begin(); it<_proptypes.end(); it++)
	{
		if ((*it) == fc._proptypes[nPos]){
			bVal = true;
		}
		else{
			return false;
		}

		nPos++;
	}

	// ����
	return (bRet && bVal);
}


void FeatureClassImpl::write(std::ostream & f){


	f.write((char*)(&_id), sizeof(FCLSID));
	int s = _name.size();
	f.write((char*)(&s), sizeof(int));
	f.write((char*)(_name.c_str()), s);
	s = _code.size();
	f.write((char*)(&s), sizeof(int));
	f.write((char*)(_code.c_str()), s);
	//ɾ��Ҫ���������  [sunka, 2010-3-1]
	//f.write((char*)(&_geom),sizeof(int));
	f.write((char*)(&_srefid), sizeof(SREFID));
	//ɾ��������  [sunka, 2010-3-1]
	/*s = _gTab.size();
	f.write((char*)(&s),sizeof(int));
	f.write((char*)(_gTab.c_str()),s);
	s = _aTab.size();
	f.write((char*)(&s),sizeof(int));
	f.write((char*)(_aTab.c_str()),s);*/

	f.write((char*)(&_partitionnum), sizeof(int));

	s = _proptypes.size();
	f.write((char*)(&s), sizeof(int));

	for (std::vector<PropType>::iterator it = _proptypes.begin(); it != _proptypes.end(); it++){
		it->write(f);
	}

	//Ҫ���������ڵĹ���
	f.write((char*)(&_prjID), sizeof(PRJID));

	//��ʶҪ������Ϊ���ϻ��ǵ�������
	f.write((char*)(&_fclstype), sizeof(int));

	//Ҫ�����Ͷ�ӦҪ�ؼ��Ϲ�������
	f.write((char*)(&_fsettype), sizeof(int));

	//Ҫ�����Ͷ�Ӧ�������ݱ��������
	f.write((char*)(&_fsetproptype), sizeof(int));
}
void FeatureClassImpl::read(std::istream & f){

	f.read((char*)(&_id), sizeof(FCLSID));
	int s = 0;
	char ca[255];
	f.read((char*)(&s), sizeof(int));
	f.read((char*)(ca), s);
	_name.assign(ca, ca + s);

	f.read((char*)(&s), sizeof(int));
	f.read((char*)(ca), s);
	_code.assign(ca, ca + s);
	//ɾ��Ҫ���������  [sunka, 2010-3-1]
	//f.read((char*)(&_geom),sizeof(int));
	f.read((char*)(&_srefid), sizeof(SREFID));

	//ɾ��_gTab  [sunka, 2010-3-1]
	//f.read((char*)(&s),sizeof(int));
	//f.read((char*)(ca),s);
	//_gTab.assign(ca,ca+s);

	//f.read((char*)(&s),sizeof(int));
	//f.read((char*)(ca),s);
	//_aTab.assign(ca,ca+s);

	f.read((char*)(&_partitionnum), sizeof(int));


	f.read((char*)(&s), sizeof(int));
	_proptypes.resize(s);
	for (std::vector<PropType>::iterator it = _proptypes.begin(); it != _proptypes.end(); it++){
		it->read(f);
	}

	f.read((char*)(&_prjID), sizeof(PRJID));	//Ҫ���������ڵĹ���

	f.read((char*)(&_fclstype), sizeof(int));//��ʶҪ������Ϊ���ϻ��ǵ�������

	f.read((char*)(&_fsettype), sizeof(int));//Ҫ�����Ͷ�ӦҪ�ؼ��Ϲ�������

	f.read((char*)(&_fsetproptype), sizeof(int));//Ҫ�����Ͷ�Ӧ�������ݱ��������
}

size_t FeatureClassImpl::sizeBuffer()
{
	size_t s = sizeof(FCLSID) //Ҫ������id
		+ 4              //Ҫ����������ռλ
		+ _name.size()   //Ҫ����������
		+ 4              //Ҫ�����ͱ���ռλ
		+ _code.size()   //Ҫ�����ͱ���
		//ɾ��Ҫ���������  [sunka, 2010-3-1]
		//+ sizeof(int)    //Ҫ�����Ͷ�Ӧ�ļ�������
		+ sizeof(SREFID) //Ҫ�����Ͷ�Ӧ�ռ�ο�ϵͳid
		//+ 4              //Ҫ�����͵ļ��α���������ռλ
		//+ _gTab.size()   //Ҫ�����͵ļ��α���������
		//+ 4              //Ҫ�����͵����Ա���������ռλ
		//+ _aTab.size()   //Ҫ�����͵����Ա���������
		+sizeof(int)    //Ҫ�����Ͷ�Ӧ��Ҫ�ؼ��ϱ�ķ�������
		+ sizeof(PRJID)  //Ҫ���������ڵĹ���
		+ sizeof(int)    //��ʶҪ������Ϊ���ϻ��ǵ�������
		+ sizeof(int)    //Ҫ�����Ͷ�ӦҪ�ؼ��Ϲ�������
		+ sizeof(int);   //Ҫ�����Ͷ�Ӧ�������ݱ��������
	return s;

}
void FeatureClassImpl::writeBuffer(Buffer &buf)
{
	size_t s = sizeBuffer();
	size_t len = 0;
	char *pi = (char *) new unsigned char[s];
	buf.attach(pi, s);
	char *ptr = pi;

	*(FCLSID*)ptr = _id;
	ptr += sizeof(FCLSID);

	*(int *)ptr = _name.size();
	ptr += 4;
	memcpy(ptr, _name.c_str(), _name.size());
	ptr += _name.size();

	*(int *)ptr = _code.size();
	ptr += 4;
	memcpy(ptr, _code.c_str(), _code.size());
	ptr += _code.size();

	//ɾ��Ҫ���������  [sunka, 2010-3-1]
	/**(int *)ptr = _geom;
	ptr += 4;*/

	*(SREFID*)ptr = _srefid;
	ptr += sizeof(SREFID);

	//ɾ��_gTab  [sunka, 2010-3-1]
	/**(int *)ptr = _gTab.size();
	ptr += 4;
	memcpy(ptr, _gTab.c_str(), _gTab.size());
	ptr += _gTab.size();

	*(int *)ptr = _aTab.size();
	ptr += 4;
	memcpy(ptr, _aTab.c_str(), _aTab.size());
	ptr += _aTab.size();*/

	*(int *)ptr = _partitionnum;
	ptr += 4;

	*(PRJID*)ptr = _prjID;
	ptr += sizeof(PRJID);

	*(int *)ptr = _fclstype;
	ptr += 4;

	*(int*)ptr = _fsettype;
	ptr += 4;

	*(int*)ptr = _fsetproptype;
	ptr += 4;

}

void FeatureClassImpl::readBuffer(Buffer &buf)
{
	int lenName, lenCode;
	char name[255], code[255];
	buf.read((char*)&_id, sizeof(FCLSID));

	buf.read((char*)&lenName, sizeof(int));
	buf.read((char*)name, lenName);
	_name.assign(name, name + lenName);

	buf.read((char*)&lenCode, sizeof(int));
	buf.read((char*)code, lenCode);
	_code.assign(code, code + lenCode);
	//ɾ��Ҫ���������  [sunka, 2010-3-1]
	//buf.read((char*)&_geom, sizeof(int));
	buf.read((char*)&_srefid, sizeof(SREFID));

	//ɾ��gTab  [sunka, 2010-3-1]
	/*buf.read((char*)&lenGTab, sizeof(int));
	buf.read((char*)gTab, lenGTab);
	_gTab.assign(gTab, gTab+lenGTab);

	buf.read((char*)&lenATab, sizeof(int));
	buf.read((char*)aTab, lenATab);
	_aTab.assign(aTab, aTab+lenATab);*/

	buf.read((char*)&_partitionnum, sizeof(int));
	buf.read((char*)&_prjID, sizeof(PRJID));
	buf.read((char*)&_fclstype, sizeof(int));
	buf.read((char*)&_fsettype, sizeof(int));
	buf.read((char*)&_fsetproptype, sizeof(int));
}

//Ҫ�����ͱ��
FCLSID FeatureClassImpl::getID(){ return _id; }
void  FeatureClassImpl::setID(FCLSID id){ _id = id; }

//Ҫ����������
std::string FeatureClassImpl::getName(){ return _name; }
void  FeatureClassImpl::setName(std::string v){ _name = v; }

//Ҫ�����ͱ���
std::string FeatureClassImpl::getCode(){ return _code; }
void  FeatureClassImpl::setCode(std::string v){ _code = v; }

//�ռ�ο�ϵͳID
void  FeatureClassImpl::setSpatialReferenceID(SREFID srefID){ _srefid = srefID; }
SREFID  FeatureClassImpl::getSpatialReferenceID(){ return _srefid; }

//���α�ķ�������
void  FeatureClassImpl::setPartitionNumber(int v){ _partitionnum = v; }
int  FeatureClassImpl::getPartitionNumber(){ return _partitionnum; }

//��Ӧ�������ֶμ���
std::vector<PropType> & FeatureClassImpl::getPropTypes(){ return _proptypes; }

//Ҫ���������ڵĹ���
PRJID FeatureClassImpl::getPrjID(){ return _prjID; }
void  FeatureClassImpl::setPrjID(PRJID prjID){ _prjID = prjID; }

//����Ҫ�����״̬
int FeatureClassImpl::getFclsType(){ return _fclstype; }
void  FeatureClassImpl::setFclsType(int fclstype){ _fclstype = fclstype; }
/**��ȡҪ�����͵�Ҫ�ؼ��Ϲ�������
* @param [in]
* @return  void
*/
int FeatureClassImpl::getFsetType() { return _fsettype; }

/**����Ҫ�����͵�Ҫ�ؼ��Ϲ�������
* @param [in] v  Ҫ�����͵�Ҫ�ؼ��Ϲ�������
* @return  void
*/
void FeatureClassImpl::setFsetType(int v) { _fsettype = v; }

/**��ȡҪ�����͵������ֶν�������
* @return
*/
int FeatureClassImpl::getFsetPropType() { return _fsetproptype; }

/**����Ҫ�����������ֶν�������
* @param [in] v		�����ֶν�������
* @return
*/
void FeatureClassImpl::setFsetPropType(int v) { _fsetproptype = v; }
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
