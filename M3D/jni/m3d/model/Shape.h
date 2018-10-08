/**@file
 *@brief	Shape类头文件
  *@author
 *@date		2013-7-10
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *Shape类头文件
 *
 */

#ifndef  M3D_SSHAPE_H_
#define M3D_SSHAPE_H_
#include "m3d/M3D.h"
#include "m3d/base/Color.h"
#include "m3d/base/BoundingBox.h"
#include "m3d/base/Object.h"
 #include "m3d/base/Mutex.h"
namespace M3D
{
class SceneNode;
class RayPickAction;
class RenderAction;
class Material;
class ShapeSet;
class SceneManager;

/**
 * 图形类型枚举
 */
typedef enum ShapeType
{
	SHAPE_FEATURE_COORDINATE = -2,//特征点的坐标
	SHAPE_COORDINATE =-1,//!<坐标
	SHAPE_POINT = 0, //!< 顶点
	SHAPE_EDGE = 1, //!< 边
	SHAPE_FACE = 2, //!< 面
	SHAPE_BODY = 3, //!< 体
	SHAPE_MODEL = 4, //!< 模型
	SHAPE_NOTE = 5, //!< 标注
	CURVE_UNKNOWN = 6,
	CURVE_POLYLINE=7,
	CURVE_ELLIPSE=8,
	CURVE_LINE=9,
	CURVE_NURBSCURVE=10,
	CURVE_HYPERBOLA=11,
	CURVE_PARABOLA=12,
	CURVE_REF_POLYLINE=13,

	SHAPE_TRIMESH = 20, //!< 面
	SHAPE_REF_TRIMESH = 21, //!< 引用面，引用面引用了面中的部分数据作为自身的数据

	SHAPE_POLYLINE = 22, //!< 折线
	SHAPE_REF_POLYLINE = 23, //!< 引用折，引用了折线中的部门数据作为自身的数据

	SHAPE_BILLBOARD = 25,//!< 属性信息note

	SHAPE_ATTRIBUTE_NOTE = 30,//!< 属性信息note

	SHAPE_MEASURE_BASE = 50,//!< 测量对象
	SHAPE_MEASURE_DISTANCE = 51,//!< 测量长度对象
    SHAPE_MEASURE_ANGLE = 52,//!< 测量弧度
    SHAPE_MEASURE_PROPERTY = 53,//!< 测量属性

    SHAPE_MEASURE_PNT_PNT_LENGTH = 53,//!<测量点点距离
    SHAPE_MEASURE_PNT_LINE_LENGTH = 54,//!<点线距离
    SHAPE_MEASURE_PNT_FACE_LENGTH = 55,//!<点面距离

    SHAPE_LIGHT_BASE = 70,
    SHAPE_LIGHT_DIRECTIONAL = 71,
    SHAPE_LIGHT_SPOT = 72,
	SHAPE_LIGHT_POINT = 73,
 
	SHAPE_LITTLE_FACE = 80, //轻量化的面

	SHAPE_NOTE_BASE = 200, //!< 基本批注
	SHAPE_VOICE_NOTE = 201, //!< 语音批注
	SHAPE_TEXT_NOTE = 202, //!< 文本批注
	SHAPE_SEQUENCE_NUMBER_NOTE = 203, //!< 序号批注
	SHAPE_THREED_GESTURE_NOTE = 204, //!< 3d手势批注
	SHAPE_COMMON_NOTE = 299, //!< 通用批注

	SHAPE_HANDLE = 300, //!< 手柄
	SHAPE_POINT_HANDLE = 301, //!< 点手柄
	SHAPE_PLANE_HANDLE = 302, //!< 平面手柄
	SHAPE_AXIS_HANDLE = 303, //!< 轴手柄
	SHAPE_ROTATE_HANDLE = 304, //!< 缩放手柄
	SHAPE_SCALE_HANDLE = 305, //!< 缩放手柄
	SHAPE_ROTATECENTER = 305, //!< 旋转中心

	SHAPE_BASE = 1000, //!< 基本类型

	SHAPE_Collection = 1001,//shape 几何图形集

	LSCENE_NODE = 400,
    LSHAPE_NODE = 401,

} SHAPETYPE;

/**
 * 图形类型枚举
 */
typedef enum ShaderMode
{
	SOLID = 0, //!<实体
	LINE = 1, //!< 线框
	SOLIDANDLINE = 2, //!< 实体和线框
	SM_POINT = 3, //!< 点
} SHADEMODE;

class ShapeExternData
{

};

/**
 * Shape接口
 */
class IShape
{
public:
	virtual void RayPick(RayPickAction* action) = 0; 

	virtual SHAPETYPE GetType(void)=0;
 
	virtual IDTYPE GetID()=0;
 
	virtual void SetID(IDTYPE  value)=0;

	virtual void SetSelected(bool isSelected) =0;

	virtual void SetScene(SceneManager* scene)= 0;
};

/**
 * @class Shape
 * @brief Shape类
 *
 * 基本图形的基类
 */
class Shape: public Object ,public IShape
{
public:
	Shape();
	virtual ~Shape();

	/**
	 * 优化内存占用接口
	 */
	void Optimize();

	virtual void OnOptimize();

	Shape( const Shape& orig );
	Shape& operator = (const Shape& orig);
	/**
	 * 获取类型。派生类需重写此方法
	 * @return 图形类型枚举
	 */
	virtual SHAPETYPE GetType(void);

	void SetType(SHAPETYPE type);

	/**
	 * 设置颜色
	 * @param r
	 * @param g
	 * @param b
	 * @param a
	 */
	virtual void SetColor(const Color &color);
	/**
	 * 获取绘制颜色,选中时为选中颜色
	 * @return
	 */
	virtual Color& GetDrawColor();

	/**
	 * 获取模型颜色
	 * @return
	 */
	virtual Color& GetColor();

	/**
	 * 获取alpha值
	 * @return
	 */
	virtual float GetAlpha();

	/**
	 * 设置alpha值
	 * @param a
	 */
	virtual void SetAlpha(float a);

	/**
	 * 设置透明度，根据resub来决定是否同时设置到子模型上
	 * @param a
	 * @param relSub
	 */
	virtual void SetAlpha(float a, bool relSub);

	/**
	 * 设置选中
	 * @param select
	 */
	virtual void SetSelected(bool select);

	/**
	 * 获取选中状态
	 * @return
	 */
	virtual bool IsSelected() const;


	/**
	 * 设置包围盒
	 * @param floats
	 */
	void SetBox(const BoundingBox& box);

	/**
	 * 计算包围盒，派生类需重写
	 */
	virtual void ComputeBox();

	/**
	 * 设置可见性
	 * @param visible
	 */
	virtual void SetVisible(bool visible);

	/**
	 * 设置可见性，根据resub来决定是否同时设置到子模型上
	 * @param visible
	 * @param relSub
	 */
	virtual void SetVisible(bool visible, bool relSub);

	/**
	 * 返回可见性
	 * @return
	 */
	virtual bool IsVisible();

	/**
	 * 获取原始可见性
	 * @return
	 */
	bool IsOrigVisible();

	/**
	 * 设置原始可见性
	 * @param visible
	 */
	void SetOrigVisible(bool visible);

	/**
	 * 获取包围盒
	 * @return
	 */
	virtual BoundingBox& GetBoundingBox();

	/**
	 * 射线拾取，需重载
	 * @param action
	 */
	virtual void RayPick(RayPickAction* action);

	/**
	 * 获取属性
	 * @return
	 */
	string GetProperties();

	virtual void InitProperties();
	/**
	 * 新增属性
	 * @param key
	 * @param value
	 */
	virtual void AddProperty(string key, string value);

	/**
	 * 清空属性
	 */
	virtual void ClearProperties();

	/**
	 * 获取id
	 * @return
	 */
	virtual IDTYPE GetID()
	{
		return this->m_Id;
	}

	/**
	 * 设置Id
	 * @param value
	 */
	virtual void SetID(IDTYPE  value)
	{
		this->m_Id = value;
	}

	std::string& GetName()
	{
		return this->m_Name;
	}

	void SetName(const std::string& value)
	{
		this->m_Name = value;
	}

	/**
	 * 获取初始颜色
	 * @return
	 */
	Color& GetInitColor()
	{
		return m_InitColor;
	}

	/**
	 * 设置初始颜色
	 * @param color
	 */
	virtual void SetInitColor(const Color &color)
	{
		m_InitColor = color;
        Shape::ResetColor();
	}

	/**
	 * 设置颜色为初始颜色
	 */
	virtual void ResetColor(){
		m_Color = m_InitColor;
	}

	virtual void ResetAlpha(){
		m_Color.m_a = m_InitColor.m_a;
	}

	/**
	 * 设置对应的Node
	 * @param node
	 */
	void SetSceneNode(SceneNode* node);

	/**
	 * 获得与之关联的Node
	 * @return
	 */
	SceneNode* GetSceneNode();

	/**
	 * 获取几何属性信息，格式化为字符串后进行传递
	 * @return
	 */
	virtual string GetGeometryInfo();

	/**
	 * 复位颜色，位置，可见性
	 */
	virtual void Restore();

	virtual void FindVisiableObject(RenderAction* renderAction);

	void SetHasChild(bool useBox);
	bool GetHasChild();

	/**
	 * 设置材质
	 * @param material
	 */
	virtual void SetMaterial(Material* material);

	/**
	 * 获取材质
	 * @return
	 */
	Material* GetMaterial();

	/**
	 * 设置几何图形集对象
	 * @param node
	 */
	void SetCADNode(ShapeSet* node);

	/**
	 * 获取置几何图形集对象
	 * @return
	 */
	ShapeSet* GetCADNode();

	/**
	 * 设置复制对象id
	 * @param objId
	 */
	void SetCopyObjId(IDTYPE objId);

	/**
	 * 获取复制对象的id
	 * @return
	 */
	IDTYPE GetCopyObjId();

	/**
	 * 运行剔除标识
	 * @return
	 */
	bool AllowExculding();
	/**
	 * 设置允许剔除标识
	 * @param allow
	 */
	void SetAlloewExculding(bool allow);

	/**
	 * 获取渲染过程是否可见
	 * @return
	 */
	bool RendreVisible();
	/**
	 * 设置渲染过程是否可见
	 * @param visible
	 */
	void SetRenderVisible(bool visible);

	/**
	 * 获取体积值
	 * @return
	 */
	virtual float GetVolumeAndArea(float& volume,float& area);

	virtual void SetScene(SceneManager* scene);

private:
	void Init();

protected:
	bool m_HasColor; //!<是否有颜色
	bool m_IsSelect; //!<是否选中
	bool m_Visible; //!<是否显示
	bool m_origVisible;//!<读取时候的显示隐藏状态
	bool m_IsFirstGetProperties; //!<是否是第一次获取属性信息
	bool m_hasChild; //!<存在下级带矩阵变换的shape
	bool m_allowExcluding;//!<允许剔除

	SceneNode* node;//!< 挂载节点
	Material* m_material;//!<关联材质
	ShapeSet* m_parentNode;//!<置几何图形集

	IDTYPE  m_copyId;//!<拷贝构造时，拷贝对象的id
	SHAPETYPE m_Type; //!<类型
	IDTYPE  m_Id; //!<编号

	Color m_Color; //!<颜色
	Color m_InitColor;//!<初始颜色 模型读取时的初始颜色，在SView中不修改。

	string m_Properties; //!<属性信息
	std::string m_Name; //!<名称

	SHADEMODE m_shaderMode; //!< 着色方式

	bool m_renderVisible; //!< 是否在渲染过程中可见

    BoundingBox m_BoundingBox; //!<包围盒
	mutable Mutex m_mutex; //!<线程锁对象

	SceneManager* m_scene;
};

}
#endif // SSHAPENODE_H_

