#include "config.h"
#include "terrain.h"
#include "factory.h"
#include "internal_demgridimpl.h"


begin_gtl_namespace
begin_gdb_namespace

double DEMGrid::getDefaultInvalidValue()
{
	return -9999.0;
}

DEMGridSharedPtr DEMGrid::create(){
#if(USING_3DGEOMETRY_TYPE==1)
	return  DEMGridSharedPtr(new DEMGridImpl());
#else
	return ((Geometry3dFactory*)Factory::getFactory(Factory::FACTORYTYPE_3D_GEOMETRY))->newDEMGrid();
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void DEMGrid::fillDEMGrid(DEMGrid::DEM & _dem, DEMGridSharedPtr & grid){

	grid->setOrigin(_dem._Xo, _dem._Yo);
	grid->setHeightRange(_dem._MinV, _dem._MaxV);
	grid->setAlpha(_dem._Alpha);
	grid->setBelta(0);
	grid->setCols(_dem._Col);
	grid->setRows(_dem._Row);
	//��������ϵ��ʾ
	grid->setCoordinateSystem(1);
	//�������굥λ
	grid->setUnit(1);
	grid->setCellSize(max(_dem._DX, _dem._DY));
	if (_dem._Nodata != std::numeric_limits<double>::max())//DBL_MAX
		grid->setInvalidValue(_dem._Nodata);
	else
		grid->setInvalidValue(DEMGrid::getDefaultInvalidValue());
	grid->setScale(_dem._HZoom);
	size_t s = _dem._Row*_dem._Col;
	float * data = new float[s];
	std::copy(_dem._Data, _dem._Data + s, data);
	grid->setHeightData(data, true);
}

void DEMGrid::initial(DEMGrid::DEM & _dem){
	_dem._Alpha = 0.0;
	_dem._Compress = 0.0;
	_dem._DataMark = "GTL-DEM";
	_dem._DX = 1;
	_dem._DY = 1;
	_dem._HZoom = 0;
	_dem._MaxV = - std::numeric_limits<double>::max();//-DBL_MAX;
	_dem._MinV = std::numeric_limits<double>::max();//DBL_MAX;
	_dem._Nodata = std::numeric_limits<double>::max();//DBL_MAX;
	_dem._Unit = "M";
	_dem._ValueType = "Double";
	_dem._Version = "1.0";
	_dem._Xo = 0;
	_dem._Yo = 0;

	_dem._Data = 0;
	_dem._Row = 0;
	_dem._Col = 0;
}
void DEMGrid::initial(DEMGrid::DEM & _dem, long rows, long cols){

	_dem._Alpha = 0.0;
	_dem._Compress = 0.0;
	_dem._DataMark = "GTL-DEM";
	_dem._DX = 1;
	_dem._DY = 1;
	_dem._HZoom = 0;
	_dem._MaxV = -std::numeric_limits<double>::max();//-DBL_MAX;
	_dem._MinV = std::numeric_limits<double>::max();//DBL_MAX;
	_dem._Nodata = std::numeric_limits<double>::max();//DBL_MAX;
	_dem._Unit = "M";
	_dem._ValueType = "Double";
	_dem._Version = "1.0";
	_dem._Xo = 0;
	_dem._Yo = 0;

	_dem._Data = new double[rows*cols];
	_dem._Row = rows;
	_dem._Col = cols;
}
void DEMGrid::copy(DEMGrid::DEM & _dem, const DEMGrid::DEM & _d){
	_dem._Alpha = _d._Alpha;
	_dem._Compress = _d._Compress;
	_dem._DataMark = _d._DataMark;
	_dem._DX = _d._DX;
	_dem._DY = _d._DY;
	_dem._HZoom = _d._HZoom;
	_dem._MaxV = _d._MaxV;
	_dem._MinV = _d._MinV;
	_dem._Nodata = _d._Nodata;
	_dem._Unit = _d._Unit;
	_dem._ValueType = _d._ValueType;
	_dem._Version = _d._Version;
	_dem._Xo = _d._Xo;
	_dem._Yo = _d._Yo;
	_dem._Row = _d._Row;
	_dem._Col = _d._Col;
	_dem._Data = new double[_d._Row*_d._Col];
	memcpy(_dem._Data, _d._Data, sizeof(double)*_d._Row*_d._Col);
}
void DEMGrid::getXYZ(const DEMGrid::DEM & _dem, long r, long c, double & x, double & y, double & z){
	if (_dem._HZoom == 0 || _dem._HZoom == 1){
		x = _dem._Xo + c*_dem._DX;
		y = _dem._Yo + r*_dem._DY;
		z = _dem._Data[r*_dem._Col + c];
	}
	else{
		x = _dem._Xo + c*_dem._DX;
		y = _dem._Yo + r*_dem._DY;
		z = _dem._Data[r*_dem._Col + c];

		x /= _dem._HZoom;
		y /= _dem._HZoom;
		z /= _dem._HZoom;
	}
}
void DEMGrid::getXYZ(const DEMGrid::DEM & _dem, long r, long c, double *xyz){
	if (_dem._HZoom == 0 || _dem._HZoom == 1){
		xyz[0] = _dem._Xo + c*_dem._DX;
		xyz[1] = _dem._Yo + r*_dem._DY;
		xyz[2] = _dem._Data[r*_dem._Col + c];
	}
	else{
		xyz[0] = _dem._Xo + c*_dem._DX;
		xyz[1] = _dem._Yo + r*_dem._DY;
		xyz[2] = _dem._Data[r*_dem._Col + c];
		xyz[0] /= _dem._HZoom;
		xyz[1] /= _dem._HZoom;
		xyz[2] /= _dem._HZoom;
	}
}
void DEMGrid::getXYZ(const DEMGrid::DEM & _dem, double *xyz){
	size_t k = 0;
	if (_dem._HZoom == 0 || _dem._HZoom == 1){
		for (int i = 0; i < _dem._Row; i++){
			for (int j = 0; j < _dem._Col; j++){
				xyz[k++] = _dem._Xo + j*_dem._DX;
				xyz[k++] = _dem._Yo + i*_dem._DY;
				xyz[k++] = _dem._Data[i*_dem._Col + j];
			}
		}
	}
	else{
		for (int i = 0; i < _dem._Row; i++){
			for (int j = 0; j < _dem._Col; j++){
				xyz[k++] = (_dem._Xo + j*_dem._DX) / _dem._HZoom;
				xyz[k++] = (_dem._Yo + i*_dem._DY) / _dem._HZoom;
				xyz[k++] = (_dem._Data[i*_dem._Col + j]) / _dem._HZoom;
			}
		}
	}
}
void DEMGrid::uninitial(DEMGrid::DEM & _dem){
	if (_dem._Data)
		delete[] _dem._Data;
	_dem._Data = 0;
}
/*
NSDTF-DEM��ʽ˵����
DataMark--------�й�����ռ����ݽ�����ʽ-�������ݽ�����ʽ(CNSDTF-RAS��CNSDTF-DEM)�ı�־���������֣�����ȱʡ��
Version--------�ÿռ����ݽ�����ʽ�İ汾��,��1.0���������֣�����ȱʡ��
Alpha--------����ǡ��������֣�����ȱʡ��
Unit--------���굥λ,K��ʾ����,M��ʾ��,D��ʾ�Զ�Ϊ��λ�ľ�γ��,S��ʾ�Զȷ����ʾ�ľ�γ��(��ʱ�����ʽΪDDDMMSS.SSSS, DDDΪ��, MMΪ��, SS.SSSSΪ��)���������֣�����ȱʡ��
Compress--------ѹ��������0��ʾ��ѹ��,1��ʾ�γ̱��롣�������֣�����ȱʡ��
Xo--------���Ͻ�ԭ��X���ꡣ�������֣�����ȱʡ��
Yo--------���Ͻ�ԭ��Y���ꡣ�������֣�����ȱʡ��
DX--------X����ļ�ࡣ�������֣�����ȱʡ��
DY--------Y����ļ�ࡣ�������֣�����ȱʡ��
Row--------�������������֣�����ȱʡ��
Col--------�������������֣�����ȱʡ��
HZoom--------�̷߳Ŵ��ʡ��������֣�����ȱʡ�����ø̵߳ķŴ���,ʹ�߳����ݿ�����������,��߳̾��Ⱦ�ȷ������,�̵߳ķŴ���Ϊ100���������DEM��HZoomΪ1��
MinV--------��ѡ���֣���Сֵ��
MaxV--------��ѡ���֣����ֵ��
*/
bool DEMGrid::readCNSDTF(std::string filename, DEMGrid::DEM & _dem){
	std::locale::global(std::locale(""));

	std::ifstream _demfile;		 //DEM�ļ���
	_demfile.open(filename.c_str()); //��ȡ�ļ�
	std::string strtmp;

	if (_demfile){
		initial(_dem);
		_demfile >> strtmp >> _dem._DataMark;
		_demfile >> strtmp >> _dem._Version;
		_demfile >> strtmp >> _dem._Alpha;
		_demfile >> strtmp >> _dem._Unit;
		_demfile >> strtmp >> _dem._Compress;
		_demfile >> strtmp >> _dem._Xo;
		_demfile >> strtmp >> _dem._Yo;
		_demfile >> strtmp >> _dem._DX;
		_demfile >> strtmp >> _dem._DY;
		_demfile >> strtmp >> _dem._Row;
		_demfile >> strtmp >> _dem._Col;
		_demfile >> strtmp >> _dem._ValueType;
		_demfile >> strtmp >> _dem._HZoom;
		_demfile >> strtmp;//>> _dem._MinV;
		_demfile >> strtmp;//>> _dem._MaxV;

		trim(strtmp, ' ');
		if (strtmp.compare("MaxV:") != 0)	{//˵��MinV��MaxVд������Ӧ��ֵ����ȡMaxV�е������ַ���
			_demfile >> strtmp;
			_demfile >> strtmp;
		}
	}
	else
		return false;

	_dem._Data = new double[_dem._Row*_dem._Col];
	double _val;
	int i, j;
	for (j = _dem._Row - 1; j >= 0; j--)	{		//���ڴ��ļ���¼Y��ֵ�ķ�����������ͬ����Ҫ��ת����
		for (i = 0; i<_dem._Col; i++){

			_demfile >> _val;
			if (_dem._HZoom != 0)
				_val = _val / _dem._HZoom;	//ԭʼ����������

			if (_val != _dem._Nodata){		//������ dNodata
				if (_dem._MinV > _val) _dem._MinV = _val;//��¼�߳��ϵ���С��ֵ
				if (_dem._MaxV < _val) _dem._MaxV = _val;//��¼�߳��ϵ������ֵ
			}
			_dem._Data[j*_dem._Col + i] = _val;
		}
	}
	return true;
}
bool DEMGrid::writeCNSDTF(const DEMGrid::DEM & _dem, std::string filename){
	std::locale::global(std::locale(""));

	std::ofstream _demfile;		 //DEM�ļ���
	_demfile.open(filename.c_str()); //��ȡ�ļ�
	std::string strtmp;

	if (_demfile){

		_demfile << "DataMark: CNSDTF-DEM" << std::endl;
		_demfile << "Version: 1.0" << std::endl;
		_demfile << "Alpha: " << _dem._Alpha << std::endl;
		_demfile << "Unit: " << _dem._Unit << std::endl;
		_demfile << "Compress: " << _dem._Compress << std::endl;
		_demfile << "X0: " << _dem._Xo << std::endl;
		_demfile << "Y0: " << _dem._Yo << std::endl;
		_demfile << "DX: " << _dem._DX << std::endl;
		_demfile << "DY: " << _dem._DY << std::endl;
		_demfile << "Row: " << _dem._Row << std::endl;
		_demfile << "Column: " << _dem._Col << std::endl;
		_demfile << "ValueType: " << _dem._ValueType << std::endl;
		_demfile << "Hzoom: " << _dem._HZoom << std::endl;
		_demfile << "MinV: " << _dem._MinV << std::endl;
		_demfile << "MaxV: " << _dem._MaxV << std::endl;
	}
	else
		return false;


	int i, j, k = 0;
	for (j = _dem._Row - 1; j >= 0; j--)	{		//���ڴ��ļ���¼Y��ֵ�ķ�����������ͬ����Ҫ��ת����
		for (i = 0; i < _dem._Col; i++){
			k = j*_dem._Col + i;
			_demfile << _dem._Data[k] << " ";
		}
		_demfile << std::endl;
	}
	return true;
}
/*ArcGIS��GRIDת���ı��ļ���ʽ˵���� �ִ�5

ncols         319----------------��������
nrows         198----------------��������
xllcorner     131985----------------�������½ǵ�Xֵ
yllcorner     19020----------------�������½ǵ�Yֵ
cellsize      5----------------���ݷֱ��ʣ�դ��Ԫ�Ŀ�ߣ�
NODATA_value -99999----------------��ֵ���ݱ�־*/
bool DEMGrid::readASC(std::string filename, DEMGrid::DEM & _dem){
	std::locale::global(std::locale(""));

	std::ifstream _demfile;		 //DEM�ļ���
	_demfile.open(filename.c_str()); //��ȡ�ļ�
	std::string strtmp;

	if (_demfile){
		initial(_dem);

		_demfile >> strtmp >> _dem._Col;
		_demfile >> strtmp >> _dem._Row;
		_demfile >> strtmp >> _dem._Xo;
		_demfile >> strtmp >> _dem._Yo;
		_demfile >> strtmp >> _dem._DX;
		_dem._DY = _dem._DX;
		_demfile >> strtmp >> _dem._Nodata;
	}
	else
		return false;

	_dem._Data = new double[_dem._Row*_dem._Col];

	int i, j, k = 0;
	for (j = 0; j < _dem._Row; j++)	{
		for (i = 0; i<_dem._Col; i++){
			_demfile >> _dem._Data[k];

			if (_dem._Data[k] != _dem._Nodata){		//������ dNodata
				if (_dem._MinV > _dem._Data[k]) _dem._MinV = _dem._Data[k];//��¼�߳��ϵ���С��ֵ
				if (_dem._MaxV < _dem._Data[k]) _dem._MaxV = _dem._Data[k];//��¼�߳��ϵ������ֵ
			}
			k++;
		}
	}

	return true;
}
bool DEMGrid::writeASC(const DEMGrid::DEM & _dem, std::string filename){
	std::locale::global(std::locale(""));

	std::ofstream _demfile;		 //DEM�ļ���
	_demfile.open(filename.c_str()); //��ȡ�ļ�

	if (_demfile){

		_demfile << "ncols      " << _dem._Col << std::endl;
		_demfile << "nrows      " << _dem._Row << std::endl;
		_demfile << "xllcorner      " << _dem._Xo << std::endl;
		_demfile << "yllcorner      " << _dem._Yo << std::endl;
		_demfile << "cellsize      " << _dem._DX << std::endl;
		_demfile << "NODATA_value  " << _dem._Nodata << std::endl;
	}
	else
		return false;


	int i, j, k = 0;
	for (j = 0; j < _dem._Row; j++)	{
		for (i = 0; i < _dem._Col; i++){
			_demfile << _dem._Data[k] << " ";
			k++;
		}
		_demfile << std::endl;
	}

	return true;
}
end_gdb_namespace
end_gtl_namespace

