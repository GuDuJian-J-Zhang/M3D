
#include "m3d/M3D.h"
#include "m3d/extras/modelmanager/DraggerCallbacks.h"
#include "m3d/extras/modelmanager/ModelAssemblyHelper.h"

namespace M3D
{
	ModelDraggerCallback::ModelDraggerCallback()
	{

	}

	ModelDraggerCallback::~ModelDraggerCallback()
	{
		this->ClearModel();
	}

	bool ModelDraggerCallback::receive(const MotionCommand& command)
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

	bool ModelDraggerCallback::receive(const TranslateInLineCommand& command)
	{
		if (this->m_draggerModels.size())
		{
			Vector3 translation = command.getTranslation();
			translation = command.getLocalToWorld()*Vector4(translation, 0);
			for (int i = 0; i < this->m_draggerModels.size(); i++)
			{
				Model* model = m_draggerModels.at(i);
				ModelAssemblyHelper::TranslateAssemblyModel(model, translation);
				if (this->GetScene())
				{
					ModelAssemblyHelper::RequestShowAllAfterAddModel(this->GetScene(), model);
				}
			}
		}

		return true;
	}

	bool ModelDraggerCallback::receive(const TranslateInPlaneCommand& command)
	{
		if (this->m_draggerModels.size())
		{
			Vector3 translation = command.getTranslation();
			translation = command.getLocalToWorld()*Vector4(translation, 0);
			for (int i = 0; i < this->m_draggerModels.size(); i++)
			{
				Model* model = m_draggerModels.at(i);
				ModelAssemblyHelper::TranslateAssemblyModel(model, translation);

				if (this->GetScene())
				{
					ModelAssemblyHelper::RequestShowAllAfterAddModel(this->GetScene(), model);
				}
			}
		}
		return true;
	}

	bool ModelDraggerCallback::receive(const Scale1DCommand& command)
	{
		if (this->m_draggerModels.size())
		{
			Vector3 scale(command.getDeltaScale(), 0, 0);
			scale = command.getLocalToWorld().Rotation()*scale + Vector3(1.0f, 1.0f, 1.0f);
			for (int i = 0; i < this->m_draggerModels.size(); i++)
			{
				Model* model = m_draggerModels.at(i);
				ModelAssemblyHelper::ScaleAssemblyModel(model, scale);

				if (this->GetScene())
				{
					ModelAssemblyHelper::RequestShowAllAfterAddModel(this->GetScene(), model);
				}
			}
		}
		return true;
	}

	bool ModelDraggerCallback::receive(const Scale2DCommand& command)
	{
		if (this->m_draggerModels.size())
		{
			Vector2 deltaScale = command.getDeltaScale();
			Vector3 scale(deltaScale.m_x, deltaScale.m_y, 0);
			scale = command.getLocalToWorld().Rotation()*scale + Vector3(1.0f, 1.0f, 1.0f);
			for (int i = 0; i < this->m_draggerModels.size(); i++)
			{
				Model* model = m_draggerModels.at(i);
				ModelAssemblyHelper::ScaleAssemblyModel(model, scale);

				if (this->GetScene())
				{
					ModelAssemblyHelper::RequestShowAllAfterAddModel(this->GetScene(), model);
				}
			}
		}
		return true;
	}

	bool ModelDraggerCallback::receive(const ScaleUniformCommand& command)
	{
		if (this->m_draggerModels.size())
		{
			double deltaScale = command.getDeltaScale();
			Vector3 scale(deltaScale, deltaScale, deltaScale);
			scale = command.getLocalToWorld().Rotation()*scale + Vector3(1.0f, 1.0f, 1.0f);
			for (int i = 0; i < this->m_draggerModels.size(); i++)
			{
				Model* model = m_draggerModels.at(i);
				ModelAssemblyHelper::ScaleAssemblyModel(model, scale);

				if (this->GetScene())
				{
					ModelAssemblyHelper::RequestShowAllAfterAddModel(this->GetScene(), model);
				}
			}
		}
		return true;
	}

	bool ModelDraggerCallback::receive(const Rotate3DCommand& command)
	{
		if (this->m_draggerModels.size())
		{
			Quaternion rotation = command.getDeltaRotation();
			rotation = command.getLocalToWorld().Rotation()*rotation*command.getLocalToWorld().Rotation().Inverse();
			for (int i = 0; i < this->m_draggerModels.size(); i++)
			{
				Model* model = m_draggerModels.at(i);
				//ModelAssemblyHelper::RotateAssemblyModel(model, rotation);
				Vector3 pivot = command.GetRefDragger()->getParentDragger()->GetTransform().Translation();
				ModelAssemblyHelper::RotateAssemblyModel(model, rotation, pivot);

				if (this->GetScene())
				{
					ModelAssemblyHelper::RequestShowAllAfterAddModel(this->GetScene(), model);
				}
			}
		}
		return true;
	}

	bool ModelDraggerCallback::AddModel(Model* model)
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

	void ModelDraggerCallback::AddModels(vector<Model*> models)
	{
		for (int i = 0; i < models.size(); i++)
		{
			this->AddModel(models.at(i));
		}
	}

	void ModelDraggerCallback::ClearModel()
	{
		for (int i = 0; i < m_draggerModels.size(); i++)
		{
			m_draggerModels.at(i)->Release();
		}
		m_draggerModels.clear();
	}

	SceneNodeDraggerCallback::SceneNodeDraggerCallback()
	{

	}

	SceneNodeDraggerCallback::~SceneNodeDraggerCallback()
	{
		this->ClearNodes();
	}

	bool SceneNodeDraggerCallback::receive(const MotionCommand& command)
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

	bool SceneNodeDraggerCallback::receive(const TranslateInLineCommand& command)
	{
		Vector3 translation = command.getTranslation();
		translation = command.getLocalToWorld()*Vector4(translation, 0);
		Model* model = (Model*)m_plane;
		ModelAssemblyHelper::TranslateAssemblyModel(model, translation);

		//if (this->m_draggerNodes.size())
		//{
		//	Vector3 translation = command.getTranslation();
		//	translation = command.getLocalToWorld()*Vector4(translation, 0);
		//	Model* model = (Model*)m_plane;
		//	ModelAssemblyHelper::TranslateAssemblyModel(model, translation);
		//	//for (int i = 0; i < this->m_draggerNodes.size(); i++)
		//	//{
		//	//	SceneNode* node = m_draggerNodes.at(i);
		//	//	//ModelAssemblyHelper::TranslateAssemblyModel(node, translation);
		//	//	node->Translate(translation, M3D::TS_WORLD);
		//	//}
		//}

		return true;
	}


	bool SceneNodeDraggerCallback::receive(const TranslateInPlaneCommand& command)
	{
		if (this->m_draggerNodes.size())
		{
			Vector3 translation = command.getTranslation();
			translation = command.getLocalToWorld()*Vector4(translation, 0);
			for (int i = 0; i < this->m_draggerNodes.size(); i++)
			{
				SceneNode* node = m_draggerNodes.at(i);
				node->Translate(translation, M3D::TS_WORLD);
			}
		}
		return true;
	}

	bool SceneNodeDraggerCallback::receive(const Scale1DCommand& command)
	{
		if (this->m_draggerNodes.size())
		{
			Vector3 scale(command.getDeltaScale(), 0, 0);
			scale = command.getLocalToWorld().Rotation()*scale + Vector3(1.0f, 1.0f, 1.0f);
			for (int i = 0; i < this->m_draggerNodes.size(); i++)
			{
				SceneNode* node = m_draggerNodes.at(i);
				node->Scale(scale);
			}
		}
		return true;
	}

	bool SceneNodeDraggerCallback::receive(const Scale2DCommand& command)
	{
		if (this->m_draggerNodes.size())
		{
			Vector2 deltaScale = command.getDeltaScale();
			Vector3 scale(deltaScale.m_x, deltaScale.m_y, 0);
			scale = command.getLocalToWorld().Rotation()*scale + Vector3(1.0f, 1.0f, 1.0f);
			for (int i = 0; i < this->m_draggerNodes.size(); i++)
			{
				SceneNode* node = m_draggerNodes.at(i);
				node->Scale(scale);
			}
		}
		return true;
	}

	bool SceneNodeDraggerCallback::receive(const ScaleUniformCommand& command)
	{
		if (this->m_draggerNodes.size())
		{
			double deltaScale = command.getDeltaScale();
			Vector3 scale(deltaScale, deltaScale, deltaScale);
			scale = command.getLocalToWorld().Rotation()*scale + Vector3(1.0f, 1.0f, 1.0f);
			if (m_plane)
			{
				//m_plane->SetTransform(scale);
			}
			/*for (int i = 0; i < this->m_draggerNodes.size(); i++)
			{
				SceneNode* node = m_draggerNodes.at(i);
				node->Scale(scale);
			}*/
		}
		return true;
	}

	bool SceneNodeDraggerCallback::receive(const Rotate3DCommand& command)
	{
		if (this->m_draggerNodes.size())
		{
			Quaternion rotation = command.getDeltaRotation();
			rotation = command.getLocalToWorld().Rotation()*rotation*command.getLocalToWorld().Rotation().Inverse();
			for (int i = 0; i < this->m_draggerNodes.size(); i++)
			{
				SceneNode* node = m_draggerNodes.at(i);
				node->Rotate(rotation,TS_WORLD);
			}
		}
		return true;
	}

	bool SceneNodeDraggerCallback::AddNode(SceneNode* node)
	{
		for (int i = 0; i < m_draggerNodes.size(); i++)
		{
			if (m_draggerNodes.at(i) == node)
			{
				return false;
			}
		}
		node->AddRef();
		m_draggerNodes.push_back(node);
		return true;
	}

	void SceneNodeDraggerCallback::AddNodes(vector<SceneNode*> nodes)
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			this->AddNode(nodes.at(i));
		}
	}

	void SceneNodeDraggerCallback::ClearNodes()
	{
		for (int i = 0; i < m_draggerNodes.size(); i++)
		{
			m_draggerNodes.at(i)->Release();
		}
		m_draggerNodes.clear();
	}

	void SceneNodeDraggerCallback::SetSectionPlane(SectionPlane * plane)
	{
		this->m_plane = plane;
	}
}
