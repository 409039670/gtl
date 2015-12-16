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
#include <fstream>
#include <set>
#include <list>
#include <memory>
#include <cmath> 
#include "time.h"
#include "assert.h"

#define ENABLE_CONSOLE_OUTPUT 0// 1-enable console window and display information 0-disable 
#define USING_UILIB_TYPE  0 //0-MFC 1-Qt 2-wxWidget   
#define USING_DEFAULT_3DGEOMETRY 1// 0-use geometry plugin library;1-use default geometry library implmented in GV3dCore
#define USING_DEFAULT_2DGEOMETRY 0// 0-use geometry plugin library;1-use default geometry library implmented in GV3dCore
#define USING_DEFAULT_MESHLIB    1// 0-use geometry plugin library;1-use default geometry library implmented in GV3dCore
#if(USING_UILIB_TYPE==0)//MFC
	// ���º궨��Ҫ������ƽ̨��Ҫ������ƽ̨
	// �Ǿ�������Ӧ�ó������蹦�ܵ� Windows��Internet Explorer �Ȳ�Ʒ��
	// ����汾��ͨ����ָ���汾�����Ͱ汾��ƽ̨���������п��õĹ��ܣ������
	// ����������

	// �������Ҫ��Ե�������ָ���汾��ƽ̨�����޸����ж��塣
	// �йز�ͬƽ̨��Ӧֵ��������Ϣ����ο� MSDN��
	#ifndef WINVER                          // ָ��Ҫ������ƽ̨�� Windows Vista��
	#define WINVER 0x0600           // ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
	#endif

	#ifndef _WIN32_WINNT            // ָ��Ҫ������ƽ̨�� Windows Vista��
	#define _WIN32_WINNT 0x0600     // ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
	#endif

	#ifndef _WIN32_WINDOWS          // ָ��Ҫ������ƽ̨�� Windows 98��
	#define _WIN32_WINDOWS 0x0410 // ����ֵ����Ϊ�ʵ���ֵ���������� Windows Me ����߰汾��
	#endif

	#ifndef _WIN32_IE                       // ָ��Ҫ������ƽ̨�� Internet Explorer 7.0��
	#define _WIN32_IE 0x0700        // ����ֵ����Ϊ��Ӧ��ֵ���������� IE �������汾��
	#endif

	#define WIN32_LEAN_AND_MEAN 
	#include "windows.h"
	#include "winbase.h"
	#include "process.h"
	#include "tchar.h"
	#pragma warning (disable:4251)

#elif(USING_UILIB_TYPE==1)//Qt

	#ifndef _PALETTEENTRY_DEFINED
	#define _PALETTEENTRY_DEFINED
	typedef struct tagPALETTEENTRY {
		unsigned char*        peRed;
		unsigned char*        peGreen;
		unsigned char*        peBlue;
		unsigned char*        peFlags;
	} PALETTEENTRY, *PPALETTEENTRY, FAR *LPPALETTEENTRY;
	#endif // !_PALETTEENTRY_DEFINED

	#ifndef _LOGPALETTE_DEFINED
	#define _LOGPALETTE_DEFINED
	/* Logical Palette */
	typedef struct tagLOGPALETTE {
		short        palVersion;
		short        palNumEntries;
		PALETTEENTRY        palPalEntry[1];
	} LOGPALETTE, *PLOGPALETTE, NEAR *NPLOGPALETTE, FAR *LPLOGPALETTE;
	#endif // !_LOGPALETTE_DEFINED



#else//wxWidget
	#ifndef _PALETTEENTRY_DEFINED
	#define _PALETTEENTRY_DEFINED
	typedef struct tagPALETTEENTRY {
		BYTE        peRed;
		BYTE        peGreen;
		BYTE        peBlue;
		BYTE        peFlags;
	} PALETTEENTRY, *PPALETTEENTRY, FAR *LPPALETTEENTRY;
	#endif // !_PALETTEENTRY_DEFINED

	#ifndef _LOGPALETTE_DEFINED
	#define _LOGPALETTE_DEFINED
	/* Logical Palette */
	typedef struct tagLOGPALETTE {
		WORD        palVersion;
		WORD        palNumEntries;
		_Field_size_opt_(palNumEntries) PALETTEENTRY        palPalEntry[1];
	} LOGPALETTE, *PLOGPALETTE, NEAR *NPLOGPALETTE, FAR *LPLOGPALETTE;
	#endif // !_LOGPALETTE_DEFINED


#endif

#ifdef GV3DCORE_EXPORTS
#ifdef _USRDLL
#define GV3DCORE_API __declspec(dllexport)
#else
#define GV3DCORE_API  
#endif
#else
#define GV3DCORE_API __declspec(dllimport)
#endif


#ifndef begin_cn_namespace
#define begin_cn_namespace namespace cn{
#endif

#ifndef end_cn_namespace
#define end_cn_namespace }
#endif

#ifndef using_cn_namespace
#define using_cn_namespace using namespace cn;
#endif


#define begin_edu_namespace namespace edu{
#define end_edu_namespace }
#define using_edu_namespace using namespace edu;

#define begin_cug_namespace namespace cug{
#define end_cug_namespace }
#define using_cug_namespace using namespace cug;

#define begin_gdb_namespace namespace gdb{
#define end_gdb_namespace }
#define using_gdb_namespace using namespace gdb;


#define begin_catalog_namespace namespace catalog{
#define end_catalog_namespace }
#define using_catalog_namespace using namespace catalog;


#define begin_sve_namespace namespace sve{
#define end_sve_namespace }
#define using_sve_namespace using namespace sve; 


#define begin_plugin_namespace namespace plugin{   
#define end_plugin_namespace                   }   
#define useing_plugin_namespace using namespace plugin ;  


#define begin_details_namespace namespace details{   
#define end_details_namespace                   }   
#define useing_details_namespace using namespace details ;

#define begin_ddb_namespace namespace ddb{   
#define end_ddb_namespace                   }   
#define useing_ddb_namespace using namespace ddb ;


using namespace std;

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace


const double  TOLERANCE = 0.0000001;
const double  PI        =  3.14159265358979323846264338327950288419716939937510582;

#ifndef INITMAX
#define INITMAX -9999999999999999999.0
#endif

#ifndef INITMIN
#define INITMIN  9999999999999999999.0
#endif

#ifndef SMALL_NUMBER
#define SMALL_NUMBER 0.0000001
#endif

class Envelope3d;
/** @defgroup Config  Config-ȫ�������� 
*  @{
*/
class GV3DCORE_API Config {

	std::locale _loc;
	static string   shaderPath;
	static string   imagePath;
private:	
	static int  textureFileModel;//	TextureReadModel //0����IO��С�����ļ���1����IO���������ļ���2�����ڴ�ӳ�������ļ�
	static int 	featureFileModel; //1����IO����д��0����IO�ڴ�ӳ���ļ���д
private:
	static Envelope3d defaultQueryEnvelope;
	static double unloadDistance;//����unloadDistance�����ʱ�򣬴��ڴ���ж�أ�
	static double loadDistance;//С��loadDistance�����ʱ�򣬲����ڴ���ж�أ�	
	static unsigned int  numberOfAddFeaturesPreframe;//ÿһ֡�ص����볡���е����Ҫ�ظ���
	static unsigned int cachedTexturesOfRender;//��Ⱦ�����е��������С����λΪ�������
	static unsigned int cachedSharedModelsOfRender;//��Ⱦ�����еĹ���ģ�ͻ����С����λΪ�������
private:
	static unsigned int versionFileDB;//�ļ����ݿ�汾
	static unsigned int versionRDB;//��ϵ���ݿ�汾
private:
	static unsigned  int cachedBlockNumber;//������������������
	static unsigned int useSameBlockID4DomDem;//DEM��DOM���ID�Ƿ��Ӧ
private://ͶӰ����
	static double fovy;
	static double zNear;
	static double zFar;
private://Display Mode
	static unsigned int stereo;//0-disable 1-enable	 
	/*QUAD_BUFFER=0,
	ANAGLYPHIC=1,
	HORIZONTAL_SPLIT=2,
	VERTICAL_SPLIT=3,
	LEFT_EYE=4,
	RIGHT_EYE=5,
	HORIZONTAL_INTERLACE=6,
	VERTICAL_INTERLACE=7,
	CHECKERBOARD=8*/	 
	static unsigned int stereoMode;
private://Dynamic Dispaching
	static unsigned int dispachMode;
public:
	static int cameraLightStatus;
private:
	static unsigned int currentCommand;//��ǰ��ť��ID	
public:		
	Config();
public:
	static string   getShaderPath();
	static string   getImagePath();
	static void   setShaderPath(string );
	static void   setImagePath(string );
	static int getTextureFileModel();
	static int getFeatureFileModel();
	static Envelope3d&  getDefaultQueryEnvelope();
	static double getUnloadDistance();
	static double getLoadDistance();
	static double getFovy();
	static double getZNear();
	static double getZFar();
	static int getNumberOfAddFeaturesPreframe();
	static unsigned int getCachedTexturesOfRender();//��Ⱦ�����е��������С����λΪ�������
	static unsigned int getCachedSharedModelsOfRender();//��Ⱦ�����еĹ���ģ�ͻ����С����λΪ�������
	static unsigned int getCachedBlockNumber();//������������������
	static unsigned int  getFDBVersion();//��ȡ�ļ���汾
	static unsigned int  getRDBVersion();//��ȡ���ݿ�汾
	static unsigned int  sameID4DEMDOMBlock();
	static int getCameraLightStatus();
	static void enableStereo(bool s = true);
	static bool isEnableStereo();
	static unsigned int getStereoMode();
	static bool isDynamicDispatching();
	static std::vector<unsigned long long> & getCacheSizes();
public:
	enum{//Ӧ�ó����й̶��Ĳ������һ����Щ������Ѿ���Render��װ��
		APP_COMMAND_UNKNOWN=0,
		APP_COMMAND_SELECT,//ѡ��
		APP_COMMAND_ROTATE,//�����ת
		APP_COMMAND_ZOOM,//�Ҽ�����
		APP_COMMAND_TRANSLATE,//�м�ƽ��
		APP_COMMAND_END
	};
	static unsigned int getCurrentCommand();
	static void setCurrentCommand(unsigned int cmd);
	static unsigned int beginPluginCommand();//��ȡ���в��������СCommandID
	static unsigned int endPluginCommand();//��ȡ���в���������CommandID
};
/** @} */


end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
