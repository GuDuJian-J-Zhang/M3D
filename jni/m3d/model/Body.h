/**@file
 *@brief	Body类头文件
 *@author	xuli
 *@date		2013-6-7
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *Body类头文件
 *
 */

#ifndef M3D_LBODY_H_
#define M3D_LBODY_H_
#include "m3d/model/LODData.h"
#include "m3d/graphics/Renderable.h"


namespace M3D
{
class Model;
class Face;
class Edge;
class IVertexSet;
class SPolyLine;
class Action;
class RenderAction;
class ExtendInfoManager;
class Point;
class VertexSet;

class M3D_API BodyExtInfo
{
	friend class Body;
public:
	BodyExtInfo();
	virtual ~BodyExtInfo();

	bool m_origVisible;//!<读取时候的显示隐藏状态

	string m_Properties; //!<属性信息
	int m_InstanceID;
	bool m_IsFirstGetProperties; //!<是否是第一次获取属性信息
};
/**@class Body
 * @brief Body类
 *
 * 提供Body类的相关方法
 */
class M3D_API Body:public Object,public IShape
{
public:
	Body();
	virtual ~Body();

	bool UseExtInfo();

	bool UnUseExtInfo();

	/**
	* 响应优化内部存储数据的内存占用
	*/
	virtual void OnOptimize();

	Body(const Body& orig);
	Body& operator =(const Body& orig);

	virtual void SetModel(Model* model);
	virtual Model* GetModel();

	/**
	* @brief 新增一个Face对象
	* @param face 要增加的Face对象指针
	*/
	virtual void AddFace(Face* face);
	virtual vector<Face*>& GetFaces();

	/**
	* 添加Edge对象，到Body中
	* @param edge
	*/
	virtual void AddEdge(Edge* edge);
	virtual vector<Edge*>* GetEdges();

	/**
	* 添加Point对象到BOdy中
	* @param pnt
	*/
	virtual void AddPoint(Point* pnt);
	virtual vector<Point*>* GetPoints();

	/**
	* 统计body下所有Mesh点的总个数，level为LOD等级
	* @param level
	* @return
	*/
	virtual int GetVertexCount(int level);

	/**
	* 设置用来存储edge折线数据的polyLine
	* @param polyLine
	*/
	void SetPolyLine(SPolyLine* polyLine);

	/**
	* 获取用来存储edge折线数据的polyLine
	* @return
	*/
	virtual SPolyLine* GetPolyLine();

	/**
	* 更新Body下所有对象的ID至上层Model对象中，主要用来管理Catia模式下对象的关联
	* @param model
	*/
	virtual void  UpdateContainsShapesId(Model* model);

	void MarkDrawDataDirty();

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
	virtual void ResetAlpha();
	virtual Color* GetDrawColor();
	virtual Color* GetColor();
	virtual float GetAlpha();
	virtual bool IsSelected() const;
	virtual void SetBox(const BoundingBox& box);
	virtual bool IsVisible();
	virtual bool IsOrigVisible();
	virtual void SetOrigVisible(bool visible);
	virtual void SetOrigVisible(bool visible, bool relSub);
	virtual BoundingBox& GetBoundingBox();
	virtual string GetProperties();
	virtual void InitProperties();
	virtual void AddProperty(string key, string value);
	virtual void ClearProperties();
	virtual IDTYPE GetID();
	virtual void SetID(IDTYPE  value);
	virtual std::string GetName();
	virtual void SetName(const std::string& value);
	virtual Color* GetInitColor();

	virtual void SetSceneNode(SceneNode* node);
	virtual SceneNode* GetSceneNode();
	virtual void SetMaterial(BaseMaterial* material);
	virtual BaseMaterial* GetMaterial();
	virtual bool AllowExculding();
	virtual void SetAlloewExculding(bool allow);
	virtual bool RendreVisible();
	virtual void SetRenderVisible(bool visible);
	virtual float GetVolumeAndArea(float& volume, float& area);
	virtual string GetGeometryInfo();
	virtual void FindVisiableObject(RenderAction* renderAction);
	virtual void Traverse(Action* action);

	virtual void AddRef(void);
	virtual void Release(void);	
	virtual int GetSVLId();
	virtual void SetSVLId(int Id);

	int GetLodCount();

	virtual void SetBodyExtInfo();

	VertexSet* GetData(int lodLevel = 0);

	bool MergeFace();
	void ResizeMeshIndex(vector<Face*>* faceArray, unsigned int Offset1, unsigned int length1, unsigned int length2, bool value);
private:
	vector<Face*> m_FaceArray;//!<body的所有face列表
	vector<Edge*> m_EdgeArray;//!<body中所有的边界线
	SPolyLine* m_polyLine;//!<Body下所有的边界线数据以顺序追加的方式存储在此变量中
	Model* model;//!<上级model
	//ExtendInfoManager* m_ExtInfoMgr;
	BodyExtInfo* m_BodyExtInfo;
	bool m_isDrawDataDirty; //是否需要重新组织绘制数据	
	int m_svlId;
	IDTYPE  m_Id; //!<编号
};
}
#endif /*BODY_H_*/

