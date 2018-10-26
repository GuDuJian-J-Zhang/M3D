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
			dragTranslation = Vector3(0, 0, 0);
			m_view->GetExplosiveView()->startExplosion(arrayModels);
		} 
		else if (command.getStage() == MotionCommand::MOVE)
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
		else if (command.getStage() == MotionCommand::FINISH)
		{
			m_view->GetExplosiveView()->endExplosion();
		}

		return false;
	}

	bool ExplosionDragerCallBack::receive(const TranslateInLineCommand& command) {

		Dragger* dragger = command.GetRefDragger();
		if (!dragger)
		{
			return false;
		}

		Vector3 translation = command.getTranslation();
		translation = command.getLocalToWorld()*Vector4(translation, 0);
		//ÀÛ»ýÍÏ¶¯¾àÀë
		dragTranslation = translation + dragTranslation;
		
		//±¬Õ¨³ß´ç
		float percent = 0;
		//±¬Õ¨ÀàÐÍ
		int type = 0;
		//·½Ïò±êÊ¶·û
		int direction = 0;

		//ÅÐ¶ÏÊÇÑØ×ÅÄÄ¸öÖáÍÏ¶¯ÍÏ×§Æ÷
		if (dragger->GetDraggerName() == L"MinusXAxis")
		{
			//Ö¸¶¨ÖáµÄ±¬Õ¨²Ù×÷
			dragTranslationAll = dragTranslationAll + translation;
			m_view->SetExplosiveViewWithDirection(arrayModels, type, percent, dragTranslationAll);
		}
		else
		{
			//±ê×¼ÖáµÄÍÏ×§±¬Õ¨
			if (dragger->GetDraggerName() == L"XAxis")
			{
				percent = dragTranslation.m_x / boundingBox.GetXLength() * 100;
				direction = 1;
			}
			else if (dragger->GetDraggerName() == L"YAxis")
			{
				percent = dragTranslation.m_y / boundingBox.GetYLength() * 100;
				direction = 2;
			}
			else if (dragger->GetDraggerName() == L"ZAxis")
			{
				percent = dragTranslation.m_z / boundingBox.GetZLength() * 100;
				direction = 3;

			}
			if (explosionType == 0)
			{
				type = 0;
			}
			else if (explosionType == 1)
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
		}
		
		return true;
	}
}