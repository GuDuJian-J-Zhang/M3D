/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *
 */
#ifndef     M3D_L_LSCENE_NODE_H_
#define     M3D_L_LSCENE_NODE_H_
#include "m3d/M3D.h"
#include "m3d/base/Object.h"
#include "m3d/M3DMacros.h"
#include "m3d/base/Vector3.h"
#include "m3d/base/Quaternion.h"
#include "m3d/base/Matrix4.h"
#include "m3d/utils/Trackball.h"
#include "m3d/renderer/GLESHead.h"

#include "m3d/base/BoundingBox.h"
#include "m3d/base/Color.h"
#include "m3d/base/Mutex.h"
#include "m3d/M3DMacros.h"

#include "m3d/action/Action.h"
#include "m3d/graphics/Renderable.h"

#include "m3d/renderer/RenderContext.h"

#include "m3d/M3DMacros.h"
#include "m3d/base/BoundingBox.h"

#include "m3d/graphics/Renderable.h"
#include "m3d/model/IShape.h"

#include "m3d/base/BoundingBox.h"
#include "m3d/base/Color.h"
#include "m3d/base/Mutex.h"
#include "m3d/M3DMacros.h"
#include "m3d/renderer/RenderContext.h"
#include "../scene/SceneNode.h"
 
namespace M3D
{
class Action;
class CallBackAction;
class PickAction;
class RayPickAction;
class Action;
class RenderAction;
class SerachAction;
class SceneNode;
class Quaternion;
class ModelShape;
class ImageModelShape;
class Face;
class Model;
class VertexSet;
class Body;
class PMIData;
class Handler;
class ModelView;
class SectionPlane;
class Note;
class ShapeSet;
class ShapeProperty;
class ExtendInfoManager;

class M3D_API SModelFileInfo {
public:
	SModelFileInfo(M3D::Model* model);
	~SModelFileInfo();

	int GetPartModelId();
	void SetPartModelId(int stk_partModelId);

	void SetInstanceMaterialId(int materialId);
	int GetInstanceMaterialId();

private:
	int stk_instanceMaterialId;
	int stk_partModelId;		

};

class M3D_API ModelExtInfo
{
	friend class Model;
private:
	ModelExtInfo();
 
	string m_Properties; //!<属性信息
	//int m_InstanceID;

	ShapeProperty* m_shapeProperty;
	bool m_IsFirstGetProperties; //!<是否是第一次获取属性信息
											  
	//int m_ProtoTypeID; //!<原型id
	Matrix3x4 m_origPlcMatrix; //!<配置矩阵

	//std::string m_Name; //!<名称				
	string m_placePath;//!<配置路径			   
	bool m_origVisible;//!<读取时候的显示隐藏状态
	Color m_origColor;//!<读取模型默认原始颜色
public:
	virtual ~ModelExtInfo();
};

class M3D_API Model:public Object,public IShape
{
public:
	Model(void);
	virtual ~Model(void);

	//优化减少内存占用
	void Optimize();
	void ClearPlcPath(bool bSub = false);

	/**
	 * @see SceneNode
	 * @param action
	 */
	virtual void Traverse(Action* action);
 
	//virtual  void AddChild(Model* childModel);

	/**
	 * 得到世界坐标系的包围盒，供快速剔除，射线查询，八叉树剔除等使用
	 * @return
	 */
	virtual BoundingBox& GetWorldBoundingBox();

	virtual BoundingBox GetTotalWorldBoundingBox();
	virtual BoundingBox GetTotalBoundingBox();
	/**
	 * @see SceneNode
	 */
	void SetLocalTransform(const Matrix3x4& localMatrix);

	Vector3 GetWorldDirection() const;

	Matrix3x4 GetLocalTransform() const;

	/**
	 * Return parent space transform matrix.
	 */
	virtual Matrix3x4 GetTransform() const;
	/**
	 * @see SceneNode
	 */
	void SetWorldTransform(const Matrix3x4& worldMatrix);
	/**
	 * @see SceneNode
	 */
	Matrix3x4& GetWorldTransform() const;
	/**
	 * @see SceneNode
	 */
	void SetTransform(const Vector3& position, const Quaternion& rotation);
	/**
	 * @see SceneNode
	 */
	void SetTransform(const Vector3& position, const Quaternion& rotation,
			float scale);
	/**
	 * @see SceneNode
	 */
	void SetTransform(const Vector3& position, const Quaternion& rotation,
			const Vector3& scale);

	/**
	 *  Mark node and child nodes to need world transform recalculation. Notify listener components.
	 */
	void MarkDirty();

	/**
	 * 在子类中重写此方法，能够实现节点脏，通知需要改变的数据
	 */
	virtual void OnMarkedDirty();

	/**
	 * Return whether transform has changed and world transform needs recalculation.
	 * @return
	 */
	bool IsDirty() const;

	bool HasDrawData()const;

	/**
	 * 得到所有的孩子节点
	 * @return
	 */
	vector<Model*>& GetChildren();

	/**
	 * @see SceneNode
	 */
	virtual void UpdateWorldTransform() const;

	void UpdataHardWareBuffer();
	/**
	* @see Shape
	*/
	virtual void Restore();
	virtual void Restore(bool resub);
	virtual void ResetColor();
	virtual SHAPETYPE GetType(void);
	virtual void SetType(SHAPETYPE type);
	virtual void RayPick(RayPickAction* action);
	virtual void FramePick(RayPickAction* action);
	virtual void SetSelected(bool select);
	virtual void SetAlpha(float a);
	virtual void SetAlpha(float a, bool relSub);
	virtual void SetVisible(bool visible);
	virtual void SetVisible(bool visible, bool relSub);
	virtual void ComputeBox();
	virtual void SetInitColor(const Color &color);
	virtual void SetInitAlpha(float alpha);
	virtual void SetInitHightlight(bool isHighlight);
	virtual bool IsHightlight();
	virtual void SetColor(const Color &color);
	virtual void SetColor(const Color& color, bool reSub);
	virtual Color* GetDrawColor();
	virtual Color* GetColor();
	virtual void ResetAlpha();
	virtual float GetAlpha();
	virtual bool IsSelected() const;

	void Selectable(bool selectable,bool reSub = false);
	bool Selectable();

	virtual void SetBox(const BoundingBox& box);
	virtual bool IsVisible();
	virtual bool IsOrigVisible();
	virtual void SetOrigVisible(bool visible);
	virtual void SetOrigVisible(bool visible, bool relSub);
	//void SetInitVisble(bool visible);
	virtual BoundingBox& GetBoundingBox();
	virtual string GetProperties();
	virtual void InitProperties();
	virtual void AddProperty(string key, string value);
	virtual void ClearProperties();
	virtual string GetStatistics();
	virtual IDTYPE GetID();
	virtual void SetID(IDTYPE  value);
	virtual std::string GetName();
	virtual void SetName(const std::string& value);
	virtual Color* GetInitColor();

	virtual void SetSceneNode(SceneNode* node);
	virtual SceneNode* GetSceneNode();
	virtual void SetMaterial(BaseMaterial* material);
	virtual void SetMaterialForLastBody(BaseMaterial* material);
	virtual BaseMaterial* GetMaterial();
	virtual bool	HasMaterial();
	virtual bool AllowExculding();
	virtual void SetAlloewExculding(bool allow);
	virtual bool RendreVisible();
	virtual void SetRenderVisible(bool visible);
	virtual float GetVolumeAndArea(float& volume, float& area);
	virtual string GetGeometryInfo();

	virtual void AddRef(void);
	virtual void Release(void);
	virtual int GetSVLId();
	virtual void SetSVLId(int Id);	

	void MarkDrawDataDirty(bool relSub);

	virtual void FindVisiableObject(RenderAction* renderAction);

	/**
	* 重置外部作用的平移旋转和缩放
	*/
	void ResetMovement();

	/**
	* @brief Move the scene node in the chosen transform space.
	*/
	virtual void Translate(const Vector3& delta, TransformSpace space = TS_LOCAL);

	virtual void Rotate(const Quaternion& delta, TransformSpace space = TS_LOCAL);

	virtual void Scale(const Vector3&  delta, TransformSpace space = TS_LOCAL);

	virtual void SetPosition(const Vector3& delta);

    Vector3 GetPosition();
    
	virtual void SetRotation(const Quaternion& delta);

	virtual void SetScale(const Vector3&  delta);
 
	virtual void SetWorldPosition(const Vector3& position);

	void SetWorldRotation(const Quaternion& rotation);

	void SetWorldDirection(const Vector3& direction);

	void SetWorldScale(float scale);

	void SetWorldScale(const Vector3& scale);	
	/**
		* @brief Return scale in world space.
		*/
	Vector3 GetWorldScale() const;

	M3D::Vector3 GetWorldPosition() const;

	bool LookAt(const Vector3& target, const Vector3& up, TransformSpace space = TS_WORLD);

	/**
	* @brief Return rotation in world space.
	*/
	Quaternion GetWorldRotation() const;
	/**
	* 设置实例ID
	* @param insID
	*/
	virtual void SetInstanceID(int insID);
	/**
	* 获取实例ID
	* @return
	*/
	virtual int GetInstatnceID();

	/**
	* 增加子model
	* @param model
	*/
	virtual void AddSubModel(Model * model);

	/**
	* 返回所有的子模型
	* @return
	*/
	virtual vector<Model*>& GetSubModels();

	/**
	*  CDJ 删除子模型
	* @param model
	*/
	virtual void RemoveSubModel(Model * model);

	Model* GetModelByName(const string& name);

	/**
	* 解除模型关系 CDJ
	* @param model
	*/
	virtual void DetachSubModel(Model * model);

	/**
	* 获取子model数量
	* @return
	*/
	virtual int GetSubModelCount();

	/**
	* 增加body
	* @param body
	*/
	virtual void AddBody(Body* body);

	/**
	* 获取实例个数
	* @return
	*/
	virtual int GetInstanceCount();

	/**
	* 获取顶点个数
	* @return
	*/
	virtual int GetVertexCount(int level);

	/**
	* 获取面个数
	* @return
	*/
	virtual int GetFaceCount();

	/**
	* 设置装配矩阵
	* @param plcMatrix
	*/
	virtual void SetPlaceMatrix(const Matrix3x4& plcMatrix);
	/**
	* 得到装配矩阵
	* @return
	*/
	virtual Matrix3x4* GetPlaceMatrix() const;

	/**
	* 获取父model
	* @return
	*/
	virtual Model* GetParent();

	/**
	* 设置父model
	* @param model
	*/
	virtual void SetParent(Model* model);

	/**
	* 更新模型信息
	*/
	virtual void UpdataInfo();

	/**
	* 清空model
	*/
	virtual void Clear();

	//about view
	/**
	* 获取模型的所有视图（用于输出视图到java端）
	* @param idArray	视图id列表
	* @param nameArray	视图名称列表
	*/
	virtual void GetModleViews(vector<int> *idArray, vector<string> *nameArray);

	/**
	* 根据id获取视图
	* @param viewId
	* @return 返回视图指针。如果未找到，则返回NULL
	*/
	virtual ModelView* GetModleView(int viewId);

	/**
	* 插入一个视图
	* @param iIndex 索引
	* @param view
	*/
	void InsertModelView(int iIndex, ModelView* view);
	/**
	* 新增一个视图
	* @param view
	*/
	virtual void AddModelView(ModelView* view);

	/**
	* 删除指定id的视图
	* @param viewId
	*/
	virtual void RemoveModelView(int viewId);

	/**
	* 调整自定义视图的顺序
	* @param viewId
	*/
	void MoveModelView(int viewId, int preViewId);

	/**
	* 获取模型中所有视图的列表
	* @return
	*/
	virtual vector<ModelView*>* GetModelViewList();

	//Plane & SectionPlane
	/**
	* 新增一个剖视平面
	* @param plane
	*/
	virtual void AddSectionPlane(SectionPlane* plane);

	/**
	* 根据id返回剖视平面
	* @param id
	* @return 剖视平面指针
	*/
	virtual SectionPlane* GetSectionPlane(int id);

	/**
	* 获取所有剖视平面的列表
	* @return
	*/
	virtual vector<SectionPlane*>* GetSectionPlaneList();

	/**
	* 删除指定id的剖视平面
	* @param id
	*/
	virtual void RemoveSectionPlane(int id);

	virtual void SetBodys(vector<Body*>& bodys);

	virtual vector<Body*>* GetBodys();

	void ClearMeshData();
	//通过索引的方式设置数据
	void AddMeshData(vector<Vector3>& points, vector<Vector3>& normals, vector<Vector3>& uvs,vector<M3D_INDEX_TYPE>& indexs);
	//使用非索引的方式设置数据
	void AddMeshData(vector<Vector3>& points, vector<Vector3>& normals, vector<Vector3>& uvs);

	void ClearLineData();

	//通过索引的方式设置数据
	void AddLineData(vector<Vector3>& points, vector<M3D_INDEX_TYPE>& indexs);
	//使用非索引的方式设置数据
	//void AddLineData(vector<Vector3>& points);
	void AddLineData(vector<Vector3>& points, Color color = Color::BLACK, bool dotted = false);
	/**
	* 设置配置id
	* @param plcId
	*/
	virtual void SetPlcId(int plcId);

	/**
	* 获取配置id
	* @return
	*/
	virtual int GetPlcId();

	/**
	* 设置PMI
	* @param pmis
	*/
	virtual void SetPMIs(const map<int, PMIData*>& pmis);

	/**
	* 获取PMI表
	* @return
	*/
	virtual map<int, PMIData*>* GetPMIs();

	/**
	* 设置关联ProtoType的id
	* @param protoTypeId
	*/
	virtual void SetProtoTypeId(int protoTypeId);
	/**
	* 获取关联ProtoTypede id
	*
	* @return
	*/
	virtual int GetProtoTypeId();

	/**
	* 纪录配置路径
	* @param plcPath
	*/
	virtual void SetPlcPath(const string& plcPath);
	/**
	* 获取配置路径
	* @return
	*/
	virtual const string& GetPlcPath();

	/**
	*
	* @param newRootCSN
	*/
	virtual void SetShapeSet(ShapeSet *shapeSet);
	/**
	*
	* @return
	*/
	virtual ShapeSet* GetShapeCollection();

	//更新包含的shape对象
	virtual void  UpdateContainsShapesId();
	virtual void UpdateContainsShapes(long id, IShape* shape);
	//获指定id 的shape对象
	virtual IShape* GetContainShape(long id);

	/**
	*
	* @return
	*/
	virtual bool IsAssembly();

	/**
	*
	* @param subModelArray
	*/
	virtual void GetAllSubModels(vector<Model*>& subModelArray);

	/**
	*
	* @param original
	*/
	virtual void CopyData(Model * original);

	/**
	*
	* @return
	*/
	virtual int GetUseablePlcId();

	virtual void SetShapeProperty(ShapeProperty* shapeProperty);
	virtual ShapeProperty* GetShapeProperty();

	/**
	* 仅仅获取模型从svl文件中读取出来的属性
	* @param key
	* @return
	*/
	virtual string GetPropertyFast(const string& key);

	ModelShape* GetModelShape();

	void SetModelShape(ModelShape* modelshape);

	virtual SModelFileInfo* GetFileInfo();
	virtual void SetFileInfo(SModelFileInfo* fileInfo);
	virtual void ClearFileInfo();
	void SetModelExtInfo();
	void SetModelExtInfo(ExtendInfoManager* infoMgr);
	void SetModelExtInfo(ExtendInfoManager* infoMgr, bool relSub);
	void SetDrawDataPrepared(bool dataPrepared);
	void SetDrawDataPrepared(bool dataPrepared, bool resub);
	ExtendInfoManager* GetExtendInfoManager();
	Matrix3x4* GetOrigPlcMartirx();
	void SetOrigPlcMatrix(const Matrix3x4& plcMatrix);

	virtual void Reset();
	void* GetUserData() const { return m_userData; }
	void SetUserData(void* val) { m_userData = val; }

	bool GenTextureCoords(bool force /*= true*/);

	void SaveProperties(string& key, string& value);

	int GetBodyCount();

	void SetNeedClip(bool val);
	void SetOriginTransparency(float fTrans);
	float GetOriginTransparency();

	void AddBodyByFace(Face* face);
protected:
	ModelShape* m_modelShape;
	mutable bool m_dirty; //!< @see SceneNode
	///World-space transform matrix.
	mutable Matrix3x4 m_worldMatrix; //!< @see SceneNode
	std::string m_Name; //!<名称	
private:
		mutable Matrix3x4 m_plcMatrix; //!<配置矩阵
		Model* m_ParentModel; //!<父模型
		vector<Model *> m_SubModelArray; //!<子模型列表

	protected:
		bool m_IsSelect; //!<是否选中
		bool m_Visible; //!<是否显示
		bool m_selectable; 

		IDTYPE  m_Id; //!<编号
		SModelFileInfo* m_fileInfo; 
		ModelExtInfo* m_modelExtInfo;
		ExtendInfoManager* m_ExtInfoMgr;

		int m_PlaceID; //!<配置ID	
		int m_InstanceID; //纪录实例的id，方便后期从文件中获取所需数据	
		int m_svlId;
		int m_ProtoTypeID; //纪律原型的id
		void* m_userData;
		float m_originTrans;
};


class M3D_API ImageModel :public Model {
public:
	ImageModel(void);
	virtual ~ImageModel(void);

	virtual SHAPETYPE GetType(void);

	void SetImagePath(const string& imagePath);
	string GetImagePath();

	void SetImageData(char* imageData, int dataLength);

	void SetImageSize(Vector3& position, Vector2& size);
	Vector2 GetImageSize();

	void SetImagePosition(Vector3& position);
	Vector3 GetImagePosition();

	void SetImageGPUID(unsigned int gupID);

	void SetFlipImage(bool flipImage);

	void SetAllowScall(bool allowScale);

	void SetAllowRotate(bool allowRotate);

	void  SetInTopShow(bool allowInTopShow);
 
	void SetAllowTran(bool allowTran);

	void SetFixShowInScreen(bool fixShow);

	void SetAllowClip(bool allowClip);

	ImageModelShape* GetImageModelShape();
private:
	void CreateModelShape();
};

class M3D_API SignModel :public Model {
public:
	SignModel(void);
	virtual ~SignModel(void);

	bool GetShowSimpleSign() const { return m_showSimpleSign; }
	virtual void SetShowSimpleSign(bool val);
	bool GetShowAllSign() const { return m_ShowAllSign; }
	void SetShowAllSign(bool val);

	virtual void OnMarkedDirty();
	bool GetNeedUpdataSign() const { return m_needUpdataSign; }
	void SetNeedUpdataSign(bool val) { m_needUpdataSign = val; }
	M3D::ImageModel* GetSimpleSignModel() const { return m_simpleSignModel; }
	void SetSimpleSignModel(M3D::ImageModel* val) { m_simpleSignModel = val; }
	M3D::Model* GetAllSignModel() const { return m_allSignModel; }
	void SetAllSignModel(M3D::Model* val) { m_allSignModel = val; }
protected:
	ImageModel* m_simpleSignModel;
	Model* m_allSignModel;

	bool m_needUpdataSign;//需要更新显示状态对应的数据
	bool m_showSimpleSign; //显示简单标记
	bool m_ShowAllSign; //显示完整标记
};

}
#endif // L_SCENE_NODE_H
