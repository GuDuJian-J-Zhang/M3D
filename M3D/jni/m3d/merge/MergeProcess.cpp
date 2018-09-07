#include "MergeProcess.h"
#include "m3d/SceneManager.h"
#include "m3d/model/ModelShape.h"
#include "m3d/utils/IDCreator.h"
#include "m3d/extras/modelmanager/ModelAssemblyHelper.h"

namespace M3D {
	MergeProcess::MergeProcess()
	{
		m_processing = 0;
	}
	MergeProcess::~MergeProcess()
	{

	}
	Model* MergeProcess::MergeParts(vector<Model*>& models, string& name, int mergeStatus, int ExceptStatus, SceneManager* scene)
	{
		Model* newModel = NULL;
		if (models.size() <= 0 || !scene)
		{
			return newModel;
		}
		Merge::SetInitStep();
		if (models.size() == 1)
		{
			Model* model = models[0];
			Merge::MergeModels(model, model, mergeStatus, ExceptStatus);
		}
		else
		{
			string strParentPath = GetParentPath(models);
			Model* parentModel = dynamic_cast<Model*>(scene->GetShape(strParentPath));
			if (!parentModel)
			{
				return newModel;
			}

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
			InsertInLast(scene, newModel, parentModel);
		}

		return newModel;
	}

	void MergeProcess::MergePartsByAssembly(vector<Model*>& models, int mergeStatus, int ExceptStatus, SceneManager* scene)
	{
		Merge::SetInitStep();
		if (models.size() > 0)
		{
			for (int i = 0; i < models.size(); i++)
			{
				Model* model = models[i];
				Merge::MergeModels(model, model, mergeStatus, ExceptStatus, false);
			}
		}
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

	bool MergeProcess::InsertInLast(SceneManager * scene, Model* sourceModel, Model* desModel)
	{
		LOGI(" ModelManager::InsertBefore BEGIN");
		bool msg = false;
		if (sourceModel && desModel)
		{
			int tempId = desModel->GetUseablePlcId();
			scene->Lock();
			desModel->AddSubModel(sourceModel);
			sourceModel->SetPlcId(tempId);
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
}