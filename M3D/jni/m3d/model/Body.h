/**@file
 *@brief	Body类头文件
 *@author	xuli
 *@date		2013-6-7
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *Body类头文件
 *
 */

#ifndef M3D_BODY_H_
#define M3D_BODY_H_

#include "m3d/model/Shape.h"
#include "m3d/model/LODData.h"
#include "m3d/action/RenderAction.h"
#include "m3d/graphics/Renderable.h"
#include "m3d/model/Point.h"

namespace M3D
{
class Model;
class Face;
class Edge;
class IVertexSet;
class SPolyLine;
/**@class Body
 * @brief Body类
 *
 * 提供Body类的相关方法
 */
class Body: public Shape, public LOD, public Renderable
{
public:
	Body();
	virtual ~Body();

	/**
	 * 响应优化内部存储数据的内存占用
	 */
	virtual void OnOptimize();

	Body(const Body& orig);
	Body& operator =(const Body& orig);

	void SetModel(Model* model);
	Model* GetModel();

	/**
	 * @brief 新增一个Face对象
	 * @param face 要增加的Face对象指针
	 */
	void AddFace(Face* face);
	vector<Face*>& GetFaces();

	/**
	 * 添加Edge对象，到Body中
	 * @param edge
	 */
	void AddEdge(Edge* edge);
	vector<Edge*>& GetEdges();

	/**
	 * 添加Point对象到BOdy中
	 * @param pnt
	 */
	void AddPoint(Point* pnt);
	vector<Point*>& GetPoints();

	/**
	 * 初始属性信息
	 */
	virtual void InitProperties();

	///shape virtual function
	virtual void Restore();
	virtual void ResetColor();
    virtual void SetInitColor(const Color &color);
	virtual void ResetAlpha();
    virtual void SetColor(const Color &color);
    virtual void SetMaterial(Material* material);
	virtual void ComputeBox();
	virtual void RayPick(RayPickAction* action);
	virtual void SetSelected(bool select);
	virtual void SetAlpha(float a);
	virtual void SetVisible(bool visible);
	virtual void FindVisiableObject(RenderAction* renderAction);

	/**
	 * 统计body下所有Mesh点的总个数，level为LOD等级
	 * @param level
	 * @return
	 */
	int GetVertexCount(int level);

	/**
	 * 设置用来存储edge折线数据的polyLine
	 * @param polyLine
	 */
	void SetPolyLine(SPolyLine* polyLine);

	/**
	 * 获取用来存储edge折线数据的polyLine
	 * @return
	 */
	SPolyLine* GetPolyLine();

	/**
	 * 更新Body下所有对象的ID至上层Model对象中，主要用来管理Catia模式下对象的关联
	 * @param model
	 */
	void  UpdateContainsShapesId(Model* model);

	/**
	 * 获取体积值
	 * @return
	 */
	virtual float GetVolumeAndArea(float& volume,float& area);

private:
	vector<Face*> m_FaceArray;//!<body的所有face列表
	vector<Edge*> m_EdgeArray;//!<body中所有的边界线
	vector<Point*> m_pointArray;//!<body中所有的点信息
	SPolyLine* m_polyLine;//!<Body下所有的边界线数据以顺序追加的方式存储在此变量中
	Model* model;//!<上级model
};
}
#endif /*BODY_H_*/

