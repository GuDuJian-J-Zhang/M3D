/*
 * PMIData.cpp
 *
 *  Created on: 2013-9-29
 *      Author: bing
 */
#include "m3d/M3D.h"
#include "m3d/model/Shape.h"
#include "m3d/model/PMIData.h"
#include "m3d/model/PolyLine.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/graphics/ImageBoard.h"
#include "m3d/action/RenderAction.h"
#include "m3d/SceneManager.h"
#include "m3d/utils/ShapeHelper.h"
namespace M3D
{
int PMIData::FixScreen = 8000;

PMIData::PMIData():Shape()
{
	m_ProtoTypeID = -1; // 所属零件ID
	m_Type = -1; //PMI类型
	m_IsParallelScreen = false;
//	m_imageboard = NULL;
	m_DefPlaneDirty = true;
	fontSize = 1.0f;
}

PMIData::~PMIData()
{
//	if(m_imageboard)
//	{
//		m_imageboard->Release();
//		m_imageboard = NULL;
//	}

	for(int i=0;i<m_Lines.size();i++)
	{
		m_Lines[i]->Release();
	}
	m_Lines.clear();

	for(int i=0;i<m_ComTexts.size();i++)
	{
		m_ComTexts[i]->Release();
	}
	m_ComTexts.clear();
}

Matrix4* PMIData::GetOutFrameMatrix()
{
	MutexLock lock(m_mutex);
	return &this->m_outFrameMatrix;
}

void PMIData::RayPick(RayPickAction* action)
{

}

void PMIData::FindVisiableObject(RenderAction* renderAction)
{
	Matrix3x4* worldMatrix = renderAction->GetWorldMatrix();

	if(this->m_DefPlaneDirty)
	{
		MutexLock lock(m_mutex);
		this->m_DefPlaneDirty = false;
		m_WorldMatrix = (*worldMatrix)*(this->GetDefPlane());
		m_GLDefPlaneMatrix = m_WorldMatrix.ToMatrix4().Transpose();
		this->SetRenderWorldMatrix(&m_GLDefPlaneMatrix);
	}

	CameraNode* camera = renderAction->GetCamera();

	Matrix3x4 controlMatrix;
	//更新固定屏幕位置显示矩阵
	if(this->m_IsParallelScreen && this->m_Type == FixScreen)
	{
		controlMatrix = (camera->GetView()*(*worldMatrix)).Inverse();
		if (renderAction->m_Control.GetAspective() < 1)
		{
			controlMatrix.MultiTranslate(Vector3(-20, -30, -10));
			controlMatrix.MultiScale(0.1);
		}
		else
		{
			controlMatrix.MultiTranslate(Vector3(-60, -40, -10));
			controlMatrix.MultiScale(0.2);
		}
	}
	//平齐屏幕更新
	else if(this->m_IsParallelScreen && this->m_Type != FixScreen)
	{
		controlMatrix = (camera->GetView()*m_WorldMatrix).Inverse();
		Vector3 newCenter = (m_WorldMatrix)*this->m_outramLocation;
		controlMatrix = GetWorldMatrix(controlMatrix,renderAction,newCenter,
				 true,false,false);
		MutexLock lock(m_mutex);
		this->m_outFrameMatrix = controlMatrix.ToMatrix4().Transpose();
	}

	renderAction->PushRenderable(this, RenderableType::RGT_PMI);
}

void PMIData::SetDefPlane(const Matrix3x4& defPlane)
{
	this->m_DefPlane = defPlane;
	this->m_DefPlaneDirty = true;
}

Matrix3x4& PMIData::GetDefPlane()
{
	return this->m_DefPlane;
}

Matrix4& PMIData::GetGLDefPlane()
{
	if(this->m_DefPlaneDirty)
	{
		m_GLDefPlaneMatrix = this->m_DefPlane.ToMatrix4().Transpose();
		this->m_DefPlaneDirty = false;
	}

	return m_GLDefPlaneMatrix;
}
void PMIData::Init()
{
//	if(!m_imageboard)
//	{
//		m_imageboard = new ImageBoard(pos,size);
//		m_imageboard->SetImage(GetDefaultVoiceImage());
//	}
}

BoundingBox   PMIData::GetComTextsBox()
{
	BoundingBox box;

	for(int i=0;i<m_ComTexts.size();i++)
	{
		ComText* comtext = m_ComTexts.at(i);
		if(comtext)
		{
			box.Merge(comtext->GetBoundingBox());
			//LOGE("PMIData::GetComTextsBox()");
		}
	}

	return box;
}

Matrix3x4  PMIData::GetWorldMatrix(const Matrix3x4& mvMatrix,RenderAction* renderAction,
		const Vector3& center,
		bool allowTran,bool allowRotate,bool allowScale)
{
		Matrix3x4 worldMatrix = Matrix3x4::IDENTITY;

		//
		//由于要实现缩放时，所有的点缩放到中心的效果。因此需要将Billboard先移动到坐标原点，在进行平移旋转和缩放操作
		//下面为逆矩阵的正操作，通过改变乘的顺序，可以减少一次矩阵求逆操作

		//将点移动回去
		worldMatrix.MultiTranslate(center);

//		worldMatrix.MultiTranslate(center.Nagative());
		//不允许缩放
		if(!allowScale)
		{
			float ylength = this->GetComTextsBox().GetYLength();

			Vector2 imageBoardSize(0.5/ylength, 0.5/ylength);
			imageBoardSize = ShapeHelper::GetCommonSize(renderAction->GetScene(), imageBoardSize);
//			LOGE("box length %f",length);
			float baseFactor = imageBoardSize.m_y;
			CameraNode* camera = renderAction->GetCamera();

			if(!camera->IsOrthographic()){
				//得到摄像机位置
				Vector3 cameraPos = camera->GetWorldPosition();
				//获取当前ortho宽和高的较小值
				float heigh = camera->GetOrthoSize();

				float dis = (cameraPos - center).Length();

				baseFactor = dis*2/heigh;
			}
			worldMatrix.MultiScale(baseFactor/camera->GetZoom());
		}
	//
	//	//不允许旋转
		if(!allowRotate)
		{
			Quaternion rotation = mvMatrix.Rotation();
			worldMatrix.MultiRotatiton(rotation);
		}

	//	//不允许平移
		if(!allowTran)
		{
			worldMatrix.MultiTranslate(mvMatrix.Translation().Nagative());
		}

		//将点移动到坐标原点
		worldMatrix.MultiTranslate(center.Nagative());

		return worldMatrix;
}

} /* namespace M3D */
