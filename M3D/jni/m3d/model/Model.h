/**@file
 *@brief	Model类头文件
 *@author
 *@date		2013-7-10
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *Model类头文件
 *
 */

#ifndef M3D_MODEL_H_
#define M3D_MODEL_H_
#include "m3d/M3DMacros.h"
#include "m3d/base/BoundingBox.h"
#include "m3d/action/RenderAction.h"
#include "m3d/graphics/Renderable.h"
#include "m3d/model/Shape.h"
class Stk_View;
namespace M3D
{
class VertexSet;
class Body;
class PMIData;
class Handler;
class ModelView;
class SectionPlane;
class Note;
class ShapeSet;
class ShapeProperty;

/**
 * @class Model
 * @brief Model类
 *
 * Model类
 */
class Model: public Shape, public Renderable
{
public:
	Model(void);
	virtual ~Model(void);
	/**
	 * 复位颜色，位置，可见性
	 */
	virtual void Restore();

	void Restore(bool resub);

	/**
	 * @see Shape
	 */
	virtual void RayPick(RayPickAction* action);
	virtual void SetSelected(bool select);
	virtual void SetAlpha(float a);
	virtual void SetAlpha(float a, bool relSub);
	virtual void SetVisible(bool visible);
	virtual void SetVisible(bool visible, bool relSub);
	virtual void ComputeBox();
    virtual void SetInitColor(const Color &color);
    virtual void SetMaterial(Material* material);
    virtual void SetColor(const Color &color);
    virtual void SetColor(const Color& color,bool reSub);
	virtual void ResetColor();
	virtual void ResetAlpha();
    
	virtual BoundingBox& GetWorldBoundingBox();
	/**
	 * 设置实例ID
	 * @param insID
	 */
    void SetInstanceID(int insID){m_InstanceID = insID;}
    /**
     * 获取实例ID
     * @return
     */
    int GetInstatnceID(){return m_InstanceID;}

	/**
	 * 获取模型信息
	 * @return
	 */
	virtual void InitProperties();

	/**
	 * @see Shape
	 * @param renderAction
	 */
	virtual void FindVisiableObject(RenderAction* renderAction);

	/**
	 * 增加子model
	 * @param model
	 */
	void AddSubModel(Model * model);

	/**
	 * 返回所有的子模型
	 * @return
	 */
	vector<Model*>& GetSubModels();

	/**
	 *  CDJ 删除子模型
	 * @param model
	 */
	void RemoveSubModel(Model * model);

	/**
	 * 解除模型关系 CDJ
	 * @param model
	 */
	void DetachSubModel(Model * model);

	/**
	 * 获取子model数量
	 * @return
	 */
	int GetSubModelCount();

	/**
	 * 增加body
	 * @param body
	 */
	void AddBody(Body* body);

	/**
	 * 获取实例个数
	 * @return
	 */
	int GetInstanceCount();

	/**
	 * 获取顶点个数
	 * @return
	 */
	int GetVertexCount(int level);

	/**
	 * 设置装配矩阵
	 * @param plcMatrix
	 */
	void SetPlaceMatrix(const Matrix4& plcMatrix);
	/**
	 * 得到装配矩阵
	 * @return
	 */
	const Matrix4&  GetPlaceMatrix();

	/**
	 * 获取父model
	 * @return
	 */
	Model* GetParent();

	/**
	 * 设置父model
	 * @param model
	 */
	void SetParent(Model* model);

	/**
	 * 更新模型信息
	 */
	void UpdataInfo();

	/**
	 * 清空model
	 */
	void Clear();

	//about view
	/**
	 * 获取模型的所有视图（用于输出视图到java端）
	 * @param idArray	视图id列表
	 * @param nameArray	视图名称列表
	 */
	void GetModleViews(vector<int> *idArray, vector<string> *nameArray);

	/**
	 * 根据id获取视图
	 * @param viewId
	 * @return 返回视图指针。如果未找到，则返回NULL
	 */
	ModelView* GetModleView(int viewId);

	/**
	 * 新增一个视图
	 * @param view
	 */
	void AddModelView(ModelView* view);

	/**
	 * 删除指定id的视图
	 * @param viewId
	 */
	void RemoveModelView(int viewId);

	/**
	 * 获取模型中所有视图的列表
	 * @return
	 */
	const vector<ModelView*>& GetModelViewList();

	//Plane & SectionPlane
	/**
	 * 新增一个剖视平面
	 * @param plane
	 */
	void AddSectionPlane(SectionPlane* plane);

	/**
	 * 根据id返回剖视平面
	 * @param id
	 * @return 剖视平面指针
	 */
	SectionPlane* GetSectionPlane(int id);

	/**
	 * 获取所有剖视平面的列表
	 * @return
	 */
	vector<SectionPlane*> GetSectionPlaneList();

	/**
	 * 删除指定id的剖视平面
	 * @param id
	 */
	void RemoveSectionPlane(int id);

	void SetBodys(vector<Body*>& bodys);

	vector<Body*>& GetBodys();

	/**
	 * 设置配置id
	 * @param plcId
	 */
	void SetPlcId(int plcId);

	/**
	 * 获取配置id
	 * @return
	 */
	int GetPlcId();

	/**
	 * 设置PMI
	 * @param pmis
	 */
	void SetPMIs(const map<int,PMIData*>& pmis);

	/**
	 * 获取PMI表
	 * @return
	 */
	map<int,PMIData*>& GetPMIs();

	/**
	 * 设置关联ProtoType的id
	 * @param protoTypeId
	 */
	void SetProtoTypeId(int protoTypeId);
	/**
	 * 获取关联ProtoTypede id
	 *
	 * @return
	 */
	int GetProtoTypeId();

	/**
	 * 纪录配置路径
	 * @param plcPath
	 */
	void SetPlcPath(const string& plcPath);
	/**
	 * 获取配置路径
	 * @return
	 */
	string& GetPlcPath();

	/**
	 *
	 * @param newRootCSN
	 */
	void SetShapeSet(ShapeSet *newRootCSN);
	/**
	 *
	 * @return
	 */
	ShapeSet* GetShapeCollection();

	//更新包含的shape对象
	void  UpdateContainsShapesId();
	void UpdateContainsShapes(long id,Shape* shape);
	//获指定id 的shape对象
	Shape* GetContainShape(long id);

	/**
	 *
	 * @return
	 */
	bool IsAssembly();

	/**
	 *
	 * @param subModelArray
	 */
	void GetAllSubModels(vector<Model*>& subModelArray);

	/**
	 *
	 * @param original
	 */
	void CopyData(Model * original);

	/**
	 *
	 * @return
	 */
	int GetUseablePlcId();

	void SetShapeProperty(ShapeProperty* shapeProperty);
	ShapeProperty* GetShapeProperty();

	/**
	 * 仅仅获取模型从svl文件中读取出来的属性
	 * @param key
	 * @return
	 */
	string GetPropertyFast(const string& key);

	/**
	 * 获取体积值
	 * @return
	 */
	virtual float GetVolumeAndArea(float& volume,float& area);
private:
	/**
	 * 对模型按照PlcID进行排序，保证装配树中显示顺序和转换器一致
	 */
	void ResortByPlcID();
	static bool CompareByPlcID(const Model* m1, const Model* m2);
private:
	int m_PlaceID; //!<配置ID
	int m_InstanceID;//!<ins ID
	string m_PlacePath;//!<配置路径
	Matrix4 m_plcMatrix; //!<配置矩阵
	int m_ProtoTypeID; //!<原型id

	Model* m_ParentModel; //!<父模型
	map<int,PMIData*> m_AllPMIMap; //!<所有PMI的列表 临时放Model下
	int m_PMICount; //!<PMI数量

	vector<Model *> m_SubModelArray; //!<子模型列表

	vector<Body*> m_BodyArray; //!<body数据
	bool m_IsFirstGetProperties; //!<是否首次获取属性信息
	vector<ModelView*> m_ModleViewList; //!<视图列表
	vector<SectionPlane*> m_SectionPlaneList; //!<剖视平面列表

	ShapeSet *m_RootCSN;//!< 几何图形集 根节点

	map<long,Shape*> m_containShapes; //!<模型下所有的shape对象

	ShapeProperty* m_shapeProperty;

	int m_userPlcId;//!< 更新配置id变量
};
}
#endif

