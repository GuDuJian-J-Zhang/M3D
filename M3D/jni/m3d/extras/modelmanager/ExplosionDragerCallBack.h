/**@file
*@brief
*@author	HBM
*@date		2018-7-24
*@version	1.0
*
*/

#ifndef M3D_EXPLOSION_DRAGERCALLBACK_H
#define M3D_EXPLOSION_DRAGERCALLBACK_H

#include "m3d/handler/dragger.h"
#include "sview/views/View.h"
using namespace SVIEW;
namespace M3D
{
	class M3D_API ExplosionDragerCallBack :public DraggerCallback
	{
	public:
		View * m_view;
		Vector3 dragTranslation;
		Vector3 dragTranslationAll;
		//爆炸类型
		int explosionType;
		//选中的模型集合
		vector<Model*> arrayModels;
		ExplosionDragerCallBack();
		BoundingBox boundingBox;
		virtual ~ExplosionDragerCallBack();
		/**
		* Receive motion commands. Returns true on success.
		*/
		virtual bool receive(const MotionCommand&);
		virtual bool receive(const TranslateInLineCommand& command);
	};
}
#endif // SHAPE_NODE_H
