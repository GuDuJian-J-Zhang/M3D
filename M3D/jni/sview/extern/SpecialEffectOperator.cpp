#include <stdlib.h>

#include "Utility.h"

#include "sview/extern/SpecialEffectOperator.h"
#include "m3d/scene/SceneNode.h"
#include "sview/extern/WorkNodes.h"
#include "m3d/SceneManager.h"
#include "sview/extern/PerspectiveData.h"
#include "sview/views/View.h"

#include "m3d/SceneManager.h"
#include "m3d/RenderManager.h"
#include "m3d/scene/GroupNode.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/action/CallBackAction.h"
#include "m3d/action/RayPickAction.h"

#include "m3d/utils/Trackball.h"

using namespace SVIEW;

namespace M3D
{
const int SpecialEffectOperator::M_EFFECT_SPEED = 3;
const int SpecialEffectOperator::M_EFFECT_KEEPTIME = 2;

SpecialEffectOperator* SpecialEffectOperator::instance = NULL;

SpecialEffectOperator* SpecialEffectOperator::GetInstance()
{
	if(SpecialEffectOperator::instance == NULL)
	{
		SpecialEffectOperator::instance = new SpecialEffectOperator();
	}

	return SpecialEffectOperator::instance;
}

SpecialEffectOperator::SpecialEffectOperator()
{
	this->Reset();
}

SpecialEffectOperator::~SpecialEffectOperator()
{

}

bool SpecialEffectOperator::ApplyBlinkEffect(View* view, int style,vector<Shape*>& shapes,
		vector<Color>& colors,int effectSpeed ,int keepTime)
{
	bool operaState = false;
    
    if(shapes.size() > 0 && colors.size() > 0){
        //正在进行动画
        StopEffect();
        
        this->m_effectStyle = style;
        this->m_effectSpeed = effectSpeed;
        this->m_effectKeepTime = keepTime;
        this->m_pView = view;
        
        m_effectShapes = shapes;
        this->m_blinkColors = colors;
        this->StartEffect();
        
        operaState  = true;
    }
    
	return operaState;
}

bool SpecialEffectOperator::CloseEffect(View* view)
{
	bool closeState = false;

	this->StopEffect();

	return closeState;
}

void SpecialEffectOperator::StartEffect()
{
	m_timer.SetTimer(BlinkEffectCallback, this, 0, 1000/m_effectSpeed);
	m_timer.StartTimer();
}

void SpecialEffectOperator::StopEffect()
{
	if( m_timer.IsStart())
	{
	 if(this->m_effectFinishCallback && m_effectFinishCallbackData)
	 {
		this->m_effectFinishCallback(this->m_effectFinishCallbackData);
	 }
      m_timer.StopTimer();
	}

	if(this->m_effectStyle == BLINK_EFFECT ){
		this->StopBlinkEffect();
	}

	this->Reset();
}

void SpecialEffectOperator::StopBlinkEffect()
{
//	int colorIndex = 0;
//	if (colorIndex < m_blinkColors.size())
//	{
		for (int shapeIndex = 0; shapeIndex < m_effectShapes.size();
				shapeIndex++)
		{
			Shape* shape = m_effectShapes[shapeIndex];
			if (shape != NULL)
			{
				//shape->SetColor(m_blinkColors[colorIndex]);
                shape->ResetColor();
			}
		}
    if(this->m_pView != NULL)
    {
        this->m_pView->RequestDraw();
    }
//	}
}

void SpecialEffectOperator::Reset()
{

    m_effectSpeed = M_EFFECT_SPEED;

	m_pView = NULL;

	m_currentColorIndex = 0;

	m_timerTickTimees = -1;

	m_blinkColors.clear();//闪烁颜色
	m_effectShapes.clear();
	m_effectNodes.clear();

	this->m_effectKeepTime = M_EFFECT_KEEPTIME;

	m_effectFinishCallback = NULL;
    m_effectFinishCallbackData  = NULL;
}

void* SpecialEffectOperator::BlinkEffectCallback(void* data)
{
	SpecialEffectOperator* effectOperator = (SpecialEffectOperator*)data;

	View* view = effectOperator->m_pView;

	vector<Color>& blinkColor = effectOperator->m_blinkColors;
	vector<Shape*>& shapes = effectOperator->m_effectShapes;
 
	static bool useInitialColor = false;

	if(effectOperator->m_currentColorIndex  < blinkColor.size()){
		for(int shapeIndex = 0;shapeIndex < shapes.size();shapeIndex++)
		{
			Shape* shape = shapes[shapeIndex];
			if(shape != NULL)
			{
				if(useInitialColor){
					shape->ResetColor();
				}else
				{
					shape->SetColor(blinkColor[effectOperator->m_currentColorIndex ]);
                    effectOperator->m_currentColorIndex ++;
				}
				useInitialColor = !useInitialColor;
			}
		}
		//LOGI("blink color : ColorIndex %d", effectOperator->m_currentColorIndex );
		view->RequestDraw();
	}else{
		effectOperator->m_currentColorIndex  = 0;
	}

	if(effectOperator->m_timerTickTimees ++ >= effectOperator->m_effectKeepTime *2){
		//LOGE("blink color stop tickTime:%d keepTime %d",effectOperator->m_timerTickTimees,effectOperator->m_effectKeepTime *2);
		effectOperator->StopEffect();
	}

	return 0;
}


}

