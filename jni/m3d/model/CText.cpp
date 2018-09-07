/*
 * CText.cpp
 *
 *  Created on: 2014-2-20
 *      Author: bing
 */

#include "CText.h"
#include "Utility.h"
#ifdef __MOBILE__
#include "FTGL/ftgles.h"
#elif _WIN32
#include "ftgl/ftgl.h"
#endif

namespace M3D
{

FTFont* CText::fontRender = NULL;
 char	CText::m_FontFilePath[256]={0};		//!< 字库文件路径

FTFont* CText::GetRender()
{
	if (CText::fontRender == NULL)
	{
		CText::InitRender();
	}
	return CText::fontRender;
}

void CText::InitRender()
{
	if(strlen(CText::m_FontFilePath) == 0)
	{
#ifdef __ANDROID__
		CText::SetFontFilePath("system/fonts/DroidSansFallback.ttf");
#elif __IOS__
		CText::SetFontFilePath("//System/Library/Fonts/Cache/STHeiti-Light.ttc");
#elif WIN32
CText::SetFontFilePath("./data/Fonts/HT_bold.ttf");
#endif
	}

	FILE* file = fopen(CText::m_FontFilePath, "r");
	if(file == NULL)
	{
		LOGE("font '%s' not exist!TextNode::font initialize ERROR!",CText::m_FontFilePath);
		return;
	}
	fclose(file);
    
	CText::fontRender = new FTPolygonFont(CText::m_FontFilePath); //new FTTextureFont(fontPath);//
	CText::fontRender->CharMap(FT_ENCODING_UNICODE);
	CText::fontRender->FaceSize(1);

	LOGI("TextNode::font initialized use FTPolygonFont with file:'%s'",CText::m_FontFilePath);
}

CText::CText() {

	memset(m_fCharSize,0,2*sizeof(float));
}


///TODO
CText::~CText() {
	// TODO Auto-generated destructor stub
}

void CText::GetFontFilePath(char* o_wchFont)
{
	for (int ii = 0; CText::m_FontFilePath[ii] != '\0'; ii++ )
	{
		*o_wchFont = CText::m_FontFilePath[ii];
		o_wchFont++;
	}
	*o_wchFont = '\0';
}

void CText::SetFontFilePath(const char* i_wchFont)
{
	memset(CText::m_FontFilePath,'\0',sizeof(CText::m_FontFilePath));
	int ii = 0;
	while (*i_wchFont != '\0')
	{
		CText::m_FontFilePath[ii] = *i_wchFont;
		ii++;
		i_wchFont++;
	}
	CText::m_FontFilePath[ii] = '\0';
}

void CText::GetInnerXYAxis(Vector3& o_dirXAxis, Vector3& o_dirYAxis)
{
	o_dirXAxis = m_dirAxis[0];
	o_dirYAxis = m_dirAxis[1];
}

void CText::SetInnerXYAxis(const Vector3& i_dirXAxis, const Vector3& i_dirYAxis)
{
	m_dirAxis[0] = i_dirXAxis;
	m_dirAxis[1] = i_dirYAxis;
}

void CText::GetCharWidthHeight(float& o_fWidth, float& o_fHeight)
{
	o_fWidth = m_fCharSize[0];
	o_fHeight = m_fCharSize[1];
}
void CText::SetCharWidthHeight(const float& iWidth, const float& iHeight)
{
	 m_fCharSize[0] = iWidth;
	 m_fCharSize[1] = iHeight;
}

void CText::CreateBuff()
{
//	vector<FTPoint> pnt;
//	vector<int> pntnum;
//	vector<int> type;
//
//		fontRender->Render(string,pnt,pntnum,type);
//
//		CText* text = texts[j];
//		//文字方向
//
//		glTranslatef(text->GetInnerLoc().X, text->GetInnerLoc().Y,
//					text->GetInnerLoc().Z);
//		STK_DIR32 XAxis, YAxis;
//		text->GetInnerXYAxis(XAxis, YAxis);
//		//glRotatef(XAxis.X, XAxis.Y, XAxis.Z, 0);
//		static float angle = 0;
//		//angle+=0.1;
//		//glRotatef(angle, 0, 1, 0);
//		//文字高度
//		FLOAT Height, Width, ScaleH, ScaleW;
//		text->GetCharWidthHeight(Width, Height);
//		ScaleW = Width * 0.08;
//		ScaleH = Width * 0.08;
//		glScalef(ScaleW, ScaleH, 1.0f);
//
//		//文字间隔
//		float ShiftX, ShiftY;
//		float ChrSpc; // 文字间隔
//		ChrSpc = text->GetCharSpacing();
//		ShiftX = ChrSpc;
//		ShiftY = 0.0;
//
//		TextNode* tn = new TextNode();
//		if (pmiData->m_IsParallelScreen && pmiData->m_Type != 8000)
//		{
//			//Vector3 dis;
//			//dis=Vector3(texts[j-1]->GetInnerLoc().X,texts[j-1]->GetInnerLoc().Y,texts[j-1]->GetInnerLoc().Z )
//			//	- Vector3(text->GetInnerLoc().X, text->GetInnerLoc().Y, text->GetInnerLoc().Z);
//			//tn->x=20*dis.x;
//			//tn->y =20*dis.y;
//			tn->m_Pos.y = 20 * (texts.size() - j - 1);
//			//	tn->z= 20*dis.z;
//			//	LOGE("dis is %f %f %f", dis.x,dis.y,dis.z);
//		}
//		string str = WStringToString(text->GetText());
//		tn->SetText((char*) str.c_str());
//		tn->m_color.SetColor(0, 0, 1, 1);
//		//LOGE("Text is %s j is %d",str.c_str(),j);
//		DrawText(tn, action);
//		delete tn;
//

}



}//ns
