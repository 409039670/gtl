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
* Common String Library(csl)
*/
#pragma once

#include <iosfwd>
#include <ios>
#include <istream>
#include <ostream>
#include <string>    // char traits            
#include <cstddef>   // ptrdiff_t
#include <cmath>
#include <locale.h>
#include "config.h"

#ifndef GTL_RECODE_STUB
#define GTL_RECODE_STUB
#endif
//#define FUTURE_NEEDS

begin_gtl_namespace
begin_gdb_namespace

/* -------------------------------------------------------------------- */
/*      derived from gdal,RFC 23 character set conversion/recoding API   */
/*      (cslencoding.cpp).                                              */
/* -------------------------------------------------------------------- */
#define GTL_ENC_LOCALE     ""
#define GTL_ENC_UTF8       "UTF-8"
#define GTL_ENC_UTF16      "UTF-16"
#define GTL_ENC_UCS2       "UCS-2"
#define GTL_ENC_UCS4       "UCS-4"
#define GTL_ENC_ASCII      "ASCII"
#define GTL_ENC_ISO8859_1  "ISO-8859-1"

int   cslEncodingCharSize(const char *pszEncoding);
void   cslClearRecodeWarningFlags();
char  *cslRecode(const char *pszSource,
	const char *pszSrcEncoding,
	const char *pszDstEncoding) ;
char  *cslRecodeFromWChar(const wchar_t *pwszSource,
	const char *pszSrcEncoding,
	const char *pszDstEncoding) ;
wchar_t  *cslRecodeToWChar(const char *pszSource,
	const char *pszSrcEncoding,
	const char *pszDstEncoding) ;
int  cslIsUTF8(const char* pabyData, int nLen);
char  *cslForceToASCII(const char* pabyData, int nLen, char chReplacementChar);

//C++�汾����չ�ַ�����
typedef std::string _gtl_string;
class CORE_API String : public _gtl_string {
public:
	String(void) {}
	String(const std::string &oStr) : _gtl_string(oStr) {}
	String(const char *pszStr) : _gtl_string(pszStr) {}

	operator const char* (void) const { return c_str(); }

	char& operator[](std::string::size_type i)
	{
		return _gtl_string::operator[](i);
	}

	const char& operator[](std::string::size_type i) const
	{
		return _gtl_string::operator[](i);
	}

	char& operator[](int i)
	{
		return _gtl_string::operator[](static_cast<std::string::size_type>(i));
	}

	const char& operator[](int i) const
	{
		return _gtl_string::operator[](static_cast<std::string::size_type>(i));
	}

	void clear() { resize(0); }

	// NULL safe assign and free.
	void seize(char *pszValue)
	{
		if (pszValue == NULL)
			clear();
		else
		{
			*this = pszValue;
			free(pszValue);
		}
	}

	String & String::printf(const char *pszFormat, ...);
	String &String::vprintf(const char *pszFormat, va_list args);
	String & format(double dfValue, const char *pszFormat = NULL);
	String & trim();
	String & recode(const char *pszSrcEncoding, const char *pszDstEncoding);

	/* case insensitive find alternates */
	size_t    ifind(const std::string & str, size_t pos = 0) const;
	size_t    ifind(const char * s, size_t pos = 0) const;
	String &  toupper(void);
	String &  tolower(void);
};

/** �ַ����ü�����
* @param [in,out] str �ַ�������
* @param [in] c ָ���Ĳü��ַ�
* @return ��
*/
template<typename CharT>
void trim(std::basic_string<CharT>  & str, CharT c)
{
	//ɾ�������˵�ָ���ַ�
	//std::basic_string<CharT>::size_type pos = str.find_last_not_of(c);
	auto pos = str.find_last_not_of(c);
	if (pos != std::basic_string<CharT>::npos)
	{
		str.erase(pos + 1);
		pos = str.find_first_not_of(c);
		if (pos != std::basic_string<CharT>::npos)
			str.erase(0, pos);
	}
	else
		str.erase(str.begin(), str.end());
	//ɾ���м��ָ���ַ�
	pos = str.find_first_of(c);
	while (pos != std::basic_string<CharT>::npos) {
		str.erase(str.begin() + pos);
		pos = str.find_first_of(c);
	}
}

/** ������ת�����ַ���
* @param [in,out] num   ��Ҫת��������
@return ת���õ����ַ���
*/
template<typename CharT, typename NumericT>
std::basic_string<CharT> numberToString(NumericT num)
{
	std::basic_ostringstream<CharT> oss;
	oss << (NumericT)num;
	if (oss.str().find_first_of(',') != std::basic_string<CharT>::npos) {//���и�ʽ����
		std::basic_string<CharT> sz = oss.str();
		trim<CharT>(sz, ',');
		return sz;
	}
	/* ���������ַ���ʽ�Ķ��ţ�Linux�±��벻ͨ,VC�¿��Ա���ͨ����������δ�����ʱֻʵ��VC�µ�*/
#ifdef _MSC_VER
	/*else if (oss.str().find_first_of('��') != std::basic_string<CharT>::npos){
	std::basic_string<CharT> sz = oss.str();
	trim<CharT>(sz, '��');
	return sz;
	}*/
#endif //_MSC_VER
	else {
		return oss.str();
	}
}

/** �ַ���ת��������
* @param [in,out] str �ַ���
* @return ת���õ�������
*/
template<typename NumericT, typename CharT>
NumericT stringToNumber(const std::basic_string<CharT> &str)
{

	std::basic_istringstream<CharT> iss(str);
	NumericT result;
	iss >> result;
	return result;
}
/** ���ַ���ת��������
@param [in,out] str �ַ���ָ��
@return ת���õ�������
*/
template<typename NumericT, typename CharT>
NumericT stringToNumber(const CharT * str)
{
	std::basic_istringstream<CharT> iss(str);
	NumericT result;
	iss >> result;
	return result;
}

void stringToWString(std::string & s, std::wstring & sd);
//ֻ�д���ͬһ���뷽ʽ������²�����ȷ�ģ����������ϵ��ͬ���򱾺���������ȷ��ִ��
void wstringToString(std::wstring & s, std::string & sd);

//C���԰汾���ַ������ַ����б��������
typedef char** CSTRLIST;//c style StringList
#define CSLT_HONOURSTRINGS      0x0001
#define CSLT_ALLOWEMPTYTOKENS   0x0002
#define CSLT_PRESERVEQUOTES     0x0004
#define CSLT_PRESERVEESCAPES    0x0008
#define CSLT_STRIPLEADSPACES    0x0010
#define CSLT_STRIPENDSPACES     0x0020
CSTRLIST cslAddString(CSTRLIST old, const char * str);
const char * cslGetString(CSTRLIST sl, int iField);
int cslIFindString(CSTRLIST sl, const char * str);
int cslFindString(CSTRLIST sl, const char * str);
char * cslDuplicateString(const char * s);
bool cslEqualString(const char * s1, const char * s2);
bool cslNEqualString(const char * s1, const char * s2, int n);
bool cslIEqualString(const char * s1, const char * s2);
bool cslNIEqualString(const char * s1, const char * s2, int n);
size_t cslConcatenateString(char* desString, const char * srcString, size_t DesSize);
size_t cslCopyString(char* desString, const char * srcString, size_t DesSize);
CSTRLIST cslTokenizeString(const char * pszString, const char * pszDelimiters, int nCSLTFlags);
CSTRLIST cslTokenizeString(const char * pszString, const char * pszDelimiters, int bHonourStrings, int bAllowEmptyTokens);
CSTRLIST cslTokenizeString(const char *pszString);
CSTRLIST cslDuplicate(CSTRLIST);
int cslCount(CSTRLIST sl);
void cslDestroy(CSTRLIST sl);
double cslStringToNumber(const char *nptr, char **endptr, char point);
double cslStringToNumber(const char *nptr);
double cslMultilingualStringToNumber(const char *nptr);
long cslStringToInteger(const char * pszNumer, int maxLen);
int cslFillString(char * pszDest, const char * pszSrc, int mexlen, char fillChar = ' ');
char* cslReplacePointByLocalePoint(const char* pszNumber, char point);
bool cslTestBoolean(const char * v);
CSTRLIST cslAddNameValue(CSTRLIST papszStrList, const char *pszName, const char *pszValue);
CSTRLIST cslSetNameValue(CSTRLIST papszList, const char *pszName, const char *pszValue);
const char *cslFetchNameValue(CSTRLIST papszStrList, const char *pszName, bool insensetive = true);
const char *cslParseNameValue(const char *pszNameValue, char **ppszKey);
int cslIFindName(CSTRLIST papszStrList, const char *pszName);


class CORE_API StringList {
	CSTRLIST _data;
public:
	inline void add(const char * s) { _data = cslAddString(_data, s);}
	inline void add(const String & s) {	_data = cslAddString(_data, s.c_str());	}
	inline void add(const std::string & s) {_data = cslAddString(_data, s.c_str());	}
	inline int count() { return cslCount(_data); }
	inline int size() { return cslCount(_data); }
	inline const char * get(size_t i) { return (const char *) _data[i]; }
	inline size_t indexInsensetive(const char * s) { return cslIFindString(_data, s); }
	inline size_t index(const char * s) { return cslFindString(_data, s); }
	inline void attach(CSTRLIST sl) {
		if (_data)
			cslDestroy(_data);
		_data = sl;
	}
	inline CSTRLIST detach( ) {
		CSTRLIST p = _data;
		_data = 0;
		return p;
	}
public:
	StringList();
	StringList(const StringList & sl);
	virtual ~StringList();
	StringList operator=(const StringList & sl);
	const char * operator[](size_t i);
};

/*
* ��std::map<std::string,std::string> ��������
* ��StringMap��һ���ַ������飬ÿ��Ԫ��Ϊһ��
* �ַ�����һ���ַ�����key��alue�Լ��ָ�����������ɣ�
* key��value֮�����:��=�ָ�
*/
class CORE_API StringMap {
	CSTRLIST _data;
public:
	inline void add(const char *pszKey, const char *pszValue) {
		_data=cslAddNameValue(_data, pszKey, pszValue);
	}
	inline void set(const char *pszKey, const char *pszValue) {
		_data=cslSetNameValue(_data, pszKey,pszValue);
	} 
	inline const char * find(const char * key, bool insensetive = true) {		 
			return  cslFetchNameValue(_data, key, insensetive);
	}
public:
	static void parse(const char * key_value, std::pair<std::string, std::string> & p);
	static std::pair<std::string, std::string> parse(const char * key_value);
public:
	StringMap();
	StringMap(const StringMap & sl);
	virtual ~StringMap();
	StringMap operator=(const StringMap & sl);
	//���ص���һ������key���ָ����ţ��Լ�value��������ɵ��ַ��������Բ���parse��������
	const char * operator[](size_t i);
};
//////////////////////////////////////////////////////////////////////////////////////////////
class CORE_API StringPrinter {
public:
	static int print(String & outString, double v);
	static int print(String & outString, float v);

	static int print(String & outString, int8_t  v);
	static int print(String & outString, uint8_t v);

	static int print(String & outString, int16_t v);
	static int print(String & outString, uint16_t v);

	static int print(String & outString, int32_t v);
	static int print(String & outString, uint32_t v);

	static int print(String & outString, int64_t v);
	static int print(String & outString, uint64_t v);

	static int print(char * pszDest, const char * pszSrc, int mexlen, char fillChar);
	static int print(char * pszDest, const char * pszSrc, int mexlen);
	static int print(char * pszDest, int32_t v, int mexlen);
	static int print(char * pszDest, uint64_t v, int mexlen); 
};
end_gdb_namespace
end_gtl_namespace



