#include "m3d/extras/annotation/AnnotationDisplayHelper.h"
#include "m3d/base/Color.h"
#include "m3d/extras/measure/tools/MeasureDisplayHelper.h"
#include "sview/views/Parameters.h"
#include "m3d/base/glue/GlueObj.h"
#include "m3d/SceneManager.h"
#include "m3d/graphics/ImageBoard.h"
#include "m3d/ResourceManager.h"

namespace M3D
{
	AnnotationDisplayHelper::AnnotationDisplayHelper()
	{

	}
	AnnotationDisplayHelper::~AnnotationDisplayHelper()
	{

	}

	ImageBoard* AnnotationDisplayHelper::createAnnotationImage(SceneManager* scene, 
		string& annotationNote, float& fontSize, int& rows, Vector2& rectStart, Vector2& rectEnd,
		Color &frameColor, Color& fillColor, Vector3 & position)
	{
		LOGI("createAnnotationImage Begin");
		ImageBoard * ret = NULL;
		GlueObj* gluObj = scene->GetGlueObj();
		Shape2DSet * shape2DSet = new Shape2DSet();
		Color blk = Color::BLACK;
		Color gr = Color::GRAY;
		Color wt = Color::WHITE;
		Color textColor(1, 0, 0);
		Color lineColor(frameColor);
		Color inColor(fillColor);
		float lineWidth = 4.0f;

		textColor = SVIEW::Parameters::Instance()->m_annotationDisplayColor;
		CreateAnnotationRectangleImage(shape2DSet, rectStart, rectEnd, lineColor,
			lineWidth, inColor, textColor, fontSize, rows, annotationNote);
		LOGI("end CreateNoteRectangleImage");

		float RectWight = rectEnd.m_x;
#ifdef WIN32
		float wideFactor = RectWight / 90.0f;
		float heightFactor = (fontSize * 1.5) / 90.0f + (fontSize * 1.2 * (rows - 1)) / 90.0f;//(rectEnd.m_y - rectStart.m_y) / 90.0f;
#else
		float wideFactor = RectWight / 100.0f;
		float heightFactor = (fontSize * 2.5) / 100.0f;
#endif
		string createImagePath = "";
		if (gluObj)
		{
			LOGI("gluObj if %p", gluObj);
			createImagePath = gluObj->createImage(shape2DSet, scene);
		}

		delete shape2DSet;
		shape2DSet = NULL;
		LOGI("createImagePath.length() %d", createImagePath.length());
		if (createImagePath.length() > 0)
		{

			Vector2 imageBoardSize(wideFactor, heightFactor);

			ret = new ImageBoard(position, imageBoardSize);

			ret->SetTexture(
				scene->GetResourceManager()->GetOrCreateTexture(
					createImagePath));
		}

		LOGI("createAnnotationImage end , return ret %p,release memory", ret);
		return ret;
	}

	ImageBoard* AnnotationDisplayHelper::createAnnotation(SceneManager* scene,
						string& annotationNote, float& fontSize, int& rows,
						Vector2& rectStart, Vector2& rectEnd, Vector3 & position)
	{
		LOGI("createAnnotation Begin");
		ImageBoard * ret = NULL;
		GlueObj* gluObj = scene->GetGlueObj();
		Shape2DSet * shape2DSet = new Shape2DSet();
		Color blk = Color::BLACK;
		Color gr = Color::GRAY;
		Color wt = Color::WHITE;
		Color textColor(1, 0, 0);
		Color inColor;
		inColor.m_a = 0;
		float lineWidth = 4.0f;
		CreateAnnotationRectangleImage(shape2DSet, rectStart, rectEnd,
		   inColor, textColor, fontSize, rows, annotationNote);
		LOGI("end CreateNoteRectangleImage");

		float RectWight = rectEnd.m_x;
#ifdef WIN32
		float wideFactor = RectWight / 90.0f;
		float heightFactor = (fontSize * 1.5) / 90.0f + (fontSize * 1.2 * (rows - 1)) / 90.0f;//(fontSize * 1.7 * rows) / 90.0f;
		
#else
		float wideFactor = RectWight / 100.0f;
		float heightFactor = (fontSize * 2.5) / 100.0f;
#endif
		string createImagePath = "";
		if (gluObj)
		{
			LOGI("gluObj if %p", gluObj);
			createImagePath = gluObj->createImage(shape2DSet, scene);
		}

		delete shape2DSet;
		shape2DSet = NULL;
		LOGI("createImagePath.length() %d", createImagePath.length());
		if (createImagePath.length() > 0)
		{

			Vector2 imageBoardSize(wideFactor, heightFactor);

			ret = new ImageBoard(position, imageBoardSize);

			ret->SetTexture(
				scene->GetResourceManager()->GetOrCreateTexture(
					createImagePath));
		}

		LOGI("createAnnotation end , return ret %p,release memory", ret);
		return ret;
	}

	void AnnotationDisplayHelper::CreateAnnotationRectangleImage(Shape2DSet*shape2DSet,
		Vector2 & rectStart, Vector2 & rectEnd, Color & lineColor, float lineWidth, Color & backColor, Color& fontColor, float fontSize, int& rows, string str)
	{
		float rect1StartX = rectStart.m_x;
		float rect1StartY = rectStart.m_y;
		float rect1EndX = rectEnd.m_x;
		float rect1EndY = rectEnd.m_y;
		float deltaLineWidth = lineWidth / 2.0f;

		//创建矩形
		Rect2D * rect1 = new Rect2D();
		rect1->SetColor(backColor);
		rect1->m_start = rectStart;
		rect1->m_end = rectEnd;
		shape2DSet->AddShape2D(rect1);
		//创建矩形文本

		Texts2D* text1 = new Texts2D;
		text1->SetColor(fontColor);
#ifdef WIN32
		Vector2 text1Start(rect1StartX,
			(rect1StartY + rect1EndY) / rows / 2.0 - fontSize / (rows + 1));//2.0f);
#else
		Vector2 text1Start(rect1StartX + fontSize / 2.0f,
			(rect1StartY + rect1EndY) / 2.0 + fontSize / 3.0f);
#endif // WIN32
		text1->m_start = text1Start;
		LOGI("text1 start %s", text1->m_start.Tostring().c_str());
		text1->m_size = fontSize;
		text1->m_texts = str;
		shape2DSet->AddShape2D(text1);

		//线1
		Line2D * line1d1 = new Line2D();
		line1d1->m_blod = lineWidth;
		line1d1->m_start = Vector2(rect1StartX + deltaLineWidth, rect1StartY);
		line1d1->m_end = Vector2(rect1StartX + deltaLineWidth, rect1EndY);
		line1d1->SetColor(lineColor);
		shape2DSet->AddShape2D(line1d1);

		//线2
		Line2D * line1d2 = new Line2D();
		line1d2->m_blod = lineWidth;
		line1d2->m_start = Vector2(rect1StartX, rect1StartY + deltaLineWidth);
		line1d2->m_end = Vector2(rect1EndX, rect1StartY + deltaLineWidth);
		line1d2->SetColor(lineColor);
		shape2DSet->AddShape2D(line1d2);
		//线3

		Line2D * line1d3 = new Line2D();
		line1d3->m_blod = lineWidth;
		line1d3->m_start = Vector2(rect1StartX, rect1EndY - deltaLineWidth);
		line1d3->m_end = Vector2(rect1EndX, rect1EndY - deltaLineWidth);
		line1d3->SetColor(lineColor);
		shape2DSet->AddShape2D(line1d3);
		//线4
		Line2D * line1d4 = new Line2D();
		line1d4->m_blod = lineWidth;
		line1d4->m_start = Vector2(rect1EndX - deltaLineWidth, rect1StartY);
		line1d4->m_end = Vector2(rect1EndX - deltaLineWidth, rect1EndY);

		line1d4->SetColor(lineColor);
		shape2DSet->AddShape2D(line1d4);
	}

	void AnnotationDisplayHelper::CreateAnnotationRectangleImage(Shape2DSet*shape2DSet, Vector2 & rectStart, Vector2 & rectEnd,
		Color & backColor, Color& fontColor, float fontSize, int& rows, string str)
	{
		float rect1StartX = rectStart.m_x;
		float rect1StartY = rectStart.m_y;
		float rect1EndX = rectEnd.m_x;
		float rect1EndY = rectEnd.m_y;

		//创建矩形
		Rect2D * rect1 = new Rect2D();
		rect1->SetColor(backColor);
		rect1->m_start = rectStart;
		rect1->m_end = rectEnd;
		shape2DSet->AddShape2D(rect1);
		//创建矩形文本

		Texts2D* text1 = new Texts2D;
		text1->SetColor(fontColor);
#ifdef WIN32
		Vector2 text1Start(rect1StartX,
		    (rect1StartY + rect1EndY) / rows / 2.0 - fontSize / (rows + 1));//2.0f);
#else
		Vector2 text1Start(rect1StartX + fontSize / 2.0f,
			(rect1StartY + rect1EndY) / 2.0 + fontSize / 3.0f);
#endif // WIN32
		text1->m_start = text1Start;
		LOGI("text1 start %s", text1->m_start.Tostring().c_str());
		text1->m_size = fontSize;
		text1->m_texts = str;
		shape2DSet->AddShape2D(text1);
	}

	void AnnotationDisplayHelper::GetAnnotationTextRectInfo(SceneManager* scene, vector<Texts2D*>& textsArray, 
		string& annoNote, float& fontSize, Vector2& rectStart, Vector2& rectEnd, int& rows)
	{
		string noteStr = "";
		if (textsArray.size() == 0)
		{
			return;
		}
		noteStr = textsArray[0]->m_texts;
		string department = SVIEW::Parameters::Instance()->m_annotationDepartment;
		string character = SVIEW::Parameters::Instance()->m_annotationCharacter;
		string user = SVIEW::Parameters::Instance()->m_annotationUser;
		int iLanguage = SVIEW::Parameters::Instance()->m_internationalLanguage;
		if (department.compare("") != 0 || character.compare("") != 0 || user.compare("") != 0)
		{
#ifdef WIN32
			noteStr += "\r\n";
#else
			noteStr += "\n";
#endif
		}
		if (department.compare("") != 0)
		{
			string name = iLanguage == 0 ? "部门:" : "Department:";
			noteStr += name + department + " ";
		}
		if (character.compare("") != 0)
		{
			string name = iLanguage == 0 ? "角色:" : "Character:";
			noteStr += name + character + " ";
		}
		if (user.compare("") != 0)
		{
			string name = iLanguage == 0 ? "用户:" : "User:";
			noteStr += name + user;
		}

		fontSize = textsArray[0]->m_size;
		if (fontSize < 1.0)
			fontSize = 15.0f;
		fontSize = fontSize * 40 / 15.0f;
		LOGI(" createNoteTextsImageN::scene %p", scene);
		GlueObj* gluObj = scene->GetGlueObj();

		int notZh = 0;
		int charNumber = 0;//MeasureDisplayHelper::countChar(noteStr, notZh);
		int rowNumber = 1;
		string maxStr = countChar(noteStr, rowNumber, charNumber);

#ifdef WIN32
		float strLength = notZh * 0.6f + (charNumber - notZh) * 2;
#else
		float strLength = notZh * 0.6f + (charNumber - notZh);
#endif
		float RectWight = (strLength + 1) * fontSize;
#ifdef WIN32
		float ScaleFactor = 1.0f;
		Vector2 tempStart(0.5, 0.5); //2.5 为调节因子
		Vector2 tempEnd(RectWight, fontSize * 3.5 + 1);
		Vector2 tempStartNoUse(RectWight, fontSize * 3.5 + 1);
		gluObj->GetMeasureTextFun()(maxStr.c_str(), fontSize, "楷体", (float*)tempStartNoUse.Data(), (float*)tempEnd.Data());
		tempEnd.m_y = tempEnd.m_y*rowNumber;// +1.0f;//留出结束空白字符
		tempEnd.m_y = rowNumber > 1 ? tempEnd.m_y * 0.8 : tempEnd.m_y;
		tempEnd.m_y += 1.0f;
		tempEnd.m_x = tempEnd.m_x*0.7f;
		//tempEnd.m_x = tempEnd.m_x*0.7f*2.0f;
		//tempStart.m_x = tempEnd.m_x / 2.0f;

		tempEnd.m_x = tempEnd.m_x + 10.0f;//留出结束空白字符
		tempStart = tempStart*ScaleFactor;
		tempEnd = tempEnd*ScaleFactor;
		RectWight = tempEnd.m_x;
		fontSize = fontSize* ScaleFactor;
#else	
		Vector2 tempStart(1, 1); //2.5 为调节因子
		Vector2 tempEnd(RectWight, fontSize * 2.5 + 1);
#endif // WIN32	

		annoNote = noteStr;
		rectStart = tempStart; 
		rectEnd = tempEnd;
		rows = rowNumber;
	}

	string AnnotationDisplayHelper::countChar(string &str, int &rowCount, int& clomnCount)
	{
		size_t tmpLength = 0;
		size_t length = 0;
		size_t rows = 1;
		string maxStr = "";
		string tmpstr = "";
		for (size_t i = 0; i < str.length(); i++)
		{
			unsigned char byte = str[i];
			tmpstr += byte;
			if (byte == '\n' && i > 0)
			{
				byte = str[i - 1];
				if (byte == '\r')
				{
					if (tmpLength < length)
					{
						tmpLength = length - 1;
						maxStr = tmpstr.substr(0, tmpstr.length() - 2);
					}
					rows++;
					length = 0;
					tmpstr = "";
				}
			}
			
			if (i % 2 == 0)
			{
				length++;
			}
		}

		if (tmpLength < length)
		{
			tmpLength = length;
			maxStr = tmpstr;
		}
		rowCount = rows;
		clomnCount = tmpLength;
		return maxStr;
	}

}