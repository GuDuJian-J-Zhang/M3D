/**@file
 *@brief	Face类头文件
 *@author
 *@date		2013-7-10
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *
 */

#ifndef M3D_FACE_H_
#define M3D_FACE_H_

#include "m3d/model/Shape.h"
#include "m3d/model/LODData.h"
#include "m3d/action/RenderAction.h"
#include "m3d/graphics/Renderable.h"
namespace M3D
{
class Edge;
class Body;

class LFace: public Renderable, public Object, public IShape
{
public:
	LFace();
	virtual ~LFace();

	virtual SHAPETYPE GetType(void);

	BoundingBox GetBoundingBox();

	virtual void FindVisiableObject(RenderAction* renderAction);

	virtual void RayPick(RayPickAction* action);
	/**
	 * 得到渲染时使用的RefMesh
	 * @return
	 */
	Mesh* GetRenderMeshData();

	/**
	 * 设置渲染时使用的RefMesh
	 * @param meshCache
	 */
	void SetRenderMeshData(Mesh* meshCache);

	///Renderable virtual function
	/**
	 * 得到渲染时的颜色
	 * @return
	 */
	virtual const Color& GetRenderColor();

	void SetColor(const Color* color);

	virtual void SetSelected(bool isSelected);

	virtual IDTYPE GetID()
	{
		return this->m_Id;
	}

	virtual void SetID(IDTYPE value)
	{
		this->m_Id = value;
	}

	IShape* GetParent();

	void SetParent(IShape* shape);

private:
	bool m_visible;
	bool m_IsSelect; ///是否选中
	IDTYPE m_Id; ///编号
	Mesh* m_drawCache; //!<渲染时使用的Mesh
	Color m_Color; ///颜色
	IShape* m_parent;
};

/**@class Face
 * @brief Face类
 *
 * 提供Face类的相关方法
 */
class Face: public Shape, public LOD, public Renderable
{
public:
	Face();
	virtual ~Face();
	Face(const Face& orig);
	Face& operator =(const Face& orig);

	/**
	 * 设置上级Body
	 * @param body
	 */
	void SetBody(Body* body);

	/**
	 * 得到上级Body
	 * @return
	 */
	Body* GetBody();

	/**
	 * 添加边数据
	 * @param edge
	 * @param level 如果需要使用，指定级lod级别。
	 */
	void AddEdge(Edge* edge, int level = 0);

	/**
	 * 得到所有的边界线
	 * @return
	 */
	vector<Edge*>& GetEdges();

	///shape virtual functions
	virtual void ComputeBox();
	virtual void RayPick(RayPickAction* action);
	virtual void SetSelected(bool select);
	virtual void SetAlpha(float a);
	virtual void SetVisible(bool visible);
	virtual string GetGeoInfo();
	virtual void InitProperties();
	virtual void FindVisiableObject(RenderAction* renderAction);
	/**
	 * 得到Face下指定lod等级Mesh顶点的个数
	 * @param level lod等级
	 * @return
	 */
	int GetVertexCount(int level = 0);

	/**
	 * 得到渲染时使用的RefMesh
	 * @return
	 */
	Mesh* GetRenderMeshData();

	/**
	 * 设置渲染时使用的RefMesh
	 * @param meshCache
	 */
	void SetRenderMeshData(Mesh* meshCache);

	///Renderable virtual function
	/**
	 * 得到渲染时的颜色
	 * @return
	 */
	virtual const Color& GetRenderColor();

	virtual Material* GetRenderMaterial();

	virtual float GetVolumeAndArea(float& volume,float& area);

private:
	Body* body; //!<上级Body
	Mesh* m_drawCache; //!<渲染时使用的Mesh
	vector<Edge*> m_edges; //!<下级Edge
};
}

#endif /*M3D_FACE_H_*/

