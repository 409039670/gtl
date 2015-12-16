#include "internal_Surface3dImpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
void  Surface3dImpl::setMaterial(MaterialPtr p) {
	if (p->getType() == 0){
		SimpleMaterial * ps = dynamic_cast<SimpleMaterial *>(p);
		SimpleMaterialImpl * pss = dynamic_cast<SimpleMaterialImpl*>(ps);
		m_pMaterial = (MaterialImpl*) pss;
		m_nMatID = m_pMaterial->GetMatID();
	}
	else{
		ComplexMaterial * ps = dynamic_cast<ComplexMaterial *>(p);
		ComplexMaterialImpl * pss = dynamic_cast<ComplexMaterialImpl*>(ps);
		m_pMaterial = (MaterialImpl*) pss;
		m_nMatID = m_pMaterial->GetMatID();
	}
}
MaterialPtr  Surface3dImpl::getMaterial() {
	if (m_pMaterial == 0) return 0;
	if (m_pMaterial->GetMatType() == 0){
		SimpleMaterialImpl * ps = dynamic_cast<SimpleMaterialImpl*>(m_pMaterial);
		SimpleMaterial * pss = dynamic_cast<SimpleMaterial*>(ps);
		return dynamic_cast<MaterialPtr>(pss);
	}
	else{
		ComplexMaterialImpl * ps = dynamic_cast<ComplexMaterialImpl*>(m_pMaterial);
		ComplexMaterial * pss = dynamic_cast<ComplexMaterial*>(ps);
		return dynamic_cast<MaterialPtr>(pss);
	}
}

void Surface3dImpl::SetMaterial(MaterialImpl* pMaterial)
{
	// 	if(m_pMaterial)
	// 	{
	// 		delete m_pMaterial;	
	// 		m_pMaterial = NULL;
	// 	}

	m_pMaterial = pMaterial;
}


void Surface3dImpl::SetMatID(Identifier::raw_type nMatID) //xx//
{
	m_nMatID = nMatID;
}

Identifier::raw_type Surface3dImpl::GetMatID() //xx//
{
	return m_nMatID;
}

MaterialImpl* Surface3dImpl::GetMaterial()
{
	return m_pMaterial;
}

bool Surface3dImpl::readBuffer(Buffer & buf)
{
	int flag = 0;

	flag = Geometry3dImpl::readBuffer(buf);

	//��Ĳ�����Ϣ
	buf.read((char*)&m_nMatID, sizeof(Identifier::raw_type));

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool Surface3dImpl::writeBuffer(Buffer& buf)
{
	int flag = 0;

	flag = Geometry3dImpl::writeBuffer(buf);

	//��Ĳ�����Ϣ
	buf.write((char*)&m_nMatID, sizeof(Identifier::raw_type));  //xx//

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}

size_t Surface3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	//������Ϣ(Geometry��3DGeometry)
	//*************************************************

	nSize += Geometry3dImpl::sizeBuffer();

	//ר����Ϣ
	//*************************************************

	nSize += sizeof(Identifier::raw_type); // m_nMatID  

	//�����Ƕ����洢 ��д�뼸���ļ� �ʲ��㼸�ζ�����ռ�õ��ڴ�
	//Material* m_pMaterial; 

	//*************************************************

	return nSize;
}

void Surface3dImpl::TranslateFrom(Matrix4x4& m4x4)
{
	Geometry3dImpl::TranslateFrom(m4x4);
}

GeometryImpl* Surface3dImpl::CreateObject()
{
	return new Surface3dImpl;
}

//���㵱ǰ����ļ�������
Vertex3d Surface3dImpl::CalCorePoint()
{
	Vertex3d corePt;
	return corePt;
}

/** �麯��
* ͳ�Ƶ�ǰ��������ļ�������
* @param
* @return long*:�����ڲ����ٿռ䡢�ⲿ�ͷ�
*               ����ֵ������ -- ���������������Ч�������������������Ч��ɫ����
*								�����������������ת��Ϊ�ȼ�����
*								����������������
*/
long* Surface3dImpl::GeoStatistics()
{
	long* statis = new long[8];

	for (int i = 0; i < 6; i++)
		statis[i] = 0;

	if (m_pMaterial)
	{
		statis[6] = 1;
		if (m_pMaterial->GetMatType() == MaterialImpl::simpleMat)
		{
			SimpleMaterialImpl* pSimpleM = dynamic_cast<SimpleMaterialImpl*>(m_pMaterial);
			if (pSimpleM->GetTexture())
				statis[7] = 1;
			else
				statis[7] = 0;
		}
		else if (m_pMaterial->GetMatType() == MaterialImpl::complexMat)
		{
			ComplexMaterialImpl* pCompleM = dynamic_cast<ComplexMaterialImpl*>(m_pMaterial);
			statis[7] = pCompleM->materialItemList.size(); // ydcl_xx_���ۼӣ���û�п����ظ����
		}
		else
			statis[7] = 0;
	}
	else
	{
		statis[6] = 0;
		statis[7] = 0;
	}

	return statis;
}

// �ͷŶ������ݲ���ָ��ͱ�������ʼֵ
bool Surface3dImpl::Init()
{
	bool res = Geometry3dImpl::Init();
	if (false == res) return false;

	m_nMatID = -1;
	m_pMaterial = NULL;

	return true;
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace