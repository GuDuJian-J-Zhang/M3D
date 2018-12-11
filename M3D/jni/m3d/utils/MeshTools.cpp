//#include "stdafx.h"
#include "m3d/utils/MeshTools.h"
#include <stdlib.h>
#include <sstream>

namespace MTools{
typedef std::pair<int, int> Edge;
typedef std::vector<Edge> VECEDGE;


typedef std::vector<int> VectorOfVertex;
typedef std::vector<int> VectorOfFace;
typedef std::vector<VectorOfVertex> VecOfVectorOfVertex;
typedef std::vector<VecOfVectorOfVertex> BiVecOfVectorOfVertex;



void MeshTools::FindFreeLoops(const std::vector<Tri>& kelems, std::vector<LOOP>& vecloops)
{
	// find free edges
	std::set<Edge> freeEdges;
	std::set<Edge>::iterator it;

	int ii=0;
	for (int i=0; i<static_cast<int>(kelems.size()); i++) {
		int n1 = kelems[i].n1;
		int n2 = kelems[i].n2;
		int n3 = kelems[i].n3;;

		Edge e;
		if (n1 < n2) {
			e = Edge(n1, n2);
		}
		else {
			e = Edge(n2, n1);
		}
		it = freeEdges.find(e);
		if (it != freeEdges.end()) {
			freeEdges.erase(it); 
		}
		else {
			freeEdges.insert(e);
		}

		if (n2 < n3) {
			e = Edge(n2, n3);
		}
		else {
			e = Edge(n3, n2);
		}
		it = freeEdges.find(e);
		if (it != freeEdges.end()) {
			freeEdges.erase(it); 
		}
		else {
			freeEdges.insert(e);
		}

		if (n3 < n1) {
			e = Edge(n3, n1);
		}
		else {
			e = Edge(n1, n3);
		}
		it = freeEdges.find(e);
		if (it != freeEdges.end()) {
			freeEdges.erase(it); 
		}
		else {
			freeEdges.insert(e);
		}
	}

	// construct an auxiliary map
	std::map<int, VECEDGE> mapNodeEdges;
	for (it = freeEdges.begin(); it != freeEdges.end(); it++) {
		Edge e = *it;
		int n1 = e.first;
		int n2 = e.second;

		if (mapNodeEdges.find(n1) == mapNodeEdges.end()) {
			VECEDGE vec;
			vec.push_back(e);
			mapNodeEdges.insert(std::map<int, VECEDGE>::value_type(n1, vec));
		}
		else {
			VECEDGE& vec = mapNodeEdges[n1];
			vec.push_back(e);
		}

		if (mapNodeEdges.find(n2) == mapNodeEdges.end()) {
			VECEDGE vec;
			vec.push_back(e);
			mapNodeEdges.insert(std::map<int, VECEDGE>::value_type(n2, vec));
		}
		else {
			VECEDGE& vec = mapNodeEdges[n2];
			vec.push_back(e);
		}
	}

	// find free loops

	while (freeEdges.size() > 0) {
		it = freeEdges.begin();
		Edge e1 = *it;
		freeEdges.erase(it);


		int n1 = e1.first;
		int n2 = e1.second;
	
		LOOP loop;
		int n0 = n1;
		loop.push_back(n1);
		loop.push_back(n2);

		Edge e2;
		VECEDGE& vec = mapNodeEdges[n2];
		while (vec.size() >= 2 && n2 != n0) {
		
			if (vec[0] == e1) {
				e2 = vec[1];
			}
			else {
				e2 = vec[0];
			}
			
		/*	it = freeEdges.find(e2);
			freeEdges.erase(it);*/
			it= freeEdges.find(e2);
			if (it!=freeEdges.end())
			{
				freeEdges.erase(it);
			}
			else 
				break;

			if (e2.first == n2) {
				n2 = e2.second;
			}
			else {
				n2 = e2.first;
			}
			loop.push_back(n2);
			vec = mapNodeEdges[n2];
			e1 = e2;
		}	

		vecloops.push_back(loop);
	}
}

inline double squareDistance(const Vtx& v1, const Vtx& v2)
{
	return ((v2.x-v1.x)*(v2.x-v1.x) +(v2.y-v1.y)*(v2.y-v1.y) + (v2.z-v1.z)*(v2.z-v1.z));
}

int MeshTools::STLToMesh(const std::vector<Vtx>& vtabs, std::vector<Vtx>& vnodes, std::vector<int>& kelems)
{
	if ((static_cast<int>(vtabs.size()) < 3) | (static_cast<int>(vtabs.size())%3 != 0)) {
		return -2;
	}

	std::map<int, int> mapOldNew;
	for (int i=0; i<static_cast<int>(vtabs.size()); i++) {
		mapOldNew.insert(std::map<int, int>::value_type(i, -1));
	}

	double epsilon = 1.e-10;

	if (0) {
		// old method

		for (int i=0; i<static_cast<int>(vtabs.size())-1; i++) {
			if (mapOldNew[i] != -1) continue;

			Vtx vi = vtabs[i];
			for (int j=i+1; j<static_cast<int>(vtabs.size()); j++) {
				if (mapOldNew[j] != -1) continue;

				Vtx vj = vtabs[j];
				if (squareDistance(vi, vj) < epsilon) {
					mapOldNew[j] = i;
				}
			}
		}
	}
	else {
		// repartion of points

		double dmin[3] = {1.e10};
		double dmax[3] = {-1.e10};

		for (int i=0; i<static_cast<int>(vtabs.size()); i++) {
			Vtx v = vtabs[i];

			//Dill修改std::min及std::max的使用方式
			dmin[0] = std::min<double>(dmin[0], v.x); 
			dmax[0] = std::max<double>(dmax[0], v.x);
			dmin[1] = std::min<double>(dmin[1], v.y);
			dmax[1] = std::max<double>(dmax[1], v.y);
			dmin[2] = std::min<double>(dmin[2], v.z);
			dmax[2] = std::max<double>(dmax[2], v.z);
		}

		double dx = dmax[0] - dmin[0];
		double dy = dmax[1] - dmin[1];
		double dz = dmax[2] - dmin[2];

		int N1, N2;
		if (dx <= dy && dx <= dz) {
			N1 = 1;
			N2 = 2;
		}
		else if (dy <= dz && dy <= dx) {
			N1 = 2;
			N2 = 0;
		}
		else if (dz <= dx && dz <= dy) {
			N1 = 0;
			N2 = 1;
		}

		// repartion of points
		int NN = (int)(sqrt(double(vtabs.size()))/3);
		int NN1 = NN;
		int NN2 = NN;

		double D1 = (dmax[N1] - dmin[N1])/NN1;
		if (D1 < 1.e-05) {
			D1 = 1.;
		}
		double D2 = (dmax[N2] - dmin[N2])/NN2;
		if (D2 < 1.e-05) {
			D2 = 1.;
		}

		BiVecOfVectorOfVertex BiVecVertices;
		for (int i=0; i<=NN1; i++) {
			VecOfVectorOfVertex vecvecV;
			for (int j=0; j<=NN2; j++) {
				VectorOfVertex vecV;
				vecvecV.push_back(vecV);
			}
			BiVecVertices.push_back(vecvecV);
		}

		double xyz[3];
		for (int i=0; i<static_cast<int>(vtabs.size()); i++) {
			Vtx v = vtabs[i];

			xyz[0] = v.x;
			xyz[1] = v.y;
			xyz[2] = v.z;

			int i1 =(int)((xyz[N1] - dmin[N1])/D1);
			int i2 =(int)((xyz[N2] - dmin[N2])/D2);

			VectorOfVertex& VecV = BiVecVertices[i1][i2];
			VecV.push_back(i);
		}

		for (int i1=0; i1<= NN1; i1++) {
			for (int i2=0; i2<= NN2; i2++) {
				VectorOfVertex& VecV = BiVecVertices[i1][i2];

				int LL = VecV.size();

				for (int i=0; i<LL-1; i++) {
					int n1 = VecV[i];
					if (mapOldNew[n1] == -1) {
						Vtx vi = vtabs[n1];

						for (int j=i+1; j<LL; j++) {
							int n2 = VecV[j];
							if (mapOldNew[n2] == -1) {
								Vtx vj = vtabs[n2];
								if (squareDistance(vi, vj) < epsilon) {
									mapOldNew[n2] = n1;
								}
							}
						}
					}
				}
			}
		}
	}

	// return results

	std::map<int, int> mapOldNode;
	int ii = 0;
	for (int i=0; i<static_cast<int>(vtabs.size()); i++) {
		int nn;
		if (mapOldNew[i] == -1)  {
			nn = ii;
			ii++;
			Vtx v = vtabs[i];
			vnodes.push_back(v);
			mapOldNode.insert(std::map<int,int>::value_type(i, nn));
		}
		else {
			nn = mapOldNew[i]; // by order
			if (mapOldNode.find(nn) == mapOldNode.end()) {
				// impossible
				return -1;
			}
			nn = mapOldNode[nn];
		}

		kelems.push_back(nn);
	}

	return 0;
}

/**********************************************************************
*函数名称：CalculateDisToPlane(const std::vector<Vtx>, const Plane, std::vector<int>)
*函数功能：计算点与切面的关系，在切面上返回0，
		在切面正面（法向侧）返回1，在切面背面返回-1
*参数说明：vnodes   点坐标
				plane	切面
*返回值：	disFlags 输出标记点
 *Created By Wuwei 2014/5/14
/**********************************************************************/
void MeshTools::CalculateDisToPlane(const std::vector<Vtx>& vnodes, const Plane& plane, std::vector<int>& disFlags )
{
	std::vector<Vtx>::const_iterator it;
	Vtx a;
	int flag;
	for ( it=vnodes.begin();it!=vnodes.end();it++) 
	{
		a.x = (plane.M_0.x-it->x);
		a.y = (plane.M_0.y-it->y);
		a.z = (plane.M_0.z-it->z);
		double er = a*plane.n;
		if (er>EP)
		{
			flag = 1;
		}
		else if (er>-EP&&er<EP)
		{
			flag = 0;
		}
		else
		{
			flag = -1;
		}
		disFlags.push_back(flag);		
	}
}


bool MeshTools::PointIsOnPlane( Vtx& point,  Plane& plane)
{
	bool isOn(false);
	Vtx tempVtx;
	tempVtx = plane.M_0 - point;
	double er = tempVtx.dot(plane.n);
	if (er>-EP&&er<EP)
	{
		isOn = true;
	}
	return isOn;
}

/**********************************************************************
*函数名称：CalculateSections(const std::vector<Vtx>, const std::vector<int>，
const Vtx ，const Vtx，std::vector<Vtx>，std::vector<LOOP>，std::vector<LOOP>)
*函数功能：计算切面与Mesh的交线
*参数说明：vnodes   点坐标
		   kelems Mesh的索引
		   planePoint	切面上的一点
		   planeNorm	切面法向量
*返回值：  crossPoints 输出交点坐标
			vecloops   交线的索引
			vecloopsOnPlane 在切面上的面片的边界索引
 *Create By Wuwei 2014/5/14
/**********************************************************************/
void MeshTools::CalculateSections(const std::vector<Vtx>& vnodes,
							  const std::vector<int>& kelems,
							  const Vtx planePoint,
							  const Vtx planeNorm,
							  std::vector<Vtx>& crossPoints,
							  std::vector<LOOP>& vecloops,
							  std::vector<LOOP>& vecloopsOnPlane)
{
	Plane plane;
	plane.M_0 = planePoint;
	plane.n = planeNorm;

	std::vector<int>disFlags;

	CalculateDisToPlane(vnodes,plane,disFlags);

	std::vector<int>::const_iterator it;
	std::map<Vtx,int> mapIndexPoints;
	std::map<Edge,int> mapEdgesCrossPoints;
	int i =-1;
	std::map<Vtx,int>::iterator itMap;
	std::set<Edge> lineEdges;
	std::vector<Tri> kelemsOnPlane;
	Tri tri;
	for (int j =0;j<static_cast<int>(kelems.size())-2;j++) 
	{
		int n1 = kelems.at(j);
		int n2 = kelems.at(++j);
		int n3 = kelems.at(++j);
	/*	int c1= disFlags[n1];
		int c2= disFlags[n2];
		int c3= disFlags[n3];*/
		int crossType =disFlags[n1]+disFlags[n2]+disFlags[n3];
		int crossType1 =disFlags[n1]*disFlags[n2]*disFlags[n3];
		Edge e1,e2,e3;
		Edge newEdge;
		newEdge.first =-1;
		newEdge.second=-1;
		if (n1 < n2) {
			e1 = Edge(n1, n2);
		}
		else {
			e1 = Edge(n2, n1);
		}
		if (n2 < n3) {
			e2 = Edge(n2, n3);
		}
		else {
			e2 = Edge(n3, n2);
		}
		if (n3 < n1) {
			e3 = Edge(n3, n1);
		}
		else {
			e3 = Edge(n1, n3);
		}
		
		if((disFlags[n1]==0)&&(disFlags[n2]==0)&&(disFlags[n3]==0))
		{
			//三个点都在平面上
			//作为一个边界提取，找边界
			tri.n1 = n1;
			tri.n2 = n2;
			tri.n3 = n3;
			kelemsOnPlane.push_back(tri);
			if (mapEdgesCrossPoints.find(e1)== mapEdgesCrossPoints.end())
			{
				mapEdgesCrossPoints.insert(std::map<Edge,int>::value_type(e1, ++i));
			}
			if (mapEdgesCrossPoints.find(e2)== mapEdgesCrossPoints.end())
			{
				mapEdgesCrossPoints.insert(std::map<Edge,int>::value_type(e2, ++i));
			}
			if (mapEdgesCrossPoints.find(e3)== mapEdgesCrossPoints.end())
			{
				mapEdgesCrossPoints.insert(std::map<Edge,int>::value_type(e3, ++i));
			}
		}
		else if((crossType1==0)&&((crossType==1)||(crossType==-1)))
		{
			//一条边在切面上
			if (mapEdgesCrossPoints.find(e1) == mapEdgesCrossPoints.end())
			{
				if(disFlags[n1]+disFlags[n2]==0)
				{
					itMap = mapIndexPoints.find(vnodes[e1.first]);
					if (itMap==mapIndexPoints.end())
					{

						mapIndexPoints.insert(std::map<Vtx,int>::value_type(vnodes[e1.first],++i));
						newEdge.first = i;
					}
					else
					{
						newEdge.first = itMap->second;
					}
					itMap = mapIndexPoints.find(vnodes[e1.second]);

					if (itMap==mapIndexPoints.end())
					{
						mapIndexPoints.insert(std::map<Vtx,int>::value_type(vnodes[e1.second],++i));
						newEdge.second = i;
					}
					else
					{
						newEdge.second = itMap->second;
					}
					mapEdgesCrossPoints.insert(std::map<Edge,int>::value_type(e1, newEdge.first));
					
					if(newEdge.first>newEdge.second)
					{  
						int temp = newEdge.first;
						newEdge.first = newEdge.second;
						newEdge.second = temp;
					}
					lineEdges.insert(newEdge);
					continue;			
				}
			}

			if (mapEdgesCrossPoints.find(e2) == mapEdgesCrossPoints.end())
			{
				if(disFlags[n2]+disFlags[n3]==0)
				{
					itMap = mapIndexPoints.find(vnodes[e2.first]);
					if (itMap==mapIndexPoints.end())
					{
						mapIndexPoints.insert(std::map<Vtx,int>::value_type(vnodes[e2.first],++i));
						newEdge.first = i;
					}
					else
					{
						newEdge.first = itMap->second;
					}
					itMap = mapIndexPoints.find(vnodes[e2.second]);

					if (itMap==mapIndexPoints.end())
					{
						mapIndexPoints.insert(std::map<Vtx,int>::value_type(vnodes[e2.second],++i));
						newEdge.second = i;
					}
					else
					{
						newEdge.second = itMap->second;
					}
					mapEdgesCrossPoints.insert(std::map<Edge,int>::value_type(e2, newEdge.first));
					
					if(newEdge.first>newEdge.second)
					{  
						int temp = newEdge.first;
						newEdge.first = newEdge.second;
						newEdge.second = temp;
					}
					lineEdges.insert(newEdge);
					continue;		
				}
			}
			if (mapEdgesCrossPoints.find(e3) == mapEdgesCrossPoints.end())
			{
				if(disFlags[n1]+disFlags[n3]==0)
				{
					itMap = mapIndexPoints.find(vnodes[e3.first]);
					if (itMap==mapIndexPoints.end())
					{
						mapIndexPoints.insert(std::map<Vtx,int>::value_type(vnodes[e3.first],++i));
						newEdge.first = i;
					}
					else
					{
						newEdge.first = itMap->second;
					}
					itMap = mapIndexPoints.find(vnodes[e3.second]);

					if (itMap==mapIndexPoints.end())
					{
						mapIndexPoints.insert(std::map<Vtx,int>::value_type(vnodes[e3.second],++i));
						newEdge.second = i;
					}
					else
					{
						newEdge.second = itMap->second;
					}
					mapEdgesCrossPoints.insert(std::map<Edge,int>::value_type(e3, newEdge.first));
				
					if(newEdge.first>newEdge.second)
					{  
						int temp = newEdge.first;
						newEdge.first = newEdge.second;
						newEdge.second = temp;
					}
					lineEdges.insert(newEdge);
					continue;			
				}
			}
			
		}
		else if ((crossType!=3)&&(crossType!=-3)
			&&(crossType!=2)&&(crossType!=-2)) 
		{
			//相交两点,计算交点
			bool isOnPlane(false);
			bool isCrossed(false);
			Vtx crossPoint;
			
			if (mapEdgesCrossPoints.find(e1) == mapEdgesCrossPoints.end())
			{
				if ((disFlags[n1]*disFlags[n2]==-1)||(disFlags[n1]*disFlags[n2]==0))
				{
					CalculateCrossPoint(vnodes,e1,plane,crossPoint,isOnPlane,isCrossed);
					if (isCrossed)
					{
						itMap = mapIndexPoints.find(crossPoint);
						if (itMap==mapIndexPoints.end())
						{
							mapIndexPoints.insert(std::map<Vtx,int>::value_type(crossPoint,++i));
							newEdge.first = i ; 
						}
						else
						{
							newEdge.first = itMap->second;
						}
						mapEdgesCrossPoints.insert(std::map<Edge,int>::value_type(e1,newEdge.first));
					}
				}
			}
			else
			{			
				newEdge.first = mapEdgesCrossPoints[e1];
			}
			if (mapEdgesCrossPoints.find(e2) == mapEdgesCrossPoints.end())
			{
				if ((disFlags[n2]*disFlags[n3]==-1)||(disFlags[n2]*disFlags[n3]==0))
				{
					CalculateCrossPoint(vnodes,e2,plane,crossPoint,isOnPlane,isCrossed);
					
					if (isCrossed)
					{
						itMap = mapIndexPoints.find(crossPoint);
						if (itMap==mapIndexPoints.end())
						{
							mapIndexPoints.insert(std::map<Vtx,int>::value_type(crossPoint,++i));
							if (newEdge.first == -1)
							{
								newEdge.first = i;	
							}
							else /*if ((i!=newEdge.first))*/
							{
								newEdge.second = i;
								
								if(newEdge.first>newEdge.second)
								{  
									int temp = newEdge.first;
									newEdge.first = newEdge.second;
									newEdge.second = temp;
								}
								lineEdges.insert(newEdge);
								continue;
							}
							mapEdgesCrossPoints.insert(std::map<Edge,int>::value_type(e2, i));
						}
						else
						{
							if (newEdge.first == -1)
							{
								newEdge.first = itMap->second;	
							}
							else if (newEdge.first!=itMap->second)
							{
								newEdge.second = itMap->second;	
								
								if(newEdge.first>newEdge.second)
								{  
									int temp = newEdge.first;
									newEdge.first = newEdge.second;
									newEdge.second = temp;
								}
								lineEdges.insert(newEdge);
								continue;
							}
							mapEdgesCrossPoints.insert(std::map<Edge,int>::value_type(e2, itMap->second));
						}
							
					}
				}
			}
			else
			{
				if (newEdge.first ==-1)
				{
					newEdge.first =  mapEdgesCrossPoints[e2];
				}
				else if (newEdge.first!=mapEdgesCrossPoints[e2])
				{
					newEdge.second =  mapEdgesCrossPoints[e2];
					
					if(newEdge.first>newEdge.second)
					{  
						int temp = newEdge.first;
						newEdge.first = newEdge.second;
						newEdge.second = temp;
					}
					lineEdges.insert(newEdge);
					continue;
				}
			}
			
			if (mapEdgesCrossPoints.find(e3) == mapEdgesCrossPoints.end())
			{
				if ((disFlags[n3]*disFlags[n1]==-1)||(disFlags[n3]*disFlags[n1]==0))
				{
					CalculateCrossPoint(vnodes,e3,plane,crossPoint,isOnPlane,isCrossed);
					if (isCrossed)
					{
						itMap = mapIndexPoints.find(crossPoint);
						if (itMap==mapIndexPoints.end())
						{
							mapIndexPoints.insert(std::map<Vtx,int>::value_type(crossPoint,++i));
							newEdge.second = i;	
						}
						else
						{
							newEdge.second = itMap->second;
						}
						mapEdgesCrossPoints.insert(std::map<Edge,int>::value_type(e3, newEdge.second));
						if(newEdge.first>newEdge.second)
						{  
							int temp = newEdge.first;
							newEdge.first = newEdge.second;
							newEdge.second = temp;
						}
						lineEdges.insert(newEdge);
						continue;
					}
					
				}
			}
			else
			{
				
				newEdge.second = mapEdgesCrossPoints[e3];
				
				if(newEdge.first>newEdge.second)
				{  
					int temp = newEdge.first;
					newEdge.first = newEdge.second;
					newEdge.second = temp;
				}
				lineEdges.insert(newEdge);
				continue;
			}
			
		}
		
	}
	 int pntNum = i+1;
	crossPoints.resize(pntNum);
	for (std::map<Vtx,int>::iterator it = mapIndexPoints.begin();it!=mapIndexPoints.end();it++) 
	{
		Vtx tempVtex = it->first;
		int tempIndex = it->second;
		crossPoints.at(tempIndex) = tempVtex;
	}	
	std::set<Edge>::iterator itEdge;
	// construct an auxiliary map
	std::map<int, VECEDGE> mapNodeEdges;
	for (itEdge = lineEdges.begin(); itEdge != lineEdges.end(); /*itEdge++*/) {
		Edge e = *itEdge;
		int n1 = e.first;
		int n2 = e.second;
		if (n1==n2)
		{
			lineEdges.erase(itEdge++);
			continue;
		}
		else itEdge++;



		if (mapNodeEdges.find(n1) == mapNodeEdges.end()) {
			VECEDGE vec;
			vec.push_back(e);
			mapNodeEdges.insert(std::map<int, VECEDGE>::value_type(n1, vec));
		}
		else {
			VECEDGE& vec = mapNodeEdges[n1];
			vec.push_back(e);
		}

		if (mapNodeEdges.find(n2) == mapNodeEdges.end()) {
			VECEDGE vec;
			vec.push_back(e);
			mapNodeEdges.insert(std::map<int, VECEDGE>::value_type(n2, vec));
		}
		else {
			VECEDGE& vec = mapNodeEdges[n2];
			vec.push_back(e);
		}
	}

	while (lineEdges.size() > 0) {
		itEdge = lineEdges.begin();
		Edge e1 = *itEdge;
		lineEdges.erase(itEdge);

		
		int n1 = e1.first;
		int n2 = e1.second;

		Edge e0= e1; 
		LOOP loop;
		int n0 = n1;
		loop.push_back(n1);
		loop.push_back(n2);

		Edge e2;
		VECEDGE& vec = mapNodeEdges[n2];
		while (vec.size() >= 2 && n2 != n0) {

			if (vec[0] == e1) {
				e2 = vec[1];
			}
			else {
				e2 = vec[0];
			}

			itEdge = lineEdges.find(e2);
			if (itEdge!=lineEdges.end())
			{
				lineEdges.erase(itEdge);
			}else
				break;
			//lineEdges.erase(e2);

			if (e2.first == n2) {
				n2 = e2.second;
			}
			else {
				n2 = e2.first;
			}
			loop.push_back(n2);
			vec = mapNodeEdges[n2];
			e1 = e2;
		}

		if (n2!=n0)
		{	
			e1 = e0;
			n2=e0.first;
			VECEDGE& vec = mapNodeEdges[n2];
			while (vec.size() >= 2 /*&& n2 != n0*/) {

				if (vec[0] == e1) {
					e2 = vec[1];
				}
				else {
					e2 = vec[0];
				}

				itEdge = lineEdges.find(e2);
				if (itEdge!=lineEdges.end())
				{
					lineEdges.erase(itEdge);
				}
				else 
					break;
				//lineEdges.erase(e2);

				if (e2.first == n2) {
					n2 = e2.second;
				}
				else {
					n2 = e2.first;
				}
				loop.insert(loop.begin(),n2);
				vec = mapNodeEdges[n2];
				e1 = e2;
			}
		}
		vecloops.push_back(loop);
	}
	//std::vector<LOOP> vecloopsOnPlane;
	if (kelemsOnPlane.size()!=0)
	{
		FindFreeLoops(kelemsOnPlane,vecloopsOnPlane);
	}	
	
}

/**********************************************************************
*函数名称：CalculateCrossPoint(const std::vector<Vtx>,const Edge， Plane,Vtx,bool ,bool  )
*函数功能：计算边与Mesh的交点
*参数说明：vnodes   点坐标
		   e 边界的索引
		   plane	切面
*返回值：  crossP 交点坐标
			isOnPlane   边e是否在切面上
			isCrossed 边e与切面是否相交
 *Create By Wuwei 2014/5/14
/**********************************************************************/
void MeshTools::CalculateCrossPoint( const std::vector<Vtx>& vnodes, const Edge & e,  Plane plane,Vtx& crossP,bool& isOnPlane ,bool& isCrossed )
{
	isCrossed = false;
	isOnPlane = false;
	int n1 = e.first;
	int n2 = e.second;
	Vtx p1 = vnodes[n1];
	Vtx p2 = vnodes[n2];
	Vtx dp = p2-p1;
	Vtx p;
	double temp =dp*plane.n;
	if (temp==0.0)
	{
		isOnPlane = true;
		crossP = p2;
	}
	else
	{
		double t;
		t=(plane.M_0*plane.n-p1*plane.n)/temp;
		if ((t<=1.0)&&(t>=0.0))
		{
			p=p1+(p2-p1)*t;
			crossP = p;
			isCrossed = true;
		}
		else
		{
			isCrossed = false;
		}
		isOnPlane = false;
	}
}

/**********************************************************************
*函数名称：CalculateTriCrossPoints(const )
*函数功能：计算两个三角形的交点
*参数说明：vnodes   点坐标
		   e 边界的索引
		   plane	切面
*返回值：  crossP 交点坐标
			isOnPlane   边e是否在切面上
			isCrossed 边e与切面是否相交
 *Create By Wuwei 2014/5/14
/**********************************************************************/
void MeshTools::GetTowTrisCrossLine( Vtx triangle1[3], Vtx triangle2[3],std::vector<Vtx>& vCrossPnt,bool&isParal)
{
	isParal = false;
	Plane plane1; //用第一个三角形构造平面
	plane1.M_0 = triangle1[0];
	plane1.n = (triangle1[1]-triangle1[0]).cross(triangle1[2]-triangle1[0]);//法线
	std::vector<Vtx> vCrossPnt1;
	std::map<Vtx,int> mapCrossPts;
	std::map<Vtx,int>::iterator itMapCrossPts;
	bool isOnPlane;
	//计算三角形2与平面1的交点vCrossPnt1
	CalculateTriWithPlane(triangle2,plane1,vCrossPnt1,isOnPlane);
	
	if (!isOnPlane)
	{
		if (vCrossPnt1.size()==2)
		{
			Vtx w[2];
			//计算三角形1与线段（vCrossPnt1）的交点
			//将交线截取在第一个三角形内
			int crossNum = SegIntersTri(triangle1,vCrossPnt1[0],vCrossPnt1[1],w);
			if (crossNum ==2)
			{
				for (int i = 0;i<crossNum;i++)
				{
					vCrossPnt.push_back(w[i]);
				}
			}
		}
		
	}
}



/**********************************************************************
*函数名称：FindIntersection2Seg(Vtx p0,Vtx p1,Vtx q0,Vtx q1,Vtx iters[2])
*函数功能：计算共面的空间线段的相交情况
*参数说明：p0				  (i)线段1的起点
						p1			  (i)线段1的终点
						q0			  (i)线段2的起点
						q1		 	  (i)线段2的终点
						iters[2]		  (o) 两线段的交点
*返回值：	int   交点个数
 *Created By Wuwei 2014/5/14
/**********************************************************************/
int MeshTools::FindIntersection2Seg(Vtx p0,Vtx p1,Vtx q0,Vtx q1,Vtx iters[2])
{	//线段两端点（start end）	d0 是线段两点的差 d0 = end- start
	//p0是线段的起点
	//线段：p0+s*d0  s is in [0,1];p1+t*d1 t is in [0,1]
	double sqrtEpsilon =/* 0.0*/1e-10;//线段夹角的阈值
	Vtx e0 = q0 - p0;
	Vtx d0 = p1 - p0;
	Vtx d1 = q1-q0;
	Vtx kcross = d0.cross(d1);//叉乘判断是否平行;
	double sqrKcross = kcross.dot(kcross);
	double sqrtLen0 = d0.dot(d0);
	double sqrtLen1 = d1.dot(d1);
	if(sqrKcross>sqrtEpsilon*sqrtLen0*sqrtLen1)
	{
		//不平行 
		//跨立判断是否相交
		Vtx e1 = q1 - p0;
		Vtx f0 = p0 - q0;
		Vtx f1 = p1 - q0;
		if (((e0.cross(d0)).dot(e1.cross(d0)))<0 
			&& ((f0.cross(d1)).dot(f1.cross(d1)))<0)//跨立相交
		{
			Plane p;
			p.M_0 = p0;
			p.n = d0.cross(kcross);
			std::vector<Vtx> tempV;
			tempV.push_back(q0);
			tempV.push_back(q1);
			Edge e =Edge(0,1);
			bool isOnplane,isCrossed;
			CalculateCrossPoint(tempV,e,p,iters[0],isOnplane,isCrossed);
			return 1;
		}

	}
	//平行，判断是否共线
	kcross = e0.cross(d0);
	sqrKcross = kcross.dot(kcross);
	double sqrtLenE = e0.dot(e0);
	if(sqrKcross>sqrtEpsilon*sqrtLen0*sqrtLenE)
	{
		return 0 ;//不相交
	}
	//共线 求相交区间
	double s0 = d0.dot(e0)/sqrtLen0,s1=s0+d0.dot(d1)/sqrtLen0,w[2];
	double smin = std::min<double>(s0,s1);
	double smax = std::max<double>(s0,s1);
	int imax = FindIntersection2Inter(0.0,1.0,smin,smax,w);
	for (int i = 0;i<imax;i++)
	{
		iters[i] = p0+d0*w[i];
	}
	return imax;
}



/**********************************************************************
*函数名称：FindIntersection2Inter(double u0,double u1,double v0,double v1,double w[2])
*函数功能：计算区间[u0，u1] [v0，v1](u0<u1,v0<v1)的相交情况
*参数说明：p0				  (i)区间1的起点
						p1			  (i)区间1的终点
						q0			  (i)区间2的起点
						q1		 	  (i)区间2的终点
						w[2]		  (o) 两区间的交点
*返回值：	int   交点个数
 *Created By Wuwei 2014/5/14
/**********************************************************************/
int MeshTools::FindIntersection2Inter(double u0,double u1,double v0,double v1,double w[2])
{
	if (u1<v0||u0>v1)
	{
		return 0;
	}

	if (u1>v0)
	{
		if (u0<v1)
		{
			if (u0<v0) w[0] = v0;else w[0] =u0;
			if (u1>v1) w[1] = v1;else w[1]=u1;
			return 2;
		} else{
			//u0==v1
			w[0] = u0;
			return 1;
		}
	}else {
		//u1==v0
		w[0]  = u1;
		return 1;
	}
}




/**********************************************************************
*函数名称：CalculateTriWithPlane(Vtx triangle[3],Plane plane,std::vector<Vtx>& vCrossPnt,bool& isOnPlane)
*函数功能：计算三角形与平面的交点
*参数说明：triangle				  (i)三角形的三个顶点坐标
						plane			      (i)平面
						vCrossPnt		  (o)交点
						isOnPlane		   (0)三角形与平面是否共面
*返回值：无
 *Created By Wuwei 2014/5/14
/**********************************************************************/

void MeshTools::CalculateTriWithPlane(Vtx triangle[3],Plane plane,std::vector<Vtx>& vCrossPnt,bool& isOnPlane)
{
	isOnPlane = false;
	std::vector<Vtx> vnodes2;
	vnodes2.push_back(triangle[0]);
	vnodes2.push_back(triangle[1]);
	vnodes2.push_back(triangle[2]);

	vector<int> disFlags;
	CalculateDisToPlane(vnodes2,plane,disFlags);
	Edge e1,e2,e3;
	int crossType =disFlags[0]+disFlags[1]+disFlags[2];
	int crossType1 =disFlags[0]*disFlags[1]*disFlags[2];
	e1 = Edge(0,1);
	e2 = Edge(1,2);
	e3 = Edge(0,2);
	if ((disFlags[0]==0)&&(disFlags[1]==0)&&(disFlags[2]==0))
	{
		//两个三角面片共面
		//怎么处理呢？？
		isOnPlane = true;
	}
	
	else if(crossType1==0)
	{
		//有点在平面上
		int j = 0;
		for(int i=0;i<3;i++)
		{
			if (disFlags[i]==0)
			{
				vCrossPnt.push_back(triangle[i]);
				j=i;
			}
		}
		if(vCrossPnt.size()==2)
		{
			return;
		}
		else if (crossType==0)
		{
			if (disFlags[(j+1)%3]*disFlags[(j+2)%3]==-1)
			{
				bool isOnPlane(false);
				bool isCrossed(false);
				Vtx crossPoint;
				Edge e =Edge((j+1)%3,(j+2)%3);
				CalculateCrossPoint(vnodes2,e,plane,crossPoint,isOnPlane,isCrossed);
				vCrossPnt.push_back(crossPoint);	
			}
			return;
		}
	}
	else if(crossType1!=0&&(crossType==1||crossType==-1))
	{
		//相交
		if (disFlags[0]*disFlags[1]==-1)
		{
			bool isOnPlane(false);
			bool isCrossed(false);
			Vtx crossPoint;
			CalculateCrossPoint(vnodes2,e1,plane,crossPoint,isOnPlane,isCrossed);
			vCrossPnt.push_back(crossPoint);	
		}

		if (disFlags[1]*disFlags[2]==-1)
		{
			bool isOnPlane(false);
			bool isCrossed(false);
			Vtx crossPoint;
			CalculateCrossPoint(vnodes2,e2,plane,crossPoint,isOnPlane,isCrossed);
			vCrossPnt.push_back(crossPoint);

		}
		if (disFlags[0]*disFlags[2]==-1)
		{
			bool isOnPlane(false);
			bool isCrossed(false);
			Vtx crossPoint;
			CalculateCrossPoint(vnodes2,e3,plane,crossPoint,isOnPlane,isCrossed);
			vCrossPnt.push_back(crossPoint);
		}
	}

}


/**********************************************************************
*函数名称：PointInTriangle(Vtx triangle[3],Vtx point)
*函数功能：判断点是否在三角形内部
*参数说明：triangle				  (i)三角形的三个顶点坐标
						point			      (i)点坐标
*返回值：bool  
							true					该点在三角形内部（包括三角形的边）
							fale					该点不在三角形内部
 *Created By Wuwei 2014/5/14
/**********************************************************************/
bool MeshTools::PointInTriangle(Vtx triangle[3],Vtx point)
{
	Vtx v0 = triangle[2]-triangle[0];
	Vtx v1 = triangle[1]-triangle[0];
	Vtx v2 = point - triangle[0];

	double dot00 = v0.dot(v0);
	double dot01 = v0.dot(v1);
	double dot02 = v0.dot(v2);
	double dot11 = v1.dot(v1);
	double dot12 = v1.dot(v2);
   //有没有可能分母为0:
	double inverDeno = 1.0/(dot00*dot11-dot01*dot01);
	

	double u = (dot11*dot02-dot01*dot12)*inverDeno;

	if (u<0.0-EP||u>(1.0+EP))
	{
		return false;
	}

	double v = (dot00*dot12-dot01*dot02)*inverDeno;

	if (v<0.0-EP||v>(1.0+EP))
	{
		return false;
	}

	return u+v<=1.0+EP;

}


static void CalculateAirVol(Vtx& v1, Vtx& v2, Vtx& v3, double& S, double& V)
{
	double a1 = v2.x - v1.x;
	double a2 = v2.y - v1.y;
	double a3 = v2.z - v1.z;

	double b1 = v3.x - v1.x;
	double b2 = v3.y - v1.y;
	double b3 = v3.z - v1.z;

	double c1 = a2*b3 - a3*b2;
	double c2 = a3*b1 - a1*b3;
	double c3 = a1*b2 - a2*b1;

	S = sqrt(c1*c1 + c2*c2 + c3*c3)/2.;

	a1 = v2.x - v1.x;
	a2 = v2.y - v1.y;

	b1 = v3.x - v1.x;
	b2 = v3.y - v1.y;

	double ss = a1*b2 - a2*b1;
	double h = v1.z + v2.z + v3.z;

	V = ss*h/6.;
}
double MeshTools::Compacity(std::vector<Vtx>& vecVtx, std::vector<Fac>& vecTri, double& air, double& vol)
{
	air = 0.;
	vol = 0.;

	for (int i=0; i<vecTri.size(); i++) {
		Fac T = vecTri[i];

		int n1 = T.n1;
		int n2 = T.n2;
		int n3 = T.n3;

		double S;
		double V;
		CalculateAirVol(vecVtx[n1], vecVtx[n2], vecVtx[n3], S, V);
		if (abs(S) < 1.e-10) {
			printf(" **** Element NULL  i = %d   S = %f\n", i, S);
		}

		air += S;
		vol += V;
	}

	double cap = 0.;
	if (air > 1.e-16) {
		cap = vol/air;
	}

	return cap;
}

double Compacity(std::vector<Vtx>& vecVtx,double& air, double& vol)
{
	air = 0.;
	vol = 0.;

	for (int i = 0; i < vecVtx.size(); i += 3)
	{
		double S;
		double V;
		CalculateAirVol(vecVtx[i], vecVtx[i + 1], vecVtx[i + 2], S, V);
		if (abs(S) < 1.e-10)
		{
			printf(" **** Element NULL  i = %d   S = %f\n", i, S);
		}

		air += S;
		vol += V;
	}

	double cap = 0.;
	if (air > 1.e-16)
	{
		cap = vol / air;
	}

	return cap;
}

/**********************************************************************
*函数名称：isIntersTri2OnPlane(Vtx triangle1[3],Vtx triangle2[3] )
*函数功能：判断共面的两个空间三角形是否相交
*参数说明：triangle1				  (i)三角形1的三个顶点坐标
						triangle2			      (i)三角形1的三个顶点坐标
*返回值：bool  
							true					两三角形相交
							fale					两三角形不相交
 *Created By Wuwei 2014/5/14
/**********************************************************************/
bool MeshTools::isIntersTri2OnPlane(Vtx triangle1[3],Vtx triangle2[3] )
{
	int i0, i1;
	Vtx kDir;
	//Vtx ;
	Plane plane1; //用第一个三角形构造平面
	plane1.M_0 = triangle1[0];
	plane1.n = (triangle1[1]-triangle1[0]).cross(triangle1[2]-triangle1[0]);//法线

	Plane plane2; //用第一个三角形构造平面
	plane2.M_0 = triangle2[0];
	plane2.n = (triangle2[1]-triangle2[0]).cross(triangle2[2]-triangle2[0]);//法线
	// test edges of triangle0 for separation
	std::vector<Vtx> vnodes2;
	vnodes2.push_back(triangle2[0]);
	vnodes2.push_back(triangle2[1]);
	vnodes2.push_back(triangle2[2]);

	std::vector<Vtx> vnodes1;
	vnodes1.push_back(triangle1[0]);
	vnodes1.push_back(triangle1[1]);
	vnodes1.push_back(triangle1[2]);
	std::vector<int> disFlags;
	for (i0 = 0, i1 = 2; i0 < 3; i1 = i0, i0++)
	{
		kDir =plane1.n.cross(triangle1[i1]- triangle1[i0]);
		Plane tempPlane;
		tempPlane.M_0 = triangle1[i1];
		tempPlane.n = kDir;		
		CalculateDisToPlane(vnodes2,tempPlane,disFlags);
		if(plane1.n.dot(plane1.n)<0)
		{
			if (disFlags[0]<0&&disFlags[1]<0&&disFlags[2]<0)
			{
				// triangle1 is entirely on positive side of triangle0 edge
				return false;
			}
		}
		else if (plane1.n.dot(plane1.n)>0)
		{
			if (disFlags[0]>0&&disFlags[1]>0&&disFlags[2]>0)
			{
				// triangle1 is entirely on positive side of triangle0 edge
				return false;
			}
		}
		
		disFlags.clear();	
	}

	
	// test edges of triangle1 for separation
	for (i0 = 0, i1 = 2; i0 < 3; i1 = i0, i0++)
	{

		kDir =plane2.n.cross(triangle2[i1]- triangle2[i0]);
		Plane tempPlane;
		tempPlane.M_0 = triangle2[i1];
		tempPlane.n = kDir;		
		CalculateDisToPlane(vnodes1,tempPlane,disFlags);
		if (disFlags[0]<0&&disFlags[1]<0&&disFlags[2]<0)
		{
			// triangle1 is entirely on positive side of triangle0 edge
			return false;
		}
		
		disFlags.clear();	
	}
	return true;
}


/**********************************************************************
*函数名称：CalculateTrisBoundaryOnPlane( std::vector<Vtx>&triVector1,std::vector<Vtx> &crossPoints,
																	   std::vector<Vtx> &vnodes,std::vector<LOOP> &crossLoops)
*函数功能：计算共面的三角形序列的边界线
*参数说明：triVector1				  (i)三角形序列的顶点（重复）
						crossPoints			  (o)三角形序列的边界点
						vnodes					   (o) 三角形序列的顶点（不重复）
						crossLoops				(o) 边界线索引
*返回值：无  
 *Created By Wuwei 2014/5/14
/**********************************************************************/
void MeshTools::CalculateTrisBoundaryOnPlane( std::vector<Vtx>&triVector1,std::vector<Vtx> &crossPoints,
										  std::vector<Vtx> &vnodes,std::vector<LOOP> &crossLoops)
{	

	std::vector<int> kelems1;
	std::vector<Tri> tri1;
	std::set<Tri> triTemp;
	std::set<Tri>::iterator itTri1;
	
	vnodes.clear();
	kelems1.clear();
	tri1.clear();
	crossLoops.clear();
	std::vector<Vtxf>triVectorf;
	for (int ii = 0;ii<static_cast<int>(triVector1.size());ii++)
	{
		 float x = (float)triVector1[ii].x;
		 float y = (float)triVector1[ii].y;
		 float z = (float)triVector1[ii].z;
		 Vtx t;
		 t.x = (double)x; t.y = (double)y; t.z = (double)z;
		 triVector1[ii] = t;

	}

	STLToMesh(triVector1,vnodes,kelems1);
	for (int i =0 ;i<static_cast<int>(kelems1.size()-2);i++)
	{	
		Tri tri;
		tri.n1 = kelems1[i];
		tri.n2 = kelems1[++i];
		tri.n3 = kelems1[++i];
		itTri1 = triTemp.find(tri);
		if (itTri1 == triTemp.end())
		{
			triTemp.insert(tri);
			tri1.push_back(tri);
		}
	
	}
	FindFreeLoops(tri1,crossLoops);

	for (int jj = 0;jj<static_cast<int>(crossLoops.size());jj++)
	{
		LOOP tempVector;
		tempVector	=crossLoops[jj];
		
			for(int ii = 0;ii<static_cast<int>(tempVector.size());ii++)
			{
				crossPoints.push_back(vnodes[tempVector[ii]]);
			}
	}
	
	kelems1.clear();
	tri1.clear();

}





/**********************************************************************
*函数名称：SegIntersTri(Vtx tri[3],Vtx point1,Vtx point2,Vtx crossPoint[2])
*函数功能：计算共面的线段与三角形的交点
*参数说明：tri						  (i)三角形的三个顶点
						point1				  (i)线段起点
						point2				  (i)线段终点
						crossPoint[2]	  (o) 交点坐标
*返回值：交点个数
 *Created By Wuwei 2014/5/14
/**********************************************************************/
int MeshTools::SegIntersTri(Vtx tri[3],Vtx point1,Vtx point2,Vtx crossPoint[2])
{
	Vtx triNormal = (tri[1]-tri[0]).cross(tri[2]-tri[0]);//法线
	Plane plane1; //线段所在的、与三角形垂直的平面 
	plane1.M_0 = point1;
	plane1.n = triNormal.cross(point1-point2);
	std::vector<Vtx> vnodes2;
	vnodes2.push_back(tri[0]);
	vnodes2.push_back(tri[1]);
	vnodes2.push_back(tri[2]);

	std::vector<Vtx> vcrossPnt;
	vcrossPnt.clear();
	bool isOnPlane(false);
	CalculateTriWithPlane(tri,plane1,vcrossPnt,isOnPlane);
	Vtx point;
	if (vcrossPnt.size()==0)
	{
		return 0 ;
	}
	else if (vcrossPnt.size()==1)
	{
		 point = vcrossPnt[0];
		if (point == point1 ||point == point2)
		{
			crossPoint[0] = point;
			return 1;
		}
		
		else
		{
			double t = ((point2-point1).dot(point)-point1.dot(point2-point1))/(point2-point1).dot(point2-point1);
				if (t>0&&t<1)
				{
					crossPoint[0] = point;
					return 1;
				}
				else
					return 0;
		}
	}
	else if (vcrossPnt.size()==2)
	{
		
		 point =vcrossPnt[0]; 
		 double t0 = ((point2-point1).dot(point)-point1.dot(point2-point1))/(point2-point1).dot(point2-point1);
		 point =vcrossPnt[1]; 
		 double t1 = ((point2-point1).dot(point)-point1.dot(point2-point1))/(point2-point1).dot(point2-point1);

		 double smin = std::min<double>(t0,t1);
		 double smax = std::max<double>(t0,t1);
		 double w[2];
		 int imax = FindIntersection2Inter(0.0,1.0,smin,smax,w);
		 for (int i = 0;i<imax;i++)
		 {
			 crossPoint[i] = point1+(point2-point1)*w[i];
		 }
		 return imax;
	}
	return -1;
}


/**********************************************************************
*函数名称：PointInTriangle3dTo2d(TriVtx triangale,Vtx point,double& u ,double& v)
*函数功能：将三角形内部（包括边界）的点坐标（三维）转换为二维的表示
*参数说明：triangale			 (i)三角形的三个顶点
						point				  (i)点坐标
						u						  (o)输出u参数
						v						  (o)输出v参数
*返回值：bool
					true    点在三角形内（包括边界）
					fale    点不在三角形内
 *Created By Wuwei 2014/5/14
/**********************************************************************/
bool MeshTools::PointInTriangle3dTo2d(TriVtx triangale,Vtx point,double& u ,double& v)
{
	Vtx v0 = triangale.v2-triangale.v0;
	Vtx v1 = triangale.v1-triangale.v0;
	Vtx v2 = point - triangale.v0;

	double dot00 = v0.dot(v0);
	double dot01 = v0.dot(v1);
	double dot02 = v0.dot(v2);
	double dot11 = v1.dot(v1);
	double dot12 = v1.dot(v2);
	//有没有可能分母为0？？？？
	//不可能，是dot00*dot11 =|v0|^2|v1|^2
	//dot01*dot01 = |v0|^2|v1|^2*cos（theta)^2
	//theta是v0,v1的夹0<theta<180 所以cos（theta)！=1
	double inverDeno = 1/(dot00*dot11-dot01*dot01);
	 u = (dot11*dot02-dot01*dot12)*inverDeno;

	if (u</*0*/-EP||u>(1+EP))
	{
		return false;
	}

	 v = (dot00*dot12-dot01*dot02)*inverDeno;


	if (v</*0*/-EP||v>(1+EP))
	{
		return false;
	}
	if (u>-EP&&u<EP)
	{
		u=0.0;
	}
	if (v>-EP&&v<EP)
	{
		v=0.0;
	}
	//double s = u+v;

	return u+v<=(1.0+EP);

}


/**********************************************************************
*函数名称：CheckTriNormal(TriVtx triangale1,TriVtx triangale2)
*函数功能：检查两个三角形的法向是否一致
*参数说明：triangale1			 (i)三角形1的三个顶点
						triangale2			 (i)三角形2的三个顶点
*返回值：bool
					true    两法向夹角<90
					fale     两法向夹角>=90
 *Created By Wuwei 2014/5/14
/**********************************************************************/
bool MeshTools::CheckTriNormal(TriVtx triangale1,TriVtx triangale2)
{
	Vtx n1 =(triangale1.v1-triangale1.v0).cross(triangale1.v2-triangale1.v0);//法线
	n1.normalize();
	Vtx n2 =(triangale2.v1-triangale2.v0).cross(triangale2.v2-triangale2.v0);//法线
	n2.normalize();

	if (n1.dot(n2)>0)
	{
		//同向
		return true;
	}
	else
		return false;

}



/**********************************************************************
*函数名称：DivideMeshWithCrossLine(LOOP crossLine,
																	const std::vector<Tri> vKelems,
																	std::vector<VECTRI>& parts)
*函数功能：采用交线作为边界将mesh分为多个部分
						部分数目应等于交线环的数目+1
*参数说明：crossLine			 (i)交线索引
						vKelems			 (i)输入Mesh的索引
						parts					 (o) 分割的每一部分的Mesh索引
*返回值：无
 *Created By Wuwei 2014/5/14
/**********************************************************************/
void MeshTools::DivideMeshWithCrossLine(LOOP crossLine,
										const std::vector<Tri> vKelems,
										std::vector<VECTRI>& parts)
{
	parts.clear();
	if (vKelems.size()==0||crossLine.size()==0)
	{
		return;
	}
	std::map<Edge,VECTRI> mapEdgeTri;
	std::map<Edge,VECTRI>::iterator itMapEdgeTri;
	std::map<Edge,VECTRI>::iterator itMapEdgeTriTemp;
	MapEdgeWithTri(vKelems,mapEdgeTri);
	std::set<Edge> crossEdgeSet;
	std::set<Edge>::iterator itEdgeSet;
	std::set<Edge>::iterator itCrossEdgeSet;;
	for (int i =0;i< static_cast<int>(crossLine.size()-1);i++)
	{
		int cl0 = crossLine[i];
		int cl1 = crossLine[++i];
		Edge ce;
		if (cl0<cl1)
		{
			ce = Edge(cl0,cl1);
		}
		else
		{
			ce = Edge(cl1,cl0);
		}
		crossEdgeSet.insert(ce);
	}
	std::vector<LOOP> vecloops;
	FindSegsLoop(crossLine,vecloops);
	std::vector<bool> isLoopClosed(vecloops.size(),true);
	for (int i =0;i< static_cast<int>(vecloops.size());i++)
	{
		std::vector<int> curLoop = vecloops[i];
		if (curLoop[0]!=curLoop[curLoop.size()-1])
		{
			isLoopClosed[i] = false;
			//return;
		}
		curLoop.clear();
	}
	//int numOfParts = static_cast<int>(vecloops.size())+1;
	typedef std::set<Edge> SETEDGE;
	typedef std::set<Tri> SETTRI;
	std::set<Edge> seedEdge1;
	seedEdge1.clear();
	std::set<Edge> seedEdge2;
	seedEdge2.clear();
	std::set<Tri> triSet1;
	triSet1.clear();
	std::set<Tri> triSet2;
	triSet2.clear();
	std::set<Tri>::iterator itTriSet;
	Edge crossEdge;
	
	int closedNum = 0;
	for (int i = 0;i<static_cast<int>(vecloops.size());i++)
	{
		if (isLoopClosed[i] == true)
		{
			closedNum++;
		}
	}
	if (closedNum == 0)
	{
		return;
	}
	int triNum = 0;
	std::map<Tri,bool> isTriVisted;
	for (int ii =0;ii<vKelems.size();ii++)
	{
		isTriVisted.insert(map<Tri,bool>::value_type(vKelems[ii],false));
	}
	std::map<Tri,bool>::iterator itTriBool;
	Edge e1,e2,e3;
	int i;
		for (i = 0;i<static_cast<int>(vecloops.size());i++)
		{
			if (isLoopClosed[i] == false)
			{
				continue;
			}
			std::set<Edge> crossEdgeSetTemp;
			crossEdgeSetTemp.clear();
			std::vector<int> curLoop = vecloops[i];
			for (int ii=0;ii<static_cast<int>(curLoop.size()-1);)
			{
				int cl0 = curLoop[ii];
				int cl1 = curLoop[++ii];
				Edge ce;
				if (cl0<cl1)
				{
					ce = Edge(cl0,cl1);
				}
				else
				{
					ce = Edge(cl1,cl0);
				}
				crossEdgeSetTemp.insert(ce);
			}
			curLoop.clear();

			itCrossEdgeSet = crossEdgeSetTemp.begin();
			
		
			do 
			{
				if(itCrossEdgeSet!=crossEdgeSetTemp.end())
				{
					crossEdge = *itCrossEdgeSet;
					itMapEdgeTri = mapEdgeTri.find(crossEdge);
					seedEdge1.clear();
					seedEdge2.clear();
					triSet1.clear();
					triSet2.clear();
					if (itMapEdgeTri!=mapEdgeTri.end())
					{
						VECTRI& vctri = itMapEdgeTri->second;
						if (vctri.size()==2)
						{
							itTriBool =isTriVisted.find(vctri[0]);
							if (itTriBool->second == false)
							{
								itTriBool->second = true;
								triSet1.insert(vctri[0]);
								GetTriEdge(vctri[0],e1,e2,e3);
								itEdgeSet = crossEdgeSet.find(e1);
								if (e1 != crossEdge&&itEdgeSet==crossEdgeSet.end())
								{
									seedEdge1.insert(e1);
								}
								itEdgeSet = crossEdgeSet.find(e2);
								if (e2 != crossEdge&&itEdgeSet==crossEdgeSet.end())
								{

									seedEdge1.insert(e2);
								}
								itEdgeSet = crossEdgeSet.find(e3);
								if (e3 != crossEdge&&itEdgeSet==crossEdgeSet.end())
								{
									seedEdge1.insert(e3);
								}
							}
							itTriBool =isTriVisted.find(vctri[1]);
							if (itTriBool->second == false)
							{
								itTriBool->second = true;
								triSet2.insert(vctri[1]);
								GetTriEdge(vctri[1],e1,e2,e3);
								itEdgeSet = crossEdgeSet.find(e1);
								if (e1 != crossEdge&&itEdgeSet==crossEdgeSet.end())
								{
									seedEdge2.insert(e1);
								}
								itEdgeSet = crossEdgeSet.find(e2);
								if (e2 != crossEdge&&itEdgeSet==crossEdgeSet.end())
								{
									seedEdge2.insert(e2);
								}
								itEdgeSet = crossEdgeSet.find(e3);
								if (e3 != crossEdge&&itEdgeSet==crossEdgeSet.end())
								{
									seedEdge2.insert(e3);
								}
								//mapEdgeTri.erase(itMapEdgeTri);
							}
						}
					} 
					itCrossEdgeSet++;
				}
				
				if (i == 0)
				{
					if (seedEdge1.size()>0&& seedEdge2.size()>0)
					{
						break;
					}
				}
				else
				{
					if (seedEdge1.size()>0|| seedEdge2.size()>0)
					{
						break;
					}
				}

			}while (itCrossEdgeSet!=crossEdgeSetTemp.end());					
			
		if (itCrossEdgeSet==crossEdgeSetTemp.end())
		{
			vecloops.clear();
			mapEdgeTri.clear();
			crossEdgeSet.clear();
			return;
		}

		while (seedEdge1.size()>0) 
		{
			itEdgeSet= seedEdge1.begin();	
			Edge e = *itEdgeSet;
			
			seedEdge1.erase(itEdgeSet);
			itMapEdgeTri = mapEdgeTri.find(e);
			if (itMapEdgeTri!=mapEdgeTri.end())
			{
				VECTRI& vctri = itMapEdgeTri->second;
				for (int ii =0;ii<static_cast<int>(vctri.size());ii++)
				{
					Tri  ctri = vctri[ii];
					itTriSet = triSet1.find(ctri);
					itTriBool = isTriVisted.find(ctri);
					if (itTriSet == triSet1.end() &&itTriBool->second == false)
					{
						triSet1.insert(ctri);
						itTriBool->second = true;
						GetTriEdge(ctri,e1,e2,e3);
						itEdgeSet = crossEdgeSet.find(e1);
						if (e1!=e&&itEdgeSet ==crossEdgeSet.end())
						{
							seedEdge1.insert(e1);
						}
						itEdgeSet = crossEdgeSet.find(e2);
						if (e2!=e&&itEdgeSet ==crossEdgeSet.end())
						{
							seedEdge1.insert(e2);
						}
						itEdgeSet = crossEdgeSet.find(e3);
						if (e3!=e&&itEdgeSet ==crossEdgeSet.end())
						{
							seedEdge1.insert(e3);
						}
					}
				}
			}		
		}

		while (seedEdge2.size()>0) 
		{
			itEdgeSet= seedEdge2.begin();	
			Edge e = *itEdgeSet;
			seedEdge2.erase(itEdgeSet);
			itMapEdgeTri = mapEdgeTri.find(e);
			if (itMapEdgeTri!=mapEdgeTri.end())
			{
				VECTRI& vctri = itMapEdgeTri->second;
				//mapEdgeTri.erase(itMapEdgeTri);
				for (int ii =0;ii<static_cast<int>(vctri.size());ii++)
				{
					Tri  ctri = vctri[ii];
					itTriSet = triSet2.find(ctri);
					itTriBool = isTriVisted.find(ctri);
					if (itTriSet == triSet2.end() &&itTriBool->second == false)
					{
						itTriBool->second = true;
						triSet2.insert(ctri);
						GetTriEdge(ctri,e1,e2,e3);
						itEdgeSet = crossEdgeSet.find(e1);
						if (e1!=e&&itEdgeSet ==crossEdgeSet.end())
						{
							seedEdge2.insert(e1);
						}
						itEdgeSet = crossEdgeSet.find(e2);
						if (e2!=e&&itEdgeSet ==crossEdgeSet.end())
						{
							seedEdge2.insert(e2);
						}
						itEdgeSet = crossEdgeSet.find(e3);
						if (e3!=e&&itEdgeSet ==crossEdgeSet.end())
						{
							seedEdge2.insert(e3);
						}
					}
				}
			}		
		}


		std::vector<Tri> part1;
		std::vector<Tri> part2;
		part1.insert(part1.end(),triSet1.begin(),triSet1.end());
		part2.insert(part2.end(),triSet2.begin(),triSet2.end());


		if(part1.size()>0)
		{
			parts.push_back(part1);
			triNum +=static_cast<int>(part1.size());
		}
		if(part2.size()>0)
		{
			parts.push_back(part2);
			triNum +=static_cast<int>(part2.size());
		}
		part1.clear();
		part2.clear();
		triSet2.clear();
		triSet1.clear();
		seedEdge1.clear();
		seedEdge2.clear();
		crossEdgeSetTemp.clear();
	}
	vecloops.clear();
	mapEdgeTri.clear();
	crossEdgeSet.clear();
	if (triNum != vKelems.size())
	{
		std::vector<Tri> remainTris;
		for (itTriBool = isTriVisted.begin();itTriBool!=isTriVisted.end();itTriBool++)
		{
			if (itTriBool->second==false)
			{
				remainTris.push_back(itTriBool->first);
			}
		}
		parts.push_back(remainTris);
	}
	return;
}



void MeshTools::GetTriEdge(Tri tri, Edge& e1, Edge& e2, Edge& e3)
{

	int n1 = tri.n1;
	int n2 = tri.n2;
	int n3 = tri.n3;


	if (n1 < n2) {
		e1 = Edge(n1,n2);
	}
	else {
		e1 = Edge(n2, n1);
	}

	if (n2 < n3) {
		e2 = Edge(n2,n3);
	}
	else {
		e2 = Edge(n3, n2);
	}


	if (n3 < n1) {
		e3 = Edge(n3,n1);
	}
	else {
		e3 = Edge(n1, n3);
	}

}


//射线与三角形的交点计算关系
//返回值：是否相交，相交且交点在三角形内部（不包括边界），返回true，否则返回false
//输入：triangle[3]三角形的三个顶点，rayStrat射线的起点，rayDir射线的方向
//输出：crossPoint 交点
bool MeshTools::RayIntersctTriangle(Vtx triangle[3],Vtx rayStrat,Vtx rayDir,
									double& t,double& u,double& v)

{

	Vtx E1 = triangle[1] - triangle[0];
	Vtx E2 = triangle[2] - triangle[0];

	Vtx  P = rayDir.cross(E2);
	double det = E1.dot(P);

	Vtx T;
	if (det>0)
	{
		T = rayStrat - triangle[0];
	}
	else
	{
		T = triangle[0] - rayStrat;
		det = -det;
	}
	if (det<0.00001)
	{
		return false;
	}

	/*double*/ u = T.dot(P);
	if (u<0.0||u>det)
	{
		return false;
	}

	Vtx Q = T.cross(E1);
	/*double*/ v = rayDir.dot(Q);
	if (v<0.0||u+v>det)
	{
		return false;
	}
	/*double*/ t = E2.dot(Q);
	if (t<-EP)
	{
		return false;
	}

	double fInvDet = 1.0/det;

	t = t*fInvDet;
	u = u*fInvDet;
	v = v*fInvDet;
	//crossPoint = rayStrat + rayDir*t;//两种计算交点的方法
	//crossPoint = (1-u-v)triangle[0] + u*triangle[1]+v*triangle[2];
	return true;

}

//点与多面体的关系
//返回值：点在多面体内部返回true，否则返回false
//输入：多面体Mesh的顶点和索引，point点坐标
bool MeshTools::PointInPolyhedron(const std::vector<Vtx>& vnodes,const std::vector<Tri>& kelems,Vtx& point)
{
	bool isIn(false);
	int i = 0;
	int numOfFace = static_cast<int>(kelems.size());
	while(i<numOfFace)
	{
		Vtx randomDir;
		double fMax = (double)RAND_MAX;
		randomDir.x = (double)rand()/fMax;
		randomDir.y = (double)rand()/fMax;
		randomDir.z = (double)rand()/fMax;
		for ( i =0;i<numOfFace;i++)
		{
			Vtx triangle[3];
			triangle[0] = vnodes[kelems[i].n1];
			triangle[1] = vnodes[kelems[i].n2];
			triangle[2] = vnodes[kelems[i].n3];
			//Vtx crossPoint;
			double t,u,v;
			if (point == triangle[0] || point == triangle[1] || point == triangle[2])
			{
				return false;
			}
			Plane triPlane;
			triPlane.M_0 = triangle[0];
			triPlane.n = GetNormal(triangle[0],triangle[1],triangle[2]);
			if (PointIsOnPlane(point,triPlane))
			{
				return false;
			}
			if (RayIntersctTriangle(triangle,point,randomDir,t,u,v))
			{
				if ((u<EP)&&(u>-EP)||(v<EP)&&(v>-EP)||(u+v<1.0+EP)&&(u+v>1.0-EP))
				{
					break;    //bad Ray ,与Mesh交点在三角面片的顶点或边界上//或点在Mesh的面上时
				}
				else
					isIn =!isIn;
			}	
		}
	}
	return isIn;
}

double MeshTools::CaculateMeshVolume(const std::vector<Vtx> vNodes,const std::vector<int>vKelems)
{
	double dVolume=0.0;

	if(vKelems.size()==0)
	{
		return -1;
	}

	for (int i = 0;i<static_cast<int>(vKelems.size()-2);i++)
	{
		Vtx v1 = vNodes[vKelems[i]];
		Vtx v2 = vNodes[vKelems[++i]];
		Vtx v3 = vNodes[vKelems[++i]];
		
		double tempV = v1.x*v2.y*v3.z+v1.y*v2.z*v3.x+v1.z*v2.x*v3.y-v1.z*v2.y*v3.x-v1.y*v2.x*v3.z-v1.x*v2.z*v3.y;
		dVolume +=tempV;
	}
	dVolume = dVolume/6.0;
	return dVolume;
}

 void MeshTools::CalculateAirVol(Vtx& v1, Vtx& v2, Vtx& v3, double& S, double& V)
{
	double a1 = v2.x - v1.x;
	double a2 = v2.y - v1.y;
	double a3 = v2.z - v1.z;

	double b1 = v3.x - v1.x;
	double b2 = v3.y - v1.y;
	double b3 = v3.z - v1.z;

	double c1 = a2*b3 - a3*b2;
	double c2 = a3*b1 - a1*b3;
	double c3 = a1*b2 - a2*b1;

	S = sqrt(c1*c1 + c2*c2 + c3*c3)/2.;

	a1 = v2.x - v1.x;
	a2 = v2.y - v1.y;

	b1 = v3.x - v1.x;
	b2 = v3.y - v1.y;

	double ss = a1*b2 - a2*b1;
	double h = v1.z + v2.z + v3.z;

	V = ss*h/6.;
}

 //将线段转变为LOOP，查看线段收尾相连的情况
 void MeshTools::FindSegsLoop(LOOP crossLine,std::vector<LOOP>& vecloops)
 {
	 std::set<Edge> crossEdgeSet;
	std::set<Edge>::iterator itEdgeSet;

	for (int i =0;i< static_cast<int>(crossLine.size()-1);i++)
	{
		int cl0 = crossLine[i];
		int cl1 = crossLine[++i];
		Edge ce;
		if (cl0<cl1)
		{
			ce = Edge(cl0,cl1);
		}
		else
		{
			ce = Edge(cl1,cl0);
		}
		crossEdgeSet.insert(ce);
	}
	std::set<Edge> tempCrossEdgeSet(crossEdgeSet);
	

// construct an auxiliary map
	std::map<int, VECEDGE> mapNodeEdges;
	for (itEdgeSet = tempCrossEdgeSet.begin(); itEdgeSet != tempCrossEdgeSet.end(); itEdgeSet++) {
		Edge e = *itEdgeSet;
		int n1 = e.first;
		int n2 = e.second;

		if (mapNodeEdges.find(n1) == mapNodeEdges.end()) {
			VECEDGE vec;
			vec.push_back(e);
			mapNodeEdges.insert(std::map<int, VECEDGE>::value_type(n1, vec));
		}
		else {
			VECEDGE& vec = mapNodeEdges[n1];
			vec.push_back(e);
		}

		if (mapNodeEdges.find(n2) == mapNodeEdges.end()) {
			VECEDGE vec;
			vec.push_back(e);
			mapNodeEdges.insert(std::map<int, VECEDGE>::value_type(n2, vec));
		}
		else {
			VECEDGE& vec = mapNodeEdges[n2];
			vec.push_back(e);
		}
	}

	// find free loops

	while (tempCrossEdgeSet.size() > 0) {
		itEdgeSet = tempCrossEdgeSet.begin();
		Edge e1 = *itEdgeSet;
		tempCrossEdgeSet.erase(itEdgeSet);


		int n1 = e1.first;
		int n2 = e1.second;
	
		LOOP loop;
		int n0 = n1;
		loop.push_back(n1);
		loop.push_back(n2);

		Edge e2;
		VECEDGE& vec = mapNodeEdges[n2];
		while (vec.size() >= 2 && n2 != n0) {
		
			if (vec[0] == e1) {
				e2 = vec[1];
			}
			else {
				e2 = vec[0];
			}
			
		/*	it = freeEdges.find(e2);
			freeEdges.erase(it);*/
			itEdgeSet= tempCrossEdgeSet.find(e2);
			if (itEdgeSet!=tempCrossEdgeSet.end())
			{
				tempCrossEdgeSet.erase(itEdgeSet);
			}
			else 
				break;

			if (e2.first == n2) {
				n2 = e2.second;
			}
			else {
				n2 = e2.first;
			}
			loop.push_back(n2);
			vec = mapNodeEdges[n2];
			e1 = e2;
		}	
		 vecloops.push_back(loop);
	}
 }


 void MeshTools::MapEdgeWithTri(const std::vector<Tri>& triVect,std::map<Edge,VECTRI> & mapEdgeTri)
 {
	 mapEdgeTri.clear();
	 std::map<Edge,VECTRI>::iterator itMapEdgeTri;
	 for (int i=0; i<static_cast<int>(triVect.size()); i++) 
	 {
		 Tri tri = triVect[i];
		 int n1 = tri.n1;
		 int n2 = tri.n2;
		 int n3 = tri.n3;

		 Edge e;
		 if (n1 < n2) {
			 e = Edge(n1, n2);
		 }
		 else {
			 e = Edge(n2, n1);
		 }
		 itMapEdgeTri = mapEdgeTri.find(e);
		 if (itMapEdgeTri==mapEdgeTri.end())
		 {
			 VECTRI vtri;
			 vtri.push_back(tri);
			 mapEdgeTri.insert(std::map<Edge, VECTRI>::value_type(e, vtri));
		 }
		 else
		 {
			 itMapEdgeTri->second.push_back(tri);
		 }

		 if (n1 < n3) {
			 e = Edge(n1, n3);
		 }
		 else {
			 e = Edge(n3, n1);
		 }
		 itMapEdgeTri = mapEdgeTri.find(e);
		 if (itMapEdgeTri==mapEdgeTri.end())
		 {
			 VECTRI vtri;
			 vtri.push_back(tri);
			 mapEdgeTri.insert(std::map<Edge, VECTRI>::value_type(e, vtri));
		 }
		 else
		 {
			 itMapEdgeTri->second.push_back(tri);
		 }

		 if (n2 < n3) {
			 e = Edge(n2, n3);
		 }
		 else {
			 e = Edge(n3, n2);
		 }
		 itMapEdgeTri = mapEdgeTri.find(e);
		 if (itMapEdgeTri==mapEdgeTri.end())
		 {
			 VECTRI vtri;
			 vtri.push_back(tri);
			 mapEdgeTri.insert(std::map<Edge, VECTRI>::value_type(e, vtri));
		 }
		 else
		 {
			 itMapEdgeTri->second.push_back(tri);
		 }	
	 }
 }


 Vtx& MeshTools::GetNormal(Vtx& v1, Vtx& v2, Vtx& v3)
 {
	 Vtx normal =(v2-v1).cross(v3-v2);//法线
	 normal.normalize();
	 return normal;
 }

 STK_STATUS MeshTools::WriteBinarySTL(const vector<M3D::Vector3>& i_vcVtxs, const vector<int>& i_vcIndexs, const STK_CHAR* i_chFileName)
 {
	 if (i_vcVtxs.size() < 3 || i_vcIndexs.size() %3!=0)
	 {
		 return STK_ERROR;
	 }
	 STK_FLOAT32 v1[3], v2[3], v3[3];
	 STK_FLOAT32 n[3];

	 FILE *fp;
	 if ((fp = fopen(i_chFileName, "wb")) == NULL)
	 {
		 //Couldn't open file
		 return STK_ERROR;
	 }
	 //header
	 {
		 char header[80];
		 memset(header, 0, 80);
		 strcpy(header, "Binary STL file generated by SView!");
		 if (fwrite(header, 80, 1, fp) < 1)
			 return STK_ERROR;
	 }
	 STK_UINT32  faceNum = static_cast<STK_UINT32>(i_vcIndexs.size() / 3);

	 //UINT32 Number of triangles
	 {
		 if (fwrite(&faceNum, sizeof(STK_UINT32), 1, fp) < 1)
			 return STK_ERROR;
	 }

	 for (int ii = 0; ii < faceNum; ii++)
	 {
		 Vtx tempV1, tempV2, tempV3;
		 Vtx tempN;
		 tempV1.x = i_vcVtxs[i_vcIndexs[ii * 3 + 0]].m_x;
		 tempV1.y = i_vcVtxs[i_vcIndexs[ii * 3 + 0]].m_y;
		 tempV1.z = i_vcVtxs[i_vcIndexs[ii * 3 + 0]].m_z;

		 tempV2.x = i_vcVtxs[i_vcIndexs[ii * 3 + 1]].m_x;
		 tempV2.y = i_vcVtxs[i_vcIndexs[ii * 3 + 1]].m_y;
		 tempV2.z = i_vcVtxs[i_vcIndexs[ii * 3 + 1]].m_z;

		 tempV3.x = i_vcVtxs[i_vcIndexs[ii * 3 + 2]].m_x;
		 tempV3.y = i_vcVtxs[i_vcIndexs[ii * 3 + 2]].m_y;
		 tempV3.z = i_vcVtxs[i_vcIndexs[ii * 3 + 2]].m_z;

		 v1[0] = (STK_FLOAT32)tempV1.x;
		 v1[1] = (STK_FLOAT32)tempV1.y;
		 v1[2] = (STK_FLOAT32)tempV1.z;

		 v2[0] = (STK_FLOAT32)tempV2.x;
		 v2[1] = (STK_FLOAT32)tempV2.y;
		 v2[2] = (STK_FLOAT32)tempV2.z;

		 v3[0] = (STK_FLOAT32)tempV3.x;
		 v3[1] = (STK_FLOAT32)tempV3.y;
		 v3[2] = (STK_FLOAT32)tempV3.z;

		 tempN = MeshTools::GetNormal(tempV1, tempV2, tempV3);

		 n[0] = (STK_FLOAT32)tempN.x;
		 n[1] = (STK_FLOAT32)tempN.y;
		 n[2] = (STK_FLOAT32)tempN.z;

		 //REAL32[3] Normal vector
		 if (fwrite(n, sizeof(STK_FLOAT32), 3, fp) < 3)
			 return STK_ERROR;

		 //REAL32[3] Vertex 1,2 & 3
		 if (fwrite(v1, sizeof(STK_FLOAT32), 3, fp) < 3)
			 return STK_ERROR;
		 if (fwrite(v2, sizeof(STK_FLOAT32), 3, fp) < 3)
			 return STK_ERROR;
		 if (fwrite(v3, sizeof(STK_FLOAT32), 3, fp) < 3)
			 return STK_ERROR;
		 //UINT16 Attribute byte count (not used)
		 {
			 short attr(0);
			 if (fwrite(&attr, sizeof(attr), 1, fp) < 1)
				 return STK_ERROR;
		 }
	 }
	 fclose(fp);
	 return STK_SUCCESS;;
 }

 STK_STATUS MeshTools::WriteAsciiSTL(const vector<M3D::Vector3>& i_vcVtxs, const vector<int>& i_vcIndexs, const STK_CHAR* i_chFileName)
 {
	 return STK_SUCCESS;
 }

 /************************************************************************//**
	*	@brief		根据配置矩阵转换点数据
	*	@param[in]	i_mtxPlacement		配置矩阵
	*	@param[in]	i_pntSrc			原始点
	*	@param[out]	o_pntDst			目标点
	*	@retval		STK_SUCCESS			正常结束
	*	@note		无
	*	@attention	无
	****************************************************************************/
 STK_STATUS MeshTools::MultMatrixVec(STK_MTX32 i_mtxPlacement, STK_PNT32 i_pntSrc, STK_PNT32& o_pntDst)
 {
	 STK_FLOAT32* line0 = i_mtxPlacement.PlcMatrix[0];
	 STK_FLOAT32* line1 = i_mtxPlacement.PlcMatrix[1];
	 STK_FLOAT32* line2 = i_mtxPlacement.PlcMatrix[2];
	 STK_FLOAT32* line3 = i_mtxPlacement.PlcMatrix[3];

	 STK_FLOAT32 W = i_pntSrc.X*line3[0] + i_pntSrc.Y*line3[1] + i_pntSrc.Z*line3[2] + line3[3];

	 o_pntDst.X = (i_pntSrc.X*line0[0] + i_pntSrc.Y*line0[1] + i_pntSrc.Z*line0[2] + line0[3]) / W;
	 o_pntDst.Y = (i_pntSrc.X*line1[0] + i_pntSrc.Y*line1[1] + i_pntSrc.Z*line1[2] + line1[3]) / W;
	 o_pntDst.Z = (i_pntSrc.X*line2[0] + i_pntSrc.Y*line2[1] + i_pntSrc.Z*line2[2] + line2[3]) / W;

	 return STK_SUCCESS;
 }

 STK_STATUS MeshTools::MultMatrixVec(M3D::Matrix3x4 i_mtxPlacement, M3D::Vector3 i_pntSrc, M3D::Vector3& o_pntDst)
 {

	o_pntDst = i_mtxPlacement.Multiply(i_pntSrc);
	return STK_SUCCESS;
 }

 /****************************************************************************
 *	@brief		计算新的配置矩阵
 *	@param[in]	i_mtxPlacement1		配置矩阵1
 *	@param[in]	i_mtxPlacement2		配置矩阵2
 *	@param[out]	o_mtxPlacement		目标配置矩阵
 *	@retval		STK_SUCCESS			正常结束
 *	@note		无
 *	@attention	无
 ****************************************************************************/
 STK_STATUS MeshTools::ConvertMatrix(STK_MTX32 i_mtxPlacement1, STK_MTX32 i_mtxPlacement2, STK_MTX32& o_mtxPlacement)
 {
	 for (int ii = 0; ii < 4; ii++)
	 {
		 for (int jj = 0; jj < 4; jj++)
		 {
			 o_mtxPlacement.PlcMatrix[ii][jj] =
				 i_mtxPlacement1.PlcMatrix[ii][0] * i_mtxPlacement2.PlcMatrix[0][jj] +
				 i_mtxPlacement1.PlcMatrix[ii][1] * i_mtxPlacement2.PlcMatrix[1][jj] +
				 i_mtxPlacement1.PlcMatrix[ii][2] * i_mtxPlacement2.PlcMatrix[2][jj] +
				 i_mtxPlacement1.PlcMatrix[ii][3] * i_mtxPlacement2.PlcMatrix[3][jj];
		 }
	 }
	 return STK_SUCCESS;
 }

}
