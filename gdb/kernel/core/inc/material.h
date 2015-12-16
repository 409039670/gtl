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
#include "texture.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

/** @defgroup material  ������������  
    *  @{
    */

// flag :
#define TEXTURE_FLAG_TEXTURE        1//������Ч
#define TEXTURE_FLAG_MATERIAL       2//������Ч
#define TEXTURE_FLAG_COLOR          4//��ɫ��Ч

// transparent: 
#define TEXTURE_TRANSPARENT_DISABLE 0 //��͸��
#define TEXTURE_TRANSPARENT_ENABLE  1 //͸�������ͱ�������ϣ��粣����, ���� alpha
#define TEXTURE_TRANSPARENT_SPECIAL 2 //����ָ�����֣��续���˵������з����Ĳ��֣�

//cullface :
#define TEXTURE_CULLFACE_BACK		0//ȱʡ�������޳�
#define TEXTURE_CULLFACE_DISABLE	1//���޳�(˫�����)
#define TEXTURE_CULLFACE_FRONT		2//�����޳�

//depth:(��ȼ��)
#define TEXTURE_DEPTH_ENABLE		0//ȱʡ��ʹ��ȱʡ��ȼ�ⷽʽ
#define TEXTURE_DEPTH_DISABLE		1//��������ȼ��
#define TEXTURE_DEPTH_ALWAYS		2//��ȱȽ�����ͨ��
#define TEXTURE_DEPTH_WRITEDISABLE	4//��Ȼ���ֻ��
/** @} */  

/** @addtogroup material  MAT_PARAM-����������ݽṹ 
    *  @{
    */
struct MAT_PARAM
{
	/** @name ���ԣ���Ա������
	* @{
	*/
	
	/** ��־λ(�������ʡ���ɫ) */
	unsigned char		flag;               
	/** ͸���ķ�ʽ 
	* -TEXTURE_TRANSPARENT_DISABLE
	* -TEXTURE_TRANSPARENT_ENABLE
	* -TEXTURE_TRANSPARENT_SPECIAL
    */
	unsigned char		transparent;        
	/** ���޳���ʽ 
	* -TEXTURE_CULLFACE_BACK
	* -TEXTURE_CULLFACE_DISABLE
	* -TEXTURE_CULLFACE_FRONT
	*/
	unsigned char		cullface;		
	
	/** ��ȼ�� */
	unsigned char		depth;	
	
	/** ����*/
	unsigned char		reserve[4];			

    /** ͸����  [ 0.0 (��͸��)  ~  1.0 (��ȫ͸��) ] */
	float		alpha;              
	
	/** ������ */
	unsigned long	ambient;	
	
	/** ������ */
	unsigned long	diffuse;			

	/** ���淴�� */
	unsigned long	specular;			

	/** ��ɫ */
	unsigned long	color;				

	/** ����ƽ�Ʋ��� */
	float		uOffset, vOffset;	

	/** ������ת��������λΪ�ȣ���ת��Ϊ (0, 0, 1)�� */
	float		rotAngle;			

	/** �߹�*/
	float		shininess;			
	/** ����ģ�� */
	float		illumination;
	/** @} */ // ���Խ�β
 
	/** @name ��������������
	*  @{
	*/

	/** ���캯��
	*/
	MAT_PARAM()
	{
		memset(this, 0, sizeof(MAT_PARAM));
	}
	 /** @} */ // ����������������β
	
};
typedef MAT_PARAM*   LPMAT_PARAM;
/** @} */

/** @addtogroup material  Material 
    *  @{
    */
class Material;
typedef std::shared_ptr<Material> MaterialSharedPtr;
typedef Material* MaterialPtr;
class CORE_API Material  
{
public:
	/*0-������1-��������*/
	virtual int getType() const =0;
	virtual Identifier::raw_type getID()=0;
	virtual void setID(Identifier::raw_type)=0;
	virtual std::string getName()=0;
	virtual void setName(std::string name)=0;

	static MaterialSharedPtr create(int type=0);
};
/** @} */

/** @addtogroup material  SimpleMaterial 
    *  @{
    */
class CORE_API SimpleMaterial  : virtual  public Material
{
public:
	virtual Identifier::raw_type getTextureID()=0;
	virtual void setTextureID(Identifier::raw_type)=0;
	virtual TexturePtr getTexture() =0;
	virtual void setTexture(TexturePtr p) =0;

	virtual MAT_PARAM & getMaterialParameter() =0;
	virtual void setMaterialParameter(MAT_PARAM &) =0;
};
/** @} */

/** @addtogroup material  COMMAT_ITEM 
    *  @{
    */
struct COMMAT_ITEM
{

	/** @name ���ԣ���Ա������
	* @{
	*/
	
	/** �������� */
	long matType;
	/** ����ID */
	int textureID;
	/** ǿ�� */
	float intensity;
	/** ��ɫ */
	unsigned long color;
	/** �� u ���������ظ��Ĵ��� */
	double  uRepeat;	
	/** �� v ���������ظ��Ĵ��� */
	double  vRepeat;	
    /** ����ƽ�� */
	float	uOffset;	
	/** ����ƽ�� */
	float	vOffset;	
	/** ������ת */
	float	rotAngle;	
	/** ͸����  [ 0.0 (��͸��)  ~  1.0 (��ȫ͸��) ] */
	float	alpha;      

	/** @} */ // ���Խ�β

	
	/** @name ��������������
	*  @{
	*/

	/** ���캯��
	*/
	COMMAT_ITEM()
	{
		memset(this, 0, sizeof(COMMAT_ITEM));
	}
    /** @} */ // ����������������β

};
/** @} */

/** @addtogroup material  ComplexMaterial 
    *  @{
    */
class CORE_API ComplexMaterial:virtual public Material {
public:
	/** ���Ӳ��ʲ��� */
	virtual std::vector<COMMAT_ITEM> & getMaterialItems() =0;
};
/** @} */
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace