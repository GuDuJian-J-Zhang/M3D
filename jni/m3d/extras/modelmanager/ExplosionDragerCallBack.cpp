#include "m3d/M3D.h"
#include "m3d/extras/modelmanager/ExplosionDragerCallBack.h"
#include "sview/extern/ExplosiveViewOperator.h"
#include "sview/views/View.h"
#include "m3d/base/BoundingBox.h"
#include "m3d/SceneManager.h"
#include "m3d/model/Model.h"


namespace M3D
{
	ExplosionDragerCallBack::ExplosionDragerCallBack()
	{

	}

	ExplosionDragerCallBack::~ExplosionDragerCallBack()
	{
		
	}
	bool ExplosionDragerCallBack::receive(const MotionCommand& command)
	{
		if (command.getStage() == MotionCommand::START)
		{
			//dragTranslation =  Vector3(0,0,0);
		} else
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
			
		}

		return false;
	}

	bool ExplosionDragerCallBack::receive(const TranslateInLineCommand& command) {

		Vector3 translation = command.getTranslation();
		translation = command.getLocalToWorld()*Vector4(translation, 0);
		//�ۻ��϶�����
		dragTranslation = translation + dragTranslation;
		//��ը�����Χ��
		//��Ҫʵ����һ�Σ��Ż���Ӧ�ٶ�
// 		BoundingBox boundingBox;
// 		if (arrayModels.size() == 0)
// 		{
// 			boundingBox = m_view->GetSceneManager()->GetSceneBox();
// 		}
// 		else
// 		{
// 			boundingBox = ((Model*)arrayModels[0])->GetWorldBoundingBox();
// 			for (int i=1; i<arrayModels.size();i++)
// 			{
// 				boundingBox.Merge(((Model*)arrayModels[i])->GetWorldBoundingBox());
// 			}
// 		}
		//��ը�ߴ�
		float percent = 0;
		//��ը����
		int type = 0;
		//�����ʶ��
		int direction = 0;
		//�ж��������ĸ����϶���ק��
		if (translation.m_x != 0)
		{
			float xLength = boundingBox.GetXLength();
			percent = dragTranslation.m_x / xLength * 100;
			direction = 1;
		}
		else if (translation.m_y != 0)
		{
			float YLength = boundingBox.GetYLength();
			percent = dragTranslation.m_y / YLength * 100;
			direction = 2;
		}
		else if (translation.m_z != 0)
		{
			float zLength = boundingBox.GetZLength();
			percent = dragTranslation.m_z / zLength * 100;
			direction = 3;
			
		}

		if (explosionType == 1)
		{
			type = direction;
		}
		else if (explosionType == 2)
		{
			type = direction + 6;
		}

		if (arrayModels.size() == 0) 
		{
			m_view->SetExplosiveView(type, percent, true);
		}
        else
        {
            m_view->SetExplosiveView(arrayModels, type, percent, true);
        }
		
		return true;
	}
}
