#include "internal_prism3dimpl.h"
#include "mathex.h"
#include "internal_spatialfuns.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

 double Prism3dImpl::Height() const
 {
 	// TODO: Add your specialized code here.
 	return (double)0;
 }
 
 
 Vertex3d Prism3dImpl::TopCenter() const
 {
 	// TODO: Add your specialized code here.
 	// NOTE: Requires a correct return value to compile.
	 return Vertex3d();
 }
 
 

 Vertex3d Prism3dImpl::BottomCenter()const
 {
 	// TODO: Add your specialized code here.
 	// NOTE: Requires a correct return value to compile.
	 return Vertex3d();
 }

bool Prism3dImpl::readBuffer(Buffer & buf)
{

	int flag = 0;
	flag = Voxel3dImpl::readBuffer(buf);

	//����������Ϣ************************	

	buf.read((char*)&m_VertNumPerSide, sizeof(int));
	if (m_VertNumPerSide > 0)
	{
		m_vVertices = new Vertex3d[m_VertNumPerSide * 2];
		buf.read((char*)m_vVertices, sizeof(Vertex3d) * m_VertNumPerSide * 2);
	}

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool Prism3dImpl::writeBuffer(Buffer& buf)
{

	int flag = 0;
	flag = Voxel3dImpl::writeBuffer(buf);

	//���������Ϣ**************************

	buf.write((char*)&m_VertNumPerSide, sizeof(int));
	if (m_VertNumPerSide > 0)
	{
		buf.write((char*)m_vVertices, sizeof(Vertex3d) * m_VertNumPerSide * 2);
	}

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}

size_t Prism3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	//������Ϣ(Geometry��3DGeometry)
	//*************************************************

	nSize += Voxel3dImpl::sizeBuffer();

	//ר����Ϣ	
	//*************************************************

	nSize += sizeof(Vertex3d) * m_VertNumPerSide * 2; //Vertex3d* m_vVertices;
	nSize += sizeof(int); // m_VertNumPerSide

	//*************************************************		

	return nSize;
}

void Prism3dImpl::TranslateFrom(Matrix4x4& m4x4)
{
	Voxel3dImpl::TranslateFrom(m4x4);

	int i;
	for (i = 0; i < m_VertNumPerSide * 2; i++)
	{
		//m_vVertices[i] *= m4x4;
		m4x4.transformAffine(m_vVertices[i]);
	}

	//���¼����������
	m_eEnvelope.Empty();
	m_eEnvelope.Union(m_vVertices, m_VertNumPerSide * 2);
}

GeometryImpl* Prism3dImpl::CreateObject()
{
	return new Prism3dImpl;
}

double Prism3dImpl::D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt)
{
	double dis, mindis = 1e+14;
	long i = 0;
	//��/�ϵ�
	for (i = 0; i < m_VertNumPerSide - 2; i++)
	{
		dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[0], m_vVertices[i + 1], m_vVertices[i + 2]);
		//if(dis < mindis) mindis = dis;
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
		dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[m_VertNumPerSide], m_vVertices[m_VertNumPerSide + i + 1], m_vVertices[m_VertNumPerSide + i + 2]);
		//	if(dis < mindis) mindis = dis;
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	}
	//����
	for (i = 0; i < m_VertNumPerSide - 1; i++)
	{
		dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[i], m_vVertices[m_VertNumPerSide + i], m_vVertices[m_VertNumPerSide + i + 1]);
		//	if(dis < mindis) mindis = dis;
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
		dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[i], m_vVertices[m_VertNumPerSide + i + 1], m_vVertices[i + 1]);
		//	if(dis < mindis) mindis = dis;
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	}
	dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[0], m_vVertices[m_VertNumPerSide - 1], m_vVertices[2 * m_VertNumPerSide - 1]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[0], m_vVertices[2 * m_VertNumPerSide - 1], m_vVertices[m_VertNumPerSide]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	return mindis*mindis;
}

//�����ⲿָ���������ε���ǰ���ζ�������ƽ��
double Prism3dImpl::D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3)
{
	double dis, mindis = 1e+14;
	long i = 0;
	//��/�ϵ�
	for (i = 0; i < m_VertNumPerSide - 2; i++)
	{
		dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[0], m_vVertices[i + 1], m_vVertices[i + 2]);
		//	if(dis < mindis) mindis = dis;
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
		dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[m_VertNumPerSide], m_vVertices[m_VertNumPerSide + i + 1], m_vVertices[m_VertNumPerSide + i + 2]);
		//	if(dis < mindis) mindis = dis;
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	}
	//����
	for (i = 0; i < m_VertNumPerSide - 1; i++)
	{
		dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[i], m_vVertices[m_VertNumPerSide + i], m_vVertices[m_VertNumPerSide + i + 1]);
		//	if(dis < mindis) mindis = dis;
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
		dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[i], m_vVertices[m_VertNumPerSide + i + 1], m_vVertices[i + 1]);
		//	if(dis < mindis) mindis = dis;
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	}
	dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[0], m_vVertices[m_VertNumPerSide - 1], m_vVertices[2 * m_VertNumPerSide - 1]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[0], m_vVertices[2 * m_VertNumPerSide - 1], m_vVertices[m_VertNumPerSide]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	return mindis*mindis;
}

//���㵱ǰ����ļ�������
Vertex3d Prism3dImpl::CalCorePoint()
{
	Vertex3d corePt;
	for (long i = 0; i < 2 * m_VertNumPerSide; i++)
		corePt += m_vVertices[i];
	corePt /= 2 * m_VertNumPerSide;
	return corePt;
}

/** �麯��
* ͳ�Ƶ�ǰ��������ļ�������
* @return long*:�����ڲ����ٿռ䡢�ⲿ�ͷ�
*               ����ֵ������ -- ���������������Ч�������������������Ч��ɫ����
*								�����������������ת��Ϊ�ȼ�����
*								����������������
*/
long* Prism3dImpl::GeoStatistics()
{
	long* statis = new long[8];

	statis[0] = m_VertNumPerSide * 2;

	for (int i = 1; i < 8; i++)
		statis[i] = 0;

	// ydcl_xx_����û�ж�������󣬵����ݱ��棬��������󡢷���͵ȼ�������Ƭ���������д�ȷ��
	statis[3] = m_VertNumPerSide + 2;
	statis[4] = m_VertNumPerSide + 2;
	statis[5] = 4 * (m_VertNumPerSide - 1);

	return statis;
}

// �ͷŶ������ݲ���ָ��ͱ�������ʼֵ
bool Prism3dImpl::Init()
{
	bool res = Voxel3dImpl::Init();
	if (false == res) return false;

	if (m_vVertices)
		delete m_vVertices;
	m_vVertices = NULL;

	m_VertNumPerSide = 0;

	return true;
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace