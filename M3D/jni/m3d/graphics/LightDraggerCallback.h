#ifndef M3D_LIGHT_DRAGGER_CALLBACK_H_
#define M3D_LIGHT_DRAGGER_CALLBACK_H_

#include "m3d/handler/dragger.h"
namespace M3D
{
	enum DraggerType
	{
		SOURCE_DRAGGER,
		DIRECTION_DRAGGER
	};
	class M3D_API LightDraggerCallback :public DraggerCallback
	{
	public:
		LightDraggerCallback();
		virtual ~LightDraggerCallback();
		/**
		* Receive motion commands. Returns true on success.
		*/
		virtual bool receive(const MotionCommand&);
		virtual bool receive(const TranslateInPlaneCommand& command);

		bool AddModel(Model* model);
		void AddModels(vector<Model*> models);
		void ClearModel();

		void SetDraggerType(DraggerType value);
	private:
		vector<Model*> m_draggerModels;
		DraggerType m_type;

	};
}

#endif