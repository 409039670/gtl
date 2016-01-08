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
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include "buffer.h"



begin_gtl_namespace
begin_gdb_namespace


/** @defgroup Connector Connector-�ռ����ݿ����ӽӿ�  
*  @{
*/
class CORE_API Connector  {	
	int _type;
	std::string _pathName;
	std::string _user;
	std::string _password;
	std::string _dbname;
	std::string _svrname;
	std::string _port;//�˿ں�
public:
	enum{
		FDB_CONNECTOR,//�ļ�ϵͳ���� 
		RDB_CONNECTOR,//��ϵ���ݿ�ϵͳ����
		DDB_CONNECTOR,//�ֲ�ʽ���ݿ�ϵͳ����
		UNKNOW_CONNECTOR//δ֪����
	};
	/** ��ȡ���Ӷ�������ͣ�FDB_CONNECTOR��ʾ�ļ�ϵͳ���ͣ�RDB_CONNECTOR��ʾ��ϵ���ݿ�ϵͳ���ͣ�
	DDB_CONNECTOR�ֲ�ʽ���ݿ�ϵͳ����,UNKNOW_CONNECTOR��ʾδ֪����
	* @return ���Ӷ��������
	*/
	inline int getType() { return _type;}
public:
	/** Ĭ�Ϲ��캯��
	*/
	Connector();
	/** ���캯����ͨ������·���������Ӷ��󣬹��������ļ�ϵͳ���͵���������
	* @param [in] pathName  ·������
	* @return 
	*/
	Connector(std::string pathName);
	/** ���캯����ͨ���������ݿ��������Ϣ�������Ӷ��󣬹��������ļ�ϵͳ���͵���������
	* @param [in] user      ���ݿ��û���
	* @param [in] password  ���ݿ��û�����
	* @param [in] dbname    ���ݿ�
	* @param [in] svr       ���ݿ������
	* @return 
	*/
	Connector(std::string user,std::string password,std::string dbname,std::string svr);
	/** ���캯����ͨ���������ݿ��������Ϣ�������Ӷ��󣬹��������ļ�ϵͳ���͵���������
	* @param [in] user      ���ݿ��û���
	* @param [in] password  ���ݿ��û�����
	* @param [in] dbname    ���ݿ�
	* @param [in] svrIP     ���ݿ������
	* @param [in] port      ���ݿ�������˿ں�
	* @return 
	*/
	Connector(std::string user,std::string password,std::string dbname,std::string svrIP,std::string port);

	/** �������캯��
	* @param [in] c      ���Ӷ���
	* @return 
	*/
	Connector(const Connector & c);
	/** ��������
	* @param [in] c      ���Ӷ���
	* @return 
	*/
	void copy(const Connector & c);
	/** �ȺŸ�ֵ����
	* @param [in] c      ���Ӷ���
	* @return 
	*/
	Connector operator = (const Connector & c);
	/** ��ȡ�����ַ��������������FDB_CONNECTOR�����ص��ǹ����ļ���·�������������RDB_CONNECTOR���ص��ǰ�����ϵ���ݿ�������ַ���
	* @param [in] pBlock    �ڴ���׵�ַָ��
	* @param [in] blockSize �ڴ���С����λΪ�ֽ�
	* @return std::string ���Ӷ��������ַ���
	*/
	std::string getConnectString();

	/** ��ȡ�����ַ��������������FDB_CONNECTOR�����ص��ǹ����ļ���·�������������RDB_CONNECTOR���ص��ǰ�����ϵ���ݿ�������ַ���
	* @param [in] pBlock    �ڴ���׵�ַָ��
	* @param [in] blockSize �ڴ���С����λΪ�ֽ�
	* @return std::string ���Ӷ��������ַ���
	*/
	const std::string getConnectString() const;

	const std::string getServeName() ;
	const std::string getPort() ;
	const std::string getDatabaseName() ;
	const std::string getUser();
	const std::string getPassword() ;
	void  setDatabaseName(string &dbname); 


};
/** @} */ 


end_gdb_namespace
end_gtl_namespace


