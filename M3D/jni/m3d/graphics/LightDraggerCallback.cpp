#include "m3d/graphics/LightDraggerCallback.h"
#include "m3d/extras/modelmanager/ModelAssemblyHelper.h"
#include "m3d/graphics/DirectionalLight.h"

namespace M3D {
	LightDraggerCallback::LightDraggerCallback()
	{

	}
	LightDraggerCallback::~LightDraggerCallback()
	{
		this->ClearModel();
	}

	bool LightDraggerCallback::receive(const MotionCommand& command)
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
			else if (command.GetType() == MotionCommand::Type::TYPE_SCALE1D)
			{
				this->receive((Scale1DCommand&)command);
			}
			else if (command.GetType() == MotionCommand::Type::TYPE_SCALE2D)
			{
				this->receive((Scale2DCommand&)command);
			}
			else if (command.GetType() == MotionCommand::Type::TYPE_SCALEUNIFROM)
			{
				this->receive((ScaleUniformCommand&)command);
			}
		}

		return false;
	}
	bool LightDraggerCallback::receive(const TranslateInPlaneCommand& command)
	{
		if (this->m_draggerModels.size())
		{
			Vector3 translation = command.getTranslation();
			translation = command.getLocalToWorld()*Vector4(translation, 0);
			for (int i = 0; i < this->m_draggerModels.size(); i++)
			{
				Model* model = m_draggerModels[i];
				if (model && model->GetType() == SHAPETYPE::SHAPE_LIGHT_DIRECTIONAL)
				{
					DirectionalLight* directionalLight = dynamic_cast<DirectionalLight*>(model);
					Vector3 directionPos = directionalLight->GetDirectionPos();
					Vector3 sourcePosition = directionalLight->GetWorldPosition();

					if (m_type == DraggerType::SOURCE_DRAGGER)
					{
						ModelAssemblyHelper::TranslateAssemblyModel(model, translation);
						if (this->GetScene())
						{
							ModelAssemblyHelper::RequestShowAllAfterAddModel(this->GetScene(), model);
						}
					}
					else if (m_type == DraggerType::DIRECTION_DRAGGER)
					{
						Model* parent = model->GetParent();
						Matrix3x4 pareWMat; 
						pareWMat = parent ? parent->GetWorldTransform() : pareWMat;
						Matrix3x4 tempmat;
						tempmat.MultiTranslate(translation);
						tempmat.MultiTranslate(directionalLight->GetDirectionPos());
						Matrix3x4 plcMat = pareWMat.Inverse() * (tempmat);
						directionPos = plcMat.Translation();
						directionalLight->SetDirectionPos(directionPos);
					}
					
					Vector3 lightDirection = directionPos - sourcePosition;
					Quaternion rotation(directionalLight->GetWorldDirection().Nagative(), lightDirection);
					ModelAssemblyHelper::RotateAssemblyModel(model, rotation);

					directionalLight->ChangeLines();
				}
				
			}
		}
		return true;
	}

	bool LightDraggerCallback::AddModel(Model* model)
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
	void LightDraggerCallback::AddModels(vector<Model*> models)
	{
		for (int i = 0; i < models.size(); i++)
		{
			this->AddModel(models.at(i));
		}
	}
	void LightDraggerCallback::ClearModel()
	{
		for (int i = 0; i < m_draggerModels.size(); i++)
		{
			m_draggerModels.at(i)->Release();
		}
		m_draggerModels.clear();
	}

	void LightDraggerCallback::SetDraggerType(DraggerType value)
	{
		m_type = value;
	}
}