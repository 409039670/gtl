// gtl_test_socketclient.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../../gtl/socket.h"

int _tmain(int argc, _TCHAR* argv[])
{
	gtl::Socket::startup();

	gtl::SocketClient sc;
	gtl::SessionSharedPtr ssp = sc.connect("127.0.0.1", 6666);

	int ok = -1;
	int cmd = gtl::Session::BEGINSESSION;
	ssp->send((const char *)&cmd, sizeof(int),0);
	
	ssp->receive((char*)&ok, sizeof(int), 0);
	if (ok == gtl::Session::OKAY)
	{
		std::cout << "��ʼ�Ự\n";
	}
////////////////////////////////////////////////////////////
	cmd = gtl::Session::RANGEQUERY;
	ssp->send((const char *)&cmd, sizeof(int), 0);
	ssp->receive((char*)&ok, sizeof(int), 0);
	if (ok == gtl::Session::OKAY)
	{
		std::cout << "��ʼ��Χ��ѯ\n";
	}
	
	//���Ͳ���
	char buf[256];
	ssp->send(buf, 256, 0);

	//���ղ�ѯ���
	int result = ssp->receive(buf, 256, 0);
////////////////////////////////////////////////////////////////
	cmd = gtl::Session::KNNQUERY;
	ssp->send((const char *)&cmd, sizeof(int), 0);
	ssp->receive((char*)&ok, sizeof(int), 0);
	if (ok == gtl::Session::OKAY)
	{
		std::cout << "��ʼ�ڽ���ѯ\n";
	}

	//���Ͳ���
	
	ssp->send(buf, 256, 0);

	//���ղ�ѯ���
	result = ssp->receive(buf, 256, 0);
//////////////////////////////////////////////////////////////////
	cmd = gtl::Session::KNNQUERY;
	ssp->send((const char *)&cmd, sizeof(int), 0);
	ssp->receive((char*)&ok, sizeof(int), 0);
	if (ok == gtl::Session::OKAY)
	{
		std::cout << "��ʼ�ڽ���ѯ\n";
	}

	//���Ͳ���

	ssp->send(buf, 256, 0);

	//���ղ�ѯ���
	result = ssp->receive(buf, 256, 0);
////////////////////////////////////////////////////////////////////
	cmd = gtl::Session::ENDSESSION;
	ssp->send((const char *)&cmd, sizeof(int), 0);
	ssp->receive((char*)&ok, sizeof(int), 0);
	if (ok == gtl::Session::OKAY)
	{
		std::cout << "�����Ự\n";
	}

	gtl::Socket::cleanup();
	return 0;
}

