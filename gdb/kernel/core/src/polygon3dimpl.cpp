#include "internal_polygon3dimpl.h"
#include "internal_spatialfuns.h"
#include "internal_delaunayimpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
int Polygon3dImpl::FrustumIntersectGeometry(FrustumImpl& frustum)
{
	int nRes = Geometry3dImpl::FrustumIntersectGeometry(frustum);
	if (nRes == 0 || nRes == 1)
		return nRes;

	int nCount = m_vRings.size();
	for (int i = 0; i<nCount; i++)
	{
		nRes = m_vRings[i]->FrustumIntersectGeometry(frustum);
		if (nRes == 1)
			return 1;
	}
	return 0;
}

double Polygon3dImpl::DPointToGeometryEx(Vertex3d& vPt)
{
	TriangleMesh3dImpl* pTempMesh = ConvertToMesh(0, 0);
	double dMin = 1e+14;
	if (pTempMesh)
	{
		dMin = pTempMesh->DPointToGeometryEx(vPt);
	}
	pTempMesh = NULL;
	return dMin;
}

char Polygon3dImpl::RPointWithGeometry(Vertex3d& vPt) const
{

	int nCount = m_vRings.size();
	for (int i = 0; i<nCount; i++)
	{
		char code = m_vRings[i]->RPointWithGeometry(vPt);
		int bFlag = m_vRings[i]->GetInOutRingFlag();
		if (bFlag) // �ڻ�
		{
			if (code == 'o')
			{
				if (i == nCount - 1)
				{
					return 'i';
				}
			}
			if (code == 'i')
			{
				return 'o';
			}
			else if (code == 'V' || code == 'E')
				return code;
		}
		else
		{
			if (code == 'o' || code == 'V' || code == 'E')
			{
				return code;
			}
			if (code == 'i')
			{
				if (i == nCount - 1)
				{
					return 'i';
				}
			}
		}
	}
	return 'o';
}


int Polygon3dImpl::RayCrossGeometry(RayImpl& ray, double& dLength) const
{
	if (0 == Geometry3dImpl::RayCrossGeometry(ray, dLength))
		return 0;

	int nCount = m_vRings.size();
	double dMin = MathEx::dDoubleMax;

	int bCross = 0;
	for (int k = 0; k<nCount; k++)
	{
		double dDis;
		int bRes = m_vRings[k]->RayCrossGeometry(ray, dDis);

		if (m_vRings[k]->GetInOutRingFlag() && bRes) // ������ڻ���������ô���߾Ͳ������������ཻ��
		{
			dLength = MathEx::dDoubleMax;
			return 0;
		}

		if (bRes && dMin > dDis)
		{
			dMin = dDis;
			bCross = 1;
		}
	}
	dLength = dMin;
	return bCross;
}

bool Polygon3dImpl::readBuffer(Buffer & buf)
{
	int flag = 0;
	flag = Surface3dImpl::readBuffer(buf);

	//ר����Ϣ******************************
	LinearRing3dImpl* pLinearRing;
	int nCount = 0;
	buf.read((char*)&nCount, sizeof(int));
	for (int i = 0; i<nCount; i++)
	{
		int flag = 0;

		pLinearRing = new LinearRing3dImpl;

		size_t nSize = 0;
		buf.read((char*)&nSize, sizeof(unsigned int));//�������Ĵ�С

		flag = pLinearRing->readBuffer(buf);

		if (flag)
		{
			m_vRings.push_back(pLinearRing);
			pLinearRing = NULL;
		}
		else
		{
			assert(0);
		}
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

bool Polygon3dImpl::writeBuffer(Buffer& buf)
{
	int flag = 0;
	flag = Surface3dImpl::writeBuffer(buf);

	//ר����Ϣ******************************
	int nCount = m_vRings.size();
	buf.write((char*)&nCount, sizeof(int));

	for (int i = 0; i<nCount; i++)
	{
		size_t nSize = m_vRings[i]->sizeBuffer();
		buf.write((char*)&nSize, sizeof(unsigned int));//���������ڴ��С

		m_vRings[i]->writeBuffer(buf);
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

size_t Polygon3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	//������Ϣ(Geometry��3DGeometry)
	//*************************************************

	nSize += Surface3dImpl::sizeBuffer();

	//ר����Ϣ
	//*************************************************

	//�ļ����һ������������Ϣ(int)
	nSize += sizeof(int);//��������ĸ���
	for (size_t i = 0; i<m_vRings.size(); i++)
	{
		nSize += sizeof(unsigned int);//�ļ��н�д����һ�����Ĵ�С
		nSize += m_vRings[i]->sizeBuffer();
	}

	//*************************************************

	return nSize;
}

void Polygon3dImpl::TranslateFrom(Matrix4x4& m4x4)
{
	Surface3dImpl::TranslateFrom(m4x4);

	size_t i;
	for (i = 0; i<m_vRings.size(); i++)
	{
		m_vRings[i]->TranslateFrom(m4x4);
	}

	//���¼����������
	m_eEnvelope.Empty();
	for (i = 0; i<m_vRings.size(); i++)
	{
		m_eEnvelope.Union(m_vRings[i]->getEnvelope());
	}
}

GeometryImpl* Polygon3dImpl::CreateObject()
{
	return new Polygon3dImpl;
}

//�����ⲿָ������ά�߶ε���ǰ���ζ�������ƽ��
double Polygon3dImpl::D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt)
{
	TriangleMesh3dImpl* pTempMesh = ConvertToMesh(0, 0);
	double mindis = 1e+14;
	if (pTempMesh)
	{
		mindis = pTempMesh->D3DLineSegToGeometryEx(begvPt, endvPt);
	}
	pTempMesh = NULL;
	return mindis;
}


//�����ⲿָ���������ε���ǰ���ζ�������ƽ��
double Polygon3dImpl::D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3)
{
	TriangleMesh3dImpl* pTempMesh = ConvertToMesh(0, 0);
	double mindis = 1e+14;
	if (pTempMesh)
	{
		mindis = pTempMesh->D3DTriangleToGeometryEx(triPt1, triPt2, triPt3);
	}
	pTempMesh = NULL;
	return mindis;
}

//���㵱ǰ����ļ�������
Vertex3d Polygon3dImpl::CalCorePoint()
{
	Vertex3d corePt;
	long nNum = m_vRings.size();
	for (long i = 0; i < nNum; i++)
		corePt += m_vRings.at(i)->CalCorePoint();
	corePt /= nNum;
	return corePt;
}
TriangleMesh3d * Polygon3dImpl::asTriangleMesh()
{
	TriangleMesh3dImpl* p = ConvertToMesh(0, 0, true);
	return dynamic_cast<TriangleMesh3d*>(p);
}
/** �ѵ�ǰ��Polygonת��Ϊ���������磬�Ѱ������ʺͰ�Χ��(֧�ֶ���⻷�������֧�ֻ����ཻ�����)
* @param [in] nMode	0--Ĭ��(ֻ����������) 1--�������������Ż��������� 2--����������ͬʱ��������������
* @param [in] nType	0--Ĭ��(�ڴ������ͷ�) 1--����Mesh���ڴ����ⲿ�ͷ�
* @param [in] bIsFoldingTest	0--Ĭ��(���л������ཻ����) 1--(�����л������ཻ����)
* @return ��ǰpolygon�����ɵ���������,������ɹ��򷵻�NULL(!!notice!!��Ҫ��������TriangleMesh3dImpl���󣬷��������ڴ�й¶)
*/
TriangleMesh3dImpl* Polygon3dImpl::ConvertToMesh(const int nMode, const int nType, bool bIsFoldingTest)
{
	if (m_pTriangleMesh)   // ���й����ڴ淽ʽ: ����Ѿ�ת�������򷵻�ȫ���ڴ�ָ��
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

	//=====================���Polygon==================== 
	//�������
	if (m_vRings.size() == 0)
	{
		return NULL;
	}


	bool isGetNormal = false;
	Vertex3d refNormal;
	bool isGetType = false;
	unsigned char VertType = 0;

	//for innerring
	int numberofOutring = 0;
	vector<Vertex3d> holemarks;

	typedef vector<Vertex3d> _vectorVertexInring;
	_vectorVertexInring chkvts;
	_vectorVertexInring::iterator itr_vts;
	_vector3DLinearRing::iterator ring_itr;
	TriangleMesh3dImpl* outputmesh = NULL;


	for (ring_itr = m_vRings.begin(); ring_itr != m_vRings.end(); ring_itr++)
	{
		LinearRing3dImpl* refRing = *ring_itr;
		chkvts.clear();

		//���ж��������
		if (refRing->GetVerticesNum()< 3)
			return NULL;

		//�������е�����
		eliminateRedundancy(refRing);

		//GetNormal
		if (!isGetNormal && 0 == refRing->GetInOutRingFlag())
		{
			refRing->GetVerticesEx(chkvts);
			GetPointsNormalEx(chkvts, refNormal);
			if (refNormal.length() != 0)
			{
				isGetNormal = true;
				chkvts.clear();
			}
			else if (!bIsFoldingTest)
			{
				return NULL;
			}
		}

		//folding test(oringinal for KML export ����linearring�������Ϲ���ļ���)
		if (bIsFoldingTest)
		{
			if (!isGetNormal)
			{
				//ȥ�����ܳ��ֵ�������С��
				refRing->GetVerticesEx(chkvts);
				double dTol = MathEx::TOL;
				while (isRingSelfIntersect(chkvts) && chkvts.size()>2)
				{
					eliminateRedundancy(refRing, dTol);
					refRing->GetVerticesEx(chkvts);
					dTol *= 10;
				}

				if (chkvts.size()<3)
					return NULL;
			}
			else
			{
				//test folding by normal
				refRing->GetVerticesEx(chkvts);
				for (itr_vts = chkvts.begin(); itr_vts != chkvts.end() - 2; itr_vts++)
				{
					Vertex3d vector1, vector2, vector3;
					vector2 = *(itr_vts + 2) - *(itr_vts + 1);
					vector1 = *(itr_vts + 1) - *(itr_vts);
					vector3 = (vector1^vector2);
					vector3.normalize();
					double theta = vector3*refNormal;
					if (theta<MathEx::HALF_PI / 2)
					{
						//ȥ�����ܳ��ֵ�������С��
						double dTol = MathEx::TOL;
						while (isRingSelfIntersect(chkvts) && chkvts.size()>2)
						{
							eliminateRedundancy(refRing, dTol);
							refRing->GetVerticesEx(chkvts);
							dTol *= 10;
						}

						if (chkvts.size()<3)
							return NULL;

						break;
					}
				}
			}
			chkvts.clear();
		}

		//��ȡ��������
		if (!isGetType)
		{
			VertType = refRing->GetVertexType();
			isGetType = true;
		}

		//=============Fast triangular for single simple ring======== 
		if (m_vRings.size() == 1)
		{
			FastTriangulationFrmSimpleRing(refRing, VertType, outputmesh, &refNormal);
			if (NULL != outputmesh)
			{
				break;
				int test = 0;
				// 				//output vertexes for autocad
				// 				FILE* fp = fopen("c:\\test.txt","w");
				// 				typedef vector<Vertex3d> _vectorVertexInring;
				// 				_vectorVertexInring::iterator itr_vts;
				// 				for (itr_vts = chkvts.begin();itr_vts != chkvts.end();itr_vts++)
				// 				{
				// 					CString str;
				// 					str.Format("%f,%f,%f\n",(*itr_vts).x,(*itr_vts).y,(*itr_vts).z);
				// 					fputs(str,fp);
				// 				}
				// 				fprintf(fp,"%s\n","");
				// 				fclose(fp);
			}
		}
		//GetRingType and holemark
		if (0 == refRing->GetInOutRingFlag())
		{
			numberofOutring++;
		}
		else
		{
			Vertex3d holevert;
			refRing->GetVerticesEx(chkvts);
			for (itr_vts = chkvts.begin(); itr_vts != chkvts.end() - 2; itr_vts++)
			{
				if (0 == IsPointInLine(*itr_vts, *(itr_vts + 1), *(itr_vts + 2), NULL))
				{//non conlinear
					if (chkvts.size() == 3)
					{
						holevert = (*itr_vts + *(itr_vts + 1) + *(itr_vts + 2)) / 3;
						break;
					}
					else
					{
						Vertex3d vector1, vector2, vector3;
						vector2 = *(itr_vts + 2) - *(itr_vts + 1);
						vector1 = *(itr_vts + 1) - *(itr_vts);
						vector3 = (vector2^vector1);
						vector3.normalize();
						double theta = vector3*refNormal;
						if (theta>MathEx::HALF_PI / 2)
						{//non concave and within a TOL of PI/2
							holevert = (*itr_vts + *(itr_vts + 1) + *(itr_vts + 2)) / 3;
							break;
						}
					}
				}
			}
			holemarks.push_back(holevert);
		}
	}
	//=====================Normal Triangulation============
	if (NULL == outputmesh)
	{
		//=====================���б�Լ����Delaunay��ɾ�����������Σ���������======================
		if (numberofOutring >1)
		{//��������⻷

			//���ɻ�����
			RINGTOP* topPolygon = new RINGTOP;
			if (m_vRings.size()>1)
			{
				_vector3DLinearRing m_vRingCopy(m_vRings);

				//�Ի����հ�Χ�н��д�С���������
				_vector3DLinearRing::iterator itr_begin, itr_end;
				itr_begin = m_vRingCopy.begin();
				itr_end = m_vRingCopy.end();
				itr_end--;
				MathEx::quick_sortEx(itr_begin, itr_end, compLinearRingSize);
				//��ring�Ӵ�С���뵽���˽ṹ��
				while (m_vRingCopy.size()>0)
				{
					if (!topologyRing(m_vRingCopy.back(), topPolygon))
					{
						return NULL;
					}
					m_vRingCopy.pop_back();
				}
			}

			//����
			outputmesh = new TriangleMesh3dImpl;
			//getholemarks
			if (topPolygon->m_vectorChildRingPtr.size()>0)
			{
				holemarks.clear();
				getPolygonholemarks(topPolygon, holemarks, refNormal);
				//
				if (!TriangulationFrmRings(true, &holemarks, m_vRings, VertType, outputmesh, &refNormal))
				{
					delete outputmesh;
					return NULL;
				}
			}
			else
			{
				if (!TriangulationFrmRings(true, NULL, m_vRings, VertType, outputmesh, &refNormal))
				{
					delete outputmesh;
					return NULL;
				}
			}

			//�ͷ�����
			if (topPolygon->m_vectorChildRingPtr.size()>0)
			{
				freetopPolygon(topPolygon);
			}
			delete topPolygon;
			topPolygon = NULL;
		}
		else
		{//������һ���⻷
			outputmesh = new TriangleMesh3dImpl;
			if (!TriangulationFrmRings(true, &holemarks, m_vRings, VertType, outputmesh, &refNormal))
			{
				delete outputmesh;
				return NULL;
			}
		}
	}//normal triangulation

	//
	outputmesh->setEnvelope(this->getEnvelope());
	outputmesh->SetMatID(this->GetMatID());
	outputmesh->SetMaterial(this->GetMaterial());

	if (0 == nMode)
	{
		//
		if (!nType)   // ���й����ڴ淽ʽ��ת�����¼ȫ��ָ��
		{
			m_pTriangleMesh = outputmesh;
		}
		return outputmesh;
	}
	else if (2 == nMode)
	{
		outputmesh->Strip();
		if (!nType)   // ���й����ڴ淽ʽ��ת�����¼ȫ��ָ��
		{
			m_pTriangleMesh = outputmesh;
		}
		return outputmesh;
	}
	else if (1 == nMode)
	{
		long* testri;
		long testrino;
		outputmesh->GetTriangles(testri, testrino);
		int maxindex = 0;
		for (int i = 0; i<testrino; i++)
		{
			if (testri[i]>maxindex)
			{
				maxindex = testri[i];
			}
		}
		if (maxindex>testrino + 2)
		{
			outputmesh->OptimizeList();
		}
		if (!nType)   // ���й����ڴ淽ʽ��ת�����¼ȫ��ָ��
		{
			m_pTriangleMesh = outputmesh;
		}
		return outputmesh;
	}
	return NULL;
}

bool Polygon3dImpl::compLinearRingSize(LinearRing3dImpl* a, LinearRing3dImpl* b)
{
	if (a->getEnvelope().Diagonal()<b->getEnvelope().Diagonal())
	{
		return true;
	}
	else if (fabs(a->getEnvelope().Diagonal() - b->getEnvelope().Diagonal())<MathEx::TOL)
	{
		if (1 == a->GetInOutRingFlag())
		{
			return true;
		}
		else
			return false;
	}
	else
	{
		return false;
	}
}

bool Polygon3dImpl::isRingSelfIntersect(vector<Vertex3d>& verts)
{
	EnsureClose(verts);
	typedef vector<Vertex3d> _vectorVertexInring;
	_vectorVertexInring::iterator itr_seedVert;
	for (itr_seedVert = verts.begin(); itr_seedVert != verts.end() - 1; itr_seedVert++)
	{
		_vectorVertexInring::iterator itr_Vert;
		for (itr_Vert = itr_seedVert + 1; itr_Vert != verts.end() - 1; itr_Vert++)
		{
			Vertex3d crossVert = Vertex3d(0.0, 0.0, 0.0);
			if ('1' == RLineSegWithLineSeg(*itr_seedVert, *(itr_seedVert + 1), *itr_Vert, *(itr_Vert + 1), 0, &crossVert))
			{
				//���ڽ������ж϶����Ƿ�����غ�
				if (crossVert != *itr_seedVert && crossVert != *(itr_seedVert + 1) &&
					crossVert != *itr_Vert && crossVert != *(itr_Vert + 1))
					return true;
			}

		}
	}
	return false;
}

bool Polygon3dImpl::topologyRing(LinearRing3dImpl* ring, RINGTOP* top)
{
	if (top->m_ptrRing == NULL)
	{
		top->m_ptrRing = ring;
		return true;
	}
	else
	{
		if (IsContain(ring, top->m_ptrRing))
		{
			if (0 == top->m_vectorChildRingPtr.size())
			{
				return insertTopNode(ring, top);
			}
			else
			{
				//���Բ����ӽڵ�
				_vectorRINGTOP::iterator itr;
				for (itr = top->m_vectorChildRingPtr.begin(); itr != top->m_vectorChildRingPtr.end(); itr++)
				{
					if (topologyRing(ring, *itr))
					{
						return true;
					}
				}
				//����slibing�ڵ�
				return insertTopNode(ring, top);
			}
		}
		else
		{
			return false;
		}
	}
}


bool Polygon3dImpl::insertTopNode(LinearRing3dImpl* ring, RINGTOP* top)
{
	//���˼��
	if (top->m_ptrRing->GetInOutRingFlag() == ring->GetInOutRingFlag())
	{
		return false;
	}
	//�½��ڵ�
	RINGTOP* newtopnode = new RINGTOP;
	newtopnode->m_ptrRing = ring;
	newtopnode->m_Parent = top;
	top->m_vectorChildRingPtr.push_back(newtopnode);
	return true;
}

bool Polygon3dImpl::IsContain(LinearRing3dImpl* testRing, LinearRing3dImpl* refRing)
{
	int resCount = 0;
	if (testRing->getEnvelope().MaxX() <= refRing->getEnvelope().MaxX() &&
		testRing->getEnvelope().MinX() >= refRing->getEnvelope().MinX())
	{
		resCount++;
	}
	if (testRing->getEnvelope().MaxY() <= refRing->getEnvelope().MaxY() &&
		testRing->getEnvelope().MinY() >= refRing->getEnvelope().MinY())
	{
		resCount++;
	}
	if (testRing->getEnvelope().MaxZ() <= refRing->getEnvelope().MaxZ() &&
		testRing->getEnvelope().MinZ() >= refRing->getEnvelope().MinZ())
	{
		resCount++;
	}

	if (resCount >= 2)
	{
		return true;
	}
	else
		return false;
}

void Polygon3dImpl::getPolygonholemarks(RINGTOP* topPolygon, vector<Vertex3d> &holemarks, Vertex3d& refNormal)
{
	_vectorRINGTOP::iterator top_itr;
	for (top_itr = topPolygon->m_vectorChildRingPtr.begin(); top_itr != topPolygon->m_vectorChildRingPtr.end(); top_itr++)
	{
		RINGTOP* curtop = *top_itr;

		if (curtop->m_vectorChildRingPtr.size()>0)
			getPolygonholemarks(curtop, holemarks, refNormal);

		if (1 == curtop->m_Parent->m_ptrRing->GetInOutRingFlag() && 0 == curtop->m_ptrRing->GetInOutRingFlag())
		{
			vector<Vertex3d> parent_verts, children_verts;
			vector<Vertex3d>::iterator vert_itr, vert_subitr, vert_flag;
			curtop->m_Parent->m_ptrRing->GetVerticesEx(parent_verts);
			curtop->m_ptrRing->GetVerticesEx(children_verts);

			for (vert_itr = parent_verts.begin(); vert_itr != parent_verts.end() - 1; vert_itr++)
			{
				Vertex3d vt1, vt2;
				vt1 = *vert_itr;
				vt2 = *(vert_itr + 1);
				double dist = 1 / MathEx::TOL_F;
				for (vert_subitr = children_verts.begin(); vert_subitr != children_verts.end() - 1; vert_subitr++)
				{
					Vertex3d vt3;
					vt3 = *vert_subitr;
					double curDist = DPointToLineSeg(vt3, vt1, vt2);
					if (dist>curDist)
					{
						dist = curDist;
						vert_flag = vert_subitr;
					}
				}
				Vertex3d vector1, vector2, vector3;
				vector1 = vt2 - vt1;
				vector2 = *vert_flag - vt2;
				vector3 = vector1^vector2;
				vector3.normalize();
				double theta = vector3*refNormal;
				if (theta>MathEx::HALF_PI / 2)
				{
					Vertex3d resVt;
					resVt = (vt1 + vt2 + *vert_flag) / 3;
					holemarks.push_back(resVt);
					break;
				}
			}
		}
		else if (1 == curtop->m_ptrRing->GetInOutRingFlag() && curtop->m_vectorChildRingPtr.size() == 0)
		{
			vector<Vertex3d> cur_verts;
			curtop->m_ptrRing->GetVerticesEx(cur_verts);
			vector<Vertex3d>::iterator vert_itr;
			for (vert_itr = cur_verts.begin(); (vert_itr + 2) != cur_verts.end(); vert_itr++)
			{
				if (0 == IsPointInLine(*vert_itr, *(vert_itr + 1), *(vert_itr + 2), NULL))
				{//non conlinear
					if (cur_verts.size() == 3)
					{
						holemarks.push_back((*vert_itr + *(vert_itr + 1) + *(vert_itr + 2)) / 3);
						break;
					}
					else
					{
						Vertex3d vector1, vector2, vector3;
						vector2 = *(vert_itr + 2) - *(vert_itr + 1);
						vector1 = *(vert_itr + 1) - *(vert_itr);
						vector3 = vector1^vector2;
						vector3.normalize();
						double theta = vector3*refNormal;
						if (theta>MathEx::HALF_PI / 2)
						{//non concave and within a TOL of PI/2
							holemarks.push_back((*vert_itr + *(vert_itr + 1) + *(vert_itr + 2)) / 3);
							break;
						}
					}
				}
			}
		}
	}
}

void Polygon3dImpl::freetopPolygon(RINGTOP* topPolygon)
{
	_vectorRINGTOP::iterator top_itr;
	for (top_itr = topPolygon->m_vectorChildRingPtr.begin(); top_itr != topPolygon->m_vectorChildRingPtr.end(); top_itr++)
	{
		RINGTOP* curtop = *top_itr;
		if (curtop->m_vectorChildRingPtr.size()>0)
			freetopPolygon(curtop);
		delete curtop;
		curtop = NULL;
	}
}

/** �麯��
* ͳ�Ƶ�ǰ��������ļ�������
* @return long*:�����ڲ����ٿռ䡢�ⲿ�ͷ�
*               ����ֵ������ -- ������(0)���������Ч����������(1)���������Ч��ɫ��(2)��
*								�������(3)��������(4)��ת��Ϊ�ȼ�����(5)
*								������(6)��������(7)��
*/
long* Polygon3dImpl::GeoStatistics()
{
	long* upstatis = Surface3dImpl::GeoStatistics();

	long* statis = new long[8];

	for (long n = 0; n < m_vRings.size(); n++)
	{
		LinearRing3dImpl* pring = m_vRings[n];
		long* dowmstatis = pring->GeoStatistics();
		for (int i = 0; i < 3; i++)
			statis[i] = dowmstatis[i];

		if (dowmstatis)
		{
			delete[]dowmstatis;
			dowmstatis = NULL;
		}
	}

	statis[3] = 1;
	statis[4] = 1;

	statis[5] = statis[0] * 3;
	// 		TriangleMesh3dImpl* ptempM = ConvertToMesh(0, 1);
	// 		if( ptempM )
	// 		{
	// 			long* tempa = NULL;
	// 			ptempM->GetTriangles(tempa, statis[5]);
	// 			statis[5] /= 3;
	// 
	// 			delete ptempM;
	// 			ptempM = NULL;
	// 		}
	// 		else
	// 			statis[5] = 0;

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
bool Polygon3dImpl::Init()
{
	bool res = Surface3dImpl::Init();
	if (false == res) return false;

	int nCount = m_vRings.size();
	for (int i = nCount - 1; i>-1; i--)
	{
		if (m_vRings[i])
			delete m_vRings[i];
	}
	m_vRings.clear();

	if (m_pTriangleMesh)
	{
		delete m_pTriangleMesh;
		m_pTriangleMesh = NULL;
	}

	return true;
}

int Polygon3dImpl::RGeoWithPolygon2D(vector<Vertex3d>& ptArray, int precisionFlag)
{
	int res = Geometry3dImpl::RGeoWithPolygon2D(ptArray, precisionFlag);

	if (precisionFlag == 0)
		return res;
	else if (precisionFlag == 1)
	{
		if (res == 0) return 0;

		vector<Vertex3d> vt3d;
		for (long i = 0; i < m_vRings.size(); i++)
		{
			if (0 == m_vRings[i]->GetInOutRingFlag())
			{
				vector<Vertex3d> tempVs;
				m_vRings[i]->GetVerticesEx(tempVs);
				for (long j = 0; j < tempVs.size(); j++)
					vt3d.push_back(tempVs[j]);
			}
		}

		vector<Vertex3d> CH3d;
		GetConvexHull(vt3d, CH3d);

		return RPolygonWithPolygon2D(CH3d, ptArray);
	}
	else if (precisionFlag == 2)
	{
		if (res == 0) return 0;

		TriangleMesh3dImpl* ptempM = ConvertToMesh(0, 1);
		if (ptempM)
		{
			int r = ptempM->RGeoWithPolygon2D(ptArray, precisionFlag);

			delete ptempM;
			ptempM = NULL;

			return r;
		}

		return -1;
	}
	else
		return -1;
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace