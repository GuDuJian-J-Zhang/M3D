/**@file
 *@brief
 *@author	CDJ
 *@date		2016-5-20
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_SECTIONLINE_H_
#define M3D_SECTIONLINE_H_
#include "m3d/M3D.h"
#include "m3d/base/Vector3.h"
#include "m3d/base/Color.h"
#include "m3d/base/Mutex.h"
#include "m3d/base/BoundingBox.h"
namespace M3D
{
	class Model;
	/**
	 * @class
	 * @brief 剖切线
	 */
class M3D_API SectionLine
{
public:
	SectionLine();
	virtual ~SectionLine();
	/**
	 *
	 * @param model
	 */
	void SetModel(Model * model);
	/**
	 *
	 * @return
	 */
	Model * GetModel();
	/**
	 *
	 * @return
	 */
	BoundingBox& GetBoundingBox();
	/**
	 * 获取剖切线点数据
	 * @return
	 */
	vector<Vector3> * GetData();
	void SetData(vector<Vector3> * data);
	/**
	 * 设置顶点数量
	 * @param count
	 */
	void SetVertexNum(int count);
	int GetVertexNum();

	/**
	 * 转换到指定平面的旋转四元数
	 * @return
	 */
	Quaternion & GetRotate();

	/**
	 * 设置父剖切线
	 * @param sectionLine
	 */
	void SetParent(SectionLine* sectionLine);
	/**
	 * 添加内部剖切线(下级剖切线)
	 * @param sectionLine
	 */
	void AddChild(SectionLine* sectionLine);
	/**
	 * 是否在sectionLine 内部
	 * @param sectionLine
	 * @return
	 */
	bool IsInside(SectionLine* sectionLine);
	/**
	 * 获取所有的自剖切线
	 * @return
	 */
	vector<SectionLine*>& GetAllChildren();

	/**
	 * 计算包围盒
	 */
	void CaculateBoundingBox();

	/**
	 * 设置在剖切线树中的深度
	 * @param depth
	 */
	void SetDepth(int depth);
	/**
	 *
	 * @return
	 */
	int GetDepth();


private:

	vector<Vector3> *m_data;//!<剖切线数据

	int m_dataVertexNum;//!<数据长度(顶点个数)

	vector<SectionLine*> m_children;//!<下级剖切线

	SectionLine * m_parent;	//!<上级剖切线

	int m_depth;//!<剖切线树的深度

	BoundingBox m_box;//!<剖切线组成的多边形的包围盒,此包围盒必须是转换到平面后的点求出的

	Matrix4 *m_transMatrix;//!<转换到二维平面的矩阵

	Model * m_model;	//!<关联的model

	Quaternion m_rotate;//!<变换到二维时的旋转四元数

	Vector3 m_center;//!<重心


};

} /* namespace M3D */
#endif /* M3D_SECTIONLINE_H_ */
