/*
 * PolyLine.cpp
 *
 *  Created on: 2013-10-5
 *      Author: bing
 */
#include "m3d/M3D.h"
#include "m3d/model/PolyLine.h"
#include "m3d/model/Shape.h"

namespace M3D
{

PolyLine::PolyLine():Shape()
{

}

PolyLine::~PolyLine()
{

}
//设置buffer，copy数据
void PolyLine::SetBuffer(int vertexNum, float* vertexBuf, int indexNum,
		M3D_INDEX_TYPE* indexBuf)
{
	if(indexNum>0)
	{
		for (int i = 0;i<indexNum;i++)
		{
			Vector3 point(vertexBuf[indexBuf[i] *3], vertexBuf[indexBuf[i] * 3+1],vertexBuf[indexBuf[i] * 3+2]);
			this->AddPoint(point);
		}
	}
	else
	{
		indexNum = vertexNum;
		for (int i = 0; i < indexNum; i++)
		{
			Vector3 point(vertexBuf[indexBuf[i] * 3], vertexBuf[indexBuf[i] * 3 + 1], vertexBuf[indexBuf[i] * 3 + 2]);
			this->AddPoint(point);
		}
	}
}

void PolyLine::ComputeBox()
{
	//float min[3] =
	//{ 0 };
	//float max[3] =
	//{ 0 };

	////point
	//for (int i = 0; i < m_VertexNum; i++)
	//{
	//	if (i == 0)
	//	{
	//		min[0] = m_VertexBuf[i * 3];
	//		min[1] = m_VertexBuf[i * 3 + 1];
	//		min[2] = m_VertexBuf[i * 3 + 2];

	//		max[0] = m_VertexBuf[i * 3];
	//		max[1] = m_VertexBuf[i * 3 + 1];
	//		max[2] = m_VertexBuf[i * 3 + 2];

	//		continue;
	//	}

	//	if (m_VertexBuf[i * 3] > max[0])
	//		max[0] = m_VertexBuf[i * 3];
	//	if (m_VertexBuf[i * 3 + 1] > max[1])
	//		max[1] = m_VertexBuf[i * 3 + 1];
	//	if (m_VertexBuf[i * 3 + 2] > max[2])
	//		max[2] = m_VertexBuf[i * 3 + 2];

	//	if (m_VertexBuf[i * 3] < min[0])
	//		min[0] = m_VertexBuf[i * 3];
	//	if (m_VertexBuf[i * 3 + 1] < min[1])
	//		min[1] = m_VertexBuf[i * 3 + 1];
	//	if (m_VertexBuf[i * 3 + 2] < min[2])
	//		min[2] = m_VertexBuf[i * 3 + 2];
	//}
	this->m_BoundingBox.Clear();
	for (int i=0;i<this->m_PointList.size();i++)
	{
		this->m_BoundingBox.Merge(this->m_PointList.at(i));
	}
	//m_BoundingBox.Define(Vector3(min[0], min[1], min[2]),Vector3(max[0], max[1], max[2]));
}

} /* namespace M3D */
