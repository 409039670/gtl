#include "internal_triangle3dimpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
bool Triangle3dImpl::readBuffer(Buffer & buf)
{
	int flag = 0;
	flag = Surface3dImpl::readBuffer(buf);

	//����Ķ�����Ϣ**********************

	buf.read((char*)m_vVetices, sizeof(Vertex3d) * 3);
	buf.read((char*)&m_vNormal, sizeof(Vertex3d));

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool Triangle3dImpl::writeBuffer(Buffer& buf)
{
	int flag = 0;
	flag = Surface3dImpl::writeBuffer(buf);

	//���������Ϣ**************************

	buf.write((char*)m_vVetices, sizeof(Vertex3d) * 3);
	buf.write((char*)&m_vNormal, sizeof(Vertex3d));

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}

size_t Triangle3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	//������Ϣ(Geometry��3DGeometry)
	//*************************************************

	nSize += Surface3dImpl::sizeBuffer();

	//ר����Ϣ
	//*************************************************

	nSize += sizeof(Vertex3d) * 3; //Vertex3d m_vVetices[3];
	nSize += sizeof(Vertex3d); //Vertex3d m_vNormal

	//*************************************************

	return nSize;
}

void Triangle3dImpl::TranslateFrom(Matrix4x4& m4x4)
{
	Surface3dImpl::TranslateFrom(m4x4);

	//�任�����ͬʱ���任����
	Vertex3d p0 = m_vVetices[0];
	p0 += m_vNormal;

	m4x4.transformAffine(p0);
	for (int i = 0; i < 3; i++)
	{
		m4x4.transformAffine(m_vVetices[i]);
	}

	m_vNormal.x = p0.x - m_vVetices[0].x;
	m_vNormal.y = p0.y - m_vVetices[0].y;
	m_vNormal.z = p0.z - m_vVetices[0].z;

	//���¼����������
	m_eEnvelope.Empty();
	m_eEnvelope.Union(m_vVetices, 3);
}

GeometryImpl* Triangle3dImpl::CreateObject()
{
	return new Triangle3dImpl;
}
//�����ⲿָ������ά�߶ε���ǰ���ζ�������ƽ��
double Triangle3dImpl::D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt)
{
	double mindis = 1e+14;
	mindis = DLineSegToTriangle(begvPt, endvPt, m_vVetices[0], m_vVetices[1], m_vVetices[2]);
	return mindis*mindis;
}

//�����ⲿָ���������ε���ǰ���ζ�������ƽ��
double Triangle3dImpl::D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3)
{
	return DTriangleToTriangleEx(triPt1, triPt2, triPt3, m_vVetices[0], m_vVetices[1], m_vVetices[2]);
}

int Triangle3dImpl::RayCrossGeometry(RayImpl& ray, double& dLength) const
{
	Vertex3d vPt1 = ray.origin;
	Vertex3d vPt2 = ray.origin + ray.direction;

	int bCross = 0;
	dLength = 1e+14;
	Vertex3d IntersectPt;
	PLANE3D plane;
	Vertex3d triPt1 = m_vVetices[0];
	Vertex3d triPt2 = m_vVetices[1];
	Vertex3d triPt3 = m_vVetices[2];

	int bRes = GetPlaneEquation(triPt1, triPt2, triPt3, plane);
	if (!bRes)
		return bCross;;
	char cRes = RLineWithPlane(vPt1, vPt2, plane, &IntersectPt);
	if (cRes == '1')
	{
		if (IsPointInTri(triPt1, triPt2, triPt3, IntersectPt))//������������
		{
			double dRes = (vPt2 - vPt1) * (IntersectPt - vPt1);
			//		if(dRes > 1e-6) //xx_0128
			if (dRes > MathEx::TOL)
			{
				double tempDis = DPointToPointEx(IntersectPt, vPt1);
				//		if(tempDis < dLength)
				if ((tempDis - dLength) < -MathEx::TOL)
				{
					dLength = tempDis;
					bCross = 1;
				}
			}
		}
	}
	return bCross;
}

//���㵱ǰ����ļ�������
Vertex3d Triangle3dImpl::CalCorePoint()
{
	Vertex3d corePt;
	for (long i = 0; i < 3; i++)
		corePt += m_vVetices[i];
	corePt /= 3;
	return corePt;
}

/** �麯��
* ͳ�Ƶ�ǰ��������ļ�������
* @return long*:�����ڲ����ٿռ䡢�ⲿ�ͷ�
*               ����ֵ������ -- ���������������Ч�������������������Ч��ɫ����
*								�����������������ת��Ϊ�ȼ�����
*								����������������
*/
long* Triangle3dImpl::GeoStatistics()
{
	long* upstatis = Surface3dImpl::GeoStatistics();
	long* statis = new long[8];

	statis[0] = 3;
	statis[1] = 0;
	statis[2] = 0;

	statis[3] = 1;
	statis[4] = 1;
	statis[5] = 1;

	statis[6] = upstatis[6];
	statis[7] = upstatis[7];

	if (upstatis)
	{
		delete[]upstatis;
		upstatis = NULL;
	}

	return statis;
}

// �ͷŶ������ݲ���ָ��ͱ�������ʼֵ
bool Triangle3dImpl::Init()
{
	bool res = Surface3dImpl::Init();
	if (false == res) return false;

	for (int i = 0; i < 3; i++)
		m_vVetices[i] = Vertex3d(0, 0, 0);

	m_vNormal = Vertex3d(0, 0, 0);

	return true;
}

int Triangle3dImpl::RGeoWithPolygon2D(vector<Vertex3d>& ptArray, int precisionFlag)
{
	int res = Geometry3dImpl::RGeoWithPolygon2D(ptArray, precisionFlag);

	if (precisionFlag == 0)
		return res;
	else if (precisionFlag == 1 || precisionFlag == 2)
	{
		if (res == 0) return 0;
		vector<Vertex3d> vts;
		for (int i = 0; i < 3; i++)
			vts.push_back(m_vVetices[i]);

		return RPolygonWithPolygon2D(vts, ptArray);
	}
	else
		return -1;
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace