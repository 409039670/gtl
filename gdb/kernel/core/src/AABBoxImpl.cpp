#include "internal_AABBoxImpl.h"
#include "internal_spatialfuns.h"


begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

Vertex3d AABBoxImpl::FetchCornerPoint(int iIndex) const
{
	assert(iIndex >= 0 && iIndex <= 7);
	return Vertex3d((iIndex & 1) ? m_VMax.x : m_VMin.x,
		(iIndex & 2) ? m_VMax.y : m_VMin.y,
		(iIndex & 4) ? m_VMax.z : m_VMin.z);
}

Vertex3d AABBoxImpl::GetClosestVertexToAnotherPt(Vertex3d& vVer) const
{
	Vertex3d vRes;
	//	if(vVer.x < m_VMin.x)  //xx_0128
	if ((vVer.x - m_VMin.x) < -MathEx::TOL)
		vRes.x = m_VMin.x;
	//	else if(vVer.x > m_VMax.x) //xx_0128
	else if ((vVer.x - m_VMax.x) > MathEx::TOL)
		vRes.x = m_VMax.x;
	else
		vRes.x = vVer.x;

	//	if(vVer.y < m_VMin.y)
	if ((vVer.y - m_VMin.y) < -MathEx::TOL)
		vRes.y = m_VMin.y;
	//	else if(vVer.y > m_VMax.y) //xx_0128
	else if ((vVer.y - m_VMax.y) > MathEx::TOL)
		vRes.y = m_VMax.y;
	else
		vRes.y = vVer.y;

	//	if(vVer.z < m_VMin.z)
	if ((vVer.z - m_VMin.z) < -MathEx::TOL)
		vRes.z = m_VMin.z;
	//	else if(vVer.z > m_VMax.z) //xx_0128
	else if ((vVer.z - m_VMax.z) > MathEx::TOL)
		vRes.z = m_VMax.z;
	else
		vRes.z = vVer.z;

	return vRes;
}

double AABBoxImpl::GetClosestLineDisToAABBBox(Vertex3d& begVer, Vertex3d& endVer) const
{
	Vertex3d VArray[8];
	VArray[0] = m_VMin;
	VArray[1].x = m_VMax.x; VArray[1].y = m_VMin.y; VArray[1].z = m_VMin.z;
	VArray[2].x = m_VMax.x; VArray[2].y = m_VMax.y; VArray[2].z = m_VMin.z;
	VArray[3].x = m_VMin.x; VArray[3].y = m_VMax.y; VArray[3].z = m_VMin.z;
	VArray[4].x = m_VMin.x; VArray[4].y = m_VMin.y; VArray[4].z = m_VMax.z;
	VArray[5].x = m_VMax.x; VArray[5].y = m_VMin.y; VArray[5].z = m_VMax.z;
	VArray[6] = m_VMax;
	VArray[7].x = m_VMin.x; VArray[7].y = m_VMax.y; VArray[7].z = m_VMax.z;

	double dis, mindis = 1e+14;
	dis = DLineSegToTriangle(begVer, endVer, VArray[0], VArray[1], VArray[2]);
	//if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DLineSegToTriangle(begVer, endVer, VArray[0], VArray[2], VArray[3]);
	//if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	dis = DLineSegToTriangle(begVer, endVer, VArray[4], VArray[5], VArray[6]);
	//if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DLineSegToTriangle(begVer, endVer, VArray[4], VArray[6], VArray[7]);
	//if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	dis = DLineSegToTriangle(begVer, endVer, VArray[0], VArray[1], VArray[5]);
	//if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DLineSegToTriangle(begVer, endVer, VArray[0], VArray[5], VArray[4]);
	//if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	dis = DLineSegToTriangle(begVer, endVer, VArray[1], VArray[2], VArray[6]);
	//if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DLineSegToTriangle(begVer, endVer, VArray[1], VArray[6], VArray[5]);
	//if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	dis = DLineSegToTriangle(begVer, endVer, VArray[2], VArray[6], VArray[7]);
	//if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DLineSegToTriangle(begVer, endVer, VArray[2], VArray[7], VArray[3]);
	//if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	dis = DLineSegToTriangle(begVer, endVer, VArray[0], VArray[3], VArray[7]);
	//if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DLineSegToTriangle(begVer, endVer, VArray[0], VArray[7], VArray[4]);
	//if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	return mindis;
}

/** ���㴫�������ε�AABB����С����
* @param [in] begVer,endVer�����߶ζ˵�
* @return �봫��������AABB�ĵ�
*/
double AABBoxImpl::GetClosestTriangleDisToAABBBox(Vertex3d& triPts1, Vertex3d& triPts2, Vertex3d& triPts3) const
{
	Vertex3d VArray[8];
	VArray[0] = m_VMin;
	VArray[1].x = m_VMax.x; VArray[1].y = m_VMin.y; VArray[1].z = m_VMin.z;
	VArray[2].x = m_VMax.x; VArray[2].y = m_VMax.y; VArray[2].z = m_VMin.z;
	VArray[3].x = m_VMin.x; VArray[3].y = m_VMax.y; VArray[3].z = m_VMin.z;
	VArray[4].x = m_VMin.x; VArray[4].y = m_VMin.y; VArray[4].z = m_VMax.z;
	VArray[5].x = m_VMax.x; VArray[5].y = m_VMin.y; VArray[5].z = m_VMax.z;
	VArray[6] = m_VMax;
	VArray[7].x = m_VMin.x; VArray[7].y = m_VMax.y; VArray[7].z = m_VMax.z;

	double dis, mindis = 1e+14;
	dis = DTriangleToTriangle(triPts1, triPts2, triPts3, VArray[0], VArray[1], VArray[2]);
	//if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DTriangleToTriangle(triPts1, triPts2, triPts3, VArray[0], VArray[2], VArray[3]);
	//if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	dis = DTriangleToTriangle(triPts1, triPts2, triPts3, VArray[4], VArray[5], VArray[6]);
	//if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DTriangleToTriangle(triPts1, triPts2, triPts3, VArray[4], VArray[6], VArray[7]);
	//if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	dis = DTriangleToTriangle(triPts1, triPts2, triPts3, VArray[0], VArray[1], VArray[5]);
	//if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DTriangleToTriangle(triPts1, triPts2, triPts3, VArray[0], VArray[5], VArray[4]);
	//if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	dis = DTriangleToTriangle(triPts1, triPts2, triPts3, VArray[1], VArray[2], VArray[6]);
	//if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DTriangleToTriangle(triPts1, triPts2, triPts3, VArray[1], VArray[6], VArray[5]);
	//if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	dis = DTriangleToTriangle(triPts1, triPts2, triPts3, VArray[2], VArray[6], VArray[7]);
	//if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DTriangleToTriangle(triPts1, triPts2, triPts3, VArray[2], VArray[7], VArray[3]);
	//if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	dis = DTriangleToTriangle(triPts1, triPts2, triPts3, VArray[0], VArray[3], VArray[7]);
	//if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DTriangleToTriangle(triPts1, triPts2, triPts3, VArray[0], VArray[7], VArray[4]);
	//if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	return mindis;
}

AABBoxImpl AABBoxImpl::Union(const Vertex3d& pnt)
{
	//xx_0128
	//	if(pnt.x < m_VMin.x) m_VMin.x = pnt.x;
	if ((pnt.x - m_VMin.x) < -MathEx::TOL) m_VMin.x = pnt.x;
	//	if(pnt.x > m_VMax.x) m_VMax.x = pnt.x;
	if ((pnt.x - m_VMax.x) > MathEx::TOL) m_VMax.x = pnt.x;
	//	if(pnt.y < m_VMin.y) m_VMin.y = pnt.y;
	if ((pnt.y - m_VMin.y) < -MathEx::TOL) m_VMin.y = pnt.y;
	//	if(pnt.y > m_VMax.y) m_VMax.y = pnt.y;
	if ((pnt.y - m_VMax.y) > MathEx::TOL) m_VMax.y = pnt.y;
	//	if(pnt.z < m_VMin.z) m_VMin.z = pnt.z;
	if ((pnt.z - m_VMin.z) < -MathEx::TOL) m_VMin.z = pnt.z;
	//	if(pnt.z > m_VMax.z) m_VMax.z = pnt.z;
	if ((pnt.z - m_VMax.z) > MathEx::TOL) m_VMax.z = pnt.z;
	return *this;
}

AABBoxImpl AABBoxImpl::Union(const Vertex3d* pnts, int nPntNum)
{
	for (int i = 0; i<nPntNum; i++)
		Union(pnts[i]);
	return *this;
}

AABBoxImpl AABBoxImpl::Union(const std::vector<Vertex3d>& pnts)
{
	int nCount = pnts.size();
	for (int i = 0; i<nCount; i++)
		Union(pnts[i]);
	return *this;
}

AABBoxImpl AABBoxImpl::Union(const AABBoxImpl& otherEnvlp)
{
	/*if(m_VMax.x < otherEnvlp.m_VMax.x)	m_VMax.x = otherEnvlp.m_VMax.x;
	if(m_VMax.y < otherEnvlp.m_VMax.y)	m_VMax.y = otherEnvlp.m_VMax.y;
	if(m_VMax.z < otherEnvlp.m_VMax.z)	m_VMax.z = otherEnvlp.m_VMax.z;*/
	if ((m_VMax.x - otherEnvlp.m_VMax.x) < -MathEx::TOL)	m_VMax.x = otherEnvlp.m_VMax.x;
	if ((m_VMax.y - otherEnvlp.m_VMax.y) < -MathEx::TOL)	m_VMax.y = otherEnvlp.m_VMax.y;
	if ((m_VMax.z - otherEnvlp.m_VMax.z) < -MathEx::TOL)	m_VMax.z = otherEnvlp.m_VMax.z;

	// xx_0128
	/*if(m_VMin.x > otherEnvlp.m_VMin.x)	m_VMin.x = otherEnvlp.m_VMin.x;
	if(m_VMin.y > otherEnvlp.m_VMin.y)	m_VMin.y = otherEnvlp.m_VMin.y;
	if(m_VMin.z > otherEnvlp.m_VMin.z)	m_VMin.z = otherEnvlp.m_VMin.z;*/
	if ((m_VMin.x - otherEnvlp.m_VMin.x) > MathEx::TOL)	m_VMin.x = otherEnvlp.m_VMin.x;
	if ((m_VMin.y - otherEnvlp.m_VMin.y) > MathEx::TOL)	m_VMin.y = otherEnvlp.m_VMin.y;
	if ((m_VMin.z - otherEnvlp.m_VMin.z) > MathEx::TOL)	m_VMin.z = otherEnvlp.m_VMin.z;
	return *this;
}

int  AABBoxImpl::Intersect(const AABBoxImpl& otherbox, AABBoxImpl& retBox) const
{
	//	if(	m_VMax.x < otherbox.m_VMin.x ||  //xx_0128
	if ((m_VMax.x - otherbox.m_VMin.x) < -MathEx::TOL ||
		//		m_VMin.x > otherbox.m_VMax.x || 
		(m_VMin.x - otherbox.m_VMax.x) > MathEx::TOL ||
		//		m_VMax.y < otherbox.m_VMin.y || 
		(m_VMax.y - otherbox.m_VMin.y) < -MathEx::TOL ||
		//		m_VMin.y > otherbox.m_VMax.y ||
		(m_VMin.y - otherbox.m_VMax.y) > MathEx::TOL ||
		//		m_VMax.z < otherbox.m_VMin.z || 
		(m_VMax.z - otherbox.m_VMin.z) < -MathEx::TOL ||
		//		m_VMin.z > otherbox.m_VMax.z
		(m_VMin.z - otherbox.m_VMax.z) > MathEx::TOL
		)//���ཻ
		return 0;

	//	if(m_VMax.x > otherbox.m_VMax.x && m_VMax.y > otherbox.m_VMax.y && m_VMax.z > otherbox.m_VMax.z  //xx_0128
	if (((m_VMax.x - otherbox.m_VMax.x) > MathEx::TOL) && ((m_VMax.y - otherbox.m_VMax.y) > MathEx::TOL) && ((m_VMax.z - otherbox.m_VMax.z) > MathEx::TOL)
		//&& m_VMin.x < otherbox.m_VMin.x && m_VMin.y < otherbox.m_VMin.y && m_VMin.z < otherbox.m_VMin.z)//����
		&& (m_VMin.x - otherbox.m_VMin.x) < -MathEx::TOL && (m_VMin.y - otherbox.m_VMin.y) < -MathEx::TOL && (m_VMin.z - otherbox.m_VMin.z) < -MathEx::TOL)//����
		return 2;

	if (m_VMax.x < otherbox.m_VMax.x && m_VMax.y < otherbox.m_VMax.y && m_VMax.z < otherbox.m_VMax.z
		//	&& m_VMin.x > otherbox.m_VMin.x && m_VMin.y > otherbox.m_VMin.y && m_VMin.z > otherbox.m_VMin.z)//������
		&& ((m_VMin.x - otherbox.m_VMin.x) > MathEx::TOL) && ((m_VMin.y - otherbox.m_VMin.y) > MathEx::TOL) && ((m_VMin.z - otherbox.m_VMin.z) > MathEx::TOL))//������
		return 3;

	retBox.m_VMax.x = min(m_VMax.x, otherbox.m_VMax.x);
	retBox.m_VMax.y = min(m_VMax.y, otherbox.m_VMax.y);
	retBox.m_VMax.z = min(m_VMax.z, otherbox.m_VMax.z);
	retBox.m_VMin.x = max(m_VMin.x, otherbox.m_VMin.x);
	retBox.m_VMin.y = max(m_VMin.y, otherbox.m_VMin.y);
	retBox.m_VMin.z = max(m_VMin.z, otherbox.m_VMin.z);

	return 1;
}

int AABBoxImpl::IsPointInBox(const Vertex3d& vPt) const
{
	Vertex3d vmin = m_VMin;
	/*	int bRes = (vPt.x >= m_VMin.x) && (vPt.x <= m_VMax.x) &&
	(vPt.y >= m_VMin.y) && (vPt.y <= m_VMax.y) &&
	(vPt.z >= m_VMin.z) && (vPt.z <= m_VMax.z);*/

	int bRes = (vPt.x - m_VMin.x > -MathEx::TOL) &&
		(vPt.x - m_VMax.x <  MathEx::TOL) &&
		(vPt.y - m_VMin.y > -MathEx::TOL) &&
		(vPt.y - m_VMax.y < MathEx::TOL) &&
		(vPt.z - m_VMin.z > -MathEx::TOL) &&
		(vPt.z - m_VMax.z < MathEx::TOL);

	return bRes;
}

int AABBoxImpl::IsBoxIntersectWithGivenBox(const AABBoxImpl& inBox) const // xx_modify_0310
{
	Vertex3d Vmin = inBox.GetMin();
	Vertex3d Vmax = inBox.GetMax();

	// �ֱ��ж����������X\Y\Z�����ͶӰ
	if ((Vmin.x - m_VMax.x) > MathEx::TOL || (m_VMin.x - Vmax.x) > MathEx::TOL) return 0;
	if ((Vmin.y - m_VMax.y) > MathEx::TOL || (m_VMin.y - Vmax.y) > MathEx::TOL) return 0;
	if ((Vmin.z - m_VMax.z) > MathEx::TOL || (m_VMin.z - Vmax.z) > MathEx::TOL) return 0;
	return 1;
}


int AABBoxImpl::IsBoxIntersectWithGivenBox2D(const AABBoxImpl& inBox, char proAxis, AABBoxImpl* intersectBox) const // xx_add_0617
{
	Vertex3d Vmin = inBox.GetMin();
	Vertex3d Vmax = inBox.GetMax();

	if (proAxis == 'z')
	{
		if ((Vmin.x - m_VMax.x) > MathEx::TOL || (m_VMin.x - Vmax.x) > MathEx::TOL) return 0;
		if ((Vmin.y - m_VMax.y) > MathEx::TOL || (m_VMin.y - Vmax.y) > MathEx::TOL) return 0;

		if (intersectBox)
		{
			Vertex3d nMin, nMax;
			nMin.x = max(Vmin.x, m_VMin.x);
			nMin.y = max(Vmin.y, m_VMin.y);

			nMax.x = min(Vmax.x, m_VMax.x);
			nMax.y = min(Vmax.y, m_VMax.y);
		}
		return 1;
	}
	else if (proAxis == 'y')
	{
		if ((Vmin.x - m_VMax.x) > MathEx::TOL || (m_VMin.x - Vmax.x) > MathEx::TOL) return 0;
		if ((Vmin.z - m_VMax.z) > MathEx::TOL || (m_VMin.z - Vmax.z) > MathEx::TOL) return 0;

		if (intersectBox)
		{
			Vertex3d nMin, nMax;
			nMin.x = max(Vmin.x, m_VMin.x);
			nMin.z = max(Vmin.z, m_VMin.z);

			nMax.x = min(Vmax.x, m_VMax.x);
			nMax.z = min(Vmax.z, m_VMax.z);
		}
		return 1;
	}
	else if (proAxis == 'x')
	{
		if ((Vmin.y - m_VMax.y) > MathEx::TOL || (m_VMin.y - Vmax.y) > MathEx::TOL) return 0;
		if ((Vmin.z - m_VMax.z) > MathEx::TOL || (m_VMin.z - Vmax.z) > MathEx::TOL) return 0;

		if (intersectBox)
		{
			Vertex3d nMin, nMax;
			nMin.y = max(Vmin.y, m_VMin.y);
			nMin.z = max(Vmin.z, m_VMin.z);

			nMax.y = min(Vmax.y, m_VMax.y);
			nMax.z = min(Vmax.z, m_VMax.z);
		}
		return 1;
	}
	else
		return 0;

}

int AABBoxImpl::IsBoxintersectWithPolygon2D(vector<Vertex3d>& ptArray)
{
	vector<Vertex3d> proBox;
	Vertex3d p2(m_VMax.x, m_VMin.y, 0.0);
	Vertex3d p4(m_VMin.x, m_VMax.y, 0.0);
	proBox.push_back(m_VMin);
	proBox.push_back(p2);
	proBox.push_back(m_VMax);
	proBox.push_back(p4);

	if (RPolygonWithPolygon2D(proBox, ptArray) == 0)
		return 0;
	else
		return 1;
}

double AABBoxImpl::Height() const
{
	return fabs(m_VMax.z - m_VMin.z);
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
