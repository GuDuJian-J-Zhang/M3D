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
#include "m3d/model/IShape.h"
#include "m3d/base/Color.h"
#include "m3d/base/BoundingBox.h"
#include "m3d/base/Object.h"
#include "m3d/base/Mutex.h"

namespace M3D
{
class SceneManager;
class SceneNode;
class RayPickAction;
class RenderAction;
class BaseMaterial;
class ShapeSet;
class Action;
/**
 * @class Shape
 * @brief Shape类
 *
 * 基本图形的基类
 */
class M3D_API Shape: public Object ,public IShape
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

	virtual void SetType(SHAPETYPE type);

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
	virtual Color* GetDrawColor();

	/**
	 * 获取模型颜色
	 * @return
	 */
	virtual Color* GetColor();

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
	* 设置可见性
	* @param visible
	*/
	virtual void SetVisible(bool visible);

	/**
	 * 设置包围盒
	 * @param floats
	 */
	virtual void SetBox(const BoundingBox& box);

	/**
	 * 计算包围盒，派生类需重写
	 */
	virtual void ComputeBox();

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
	virtual bool IsOrigVisible();

	/**
	 * 设置原始可见性
	 * @param visible
	 */
	virtual void SetOrigVisible(bool visible);


	virtual void SetOrigVisible(bool visible, bool relSub);

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

	virtual void FramePick(RayPickAction* action);
	/**
	 * 获取属性
	 * @return
	 */
	virtual string GetProperties();

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

	virtual std::string GetName()
	{
		return this->m_Name;
	}

	virtual void SetName(const std::string& value)
	{
		this->m_Name = value;
	}

	/**
	 * 获取初始颜色
	 * @return
	 */
	virtual Color* GetInitColor()
	{
		//return &m_InitColor;
		return &this->m_Color;
	}
	virtual void SetInitAlpha(float alpha)
	{
		//m_InitColor.m_a = alpha;
		Shape::ResetColor();
	}

	virtual void SetInitHightlight(bool isHighlight) 
	{
		this->m_isHighlight = isHighlight;
	}

	virtual bool IsHightlight() 
	{
		return this->m_IsSelect || this->m_isHighlight ;
	}
	/**
	 * 设置初始颜色
	 * @param color
	 */
	virtual void SetInitColor(const Color &color)
	{
		//m_InitColor = color;
        Shape::ResetColor();
	}

	/**
	 * 设置颜色为初始颜色
	 */
	virtual void ResetColor(){
		//m_Color = m_InitColor;
	}

	virtual void ResetAlpha(){
		//m_Color.m_a = m_InitColor.m_a;
	}

	/**
	* 复位颜色，位置，可见性
	*/
	virtual void Restore();

	/**
	 * 设置对应的Node
	 * @param node
	 */
	virtual void SetSceneNode(SceneNode* node);

	/**
	 * 获得与之关联的Node
	 * @return
	 */
	virtual SceneNode* GetSceneNode();

		/**
	 * 设置材质
	 * @param material
	 */
	virtual void SetMaterial(BaseMaterial* material);

	/**
	 * 获取材质
	 * @return
	 */
	virtual BaseMaterial* GetMaterial();

	/**
	* 运行剔除标识
	* @return
	*/
	virtual bool AllowExculding();
	/**
	* 设置允许剔除标识
	* @param allow
	*/
	virtual void SetAlloewExculding(bool allow);

	/**
	* 获取渲染过程是否可见
	* @return
	*/
	virtual bool RendreVisible();
	/**
	* 设置渲染过程是否可见
	* @param visible
	*/
	virtual void SetRenderVisible(bool visible);

	/**
	* 获取体积值
	* @return
	*/
	virtual float GetVolumeAndArea(float& volume, float& area);

	/**
	 * 获取几何属性信息，格式化为字符串后进行传递
	 * @return
	 */
	virtual string GetGeometryInfo();
	virtual void Traverse(Action* action);
	virtual void FindVisiableObject(RenderAction* renderAction);

	void SetHasChild(bool useBox);
	bool GetHasChild();
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
	virtual void SetCopyObjId(IDTYPE objId);

	/**
	 * 获取复制对象的id
	 * @return
	 */
	virtual IDTYPE GetCopyObjId();

	/**
	* @brief 引用计数加一
	*/
	virtual void AddRef(void);
	/**
	* @brief 引用计数减一
	*/
	virtual void Release(void);	

	virtual int GetSVLId();
	virtual void SetSVLId(int Id);
	virtual void SetScene(SceneManager* scene);
private:
	void Init();

protected:
	//bool m_HasColor; //!<是否有颜色
	bool m_IsSelect; //!<是否选中
	bool m_Visible; //!<是否显示
	//bool m_origVisible;//!<读取时候的显示隐藏状态
	bool m_IsFirstGetProperties; //!<是否是第一次获取属性信息
	//bool m_hasChild; //!<存在下级带矩阵变换的shape
	bool m_allowExcluding;//!<允许剔除
	bool m_isHighlight;

	SceneNode* node;//!< 挂载节点
	BaseMaterial* m_material;//!<关联材质
	ShapeSet* m_parentNode;//!<置几何图形集

	//IDTYPE  m_copyId;//!<拷贝构造时，拷贝对象的id
	SHAPETYPE m_Type; //!<类型
	IDTYPE  m_Id; //!<编号

	Color m_Color; //!<颜色
	//Color m_InitColor;//!<初始颜色 模型读取时的初始颜色，在SView中不修改。

	//string m_Properties; //!<属性信息
	std::string m_Name; //!<名称

	//SHADEMODE m_shaderMode; //!< 着色方式

	//bool m_renderVisible; //!< 是否在渲染过程中可见

    BoundingBox m_BoundingBox; //!<包围盒
	//mutable Mutex m_mutex; //!<线程锁对象

	//SceneManager* m_scene;
};

}
#endif // SSHAPENODE_H_

