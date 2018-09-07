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

#ifndef  M3D_ISSHAPE_H_
#define M3D_ISSHAPE_H_

#include "m3d/M3D.h"
#include "m3d/base/Color.h"
#include "m3d/base/BoundingBox.h"
#include "m3d/base/Object.h"
#include "m3d/base/Mutex.h"

namespace M3D
{
class SceneNode;
class Action;
class RayPickAction;
class RenderAction;
class BaseMaterial;
class ShapeSet;

//全局ID
class M3D_API SVLGlobalID {
public:
	SVLGlobalID();
	SVLGlobalID(unsigned int objectId);
	SVLGlobalID(unsigned int documentId, unsigned int objectId);
	bool operator < (const SVLGlobalID &cmp) const;
	SVLGlobalID operator + (const SVLGlobalID &cmp) const;
	SVLGlobalID& operator += (const SVLGlobalID &cmp);
	SVLGlobalID operator - (const SVLGlobalID &cmp) const;
	SVLGlobalID& operator -= (const SVLGlobalID &cmp);
	bool operator != (const SVLGlobalID &cmp);
	bool operator ==(const SVLGlobalID &cmp);

	unsigned int Code() const
	{
		const static unsigned int offset = 1000000000;
		return documentId*offset + objectId;
	}

	bool Valid()
	{
		return this->objectId > 0 && this->objectId != 4294967295;
	}

	string ToString()
	{
		string ret = "";

		ret += ("DocumentID:" + IntToString(documentId));
		ret += ("ObjectID:" + IntToString(objectId));

		return ret;
	}

	static SVLGlobalID G_DefaultGlobalID;
	unsigned int	documentId;		/*!文档id*/
	unsigned int	objectId;		/*!对象id*/
};


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

	SHAPE_IMAGE_MODEL = 56,//!<用来显示图片对象
	SHAPE_TEXT_MODEL = 56,//!<用来显示文字对象

    SHAPE_LIGHT_BASE = 70,
    SHAPE_LIGHT_DIRECTIONAL = 71,
    SHAPE_LIGHT_SPOT = 72,
	SHAPE_LIGHT_POINT = 73,
 
	SHAPE_NOTE_BASE = 200, //!< 基本批注
	SHAPE_VOICE_NOTE = 201, //!< 语音批注
	SHAPE_TEXT_NOTE = 202, //!< 文本批注
	SHAPE_SEQUENCE_NUMBER_NOTE = 203, //!< 序号批注
	SHAPE_THREED_GESTURE_NOTE = 204, //!< 3d手势批注
	SHAPE_ANNOTATION_NOTE = 205, //!< 批注
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

	//LSCENE_NODE = 400,
    MODEL_SHAPE = 401,
	IMAGEMODEL_SHAPE = 402, //显示图片MODEL_SHAPE
	LINEMODEL_SHAPE = 403, //用来显示线对象的Model_Shape

	SHAPE_NOTCONTAIN_IMAGEMODEL = 404,
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

class M3D_API ShapeExternData
{

};

/**
 * Shape接口
 */
class M3D_API IShape :public IObject
{
public:
	/**
	* 获取类型。派生类需重写此方法
	* @return 图形类型枚举
	*/
	virtual SHAPETYPE GetType(void) = 0;

	virtual void SetType(SHAPETYPE type) = 0;

	/**
	* 设置颜色
	* @param r
	* @param g
	* @param b
	* @param a
	*/
	virtual void SetColor(const Color &color) = 0;

	/**
	* 获取绘制颜色,选中时为选中颜色
	* @return
	*/
	virtual Color* GetDrawColor() = 0;

	/**
	* 获取模型颜色
	* @return
	*/
	virtual Color* GetColor() = 0;

	/**
	* 获取alpha值
	* @return
	*/
	virtual float GetAlpha() = 0;

	/**
	* 设置alpha值
	* @param a
	*/
	virtual void SetAlpha(float a) = 0;

	/**
	* 设置透明度，根据resub来决定是否同时设置到子模型上
	* @param a
	* @param relSub
	*/
	virtual void SetAlpha(float a, bool relSub) = 0;

	/**
	* 设置选中
	* @param select
	*/
	virtual void SetSelected(bool select) = 0;

	/**
	* 获取选中状态
	* @return
	*/
	virtual bool IsSelected() const = 0;

	/**
	* 设置可见性
	* @param visible
	*/
	virtual void SetVisible(bool visible) = 0;

	/**
	* 设置包围盒
	* @param floats
	*/
	virtual void SetBox(const BoundingBox& box) = 0;

	/**
	* 计算包围盒，派生类需重写
	*/
	virtual void ComputeBox() = 0;

	/**
	* 设置可见性，根据resub来决定是否同时设置到子模型上
	* @param visible
	* @param relSub
	*/
	virtual void SetVisible(bool visible, bool relSub) = 0;

	virtual void SetOrigVisible(bool visible, bool relSub) = 0;

	/**
	* 返回可见性
	* @return
	*/
	virtual bool IsVisible() = 0;

	/**
	* 获取原始可见性
	* @return
	*/
	virtual bool IsOrigVisible() = 0;

	/**
	* 设置原始可见性
	* @param visible
	*/
	virtual void SetOrigVisible(bool visible) = 0;

	/**
	* 获取包围盒
	* @return
	*/
	virtual BoundingBox& GetBoundingBox() = 0;

	/**
	* 射线拾取，需重载
	* @param action
	*/
	virtual void RayPick(RayPickAction* action) = 0;

	/**
	* 框选操作，需重载
	* @param action
	*/
	virtual void FramePick(RayPickAction* action) = 0;

	/**
	* 获取属性
	* @return
	*/
	virtual string GetProperties() = 0;

	virtual void InitProperties() = 0;
	/**
	* 新增属性
	* @param key
	* @param value
	*/
	virtual void AddProperty(string key, string value) = 0;

	/**
	* 清空属性
	*/
	virtual void ClearProperties() = 0;

	/**
	* 获取id
	* @return
	*/
	virtual IDTYPE GetID() = 0;

	/**
	* 设置Id
	* @param value
	*/
	virtual void SetID(IDTYPE  value) = 0;

	virtual std::string GetName() = 0;

	virtual void SetName(const std::string& value) = 0;

	/**
	* 获取初始颜色
	* @return
	*/
	virtual Color* GetInitColor() = 0;

	/**
	* 设置初始颜色
	* @param color
	*/
	virtual void SetInitColor(const Color &color) = 0;

	virtual void SetInitAlpha(float alpha) = 0;

	/**
	* 设置默认初始高亮状态
	*/
	virtual void SetInitHightlight(bool isHighlight) = 0;
	/**
	* 获取当前高亮状态
	*/
	virtual bool IsHightlight() = 0;

	/**
	* 设置颜色为初始颜色
	*/
	virtual void ResetColor() = 0;

	virtual void ResetAlpha() = 0;

	/**
	* 复位颜色，位置，可见性
	*/
	virtual void Restore() = 0;;

	/**
	* 设置对应的Node
	* @param node
	*/
	virtual void SetSceneNode(SceneNode* node) = 0;

	/**
	* 获得与之关联的Node
	* @return
	*/
	virtual SceneNode* GetSceneNode() = 0;

	/**
	* 设置材质
	* @param material
	*/
	virtual void SetMaterial(BaseMaterial* material) = 0;

	/**
	* 获取材质
	* @return
	*/
	virtual BaseMaterial* GetMaterial() = 0;

	/**
	* 运行剔除标识
	* @return
	*/
	virtual bool AllowExculding() = 0;
	/**
	* 设置允许剔除标识
	* @param allow
	*/
	virtual void SetAlloewExculding(bool allow) = 0;

	/**
	* 获取渲染过程是否可见
	* @return
	*/
	virtual bool RendreVisible() = 0;
	/**
	* 设置渲染过程是否可见
	* @param visible
	*/
	virtual void SetRenderVisible(bool visible) = 0;

	/**
	* 获取体积值
	* @return
	*/
	virtual float GetVolumeAndArea(float& volume, float& area) = 0;

	/**
	* 获取几何属性信息，格式化为字符串后进行传递
	* @return
	*/
	virtual string GetGeometryInfo() = 0;

	virtual void FindVisiableObject(RenderAction* renderAction) = 0;

	virtual void Traverse(Action* action) =0;

	/**
	* 获SVLX文件对象的Id
	* @return
	*/
	virtual int GetSVLId()= 0;
	/**
	* 设置SVLX模型对象的Id
	* @return
	*/
	virtual void SetSVLId(int Id) = 0;
};
}
#endif // SSHAPENODE_H_

