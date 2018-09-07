#ifndef M3D_MODEL_MERGE_PROCESS_H_
#define M3D_MODEL_MERGE_PROCESS_H_

#include "Merge.h"

namespace M3D {
	class M3D_API MergeProcess
	{
	public: 
		MergeProcess();
		~MergeProcess();

		void MergeFaces(vector<Model*>& models, int mergeStatus, int ExceptStatus);// , SceneManager* scene);
		Model* MergeParts(vector<Model*>& models, string& name, int mergeStatus, int ExceptStatus, SceneManager* scene);
		list<Model*>* MergePartsByAssembly(vector<Model*>& models, int mergeStatus, int ExceptStatus, SceneManager* scene);
		list<Model*>* SeparateModels(vector<Model*>& models, SceneManager* scene);
		int GetProcessStep();
		list<Model*>* getAddedModels();
	private:
		bool SeparateModel(Model* model, SceneManager* scene);
		bool SeparateModelByMesh(Model* model, SceneManager* scene);
		string GetParentPath(vector<Model*>& models);
		bool InsertModel(SceneManager * scene, Model* sourceModel, Model* desModel);
	private: 
		int m_processing;
		list<Model*> m_newModels;
	};
}
#endif