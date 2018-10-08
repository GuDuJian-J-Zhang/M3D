/**@file
 *@brief	Face类头文件
 *@author
 *@date		2013-7-10
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *Face类头文件
 *
 */

#ifndef M3D_EDGE_H_
#define M3D_EDGE_H_
#include "m3d/model/Shape.h"
#include "m3d/model/Curve.h"
#include "m3d/action/RenderAction.h"
#include "m3d/graphics/Renderable.h"
namespace M3D
{
class Face;
class Body;
class RayPickAction;
/**
 * @class Face
 * @brief Face类
 *
 * 提供Face类的相关方法
 */
class M3D_API Edge: public Shape, public Renderable
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
	 * 计算包围盒，派生类需重写
	 */
	virtual void ComputeBox();

	virtual void RayPick(RayPickAction* action);

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

	virtual void InitProperties();

private:
	///1级TriMeshData
	RefPolyLine * m_lineSet;

	Face* face; //!< 上级面
	Body* body; //!<上级Body
protected:

};

}

#endif /*EDGE_H_*/

