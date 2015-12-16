#include "material.h"
#include "factory.h"
#include "internal_ComplexMaterialImpl.h"
#include "internal_SimpleMaterialImpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

///////////////////////////////////////////////////////////////
MaterialSharedPtr Material::create(int type){
#if (USING_DEFAULT_3DGEOMETRY==0)
	if (type == 0)
		return  ((Geometry3dFactory*)Factory::getFactory(Factory::FACTORYTYPE_3D_GEOMETRY))->newSimpleMaterial();
	else
		return  ((Geometry3dFactory*)Factory::getFactory(Factory::FACTORYTYPE_3D_GEOMETRY))->newComplexMaterial();
#else
	if (type == 0)
		return  MaterialSharedPtr(new SimpleMaterialImpl());
	else
		return  MaterialSharedPtr(new ComplexMaterialImpl());
#endif
	
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
