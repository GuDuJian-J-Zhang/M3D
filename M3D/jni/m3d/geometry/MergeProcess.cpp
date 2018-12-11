#include "MergeProcess.h"
#include "m3d/SceneManager.h"
#include "m3d/model/ModelShape.h"
#include "m3d/utils/IDCreator.h"
#include "m3d/extras/modelmanager/ModelAssemblyHelper.h"
#include "m3d/model/Body.h"

namespace M3D {
	MergeProcess::MergeProcess()
	{
		m_processing = 0;
		m_newModels.resize(0);
	}
	MergeProcess::~MergeProcess()
	{

	}

	void MergeProcess::MergeFaces(vector<Model*>& models, int mergeStatus, int ExceptStatus)
	{
		if (models.size() <= 0)
		{
			return;
		}
		Merge::SetInitStep();
		for (size_t i = 0; i < models.size(); i++)
		{
			Model* model = models[i];
			Merge::MergeModelFaces(model, mergeStatus, ExceptStatus);
		}
	}

	Model* MergeProcess::MergeParts(vector<Model*>& models, string& name, int mergeStatus, int ExceptStatus, SceneManager* scene)
	{
		Model* newModel = NULL;
		Model* parentModel = NULL;
		if (models.size() <= 0 || !scene)
		{
			return newModel;
		}
		Merge::SetInitStep();

		parentModel = models[0]->GetParent();
		if (models.size() == 1 && !parentModel)
		{
			Model* model = models[0];
			Merge::MergeModels(model, model, mergeStatus, ExceptStatus);
		}
		else
		{
			newModel = new Model();
			newModel->SetName(name);
			newModel->SetVisible(true);

			ModelShape* tShapeNode = new ModelShape();
			newModel->SetModelShape(tShapeNode);
			newModel->SetDrawDataPrepared(true);
			tShapeNode->UpdateWorldTransform();
			for (int i = 0; i < models.size(); i++)
			{
				Model* srcModel = models[i];
				Merge::MergeModels(srcModel, newModel, mergeStatus, ExceptStatus, true);
			}
			newModel->SetModelExtInfo(scene->GetExtendInfoManager());

			if (models.size() == 1)
			{
				newModel->SetPlcId(models[0]->GetPlcId());
				newModel->SetSVLId(models[0]->GetSVLId());
				newModel->SetInstanceID(models[0]->GetInstatnceID());
				//parentModel = models[0]->GetParent();
			}
			else
			{
				string strParentPath = GetParentPath(models);
				parentModel = dynamic_cast<Model*>(scene->GetShape(strParentPath));
			}
			for (int i = 0; i < models.size(); i++)
			{
				Model* srcModel = models[i];
				scene->AsynRemoveModelFromeScene(srcModel->GetParent(), srcModel);
			}
			if (InsertModel(scene, newModel, parentModel))
			{
				newModel->SetPlaceMatrix(parentModel->GetWorldTransform().Inverse());
			}
		}
		
		return newModel;
	}

	list<Model*>* MergeProcess::MergePartsByAssembly(vector<Model*>& models, int mergeStatus, int ExceptStatus, SceneManager* scene)
	{
		Merge::SetInitStep();
		Model* newModel = NULL;
		m_newModels.clear();
		if (models.size() > 0)
		{
			for (int i = 0; i < models.size(); i++)
			{
				Model* model = models[i];
				if (model)
				{
					Model* parentModel = model->GetParent();
					if (parentModel)
					{
						newModel = new Model();
						newModel->SetName(model->GetName());
						newModel->SetVisible(true);
						newModel->SetPlcId(model->GetPlcId());
						newModel->SetSVLId(model->GetSVLId());
						newModel->SetInstanceID(model->GetInstatnceID());

						ModelShape* tShapeNode = new ModelShape();
						newModel->SetModelShape(tShapeNode);
						newModel->SetDrawDataPrepared(true);
						tShapeNode->UpdateWorldTransform();

						Merge::MergeModels(model, newModel, mergeStatus, ExceptStatus, true);
						newModel->SetModelExtInfo(scene->GetExtendInfoManager());
						if (InsertModel(scene, newModel, parentModel))
						{
							newModel->SetPlaceMatrix(parentModel->GetWorldTransform().Inverse());
							m_newModels.push_back(newModel);

							scene->AsynRemoveModelFromeScene(parentModel, model);
						}
						else
						{
							delete newModel;
							newModel = NULL;
						}
						
					}
					else
					{
						Merge::MergeModels(model, model, mergeStatus, ExceptStatus);
					}
				}	
			}
		}
		return &m_newModels;
	}

	list<Model*>* MergeProcess::SeparateModels(vector<Model*>& models, SceneManager* scene)
	{
		Merge::SetInitStep();
		Model* newModel = NULL;
		m_newModels.clear();

		if (models.size() > 0)
		{
			for (int i = 0; i < models.size(); i++)
			{
				SeparateModel(models[i],scene);
			}
		}
		return &m_newModels;
	}

	bool MergeProcess::SeparateModel(Model* model, SceneManager* scene)
	{
		if (!model)
		{
			return false;
		}
		vector<Model*>& children = model->GetSubModels();
		if (children.size() > 0)
		{
			for (size_t i = 0; i < children.size(); i++)
			{
				SeparateModel(children[i], scene);
			}
		}
		else
		{
			SeparateModelByMesh(model,scene);
		}
		return true;
	}

	bool MergeProcess::SeparateModelByMesh(Model* model, SceneManager* scene)
	{
		if (!model)
		{
			return false;
		}

		ModelShape * modelShape = model->GetModelShape();
		if (modelShape->IsDrawDataPrepared())
		{
			vector<Body*>* bodys = model->GetBodys();
			if (bodys)
			{
				int serial = 0;
				char buffer[SA_BUFFER_SIZE_SMALL] = { 0 };
				if (bodys->size() <= 1)
				{
					return true;
				}
				for (size_t i = 0; i < bodys->size(); i++)
				{
					Body* body = bodys->at(i);
					if (body)
					{
						Merge::AddStep();
						Model* createModel = new Model();
						sprintf(buffer, "SeparateModel%d", i+1);
						string name = buffer;

						createModel->SetName(name);
						createModel->SetVisible(true);
						createModel->SetDrawDataPrepared(true);
						createModel->AddBody(body);
						createModel->SetModelExtInfo(scene->GetExtendInfoManager());
						if (InsertModel(scene, createModel, model))
						{
							createModel->SetPlaceMatrix(model->GetWorldTransform().Inverse());
							m_newModels.push_back(createModel);
							body->Release();
						}
						else
						{
							delete createModel;
							createModel = NULL;
						}
					}
				}
				bodys->clear();
			}
		}
		return true;
	}

	string MergeProcess::GetParentPath(vector<Model*>& models)
	{
		string path = "";
		for (int i = 0; i < models.size(); i++)
		{
			Model* srcModel = models[i];
			if (srcModel)
			{
				if (i == 0)
				{
					path = models[i]->GetPlcPath();
				}
				else
				{
					path = Merge::CalculateParentPlcPath(path, models[i]->GetPlcPath());
				}
			}
		}
		return path;
	}

	bool MergeProcess::InsertModel(SceneManager * scene, Model* sourceModel, Model* desModel)
	{
		bool msg = false;
		if (sourceModel && desModel)
		{
			bool bFinished = false;
			scene->Lock();
			vector<Model*>& subModels = desModel->GetSubModels();

			for (vector<Model*>::iterator it = subModels.begin(); it != subModels.end(); it++)
			{
				Model* model = *it;
				if (model && model->GetPlcId() == sourceModel->GetPlcId())
				{
					sourceModel->AddRef();
					sourceModel->SetParent(desModel);
					subModels.insert(it, sourceModel);
					bFinished = true;
					break;
				}
			}
			if (!bFinished)
			{
				int tempId = desModel->GetUseablePlcId();
				desModel->AddSubModel(sourceModel);
				sourceModel->SetPlcId(tempId);
			}
			sourceModel->ClearPlcPath(true);

			scene->AsynUpdateModelCacheInfo(sourceModel, true);
			scene->RequestUpdateWhenSceneBoxChanged();
			scene->UnLock();
			msg = true;
		}
		return msg;
	}
	int MergeProcess::GetProcessStep()
	{
		return Merge::GetStep();
	}

	list<Model*>* MergeProcess::getAddedModels()
	{
		return &m_newModels;
	}
}