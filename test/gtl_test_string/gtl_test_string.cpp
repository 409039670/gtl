// gtl_test_string.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../../gtl/gtl.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//test for 8 bits char
	{
		std::string sz = "����TEST";
		void * buf = 0;
		std::size_t s = 0;
		gtl::stringToBuffer<char>(sz, buf, s);
		std::string sz2;
		gtl::bufferToString<char>(buf, sz2);
		if (buf)
			delete[] buf;
	}
	//test for 16 bits char
	{
		std::wstring sz = L"����TEST"; 
		assert(sz.length() == sz.size());
		void * buf = 0;
		std::size_t s = 0;
		gtl::stringToBuffer<wchar_t>(sz, buf, s);
		std::wstring sz2;
		gtl::bufferToString<wchar_t>(buf, sz2);
		if (buf)
			delete[] buf;
	}	
	return 0;
}

