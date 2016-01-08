#pragma once
/*
* Geosciences Template Library
*
* Copyright (c) 2008
* Zhenwen He (zwhe@hotmail.com)
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Zhenwen He  makes no  representations
* about the suitability of this software for any purpose.
* It is provided "as is" without express or implied warranty.
*/
#pragma once
#include "mesh.h" 


begin_gtl_namespace
begin_gdb_namespace
class EclipseMeshImpl;
//������Ϣ�ṹ��
struct PropertyInfo
{
	std::string name;
	double* proData;
};
//��������Ϣ�ṹ��
struct CarfinInfo
{
	//���ܵ�λ��
	int mini;
	int maxi;
	int minj;
	int maxj;
	int mink;
	int maxk;
	//�����������
	int ni;
	int nj;
	int nk;
	//����������
	std::vector<PropertyInfo> VecPropertyInfo;
};
class EclipseMeshReader
{
public:
	EclipseMeshReader(void);
	~EclipseMeshReader(void);
	void readGredclFile(const std::string filename);
	void constructMesh(EclipseMeshImpl * mesh);//��mesh����ֵ
private:
	char* getLine();
	int CompareKeyword(char* keyword, int flag = 0);
	int ReadSpecGrid();
	int ReadCoord();
	int ReadZcorn();
	int ReadActnum();
	int ReadDoubles(int nfloats, double* farray);
	int ReadBytes(int nbytes, char* barray);
	static int getCarinfo(const char* line, int& i0, int& i1, int& j0, int&j1, int &k0, int& k1,
		int& nx, int& ny, int& nz);

private:
	FILE* fp;
	char m_Line[1024];
	int m_LineNumber;
	int m_ni, m_nj, m_nk;
	//��Žǵ�����ģ��������Ϣ
	std::vector<PropertyInfo> m_VecPropertyInfo;
	std::vector<CarfinInfo> m_VecCarfin;
public:
	double* m_coord; //�洢�ز㶥�������꣬��6*(m_iDim+1)*(m_jDim+1)��ֵ��
	double* m_zcorn; //���нǵ����񶥵�Ķ���Zֵ��(2*m_iDim)*(2*m_jDim)*(2*m_kDim)��ֵ
	char* m_actnum; //������Ч�ԣ�nx*ny*nz��ֵ
};
end_gdb_namespace
end_gtl_namespace

