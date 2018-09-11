#include "Annotation.h"
#include "m3d/action/RenderAction.h"
#include "m3d/model/Note.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/utils/ShapeHelper.h"
#include "m3d/graphics/ImageBoard.h"
#include "m3d/extras/annotation/AnnotationFactory.h"

namespace M3D
{
	Annotation::Annotation()
	{
		m_bLeaderLine = false;  
		m_bStub = false;
		m_bEnvelope = false;
		m_bFixed = false;
		m_Id = OBJID++;
		m_fgap = 0;
		m_CreateID = m_Id;
		SetType(SHAPE_ANNOTATION_NOTE);
	}
	Annotation::~Annotation()
	{

	}
	void Annotation::Init()
	{

	}

	Vector3& Annotation::GetAnnotationPos()
	{
		return m_annotationPos;
	}

	void Annotation::SetAnnotationPos(const Vector3& pos)
	{
		m_annotationPos = pos;
	}

	Vector3& Annotation::GetCenterPos()
	{
		return m_centerPos;
	}
	void Annotation::SetCenterPos(const Vector3& pos)
	{
		m_centerPos = pos;
	}

	Vector3& Annotation::GetTextsPos()
	{
		return m_textPos;
	}

	void Annotation::SetTextsPos(const Vector3& pos)
	{
		m_textPos = pos;
	}

	Matrix4* Annotation::GetOutFrameMatrix()
	{
		MutexLock lock(m_mutex);
		return &this->m_outFrameMatrix;
	}

	Matrix4* Annotation::GetTextMatrix()
	{
		MutexLock lock(m_mutex);
		return &this->m_textMatrix;
	}

	void Annotation::FindVisiableObject(RenderAction* renderAction)
	{ 
		SetVisiableObject(renderAction);

		AnnotationFactory::ModifyPntAnnotation(this, renderAction);
		renderAction->PrepareRenderAnnotation(this);
	}

	BoundingBox   Annotation::GetComTextsBox()
	{
		BoundingBox box;

		for (int i = 0; i < m_ComTexts.size(); i++)
		{
			ComText* comtext = m_ComTexts.at(i);
			if (comtext)
			{
				box.Merge(comtext->GetBoundingBox());
				//LOGE("PMIData::GetComTextsBox()");
			}
		}

		return box;
	}

	Matrix3x4  Annotation::GetWorldMatrix(const Matrix3x4& mvMatrix, RenderAction* renderAction,
		const Vector3& center,
		bool allowTran, bool allowRotate, bool allowScale)
	{
		Matrix3x4 worldMatrix = Matrix3x4::IDENTITY;

		//
		//由于要实现缩放时，所有的点缩放到中心的效果。因此需要将Billboard先移动到坐标原点，在进行平移旋转和缩放操作
		//下面为逆矩阵的正操作，通过改变乘的顺序，可以减少一次矩阵求逆操作

		//将点移动回去
		worldMatrix.MultiTranslate(center);

		//		worldMatrix.MultiTranslate(center.Nagative());
		//不允许缩放
		if (!allowScale)
		{
			float ylength = this->GetComTextsBox().GetYLength();

			Vector2 imageBoardSize(0.5 / ylength, 0.5 / ylength);
			imageBoardSize = ShapeHelper::GetCommonSize(renderAction->GetScene(), imageBoardSize);
			//			LOGE("box length %f",length);
			float baseFactor = imageBoardSize.m_y;
			CameraNode* camera = renderAction->GetCamera();

			if (!camera->IsOrthographic()) {
				//得到摄像机位置
				Vector3 cameraPos = camera->GetWorldPosition();
				//获取当前ortho宽和高的较小值
				float heigh = camera->GetOrthoSize();

				float dis = (cameraPos - center).Length();

				baseFactor = dis * 2 / heigh;
			}
			worldMatrix.MultiScale(baseFactor / camera->GetZoom());
		}
		//
		//	//不允许旋转
		if (!allowRotate)
		{
			Quaternion rotation = mvMatrix.Rotation();
			worldMatrix.MultiRotatiton(rotation);
			float agn = rotation.RollAngle();
			float agn1 = rotation.RollAngle();
		}

		//	//不允许平移
		if (!allowTran)
		{
			worldMatrix.MultiTranslate(mvMatrix.Translation().Nagative());
		}

		//将点移动到坐标原点
		worldMatrix.MultiTranslate(center.Nagative());

		return worldMatrix;
	}

	bool Annotation::GetHaveStub()
	{
		return m_bStub;
	}
	void Annotation::SetHaveStub(bool value)
	{
		m_bStub = value;
	}

	bool Annotation::GetHaveEnvelope()
	{
		return m_bEnvelope;
	}
	void Annotation::SetHaveEnvelope(bool value)
	{
		m_bEnvelope = value;
	}

	bool Annotation::GetHaveLeaderLine()
	{
		return m_bLeaderLine;
	}
	void Annotation::SetHaveLeaderLine(bool value)
	{
		m_bLeaderLine = value;
	}

	string Annotation::getText()
	{
		return m_textValue;
	}
	void Annotation::SetText(string value)
	{
		m_textValue = value;
	}

	float Annotation::GetPosAverage()
	{
		return m_fTextPos;
	}
	void Annotation::SetPosAverage(float data)
	{
		m_fTextPos = data;
	}

	float Annotation::GetGap()
	{
		return m_fgap;
	}
	void Annotation::SetGap(float data)
	{
		m_fgap = data;
	}

	bool Annotation::GetFixed()
	{
		return m_bFixed;
	}
	void Annotation::SetFixed(bool value)
	{
		m_bFixed = value;
	}


	void Annotation::SetFillColor(const Color &color)
	{
		m_FillColor = color;
	}

	Color* Annotation::GetFillColor()
	{
		return &m_FillColor;
	}

	void Annotation::SetFrameColor(const Color &color)
	{
		m_FrameColor = color;
	}

	Color* Annotation::GetFrameColor()
	{
		return &m_FrameColor;
	}

	IDTYPE Annotation::GetCreateID()
	{
		return m_CreateID;
	}

	void Annotation::SetCreateID(IDTYPE createID)
	{
		m_CreateID = createID;
	}
}