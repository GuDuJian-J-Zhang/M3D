/**
 *@file
 *@brief	射线拾取动作
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */

#ifndef M3D_RAYPICK_ACTION_
#define M3D_RAYPICK_ACTION_

#include "m3d/action/Action.h"
#include "m3d/base/Matrix3x4.h"
#include "m3d/base/Vector3.h"
#include "m3d/base/Ray.h"
#include "m3d/base/Frustum.h"
#include "m3d/model/Shape.h"
#include "m3d/renderer/GLESHead.h"


namespace M3D
{
class SceneManager;
class CameraNode;

/**
 * @class RayPickActionData
 * @brief 射线拾取action用到状态等变量的封装
 */
class M3D_API RayPickActionData
{
	friend class RayPickAction;

public:
	/**
	 * @brief 得到摄像机空间射线
	 * @return 射线对象
	 */
	const Ray& GetCameraRay()const;

	const Frustum& GetFramePickFrustum()const;

	/**
	 * @brief 得到模型当前坐标系的射线
	 * @return 射线对象
	 */
	const Ray& GetModelRay()const;

private:
	/**
	 * @brief 设置模型变换矩阵
	 * @param m 变换矩阵
	 */
	void SetModelMatrix(const Matrix3x4& m);

private:
	Ray m_cameraRay; //!< 经过view变换的ray
	Ray m_modelRay;  //!< 经过modelview变换后的ray
	Frustum m_framePickFrustum;//!< 框选拾取椎体

	Matrix3x4 m_modelMatrix; //!< 模型空间变换矩阵
	vector<Vector3> m_IntersectPnts; //!< 存储射线和三角面片交点
};

/**
 * @class PickTypeFilter
 * @brief 射线拾取类型过滤器
 */
class M3D_API PickTypeFilter
{
private:
	const static int PICKTYPENUM = 600;//!< 射线拾取类型数组大小

public:
	/**
	 * @brief 构造函数
	 */
	PickTypeFilter();

	/**
	 * @brief 析构函数
	 */
	virtual ~PickTypeFilter();

	/**
	 * @brief 当前拾取shape对象的类型
	 * @return 对象类型
	 */
	int GetPickShapeType();
	/**
	 * @brief 当前拾取对象几何属性类型
	 * @return 几何属性类型
	 */
	int GetPickGeoType();

	/**
	 * @brief 设置当前拾取对象类型
	 * @param shapeType 拾取对象类型
	 */
	void SetPickShapeType(int shapeType);
	/**
	 * @brief 设置当前拾取对象几何几何类型
	 * @param geoType 拾取对象几何类型
	 */
	void SetPickGeoType(int geoType);
	/**
	 * @brief 判断当前过滤器中是否包含传入的类型
	 * @param shapeType 传入类型
	 * @return true包含 false 不包含
	 */
	bool CanPickShape(int shapeType);
	/**
	 * @brief 判断当前过滤器是否包含出入的几何类型
	 * @param geoType 出入几何类型
	 * @return true包含 false不包含
	 */
	bool CanPickGeo(int geoType);

	/**
	 * @brief 判断是否能够拾取当前颜色
	 * @param color 传入颜色
	 * @return true包含 false不包含
	 */
	bool CanPickColor(Color* color);

private:
	int currentShapeType; //!<过滤器应 当前shape对象类型
	int currentGeoType;//!< 允许拾取的对象 具有的几何属性
	int currentColorType;//!<允许拾取对象  具有的颜色属性
	bool pickShapeTyeArray[PICKTYPENUM];//!< 够拾取的子类型列表
	bool pickGeoTyeArray[PICKTYPENUM];//!< 能够拾取的子类型几何属性列表
};

/**
 * @class
 * @brief 射线求交动作，包括射线与三角形求交，射线与点求交，射线与线段求交
 */
class M3D_API RayPickAction: public Action
{
private:
	static float MINVALUE;//!< 线相交最小距离

	const static int MAXVALUE = INT_MAX;//!< 射线不相交，默认最大距离。
public:
	/**
	 * @brief 射线和三角形相交判断
	 * @param tri 三角形
	 * @param ray 射线
	 * @param I 交点
	 * @return true有交点 false没有交点
	 */
	static bool ISintersectRayAndTriangle(float* tri, const Ray&ray,Vector3& I);
	/**
	 * @brief 射线和三角形相交判断
	 * @param v0 第一个点
	 * @param v1第二个点
	 * @param v2第三个点
	 * @param ray 射线
	 * @param I 交点
	 * @return true有交点 false没有交点
	 */
	static bool ISintersectRayAndTriangle(const Vector3& v0,const Vector3& v1,
			const Vector3& v2, const Ray&ray,Vector3& I);

	/**
	 *测试绘制
	 */
	static void DrawSomeInfoForTest();
	/**
	 * @brief 射线和包围盒相交判断
	 * @param ray 射线
	 * @param box 包围盒
	 * @return true相交 false不相交
	 */
	static bool IsRayHitBox(const Ray& ray,const BoundingBox& box);
	/**
	 * @brief 射线和包围盒相交判断
	 * @param ray 射线
	 * @param box 包围盒
	 * @param intersectPnts 交点
	 * @return true相交 false不相交
	 */
	static bool RayIntersectBoxPnt(const Ray& ray,const BoundingBox& box,vector<Vector3>& intersectPnts);

	/**
	 * @brief 得到空间中亮点，在屏幕空间中的距离
	 * @param pnt1 空间点
	 * @param pnt2 空间点
	 * @param scene 场景管理
	 * @return 距离值
	 */
	static float GetScreenDis(const Vector3& pnt1,const Vector3& pnt2,SceneManager* scene);
	/**
	 * @brief 拾取边界线中的特征点
	 * @param screenPnt 屏幕坐标
	 * @param scene 场景
	 * @param featureCoordinate 空间特征点坐标
	 * @return true相交 false不相交
	 */
	static bool PickFeaturePnt(Vector2& screenPnt,SceneManager* scene,Vector3& featureCoordinate);

public:
	/**
	 * @brief 构造函数
	 */
	RayPickAction();
	/**
	 * @brief 构造函数
	 * @param scene
	 */
	RayPickAction(SceneManager* scene);
	/**
	 * @brief 析构函数
	 */
	virtual ~RayPickAction();
	/**
	 * @brief 临时记录和模型的相交点
	 * @param pos 模型交点
	 */
	void AddIntersectPnt(const Vector3& pos);
	/**
	 * @brief 通过模型矩阵，更新模型局部坐标系射线
	 * @param modelMatrix 模型世界坐标系变换矩阵
	 */
	void UpdataModelRay(const Matrix3x4& modelMatrix);
	/**
	 * @brief 得到最前端拾取对象
	 * @return 拾取到的对象 NULL 表示没有拾取到
	 */
	IShape* GetNearPickShape();

	vector<IShape*>& GetFramePickShapes();

	/**
	 * @brief 得到最后端拾取对象
	 * @return 拾取对象 NULL 表示没有拾取到
	 */
	int GetFarPickNode();
	/**
	 * @brief 判断交点是否可见，主要有剖切盖面等影响
	 * @param point 带判断点
	 * @return
	 */
	bool PointVisiable(const Vector3& point);
	/**
	 * @brief 得到最前端的拾取到的点
	 * @param vec 拾取到的点
	 * @return true有交点 false没有交点
	 */
	bool GetNearPickPoint(Vector3& vec);

	/**
	 * @brief 设置框选标识
	 * @param flag
	 */
	void SetPickAll(const bool flag);
	/**
	 * @brief 是否框选
	 * @return
	 */
	bool IsPickAll(void) const;
	/**
	 * @brief 设置射线 根据屏幕坐标点
	 * @param winx 屏幕坐标x
	 * @param winY 屏幕坐标y
	 */
	void SetRay(int winx, int winY);

	/**
	 * @brief 设置射线 根据屏幕坐标点
	 * @param screenPnt
	 */
	void SetRay(const Vector2& screenPnt);

	///设置框选区域
	void SetFramePickSection(const Vector2& leftTop,const Vector2& rightBottom);
	///设置框选类型
	void SetFramePickType(int framePickType);

	/**
	 * @brief 设置射线 根据屏幕坐标点
	 * @param viewportPoint
	 */
	void SetPoint(const Vector2 & viewportPoint);
	/**
	 * @brief 射线是否和包围盒相交
	 * @param box 待求交的包围盒
	 * @return true相交  false 不相交
	 */
	bool Intersect(const BoundingBox& box);

	/**
	* @brief 否和包围盒相交
	* @param box 待求交的包围盒
	* @return true相交  false 不相交
	*/
	bool FrustumIntersetWithWorldBox(const BoundingBox& box);

	void AddToFramePickCollection(IShape* shape);

	/**
	 * @brief 射线是否和三角形有交点
	 * @param v0 三角形第一个点
	 * @param v1 第二个点
	 * @param v2第三个点
	 * @param I 交点
	 * @return true有交点 false没有交点
	 */
	bool IsintersectRayAndTriangle(const Vector3& v0,const Vector3& v1,const Vector3& v2,Vector3& I);
	/**
	 * @brief 射线是否和线段相交
	 * @param v0 线段起点
	 * @param v1线段终点
	 * @param intersection 交点
	 * @return true相交 false 不相交
	 */
	bool Intersect(const Vector3& v0, const Vector3& v1,
			Vector3 & intersection) ;
	/**
	 * @brief 射线折否和点相交
	 * @param point 待判断点
	 * @return true相交 false不相交
	 */
	bool Intersect(const Vector3& point) ;

	void FramePick(IShape* shape);

	/**
	 * @brief 射线开始和shape对象求交
	 * @param shape 待求交对象
	 */
	void RayPick(IShape* shape);
	/**
	 * @brief 设置相交判断阈值，屏幕坐标系像素个数
	 * @param radiusinpixels 像素个数
	 */
	void SetRadius(const float radiusinpixels);
	/**
	 * @brief 将相交对象，加入缓存列表，供最后判断拾取先后关系使用
	 * @param shape shape对象
	 */
	void AddShape(IShape* shape);

	void AddFramePickShape(IShape* shape);

	/**
	 * @brief 得到拾取类型
	 * @return 拾取类型
	 */
	int GetPickShapeType();
	/**
	 * @brief 得到允许拾取对象的几何属性
	 * @return 允许拾取对象具有的几何属性
	 */
	int GetPickGeoType();

	/**
	 * @brief 设置待拾取对象的类型
	 * @param shapeType 拾取对象的类型
	 */
	void SetPickShapeType(int shapeType);
	/**
	 * @brief 设置待拾取对象具有的几何属性
	 * @param geoType 拾取对象的几何属性
	 */
	void SetPickGeoType(int geoType);
	/**
	 * @brief 判断当前拾取动作能否拾取此类型
	 * @param shapeType 待判断的类型
	 * @return true 能够拾取此类型 false 不能够拾取此类型
	 */
	bool CanPickShape(int shapeType);
	/**
	 * @brief 判断当前拾取动作，能否拾取此几何类型的shape
	 * @param geoType 待判断的几何属性
	 * @return true 能够拾取此类型 false 不能够拾取此类型
	 */
	bool CanPickGeo(int geoType);
	/**
	 * @brief 判断当前拾取动作，能否拾取此几何类型的shape
	 * @param color 待判断的颜色
	 * @return 能够拾取此类型 false 不能够拾取此类型
	 */
	bool CanPickColor(Color* color);
	/**
	 * @brief 设置当前求交的摄像机
	 * @param camera 摄像机对象
	 */
	void SetCamera(CameraNode* camera);
	/**
	 * @brief 获取摄像机对象
	 * @return 摄像机对象
	 */
	CameraNode* GetCamera();

	/**
	 * @brief 获取射线求交的数据
	 * @return 射线求交数据
	 */
	RayPickActionData* GetData();

	/**
	 * @brief 设置拾取阈值，此阈值对应屏幕上的距离，不可过大也不用过小。
	 * 建议 10 -- 20 像素
	 * @param radius
	 */
	void SetPickRadius(float radius);

	/**
	 * @brief 计算两点在屏幕上的距离
	 * @param pnt1
	 * @param pnt2
	 * @return
	 */
	float GetScreenDis(const Vector3& pnt1,const Vector3& pnt2);

private:
	virtual void OnExecute(SceneNode* node);
	void Init();
	void SetScene(SceneManager* sceneManager);
private:
	map<IShape*, vector<Vector3> > m_PickShapesMap; //!<存储所有相交的对象
	map<IShape*, map<int, vector<Vector3> > > m_MultiPickShapesMap; //!<存储所有的多选对象

	float m_fRadius; //!<投影到屏幕后的阈值，判断投影到屏幕后的两个点之间的距离

	SceneManager* sceneManager; //!< 场景管理类
	RayPickActionData* state; //!< 射线求交动作数据
	PickTypeFilter pickTypeFilter; //!<射线求交过滤器
	CameraNode* m_camera; //!< 摄像机对象

	bool m_isPickAll;//!< 是否框选

	int m_framePickType; //!<框选类型 1 inside 全部在内部  2 intersect 相交即可

	vector<IShape*> m_framePickShapesArrary; //!<存储所有框选相交的对象
};
}

#endif /*M3D_RAYPICK_ACTION_*/
