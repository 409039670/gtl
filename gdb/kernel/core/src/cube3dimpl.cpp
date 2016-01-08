#include "internal_cube3dimpl.h"
#include "mathex.h"
#include "internal_spatialfuns.h"


begin_gtl_namespace
begin_gdb_namespace
double Cube3dImpl::Length() const
{
	// TODO: Add your specialized code here.
	return (double)0;
}

//##ModelId=49E3472800DB
double Cube3dImpl::Width() const
{
	// TODO: Add your specialized code here.
	return (double)0;
}

//##ModelId=49E3472800DC
double Cube3dImpl::Height() const
{
	// TODO: Add your specialized code here.
	return (double)0;
}

//##ModelId=49ECAD70036B
Vertex3d Cube3dImpl::Center() const
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	return Vertex3d(0, 0, 0);
}


size_t Cube3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	//������Ϣ(Geometry��3DGeometry)
	//************************************************

	nSize += Cube3dImpl::sizeBuffer();

	//ר����Ϣ
	//*************************************************

	nSize += sizeof(Vertex3d) * 8;// m_vVertices[8];

	//*************************************************

	return nSize;
}

bool Cube3dImpl::readBuffer(Buffer& buf)
{
	int flag = 0;
	flag = Cube3dImpl::readBuffer(buf);

	//����������Ϣ**************************

	buf.read((char*)m_vVertices, sizeof(Vertex3d) * 8);

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool Cube3dImpl::writeBuffer(Buffer& buf)
{
	int flag = 0;
	flag = Cube3dImpl::writeBuffer(buf);

	//���������Ϣ**************************

	buf.write((char*)m_vVertices, sizeof(Vertex3d) * 8);

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}

}

void Cube3dImpl::TranslateFrom(Matrix4x4& m4x4)
{
	Cube3dImpl::TranslateFrom(m4x4);

	int i = 0;
	for (i = 0; i < 8; i++)
	{
		//m_vVertices[i] *= m4x4;
		m4x4.transformAffine(m_vVertices[i]);
	}

	//���¼����������
	m_eEnvelope.Empty();
	for (i = 0; i < 8; i++)
	{
		m_eEnvelope.Union(m_vVertices[i]);
	}

}

GeometryImpl* Cube3dImpl::CreateObject()
{
	return new Cube3dImpl;
}

//�����ⲿָ������ά�߶ε���ǰ���ζ�������ƽ��
double Cube3dImpl::D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt)
{
	double dis, mindis = 1e+14;

	dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[0], m_vVertices[1], m_vVertices[2]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[0], m_vVertices[2], m_vVertices[3]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[4], m_vVertices[5], m_vVertices[6]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[4], m_vVertices[6], m_vVertices[7]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[0], m_vVertices[1], m_vVertices[5]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[0], m_vVertices[5], m_vVertices[4]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[1], m_vVertices[2], m_vVertices[6]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[1], m_vVertices[6], m_vVertices[5]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[2], m_vVertices[6], m_vVertices[7]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[2], m_vVertices[7], m_vVertices[3]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[0], m_vVertices[3], m_vVertices[7]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[0], m_vVertices[7], m_vVertices[4]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	return mindis*mindis;
}

//�����ⲿָ���������ε���ǰ���ζ�������ƽ��
double Cube3dImpl::D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3)
{
	double dis, mindis = 1e+14;

	dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[0], m_vVertices[1], m_vVertices[2]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[0], m_vVertices[2], m_vVertices[3]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[4], m_vVertices[5], m_vVertices[6]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[4], m_vVertices[6], m_vVertices[7]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[0], m_vVertices[1], m_vVertices[5]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[0], m_vVertices[5], m_vVertices[4]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[1], m_vVertices[2], m_vVertices[6]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[1], m_vVertices[6], m_vVertices[5]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[2], m_vVertices[6], m_vVertices[7]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[2], m_vVertices[7], m_vVertices[3]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[0], m_vVertices[3], m_vVertices[7]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[0], m_vVertices[7], m_vVertices[4]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	return mindis*mindis;
}

//���㵱ǰ����ļ�������
Vertex3d Cube3dImpl::CalCorePoint()
{
	Vertex3d corePt;
	for (int i = 0; i < 8; i++)
		corePt += m_vVertices[i];
	corePt /= 8;
	return corePt;
}

/** �麯��
* ͳ�Ƶ�ǰ��������ļ�������
* @return long*:�����ڲ����ٿռ䡢�ⲿ�ͷ�
*               ����ֵ������ -- ���������������Ч�������������������Ч��ɫ����
*								�����������������ת��Ϊ�ȼ�����
*								����������������
*/
long* Cube3dImpl::GeoStatistics()
{
	long* statis = new long[8];

	statis[0] = 8;
	for (int i = 1; i < 8; i++)
		statis[i] = 0;

	return statis;
}

// �ͷŶ������ݲ���ָ��ͱ�������ʼֵ
bool Cube3dImpl::Init()
{
	bool res = Cube3dImpl::Init();
	if (false == res) return false;

	for (int i = 0; i<8; i++)
		m_vVertices[i] = Vertex3d(0, 0, 0);

	return true;
}

int Cube3dImpl::RGeoWithPolygon2D(vector<Vertex3d>& ptArray, int precisionFlag)
{
	int res = Geometry3dImpl::RGeoWithPolygon2D(ptArray, precisionFlag);

	if (precisionFlag == 0)
	{
		return res;
	}
	else if (precisionFlag == 1)
	{
		if (res == 0)
			return 0;

		vector<Vertex3d> pts;
		for (int i = 0; i < 8; i++)
			pts.push_back(m_vVertices[i]);

		vector<Vertex3d> CHpts;
		GetConvexHull(pts, CHpts);

		if (0 == RPolygonWithPolygon2D(CHpts, ptArray))
			return 0;
		else
			return 1;
	}
	else if (precisionFlag == 2)
	{
		if (res == 0)
			return 0;

		vector<Vertex3d> pts1, pts2, pts3, pts4, pts5, pts6;

		pts1.push_back(m_vVertices[0]);
		pts1.push_back(m_vVertices[1]);
		pts1.push_back(m_vVertices[2]);
		pts1.push_back(m_vVertices[3]);
		if (RPolygonWithPolygon2D(pts1, ptArray) != 0)
			return 1;

		pts2.push_back(m_vVertices[4]);
		pts2.push_back(m_vVertices[5]);
		pts2.push_back(m_vVertices[6]);
		pts2.push_back(m_vVertices[7]);
		if (RPolygonWithPolygon2D(pts1, ptArray) != 0)
			return 1;

		pts3.push_back(m_vVertices[0]);
		pts3.push_back(m_vVertices[1]);
		pts3.push_back(m_vVertices[5]);
		pts3.push_back(m_vVertices[4]);
		if (RPolygonWithPolygon2D(pts1, ptArray) != 0)
			return 1;

		pts4.push_back(m_vVertices[2]);
		pts4.push_back(m_vVertices[6]);
		pts4.push_back(m_vVertices[7]);
		pts4.push_back(m_vVertices[3]);
		if (RPolygonWithPolygon2D(pts1, ptArray) != 0)
			return 1;

		pts5.push_back(m_vVertices[0]);
		pts5.push_back(m_vVertices[2]);
		pts5.push_back(m_vVertices[6]);
		pts5.push_back(m_vVertices[5]);
		if (RPolygonWithPolygon2D(pts1, ptArray) != 0)
			return 1;

		pts6.push_back(m_vVertices[0]);
		pts6.push_back(m_vVertices[3]);
		pts6.push_back(m_vVertices[7]);
		pts6.push_back(m_vVertices[4]);
		if (RPolygonWithPolygon2D(pts1, ptArray) != 0)
			return 1;

		return 0;
	}
	else
		return -1;
}

end_gdb_namespace
end_gtl_namespace

