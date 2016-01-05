#include "internal_featureclassimpl.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

static std::map<FCLSID, FeatureClassSharedPtr> * g_fclsmap = 0;//ȫ�ֵ�Ҫ������ID��ϵͳ����ǰҪ����Map������

///////////////////////////////////////////////////////////
FeatureClassSharedPtr FeatureClass::findFeatureClass(FCLSID fclsid){
	//��map�в�����Ӧ��Ҫ�����Ͷ���δ���ҵ�����NULL
	std::map<FCLSID, FeatureClassSharedPtr>::iterator it;
	it = g_fclsmap->find(fclsid);
	if (it != g_fclsmap->end())
	{
		return (*g_fclsmap)[fclsid];
	}
	else
	{
		return FeatureClassSharedPtr();
	}
}

std::map<FCLSID, FeatureClassSharedPtr> & FeatureClass::getFeatureClassMap(){ return *g_fclsmap; }
void  FeatureClass::setFeatureClassMap(std::map<FCLSID, FeatureClassSharedPtr> * p){ g_fclsmap = p; }

struct FeatureClassDeletor{
	void operator()(FeatureClassPtr p){
		delete dynamic_cast<FeatureClassImpl*>(p);
	}
};

FeatureClassSharedPtr FeatureClass::create(){
	return FeatureClassSharedPtr((FeatureClassPtr) new FeatureClassImpl(),
		FeatureClassDeletor());
}
FeatureClassSharedPtr FeatureClass::create(std::string name,std::string code){
	return FeatureClassSharedPtr((FeatureClassPtr) new FeatureClassImpl(name,code),
		FeatureClassDeletor());
}
FeatureClassSharedPtr FeatureClass::create(FCLSID id, std::string name, std::string code){
	return FeatureClassSharedPtr((FeatureClassPtr) new FeatureClassImpl(id,name,code),
		FeatureClassDeletor());
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
