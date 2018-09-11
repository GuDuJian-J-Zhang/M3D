/*
 * SectionLine.cpp
 *
 *  Created on: 2016-5-20
 *      Author: CDJ
 */

#include "m3d/graphics/SectionLine.h"

namespace M3D
{

SectionLine::SectionLine()
{

	m_data = NULL;
	m_parent = NULL;
	m_depth = -1;

}

SectionLine::~SectionLine()
{

}


void SectionLine::SetModel(Model* model)
{
}

Model* SectionLine::GetModel()
{
	Model * ret = NULL;
	return ret;
}

BoundingBox& SectionLine::GetBoundingBox()
{
	return m_box;
}

vector<Vector3> * SectionLine::GetData()
{
	return m_data;
}

void SectionLine::SetData(vector<Vector3> * data)
{
	m_data = data;
}

void SectionLine::SetVertexNum(int count)
{
	m_dataVertexNum = count;
}
int SectionLine::GetVertexNum()
{
	return m_dataVertexNum;
}

void SectionLine::SetParent(SectionLine* sectionLine)
{
	this->m_parent = sectionLine;
}

void SectionLine::AddChild(SectionLine* sectionLine)
{
	this->m_children.push_back(sectionLine);
}

bool SectionLine::IsInside(SectionLine* sectionLine)
{
	bool ret = false;
	if(sectionLine && this != sectionLine)
	{
//		if(this->m_box.IsInside(sectionLine->m_box) == INSIDE)
//		{
//			if(this ==sectionLine )
//			{
//				LOGE("can not equal");
//			}
////			LOGE("Inside");
//			ret = true;
//		}
		BoundingBox & box = sectionLine->m_box;
		if (box.m_min.m_x < m_box.m_min.m_x && box.m_max.m_x > m_box.m_max.m_x &&
				 box.m_min.m_y < m_box.m_min.m_y && box.m_max.m_y > m_box.m_max.m_y
				)
		{
			ret = true;
		}

	}

	return ret;
}

vector<SectionLine*>& SectionLine::GetAllChildren()
{
	return this->m_children;
}

Quaternion& SectionLine::GetRotate()
{
	return m_rotate;
}

///转化到二维后，才是正确的包围盒
void SectionLine::CaculateBoundingBox()
{
	if (m_data)
	{
		m_box.Define(&(m_data->at(0)), m_data->size());
	}
}

void SectionLine::SetDepth(int depth)
{
	m_depth = depth;
}
int SectionLine::GetDepth()
{
	return m_depth;
}


}
 /* namespace M3D */
