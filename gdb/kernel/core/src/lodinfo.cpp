#include "feature.h"
#include "internal_loadinfoimpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

LODInfoSharedPtr LODInfo::create(){
	return LODInfoSharedPtr(new LODInfoImpl());
}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
