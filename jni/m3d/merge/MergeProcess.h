#ifndef M3D_MODEL_MERGE_PROCESS_H_
#define M3D_MODEL_MERGE_PROCESS_H_

#include "Merge.h"

namespace M3D {
	class M3D_API MergeProcess
	{
	public: 
		MergeProcess();
		~MergeProcess();

		Model* MergeParts(vector<Model*>& models, string& name, int mergeStatus, int ExceptStatus, SceneManager* scene);
		void MergePartsByAssembly(vector<Model*>& models, int mergeStatus, int ExceptStatus, SceneManager* scene);
		int GetProcessStep();
	private:
		string GetParentPath(vector<Model*>& models);
		bool InsertInLast(SceneManager * scene, Model* sourceModel, Model* desModel);

	private: 
		int m_processing;
	};
}
#endif