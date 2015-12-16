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
#include "internal_Volume3dImpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
//Ҫ������һ����, ���ķ���, �Ƿ����͸����ʾ, 
//�Ƿ��Զ�������������, ��������ӳ��Ч��
//,S ��������ϵ��, T ��������ϵ��, S ���������ƫ����, T 
//���������ƫ����,  S ������ת��ʽ, T ������ת��ʽ,����ģʽ, 
// �Ŵ��˾�, ��С�˾�,
//##ModelId=49E347250222
class MultiSolid3dImpl : virtual public cn::edu::cug::gdb::MultiSolid3d,virtual  public Volume3dImpl
{
	/** @defgroup MultiSolid3dImpl MultiSolid3dImpl-��ά����
	*  @{
	*/

private:
	/** @name ���ԣ���Ա������
	* @{
	*/

	//##ModelId=49E6F6F302DE
	/** ��������� */
	string m_name;

	//##ModelId=49E72F9F01E4
	/** ȱʡ��ɫ */
	unsigned long m_defaultColor;

	//##ModelId=49E6F72B001F
	/** ��ɫ���� */
	std::vector<unsigned long> m_colors;//��ɫ���� 

	//##ModelId=49E6F73600AB
	/** �Ƿ�ʹ�õ�ɫ */
	int m_bApplyDotColor;

	//##ModelId=49E6F742003E
	/** ������������ */
	std::vector<int> m_indices;// �������� 

	//##ModelId=49E6F74E000F
	/** �Ƿ�ʹ������ */
	int m_bApplyTexture; //

	//##ModelId=49E730070399
	/** �������ID */
	int m_textureId;//ID 

	//##ModelId=49E730120128
	/** ������ͼ��ز��� */
	float* m_texpara;

	/** @} */ // ���Խ�β

public:

	/** @defgroup MultiSolid3dImpl ��������Ա������
	* @{
	*/


	/** @name ��������������
	*  @{
	*/

	/** ���캯��
	*/
	MultiSolid3dImpl()
	{
		m_byteTypeID = GEOTYPE_3D_MULTISOLID;
	}

	/** @} */ // ����������������β



	/** @name ���ݲ���
	*  @{
	*/
	/**  ��ѯ����ӿ�
	* @param  [in] signal  �������ͱ�ʶ
	* @param  [out] p ��Ӧ�Ķ���ָ��
	* @return 1 -- ��ѯ�ɹ���0 -- ��ѯʧ��
	*/
	virtual bool queryInterface(int signal, void ** p){
		switch (signal){
		case cn::edu::cug::gdb::GEOMTYPE_3D_MULTISOLID:
		{
			*p = (void*)(cn::edu::cug::gdb::MultiSolid3d *) this;
			return true;
		}
		case cn::edu::cug::gdb::GEOMTYPE_3D_VOLUME:
		{
			*p = (void*)(cn::edu::cug::gdb::Volume3d *) this;
			return true;
		}
		default:
		{
			return false;
		}
		}
	}

	/**  ��ȡ�������
	* @param  [out] f  �������
	* @return ��
	*/
	virtual void getSurfaces(std::vector<cn::edu::cug::gdb::Surface3d*> & f){} //$$

	//##ModelId=49E6F3B3006D
	/** ��ȡ�����弸���ϵ������
	* @return �����
	*/
	unsigned int NumberGeometry();

	//##ModelId=49E6F4870222
	/** Ҫ�����η���ƽ��/��ƽ��
	* @param [in] bMeanNormal �Ƿ�Ҫ����ƽ��
	*/
	void SetMeanNormal(int bMeanNormal);

	//##ModelId=49E6F81D008C
	/** ��ȡ����������ĸ�����
	* @return ��������������
	*/
	unsigned int GetDataSize();

	//##ModelId=49E732A30232
	/** ��ȡ���ж���
	* @param [in] n ������Ŀ
	* @param [in] pdData ���󻺴�
	*/
	void GetAll(unsigned int n, Vertex3d* pdData);

	//##ModelId=49E732B1033C
	/** �趨���ж���
	* @param [in] n ������Ŀ
	* @param [in] pdData ���󻺴�
	*/
	void SetAll(unsigned int n, const Vertex3d* pdData);

	//##ModelId=49E736D101D4
	/** ��ȡ���������ߴ�
	* @return ���������ߴ�
	*/
	unsigned int GetIndexSize();

	//##ModelId=49E736DA02AF
	/** ��ȡ��������
	* @param [in] nIndexSize ������Ŀ
	* @param [in] IndexData ��������
	* @return �����ɹ����
	*/
	int GetIndex(int nIndexSize, int* IndexData);

	/** @} */ // ���ݲ������β



	/** @name �������
	*  @{
	*/

	/** ����һ����ǰ�����������ڴ�ռ�
	* @param
	* @return ���ٵĵ�ǰ������ڴ�ռ���׵�ַ
	*/
	GeometryImpl* CreateObject();

	/** �����ⲿ������4ά����ʵ�ֵ�ǰ����ļ��α任
	* @param [in] m4x4	������4ά����
	* @return
	*/
	void TranslateFrom(Matrix4x4& m4x4);

	/** ���㵱ǰ���������ڴ�Ĵ�С
	* @param
	* @return ��ǰ���������ڴ��С
	*/
	size_t sizeBuffer();

	/** �ӻ������н�����ǰ���󣬲�����Ա������ֵ
	* @param [in] buf	������������
	* @return 1 -- �����ɹ���0 -- ����ʧ��
	*/
	bool readBuffer(Buffer & buf);

	/** �ѵ�ǰ����ĳ�Ա�������ݣ������һ��������
	* @param [out] buf	������������
	* @return 1 -- ����ɹ���0 -- ���ʧ��
	*/
	bool writeBuffer(Buffer& buf);

	/** �ͷŶ������ݲ���ָ��ͱ�������ʼֵ
	* @return �Ƿ�ִ�гɹ�
	*/
	virtual bool Init()
	{
		return true;
	}

	/** @} */ // ����������β

	/** @} */ // ������β

	/** @} */ // ģ���β
};

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
