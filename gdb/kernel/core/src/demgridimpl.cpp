#include "internal_demgridimpl.h"
#include "mathex.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
 
 Vertex3d DEMGridImpl::GetMaxPoint() const
 {
 	// TODO: Add your specialized code here.
 	// NOTE: Requires a correct return value to compile.
	 return Vertex3d();
 }
 
 
 Vertex3d DEMGridImpl::GetMinPoint() const
 {
 	// TODO: Add your specialized code here.
 	// NOTE: Requires a correct return value to compile.

	 return Vertex3d();
 }
  
 
 void DEMGridImpl::Extract(int nRow, int nCol)
 {
 	// TODO: Add your specialized code here.
 }

//ע�⣺������������һ���Ǵ�����꣬���Եģ�������Եģ���DEMԭ��ģ�
//�õ�����PointIn.z
int DEMGridImpl::Interpolate(Vertex3d& PointIn)
{
	double m_THRESHOLD = -99998;

	if (m_pfDemHeightData == NULL)
		return 0;

	PointIn.z = m_lInvalidData;
	int row, col;
	double fDist[4], dx, dy, fdx, fdy;
	long np, np1, np2, np3;

	//��DEMԭ���
	double xx = PointIn.x - m_dXOrigin;
	double yy = PointIn.y - m_dYOrigin;

	//PointIn��X,Y ֵ�������ڲ�����ڵ����к�
	row = (int)(yy / m_dCellSize);
	col = (int)(xx / m_dCellSize);

	//�������к� ����ĿǰDEM ��Χ��̸߳�����Чֵ
	if (row<0 || col<0 || row >= m_nRow || col >= m_nCol)
	{
		PointIn.z = m_lInvalidData;
		return 0;
	}
	np = row * m_nCol + col;
	dx = xx - col * m_dCellSize;
	dy = yy - row * m_dCellSize;
	//if(fabs(dx)<0.001 && fabs(dy)<0.001) 
	if ((fabs(dx) - 0.001) < -MathEx::TOL && (fabs(dy) - 0.001) < -MathEx::TOL)
	{
		//	if( m_pfDemHeightData[np] < m_THRESHOLD ) 
		if ((m_pfDemHeightData[np] - m_THRESHOLD) < -MathEx::TOL)
		{
			PointIn.z = m_lInvalidData;
			return 0;
		}
		else
		{
			PointIn.z = m_pfDemHeightData[np];
			return 1;
		}
	}

	if (col != m_nCol - 1)
		np1 = np + 1;
	else
		np1 = np;

	if (row != m_nRow - 1)
		np3 = np + m_nCol;
	else
		np3 = np;

	if (col != m_nCol - 1)
		np2 = np3 + 1;
	else
		np2 = np;

	/*if(m_pfDemHeightData[np]<m_THRESHOLD || m_pfDemHeightData[np1]<m_THRESHOLD ||
	m_pfDemHeightData[np2]<m_THRESHOLD || m_pfDemHeightData[np3]<m_THRESHOLD)*/
	if ((m_pfDemHeightData[np] - m_THRESHOLD) < -MathEx::TOL || (m_pfDemHeightData[np1] - m_THRESHOLD) < -MathEx::TOL ||
		(m_pfDemHeightData[np2] - m_THRESHOLD) < -MathEx::TOL || (m_pfDemHeightData[np3] - m_THRESHOLD) < -MathEx::TOL)
	{
		PointIn.z = m_lInvalidData;
		return 0;
	}
	//	if(fabs(dx)<0.001)
	if ((fabs(dx) - 0.001) < -MathEx::TOL)
	{
		fDist[0] = dy / m_dCellSize;
		PointIn.z = (m_pfDemHeightData[np] + (m_pfDemHeightData[np3] - m_pfDemHeightData[np])*fDist[0]);
		return 1;
	}
	//else if(fabs(dy)<0.001)
	else if ((fabs(dy) - 0.001) < -MathEx::TOL)
	{
		fDist[0] = dx / m_dCellSize;
		PointIn.z = (m_pfDemHeightData[np] + (m_pfDemHeightData[np1] - m_pfDemHeightData[np])*fDist[0]);
		return 1;
	}
	else
	{
		//���ж��ĸ����Ƿ�����Чֵ��ֻ���ĸ��㶼��Ч�˲��ܽ��в�ֵ���㣩
		if ((m_lInvalidData != m_pfDemHeightData[np]) && (m_lInvalidData != m_pfDemHeightData[np1]) && (m_lInvalidData != m_pfDemHeightData[np2]) && (m_lInvalidData != m_pfDemHeightData[np3]))
		{
			fdx = m_dCellSize - dx;
			fdy = m_dCellSize - dy;
			fDist[0] = 1.0f / (dx*dx + dy*dy);
			fDist[1] = 1.0f / (fdx*fdx + dy*dy);
			fDist[2] = 1.0f / (fdx*fdx + fdy*fdy);
			fDist[3] = 1.0f / (fdy*fdy + dx*dx);
			PointIn.z = (m_pfDemHeightData[np] * fDist[0] + m_pfDemHeightData[np1] * fDist[1]
				+ m_pfDemHeightData[np2] * fDist[2] + m_pfDemHeightData[np3] * fDist[3]) / (fDist[0] + fDist[1] + fDist[2] + fDist[3]);
			return 1;
		}
		else
		{
			PointIn.z = m_lInvalidData;
			return 0;
		}
	}
}

//ֻ��ÿ���㶼�ڲ�����Ч�ظ̣߳������ŷ���TRUE;
int DEMGridImpl::Interpolate(std::vector<Vertex3d>& vVertices)
{
	long nCount = vVertices.size();

	int flag = 0;
	for (long i = 0; i<nCount; i++)
	{
		flag = Interpolate(vVertices[i]);//���� �� �ڲ��ĵ� ��Ҫ��ΪԴ���ݵ� ���������ĸ̼߳����𣿣�
		if (!flag) return 0;
	}
	return 1;
}


long DEMGridImpl::CopyDem(DEMGridImpl& src, DEMGridImpl& tar, bool bCopyData)
{
	double dXOrigin = 0.0;				//���½�Xԭ������
	double dYOrigin = 0.0;				//���½�Yԭ������
	float fMinZ = 999999;
	float fMaxZ = -99999;				//�����С�߳�ֵ
	double dAlfa = 1.0;					//alfa
	double dBelta = 0.0;				//belta
	int nCol = 1;						//DEM����
	int nRow = 1;						//DEM����
	int bCoorFlag = 1;				//����ϵ��ʾ,TRUEΪ�������,FALSEΪ��γ������ϵ(m_bZblx)
	int bUnitFlag = 1;				//���굥λ,TRUEΪ��,FALSEΪ����(m_bMeter)
	double dCellSize = 1.0;				//�������(m_fCellSize)
	long lInvalidData = -99999;			//��Ч�߳�ֵ(m_lNoData)
	long lScale = 1;					//������(m_Scale)
	float *pfDemHeightData = NULL;		//DEMʵ�ʸ߳�����(m_pDataByte)
	unsigned long *pcVexColor = NULL;		//������ɫ
	Vertex2f *pvVexTexCoor = NULL;		// ������������

	src.GetDEMOriginXY(dXOrigin, dYOrigin);
	src.GetDEMHeightRange(fMinZ, fMaxZ);
	dAlfa = src.GetDEMAlpha();
	dBelta = src.GetDEMBelta();
	src.GetDEMRowCol(nCol, nRow);
	bCoorFlag = src.GetDEMCoorFlag();
	bUnitFlag = src.GetDEMUnitFlag();
	dCellSize = src.GetDEMCellSize();
	lInvalidData = src.GetDEMInvalidata();
	lScale = src.GetDEMScale();

	tar.SetDEMOriginXY(dXOrigin, dYOrigin);
	tar.SetDEMHeightRange(fMinZ, fMaxZ);
	tar.SetDEMAlpha(dAlfa);
	tar.SetDEMBelta(dBelta);
	tar.SetDEMRowCol(nCol, nRow);
	tar.SetDEMCoorFlag(bCoorFlag);
	tar.SetDEMUnitFlag(bUnitFlag);
	tar.SetDEMCellSize(dCellSize);
	tar.SetDEMInvalidata(lInvalidData);
	tar.SetDEMScale(lScale);

	if (bCopyData)
	{
		int np = nCol * nRow;
		pfDemHeightData = new float[np];
		float* pData1 = src.GetDEMHeightData();
		memcpy(pfDemHeightData, pData1, sizeof(float)*np);


		unsigned long* pData2 = src.GetDEMVexColor();
		if (pData2)
		{
			pcVexColor = new unsigned long[np];
			memcpy(pcVexColor, pData2, sizeof(unsigned long)*np);
		}

		Vertex2f* pData3 = src.GetDEMVexTexCoor();
		if (pData3)
		{
			pvVexTexCoor = new Vertex2f[np];
			memcpy(pvVexTexCoor, pData3, sizeof(Vertex2f)*np);
		}

		tar.SetDEMHeightData(pfDemHeightData);
		tar.SetDEMVexColor(pcVexColor);
		tar.SetDEMVexTexCoor(pvVexTexCoor);

	}
	else
	{
		float* pfDemHeightData = src.GetDEMHeightData();
		tar.SetDEMHeightData(pfDemHeightData, false);
		unsigned long* pData2 = src.GetDEMVexColor();
		tar.SetDEMVexColor(pData2, false);
		Vertex2f* pvVexTexCoor = src.GetDEMVexTexCoor();
		tar.SetDEMVexTexCoor(pvVexTexCoor, false);
	}

	return 1;
}

/** ���ݴ������ת������ת��ǰDEM
* @param [in] m4x4 ��֪����ת����
* @return
*/
void DEMGridImpl::TranslateForm(Matrix4x4& m4x4)
{

}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace

