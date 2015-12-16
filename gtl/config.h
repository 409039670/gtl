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


#pragma  once

#ifdef WIN32 
#define USE_WINSOWS_OS   //Windows OS

#ifdef _DEBUG
#define  _GTL_DEBUG_
#endif


#else
#define USE_LINUX_OS   //Linux OS
#ifndef MAX_PATH
#define MAX_PATH          260
#endif //MAX_PATH
#endif
/*
*���ļ�ʵ��ȫ�ֲ�������
*/

/*��ʼͼ����Ⱦ����֧��Ԥ����
  ���б�����ֻ����һ��Ϊ1��Ϊ1��ʾ��ǰʹ�õ�����*/
#define  USE_OPENGL_ENGINE   1 //SGI OpenGL ��Ⱦ����
#define  USE_DIRECTX_ENGINE  0 //MS DirectX ��Ⱦ����
#define  USE_MESA3D_ENGINE   0 //Mesa3D ��Ⱦ����
#define  USE_OSG_ENGINE      0 //OSG    ��Ⱦ����
#define  USE_OPENGINVENTOR_ENGINE 0 //SGI Open Inventor��Ⱦ����

#define  GTL_VERSION  2
/*����ͼ����Ⱦ����֧��Ԥ����*/

#ifdef USE_WINDOWS_OS
#include <windows.h>
#include "minwindef.h"
#endif
