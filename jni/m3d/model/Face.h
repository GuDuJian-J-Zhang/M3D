/**@file
 *@brief	Face类头文件
 *@author
 *@date		2013-7-10
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *
 */

#ifndef M3D_LFACE_H_
#define M3D_LFACE_H_
#include "m3d/model/LODData.h"
#include "m3d/model/IShape.h"
#include "m3d/graphics/Renderable.h"
namespace M3D
{
class Edge;
class Body;
class Action;
class RenderAction;
class ExtendInfoManager;
class GeometryAttribute;
class M3D_API FaceExtInfo
{
	friend class Face;
private:
	FaceExtInfo();

	Color m_InitColor;//!<初始颜色 模型读取时的初始颜色，在SView中不修改。
	bool m_origVisible;//!<读取时候的显示隐藏状态
	bool m_origHighlight;
	string m_Properties; //!<属性信息
	int m_InstanceID;
	bool m_IsFirstGetProperties; //!<是否是第一次获取属性信息
public:
	virtual ~FaceExtInfo();
};

class M3D_API Face:public Object, public Renderable,public IShape
{
public:
	Face();
	virtual ~Face();

	//bool UseExtInfo();

	//bool UnUseExtInfo();

	Face(const Face& orig);
	Face& operator =(const Face& orig);

	void SetData(Mesh* vertexData);
	Mesh* GetData();
	virtual void SetRenderMeshData(Mesh* meshCache);
 
	virtual bool Visible();
 
	virtual void SetBody(Body* body);

	virtual Body* GetBody();

	virtual void AddEdge(Edge* edge, int level = 0);
	virtual vector<Edge*>* GetEdges();

	virtual int GetVertexCount(int level = 0);
	virtual Mesh* GetRenderMeshData();
 
	virtual BaseMaterial* GetRenderMaterial();

	const Color& GetRenderColor();
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
	GeometryAttribute* GetGeoAttribute();

	virtual IDTYPE GetCopyObjId();
	virtual void SetFaceExtInfo();
private:
	static BoundingBox m_globalFaceBox;

private:
	bool m_visible;
	bool m_IsHighlight;
	bool m_IsSelect; ///是否选中
	IDTYPE m_Id; ///编号
	Mesh* m_drawCache; //!<渲染时使用的Mesh
	Mesh* m_drawMesh; //!<渲染时使用的Mesh
	mutable Color m_Color; ///颜色
	Body* m_body;
	BaseMaterial* m_material;//!<关联材质
	//ExtendInfoManager* m_ExtInfoMgr;
	FaceExtInfo* m_FaceExtInfo;	
	int m_svlId;
};
}

#endif /*M3D_FACE_H_*/

