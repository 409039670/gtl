#include "internal_tetrahedron3dimpl.h"
#include "internal_spatialfuns.h"
#include "mathex.h"


begin_gtl_namespace
begin_gdb_namespace

bool Tetrahedron3dImpl::readBuffer(Buffer & buf)
{

	int flag = 0;
	flag = Voxel3dImpl::readBuffer(buf);

	//����������Ϣ************************

	buf.read((char*)m_vVertices, sizeof(Vertex3d) * 4);

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool Tetrahedron3dImpl::writeBuffer(Buffer& buf)
{
	int flag = 0;
	flag = Voxel3dImpl::writeBuffer(buf);

	//���������Ϣ**************************

	buf.write((char*)m_vVertices, sizeof(Vertex3d) * 4);

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}

size_t Tetrahedron3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	//������Ϣ(Geometry��3DGeometry)
	//*************************************************

	nSize += Voxel3dImpl::sizeBuffer();

	//ר����Ϣ
	//*************************************************

	nSize += sizeof(Vertex3d) * 4; //Vertex3d m_vVertices[4];

	//*************************************************

	return nSize;
}

void Tetrahedron3dImpl::TranslateFrom(Matrix4x4& m4x4)
{
	Voxel3dImpl::TranslateFrom(m4x4);

	for (int i = 0; i < 4; i++)
	{
		//m_vVertices[i] *= m4x4;
		m4x4.transformAffine(m_vVertices[i]);
	}

	//���¼����������
	m_eEnvelope.Empty();
	m_eEnvelope.Union(m_vVertices, 4);
}

GeometryImpl* Tetrahedron3dImpl::CreateObject()
{
	return new Tetrahedron3dImpl;
}

//�����ⲿָ������ά�߶ε���ǰ���ζ�������ƽ��
double Tetrahedron3dImpl::D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt)
{
	double dis, mindis = 1e+14;

	dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[0], m_vVertices[1], m_vVertices[2]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[0], m_vVertices[2], m_vVertices[3]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[0], m_vVertices[3], m_vVertices[1]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[1], m_vVertices[2], m_vVertices[3]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	return mindis*mindis;
}

//�����ⲿָ���������ε���ǰ���ζ�������ƽ��
double Tetrahedron3dImpl::D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3)
{
	double dis, mindis = 1e+14;

	dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[0], m_vVertices[1], m_vVertices[2]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[0], m_vVertices[2], m_vVertices[3]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[0], m_vVertices[3], m_vVertices[1]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[1], m_vVertices[2], m_vVertices[3]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	return mindis*mindis;
}

//���㵱ǰ����ļ�������
Vertex3d Tetrahedron3dImpl::CalCorePoint()
{
	Vertex3d corePt;
	for (long i = 0; i < 4; i++)
		corePt += m_vVertices[i];
	corePt /= 4;
	return corePt;
}

/** �麯��
* ͳ�Ƶ�ǰ��������ļ�������
* @return long*:�����ڲ����ٿռ䡢�ⲿ�ͷ�
*               ����ֵ������ -- ���������������Ч�������������������Ч��ɫ����
*								�����������������ת��Ϊ�ȼ�����
*								����������������
*/
long* Tetrahedron3dImpl::GeoStatistics()
{
	long* statis = new long[8];

	statis[0] = 4;

	for (int i = 1; i < 8; i++)
		statis[i] = 0;

	// ydcl_xx_����û�ж�������󣬵����ݱ��棬��������󡢷���͵ȼ�������Ƭ���������д�ȷ��
	statis[3] = 4;
	statis[4] = 4;
	statis[5] = 4;

	return statis;
}

// �ͷŶ������ݲ���ָ��ͱ�������ʼֵ
bool Tetrahedron3dImpl::Init()
{
	bool res = Voxel3dImpl::Init();
	if (false == res) return false;

	for (int i = 0; i < 4; i++)
		m_vVertices[i] = Vertex3d(0, 0, 0);

	return true;
}
end_gdb_namespace
end_gtl_namespace

