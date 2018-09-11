/**@file
 *@brief	测量算法工具类
 *@author
 *@date		2015-8-17
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */

#ifndef M3D_MEASURE_CALCULATE_HELPER_H_
#define M3D_MEASURE_CALCULATE_HELPER_H_
#include "m3d/M3D.h"
namespace M3D
{
class Vector3;
class Matrix3x4;
class PlaneFaceAttribute;
class RevolutionFaceAttribute;
class CylinderFaceAttribute;
class ConicalFaceAttribute;
class SphereFaceAttribute;
class ToroidalFaceAttribute;
class LineAttribute;
class EllipseAttribute;
class Mesh;
class RefPolyLine;

/**
 *  @brief 测量计算辅助类
 */
class M3D_API MeasureCalculateHelper
{
public:
	MeasureCalculateHelper();
	virtual ~MeasureCalculateHelper();

	/**
	 * @brief 点是否在线段上
	 * @param pntCoordinate 测试点
	 * @param lineAttribute 测试线段
	 * @return 在返回true 否则 false
	 */

	static bool pntInSegment(const Vector3& pntCoordinate,
			const LineAttribute& lineAttribute);

	/**
	 * @brief 点的投影点是否在线段上
	 * @param pntCoordinate
	 * @param point1
	 * @param point2
	 * @return
	 */
	/**
	 * @brief 点的投影点是否在线段上,并返回投影点
	 * @param pntCoordinate 测试点坐标
	 * @param point1 线段开始点
	 * @param point2 线段结束点
	 * @param projecPnt 返回的投影点
	 * @return 若有投影点则为true 否则为false
	 */
	static bool pntInSegment(const Vector3& pntCoordinate,
			const Vector3& point1, const Vector3 & point2, Vector3 & projecPnt);

	/**
	 * @brief 点点的距离
	 * @param fistCoordinate 第一点
	 * @param secondCoordinate 第二点
	 * @param distance 距离
	 * @return 成功为true 否则为false
	 */
	static bool PntPntDistance(const Vector3& fistCoordinate,
			const Vector3& secondCoordinate, float& distance);

	/**
	 * @brief 点点的距离在XYZ上的分量
	 * @param fistCoordinate 第一点
	 * @param secondCoordinate 第二点
	 * @param distanceX X轴分量
	 * @param distanceY Y轴分量
	 * @param distanceZ Z轴分量
	 * @return 成功为true 失败为false
	 */
	static bool PntPntDistance(const Vector3& fistCoordinate,
			const Vector3& secondCoordinate, float& distanceX, float& distanceY,
			float& distanceZ);

	/**
	 * @brief 点到直线距离
	 * @param pntCoordinate 点坐标
	 * @param lineAttribute 直线几何信息
	 * @param distance 距离
	 * @param inLine =0 0 在线上 1  在延长线上 -1 在反向延长线上
	 * @param projectPnt 投影点
	 * @return true 成功 false 失败
	 */
	static bool PntLineDistance(const Vector3& pntCoordinate,
			const LineAttribute& lineAttribute, float& distance, int& inLine,
			Vector3& projectPnt);

	/**
	 * @brief 点到直线距离，根据点到折线是否有交点，若有就说明与圆弧有交点
	 * @param pntCoordinate 点坐标
	 * @param ellipseAttribute 圆弧线几何信息
	 * @param distance 距离
	 * @param inLine 0 在线上 1  在延长线上 -1 在反向延长线上
	 * @param projectPnt 投影点,无法计算则返回圆弧上距离pntCoordinate最近的点
	 * @return true 成功 false 失败
	 */
	static bool PntLineDistance(const Vector3& pntCoordinate,
			const EllipseAttribute& ellipseAttribute, float& distance,
			int& inLine, Vector3& projectPnt,Matrix3x4& worldMatrix,RefPolyLine * m_lineSet);

	/**
	 * @brief 点到平面距离
	 * @param pntCoordinate 测试点
	 * @param planFaceAttribute	平面
	 * @param distance 距离
	 * @param projectPnt 测试点在平面上的投影点
	 * @return true 成功 false 失败
	 */
	static bool PntFaceDistance(const Vector3& pntCoordinate,
			const PlaneFaceAttribute& planFaceAttribute, float& distance,
			Vector3& projectPnt);

	/**
	 * @brief 点到旋转面距离
	 * @param pntCoordinate 测试点
	 * @param revolutionFaceAttribute 旋转面
	 * @param distance 距离
	 * @param projectPnt 投影点
	 * @return true 成功 false 失败
	 */
	static bool PntFaceDistance(const Vector3& pntCoordinate,
			const RevolutionFaceAttribute& revolutionFaceAttribute,
			float& distance, Vector3& projectPnt);

	/**
	 * @brief 点到圆柱面距离
	 * @param pntCoordinate 测试点
	 * @param cylinderFaceAttribute 圆柱面
	 * @param distance 距离
	 * @param projectPnt 投影点
	 * @return true 成功 false 失败
	 */
	static bool PntFaceDistance(const Vector3& pntCoordinate,
			const CylinderFaceAttribute& cylinderFaceAttribute, float& distance,
			Vector3& projectPnt);

	/**
	 * @brief 点到圆锥面距离
	 * @param pntCoordinate 测试点
	 * @param conicalFaceAttribute 圆锥面
	 * @param distance 距离
	 * @param projectPnt 投影点
	 * @return true 成功 false 失败
	 */
	static bool PntFaceDistance(const Vector3& pntCoordinate,
			const ConicalFaceAttribute& conicalFaceAttribute, float& distance,
			Vector3& projectPnt);

	/**
	 * @brief 点到球面距离
	 * @param pntCoordinate 测试点
	 * @param sphereFaceAttribute 球面
	 * @param distance 距离
	 * @param projectPnt 投影点
	 * @return true 成功 false 失败
	 */
	static bool PntFaceDistance(const Vector3& pntCoordinate,
			const SphereFaceAttribute& sphereFaceAttribute, float& distance,
			Vector3& projectPnt);

	/**
	 * @brief 点到轮胎面距离
	 * @param pntCoordinate 测试点
	 * @param toroidalFaceAttribute 轮胎面
	 * @param distance 距离
	 * @param projectPnt 投影点
	 * @return true 成功 false 失败
	 */
	static bool PntFaceDistance(const Vector3& pntCoordinate,
			const ToroidalFaceAttribute& toroidalFaceAttribute, float& distance,
			Vector3& projectPnt);

	/**
	 * 当选择的面没有几何属性时，计算出点到mesh面的最短距离
	 * @param pntCoordinate 测试点
	 * @param meshdata mesh数据
	 * @param distance 距离
	 * @param projection 投影点
	 * @return true 成功 false 失败
	 */
	static bool PntFaceDistance(const Vector3& pntCoordinate, Mesh * meshdata,
			float& distance, float & maxDistance, Vector3& projection,Vector3& maxPoint);

	/**
	 * @brief 直线到直线距离
	 * @param lineAttribute0 第一条线
	 * @param lineAttribute1 第二条线
	 * @param distance 距离
	 * @param projectPnt0 距离最近点对点1
	 * @param projectPnt1 距离最近点对点2
	 * @param parallel 是否平行
	 * @return true 成功 false 失败
	 */
	static bool LineLineDistance(const LineAttribute& lineAttribute0,
			const LineAttribute& lineAttribute1, float& distance,
			Vector3& projectPnt0, Vector3& projectPnt1, bool &parallel);

	/**
	 * @brief 线和椭圆线之间的距离
	 * @param lineAttribute 线几何属性
	 * @param ellipseAttribute  椭圆几何属性
	 * @param distance 距离
	 * @param projectPnt 投影点
	 * @return true 成功 false 失败
	 */
	static bool LineLineDistance(const LineAttribute& lineAttribute,
			const EllipseAttribute& ellipseAttribute, float& distance,
			Vector3& projectPnt);

	/**
	 * @brief 椭圆与椭圆之间的距离
	 * @param ellipseAttribute0 椭圆几何属性
	 * @param ellipseAttribute1 椭圆几何属性
	 * @param distance 距离
	 * @param projectPnt 投影点
	 * @return true 成功 false 失败
	 */
	static bool LineLineDistance(const EllipseAttribute& ellipseAttribute0,
			const EllipseAttribute& ellipseAttribute1, float& distance,
			Vector3& projectPnt);

	/**
	 * @brief 线到面的距离
	 * @param lineAttribute 直线
	 * @param planFaceAttribute 平面
	 * @param distance 距离
	 * @param projectPnt0 线上点
	 * @param projectPnt1 面上点
	 * @return true 成功 false 失败
	 */
	static bool LineFaceDistance(const LineAttribute& lineAttribute,
			const PlaneFaceAttribute& planFaceAttribute, float& distance,
			Vector3& projectPnt0, Vector3& projectPnt1);


	/**
	 * @brief 面面之间的距离
	 * @param planFaceAttribute0 平面几何属性
	 * @param planFaceAttribute1 平面集合属性
	 * @param distance 距离
	 * @param projectPnt0 投影点1
	 * @param projectPnt1 投影点2
	 * @return true 成功 false 失败
	 */
	static bool FaceFaceDistance(const PlaneFaceAttribute& planFaceAttribute0,
			const PlaneFaceAttribute& planFaceAttribute1, float& distance,
			Vector3& projectPnt0, Vector3& projectPnt1);

	/**
	 * @brief 计算两条非平行线间的夹角
	 * @param lineAttribute0 第一条线
	 * @param lineAttribute1 第二条线
	 * @param angle 夹角
	 * @return true 成功 false 失败
	 */
	static bool LineLineAngle(const LineAttribute& lineAttribute0,
			const LineAttribute& lineAttribute1, float& angle);


	/**
	 * @brief 两个非平行平面间的夹角
	 * @param planFaceAttribute0 平面1
	 * @param planFaceAttribute1 平面2
	 * @param angle 夹角
	 * @return true 成功 false 失败
	 */
	static bool FaceFaceAngle(const PlaneFaceAttribute& planFaceAttribute0,
			const PlaneFaceAttribute& planFaceAttribute1, float& angle);


	/**
	 * @brief 直线与平面之间的夹角
	 * @param lineAttribute0 直线
	 * @param planFaceAttribute1 平面
	 * @param angle 夹角
	 * @return true 成功 false 失败
	 */
	static bool LineFaceAngle(const LineAttribute& lineAttribute0,
			const PlaneFaceAttribute& planFaceAttribute1, float& angle);

	/**
	 * @brief 边线的长度
	 * @param lineAttribute0 线段
	 * @param length 长度
	 * @return true 成功 false 失败
	 */
	static bool LineProperty(const LineAttribute& lineAttribute0,
			float& length);

	/**
	 * @brief 圆或圆弧的直径（半径）、圆心
	 * @param ellipseAttribute0 椭圆几何属性
	 * @param radius 半径
	 * @param center 椭圆中心
	 * @return true 成功 false 失败
	 */
	static bool LineProperty(const EllipseAttribute& ellipseAttribute0,
			float& radius, Vector3& center);

	/**
	 * @brief 已知三点坐标，求三角形面积
	 * @param point1 三角形顶点1
	 * @param point2 三角形顶点2
	 * @param point3 三角形顶点3
	 * @param area
	 * @return true 成功 false 失败
	 */
	static bool triangleArea(const Vector3 & point1, const Vector3 & point2,
			const Vector3 &point3, float & area);

	/**
	 * @brief 计算粗任意面的面积，通过三角形面积累加
	 * @param meshData mesh数据
	 * @param area 面积
	 * @return true 成功 false 失败
	 */
	static bool faceArea(Mesh * meshData, float &area);

	/**
     * @brief 求折线长度
     * @param lineSet 折线端点集合
     * @param length 折线长度
     */
    static void PolyLineLength(RefPolyLine * lineSet,float & length);
    
    /**
	 * @brief 点到折线的距离，针对没有几何属性的线
	 * @param lineSet 折线定点集合
	 * @param point 点
	 * @param distance 距离
	 * @return true 成功 false 失败
	 */
	static bool PntLineDistacne(RefPolyLine * lineSet, const Vector3 & point,
			float & distance, Vector3 & minPoint);

	/**
	 * @brief 点point到线段point1---point2最短距离
	 * @param point1 线段端点1
	 * @param point2 线段端点2
	 * @param point 点1
	 * @param minDistance 最小距离
	 * @return true 成功 false 失败
	 */
	static bool PntLineDistance(const Vector3& point1, const Vector3& point2,
			const Vector3& point, float & minDistance, Vector3& minPoint);

	/**
	 * @brief 判断射线是不是与线段有交点
	 * @param RayPoint0 射线1
	 * @param RayPoint1 射线2
	 * @param point0 点1
	 * @param point1 点2
	 * @return true 成功 false 失败
	 */
	static bool RaySegmentInsect(const Vector3& RayPoint0,const Vector3& RayPoint1,
			const Vector3& point0,const Vector3 & point1);

	/**
	 * 求两个圆弧间的距离，距离有四种类型
	 * 1 两圆弧所在平面平行，distance1为轴距,distance2为圆心距
	 * 2 两圆弧为同心圆弧，distance1为轴距，distance为半径差
	 * 0 两圆弧所在平面不平行，轴距无效，distance2为圆心距
	 * @param ellipseAttribute0
	 * @param ellipseAttribute1
	 * @param distance1
	 * @param distance2
	 * @param type 取值 0,1,2
	 * @return true 成功 false 失败
	 */
	static bool LineLineDistance(const EllipseAttribute& ellipseAttribute0,
				const EllipseAttribute& ellipseAttribute1, float& distance1,float &distance2,
				Vector3& projectPnt0,Vector3& projectPnt1,
				int & type);

	/**
	 * @brief 求空间两条线段间的最短距离
	 * @param segment1Start  线段1开始点
	 * @param segment1End 线段1结束点
	 * @param segment2Start 线段2开始点
	 * @param segment2End 线段2 结束点
	 * @param projectPnt1 投影点1
	 * @param projectPnt2 投影点2
	 * @return true 成功 false 失败
	 */
	static bool SegmentSegmentDiatance(const Vector3& segment1Start,const Vector3& segment1End,
			const Vector3& segment2Start,const Vector3& segment2End, Vector3& projectPnt1,Vector3 & projectPnt2,
			float &minDistance);


	/**
	 * @brief 求空间两条折线间的最短距离()
	 * @param polyLine1 折线1
	 * @param polyLine2 折线2
	 * @param projectPnt1 投影点1
	 * @param projectPnt2 投影点2
	 * @param minDistance 最小距离
	 */
	static void PolylinePolylineDistance(RefPolyLine * polyLine1,RefPolyLine * polyLine2,Vector3& projectPnt1,
			Vector3 & projectPnt2,float &minDistance,Matrix3x4 & mat1,Matrix3x4 & mat2);

};

}

#endif /*M3D_MEASURE_CALCULATE_HELPER_H_*/

