#include "m3d/M3D.h"
#include "m3d/extras/modelmanager/PivotCallbacks.h"
#include "m3d/extras/modelmanager/ModelAssemblyHelper.h"

namespace M3D
{
	PivotCallback::PivotCallback()
	{
		m_pivotCB = NULL;
	}

	PivotCallback::PivotCallback(Matrix3x4& ma)
	{
		this->ma = ma;
	}

	PivotCallback::~PivotCallback()
	{
		this->ClearModel();
	}

	bool PivotCallback::receive(const MotionCommand& command)
	{
		if (command.getStage() == MotionCommand::MOVE)
		{
			//this->receive(command);

			if (command.GetType() == MotionCommand::Type::TYPE_TRANSLATELINE)
			{
				this->receive((TranslateInLineCommand&)command);
			}
			else if (command.GetType() == MotionCommand::Type::TYPE_TRANSLATEINPLANE)
			{
				this->receive((TranslateInPlaneCommand&)command);
			}
			else if (command.GetType() == MotionCommand::Type::TYPE_ROTATE3D)
			{
				this->receive((Rotate3DCommand&)command);
			}
			//else if (command.GetType() == MotionCommand::Type::TYPE_SCALE1D)
			//{
			//	this->receive((Scale1DCommand&)command);
			//}
			//else if (command.GetType() == MotionCommand::Type::TYPE_SCALE2D)
			//{
			//	this->receive((Scale2DCommand&)command);
			//}
			//else if (command.GetType() == MotionCommand::Type::TYPE_SCALEUNIFROM)
			//{
			//	this->receive((ScaleUniformCommand&)command);
			//}

			wstring ws = L"";
			if (m_pivotCB != NULL)
			{
				m_pivotCB(0, ws, ma);
			}
		}

		return false;
	}

	bool PivotCallback::receive(const TranslateInLineCommand& command)
	{
		if (this->m_draggerModels.size())
		{
			Vector3 translation = command.getTranslation();
			translation = command.getLocalToWorld()*Vector4(translation, 0);

			Matrix3x4 modelWMat;
			modelWMat = ma;
			Matrix3x4 tempmat;
			tempmat.MultiTranslate(translation);
			Matrix3x4 plcMat = tempmat * modelWMat;
			
			ma.m_m00 = plcMat.m_m00;
			ma.m_m01 = plcMat.m_m01;
			ma.m_m02 = plcMat.m_m02;
			ma.m_m03 = plcMat.m_m03;
			ma.m_m10 = plcMat.m_m10;
			ma.m_m11 = plcMat.m_m11;
			ma.m_m12 = plcMat.m_m12;
			ma.m_m13 = plcMat.m_m13;
			ma.m_m20 = plcMat.m_m20;
			ma.m_m21 = plcMat.m_m21;
			ma.m_m22 = plcMat.m_m22;
			ma.m_m23 = plcMat.m_m23;
		}

		return true;
	}

	bool PivotCallback::receive(const TranslateInPlaneCommand& command)
	{
		if (this->m_draggerModels.size())
		{
			Vector3 translation = command.getTranslation();
			translation = command.getLocalToWorld()*Vector4(translation, 0);

			Matrix3x4 modelWMat;
			modelWMat = ma;
			Matrix3x4 pareWMat;
			Matrix3x4 tempmat;
			tempmat.MultiTranslate(translation);
			Matrix3x4 plcMat = tempmat * modelWMat;

			ma.m_m00 = plcMat.m_m00;
			ma.m_m01 = plcMat.m_m01;
			ma.m_m02 = plcMat.m_m02;
			ma.m_m03 = plcMat.m_m03;
			ma.m_m10 = plcMat.m_m10;
			ma.m_m11 = plcMat.m_m11;
			ma.m_m12 = plcMat.m_m12;
			ma.m_m13 = plcMat.m_m13;
			ma.m_m20 = plcMat.m_m20;
			ma.m_m21 = plcMat.m_m21;
			ma.m_m22 = plcMat.m_m22;
			ma.m_m23 = plcMat.m_m23;
		}
		return true;
	}

	//bool PivotCallback::receive(const Scale1DCommand& command)
	//{
	//	if (this->m_draggerModels.size())
	//	{
	//		Vector3 scale(command.getDeltaScale(), 0, 0);
	//		scale = command.getLocalToWorld().Rotation()*scale + Vector3(1.0f, 1.0f, 1.0f);
	//		for (int i = 0; i < this->m_draggerModels.size(); i++)
	//		{
	//			Model* model = m_draggerModels.at(i);
	//			ModelAssemblyHelper::ScaleAssemblyModel(model, scale);

	//			if (this->GetScene())
	//			{
	//				ModelAssemblyHelper::RequestShowAllAfterAddModel(this->GetScene(), model);
	//			}
	//		}
	//	}
	//	return true;
	//}

	//bool PivotCallback::receive(const Scale2DCommand& command)
	//{
	//	if (this->m_draggerModels.size())
	//	{
	//		Vector2 deltaScale = command.getDeltaScale();
	//		Vector3 scale(deltaScale.m_x, deltaScale.m_y, 0);
	//		scale = command.getLocalToWorld().Rotation()*scale + Vector3(1.0f, 1.0f, 1.0f);
	//		for (int i = 0; i < this->m_draggerModels.size(); i++)
	//		{
	//			Model* model = m_draggerModels.at(i);
	//			ModelAssemblyHelper::ScaleAssemblyModel(model, scale);

	//			if (this->GetScene())
	//			{
	//				ModelAssemblyHelper::RequestShowAllAfterAddModel(this->GetScene(), model);
	//			}
	//		}
	//	}
	//	return true;
	//}

	//bool PivotCallback::receive(const ScaleUniformCommand& command)
	//{
	//	if (this->m_draggerModels.size())
	//	{
	//		double deltaScale = command.getDeltaScale();
	//		Vector3 scale(deltaScale, deltaScale, deltaScale);
	//		scale = command.getLocalToWorld().Rotation()*scale + Vector3(1.0f, 1.0f, 1.0f);
	//		for (int i = 0; i < this->m_draggerModels.size(); i++)
	//		{
	//			Model* model = m_draggerModels.at(i);
	//			ModelAssemblyHelper::ScaleAssemblyModel(model, scale);

	//			if (this->GetScene())
	//			{
	//				ModelAssemblyHelper::RequestShowAllAfterAddModel(this->GetScene(), model);
	//			}
	//		}
	//	}
	//	return true;
	//}

	bool PivotCallback::receive(const Rotate3DCommand& command)
	{
		Matrix3x4 matrix3x4 = ma;

		Quaternion rotation = command.getDeltaRotation();
		rotation = command.getLocalToWorld().Rotation()*rotation*command.getLocalToWorld().Rotation().Inverse();
		Vector3 pivot = command.GetRefDragger()->getParentDragger()->GetTransform().Translation();
		Matrix3x4 trans1,rot1, trans2;
		trans1.MultiTranslate(-pivot);
		rot1.MultiRotatiton(rotation);
		trans2.MultiTranslate(pivot);
		Matrix3x4 tempmat = trans2 * rot1 * trans1;
		Matrix3x4 plcMat = tempmat * matrix3x4;

		ma.m_m00 = plcMat.m_m00;
		ma.m_m01 = plcMat.m_m01;
		ma.m_m02 = plcMat.m_m02;
		ma.m_m03 = plcMat.m_m03;
		ma.m_m10 = plcMat.m_m10;
		ma.m_m11 = plcMat.m_m11;
		ma.m_m12 = plcMat.m_m12;
		ma.m_m13 = plcMat.m_m13;
		ma.m_m20 = plcMat.m_m20;
		ma.m_m21 = plcMat.m_m21;
		ma.m_m22 = plcMat.m_m22;
		ma.m_m23 = plcMat.m_m23;

		return true;
	}

	bool PivotCallback::AddModel(Model* model)
	{
		for (int i = 0; i < m_draggerModels.size(); i++)
		{
			if (m_draggerModels.at(i) == model)
			{
				return false;
			}
		}
		model->AddRef();
		m_draggerModels.push_back(model);
		return true;
	}

	void PivotCallback::AddModels(vector<Model*> models)
	{
		for (int i = 0; i < models.size(); i++)
		{
			this->AddModel(models.at(i));
		}
	}

	void PivotCallback::ClearModel()
	{
		for (int i = 0; i < m_draggerModels.size(); i++)
		{
			m_draggerModels.at(i)->Release();
		}
		m_draggerModels.clear();
	}	
}
