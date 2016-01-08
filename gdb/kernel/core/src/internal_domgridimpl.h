#pragma once
#include "terrain.h"


begin_gtl_namespace
begin_gdb_namespace

class  DOMGridImpl:public DOMGrid
{
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

	virtual bool copy(const DOMGridSharedPtr & src, bool bCopyData)
	{
		return DOMGridImpl::CopyDom(*(DOMGridImpl*)(src.get()), *this, bCopyData) == 0 ? false : true;
	}
	virtual void setParameters(double resolvx, double resolvy, double east, double west, double north, double south,
		int ImageW, int ImageH, int ImageD, unsigned char * pData,
		unsigned long long datalength, bool bAlloced = true);
	virtual void setParameters(double east, double west, double north, double south, const ImageSharedPtr & p);
	virtual void getParameters(double& resolvx, double& resolvy, double& east, double& west, double& north, double& south,
		int& ImageW, int& ImageH, int& ImageD, unsigned char *& pData,
		unsigned long long & datalength, bool bAlloced = true);

	virtual void getResolution(double& resolvx, double& resolvy);

	virtual void getDirection(double& east, double& west, double& north, double& south);

	virtual void getImageSize(int& ImageW, int& ImageH, int& ImageD);

	virtual unsigned char * getImageData();
	virtual unsigned long long  getImageLength();

	virtual unsigned int getBitsPerPixel() const;
	virtual DOMBlockSharedPtr clip(const Envelope2d & rect);
	virtual unsigned char * getPixel(int ix, int iy);
	virtual unsigned char * getPixel(double x, double y);

	virtual void IP2WP(int x, int y, double * wx, double * wy);
	virtual void WP2IP(double   wx, double   wy, int *ix, int *iy);
	//rect.lengthX() �����xPixelNumber��yPixelMumber
	virtual unsigned long long calculateXPixelNmuber(double length);
	virtual unsigned long long  calculateYPixelNmuber(double length);

	virtual DOMBlockSharedPtr clip(int begX, int xNumbers, int begY, int yNumbers);

	virtual void clip(DOMLayerInfoSharedPtr lay, std::vector<DOMBlockSharedPtr> & blocks);
	virtual unsigned char* getLayerPixel(DOMLayerInfoSharedPtr lay, int row, int col);

protected:

	/** @name ���ԣ���Ա������
	* @{
	*/

	/** x����ֱ��� */
	double m_resolvx;

	/** y����ֱ��� */
	double m_resolvy;

	/** �� */
	double m_east;

	/** �� */
	double m_west;

	/** �� */
	double m_north;

	/** �� */
	double m_south;

	/** Ӱ���� */
	int m_ImageW;

	/** Ӱ��߶� */
	int m_ImageH;

	/** Ӱ����� */
	int m_ImageD;

	/** Ӱ������ */
	unsigned char* m_pData;

	/** Ӱ�����ݳ��� */
	unsigned long long m_pDataLength;

	///**Ӱ��ѹ������ */
	//int m_compressType;

	/** �ڴ�ά����� */
	bool m_bDomDataAlloced;

	/** @} */ // ���Խ�β

public:

	/** @defgroup DOMGridImpl ��������Ա������
	* @{
	*/


	/** @name ��������������
	*  @{
	*/

	/** ���캯��
	*/
	DOMGridImpl() :m_resolvx(1.0), m_resolvy(1.0), m_east(1.0), m_west(1.0), m_north(1.0), m_south(1.0),
		m_ImageW(1), m_ImageH(1), m_ImageD(24), m_pData(NULL), m_pDataLength(0), m_bDomDataAlloced(true) {}


	/** ��������
	*/
	virtual ~DOMGridImpl()
	{
		if (m_pData)
		{
			delete[]m_pData;
			m_pData = NULL;
		}
	}

	/** @} */ // ����������������β

	/** ����dom����
	* @param [in] src	Դdom����
	* @param [out] tar	dom����
	* @return 1 -- �����ɹ���0 -- ����ʧ��
	*/
	static long CopyDom(DOMGridImpl& src, DOMGridImpl& tar, bool bCopyData);


	/** @name ���ݲ���
	*  @{
	*/

	/** ����Dom
	* @param [in] resolvx x����ֱ���
	* @param [in] resolvy y����ֱ���
	* @param [in] east ��
	* @param [in] west ��
	* @param [in] north ��
	* @param [in] south ��
	* @param [in] ImageW  Ӱ����
	* @param [in] ImageH  Ӱ��߶�
	* @param [in] pData  Ӱ������
	* @param [in] bAlloced,�Ƿ���Ҫ����ά���ڴ�
	* -����ά���ڴ�
	* -����ά���ڴ�
	*/
	void SetDom(double resolvx, double resolvy, double east, double west, double north, double south, int ImageW, int ImageH, int ImageD, unsigned char* pData, unsigned long long datalength, bool bAlloced = true)
	{
		m_resolvx = resolvx;
		m_resolvy = resolvy;

		m_east = east;
		m_west = west;
		m_north = north;
		m_south = south;

		m_ImageW = ImageW;
		m_ImageH = ImageH;
		m_ImageD = ImageD;

		if (m_pData)
		{
			if (m_bDomDataAlloced)
			{
				delete[]m_pData;
			}
			m_pData = NULL;
		}

		m_pData = pData;
		m_bDomDataAlloced = bAlloced;

		m_pDataLength = datalength;
	}

	/** ��ȡDom����
	* @param [out] resolvx x����ֱ���
	* @param [out] resolvy y����ֱ���
	* @param [out] east ��
	* @param [out] west ��
	* @param [out] north ��
	* @param [out] south ��
	* @param [out] ImageW  Ӱ����
	* @param [out] ImageH  Ӱ��߶�
	* @param [out] pData  Ӱ������
	* @param [in] bAlloced,�Ƿ����¿����ڴ�
	*/
	void GetDomPara(double& resolvx, double& resolvy, double& east, double& west, double& north, double& south, int& ImageW, int& ImageH, int& ImageD, unsigned char*& pData, unsigned long long& datalength, bool bAlloced = true)
	{
		resolvx = m_resolvx;
		resolvy = m_resolvy;

		east = m_east;
		west = m_west;
		north = m_north;
		south = m_south;

		ImageW = m_ImageW;
		ImageH = m_ImageH;
		ImageD = m_ImageD;

		if (bAlloced)
		{
			if (pData)
				delete[]pData;
			pData = NULL;

			int SizeData = m_ImageW * m_ImageH * m_ImageD / 8;
			pData = new unsigned char[SizeData];
			for (int i = 0; i < SizeData; i++)
				pData[i] = m_pData[i];
		}
		else
			pData = m_pData;

		datalength = m_pDataLength;
	}


	/** ��ȡDomӰ��ֱ���
	* @param [out] resolvx DomX����ֱ���
	* @param [out] resolvy DomY����ֱ���
	*/
	void GetDomResolv(double& resolvx, double& resolvy)
	{
		resolvx = m_resolvx;
		resolvy = m_resolvy;
	}

	/** ��ȡDomӰ����
	* @param [out] east ��
	* @param [out] west ��
	* @param [out] north ��
	* @param [out] south ��
	*/
	void GetDomDir(double& east, double& west, double& north, double& south)
	{
		east = m_east;
		west = m_west;
		north = m_north;
		south = m_south;
	}

	/** ��ȡDomӰ���С
	* @param [out] ImageW Ӱ����
	* @param [out] ImageH Ӱ��߶�
	* @param [out] ImageD Ӱ�����
	*/
	void GetDomImageSize(int& ImageW, int& ImageH, int& ImageD)
	{
		ImageW = m_ImageW;
		ImageH = m_ImageH;
		ImageD = m_ImageD;
	}

	/** ��ȡDomӰ������
	* @return ָ��DomӰ�����ݵ�ָ��
	*/
	unsigned char* GetDomData()
	{
		return m_pData;
	}

	unsigned long long GetImageLength()
	{
		return m_pDataLength;
	}

	/** @} */ // ���ݲ������β



	/** @name �������
	*  @{
	*/

	/** @} */ // ����������β

	/** @} */ // ������β

	/** @} */ // ģ���β


};
end_gdb_namespace
end_gtl_namespace



