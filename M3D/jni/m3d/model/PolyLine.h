/**@file
 *@brief	PolyLine类头文件
 *@author	bing
 *@date		2013-10-5
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *PolyLine类头文件
 *
 */

#ifndef M3D_POLYLINE_H_
#define M3D_POLYLINE_H_
#include "m3d/model/Shape.h"

namespace M3D
{
/**@class PolyLine
 * @brief PolyLine类
 *
 * 表示一个多顶点折线
 */
class M3D_API PolyLine: public Shape
{
public:
	PolyLine();
	virtual ~PolyLine();

	void ComputeBox();

	/**
	 * 设置buff
	 * @param vertexNum
	 * @param vertexBuf
	 * @param indexNum
	 * @param indexBuf
	 */
	void SetBuffer(int vertexNum, float* vertexBuf, int indexNum,
			M3D_INDEX_TYPE* indexBuf);

	/**
	 * 获取buff
	 * @param outVertexNum
	 * @param outVertexBuf
	 * @param outIndexNum
	 * @param outIndexBuf
	 */
	void GetBuffer(int* outVertexNum, float** outVertexBuf, int* outIndexNum,
			M3D_INDEX_TYPE** outIndexBuf);

	/**
	 * 添加点到折线中
	 * @param pnt
	 */
	void AddPoint(Vector3 &pnt){m_PointList.push_back(pnt);}
	/**
	 * 获取这线中的所有点
	 * @return
	 */
	vector<Vector3> &GetPointList(){return m_PointList;}

private:

	float* m_VertexBuf; //!<顶点buf
	int m_VertexNum; //!<顶点数量
	M3D_INDEX_TYPE* m_IndexBuf; //!<索引buf
	int m_IndexNum; //!<索引数量

	vector<Vector3> m_PointList;//!<点列表
};

}
#endif /* POLYLINE_H_ */
