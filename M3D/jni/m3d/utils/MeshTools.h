#ifndef _MESHTOOL_H_
#define _MESHTOOL_H_


#include <map>
#include <set>
#include <vector>
#include <math.h>
#include "m3d/base/MathDefs.h"
namespace MTools{
#define  EPL 1.e-5
#define  EP 1.e-8
#define  EPs 1.e-7


using namespace std;
typedef std::pair<int, int> Edge;

typedef struct _tri {
	int n1;
	int n2;
	int n3;
	bool operator==(const _tri t)const
	{
		return (n1==t.n1) && (n2==t.n2)&&(n3==t.n3);
	}
	bool  operator<(const _tri& t)const
	{
		if ((n1<t.n1)||(!(n1>t.n1)&&(n2<t.n2))||(!(n1>t.n1)&&!(n2>t.n2)&&(n3<t.n3)))
		{
			return true;
		}
		else
			return false;
	}

} Tri;

typedef std::vector<Tri> VECTRI;

typedef struct _vtx
{
	double x;
	double y;
	double z;

	bool operator==(const _vtx v)const
	{
		return (M3D::Abs(x-v.x)<EP) && (M3D::Abs(y-v.y)<EP)&&(M3D::Abs(z-v.z)<EP);
		/*return (x==v.x) && (y==v.y)&&(z==v.z);*/
	}
	//operator </*/+/- add by wuwei
	//2014-5-14
	bool  operator<(const _vtx& v)const
	{
		if ((x<(v.x-EPs))||(!(x>v.x+EPs)&&(y<(v.y-EPs)))||(!(x>v.x+EPs)&&(!(y>v.y+EPs))&&(z<(v.z-EPs))))
		{
			return true;
		}
		else
			return false;
	}

	double operator*(const _vtx v)
	{
		return x*v.x+y*v.y+z*v.z;
	}

	_vtx operator*( double v)
	{
		_vtx r;
		r.x=x*v;
		r.y=y*v;
		r.z=z*v;
		return r;
	}

	_vtx operator+(const _vtx v)
	{	
		_vtx r;
		r.x=x+v.x;
		r.y=y+v.y;
		r.z=z+v.z;
		return r;
	}

	_vtx  operator-(const _vtx v)
	{
		_vtx r;
		r.x=x-v.x;
		r.y=y-v.y;
		r.z=z-v.z;
		return r;
	}

	_vtx cross(const _vtx v)
	{
		_vtx r;
		r.x=y*v.z-z*v.y;
		r.y=z*v.x-x*v.z;
		r.z=x*v.y-y*v.x;
		return r;
	}

	double dot(const _vtx v)
	{
		double ret;
		ret=x*v.x+y*v.y+z*v.z;
		return ret;
	}

	void normalize()
	{
		double lengh =sqrt(x*x+y*y+z*z);
		x = x/lengh;
		y = y/lengh;
		z = z/lengh;
	}
	//operator </*/+/- end
} Vtx;

typedef struct _vtxf
{
	float x;
	float y;
	float z;
} Vtxf;

//struct Plane add by wuwei
//2014-5-14
typedef struct _plane
{
	_vtx M_0;
	_vtx n;
} Plane;
//Plane end 

typedef struct _triVtx
{
	Vtx v0;
	Vtx v1;
	Vtx v2;

	bool  operator==(const _triVtx& tri)const
	{
		return (v0==tri.v0)&&(v1==tri.v1)&&(v2 == tri.v2);
	}

	bool  operator<(const _triVtx& tri)const
	{
		if (v0<tri.v0||((v0==tri.v0)&&(v1<(tri.v1)))||((v0==tri.v0)&&(v1==tri.v1)&&(v2<tri.v2)))
		{
			return true;
		}
		else
			return false;
	}
} TriVtx;


//Ascii码文件读取结构
class M3D_API STLFacet
{
public:
	Vtxf norm;
	Vtxf tri[3];
};

typedef struct _fac {
	int n1;
	int n2;
	int n3;
} Fac;

typedef std::vector<int> LOOP;
typedef std::vector<Vtxf> VNODE;

typedef struct _stlMesh
{
	std::vector<Vtx> vnodes;
	std::vector<int> kelems;
}STLMesh;

class   M3D_API MeshTools
{
public:
	static void FindFreeLoops(const std::vector<Tri>& kelems, std::vector<LOOP>& vecloops);

	static int STLToMesh(const std::vector<Vtx>& vtabs, std::vector<Vtx>& vnodes, std::vector<int>& kelems);
	
	static void CalculateDisToPlane(const std::vector<Vtx>& vnodes, const Plane& plane, std::vector<int>& disFlags );
	static void CalculateCrossPoint(const std::vector<Vtx>& vnodes, const Edge & e, Plane plane,Vtx& crossP,bool& isOnPlane ,bool& isCrossed);
	static void CalculateSections(const std::vector<Vtx>& vnodes, 
		const std::vector<int>& kelems, 
		const Vtx planePoint,const Vtx planeNorm,
		std::vector<Vtx>& crossPoints, 
		std::vector<LOOP>& vecloops,
		std::vector<LOOP>& vecloopsOnPlane);
	static void GetTowTrisCrossLine( Vtx triangle1[3], Vtx triangle2[3],std::vector<Vtx>& vCrossPnt,bool&isParal);
	static bool PointInTriangle(Vtx triangale[3],Vtx point);
	static void CalculateTriWithPlane(Vtx triangale1[3],Plane plane,std::vector<Vtx>& vCrossPnt);
	static void CalculateTriWithPlane(Vtx triangale[3],Plane plane,std::vector<Vtx>& vCrossPnt,bool& isOnPlane);
	static double Compacity(std::vector<Vtx>& vecVtx, std::vector<Fac>& vecTri, double& air, double& vol);	//计算Mesh的面积和体积
	static double Compacity(std::vector<Vtx>& vecVtx,double& air, double& vol);	//计算Mesh的面积和体积
	static int FindIntersection2Inter(double u0,double u1,double v0,double v1,double w[2]);
	static int FindIntersection2Seg(Vtx p0,Vtx p1,Vtx q0,Vtx q1,Vtx iters[2]);
	static void CalculateTrisBoundaryOnPlane( std::vector<Vtx>&triVector1,std::vector<Vtx> &crossPoints);
	static void CalculateTrisBoundaryOnPlane( std::vector<Vtx>&triVector1,std::vector<Vtx> &crossPoints, std::vector<Vtx> &vnodes,std::vector<LOOP> &crossLoops);
	static int SegIntersTri(Vtx tri[3],Vtx point1,Vtx point2,Vtx crossPoint[2]);
	// read stl file add by gaoqq
	static int ReadSTL(char * filename, std::vector<Vtx>& vnodes, std::vector<int>& kelems);
	static bool isIntersTri2OnPlane(Vtx triangale1[3],Vtx triangale2[3]);
	static bool PointInTriangle3dTo2d(TriVtx triangale,Vtx point,double& u ,double& v);
	static bool CheckTriNormal(TriVtx triangale1,TriVtx triangale2);
	static void DivideMeshWithCrossLine(LOOP crossLine, const std::vector<Tri> vKelems, std::vector<VECTRI>& parts);
	static void GetTriEdge(Tri tri, Edge& e1, Edge& e2, Edge& e3);
	static bool PointInPolyhedron(const std::vector<Vtx>& vnodes,const std::vector<Tri>& kelems,Vtx& point);
	static bool RayIntersctTriangle(Vtx triangle[3],Vtx rayStrat,Vtx rayDir, double& t,double& u,double& v);
	static double CaculateMeshVolume(const std::vector<Vtx> vNodes,const std::vector<int>vKelems);
	static void CalculateAirVol(Vtx& v1, Vtx& v2, Vtx& v3, double& S, double& V);
	static void FindSegsLoop(LOOP crossLine,std::vector<LOOP>& vecloops);
	static void MapEdgeWithTri(const std::vector<Tri>& triVect,std::map<Edge,VECTRI> & mapEdgeTri);
	static bool PointIsOnPlane( Vtx& point,  Plane& plane);
	static Vtx& GetNormal(Vtx& v1, Vtx& v2, Vtx& v3);
};
}
#endif 
