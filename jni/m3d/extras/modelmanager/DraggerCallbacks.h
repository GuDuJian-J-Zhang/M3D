/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *
 */

#ifndef M3D_DRAGGER_CALLBACK_H_
#define M3D_DRAGGER_CALLBACK_H_

#include "m3d/handler/dragger.h"

namespace M3D
{
	class M3D_API ModelDraggerCallback :public DraggerCallback
	{
	public:
		ModelDraggerCallback();
		virtual ~ModelDraggerCallback();
		/**
		* Receive motion commands. Returns true on success.
		*/
		virtual bool receive(const MotionCommand&);
		virtual bool receive(const TranslateInLineCommand& command);
		virtual bool receive(const TranslateInPlaneCommand& command);
		virtual bool receive(const Scale1DCommand& command);
		virtual bool receive(const Scale2DCommand& command);
		virtual bool receive(const ScaleUniformCommand& command);
		virtual bool receive(const Rotate3DCommand& command);

		bool AddModel(Model* model);
		void AddModels(vector<Model*> models);
		void ClearModel();

	private:
		vector<Model*> m_draggerModels;
	};

	class M3D_API SceneNodeDraggerCallback :public DraggerCallback
	{
	public:
		SceneNodeDraggerCallback();
		virtual ~SceneNodeDraggerCallback();
		/**
		* Receive motion commands. Returns true on success.
		*/
		virtual bool receive(const MotionCommand&);
		virtual bool receive(const TranslateInLineCommand& command);
		virtual bool receive(const TranslateInPlaneCommand& command);
		virtual bool receive(const Scale1DCommand& command);
		virtual bool receive(const Scale2DCommand& command);
		virtual bool receive(const ScaleUniformCommand& command);
		virtual bool receive(const Rotate3DCommand& command);

		bool AddNode(SceneNode* node);
		void AddNodes(vector<SceneNode*> nodes);
		void ClearNodes();

	private:
		vector<SceneNode*> m_draggerNodes;
	};



}
#endif // SHAPE_NODE_H
