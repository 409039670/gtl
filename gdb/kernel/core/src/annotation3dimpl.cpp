#include "internal_annotation3dimpl.h"
#include "internal_spatialfuns.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
Annotation3dImpl::Annotation3dImpl(void)
{
	m_byteTypeID = GEOTYPE_3D_ANNOTATION;
	m_strAnnoString = "EMPTY";//ע���ַ���
	m_strFontName = "SYSTEM";//ע������
}

Annotation3dImpl::~Annotation3dImpl(void)
{
}

Quaternion Annotation3dImpl::GetOrientation() const
{
	return m_qOrientation;
}
void Annotation3dImpl::SetOrientation(Quaternion orientation)
{
	m_qOrientation = orientation;
}

long Annotation3dImpl::GetAnnoType() const
{
	return m_lAnnoType;
}
void Annotation3dImpl::SetAnnoType(long lAnnoType)
{
	m_lAnnoType = lAnnoType;
}

std::string Annotation3dImpl::GetAnnoString() const
{
	return m_strAnnoString;
}
void Annotation3dImpl::SetAnnoString(std::string annoString)
{
	m_strAnnoString = annoString;
}

string Annotation3dImpl::GetFontName() const
{
	return m_strFontName;
}
void Annotation3dImpl::SetFontName(string fontName)
{
	m_strFontName = fontName;
}

unsigned char Annotation3dImpl::GetStyle() const
{
	return m_byteStyle;
}
void Annotation3dImpl::SetStyle(unsigned char bStyle)
{
	m_byteStyle = bStyle;
}

long Annotation3dImpl::GetHeight() const
{
	return m_lHeight;
}
void Annotation3dImpl::SetHeight(long lHeight)
{
	m_lHeight = lHeight;
}

long Annotation3dImpl::GetWidth() const
{
	return m_lWidth;
}
void Annotation3dImpl::SetWidth(long width)
{
	m_lWidth = width;
}


long Annotation3dImpl::GetSpaces() const
{
	return m_lSpaces;
}
void Annotation3dImpl::SetSpaces(long lSpace)
{
	m_lSpaces = lSpace;
}

long Annotation3dImpl::GetColor() const
{
	return m_lColor;
}
void Annotation3dImpl::SetColor(long color)
{
	m_lColor = color;
}

size_t Annotation3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	//������Ϣ(Geometry��3DGeometry)
	//*************************************************

	nSize += Point3dImpl::sizeBuffer();

	//ר����Ϣ
	//*************************************************
	nSize += sizeof(Vertex3d); // m_vPosition
	nSize += sizeof(Quaternion); // m_qOrientation
	nSize += sizeof(long); // m_lAnnoType
	nSize += sizeof(unsigned char); // m_byteStyle

	nSize += sizeof(int); //����int charNum ��¼�ַ������ַ���������д���ļ� 
	int charNum = m_strAnnoString.length();
	nSize += sizeof(char) * charNum; //m_strAnnoString

	nSize += sizeof(int); //����int charNum ��¼�ַ������ַ���������д���ļ� 
	charNum = m_strFontName.length();
	nSize += sizeof(char) * charNum; //m_strFontName

	nSize += sizeof(long); // m_lHeight
	nSize += sizeof(long); // m_lWidth
	nSize += sizeof(long); // m_lSpaces
	nSize += sizeof(long); // m_lColor

	//*************************************************

	return nSize;
}

bool Annotation3dImpl::readBuffer(Buffer & buf)
{

	int flag = 0;
	flag = Point3dImpl::readBuffer(buf);

	//������Ϣ*************************************
	Vertex3d vPosition;
	buf.read((char*)&vPosition, sizeof(Vertex3d));
	buf.read((char*)&m_qOrientation, sizeof(Quaternion));
	buf.read((char*)&m_lAnnoType, sizeof(long));
	buf.read((char*)&m_byteStyle, sizeof(unsigned char));
	m_vPoint.x = vPosition.x, m_vPoint.y = vPosition.y, m_vPoint.z = vPosition.z;

	int ll = 0;
	buf.read((char*)&ll, sizeof(int));
	if (ll > 0)
	{
		char tempbuf[50];
		buf.read(tempbuf, sizeof(char) * ll);
		tempbuf[ll] = '\0';
		m_strAnnoString = tempbuf;
	}

	ll = 0;
	buf.read((char*)&ll, sizeof(int));
	if (ll > 0)
	{
		char tempbuf[50];
		buf.read(tempbuf, sizeof(char) * ll);
		tempbuf[ll] = '\0';
		m_strFontName = tempbuf;
	}

	buf.read((char*)&m_lHeight, sizeof(long));
	buf.read((char*)&m_lWidth, sizeof(long));
	buf.read((char*)&m_lSpaces, sizeof(long));
	buf.read((char*)&m_lColor, sizeof(long));

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Annotation3dImpl::writeBuffer(Buffer& buf)
{

	int flag = 0;
	flag = Point3dImpl::writeBuffer(buf);

	//������Ϣ*************************************
	Vertex3d vPosition(m_vPoint);
	buf.write((char*)&vPosition, sizeof(Vertex3d));
	buf.write((char*)&m_qOrientation, sizeof(Quaternion));
	buf.write((char*)&m_lAnnoType, sizeof(long));
	buf.write((char*)&m_byteStyle, sizeof(unsigned char));

	int ll = m_strAnnoString.length();
	buf.write((char*)&ll, sizeof(int));
	if (ll > 0)
	{
		buf.write((char*)m_strAnnoString.c_str(), sizeof(char) * ll);
	}

	ll = m_strFontName.length();
	buf.write((char*)&ll, sizeof(int));
	if (ll > 0)
	{
		buf.write((char*)m_strFontName.c_str(), sizeof(char) * ll);
	}

	buf.write((char*)&m_lHeight, sizeof(long));
	buf.write((char*)&m_lWidth, sizeof(long));
	buf.write((char*)&m_lSpaces, sizeof(long));
	buf.write((char*)&m_lColor, sizeof(long));

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Annotation3dImpl::TranslateFrom(Matrix4x4& m4x4)
{
	Point3dImpl::TranslateFrom(m4x4);

	//m_vPosition *= m4x4;
	m4x4.transformAffine(m_vPoint);

	//���¼����������
	m_eEnvelope.Empty();

	Vertex3d vmin = m_eEnvelope.GetMin();
	Vertex3d vmax = m_eEnvelope.GetMax();

	Vertex3d v[8];
	v[0] = vmin;
	v[1].x = vmin.x; v[1].y = vmin.y; v[1].z = vmax.z;
	v[2].x = vmax.x; v[2].y = vmin.y; v[2].z = vmax.z;
	v[3].x = vmin.x; v[3].y = vmax.y; v[3].z = vmax.z;
	v[4].x = vmax.x; v[4].y = vmin.y; v[4].z = vmax.z;
	v[5].x = vmax.x; v[5].y = vmax.y; v[5].z = vmax.z;
	v[6].x = vmin.x; v[6].y = vmax.y; v[6].z = vmax.z;
	v[7] = vmax;
	for (int i = 0; i < 8; i++)
	{
		//v[i] *= m4x4;
		m4x4.transformAffine(v[i]);
		m_eEnvelope.Union(v[i]);
	}
}

GeometryImpl* Annotation3dImpl::CreateObject()
{
	return new Annotation3dImpl;
}

//�����ⲿָ������ά�߶ε���ǰ���ζ�������ƽ��(ȡ�������)
double Annotation3dImpl::D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt)
{
	double mindis = m_eEnvelope.GetClosestLineDisToAABBBox(begvPt, endvPt);
	return mindis * mindis;
}

//�����ⲿָ���������ε���ǰ���ζ�������ƽ��
double Annotation3dImpl::D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3)
{
	double mindis = m_eEnvelope.GetClosestTriangleDisToAABBBox(triPt1, triPt2, triPt3);
	return mindis*mindis;
}

//���㵱ǰ����ļ�������
Vertex3d Annotation3dImpl::CalCorePoint()
{
	Vertex3d corePt;
	corePt = m_eEnvelope.CenterPoint();
	return corePt;
}

/** �麯��
* ͳ�Ƶ�ǰ��������ļ�������
* @return long*:�����ڲ����ٿռ䡢�ⲿ�ͷ�
*               ����ֵ������ -- ���������������Ч�������������������Ч��ɫ����
*								�����������������ת��Ϊ�ȼ�����
*								����������������
*/
long* Annotation3dImpl::GeoStatistics()
{
	long* statis = new long[8];

	statis[0] = 1;
	for (int i = 1; i < 8; i++)
		statis[i] = 0;

	return statis;
}

// �ͷŶ������ݲ���ָ��ͱ�������ʼֵ
bool Annotation3dImpl::Init()
{
	bool res = Point3dImpl::Init();
	if (false == res) return false;

	m_strAnnoString = "EMPTY";//ע���ַ���
	m_strFontName = "SYSTEM";//ע������

	m_vPoint.zero();
	m_qOrientation = Quaternion(1, 0, 0, 0);

	return true;
}

int Annotation3dImpl::RGeoWithPolygon2D(vector<Vertex3d>& ptArray, int precisionFlag)
{
	if (precisionFlag >= 0 && precisionFlag < 3)
	{
		char f = RPointWithPolygon2d(m_vPoint, ptArray);
		if (f == 'i' || f == 'E' || f == 'V')
			return 1;
		else
			return 0;
	}
	else
		return -1;
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
