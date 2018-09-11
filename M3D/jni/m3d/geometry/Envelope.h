#ifndef M3D_MODEL_ENVELOPE_H_
#define M3D_MODEL_ENVELOPE_H_
#include "m3d/model/Model.h"

namespace M3D
{
	class M3D_API Envelope
	{
	public: 
		Envelope();
		~Envelope();

		bool CreateEnvelope(Model* model, bool rmvInner, bool rmvLittle, float rmvPercent, SceneManager* scene);
		int GetCurrentStep() { return m_Step; };
	private:
		//bool RmvLittleParts(Model* model, bool rmvLittle, float rmvRange);
		void Enveloping(Model* model, Model* mergeModel, bool rmvInner, bool rmvLittle, float rmvData, SceneManager* scene);
		bool CheckOutSide(int startIndex, int endIndex, M3D_INDEX_TYPE* Indexs, Vector3 *Pnts, Vector3& boxCenter, 
			Model* model, SceneManager* scene);
	private:
		vector<Model*> m_ModelVec;
		vector<Vector3> m_StandardDirection;
		int m_Step;
	};
}
#endif
