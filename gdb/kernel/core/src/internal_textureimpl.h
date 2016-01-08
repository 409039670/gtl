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
#include "texture.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

class TextureImpl : public Texture
{
protected:

	/** ����ID */
	unsigned long long m_nTextureID;

	/** ����mingch */
	string m_strTextureName;

	/** �������� */
	int m_nTextureType;

	/** �����ڴ����� */
	unsigned char* m_pTexdata;

	/** �������ݳ��� */
	size_t m_nLength;

	int m_wrapMode;

public:
	/** ���캯��
	*/
	TextureImpl();
	/** ��������
	*/
	~TextureImpl();
	 
	enum{ BMP, JPG, DDS, TGA, TIF };

	// WRAPģʽ                  
	enum WrapMode {
		CLAMP = 0x2900,
		REPEAT = 0x2901,
	};

 

	/** �������ID
	* @param
	* @return  ����ID
	*/
	unsigned long long GetTextureID() const
	{
		return m_nTextureID;
	}

	/** ����ע����ɫ
	* @param [in] nTexI	 ����ID
	* @return
	*/
	void SetTextureID(unsigned long long nTexID)
	{
		m_nTextureID = nTexID;
	}

	/** �����������
	* @param
	* @return ��������
	*/
	std::string GetTextureName() const
	{
		return m_strTextureName;
	}

	/** ������������
	* @param [in]  strTexName ��������
	* @return
	*/
	void SetTextureName(std::string strTexName)
	{
		m_strTextureName = strTexName;
	}

	/** �����������
	* @param
	* @return ��������
	*/
	int GetTextureType() const
	{
		return m_nTextureType;
	}

	/** ������������
	* @param [in] nTexType ��������
	* @return
	*/
	void SetTextureType(int nTexType)
	{
		m_nTextureType = nTexType;
	}

	/** ����������ݳ���
	* @param
	* @return  �������ݳ���
	*/
	size_t GetTextureSize() const
	{
		return m_nLength;
	}

	/** �����������ݳ���
	* @param [in] length  �������ݳ���
	* @return
	*/
	void SetTextureSize(size_t length)
	{
		m_nLength = length;
	}

	/** ��������ڴ�����
	* @param
	* @return �����ڴ�����ָ��
	*/
	unsigned char* GetTextureData() const
	{
		return m_pTexdata;
	}

	/** ���������ڴ�����
	* @param [in] texdata �����ڴ�����ָ��
	* @return
	*/
	void SetTextureData(unsigned char* texdata)
	{
		m_pTexdata = texdata;
	}

	/** ��������WRAPģʽ
	* @param
	* @return �����WRAPģʽ
	*/
	int GetWrapMode() const
	{
		return m_wrapMode;
	}

	/** ���������WRAPģʽ
	* @param [in] nWrapMode ���������WRAPģʽ
	* @return
	*/
	void SetWrapMode(int nWrapMode)
	{
		m_wrapMode = nWrapMode;
	}
	 

public:

	virtual cn::edu::cug::gdb::Identifier::raw_type getID() const {
		return m_nTextureID;
	}
	virtual void setID(cn::edu::cug::gdb::Identifier::raw_type d) {
		m_nTextureID = d;
	}
	virtual std::string getName() const {
		return m_strTextureName;
	}
	virtual void setName(std::string strTexName){
		m_strTextureName = strTexName;
	}
	virtual int getType() const {
		return this->m_nTextureType;
	}
	virtual void setType(int nTexType){
		m_nTextureType = nTexType;
	}
	virtual size_t getDataSize() const {
		return m_nLength;
	}
	virtual void setDataSize(size_t length) {
		m_nLength = length;
	}
	virtual unsigned char* getData() const{
		return m_pTexdata;
	}
	virtual void setData(unsigned char* texdata){
		m_pTexdata = texdata;
	}

	virtual int getWrapMode() const
	{
		return m_wrapMode;
	}

	virtual void setWrapMode(int nWrapMode)
	{
		m_wrapMode = nWrapMode;
	} 
};
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace