#pragma once
#include "frustum.h"
#include "quaternion.h"


begin_gtl_namespace
begin_gdb_namespace
class  FrustumImpl : public Frustum
{
private:
	enum {
		TOP = 0,
		BOTTOM,
		LEFT,
		RIGHT,
		NEARP,
		FARP
	};

public:
	FrustumImpl();
	~FrustumImpl(void);

	FrustumImpl(const FrustumImpl & f);

	virtual void set(Vertex3d& eye, Vertex3d& cen, double nearD, double farD, double angle, double ratio);
	virtual void setCamera(Vertex3d& eye, Vertex3d& cen);
	virtual void setCamInternals(double nearD, double farD, double angle, double ratio);
	virtual FrustumSharedPtr clone();
public:
	PLANE3D plane[6];

	Vertex3d vPlaneNor[6];
	int vPlaneNorSign[6][3];

	Vertex3d vPlaneVertices[8];

	double nearD, farD, ratio, angle, tang;
	double nw, nh, fw, fh;

	double radius; // �����뾶
	Vertex3d sphCen; // ���������

	// ò����Uncapped Cone
	Vertex3d coneCen; // ���Բ׶����
	double coneFov; // ���Բ׶�Ƕ� ����
	Vertex3d coneDir; // ���Բ׶���߷���
};



end_gdb_namespace
end_gtl_namespace



