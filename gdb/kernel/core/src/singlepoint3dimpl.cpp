#include "internal_singlepoint3dimpl.h"
#include "internal_spatialfuns.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

SinglePoint3dImpl::SinglePoint3dImpl(void)
{
	m_byteTypeID = GEOTYPE_3D_SINGLEPOINT;
	m_dPointSize = 0.0;
}

SinglePoint3dImpl::~SinglePoint3dImpl(void)
{

}

bool SinglePoint3dImpl::readBuffer(Buffer & buf)
{
	int flag = 0;

	flag = Geometry3dImpl::readBuffer(buf);

	buf.read((char*)&m_vPoint, sizeof(ShadedVertex3d));
	buf.read((char*)&m_dPointSize, sizeof(double));

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool SinglePoint3dImpl::writeBuffer(Buffer& buf)
{
	int flag = 0;

	flag = Geometry3dImpl::writeBuffer(buf);

	buf.write((char*)&m_vPoint, sizeof(ShadedVertex3d));
	buf.write((char*)&m_dPointSize, sizeof(double));

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}

size_t SinglePoint3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	//������Ϣ(3DPoint)
	//*************************************************

	nSize += Point3dImpl::sizeBuffer();

	//ר����Ϣ
	//*************************************************

	nSize += sizeof(ShadedVertex3d);
	nSize += sizeof(double);

	return nSize;
}

void SinglePoint3dImpl::TranslateFrom(Matrix4x4& m4x4)
{
	Point3dImpl::TranslateFrom(m4x4);

	//�任�����ͬʱ�����¼����������
	m_eEnvelope.Empty();
	Vertex3d tempV(m_vPoint.x, m_vPoint.y, m_vPoint.z);

	m4x4.transformAffine(tempV);
	m_eEnvelope.Union(tempV);

	m_vPoint.x = tempV.x;
	m_vPoint.y = tempV.y;
	m_vPoint.z = tempV.z;
}

GeometryImpl* SinglePoint3dImpl::CreateObject()
{
	return new SinglePoint3dImpl;
}

//���㵱ǰ����ļ�������
Vertex3d SinglePoint3dImpl::CalCorePoint()
{
	Vertex3d tempVt(m_vPoint.x, m_vPoint.y, m_vPoint.z);
	return tempVt;
}

double SinglePoint3dImpl::DPointToGeometryEx(Vertex3d& vPt) const
{
	double dMin = 1e+14;
	Vertex3d tempVt(m_vPoint.x, m_vPoint.y, m_vPoint.z);
	dMin = DPointToPointEx(vPt, tempVt);
	return dMin;
}

//�����ⲿָ������ά�߶ε���ǰ���ζ�������ƽ��
double SinglePoint3dImpl::D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt)
{
	double mindis = 1e+14;
	Vertex3d tempVt(m_vPoint.x, m_vPoint.y, m_vPoint.z);
	mindis = DPointToLineSegEx(tempVt, begvPt, endvPt);
	return mindis;
}

//�����ⲿָ���������ε���ǰ���ζ�������ƽ��
double SinglePoint3dImpl::D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3)
{
	double mindis = 1e+14;
	Vertex3d tempVt(m_vPoint.x, m_vPoint.y, m_vPoint.z);
	mindis = DPointToTriangleEx(tempVt, triPt1, triPt2, triPt3);
	return mindis;
}

/** �麯��
* ͳ�Ƶ�ǰ��������ļ�������
* @return long*:�����ڲ����ٿռ䡢�ⲿ�ͷ�
*               ����ֵ������ -- ���������������Ч�������������������Ч��ɫ����
*								�����������������ת��Ϊ�ȼ�����
*								����������������
*/
long* SinglePoint3dImpl::GeoStatistics()
{
	long* statis = new long[8];

	for (int i = 1; i < 8; i++)
		statis[i] = 0;

	statis[0] = 1;
	statis[2] = 1;

	return statis;
}

// �ͷŶ������ݲ���ָ��ͱ�������ʼֵ
bool SinglePoint3dImpl::Init()
{
	bool res = Point3dImpl::Init();
	if (false == res) return false;

	m_vPoint = ShadedVertex3d(0, 0, 0, 0, 0, 0);
	m_dPointSize = 0.0;

	return true;
}

int SinglePoint3dImpl::RGeoWithPolygon2D(vector<Vertex3d>& ptArray, int precisionFlag)
{
	if (precisionFlag >= 0 && precisionFlag < 3)
	{
		char f = RPointWithPolygon2d(Vertex3d(m_vPoint.x, m_vPoint.y, m_vPoint.z), ptArray);
		if (f == 'i' || f == 'E' || f == 'V')
			return 1;
		else
			return 0;
	}
	else
		return -1;
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace