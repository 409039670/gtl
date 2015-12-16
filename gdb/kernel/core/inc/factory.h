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
#include "Buffer.h"
#include "material.h"
#include "matrix4x4.h"
#include "describablebuffer.h"
#include "builder.h"
#include "render.h"
#include "geometry2d.h"
#include "image.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
class Factory;

typedef std::shared_ptr<Factory>  FactorySharedPtr;
/** @defgroup Factory  Factory-�๤������ 
*  @{
*/
class GV3DCORE_API Factory{
protected:
	int _type;
public:
	enum {
		FACTORYTYPE_2D_GEOMETRY,//��ά���ο�
		FACTORYTYPE_2D_RENDER,  //��ά��ʾ��
		FACTORYTYPE_3D_GEOMETRY,//��ά���ο�
		FACTORYTYPE_FDB_BUILDER,//�ļ����ݿ�
		FACTORYTYPE_RDB_BUILDER,//��ϵ���ݿ�
		FACTORYTYPE_CDB_BUILDER,//��Ⱥ���ݿ�
		FACTORYTYPE_DDB_BUILDER,//�ֲ�ʽ�������ݿ�
		FACTORYTYPE_3D_RENDER,
		FACTORYTYPE_MESH,//�����
		FACTORYTYPE_FMESH_BUILDER,//�ļ��������ݿ�
		FACTORYTYPE_RMESH_BUILDER,//���ڹ�ϵ���ݿ���������ݿ�
		FACTORYTYPE_CMESH_BUILDER,//���ڼ�Ⱥ���������ݿ�
		FACTORYTYPE_DMESH_BUILDER,//���ڷֲ�ʽ���ݿ���������ݿ�

		FACTORYTYPE_FTERRAIN_BUILDER,//�ļ��������ݿ�
		FACTORYTYPE_RTERRAIN_BUILDER,//���ڹ�ϵ���ݿ�ĵ������ݿ�
		FACTORYTYPE_CTERRAIN_BUILDER,//���ڼ�Ⱥ�ĵ������ݿ�
		FACTORYTYPE_DTERRAIN_BUILDER,//���ڷֲ�ʽ���ݿ�ĵ������ݿ�

		FACTORYTYPE_IMAGE
	};
public:
	Factory();
	virtual ~Factory();
	inline int getType(){return _type;}
public:
	static void setFactory(FactorySharedPtr p);
	static Factory * getFactory(int);
	/*static void*  loadModule(const char *  lib);
	static void  freeModule(void * module);*/
	static std::map<int,FactorySharedPtr> &   getFactoryMap();
};
/** @} */

/** @addtogroup Factory  Geometry2DFactory-2D���ι����� 
*  @{
*/
class GV3DCORE_API Geometry2dFactory: public Factory
{
public:
	Geometry2dFactory() 
	{
		_type = FACTORYTYPE_2D_GEOMETRY;
	}
public:
	/** �������ͣ����� Element2d
	* @param [in] type: Ԫ������
	* @return Element2d����ָ��
	*/	
	virtual  GeometrySharedPtr newGeometry(int signal)=0; 
	/** ����Buffer������ Element2d
	* @param [in] buf: Ԫ�ض���Ļ�����
	* @return Element2d����ָ��
	*/		
	virtual  GeometrySharedPtr newGeometry( Buffer & buf)=0; 
	
	/** �������ͣ����� Element2d
	* @param [in] type: Ԫ������
	* @return Element2d����ָ��
	*/	
	virtual Element2dSharedPtr newElement( int type ) = 0;

	/** ����Buffer������ Element2d
	* @param [in] buf: Ԫ�ض���Ļ�����
	* @return Element2d����ָ��
	*/	
	virtual Element2dSharedPtr newElement( Buffer & buf) = 0;

	/** ���� ElementCollection2d
	* @return ElementCollection2dָ��
	*/	
	virtual ElementCollection2dSharedPtr newElementCollection( ) = 0;

	/** ���� GeometryCollection2d
	* @return GeometryCollection2dָ��
	*/	
	virtual GeometryCollection2dSharedPtr newGeometryCollection( ) = 0;

	/** ���� Pen2d
	* @return Pen2dָ��
	*/	
	virtual Pen2dSharedPtr newPen( ) = 0;

	/** ���� Symbol2d */	
	virtual Symbol2dSharedPtr newSymbol( double width, double height ) = 0;

	/** ���� SymbolLibrary2d */	
	virtual SymbolLibrary2dSharedPtr newSymbolLibrary( ) = 0;

	/** ���� SymbolManager2d */	
	virtual SymbolManager2dSharedPtr newSymbolManager( int nType ) = 0;

	/** ���� GeometrySelectManager2d */	
	virtual GeometrySelectManager2dSharedPtr newGeometrySelectManager2d() = 0;

	/** �������� Ray */	
	virtual RaySharedPtr  newRay(const Vertex3d& o, const Vertex3d & d) = 0;

	/** ����׵�� Frustum */	
	virtual FrustumSharedPtr  newFrustum() = 0;

	/** ����·�� */	
	virtual Path2dSharedPtr  newPath() = 0;
	/** ����·������ */	
	virtual PathCollection2dSharedPtr  newPathCollection() = 0;
	/** ������ */	
	virtual Ring2dSharedPtr  newRing() = 0;
	/** ���������� */	
	virtual RingCollection2dSharedPtr  newRinghCollection() = 0;
};
/** @} */



/** @addtogroup Factory  Render2dFactory-2D��ʾ������ 
*  @{
*/
class GV3DCORE_API Render2dFactory: public cn::edu::cug::gdb::Factory
{
public:
	Render2dFactory()
	{
		_type = FACTORYTYPE_2D_RENDER;
	}

public:
	/** ���� Render2d
	* @param [in] type: Ԫ������
	* @param [out] 
	* @return Render2d����ָ��
	*/	
	virtual  Render2dSharedPtr newRender(int signal)=0; 
	
	
	/** ���� Coordsys2d
	* @param [in] type: Ԫ������
	* @param [out] 
	* @return Coordsys2d����ָ��
	*/	
	virtual  Coordsys2dSharedPtr newCoordsys()=0; 
};
/** @} */



/** @addtogroup Factory  Geometry3dFactory-3D���ι����� 
*  @{
*/
class GV3DCORE_API Geometry3dFactory: public Factory{
public:
	Geometry3dFactory(){
		_type = FACTORYTYPE_3D_GEOMETRY;
	}
public:
	virtual  MaterialSharedPtr newSimpleMaterial()=0;
	virtual  MaterialSharedPtr newComplexMaterial()=0;
	virtual  TextureSharedPtr  newTexture()=0;
	virtual  SharedModelSharedPtr newSharedModel()=0;
	virtual  GeometrySharedPtr newGeometry(int signal)=0; 
	virtual  GeometrySharedPtr newGeometry( Buffer & buf)=0; 
	virtual  VertexVisitorSharedPtr newVertexVisitor(int signal)=0;
	virtual  RaySharedPtr  newRay(const Vertex3d& o, const Vertex3d & d)=0;
	virtual  FrustumSharedPtr  newFrustum()=0;
	virtual DEMGridSharedPtr newDEMGrid()=0;
	virtual DOMGridSharedPtr newDOMGrid()=0;
};
/** @} */



/** @addtogroup Factory  ImageFactory-ͼ�񹤳��� 
*  @{
*/
class GV3DCORE_API ImageFactory: public Factory{
public:
	ImageFactory(){
		_type = FACTORYTYPE_IMAGE;
	}
public:
	 
	virtual ImageSharedPtr newImage()=0;
	virtual ImageSharedPtr newImage(unsigned w, unsigned h, unsigned bpp, void * pdata)=0;
};
/** @} */



/** @addtogroup Factory  FDBBuilderFactory-�ļ����ݿ⹤���� 
*  @{
*/
class GV3DCORE_API FDBBuilderFactory: public Factory{
public:
	FDBBuilderFactory(){
		_type = FACTORYTYPE_FDB_BUILDER;
	}
public:
	virtual  BuilderSharedPtr newBuilder()=0;
};
/** @} */



/** @addtogroup Factory  RDBBuilderFactory-��ϵ���ݿ⹤���� 
*  @{
*/
class GV3DCORE_API RDBBuilderFactory: public Factory{
public:
	RDBBuilderFactory(){
		_type = FACTORYTYPE_RDB_BUILDER;
	}
public:
	virtual  BuilderSharedPtr newBuilder()=0;
};
/** @} */



/** @addtogroup Factory  CDBBuilderFactory-��Ⱥ���ݿ⹤���� 
*  @{
*/
class GV3DCORE_API CDBBuilderFactory: public Factory{//��Ⱥ���ݿ�
public:
	CDBBuilderFactory(){
		_type = FACTORYTYPE_CDB_BUILDER;
	}
public:
	virtual  BuilderSharedPtr newBuilder()=0;
};
/** @} */



/** @addtogroup Factory  CDBBuilderFactory-�ֲ�ʽ���ݿ⹤���� 
*  @{
*/
class GV3DCORE_API DDBBuilderFactory: public Factory{ 
public:
	DDBBuilderFactory(){
		_type = FACTORYTYPE_DDB_BUILDER;
	}
public:
	virtual  BuilderSharedPtr  newBuilder()=0;
};
/** @} */



/** @addtogroup Factory  Render3dFactory-3D��Ⱦ������ 
*  @{
*/
class GV3DCORE_API Render3dFactory : public gdb::Factory {
public:
	Render3dFactory(){
		_type = FACTORYTYPE_3D_RENDER;
	}
	virtual  RenderSharedPtr newRender()=0;

};
/** @} */
typedef std::shared_ptr<Render3dFactory> Render3dFactorySharedPtr;


/** @addtogroup Factory  MeshFactory-���񹤳��� 
*  @{
*/
class GV3DCORE_API MeshFactory : public gdb::Factory {
public:
	MeshFactory(){
		_type = FACTORYTYPE_MESH;
	}
	virtual  MeshSharedPtr newMesh(int signal)=0;
};
/** @} */
typedef std::shared_ptr<MeshFactory> MeshFactorySharedPtr;

/** @addtogroup Factory  FMeshBuilderFactory-�ļ�������⹤���� 
*  @{
*/
class GV3DCORE_API FMeshBuilderFactory: public Factory{
public:
	FMeshBuilderFactory(){
		_type = FACTORYTYPE_FMESH_BUILDER;
	}
public:
	virtual  MeshBuilderSharedPtr newBuilder()=0;
};
/** @} */
/** @addtogroup Factory  RMeshBuilderFactory-��ϵ���ݿ������⹤���� 
*  @{
*/
class GV3DCORE_API RMeshBuilderFactory: public Factory{
public:
	RMeshBuilderFactory(){
		_type = FACTORYTYPE_RMESH_BUILDER;
	}
public:
	virtual  MeshBuilderSharedPtr newBuilder()=0;
};
/** @} */
/** @addtogroup Factory  CMeshBuilderFactory-��Ⱥ������⹤���� 
*  @{
*/
class GV3DCORE_API CMeshBuilderFactory: public Factory{
public:
	CMeshBuilderFactory(){
		_type = FACTORYTYPE_CMESH_BUILDER;
	}
public:
	virtual  MeshBuilderSharedPtr newBuilder()=0;
};
/** @} */
/** @addtogroup Factory  DMeshBuilderFactory-�ֲ�ʽ������⹤���� 
*  @{
*/
class GV3DCORE_API DMeshBuilderFactory: public Factory{//��Ⱥ���ݿ�
public:
	DMeshBuilderFactory(){
		_type = FACTORYTYPE_DMESH_BUILDER;
	}
public:
	virtual  MeshBuilderSharedPtr  newBuilder()=0;
};
/** @} */

/** @addtogroup Factory  FTerrainBuilderFactory-�ļ�����ο⹤���� 
*  @{
*/
class GV3DCORE_API FTerrainBuilderFactory: public Factory{
public:
	FTerrainBuilderFactory(){
		_type = FACTORYTYPE_FTERRAIN_BUILDER;
	}
public:
	virtual  TerrainBuilderSharedPtr newBuilder()=0;
};
/** @} */
/** @addtogroup Factory  RTerrainBuilderFactory-��ϵ���ݿ����ο⹤���� 
*  @{
*/
class GV3DCORE_API RTerrainBuilderFactory: public Factory{
public:
	RTerrainBuilderFactory(){
		_type = FACTORYTYPE_RTERRAIN_BUILDER;
	}
public:
	virtual  TerrainBuilderSharedPtr newBuilder()=0;
};
/** @} */
/** @addtogroup Factory  CTerrainBuilderFactory-��Ⱥ����ο⹤���� 
*  @{
*/
class GV3DCORE_API CTerrainBuilderFactory: public Factory{
public:
	CTerrainBuilderFactory(){
		_type = FACTORYTYPE_CTERRAIN_BUILDER;
	}
public:
	virtual  TerrainBuilderSharedPtr newBuilder()=0;
};
/** @} */
/** @addtogroup Factory  DTerrainBuilderFactory-�ֲ�ʽ����ο⹤���� 
*  @{
*/
class GV3DCORE_API DTerrainBuilderFactory: public Factory{
public:
	DTerrainBuilderFactory(){
		_type = FACTORYTYPE_DTERRAIN_BUILDER;
	}
public:
	virtual  TerrainBuilderSharedPtr  newBuilder()=0;
};
/** @} */
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
