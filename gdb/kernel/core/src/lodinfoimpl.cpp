#include "internal_loadinfoimpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
LODInfoImpl::LODInfoImpl(){
	_fid = 0;//Ҫ�ض���ID
	_cur_level = 0;
}

LODInfoImpl::~LODInfoImpl(){

	// ���infos
	_infos.clear();
}

bool LODInfoImpl::addInfo(const LOD_INFO &lodinfo, GeometrySharedPtr g){
	if (g)	{
		// �������������û�иü���Ķ���������ɾ����û����ֱ�Ӳ���
		for (std::vector<Info>::iterator it = _infos.begin(); it < _infos.end(); it++) {
			if (it->first.lodLevel == lodinfo.lodLevel) {

				// �ٴ�vector�������λ�õ�LOD_INFO
				_infos.erase(it);
				break;
			}
		}

		// �����µ�Lod��Ϣ
		Info info;
		info.first = lodinfo;
		info.second = g;

		// ����ʱ�Ǹ���LOD_PARAM�ж���ļ�����еģ���˴˴�����������
		_infos.push_back(info);

		return true;
	}
	else{
		return false;
	}
}


// ��featureʹ�ã�ֻ��LOD�ļ���ÿ������Ķ�Ӧ�Ĳ�������д���ļ�:����Ϣ����Geometry������
size_t LODInfoImpl::sizeBuffer(){

	// ����ĸ���
	int nLevels = getLevels();
	// ��¼ռ�ÿռ�ĳ���
	size_t s = sizeof(int)//��������ռ�
		+ sizeof(LOD_INFO) * nLevels;

	return s;
}
bool LODInfoImpl::writeBuffer(Buffer& buf){

	// ����ĸ��� 
	int nLevels = getLevels();
	// ��¼ռ�ÿռ�ĳ���
	size_t s = sizeBuffer();

	int * pi = (int *) new unsigned char[s];
	if (pi == NULL)/**�����벻���㹻�ռ䣬����false ����PY add2009.12.22*/
	{
		return false;
	}
	buf.attach(pi, s);

	*pi = (int)nLevels;//д�뼶�����
	pi++;

	LOD_INFO *pInf = (LOD_INFO*)pi;
	//��ʼд����ز�����Ϣ
	for (std::vector<Info>::iterator it = _infos.begin(); it != _infos.end(); it++){
		*pInf = it->first;
		pInf++;
	}

	//д��������Ϣ
	return true;
}

bool LODInfoImpl::readBuffer(const Buffer & buf){
	if (0 == buf.size())/**��bufΪ�գ�����false ����PY add2009.12.22*/
	{
		return false;
	}
	int * pi = (int *)(((Buffer&)buf).data());
	int nSize = (int)*pi;//����LOD�ļ���
	pi++;

	_infos.resize(nSize);//���ô洢�����ĸ���

	LOD_INFO *pInf = (LOD_INFO*)pi;
	for (std::vector<Info>::iterator it = _infos.begin(); it != _infos.end(); it++) {
		it->first = *pInf;
		pInf++;
	}

	//����������Ϣ
	return true;
}

// ��LOD�������Ϣд���ļ�������Ϣ�洢��LOD�ļ���
void LODInfoImpl::write(std::ostream & f){

	// ��д������һ����һ��һ�е�д
	for (std::vector<Info>::iterator it = _infos.begin(); it != _infos.end(); it++){
		// д������ID
		f.write((char*)(&_fid), sizeof(FID));

		// д������LOD����
		f.write((char*)(&(it->first.lodLevel)), sizeof(FID));

		// д��ռ�����
		GeometrySharedPtr pGom = it->second;

		// ������ڣ���д��
		int nHasGeom = 0;
		int nGeoType = 0;
		if (pGom){
			// �ռ������Ƿ����
			nHasGeom = 1;
			f.write((char*)(&(nHasGeom)), sizeof(int));
			nGeoType = pGom->getType();
			f.write((char*)(&(nGeoType)), sizeof(int));
			pGom->write(f);
		}
		else{ // �ռ����ݲ�����
			nHasGeom = 0;
			f.write((char*)(&(nHasGeom)), sizeof(int));
		}
	}
}

// LOD��Read������������Ҫ���ж��룬��ʱ���ṩ
void LODInfoImpl::read(std::istream & f){

	//	//��������Ķ���Id
	//	f.read((char*)(&_fid),sizeof(FID));
	//
	//	//�����ܹ��ļ���
	//	int nLevles = 0;
	//	f.read((char*)(&nLevles),sizeof(int));
	//	_infos.resize(nLevles);
	//
	//	// �Ƿ�ӵ�пռ����ݿ�
	//	int nHasGeom = 0;
	//	// �ռ����ݵļ�������
	//#ifndef USE_WHU_GEOMETRY
	//
	//	GEOM_TYPE nGeoType = GEOM_TYPE_UNKNOWN;
	//#else
	//	int nGeoType = 0;
	//#endif //USE_WHU_GEOMETRY
	//
	//	//д��ÿ����Ӧ�ľ���Ϳռ�����
	//	for(std::vector<Info>::iterator it = _infos.begin();it!=_infos.end();it++){
	//		f.read((char*)(&(it->first)),sizeof(double)); //����
	//		f.read((char*)(&(nHasGeom)),sizeof(int));
	//
	//		// ��ȡ�ռ�����
	//		if (nHasGeom){			
	//			f.read((char*)(&(nGeoType)),sizeof(int));
	//#ifndef USE_WHU_GEOMETRY
	//			Geometry *pGeom = Geometry::newGeometry((GEOM_TYPE)nGeoType);
	//#else
	//			ct3d::Geometry *pGeom = ct3d::Geometry::newGeometry((int)nGeoType);
	//#endif //USE_WHU_GEOMETRY
	//
	//
	//			pGeom->read(f);
	//			it->second = pGeom;
	//		}		
	//	}	
}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
