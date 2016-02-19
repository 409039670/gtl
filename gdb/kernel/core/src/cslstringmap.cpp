#include "csl.h"
begin_gtl_namespace
begin_gdb_namespace
void StringMap::parse(const char * key_value, std::pair<std::string, std::string> & p) {
	char * key = 0;
	p.second = cslParseNameValue(key_value, &key);
	p.first = (const char*)key;
	if (key)
		free(key);
}
std::pair<std::string, std::string>  StringMap::parse(const char * key_value) {
	std::pair<std::string, std::string> p;
	char * key = 0;
	p.second = cslParseNameValue(key_value, &key);
	p.first = (const char*)key;
	if (key)
		free(key);
	return p;
}
StringMap::StringMap() {
	_data = 0;
}
StringMap::StringMap(const StringMap & sl) {
	_data = 0;
	cslDuplicate(sl._data);
}
StringMap::~StringMap() {
	if (_data)
		cslDestroy(_data);
	_data = 0;
}
StringMap  StringMap::operator=(const StringMap & sl) {
	_data = 0;
	cslDuplicate(sl._data);
	return *this;
}
//���ص���һ������key���ָ����ţ��Լ�value��������ɵ��ַ��������Բ���parse��������
const char *  StringMap::operator[](size_t i) {
	return (const char *)_data[i];
}
end_gdb_namespace
end_gtl_namespace
