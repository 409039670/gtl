#include "internal_group3dimpl.h"



begin_gtl_namespace
begin_gdb_namespace

int Group3dImpl::FrustumIntersectGeometry(FrustumImpl& frustum)
{
	int nRes = Geometry3dImpl::FrustumIntersectGeometry(frustum);

	if (nRes == 0 || nRes == 1)
		return nRes;

	int nCount = m_vGeoCollection.size();
	for (int i = 0; i<nCount; i++)
	{
		nRes = m_vGeoCollection[i]->FrustumIntersectGeometry(frustum);
		if (nRes == 1)
			return 1;
	}
	return 0;
}

int Group3dImpl::IsPointInGeometry(Vertex3d& vPt) const
{
	int nCount = m_vGeoCollection.size();
	for (int i = nCount - 1; i>-1; i--)
	{
		if (m_vGeoCollection[i]->IsPointInGeometry(vPt))
			return 1;
	}
	return 0;
}

double Group3dImpl::DPointToGeometryEx(Vertex3d& vPt) const
{
	double dMin;
	int nCount = m_vGeoCollection.size();
	for (int i = 0; i<nCount; i++)
	{
		double dDis = m_vGeoCollection[i]->DPointToGeometryEx(vPt);
		if (dDis == 0.0)
			m_vGeoCollection[i]->DPointToGeometryEx(vPt);
		if (i == 0)
			dMin = dDis;
		else
		{
			//		if(dMin > dDis)  //xx_0128
			if ((dMin - dDis) > MathEx::TOL)
				dMin = dDis;
		}
	}
	return dMin;
}

int Group3dImpl::RayCrossGeometry(RayImpl& ray, double& dLength) const
{
	if (0 == Geometry3dImpl::RayCrossGeometry(ray, dLength))
		return 0;

	int nCount = m_vGeoCollection.size();
	double dMin = MathEx::dDoubleMax;

	int bCross = 0;
	for (int k = 0; k<nCount; k++)
	{
		double dDis;
		int bRes = m_vGeoCollection[k]->RayCrossGeometry(ray, dDis);

		//	if(bRes && dMin > dDis) //xx_0128
		if (bRes && ((dMin - dDis) > MathEx::TOL))
		{
			bCross = 1;
			dMin = dDis;
		}
	}
	dLength = dMin;
	return bCross;
}

bool Group3dImpl::readBuffer(Buffer & buf)
{

	int flag1 = 0;

	flag1 = Geometry3dImpl::readBuffer(buf);

	//ָ����������Ķ�����Ϣ**************

	int nCount = 0;
	buf.read((char*)&nCount, sizeof(int));//���ж������

	GeometryImpl* pGeometry = NULL;
	int signal = -1;
	int flag = 0;
	m_vGeoCollection.reserve(nCount);
	for (int i = 0; i<nCount; i++)
	{

		unsigned int  nSize = 0;
		buf.read((char*)&nSize, sizeof(unsigned int));//�������Ĵ�С

		char* p = (char*)buf.data() + buf.getPos();
		memcpy((char*)&signal, p, sizeof(int));//��ö���� TypeID �Դ��б�����һ�����
		pGeometry = GeometryImpl::create(signal);

		flag = pGeometry->readBuffer(buf);
		if (!flag) return 0;

		Geometry3dImpl* p3DGeometry = dynamic_cast<Geometry3dImpl*>(pGeometry);
		m_vGeoCollection.push_back(p3DGeometry);//��3DGeometry��ָ����ӵ���Լ��ζ���Ĵ洢�ṹ��
	}

	if (flag1)
	{
		return true;
	}
	else
	{
		return false;
	}

}


bool Group3dImpl::writeBuffer(Buffer& buf)
{
	int flag1 = 0;

	flag1 = Geometry3dImpl::writeBuffer(buf);

	//���������Ӧ����Ϣ********************
	int nCount = m_vGeoCollection.size();
	buf.write((char*)&nCount, sizeof(int));//���ж���ĸ���

	for (int i = 0; i<nCount; i++)
	{
		size_t nSize = m_vGeoCollection[i]->sizeBuffer();
		buf.write((char*)&nSize, sizeof(unsigned int));//���������ڴ��С

		int flag = 0;
		flag = m_vGeoCollection[i]->writeBuffer(buf);
		if (!flag) return 0;
	}

	if (flag1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

size_t Group3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	//������Ϣ(Geometry��3DGeometry)
	//*************************************************

	nSize += Geometry3dImpl::sizeBuffer();

	//ר����Ϣ
	//*************************************************

	nSize += sizeof(int); //���ж������
	for (size_t i = 0; i< m_vGeoCollection.size(); i++)
	{
		nSize += sizeof(unsigned int); //��һ������ռ�õĵ��ڴ��С
		nSize += m_vGeoCollection[i]->sizeBuffer();
	}

	//*************************************************

	return nSize;
}

void Group3dImpl::TranslateFrom(Matrix4x4& m4x4)
{
	Geometry3dImpl::TranslateFrom(m4x4);

	long i;
	long nCount = m_vGeoCollection.size();
	for (i = 0; i < nCount; i++)
	{
		m_vGeoCollection[i]->TranslateFrom(m4x4);
	}

	//���¼����������
	m_eEnvelope.Empty();
	for (i = 0; i < nCount; i++)
	{
		m_eEnvelope.Union(m_vGeoCollection[i]->getEnvelope());
	}
}

GeometryImpl* Group3dImpl::CreateObject()
{
	return new Group3dImpl;
}

//�����ⲿָ������ά�߶ε���ǰ���ζ�������ƽ��
double Group3dImpl::D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt)
{
	double dis, mindis = 1e+14;
	for (long i = 0; i < m_vGeoCollection.size(); i++)
	{
		dis = m_vGeoCollection[i]->D3DLineSegToGeometry(begvPt, endvPt);
		//	if(dis < mindis) mindis = dis;
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	}
	return mindis*mindis;
}

//�����ⲿָ���������ε���ǰ���ζ�������ƽ��
double Group3dImpl::D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3)
{
	double dis, mindis = 1e+14;
	for (long i = 0; i < m_vGeoCollection.size(); i++)
	{
		dis = m_vGeoCollection[i]->D3DTriangleToGeometry(triPt1, triPt2, triPt3);
		//	if(dis < mindis) mindis = dis;
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	}
	return mindis*mindis;
}

//���㵱ǰ����ļ�������
Vertex3d Group3dImpl::CalCorePoint()
{
	Vertex3d corePt;
	long nNun = m_vGeoCollection.size();
	for (long i = 0; i < nNun; i++)
		corePt += m_vGeoCollection.at(i)->CalCorePoint();
	corePt /= nNun;
	return corePt;
}

/** �麯��
* ͳ�Ƶ�ǰ��������ļ�������
* @return long*:�����ڲ����ٿռ䡢�ⲿ�ͷ�
*               ����ֵ������ -- ���������������Ч�������������������Ч��ɫ����
*								�����������������ת��Ϊ�ȼ�����
*								����������������
*/
long* Group3dImpl::GeoStatistics()
{
	int i = 0;
	long* statis = new long[8];
	for (i = 0; i < 8; i++)
		statis[i] = 0;


	long n = 0;
	for (n = 0; n < m_vGeoCollection.size(); n++)
	{
		long* tempsta = m_vGeoCollection[n]->GeoStatistics();
		if (tempsta)
		{
			for (i = 0; i < 8; i++)
				statis[i] += tempsta[i];
			delete[]tempsta;
		}
	}

	// ydcl_xx_��������û��ȥ���ظ�

	return statis;
}

// �ͷŶ������ݲ���ָ��ͱ�������ʼֵ
bool Group3dImpl::Init()
{
	bool res = Geometry3dImpl::Init();
	if (false == res) return false;

	Clear();

	return true;
}

int Group3dImpl::RGeoWithPolygon2D(vector<Vertex3d>& ptArray, int precisionFlag)
{
	int res = Geometry3dImpl::RGeoWithPolygon2D(ptArray, precisionFlag);

	//if(precisionFlag == 0)
	{
		return res;
	}
	// 		else if( precisionFlag == 1 || precisionFlag == 2 )
	// 		{
	// 			if(res == 0)
	// 				return 0;
	// 
	// 			for( long i = 0; i < m_vGeoCollection.size(); i++ )
	// 				if( RGeoWithPolygon2D(ptArray,precisionFlag) == 1 )
	// 					return 1;
	// 
	// 			return 0;
	// 		}
	// 		else
	// 			return -1;
}
end_gdb_namespace
end_gtl_namespace

