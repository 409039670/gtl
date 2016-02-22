#include "internal_spatialfuns.h"
#include "internal_trianglemesh3dimpl.h"
#include "internal_polygon3dimpl.h"


begin_gtl_namespace
begin_gdb_namespace
int TriangleMesh3dImpl::Strip()
{
	if (0 == m_lTriListSize || 0 == m_lVertNum)
	{
		return 0;
	}
	PrimitiveGroup* resultList = NULL;
	unsigned long resultNum = 0;
	bool bResult = false;
	bResult = GenerateStripsEx(true, (const unsigned long*)m_lTriangleList, m_lTriListSize, &resultList, &resultNum);
	if (bResult)
	{
		if (resultNum>1)
		{
			for (int i = 0; i < resultNum; i++)
			{
				resultList[i].indices = NULL;
			}
			delete[]resultList;

			return 0;
		}

		m_lStripIndices = (long*)resultList->indices;
		m_lStripListSize = (long)resultList->numIndices;

		for (int i = 0; i < resultNum; i++)
		{
			resultList[i].indices = NULL;
		}
		delete[]resultList;
		return 1;
	}
	else
		return 0;
}

//##ModelId=49EF1556000F
int TriangleMesh3dImpl::Fan()
{
	return 0;
}

int TriangleMesh3dImpl::OptimizeList()
{
	if (0 == m_lTriListSize || 0 == m_lVertNum)
	{
		return 0;
	}
	PrimitiveGroup* resultList = NULL;
	unsigned long resultNum = 0;
	bool bResult = false;
	bResult = GenerateStripsEx(false, (const unsigned long*)m_lTriangleList, m_lTriListSize, &resultList, &resultNum);
	if (bResult)
	{
		if (resultNum>1)
		{
			for (int i = 0; i < resultNum; i++)
			{
				resultList[i].indices = NULL;
			}
			delete[]resultList;

			return 0;
		}

		if (m_lTriangleList)
		{
			delete[]m_lTriangleList;
			m_lTriangleList = NULL;
		}
		m_lTriangleList = (long*)resultList->indices;
		m_lTriListSize = (long)resultList->numIndices;

		for (int i = 0; i < resultNum; i++)
		{
			resultList[i].indices = NULL;
		}
		delete[]resultList;

		return 1;
	}
	else
		return 0;
}

bool TriangleMesh3dImpl::ConvertToPoly(const int nMode, vector<Polygon3dImpl*> &resPolySoup)
{
	//initial and check
	if (0 == m_lTriListSize || NULL == m_lTriangleList)
	{
		return false;
	}
	resPolySoup.clear();

	//Compacted mode
	if (0 == nMode)
	{
		m_uiTriangleMark = new UINT[m_lTriListSize / 3];
		for (int i = 0; i<m_lTriListSize / 3; i++)
		{
			m_uiTriangleMark[i] = 0;
		}

		//group and track boundaries
		for (int i = 0; i< m_lTriListSize / 3; i++)
		{
			if (0 == m_uiTriangleMark[i])
			{
				if (GrowTriangleGroup(i))//�˴�iΪ��i��������
				{
					if (0 != m_vectBoundaryEdges.size())
					{
						Polygon3dImpl* newPly = NULL;
						if (TrackBoundaries(newPly))
						{
							if (NULL != newPly)
							{
								resPolySoup.push_back(newPly);
								continue;
							}
							else
							{
								return false;
							}
						}
						else
						{
							return false;
						}
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
		}

		//����
		if (m_uiTriangleMark)
		{
			delete[] m_uiTriangleMark;
			m_uiTriangleMark = NULL;
		}

		m_vectBoundaryEdges.clear();

		if (m_uiBoundEdgeMark)
		{
			delete[] m_uiBoundEdgeMark;
			m_uiBoundEdgeMark = NULL;
		}
	}
	else//Simplify mode
	{
		std::deque<long> m_vectRingVerts;
		for (int i = 0; i< m_lTriListSize; i += 3)//ע�������ʽ
		{
			m_vectRingVerts.clear();
			m_vectRingVerts.push_back(m_lTriangleList[i]);
			m_vectRingVerts.push_back(m_lTriangleList[i + 1]);
			m_vectRingVerts.push_back(m_lTriangleList[i + 2]);

			Polygon3dImpl* newPoly = new Polygon3dImpl;
			std::vector<LinearRing3dImpl*> polyRings;
			polyRings.clear();

			AABBoxImpl env;
			LinearRing3dImpl* newRing = NULL;
			newRing = GenNewLinearRing(m_vectRingVerts, m_iVertexType, 0);
			if (NULL != newRing)
			{
				polyRings.push_back(newRing);
				newPoly->SetRings(polyRings);
				env.Union(newRing->getEnvelope());
				newPoly->setEnvelope(env);
				newPoly->SetMaterial(this->GetMaterial());
				newPoly->SetMatID(this->GetMatID());
			}
			else
			{
				return false;
			}
			resPolySoup.push_back(newPoly);
		}
	}
	return true;
}

bool TriangleMesh3dImpl::GrowTriangleGroup(long lTri)
{
	if (0 == m_lTriListSize || NULL == m_lTriangleList)
	{
		return false;
	}

	m_uiTriangleMark[lTri] = 1;
	Vertex3d vNormal = GetTriNormal(lTri);//�˴�lTriΪ��lTri��������

	if (Vertex3d(0.0, 0.0, 0.0) != vNormal)
	{
		//�ݹ���ҹ���������
		if (!TrackTriangleByEdge(vNormal, lTri, m_lTriangleList[3 * lTri], m_lTriangleList[3 * lTri + 1]))
		{
			m_vectBoundaryEdges.push_back(m_lTriangleList[3 * lTri]);
			m_vectBoundaryEdges.push_back(m_lTriangleList[3 * lTri + 1]);
		}
		if (!TrackTriangleByEdge(vNormal, lTri, m_lTriangleList[3 * lTri + 1], m_lTriangleList[3 * lTri + 2]))
		{
			m_vectBoundaryEdges.push_back(m_lTriangleList[3 * lTri + 1]);
			m_vectBoundaryEdges.push_back(m_lTriangleList[3 * lTri + 2]);
		}
		if (!TrackTriangleByEdge(vNormal, lTri, m_lTriangleList[3 * lTri + 2], m_lTriangleList[3 * lTri]))
		{
			m_vectBoundaryEdges.push_back(m_lTriangleList[3 * lTri + 2]);
			m_vectBoundaryEdges.push_back(m_lTriangleList[3 * lTri]);
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool TriangleMesh3dImpl::TrackTriangleByEdge(Vertex3d vNormal, long oriTri, long v1, long v2)
{
	if (0 == m_lTriListSize || NULL == m_lTriangleList)
	{
		return false;
	}

	bool bIsFound = false;
	for (int i = 0; i< m_lTriListSize / 3; i++)
	{
		//����
		if ((m_lTriangleList[3 * i] == v1 || m_lTriangleList[3 * i + 1] == v1 || m_lTriangleList[3 * i + 2] == v1)
			&& (m_lTriangleList[3 * i] == v2 || m_lTriangleList[3 * i + 1] == v2 || m_lTriangleList[3 * i + 2] == v2))
		{
			//ƽ��
			if (GetTriNormal(i) == vNormal)//�˴�iΪ��i��������
			{
				//��δ������
				if (0 == m_uiTriangleMark[i])
				{
					//�ҵ������Σ����ñ��
					bIsFound = true;
					m_uiTriangleMark[i] = 1;

					//�ݹ���ң����û���ҵ����¼Ϊ�߽�
					if ((m_lTriangleList[3 * i] != v1 && m_lTriangleList[3 * i] != v2) || (m_lTriangleList[3 * i + 1] != v1 && m_lTriangleList[3 * i + 1] != v2))
					{
						if (!TrackTriangleByEdge(vNormal, i, m_lTriangleList[3 * i], m_lTriangleList[3 * i + 1]))
						{
							m_vectBoundaryEdges.push_back(m_lTriangleList[3 * i]);
							m_vectBoundaryEdges.push_back(m_lTriangleList[3 * i + 1]);
						}
					}
					if ((m_lTriangleList[3 * i + 2] != v1 && m_lTriangleList[3 * i + 2] != v2) || (m_lTriangleList[3 * i + 1] != v1 && m_lTriangleList[3 * i + 1] != v2))
					{
						if (!TrackTriangleByEdge(vNormal, i, m_lTriangleList[3 * i + 1], m_lTriangleList[3 * i + 2]))
						{
							m_vectBoundaryEdges.push_back(m_lTriangleList[3 * i + 1]);
							m_vectBoundaryEdges.push_back(m_lTriangleList[3 * i + 2]);
						}
					}
					if ((m_lTriangleList[3 * i] != v1 && m_lTriangleList[3 * i] != v2) || (m_lTriangleList[3 * i + 2] != v1 && m_lTriangleList[3 * i + 2] != v2))
					{
						if (!TrackTriangleByEdge(vNormal, i, m_lTriangleList[3 * i + 2], m_lTriangleList[3 * i]))
						{
							m_vectBoundaryEdges.push_back(m_lTriangleList[3 * i + 2]);
							m_vectBoundaryEdges.push_back(m_lTriangleList[3 * i]);
						}
					}
				}
				else//�Ѿ�������
				{
					if (i == oriTri)
					{
						//���Ϊԭʼ���������������
						continue;
					}
					else
					{
						//���Ϊ��������������������ֱ�ӽ���ѭ��
						bIsFound = true;
						break;
					}
				}
			}
		}
	}

	return bIsFound;
}

bool TriangleMesh3dImpl::TrackBoundaries(Polygon3dImpl*& newPly)
{
	//Init boundMark
	m_uiBoundEdgeMark = new UINT[m_vectBoundaryEdges.size() / 2];
	for (int i = 0; i<m_vectBoundaryEdges.size() / 2; i++)
	{
		m_uiBoundEdgeMark[i] = 0;
	}

	//track
	std::vector<LinearRing3dImpl*> m_vectLinearRings;
	std::deque<long> m_vectRingVerts;

	std::vector<long>::iterator itr_EdgeVert;
	itr_EdgeVert = m_vectBoundaryEdges.begin();

	for (int i = 0; i<m_vectBoundaryEdges.size() / 2; i++)
	{
		if (0 == m_uiBoundEdgeMark[i])
		{
			m_vectRingVerts.clear();

			//add start node
			m_vectRingVerts.push_back(*(itr_EdgeVert + 2 * i));
			m_vectRingVerts.push_back(*(itr_EdgeVert + 2 * i + 1));
			m_uiBoundEdgeMark[i] = 1;

			//�ݹ�˫���ѯ
			TrackEdgeByVert(*(itr_EdgeVert + 2 * i), *(itr_EdgeVert + 2 * i + 1), m_vectRingVerts);
			if (0 == m_vectRingVerts.size())
			{
				return false;
			}

			//Gen new close LinearRing,��ʼ����־��Ϊ�ڻ�
			LinearRing3dImpl* newRing = NULL;
			newRing = GenNewLinearRing(m_vectRingVerts, m_iVertexType, 1);
			if (NULL != newRing)
			{
				m_vectLinearRings.push_back(newRing);
			}
			else
			{
				return false;
			}
		}
	}

	//new polygon
	newPly = new Polygon3dImpl;
	AABBoxImpl env;

	//�ҳ�Ψһ���⻷
	double dBBsize = 0;
	std::vector<LinearRing3dImpl*>::iterator itr_OutRing_ptr;
	std::vector<LinearRing3dImpl*>::iterator itr_LinearRing = m_vectLinearRings.begin();
	//env.setEnvelope((*itr_LinearRing)->getEnvelope());
	env.Union((*itr_LinearRing)->getEnvelope());

	itr_OutRing_ptr = itr_LinearRing;
	dBBsize = (*itr_LinearRing)->getEnvelope().Diagonal();
	for (itr_LinearRing = m_vectLinearRings.begin() + 1; itr_LinearRing != m_vectLinearRings.end(); itr_LinearRing++)
	{
		env.Union((*itr_LinearRing)->getEnvelope());
		double dcurBBsize = (*itr_LinearRing)->getEnvelope().Diagonal();
		if (dBBsize<dcurBBsize)
		{
			dBBsize = dcurBBsize;
			itr_OutRing_ptr = itr_LinearRing;
		}
	}
	(*itr_OutRing_ptr)->SetInOutRingFlag(0);

	newPly->SetRings(m_vectLinearRings);
	newPly->setEnvelope(env);
	newPly->SetMaterial(this->GetMaterial());
	newPly->SetMatID(this->GetMatID());

	//������б߽�
	m_vectBoundaryEdges.clear();
	if (m_uiBoundEdgeMark)
	{
		delete[] m_uiBoundEdgeMark;
		m_uiBoundEdgeMark = NULL;
	}

	return true;
}

void TriangleMesh3dImpl::TrackEdgeByVert(long toVert, long frmVert, std::deque<long>& m_vectRing)
{
	std::vector<long>::iterator itr_EdgeVert;
	itr_EdgeVert = m_vectBoundaryEdges.begin();

	for (int i = 0; i<m_vectBoundaryEdges.size() / 2; i++)
	{
		if (0 == m_uiBoundEdgeMark[i])
		{
			//˫�����
			if (*(itr_EdgeVert + 2 * i) == frmVert || *(itr_EdgeVert + 2 * i + 1) == toVert)
			{
				m_uiBoundEdgeMark[i] = 1;

				if (*(itr_EdgeVert + 2 * i) == frmVert && *(itr_EdgeVert + 2 * i + 1) == toVert)
				{
					return;
				}
				else if (*(itr_EdgeVert + 2 * i) == frmVert)
				{
					m_vectRing.push_back(*(itr_EdgeVert + 2 * i + 1));
					TrackEdgeByVert(toVert, *(itr_EdgeVert + 2 * i + 1), m_vectRing);
				}
				else
				{
					m_vectRing.push_front(*(itr_EdgeVert + 2 * i));
					TrackEdgeByVert(*(itr_EdgeVert + 2 * i), frmVert, m_vectRing);
				}
			}

		}
	}
}

//����TriangleMesh3dImpl::GenNewLinearRing��ʹ��
template <typename T>
static void FillLinearRingUsingIndex(LinearRing3dImpl* ring, std::deque<long>& m_vectRingVerts, void* m_vVertices)
{
	std::deque<long>::iterator itr_RingDeq;
	itr_RingDeq = m_vectRingVerts.begin();
	T* newType = new T[m_vectRingVerts.size() + 1];
	for (int i = 0; i<m_vectRingVerts.size(); i++)
	{
		newType[i] = ((T*)m_vVertices)[*(itr_RingDeq + i)];
	}

	//close
	newType[m_vectRingVerts.size()] = newType[0];

	ring->SetVertices(m_vectRingVerts.size() + 1, (void*)newType);
}

LinearRing3dImpl* TriangleMesh3dImpl::GenNewLinearRing(std::deque<long>& m_vectRingVerts, unsigned char m_iVertexType, int m_OutorIn)
{
	LinearRing3dImpl* newRing = new LinearRing3dImpl;
	newRing->SetInOutRingFlag(m_OutorIn);
	newRing->SetVertexType(m_iVertexType);
	switch (m_iVertexType)
	{
	case VERTEXTYPE_V3f:
		FillLinearRingUsingIndex<Vertex3f>(newRing, m_vectRingVerts, m_vVertices);
		break;
	case VERTEXTYPE_V3d:
		FillLinearRingUsingIndex<Vertex3d>(newRing, m_vectRingVerts, m_vVertices);
		break;
	case VERTEXTYPE_V3fT2f:
		FillLinearRingUsingIndex<TexturedVertex3f>(newRing, m_vectRingVerts, m_vVertices);
		break;
	case VERTEXTYPE_V3dT2d:
		FillLinearRingUsingIndex<TexturedVertex3d>(newRing, m_vectRingVerts, m_vVertices);
		break;
	case VERTEXTYPE_V3fN3fC3f:
		FillLinearRingUsingIndex<ShadedVertex3f>(newRing, m_vectRingVerts, m_vVertices);
		break;
	case VERTEXTYPE_V3dN3dC3f:
		FillLinearRingUsingIndex<ShadedVertex3d>(newRing, m_vectRingVerts, m_vVertices);
		break;
	case VERTEXTYPE_V3fT2fN3fC3f:
		FillLinearRingUsingIndex<VersatileVertex3f>(newRing, m_vectRingVerts, m_vVertices);
		break;
	case VERTEXTYPE_V3dT2dN3dC3f:
		FillLinearRingUsingIndex<VersatileVertex3d>(newRing, m_vectRingVerts, m_vVertices);
		break;
	}
	vector<Vertex3d> vect_verts;
	newRing->GetVerticesEx(vect_verts);
	AABBoxImpl BBbox;
	BBbox.Union(vect_verts);
	newRing->setEnvelope(BBbox);
	return newRing;
}


Vertex3d TriangleMesh3dImpl::GetTriNormal(long lTri)
{
	if (0 == m_lTriListSize || NULL == m_lTriangleList)
	{
		return Vertex3d(0.0, 0.0, 0.0);
	}

	Vertex3d v1, v2, v3;
	switch (m_iVertexType)
	{
	case VERTEXTYPE_V3f:
		v1.x = ((Vertex3f*)m_vVertices)[m_lTriangleList[3 * lTri]].x;
		v1.y = ((Vertex3f*)m_vVertices)[m_lTriangleList[3 * lTri]].y;
		v1.z = ((Vertex3f*)m_vVertices)[m_lTriangleList[3 * lTri]].z;
		v2.x = ((Vertex3f*)m_vVertices)[m_lTriangleList[3 * lTri + 1]].x;
		v2.y = ((Vertex3f*)m_vVertices)[m_lTriangleList[3 * lTri + 1]].y;
		v2.z = ((Vertex3f*)m_vVertices)[m_lTriangleList[3 * lTri + 1]].z;
		v3.x = ((Vertex3f*)m_vVertices)[m_lTriangleList[3 * lTri + 2]].x;
		v3.y = ((Vertex3f*)m_vVertices)[m_lTriangleList[3 * lTri + 2]].y;
		v3.z = ((Vertex3f*)m_vVertices)[m_lTriangleList[3 * lTri + 2]].z;
		break;
	case VERTEXTYPE_V3d:
		v1.x = ((Vertex3d*)m_vVertices)[m_lTriangleList[3 * lTri]].x;
		v1.y = ((Vertex3d*)m_vVertices)[m_lTriangleList[3 * lTri]].y;
		v1.z = ((Vertex3d*)m_vVertices)[m_lTriangleList[3 * lTri]].z;
		v2.x = ((Vertex3d*)m_vVertices)[m_lTriangleList[3 * lTri + 1]].x;
		v2.y = ((Vertex3d*)m_vVertices)[m_lTriangleList[3 * lTri + 1]].y;
		v2.z = ((Vertex3d*)m_vVertices)[m_lTriangleList[3 * lTri + 1]].z;
		v3.x = ((Vertex3d*)m_vVertices)[m_lTriangleList[3 * lTri + 2]].x;
		v3.y = ((Vertex3d*)m_vVertices)[m_lTriangleList[3 * lTri + 2]].y;
		v3.z = ((Vertex3d*)m_vVertices)[m_lTriangleList[3 * lTri + 2]].z;
		break;
	case VERTEXTYPE_V3fT2f:
		v1.x = ((TexturedVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri]].x;
		v1.y = ((TexturedVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri]].y;
		v1.z = ((TexturedVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri]].z;
		v2.x = ((TexturedVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri + 1]].x;
		v2.y = ((TexturedVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri + 1]].y;
		v2.z = ((TexturedVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri + 1]].z;
		v3.x = ((TexturedVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri + 2]].x;
		v3.y = ((TexturedVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri + 2]].y;
		v3.z = ((TexturedVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri + 2]].z;
		break;
	case VERTEXTYPE_V3dT2d:
		v1.x = ((TexturedVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri]].x;
		v1.y = ((TexturedVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri]].y;
		v1.z = ((TexturedVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri]].z;
		v2.x = ((TexturedVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri + 1]].x;
		v2.y = ((TexturedVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri + 1]].y;
		v2.z = ((TexturedVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri + 1]].z;
		v3.x = ((TexturedVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri + 2]].x;
		v3.y = ((TexturedVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri + 2]].y;
		v3.z = ((TexturedVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri + 2]].z;
		break;
	case VERTEXTYPE_V3fN3fC3f:
		v1.x = ((ShadedVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri]].x;
		v1.y = ((ShadedVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri]].y;
		v1.z = ((ShadedVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri]].z;
		v2.x = ((ShadedVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri + 1]].x;
		v2.y = ((ShadedVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri + 1]].y;
		v2.z = ((ShadedVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri + 1]].z;
		v3.x = ((ShadedVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri + 2]].x;
		v3.y = ((ShadedVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri + 2]].y;
		v3.z = ((ShadedVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri + 2]].z;
		break;
	case VERTEXTYPE_V3dN3dC3f:
		v1.x = ((ShadedVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri]].x;
		v1.y = ((ShadedVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri]].y;
		v1.z = ((ShadedVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri]].z;
		v2.x = ((ShadedVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri + 1]].x;
		v2.y = ((ShadedVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri + 1]].y;
		v2.z = ((ShadedVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri + 1]].z;
		v3.x = ((ShadedVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri + 2]].x;
		v3.y = ((ShadedVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri + 2]].y;
		v3.z = ((ShadedVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri + 2]].z;
		break;
	case VERTEXTYPE_V3fT2fN3fC3f:
		v1.x = ((VersatileVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri]].x;
		v1.y = ((VersatileVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri]].y;
		v1.z = ((VersatileVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri]].z;
		v2.x = ((VersatileVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri + 1]].x;
		v2.y = ((VersatileVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri + 1]].y;
		v2.z = ((VersatileVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri + 1]].z;
		v3.x = ((VersatileVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri + 2]].x;
		v3.y = ((VersatileVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri + 2]].y;
		v3.z = ((VersatileVertex3f*)m_vVertices)[m_lTriangleList[3 * lTri + 2]].z;
		break;
	case VERTEXTYPE_V3dT2dN3dC3f:
		v1.x = ((VersatileVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri]].x;
		v1.y = ((VersatileVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri]].y;
		v1.z = ((VersatileVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri]].z;
		v2.x = ((VersatileVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri + 1]].x;
		v2.y = ((VersatileVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri + 1]].y;
		v2.z = ((VersatileVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri + 1]].z;
		v3.x = ((VersatileVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri + 2]].x;
		v3.y = ((VersatileVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri + 2]].y;
		v3.z = ((VersatileVertex3d*)m_vVertices)[m_lTriangleList[3 * lTri + 2]].z;
		break;
	}
	if (v1 != v2 && v3 != v2 && v1 != v3)
	{
		Vertex3d vNrm = (v2 - v1) ^ (v3 - v2);
		vNrm.normalize();
		return vNrm;
	}
	else
	{
		return Vertex3d(0.0, 0.0, 0.0);
	}
}

TriangleMesh3dImpl::TriangleMesh3dImpl()
{
	m_byteTypeID = GEOTYPE_3D_TRIANGLEMESH;

	m_vVertices = NULL;

	m_lVertNum = 0;
	m_iVertexType = VERTEXTYPE_V3d;
	m_lTriangleList = NULL;
	m_lTriListSize = 0;
	m_lStripIndices = NULL;
	m_lStripListSize = 0;
	m_uiTriangleMark = NULL;
	m_vectBoundaryEdges.clear();
	m_uiBoundEdgeMark = NULL;
}
TriangleMesh3dImpl::~TriangleMesh3dImpl()
{
	if (m_lTriangleList)
	{
		delete[]m_lTriangleList;
		m_lTriangleList = NULL;
	}
	if (m_lStripIndices)
	{
		delete[]m_lStripIndices;
		m_lStripIndices = NULL;
	}
	if (m_uiTriangleMark)
	{
		delete[] m_uiTriangleMark;
		m_uiTriangleMark = NULL;
	}
	m_vectBoundaryEdges.clear();
	if (m_uiBoundEdgeMark)
	{
		delete[] m_uiBoundEdgeMark;
		m_uiBoundEdgeMark = NULL;
	}
}

char  TriangleMesh3dImpl::RPointWithGeometry(Vertex3d& vPt) const
{
	// ֻ��Ҫ�жϵ��Ƿ���ĳ����������
	char cRes = Geometry3dImpl::RPointWithGeometry(vPt);
	if (cRes == 'o')
		return 'o';

	int f, k = 0;
	char code = '?';
	double dFaceSize = m_lTriListSize / 3;

	vector<Vertex3d> vt3d;
	GetVerticesEx(vt3d);

	for (f = 0; f < dFaceSize; f++) {  /* Begin check each face */
		code = RPointWithTri(vt3d[m_lTriangleList[f * 3]], vt3d[m_lTriangleList[f * 3 + 1]], vt3d[m_lTriangleList[f * 3 + 2]], vPt, false);
		if (code == 'V' || code == 'E')
			return code;
		if (code == 'F')
			return 'i';
	}

	return 'o';

	// ***************************************************************

	// 	char cRes = Geometry3dImpl::RPointWithGeometry(vPt);
	// 	if(cRes == 'o')
	// 		return 'o';
	// 
	// 	Vertex3d r;  /* Ray endpoint. */
	// 	Vertex3d p;  /* Intersection point; not used. */
	// 	int f, k = 0, crossings = 0;
	// 	char code = '?';
	// 	
	// 	double radius = m_eEnvelope.Diagonal();
	// 	double dFaceSize = m_lTriListSize / 3;
	// 
	// 	vector<Vertex3d> vt3d;
	// 	GetVerticesEx(vt3d);
	// 
	// LOOP:
	// 	//while( k++ < dFaceSize ) {
	// 	while(1){
	// 		crossings = 0;
	// 
	// 		ct3d::GetRandomRay( r, radius ); // �������ray������������Ϊradius
	// 		r = vPt + r;	// ����r�õ�ray���յ�����
	// 
	// 		for ( f = 0; f < dFaceSize; f++ ) {  /* Begin check each face */
	// 			AABBoxImpl env;
	// 			env.Union(vt3d[m_lTriangleList[f*3]]);
	// 			env.Union(vt3d[m_lTriangleList[f*3+1]]);
	// 			env.Union(vt3d[m_lTriangleList[f*3+2]]);
	// 			
	// 			if ((vPt.x - env.GetMin().x) < -MathEx::TOL && (r.x - env.GetMin().x) < -MathEx::TOL || 
	// 				(vPt.y - env.GetMin().y) < -MathEx::TOL && (r.y - env.GetMin().y) < -MathEx::TOL ||
	// 				(vPt.z - env.GetMin().z) < -MathEx::TOL && (r.z - env.GetMin().z) < -MathEx::TOL)	//	segment��������������ཻ
	// 				code = '0';	
	// 			else if ((vPt.x - env.GetMax().x) > MathEx::TOL && (r.x - env.GetMax().x) > MathEx::TOL || 
	// 				(vPt.y - env.GetMax().y) > MathEx::TOL && (r.y - env.GetMax().y) >  MathEx::TOL ||
	// 				(vPt.z - env.GetMax().z) > MathEx::TOL && (r.z - env.GetMax().z) > 	MathEx::TOL )//	segment��������������ཻ
	// 				code = '0';	
	// 			else 
	// 				code = ct3d::RRayCrossTri(vPt,r,vt3d[m_lTriangleList[f*3]],vt3d[m_lTriangleList[f*3+1]],vt3d[m_lTriangleList[f*3+2]]);
	// 
	// 			/* If ray is degenerate, then goto outer while to generate another. */
	// 			if ( code == 'p' || code == 'v' || code == 'e' ) {
	// 				goto LOOP;
	// 			}
	// 
	// 			/* If ray hits face at interior point, increment crossings. */
	// 			else if ( code == 'f' ) {
	// 				crossings++;
	// 			}
	// 
	// 			/* If query endpoint q sits on a V/E/F, return that code. */
	// 			else if ( code == 'V' || code == 'E' || code == 'F' )
	// 				return( code );
	// 
	// 			/* If ray misses triangle, do nothing. */
	// 			else if ( code == '0' )
	// 				;
	// 
	// 			else 
	// 				return '?';
	// 
	// 		} /* End check each face */
	// 
	// 		/* No degeneracies encountered: ray is generic, so finished. */
	// 		break;
	// 
	// 	} /* End while loop */
	// 
	// 	TRACE( "Crossings = %d\n", crossings );
	// 	/* q strictly interior to polyhedron if an odd number of crossings. */
	// 	if( ( crossings % 2 ) == 1 )	// �������ཻ
	// 		return   'i';
	// 	else return 'o';

}

int TriangleMesh3dImpl::FrustumIntersectGeometry(FrustumImpl& frustum)
{
	int nRes = Geometry3dImpl::FrustumIntersectGeometry(frustum);

	if (nRes == 0 || nRes == 1)
		return nRes;

	double dFaceSize = m_lTriListSize / 3;
	int bCross = 0;
	vector<Vertex3d> vt3d;
	GetVerticesEx(vt3d);

	for (int f = 0; f < dFaceSize; f++)
	{
		if ('1' == RFrustumWithTriangle(frustum, vt3d[m_lTriangleList[f * 3]], vt3d[m_lTriangleList[f * 3 + 1]], vt3d[m_lTriangleList[f * 3 + 2]]))
			return 1;
	}
	return 0;
}

int TriangleMesh3dImpl::RayCrossGeometry(RayImpl& ray, double& dLength) const
{
	if (0 == Geometry3dImpl::RayCrossGeometry(ray, dLength))
		return 0;

	double dFaceSize = m_lTriListSize / 3;
	int bCross = 0;
	vector<Vertex3d> vt3d;
	GetVerticesEx(vt3d);

	Vertex3d vPt1 = ray.origin;
	Vertex3d vPt2 = ray.origin + ray.direction;

	Vertex3d IntersectPt;
	for (int f = 0; f < dFaceSize; f++)
	{  /* Begin check each face */
		PLANE3D plane;
		Vertex3d triPt1 = vt3d[m_lTriangleList[f * 3]];
		Vertex3d triPt2 = vt3d[m_lTriangleList[f * 3 + 1]];
		Vertex3d triPt3 = vt3d[m_lTriangleList[f * 3 + 2]];

		AABBoxImpl env;
		env.Union(triPt1);
		env.Union(triPt2);
		env.Union(triPt3);

		int rRes = IsRayCrossEnvelope(ray, env);
		if (rRes == 0)
			continue;

		int bRes = GetPlaneEquation(triPt1, triPt2, triPt3, plane);
		if (!bRes)
			continue;
		char cRes = RLineWithPlane(vPt1, vPt2, plane, &IntersectPt);
		if (cRes == '1')
		{
			if (IsPointInTri(triPt1, triPt2, triPt3, IntersectPt))//������������
			{
				double dRes = (vPt2 - vPt1) * (IntersectPt - vPt1);
				//	if(dRes > 1e-6) //xx_0128
				if (dRes > MathEx::TOL)
				{
					// ����Ҫע����� ��Ҫ�Ƚ��ȴ����ĸ�ǽ��
					double tempDis = DPointToPointEx(IntersectPt, vPt1);
					//	if(tempDis < dLength)
					if ((tempDis - dLength) < -MathEx::TOL)
					{
						dLength = tempDis;
						bCross = 1;
					}
				}
			}
		}
	}
	return bCross;
}

double TriangleMesh3dImpl::DPointToGeometryEx(Vertex3d& vPt) const
{
	double dFaceSize = m_lTriListSize / 3;

	vector<Vertex3d> vt3d;
	GetVerticesEx(vt3d);

	double dMin;
	Vertex3d tempPt;
	for (int f = 0; f < dFaceSize; f++)
	{  /* Begin check each face */
		double dDis = DPointToTriangleEx(vPt, vt3d[m_lTriangleList[f * 3]], vt3d[m_lTriangleList[f * 3 + 1]], vt3d[m_lTriangleList[f * 3 + 2]], &tempPt);
		if (dDis == 0)
			DPointToTriangleEx(vPt, vt3d[m_lTriangleList[f * 3]], vt3d[m_lTriangleList[f * 3 + 1]], vt3d[m_lTriangleList[f * 3 + 2]], &tempPt);
		if (f == 0)
			dMin = dDis;
		else
		{
			//	if(dMin > dDis) //xx_0128
			if ((dMin - dDis) > MathEx::TOL)
				dMin = dDis;
		}
	}
	return dMin;
}
void TriangleMesh3dImpl::GetVerticesEx(vector<Vertex3d>& vt3d) const
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

bool TriangleMesh3dImpl::readBuffer(Buffer & buf)
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

	//������������

	buf.read((char*)&m_lTriListSize, sizeof(long));
	if (m_lTriListSize > 0)
	{
		m_lTriangleList = new long[m_lTriListSize];
		buf.read((char*)m_lTriangleList, sizeof(long) * m_lTriListSize);
	}

	//����������������

	buf.read((char*)&m_lStripListSize, sizeof(long));
	if (m_lStripListSize > 0)
	{
		m_lStripIndices = new long[m_lStripListSize];
		buf.read((char*)m_lStripIndices, sizeof(long) * m_lStripListSize);
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


bool TriangleMesh3dImpl::writeBuffer(Buffer& buf)
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

	//������������

	buf.write((char*)&m_lTriListSize, sizeof(long));
	if (m_lTriListSize > 0)
	{
		buf.write((char*)m_lTriangleList, sizeof(long) * m_lTriListSize);
	}

	//����������������

	buf.write((char*)&m_lStripListSize, sizeof(long));
	if (m_lStripListSize > 0)
	{
		buf.write((char*)m_lStripIndices, sizeof(long) * m_lStripListSize);
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

size_t TriangleMesh3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	//������Ϣ(Geometry��3DGeometry)
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

	nSize += sizeof(long); // 	long m_lTriListSize;
	nSize += sizeof(long) * m_lTriListSize; // 	long* m_lTriangleList;

	nSize += sizeof(long); // 	long m_lStripListSize;
	nSize += sizeof(long) * m_lStripListSize; // 	long* m_lStripIndices;


	//*************************************************

	return nSize;
}

void TriangleMesh3dImpl::TranslateFrom(Matrix4x4& m4x4)
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
			//pVetexList[i] *= m4x4;

			m4x4.transformAffine(pVetexList[i]);
			m_eEnvelope.Union(pVetexList[i]);
		}
		else if (m_iVertexType == VERTEXTYPE_V3f)
		{
			Vertex3f* pVetexList = (Vertex3f*)m_vVertices;
			m4x4.transformAffine(pVetexList[i]);
			//pVetexList[i] *= m4x4;

			v.x = pVetexList[i].x;
			v.y = pVetexList[i].y;
			v.z = pVetexList[i].z;
			m_eEnvelope.Union(v);

		}
		else if (m_iVertexType == VERTEXTYPE_V3dT2d)
		{
			TexturedVertex3d* pVetexList = (TexturedVertex3d*)m_vVertices;
			//pVetexList[i] *= m4x4;	
			m4x4.transformAffine(pVetexList[i]);

			m_eEnvelope.Union(pVetexList[i]);
		}
		else if (m_iVertexType == VERTEXTYPE_V3fT2f)
		{
			TexturedVertex3f* pVetexList = (TexturedVertex3f*)m_vVertices;
			//pVetexList[i] *= m4x4;	
			m4x4.transformAffine(pVetexList[i]);

			v.x = pVetexList[i].x;
			v.y = pVetexList[i].y;
			v.z = pVetexList[i].z;
			m_eEnvelope.Union(v);
		}
		else if (m_iVertexType == VERTEXTYPE_V3dN3dC3f)
		{
			ShadedVertex3d* pVetexList = (ShadedVertex3d*)m_vVertices;
			//pVetexList[i] *= m4x4;	
			m4x4.transformAffine(pVetexList[i]);

			m_eEnvelope.Union(pVetexList[i]);
		}
		else if (m_iVertexType == VERTEXTYPE_V3fN3fC3f)
		{
			ShadedVertex3f* pVetexList = (ShadedVertex3f*)m_vVertices;
			//pVetexList[i] *= m4x4;	
			m4x4.transformAffine(pVetexList[i]);

			v.x = pVetexList[i].x;
			v.y = pVetexList[i].y;
			v.z = pVetexList[i].z;
			m_eEnvelope.Union(v);
		}
		else if (m_iVertexType == VERTEXTYPE_V3dT2dN3dC3f)
		{
			VersatileVertex3d* pVetexList = (VersatileVertex3d*)m_vVertices;
			//pVetexList[i] *= m4x4;
			m4x4.transformAffine(pVetexList[i]);

			m_eEnvelope.Union(pVetexList[i]);
		}
		else if (m_iVertexType == VERTEXTYPE_V3fT2fN3fC3f)
		{
			VersatileVertex3f* pVetexList = (VersatileVertex3f*)m_vVertices;
			//pVetexList[i] *= m4x4;	
			m4x4.transformAffine(pVetexList[i]);

			v.x = pVetexList[i].x;
			v.y = pVetexList[i].y;
			v.z = pVetexList[i].z;
			m_eEnvelope.Union(v);
		}
	}

}

GeometryImpl* TriangleMesh3dImpl::CreateObject()
{
	return new TriangleMesh3dImpl;
}
//�����ⲿָ������ά�߶ε���ǰ���ζ�������ƽ��
double TriangleMesh3dImpl::D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt)
{
	double dis, mindis = 1e+14;
	Vertex3d p1, p2, p3;

	std::vector<Vertex3d> positons;
	GetVerticesEx(positons);

	for (long i = 0; i<m_lTriListSize / 3; i++)//����ÿ��������
	{
		p1 = positons[m_lTriangleList[3 * i]];
		p2 = positons[m_lTriangleList[3 * i + 1]];
		p3 = positons[m_lTriangleList[3 * i + 2]];

		dis = DLineSegToTriangleEx(begvPt, endvPt, p1, p2, p3);
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	}

	return mindis;
}

//�����ⲿָ���������ε���ǰ���ζ�������ƽ��
double TriangleMesh3dImpl::D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3)
{
	double dis, mindis = 1e+14;
	Vertex3d p1, p2, p3;

	std::vector<Vertex3d> positons;
	GetVerticesEx(positons);

	for (long i = 0; i<m_lTriListSize / 3; i++)//����ÿ��������
	{
		p1 = positons[m_lTriangleList[3 * i]];
		p2 = positons[m_lTriangleList[3 * i + 1]];
		p3 = positons[m_lTriangleList[3 * i + 2]];

		dis = DTriangleToTriangleEx(triPt1, triPt2, triPt3, p1, p2, p3);
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	}

	return mindis;
}

//���㵱ǰ����ļ�������
Vertex3d TriangleMesh3dImpl::CalCorePoint()
{

	std::vector<Vertex3d> positons;
	GetVerticesEx(positons);

	Vertex3d corePt;
	for (long i = 0; i < m_lVertNum; i++)
		corePt += positons[i];
	corePt /= m_lVertNum;

	return corePt;
}

/** �麯��
* ͳ�Ƶ�ǰ��������ļ�������
* @return long*:�����ڲ����ٿռ䡢�ⲿ�ͷ�
*               ����ֵ������ -- ������(0)���������Ч����������(1)���������Ч��ɫ��(2)��
*								�������(3)��������(4)��ת��Ϊ�ȼ�����(5)
*								������(6)��������(7)��
*/
long* TriangleMesh3dImpl::GeoStatistics()
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
	statis[4] = m_lTriListSize / 3;
	statis[5] = statis[4];

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
bool TriangleMesh3dImpl::Init()
{
	bool res = Surface3dImpl::Init();
	if (false == res) return false;

	VertexVisitor::releaseVertexArray(m_iVertexType, m_vVertices);

	if (m_lTriangleList)
	{
		delete[]m_lTriangleList;
		m_lTriangleList = NULL;
	}
	if (m_lStripIndices)
	{
		delete[]m_lStripIndices;
		m_lStripIndices = NULL;
	}
	if (m_uiTriangleMark)
	{
		delete[] m_uiTriangleMark;
		m_uiTriangleMark = NULL;
	}
	m_vectBoundaryEdges.clear();
	if (m_uiBoundEdgeMark)
	{
		delete[] m_uiBoundEdgeMark;
		m_uiBoundEdgeMark = NULL;
	}

	m_lVertNum = 0;
	m_iVertexType = VERTEXTYPE_V3d;
	m_lTriListSize = 0;
	m_lStripListSize = 0;

	return true;
}

int TriangleMesh3dImpl::RGeoWithPolygon2D(vector<Vertex3d>& ptArray, int precisionFlag)
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
		GetVerticesEx(vt3d);

		for (long i = 0; i<m_lTriListSize / 3; i++)//����ÿ��������
		{
			vector<Vertex3d> tempVs;
			tempVs.push_back(vt3d[m_lTriangleList[3 * i]]);
			tempVs.push_back(vt3d[m_lTriangleList[3 * i + 1]]);
			tempVs.push_back(vt3d[m_lTriangleList[3 * i + 2]]);

			if (0 != RPolygonWithPolygon2D(tempVs, ptArray))
				return 1;
		}

		return 0;
	}
	else
		return -1;
}

end_gdb_namespace
end_gtl_namespace

