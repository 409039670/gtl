������õ���freeImage317�ľ�̬�⡣
����freeimage���ִ���Severity	Code	Description	Project	File	Line	Suppression State
Error	C1189	#error:  Macro definition of snprintf conflicts with Standard Library function declaration	
LibTIFF4	C:\Program Files (x86)\Windows Kits\10\Include\10.0.10240.0\ucrt\stdio.h	1927	

���������Ҫ������VS2015������C99��׼

�ҵ�LibTIFF4�����е�tif_config.h�е����д��룺
#ifdef _WIN32

#define snprintf _snprintf
#define lfind _lfind

#endif // _WIN32
�滻Ϊ��
#ifdef _WIN32

#if _MSC_VER<1900
#define snprintf _snprintf
#endif
#define lfind _lfind

#endif // _WIN32