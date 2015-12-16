#pragma once
#include "geometry.h" 
#include "internal_OBBoxImpl.h"
#include "internal_AABBoxImpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

enum
{
	GEOTYPE_3D_GEOMETRY = GEOMTYPE_3D_GEOMETRY,
	GEOTYPE_3D_POINT,
	GEOTYPE_3D_VOLUME,
	GEOTYPE_3D_SURFACE,
	GEOTYPE_3D_CURVE,
	GEOTYPE_3D_GROUP,
	GEOTYPE_3D_VOXEL,
	GEOTYPE_3D_SOLID,
	GEOTYPE_3D_MULTISOLID,
	GEOTYPE_3D_PRISM,
	GEOTYPE_3D_TETRAHEDRON,
	GEOTYPE_3D_PARAMETERIZEDSURFACE,
	GEOTYPE_3D_TRIANGLEMESH,
	GEOTYPE_3D_TRIANGLE,
	GEOTYPE_3D_MULTIPOLYGON,
	GEOTYPE_3D_POLYGON,
	GEOTYPE_3D_LINEARRING,
	GEOTYPE_3D_PARAMETERIZEDCURVE,
	GEOTYPE_3D_LINESTRING,
	GEOTYPE_3D_MULTIPOLYLINE,
	GEOTYPE_3D_REFERENCE,
	GEOTYPE_3D_ANNOTATION,
	GEOTYPE_3D_PATHLINE,
	GEOTYPE_3D_PATHPOINT,
	GEOTYPE_3D_DRAWCAMERA,
	GEOTYPE_3D_CUBE,
	GEOTYPE_3D_SINGLEPOINT,
	GEOTYPE_3D_MULTIPOINT,
	GEOTYPE_3D_AUXBASEGROUP,
	GEOTYPE_3D_FEATUREINFOGROUP,

	GEOTYPE_3D_CUBIOD, //������
	GEOTYPE_3D_IRREGULARVOL,//��������
	GEOTYPE_3D_CYLINDER,//Բ��
	GEOTYPE_3D_SPHERE
};

class GeometryImpl : virtual public Geometry
{
	/** @defgroup GeometryImpl GeometryImpl-�����ࣨ��������ࣩ
	*  @{
	*/

protected:
	/** @name ���ԣ���Ա������
	* @{
	*/
	/** ���ζ�������ͣ�ÿ�����Ͷ�Ӧһ����� */
	int m_byteTypeID;

	//##ModelId=49E531E4037A
	/** ���νṹ��ά�� */
	unsigned char m_byteDim;

	/** �������������õĴ��� */
	int	m_nRefCount;

	/** ���ζ����־��Ϣ */
	int m_nObjFlag;

	/** ���ζ������ģʽ���˱�־Ϊ�����ڼ�ʹ�ã������༭�ͱ��� */
	int m_nDrawMode;

	/** ���ζ��������*/
	string m_sObjName;

	/** @} */ // ���Խ�β


	/** @defgroup GeometryImpl ��������Ա������
	* @{
	*/


	/** @name ��������������
	*  @{
	*/

protected:

public:
	GeometryImpl();
	virtual ~GeometryImpl();

public:
	/** ��̬�������ڴ���ָ�����͵ļ��ζ���
	*  ���ݴ���Ķ������ͣ��½�һ����Ӧ�������ָ�룬�����ٿռ�
	* @param [in] signal
	* -0   GEOTYPE_3D_POINT
	* -1   GEOTYPE_3D_VOLUME
	* -2   GEOTYPE_3D_SURFACE
	* -3   GEOTYPE_3D_CURVE
	* -4   GEOTYPE_3D_GROUP
	* -5   GEOTYPE_3D_VOXEL
	* -6   GEOTYPE_3D_SOLID
	* -7   GEOTYPE_3D_MULTISOLID
	* -8   GEOTYPE_3D_PRISM
	* -9   GEOTYPE_3D_TETRAHEDRON
	* -10  GEOTYPE_3D_PARAMETERIZEDSURFACE
	* -11  GEOTYPE_3D_TRIANGLEMESH
	* -12  GEOTYPE_3D_TRIANGLE
	* -13  GEOTYPE_3D_MULTIPOLYGON
	* -14  GEOTYPE_3D_POLYGON
	* -15  GEOTYPE_3D_LINEARRING
	* -16  GEOTYPE_3D_PARAMETERIZEDCURVE
	* -17  GEOTYPE_3D_LINESTRING
	* -18  GEOTYPE_3D_MULTIPOLYLINE
	* -20  GEOTYPE_3D_REFERENCE
	* -21  GEOTYPE_3D_ANNOTATION
	*/
	static GeometryImpl * create(int signal);

	/** �½����ζ��󣨸���Buffer�е�typeʵ�ֶ���ָ��Ĵ������ڴ�Ŀ��٣�
	*��Buffer�ж�ȡ���ݣ��½�һ��������,����readBufferʵ�ָ����ڴ������������Ŀ���
	* @return �����ָ��
	*/
	static GeometryImpl * create(Buffer & buf);


	/** @} */ // ����������������β

public:

	/** @name ���ݲ���
	*  @{
	*/

	/** ��ö���ļ������ͱ��
	* @param
	* @return ����ļ������ͱ��
	*/
	virtual int getType() const //���ؼ�������
	{
		return m_byteTypeID;
	}
	/** ָ������ļ������ͱ��
	* @param [in] type,����ļ������ͱ��
	*/
	void setType(const int type)
	{
		m_byteTypeID = type;
	}

	/** ָ�����ζ���ı�־��Ϣ
	* @param [in] bVisible,���ζ���ı�־��Ϣ
	*/
	void SetVisibleFlag(int bVisible)
	{
		if (bVisible)
			m_nObjFlag |= 0x01;
		else
			m_nObjFlag &= ~0x01;
	}
	/** ��ü��ζ���ı�־��Ϣ
	* @param
	* @return ����ı�־��Ϣ
	*/
	int GetVisibleFlag()
	{
		if (m_nObjFlag & 0x01)
			return 1;
		else
			return 0;
	}


	/** ��ȡ���νṹ������ά��
	* @return ��������ά��
	*/
	unsigned char GetDimension();

	/** ����һ�����ü���
	* @return
	*/
	void AddRef();

	/** �ͷŶ��󣨼���һ�����ü�����
	* @return
	*/
	void Release();

	/** ��ȡ���ζ������ģʽ���˱�־Ϊ�����ڼ�ʹ�ã������༭�ͱ���
	* @return ���ζ������ģʽ
	*/
	virtual int GetDrawMode();

	/** ���ü��ζ������ģʽ���˱�־Ϊ�����ڼ�ʹ�ã������༭�ͱ���
	* @return
	*/
	virtual void SetDrawMode(int nDrawMode);

	/** ��ȡ���ζ����־
	* @return ���ζ����־
	*/
	int GetObjFlag() const;

	/** ���ü��ζ����־
	* @return
	*/
	void SetObjFlag(int nObjFlag);

	/** ��ü��ζ�������
	* @return
	*/
	void SetObjName(string strName)
	{
		m_sObjName = strName;
	}

	/** ���ü��ζ�������
	* @return
	*/
	string GetObjName()
	{
		return m_sObjName;
	}

	/** @} */ // ���ݲ������β



	/** @name �������
	*  @{
	*/

	/** ��ָ�����󿽱�����ǰ����
	* @return ��������ָ��
	*/
	virtual bool copy(GeometryImpl * g);

	/** ����һ����ǰ����ĸ���
	* @return ��������ָ��
	*/
	virtual GeometryImpl* Clone();

	/** ��Դ����������Ը��Ƶ�������
	* @param [in] pSrcGeometry	Դ����
	* @return
	*/
	virtual void copyAttr(GeometryImpl* pSrcGeometry);

	/** ʹ��new �����������������ʾ��������
	* @param
	* @return ������Ĳ���m_nSize
	*/
	virtual GeometryImpl* CreateObject() = 0;

	/** ���ݴ���ı任������ɶ���ļ��α任
	* @param
	* @return ������Ĳ���m_nSize
	*/
	virtual void TranslateFrom(Matrix4x4& m4x4);

	/** ���㵱ǰ���������ڴ�Ĵ�С
	* @param
	* @return ������Ĳ���m_nSize
	*/
	virtual size_t sizeBuffer() = 0;

	/** �ӻ����ȡ�����¼
	* @param [in] pBuffer ����
	* @return ��ȡ�ɹ����
	*/
	virtual bool readBuffer(Buffer & buf) = 0;

	/** ������д�뻺��
	* @param [in] pBuffer ����
	* @return д��ɹ����
	*/
	virtual bool writeBuffer(Buffer& buf) = 0;

	/** ʵ�������ڴ�д���ļ�
	* @param [in] f	�����������
	*/
	virtual void write(std::ostream & f);

	/** �ӻ������н�����ǰ���󣬲�����Ա������ֵ
	* @param [in] f	������������
	*/
	virtual void read(std::istream & f);

	/** ��ȡ������İ�Χ��
	* @return AABBoxImpl������,������İ�Χ��
	*/
	virtual AABBoxImpl & getEnvelope() = 0;
	/** ���ü�����İ�Χ��
	* @param [in] e3d	AABBoxImpl������,������İ�Χ��
	*/
	virtual void setEnvelope(const AABBoxImpl & e3d) = 0;


	

	/** �õ�����ļ�������
	* @param [out] ��Vertex3d����ʽ����
	*/
	virtual Vertex3d CalCorePoint() = 0;

	/** �ͷŶ������ݲ���ָ��ͱ�������ʼֵ
	* @return �Ƿ�ִ�гɹ�
	*/
	virtual bool Init(){ return true; }

	virtual long * GeoStatistics(){
		return 0;
	}
public:
	// ��ȡ���ζ���ı߽���Σ�Ϊ��ά�߽磬����Ƕ�ά������Z�ķ�ΧΪ0
	// * @param  [in] ��
	// * @return ���ر߽���ε�����
	virtual void getEnvelope(Envelope3d & d)  {
		AABBoxImpl & env3d = getEnvelope();
		Vertex3d vMin = env3d.GetMin();
		Vertex3d vMax = env3d.GetMax();

		d.minx = vMin.x; d.maxx = vMax.x;
		d.miny = vMin.y; d.maxy = vMax.y;
		d.minz = vMin.z; d.maxz = vMax.z;
	}
	// ��ȡ���ζ����״̬��ÿ��BIT��ʾһ��״̬����
	// * @param  [in] ��
	// * @return ���ζ����״̬
	virtual int getStatus() const 	{
		return 0;
	}
	// ���ü��ζ����״̬��ÿ��BIT��ʾһ��״̬����
	// * @param  [in] �µ�ֵ
	// * @return ��
	//##ModelId=4D21B8750009
	virtual void setStatus(int s) {

	}
	// ��ȡ���ζ��������
	// * @param  [in] ��
	// * @return  ���ؼ��ζ�������
	//##ModelId=4D21B875000C
	virtual std::string getName()const  {
		return m_sObjName;
	}
	// ���ü��ζ��������
	// * @param  [in] �µļ��ζ�������
	// * @return ��
	//##ModelId=4D21B875000E
	virtual void setName(std::string s) {
		m_sObjName = s;
	}
	// ��g��ָ��ļ��ζ����Ƶ������ζ���
	// * @param  [in] g const Geometry *, ָ��������ļ��϶����ָ�� 
	// * @return �Ƿ񿽱��ɹ�
	virtual bool copy(const Geometry * g){
		return GeometryImpl::copy(dynamic_cast<const GeometryImpl*>(g));
	}
	// ��¡����
	// * @param  [in]  
	// * @return �Ƿ񿽱��ɹ�
	virtual Geometry * clone(){
		return this->Clone();
	}
	// ���ü��ζ���İ�Χ�߽���Σ���Ҫ��������ʹ��
	// * @param  [in] e3d const Envelope3d & �߽����
	// * @return ��
	virtual void setEnvelope(const Envelope3d & e)  {
		AABBoxImpl   env3d(e.minx, e.miny, e.minz, e.maxx, e.maxy, e.maxz);
		this->setEnvelope(env3d);
	}
	virtual void translateFrom(Matrix4x4& m4x4){
		Matrix4x4 m(m4x4.getData());
		TranslateFrom(m);
	}
	virtual void setVisible(bool b) {
		SetVisibleFlag(b ? 1 : 0);
	}
	virtual bool isVisible() {
		return GetVisibleFlag() == 0 ? false : true;
	}
	virtual long * statistics(){
		return GeoStatistics();
	}
	/** �麯��
	* �ж������Ƿ񴩹���ǰ�����壬��������㵽�뼸���彻�����С����
	* @param [in] ray	��������
	* @param [out] dLength	������㵽�뼸���彻�����С���룬�����ཻ���ֵȡ�����
	* @return	1 -- �ཻ��0 -- ���ཻ
	*/
	virtual int rayCross(const RaySharedPtr & ray, double& length) const{
		return 0;
	}
	/** @} */

	/** �жϽ�ͷ׵���뵱ǰ�������Ƿ��ཻ
	* @param [in] frustum	����׵��
	* @return	1 -- �ཻ��0 -- ���ཻ
	*/
	virtual int frustumIntersect(const FrustumSharedPtr & frustum) const
	{
		return 0;
	}

	/** @} */ // ����������β

	/** @} */ // ������β

	/** @} */ // ģ���β
};


end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace

