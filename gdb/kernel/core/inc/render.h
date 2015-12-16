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
#include "geometry2d.h"
#include "geometry3d.h"
#include "builders.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
/** @defgroup Render Render-��Ⱦ����  
*  @{
*/
class CORE_API GraphicsDevice{
	void *  _handle;
public:
	explicit GraphicsDevice(void * pHandle): _handle(pHandle) { } 
	inline  void* getHandle() const {return _handle;}
	inline void setHandle(void* pHandle) {_handle=pHandle;}
	inline void copy(const GraphicsDevice & g) {_handle = g._handle;} 
	virtual ~GraphicsDevice(){_handle=0;}
};
typedef std::shared_ptr<GraphicsDevice> GraphicsDeviceSharedPtr;
/** @} */ 


/** @addtogroup Render Camera-�����  
*  @{
*/
class CORE_API Camera{
public:	
	virtual void setViewMatrix(const Vertex3d eye,const Vertex3d center, const Vertex3d up)=0;
	virtual void getViewMatrix(Vertex3d &eye,  Vertex3d & center,  Vertex3d & up)=0;
	virtual void setProjectionMatrix(double  FovAngleY,double AspectRatio,double NearZ, double FarZ )=0;
	virtual void getProjectionMatrix(double & FovAngleY,double& AspectRatio,double &NearZ, double &FarZ )=0;
	virtual Matrix4x4SharedPtr getProjectionMatrix()=0;
	virtual void setProjectionMatrix(Matrix4x4SharedPtr)=0;
	virtual void WP2VP(const Vertex3d & w ,Vertex3f& v)=0;
	virtual void VP2WP(const  Vertex3f & v ,Vertex3d & w)=0;
};		  
typedef std::shared_ptr<Camera> CameraSharedPtr;
/** @} */

class Render3d;
class Render2d;
class Render;
typedef std::shared_ptr<Render> RenderSharedPtr;
typedef Render2d* Render2dPtr;
typedef std::shared_ptr<Render2d> Render2dSharedPtr;
typedef Render3d* Render3dPtr;
typedef std::shared_ptr<Render3d> Render3dSharedPtr;

/** @addtogroup Render Render-��Ⱦ������  
*  @{
*/
class  CORE_API Render 
{
protected:
	int               _dimension;
protected:
	GeometrySharedPtr _geometricTrack;
protected:
	BuildersSharedPtr  _builders;
	Vertex3d          _origin;//��Ҫ���ڽ������ݴ����С���ݣ�Ĭ��Ϊ0
	
public:
	virtual void setGeometricTrack(GeometrySharedPtr p) =0;
	inline GeometrySharedPtr getGeometricTrack() {return _geometricTrack;}
	virtual void addNode(FeatureSharedPtr fsp)=0;
public:
	inline void setModelBuilder(BuilderSharedPtr bsp) {_builders->setModelBuilder(bsp);}
	inline BuilderSharedPtr getModelBuilder( ) {return _builders->getModelBuilder();}
	inline void setMeshBuilder(MeshBuilderSharedPtr bsp) {_builders->setMeshBuilder(bsp);}
	inline MeshBuilderSharedPtr getMeshBuilder( ) {return _builders->getMeshBuilder();}
	inline void setTerrianBuilder(TerrainBuilderSharedPtr bsp) {_builders->setTerrainBuilder(bsp);}
	inline TerrainBuilderSharedPtr getTerrainBuilder( ) {return _builders->getTerrainBuilder();}

	inline void setBuilders(BuildersSharedPtr p){_builders = p;}
	inline BuildersSharedPtr getBuilders() {return _builders;}
	
	inline Vertex3d & getOrigin() {return _origin;}
	inline void setOrigin(const Vertex3d & v) {_origin = v;}
	inline double getOriginX() {return _origin.x;}
	inline double getOriginY() {return _origin.y;}
	inline double getOriginZ() {return _origin.z;}

	inline  int getDimension() {return _dimension;}

public:
	virtual bool initial(const GraphicsDevice & gd)=0;
	virtual void cleanup()=0;

	virtual void WP2VP(const Vertex3d & w ,Vertex3f& v)=0;
	virtual void VP2WP(const  Vertex3f & v ,Vertex3d & w, bool bNearest=false)=0;

	virtual void WP2VP(const Vertex2d & w ,Vertex2d & v){}
	virtual void VP2WP(const Vertex2d & v ,Vertex2d & w){}
	virtual void WP2VP(double wpx,double wpy,double& vpx,double& vpy){}
	virtual void VP2WP(double vpx,double vpy,double &wpx,double &wpy){}

	virtual gdb::SelectorRef  select(const gdb::Vertex3f &   mouse_point_in_viewport ) = 0;
	virtual gdb::SelectorRef  select(const gdb::Vertex3f &  mouse_point_left_bottom_in_viewport,
		const gdb::Vertex3f &  mouse_point_right_top_in_viewport) = 0;
	/** ���ѡ��, ���Զ��ָ�ԭ״̬
	* @param [in] ��
	* @return ��
	*/
	virtual void clearSelector()=0;
	/** ǿ�����»��Ƴ���
	* @param [in] dynymic_loading_data  ���ƹ����ж�̬�������ݵ�ģʽ��0Ϊ������
	* @return ��
	*/
	virtual void requestRedraw(int dynymic_loading_data=0)=0;

	Render();

	virtual Render3d * asRender3d();
	virtual Render2d * asRender2d();

};

/** @} */

/** @addtogroup Render Render3d-��Ⱦ������  
*  @{
*/
class  CORE_API Render3d : public Render 
{
public:
	Render3d();
public://�����صĲ���
	virtual void home()=0;
	
		
	virtual void setLookAt(const Vertex3d& eye, const Vertex3d& center,const Vertex3d& up)=0;
	virtual void getLookAt( Vertex3d&eye,  Vertex3d&center, Vertex3d&up)=0;
	virtual CameraSharedPtr  getCamera()=0;
	
	virtual Vertex3d getSceneCenter()=0;
	virtual double getSceneRadius()=0;
	virtual void rotate(const cn::edu::cug::gdb::Quaternion & q)=0;
	virtual void translate(const cn::edu::cug::gdb::Vertex3d & delta)=0;

public:
	virtual void addNode(FeatureSharedPtr fsp)=0;
	virtual void addNode(BlockSharedPtr bsp)=0;
	virtual void setGeometricTrack(GeometrySharedPtr p) =0;
protected:	
	virtual void renderScene()=0;	
public:
	virtual void startRenderingThread() = 0;
};

/** @} */



/** @addtogroup Render  Coordsys2d-����ϵͳ����
*  @{
*/
class Coordsys2d
{
public:

	/** @name WPTOLPת��������
	*  @{
	*/
	enum WptoLpConvertMode
	{
		WPTOLP_ENVELOPE3D_TO_CRECT = 0,
		WPTOLP_ENVELOPE3D_TO_GDIPLUS_RECT ,
		WPTOLP_ENVELOPE3D_TO_GDIPLUS_RECTF ,

		WPTOLP_VERTEX2D_TO_CPOINT ,
		WPTOLP_VERTEX2D_TO_POINT ,
		WPTOLP_VERTEX2D_TO_GDIPLUS_POINT ,
		WPTOLP_VERTEX2D_TO_GDIPLUS_POINTF ,

		WPTOLP_VERTEXCOLLECTION2D_TO_CPOINTS ,
		WPTOLP_VERTEXCOLLECTION2D_TO_POINTS ,
		WPTOLP_VERTEXCOLLECTION2D_TO_GDIPLUS_POINTS ,
		WPTOLP_VERTEXCOLLECTION2D_TO_GDIPLUS_POINTFS
	};
	/** @}*/


	/** @name WPTOLPת��������
	*  @{
	*/
	enum LptoWpConvertMode
	{
		LPTOWP_POINT_TO_VERTEX2D = 0 ,
		LPTOWP_CPOINT_TO_VERTEX2D ,
		LPTOWP_GDIPLUS_POINT_TO_VERTEX2D ,
		LPTOWP_GDIPLUS_POINTF_TO_VERTEX2D ,
		LPTOWP_CRECT_TO_ENVELOPE3D,
	};
	/** @}*/

	/** @name ����ϵͳ����
	*  @{
	*/
	enum  enum_GEO_WORLD_MAP_MODE//���������ͼֽ����֮���ӳ���ϵ
	{
		WMM_NORMAL=0X0000,        //����ʹ�ã�
		WMM_X_AXIS_REVERSE=0X0001,//���Ե���ʹ�ã�Ҳ������WMM_Y_AXIS_REVERSE����ʹ�ã�Ч����ͬ��WMM_XY_AXIS_REVERSE
		WMM_Y_AXIS_REVERSE=0X0002,//���Ե���ʹ�ã�Ҳ������WMM_X_AXIS_REVERSE����ʹ�ã�Ч����ͬ��WMM_XY_AXIS_REVERSE
		WMM_XY_AXIS_REVERSE=0X0003,//����ʹ��
		WMM_AXIS_ROTATE=0X0004     //����ʹ��
	};
	enum enum_GEO_MAP_PROJECTION_TYPE//ͶӰ����
	{
		MPT_UNKNOWN=0,
		MPT_GAUSS=1,
		MPT_ALBERS,
		MPT_LAMBERT,
		MPT_UTM,
		MPT_MAX
	};

	enum enum_GEO_COORDSYS_TYPE//����ϵͳ����
	{
		CST_UNKNOWN=0,
		CST_BEIJING54=1,
		CST_XIAN80,
		CST_WGS84,
		CST_MAX
	};

	enum enum_GEO_ELLIPSIOD_TYPE//����������
	{
		ET_UNKNOWN=0,
		ET_KLSFSJ=1,
		ET_IUGG75,
		ET_WGS84,
		ET_USERDEF,//�û��Զ�������
		ET_MAX
	};
	/** @}*/

public:
	/** �������� ==>  �߼����� 
	* @param [in] wx,wy,��������
	* @param [out] nx,ny,lx,ly,dx,dy:�߼����� 
	* @return 
	*/
	virtual void WP2VP(double wx,double wy,int *nx,   int *ny   ) = 0;
	virtual void WP2VP(double wx,double wy,long *lx,  long *ly  ) = 0;
	virtual void WP2VP(double wx,double wy,float *dx, float *dy ) = 0;
	virtual void WP2VP(double wx,double wy,double *dx,double *dy) = 0;


	/**  ��ȡ��ʾ����
	* @return ��ʾ����
	*/
	virtual double getDisplayScale()=0;

	/**  ��ȡ��ʾ����
	* @param [in] ds ��ʾ����
	* @return 
	*/
	virtual void setDisplayScale(double ds) = 0;

	/** WP2VP����ת���ӿ�
	* @param [in] srcPtr: ���������µ�
	* @param [in]   convertModeģʽ����
	* 				WPTOLP_ENVELOPE3D_TO_CRECT,
	* 				WPTOLP_ENVELOPE3D_TO_GDIPLUS_RECT ,
	* 				WPTOLP_ENVELOPE3D_TO_GDIPLUS_RECTF ,
	* 				WPTOLP_VERTEX2D_TO_CPOINT ,
	* 				WPTOLP_VERTEX2D_TO_POINT ,
	* 				WPTOLP_VERTEX2D_TO_GDIPLUS_POINT ,
	* 				WPTOLP_VERTEX2D_TO_GDIPLUS_POINTF ,
	* 				WPTOLP_VERTEXCOLLECTION2D_TO_CPOINTS ,
	* 				WPTOLP_VERTEXCOLLECTION2D_TO_POINTS ,
	* 				WPTOLP_VERTEXCOLLECTION2D_TO_GDIPLUS_POINTS ,
	* 				WPTOLP_VERTEXCOLLECTION2D_TO_GDIPLUS_POINTFS 
	* @param [out] tgtPtr: �߼������µ�
	* @return 
	*/
	virtual void WP2VP(void* srcPtr, void* tgtPtr, int convertMode) = 0;

	/** �߼����� ==>  �������� 
	* @param [in] x,y,lx,ly:�߼�����
	* @param [out] wx,wy,�������� 
	* @return 
	*/
	virtual void VP2WP(long x,    long y,    double * wx, double *wy) = 0;	
	virtual void VP2WP(double lx, double ly, double * wx, double *wy) = 0;

	/** VP2WP����ת���ӿ�
	* @param [in] srcPtr: �߼������µ�
	* @param [in]   convertModeģʽ����
	* 				LPTOWP_POINT_TO_VERTEX2D = 0 ,
	* 				LPTOWP_CPOINT_TO_VERTEX2D ,
	* 				LPTOWP_GDIPLUS_POINT_TO_VERTEX2D ,
	* 				LPTOWP_GDIPLUS_POINTF_TO_VERTEX2D ,
	* 				LPTOWP_CRECT_TO_ENVELOPE3D,
	* @param [out] tgtPtr: ���������µ�
	* @return 
	*/	
	virtual void VP2WP(void* srcPtr, void* tgtPtr, int convertMode) = 0;

	/** Windows���߼����������糤��ת�� 
	* @param [in] lLen�߼�����
	* @return ���糤��
	*/
	virtual double LLtoWL(double lLen) = 0;	

	/** ���糤����Windows���߼�����ת�� 
	* @param [in] fLen���糤��
	* @return �߼�����
	*/
	virtual double WLtoLL(double fLen) = 0;

	/** ͼֽ���ȣ���λ����ǰ����ϵ��λ���ס��������ף� => �߼����� 
	* @param [in] m ͼֽ����
	* @return �߼�����
	*/
	virtual double mapLengthtoLL(double m) = 0;

	/** �߼����� => ͼֽ���ȣ���λ����ǰ����ϵ��λ���ס��������ף� 
	* @param [in] m �߼�����
	* @return ͼֽ����
	*/
	virtual double LLtoMapLength(double m) = 0;

	/** Windows���߼�����ת���ɺ��׳��� 
	* @param [in] lLen�߼�����
	* @return ���׳���
	*/
	virtual double LLtoMM(double lLen) = 0;

	/** ���׳���ת����Windows���߼����� 
	* @param [in] fLen���׳���
	* @return �߼�����
	*/
	virtual double MMtoLL(double fLen) = 0;

	/** �ڱ�����scale (�����߷�ĸ)�£� mm ����ֽ�ϵ�λ��������Ӧ����������
	* ע�⣺��ת����ͼ�ε���ʾ�����޹� 
	* @param [in] mm����ֽ�ϵ�λ����
	* @return �������곤��
	*/
	virtual double MMtoWL(double mm) = 0;

	/** �ڱ�����dMapScale (�����߷�ĸ)�£� mm ����ֽ�ϵ�λ��������Ӧ���������� 
	* @param [in] mm:����ֽ�ϵ�λ����, dMapScale:������
	* @return �������곤��
	*/
	virtual double MMtoWL(double mm, double dMapScale) = 0;

	/** ����ͼ����ʾ����������mm ����ֽ�ϵ�λ��������Ӧ���������� 
	* @param [in] mm:����ֽ�ϵ�λ����
	* @return �������곤��
	*/
	virtual double MMtoWL_ByDisplayScale(double mm) = 0;

	/** ����ͼ����ʾ�������ڱ�����dMapScale (�����߷�ĸ)�£�mm ����ֽ�ϵ�λ��������Ӧ���������� 
	* @param [in] mm:����ֽ�ϵ�λ����, dMapScale:������
	* @return �������곤��
	*/
	virtual double MMtoWL_ByDisplayScale(double mm,double dMapScale) = 0;

	/** �ڱ�����scale (�����߷�ĸ)�£� �������곤��wl����Ӧ��ֽ�ϵ�λ����(mm ����)
	* ע�⣺��ת����ͼ�ε���ʾ�����޹� 
	* @param [in] wl:�������곤��
	* @return ֽ�ϵ�λ����(mm ����)
	*/
	virtual double WLtoMM(double wl) = 0;

	/** �ڱ�����dMapScale (�����߷�ĸ)�£� �������곤��wl����Ӧ��ֽ�ϵ�λ����(mm ����) 
	* @param [in] wl:�������곤��, dMapScale:������
	* @return ֽ�ϵ�λ����(mm ����)
	*/
	virtual double WLtoMM(double wl, double dMapScale) = 0;

	/** ����ͼ����ʾ�����������������곤��wl����Ӧ��ֽ�ϵ�λ����(mm ����) 
	* @param [in] wl:�������곤��
	* @return ֽ�ϵ�λ����(mm ����)
	*/
	virtual double WLtoMM_ByDisplayScale(double wl) = 0;

	/** ����ͼ����ʾ�������ڱ�����dMapScale (�����߷�ĸ)�£������������곤��wl����Ӧ��ֽ�ϵ�λ����(mm ����)
	* @param [in] wl:�������곤��, dMapScale:������
	* @return ֽ�ϵ�λ����(mm ����)
	*/
	virtual double WLtoMM_ByDisplayScale(double wl,double dMapScale) = 0;

	/** ��ȡϵͳ��λ��Ӧ���������ƣ�0-��,1-����,2-����,3-Ӣ��,4-ǧ�ף�5���ȷ���, 6-�� 
	* @return ϵͳ��λ��Ӧ����������
	*/
	virtual std::string getUnitNameCHS() = 0;

	/** �����ڲ�����ϵ  ==> �豸����ϵ 
	* @param [in] orgX,orgY: ԭ������
	* @param [in] dScaleX,dScaleY: X��Y������ű���
	* @param [in] dAngRad:��ת�Ƕȣ����ȵ�λ��
	* @param [in] apX,apY: �����ڲ�����ϵ����
	* @param [out] dpX, dpY: �豸����ϵ����
	* @return 
	*/
	virtual void APtoDP(double orgX,double orgY, double dScaleX,double dScaleY,
		double dAngRad,double apX,double apY,double &dpX, double &dpY) = 0;

	/** �豸����ϵ  ==> �����ڲ��ڲ�����ϵ 
	* @param [in] orgX,orgY: ԭ������
	* @param [in] dScaleX,dScaleY: X��Y������ű���
	* @param [in] dAngRad:��ת�Ƕȣ����ȵ�λ��
	* @param [in] dpX, dpY: �豸����ϵ����
	* @param [out] apX,apY: ��������ϵ����
	* @return 
	*/
	virtual void DPtoAP(double orgX,double orgY, double dScaleX,double dScaleY,
		double dAngRad,double dpX, double dpY,double &apX,double &apY) = 0;

public:
	/** �߼�����ϵ�ڽ���ƽ��
	* @param [in] dOffX,dOffY: X��Y���ƫ����
	* @return 
	*/
	virtual void translateInVp(double dOffX, double dOffY) = 0;

	/** ��������ϵ�ڽ���ƽ��
	* @param [in] dOffX,dOffY: X��Y���ƫ����
	* @return 
	*/
	virtual void translateInWp(double dOffX, double dOffY) = 0;

	/** ����Ϊ1:1��ʾ 
	* @param [in] dMinX, dMinY,dMaxX,dMaxY: ָ����ʾ��Χ
	* @param [out] 
	* @return 
	*/
	virtual void make11Display(double dMinX, double dMinY,double dMaxX,double dMaxY) = 0;

	/** ��ָ����Ϊ���ģ��ӿ��߼����꣩���Ŵ����Сͼ�Σ�dZoomFactor>1Ϊ�Ŵ�С��1Ϊ��С 
	* @param [in] vpCenterX, vpCenterY: ���ĵ����ꣻ dZoomFactor����������
	* @return 
	*/
	virtual void zoomVp(double vpCenterX, double vpCenterY, double dZoomFactor ) = 0;
	
	/** ��ָ����Ϊ���ģ��Ŵ����Сͼ�Σ�dZoomFactor>1Ϊ�Ŵ�С��1Ϊ��С
	* @param [in] ptCenterX, ptCenterY���������ĵ�
	* @param [in] dZoomFactor����������
	* @return 
	*/
	virtual void zoomWp(double wpCenterX, double wpCenterY, double dZoomFactor ) = 0;

	/** �����η�Χ�ڵ�ͼ����ʾ��View��ͼ��, ���Զ�������Ӧ������ 
	* @param [in] pRect: ���η�Χ
	* @return 
	*/
	virtual void displayWpRangeInCurView(Envelope3d* pRect) = 0;

	/** �����η�Χ�ڵ�ͼ����ʾ��View��ͼ��, ���Զ�������Ӧ������ 
	* @param [in]  dMinX, dMinY,dMaxX,dMaxY: ���η�Χ
	* @return 
	*/
	virtual void displayWpRangeInCurView(double dMinX, double dMinY,double dMaxX,double dMaxY) = 0;

	/** ��ָ����ΧrctBound�ڵ�ͼ����ʾ��ָ�����߼����귶ΧrctLog��
	* @param [in] rctBound: ָ�����������귶Χ
	* @param [in] rctLogX,rctLogY,rctLogCX,rctLogCY: �綨�߼����귶Χ
	* @param [in] visiableWndRectΪָ���Ŀɼ����ھ��Σ��߼����꣩�����Ĵ�СӰ�쵽�䷶Χ�ڵĶ����Ƿ�ɼ�
	* @param [in] visbRctX,visbRctY,visbRctCX,visbRctCY: �綨�ɼ����ڷ�Χ 
	* @return 
	*/
	virtual void displayWpRangeInVpRect(Envelope3d& rctBound,
		double rctLogX, double rctLogY, double rctLogCX, double rctLogCY,
		double visbRctX, double visbRctY, double visbRctCX, double visbRctCY) = 0;

	/** ��ȡ��λ���� 
	* @return ��λ���� 
	*/
	virtual int getUnitType()const = 0;

	/** ���õ�λ���� 
	* @param [in] it ��λ����
	* @return 
	*/
	virtual void setUnitType(int it) = 0;

	/** ����ÿ�׵��߼���λ 
	* @param [in] x,y: X���Y�᷽����ÿ�׵��߼���λ
	* @return 
	*/
	virtual void setLogicalUnitsPtrMeter(double x,double y) = 0;

	/** ��ȡÿ�׵��߼���λ 
	* @param [out] x,y: X���Y�᷽����ÿ�׵��߼���λ
	* @return 
	*/
	virtual void getLogicalUnitsPtrMeter(double &x,double &y) = 0;

	/** ����X��ÿӢ��������� 
	* @param [in] X��ÿӢ���������
	* @return 
	*/
	virtual void setDpiX(double x) = 0;

	/** ����Y��ÿӢ��������� 
	* @param [in] Y��ÿӢ���������
	* @return 
	*/
	virtual void setDpiY(double y) = 0;

	/** ��ȡX��ÿӢ��������� 
	* @return X��ÿӢ���������
	*/
	virtual double getDpiX() = 0;

	/** ��ȡY��ÿӢ��������� 
	* @return Y��ÿӢ��������� 
	*/
	virtual double getDpiY() = 0;

public:
	/** ��ȡ��ͼģʽ 
	* @return ��ͼģʽ
	*/
	virtual int getMapMode() = 0;

	/** ���û�ͼģʽ 
	* @param [in] newMapMode: ��ͼģʽ
	* @return 
	*/
	virtual void setMapMode(int newMapMode) = 0;

	/** ��ȡ���ڵ�λ����Ϣ 
	* @param [out] x,y: ���ڵ���ʼ������
	* @param [out] cx,cy: ���ڵĿ�͸�
	* @return 
	*/
	virtual void getViewport(long &x,long &y,long &cx,long &cy) = 0;

	/** ���ô��ڵ�λ�� 
	* @param [in] x,y: ���ڵ���ʼ������
	* @param [in] cx,cy: ���ڵĿ�͸�
	* @return 
	*/
	virtual void setViewport(long x,long y,long cx,long cy) = 0;

	/** ��ȡ���ڵ���ʼ�� 
	* @param [out] x,y: ���ڵ���ʼ��
	* @return 
	*/
	virtual void getStartOfViewport(long & x,long & y) = 0;

	/** ���ô��ڵ���ʼ�� 
	* @param [in] xNew,yNew: ���ڵ���ʼ��
	* @return 
	*/
	virtual void setStartOfViewport(long xNew,long yNew) = 0;

	/** ��ȡ���ڳߴ��С 
	* @param [out] cx, cy: ���ڵĿ�͸�
	* @return 
	*/
	virtual void  getViewportSize(int &cx, int &cy) = 0;

	/** ���ô��ڳߴ��С 
	* @param [in] cx, cy: ���ڵĿ�͸�
	* @return 
	*/
	virtual void  setViewportSize(int cx, int cy) = 0;

	/** ������������ϵ��� 
	* @param [in] sxNew,syNew: ��������ϵ���
	* @return 
	*/
	virtual void setStartOfWp(double sxNew,double syNew) = 0; 

	/** ��ȡ��������ϵ��� 
	* @param [out] sxNew,syNew: ��������ϵ���
	* @return 
	*/
	virtual void getStartOfWp(double & sxNew,double & syNew) = 0;

	/** ��ȡ��������ϵ���X 
	* @return ��������ϵ���X
	*/
	virtual double getStartOfWpX() = 0;

	/** ��ȡ��������ϵ���Y 
	* @return ��������ϵ���Y 
	*/
	virtual double getStartOfWpY() = 0;

	/** ������ת�Ƕȣ��Ƕȵ�λ�� 
	* @param [in] dAngle:��ת�Ƕȣ��Ƕȵ�λ�� 
	* @return 
	*/
	virtual void   setRotateAngle(double dAngle) = 0; 

	/** ��ȡ��ת�Ƕ� ���Ƕȵ�λ�� 
	* @return ��ת�Ƕ� ���Ƕȵ�λ�� 
	*/
	virtual double getRotateAngle() const  = 0;

	/** ��ȡ�����ͼģʽ 
	* @return �����ͼģʽ
	*/
	virtual unsigned int getWpMapMode()const = 0 ;

	/** ���������ͼģʽ 
	* @param [in] wWorldMapMode:�����ͼģʽ
	* @return 
	*/
	virtual void setWpMapMode(unsigned int  wWorldMapMode) = 0;

};
/** @}*/


/** @addtogroup Render  ��ά��ʾ������
*  @{
*/
enum Render2dType
{
	RENDER_TYPE_2D_GDI_PLUS,	//Gdiplus��ʾ��
	RENDER_TYPE_2D_GDI			//Gdi��ʾ��
};
/** @}*/

/** @addtogroup Render Rect2d-�߼������µľ��η�Χ�ṹ��
*  @{
*/
struct Rect2d
{
	long    left;
	long    top;
	long    right;
	long    bottom;
} ;
/** @}*/


/** @addtogroup Render  ��ά��ʾ�ӿ�
*  @{
*/
class CORE_API Render2d : public Render
{
public:
	Render2d();
public:

	/** @name ��ά���ζ������ʾģʽ
	*  @{
	*/
	enum GeometryDisplayMode2d
	{
		RENDER2D_GEOMETRY_DISPLAY_MODE_NORMAL,		//������ʾ
		RENDER2D_GEOMETRY_DISPLAY_MODE_DEFINE_COLOR	//ָ����ɫ��ʾ
	};
	/** @}*/

	/** @name ��άͼ����ȫ����ʾģʽ
	*  @{
	*/
	enum GV2dGlobalDisplayMode2d
	{
		RENDER2D_GLOBAL_DISPLAY_MODE_REAL,	//������ʾ����ʾ������ʵ�������뻨��
		RENDER2D_GLOBAL_DISPLAY_MODE_EDIT	//�༭ģʽ��ֻ��ʾϵͳ���ͣ���ɫ���
	};
	/** @}*/

	/** @name ��ʾ������
	*  @{
	*/
	enum GV2dGlobalDisplayQuality2d
	{
		RENDER2D_GLOBAL_DISPLAY_QUALITY_LOW,	//��������ʾ���ٶ�����
		RENDER2D_GLOBAL_DISPLAY_QUALITY_HIGH	//��������ʾ����������
	};
	/** @}*/

	/** @name ˢ����ͼʱ�����ݵ��ȵ�ģʽ
	*  @{
	*/
	enum GV2dScheduleDataMode2d
	{
		RENDER2D_SCHEDULE_DATA_NO,					//����������
		RENDER2D_SCHEDULE_DATA_WITH_CHANGED_BOX,	//�Զ����ݵ�ǰ�ӿڣ�ǰ���ӿڣ���������ı仯��Χ��������
		RENDER2D_SCHEDULE_DATA_WITH_CURRENT_BOX		//ֱ�Ӹ��ݵ�ǰ���ӿڣ����е���
	};
	/** @}*/
	
public:	

	/** ���û��Զ���ķ�ʽ�����»��Ƶ�ǰ�ӿ��еļ��ζ���
	* �����ǰ���ӿڷ����仯������ݸı�ľ���,�Զ��õ����������ʾ�Ļ���
	* @param [in] nDispMode-��ʾģʽ��
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_NORMAL,         ������ʾ
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_DEFINE_COLOR    ָ����ɫ��ʾ 
	* @param [in] pColor: �Զ�����ɫ
	* @param [in] drawGeometricTrack: �Ƿ���Ҫ���ƹ켣�ߣ���Ƥ���ߣ�
	* @param [in] scheduleDataMode: ˢ����ͼʱ�����ݵ��ȵ�ģʽ:GV2dScheduleDataMode2d
	* RENDER2D_SCHEDULE_DATA_NO,				//����������
	* RENDER2D_SCHEDULE_DATA_WITH_CHANGED_BOX,	//�Զ����ݵ�ǰ�ӿڣ�ǰ���ӿڣ���������ı仯��Χ��������
	* RENDER2D_SCHEDULE_DATA_WITH_CURRENT_BOX	//ֱ�Ӹ��ݵ�ǰ���ӿڣ����е���
	* @return ��
	*/
	virtual void redrawView(int nDispMode,Color4f* pColor,bool drawGeometricTrack,int scheduleDataMode) = 0;

	/** ���û��Զ���ķ�ʽ�����»��Ƶ�ǰ�ӿ��еļ��ζ���
	* �˺�������ݵ�ǰ���ӿڷ�Χ��ǿ�Ƶ��������¸�����ʾ����
	* @param [in] nDispMode-��ʾģʽ��
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_NORMAL,         ������ʾ
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_DEFINE_COLOR    ָ����ɫ��ʾ 
	* @param [in] pColor: �Զ�����ɫ
	* @param [in] drawGeometricTrack: �Ƿ���Ҫ���ƹ켣�ߣ���Ƥ���ߣ�
	* @return ��
	*/
	//virtual void redrawViewForceRefreshScene(int nDispMode,Color4f* pColor,bool drawGeometricTrack) = 0;
	
	/** �û����λͼ��ˢ�µ�ǰ�ӿڣ����ಿ�֣��ñ���ɫ��䣬�˺���ר��������ʱ��ˢ��
	* @param [in] offX,offY: λͼ���½ǵ������
	* @param [in] bmpWidth,bmpHeight: λͼ�Ŀ�͸�
	* @param [in] drawGeometricTrack: �Ƿ���Ҫ���ƹ켣�ߣ���Ƥ���ߣ�
	* @return 
	*/
	virtual void redrawViewWithCachedBmp(double vpx, double vpy, double vpBmpWidth, double vpBbmpHeight, bool drawGeometricTrack) = 0;
 
	
	/** ���ݵ�ǰ���ں;ɵĴ���λ�ã�����ı�ľ���,���õ����������ʾ�Ļ���
	* @return ��
	*/
	virtual void refreshSceneWithChangedBox() = 0;

	/** ���ݵ�ǰ���ӿڷ�Χ��ǿ�Ƶ��������¸�����ʾ����
	* @return ��
	*/
	virtual void refreshSceneWithCurrentWndBox() = 0;

	/** ���ݴ���ĸ��¾��Σ�ˢ�³����е�����
	* @param [in] changedBoxes:���¾����б�
	* @return ��
	*/
	virtual void refreshScence(std::list<Envelope3d> & changedBoxes) = 0;
	
	/** ������ʾ������
	* @param [in] nQuality:��ʾ����
	* RENDER2D_GLOBAL_DISPLAY_QUALITY_LOW,	//��������ʾ���ٶ�����
	* RENDER2D_GLOBAL_DISPLAY_QUALITY_HIGH	//��������ʾ����������
	* @return ��
	*/
	virtual void setRenderQuality(int nQuality) = 0;

	/** ���¸��ݵ�ǰ�ӿڷ�Χ�����´ӻ����л�ȡ���ݣ�����λ��ͼ��ȫͼ��ʾ
	* @param [in] curViewRange:��ǰ�ӿڷ�Χ
	* @return ��
	*/
	virtual void home(Envelope3d& curViewRange) = 0;

	/** ��������ϵͳ 
	* @param [in] pCoorSys:����ϵͳ
	* @return 
	*/
	virtual void setCoordsys(Coordsys2dSharedPtr& pCoorSys) = 0;

	/** ���ñ�����ɫ 
	* @param [in] cr:������ɫ 
	* @return 
	*/
	virtual void setBackgroundColor(Color4f& cr) = 0;

	/** ��ȡ������ɫ 
	* @param [out] cr:������ɫ 
	* @return 
	*/
	virtual void getBackgroundColor(Color4f& cr) = 0;

	/** ��ȡ������ʾ�ģ������λͼָ��
	* @param [out] gdiplusBitmap:�����λͼָ��,Gdiplus::Bitmap����
	* @return 
	*/
	virtual void getCachedBitmap(void ** gdiplusBitmap) = 0;

	/** ��ȡ����ϵͳ 
	* @return ����ϵͳָ��
	*/
	virtual Coordsys2dSharedPtr& getCoordsys() = 0;

	/** ��ʾ��άͼ�ζ��� 
	* @param [in] pObj:��άͼ�ζ���
	* @param [in] env: �豸����
	* @param [in] nDispMode-��ʾģʽ��
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_NORMAL,         ������ʾ
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_DEFINE_COLOR    ָ����ɫ��ʾ 
	* @param [in] pColor: �Զ�����ɫ
	* @return 
	*/	
	virtual void display2DGeometry(GeometrySharedPtr& pObj,GraphicsDevice& env,int nDispMode,Color4f* pColor) = 0;

	/** ���ô�ӡ״̬ 
	* @param [in] bPrinting: �Ƿ��ڴ�ӡ״̬
	* @return 
	*/
	virtual void setPrintingStatus(bool bPrinting) = 0;

	/** ��ȡ�Ƿ��ڴ�ӡ״̬ 
	* @return �Ƿ��ڴ�ӡ״̬ 
	*/
	virtual bool getPrintingStatus() = 0;

	/** �������������ӷ�ʽ 
	* @param [in] mode:���������ӷ�ʽ
	* @return 
	*/
	virtual void setLineJoinMode(int mode) = 0;

	/** ��ȡ���������ӷ�ʽ 
	* @return ���������ӷ�ʽ
	*/
	virtual int  getLineJoinMode() = 0;

	/** ���� ͼ�ε�ȫ����ʾģʽ 
	* @param [in] mode: ͼ�ε�ȫ����ʾģʽ
	*			  RENDER2D_GLOBAL_DISPLAY_MODE_REAL:��ʾ��ʵ���͡�����
	*			  RENDER2D_GLOBAL_DISPLAY_MODE_EDIT:ͼ�α༭ģʽ������ʾ���͡���仨��
	* @return 
	*/
	virtual void setMapGlobalDisplayMode(int mode) = 0;

	/** ��ȡ������ ͼ�ε�ȫ����ʾģʽ 
	* @return mode: ͼ�ε�ȫ����ʾģʽ
	*   RENDER2D_GLOBAL_DISPLAY_MODE_REAL:��ʾ��ʵ���͡�����
	*   RENDER2D_GLOBAL_DISPLAY_MODE_EDIT:ͼ�α༭ģʽ������ʾ���͡���仨�� 
	*/
	virtual int  getMapGlobalDisplayMode() = 0;

	/** ������������ϵ�£���ǰ���ڵķ�Χ 
	* @return 
	*/
	virtual void calculateWpWndRange() = 0;

	virtual void setGeometricTrack(GeometrySharedPtr p) =0;
	virtual void addNode(FeatureSharedPtr fsp)=0;
};
/** @}*/


/** @addtogroup Render  ��ά������ʾ�ӿ�
*  @{
*/
class GdiplusRenderSymbol2d
{
public:
	/** ��������ϵ  
	* @param [in] pCoordSys:����ϵ
	* @return 
	*/
	virtual void setCoordSys(Coordsys2dSharedPtr& pCoordSys) = 0;

	/**�Ƿ���ʾ����ŵĲ����  
	* @return �Ƿ���ʾ����ŵĲ����
	*/
	virtual bool isDisplaySymPointInsertPt() = 0;

	/**�����Ƿ���ʾ����ŵĲ����  
	* @param [in] b:�Ƿ���ʾ����ŵĲ����
	* @return 
	*/
	virtual void setDisplaySymPointInsertPt(bool b) = 0;

	/**�Ƿ���ʾ��ʵ���߿�  
	* @return �Ƿ���ʾ��ʵ���߿� 
	*/
	virtual bool isDisplayRealLineWidth() = 0;

	/**�����Ƿ���ʾ��ʵ���߿�  
	* @param [in] b �Ƿ���ʾ��ʵ���߿�
	* @return 
	*/
	virtual void setDisplayRealLineWidth(bool b) = 0;

	/**�߿��Ƿ������ʾ�ı���������Ӧ�ķŴ����С 
	* @return �߿��Ƿ������ʾ�ı���������Ӧ�ķŴ����С
	*/
	virtual bool isLineWidthFitToScale() = 0;

	/**�߿��Ƿ������ʾ�ı���������Ӧ�ķŴ����С 
	* @param [in] b�߿��Ƿ������ʾ�ı���������Ӧ�ķŴ����С
	* @return 
	*/
	virtual void setLineWidthFitToScale(bool b) = 0;

	/**�Ƿ��ڴ�ӡ״̬  
	* @return �Ƿ��ڴ�ӡ״̬ 
	*/
	virtual bool isPrinting() = 0;

	/**�����Ƿ��ڴ�ӡ״̬  
	* @param [in] b �Ƿ��ڴ�ӡ״̬
	* @return 
	*/
	virtual void setPrinting(bool b) = 0;

	/**�Ƿ�����ӡ  
	* @return �Ƿ�����ӡ  
	*/
	virtual bool isTransversePrinting() = 0;

	/**�����Ƿ�����ӡ  
	* @param [in] b �Ƿ�����ӡ
	* @return 
	*/
	virtual void setTransversePrinting(bool b) = 0;

	/**��ʾ�����
	* @param [in] pPoint: ����Ŷ���ָ��
	* @param [in] env: �豸����
	* @param [in] nSpecialMode: ��ʾģʽ
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_NORMAL-����������ʾ����ʱpColorΪ��
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_DEFINE_COLOR-��������ɫ��ʾ����ʱpColor����Ϊ��
	* @param [in] pColor - ָ����ɫ
	* @param [in] orgX,orgY - ������豸����
	* @param [in] dZoomFactor - ָ���������ʾ����
	* @param [in] dAngRad - ������ת�Ƕȣ����ȵ�λ��
	* @return 
	*/
	virtual void drawSymbolPoint( Symbol2dSharedPtr& pPoint, GraphicsDevice& env,int nSpecialMode,
		Color4f *pColor, double orgX, double orgY, double dZoomFactor, double dAngRad ) = 0;
	
	/**	������Ż��Ƶ�ָ���ľ���������, rct: �豸����
	* @param [in] pPoint: ����Ŷ���ָ��
	* @param [in] env: �豸����
	* @param [in] nSpecialMode: ��ʾģʽ
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_NORMAL-����������ʾ����ʱpColorΪ��
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_DEFINE_COLOR-��������ɫ��ʾ����ʱpColor����Ϊ��
	* @param [in] pColor - ָ����ɫ
	* @param [in] rct: �豸�����µľ�������
	* @return 
	*/
	virtual void drawSymbolPointInRect( Symbol2dSharedPtr& pPoint,
		GraphicsDevice& env,int nSpecialMode,Color4f *pColor,Rect2d &rct) = 0;
	
	/**��ָ��������,���Ƶ����������ţ����߷��ţ�(�����ڵ���ŵĻ���)
	* @param [in] pRgn: �����ţ����߷��Ŷ���ָ��
	* @param [in] env: �豸����
	* @param [in] nSpecialMode: ��ʾģʽ
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_NORMAL-����������ʾ����ʱpColorΪ��
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_DEFINE_COLOR-��������ɫ��ʾ����ʱpColor����Ϊ��
	* @param [in] pColor - ָ����ɫ
	* @param [in] orgX,orgY - ������豸����
	* @param [in] dZoomFactor - ָ���������ʾ����
	* @param [in] dAngRad - ������ת�Ƕȣ����ȵ�λ��
	* @return 
	*/
	virtual void drawSymbol( Symbol2dSharedPtr& pRgn, GraphicsDevice& env,int nSpecialMode,
		Color4f *pColor, double orgX, double orgY, double dZoomFactor, double dAngRad ) = 0;
	
	/**	���������ţ����߷���, ���Ƶ�ָ���ľ���������(������������), rct: �豸����
	* @param [in] pRgn: �������ţ����߷��Ŷ���ָ��
	* @param [in] env: �豸����
	* @param [in] rct: �豸�����µľ�������
	* @param [in] bKeepHZScale: �Ƿ񱣳ֺ��ݱ��������ΪFALSE�����������
	* @return 
	*/
	virtual void drawSymbolInRect( Symbol2dSharedPtr& pRgn, GraphicsDevice& env, Rect2d &rct,bool bKeepHZScale) = 0;
	
	/**	���ƾ���ptCount����ĵ���pts ��ɵı߽�������, �˺������������߽�, �˴��õ�������ת
	* @param [in] pRgn: �����ţ����߷��Ŷ���ָ��
	* @param [in] env: �豸����
	* @param [in] pts: ����������׵�ַ
	* @param [in] ptCount: ����ĸ���
	* @param [in] pcrBK:���������ɫ���������Ҫ������䣬��pcrBKָ��Ϊ��
	* @param [in] dZoomFactor - ָ���������ʾ����
	* @param [in] dAngDeg-���Ƕȣ���λΪ��
	* @return 
	*/
	virtual void fillPathWithSymbol( Symbol2dSharedPtr& pRgn, GraphicsDevice& env, 
		Vertex2d* pts, long ptCount, Color4f* pcrBK,double dZoomFactor,double dAngDeg ) = 0;
	
	/** ��ʾ��ɷ��ŵ�ԭ�� 
	* @param [in] geom: ���ŵ�ԭ�Ӷ���ָ��
	* @param [in] env: �豸����
	* @param [in] nSpecialMode: ��ʾģʽ
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_NORMAL-����������ʾ����ʱpColorΪ��
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_DEFINE_COLOR-��������ɫ��ʾ����ʱpColor����Ϊ��
	* @param [in] pColor - ָ����ɫ
	* @param [in] orgX,orgY - ������豸����
	* @param [in] dScaleX - ָ�������X����ʾ����
	* @param [in] dScaleY - ָ�������Y����ʾ����
	* @param [in] dAngRad - ������ת�Ƕȣ����ȵ�λ��
	* @return 
	*/
	virtual void drawAtom( GeometrySharedPtr& geom, GraphicsDevice& env,int nSpecialMode,Color4f *pColor,
		double orgX, double orgY, double dScaleX,double dScaleY, double dAngRad) = 0;
};
/** @}*/

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
