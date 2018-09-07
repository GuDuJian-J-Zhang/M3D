/**@file
 *@brief
 *@author	CDJ
 *@date		2016-5-27
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_SECTIONLINEHELPER_H_
#define M3D_SECTIONLINEHELPER_H_
#include "m3d/M3D.h"
#include "m3d/graphics/SectionPlane.h"
#include "m3d/SceneManager.h"
#include "m3d/scene/SceneNode.h"
namespace M3D
{
template <class C> void FreeClear( C & cntr ) {
    for ( typename C::iterator it = cntr.begin();
              it != cntr.end(); ++it ) {
        delete * it;
    }
    cntr.clear();
}

class Face;
class Body;
class Model;

/**
 * @class
 * @brief 剖切线计算和盖面计算辅助类
 */
class M3D_API SectionLineHelper
{
public:
	SectionLineHelper();
	virtual ~SectionLineHelper();

	/**
	 * 计算剖切线
	 * @param data
	 * @param pScene
	 */
	static void DoSectionLines(void * data,SceneManager * pScene);

	/**
	 * 获取剖切线
	 * @param data
	 * @param node
	 */
	static void GetSectionLines(void* data, Model* node);

	/**
	 * 获取与剖切面相交的model
	 * @param data
	 * @param factors
	 * @param model
	 */
	static void GetInsectModel(void * data, const float * factors, Model * model);
	/**
	 * 获取与剖切面相交的Body
	 * @param lineDatas
	 * @param factors
	 * @param model
	 */

	static void GetInsectBody(vector<Vector3> & lineDatas, const float * factors, Body * model);

	/**
	 * 获取与剖切面相交的face
	 * @param lineDatas
	 * @param factors
	 * @param model
	 */
	static void GetInsectFace(vector<Vector3>& lineDatas, const float * factors, Face * model);

	/**
	 * 获取与剖切面相交的mesh
	 * @param lineData
	 * @param factors
	 * @param model
	 */
	static void GetInsectMesh(vector<Vector3>& lineData, const float * factors, Mesh * model);

	/**
	 * 获取空间直线和空间平面的交点
	 * @param planeNormal
	 * @param planePoint
	 * @param lineVector
	 * @param linePoint
	 * @return
	 */
	static Vector3 GetPlaneInsetPoint(Vector3 & planeNormal, Vector3& planePoint, Vector3 & lineVector,
			Vector3 & linePoint);

	/**
	 * 两个vector3是否相等
	 * @param v1
	 * @param v2
	 * @return
	 */
	static bool Eq(Vector3& v1, Vector3& v2);


	/**
	 * 计算点云重心
	 * @param inPnts
	 * @return
	 */
	static Vector3 CalculatePntsCenter(const vector<Vector3>* inPnts);


	/**
	 * 转换到Oxy二维平面
	 * @param pCurSectionPlane
	 * @return
	 */
	static bool TransformTo2D(SectionPlane* pCurSectionPlane);


	/**
	 * 求两点的垂直平分线的方向向量
	 * @param p1
	 * @param p2
	 * @return
	 */
	static Vector3 GetMidnormal(Vector3& p1,Vector3& p2);


	/**
	 * 求某一点是否在多边形内部
	 * @param point
	 * @param vnodes
	 * @return
	 */
	static bool PointIn2DPolygon( Vector3 point,const std::vector<Vector3>& vnodes);


	/**
	 * 求出多边形内部一点
	 * @param vnodes
	 * @return
	 */
	static Vector3 CalculatePointIn2DPloygon(const std::vector<Vector3>& vnodes);

	/**
	 * 排序剖切线
	 * @param sectionLine
	 * @param outData
	 */
	static void SortSectionLines(SectionLine * sectionLine,void * outData);

	/**
	 * 三角剖分
	 * @param pCurSectionPlane
	 */
	static void Triangulation(SectionPlane* pCurSectionPlane);

	/**
	 * 准备三角剖分数据
	 * @param crossSections
	 */
	static void PrepareTriangulationData(vector<CrossSection*>& crossSections);

	/**
	 * 三角剖分
	 * @param crossSections
	 * @param origina
	 */
	static void SectionLinesTriangulation(vector<CrossSection*>& crossSections,SectionDataBase& origina);
	/**
	 * 通过PolyToTri库剖分
	 * @param crossSections
	 * @param dataBase
	 * @return
	 */
	static bool PolyToTriTriangulation(vector<CrossSection*>& crossSections,SectionDataBase& dataBase);

	/**
	 * 执行人体剖切线流程
	 * @param data
	 * @param pScene
	 */
	static void DoFigureSectionLine(void * data,SceneManager * pScene);

	/**
	 * 获取人体剖切线
	 * @param data
	 * @param node
	 */
	static void GetFigureSectionLines(void* data, Model* node);

	/**
	 *获取与剖切面相交的model
	 * @param data
	 * @param factors
	 * @param model
	 */
	static void GetFigureInsectModel(void * data, const float * factors, Model * model);

	/**
	 * 点与空间平面关系
	 * @param factor
	 * @param point
	 * @return 返回 -1,1说明不在平面上 返回0 说明在平面上
	 */
	static int IsPointInPlane(const float * factor, Vector3& point);

	////static void SaveCurrentSectionLine(SceneManager* scene);

	////static Shape* CreateSectionLineShape(SectionPlane* pCurSectionPlane);
};

} /* namespace M3D */
#endif /* M3D_SECTIONLINEHELPER_H_ */
