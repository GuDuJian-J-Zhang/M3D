/*
 * FPSNode.cpp
 *
 *  Created on: 2013-9-26
 *      Author: bing
 */
#include "m3d/M3D.h"
#include "m3d/scene/FPSNode.h"
#include "m3d/scene/GroupNode.h"

#include "sview/views/Parameters.h"
#include "m3d/model/ComText.h"
#include "m3d/utils/M3DTools.h"
#include "m3d/action/RenderAction.h"

using namespace SVIEW;

namespace M3D
{
map<string,ComText*> FPSNode::fpsTexts;

FPSNode::FPSNode():SceneNode()
{
	m_iSceneHeight = 600;
	m_iViewHeight = 100;
	m_iViewWidth = 400;
	m_iViewX = 0;
	m_iViewY = (int) (m_iSceneHeight * 0.9 - m_iViewHeight);

	m_pTextNode = new TextNode();
	m_pTextNode->SetText("FPS:0");
	m_pTextNode->m_color.SetColor(1, 0, 0, 1);
	m_fFrameRate = 0;
	m_iFrameCount = 0;
	m_lLastFrameTime = GetTimeLong();

	//InitFpsTexts();
}
FPSNode::~FPSNode()
{
	if(m_pTextNode)
	{
		delete m_pTextNode;
		m_pTextNode = NULL;
	}

}

void FPSNode::SetFPS(float fps)
{
	m_fps = fps;
	char tmpText[20];
	memset(tmpText, '\0', sizeof(tmpText));
	sprintf(tmpText, "FPS:%.2f", fps);
//	m_pTextNode->SetText(tmpText);
	//	LOGI("FPSNode::setFPS %s", tmpText);


}

//Note that the order of release
//void FPSNode::FinalRelease(void)
//{
//	delete m_pTextNode;
//	Object::FinalRelease();
//}

void FPSNode::SetSceneHeight(int height)
{
	m_iSceneHeight = height;
	m_iViewY = (int) (m_iSceneHeight * 0.9 - m_iViewHeight);
}

int FPSNode::GetType(void)
{
	return FPS_NODE;
}

void FPSNode::FindVisiableObject(RenderAction* renderAction)
{
	if (!Parameters::Instance()->m_IsShowFPS || !this->IsVisible())
	{
		renderAction->SetFPSNode(NULL);
	}
	else
	{
		renderAction->SetFPSNode(this);
	}
}

ComText* FPSNode::GetFPSMesh()
{
	this->InitFpsTexts();
	ComText* ret = NULL;
	map<string,ComText*>::iterator it = fpsTexts.find(M3DTOOLS::floatToString(this->m_fps,0) + "f/s");
	if( it!= fpsTexts.end())
	{
		ret = it->second;
	}
	else
	{
		ret = NULL;
	}

	return ret;
}

void FPSNode::InitFpsTexts()
{
	if(fpsTexts.size() == 0)
	{
		  fpsTexts.clear();
		  for(int i=0;i<100;i++)
		  {
				//临时这样用
			ComText* ct = new ComText();
			CText* t = new CText;

			string text =  M3DTOOLS::IntToString(i) + "f/s";

			t->SetText(text);
			Vector3 pos;
			t->SetInnerLoc(pos);

			Vector3 xAxis, yAxis;
			xAxis.m_x = 0;
			xAxis.m_y = 0;
			xAxis.m_z = 0;
			t->SetInnerXYAxis(xAxis, yAxis);

			float fontSize = 20;

			//width height
			t->SetCharWidthHeight(fontSize, fontSize);
			ct->AddCText(t);

			fpsTexts.insert(map<string,ComText*>::value_type(text,ct));
		  }
	}

}
}
