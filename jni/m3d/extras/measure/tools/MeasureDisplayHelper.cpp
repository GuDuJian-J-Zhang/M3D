#include "m3d/extras/measure/tools/MeasureDisplayHelper.h"
#include "m3d/model/GeoAttribute.h"
#include "m3d/extras/measure/Measure.h"
#include "m3d/extras/measure/MeasureAngle.h"
#include "m3d/extras/measure/MeasureDistance.h"
#include "m3d/extras/measure/MeasureProperty.h"
#include "m3d/SceneManager.h"
#include "m3d/base/json/json.h"
#include "m3d/utils/M3DTools.h"
#include "m3d/graphics/ImageBoard.h"
#include "m3d/base/glue/GlueObj.h"
#include "m3d/utils/ShapeHelper.h"
#include "m3d/ResourceManager.h"
#include "m3d/base/Ray.h"
#include "m3d/base/Plane.h"
#include "m3d/SceneManager.h"
#include "sview/views/Parameters.h"
#include "m3d/base/Quaternion.h"
namespace M3D
{

Shape2D::Shape2D()
{
	this->Init();
}

Shape2D::~Shape2D()
{

}

string Shape2D::toJson()
{
	string ret;

	return ret;
}

bool Shape2D::fromJson(const string& json)
{
	bool ret = false;

	return ret;
}

void Shape2D::SetColor(const Color& color)
{
	this->m_color = color;
}

Color& Shape2D::GetColor()
{
	return this->m_color;
}

void Shape2D::Init()
{
	this->m_type = SHAPE2D_BASE;
}

Texts2D::Texts2D() :
		Shape2D()
{
	this->Init();
}

Texts2D::~Texts2D()
{

}

string Texts2D::toJson()
{
	Json::Value retJson;

	retJson["Name"] = "Texts2D";

	retJson["StartX"] = this->m_start.m_x;
	retJson["StartY"] = this->m_start.m_y;

	retJson["FontSize"] = this->m_size;

	retJson["FontName"] = this->m_fontName;

	retJson["Texts"] = this->m_texts;

	retJson["ClrR"] = this->m_color.m_r;
	retJson["ClrG"] = this->m_color.m_g;
	retJson["ClrB"] = this->m_color.m_b;
	retJson["ClrA"] = this->m_color.m_a;

	retJson["AlignCenter"] = this->m_alignCenter;

	return retJson.toStyledString();
}

bool Texts2D::fromJson(const string& json)
{
	bool ret = false;

	return ret;
}

void Texts2D::Init()
{
	this->m_type = SHAPE2D_TEXT;
	m_size = 15.0f;
	m_alignCenter = false;
}

Line2D::Line2D() :
		Shape2D()
{
	this->Init();
}

Line2D::~Line2D()
{

}

string Line2D::toJson()
{
	Json::Value retJson;

	retJson["Name"] = "Line2D";

	retJson["StartX"] = this->m_start.m_x;
	retJson["StartY"] = this->m_start.m_y;

	retJson["EndX"] = this->m_end.m_x;
	retJson["EndY"] = this->m_end.m_y;

	retJson["LineType"] = this->m_lineType;

	retJson["ClrR"] = this->m_color.m_r;
	retJson["ClrG"] = this->m_color.m_g;
	retJson["ClrB"] = this->m_color.m_b;
	retJson["ClrA"] = this->m_color.m_a;

	retJson["Blod"] = this->m_blod;

	return retJson.toStyledString();
}

bool Line2D::fromJson(const string& json)
{
	bool ret = false;

	return ret;
}

void Line2D::Init()
{
	this->m_type = SHAPE2D_LINE;
	this->m_lineType = 0;
}

Rect2D::Rect2D() :
		Shape2D()
{
	this->Init();
}

Rect2D::~Rect2D()
{

}

string Rect2D::toJson()
{
	Json::Value retJson;

	retJson["Name"] = "Rect2D";

	retJson["StartX"] = this->m_start.m_x;
	retJson["StartY"] = this->m_start.m_y;

	retJson["EndX"] = this->m_end.m_x;
	retJson["EndY"] = this->m_end.m_y;
	retJson["Style"] = this->m_style;

	retJson["ClrR"] = this->m_color.m_r;
	retJson["ClrG"] = this->m_color.m_g;
	retJson["ClrB"] = this->m_color.m_b;
	retJson["ClrA"] = this->m_color.m_a;

	retJson["RadiusX"] = this->m_radius.m_x;
	retJson["RadiusY"] = this->m_radius.m_y;

	retJson["Stroke"] = this->m_stroke;

	retJson["Round"] = this->m_round;

	retJson["StrokeWidth"] = this->m_strokeWidth;

	return retJson.toStyledString();
}

bool Rect2D::fromJson(const string& json)
{
	bool ret = false;

	return ret;
}

void Rect2D::Init()
{
	this->m_type = SHAPE2D_RECT;
	m_strokeWidth = 2.0f;
	m_round = false;
	m_stroke = false;

}

Shape2DSet::Shape2DSet() :
		Shape2D()
{
	this->Init();
}

bool Shape2DSet::AddShape2D(Shape2D* shape2d)
{
	bool ret = false;
	if (shape2d != NULL)
	{
		ret = true;
		this->m_displayShapes.push_back(shape2d);
	}

	return ret;
}

string Shape2DSet::toJson()
{
	Json::Value retJson;

	//构造jsonArray
	for (int i = 0; i < m_displayShapes.size(); i++)
	{
		retJson[M3DTOOLS::IntToString(i)] = m_displayShapes[i]->toJson();
	}
	return retJson.toStyledString();
}

vector<Shape2D*>& Shape2DSet::GetShapes()
{
	return this->m_displayShapes;
}

bool Shape2DSet::fromJson(const string& json)
{
	bool ret = false;

	return ret;
}

Shape2DSet::~Shape2DSet()
{
	for (int i = 0; i < m_displayShapes.size(); i++)
	{
		delete m_displayShapes[i];
	}

	m_displayShapes.resize(0);
}

Rect Shape2DSet::GetRect()
{
	Rect ret;
	for (int i = 0; i < m_displayShapes.size(); i++)
	{
		Shape2D* shape2d = m_displayShapes[i];
		if (shape2d->GetType() == SHAPE2D_LINE)
		{
			Line2D* line = (Line2D*) shape2d;
			ret.Merge(line->m_start);
			ret.Merge(line->m_end);
		}
		else if (shape2d->GetType() == SHAPE2D_RECT)
		{
			Rect2D* rect = (Rect2D*) shape2d;
			ret.Merge(rect->m_start);
			ret.Merge(rect->m_end);
        }else if(shape2d->GetType() == SHAPE2D_ELLIPSE){
            Ellipse2D* rect = (Ellipse2D*) shape2d;
            ret.Merge(rect->m_start);
            ret.Merge(rect->m_end);
        }
	}
	return ret;
}

void Shape2DSet::Init()
{
	//this->m_type = SHAPE2D_BASE;
	m_displayShapes.resize(0);
}

Ellipse2D::Ellipse2D():Shape2D()
{
	this->Init();
}

Ellipse2D::~Ellipse2D()
{
}

void Ellipse2D::Init()
{
	this->m_type = SHAPE2D_ELLIPSE;
}

string Ellipse2D::toJson()
{
	Json::Value retJson;

	retJson["Name"] = "Ellipse2D";

	retJson["StartX"] = this->m_start.m_x;
	retJson["StartY"] = this->m_start.m_y;

	retJson["EndX"] = this->m_end.m_x;
	retJson["EndY"] = this->m_end.m_y;


	retJson["ClrR"] = this->m_color.m_r;
	retJson["ClrG"] = this->m_color.m_g;
	retJson["ClrB"] = this->m_color.m_b;
	retJson["ClrA"] = this->m_color.m_a;


	retJson["IsFill"] = this->m_isFill;

	retJson["StrokeWidth"] = this->m_strokeWidth;

	return retJson.toStyledString();
}

bool Ellipse2D::fromJson(const string& json)
{
	bool ret = false;
	return ret;
}

MeasureDisplayHelper::MeasureDisplayHelper()
{

}

MeasureDisplayHelper::~MeasureDisplayHelper()
{

}
void MeasureDisplayHelper::createShadowRect(Shape2DSet*shape2DSet,
		Vector2 & rectStart, Vector2 & rectEnd, float rect2L)
{
	float rect1StartX = rectStart.m_x;
	float rect1StartY = rectStart.m_y;
	float rect1EndX = rectEnd.m_x;
	float rect1EndY = rectEnd.m_y;
	//创建阴影底层矩形

	Rect2D * rect0 = new Rect2D();
	rect0->SetColor(Color(0.3, 0.3, 0.3));
	rect0->m_start = Vector2(rect1StartX + 8, rect1StartY + 8);
	rect0->m_end = Vector2(rect1EndX + rect2L + 8, rect1EndY + 8);
	shape2DSet->AddShape2D(rect0);
}

void MeasureDisplayHelper::createRectImage(Shape2DSet* shape2DSet,
		Vector2 & rect1Start, Vector2 & rect1End, float rect2L,
		Color& rect1Color, Color & rect2Color, Color& text1Color,
		Color & text2Color, string str1, string str2, float fontSize,
		bool drawD3)
{
	LOGI("createRectImage shape2DSet %p", shape2DSet);
	float rect1StartX = rect1Start.m_x;
	float rect1StartY = rect1Start.m_y;
	float rect1EndX = rect1End.m_x;
	float rect1EndY = rect1End.m_y;

	Color lineColor(0.45, 0.45, 0.45);

	//创建左边属性矩形
	Rect2D * rect1 = new Rect2D();
	rect1->SetColor(rect1Color);
	rect1->m_start = rect1Start;
	rect1->m_end = rect1End;
	shape2DSet->AddShape2D(rect1);
	//创建矩形文本

	Texts2D* text1 = new Texts2D;
	text1->SetColor(text1Color);
#ifdef WIN32
	Vector2 text1Start(rect1StartX + 10,
		(rect1StartY + rect1EndY) / 2.0 - fontSize / 2);
#else
	Vector2 text1Start(rect1StartX + 10,
		(rect1StartY + rect1EndY) / 2.0 + fontSize / 3);
#endif // WIN32
	text1->m_start = text1Start;
	LOGI("text1 start %s", text1->m_start.Tostring().c_str());
	text1->m_size = fontSize;
	text1->m_texts = str1;
	shape2DSet->AddShape2D(text1);

	//线1
	Line2D * line1d1 = new Line2D();
	line1d1->m_start = Vector2(rect1StartX, rect1StartY);
	line1d1->m_end = Vector2(rect1StartX, rect1EndY);
	line1d1->m_blod = 4.0;
	line1d1->SetColor(lineColor);
	shape2DSet->AddShape2D(line1d1);

	//线2
	Line2D * line1d2 = new Line2D();
	line1d2->m_start = Vector2(rect1StartX, rect1StartY);
	line1d2->m_end = Vector2(rect1EndX, rect1StartY);
	line1d2->m_blod = 4.0;
	line1d2->SetColor(lineColor);
	shape2DSet->AddShape2D(line1d2);
	//线3
	if (drawD3)
	{
		Line2D * line1d3 = new Line2D();
		line1d3->m_start = Vector2(rect1StartX, rect1EndY);
		line1d3->m_end = Vector2(rect1EndX, rect1EndY);
		line1d3->m_blod = 4.0;
		line1d3->SetColor(lineColor);
		shape2DSet->AddShape2D(line1d3);
	}
	//线4
	Line2D * line1d4 = new Line2D();
	line1d4->m_start = Vector2(rect1EndX, rect1StartY);
	line1d4->m_end = Vector2(rect1EndX, rect1EndY);
	line1d4->m_blod = 4.0;
	line1d4->SetColor(lineColor);
	shape2DSet->AddShape2D(line1d4);

	float rect2StartX = rect1EndX;
	float rect2StartY = rect1StartY;
	float rect2EndX = rect1EndX + rect2L;
	float rect2EndY = rect1EndY;

	//创建右边属性矩形
	Rect2D * rect2 = new Rect2D();
	rect2->SetColor(rect2Color);
	rect2->m_start = Vector2(rect2StartX, rect2StartY);
	rect2->m_end = Vector2(rect2EndX, rect2EndY);
	shape2DSet->AddShape2D(rect2);
	//创建矩形文本

	Texts2D* text2 = new Texts2D;
	text2->SetColor(text2Color);
#ifdef WIN32
	Vector2 text2Start(rect2StartX + 10,
			(rect2StartY + rect2EndY) / 2.0 - fontSize / 2);
#else
	Vector2 text2Start(rect2StartX + 10,
		(rect2StartY + rect2EndY) / 2.0 + fontSize / 3);
#endif
	text2->m_start = text2Start;
	LOGI("text2 start %s", text2->m_start.Tostring().c_str());
	text2->m_size = fontSize;
	text2->m_texts = str2;
	shape2DSet->AddShape2D(text2);

	//线1
	Line2D * line2d1 = new Line2D();
	line2d1->m_start = Vector2(rect2StartX, rect2StartY);
	line2d1->m_end = Vector2(rect2StartX, rect2EndY);
	line2d1->m_blod = 4.0;
	line2d1->SetColor(lineColor);
	shape2DSet->AddShape2D(line2d1);

	//线2
	Line2D * line2d2 = new Line2D();
	line2d2->m_start = Vector2(rect2StartX, rect2StartY);
	line2d2->m_end = Vector2(rect2EndX, rect2StartY);
	line2d2->m_blod = 4.0;
	line2d2->SetColor(lineColor);
	shape2DSet->AddShape2D(line2d2);
	//线3
	if (drawD3)
	{
		Line2D * line2d3 = new Line2D();
		line2d3->m_start = Vector2(rect2StartX, rect2EndY);
		line2d3->m_end = Vector2(rect2EndX, rect2EndY);
		line2d3->m_blod = 4.0;
		line2d3->SetColor(lineColor);
		shape2DSet->AddShape2D(line2d3);
	}

	//线4
	Line2D * line2d4 = new Line2D();
	line2d4->m_start = Vector2(rect2EndX, rect2StartY);
	line2d4->m_end = Vector2(rect2EndX, rect2EndY);
	line2d4->m_blod = 4.0;
	line2d4->SetColor(lineColor);
	shape2DSet->AddShape2D(line2d4);

}

void MeasureDisplayHelper::MeasureText(const string& measureText, float fontSize, const string& fontName, Vector2 & o_rect1Start, Vector2 & o_rect1End)
{

}

void MeasureDisplayHelper::addImageToMemory(SceneManager* scene,
		ImageBoard* &imageboard, Shape2DSet * shape2DSet, Vector3 pntInPlane,
		float wideFactor, float heightFactor)
{
	LOGI("MeasureDisplayHelper::addImageToMemory");
	GlueObj* gluObj = scene->GetGlueObj();

	LOGI("gluObj value %p", gluObj);
	string createImagePath = "";

	if (gluObj)
	{
		LOGI("gluObj if %p", gluObj);
		LOGI("shape2DSet if %p", shape2DSet);
		createImagePath = gluObj->createImage(shape2DSet, scene);
		LOGI("createImagePath %s", createImagePath.c_str());
	}

	delete shape2DSet;

	if (createImagePath.length() > 0)
	{
		Vector2 imageBoardSize(wideFactor, heightFactor);
		
		imageboard = new ImageBoard(pntInPlane, imageBoardSize);

		imageboard->SetTexture(
				scene->GetResourceManager()->GetOrCreateTexture(
						createImagePath));
	}
}

void MeasureDisplayHelper::GetRelativePosition(vector<Vector2>& position,
		int count, float rectangleWight, float fontsize)
{
	for (int i = 0; i < count; i++)
	{
		Vector2 tempStart(1, i * fontsize * 2.5 + 1); //2.5 为调节因子
		Vector2 tempEnd(rectangleWight, (i + 1) * fontsize * 2.5 + 1);

		position.push_back(tempStart);
		position.push_back(tempEnd);
	}
}

//创建批注文字显示的图片，暂时在这里实现
ImageBoard* MeasureDisplayHelper::createNoteTextsImage(SceneManager* scene,
		vector<Texts2D*>& textsArray, Vector3 & position)
{
	LOGI("C++ createNoteTextsImage");
	LOGI("textsArray size %d", textsArray.size());
	Shape2DSet * shape2DSet = new Shape2DSet();

	LOGI("shape2DSet %p", shape2DSet);

	Color gr = Color::GRAY;
	Color wt = Color::WHITE;
	Color blk = Color::BLACK;
	Color grn = Color::GREEN;
	Color rd = Color::RED;
	Color blu = Color::BLUE;
	Color dgn = Color(0, 100, 0);

	Color colorList[] =
	{ gr, dgn, blu, rd };

	vector<Vector2> rectPosition;
	vector<float> fontSizeList; //先保留

	float maxStrLengthLeft = 0.0f;
	float maxStrLengthRight = 0.0f;
	float maxFntSizeLeft = 0.0f;
	float maxFntSizeRight = 0.0f;

	//获得最大字符串长度，估计文本框长度
	for (vector<Texts2D*>::iterator it = textsArray.begin();
			it != textsArray.end(); it = it + 2)
	{
		float tempStrLengthLeft;
		fontSizeList.push_back((*it)->m_size);
		int notZh = 0;
		int charNumber = countChar((*it)->m_texts, notZh);
		tempStrLengthLeft = notZh * 0.50f + (charNumber - notZh);

		if (tempStrLengthLeft > maxStrLengthLeft)
		{
			maxStrLengthLeft = tempStrLengthLeft;
			maxFntSizeLeft = (*it)->m_size;
		}

		float tempStrLenghtRight;
		fontSizeList.push_back((*(it + 1))->m_size);
		notZh = 0;
		charNumber = countChar((*(it + 1))->m_texts, notZh);
		LOGI("charNumber = %d not chiniese = %d",charNumber,notZh);
		tempStrLenghtRight = notZh * 0.50f + (charNumber - notZh);
		if (tempStrLenghtRight > maxStrLengthRight)
		{
			maxStrLengthRight = tempStrLenghtRight;
			maxFntSizeRight = (*(it + 1))->m_size;
		}

	}
	maxFntSizeLeft = maxFntSizeLeft * 40.0 / 15; //乘除操作时为了调节界面显示文本的大小
	maxFntSizeRight = maxFntSizeRight * 40.0 / 15;
	float leftRectWight = maxStrLengthLeft * maxFntSizeLeft + 20;
	float rightRectWight = maxStrLengthRight * maxFntSizeRight
			+ maxFntSizeRight;

	LOGI(
			"maxStrLengthLeft = %d, maxStrLengthRight = %d", maxStrLengthLeft, maxStrLengthRight);
	LOGI(
			"leftRectWight = %f, rightRectWight = %f", leftRectWight, rightRectWight);
	//vector<Vector2> rectPosition;
	MeasureDisplayHelper::GetRelativePosition(rectPosition,
			textsArray.size() / 2, leftRectWight, maxFntSizeLeft);

	MeasureDisplayHelper::createShadowRect(shape2DSet, rectPosition.at(0),
			rectPosition.at(rectPosition.size() - 1), rightRectWight);

	int icolor = 0;
	for (int it = 0; it < textsArray.size() - 2; it = it + 2)
	{

		MeasureDisplayHelper::createRectImage(shape2DSet, rectPosition.at(it),
				rectPosition.at(it + 1), rightRectWight, colorList[icolor], wt,
				wt, blk, textsArray.at(it)->m_texts,
				textsArray.at(it + 1)->m_texts, maxFntSizeLeft, false);

		if (icolor > 3)
		{
			icolor = 0;
		}
		icolor++;
	}

	//绘制最后一个矩形条
	MeasureDisplayHelper::createRectImage(shape2DSet,
			rectPosition.at(rectPosition.size() - 2),
			rectPosition.at(rectPosition.size() - 1), rightRectWight,
			colorList[icolor], wt, wt, blk,
			textsArray.at(textsArray.size() - 2)->m_texts,
			textsArray.at(textsArray.size() - 1)->m_texts, maxFntSizeLeft,
			true);
	LOGI("shape2DSet %p", shape2DSet);
	LOGI("begin addImageToMemory");
	ImageBoard* imageboard = NULL;
	float wideFactor = leftRectWight + rightRectWight;
	float heightFactor = maxFntSizeLeft * 2.5 * textsArray.size() / 2.0;
	MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet,
			position, wideFactor / 100.0, heightFactor / 100.0);
	LOGI("end addImageToMemory");

	//释放内存
	for (int i = 0; i < textsArray.size(); i++)
	{
		delete textsArray[i];
	}

	return imageboard;
}

int MeasureDisplayHelper::countChar(string &str, int &notZH)
{
	size_t length = 0;
	for (size_t i = 0, len = 0; i < str.length(); i += len)
	{
#ifdef _WIN32
		len = 2;
#else
		unsigned char byte = str[i];
		if (byte >= 0xFC) // lenght 6
			len = 6;
		else if (byte >= 0xF8)
			len = 5;
		else if (byte >= 0xF0)
			len = 4;
		else if (byte >= 0xE0)
			len = 3;
		else if (byte >= 0xC0)
			len = 2;
		else
		{
			len = 1;
			notZH++;
		}
#endif
		length++;
	}
	return length;
}

ImageBoard* MeasureDisplayHelper:: createNoteTextsImageN(SceneManager* scene,
		vector<Texts2D*>& textsArray, Vector3 & position)
{
	LOGI("createNoteTextsImageN Begin");
	//SVIEW::Parameters parameter;
	ImageBoard * ret = NULL;
	string noteStr="";
	Shape2DSet * shape2DSet = new Shape2DSet();
	if (textsArray.size() >= 4)
	{
		if (textsArray[3]->m_texts == "")
		{
			noteStr = textsArray[1]->m_texts;
		}
		else if(SVIEW::Parameters::Instance()->m_textNoteStyle == 0)
		{
			noteStr = textsArray[1]->m_texts;
		}
		else if(SVIEW::Parameters::Instance()->m_textNoteStyle == 1)
		{
			noteStr = textsArray[3]->m_texts + "：" + textsArray[1]->m_texts;
		}
	}
	else
	{
		noteStr = textsArray[1]->m_texts;
	}

	float fontSize = textsArray[1]->m_size;
	if (fontSize < 1.0)
		fontSize = 15.0f;
	fontSize = fontSize * 40 / 15.0f;
	LOGI(" createNoteTextsImageN::scene %p", scene);
	GlueObj* gluObj = scene->GetGlueObj();

	int notZh = 0;
	int charNumber = MeasureDisplayHelper::countChar(noteStr, notZh);
#ifdef WIN32
	float strLength = notZh * 0.6f + (charNumber - notZh)*2;
#else
	float strLength = notZh * 0.6f + (charNumber - notZh);
#endif
	float RectWight = (strLength + 1) * fontSize;
#ifdef WIN32
	float ScaleFactor = 1.0f;
	Vector2 tempStart(0.5, 0.5); //2.5 为调节因子
	Vector2 tempEnd(RectWight, fontSize * 3.5 + 1);
	Vector2 tempStartNoUse(RectWight, fontSize * 3.5 + 1);
	gluObj->GetMeasureTextFun()(noteStr.c_str(), fontSize, "楷体",(float*)tempStartNoUse.Data(), (float*)tempEnd.Data());
	tempEnd.m_y = tempEnd.m_y + 1.0f;//留出结束空白字符
	tempEnd.m_x = tempEnd.m_x*0.7;
	tempEnd.m_x = tempEnd.m_x + 10.0f;//留出结束空白字符
	tempStart = tempStart*ScaleFactor;
	tempEnd = tempEnd*ScaleFactor;
	RectWight = tempEnd.m_x;
	fontSize = fontSize* ScaleFactor;
#else	
	Vector2 tempStart(1, 1); //2.5 为调节因子
	Vector2 tempEnd(RectWight, fontSize * 2.5 + 1);
#endif // WIN32	
	Color blk = Color::BLACK;
	Color gr = Color::GRAY;
	Color wt = Color::WHITE;
	Color textColor(0.15, 0.15, 0.15);
	Color lineColor(0.03, 0.38, 0.73);
	float lineWidth = 4.0f;
	CreateNoteRectangleImage(shape2DSet, tempStart, tempEnd, lineColor,
			lineWidth, wt, textColor, fontSize, noteStr);
LOGI("end CreateNoteRectangleImage");

#ifdef WIN32
	float wideFactor = RectWight / 90.0f;
	float heightFactor = (fontSize * 1.7) / 90.0f;
#else
	float wideFactor = RectWight / 100.0f;
	float heightFactor = (fontSize * 2.5) / 100.0f;
#endif
	string createImagePath = "";
	if (gluObj)
	{
		LOGI("gluObj if %p", gluObj);
		//LOGI("shape2DSet if %p", shape2DSet);
		createImagePath = gluObj->createImage(shape2DSet, scene);
		//LOGI("createImagePath %s", createImagePath.c_str());
	}

	delete shape2DSet;
	shape2DSet = NULL;
	LOGI("createImagePath.length() %d",createImagePath.length());
	if (createImagePath.length() > 0)
	{

		Vector2 imageBoardSize(wideFactor, heightFactor);
		//imageBoardSize = ShapeHelper::GetCommonSize(scene, imageBoardSize);

		ret = new ImageBoard(position, imageBoardSize);

		ret->SetTexture(
				scene->GetResourceManager()->GetOrCreateTexture(
						createImagePath));
	}

	//释放内存
	for (int i = 0; i < textsArray.size(); i++)
	{
		delete textsArray[i];
		textsArray[i] = NULL;
	}

	LOGI("createNoteTextsImageN end , return ret %p,release memory",ret);
	return ret;

}

void MeasureDisplayHelper::CreateNoteRectangleImage(Shape2DSet*shape2DSet,
		Vector2 & rectStart, Vector2 & rectEnd , Color & lineColor,float lineWidth,Color & backColor,Color& fontColor, float fontSize,string str)
{
	float rect1StartX = rectStart.m_x;
	float rect1StartY = rectStart.m_y;
	float rect1EndX = rectEnd.m_x;
	float rect1EndY = rectEnd.m_y;
	float deltaLineWidth = lineWidth/2.0f;

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
		(rect1StartY + rect1EndY) / 2.0 - fontSize / 2.0f);
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
	line1d1->m_start = Vector2(rect1StartX+deltaLineWidth, rect1StartY);
	line1d1->m_end = Vector2(rect1StartX+deltaLineWidth, rect1EndY);
	line1d1->SetColor(lineColor);
	shape2DSet->AddShape2D(line1d1);

	//线2
	Line2D * line1d2 = new Line2D();
	line1d2->m_blod = lineWidth;
	line1d2->m_start = Vector2(rect1StartX, rect1StartY+deltaLineWidth);
	line1d2->m_end = Vector2(rect1EndX, rect1StartY+deltaLineWidth);
	line1d2->SetColor(lineColor);
	shape2DSet->AddShape2D(line1d2);
	//线3

	Line2D * line1d3 = new Line2D();
	line1d3->m_blod = lineWidth;
	line1d3->m_start = Vector2(rect1StartX, rect1EndY-deltaLineWidth);
	line1d3->m_end = Vector2(rect1EndX, rect1EndY-deltaLineWidth);
	line1d3->SetColor(lineColor);
	shape2DSet->AddShape2D(line1d3);
	//线4
	Line2D * line1d4 = new Line2D();
	line1d4->m_blod = lineWidth;
	line1d4->m_start = Vector2(rect1EndX-deltaLineWidth, rect1StartY);
	line1d4->m_end = Vector2(rect1EndX-deltaLineWidth, rect1EndY);

	line1d4->SetColor(lineColor);
	shape2DSet->AddShape2D(line1d4);

}

Color MeasureDisplayHelper::m_textColor = Color(0.15, 0.15, 0.15);
Color MeasureDisplayHelper::m_lineColor = Color(0.03, 0.38, 0.73);

ImageBoard*
MeasureDisplayHelper::CreateSequenceNumberImage(SceneManager* scene,
			Texts2D* text, Vector3 & position)
{
	if(!text)
	{
		return NULL;
	}
	ImageBoard * ret = NULL;
	string noteStr= text->m_texts;
	Shape2DSet * shape2DSet = new Shape2DSet();
	float RectWight = 130.0f;
	float fontSize = 70.0f;
	int notZh = 0;
	MeasureDisplayHelper::countChar(noteStr, notZh);
	if(notZh>2)
	{
		float f = notZh-2;
		RectWight = RectWight+fontSize*0.6*f;
	}
	Vector2 rectStart(1, 1);
	Vector2 rectEnd(RectWight, RectWight);

	Color wt = Color::WHITE;
	Color textColor(m_textColor);//默认颜色
	Color lineColor(m_lineColor);//默认线色
	float deltaLineWidth = 6.0f;
	float rect1StartX = rectStart.m_x+deltaLineWidth/2;
	float rect1StartY = rectStart.m_y+deltaLineWidth/2;
	float rect1EndX = rectEnd.m_x-deltaLineWidth/2;
	float rect1EndY = rectEnd.m_y-deltaLineWidth/2;
	Vector2 rect1Start(rect1StartX, rect1StartY);
	Vector2 rect1End(rect1EndX, rect1EndY);

	LOGI("notZh === %d",notZh);
	//创建用于画实心椭圆的矩形
	Ellipse2D * fillEllipse = new Ellipse2D();
	fillEllipse->m_color = wt;
	fillEllipse->m_start = rectStart;
	fillEllipse->m_end = rectEnd;
	fillEllipse->m_isFill = true;
	shape2DSet->AddShape2D(fillEllipse);

	//创建用于画空心椭圆的矩形
	Ellipse2D * strokeEllipse= new Ellipse2D();
	strokeEllipse->m_color = lineColor;
	strokeEllipse->m_start = rect1Start;
	strokeEllipse->m_end = rect1End;
	strokeEllipse->m_strokeWidth = deltaLineWidth;
	strokeEllipse->m_isFill = false;
	shape2DSet->AddShape2D(strokeEllipse);

	//创建矩形文本
	Texts2D* text1 = new Texts2D;
	text1->SetColor(textColor);
#ifdef WIN32
	float start = (rect1EndX - rect1StartX) / 2 - fontSize / 2.0f;
	//矫正一下，
	if (start>20)
	{
		start = 20;
	}

	//只有一个字母
	if (noteStr.length() == 1)
	{
		start = 30;
	}else if (noteStr.length() ==3)
	{
		start = 5;
	}

	Vector2 text1Start(start,
		(rect1StartY + rect1EndY) / 2.0 - fontSize / 2.0f);
#else
	Vector2 text1Start((rect1StartX + rect1EndX) / 2,
		(rect1StartY + rect1EndY) / 2.0 + fontSize / 3.0f);
#endif // WIN32
	text1->m_start = text1Start;
	text1->m_size = fontSize;
	text1->m_texts = noteStr;
	text1->m_alignCenter = true;
	text1->m_fontName = "Consolas";
	shape2DSet->AddShape2D(text1);

	GlueObj* gluObj = scene->GetGlueObj();
	string createImagePath = "";
	float wideFactor = RectWight/140.0f;
	float heightFactor = RectWight/140.0f;
	if (gluObj)
	{
		LOGI("gluObj if %p", gluObj);
		//LOGI("shape2DSet if %p", shape2DSet);
		createImagePath = gluObj->createImage(shape2DSet, scene);
		//LOGI("createImagePath %s", createImagePath.c_str());
	}
	delete shape2DSet;
	shape2DSet = NULL;
	LOGI("createImagePath.length() %d",createImagePath.length());
	if (createImagePath.length() > 0)
	{

		Vector2 imageBoardSize(wideFactor, heightFactor);
		//imageBoardSize = ShapeHelper::GetCommonSize(scene, imageBoardSize);

		ret = new ImageBoard(position, imageBoardSize);

		ret->SetTexture(
				scene->GetResourceManager()->GetOrCreateTexture(
						createImagePath));
	}

	//释放内存
	delete text;
	text = NULL;
	LOGI("createNoteTextsImageN end , return ret %p,release memory",ret);
	return ret;
}


void MeasureDisplayHelper::CreateSelectFaceMark(vector<Vector3>& pointArray, Vector3 & boxCenter,
		Vector3 & direction, Vector3 & faceNormal, float length)
{
	//假设direction表示X轴，faceNormal表示Z轴，则可求出另外一个边方向向量Y
	LOGI("boxCenter %s",boxCenter.Tostring().c_str());
	Vector3 X = direction.Normalized();
	Vector3 Z = faceNormal.Normalized();
	Vector3 Y = (direction.CrossProduct(faceNormal)).Normalized();
	LOGI("X %s",X.Tostring().c_str());
	LOGI("Y %s",Y.Tostring().c_str());
	LOGI("Z %s",Z.Tostring().c_str());
	LOGI("length %f",length);
	Vector3 p1 = boxCenter + length * (X + Y);
	LOGI("p1 %s",X.Tostring().c_str());
	pointArray.push_back(p1);
	Vector3 p2 = boxCenter + length * (X - Y);
	LOGI("p2 %s",X.Tostring().c_str());
	pointArray.push_back(p2);

	Vector3 p3 = boxCenter + length * (Y - X);
	LOGI("p3 %s",X.Tostring().c_str());
	pointArray.push_back(p3);
	Vector3 p4 = boxCenter - length * (X + Y);
	LOGI("p4 %s",X.Tostring().c_str());
	pointArray.push_back(p4);

}

void MeasureDisplayHelper::SetMeasureUnit(int unitFlag,vector<float>&values,string & unit)
{
	switch (unitFlag)
	{
	case 0:
		unit = "";
		break;
	case 1:
	{
		unit = " mm"; //毫米
	}

		break;
	case 2:
	{
		unit = " cm"; //厘米
		for (int i = 0; i < values.size(); i++)
		{
			values.at(i) = values.at(i) / 10.0f;
		}
	}
		break;
	case 3:
	{
		unit = " m"; //米
		for (int i = 0; i < values.size(); i++)
		{
			values.at(i) = values.at(i) / 1000.0f;
		}
	}
		break;
	case 4:
	{
		unit = " in"; //英寸
		for (int i = 0; i < values.size(); i++)
		{
			values.at(i) = values.at(i) * 0.0393701f;
		}
	}
		break;
	case 5:
	{
		unit = " ft"; //英尺
		for (int i = 0; i < values.size(); i++)
		{
			values.at(i) = values.at(i) * 0.0032808f;
		}
	}
		break;
	default:
		LOGE(" Invalid Unit Flag");
		break;

	}
}

void MeasureDisplayHelper::CreateAngleMark(const Vector3 & line1Start,const Vector3 & line1End,
		const Vector3& line2Start,const Vector3 & line2End,
		float theta, vector<Vector3>& lines)
{
	LOGI("MeasureDisplayHelper::CreateAngleMark BEGIN");
	if (theta < 0.000001 && theta > -0.000001)
	{
		LOGI("MeasureDisplayHelper::CreateAngleMark ZERO VECTOR");
		return;
	}
	float line1Length = (line1End - line1Start).Length();
	float line2Length = (line2End - line2Start).Length();
	float radius =
			line1Length > line2Length ? line2Length / 3.0f : line1Length / 3.0f;

	Vector3 center = line2Start;
	float perTheta = theta / 5.0f;
	Vector3 line1Direction = (line1End - line1Start).Normalized();
	Vector3 line2Direction = (line2End - line2Start).Normalized();
	Vector3 axis = line1Direction.CrossProduct(line2Direction);
	axis = axis.Normalized();
	float cosTheta = line1Direction.DotProduct(line2Direction);

	Vector3 startPoint = center + radius * line1Direction; //端点起始点
	lines.push_back(startPoint);
	if (theta < 90.0f || (theta > 90.0f && theta < 180.0f))
	{
		for (int i = 0; i < 4; i++)
		{
			Quaternion q(perTheta * (i + 1), axis);
			q.Normalize();
			Quaternion line1DirectionQua(0, line1Direction.m_x,
					line1Direction.m_y, line1Direction.m_z);
			Quaternion tempVectorQuaternion = q * line1DirectionQua
					* q.Conjugate();
			Vector3 newVector(tempVectorQuaternion.m_x,
					tempVectorQuaternion.m_y, tempVectorQuaternion.m_z);

			newVector.Normalize();
			Vector3 newPoint = center + newVector * radius;

			lines.push_back(newPoint);
		}
	}
//	else if(theta -180.0f >-0.0000001 && theta - 180.0f<0.0000001)
//	{
//
//	}
	else
	{
		Vector3 rightPoint = startPoint + radius * line2Direction;//直角
		lines.push_back(rightPoint);
	}

	if (theta >= 0)
	{
		Vector3 endPoint = center + radius * line2Direction; //端点结束点
		lines.push_back(endPoint);
	}
	else
	{
		Vector3 endPoint = center - radius * line2Direction; //端点结束点
		lines.push_back(endPoint);
	}

	LOGI("MeasureDisplayHelper::CreateAngleMark END");
}

void MeasureDisplayHelper::CreateDesignerAngleMark(const Vector3 & line1Start, const Vector3 & line1End,
	const Vector3& line2Start, const Vector3 & line2End,
	const Vector3& center, float radius, float theta, vector<Vector3>& lines)
{
	if (theta < 0.000001 && theta > -0.000001)
	{
		return;
	}
	float line1Length = (line1End - line1Start).Length();
	float line2Length = (line2End - line2Start).Length();
	int num = (int)abs(theta / 10);
	float perTheta = theta / num;
	Vector3 line1Direction = (line1End - line1Start).Normalized();
	Vector3 line2Direction = (line2End - line2Start).Normalized();
	Vector3 axis = line1Direction.CrossProduct(line2Direction);
	axis = axis.Normalized();
	float cosTheta = line1Direction.DotProduct(line2Direction);

	Vector3 startPoint = center + radius * line1Direction; //端点起始点
	lines.push_back(startPoint);
	for (int i = 0; i < num - 1; i++)
	{
		Quaternion q(perTheta * (i + 1), axis);
		q.Normalize();
		Quaternion line1DirectionQua(0, line1Direction.m_x,
			line1Direction.m_y, line1Direction.m_z);
		Quaternion tempVectorQuaternion = q * line1DirectionQua
			* q.Conjugate();
		Vector3 newVector(tempVectorQuaternion.m_x,
			tempVectorQuaternion.m_y, tempVectorQuaternion.m_z);

		newVector.Normalize();
		Vector3 newPoint = center + newVector * radius;

		lines.push_back(newPoint);
	}

	Vector3 endPoint = center + radius * line2Direction; //端点结束点
	lines.push_back(endPoint);
}

bool MeasureDisplayHelper::GetPropertyFromString(string & propertiesStr,string & property)
{
	bool ret = false;
	vector<string> tempStrSet;

//TODO

	return ret;
}

float MeasureDisplayHelper::GetInnerUnitValue(float srcValue, string& unit)
{
	float ret = srcValue;
	if (unit == "mm")
	{
		ret = srcValue;
	}
	else if (unit == "cm")
	{
		ret = srcValue *10.0f;
	}
	else if (unit == "m")
	{
		ret = srcValue *1000.0f;
	}
	else if (unit == "in")
	{
		ret = srcValue / 0.0393701f;
	}
	else if (unit == "ft")
	{
		ret = srcValue / 0.0032808f;
	}

	return ret;
}

}