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
#include "internal_textureimpl.h"
#include "internal_materialimpl.h"


begin_gtl_namespace
begin_gdb_namespace
class SimpleMaterialImpl : virtual public  SimpleMaterial, virtual public MaterialImpl
{ 
protected:

	/** ����ID����*/
	Identifier::raw_type m_nTextureID;

	/** ����������ָ��*/
	TextureImpl* m_pTexture;

public:
	/** ���ʲ���*/
	MAT_PARAM m_matParam;
 

public:
 

	/** ���캯��
	*/
	SimpleMaterialImpl();
	/** ��������
	*/
	virtual ~SimpleMaterialImpl();

 
	/** ��������
	* @param [in] pTextruָ�������ָ��
	*/
	void SetTexture(TextureImpl* pTextrue)
	{
		// 		if(m_pTexture)
		// 		{
		// 			delete m_pTexture;	
		// 			m_pTexture = NULL;
		// 		}

		m_pTexture = pTextrue;
	}

	/** ��ȡ����
	* @return �������ָ��
	*/
	TextureImpl* GetTexture()
	{
		return m_pTexture;
	}

	/** ��ȡ���ʲ���
	* @return ����
	*/
	MAT_PARAM GetMatParam() const
	{
		return m_matParam;
	}
	/** ���ò��ʲ���
	* @param [in] param���ʲ���
	*/
	void SetMatParam(MAT_PARAM param)
	{
		m_matParam = param;
	}
	/** ��ȡ����������ID
	* @return ����id
	*/
	Identifier::raw_type GetTextureID() const  //xx//
	{
		return m_nTextureID;
	}
	/** ���ù���������ID
	* @param [in] nTexID����ID
	*/
	void SetTextureID(Identifier::raw_type nTexID)  //xx//
	{
		m_nTextureID = nTexID;
	}

 
public:
	virtual Identifier::raw_type getTextureID() {
		return m_nTextureID;
	}
	virtual void setTextureID(Identifier::raw_type d) {
		m_nTextureID = d;
	}
	virtual TexturePtr getTexture() {
		return dynamic_cast<TexturePtr>(m_pTexture);
	}
	virtual void setTexture(TexturePtr p){
		m_pTexture = dynamic_cast<TextureImpl*>(p);
	}

	virtual MAT_PARAM & getMaterialParameter() {
		return m_matParam;
	}
	virtual void setMaterialParameter(MAT_PARAM & d) {
		m_matParam = d;
	}
};

end_gdb_namespace
end_gtl_namespace





