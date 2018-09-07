/*
 * ComText.cpp
 *
 *  Created on: 2014-2-20
 *      Author: bing
 */

#include "ComText.h"
#include "m3d/model/MeshData.h"
#include "m3d/utils/MeshHelper.h"
#include "m3d/action/RenderAction.h"
namespace M3D
{

ComText::ComText():Shape() {
	m_isDirty = true;
	Init();
}

///TODO
ComText::~ComText() {
	for(int i=0;i<m_CTextList.size();i++)
	{
		delete m_CTextList[i];
	}

	if(m_VertexSet)
	{
		m_VertexSet->Release();
		m_VertexSet = NULL;
	}

	if(m_Meshdata)
	{
		m_Meshdata->Release();
		m_Meshdata = NULL;
	}

}

CText* ComText::GetCText(int pos)
{
	return m_CTextList.at(pos);

}

void ComText::AddCText(CText* ct)
{
	m_CTextList.push_back(ct);
}


const Color & ComText::GetShapeColor()
{
	return *this->GetColor();
}

void ComText::Init()
{
	m_VertexSet = new VertexSet();
	m_VertexSet->AddRef();
	m_Meshdata = new Mesh(m_VertexSet);
	m_Meshdata->AddRef();

	this->MarkDirty();
}


void ComText::MarkDirty()
{
	m_isDirty = true;
}


Mesh* ComText::GetMesh()
{
	if(this->m_isDirty)
	{
		this->UpDataMesh();
		this->m_isDirty = false;
	}
	return (this->m_Meshdata);
}

/**
 * 计算包围盒，派生类需重写
 */
void ComText::ComputeBox()
{
	if(this->GetMesh())
	{
		this->SetBox(this->GetMesh()->GetBoundingBox());
	}
}

void ComText::UpDataMesh()
{
	MutexLock lock(m_mutex);

	this->m_BoundingBox.Clear();

	this->m_Meshdata->Clear();
	this->m_VertexSet->Clear();

	this->m_VertexSet->SetUseIndex(false);

	FTFont* font = CText::GetRender();
	if (!font)
	{
		return;
	}
	vector<Vector3>* comTextsPnts = this->m_VertexSet->GetPositionArray();
//	LOGI("ComText::UpDataMesh cTextCount:%d ",m_CTextList.size());

	//遍历所有的CText，根据CText的内容构建文本
	for(int i = 0; i < m_CTextList.size(); i++)
	{
		//存储所有的顶点
		VertexSet vertexSet;
		Mesh meshdata(NULL);

		CText* text = m_CTextList[i];

		string& textStr = text->GetText();

//		LOGI("ComText::UpDataMesh begin GetTextsMesh:%s type:%d",textStr.c_str());

		if (MeshHelper::GetTextsMesh(textStr, vertexSet, meshdata, *font))
		{
			//将字体大小缩放到合适比例
			float Height, Width, ScaleH, ScaleW;
			text->GetCharWidthHeight(Width, Height);
//			LOGI("ComText::UpDataMesh i:%d cText:%s height:%f width:%f",i,textStr.c_str(),Height,Width);
			ScaleH = Height;
			ScaleW =ScaleH;

			Vector3 textPos = text->GetInnerLoc();

			Vector3 tmpX,tmpY;
			text->GetInnerXYAxis(tmpX,tmpY);
			Vector3 tmpZ(0.0,0.0,1.0);
//			LOGI("ComText::UpDataMesh xAxis:%f %f %f  yAxis:%f %f %f",
//					tmpX.m_x,tmpX.m_y,tmpX.m_z,tmpY.m_x,tmpY.m_y,tmpY.m_z);
			//Quaternion rotate(tmpX,tmpY,tmpZ);// = Quaternion::IDENTITY
			Quaternion rotate(Vector3(1,0,0),tmpX);// = Quaternion::IDENTITY

//			Matrix3x4 controlMatrix;
//			controlMatrix.MultiTranslate(textPos);
//			controlMatrix.MultiScale(Vector3(ScaleW, ScaleH, 1.0f));
//			controlMatrix.MultiRotatiton(rotate);


		Matrix3x4 controlMatrix(textPos, rotate,
					Vector3(ScaleW, ScaleH, 1.0f)); //控制字体的实际大小


			Vector3 textPnt;
			vector<Vector3>* pnts = vertexSet.GetPositionArray();
			//解索引数据，按照数组的方式进行存储
			for (int i = 0; i < pnts->size(); ++i)
			{
				//根据文本的起始位置，对分元后的点进行偏移
				textPnt = controlMatrix * ((*pnts)[i]);
				comTextsPnts->push_back(textPnt);
			}

			//设置显示数据的有效长度，原有长度加上新的网格的有效数据长度
			m_Meshdata->SetDataLength(
					m_Meshdata->GetDataLength() + meshdata.GetDataLength());

			//
//			LOGI("ComText::UpDataMesh i:%d END.",i);
		}
	}

	m_Meshdata->SetDataLength(comTextsPnts->size());
	m_Meshdata->SetDataOffset(0);

}

}
