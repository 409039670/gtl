#include "internal_rectangle3dimpl.h"
#include "internal_polygon3dimpl.h"


begin_gtl_namespace
begin_gdb_namespace
//##ModelId=49E34729000F
// Rectangle3dImpl::Length()
// {
// }
// 
// //##ModelId=49E347290010
// Rectangle3dImpl::Width()
// {
// }
// 
// //##ModelId=49E347290011
// Rectangle3dImpl::Area()
// {
// }

bool Rectangle3dImpl::readBuffer(Buffer & buf)
{
	return false;
}


bool Rectangle3dImpl::writeBuffer(Buffer& buf)
{
	return false;
}

size_t Rectangle3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	//������Ϣ(Geometry��3DGeometry)
	//*************************************************

	nSize += Surface3dImpl::sizeBuffer();

	//ר����Ϣ
	//*************************************************

	nSize += sizeof(Vertex3d) * 4;// m_vVertices[4];


	return nSize;
}

void Rectangle3dImpl::TranslateFrom(Matrix4x4& m4x4)
{
	Surface3dImpl::TranslateFrom(m4x4);

	for (int i = 0; i < 4; i++)
	{
		//m_vVertices[i] *= m4x4;
		m4x4.transformAffine(m_vVertices[i]);
	}
}
GeometryImpl* Rectangle3dImpl::CreateObject()
{
	return new Rectangle3dImpl;
}
/** �麯��������������ʵ��
* �����ⲿָ������ά�߶ε���ǰ���ζ�������ƽ��
* @param [in] vPt	Vertex3d������ά����������
* @return �㵽��ǰ���ζ�������ƽ��
*/
double Rectangle3dImpl::D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt)
{
	return 0;
}
//���㵱ǰ����ļ�������
Vertex3d Rectangle3dImpl::CalCorePoint()
{
	Vertex3d corePt;
	for (long i = 0; i < 4; i++)
		corePt += m_vVertices[i];
	corePt /= 4;
	return corePt;
}

/** �麯��
* ͳ�Ƶ�ǰ��������ļ�������
* @return long*:�����ڲ����ٿռ䡢�ⲿ�ͷ�
*               ����ֵ������ -- ���������������Ч�������������������Ч��ɫ����
*								�����������������ת��Ϊ�ȼ�����
*								����������������
*/
long* Rectangle3dImpl::GeoStatistics()
{
	long* statis = new long[8];


	for (int i = 0; i < 8; i++)
		statis[i] = 0;

	// ydcl_xx

	return statis;
}

// �ͷŶ������ݲ���ָ��ͱ�������ʼֵ
bool Rectangle3dImpl::Init()
{
	/*bool res = Polygon3dImpl::Init();
	if(false == res) return false;

	for( int i = 0; i < 4; i++ )
	m_vVertices[i] = Vertex3d(0,0,0);*/

	return true;
}

end_gdb_namespace
end_gtl_namespace

