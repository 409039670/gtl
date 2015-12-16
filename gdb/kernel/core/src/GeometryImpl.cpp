#include "internal_GeometryImpl.h"
#include "internal_Geometry3dImpl.h"
#include "internal_point3dimpl.h"
#include "internal_SinglePoint3dImpl.h"
#include "internal_MultiPoint3dImpl.h"
#include "internal_Annotation3dImpl.h"
#include "internal_Reference3dImpl.h"

#include "internal_Curve3dImpl.h"
#include "internal_LineString3dImpl.h"
#include "internal_LinearRing3dImpl.h"
#include "internal_MultiPolyline3dImpl.h"
#include "internal_ParameterizedCurve3dImpl.h"

#include "internal_Surface3dImpl.h"
#include "internal_TriangleMesh3dImpl.h"
#include "internal_Polygon3dImpl.h"
#include "internal_MultiPolygon3dImpl.h"
#include "internal_ParameterizedSurface3dImpl.h"
#include "internal_Triangle3dImpl.h"
#include "internal_Rectangle3dImpl.h"

#include "internal_Volume3dImpl.h"
#include "internal_Solid3dImpl.h"
#include "internal_MultiSolid3dImpl.h"
#include "internal_Voxel3dImpl.h"
#include "internal_Cube3dImpl.h"
#include "internal_Prism3dImpl.h"
#include "internal_Tetrahedron3dImpl.h"

#include "internal_Group3dImpl.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
GeometryImpl::GeometryImpl()
{
	m_byteDim = 0;
	m_nRefCount = 1;
	m_nDrawMode = 0;
	m_nObjFlag = 1;
	m_sObjName = "GEOMETRY";
}
GeometryImpl::~GeometryImpl()
{

}

unsigned char GeometryImpl::GetDimension()
{
	return m_byteDim;
}

/** ����һ�����ü���
* @return
*/
void GeometryImpl::AddRef()
{
	m_nRefCount++;
}

/** �ͷŶ��󣨼���һ�����ü�����
* @return
*/
void GeometryImpl::Release()
{
	m_nRefCount--;
	if (m_nRefCount == 0)
		delete this;
}

/** ��ȡ���ζ������ģʽ���˱�־Ϊ�����ڼ�ʹ�ã������༭�ͱ���
* @return ���ζ������ģʽ
*/
int GeometryImpl::GetDrawMode()
{
	return m_nDrawMode;
}

/** ���ü��ζ������ģʽ���˱�־Ϊ�����ڼ�ʹ�ã������༭�ͱ���
* @return
*/
void GeometryImpl::SetDrawMode(int nDrawMode)
{
	m_nDrawMode = nDrawMode;
}

/** ��ȡ���ζ����־
* @return ���ζ����־
*/
int GeometryImpl::GetObjFlag() const
{
	return m_nObjFlag;
}

/** ���ü��ζ����־
* @return
*/
void GeometryImpl::SetObjFlag(int nObjFlag)
{
	m_nObjFlag = nObjFlag;
}

bool GeometryImpl::copy(GeometryImpl * geo)
{
	Init();

	bool flag = false;

	size_t nSize = geo->sizeBuffer();
	char* pMem = new char[nSize];
	//buff �ӹ�pMem���ڴ�ռ䣬copy()������������buff���������ͷ�
	Buffer buff;
	buff.attach(pMem, nSize);

	//��ǰ����this�������������pCopy������Ӷ�ʵ�ֶ���Ŀ���
	flag = geo->writeBuffer(buff);
	if (!flag) return false;
	buff.setPos(0);
	return readBuffer(buff);
}



//���ƹ����Ĳ���ָ��͹�����ģ��ָ�������
void subCloneAttr(GeometryImpl* pSrcGeometry, GeometryImpl* pTarGeometry)
{
	unsigned char signal = pSrcGeometry->getType();
	switch (signal)
	{
	case GEOTYPE_3D_GROUP:
	{
		Group3dImpl* pSrcGroup = dynamic_cast<Group3dImpl*>(pSrcGeometry);
		Group3dImpl* pTarGroup = dynamic_cast<Group3dImpl*>(pTarGeometry);

		vector<Geometry3dImpl*> srcChilds, tarChilds;
		pSrcGroup->GetCollection(srcChilds);
		pTarGroup->GetCollection(tarChilds);

		for (int i = 0; i < srcChilds.size(); i++)
		{
			subCloneAttr(srcChilds[i], tarChilds[i]);
		}
		break;
	}
	case GEOTYPE_3D_SOLID:
	{
		Solid3dImpl* pSrcSolid = dynamic_cast<Solid3dImpl*>(pSrcGeometry);
		Solid3dImpl* pTarSolid = dynamic_cast<Solid3dImpl*>(pTarGeometry);

		vector<Surface3dImpl*> srcChilds, tarChilds;
		pSrcSolid->GetBoundarySurfaces(srcChilds);
		pTarSolid->GetBoundarySurfaces(tarChilds);

		for (int i = 0; i < srcChilds.size(); i++)
		{
			subCloneAttr(srcChilds[i], tarChilds[i]);
		}
		break;
	}
	case GEOTYPE_3D_TRIANGLEMESH:
	case GEOTYPE_3D_POLYGON:
	case GEOTYPE_3D_TRIANGLE:
	case GEOTYPE_3D_MULTIPOLYGON:
	{
		Surface3dImpl* pSrcSurface = dynamic_cast<Surface3dImpl*>(pSrcGeometry);
		Surface3dImpl* pTarSurface = dynamic_cast<Surface3dImpl*>(pTarGeometry);
		pTarSurface->SetMaterial(pSrcSurface->GetMaterial());
		break;
	}
	case GEOTYPE_3D_REFERENCE:
	{
		Reference3dImpl* pSrc = dynamic_cast<Reference3dImpl*>(pSrcGeometry);
		Reference3dImpl* pTar = dynamic_cast<Reference3dImpl*>(pTarGeometry);
		pTar->SetLinkModel(pSrc->GetLinkModel());
		break;
	}
	}
}

/** ��Դ����������Ը��Ƶ�������
* @param [in] pSrcGeometry	Դ����
* @return
*/
void GeometryImpl::copyAttr(GeometryImpl* pSrcGeometry)
{
	subCloneAttr(pSrcGeometry, this);
}

GeometryImpl* GeometryImpl::Clone()
{
	//pCopy ���ٵ��ڴ�ռ��ɵ������ĺ��������ͷ�
	GeometryImpl* pCopy = CreateObject();

	size_t nSize = this->sizeBuffer();
	char* pMem = new char[nSize];
	//buff �ӹ�pMem���ڴ�ռ䣬Clone()������������buff���������ͷ�
	Buffer buff;
	buff.attach(pMem, nSize);

	//��ǰ����this�������������pCopy������Ӷ�ʵ�ֶ���Ŀ���
	this->writeBuffer(buff);
	buff.setPos(0);
	pCopy->readBuffer(buff);

	pCopy->copyAttr(this);

	return pCopy;
}

void GeometryImpl::TranslateFrom(Matrix4x4& m4x4)
{

}

size_t GeometryImpl::sizeBuffer()
{
	size_t nSize = 0;
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.

	nSize += sizeof(int); //m_byteTypeID (GeometryImpl)

	nSize += sizeof(unsigned char); //m_byteODim  (GeometryImpl)
	nSize += sizeof(int);  //m_nRefCount (GeometryImpl)
	nSize += sizeof(int);  //m_nObjFlag  (GeometryImpl)

	nSize += sizeof(int); //����int charNum ��¼�ַ������ַ���������д���ļ� 
	int charNum = m_sObjName.length();

	nSize += sizeof(char) * charNum; //m_sObjName (GeometryImpl)

	return nSize;
}

bool GeometryImpl::readBuffer(Buffer & buf)
{
	buf.read((char*)&m_byteTypeID, sizeof(int));
	buf.read((char*)&m_byteDim, sizeof(unsigned char));
	buf.read((char*)&m_nRefCount, sizeof(int));
	buf.read((char*)&m_nObjFlag, sizeof(int));

	int ll = 0;
	buf.read((char*)&ll, sizeof(int));
	if (ll > 0)
	{
		m_sObjName.assign(((char*)buf.data()) + buf.getPos(), ll);
		buf.setPos(buf.getPos() + ll);
	}

	return true;
}

//##ModelId=49E5EEA600BB
bool GeometryImpl::writeBuffer(Buffer& buf)
{
	buf.write((char*)&m_byteTypeID, sizeof(int));
	buf.write((char*)&m_byteDim, sizeof(unsigned char));
	buf.write((char*)&m_nRefCount, sizeof(int));
	buf.write((char*)&m_nObjFlag, sizeof(int));

	int ll = 0;
	ll = m_sObjName.length();
	buf.write((char*)&ll, sizeof(int));
	if (ll > 0)
	{
		buf.write((char*)m_sObjName.c_str(), sizeof(char) * ll);
	}

	return true;
}

void GeometryImpl::write(std::ostream & f)
{
	bool flag = false;

	unsigned int nSize = this->sizeBuffer();
	f.write((char*)&nSize, sizeof(unsigned int));//���������ڴ��С,д���ļ�

	char* pMem = new char[nSize];

	Buffer buf;
	buf.attach(pMem, nSize);
	flag = this->writeBuffer(buf);//���������ڴ��д��
	if (!flag) return;

	f.write((char*)pMem, nSize);//������õĶ���һ����д���ļ�
}

void GeometryImpl::read(std::istream & f)
{
	bool flag = false;

	unsigned int nSize = 0;
	f.read((char*)&nSize, sizeof(unsigned int));//�������������ڴ��С

	char* pMem = new char[nSize];
	f.read(pMem, nSize);//��һ����������ڴ棨����

	Buffer buf;
	buf.attach(pMem, nSize);//�����˷��ϵ�ǰ�����С������,��ʹbuf�ӹ�pMem���ٵ��ڴ棺��Ӧһ�����ļ��ж���Ķ���
	flag = this->readBuffer(buf);//���ڴ�����á����������

	assert(flag);
}


//���о�̬�������ڴ���ָ�����͵ļ��ζ���
//���ݴ���Ķ������ͣ��½�һ����Ӧ�������ָ�룬�����ٿռ�
GeometryImpl * GeometryImpl::create(int signal)
{
	if (signal == GEOTYPE_3D_TRIANGLEMESH)//11	
	{
		return dynamic_cast<GeometryImpl*>(new TriangleMesh3dImpl());
	}
	else if (signal == GEOTYPE_3D_POINT)//0
	{
		return dynamic_cast<GeometryImpl*>(new Point3dImpl());
	}
	else if (signal == GEOTYPE_3D_SINGLEPOINT)//0
	{
		return dynamic_cast<GeometryImpl*>(new SinglePoint3dImpl());
	}
	else if (signal == GEOTYPE_3D_MULTIPOINT)//0
	{
		return dynamic_cast<GeometryImpl*>(new MultiPoint3dImpl());;
	}
	else if (signal == GEOTYPE_3D_ANNOTATION)//21
	{
		return dynamic_cast<GeometryImpl*>(new Annotation3dImpl());
	}
	else if (signal == GEOTYPE_3D_REFERENCE)//20
	{
		return dynamic_cast<GeometryImpl*>(new Reference3dImpl());
	}
	else if (signal == GEOTYPE_3D_VOLUME)//1
	{
		return dynamic_cast<GeometryImpl*>(new Volume3dImpl());
	}
	else if (signal == GEOTYPE_3D_SURFACE)//2
	{
		return dynamic_cast<GeometryImpl*>(new Surface3dImpl());
	}
	else if (signal == GEOTYPE_3D_CURVE)//3
	{
		return dynamic_cast<GeometryImpl*>(new Curve3dImpl());
	}
	else if (signal == GEOTYPE_3D_GROUP)//4
	{
		return dynamic_cast<GeometryImpl*>(new Group3dImpl());
	}
	else if (signal == GEOTYPE_3D_VOXEL)//5
	{
		return dynamic_cast<GeometryImpl*>(new Voxel3dImpl());
	}
	else if (signal == GEOTYPE_3D_SOLID)//6
	{
		return dynamic_cast<GeometryImpl*>(new Solid3dImpl());
	}
	else if (signal == GEOTYPE_3D_MULTISOLID)//7
	{
		return dynamic_cast<GeometryImpl*>(new MultiSolid3dImpl());
	}
	else if (signal == GEOTYPE_3D_PRISM)//8
	{
		return dynamic_cast<GeometryImpl*>(new Prism3dImpl());
	}
	else if (signal == GEOTYPE_3D_TETRAHEDRON)//9
	{
		return dynamic_cast<GeometryImpl*>(new Tetrahedron3dImpl());
	}
	else if (signal == GEOTYPE_3D_PARAMETERIZEDSURFACE)//10
	{
		return dynamic_cast<GeometryImpl*>(new ParameterizedSurface3dImpl());
	}
	else if (signal == GEOTYPE_3D_TRIANGLE)//12
	{
		return dynamic_cast<GeometryImpl*>(new Triangle3dImpl());
	}
	else if (signal == GEOTYPE_3D_MULTIPOLYGON)//13
	{
		return dynamic_cast<GeometryImpl*>(new MultiPolygon3dImpl());
	}
	else if (signal == GEOTYPE_3D_POLYGON)//14
	{
		return dynamic_cast<GeometryImpl*>(new Polygon3dImpl());
	}
	else if (signal == GEOTYPE_3D_LINEARRING)//15
	{
		return dynamic_cast<GeometryImpl*>(new LinearRing3dImpl());
	}
	else if (signal == GEOTYPE_3D_PARAMETERIZEDCURVE)//16
	{
		return dynamic_cast<GeometryImpl*>(new ParameterizedCurve3dImpl());
	}
	else if (signal == GEOTYPE_3D_LINESTRING)//17
	{
		return dynamic_cast<GeometryImpl*>(new LineString3dImpl());
	}
	else if (signal == GEOTYPE_3D_MULTIPOLYLINE)//18
	{
		return dynamic_cast<GeometryImpl*>(new MultiPolyline3dImpl());
	}
	else
	{
		return 0;
	}
	 
}


//�½����ζ��󣨸���Buffer�е�typeʵ�ֶ���ָ��Ĵ������ڴ�Ŀ��٣�
//����readBufferʵ�ָ����ڴ������������Ŀ���
//���ض����ָ��
GeometryImpl * GeometryImpl::create(Buffer & buf)//��Buffer�ж�ȡ���ݣ��½�һ��������
{
	//��ö���� TypeID �Դ��б�����һ�����
	unsigned char signal = -1;
	//	char* p = (char*)buf.data() + buf.getPos();
	char* p = (char*)buf.data();
	memcpy((char*)&signal, p, sizeof(unsigned char));

	GeometryImpl* pGeometry = create(signal);
	if (!pGeometry)
		return pGeometry;

	//���������ڴ��buf �������Ӧ�����в����ض���ָ��

	if (true == pGeometry->readBuffer(buf))
		return pGeometry;
	else
	{
		delete pGeometry;
		return NULL;
	}
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace