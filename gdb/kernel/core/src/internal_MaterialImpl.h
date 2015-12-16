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
#include "material.h"
#include "internal_TextureImpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
 
class MaterialImpl:virtual public Material
{
public:
	/*0-������1-��������*/
	virtual int getType() const { return m_nMatType; }
	virtual Identifier::raw_type getID() { return m_nMatID; }
	virtual void setID(Identifier::raw_type mid) { m_nMatID = mid; }
	virtual std::string getName() { return m_strMatName; }
	virtual void setName(std::string name) { m_strMatName = name; }
protected:
 
	/** �������� */
	int m_nMatType;

	/** ����ID */
	unsigned long long m_nMatID;

	/** �������� */
	string m_strMatName;

	/** @} */ // ���Խ�β

 
protected:
	/** ���캯��
	*/
	MaterialImpl();

public:
	enum{ simpleMat, complexMat };
	/** ��������
	*/
	virtual ~MaterialImpl();
public:

	/** �����������
	* @param
	* @return ��������
	*/
	int GetMatType()
	{
		return m_nMatType;
	}

	/** �������ID
	* @param
	* @return ����ID
	*/
	unsigned long long GetMatID()
	{
		return m_nMatID;
	}

	/** ��������ID
	* @param [in]  nMatID ����ID
	* @return
	*/
	void SetMatID(unsigned long long nMatID)
	{
		m_nMatID = nMatID;
	}

	/** �����������
	* @param
	* @return ��������
	*/
	string GetMatName()
	{
		return m_strMatName;
	}

	/** ������������
	* @param [in] name ��������
	* @return
	*/
	void SetMatName(string name)
	{
		m_strMatName = name;
	}

	 

};

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace