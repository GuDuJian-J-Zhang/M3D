﻿/**@file
 *@brief	Plane类头文件
 *@author	bing
 *@date		2013-11-27
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *Plane类头文件
 *
 */

#ifndef M3D_PLANE_H_
#define M3D_PLANE_H_

#include "m3d/M3D.h"
#include "m3d/base/Vector3.h"
#include "m3d/base/Color.h"
#include "m3d/base/Mutex.h"
#include "m3d/base/BoundingBox.h"
#include "m3d/graphics/SectionLine.h"
#include "m3d/graphics/CrossSection.h"
#include "m3d/model/Model.h"

namespace M3D
{
class SectionPlane;
class Model;
class SceneManager;
class Ray;
struct SectionDataBase
{
	vector< SectionLine* >m_scetionLinesData;
	SectionPlane * m_sectionPlane;
	Quaternion m_rotate;///转换到二维的旋转四元数
	Vector3 m_center;///重心
};

struct FigureSectionLine
{
	vector<Vector3> linesData;
	SectionPlane * sectionPlane;
	Model * boundingNode;
	float circumference;
};

//class Vector3;
typedef Vector3 Point3D;
//
/**@class Plane
 * @brief Plane类
 *
 * 表示一个平面，包含定义平面的参数及可设置的绘制示意平面的数据（2个三角形）
 */
class M3D_API SectionPlane : public Model
{
public:
	SectionPlane();

	/**
	 * 使用点和法线等的构造函数
	 * @param origin
	 * @param normal
	 * @param uDir
	 * @param vDir
	 */
	SectionPlane(const Point3D& origin, const Point3D& normal,const Point3D& uDir, const Point3D& vDir);

	SectionPlane(const Point3D& origin, const Point3D& normal);

	virtual ~SectionPlane();

	virtual SHAPETYPE GetType(void);

	/**
	 * 获取交点
	 * @param origin
	 * @param dir
	 * @return
	 */
	Point3D GetInsectPnt(const Point3D& origin, const Point3D& dir);
	/**
	* 通过矩阵变换剖面位置
	*/
	void SetDraggerTransform(Matrix3x4& transform);

	/**
	 *
	 * @return
	 */
	IDTYPE GetID();
	/**
	 *
	 * @param id
	 */
	void SetID(int id);
	/**
	 *
	 * @return
	 */
	bool GetEnable();
	/**
	 *
	 * @param flag
	 */
	void SetEnable(bool flag);
	/**
	 * 设置怕剖切面的辅助面显示标示
	 * @param flag
	 */
	void SetShowPlaneRect(bool flag);
	bool GetShowPlaneRect();
	/**
	 * 设置剖切盖面显示标示
	 * @param flag
	 */
	void SetShowClipPlane(bool flag);
	bool GetShowClipPlane();
    /**
     * 设置剖切面显示标示
     * @param flag
     */
    void SetShowCappingPlane(bool flag);
    bool GetShowCappingPlane();
	/**
	 *
	 * @param name
	 */
	void SetName(string name);
	string GetName();
	//void setPlaneRect(Point3D* point1,Point3D* point2);

	/**
	 * 设置平面参数
	 * @param A
	 * @param B
	 * @param C
	 * @param D
	 * @note 即公式：Ax+By+Cz+D = 0  的参数
	 */
	void SetPlaneParam(float A, float B, float C, float D);

	/**
	 *
	 * @param center
	 * @param size
	 */
	void SetDrawPlane(const Vector3 center,const Vector2 size);

	/**
	 * TODO
	 * @param box
	 */
	void SetDrawPlane(const BoundingBox& box);

	/**
	 * 复制一些参数
	 * @param sectionPlane
	 */
	void Copy(SectionPlane* sectionPlane);

	/**
	 * 获得平面参数，用于glClipPlanef
	 * @return
	 */
	float* GetEquation();

	bool IsPlaneAvaliable();

	/**
	 * 得到要显示面的顶点坐标值
	 * @return
	 */
	Vector3* GetPointArray();

	/**
	 * 得到显示面的顶点法线值
	 * @return
	 */
	Vector3* GetNormalArray();

	/**
	 * 得到要显示的线的顶点
	 * @return
	 */
	Vector3* GetLinePointArray();

	/**
	 *
	 * @return
	 */
	Color& GetFaceColor();
	/**
	 *
	 * @return
	 */
	Color& GetEdgeColor();

	/**
	 *
	 * @param color
	 */
	void SetFaceColor(Color & color);
	/**
	 *
	 * @param color
	 */
	void SetEdgeColor(Color & color)
	{
		m_edgeColor = color;
	}

	/**
	 * 清除剖切线数据库的数据
	 */
	void ClearSectionLineDataBse();
	SectionDataBase m_scetionLinesDataBase; //!<TODO 存放临时数据
	SectionLine * m_sectionLineRoot;//!<剖切线树根
	vector<CrossSection*> m_corssSections;//!<盖面数组

	//for cloth

	FigureSectionLine m_figureSectonLine;//!<人体剖切线数据结构体
	/**
	 * 清除数据
	 */
	void ClearFigureSectionLines();
	/**
	 *
	 * @param data
	 */
	void GetModelWidthAndLength(void * data);
	/**
	 * 获取剖切线数据信息
	 * @param data
	 */
	void GetSectionLineInfo(void * data);
	float* GetTransformPlaneParam();
	BoundingBox GetSceneBox() const { return m_sceneBox; }
	void SetSceneBox(BoundingBox val) { m_sceneBox = val; }
	SceneManager* GetScene() const { return m_scene; }
	void SetScene(SceneManager* val) { m_scene = val; }
	/**
	* 更新绘制数据
	*/
	void UpdateDrawData();

	/**
	* 得到要显示面的中心点
	* @return
	*/
	Vector3 GetCenterPointArray();

	virtual void OnMarkedDirty()
	{
		this->SetDraggerTransform(GetWorldTransform());
	}

	virtual void RayPick(RayPickAction* action);

    bool Intersect(Ray& ray, Vector3& intersectPnt);
	void SetCreateId(IDTYPE id);

	IDTYPE GetCreateId();

private:
	void SetTransformPlaneParam(float A, float B, float C, float D);

private:

	void Init();

	/**
	 * 根据法线计算参数
	 */
	void GetParam();


	//平面参数
//		float A;
//		float B;
//		float C;
//		float D;

	float m_transformEquation[4]; ///平面参数,A B C D顺序排列
	float m_Equation[4]; ///平面参数,A B C D顺序排列
	int m_GLClipPlaneID;//!<

	Point3D m_Origin; //!<起始范围
	Point3D m_Normal; //!<法线

	Point3D m_UDir; //!<U方向
	Point3D m_VDir; //!<V方向
	string m_Name; //!<名称

	bool m_Enable; //!<是否可用

	bool m_ShowPlaneRect; //!<是否显示平面长方形
	bool m_ShowClipPlane; //!<是否显示盖面的截面（剖面时使用）
    bool m_ShowCappingPlane; //!<是否显示剖面的截面（剖面时使用）
	///剖视平面相关
	float m_Width;//!<宽
	float m_Height;//!<高

	vector<Vector3> m_points;//!<顶点

	vector<Vector3> m_normals;//!<法线

	vector<Vector2> m_uvs;//!<uv坐标

	vector<Vector3> m_edgePoints;//!<边界线顶点

	bool m_dirty;//!<

	Vector3 m_center ;//!<
	Vector2 m_size ;//!<

	BoundingBox m_box;//!<

	BoundingBox m_sceneBox;

	Color m_edgeColor;//!<

	Color m_faceColor;//!<

	Color m_selectColor;

	//TODO:中心点？ 交点？四个点？
	//TODO:索引
	//TODO:透明度
	//TODO:颜色
	// 
	SceneManager* m_scene;
	mutable Mutex m_mutex;//!<
	Matrix3x4 m_innerTransform; //默认旋转从标准面变化过来的中间差值量
	Vector3 m_tempCenter; //构建剖切面的中心点;

	IDTYPE  m_createId;

};
}

#endif /* M3D_PLANE_H_ */
