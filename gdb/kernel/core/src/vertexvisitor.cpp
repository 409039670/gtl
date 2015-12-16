#include "config.h"
#include "vertexvisitor.h"
#include "factory.h"
#include "internal_Vertex3dVisitorImpl.h"
#include "internal_Vertex3fVisitorImpl.h"
#include "internal_TexturedVertex3dVisitorImpl.h"
#include "internal_TexturedVertex3fVisitorImpl.h"
#include "internal_ShadedVertex3dVisitorImpl.h"
#include "internal_ShadedVertex3fVisitorImpl.h"
#include "internal_VersatileVertex3dVisitorImpl.h"
#include "internal_VersatileVertex3fVisitorImpl.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
class VertexVisitorDeletor;
struct VertexVisitorDeletor{
	void operator()(VertexVisitor* p){
		switch (p->getType()){
			case VERTEXTYPE_V3d:{
				delete (Vertex3dVisitorImpl*)p;
				break;
			}
			case VERTEXTYPE_V3f:{
				delete (Vertex3fVisitorImpl*)p;
				break;
			}
			case VERTEXTYPE_V3dT2d:{
				delete (TexturedVertex3dVisitorImpl*)p;
				break;
			}
			case VERTEXTYPE_V3fT2f:{
				delete (TexturedVertex3fVisitorImpl*)p;
				break;
			}
			case VERTEXTYPE_V3dN3dC3f:{
				delete (ShadedVertex3dVisitorImpl*)p;
				break;
			}
			case VERTEXTYPE_V3fN3fC3f:{
				delete (ShadedVertex3fVisitorImpl*)p;
				break;
			}
			case VERTEXTYPE_V3fT2fN3fC3f:{
				delete (VersatileVertex3fVisitorImpl*)p;
				break;
			}
			case VERTEXTYPE_V3dT2dN3dC3f:{
				delete (VersatileVertex3dVisitorImpl*)p;
				break;
			}
		}
	}
};

VertexVisitorSharedPtr VertexVisitor::create(int signal)
{
#if(USING_DEFAULT_3DGEOMETRY==1)
	switch (signal){
	case VERTEXTYPE_V3d:
		return VertexVisitorSharedPtr(
			(VertexVisitorPtr) new Vertex3dVisitorImpl(),
			VertexVisitorDeletor());
	case VERTEXTYPE_V3f:
		return VertexVisitorSharedPtr(
			(VertexVisitorPtr) new Vertex3fVisitorImpl(),
			VertexVisitorDeletor());
	case VERTEXTYPE_V3dT2d:
		return VertexVisitorSharedPtr(
			(VertexVisitorPtr) new TexturedVertex3dVisitorImpl(),
			VertexVisitorDeletor());
	case VERTEXTYPE_V3fT2f:
		return VertexVisitorSharedPtr(
			(VertexVisitorPtr) new TexturedVertex3fVisitorImpl(),
			VertexVisitorDeletor());
	case VERTEXTYPE_V3dN3dC3f:
		return VertexVisitorSharedPtr(
			(VertexVisitorPtr) new ShadedVertex3dVisitorImpl(),
			VertexVisitorDeletor());
	case VERTEXTYPE_V3fN3fC3f:
		return VertexVisitorSharedPtr(
			(VertexVisitorPtr) new ShadedVertex3fVisitorImpl(),
			VertexVisitorDeletor());
	case VERTEXTYPE_V3fT2fN3fC3f:
		return VertexVisitorSharedPtr(
			(VertexVisitorPtr) new VersatileVertex3fVisitorImpl(),
			VertexVisitorDeletor());
	case VERTEXTYPE_V3dT2dN3dC3f:
		return VertexVisitorSharedPtr(
			(VertexVisitorPtr) new VersatileVertex3dVisitorImpl(),
			VertexVisitorDeletor());
	}
	return VertexVisitorSharedPtr();
#else
	VertexVisitorSharedPtr p = ((Geometry3dFactory*)Factory::getFactory(Factory::FACTORYTYPE_3D_GEOMETRY))->newVertexVisitor(signal);
	return p;
#endif

}
//////////////////////////////////////////////////////////////////////////
VertexVisitor::VertexVisitor(){
	_vertexlist = 0;
	_vertexnumber=0;
	_vertextype = 0;
}
VertexVisitor::VertexVisitor(void * &vl, long &vn, unsigned char &vt){
	_vertexlist = &vl;
	_vertexnumber=&vn;
	_vertextype = &vt;
}


int VertexVisitor::releaseVertexArray(unsigned char iVertexType, void*& pVertexPointer)
{
	if (pVertexPointer==0)
		return 0;

	switch (iVertexType)
	{
	case VERTEXTYPE_V3f:	// for Vertex3f
	{
		delete[]((Vertex3f*)pVertexPointer);
		break;
	}
	case VERTEXTYPE_V3d:				// for Vertex3d
	{
		delete[]((Vertex3d*)pVertexPointer);
		break;
	}
	case VERTEXTYPE_V3fT2f:			// for TexturedVertex3f
	{
		delete[]((TexturedVertex3f*)pVertexPointer);
		break;
	}
	case VERTEXTYPE_V3dT2d:			// for TexturedVertex3d
	{
		delete[]((TexturedVertex3d*)pVertexPointer);
		break;
	}
	case VERTEXTYPE_V3fN3fC3f:		// for ShadedVertex3f
	{
		delete[]((ShadedVertex3f*)pVertexPointer);
		break;
	}
	case VERTEXTYPE_V3dN3dC3f:		// for ShadedVertex3d
	{
		delete[]((ShadedVertex3d*)pVertexPointer);
		break;
	}
	case VERTEXTYPE_V3fT2fN3fC3f:	// for VersatileVertex3f
	{
		delete[]((VersatileVertex3f*)pVertexPointer);
		break;
	}
	case VERTEXTYPE_V3dT2dN3dC3f:	// for VersatileVertex3d
	{
		delete[]((VersatileVertex3d*)pVertexPointer);
		break;
	}
	default:
	{
		assert(0);
		return -1;
	}
	}

	pVertexPointer = 0;
	return 0;
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace