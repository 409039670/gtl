#include "internal_curve3dimpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

size_t Curve3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	//������Ϣ(Geometry��3DGeometry)
	//*************************************************

	nSize += Geometry3dImpl::sizeBuffer();

	//ר����Ϣ
	//*************************************************


	return nSize;
}

bool Curve3dImpl::readBuffer(Buffer & buf)
{
	int flag = 0;

	flag = Geometry3dImpl::readBuffer(buf);

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool Curve3dImpl::writeBuffer(Buffer& buf)
{
	int flag = 0;

	flag = Geometry3dImpl::writeBuffer(buf);

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Curve3dImpl::TranslateFrom(Matrix4x4& m4x4)
{
	Geometry3dImpl::TranslateFrom(m4x4);
}

GeometryImpl* Curve3dImpl::CreateObject()
{
	return new Curve3dImpl;
}

//���㵱ǰ����ļ�������
Vertex3d Curve3dImpl::CalCorePoint()
{
	Vertex3d corePt;
	return corePt;
}

// �ͷŶ������ݲ���ָ��ͱ�������ʼֵ
bool Curve3dImpl::Init()
{
	return Geometry3dImpl::Init();
}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace