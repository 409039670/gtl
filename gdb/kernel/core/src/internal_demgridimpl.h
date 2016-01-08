#pragma once
#include "terrain.h"


begin_gtl_namespace
begin_gdb_namespace

 

class DEMGridImpl :	public DEMGrid
{
public:

public:
	/** ��Buffer�ж�ȡ��Ϣ��䱾���ζ���
	* @param  [in,out] buf Buffer & ����������������
	* @return �ɹ�����true�����򷵻�false
	*/
	virtual bool readBuffer(Buffer & buf){
		return false;
	}
	/** ���������Ϣд��Buffer��
	* @param  [in,out] buf Buffer & ����������������
	* @return �ɹ�����true�����򷵻�false
	*/
	virtual bool writeBuffer(Buffer& buf){
		return false;
	}

	virtual bool copy(const DEMGridSharedPtr & src, bool bCopyData)
	{
		return DEMGridImpl::CopyDem(*(DEMGridImpl*)(src.get()), *this, bCopyData) == 0 ? false : true;
	}
	virtual Envelope3d getEnvelope()
	{
		return  GetDEMScope(); 
	}
	virtual void setAlpha(double dAlfa){
		SetDEMAlpha(dAlfa);
	}

	virtual void setBelta(double dBelta){
		SetDEMBelta(dBelta);
	}
	virtual double getAlpha() const
	{
		return GetDEMAlpha();
	}

	virtual double getBelta() const
	{
		return GetDEMBelta();
	}

	virtual	void setOrigin(double dx, double dy)
	{
		SetDEMOriginXY(dx, dy);
	}

	virtual	void getOrigin(double& dx, double& dy)
	{
		GetDEMOriginXY(dx, dy);
	}
	virtual	void setHeightRange(float fminz, float fmaxz)
	{
		SetDEMHeightRange(fminz, fmaxz);
	}

	virtual  void getHeightRange(float& fminz, float& fmaxz)
	{
		GetDEMHeightRange(fminz, fmaxz);
	}
	virtual void setRows(int nRow){
		m_nRow = nRow;
	}

	virtual void setCols(int nCol){
		m_nCol = nCol;
	}

	virtual int getRows()const{
		return m_nRow;
	}

	virtual int getCols()const{
		return m_nCol;
	}
	virtual void setCoordinateSystem(int bFlag = 1){
		SetDEMCoorFlag(bFlag);
	}

	virtual int getCoordinateSystem(){
		return m_bCoorFlag;
	}

	virtual void setUnit(int bFlag = 1){
		m_bUnitFlag = bFlag;
	}

	virtual int getUnit(){
		return m_bUnitFlag;
	}

	virtual void setCellSize(double dSize){
		m_dCellSize = dSize;
	}
	virtual double getCellSize(){
		return m_dCellSize;
	}

	virtual void setScale(long lScale){
		m_lScale = lScale;
	}
	virtual long getScale(){
		return m_lScale;
	}

	virtual void setHeightData(float *pData, bool bAlloced = true){
		SetDEMHeightData(pData, bAlloced);
	}

	virtual float* getHeightData(){
		return m_pfDemHeightData;
	}

	virtual void setVextexColors(std::vector<Color4f> & colors){
		size_t s = colors.size();
		unsigned long * cs = new unsigned long[s];
		for (size_t i = 0; i<s; i++)
		{
			long c = 0;
			Color::convert(colors[i], c);
			cs[i] = c;
		}
		SetDEMVexColor(cs, true);
	}

	virtual void getVextexColors(std::vector<Color4f> & colors){
		//unsigned long* _colors =GetDEMVexColor();
		size_t s = getRows()*getCols();
		for (size_t i = 0; i<s; i++)
		{
			Color4f _col;
			Color::convert(m_pcVexColor[i], _col);
			colors.push_back(_col);
		}

	}

	virtual void setVertexTextureCoordinates(std::vector<Vertex2d> & VexTexCoor){
		size_t s = VexTexCoor.size();
		Vertex2f* vexTexCoor = new Vertex2f[s];
		for (size_t i = 0; i<s; i++)
		{
			vexTexCoor[i].x = VexTexCoor[i].x;
			vexTexCoor[i].y = VexTexCoor[i].y;
		}

		SetDEMVexTexCoor(vexTexCoor, true);
	}
	virtual void getVertexTextureCoordinates(std::vector<Vertex2d> & VexTexCoor){
		//Vertex2f* vexTexCoor = GetDEMVexTexCoor();
		size_t s = getRows()*getCols();
		for (size_t i = 0; i<s; i++)
		{
			VexTexCoor[i].x = m_pvVexTexCoor[i].x;
			VexTexCoor[i].y = m_pvVexTexCoor[i].y;
		}
	}
	virtual void setInvalidValue(long lInvalidata){
		m_lInvalidData = lInvalidata;
	}

	virtual long getInvalidValue(){
		return m_lInvalidData;
	}
	virtual double interpolate(const double x, const double y){
		Vertex3d _v;
		_v.x = x;
		_v.y = y;
		Interpolate(_v);
		return _v.z;
	}

	virtual int interpolate(Vertex3d & v){
		Vertex3d _v;
		converFroGv(_v, v);
		int r = Interpolate(_v);
		v.x = _v.x;
		v.y = _v.y;
		v.z = _v.z;
		return r;
	}
	virtual int interpolate(std::vector<Vertex3d>& vVertices){
		if (vVertices.empty()) return 0;
		std::vector<Vertex3d>::iterator it = vVertices.begin();

		for (; it != vVertices.end(); it++){
			interpolate(*it);
			/*if(interpolate(*it)==0){
			it->z=m_lInvalidData;
			}*/
		}
		return 1;
	}
	virtual Vertex3d getMaxPoint() const {
		//ûʵ��GetMaxPoint(),����������ûʵ��
		//Vertex3d _v = GetMaxPoint();
		//Vertex3d v;
		//v.x=_v.x;
		//v.y=_v.y;
		//v.z=_v.z;
		//return v;
		return Vertex3d();
	}

	virtual Vertex3d getMinPoint() const{
		//ûʵ��GetMaxPoint(),����������ûʵ��
		//Vertex3d _v  = GetMinPoint();
		//Vertex3d v;
		//v.x=_v.x;
		//v.y=_v.y;
		//v.z=_v.z;
		//return v;
		return Vertex3d();
	}
	virtual void scale(double dScale)
	{
		//ûʵ��ScaleDEM(),����������ûʵ��
		//ScaleDEM(dScale);
	}
	virtual Vertex3d getMaxPointInPolygon(std::vector<Vertex3d>& pPolygon) const{

		//ûʵ��ScaleDEM(),����������ûʵ��
		return Vertex3d();
	}

	virtual Vertex3d getMinPointInPolygon(std::vector<Vertex3d>&pPolygon) const{
		//ûʵ��ScaleDEM(),����������ûʵ��
		return Vertex3d();
	}
	virtual int clip(std::vector<Vertex3d>&pPolygon){
		//ûʵ��ScaleDEM(),����������ûʵ��
		return 0;
	}

	virtual void extract(int beg_nRow, int beg_nCol, int end_nRow, int end_nCol, DEMGridSharedPtr& extractGrid){
		if (!extractGrid.get())
			extractGrid = DEMGrid::create();
		extractGrid->setOrigin(m_dXOrigin + beg_nCol*m_dCellSize, m_dYOrigin + beg_nRow*m_dCellSize);
		extractGrid->setAlpha(m_dAlfa);
		extractGrid->setBelta(m_dBelta);
		extractGrid->setCellSize(m_dCellSize);
		extractGrid->setCols(end_nCol - beg_nCol + 1);
		extractGrid->setRows(end_nRow - beg_nRow + 1);
		extractGrid->setCoordinateSystem(m_bCoorFlag);
		extractGrid->setScale(m_lScale);
		extractGrid->setUnit(m_bUnitFlag);
		extractGrid->setInvalidValue(m_lInvalidData);
		extractGrid->setScale(m_lScale);
		//����ÿ���������ɫֵ
		if (m_pcVexColor != NULL)
		{
			//extractGrid->setVextexColors
		}
		//����ÿ���������������
		if (m_pvVexTexCoor != NULL)
		{
		}
		//���ø߳�ֵ
		if (m_pfDemHeightData != NULL)
		{
			if (extractGrid->getHeightData() == NULL)
			{
				float* pd = new float[(end_nRow - beg_nRow + 1)*(end_nCol - beg_nCol + 1)];
				extractGrid->setHeightData(pd, true);
			}
			for (int i = beg_nRow; i <= end_nRow; i++)
			{
				for (int j = beg_nCol; j <= end_nCol; j++)
				{
					//float* pd = new float[(end_nRow-beg_nRow+1)*(end_nCol-beg_nCol+1)]
					double _x, _y;
					getXY(i, j, _x, _y);
					extractGrid->interpolate(_x, _y);
				}
			}
		}
	}
	virtual void getDEMBound(std::vector<Vertex3d>&pPolygon){
		////ûʵ��ScaleDEM(),����������ûʵ��
	}
	virtual void transform(Matrix4x4& m4x4){
		//ûʵ��
	}

	//�����������ת����������������
	void converToGv(Vertex3d & _v, Vertex3d & v){
		v.x = _v.x;
		v.y = _v.y;
		v.z = _v.z;
	}
	void converFroGv(Vertex3d & _v, Vertex3d & v){
		_v.x = v.x;
		_v.y = v.y;
		_v.z = v.z;
	}

protected:

	/** @name ���ԣ���Ա������
	* @{
	*/

	/** ���½�Xԭ������ */
	double m_dXOrigin;

	/** ���½�Yԭ������ */
	double m_dYOrigin;

	/** �����С�߳�ֵ */
	float m_fMinZ, m_fMaxZ;

	/** alfa */
	double m_dAlfa;

	/** belta */
	double m_dBelta;

	/** DEM���� */
	int m_nCol;

	/** DEM���� */
	int m_nRow;

	/** ����ϵ��ʾ,1Ϊ�������,0Ϊ��γ������ϵ */
	int m_bCoorFlag;

	/** ���굥λ,1Ϊ��,0Ϊ���� */
	int m_bUnitFlag;

	/** ������� */
	double m_dCellSize;

	/** ��Ч�߳�ֵ */
	long m_lInvalidData;

	/** ������ */
	long m_lScale;

	/** ÿ��������߳����� */
	float *m_pfDemHeightData;

	/** DEM ÿ����������ɫֵ */
	unsigned long *m_pcVexColor;

	/** ������������ */
	Vertex2f *m_pvVexTexCoor;

	/** DEM �̡߳���ɫ�������������������Ҫ����ά�����ڴ� */
	bool m_bDemDataAlloced;
	bool m_bVexColorAlloced;
	bool m_bTexCoorAlloced;

	/** @} */ // ���Խ�β

public:

	/** @defgroup DEMGridImpl ��������Ա������
	* @{
	*/


	/** @name ��������������
	*  @{
	*/

	/** ���캯��
	*/
	DEMGridImpl() :m_dXOrigin(0), m_dYOrigin(0), m_fMinZ(0), m_fMaxZ(0), m_dAlfa(0), m_dBelta(0), m_nCol(0), m_nRow(0), m_dCellSize(0),
		m_bCoorFlag(1), m_bUnitFlag(1), m_lInvalidData(-99999), m_lScale(1), m_pfDemHeightData(NULL), m_pcVexColor(NULL), m_pvVexTexCoor(NULL),
		m_bDemDataAlloced(true), m_bVexColorAlloced(true), m_bTexCoorAlloced(true) {}


	virtual ~DEMGridImpl()
	{
		if (m_pfDemHeightData)
		{
			if (m_bDemDataAlloced)
				delete[]m_pfDemHeightData;
			m_pfDemHeightData = NULL;
		}
		if (m_pcVexColor)
		{
			if (m_bVexColorAlloced)
				delete[]m_pcVexColor;
			m_pcVexColor = NULL;
		}
		if (m_pvVexTexCoor)
		{
			if (m_bTexCoorAlloced)
				delete[]m_pvVexTexCoor;
			m_pvVexTexCoor = NULL;
		}
	}
	/** @} */ // ����������������β

	/** ����dem����
	* @param [in] src	Դdem����
	* @param [out] tar	dem����
	* @return 1 -- �����ɹ���0 -- ����ʧ��
	*/
	static long CopyDem(DEMGridImpl& src, DEMGridImpl& tar, bool bCopyData);


	/** @name ���ݲ���
	*  @{
	*/
	/** ��ȡdem����ά�����
	* @param
	* @return dem����ά�����
	*/
	Envelope3d GetDEMScope()
	{
		Vertex3d vMin, vMax;
		GetDEMOriginXY(vMin.x, vMin.y);
		vMax.x = vMin.x + (m_nCol - 1) * m_dCellSize;
		vMax.y = vMin.y + (m_nRow - 1) * m_dCellSize;
		float minz, maxz;
		GetDEMHeightRange(minz, maxz);
		vMin.z = minz;
		vMax.z = maxz;

		Envelope3d env;
		env.setMin(vMin);
		env.setMax(vMax);
		return env;
	}

	/** ����alfa
	* @param [in] dAlfa	����
	*/
	void SetDEMAlpha(double dAlfa)
	{
		m_dAlfa = dAlfa;
	}

	/** ����Belta
	* @param [in] dBelta	����
	*/
	void SetDEMBelta(double dBelta)
	{
		m_dBelta = dBelta;
	}
	/** ��ȡalfa
	* @return Alfa
	*/
	double GetDEMAlpha() const
	{
		return m_dAlfa;
	}
	/** ��ȡbelta
	* @return Belta
	*/
	double GetDEMBelta() const
	{
		return m_dBelta;
	}

	/** ����DEM���½�ԭ��x,y����
	* @param [in] dx x����
	* @param [in] dy y����
	*/
	void SetDEMOriginXY(double dx, double dy)
	{
		m_dXOrigin = dx;
		m_dYOrigin = dy;
	}

	/** ��ȡDEM���½�ԭ��x,y����
	* @param [out] dx DEM���½�ԭ��x����
	* @param [out] dy DEM���½�ԭ��y����
	*/
	void GetDEMOriginXY(double& dx, double& dy)
	{
		dx = m_dXOrigin;
		dy = m_dYOrigin;
	}
	/** ����DEM�̷߳�Χ
	* @param [in] fminz ��С�߳�ֵ
	* @param [in] fmaxz ���߳�ֵ
	*/
	void SetDEMHeightRange(float fminz, float fmaxz)
	{
		m_fMinZ = fminz;
		m_fMaxZ = fmaxz;
	}
	/** ��ȡDEM�̷߳�Χ
	* @param [out] fminz DEM��С�߳�ֵ
	* @param [out] fmaxz DEM���߳�ֵ
	*/
	void GetDEMHeightRange(float& fminz, float& fmaxz)
	{
		fminz = m_fMinZ;
		fmaxz = m_fMaxZ;
	}
	/** ����DEM����������
	* @param [in] nCol dem����
	* @param [in] nRow dem����
	*/
	void SetDEMRowCol(int nCol, int nRow)
	{
		m_nRow = nRow;
		m_nCol = nCol;
	}
	/** ��ȡDEM����������
	* @param [out] nCol dem����
	* @param [out] nRow dem����
	*/
	void GetDEMRowCol(int& nCol, int& nRow)
	{
		nCol = m_nCol;
		nRow = m_nRow;
	}
	/** ����DEM����ϵ����
	* @param [in] bflag,����ϵ����,��Ϊ���¼���:
	* -1Ϊ�������
	* -0Ϊ��γ������ϵ
	*/
	void SetDEMCoorFlag(int bFlag = 1)
	{
		m_bCoorFlag = bFlag;
	}
	/** ��ȡDEM����ϵ����
	* @return ����ϵ����,��Ϊ���¼���:
	* -1Ϊ�������
	* -0Ϊ��γ������ϵ
	*/
	int GetDEMCoorFlag()
	{
		return m_bCoorFlag;
	}
	/** ����DEM��λ����
	* @param [in] bflag,���굥λ����,��Ϊ���¼���:
	* -1Ϊ��
	* -0Ϊ����
	*/
	void SetDEMUnitFlag(int bFlag = 1)
	{
		m_bUnitFlag = bFlag;
	}
	/** ��ȡDEM���굥λ����
	* @return ����ϵ����,��Ϊ���¼���:
	* -1Ϊ��
	* -0Ϊ����
	*/
	int GetDEMUnitFlag()
	{
		return m_bUnitFlag;
	}
	/** ����DEM�������
	* @param [in] dSize,�������
	*/
	void SetDEMCellSize(double dSize)
	{
		m_dCellSize = dSize;
	}
	/** ��ȡDEM�������
	* @return double���͸������
	*/
	double GetDEMCellSize()
	{
		return m_dCellSize;
	}
	/** ����DEM�߳�ֵ���ű���
	* @param [in] lScale,�߳�ֵ���ű���
	*/
	void SetDEMScale(long lScale)
	{
		m_lScale = lScale;
	}
	/** ��ȡDEM�߳�ֵ���ű���
	* @return �߳�ֵ���ű���
	*/
	long GetDEMScale()
	{
		return m_lScale;
	}
	/** ����DEMÿ��������ĸ߳�����
	* @param [in] pData,ָ��float���͵�һ�����ݵ�ָ��
	* @param [in] bAlloced,�Ƿ���Ҫ����ά���ڴ�
	* -����ά���ڴ�
	* -����ά���ڴ�
	*/
	void SetDEMHeightData(float *pData, bool bAlloced = true)
	{
		if (m_pfDemHeightData)
		{
			if (m_bDemDataAlloced)
			{
				delete[]m_pfDemHeightData;
			}
			m_pfDemHeightData = NULL;
		}

		m_pfDemHeightData = pData;
		m_bDemDataAlloced = bAlloced;
	}
	/** ��ȡDEMÿ��������ĸ߳�����
	* @return ָ��demÿ��������߳����ݵ�ָ��
	*/
	float* GetDEMHeightData()
	{
		return m_pfDemHeightData;
	}
	/** ����DEMÿ�����������ɫֵ
	* @param [in] pData,ָ��unsigned long���͵�һ�����ݵ�ָ��
	* @param [in] bAlloced,�Ƿ���Ҫ����ά���ڴ�
	* -����ά���ڴ�
	* -����ά���ڴ�
	*/
	void SetDEMVexColor(unsigned long *pColor, bool bAlloced = true)
	{
		if (m_pcVexColor)
		{
			if (m_bVexColorAlloced)
			{
				delete[]m_pcVexColor;
			}
			m_pcVexColor = NULL;
		}
		m_pcVexColor = pColor;
		m_bVexColorAlloced = bAlloced;
	}
	/** ��ȡDEMÿ�����������ɫֵ
	* @return ָ��demÿ����������ɫ���ݵ�ָ��
	*/
	unsigned long* GetDEMVexColor()
	{
		return m_pcVexColor;
	}
	/** ����DEMÿ�������������������
	* @param [in] pData,ָ��Vertex2f���͵�һ�����ݵ�ָ��
	* @param [in] bAlloced,�Ƿ���Ҫ����ά���ڴ�
	* -����ά���ڴ�
	* -����ά���ڴ�
	*/
	void SetDEMVexTexCoor(Vertex2f* pVexTexCoor, bool bAlloced = true)
	{
		if (m_pvVexTexCoor)
		{
			if (m_bTexCoorAlloced)
			{
				delete[]m_pvVexTexCoor;
			}
			m_pvVexTexCoor = pVexTexCoor;
		}
		m_pvVexTexCoor = pVexTexCoor;
		m_bTexCoorAlloced = bAlloced;
	}
	/** ��ȡDEMÿ�������������������
	* @return ָ��demÿ���������������������ݵ�ָ��
	*/
	Vertex2f* GetDEMVexTexCoor()
	{
		return m_pvVexTexCoor;
	}
	/** ����DEM��Ч�߳�ֵ
	* @param [in] pData,ָ��long���͵�����,����Ϊ��Ч�߳�ֵ�Ĵ�С
	*/
	void SetDEMInvalidata(long lInvalidata)
	{
		m_lInvalidData = lInvalidata;
	}
	/** ��ȡDEM��Ч�߳�ֵ
	* @return DEM��Ч�߳�ֵ
	*/
	long GetDEMInvalidata()
	{
		return m_lInvalidData;
	}


	/** @} */ // ���ݲ������β



	/** @name �������
	*  @{
	*/

	//##ModelId=49EF1205035B
	/** �ڵ�ǰDEM���ڲ�������ĸ߳�
	* @param [in] vVer	���������
	*/
	int Interpolate(Vertex3d& vVer);

	//##ModelId=49EF141C0232
	/** �ڵ�ǰDEM���ڲ������㼯�����и߳�
	* ���ܣ��ڲ������ĸ߳�ֵ.
	* ������PointInΪ�����ĵ㣬vVerticesΪ�����ĵ㼯������x,yӦ���ɵ����߸�ֵ����z�����ڲ�ĸ߳�ֵ.
	* ע�⣺������������һ���Ǵ�����꣬���Եģ�������Եģ���DEMԭ��ģ�
	*      ��������ĵ�ƽ��λ�ò���DEM��Χ�ڣ����ڲ�ĸ߳�ֵ������ЧֵNODATA��
	* @param [in] vVertices	����㼯
	*/
	int Interpolate(std::vector<Vertex3d>& vVertices);

	//##ModelId=49EF1297030D
	/** �õ���ǰDEM�߳����ĵ�
	* @return ��ǰDEM�߳����ĵ�
	*/
	Vertex3d GetMaxPoint() const;

	//##ModelId=49EF12AC01E4
	/** �õ���ǰDEM�߳���С�ĵ�
	* @return ��ǰDEM�߳���С�ĵ�
	*/
	Vertex3d GetMinPoint() const;

	//##ModelId=49EF12FC03A9
	/** �Ե�ǰDEM�ϵ����е�ĸ߳̽�������
	* @param [in] dScale ���ű���
	*/
	void ScaleDEM(double dScale);

	

	//##ModelId=49EF13E7001F
	/** ��ȡһ����Χ��DEM
	* @param [in] nRow �к�
	* @param [in] nCol �к�
	*/
	void Extract(int nRow, int nCol);
 
	//##ModelId=49EF144F00EA
	/** ���ݴ������ת������ת��ǰDEM
	* @param [in] m4x4 ��֪����ת����
	* @return
	*/
	void TranslateForm(Matrix4x4& m4x4);

	//�õ�Grid�����е�xyֵ
	void getXY(int nRow, int nCol, double& x, double &y){
		x = m_dXOrigin + m_dCellSize*nCol;
		y = m_dYOrigin + m_dCellSize*nRow;
	}

	/** @} */ // ����������β

	/** @} */ // ������β

	/** @} */ // ģ���β


};

end_gdb_namespace
end_gtl_namespace



