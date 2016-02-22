#include "internal_volume3dimpl.h"


begin_gtl_namespace
begin_gdb_namespace
bool Volume3dImpl::readBuffer(Buffer & buf)
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


bool Volume3dImpl::writeBuffer(Buffer& buf)
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


size_t Volume3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	//������Ϣ(Geometry��3DGeometry)
	//*************************************************

	nSize += Geometry3dImpl::sizeBuffer();

	//ר����Ϣ
	//*************************************************


	return nSize;
}

void Volume3dImpl::TranslateFrom(Matrix4x4& m4x4)
{
	Geometry3dImpl::TranslateFrom(m4x4);
}

GeometryImpl* Volume3dImpl::CreateObject()
{
	return new Volume3dImpl;
}

//���㵱ǰ����ļ�������
Vertex3d Volume3dImpl::CalCorePoint()
{
	Vertex3d corePt;
	return corePt;
}

// �ͷŶ������ݲ���ָ��ͱ�������ʼֵ
bool Volume3dImpl::Init()
{
	return Geometry3dImpl::Init();
}

end_gdb_namespace
end_gtl_namespace

