#include "internal_MultiPolygon3dImpl.h"
#include "internal_spatialfuns.h"
#include "internal_TriangleMesh3dImpl.h"
#include "internal_TextureImpl.h"
#include "internal_SimpleMaterialImpl.h"
#include "internal_ComplexMaterialImpl.h"
#include "internal_DelaunayImpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

void MultiPolygon3dImpl::GetVerticesEx(vector<Vertex3d>& vt3d) const
{
	vt3d.reserve(m_lVertNum);
	if (m_iVertexType == VERTEXTYPE_V3d)
	{
		for (int i = 0; i<m_lVertNum; i++)
		{
			Vertex3d tempVt = ((Vertex3d*)m_vVertices)[i];
			vt3d.push_back(tempVt);
		}
	}

	else if (m_iVertexType == VERTEXTYPE_V3f)
	{
		for (int i = 0; i<m_lVertNum; i++)
		{
			Vertex3f tempVt = ((Vertex3f*)m_vVertices)[i];
			vt3d.push_back(Vertex3d(tempVt.x, tempVt.y, tempVt.z));
		}
	}

	else if (m_iVertexType == VERTEXTYPE_V3fT2f)
	{
		for (int i = 0; i<m_lVertNum; i++)
		{
			TexturedVertex3f tempVt = ((TexturedVertex3f*)m_vVertices)[i];
			vt3d.push_back(Vertex3d(tempVt.x, tempVt.y, tempVt.z));
		}
	}

	else if (m_iVertexType == VERTEXTYPE_V3dT2d)
	{
		for (int i = 0; i<m_lVertNum; i++)
		{
			TexturedVertex3d tempVt = ((TexturedVertex3d*)m_vVertices)[i];
			vt3d.push_back(Vertex3d(tempVt.x, tempVt.y, tempVt.z));
		}
	}

	else if (m_iVertexType == VERTEXTYPE_V3dN3dC3f)
	{
		for (int i = 0; i<m_lVertNum; i++)
		{
			ShadedVertex3d tempVt = ((ShadedVertex3d*)m_vVertices)[i];
			vt3d.push_back(Vertex3d(tempVt.x, tempVt.y, tempVt.z));
		}
	}

	else if (m_iVertexType == VERTEXTYPE_V3fN3fC3f)
	{
		for (int i = 0; i<m_lVertNum; i++)
		{
			ShadedVertex3f tempVt = ((ShadedVertex3f*)m_vVertices)[i];
			vt3d.push_back(Vertex3d(tempVt.x, tempVt.y, tempVt.z));
		}
	}

	else if (m_iVertexType == VERTEXTYPE_V3fT2fN3fC3f)
	{
		for (int i = 0; i<m_lVertNum; i++)
		{
			VersatileVertex3f tempVt = ((VersatileVertex3f*)m_vVertices)[i];
			vt3d.push_back(Vertex3d(tempVt.x, tempVt.y, tempVt.z));
		}
	}

	else if (m_iVertexType == VERTEXTYPE_V3dT2dN3dC3f)
	{
		for (int i = 0; i<m_lVertNum; i++)
		{
			VersatileVertex3d tempVt = ((VersatileVertex3d*)m_vVertices)[i];
			vt3d.push_back(Vertex3d(tempVt.x, tempVt.y, tempVt.z));
		}
	}
}

bool MultiPolygon3dImpl::readBuffer(Buffer & buf)
{

	int flag = 0;
	flag = Surface3dImpl::readBuffer(buf);

	//����������Ϣ************************

	//�������ͱ���
	buf.read((char*)&m_iVertexType, sizeof(unsigned char));

	//�����б�
	buf.read((char*)&m_lVertNum, sizeof(long));
	if (m_lVertNum > 0)
	{
		if (m_iVertexType == VERTEXTYPE_V3d)
		{
			m_vVertices = new Vertex3d[m_lVertNum];
			buf.read((char*)m_vVertices, sizeof(Vertex3d) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3f)
		{
			m_vVertices = new Vertex3f[m_lVertNum];
			buf.read((char*)m_vVertices, sizeof(Vertex3f) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3dT2d)
		{
			m_vVertices = new TexturedVertex3d[m_lVertNum];
			buf.read((char*)m_vVertices, sizeof(TexturedVertex3d) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3fT2f)
		{
			m_vVertices = new TexturedVertex3f[m_lVertNum];
			buf.read((char*)m_vVertices, sizeof(TexturedVertex3f) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3dN3dC3f)
		{
			m_vVertices = new ShadedVertex3d[m_lVertNum];
			buf.read((char*)m_vVertices, sizeof(ShadedVertex3d) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3fN3fC3f)
		{
			m_vVertices = new ShadedVertex3f[m_lVertNum];
			buf.read((char*)m_vVertices, sizeof(ShadedVertex3f) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3dT2dN3dC3f)
		{
			m_vVertices = new VersatileVertex3d[m_lVertNum];
			buf.read((char*)m_vVertices, sizeof(VersatileVertex3d) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3fT2fN3fC3f)
		{
			m_vVertices = new VersatileVertex3f[m_lVertNum];
			buf.read((char*)m_vVertices, sizeof(VersatileVertex3f) * m_lVertNum);
		}
	}

	// ����θ��� //long m_lPolygonNum;
	buf.read((char*)&m_lPolygonNum, sizeof(long));

	// ÿ������εĶ����� //long* m_pPolygonVertexNums;
	if (m_lPolygonNum > 0)
	{
		m_pPolygonVertexNums = new long[m_lPolygonNum];
		buf.read((char*)m_pPolygonVertexNums, sizeof(long) * m_lPolygonNum);
	}

	// ����ζ��������б� //long* m_pPolygonVertexIndexs;
	long Num = GetPolygonIndexNum();
	if (Num > 0)
	{
		m_pPolygonVertexIndexs = new long[Num];
		buf.read((char*)m_pPolygonVertexIndexs, sizeof(long) * Num);
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


bool MultiPolygon3dImpl::writeBuffer(Buffer& buf)
{
	int flag = 0;
	flag = Surface3dImpl::writeBuffer(buf);

	//���������Ϣ**************************

	//�������ͱ���

	buf.write((char*)&m_iVertexType, sizeof(unsigned char));

	//�����б�
	buf.write((char*)&m_lVertNum, sizeof(long));

	if ((m_lVertNum > 0) && m_vVertices)
	{
		if (m_iVertexType == VERTEXTYPE_V3d)
		{
			Vertex3d* pVetexList = (Vertex3d*)m_vVertices;
			buf.write((char*)pVetexList, sizeof(Vertex3d) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3f)
		{
			Vertex3f* pVetexList = (Vertex3f*)m_vVertices;
			buf.write((char*)pVetexList, sizeof(Vertex3f) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3dT2d)
		{
			TexturedVertex3d* pVetexList = (TexturedVertex3d*)m_vVertices;
			buf.write((char*)pVetexList, sizeof(TexturedVertex3d) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3fT2f)
		{
			TexturedVertex3f* pVetexList = (TexturedVertex3f*)m_vVertices;
			buf.write((char*)pVetexList, sizeof(TexturedVertex3f) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3dN3dC3f)
		{
			ShadedVertex3d* pVetexList = (ShadedVertex3d*)m_vVertices;
			buf.write((char*)pVetexList, sizeof(ShadedVertex3d) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3fN3fC3f)
		{
			ShadedVertex3f* pVetexList = (ShadedVertex3f*)m_vVertices;
			buf.write((char*)pVetexList, sizeof(ShadedVertex3f) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3dT2dN3dC3f)
		{
			VersatileVertex3d* pVetexList = (VersatileVertex3d*)m_vVertices;
			buf.write((char*)pVetexList, sizeof(VersatileVertex3d) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3fT2fN3fC3f)
		{
			VersatileVertex3f* pVetexList = (VersatileVertex3f*)m_vVertices;
			buf.write((char*)pVetexList, sizeof(VersatileVertex3f) * m_lVertNum);
		}
	}

	// ����θ��� //long m_lPolygonNum;
	buf.write((char*)&m_lPolygonNum, sizeof(long));

	// ÿ������εĶ����� //long* m_pPolygonVertexNums;
	if (m_lPolygonNum > 0)
	{
		buf.write((char*)m_pPolygonVertexNums, sizeof(long) * m_lPolygonNum);
	}

	// ����ζ��������б� //long* m_pPolygonVertexIndexs;
	long Num = GetPolygonIndexNum();
	if (Num > 0)
	{
		buf.write((char*)m_pPolygonVertexIndexs, sizeof(long) * Num);
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

size_t MultiPolygon3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	//������Ϣ(Surface3dImpl)
	//*************************************************

	nSize += Surface3dImpl::sizeBuffer();

	//ר����Ϣ
	//*************************************************

	nSize += sizeof(unsigned char); // 	unsigned char m_iVertexType;
	nSize += sizeof(long); // 	long m_lVertNum;

	if (m_iVertexType == VERTEXTYPE_V3d)	//void* m_vVertices;	
	{
		nSize += sizeof(Vertex3d) * m_lVertNum;
	}
	else if (m_iVertexType == VERTEXTYPE_V3f)
	{
		nSize += sizeof(Vertex3f) * m_lVertNum;
	}
	else if (m_iVertexType == VERTEXTYPE_V3dT2d)
	{
		nSize += sizeof(TexturedVertex3d) * m_lVertNum;
	}
	else if (m_iVertexType == VERTEXTYPE_V3fT2f)
	{
		nSize += sizeof(TexturedVertex3f) * m_lVertNum;
	}
	else if (m_iVertexType == VERTEXTYPE_V3dN3dC3f)
	{
		nSize += sizeof(ShadedVertex3d) * m_lVertNum;
	}
	else if (m_iVertexType == VERTEXTYPE_V3fN3fC3f)
	{
		nSize += sizeof(ShadedVertex3f) * m_lVertNum;
	}
	else if (m_iVertexType == VERTEXTYPE_V3dT2dN3dC3f)
	{
		nSize += sizeof(VersatileVertex3d) * m_lVertNum;
	}
	else if (m_iVertexType == VERTEXTYPE_V3fT2fN3fC3f)
	{
		nSize += sizeof(VersatileVertex3f) * m_lVertNum;
	}

	nSize += sizeof(long);					// 	long m_lPolygonNum;
	nSize += sizeof(long) * m_lPolygonNum;	// 	long* m_pPolygonVertexNums;

	long Num = GetPolygonIndexNum();
	nSize += sizeof(long) * Num;			// 	long* m_pPolygonVertexIndexs;

	return nSize;
}

void MultiPolygon3dImpl::TranslateFrom(Matrix4x4& m4x4)
{
	Surface3dImpl::TranslateFrom(m4x4);

	//�任�����ͬʱ�����¼����������
	m_eEnvelope.Empty();
	Vertex3d v;

	long i;
	for (i = 0; i < m_lVertNum; i++)
	{
		if (m_iVertexType == VERTEXTYPE_V3d)	//void* m_vVertices;	
		{
			Vertex3d* pVetexList = (Vertex3d*)m_vVertices;

			m4x4.transformAffine(pVetexList[i]);
			m_eEnvelope.Union(pVetexList[i]);
		}
		else if (m_iVertexType == VERTEXTYPE_V3f)
		{
			Vertex3f* pVetexList = (Vertex3f*)m_vVertices;
			m4x4.transformAffine(pVetexList[i]);

			v.x = pVetexList[i].x;
			v.y = pVetexList[i].y;
			v.z = pVetexList[i].z;
			m_eEnvelope.Union(v);

		}
		else if (m_iVertexType == VERTEXTYPE_V3dT2d)
		{
			TexturedVertex3d* pVetexList = (TexturedVertex3d*)m_vVertices;
			m4x4.transformAffine(pVetexList[i]);

			m_eEnvelope.Union(pVetexList[i]);
		}
		else if (m_iVertexType == VERTEXTYPE_V3fT2f)
		{
			TexturedVertex3f* pVetexList = (TexturedVertex3f*)m_vVertices;
			m4x4.transformAffine(pVetexList[i]);

			v.x = pVetexList[i].x;
			v.y = pVetexList[i].y;
			v.z = pVetexList[i].z;
			m_eEnvelope.Union(v);
		}
		else if (m_iVertexType == VERTEXTYPE_V3dN3dC3f)
		{
			ShadedVertex3d* pVetexList = (ShadedVertex3d*)m_vVertices;
			m4x4.transformAffine(pVetexList[i]);

			m_eEnvelope.Union(pVetexList[i]);
		}
		else if (m_iVertexType == VERTEXTYPE_V3fN3fC3f)
		{
			ShadedVertex3f* pVetexList = (ShadedVertex3f*)m_vVertices;
			m4x4.transformAffine(pVetexList[i]);

			v.x = pVetexList[i].x;
			v.y = pVetexList[i].y;
			v.z = pVetexList[i].z;
			m_eEnvelope.Union(v);
		}
		else if (m_iVertexType == VERTEXTYPE_V3dT2dN3dC3f)
		{
			VersatileVertex3d* pVetexList = (VersatileVertex3d*)m_vVertices;
			m4x4.transformAffine(pVetexList[i]);

			m_eEnvelope.Union(pVetexList[i]);
		}
		else if (m_iVertexType == VERTEXTYPE_V3fT2fN3fC3f)
		{
			VersatileVertex3f* pVetexList = (VersatileVertex3f*)m_vVertices;
			m4x4.transformAffine(pVetexList[i]);

			v.x = pVetexList[i].x;
			v.y = pVetexList[i].y;
			v.z = pVetexList[i].z;
			m_eEnvelope.Union(v);
		}
	}
}

GeometryImpl* MultiPolygon3dImpl::CreateObject()
{
	return new MultiPolygon3dImpl;
}

//���㵱ǰ����ļ�������
Vertex3d MultiPolygon3dImpl::CalCorePoint()
{
	std::vector<Vertex3d> positons;
	GetVerticesEx(positons);

	Vertex3d corePt;
	for (long i = 0; i < m_lVertNum; i++)
		corePt += positons[i];
	corePt /= m_lVertNum;

	return corePt;
}

// �����ⲿָ������ά����󵽵�ǰ���ζ�������ƽ��
double MultiPolygon3dImpl::DPointToGeometryEx(Vertex3d& vPt) const
{
	std::vector<Vertex3d> positions;
	std::vector<long> pPolygonVertexIndexs;
	std::vector<long> pPolygonVertexNums;

	GetVerticesEx(positions);
	GetPolygonVertexIndexsEx(pPolygonVertexIndexs, pPolygonVertexNums);

	double dis, mindis = 1e+14;
	long count = 0;
	long i = 0, j = 0;
	for (i = 0; i < pPolygonVertexNums.size(); i++)
	{
		LinearRing3dImpl* ptempLinearRing = new LinearRing3dImpl;
		Vertex3d* tempVertex = new Vertex3d[pPolygonVertexNums.at(i)];
		for (j = count; j < count + pPolygonVertexNums.at(i); j++)
		{
			tempVertex[j - count] = positions.at(pPolygonVertexIndexs.at(j));
		}
		count += pPolygonVertexNums.at(i);
		ptempLinearRing->SetVertices(pPolygonVertexNums.at(i), tempVertex);

		dis = ptempLinearRing->DPointToGeometryEx(vPt);
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
		if (ptempLinearRing)
			delete ptempLinearRing;
		ptempLinearRing = NULL;
	}

	return mindis;
}

// �麯��������������ʵ��
double MultiPolygon3dImpl::D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt)
{
	std::vector<Vertex3d> positions;
	std::vector<long> pPolygonVertexIndexs;
	std::vector<long> pPolygonVertexNums;

	GetVerticesEx(positions);
	GetPolygonVertexIndexsEx(pPolygonVertexIndexs, pPolygonVertexNums);

	double dis, mindis = 1e+14;
	long count = 0;
	long i = 0, j = 0;
	for (i = 0; i < pPolygonVertexNums.size(); i++)
	{
		LinearRing3dImpl* ptempLinearRing = new LinearRing3dImpl;
		Vertex3d* tempVertex = new Vertex3d[pPolygonVertexNums.at(i)];
		for (j = count; j < count + pPolygonVertexNums.at(i); j++)
		{
			tempVertex[j - count] = positions.at(pPolygonVertexIndexs.at(j));
		}
		count += pPolygonVertexNums.at(i);
		ptempLinearRing->SetVertices(pPolygonVertexNums.at(i), tempVertex);

		dis = ptempLinearRing->D3DLineSegToGeometryEx(begvPt, endvPt);
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
		if (ptempLinearRing)
			delete ptempLinearRing;
		ptempLinearRing = NULL;
	}

	return mindis;
}

// �麯��������������ʵ��
double MultiPolygon3dImpl::D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3)
{
	std::vector<Vertex3d> positions;
	std::vector<long> pPolygonVertexIndexs;
	std::vector<long> pPolygonVertexNums;

	GetVerticesEx(positions);
	GetPolygonVertexIndexsEx(pPolygonVertexIndexs, pPolygonVertexNums);

	double dis, mindis = 1e+14;
	long count = 0;
	long i = 0, j = 0;
	for (i = 0; i < pPolygonVertexNums.size(); i++)
	{
		LinearRing3dImpl* ptempLinearRing = new LinearRing3dImpl;
		Vertex3d* tempVertex = new Vertex3d[pPolygonVertexNums.at(i)];
		for (j = count; j < count + pPolygonVertexNums.at(i); j++)
		{
			tempVertex[j - count] = positions.at(pPolygonVertexIndexs.at(j));
		}
		count += pPolygonVertexNums.at(i);
		ptempLinearRing->SetVertices(pPolygonVertexNums.at(i), tempVertex);

		dis = ptempLinearRing->D3DTriangleToGeometryEx(triPt1, triPt2, triPt3);
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
		if (ptempLinearRing)
			delete ptempLinearRing;
		ptempLinearRing = NULL;
	}

	return mindis;
}

int MultiPolygon3dImpl::RayCrossGeometry(RayImpl& ray, double& dLength) const
{
	if (0 == Geometry3dImpl::RayCrossGeometry(ray, dLength))
		return 0;

	int flag = 0;
	int bCross = 0;
	std::vector<Vertex3d> positions;
	std::vector<long> pPolygonVertexIndexs;
	std::vector<long> pPolygonVertexNums;

	GetVerticesEx(positions);
	GetPolygonVertexIndexsEx(pPolygonVertexIndexs, pPolygonVertexNums);

	double dis = 0.0, mindis = 1e+14;
	long count = 0;
	long i = 0, j = 0;
	for (i = 0; i < m_lPolygonNum; i++)
	{
		AABBoxImpl ENV;
		for (j = count; j < count + m_pPolygonVertexNums[i]; j++)
		{
			ENV.Union(positions.at(m_pPolygonVertexIndexs[j]));
		}
		if (0 == IsRayCrossEnvelope(ray, ENV))
		{
			count += m_pPolygonVertexNums[i];
			continue;
		}

		LinearRing3dImpl* ptempLinearRing = new LinearRing3dImpl;
		Vertex3d* tempVertex = new Vertex3d[m_pPolygonVertexNums[i]];
		for (j = count; j < count + m_pPolygonVertexNums[i]; j++)
		{
			tempVertex[j - count] = positions.at(m_pPolygonVertexIndexs[j]);
		}
		count += m_pPolygonVertexNums[i];
		ptempLinearRing->SetVertices(m_pPolygonVertexNums[i], tempVertex);
		ptempLinearRing->setEnvelope(ENV);

		flag = ptempLinearRing->RayCrossGeometry(ray, dis);
		if (flag == 1)
		{
			bCross = 1;
			if (dis < mindis)
				mindis = dis;
		}
		if (ptempLinearRing)
			delete ptempLinearRing;
		ptempLinearRing = NULL;
	}
	dLength = mindis;
	return bCross;
}

// ģ�庯�������ݶ������ͻ�����л�
template <typename T>
static void getTypeRings(unsigned char m_cType, void* pVertList, \
	long aPolyCount, long* pEachVertNum, long* pVertIndex, std::vector<LinearRing3dImpl*>& m_pRings)
{
	int index1, index2;
	index1 = index2 = 0;
	int aRingCount = aPolyCount;

	T* pCurVertList = NULL;
	pCurVertList = (T*)pVertList;
	for (int i = 0; i<aRingCount; i++)
	{
		AABBoxImpl anEnvelope;
		int aVertCount = pEachVertNum[i];
		index2 += aVertCount;  // ��¼��ֹ���
		LinearRing3dImpl* pRing = new LinearRing3dImpl();
		pRing->SetVertexType(m_cType);

		T* pVertTemp = new T[aVertCount];
		for (int j = index1; j<index2; j++)
		{
			pVertTemp[j - index1] = pCurVertList[pVertIndex[j]];
			anEnvelope.Union(pVertTemp[j - index1]);
		}
		pRing->SetVertices(aVertCount, (void*)pVertTemp);
		pRing->setEnvelope(anEnvelope);
		m_pRings.push_back(pRing);
		index1 = index2;
		pRing = NULL;
	}
}

//-------------------------------------------------------------------
bool MultiPolygon3dImpl::getRings(std::vector<LinearRing3dImpl*>& m_pRings)
{
	switch (m_iVertexType)   // ���ݶ�����������ÿ��Ring�Ķ�����
	{
	case VERTEXTYPE_V3d:
	{
		getTypeRings<Vertex3d>(m_iVertexType, m_vVertices, m_lPolygonNum, m_pPolygonVertexNums, \
			m_pPolygonVertexIndexs, m_pRings);
	}
	break;

	case VERTEXTYPE_V3dT2dN3dC3f:
	{
		getTypeRings<VersatileVertex3d>(m_iVertexType, m_vVertices, m_lPolygonNum, m_pPolygonVertexNums, \
			m_pPolygonVertexIndexs, m_pRings);
	}
	break;
	}
	return TRUE;
}
// ģ�庯�����ϲ������߻����ɵ�Mesh
template <typename T>
static BOOL mergeAllMesh(unsigned char m_cType, vector<LinearRing3dImpl*>& pRingList, TriangleMesh3dImpl* &pDestMesh)
{
	BOOL fSucflg = FALSE;
	int aSize = pRingList.size();
	for (int i = 0; i<aSize; i++)
	{
		long aVertCount = 0;
		vector<LinearRing3dImpl*> pTempList;
		pTempList.push_back(pRingList[i]);
		TriangleMesh3dImpl* pTempMesh = new TriangleMesh3dImpl;
		if (TriangulationFrmRings(false, NULL, pTempList, m_cType, pTempMesh))
		{
			fSucflg = mergeTriangleMesh<T>(m_cType, pTempMesh, pDestMesh);
			delete pTempMesh;
			pTempMesh = NULL;
			pTempList.clear();
		}
	}

	return (TRUE == fSucflg);
}

// ģ�庯�������ݶ������������ϲ�Mesh
template <typename T>
static BOOL mergeTriangleMesh(unsigned char m_cType, TriangleMesh3dImpl* pTempMesh, TriangleMesh3dImpl* &pDestMesh)
{
	int i, j;
	i = j = 0;
	long aOldVertSize, aNewVertSize, aTempVertSize;
	aOldVertSize = aNewVertSize = aTempVertSize = 0;
	T* pTempVertList = NULL;  // ���ϲ��Ķ����б�������������Ƭʱͳһ����
	T* pOldVertList = NULL;   // ԭʼ�����б��ϲ�����Ҫ�����ڴ�
	T* pNewVertList = NULL;   // �µĶ����б�
	long* pTempTriangle = NULL;  // ���ϲ���������������
	long* pOldTriangle = NULL;   // ԭʼ������������
	long* pNewTriangle = NULL;   // �µ�������������
	long aTempTriSize, aOldTriSize;
	aTempTriSize = aOldTriSize = 0;
	pDestMesh->GetTriangles(pOldTriangle, aOldTriSize);
	pDestMesh->GetVertices((void*&)pOldVertList, aOldVertSize);
	pTempMesh->GetTriangles(pTempTriangle, aTempTriSize);
	pTempMesh->GetVertices((void*&)pTempVertList, aTempVertSize);

	// ���������б�
	aNewVertSize = aOldVertSize + aTempVertSize;
	pNewVertList = new T[aNewVertSize];
	for (i = 0; i<aOldVertSize; i++)
	{
		pNewVertList[i] = pOldVertList[i];
	}
	for (j = aOldVertSize; j<aNewVertSize; j++)
	{
		pNewVertList[j] = pTempVertList[j - aOldVertSize];
	}
	i = j = 0;

	// ����������
	int aNewSize = aOldTriSize + aTempTriSize;
	pNewTriangle = new long[aNewSize];
	for (i = 0; i<aOldTriSize; i++)
	{
		pNewTriangle[i] = pOldTriangle[i];
	}
	for (j = aOldTriSize; j<aNewSize; j++)
	{
		// ������ֵ = ԭ����ֵ + ԭ������� 
		pNewTriangle[j] = pTempTriangle[j - aOldTriSize] + aOldVertSize;
	}
	pDestMesh->SetTriangles(pNewTriangle, aNewSize);
	pDestMesh->SetVertexType(m_cType);
	pDestMesh->SetVertices((void*)pNewVertList, aNewVertSize);
	pDestMesh->setEnvelope(pDestMesh->getEnvelope().Union(pTempMesh->getEnvelope()));

	delete[]pOldVertList;
	delete[]pOldTriangle;
	pOldTriangle = NULL;
	pOldVertList = NULL;
	return TRUE;
}

//-------------------------------------------------------------------
TriangleMesh3dImpl* MultiPolygon3dImpl::ConvertToMesh(const int nMode, const int nType)
{
	if (m_pTriangleMesh)   // ���й����ڴ棺�Ѿ�ת���򷵻�ָ��ֵ
	{
		if (nType == 0)
		{
			return m_pTriangleMesh;
		}
		else if (nType == 1)
		{
			TriangleMesh3dImpl* pNewMesh = dynamic_cast<TriangleMesh3dImpl*>(m_pTriangleMesh->Clone());
			return pNewMesh;
		}
		else
		{
			//assert(0);
			return NULL;
		}
	}

	// 1. �ӵ�ǰmultiPolygon�г�ȡRings
	m_vRings.clear();
	getRings(m_vRings);
	if (m_vRings.empty())
	{
		return NULL;
	}

	BOOL fSucFlg = FALSE;
	TriangleMesh3dImpl* pOutMesh = new TriangleMesh3dImpl;

	// 2. �ϲ�������ɵ�mesh
	switch (m_iVertexType)
	{
	case VERTEXTYPE_V3d:
	{
		fSucFlg = mergeAllMesh<Vertex3d>(VERTEXTYPE_V3d, m_vRings, pOutMesh);
	}
	break;

	case VERTEXTYPE_V3dT2dN3dC3f:
	{
		fSucFlg = mergeAllMesh<VersatileVertex3d>(VERTEXTYPE_V3dT2dN3dC3f, m_vRings, pOutMesh);
	}
	break;

	default:
		break;

	}

	if (!fSucFlg)
	{
		delete pOutMesh;
		pOutMesh = NULL;
		return pOutMesh;
	}

	// 3. ��������Ϣ
	MaterialImpl* pCurMat = GetMaterial();
	if (pCurMat != NULL)
	{
		if (MaterialImpl::simpleMat == pCurMat->GetMatType())
		{
			SimpleMaterialImpl* pCurSimple = dynamic_cast<SimpleMaterialImpl*>(pCurMat);
			SimpleMaterialImpl* pSimpleMat = new SimpleMaterialImpl;
			pSimpleMat->SetMatParam(pCurSimple->GetMatParam());
			pSimpleMat->SetMatName(pCurSimple->GetMatName());
			pSimpleMat->SetMatID(pCurSimple->GetMatID());
			TextureImpl* pCurTex = pCurSimple->GetTexture();
			if (pCurTex != NULL)
			{
				TextureImpl* pNewTex = new TextureImpl();
				pSimpleMat->SetTextureID(pCurSimple->GetTextureID());
				pNewTex->SetTextureID(pCurTex->GetTextureID());
				pNewTex->SetTextureName(pCurTex->GetTextureName());
				pNewTex->SetTextureType(pCurTex->GetTextureType());
				pSimpleMat->SetTexture(pNewTex);
			}
			pOutMesh->SetMaterial(pSimpleMat);
		}
	}

	if (!nType)
	{
		m_pTriangleMesh = pOutMesh;
	}
	return pOutMesh;
}

int MultiPolygon3dImpl::FrustumIntersectGeometry(FrustumImpl& frustum)
{
	int nRes = Geometry3dImpl::FrustumIntersectGeometry(frustum);

	if (nRes == 0 || nRes == 1)
		return nRes;

	vector<Vertex3d> vt3d;
	vector<long> vtIndexs;
	vector<long> vtNums;
	GetVerticesEx(vt3d);
	GetPolygonVertexIndexsEx(vtIndexs, vtNums);
	long count = 0;
	for (int f = 0; f < vtNums.size(); f++)
	{
		vector<Vertex3d> tempvt;
		for (int j = count; j < count + vtNums.at(f); j++)
			tempvt.push_back(vt3d.at(vtIndexs.at(j)));
		count += vtNums.at(f);

		if ('1' == RFrustumWithPolygon(frustum, tempvt))
			return 1;
	}
	return 0;
}

/** �麯��
* ͳ�Ƶ�ǰ��������ļ�������
* @return long*:�����ڲ����ٿռ䡢�ⲿ�ͷ�
*               ����ֵ������ -- ������(0)���������Ч����������(1)���������Ч��ɫ��(2)��
*								�������(3)��������(4)��ת��Ϊ�ȼ�����(5)
*								������(6)��������(7)��
*/
long* MultiPolygon3dImpl::GeoStatistics()
{
	long* upstatis = Surface3dImpl::GeoStatistics();

	long* statis = new long[8];

	statis[0] = m_lVertNum;

	if (m_iVertexType == VERTEXTYPE_V3d || m_iVertexType == VERTEXTYPE_V3f)
	{
		statis[1] = 0;
		statis[2] = 0;
	}
	else if (m_iVertexType == VERTEXTYPE_V3fT2f || m_iVertexType == VERTEXTYPE_V3dT2d)
	{
		statis[1] = m_lVertNum;
		statis[2] = 0;
	}
	else if (m_iVertexType == VERTEXTYPE_V3dN3dC3f || m_iVertexType == VERTEXTYPE_V3fN3fC3f)
	{
		statis[1] = 0;
		statis[2] = m_lVertNum;
	}
	else if (m_iVertexType == VERTEXTYPE_V3fT2fN3fC3f || m_iVertexType == VERTEXTYPE_V3dT2dN3dC3f)
	{
		statis[1] = m_lVertNum;
		statis[2] = m_lVertNum;
	}

	statis[3] = 1;
	statis[4] = m_lPolygonNum;

	statis[5] = 0;
	if (m_pPolygonVertexNums)
	{
		for (long i = 0; i < m_lPolygonNum; i++)
			statis[5] += (m_pPolygonVertexNums[i] - 2);
	}

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
bool MultiPolygon3dImpl::Init()
{
	bool res = Surface3dImpl::Init();
	if (false == res) return false;

	/* @brief ��ʱ�߻�����*/
	std::vector<LinearRing3dImpl*> m_vRings;

	VertexVisitor::releaseVertexArray(m_iVertexType, m_vVertices);

	if (m_pPolygonVertexNums)
	{
		delete[]m_pPolygonVertexNums;
		m_pPolygonVertexNums = NULL;
	}
	if (m_pPolygonVertexIndexs)
	{
		delete[]m_pPolygonVertexIndexs;
		m_pPolygonVertexIndexs = NULL;
	}
	if (m_pTriangleMesh)
	{
		delete m_pTriangleMesh;
		m_pTriangleMesh = NULL;
	}
	if (pTempMesh)
	{
		delete pTempMesh;
		pTempMesh = NULL;
	}

	m_iVertexType = VERTEXTYPE_V3d;
	m_lVertNum = 0;
	m_lPolygonNum = 0;

	for (int i = 0; i < m_vRings.size(); i++)
	{
		if (m_vRings[i])
		{
			delete m_vRings[i];
			m_vRings[i] = NULL;
		}
		m_vRings.clear();
	}

	return true;
}

int MultiPolygon3dImpl::RGeoWithPolygon2D(vector<Vertex3d>& ptArray, int precisionFlag)
{
	int res = Geometry3dImpl::RGeoWithPolygon2D(ptArray, precisionFlag);

	if (precisionFlag == 0)
		return res;
	else if (precisionFlag == 1)
	{
		if (res == 0) return 0;

		vector<Vertex3d> vt3d;
		GetVerticesEx(vt3d);

		vector<Vertex3d> CH3d;
		GetConvexHull(vt3d, CH3d);

		return RPolygonWithPolygon2D(CH3d, ptArray);
	}
	else if (precisionFlag == 2)
	{
		if (res == 0) return 0;

		vector<Vertex3d> vt3d;
		vector<long> vtIndexs;
		vector<long> vtNums;
		GetVerticesEx(vt3d);
		GetPolygonVertexIndexsEx(vtIndexs, vtNums);
		long count = 0;
		for (int f = 0; f < vtNums.size(); f++)
		{
			vector<Vertex3d> tempvt;
			for (int j = count; j < count + vtNums.at(f); j++)
				tempvt.push_back(vt3d.at(vtIndexs.at(j)));
			count += vtNums.at(f);

			if (0 != RPolygonWithPolygon2D(tempvt, ptArray))
				return 1;
		}
		return 0;
	}
	else
		return -1;
}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace