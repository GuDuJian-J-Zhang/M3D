/**@file
 *@brief	边类头文件
 *@author
 *@date		2013-7-10
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *Face类头文件
 *
 */

#ifndef M3D_EDGE_H_
#define M3D_EDGE_H_
#include "m3d/model/IShape.h"
#include "m3d/model/Curve.h"
#include "m3d/graphics/Renderable.h"
namespace M3D
{
class Face;
class Body;
class RayPickAction;
class RenderAction;
/**
 * @class Face
 * @brief Face类
 *
 * 提供Face类的相关方法
 */
class M3D_API Edge: public Object,public IShape, public Renderable
{
public:
	Edge();
	virtual ~Edge();
	Edge(const Edge& orig);
	Edge& operator =(const Edge& orig);
	void AddData(RefPolyLine * m_lineSet, int lodLevel = 0);
	void SetFace(Face* face);
	Face* GetFace();
	void SetBody(Body* face);
	Body* GetBody();

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
	void Selectable(bool selectable);
	bool Selectable();
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
	/**
	 * 得到显示数据
	 * @return
	 */
	RefPolyLine* GetLineData();
	/**
	 * 得到几何属性
	 * @return
	 */
	virtual string GetGeoInfo();

	virtual const  Color& GetRenderColor();

	void SetNeedClip(bool val);
	bool GetNeedClip();
private:
	///1级TriMeshData	
	bool m_visible;	
	bool m_IsHighlight;
	bool m_selectable;
	RefPolyLine * m_lineSet;
	mutable Color m_Color; ///颜色
	bool m_IsSelect; ///是否选中
	IDTYPE m_Id; ///编号
	Face* face; //!< 上级面
	Body* body; //!<上级Body
	int m_svlId;
	string m_Properties; //!<属性信息
	bool m_IsFirstGetProperties; //!<是否是第一次获取属性信息
	bool m_bNeedClip;
protected:

};

}

#endif /*EDGE_H_*/

