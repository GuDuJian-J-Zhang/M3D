#include "m3d/extras/measure/MeasureFactory.h"

#include "m3d/extras/measure/MeasureGroup.h"
#include "m3d/extras/measure/Measure.h"

#include "m3d/SceneManager.h"
#include "m3d/RenderManager.h"

#include "m3d/scene/ShapeNode.h"

#include "m3d/base/Ray.h"
#include "m3d/model/Line3D.h"
#include "m3d/model/Point.h"
#include "m3d/model/PolyLine.h"

#include "m3d/SceneManager.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/base/Plane.h"
#include "m3d/utils/M3DTools.h"
#include "m3d/model/Edge.h"
#include "m3d/model/Curve.h"
#include "m3d/model/Model.h"
#include "m3d/model/Curve.h"
#include "m3d/model/GeoAttribute.h"
#include "m3d/base/Matrix3x4.h"

#include "m3d/utils/ShapeHelper.h"
#include "m3d/Handler/HandlerPoint.h"

#include "m3d/extras/measure/MeasureDistance.h"
#include "m3d/extras/measure/MeasureProperty.h"
#include "m3d/extras/measure/MeasureAngle.h"

#include "m3d/extras/measure/tools/GeometryHelper.h"
#include "m3d/extras/measure/tools/MeasureCalculateHelper.h"
#include "m3d/extras/measure/tools/MeasureDisplayHelper.h"

#include "m3d/graphics/ImageBoard.h"
#include "m3d/ResourceManager.h"

#include "sview/views/Parameters.h"
#include "m3d/base/glue/GlueObj.h"
#include <algorithm>
#include "m3d/model/Face.h"

using namespace SVIEW;

namespace M3D
{
    const string MeasureFactory::Serializer_XML_Element_SVL = "SVL";
    
    const string MeasureFactory::Serializer_XML_Element_Model = "Model";
    const string MeasureFactory::Serializer_XML_Element_Users = "Users";
    const string MeasureFactory::Serializer_XML_Element_User = "User";
    const string MeasureFactory::Serializer_XML_Element_Measures = "Measures";

   
    const string MeasureFactory::Serializer_XML_Element_Color = "Color";
    
    const string MeasureFactory::Serializer_XML_Element_ImageBoard = "ImageBoard";
    const string MeasureFactory::Serializer_XML_Element_Center = "Center";
    
    const string MeasureFactory::Serializer_XML_Element_Line3Ds = "Line3Ds";
    const string MeasureFactory::Serializer_XML_Element_Line3D = "Line3D";
    const string MeasureFactory::Serializer_XML_Element_Points = "Points";
    const string MeasureFactory::Serializer_XML_Element_Point = "Point";
    const string MeasureFactory::Serializer_XML_Element_Position = "Position";
    const string MeasureFactory::Serializer_XML_Element_Direction = "Direction";
    
    const string MeasureFactory::Serializer_XML_Element_MeasureTexts = "MeasureTexts";
    const string MeasureFactory::Serializer_XML_Element_Texts = "Texts";
    const string MeasureFactory::Serializer_XML_Element_Text = "Text";
    const string MeasureFactory::Serializer_XML_Element_Location = "Location";
    
    const string MeasureFactory::Serializer_XML_Element_Attributes = "Attributes";
    const string MeasureFactory::Serializer_XML_Element_Attribute = "Attribute";
    
    const string MeasureFactory::Serializer_XML_Attribute_ID = "ID";
    const string MeasureFactory::Serializer_XML_Attribute_MeasureType = "MeasureType";
    const string MeasureFactory::Serializer_XML_Attribute_UserID = "UserID";
    const string MeasureFactory::Serializer_XML_Attribute_Created = "Created";
    const string MeasureFactory::Serializer_XML_Attribute_R = "R";
    const string MeasureFactory::Serializer_XML_Attribute_G = "G";
    const string MeasureFactory::Serializer_XML_Attribute_B = "B";
    const string MeasureFactory::Serializer_XML_Attribute_Color = "Color";
    const string MeasureFactory::Serializer_XML_Attribute_X = "X";
    const string MeasureFactory::Serializer_XML_Attribute_Y = "Y";
    const string MeasureFactory::Serializer_XML_Attribute_Z = "Z";
    const string MeasureFactory::Serializer_XML_Attribute_Value = "Value";
    const string MeasureFactory::Serializer_XML_Attribute_Length = "Length" ;
    const string MeasureFactory::Serializer_XML_Attribute_Type = "Type";
    const string MeasureFactory::Serializer_XML_Attribute_Key = "Key";
    const string MeasureFactory::Serializer_XML_Attribute_URI = "Uri";
    const string MeasureFactory::Serializer_XML_Attribute_Width = "Width";
    const string MeasureFactory::Serializer_XML_Attribute_Height = "Height";
    const string MeasureFactory::Serializer_XML_Attribute_Name = "Name";
    
    
MeasureFactory::MeasureFactory(void)
{

}

MeasureFactory::~MeasureFactory()
{

}

Color MeasureFactory::m_leaderColor = Color(0.294f,0.294f,0.294f);
Color MeasureFactory::m_selectColor = Color(0.0f,1.0f,0.0f);
Color MeasureFactory::m_exLineColor = Color(0.51f,1.0f,0.51f);
Color MeasureFactory::m_measureColor1 = Color(0.0f,0.443f,0.78f);
Color MeasureFactory::m_measureColor2 = Color(0.969f,0.373f,0.047f);
Color MeasureFactory::m_measureColor3 = Color(0.071f,0.686f,0.565f);

Measure* MeasureFactory::CreateDistanceMeasure(int firstShapeID,
		int secondShapID, int type, Vector2& screenPnt, SceneManager* scene)
{
	Measure* measure = NULL;

	//判断场景是否为空
	if (scene == NULL)
	{
		return measure;
	}

	//填充meaureNote,根据不同的测量类型创建不同的Note对象
	if (type == Measure::MEASURE_TYPE_PNT_PNT_DISTANCE) ///点到点测量
	{
		measure = createPntToPntDistance(firstShapeID, secondShapID, screenPnt,
				scene);
	}
	else if (type == Measure::MEASURE_TYPE_PNT_LINE_DISTANCE)
	{
		measure = createPntTOLineDistance(firstShapeID, secondShapID, screenPnt,
				scene);
	}
	else if (type == Measure::MEASURE_TYPE_PNT_FACE_DISTANCE) ///点到面距离
	{
		measure = createPntToFaceDistance(firstShapeID, secondShapID, screenPnt,
				scene);
	}
	else if (type == Measure::MEASURE_TYPE_LINE_LINE_DISTANCE) ///线到线距离
	{
		measure = createLineToLineDistance(firstShapeID, secondShapID,
				screenPnt, scene);
	}
	else if (type == Measure::MEASURE_TYPE_LINE_FACE_DISTANCE) ///线到面距离
	{
		measure = createLineToFaceDistance(firstShapeID, secondShapID,
				screenPnt, scene);
	}
//	else if (type == Measure::MEASURE_TYPE_FACE_FACE_DISTANCE) ///面到面距离
//	{
//		measure = createFaceToFaceDistance(firstShapeID, secondShapID,
//				screenPnt, scene);
//	}

	AddMeasureToScene(scene,measure);

	if (measure)
	{
		measure->SetFrontShow(true);
	}
    
	return measure;
}
string MeasureFactory::MeasureToXMLElement(SceneManager* scene , Measure *measure)
{
    LOGI("begin serialize");
    //序列化
    string xmlElement = "";
    
    XMLDocument pDoc; //创建文档
    //添加声明
    //XMLDeclaration * pDeclaration = pDoc->NewDeclaration("xml version=\"1.0\" encoding=\"utf-8\" standalone=\"no\"");
    //pDoc->LinkEndChild(pDeclaration);
    
    //创建根节点
    XMLElement *pMeasure = pDoc.NewElement(Serializer_XML_Element_Measures.c_str());
    pDoc.LinkEndChild(pMeasure);
    pMeasure->SetAttribute(Serializer_XML_Attribute_ID.c_str(), measure->GetID());
    pMeasure->SetAttribute(Serializer_XML_Attribute_MeasureType.c_str(), measure->GetMeasureType());
    
    {
        {
            //ImageBoard节点
            XMLElement * pImageBoard = pDoc.NewElement(Serializer_XML_Element_ImageBoard.c_str());
            pMeasure->LinkEndChild(pImageBoard);
            {
                
                if (measure->m_imageBoardList.size() > 0) {
                    ImageBoard *imageBoard = measure->m_imageBoardList[0];
                    Vector3 position = imageBoard->GetPosition();
                    XMLElement * pCenter = pDoc.NewElement(Serializer_XML_Element_Center.c_str());
                    pCenter->SetAttribute(Serializer_XML_Attribute_X.c_str(), position.m_x);
                    pCenter->SetAttribute(Serializer_XML_Attribute_Y.c_str(), position.m_y);
                    pCenter->SetAttribute(Serializer_XML_Attribute_Z.c_str(), position.m_z);
                    pImageBoard->LinkEndChild(pCenter);
                }
            }
        }
        //Points节点
        XMLElement * pPoints = pDoc.NewElement(Serializer_XML_Element_Points.c_str());
        pMeasure->LinkEndChild(pPoints);
        {
            for (int i = 0; i < measure->m_PointList.size(); i++)
            {
                Point* point = measure->m_PointList[i];
                //创建Point节点
                XMLElement * pPoint = pDoc.NewElement(Serializer_XML_Element_Point.c_str());
                pPoints->LinkEndChild(pPoint);
                
                {
                    pPoint->SetAttribute(Serializer_XML_Attribute_X.c_str(), point->GetCoordinate().m_x);
                    pPoint->SetAttribute(Serializer_XML_Attribute_Y.c_str(), point->GetCoordinate().m_y);
                    pPoint->SetAttribute(Serializer_XML_Attribute_Z.c_str(), point->GetCoordinate().m_z);
                    Color color = point->GetColor();
                    string temstr = M3DTOOLS::floatToString(color.m_r) + " "
                    + M3DTOOLS::floatToString(color.m_g) + " "
                    + M3DTOOLS::floatToString(color.m_b);
                    
                    pPoint->SetAttribute(Serializer_XML_Attribute_Color.c_str(), temstr.c_str());
                    
                }
            }
            
        }
        
        Vector3 anchor;
        
        {
            //Line3Ds节点
            XMLElement * pLine3Ds = pDoc.NewElement(Serializer_XML_Element_Line3Ds.c_str());
            pMeasure->LinkEndChild(pLine3Ds);
            {
                for (int i = 0; i < measure->m_LineList.size(); i++)
                {
                    Line3D* line3D =measure->m_LineList[i];
                    //创建Point节点
                    XMLElement * pLine3D = pDoc.NewElement(Serializer_XML_Element_Line3D.c_str());
                    
                    
                    pLine3Ds->LinkEndChild(pLine3D);
                    
                    {
                        
                        //创建Color节点
                        XMLElement * pColor = pDoc.NewElement(Serializer_XML_Element_Color.c_str());
                        pLine3D->LinkEndChild(pColor);
                        {
                            Color color = line3D->GetColor();
                            pColor->SetAttribute(Serializer_XML_Attribute_R.c_str(), color.m_r);
                            pColor->SetAttribute(Serializer_XML_Attribute_G.c_str(), color.m_g);
                            pColor->SetAttribute(Serializer_XML_Attribute_B.c_str(), color.m_b);
                        }
                        
                        //创建Position节点
                        XMLElement * pPosition = pDoc.NewElement(Serializer_XML_Element_Position.c_str());
                        pLine3D->LinkEndChild(pPosition);
                        {
                            pPosition->SetAttribute(Serializer_XML_Attribute_X.c_str(), line3D->GetPosition().m_x);
                            pPosition->SetAttribute(Serializer_XML_Attribute_Y.c_str(), line3D->GetPosition().m_y);
                            pPosition->SetAttribute(Serializer_XML_Attribute_Z.c_str(), line3D->GetPosition().m_z);
                        }
                        
                        //创建Direction节点
                        XMLElement * pDirection = pDoc.NewElement(Serializer_XML_Element_Direction.c_str());
                        pLine3D->LinkEndChild(pDirection);
                        
                        {
                            pDirection->SetAttribute(Serializer_XML_Attribute_X.c_str(), line3D->GetEndPoint().m_x);
                            pDirection->SetAttribute(Serializer_XML_Attribute_Y.c_str(), line3D->GetEndPoint().m_y);
                            pDirection->SetAttribute(Serializer_XML_Attribute_Z.c_str(), line3D->GetEndPoint().m_z);
                            
                        }
                        if (line3D->GetName() == "MeasureImageLeader") {
                            pLine3D->SetAttribute(Serializer_XML_Attribute_Type.c_str(), 1);
                            anchor = line3D->m_EndPoint;
                        }else{
                            pLine3D->SetAttribute(Serializer_XML_Attribute_Type.c_str(), 0);
                        }
                        
                    }
                }
                
                //			}
            }
        }
        
        
        
        {
            //CompositeTexts
            XMLElement * pMeasureTexts = pDoc.NewElement(Serializer_XML_Element_MeasureTexts.c_str());
            pMeasure->LinkEndChild(pMeasureTexts);
            if (measure->m_ComTexts.size() > 0)
            {
                for (int i = 0; i < measure->m_ComTexts.size(); i++) {
                    ComText *text = measure->m_ComTexts[i];
                    {
                        //Texts
                        XMLElement * pTexts = pDoc.NewElement(Serializer_XML_Element_Texts.c_str());
                        pMeasureTexts->LinkEndChild(pTexts);
                        {
                            //Text
                            XMLElement * pText = pDoc.NewElement(Serializer_XML_Element_Text.c_str());
                            pTexts->LinkEndChild(pText);
                            pText->SetAttribute(Serializer_XML_Attribute_Value.c_str(),text->GetCText(0)->GetText().c_str());
                            pText->SetAttribute(Serializer_XML_Attribute_Type.c_str(),text->GetCText(0)->GetLanguageType().c_str());
                            //Location
                            {
                                XMLElement * pLocation = pDoc.NewElement(Serializer_XML_Element_Location.c_str());
                                pText->LinkEndChild(pLocation);
                                {
                                    pLocation->SetAttribute(Serializer_XML_Attribute_X.c_str(), anchor.m_x);
                                    pLocation->SetAttribute(Serializer_XML_Attribute_Y.c_str(), anchor.m_y);
                                    pLocation->SetAttribute(Serializer_XML_Attribute_Z.c_str(), anchor.m_z);
                                }
                                
                                
                                //Color
                                XMLElement * pColor = pDoc.NewElement(Serializer_XML_Element_Color.c_str());
                                pText->LinkEndChild(pColor);
                                {
                                    Color color = text->GetColor();
                                    pColor->SetAttribute(Serializer_XML_Attribute_R.c_str(),
                                                         color.m_r);
                                    pColor->SetAttribute(Serializer_XML_Attribute_G.c_str(),
                                                         color.m_g);
                                    pColor->SetAttribute(Serializer_XML_Attribute_B.c_str(),
                                                         color.m_b);
                                }
                                
                            }
                        }
                        
                    }
                }
                
            }
        }
        
        {
            XMLElement * pAttributes = pDoc.NewElement(Serializer_XML_Element_Attributes.c_str());
            pMeasure->LinkEndChild(pAttributes);
            for (map<string, string>::iterator it = measure->GetPropertyMap().begin();
                 it != measure->GetPropertyMap().end(); it++)
            {
                XMLElement * pAttribute = pDoc.NewElement(Serializer_XML_Element_Attribute.c_str());
                pAttributes->LinkEndChild(pAttribute);
                pAttribute->SetAttribute(Serializer_XML_Attribute_Key.c_str(), it->first.c_str());
                pAttribute->SetAttribute(Serializer_XML_Attribute_Value.c_str(), it->second.c_str());
            }
            
        }
        
    }
    
    tinyxml2::XMLPrinter priter;
    pDoc.Accept(&priter);
    xmlElement = priter.CStr();
    
    LOGI("%s", priter.CStr());
    LOGE("end serialize");
    
    return xmlElement;
}
/**
 * @brief 根据XML信息创建点点测量
 * @param xmlElement
 * @param scene 场景指针
 * @return 返回测量对象，若未创建成功则返回NULL
 */
Measure* MeasureFactory::CreateMeasureFromXMLElement(SceneManager* scene, const string& xmlElement)
{
    LOGI("NoteFactory::CreateTextNoteFromXMLElement BEGIN");
    Measure* measure = NULL;
    float fSize;
    InternationalManager  language;
    language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);
    Vector3 startPnt, endPnt, location;
    string contentStr;
    if (xmlElement.length() == 0)
    {
        return measure;
    }
    measure = new MeasureDistance;
    const char* xml = xmlElement.c_str();
    XMLDocument doc;
    doc.Parse(xml);
        
    XMLElement* xmlMeasure = doc.FirstChildElement(Serializer_XML_Element_Measures.c_str());
        
    if (xmlMeasure == NULL)
    {
        return NULL;
    }
    int id = xmlMeasure->IntAttribute(Serializer_XML_Attribute_ID.c_str());
    //textNote->SetID(id);
    
    int measureType = xmlMeasure->IntAttribute(Serializer_XML_Attribute_MeasureType.c_str());
    measure->SetMeasureType(measureType);
    
    Vector3 pntInPlane;
    
    XMLElement* xmlImageBoard = xmlMeasure->FirstChildElement(Serializer_XML_Element_ImageBoard.c_str());
    if (xmlImageBoard) {
        XMLElement* xmlCenter = xmlImageBoard->FirstChildElement(Serializer_XML_Element_Center.c_str());
        pntInPlane.m_x = xmlCenter->FloatAttribute(Serializer_XML_Attribute_X.c_str());
        pntInPlane.m_y = xmlCenter->FloatAttribute(Serializer_XML_Attribute_Y.c_str());
        pntInPlane.m_z = xmlCenter->FloatAttribute(Serializer_XML_Attribute_Z.c_str());
    }
    
    XMLElement* xmlPoints = xmlMeasure->FirstChildElement(Serializer_XML_Element_Points.c_str());
    //LOGI("xmlLeaders");
    if (xmlPoints)
    {
        XMLElement* xmlPoint = xmlPoints->FirstChildElement(Serializer_XML_Element_Point.c_str());
        Vector3 temVec[10];
        int i = 0;
        while (xmlPoint)
        {
            temVec[i].m_x = xmlPoint->FloatAttribute(Serializer_XML_Attribute_X.c_str());
            temVec[i].m_y = xmlPoint->FloatAttribute(Serializer_XML_Attribute_Y.c_str());
            temVec[i].m_z = xmlPoint->FloatAttribute(Serializer_XML_Attribute_Z.c_str());
                
            xmlPoint = xmlPoint->NextSiblingElement(Serializer_XML_Element_Point.c_str());
            i++;
        }
        startPnt = temVec[0];
        Point *point1 = new Point(startPnt);
        point1->SetDrawType(1);
        point1->SetSize(0.8);
        measure->AddPoint(point1);
        if (i == 1) {
            endPnt = startPnt;
        } else {
            endPnt = temVec[1];
            Point *point2 = new Point(endPnt);
            point2->SetDrawType(1);
            point2->SetSize(0.8);
            measure->AddPoint(point2);
        }
    }
        
//    float lineLength = (startPnt - endPnt).Length();
    XMLElement* xmlLine3Ds = xmlMeasure->FirstChildElement(Serializer_XML_Element_Line3Ds.c_str());
    //LOGI("xmlLeaders");
    if (xmlLine3Ds)
    {
        XMLElement* xmlLine3D = xmlLine3Ds->FirstChildElement(Serializer_XML_Element_Line3D.c_str());
        Line3D* line;
        while (xmlLine3D)
        {
                
            Color temColor;
            Vector3 position,direction;
            int type = xmlLine3D->FloatAttribute(Serializer_XML_Attribute_Type.c_str());
                
            XMLElement* xmlColor = xmlLine3D->FirstChildElement(Serializer_XML_Element_Color.c_str());
            {
                temColor = Color();
                temColor.m_r = xmlColor->FloatAttribute(Serializer_XML_Attribute_R.c_str());
                temColor.m_g = xmlColor->FloatAttribute(Serializer_XML_Attribute_G.c_str());
                temColor.m_b = xmlColor->FloatAttribute(Serializer_XML_Attribute_B.c_str());
                    
            }
            XMLElement* xmlPosition = xmlLine3D->FirstChildElement(Serializer_XML_Element_Position.c_str());
            {
                Vector3 temVec = Vector3();
                temVec.m_x = xmlPosition->FloatAttribute(Serializer_XML_Attribute_X.c_str());
                temVec.m_y = xmlPosition->FloatAttribute(Serializer_XML_Attribute_Y.c_str());
                temVec.m_z = xmlPosition->FloatAttribute(Serializer_XML_Attribute_Z.c_str());
                position = temVec;
            }
            XMLElement* xmlDirection = xmlLine3D->FirstChildElement(Serializer_XML_Element_Direction.c_str());
            {
                Vector3 temVec = Vector3();
                temVec.m_x = xmlDirection->FloatAttribute(Serializer_XML_Attribute_X.c_str());
                temVec.m_y = xmlDirection->FloatAttribute(Serializer_XML_Attribute_Y.c_str());
                temVec.m_z = xmlDirection->FloatAttribute(Serializer_XML_Attribute_Z.c_str());
                direction = temVec;
            }
            line = new Line3D(position,direction);
            line->SetColor(temColor);
            if (type == 1) {
                line->SetName("MeasureImageLeader");
            }
            measure->AddLine(line);
            xmlLine3D = xmlLine3D->NextSiblingElement(Serializer_XML_Element_Line3D.c_str());
            
            }
        }
    
    XMLElement* xmlMeasureTexts= xmlMeasure->FirstChildElement(Serializer_XML_Element_MeasureTexts.c_str());
        
    if (xmlMeasureTexts)
    {
        XMLElement* texts = xmlMeasureTexts->FirstChildElement(Serializer_XML_Element_Texts.c_str());
        while (texts)
        {
                //LOGI("Begin Texts");
            XMLElement* xmlText = texts->FirstChildElement(Serializer_XML_Element_Text.c_str());
                
            string value = xmlText->Attribute(Serializer_XML_Attribute_Value.c_str());
            contentStr = value;
            string languageType = xmlText->Attribute(Serializer_XML_Attribute_Type.c_str());
                //LOGI("textNote->m_ComTexts.size() %d",textNote->m_ComTexts.size());
//                if (measure->m_ComTexts.size() > 0)
//                {
//                    CText * tempText = new CText;
//                    tempText->SetText(value);
//                    measure->m_ComTexts[0]->AddCText(tempText);
//                }
//                else
//                {
                    //	LOGI("textNote->m_ComTexts pushback");
                ComText * tempComeText = new ComText;
                CText * tempText = new CText;
                tempText->SetText(value);
                tempText->SetLanguageType(languageType);
                tempComeText->AddCText(tempText);
                measure->m_ComTexts.push_back(tempComeText);
                    //	LOGI("textNote->m_ComTexts.size() %d",textNote->m_ComTexts.size());
//                }
                
            if (xmlText) {
                Color temColor;
                XMLElement* xmlColor = xmlText->FirstChildElement(Serializer_XML_Element_Color.c_str());
                {
                    temColor = Color();
                    temColor.m_r = xmlColor->FloatAttribute(Serializer_XML_Attribute_R.c_str());
                    temColor.m_g = xmlColor->FloatAttribute(Serializer_XML_Attribute_G.c_str());
                    temColor.m_b = xmlColor->FloatAttribute(Serializer_XML_Attribute_B.c_str());
                        
                }
                XMLElement* xmlLocation = xmlText->FirstChildElement(Serializer_XML_Element_Location.c_str());
                {
                    location = Vector3();
                    location.m_x = xmlLocation->FloatAttribute(Serializer_XML_Attribute_X.c_str());
                    location.m_y = xmlLocation->FloatAttribute(Serializer_XML_Attribute_Y.c_str());
                    location.m_z = xmlLocation->FloatAttribute(Serializer_XML_Attribute_Z.c_str());
                    
                }
            }
            texts = texts->NextSiblingElement(Serializer_XML_Element_Texts.c_str());
        }
    }
    //LOGI("End compositeTexts");
        
    //LOGI("Begin xmlAttributes");
    //解析Attributes元素及子元素
    map<string, string> attributeList;
    XMLElement* xmlAttributes = xmlMeasure->FirstChildElement(Serializer_XML_Element_Attributes.c_str());
    if (xmlAttributes)
    {
        XMLElement* xmlAttribute = xmlAttributes->FirstChildElement(Serializer_XML_Element_Attribute.c_str());
        while (xmlAttribute)
        {
            string key = xmlAttribute->Attribute(Serializer_XML_Attribute_Key.c_str());
            string value = xmlAttribute->Attribute(Serializer_XML_Attribute_Value.c_str());
            measure->SetProperty(key, value);
            //attributeList.insert (make_pair(key,value));
            xmlAttribute = xmlAttribute->NextSiblingElement(Serializer_XML_Element_Attribute.c_str());
        }
    }
    
    {
        //构造显示文本
        vector<float> textLength;
        vector<float> values;
        vector<string> languages;
        for (int i = 0; i < measure->m_ComTexts.size(); i++) {
            CText *text = measure->m_ComTexts[i]->GetCText(0);
            string distanceString = text->GetText().c_str();
            float distance = atof(distanceString.c_str());
            string unit = M3DTOOLS::IntToString(SVIEW::Parameters::Instance()->m_measureUnitStyle);
            distance = MeasureDisplayHelper::GetInnerUnitValue(distance, unit);
            values.push_back(distance);
            string languageString = text->GetLanguageType().c_str();
            languages.push_back(languageString);
            string typestr = language.GetCurrentTypeValue(languageString);
            float projectDistTextLength =  language.GetCurrentTypeValueLength(languageString);
            textLength.push_back(projectDistTextLength);
        }
        float maxTextLength = *std::max_element(textLength.begin(),textLength.end());
        string unit;
        if (measureType == MeasureAngle::MEASURE_TYPE_LINE_LINE_ANGLE || measureType == MeasureAngle::MEASURE_TYPE_FACE_FACE_ANGLE ||
            measureType == MeasureAngle::MEASURE_TYPE_LINE_FACE_ANGLE) {
            
        }else{
            MeasureDisplayHelper::SetMeasureUnit(
                                                 SVIEW::Parameters::Instance()->m_measureUnitStyle, values,
                                                 unit);
        }
       
        
        
        //长度
        Shape2DSet * shape2DSet = new Shape2DSet();
        float fntSize = 40;
        float rect2L = 450.0;
        Color gr = Color::GRAY;
        Color wt = Color::WHITE;
        Color blk = Color::BLACK;
        Color grn = Color::GREEN;
        Color rd = Color::RED;
        Color blu = Color::BLUE;
            
        for (int i = 0; i < values.size(); i++) {
            string lengthstr = M3DTOOLS::floatToString(values.at(i)) + unit;
            string typestr = language.GetCurrentTypeValue(languages.at(i));
            Vector2 rectStart;
            Vector2 rectEnd;
            Color tempColor;
            if (i == 0) {
                rectStart.m_x = rectStart.m_y = 1.0;
                
                rectEnd.m_x = maxTextLength;
                rectEnd.m_y = 100.0;
                tempColor = m_measureColor1;
            }else if (i == 1){
                rectStart.m_x = 1.0;
                rectStart.m_y = 100.0;
                
                rectEnd.m_x = maxTextLength;
                rectEnd.m_y = 200.0;
                tempColor = m_measureColor2;
            }else if (i == 2){
                rectStart.m_x = 1.0;
                rectStart.m_y = 200.0;
                
                rectEnd.m_x = maxTextLength;
                rectEnd.m_y = 300.0;
                tempColor = m_measureColor3;
            }
            bool ret = false;
            if (i + 1 == values.size()) {
                ret = true;
            }
            MeasureDisplayHelper::createRectImage(shape2DSet, rectStart,
                                                  rectEnd, rect2L, tempColor, wt, wt, blk, typestr, lengthstr,
                                                  fntSize, ret);
        }
            
        ImageBoard* imageboard = NULL;
        MeasureDisplayHelper::addImageToMemory(scene, imageboard,
                                                shape2DSet, pntInPlane,(maxTextLength+rect2L)/100.0f, values.size());
        measure->AddImage(imageboard);
    }
    measure->SetFrontShow(true); //设置是否前端显示
    if (measure)
    {
        AddMeasureToScene(scene, measure);
    }
    LOGI("NoteFactory::CreateTextNoteFromXMLElement END");
    return measure;
}
Measure* MeasureFactory::CreateAngleMeasure(int firstShapeID, int secondShapID,
		int type, Vector2& screenPnt, SceneManager* scene)
{
	LOGI("C++ angle measure main function");
	Measure* measure = NULL;

	//判断场景是否为空
	if (scene == NULL)
	{
		return measure;
	}

	//填充meaureNote,根据不同的测量类型创建不同的Note对象
	if (type == Measure::MEASURE_TYPE_LINE_LINE_ANGLE) ///两条非平行线间的夹角
	{
		LOGI("C++ angle measure line line function");
		measure = createLineToLineAngle(firstShapeID, secondShapID, screenPnt,
				scene);
	}
	else if (type == Measure::MEASURE_TYPE_FACE_FACE_ANGLE) ///两个面间的夹角
	{
		LOGI("C++ angle measure face face function");
		measure = createFaceToFaceAngle(firstShapeID, secondShapID, screenPnt,
				scene);
	}
	else if (type == Measure::MEASURE_TYPE_LINE_FACE_ANGLE) /// 直线与平面之间的夹角
	{
		LOGI("C++ angle measure line face function");
		measure = createLineToFaceAngle(firstShapeID, secondShapID, screenPnt,
				scene);
	}

	AddMeasureToScene(scene,measure);
	if (measure)
	{
		measure->SetFrontShow(true);
	}
	return measure;
}

Measure* MeasureFactory::CreatePropertyMeasure(int shapeID, int type,
		Vector2& screenPnt, SceneManager* scene)
{
	LOGI("C++ CreatePropertyMeasure Type: %d", type);
	Measure* measure = NULL;

	//判断场景是否为空
	if (scene == NULL)
	{
		return measure;
	}

	//填充meaureNote,根据不同的测量类型创建不同的Note对象
	if (type == Measure::MEASURE_TYPE_PNT_COORD) ///点的坐标值
	{
		LOGI("C++ CreatePropertyMeasure");
		measure = createPntProperty(shapeID, screenPnt, scene);
	}
	else if (type == Measure::MEASURE_TYPE_LINE_LENGTH) ///边线的长度
	{
		measure = createLineProperty(shapeID, screenPnt, scene);
	}
	else if (type == Measure::MEASURE_TYPE_CRICLE_PROPERTY) ///圆或圆弧的弧长、直径（半径）、圆心
	{
		measure = createCircleProperty(shapeID, screenPnt, scene);
	}
	else if (type == Measure::MEASURE_TYPE_FACE_PROPERTY) ////面的属性
	{
		LOGI("C++ CreateFacePropertyMeasure");
		measure = createFaceProperty(shapeID, screenPnt, scene);
	}
	else if (type == Measure::MEASURE_TYPE_APERTURE_PROPERTY) ////孔的圆心、直径（半径）、深度
	{
		measure = createApertureProperty(shapeID, screenPnt, scene);
	}
	else if(type == Measure::MEASURE_TYPE_MODEL_PROPERTY )
	{
		measure = createModelProperty(shapeID, screenPnt, scene);
	}

	AddMeasureToScene(scene,measure);

	return measure;
}


Measure* MeasureFactory::CreateFaceDistanceMeasure(int firstShapeID, int secondShapID,
		int thirdShapeID, int forthShapID,
		int type, Vector2& screenPnt, SceneManager* scene)
{


	Measure* measure = NULL;

	//判断场景是否为空
	if (scene == NULL)
	{
		return measure;
	}
//	if (type == Measure::MEASURE_TYPE_PNT_FACE_DISTANCE) ///点到面距离
//	{
//		measure = createPntToFaceDistance(firstShapeID, secondShapID,
//				 thirdShapeID,  forthShapID,screenPnt,
//				scene);
//	}
//	else if (type == Measure::MEASURE_TYPE_LINE_FACE_DISTANCE) ///线到面距离
//	{
//		measure = createLineToFaceDistance(firstShapeID, secondShapID,
//				thirdShapeID,  forthShapID,screenPnt, scene);
//	}
//	else if (type == Measure::MEASURE_TYPE_FACE_FACE_DISTANCE) ///面到面距离
//	{
//		measure = createFaceToFaceDistance(firstShapeID, secondShapID,
//				thirdShapeID,  forthShapID,screenPnt, scene);
//	}

	if (type == Measure::MEASURE_TYPE_FACE_FACE_DISTANCE) ///面到面距离
		{
			measure = createFaceToFaceDistance(firstShapeID, secondShapID,
					thirdShapeID,  forthShapID,screenPnt, scene);
		}
	AddMeasureToScene(scene, measure);

	if (measure)
	{
		measure->SetFrontShow(true);
	}
	return measure;

}

Measure* MeasureFactory::CreateFaceAngleMeasure(int firstShapeID, int secondShapID,
		int thirdShapeID, int forthShapID,
		int type, Vector2& screenPnt, SceneManager* scene)
{
	Measure* measure = NULL;

	//判断场景是否为空
	if (scene == NULL)
	{
		return measure;
	}

	if (type == Measure::MEASURE_TYPE_FACE_FACE_ANGLE) ///两个面间的夹角
	{
		LOGI("C++ angle measure face face function");
		measure = createFaceToFaceAngle(firstShapeID, secondShapID,thirdShapeID,  forthShapID, screenPnt,
				scene);
	}
	else if (type == Measure::MEASURE_TYPE_LINE_FACE_ANGLE) /// 直线与平面之间的夹角
	{
		LOGI("C++ angle measure line face function");
		measure = createLineToFaceAngle(firstShapeID, secondShapID, thirdShapeID,  forthShapID,screenPnt,
				scene);
	}

	AddMeasureToScene(scene, measure);
	if (measure)
	{
		measure->SetFrontShow(true);
	}
	return measure;

}

void MeasureFactory::GetMeasureProperty(int shapeID, int type,
			 SceneManager* scene,string & outString)
{
	//填充meaureNote,根据不同的测量类型创建不同的Note对象
	if (type == Measure::MEASURE_TYPE_PNT_COORD) ///点的坐标值
	{
		LOGI("C++ CreatePropertyMeasure");
		GetPntProperty(shapeID,  scene,outString);
	}
//	else if (type == Measure::MEASURE_TYPE_CRICLE_PROPERTY) ///圆或圆弧的弧长、直径（半径）、圆心
//	{
//		GetCircleProperty(shapeID,  scene,outString);
//	}
	else if (type == Measure::MEASURE_TYPE_LINE_LENGTH ||
			type == Measure::MEASURE_TYPE_CRICLE_PROPERTY) ///边线的长度
	{
		GetLineProperty(shapeID,  scene,outString);
	}

	else if (type == Measure::MEASURE_TYPE_FACE_PROPERTY) ////面的属性
	{
		LOGI("C++ CreateFacePropertyMeasure");
		GetFaceProperty(shapeID,  scene,outString);
	}
	else if(type == Measure::MEASURE_TYPE_MODEL_PROPERTY )
	{
		GetModelProperty(shapeID,  scene,outString);
	}

}

bool MeasureFactory::AddMeasureToScene(SceneManager* scene,Measure* measure)
{
	bool ret = false;
	if(scene && measure)
	{
		//得到测量组节点，创建的测量节点加入到此组中
		MeasureGroup* measureGroup = scene->GetMeasureGroup();

		//创建一个节点，用来挂载创建的测量measureNote对象
		ShapeNode* node = new ShapeNode();
		node->SetShape(measure);
		measure->SetScene(scene);

		//给新创建的Node设置一个名字
		char strID[10];
		sprintf(strID, "%d", node->GetID());
		node->SetName(measureGroup->GetName() + "|" + strID);

		//修改场景结构，加锁
		scene->Lock();

		//将创建的测量对象加入到测量组中
		measureGroup->AddChild(node);

		scene->AddShapeIDToMap(measure);
        
        scene->GetRenderManager()->RequestRedraw();

		//完成修改，解锁
		scene->UnLock();

		ret = true;
	}
	return ret;
}

Measure* MeasureFactory::createPntToPntDistance(int firstPntId, int secondPntId,
		Vector2& screenPnt, SceneManager* scene)
{
	Measure* measure = NULL;
	InternationalManager  language;
	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);

	Shape* firstShape = scene->GetShape(firstPntId);
	Shape* secondShape = scene->GetShape(secondPntId);

	//如果的shape对象存在
	if (firstShape && secondShape && firstShape->GetType() == SHAPE_POINT_HANDLE
			&& secondShape->GetType() == SHAPE_POINT_HANDLE)
	{
		measure = new MeasureDistance;

		measure->SetMeasureType(Measure::MEASURE_TYPE_PNT_PNT_DISTANCE);

		HandlerPoint* firstPnt = (HandlerPoint*) firstShape;
		HandlerPoint* secondPnt = (HandlerPoint*) secondShape;

		Vector3 fCoordiante = firstPnt->GetPosition();
		Vector3 sCoordiante = secondPnt->GetPosition();

		float lineLength = (fCoordiante - sCoordiante).Length();

		//camera射线
		Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
				screenPnt.m_x, screenPnt.m_y);

		Vector3 lineCenter = (fCoordiante + sCoordiante) / 2;

		//构造投影平面
		Plane projPlane(cameraRay.GetDirection(), lineCenter);

		Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

		//构造线
		Line3D* line1 = new Line3D(fCoordiante, sCoordiante);
		Color line1Color = Color::BLACK;
		line1->SetColor(m_measureColor1);
		Line3D* line2 = new Line3D(lineCenter, pntInPlane);//文本引线
		//Color oriangle(1.0f,0.44f,0.0f) ;
		line2->SetColor(m_leaderColor);
		line2->SetName("MeasureImageLeader");

		//给线设置箭头末端符号
//		line1->SetStartArrow(1);
//		line1->SetEndArrow(1);

		//构造显示文本

		//测量结果单位转换
		vector<float> valuse;
		valuse.push_back(lineLength);
		string unit;
		MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
				,valuse,unit);
		LOGE("SVIEW::Parameters::Instance()->m_measureUnitStyle %d",SVIEW::Parameters::Instance()->m_measureUnitStyle);
		string lengthstr = M3DTOOLS::floatToString(valuse.at(0)) + unit;
		string typestr = language.GetCurrentTypeValue("PointPointDistance");
		float rectLength = language.GetCurrentTypeValueLength("PointPointDistance");
		//根据整个场景包围盒的大小来确定文字的大小
		float fontSize = scene->GetSceneBox().Length() / 20;
        
        //解决协同中文本读取
        ComText* ct = new ComText();
        CText* t = new CText;
        t->SetLanguageType("PointPointDistance");
        t->SetText(lengthstr);
        ct->AddCText(t);
        measure->m_ComTexts.push_back(ct);
        
		Point* pnt1 = new Point(fCoordiante);
		pnt1->SetDrawType(1);
		pnt1->SetSize(0.8f);
		Point* pnt2 = new Point(sCoordiante);
		pnt2->SetDrawType(1);
		pnt2->SetSize(0.8f);
		//将线加入测量对象中
		measure->AddLine(line1);
		measure->AddLine(line2);

		measure->AddPoint(pnt1);
		measure->AddPoint(pnt2);

		Shape2DSet * shape2DSet = new Shape2DSet();
		float fntSize = 40;
		Vector2 rect1Start(1.0, 1.0);
		Vector2 rect1End(rectLength, 100.0);
		float rect2L = 450.0;
		Color gr = Color::GRAY;
		Color wt = Color::WHITE;
		Color blk = Color::BLACK;
		Color grn = Color::GREEN;
		Color rd = Color::RED;
		Color blu = Color::BLUE;
//		MeasureDisplayHelper::createShadowRect(shape2DSet, rect1Start, rect1End,
//				rect2L);
		MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start, rect1End,
				rect2L, m_measureColor1, wt, wt, blk, typestr, lengthstr, fntSize, true);

		ImageBoard* imageboard=NULL;
		MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
						(rect2L + rectLength)/100.0f, 1);
		measure->AddImage(imageboard);
	}
	else
	{
		LOGE("MeasureFactory::createPntToPntDistance shape is NULL");
	}

	return measure;
}
    
Measure* MeasureFactory::createPntTOLineDistance(int firstPntId,
		int secondLineId, Vector2& screenPnt, SceneManager* scene)
{
	Measure* measure = NULL;
	InternationalManager language;
	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);

	Shape* firstShape = scene->GetShape(firstPntId);
	Shape* secondShape = scene->GetShape(secondLineId);

	LOGE("createPntTOLineDistance step1");
	//如果的shape对象存在
	if (firstShape && secondShape && firstShape->GetType() == SHAPE_POINT_HANDLE
			&& secondShape->GetType() == SHAPE_EDGE)
	{
		LOGE("createPntTOLineDistance step2");
		measure = new MeasureDistance;

		measure->SetMeasureType(Measure::MEASURE_TYPE_PNT_LINE_DISTANCE);

		HandlerPoint* firstPnt = (HandlerPoint*) firstShape;
		Edge* secondEdge = (Edge*) secondShape;

		//得到边界线数据
		RefPolyLine * m_lineSet = secondEdge->GetLineData();

		GeometryAttribute* geo = m_lineSet->GetGeoAttribute();
		///暂时仅支持，点到直线距离的测量
		if (geo != NULL && geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE)
		{
			LineAttribute lineAttribute = *((LineAttribute*) geo);

			Matrix3x4 worldMatrix = ShapeHelper::GetShapeWorldMatrix(
					secondEdge);

			GeometryHelper::Transform(lineAttribute, worldMatrix);
			Vector3 firstCoord = firstPnt->GetPosition();

			float distance, minDistance, maxDistance;
			int inLine;
			Vector3 projectPnt;

			MeasureCalculateHelper::PntLineDistance(firstCoord, lineAttribute,
					distance, inLine, projectPnt);

			Vector3 pntStartPoint = lineAttribute.GetStartPoint(); //!< Line起点
			Vector3 pntEndPoint = lineAttribute.GetEndPoint(); //!< Line终点

			float startD = (firstCoord - pntStartPoint).Length();
			float endD = (firstCoord - pntEndPoint).Length();
			Vector3 maxPoint, minPoint;
//			if(MeasureCalculateHelper::pntInSegment(projectPnt,lineAttribute))
//			{
//				minDistance = distance;
//				maxDistance = startD>endD?startD:endD;
//				LOGI("Inner Segment");
//			}
//			else
//			{
//				if(startD>endD)
//				{
//					maxDistance = startD;
//					minDistance = endD;
//				}
//				else
//				{
//					maxDistance = endD;
//					minDistance = startD;
//				}
//			}

			//此处，不在使用投影距离为最小距离。
			if (startD > endD)
			{
				maxDistance = startD;
				minDistance = endD;
				maxPoint = pntStartPoint;
				minPoint = pntEndPoint;
			}
			else
			{
				maxDistance = endD;
				minDistance = startD;
				minPoint = pntStartPoint;
				maxPoint = pntEndPoint;
			}

			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(),
					(pntStartPoint + pntEndPoint) / 2);

			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//构造线
			Line3D* line1 = new Line3D(pntStartPoint, pntEndPoint); //测量的线
			Color line1Color = Color::GREEN;
			line1->SetColor(m_selectColor);

			Vector3 line1Center = (pntStartPoint + pntEndPoint) / 2; //测量线中点

//			Line3D* line2 = new Line3D(firstCoord, line1Center);
			Point * point1 = new Point(firstCoord);
			point1->SetDrawType(1);
			point1->SetSize(0.8f);

			Line3D * line6 = new Line3D(firstCoord, projectPnt); //投影距离线
			Color line6Color = Color::BLACK;
			line6->SetColor(m_measureColor1);
			Point * point2 = new Point(projectPnt); //投影点
			point2->SetDrawType(1);
			point2->SetSize(0.8f);

			Line3D * line4 = new Line3D(maxPoint, firstCoord); //端点长距离
			Color line4Color = Color::RED;
			line4->SetColor(m_measureColor2);

			Line3D* line5 = new Line3D(minPoint, firstCoord); //端点短距离
			Color line5Color = Color::BLUE;
			line5->SetColor(m_measureColor3);

			Line3D* line7 = NULL; //延长线
			if (!(MeasureCalculateHelper::pntInSegment(projectPnt,
					lineAttribute)))
			{
				line7 = new Line3D(minPoint, projectPnt); //延长线
				Color line7Color = Color::BLACK;
				line7->SetColor(m_exLineColor);
				line7->SetName("exLine");

			}

			//给线设置箭头末端符号
			//line2->SetStartArrow(1);
			//line2->SetEndArrow(1);

			Line3D* line3 = new Line3D((projectPnt + firstCoord) / 2,
					pntInPlane); //文本引线

			line3->SetName("MeasureImageLeader");
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line3->SetColor(m_leaderColor);

			//根据整个场景包围盒的大小来确定文字的大小
			float fontSize = scene->GetSceneBox().Length() / 20;

			//将线加入测量对象中
			measure->AddLine(line1);
			//measure->AddLine(line2);
			measure->AddLine(line3);
			measure->AddLine(line4);
			measure->AddLine(line5);
			measure->AddLine(line6);
			if (line7)
			{
				measure->AddLine(line7);
			}
			measure->AddPoint(point1);
			measure->AddPoint(point2);

			//构造显示文本
			vector<float> textLength;
			string typestr = language.GetCurrentTypeValue(
					"PointLineProjectDistance");
			float projectDistTextLength =  language.GetCurrentTypeValueLength(
					"PointLineProjectDistance");
			textLength.push_back(projectDistTextLength);

			string maxDistanceTypeStr = language.GetCurrentTypeValue(
					"PointLineToTerminalMaxDistance");
			float maxDstanceTextlength = language.GetCurrentTypeValueLength(
					"PointLineToTerminalMaxDistance");
			textLength.push_back(maxDstanceTextlength);

			string minDistanceTypeStr = language.GetCurrentTypeValue(
					"PointLineToTerminalMinDistance");
			float minDstanceTextlength = language.GetCurrentTypeValueLength(
					"PointLineToTerminalMinDistance");
			textLength.push_back(minDstanceTextlength);

			float maxTextLength = *std::max_element(textLength.begin(),textLength.end());

            
            

			vector<float> values;
			values.push_back(distance);
			values.push_back(maxDistance);
			values.push_back(minDistance);
			string unit;
			MeasureDisplayHelper::SetMeasureUnit(
					SVIEW::Parameters::Instance()->m_measureUnitStyle, values,
					unit);
			string lengthstr = M3DTOOLS::floatToString(values.at(0)) + unit;
			string maxDistancStr = M3DTOOLS::floatToString(values.at(1)) + unit;
			string minDistanceStr = M3DTOOLS::floatToString(values.at(2))
					+ unit;

            //解决协同中文本读取
            ComText* ctLength = new ComText();
            CText* tLength = new CText;
            tLength->SetLanguageType("PointLineProjectDistance");
            tLength->SetText(lengthstr);
            ctLength->AddCText(tLength);
            measure->m_ComTexts.push_back(ctLength);
            
            ComText* ctMaxDistanc = new ComText();
            CText* tMaxDistanc = new CText;
            tMaxDistanc->SetLanguageType("PointLineToTerminalMaxDistance");
            tMaxDistanc->SetText(maxDistancStr);
            ctMaxDistanc->AddCText(tMaxDistanc);
            measure->m_ComTexts.push_back(ctMaxDistanc);

            ComText* ctMinDistance = new ComText();
            CText* tMinDistance = new CText;
            tMinDistance->SetLanguageType("PointLineToTerminalMinDistance");
            tMinDistance->SetText(minDistanceStr);
            ctMinDistance->AddCText(tMinDistance);
            measure->m_ComTexts.push_back(ctMinDistance);
            
			//长度
			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End(maxTextLength, 100.0);
			float rect2L = 450.0;
			Color gr = Color::GRAY;
			Color wt = Color::WHITE;
			Color blk = Color::BLACK;
			Color grn = Color::GREEN;
			Color rd = Color::RED;
			Color blu = Color::BLUE;
			Vector2 rectStart(1.0, 1.0);
			Vector2 rectEnd(maxTextLength, 300.0);
//			MeasureDisplayHelper::createShadowRect(shape2DSet, rectStart,
//					rectEnd, rect2L);
			MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start,
					rect1End, rect2L, m_measureColor1, wt, wt, blk, typestr, lengthstr,
					fntSize, false);

			Vector2 rect2Start(1, 100.0);
			Vector2 rect2End(maxTextLength, 200.0);
			MeasureDisplayHelper::createRectImage(shape2DSet, rect2Start,
					rect2End, rect2L, m_measureColor2, wt, wt, blk,maxDistanceTypeStr
					, maxDistancStr,
					fntSize, false);
			Vector2 rect3Start(1, 200.0);
			Vector2 rect3End(maxTextLength, 300.0);
			MeasureDisplayHelper::createRectImage(shape2DSet, rect3Start,
					rect3End, rect2L, m_measureColor3, wt, wt, blk,
					minDistanceTypeStr, minDistanceStr,
					fntSize, true);

			ImageBoard* imageboard = NULL;
			MeasureDisplayHelper::addImageToMemory(scene, imageboard,
					shape2DSet, pntInPlane,(maxTextLength+rect2L)/100.0f, 3);
			measure->AddImage(imageboard);
		}
		else if (geo != NULL
				&& geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_ELLIPSE)
		{
			LOGI("point to circle distance");
			Color gr = Color::GRAY;
			Color wt = Color::WHITE;
			Color blk = Color::BLACK;
			Color grn = Color::GREEN;
			Color rd = Color::RED;
			Color blu = Color::BLUE;
			//几何属性为空，则赋值默认几何属性为 M3D_GEOATTR_TYPE_UNKNOWN
			EllipseAttribute ellipseAttribute = *((EllipseAttribute*) geo);

			Matrix3x4 worldMatrix = ShapeHelper::GetShapeWorldMatrix(
					secondEdge);

			GeometryHelper::Transform(ellipseAttribute, worldMatrix);
			Vector3 firstCoord = firstPnt->GetPosition();

			float distance;
			int inLine;

			Vector3 projectPnt; //圆弧圆心

			MeasureCalculateHelper::PntLineDistance(firstCoord,
					ellipseAttribute, distance, inLine, projectPnt, worldMatrix,
					m_lineSet);

			Vector3 pntStartPoint = ellipseAttribute.GetStartPoint(); //!< Line起点
			Vector3 pntEndPoint = ellipseAttribute.GetEndPoint(); //!< Line终点

			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(),
					(pntStartPoint + pntEndPoint) / 2);

			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//构造线
			Line3D* line1 = new Line3D(projectPnt, firstCoord);//测量线1
			line1->SetColor(m_measureColor1);


			Vector3 line1Center = (projectPnt + firstCoord) / 2;

			Line3D* line2 = new Line3D(line1Center, pntInPlane); //文本引线
			line2->SetName("MeasureImageLeader");
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line2->SetColor(m_leaderColor);

			Point * point1 = new Point(firstCoord);
			point1->SetDrawType(1);
			point1->SetSize(0.8f);

			Point * point2 = new Point(projectPnt);
			point2->SetDrawType(1);
			point2->SetSize(0.8f);
			measure->AddPoint(point2);


			//构造显示文本
			string typestr = language.GetCurrentTypeValue(
					"PointLineMinDistance");
			float minDistancTextlength = language.GetCurrentTypeValueLength(
					"PointLineMinDistance");
			//根据整个场景包围盒的大小来确定文字的大小
			float fontSize = scene->GetSceneBox().Length() / 20;

			//构造选中的圆弧
			SPolyLine * xPolyLine = m_lineSet->GetRefLine();
			vector<Vector3> & pntPoints = xPolyLine->GetPoints();
			unsigned int offset = m_lineSet->GetDataOffset();
			unsigned int dataLength = m_lineSet->GetDataLength();

			int countIt = 1;
			for (vector<Vector3>::iterator it = pntPoints.begin() + offset;
					it != pntPoints.begin() + offset + dataLength; it = it + 2)
			{

				Line3D * lineTemp = new Line3D(worldMatrix * (*it),
						worldMatrix * (*(it + 1)));
				Color tempColor = Color::GREEN;
				lineTemp->SetColor(m_selectColor);
				measure->AddLine(lineTemp);
				//	LOGI("Add line number: %d", countIt);
			}

			//将线加入测量对象中
			measure->AddLine(line1);
			measure->AddLine(line2);
			measure->AddPoint(point1);

			vector<float> values;
			values.push_back(distance);
			string unit;
			MeasureDisplayHelper::SetMeasureUnit(
					SVIEW::Parameters::Instance()->m_measureUnitStyle, values,
					unit);
			string distancestr = M3DTOOLS::floatToString(values.at(0)) + unit;
            //解决协同中文本读取
            ComText* ctLength = new ComText();
            CText* tLength = new CText;
            tLength->SetLanguageType("PointLineMinDistance");
            tLength->SetText(distancestr);
            ctLength->AddCText(tLength);
            measure->m_ComTexts.push_back(ctLength);
			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End(minDistancTextlength, 100.0);
			float rect2L = 450.0;

//			MeasureDisplayHelper::createShadowRect(shape2DSet, rect1Start,
//					rect1End, rect2L);
			MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start,
					rect1End, rect2L, m_measureColor1, wt, wt, blk, typestr, distancestr,
					fntSize, true);

			ImageBoard* imageboard = NULL;
			MeasureDisplayHelper::addImageToMemory(scene, imageboard,
					shape2DSet, pntInPlane, (minDistancTextlength+rect2L)/100.0f, 1);
			measure->AddImage(imageboard);

		}

		else
		{
			LOGI("point to polyline distance");
			//没有几何属性，相当于求折线距离。折线距离为点到各折线段最短距离中的最小值
			Vector3 firstCoord = firstPnt->GetPosition();
			Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(
					secondEdge);
			Matrix3x4 inverseMat = worldMatrix2.Inverse();
			Vector3 modelVec = inverseMat * firstCoord;

			float distance;
			Vector3 minPoint;
			MeasureCalculateHelper::PntLineDistacne(m_lineSet, modelVec,
					distance, minPoint);
			minPoint = worldMatrix2 * minPoint;

			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(), firstCoord);

			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//构造线
			Line3D* line2 = new Line3D((minPoint + firstCoord) / 2, pntInPlane);
			line2->SetName("MeasureImageLeader");
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line2->SetColor(m_leaderColor);
			Line3D* line1 = new Line3D(firstCoord, minPoint);
			line1->SetColor(m_measureColor1);
			Point * point1 = new Point(firstCoord);
			point1->SetDrawType(1);
			point1->SetSize(0.8f);
			Point * point2 = new Point(minPoint);
			point2->SetDrawType(1);
			point2->SetSize(0.8f);
			measure->AddPoint(point2);

			//			line2->SetEndArrow(1);

			//构造显示文本
			string typestr = language.GetCurrentTypeValue(
					"PointLineToPolyLineDistance");
			float minDistancTextlength = language.GetCurrentTypeValueLength(
					"PointLineToPolyLineDistance");

			//根据整个场景包围盒的大小来确定文字的大小
			float fontSize = scene->GetSceneBox().Length() / 20;

			//构造选中的折线
			SPolyLine * xPolyLine = m_lineSet->GetRefLine();
			vector<Vector3> & pntPoints = xPolyLine->GetPoints();
			unsigned int offset = m_lineSet->GetDataOffset();
			unsigned int dataLength = m_lineSet->GetDataLength();

			int countIt = 1;
			for (vector<Vector3>::iterator it = pntPoints.begin() + offset;
					it != pntPoints.begin() + offset + dataLength; it = it + 2)
			{

				Line3D * lineTemp = new Line3D(worldMatrix2 * (*it),
						worldMatrix2 * (*(it + 1)));
				Color tempColor = Color::GREEN;
				lineTemp->SetColor(m_selectColor);

				measure->AddLine(lineTemp);
				//LOGI("Add line number: %d", countIt);
			}

			//将线加入测量对象中
			measure->AddLine(line1);
			measure->AddLine(line2);
			vector<float> values;
			values.push_back(distance);
			string unit;
			MeasureDisplayHelper::SetMeasureUnit(
					SVIEW::Parameters::Instance()->m_measureUnitStyle, values,
					unit);
			string distancestr = M3DTOOLS::floatToString(values.at(0)) + unit;
            
            //解决协同中文本读取
            ComText* ctLength = new ComText();
            CText* tLength = new CText;
            tLength->SetLanguageType("PointLineToPolyLineDistance");
            tLength->SetText(distancestr);
            ctLength->AddCText(tLength);
            measure->m_ComTexts.push_back(ctLength);
            
			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End(minDistancTextlength, 100.0);
			float rect2L = 450.0;
			Color gr = Color::GRAY;
			Color wt = Color::WHITE;
			Color blk = Color::BLACK;
			Color grn = Color::GREEN;
			Color rd = Color::RED;
			Color blu = Color::BLUE;

//			MeasureDisplayHelper::createShadowRect(shape2DSet, rect1Start,
//					rect1End, rect2L);
			MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start,
					rect1End, rect2L, m_measureColor1, wt, wt, blk, typestr, distancestr,
					fntSize, true);

			ImageBoard* imageboard = NULL;
			MeasureDisplayHelper::addImageToMemory(scene, imageboard,
					shape2DSet, pntInPlane, (minDistancTextlength+rect2L)/100.0f, 1);
			measure->AddImage(imageboard);

		}
	}
	else
	{
		LOGE("MeasureFactory::createPntTOLineDistance shape is NULL");
	}

	return measure;
}

Measure* MeasureFactory::createLineToLineDistance(int firstLineId,
		int secondLineId, Vector2& screenPnt, SceneManager* scene)
{
	Measure* measure = NULL;
	LOGE("createLineTOLineDistance step1");
	InternationalManager language;
	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);

	Shape * firstShape = scene->GetShape(firstLineId);
	Shape *secondShape = scene->GetShape(secondLineId);

	if (firstShape && secondShape && firstShape->GetType() == SHAPE_EDGE
			&& secondShape->GetType() == SHAPE_EDGE)
	{

		LOGE("createLineTOLineDistance step2");
		measure = new MeasureDistance;

		measure->SetMeasureType(Measure::MEASURE_TYPE_LINE_LINE_DISTANCE);

		Edge * firstEdge = (Edge*) firstShape;
		Edge * secondEdge = (Edge *) secondShape;

		RefPolyLine * firstLineSet = firstEdge->GetLineData();
		RefPolyLine * secondLineSet = secondEdge->GetLineData();

		GeometryAttribute * firstGeo = firstLineSet->GetGeoAttribute();
		GeometryAttribute * secondGeo = secondLineSet->GetGeoAttribute();

		if (firstGeo != NULL
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE
				&& secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE)
		{
			LOGE("createLineTOLineDistance step3");
			LineAttribute firstLineAttribute = *((LineAttribute *) firstGeo);
			LineAttribute secondLineAttribute = *((LineAttribute *) secondGeo);

			Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstEdge);
			Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(
					secondEdge);
			GeometryHelper::Transform(firstLineAttribute, worldMatrix1);
			GeometryHelper::Transform(secondLineAttribute, worldMatrix2);

			float distance;
			Vector3 projectPnt1, projectPnt2;
			bool parallel = true;
			MeasureCalculateHelper::LineLineDistance(firstLineAttribute,
					secondLineAttribute, distance, projectPnt1, projectPnt2,
					parallel);

			Vector3 pntStartPoint1 = firstLineAttribute.GetStartPoint();
			Vector3 pntEndPoint1 = firstLineAttribute.GetEndPoint();

			Vector3 pntStartPoint2 = secondLineAttribute.GetStartPoint();
			Vector3 pntEndPoint2 = secondLineAttribute.GetEndPoint();

			//判断得到的垂足是否在两条线段上，只要一条不在，就用四个端点之间的距离来判断。
			float sdis1, edis1, sdis2, edis2;

			bool in1, in2;
			in1 = MeasureCalculateHelper::pntInSegment(projectPnt1,
					firstLineAttribute);
			in2 = MeasureCalculateHelper::pntInSegment(projectPnt2,
					secondLineAttribute);

			float firstLineStartToSecondLineMinDistance = 0.0f;
			Vector3 firstStartProjectPnt;
			MeasureCalculateHelper::PntLineDistance(pntStartPoint2,
					pntEndPoint2, pntStartPoint1,
					firstLineStartToSecondLineMinDistance,
					firstStartProjectPnt);
			float firstLineEndToSecondLineMinDistance = 0.0f;
			Vector3 firstEndProjectPnt;
			MeasureCalculateHelper::PntLineDistance(pntStartPoint2,
					pntEndPoint2, pntEndPoint1,
					firstLineEndToSecondLineMinDistance, firstEndProjectPnt);
			float secondLineStartToFirstLineMinDistance = 0.0f;
			Vector3 secondStartProjectPnt;
			MeasureCalculateHelper::PntLineDistance(pntStartPoint1,
					pntEndPoint1, pntStartPoint2,
					secondLineStartToFirstLineMinDistance,
					secondStartProjectPnt);
			float secondLineEndToFirstLineMinDistance = 0.0f;
			Vector3 secondEndProjectPnt;
			MeasureCalculateHelper::PntLineDistance(pntStartPoint1,
					pntEndPoint1, pntEndPoint2,
					secondLineEndToFirstLineMinDistance, secondEndProjectPnt);

			float min = 0;
			Vector3 minPoint1, minPoint2;
			sdis2 = secondLineStartToFirstLineMinDistance;
			edis2 = secondLineEndToFirstLineMinDistance;
			sdis1 = firstLineStartToSecondLineMinDistance;
			edis1 = firstLineEndToSecondLineMinDistance;
			if (sdis2 > edis2)
			{
				min = edis2;
				minPoint1 = secondEndProjectPnt;
				minPoint2 = pntEndPoint2;
			}
			else
			{
				min = sdis2;
				minPoint1 = secondStartProjectPnt;
				minPoint2 = pntStartPoint2;
			}

			if (min > sdis1)
			{
				min = sdis1;
				minPoint1 = firstStartProjectPnt;
				minPoint2 = pntStartPoint1;
			}

			if (min > edis1)
			{
				min = edis1;
				minPoint1 = pntEndPoint1;
				minPoint2 = firstEndProjectPnt;
			}

			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

			Vector3 lineCenter = (projectPnt1 + projectPnt2) / 2;
			Vector3 dxdydz = projectPnt2 - projectPnt1;

			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(), lineCenter);

			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//构造线
			Line3D* line1 = new Line3D(pntStartPoint1, pntEndPoint1);//测量线1
			Color line1Color = Color::GREEN;
			line1->SetColor(m_selectColor);
			//Vector3 line1Center = (pntStartPoint1, pntEndPoint1) / 2;

			Line3D* line2 = new Line3D(pntStartPoint2, pntEndPoint2);//测量线2
			Color line2Color = Color::GREEN;
			line2->SetColor(m_selectColor);
			//Vector3 line2Center = (pntStartPoint2, pntEndPoint2) / 2;

			Line3D* line3 = new Line3D(projectPnt1, projectPnt2);//公垂线段
			Color line3Color = Color::BLACK;
			line3->SetColor(m_measureColor1);

			Line3D* line4 = new Line3D(lineCenter, pntInPlane);//引线
			line4->SetName("MeasureImageLeader");
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line4->SetColor(m_leaderColor);

			Line3D* line6 = new Line3D(minPoint1, minPoint2); //最小距离
			Color line6Color = Color::RED;
			line6->SetColor(m_measureColor2);

			Vector3 extern1;
			if((projectPnt1-pntStartPoint1).Length()>(projectPnt1-pntEndPoint1).Length())
			{
				extern1 = pntEndPoint1;
			}
			else
			{
				extern1 = pntStartPoint1;
			}
			Line3D* line7 = new Line3D(extern1,projectPnt1);//线1延长线
			Color line7Color = Color::BLACK;
			line7->SetColor(m_exLineColor);
			line7->SetName("exLine");

			Vector3 extern2;
			if ((projectPnt2 - pntStartPoint2).Length()
					> (projectPnt2 - pntEndPoint2).Length())
			{
				extern2 = pntEndPoint2;
			}
			else
			{
				extern2 = pntStartPoint2;
			}
			Line3D* line8 = new Line3D(extern2,projectPnt2);//线2延长线
			Color line8Color = Color::BLACK;
			line8->SetColor(m_exLineColor);
			line8->SetName("exLine");

			Point * point1 = new Point(projectPnt1);//垂足1
			point1->SetDrawType(1);
			point1->SetSize(0.8f);
			Point * point2 = new Point(projectPnt2);//垂足2
			point2->SetDrawType(1);
			point2->SetSize(0.8f);


			string dx = M3DTOOLS::floatToString(dxdydz.m_x);
			string dy = M3DTOOLS::floatToString(dxdydz.m_y);
			string dz = M3DTOOLS::floatToString(dxdydz.m_z);

			//根据整个场景包围盒的大小来确定文字的大小
			float fontSize = scene->GetSceneBox().Length() / 20;

			measure->AddLine(line1);
			measure->AddLine(line2);
			measure->AddLine(line3);
			measure->AddLine(line4);
			//measure->AddLine(line5);
			measure->AddLine(line6);
			if (!in1)
			{
				measure->AddLine(line7);
			}
			if(!in2)
			{
				measure->AddLine(line8);
			}
			measure->AddPoint(point1);
			measure->AddPoint(point2);

			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;

			//设置初始颜色、位置等信息
			float rect2L = 450.0;
			Color gr = Color::GRAY;
			Color wt = Color::WHITE;
			Color blk = Color::BLACK;
			Color grn = Color::GREEN;
			Color rd = Color::RED;
			Color blu = Color::BLUE;

			if (parallel)
			{

				vector<float> textLength;

				string typestr = language.GetCurrentTypeValue("LineLineParallelDistance");
				float parallelDistTextLength =  language.GetCurrentTypeValueLength(
						"LineLineParallelDistance");
				textLength.push_back(parallelDistTextLength);

				string minDistanceTypeStr = language.GetCurrentTypeValue("LineLineMinDistance");
				float minDstanceTextlength = language.GetCurrentTypeValueLength(
						"LineLineMinDistance");
				textLength.push_back(minDstanceTextlength);

				float maxTextLength = *std::max_element(textLength.begin(),textLength.end());

				vector<float> values;
				values.push_back(distance);
				values.push_back(min);
				string unit;
				MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
						,values,unit);
				string distancestr = M3DTOOLS::floatToString(values.at(0)) + unit;
				string minStr = M3DTOOLS::floatToString(values.at(1)) + unit;
                
                //解决协同中文本读取
                ComText* ctLength = new ComText();
                CText* tLength = new CText;
                tLength->SetLanguageType("LineLineParallelDistance");
                tLength->SetText(distancestr);
                ctLength->AddCText(tLength);
                measure->m_ComTexts.push_back(ctLength);
                
                ComText* ctMin = new ComText();
                CText* tMin = new CText;
                tMin->SetLanguageType("LineLineMinDistance");
                tMin->SetText(minStr);
                ctMin->AddCText(tMin);
                measure->m_ComTexts.push_back(ctMin);
                
				Vector2 rectStart(1.0, 1.0);
				Vector2 rectEnd(maxTextLength, 200.0);
//				MeasureDisplayHelper::createShadowRect(shape2DSet, rectStart,
//						rectEnd, rect2L);
				Vector2 rect1Start(1.0, 1.0);
				Vector2 rect1End(maxTextLength, 100.0);
				MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start,
						rect1End, rect2L, m_measureColor1, wt, wt, blk, typestr, distancestr,
						fntSize, false);

				Vector2 rect3Start(1.0, 100.0);
				Vector2 rect3End(maxTextLength, 200.0);

				MeasureDisplayHelper::createRectImage(shape2DSet, rect3Start,
						rect3End, rect2L, m_measureColor2, wt, wt, blk,minDistanceTypeStr ,minStr,
						fntSize, true);//min

				ImageBoard* imageboard=NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
								(maxTextLength + rect2L)/100.0f, 2);
				measure->AddImage(imageboard);
			}
			else
			{
				vector<float> textLength;
				string typestr1 = language.GetCurrentTypeValue("LineLineMinDistance");
				float minDstanceTextlength = language.GetCurrentTypeValueLength(
						"LineLineMinDistance");
				textLength.push_back(minDstanceTextlength);

				string skewLinesDistStr  = language.GetCurrentTypeValue("LineLineSkewLinesDistance");
				float  skewLinesDistTextlength = language.GetCurrentTypeValueLength(
						"LineLineSkewLinesDistance");
				textLength.push_back(skewLinesDistTextlength);

				float maxTextLength = *std::max_element(textLength.begin(),textLength.end());



				vector<float> values;
				values.push_back(distance);
				values.push_back(min);
				string unit;
				MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
						,values,unit);
				string distanceStr = M3DTOOLS::floatToString(values.at(0)) + unit;
				string minStr = M3DTOOLS::floatToString(values.at(1)) + unit;
                //解决协同中文本读取
                ComText* ctDistance = new ComText();
                CText* tDistance = new CText;
                tDistance->SetText(distanceStr);
                tDistance->SetLanguageType("LineLineSkewLinesDistance");
                ctDistance->AddCText(tDistance);
                measure->m_ComTexts.push_back(ctDistance);
                
                ComText* ctMinDistance = new ComText();
                CText* tMinDistance = new CText;
                tMinDistance->SetText(minStr);
                tMinDistance->SetLanguageType("LineLineMinDistance");
                ctMinDistance->AddCText(tMinDistance);
                measure->m_ComTexts.push_back(ctMinDistance);
                
                
				//string lengthstr2 = M3DTOOLS::floatToString(max);
				Vector2 rectStart(1.0, 1.0);
				Vector2 rectEnd(maxTextLength, 200.0);
//				MeasureDisplayHelper::createShadowRect(shape2DSet, rectStart, rectEnd,
//						rect2L);
				Vector2 rect1Start(1.0, 1.0);
				Vector2 rect1End(maxTextLength, 100.0);

				MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start,
						rect1End, rect2L, m_measureColor1, wt, wt, blk,skewLinesDistStr, distanceStr,
						fntSize, false);//distance

				Vector2 rect3Start(1.0, 100.0);
				Vector2 rect3End(maxTextLength, 200.0);

				MeasureDisplayHelper::createRectImage(shape2DSet, rect3Start,
						rect3End, rect2L, m_measureColor2, wt, wt, blk, typestr1, minStr,
						fntSize, true);//min

				ImageBoard* imageboard=NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
						(maxTextLength + rect2L)/100.0f, 2);
				measure->AddImage(imageboard);
			}

			//将文本加入到测量对象中
			//	measure->AddText(lengthstr, pntInPlane, fontSize);

		}
		else if (firstGeo != NULL
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_ELLIPSE
				&& secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_ELLIPSE)
		{

			EllipseAttribute firstEllipseAttribute = *((EllipseAttribute *) firstGeo);
			EllipseAttribute secondEllipseAttribute = *((EllipseAttribute *) secondGeo);

			Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstEdge);
			Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(
					secondEdge);
			GeometryHelper::Transform(firstEllipseAttribute, worldMatrix1);
			GeometryHelper::Transform(secondEllipseAttribute, worldMatrix2);

			float distance0,distance1;
			Vector3 projectPnt0,projectPnt1;
			int type;

			MeasureCalculateHelper::LineLineDistance(firstEllipseAttribute,secondEllipseAttribute,
					distance0,distance1,projectPnt0,projectPnt1,type);

			Vector3 center0 ,center1;//圆心
			float radius0,radius1 ;//半径
			Vector3 start0,start1;//起点
			Vector3 dirX0,dirY0,dirZ0,dirX1,dirY1,dirZ1;

			firstEllipseAttribute.GetXYZDir(dirX0,dirY0,dirZ0);
			secondEllipseAttribute.GetXYZDir(dirX1,dirY1,dirZ1);

			center0 = firstEllipseAttribute.GetCenterPoint();
			center1 = secondEllipseAttribute.GetCenterPoint();
			radius0 = firstEllipseAttribute.GetMajorRadius();
			radius1 = secondEllipseAttribute.GetMajorRadius();
			start0 = firstEllipseAttribute.GetStartPoint();
			start1 = secondEllipseAttribute.GetStartPoint();

			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

			Vector3 lineCenter = (projectPnt1 + projectPnt0) / 2;

			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(), lineCenter);

			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//构造线
			//Line3D* line1 = new Line3D(pntStartPoint1, pntEndPoint1);//轴线1

			//构造圆心
			Point * centerPoint1 = new Point(center1);
			centerPoint1->SetDrawType(1);
			centerPoint1->SetSize(0.8f);
			Point * centerPoint0 = new Point(center0);
			centerPoint0->SetDrawType(1);
			centerPoint0->SetSize(0.8f);

			measure->AddPoint(centerPoint0);
			measure->AddPoint(centerPoint1);

			//构造选中的圆弧
			{
				SPolyLine * xPolyLine = firstLineSet->GetRefLine();
				vector<Vector3> & pntPoints = xPolyLine->GetPoints();
				unsigned int offset = firstLineSet->GetDataOffset();
				unsigned int dataLength = firstLineSet->GetDataLength();

				int countIt = 1;
				for (vector<Vector3>::iterator it = pntPoints.begin() + offset;
						it != pntPoints.begin() + offset + dataLength; it = it+2)
				{

					Line3D * lineTemp = new Line3D(worldMatrix1 * (*it),
							worldMatrix1 * (*(it + 1)));
					Color tempColor = Color::GREEN;
					lineTemp->SetColor(m_selectColor);
					measure->AddLine(lineTemp);
				}
			}

			{
				SPolyLine * xPolyLine = secondLineSet->GetRefLine();
				vector<Vector3> & pntPoints = xPolyLine->GetPoints();
				unsigned int offset = secondLineSet->GetDataOffset();
				unsigned int dataLength = secondLineSet->GetDataLength();

				int countIt = 1;
				for (vector<Vector3>::iterator it = pntPoints.begin() + offset;
						it != pntPoints.begin() + offset + dataLength; it = it+2)
				{

					Line3D * lineTemp = new Line3D(worldMatrix2 * (*it),
							worldMatrix2 * (*(it + 1)));
					Color tempColor = Color::GREEN;
					lineTemp->SetColor(m_selectColor);
					measure->AddLine(lineTemp);
				}
			}

			vector<float> values;
			values.push_back(distance0);
			values.push_back(distance1);
			string unit;
			MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
					,values,unit);
			string distance0Str = M3DTOOLS::floatToString(values.at(0))+unit;
			string distance1Str = M3DTOOLS::floatToString(values.at(1))+unit;
            
			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			float rect2L = 450.0;
			Color gr = Color::GRAY;
			Color wt = Color::WHITE;
			Color blk = Color::BLACK;
			Color grn = Color::GREEN;
			Color rd = Color::RED;
			Color blu = Color::BLUE;

			//设置初始颜色、位置等信息

			if(0 == type)
			{
				//构造线
				Line3D* line1 = new Line3D(center0, center1);//圆心距
				line1->SetColor(rd);

				measure->AddLine(line1);

				Line3D* line2 = new Line3D(lineCenter,pntInPlane);//引线
				line2->SetName("MeasureImageLeader");
				//Color oriangle(1.0f,0.44f,0.0f) ;
				line2->SetColor(m_leaderColor);
				measure->AddLine(line2);

				string  circlesCenterDistTypeStr = language.GetCurrentTypeValue("LineLineCirclesCenterDistance");
				float circlesCenterDistTextLength = language.GetCurrentTypeValueLength("LineLineCirclesCenterDistance");
                
                ComText* ctDistance1 = new ComText();
                CText* tDistance1 = new CText;
                tDistance1->SetText(distance1Str);
                tDistance1->SetLanguageType("LineLineCirclesCenterDistance");
                ctDistance1->AddCText(tDistance1);
                measure->m_ComTexts.push_back(ctDistance1);
                
				Vector2 rectStart(1.0, 1.0);
				Vector2 rectEnd(circlesCenterDistTextLength, 100.0);
//				MeasureDisplayHelper::createShadowRect(shape2DSet, rectStart, rectEnd,
//						rect2L);

				Vector2 rect1Start(1.0, 1.0);
				Vector2 rect1End(circlesCenterDistTextLength, 100.0);
				MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start,
						rect1End, rect2L, m_measureColor1, wt, wt, blk,circlesCenterDistTypeStr ,distance1Str,
						fntSize, true);//distance1

				ImageBoard* imageboard=NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
						(circlesCenterDistTextLength + rect2L)/100.0f, 1);
				measure->AddImage(imageboard);
			}
			else if(1 == type)
			{
				Line3D* line1 = new Line3D(center0, center1);//圆心距
				line1->SetColor(m_measureColor1);
				measure->AddLine(line1);

				Line3D* line2 = new Line3D(projectPnt0,projectPnt1);//轴距
				line2->SetColor(m_measureColor2);
				measure->AddLine(line2);

				Line3D* line3 = new Line3D(center0,projectPnt0);//辅助线
				line3->SetColor(m_exLineColor);
				line3->SetName("exLine");
				measure->AddLine(line3);

				Line3D* line4 = new Line3D((center0+center1)/2,pntInPlane);//引线
				line4->SetName("MeasureImageLeader");
				//Color oriangle(1.0f,0.44f,0.0f) ;
				line4->SetColor(m_leaderColor);
				measure->AddLine(line4);

				Point* point1 = new Point(projectPnt0);
				point1->SetDrawType(1);
				point1->SetSize(0.8f);
				measure->AddPoint(point1);
				Point* point2 = new Point(projectPnt1);
				point2->SetDrawType(1);
				point2->SetSize(0.8f);
				measure->AddPoint(point2);

				vector<float> textLength;
				string  circlesCenterDistTypeStr = language.GetCurrentTypeValue("LineLineCirclesCenterDistance");
				float circlesCenterDistTextLength = language.GetCurrentTypeValueLength("LineLineCirclesCenterDistance");
				textLength.push_back(circlesCenterDistTextLength);

				string  wheelbaseTypeStr = language.GetCurrentTypeValue("LineLineWheelbase");
				float  wheelbaseTextLength = language.GetCurrentTypeValueLength("LineLineWheelbase");
				textLength.push_back(wheelbaseTextLength);
                //解决协同中文本读取
                ComText* ctDistance1 = new ComText();
                CText* tDistance1 = new CText;
                tDistance1->SetText(distance1Str);
                tDistance1->SetLanguageType("LineLineCirclesCenterDistance");
                ctDistance1->AddCText(tDistance1);
                measure->m_ComTexts.push_back(ctDistance1);
                
                ComText* ctDistance0 = new ComText();
                CText* tDistance0 = new CText;
                tDistance0->SetText(distance0Str);
                tDistance0->SetLanguageType("LineLineWheelbase");
                ctDistance0->AddCText(tDistance0);
                measure->m_ComTexts.push_back(ctDistance0);
                
				float maxTextLength = *std::max_element(textLength.begin(),textLength.end());

				Vector2 rectStart(1.0, 1.0);
				Vector2 rectEnd(maxTextLength, 200.0);
//				MeasureDisplayHelper::createShadowRect(shape2DSet, rectStart, rectEnd,
//						rect2L);

				Vector2 rect1Start(1.0, 1.0);
				Vector2 rect1End(maxTextLength, 100.0);
				MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start,
						rect1End, rect2L, m_measureColor1, wt, wt, blk, circlesCenterDistTypeStr, distance1Str,
						fntSize, false);//distance1


				Vector2 rect2Start(1.0, 100.0);
				Vector2 rect2End(maxTextLength, 200.0);
				MeasureDisplayHelper::createRectImage(shape2DSet, rect2Start,
						rect2End, rect2L, m_measureColor2, wt, wt, blk, wheelbaseTypeStr, distance0Str,
						fntSize, true);//distance0

				ImageBoard* imageboard=NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
						(maxTextLength + rect2L)/100.0f, 2);
				measure->AddImage(imageboard);
			}
			else if(2==type)
			{
				//构造线
				string radiusDifferenceTypeStr = language.GetCurrentTypeValue("LineLineRadiusDifference");
				float radiusDifferenceTextLength = language.GetCurrentTypeValueLength("LineLineRadiusDifference");
				Line3D* line1 = new Line3D(projectPnt0, projectPnt1);//半径差
				line1->SetColor(m_measureColor1);
				measure->AddLine(line1);

                ComText* ctDistance1 = new ComText();
                CText* tDistance1 = new CText;
                tDistance1->SetText(distance1Str);
                tDistance1->SetLanguageType("LineLineRadiusDifference");
                ctDistance1->AddCText(tDistance1);
                measure->m_ComTexts.push_back(ctDistance1);
                
				Line3D* line2 = new Line3D(lineCenter,pntInPlane);//引线
				line2->SetName("MeasureImageLeader");
				//Color oriangle(1.0f,0.44f,0.0f) ;
				line2->SetColor(m_leaderColor);
				measure->AddLine(line2);


				Vector2 rectStart(1.0, 1.0);
				Vector2 rectEnd(radiusDifferenceTextLength, 100.0);
//				MeasureDisplayHelper::createShadowRect(shape2DSet, rectStart, rectEnd,
//						rect2L);
				Vector2 rect1Start(1.0, 1.0);
				Vector2 rect1End(radiusDifferenceTextLength, 100.0);

				MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start,
						rect1End, rect2L, m_measureColor1, wt, wt, blk, radiusDifferenceTypeStr, distance1Str,
						fntSize, true);//distance1

				ImageBoard* imageboard=NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
						(radiusDifferenceTextLength + rect2L)/100.0f, 1);
				measure->AddImage(imageboard);
			}
		}
		else if ((firstGeo != NULL
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_ELLIPSE
				&& secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE)
				|| (firstGeo != NULL
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE
				&& secondGeo != NULL
			    && secondGeo->GetGeoAttrType()== M3D_GEOATTR_TYPE_ELLIPSE))
		{
			LOGI("line and ellipse distance BEGIN");
			Color gr = Color::GRAY;
			Color wt = Color::WHITE;
			Color blk = Color::BLACK;
			Color grn = Color::GREEN;
			Color rd = Color::RED;
			Color blu = Color::BLUE;
			Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(firstEdge);
			Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(secondEdge);

			Vector3 minProject1,minProject2;
			float minDistance=100000.0f;
			MeasureCalculateHelper::PolylinePolylineDistance(firstLineSet,secondLineSet,minProject1,
					minProject2,minDistance,worldMatrix1,worldMatrix2);
			//minProject1 =worldMatrix1* minProject1;
			//minProject2 =worldMatrix2* minProject2;

			//构造选中的折线
			{
				SPolyLine * xPolyLine = firstLineSet->GetRefLine();
				vector<Vector3> & pntPoints = xPolyLine->GetPoints();
				unsigned int offset = firstLineSet->GetDataOffset();
				unsigned int dataLength = firstLineSet->GetDataLength();

				int countIt = 1;
				for (vector<Vector3>::iterator it = pntPoints.begin() + offset;
						it != pntPoints.begin() + offset + dataLength; it = it+2)
				{

					Line3D * lineTemp = new Line3D(worldMatrix1 * (*it),
							worldMatrix1 * (*(it + 1)));
					Color tempColor = Color::GREEN;
					lineTemp->SetColor(m_selectColor);
					measure->AddLine(lineTemp);
				}
			}

			{
				SPolyLine * xPolyLine = secondLineSet->GetRefLine();
				vector<Vector3> & pntPoints = xPolyLine->GetPoints();
				unsigned int offset = secondLineSet->GetDataOffset();
				unsigned int dataLength = secondLineSet->GetDataLength();

				int countIt = 1;
				for (vector<Vector3>::iterator it = pntPoints.begin() + offset;
						it != pntPoints.begin() + offset + dataLength; it = it+2)
				{

					Line3D * lineTemp = new Line3D(worldMatrix2 * (*it),
							worldMatrix2 * (*(it + 1)));
					Color tempColor = Color::GREEN;
					lineTemp->SetColor(m_selectColor);
					measure->AddLine(lineTemp);
				}
			}

			Vector3 lineCenter = (minProject1 +minProject2)/2;

			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);
			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(), lineCenter);
			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//构造线

			Line3D * line1 = new Line3D(minProject1,minProject2);//测量线
			line1->SetColor(m_measureColor1);
			measure->AddLine(line1);
			Point *point1 = new Point(minProject1);
			point1->SetDrawType(1);
			point1->SetSize(0.8f);
			Point *point2 = new Point(minProject2);
			point2->SetDrawType(1);
			point2->SetSize(0.8f);
			measure->AddPoint(point1);
			measure->AddPoint(point2);

			Line3D * line2= new Line3D(lineCenter,pntInPlane);//引线
			line2->SetColor(gr);
			line2->SetName("MeasureImageLeader");
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line2->SetColor(m_leaderColor);
			measure->AddLine(line2);



			vector<float> textLength;
			string minDistanceTypeStr = language.GetCurrentTypeValue(
					"LineLineMinDistance");
			float minDstanceTextlength = language.GetCurrentTypeValueLength(
					"LineLineMinDistance");
			textLength.push_back(minDstanceTextlength);
			float maxTextLength = *std::max_element(textLength.begin(),textLength.end());
			vector<float> values;
			values.push_back(minDistance);
			string unit;
			MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
					,values,unit);
			string minDistanceStr = M3DTOOLS::floatToString(values.at(0))+unit;

            ComText* ctminDistance = new ComText();
            CText* tminDistance = new CText;
            tminDistance->SetText(minDistanceStr);
            tminDistance->SetLanguageType("LineLineMinDistance");
            ctminDistance->AddCText(tminDistance);
            measure->m_ComTexts.push_back(ctminDistance);
            
			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End(maxTextLength, 100.0);
			float rect2L = 450.0;

//			MeasureDisplayHelper::createShadowRect(shape2DSet, rect1Start, rect1End,
//					rect2L);

			MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start, rect1End,
					rect2L, m_measureColor1, wt, wt, blk, minDistanceTypeStr, minDistanceStr, fntSize, true);//minDistance

			ImageBoard* imageboard=NULL;
			MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
					(maxTextLength + rect2L)/100.0f, 1);
			measure->AddImage(imageboard);

			LOGI("line and ellipse distance END");

		}
	}
	else
	{
		LOGE("MeasureFactory::createLineTOLineDistance shape is NULL");
	}
	return measure;
}

Measure* MeasureFactory::createLineToFaceDistance(int firstLineId,
		int secondFaceId, Vector2& screenPnt, SceneManager* scene)
{
	Measure* measure = NULL;
	LOGI("createLineTOFaceAngle step1");
	InternationalManager language;
	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);

	Shape * firstShape = scene->GetShape(firstLineId);
	Shape * secondShape = scene->GetShape(secondFaceId);

	if (firstShape && secondShape && firstShape->GetType() == SHAPE_EDGE
			&& secondShape->GetType() == SHAPE_FACE)
	{
		LOGE("createLineTOFaceDistance step2");
		measure = new MeasureDistance;
		measure->SetMeasureType(Measure::MEASURE_TYPE_LINE_FACE_DISTANCE);

		Edge * firstEdge = (Edge*) firstShape;
		Face * secondFace = (Face *) secondShape;

		RefPolyLine * firstLineSet = firstEdge->GetLineData();
		Mesh * secondMeshData = (Mesh *) secondFace->GetData();

		GeometryAttribute * firstGeo = firstLineSet->GetGeoAttribute();
		GeometryAttribute * secondGeo = secondMeshData->GetGeoAttribute();

		if (firstGeo != NULL
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE
				&& secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
		{
			LOGI("createLineToFace step3");
			LineAttribute firstLineAttribute = *((LineAttribute *) firstGeo);
			PlaneFaceAttribute secondPlaneFaceAttribute =
					*((PlaneFaceAttribute *) secondGeo);

			Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstEdge);
			Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(
					secondFace);
			GeometryHelper::Transform(firstLineAttribute, worldMatrix1);
			GeometryHelper::Transform(secondPlaneFaceAttribute, worldMatrix2);

			float distance = -1;
			Vector3 projectPnt1, projectPnt2;
			MeasureCalculateHelper::LineFaceDistance(firstLineAttribute,
					secondPlaneFaceAttribute, distance, projectPnt1,
					projectPnt2);

			BoundingBox boundingBox = secondMeshData->GetBoundingBox();
			LOGI("boundingoxmin %s max %s",boundingBox.m_min.Tostring().c_str(),boundingBox.m_max.Tostring().c_str());



			Vector3 boxCenter = boundingBox.Center();
			GeometryHelper::Transform(boxCenter, worldMatrix2);



			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

			Vector3 line1Start = firstLineAttribute.GetStartPoint();
			Vector3 line1End = firstLineAttribute.GetEndPoint();
			Vector3 lineCenter = (line1Start + line1End) / 2;
			Vector3 center = (lineCenter + boxCenter) / 2;
			Vector3 line1Direction = (line1End-line1Start);
			Vector3 faceNormal = secondPlaneFaceAttribute.GetNormal();
			float markLngth;//表示平面的正方形长度
			vector<float> tempLength;
			tempLength.push_back(boundingBox.GetXLength());
			tempLength.push_back(boundingBox.GetYLength());
			tempLength.push_back(boundingBox.GetZLength());
			sort(tempLength.begin(), tempLength.end()); //排序
			LOGI("boxLength %f %f %f",tempLength[0],tempLength[1],tempLength[2]);
			if (tempLength[0] > 2.0f)
				markLngth = tempLength[0];
			else
				markLngth = tempLength[1];
			Color gr = Color::GRAY;
			Color wt = Color::WHITE;
			Color blk = Color::BLACK;
			Color grn = Color::GREEN;
			Color rd = Color::RED;
			Color blu = Color::BLUE;

			//构造投影平面


			Plane measurePlane(secondPlaneFaceAttribute.GetNormal(),secondPlaneFaceAttribute.GetOrigin());
			Vector3 lineCenterProject = measurePlane.Project(lineCenter);//直线中心点到测量面的投影点
			Plane projPlane(cameraRay.GetDirection(), (lineCenter+lineCenterProject)/2.0f);
			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());
			//构造线
			Line3D *line1 = new Line3D(lineCenter, lineCenterProject);//测量线
			line1->SetColor(m_measureColor1);

			Line3D * line2 = new Line3D((lineCenter+lineCenterProject)/2.0f, pntInPlane);//引线
			line2->SetName("MeasureImageLeader");
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line2->SetColor(m_leaderColor);
			Line3D *line3 = new Line3D(line1Start, line1End);//测量线
			line3->SetColor(m_selectColor);

			Line3D *lineEx = new Line3D(lineCenterProject, boxCenter);//辅助线
			lineEx->SetColor(m_exLineColor);
			lineEx->SetName("exLine");
			measure->AddLine(lineEx);

			Point * pointProject = new Point(lineCenterProject);
			pointProject->SetDrawType(1);
			pointProject->SetSize(0.8f);
			measure->AddPoint(pointProject);

			Point * pointLineCenter = new Point(lineCenter);
			pointLineCenter->SetDrawType(1);
			pointLineCenter->SetSize(0.8f);
			measure->AddPoint(pointLineCenter);



			//line2->SetStartArrow(1);

			//构造显示文本//构造矩形
			vector<float> textLength;
			string distanceStr;

			vector<Vector3> pointArray;
			if(distance>=0.0f)
			{
				vector<float> values;
				values.push_back(distance);
				string unit;
				MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
						,values,unit);
				distanceStr = M3DTOOLS::floatToString(values.at(0))+unit;
				//anglestr = M3DTOOLS::floatToString(distance);
//				MeasureDisplayHelper::CreateSelectFaceMark(pointArray,boxCenter,line1Direction,
//						faceNormal,markLngth/2.0);//平面标志
                
                
			}
			else
			{
				LOGI("createLineToFace step3 Error");
				delete measure;
				measure = NULL;
				return measure;//返回空 不能测量
//				distanceStr = language.GetCurrentTypeValue("InvalidMeasureObject");
//				Plane plane(faceNormal,boxCenter);
//				line1Direction = secondPlaneFaceAttribute.GetOrigin()-plane.Project(line1Start);
//				LOGI("line1Direction %s",line1Direction.Tostring().c_str());
//				MeasureDisplayHelper::CreateSelectFaceMark(pointArray,boxCenter,line1Direction,
//						faceNormal,markLngth/2.0);
			}

//			Line3D * m1 = new Line3D(pointArray[0],pointArray[1]);
//			m1->SetColor(rd);//平面标志
//			Line3D * m2 = new Line3D(pointArray[2],pointArray[3]);
//			m2->SetColor(rd);//平面标志
//			Line3D * m3 = new Line3D(pointArray[0],pointArray[2]);
//			m3->SetColor(rd);//平面标志
//			Line3D * m4 = new Line3D(pointArray[3],pointArray[1]);
//			m4->SetColor(rd);//平面标志

			string typestr = language.GetCurrentTypeValue("LineFaceDistance");
			float distanctTextLength =  language.GetCurrentTypeValueLength(
					"LineFaceDistance");
			textLength.push_back(distanctTextLength);
            ComText* ctDistance = new ComText();
            CText* tDistance = new CText;
            tDistance->SetText(distanceStr);
            tDistance->SetLanguageType("LineFaceDistance");
            ctDistance->AddCText(tDistance);
            measure->m_ComTexts.push_back(ctDistance);
			float maxTextLength = *std::max_element(textLength.begin(),textLength.end());

			//根据整个场景包围盒的大小来确定文字的大小
			float fontSize = scene->GetSceneBox().Length() / 20;

			measure->AddLine(line1);
			measure->AddLine(line2);
			measure->AddLine(line3);
//			measure->AddLine(m1);//平面标志
//			measure->AddLine(m2);//平面标志
//			measure->AddLine(m3);//平面标志
//			measure->AddLine(m4);//平面标志

			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End(maxTextLength, 100.0);
			float rect2L = 450.0;
//			MeasureDisplayHelper::createShadowRect(shape2DSet, rect1Start, rect1End,
//					rect2L);
			MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start, rect1End,
					rect2L, m_measureColor1, wt, wt, blk, typestr, distanceStr, fntSize, true);//distance

			ImageBoard* imageboard=NULL;
			MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
					(maxTextLength + rect2L)/100.0f, 1);
			measure->AddImage(imageboard);
		}
		else
		{
			LOGI("createLineToFace step3 Error");
			delete measure;
			measure = NULL;

		}
	}
	return measure;
}

Measure* MeasureFactory::createPntToFaceDistance(int firstPntId,
		int secondFaceId, Vector2& screenPnt, SceneManager* scene)
{
	Measure* measure = NULL;

	LOGI("createLineTOFaceAngle step1");
	InternationalManager language;
	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);

	Shape * firstShape = scene->GetShape(firstPntId);
	Shape * secondShape = scene->GetShape(secondFaceId);

	if (firstShape && secondShape && firstShape->GetType() == SHAPE_POINT_HANDLE
			&& secondShape->GetType() == SHAPE_FACE)
	{
		LOGE("createLineTOFaceAngle step2");
		measure = new MeasureDistance;
		measure->SetMeasureType(Measure::MEASURE_TYPE_PNT_FACE_DISTANCE);
		HandlerPoint * firstPnt = (HandlerPoint *) firstShape;
		Face * secondFace = (Face*) secondShape;
		Vector3 fCoordinate = firstPnt->GetPosition();
		Mesh * secondMeshData = (Mesh*) secondFace->GetData();

		GeometryAttribute * secondGeo = secondMeshData->GetGeoAttribute();
		if (secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
		{
			PlaneFaceAttribute secondPlaneFaceAttribute =
					*((PlaneFaceAttribute *) secondGeo);

			Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(
					secondFace);
			GeometryHelper::Transform(secondPlaneFaceAttribute, worldMatrix2);

			Plane planeTemp(secondPlaneFaceAttribute.GetNormal(),
					secondPlaneFaceAttribute.GetOrigin());
			Vector3 pntProject = planeTemp.Project(fCoordinate);

			float Length = (fCoordinate - pntProject).Length();
			Vector3 center = (fCoordinate + pntProject) / 2;

			//把平面当成mesh面 求出最近 最远距离
			Matrix3x4 inverseMat = worldMatrix2.Inverse();
			Vector3 modelVec = inverseMat * fCoordinate;
			float distance, maxDistance;
			Vector3 projectPntMesh,maxPoint;
			MeasureCalculateHelper::PntFaceDistance(modelVec, secondMeshData,
					distance, maxDistance, projectPntMesh,maxPoint);
			projectPntMesh = worldMatrix2 * projectPntMesh;
			maxPoint = worldMatrix2 * maxPoint;

			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(), center);

			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//構造線
			Line3D * line1 = new Line3D(fCoordinate, pntProject);//投影距离
			Color line1Color = Color::BLACK;
			line1->SetColor(m_measureColor1);

			Line3D *line2 = new Line3D(center, pntInPlane);//引线
			line2->SetName("MeasureImageLeader");
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line2->SetColor(m_leaderColor);
			Point * point1 = new Point(fCoordinate);//测量点
			point1->SetDrawType(1);
			point1->SetSize(0.8);

			Point * point2 = new Point(pntProject); //投影点
			point2->SetDrawType(1);
			point2->SetSize(0.8);

			Point * point3 = new Point(maxPoint); //最大点
			point3->SetDrawType(1);
			point3->SetSize(0.8);

			Point * point4 = new Point(projectPntMesh); //最小点
			point4->SetDrawType(1);
			point4->SetSize(0.8);


			Line3D * line3 = new Line3D(fCoordinate,projectPntMesh);//最小距离，到有边界的面
			Color line3Color = Color::RED;
			line3->SetColor(m_measureColor3);

			Line3D * line4 = new Line3D(fCoordinate,maxPoint);//最大距离，到有边界的面
			Color line4Color = Color::BLUE;
			line4->SetColor(m_measureColor2);

			Line3D* line5 = new Line3D(projectPntMesh,pntProject);//到边界面最小距离和投影距离的连线
			line5->SetColor(m_exLineColor);
			line5->SetName("exLine");

			float fontSize = scene->GetSceneBox().Length() / 20;

			vector<float> textLength;
			string typestr = language.GetCurrentTypeValue("PointFaceProjectDistance");
			float typeTextLength = language.GetCurrentTypeValueLength(
					"PointFaceProjectDistance");
			textLength.push_back(typeTextLength);

			string maxDistanceTypeStr = language.GetCurrentTypeValue("PointFaceMaxDistance");
			float maxDstanceTextlength = language.GetCurrentTypeValueLength(
					"PointFaceMaxDistance");
			textLength.push_back(maxDstanceTextlength);

			string minDistanceTypeStr = language.GetCurrentTypeValue("PointFaceMinDistance");
			float minDstanceTextlength = language.GetCurrentTypeValueLength(
					"PointFaceMinDistance");
			textLength.push_back(minDstanceTextlength);

			float maxTextLength = *std::max_element(textLength.begin(),textLength.end());



			measure->AddLine(line2);

			measure->AddLine(line5);
			measure->AddPoint(point1);

			vector<float> values;
			values.push_back(Length);
			values.push_back(maxDistance);
			values.push_back(distance);
			string unit;
			MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
					,values,unit);
			string LengthStr = M3DTOOLS::floatToString(values.at(0)) + unit;
			string maxDistanceStr = M3DTOOLS::floatToString(values.at(1))+unit;
			string distanceStr = M3DTOOLS::floatToString(values.at(2)) + unit;
            
            
			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End(maxTextLength, 100.0);
			float rect2L = 450.0;
			Color gr = Color::GRAY;
			Color wt = Color::WHITE;
			Color blk = Color::BLACK;
			Color grn = Color::GREEN;
			Color rd = Color::RED;
			Color blu = Color::BLUE;

			Vector2 rectStart(1.0, 1.0);
			Vector2 rectEnd(maxTextLength, 300.0);
//			MeasureDisplayHelper::createShadowRect(shape2DSet, rectStart, rectEnd,
//					rect2L);
			if(distance>0)
			{
				measure->AddLine(line1);
				measure->AddLine(line3);
				measure->AddLine(line4);
				measure->AddPoint(point2);
				measure->AddPoint(point3);
				measure->AddPoint(point4);
                //解决协同中文本读取
                ComText* ctLength = new ComText();
                CText* tLength = new CText;
                tLength->SetText(LengthStr);
                tLength->SetLanguageType("PointFaceProjectDistance");
                ctLength->AddCText(tLength);
                measure->m_ComTexts.push_back(ctLength);
                
                ComText* ctMaxDistance = new ComText();
                CText* tMaxDistance = new CText;
                tMaxDistance->SetText(maxDistanceStr);
                tMaxDistance->SetLanguageType("PointFaceMaxDistance");
                ctMaxDistance->AddCText(tMaxDistance);
                measure->m_ComTexts.push_back(ctMaxDistance);
                
                ComText* ctDistance = new ComText();
                CText* tDistance = new CText;
                tDistance->SetText(distanceStr);
                tDistance->SetLanguageType("PointFaceMinDistance");
                ctDistance->AddCText(tDistance);
                measure->m_ComTexts.push_back(ctDistance);
                
				MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start,
						rect1End, rect2L, m_measureColor1, wt, wt, blk, typestr,
						LengthStr, fntSize, true); //length

				Vector2 rect2Start(1.0, 100.0);
				Vector2 rect2End(maxTextLength, 200.0);
				MeasureDisplayHelper::createRectImage(shape2DSet, rect2Start,
						rect2End, rect2L, m_measureColor2, wt, wt, blk,
						maxDistanceTypeStr, maxDistanceStr, fntSize, true); //maxDistance

				Vector2 rect3Start(1.0, 200.0);
				Vector2 rect3End(maxTextLength, 300.0);
				MeasureDisplayHelper::createRectImage(shape2DSet, rect3Start,
						rect3End, rect2L, m_measureColor3, wt, wt, blk,
						minDistanceTypeStr, distanceStr, fntSize, true); //distance
				ImageBoard* imageboard = NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard,
						shape2DSet, pntInPlane,
						(maxTextLength + rect2L) / 100.0f, 3);
				measure->AddImage(imageboard);
			}
			else
			{
                ComText* ctDistance = new ComText();
                CText* tDistance = new CText;
                tDistance->SetText(distanceStr);
                tDistance->SetLanguageType("PointFaceMinDistance");
                ctDistance->AddCText(tDistance);
                measure->m_ComTexts.push_back(ctDistance);
                
				Vector2 rect3Start(1.0, 1.0);
				Vector2 rect3End(maxTextLength, 100.0);
				MeasureDisplayHelper::createRectImage(shape2DSet, rect3Start,
						rect3End, rect2L, m_measureColor1, wt, wt, blk,
						minDistanceTypeStr, distanceStr, fntSize, true); //distance
				ImageBoard* imageboard = NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard,
						shape2DSet, pntInPlane,
						(maxTextLength + rect2L) / 100.0f, 1);
				measure->AddImage(imageboard);
			}

		}
		else
		{
			//没有几何属性属性

			Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(
					secondFace);
			Matrix3x4 inverseMat = worldMatrix2.Inverse();
			Vector3 modelVec = inverseMat * fCoordinate;
			float distance, maxDistance;
			Vector3 projectPnt,maxPoint;
			MeasureCalculateHelper::PntFaceDistance(modelVec, secondMeshData,
					distance, maxDistance, projectPnt,maxPoint);
			projectPnt = worldMatrix2 * projectPnt;
			maxPoint = worldMatrix2 * maxPoint;

			Vector3 center = (fCoordinate + projectPnt) / 2;
			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(), projectPnt);

			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//構造線
			//Line3D * line1 = new Line3D(fCoordinate, projectPnt);
			Line3D *line2 = new Line3D(center, pntInPlane);
			line2->SetName("MeasureImageLeader");
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line2->SetColor(m_leaderColor);
			Point * point1 = new Point(fCoordinate);//测量点
			point1->SetDrawType(1);
			point1->SetSize(0.8);

			Point * point2 = new Point(projectPnt);//测量点
			point2->SetDrawType(1);
			point2->SetSize(0.8);

			Point * point3 = new Point(maxPoint);//测量点
			point3->SetDrawType(1);
			point3->SetSize(0.8);

			Line3D * line3 = new Line3D(fCoordinate, projectPnt); //最小距离，到有边界的面
			Color line3Color = Color::RED;
			line3->SetColor(m_measureColor1);

			Line3D * line4 = new Line3D(fCoordinate, maxPoint); //最小距离，到有边界的面
			Color line4Color = Color::BLUE;
						line4->SetColor(m_measureColor2);
			float fontSize = scene->GetSceneBox().Length() / 20;

			vector<float> textLength;

			string maxDistanceTypeStr = language.GetCurrentTypeValue("PointFaceMaxDistance");
			float maxDstanceTextlength = language.GetCurrentTypeValueLength(
					"PointFaceMaxDistance");
			textLength.push_back(maxDstanceTextlength);

			string minDistanceTypeStr = language.GetCurrentTypeValue("PointFaceMinDistance");
			float minDstanceTextlength = language.GetCurrentTypeValueLength(
					"PointFaceMinDistance");
			textLength.push_back(minDstanceTextlength);

			float maxTextLength = *std::max_element(textLength.begin(),textLength.end());

			//measure->AddLine(line1);
			measure->AddLine(line2);

			measure->AddPoint(point1);


			vector<float> values;
			values.push_back(distance);
			values.push_back(maxDistance);

			string unit;
			MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
					,values,unit);
			string distanceStr = M3DTOOLS::floatToString(values.at(0)) + unit;
			string maxDistanceStr = M3DTOOLS::floatToString(values.at(1))+unit;
            

			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End(maxTextLength, 100.0);
			float rect2L = 450.0;
			Color gr = Color::GRAY;
			Color wt = Color::WHITE;
			Color blk = Color::BLACK;
			Color grn = Color::GREEN;
			Color rd = Color::RED;
			Color blu = Color::BLUE;

			Vector2 rectStart(1.0, 1.0);
			Vector2 rectEnd(maxTextLength, 200.0);
//			MeasureDisplayHelper::createShadowRect(shape2DSet, rectStart, rectEnd,
//					rect2L);
			if(distance>0)
			{
                //解决协同中文本读取
                ComText* ctDistance = new ComText();
                CText* tDistance = new CText;
                tDistance->SetLanguageType("PointFaceMinDistance");
                tDistance->SetText(distanceStr);
                ctDistance->AddCText(tDistance);
                measure->m_ComTexts.push_back(ctDistance);
                
                ComText* ctMaxDistance = new ComText();
                CText* tMaxDistance = new CText;
                tMaxDistance->SetLanguageType("PointFaceMaxDistance");
                tMaxDistance->SetText(maxDistanceStr);
                ctMaxDistance->AddCText(tMaxDistance);
                measure->m_ComTexts.push_back(ctMaxDistance);
                
				measure->AddLine(line3);
				measure->AddLine(line4);
				measure->AddPoint(point2);
				measure->AddPoint(point3);
				MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start,
						rect1End, rect2L, m_measureColor1, wt, wt, blk,
						minDistanceTypeStr, distanceStr, fntSize, false);

				Vector2 rect2Start(1.0, 100.0);
				Vector2 rect2End(maxTextLength, 200.0);
				MeasureDisplayHelper::createRectImage(shape2DSet, rect2Start,
						rect2End, rect2L, m_measureColor2, wt, wt, blk,
						maxDistanceTypeStr, maxDistanceStr, fntSize, true);

				ImageBoard* imageboard = NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard,
						shape2DSet, pntInPlane,
						(maxTextLength + rect2L) / 100.0f, 2);
				measure->AddImage(imageboard);
			}
			else
			{
                //解决协同中文本读取
                ComText* ctDistance = new ComText();
                CText* tDistance = new CText;
                tDistance->SetLanguageType("PointFaceMinDistance");
                tDistance->SetText(distanceStr);
                ctDistance->AddCText(tDistance);
                measure->m_ComTexts.push_back(ctDistance);
                
				//measure->AddLine(line3);
				//measure->AddLine(line4);
				Vector2 rect3Start(1.0, 1.0);
				Vector2 rect3End(maxTextLength, 100.0);
				MeasureDisplayHelper::createRectImage(shape2DSet, rect3Start,
						rect3End, rect2L, m_measureColor1, wt, wt, blk,
						minDistanceTypeStr, distanceStr, fntSize, true); //distance
				ImageBoard* imageboard = NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard,
						shape2DSet, pntInPlane,
						(maxTextLength + rect2L) / 100.0f, 1);
				measure->AddImage(imageboard);
			}
		}

	}

	return measure;
}

Measure* MeasureFactory::createFaceToFaceDistance(int firstFaceId,
		int secondFaceId, Vector2& screenPnt, SceneManager* scene)
{
	Measure* measure = NULL;

	LOGI("createFaceTOFaceAngle step1");
	InternationalManager language;
	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);

	Shape * firstShape = scene->GetShape(firstFaceId);
	Shape * secondShape = scene->GetShape(secondFaceId);

	if (firstShape && secondShape && firstShape->GetType() == SHAPE_FACE
			&& secondShape->GetType() == SHAPE_FACE)
	{
		LOGE("createFaceTOFaceAngle step2");
		measure = new MeasureDistance;
		measure->SetMeasureType(Measure::MEASURE_TYPE_FACE_FACE_DISTANCE);

		Face * firstFace = (Face*) firstShape;
		Face * secondFace = (Face *) secondShape;

		Mesh * firstMeshData = (Mesh*) firstFace->GetData();
		Mesh * secondMeshData = (Mesh *) secondFace->GetData();

		GeometryAttribute * firstGeo = firstMeshData->GetGeoAttribute();
		GeometryAttribute * secondGeo = secondMeshData->GetGeoAttribute();

		if (firstGeo != NULL
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE
				&& secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
		{
			LOGI("createFaceTOFaceAngle step3");
			Color gr = Color::GRAY;
			Color wt = Color::WHITE;
			Color blk = Color::BLACK;
			Color grn = Color::GREEN;
			Color rd = Color::RED;
			Color blu = Color::BLUE;
			PlaneFaceAttribute firstPlaneFaceAttribute =
					*((PlaneFaceAttribute *) firstGeo);
			PlaneFaceAttribute secondPlaneFaceAttribute =
					*((PlaneFaceAttribute *) secondGeo);

			Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstFace);
			Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(
					secondFace);
			GeometryHelper::Transform(firstPlaneFaceAttribute, worldMatrix1);
			GeometryHelper::Transform(secondPlaneFaceAttribute, worldMatrix2);

			float distance;
			Vector3 projectPnt0, projectPnt1;
			MeasureCalculateHelper::FaceFaceDistance(firstPlaneFaceAttribute,
					secondPlaneFaceAttribute, distance, projectPnt0,
					projectPnt1);

			BoundingBox boundingBox1 = firstMeshData->GetBoundingBox();
			Vector3 boxCenter1 = boundingBox1.Center();
			GeometryHelper::Transform(boxCenter1, worldMatrix1);
			BoundingBox boundingBox2 = secondMeshData->GetBoundingBox();
			Vector3 boxCenter2 = boundingBox2.Center();
			GeometryHelper::Transform(boxCenter2, worldMatrix2);

			Vector3 center = (boxCenter1 + boxCenter2) / 2;
			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(), center);

			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//求面2包围盒中点到面1的投影点
			Plane face1Plane(firstPlaneFaceAttribute.GetNormal(),boxCenter1);
			Vector3 boxCenter2Project = face1Plane.Project(boxCenter2);

			//构造线
			Line3D *line1 = new Line3D(center, pntInPlane);//引线
			line1->SetName("MeasureImageLeader");
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line1->SetColor(m_leaderColor);
			Line3D * line2 = new Line3D(boxCenter1, boxCenter2);//辅助线
			line2->SetColor(m_measureColor1);

			Line3D * line3 = new Line3D(boxCenter2Project,boxCenter2);//测量线
			line3->SetColor(m_measureColor1);

			Point * point1 = new Point(boxCenter1);
			point1->SetSize(0.8f);
			point1->SetDrawType(1);

			Point * point2 = new Point(boxCenter2);
			point2->SetSize(0.8f);
			point2->SetDrawType(1);

			//line1->SetStartArrow(1);
			//	line2->SetStartArrow(1);

			//构造显示文本
			string distanceStr;
			if(distance>=0.0f)
			{
				vector<float> values;
				values.push_back(distance);
				string unit;
				MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
						,values,unit);
				distanceStr = M3DTOOLS::floatToString(values.at(0)) + unit;
				//anglestr = M3DTOOLS::floatToString(distance);
                
			}
			else
			{
				delete measure;
				measure = NULL;
				return measure;//不支持的化 返回空；
				//distanceStr = language.GetCurrentTypeValue("InvalidMeasureObject");
			}
			vector<float> textLength;
			string typestr = language.GetCurrentTypeValue("FaceFaceDistance");
			float typeStrTextLength =  language.GetCurrentTypeValueLength("FaceFaceDistance");
			textLength.push_back(typeStrTextLength);
            ComText* ctDistance = new ComText();
            CText* tDistance = new CText;
            tDistance->SetText(distanceStr);
            tDistance->SetLanguageType("FaceFaceDistance");
            ctDistance->AddCText(tDistance);
            measure->m_ComTexts.push_back(ctDistance);
			float maxTextLength = *std::max_element(textLength.begin(),textLength.end());
			//根据整个场景包围盒的大小来确定文字的大小
			float fontSize = scene->GetSceneBox().Length() / 20;

			measure->AddLine(line1);
			measure->AddLine(line2);
			measure->AddPoint(point1);
			measure->AddPoint(point2);



			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End(typeStrTextLength, 100.0);
			float rect2L = 450.0;


//			MeasureDisplayHelper::createShadowRect(shape2DSet, rect1Start, rect1End,
//					rect2L);
			MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start, rect1End,
					rect2L, m_measureColor1, wt, wt, blk, typestr, distanceStr, fntSize, true);

			ImageBoard* imageboard=NULL;
			MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
					(maxTextLength + rect2L)/100.0f, 1);
			measure->AddImage(imageboard);
		}
		else
		{
			delete measure;
			measure = NULL;
		}
	}
	return measure;
}

Measure* MeasureFactory::createLineToLineAngle(int firstLineId,
		int secondLineId, Vector2& screenPnt, SceneManager* scene)
{
	Measure* measure = NULL;
	LOGI("createLineTOLineAngle step1");
	InternationalManager language;
	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);

	Shape * firstShape = scene->GetShape(firstLineId);
	Shape * secondShape = scene->GetShape(secondLineId);

	if (firstShape && secondShape && firstShape->GetType() == SHAPE_EDGE
			&& secondShape->GetType() == SHAPE_EDGE)
	{
		LOGE("createLineTOLineAngle step2");
		measure = new MeasureAngle;
		measure->SetMeasureType(Measure::MEASURE_TYPE_LINE_LINE_ANGLE);

		Edge * firstEdge = (Edge*) firstShape;
		Edge * secondEdge = (Edge *) secondShape;

		RefPolyLine * firstLineSet = firstEdge->GetLineData();
		RefPolyLine * secondLineSet = secondEdge->GetLineData();

		GeometryAttribute * firstGeo = firstLineSet->GetGeoAttribute();
		GeometryAttribute * secondGeo = secondLineSet->GetGeoAttribute();

		Color gr = Color::GRAY;
		Color wt = Color::WHITE;
		Color blk = Color::BLACK;
		Color grn = Color::GREEN;
		Color rd = Color::RED;
		Color blu = Color::BLUE;
		if (firstGeo != NULL
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE
				&& secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE)
		{
			LOGI("createLineTOLineAngle step3");
			LineAttribute firstLineAttribute = *((LineAttribute *) firstGeo);
			LineAttribute secondLineAttribute = *((LineAttribute *) secondGeo);

			Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstEdge);
			Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(
					secondEdge);
			GeometryHelper::Transform(firstLineAttribute, worldMatrix1);
			GeometryHelper::Transform(secondLineAttribute, worldMatrix2);

			float angle;
			MeasureCalculateHelper::LineLineAngle(firstLineAttribute,
					secondLineAttribute, angle);
			//将夹角转换到0-π/2
			//if (angle > 90.0)
				//angle = 180.0 - angle;

			LOGI("line line degree is %f", angle);
			//angle = M_RADTODEG * angle;
			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

			Vector3 line1Start = firstLineAttribute.GetStartPoint();
			Vector3 line1End = firstLineAttribute.GetEndPoint();
			Vector3 line1Vector = line1End-line1Start;
			//Vector3 line1Center = (line1Start+line1End)/2;
			Vector3 line2Start = secondLineAttribute.GetStartPoint();
			Vector3 line2End = secondLineAttribute.GetEndPoint();
			Vector3 line2Center = (line2Start + line2End) / 2;
			//平移线1，使其与线2相交
			Vector3 removeLine1End = line2Start+line1Vector;

			//Vector3 center = (line1Center+line2Center)/2;

			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(), line1Start);

			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//构造线


			Line3D * line2 = new Line3D(line1Start, line1End);//测量线1
			Color lineColor = Color::GREEN;
			line2->SetColor(m_selectColor);
			Line3D * line3 = new Line3D(line2Start, line2End);//测量线2
			//Color lineColor = Color::GREEN;
			line3->SetColor(m_selectColor);
			Line3D * line4 = new Line3D(line2Start,removeLine1End);//线1平移线
			Color line4Color = Color::YELLOW;
			line4->SetColor(m_exLineColor);
			line4->SetName("exLine");

			Line3D * line5 = new Line3D(line1Start,line2Start);//延长线
		//	Color line4Color = Color::BLACK;
			line5->SetColor(m_exLineColor);
			line5->SetName("exLine");

			Line3D * line6 = new Line3D(line1End,removeLine1End);//延长线
			//Color line4Color = Color::BLACK;
			line6->SetColor(m_exLineColor);
			line6->SetName("exLine");

			vector<Vector3> linesTemp;
			MeasureDisplayHelper::CreateAngleMark(line2Start,removeLine1End,line2Start,line2End,
					angle,linesTemp);
			Line3D *line1;
			if(linesTemp.size()>0)
			{
				for (int i = 0; i < linesTemp.size() - 1; i++)
				{
					Line3D * lineTemp = new Line3D(linesTemp[i],
							linesTemp[i + 1]);
					lineTemp->SetColor(m_measureColor1);
					measure->AddLine(lineTemp);
				}
				if ((angle>0.0f && angle < 90.0f) || (angle > 90.0f && angle<180.0f))
				{
					line1 = new Line3D(
							(linesTemp[linesTemp.size() / 2]
									+ linesTemp[linesTemp.size() / 2 - 1])
									/ 2.0f, pntInPlane); //引线
				}
				else if (angle - 90.0f < 0.0000001f
						&& angle - 90.0f > -0.0000001f)
				{
					line1 = new Line3D(linesTemp[1], pntInPlane); //引线
				}
				else
				{
					line1 = new Line3D(line2Start, pntInPlane);//引线
				}
			}
			else
			{
				line1 = new Line3D(line2Start, pntInPlane);//引线
			}

			line1->SetName("MeasureImageLeader");
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line1->SetColor(m_leaderColor);

			//构造显示文本

			vector<float> textLength;
			string typestr = language.GetCurrentTypeValue("LineLineAngle");
			float typeStrTextLength =  language.GetCurrentTypeValueLength("LineLineAngle");
			textLength.push_back(typeStrTextLength);

			float maxTextLength = *std::max_element(textLength.begin(),textLength.end());
			string anglestr = M3DTOOLS::floatToString(angle);

            ComText* ct = new ComText();
            CText* t = new CText;
            t->SetText(anglestr);
            t->SetLanguageType("LineLineAngle");
            ct->AddCText(t);
            measure->m_ComTexts.push_back(ct);
            
			//根据整个场景包围盒的大小来确定文字的大小
			float fontSize = scene->GetSceneBox().Length() / 20;

			measure->AddLine(line1);
			measure->AddLine(line2);
			measure->AddLine(line3);
			measure->AddLine(line4);
			//measure->AddLine(line5);
			//measure->AddLine(line6);

			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End(maxTextLength, 100.0f);
			float rect2L = 450.0;

//			MeasureDisplayHelper::createShadowRect(shape2DSet, rect1Start, rect1End,
//					rect2L);
			MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start, rect1End,
					rect2L, m_measureColor1, wt, wt, blk, typestr, anglestr, fntSize, true);

			ImageBoard* imageboard=NULL;
			MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
					(maxTextLength + rect2L)/100.0f, 1);
			measure->AddImage(imageboard);
		}
		else
		{
			delete measure;
			measure = NULL;
		}

	}

	return measure;
}

Measure* MeasureFactory::createFaceToFaceAngle(int firstFaceId,
		int secondFaceId, Vector2& screenPnt, SceneManager* scene)
{
	Measure* measure = NULL;
	InternationalManager language;
	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);

	LOGI("createFaceTOFaceAngle step1");

	Shape * firstShape = scene->GetShape(firstFaceId);
	Shape * secondShape = scene->GetShape(secondFaceId);

	if (firstShape && secondShape && firstShape->GetType() == SHAPE_FACE
			&& secondShape->GetType() == SHAPE_FACE)
	{
		LOGE("createFaceTOFaceAngle step2");
		measure = new MeasureAngle;
		measure->SetMeasureType(Measure::MEASURE_TYPE_FACE_FACE_ANGLE);

		Face * firstFace = (Face*) firstShape;
		Face * secondFace = (Face *) secondShape;

		Mesh * firstMeshData = (Mesh*) firstFace->GetData();
		Mesh * secondMeshData = (Mesh *) secondFace->GetData();

		GeometryAttribute * firstGeo = firstMeshData->GetGeoAttribute();
		GeometryAttribute * secondGeo = secondMeshData->GetGeoAttribute();

		if (firstGeo != NULL
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE
				&& secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
		{
			LOGI("createFaceTOFaceAngle step3");
			PlaneFaceAttribute firstPlaneFaceAttribute =
					*((PlaneFaceAttribute *) firstGeo);
			PlaneFaceAttribute secondPlaneFaceAttribute =
					*((PlaneFaceAttribute *) secondGeo);

			Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstFace);
			Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(
					secondFace);
			GeometryHelper::Transform(firstPlaneFaceAttribute, worldMatrix1);
			GeometryHelper::Transform(secondPlaneFaceAttribute, worldMatrix2);

			float angle;
			MeasureCalculateHelper::FaceFaceAngle(firstPlaneFaceAttribute,
					secondPlaneFaceAttribute, angle);

			BoundingBox boundingBox1 = firstMeshData->GetBoundingBox();
			Vector3 boxCenter1 = boundingBox1.Center();
			GeometryHelper::Transform(boxCenter1, worldMatrix1);
			BoundingBox boundingBox2 = secondMeshData->GetBoundingBox();
			Vector3 boxCenter2 = boundingBox2.Center();
			GeometryHelper::Transform(boxCenter2, worldMatrix2);

			Vector3 center = (boxCenter1 + boxCenter2) / 2;

			//angle = M_RADTODEG * angle;
			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

			//Vector3 face1Origin = firstPlaneFaceAttribute.GetOrigin();
			//Vector3 face2Origin = secondPlaneFaceAttribute.GetOrigin();

			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(), center);

			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//构造线
			Line3D *line1 = new Line3D(center, pntInPlane);
			line1->SetName("MeasureImageLeader");
			Line3D * line2 = new Line3D(boxCenter1, boxCenter2);
			line2->SetColor(m_measureColor1);
			Point * point1 = new Point(boxCenter1);
			point1->SetDrawType(1);
			point1->SetSize(0.8f);
			measure->AddPoint(point1);

			Point * point2 = new Point(boxCenter2);
			point2->SetDrawType(1);
			point2->SetSize(0.8f);
			measure->AddPoint(point2);


			//line1->SetStartArrow(1);
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line1->SetColor(m_leaderColor);
			//	line2->SetStartArrow(1);

			//构造显示文本
			string anglestr = M3DTOOLS::floatToString(angle);


			vector<float> textLength;
			string typestr = language.GetCurrentTypeValue("FaceFaceAngle");
			float typeStrTextLength =  language.GetCurrentTypeValueLength("FaceFaceAngle");
			textLength.push_back(typeStrTextLength);
			float maxTextLength = *std::max_element(textLength.begin(),textLength.end());

            ComText* ct = new ComText();
            CText* t = new CText;
            t->SetText(anglestr);
            t->SetLanguageType("FaceFaceAngle");
            ct->AddCText(t);
            measure->m_ComTexts.push_back(ct);
			//根据整个场景包围盒的大小来确定文字的大小
			float fontSize = scene->GetSceneBox().Length() / 20;

			measure->AddLine(line1);
			measure->AddLine(line2);
			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End(maxTextLength, 100.0);
			float rect2L = 450.0;
			Color gr = Color::GRAY;
			Color wt = Color::WHITE;
			Color blk = Color::BLACK;
			Color grn = Color::GREEN;
			Color rd = Color::RED;
			Color blu = Color::BLUE;

//			MeasureDisplayHelper::createShadowRect(shape2DSet, rect1Start, rect1End,
//					rect2L);
			MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start, rect1End,
					rect2L, m_measureColor1, wt, wt, blk, typestr, anglestr, fntSize, true);
			ImageBoard* imageboard=NULL;
			MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
					(maxTextLength + rect2L)/100.0f, 1);
			measure->AddImage(imageboard);
		}
		else
		{
			delete measure;
			measure = NULL;
		}
	}
	return measure;
}
Measure* MeasureFactory::createLineToFaceAngle(int firstLineId,
		int secondFaceId, Vector2& screenPnt, SceneManager* scene)
{
	Measure* measure = NULL;
	LOGI("createLineTOFaceAngle step1");
	InternationalManager language;
	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);

	Shape * firstShape = scene->GetShape(firstLineId);
	Shape * secondShape = scene->GetShape(secondFaceId);

	if (firstShape && secondShape && firstShape->GetType() == SHAPE_EDGE
			&& secondShape->GetType() == SHAPE_FACE)
	{
		LOGE("createLineTOFaceAngle step2");
		measure = new MeasureAngle;
		measure->SetMeasureType(Measure::MEASURE_TYPE_LINE_FACE_ANGLE);

		Edge * firstEdge = (Edge*) firstShape;
		Face * secondFace = (Face *) secondShape;

		RefPolyLine * firstLineSet = firstEdge->GetLineData();
		Mesh * secondMeshData = (Mesh *) secondFace->GetData();

		GeometryAttribute * firstGeo = firstLineSet->GetGeoAttribute();
		GeometryAttribute * secondGeo = secondMeshData->GetGeoAttribute();

		Color gr = Color::GRAY;
		Color wt = Color::WHITE;
		Color blk = Color::BLACK;
		Color grn = Color::GREEN;
		Color rd = Color::RED;
		Color blu = Color::BLUE;
		if (firstGeo != NULL
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE
				&& secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
		{
			LOGI("createFaceTOFaceAngle step3");
			LineAttribute firstLineAttribute = *((LineAttribute *) firstGeo);
			PlaneFaceAttribute secondPlaneFaceAttribute =
					*((PlaneFaceAttribute *) secondGeo);

			Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstEdge);
			Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(
					secondFace);
			GeometryHelper::Transform(firstLineAttribute, worldMatrix1);
			GeometryHelper::Transform(secondPlaneFaceAttribute, worldMatrix2);

			float angle;
			MeasureCalculateHelper::LineFaceAngle(firstLineAttribute,
					secondPlaneFaceAttribute, angle);

			BoundingBox boundingBox = secondMeshData->GetBoundingBox();
			Vector3 boxCenter = boundingBox.Center();
			GeometryHelper::Transform(boxCenter, worldMatrix2);

			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

			Vector3 line1Start = firstLineAttribute.GetStartPoint();
			Vector3 line1End = firstLineAttribute.GetEndPoint();
			Vector3 lineCenter = (line1Start + line1End) / 2;
			Vector3 center = (lineCenter + boxCenter) / 2;

			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(), line1Start);

			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//构造线
			Line3D *line1 = new Line3D(lineCenter, boxCenter);
			line1->SetColor(m_measureColor1);
			Line3D * line2 = new Line3D(center, pntInPlane);//文本引线
			line2->SetName("MeasureImageLeader");
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line2->SetColor(m_leaderColor);
			Line3D *line3 = new Line3D(line1Start, line1End);
			line3->SetColor(m_selectColor);
			Point * point1 = new Point(lineCenter);
			point1->SetDrawType(1);
			point1->SetSize(0.8f);
			measure->AddPoint(point1);

			Point * point2 = new Point(boxCenter);
			point2->SetDrawType(1);
			point2->SetSize(0.8f);
			measure->AddPoint(point2);


			//构造显示文本
			string anglestr = M3DTOOLS::floatToString(angle);

			vector<float> textLength;
			string typestr = language.GetCurrentTypeValue("LineFaceAngle");
			float typeStrTextLength =  language.GetCurrentTypeValueLength("LineFaceAngle");
			textLength.push_back(typeStrTextLength);
			float maxTextLength = *std::max_element(textLength.begin(),textLength.end());
			//根据整个场景包围盒的大小来确定文字的大小
			float fontSize = scene->GetSceneBox().Length() / 20;

            ComText* ct = new ComText();
            CText* t = new CText;
            t->SetText(anglestr);
            t->SetLanguageType("LineFaceAngle");
            ct->AddCText(t);
            measure->m_ComTexts.push_back(ct);
            
			measure->AddLine(line1);
			measure->AddLine(line2);
			measure->AddLine(line3);

			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End(typeStrTextLength, 100.0);
			float rect2L = 450.0;

//			MeasureDisplayHelper::createShadowRect(shape2DSet, rect1Start, rect1End,
//					rect2L);
			MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start, rect1End,
					rect2L, m_measureColor1, wt, wt, blk, typestr, anglestr, fntSize, true);

			ImageBoard* imageboard=NULL;
			MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
					(maxTextLength + rect2L)/100.0f, 1);
			measure->AddImage(imageboard);

		}
		else
		{
			delete measure;
			measure = NULL;
		}
	}
	return measure;
}

Measure* MeasureFactory::createPntProperty(int shapeId, Vector2& screenPnt,
		SceneManager* scene)
{
	Measure* measure = NULL;

	Shape* firstShape = scene->GetShape(shapeId);

	if (firstShape && firstShape->GetType() == SHAPE_POINT_HANDLE)
	{
		LOGI("C++ createPntProperty");
		HandlerPoint * firstPnt = (HandlerPoint *) firstShape;

		measure = new MeasureProperty;

		measure->SetMeasureType(Measure::MEASURE_TYPE_PNT_COORD);

		Vector3 fCoordinate = firstPnt->GetPosition();

		Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
				screenPnt.m_x, screenPnt.m_y);

		Plane projPlane(cameraRay.GetDirection(), fCoordinate);
		Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

		Line3D* line1 = new Line3D(fCoordinate, pntInPlane);
		line1->SetName("MeasureImageLeader");
		//Color oriangle(1.0f,0.44f,0.0f) ;
		line1->SetColor(m_leaderColor);

		Point * point1 = new Point(fCoordinate);
		Color point1Color = Color::GREEN;
		point1->SetColor(point1Color);
		point1->SetDrawType(1);
		point1->SetSize(0.8);

		//构造显示文本

		string xStr = M3DTOOLS::floatToString(fCoordinate.m_x) + ", ";
		string yStr = M3DTOOLS::floatToString(fCoordinate.m_y) + ", ";
		string zStr = M3DTOOLS::floatToString(fCoordinate.m_z) ;
		string typestr = "点坐标";
		string coordinateStr = xStr + yStr + zStr;
		//根据整个场景包围盒的大小来确定文字的大小
		float fontSize = scene->GetSceneBox().Length() / 2;
		LOGI("point property fontSize %f",fontSize);
		//将线加入测量对象中
		measure->AddLine(line1);
		measure->AddPoint(point1);


		Shape2DSet * shape2DSet = new Shape2DSet();
		float fntSize = 40;
		Vector2 rect1Start(1.0, 1.0);
		Vector2 rect1End((typestr.length()-1)*fntSize/2+fntSize, fntSize*2.5f);
		float rect2L = fntSize*coordinateStr.length()/2.0+fntSize/2;
		Color gr = Color::GRAY;
		Color wt = Color::WHITE;
		Color blk = Color::BLACK;
		Color grn = Color::GREEN;
		Color rd = Color::RED;
		Color blu = Color::BLUE;

//		MeasureDisplayHelper::createShadowRect(shape2DSet, rect1Start, rect1End,
//				rect2L);
		MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start, rect1End,
				rect2L, gr, wt, wt, blk, typestr, coordinateStr, fntSize, true);

		ImageBoard* imageboard;
		MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
						((typestr.length()-1)*fntSize/2+fntSize+rect2L)/100.0, fntSize*2.5f/100.0);
		measure->AddImage(imageboard);

	}
	return measure;
}

Measure* MeasureFactory::createLineProperty(int shapeId, Vector2& screenPnt,
		SceneManager* scene)
{
	Measure* measure = NULL;

	Shape* firstShape = scene->GetShape(shapeId);
	//如果的shape对象存在
	if (firstShape && firstShape->GetType() == SHAPE_EDGE)
	{
		Edge* edge = (Edge*) firstShape;

		measure = new MeasureProperty;

		measure->SetMeasureType(Measure::MEASURE_TYPE_LINE_LENGTH);
		//得到边界线数据
		RefPolyLine * m_lineSet = edge->GetLineData();

		GeometryAttribute* geo = m_lineSet->GetGeoAttribute();
		///暂时仅支持，点到直线距离的测量
		if (geo != NULL && geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE)
		{
			LineAttribute lineAttribute = *((LineAttribute*) geo);

			Matrix3x4 worldMatrix = ShapeHelper::GetShapeWorldMatrix(edge);
			GeometryHelper::Transform(lineAttribute, worldMatrix);

			Vector3 pntStartPoint = lineAttribute.GetStartPoint();
			Vector3 pntEndPoint = lineAttribute.GetEndPoint();
			Vector3 lineCenter = (pntStartPoint + pntEndPoint) / 2;

			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(), lineCenter);

			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//构造线
			Line3D* line1 = new Line3D(pntStartPoint, pntEndPoint);
			Color line1Color = Color::GREEN;
			line1->SetColor(line1Color);
			Line3D* line2 = new Line3D(lineCenter, pntInPlane);
			Color line2Color = Color::GRAY;
			//line2->SetColor(line2Color);
			line2->SetName("MeasureImageLeader");
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line2->SetColor(m_leaderColor);
			float length = (pntStartPoint - pntEndPoint).Length();

			//构造显示文本
			string lengthstr = M3DTOOLS::floatToString(length);

			string typestr = "长度";

			//根据整个场景包围盒的大小来确定文字的大小
			float fontSize = scene->GetSceneBox().Length() / 20;

			//将线加入测量对象中
			measure->AddLine(line1);
			measure->AddLine(line2);
			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End(180.0, 100.0);
			float rect2L = 450.0;
			Color gr = Color::GRAY;
			Color wt = Color::WHITE;
			Color blk = Color::BLACK;
			Color grn = Color::GREEN;
			Color rd = Color::RED;
			Color blu = Color::BLUE;

//			MeasureDisplayHelper::createShadowRect(shape2DSet, rect1Start, rect1End,
//					rect2L);
			MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start, rect1End,
					rect2L, gr, wt, wt, blk, typestr, lengthstr, fntSize, true);

			ImageBoard* imageboard=NULL;
			MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
							6, 1);
			measure->AddImage(imageboard);
		}
	}

	return measure;
}

Measure* MeasureFactory::createCircleProperty(int shapeId, Vector2& screenPnt,
		SceneManager* scene)
{
	Measure* measure = NULL;

	Shape* firstShape = scene->GetShape(shapeId);
	//如果的shape对象存在
	if (firstShape && firstShape->GetType() == SHAPE_EDGE)
	{
		Edge* edge = (Edge*) firstShape;

		measure = new MeasureProperty;

		measure->SetMeasureType(Measure::MEASURE_TYPE_CRICLE_PROPERTY);
		//得到边界线数据
		RefPolyLine * m_lineSet = edge->GetLineData();

		GeometryAttribute* geo = m_lineSet->GetGeoAttribute();
		///暂时仅支持，点到直线距离的测量
		if (geo != NULL && geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_ELLIPSE)
		{
			EllipseAttribute ellipseAttribute = *((EllipseAttribute*) geo);

			Matrix3x4 worldMatrix = ShapeHelper::GetShapeWorldMatrix(edge);
			GeometryHelper::Transform(ellipseAttribute, worldMatrix);
			Vector3 pntStartPoint = ellipseAttribute.GetStartPoint(); //!< Line起点
			Vector3 pntEndPoint = ellipseAttribute.GetEndPoint(); //!< Line终点
			Vector3 circleCenter = ellipseAttribute.GetCenterPoint(); //圆心
			float radius = ellipseAttribute.GetMajorRadius(); //长半轴 当 半径
			Vector3 xVector, yVector, zVector;
			ellipseAttribute.GetXYZDir(xVector, yVector, zVector);

			//求弧长
			float arcLength = 0; //弧长
			Vector3 startVector = pntStartPoint - circleCenter;
			Vector3 endVector = pntEndPoint - circleCenter;
			Vector3 crossVector = startVector.CrossProduct(endVector);
			float dotValue = crossVector.DotProduct(zVector);
			float alpha =
					acos(
							startVector.DotProduct(
									endVector
											/ (startVector.Length()
													* endVector.Length())));
			if (pntStartPoint == pntEndPoint)
			{
				//说明是个完成的圆
				arcLength = 2 * M_PI * radius;
			}
			else if (dotValue > 0)
			{
				//说明与z方向同向
				arcLength = alpha * radius;
			}
			else
			{
				//说明方向相反
				arcLength = (2 * M_PI - alpha) * radius;
			}

			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);
			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(), circleCenter);
			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//构造选中的圆弧
			SPolyLine * xPolyLine = m_lineSet->GetRefLine();
			vector<Vector3> &pntPoints = xPolyLine->GetPoints();
			unsigned int offset = m_lineSet->GetDataOffset();
			unsigned int dataLength = m_lineSet->GetDataLength();

			//	int countIt =1;
			for (vector<Vector3>::iterator it = pntPoints.begin() + offset;
					it != pntPoints.begin() + offset + dataLength ; it=it+2)
			{

				//Vector3 startPnt =  worldMatrix*( *it) ;
				//Vector3 endPnt = worldMatrix*(*(it+1));
				Line3D * lineTemp = new Line3D(worldMatrix * (*it),
						worldMatrix * (*(it + 1)));
				Color LineTempColor = Color::GREEN;
				lineTemp->SetColor(LineTempColor);
				measure->AddLine(lineTemp);
				//LOGI("Add line number: %d",countIt);
			}
			//构造线
			Line3D* line1 = new Line3D(circleCenter, pntInPlane);
			line1->SetName("MeasureImageLeader");
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line1->SetColor(m_leaderColor);
			Line3D* line2 = new Line3D(circleCenter,pntStartPoint);//半径线
			Color line2Color = Color::RED;
			line2->SetColor(line2Color);
			Point * centerPoint = new Point(circleCenter);
			centerPoint->SetDrawType(1);
			centerPoint->SetSize(0.5f);

			//构造显示文本
			string radiusStr = M3DTOOLS::floatToString(radius);
			string centerStrX = M3DTOOLS::floatToString(circleCenter.m_x);
			string centerStrY = M3DTOOLS::floatToString(circleCenter.m_y);
			string centerStrZ = M3DTOOLS::floatToString(circleCenter.m_z);
			string centerStr = centerStrX + "," + centerStrY + "," + centerStrZ;

			string arcLengthStr = M3DTOOLS::floatToString(arcLength);
			string cstr = "圆心";

			//string circlestr = radiusStr + centerStr;

			//根据整个场景包围盒的大小来确定文字的大小
			float fontSize = scene->GetSceneBox().Length() / 20;

			//将线加入测量对象中
			measure->AddLine(line1);
			measure->AddLine(line2);
			measure->AddPoint(centerPoint);
			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End(180.0, 100.0);
			float rect2L = 450.0;
			Color gr = Color::GRAY;
			Color wt = Color::WHITE;
			Color blk = Color::BLACK;
			Color grn = Color::GREEN;
			Color rd = Color::RED;
			Color blu = Color::BLUE;

			Vector2 rectStart(1.0, 1.0);
			Vector2 rectEnd(180.0, 300.0);
//			MeasureDisplayHelper::createShadowRect(shape2DSet, rectStart, rectEnd,
//					rect2L);

			MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start, rect1End,
					rect2L, gr, wt, wt, blk, cstr, centerStr, fntSize, false);

			Vector2 rect2Start(1.0, 100.0);
			Vector2 rect2End(180.0, 200.0);
			MeasureDisplayHelper::createRectImage(shape2DSet, rect2Start, rect2End,
					rect2L, rd, wt, wt, blk, "半径", radiusStr, fntSize, false);

			Vector2 rect3Start(1.0, 200.0);
			Vector2 rect3End(180.0, 300.0);
			MeasureDisplayHelper::createRectImage(shape2DSet, rect3Start, rect3End,
					rect2L, grn, wt, blk, blk, "弧长", arcLengthStr, fntSize,
					true);

			ImageBoard* imageboard=NULL;
			MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
							6, 3);
			measure->AddImage(imageboard);
		}
	}

	return measure;
}

Measure* MeasureFactory::createFaceProperty(int shapeId, Vector2& screenPnt,
		SceneManager* scene)
{
	LOGI("face measure");
	Measure* measure = NULL;

	Shape* firstShape = scene->GetShape(shapeId);
	//如果的shape对象存在
	if (firstShape && firstShape->GetType() == SHAPE_FACE)
	{
		Face* face = (Face*) firstShape;

		measure = new MeasureProperty;

		measure->SetMeasureType(Measure::MEASURE_TYPE_FACE_PROPERTY);
		//得到边界线数据

		Mesh* meshData = (Mesh*) face->GetData();
		GeometryAttribute* geo = meshData->GetGeoAttribute();

		//计算面积
		float area = 0.0f;
		MeasureCalculateHelper::faceArea(meshData, area);

		///暂时仅支持，点到直线距离的测量
		if (geo != NULL && geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
		{
			LOGI("planceface measure");
			PlaneFaceAttribute planeFaceAttribute = *((PlaneFaceAttribute*) geo);

			Matrix3x4 worldMatrix = ShapeHelper::GetShapeWorldMatrix(face);
			GeometryHelper::Transform(planeFaceAttribute, worldMatrix);

			BoundingBox boundingBox = meshData->GetBoundingBox();
			Vector3 boxCenter = boundingBox.Center();
			GeometryHelper::Transform(boxCenter, worldMatrix);

			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(), boxCenter);
			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//构造线
			Line3D* line1 = new Line3D(boxCenter, pntInPlane);
			line1->SetName("MeasureImageLeader");
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line1->SetColor(m_leaderColor);

			//float length = (pntStartPoint - pntEndPoint).Length();

			//构造显示文本
			Vector3 planeNormal = planeFaceAttribute.GetNormal();

			string areastr = M3DTOOLS::floatToString(area);
			//	string planestr = normalstr + originstr;
			//根据整个场景包围盒的大小来确定文字的大小
			float fontSize = scene->GetSceneBox().Length() / 20;

			//将线加入测量对象中
			measure->AddLine(line1);
			//measure->AddLine(line2);

			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End(180.0, 100.0);
			float rect2L = 450.0;
			Color gr = Color::GRAY;
			Color wt = Color::WHITE;
			Color blk = Color::BLACK;
			Color grn = Color::GREEN;
			Color rd = Color::RED;
			Color blu = Color::BLUE;

//			MeasureDisplayHelper::createShadowRect(shape2DSet, rect1Start, rect1End,
//					rect2L);

			MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start, rect1End,
					rect2L, gr, wt, wt, blk, "面积", areastr, fntSize, true);

			ImageBoard* imageboard=NULL;
			MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
							6, 1);
			measure->AddImage(imageboard);
		}
		else if (geo != NULL
				&& geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_REVOLUTIONFACE)
		{
			RevolutionFaceAttribute revolutionFaceAttribute =
					*((RevolutionFaceAttribute*) geo);

			Matrix3x4 worldMatrix = ShapeHelper::GetShapeWorldMatrix(face);
			GeometryHelper::Transform(revolutionFaceAttribute, worldMatrix);

			BoundingBox boundingBox = meshData->GetBoundingBox();
			Vector3 boxCenter = boundingBox.Center();
			GeometryHelper::Transform(boxCenter, worldMatrix);

			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(), boxCenter);
			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//构造线
			Line3D* line1 = new Line3D(boxCenter, pntInPlane);
			line1->SetName("MeasureImageLeader");
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line1->SetColor(m_leaderColor);
			//float length = (pntStartPoint - pntEndPoint).Length();

			//构造显示文本
			float revolutionRadius = revolutionFaceAttribute.GetRadius();

			string areastr = M3DTOOLS::floatToString(area);

			//根据整个场景包围盒的大小来确定文字的大小
			float fontSize = scene->GetSceneBox().Length() / 20;

			//将线加入测量对象中
			measure->AddLine(line1);
			//measure->AddLine(line2);

			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End(200.0, 100.0);
			float rect2L = 450.0;
			Color gr = Color::GRAY;
			Color wt = Color::WHITE;
			Color blk = Color::BLACK;
			Color grn = Color::GREEN;
			Color rd = Color::RED;
			Color blu = Color::BLUE;

//			MeasureDisplayHelper::createShadowRect(shape2DSet, rect1Start, rect1End,
//					rect2L);

			MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start, rect1End,
					rect2L, gr, wt, wt, blk, "面积", areastr, fntSize, true);

			ImageBoard* imageboard=NULL;
			MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
							6,1);
			measure->AddImage(imageboard);
		}
		else if (geo != NULL
				&& geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_CYLINDERFACE)
		{
			CylinderFaceAttribute revolutionFaceAttribute =
					*((CylinderFaceAttribute*) geo);

			Matrix3x4 worldMatrix = ShapeHelper::GetShapeWorldMatrix(face);
			GeometryHelper::Transform(revolutionFaceAttribute, worldMatrix);

			BoundingBox boundingBox = meshData->GetBoundingBox();
			Vector3 boxCenter = boundingBox.Center();
			GeometryHelper::Transform(boxCenter, worldMatrix);

			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(), boxCenter);
			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//构造线
			Line3D* line1 = new Line3D(boxCenter, pntInPlane);
			line1->SetName("MeasureImageLeader");
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line1->SetColor(m_leaderColor);
			//float length = (pntStartPoint - pntEndPoint).Length();

			//构造显示文本
			//	float revolutionRadius = revolutionFaceAttribute.GetRadius();

			string areastr = M3DTOOLS::floatToString(area);

			//根据整个场景包围盒的大小来确定文字的大小
			float fontSize = scene->GetSceneBox().Length() / 20;

			//将线加入测量对象中
			measure->AddLine(line1);
			//measure->AddLine(line2);

			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End(200.0, 100.0);
			float rect2L = 450.0;
			Color gr = Color::GRAY;
			Color wt = Color::WHITE;
			Color blk = Color::BLACK;
			Color grn = Color::GREEN;
			Color rd = Color::RED;
			Color blu = Color::BLUE;

//			MeasureDisplayHelper::createShadowRect(shape2DSet, rect1Start, rect1End,
//					rect2L);

			MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start, rect1End,
					rect2L, gr, wt, wt, blk, "面积", areastr, fntSize, true);
			ImageBoard* imageboard=NULL;
			MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
							6, 1);
			measure->AddImage(imageboard);
		}
		else if (geo != NULL
				&& geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_CONICALFACE)
		{
			ConicalFaceAttribute revolutionFaceAttribute =
					*((ConicalFaceAttribute*) geo);

			Matrix3x4 worldMatrix = ShapeHelper::GetShapeWorldMatrix(face);
			GeometryHelper::Transform(revolutionFaceAttribute, worldMatrix);

			BoundingBox  boundingBox = meshData->GetBoundingBox();
			Vector3 boxCenter = boundingBox.Center();
			GeometryHelper::Transform(boxCenter, worldMatrix);

			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(), boxCenter);
			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//构造线
			Line3D* line1 = new Line3D(boxCenter, pntInPlane);
			line1->SetName("MeasureImageLeader");
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line1->SetColor(m_leaderColor);
			//float length = (pntStartPoint - pntEndPoint).Length();

			//构造显示文本
			//float revolutionRadius = revolutionFaceAttribute.GetRadius();

			string areastr = M3DTOOLS::floatToString(area);

			//根据整个场景包围盒的大小来确定文字的大小
			float fontSize = scene->GetSceneBox().Length() / 20;

			//将线加入测量对象中
			measure->AddLine(line1);
			//measure->AddLine(line2);

			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End(200.0, 100.0);
			float rect2L = 450.0;
			Color gr = Color::GRAY;
			Color wt = Color::WHITE;
			Color blk = Color::BLACK;
			Color grn = Color::GREEN;
			Color rd = Color::RED;
			Color blu = Color::BLUE;

//			MeasureDisplayHelper::createShadowRect(shape2DSet, rect1Start, rect1End,
//					rect2L);

			MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start, rect1End,
					rect2L, gr, wt, wt, blk, "面积", areastr, fntSize, true);

			ImageBoard* imageboard=NULL;
			MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
							6, 1);
			measure->AddImage(imageboard);
		}
		else if (geo != NULL
				&& geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_SPHEREFACE)
		{
			SphereFaceAttribute revolutionFaceAttribute =
					*((SphereFaceAttribute*) geo);

			Matrix3x4 worldMatrix = ShapeHelper::GetShapeWorldMatrix(face);
			GeometryHelper::Transform(revolutionFaceAttribute, worldMatrix);

			BoundingBox  boundingBox = meshData->GetBoundingBox();
			Vector3 boxCenter = boundingBox.Center();
			GeometryHelper::Transform(boxCenter, worldMatrix);

			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(), boxCenter);
			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//构造线
			Line3D* line1 = new Line3D(boxCenter, pntInPlane);
			line1->SetName("MeasureImageLeader");
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line1->SetColor(m_leaderColor);
			//float length = (pntStartPoint - pntEndPoint).Length();

			//构造显示文本
			//float revolutionRadius = revolutionFaceAttribute.GetRadius();

			string areastr = M3DTOOLS::floatToString(area);

			//根据整个场景包围盒的大小来确定文字的大小
			float fontSize = scene->GetSceneBox().Length() / 20;

			//将线加入测量对象中
			measure->AddLine(line1);
			//measure->AddLine(line2);

			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End(200.0, 100.0);
			float rect2L = 450.0;
			Color gr = Color::GRAY;
			Color wt = Color::WHITE;
			Color blk = Color::BLACK;
			Color grn = Color::GREEN;
			Color rd = Color::RED;
			Color blu = Color::BLUE;

//			MeasureDisplayHelper::createShadowRect(shape2DSet, rect1Start, rect1End,
//					rect2L);

			MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start, rect1End,
					rect2L, gr, wt, wt, blk, "面积", areastr, fntSize, true);
			ImageBoard* imageboard=NULL;
			MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
							6, 1);
			measure->AddImage(imageboard);
		}
		else if (geo != NULL
				&& geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_TOROIDALFACE)
		{
			ToroidalFaceAttribute revolutionFaceAttribute =
					*((ToroidalFaceAttribute*) geo);

			Matrix3x4 worldMatrix = ShapeHelper::GetShapeWorldMatrix(face);
			GeometryHelper::Transform(revolutionFaceAttribute, worldMatrix);

			BoundingBox boundingBox = meshData->GetBoundingBox();
			Vector3 boxCenter = boundingBox.Center();
			GeometryHelper::Transform(boxCenter, worldMatrix);

			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(), boxCenter);
			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//构造线
			Line3D* line1 = new Line3D(boxCenter, pntInPlane);
			line1->SetName("MeasureImageLeader");
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line1->SetColor(m_leaderColor);
			//float length = (pntStartPoint - pntEndPoint).Length();

			//构造显示文本
			//float revolutionRadius = revolutionFaceAttribute.GetRadius();

			string areastr = M3DTOOLS::floatToString(area);

			//根据整个场景包围盒的大小来确定文字的大小
			float fontSize = scene->GetSceneBox().Length() / 20;

			//将线加入测量对象中
			measure->AddLine(line1);
			//measure->AddLine(line2);

			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End(200.0, 100.0);
			float rect2L = 450.0;
			Color gr = Color::GRAY;
			Color wt = Color::WHITE;
			Color blk = Color::BLACK;
			Color grn = Color::GREEN;
			Color rd = Color::RED;
			Color blu = Color::BLUE;

//			MeasureDisplayHelper::createShadowRect(shape2DSet, rect1Start, rect1End,
//					rect2L);

			MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start, rect1End,
					rect2L, gr, wt, wt, blk, "面积", areastr, fntSize, true);

			ImageBoard* imageboard=NULL;
			MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
							6, 1);
			measure->AddImage(imageboard);
		}
		else
		{
			Matrix3x4 worldMatrix = ShapeHelper::GetShapeWorldMatrix(face);


			BoundingBox boundingBox = meshData->GetBoundingBox();
			Vector3 boxCenter = boundingBox.Center();
			GeometryHelper::Transform(boxCenter, worldMatrix);

			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(), boxCenter);
			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//构造线
			Line3D* line1 = new Line3D(boxCenter, pntInPlane);
			line1->SetName("MeasureImageLeader");
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line1->SetColor(m_leaderColor);
			//float length = (pntStartPoint - pntEndPoint).Length();

			//构造显示文本


			string areastr = M3DTOOLS::floatToString(area);
			//	string planestr = normalstr + originstr;
			//根据整个场景包围盒的大小来确定文字的大小
			float fontSize = scene->GetSceneBox().Length() / 20;

			//将线加入测量对象中
			measure->AddLine(line1);
			//measure->AddLine(line2);

			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End(180.0, 100.0);
			float rect2L = 450.0;
			Color gr = Color::GRAY;
			Color wt = Color::WHITE;
			Color blk = Color::BLACK;
			Color grn = Color::GREEN;
			Color rd = Color::RED;
			Color blu = Color::BLUE;

//			MeasureDisplayHelper::createShadowRect(shape2DSet, rect1Start, rect1End,
//					rect2L);

			MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start, rect1End,
					rect2L, gr, wt, wt, blk, "面积", areastr, fntSize, true);

			ImageBoard* imageboard=NULL;
			MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
							6, 1);
			measure->AddImage(imageboard);

		}
	}

	return measure;
}

Measure* MeasureFactory::createApertureProperty(int shapeId, Vector2& screenPnt,
		SceneManager* scene)
{
	Measure* measure = NULL;

	return measure;
}

Measure* MeasureFactory::createModelProperty(int shapeId, Vector2& screenPnt,
		SceneManager* scene)
{
	Measure* measure = NULL;

	Shape* firstShape = scene->GetShape(shapeId);
	//如果的shape对象存在
	if (firstShape && firstShape->GetType() == SHAPE_MODEL)
	{
		Model * model = (Model*) firstShape;

		measure = new MeasureProperty;

		measure->SetMeasureType(Measure::MEASURE_TYPE_MODEL_PROPERTY);

		string modelName = model->GetName();
		Color modelColor = model->GetColor();
		LOGI("red %f green %f blue %f alpha %f",modelColor.m_r,modelColor.m_g,modelColor.m_b,modelColor.m_a);
		int modelFaceLod0 = model->GetVertexCount(0) / 3;
		int modelFaceLod1 = model->GetVertexCount(1) / 3;

		Matrix3x4 worldMatrix = ShapeHelper::GetShapeWorldMatrix(model);

		BoundingBox & boundingBox = model->GetBoundingBox();
		Vector3 boxCenter = boundingBox.Center();
		GeometryHelper::Transform(boxCenter, worldMatrix);
		//camera射线
		Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
				screenPnt.m_x, screenPnt.m_y);

		//构造投影平面
		Plane projPlane(cameraRay.GetDirection(), boxCenter);
		Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

		//构造线
		Line3D* line1 = new Line3D(boxCenter, pntInPlane);
		line1->SetName("MeasureImageLeader");
		//Color oriangle(1.0f,0.44f,0.0f) ;
		line1->SetColor(m_leaderColor);
		measure->AddLine(line1);

		Shape2DSet * shape2DSet = new Shape2DSet();
		float fntSize = 40;

		float rect2L = 500.0;
		Color gr = Color::GRAY;
		Color wt = Color::WHITE;
		Color blk = Color::BLACK;
		Color grn = Color::GREEN;
		Color rd = Color::RED;
		Color blu = Color::BLUE;

		Vector2 rectStart(1.0, 1.0);
		Vector2 rectEnd(180.0, 200.0);
//		MeasureDisplayHelper::createShadowRect(shape2DSet, rectStart, rectEnd,
//				rect2L);

		Vector2 rect1Start(1.0, 1.0);
		Vector2 rect1End(180.0, 100.0);

		MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start, rect1End,
				rect2L, gr, wt, wt, blk, "名称", modelName, fntSize, false);

//		Vector2 rect2Start(1.0, 100.0);
//		Vector2 rect2End(180.0, 200.0);
//		MeasureDisplayHelper::createRectImage(shape2DSet, rect2Start, rect2End,
//				rect2L, grn, wt, blk, blk, "颜色",
//				M3DTOOLS::floatToString(modelColor.m_r)+", "+
//				M3DTOOLS::floatToString(modelColor.m_g)+", "+
//				M3DTOOLS::floatToString(modelColor.m_b)+", "+
//				M3DTOOLS::floatToString(modelColor.m_a)+", "
//				, fntSize, false);


		Vector2 rect3Start(1.0, 100.0);
		Vector2 rect3End(180.0, 200.0);

		MeasureDisplayHelper::createRectImage(shape2DSet, rect3Start, rect3End,
				rect2L, rd, wt, wt, blk, "面数", IntToString(modelFaceLod0),
				fntSize, true);

//		Vector2 rect4Start(1.0, 200.0);
//		Vector2 rect4End(180.0, 300.0);
//
//		MeasureDisplayHelper::createRectImage(shape2DSet, rect4Start, rect4End,
//				rect2L, rd, wt, wt, blk, "面数1", IntToString(modelFaceLod1),
//				fntSize, true);


		ImageBoard* imageboard=NULL;
		MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
						6, 2);
		measure->AddImage(imageboard);

	}

	return measure;
}

bool MeasureFactory::UpdateMeasureImagePos(Measure* measure,Vector2& screenPnt,SceneManager* scene)
{

	bool ret = false;
	Vector3 tempPos;

	if(measure != NULL && scene != NULL)
	{
		vector<ImageBoard*>& imageboards = measure->GetImageBoards();

		for(int i =0;i<imageboards.size();i++)
		{
			ImageBoard* imageBoard = imageboards.at(i);

			if(imageBoard)
			{
//				if(imageBoard->GetName() == "TextsImage")
				{
					scene->Lock();

					Vector3 newPos = imageBoard->GetPosition();

					Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
							screenPnt.m_x, screenPnt.m_y);
					Plane projPlane(cameraRay.GetDirection(), newPos);


					newPos = projPlane.Project(cameraRay.GetOrigin());//得到文本点到平面的投影点

					imageBoard->SetPosition(newPos);
					tempPos = newPos;
					scene->UnLock();
				}
			}
		}

		vector<Line3D*> &lines = measure->m_LineList;
		for(int i =0 ;i<lines.size();i++)
		{
			Line3D *line = lines.at(i);
			if(line)
			{
				if(line->GetName()=="MeasureImageLeader")
				{
					scene->Lock();

					line->m_EndPoint=  tempPos;

					scene->UnLock();

				}
			}
		}
        
        scene->GetRenderManager()->RequestRedraw();

		return ret;
	}

	return ret;

}

/*********************************************************************************/
///获取属性
///
void MeasureFactory::GetPntProperty(int shapeId,
		SceneManager* scene,string & propertyStr)
{
	LOGI("MeasureFactory::GetPntProperty BEGIN");
	InternationalManager language;
	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);
	propertyStr = "";
	Shape* firstShape = scene->GetShape(shapeId);

	//firstShape->ClearProperties();
	if (firstShape && firstShape->GetType() == SHAPE_POINT_HANDLE)
	{
		LOGI("C++ createPntProperty");
		HandlerPoint * firstPnt = (HandlerPoint *) firstShape;

		Vector3 fCoordinate = firstPnt->GetPosition();
		string xStr = M3DTOOLS::floatToString(fCoordinate.m_x) + ", ";
		string yStr = M3DTOOLS::floatToString(fCoordinate.m_y) + ", ";
		string zStr = M3DTOOLS::floatToString(fCoordinate.m_z) ;
		string typestr = language.GetCurrentTypeValue("PointPropertiesCoordinater");
		string coordinateStr = xStr + yStr + zStr;

		firstPnt->ClearProperties();
		firstPnt->AddProperty(typestr,coordinateStr);

		propertyStr = firstPnt->GetProperties();
	}
	else
	{
		LOGE("MeasureFactory::GetPntProperty is NULL");
		propertyStr = "";
	}

	LOGI("MeasureFactory::GetPntProperty END");
}
///
void MeasureFactory::GetLineProperty(int shapeId,
		SceneManager* scene,string & propertyStr)
{
	LOGI(" MeasureFactory::GetLineProperty BEGIN");
	InternationalManager language;
	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);
	propertyStr = "";
	Shape* firstShape = scene->GetShape(shapeId);
	//firstShape->ClearProperties();
	//如果的shape对象存在
	if (firstShape && firstShape->GetType() == SHAPE_EDGE)
	{
		Edge* edge = (Edge*) firstShape;
		//得到边界线数据
		RefPolyLine * m_lineSet = edge->GetLineData();

		GeometryAttribute* geo = m_lineSet->GetGeoAttribute();
		///暂时仅支持，点到直线距离的测量
		if (geo != NULL && geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE)
		{
			LineAttribute lineAttribute = *((LineAttribute*) geo);

			Matrix3x4 worldMatrix = ShapeHelper::GetShapeWorldMatrix(edge);
			GeometryHelper::Transform(lineAttribute, worldMatrix);

			Vector3 pntStartPoint = lineAttribute.GetStartPoint();
			Vector3 pntEndPoint = lineAttribute.GetEndPoint();
			Vector3 lineCenter = (pntStartPoint + pntEndPoint) / 2;

			float length = (pntStartPoint - pntEndPoint).Length();
			//构造显示文本
			string lengthstr = M3DTOOLS::floatToString(length);

			string typestr = language.GetCurrentTypeValue(
					"LinePropertiesLength");

		//	edge->ClearProperties();

			edge->AddProperty(typestr, lengthstr);

			propertyStr = edge->GetProperties();

		}
		else if (geo != NULL && geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_ELLIPSE)
		{
			EllipseAttribute ellipseAttribute = *((EllipseAttribute*) geo);

			Matrix3x4 worldMatrix = ShapeHelper::GetShapeWorldMatrix(edge);
			GeometryHelper::Transform(ellipseAttribute, worldMatrix);
			Vector3 pntStartPoint = ellipseAttribute.GetStartPoint(); //!< Line起点
			Vector3 pntEndPoint = ellipseAttribute.GetEndPoint(); //!< Line终点
			Vector3 circleCenter = ellipseAttribute.GetCenterPoint(); //圆心
			float radius = ellipseAttribute.GetMajorRadius(); //长半轴 当 半径
			Vector3 xVector, yVector, zVector;
			ellipseAttribute.GetXYZDir(xVector, yVector, zVector);

			//求弧长
			float arcLength = 0; //弧长
			Vector3 startVector = pntStartPoint - circleCenter;
			Vector3 endVector = pntEndPoint - circleCenter;
			Vector3 crossVector = startVector.CrossProduct(endVector);
			float dotValue = crossVector.DotProduct(zVector);
			float alpha =
					acos(
							startVector.DotProduct(
									endVector
											/ (startVector.Length()
													* endVector.Length())));
			if (pntStartPoint == pntEndPoint)
			{
				//说明是个完成的圆
				arcLength = 2 * M_PI * radius;
			}
			else if (dotValue > 0)
			{
				//说明与z方向同向
				arcLength = alpha * radius;
			}
			else
			{
				//说明方向相反
				arcLength = (2 * M_PI - alpha) * radius;
			}
			//构造显示文本
			string radiusStr = M3DTOOLS::floatToString(radius);
			string centerStrX = M3DTOOLS::floatToString(circleCenter.m_x);
			string centerStrY = M3DTOOLS::floatToString(circleCenter.m_y);
			string centerStrZ = M3DTOOLS::floatToString(circleCenter.m_z);
			string centerStr = centerStrX + "," + centerStrY + "," + centerStrZ;

			string arcLengthStr = M3DTOOLS::floatToString(arcLength);
			string cstr = language.GetCurrentTypeValue(
					"LinePropertiesCircleCenter");
			string rstr = language.GetCurrentTypeValue(
					"LinePropertiesCircleRadius");
			string arcstr = language.GetCurrentTypeValue(
					"LinePropertiesCircleArcLength");

			//edge->ClearProperties();
			edge->AddProperty(cstr, centerStr);
			edge->AddProperty(rstr, radiusStr);
			edge->AddProperty(arcstr, arcLengthStr);
			propertyStr = edge->GetProperties();
		}
		else
		{
			//无几何属性的
			float length = 0.0f;//长度
			MeasureCalculateHelper::PolyLineLength(m_lineSet,length);

			string lengthstr = M3DTOOLS::floatToString(length);
			string typestr = language.GetCurrentTypeValue("LinePropertiesLength");

			edge->AddProperty(typestr,lengthstr);
			//------------此处先保留，以后改进 TODO
			propertyStr = edge->GetProperties();
		}
	}
	else
	{
		LOGE("MeasureFactory::GetLineProperty is NULL");
		propertyStr = "";
	}

	LOGI(" MeasureFactory::GetLineProperty END");
}
///
void MeasureFactory::GetCircleProperty(int shapeId,
		SceneManager* scene,string & propertyStr)
{
//	LOGI(" MeasureFactory::GetCircleProperty BEGIN");
//	InternationalManager language;
//	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);
//	propertyStr = "";
//	Shape* firstShape = scene->GetShape(shapeId);
//	firstShape->ClearProperties();
//	//如果的shape对象存在
//	if (firstShape && firstShape->GetType() == SHAPE_EDGE)
//	{
//		Edge* edge = (Edge*) firstShape;
//		//得到边界线数据
//		RefPolyLine * m_lineSet = edge->GetLineData();
//
//		GeometryAttribute* geo = m_lineSet->GetGeoAttribute();
//		///暂时仅支持，点到直线距离的测量
//		if (geo != NULL && geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_ELLIPSE)
//		{
//			EllipseAttribute ellipseAttribute = *((EllipseAttribute*) geo);
//
//			Matrix3x4 worldMatrix = ShapeHelper::GetShapeWorldMatrix(edge);
//			GeometryHelper::Transform(ellipseAttribute, worldMatrix);
//			Vector3 pntStartPoint = ellipseAttribute.GetStartPoint(); //!< Line起点
//			Vector3 pntEndPoint = ellipseAttribute.GetEndPoint(); //!< Line终点
//			Vector3 circleCenter = ellipseAttribute.GetCenterPoint(); //圆心
//			float radius = ellipseAttribute.GetMajorRadius(); //长半轴 当 半径
//			Vector3 xVector, yVector, zVector;
//			ellipseAttribute.GetXYZDir(xVector, yVector, zVector);
//
//			//求弧长
//			float arcLength = 0; //弧长
//			Vector3 startVector = pntStartPoint - circleCenter;
//			Vector3 endVector = pntEndPoint - circleCenter;
//			Vector3 crossVector = startVector.CrossProduct(endVector);
//			float dotValue = crossVector.DotProduct(zVector);
//			float alpha =
//					acos(
//							startVector.DotProduct(
//									endVector
//											/ (startVector.Length()
//													* endVector.Length())));
//			if (pntStartPoint == pntEndPoint)
//			{
//				//说明是个完成的圆
//				arcLength = 2 * M_PI * radius;
//			}
//			else if (dotValue > 0)
//			{
//				//说明与z方向同向
//				arcLength = alpha * radius;
//			}
//			else
//			{
//				//说明方向相反
//				arcLength = (2 * M_PI - alpha) * radius;
//			}
//			//构造显示文本
//			string radiusStr = M3DTOOLS::floatToString(radius);
//			string centerStrX = M3DTOOLS::floatToString(circleCenter.m_x);
//			string centerStrY = M3DTOOLS::floatToString(circleCenter.m_y);
//			string centerStrZ = M3DTOOLS::floatToString(circleCenter.m_z);
//			string centerStr = centerStrX + "," + centerStrY + "," + centerStrZ;
//
//			string arcLengthStr = M3DTOOLS::floatToString(arcLength);
//			string cstr = language.GetCurrentTypeValue(
//					"LinePropertiesCircleCenter");
//			string rstr = language.GetCurrentTypeValue(
//					"LinePropertiesCircleRadius");
//			string arcstr = language.GetCurrentTypeValue(
//					"LinePropertiesCircleArcLength");
//
//			//edge->ClearProperties();
//			edge->AddProperty(cstr, centerStr);
//			edge->AddProperty(rstr, radiusStr);
//			edge->AddProperty(arcstr, arcLengthStr);
//			propertyStr = edge->GetProperties();
//		}
//
//	}
//
//	LOGI(" MeasureFactory::GetCircleProperty END");
}
///
void MeasureFactory::GetFaceProperty(int shapeId,
		SceneManager* scene,string & propertyStr)
{

	LOGI(" MeasureFactory::GetFaceProperty BEGIN");
	InternationalManager language;
	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);
	propertyStr = "";
	Shape* firstShape = scene->GetShape(shapeId);
	//firstShape->ClearProperties();
	//如果的shape对象存在
	if (firstShape && firstShape->GetType() == SHAPE_FACE)
	{
		//------------此处先保留，以后改进 TODO
		Face* face = (Face*) firstShape;
		//得到边界线数据

		Mesh* meshData = (Mesh*) face->GetData();
		GeometryAttribute* geo = meshData->GetGeoAttribute();
		//计算面积
		//float area = 0.0f;
		//MeasureCalculateHelper::faceArea(meshData, area);

		//int triangleNumber = meshData->GetDataLength()/3;

		//string triangleNumberStr = M3DTOOLS::IntToString(triangleNumber);
		//string triangleTypeStr =language.GetCurrentTypeValue("FacePropertiesTrianglePatchNumber");
		//string areastr = M3DTOOLS::floatToString(area);
		//string typestr = language.GetCurrentTypeValue("FacePropertiesArea");

		//face->ClearProperties();
		//face->AddProperty(triangleTypeStr,triangleNumberStr);
		//face->AddProperty(typestr,areastr);
		//------------此处先保留，以后改进 TODO
		propertyStr = face->GetProperties();
	}

	LOGI(" MeasureFactory::GetFaceProperty END");
}

void MeasureFactory::GetModelProperty(int shapeId,
		SceneManager* scene,string & propertyStr)
{

	LOGI(" MeasureFactory::GetModelProperty BEGIN");
	//InternationalManager language;
//	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);
	propertyStr = "";
	Shape* firstShape = scene->GetShape(shapeId);
	//firstShape->ClearProperties();
	//如果的shape对象存在
	if (firstShape && firstShape->GetType() == SHAPE_MODEL)
	{
		LOGI("firstShape && firstShape->GetType() == SHAPE_MODEL");
		Model * model = (Model*) firstShape;
//		string modelName = model->GetName();
//		Color modelColor = model->GetColor();
//		int modelFaceLod0 = model->GetVertexCount(0) / 3;
//		int modelFaceLod1 = model->GetVertexCount(1) / 3;
//
//		string nameStr =language.GetCurrentTypeValue("ModelPropertiesName");
//		string faceName = language.GetCurrentTypeValue("ModelPropertiesTrianglePatchNumber");
//
//		string faceTriangleNumber = M3DTOOLS::IntToString(modelFaceLod0);

/*		model->ClearProperties();

		model->AddProperty(nameStr,modelName);
		model->AddProperty(faceName,faceTriangleNumber);*/

		propertyStr = model->GetProperties();

	}
	else
	{
		Model * model = scene->GetModel();
		propertyStr = model->GetProperties();
	}
	LOGI("propertyStr = %s",propertyStr.c_str());
	LOGI(" MeasureFactory::GetModelProperty END");

}

/*****************************************************************************************/
///cdj
Measure* MeasureFactory::createLineToFaceDistance(int firstLineId,
		int secondFaceId, int thirdPntId, int forthPntId, Vector2& screenPnt,
		SceneManager* scene)
{

	Measure* measure = NULL;
	LOGI("createLineTOFaceAngle step1");
	InternationalManager language;
	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);

	Shape * firstShape = scene->GetShape(firstLineId);
	Shape * secondShape = scene->GetShape(secondFaceId);
	Shape * forthShape = scene->GetShape(forthPntId);

	if (firstShape && secondShape && forthShape&& firstShape->GetType() == SHAPE_EDGE
			&& secondShape->GetType() == SHAPE_FACE && forthShape->GetType()==SHAPE_POINT_HANDLE)
	{
		LOGE("createLineTOFaceDistance step2");
		measure = new MeasureDistance;
		measure->SetMeasureType(Measure::MEASURE_TYPE_LINE_FACE_DISTANCE);

		Edge * firstEdge = (Edge*) firstShape;
		Face * secondFace = (Face *) secondShape;

		HandlerPoint * forthPnt = (HandlerPoint *) forthShape;
		Vector3 forthCoordinate = forthPnt->GetPosition();

		RefPolyLine * firstLineSet = firstEdge->GetLineData();
		Mesh * secondMeshData = (Mesh *) secondFace->GetData();

		GeometryAttribute * firstGeo = firstLineSet->GetGeoAttribute();
		GeometryAttribute * secondGeo = secondMeshData->GetGeoAttribute();

		if (firstGeo != NULL
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE
				&& secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
		{
			LOGI("createLineToFace step3");
			LineAttribute firstLineAttribute = *((LineAttribute *) firstGeo);
			PlaneFaceAttribute secondPlaneFaceAttribute =
					*((PlaneFaceAttribute *) secondGeo);

			Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstEdge);
			Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(
					secondFace);
			GeometryHelper::Transform(firstLineAttribute, worldMatrix1);
			GeometryHelper::Transform(secondPlaneFaceAttribute, worldMatrix2);

			float distance = -1;
			Vector3 projectPnt1, projectPnt2;
			MeasureCalculateHelper::LineFaceDistance(firstLineAttribute,
					secondPlaneFaceAttribute, distance, projectPnt1,
					projectPnt2);

			BoundingBox boundingBox = secondMeshData->GetBoundingBox();
			LOGI("boundingoxmin %s max %s",boundingBox.m_min.Tostring().c_str(),boundingBox.m_max.Tostring().c_str());


			Vector3 boxCenter = boundingBox.Center();
			GeometryHelper::Transform(boxCenter, worldMatrix2);

			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

			Vector3 line1Start = firstLineAttribute.GetStartPoint();
			Vector3 line1End = firstLineAttribute.GetEndPoint();
			Vector3 lineCenter = (line1Start + line1End) / 2;
			Vector3 center = (lineCenter + boxCenter) / 2;
			Vector3 line1Direction = (line1End-line1Start);
			Vector3 faceNormal = secondPlaneFaceAttribute.GetNormal();
			float markLngth;//表示平面的正方形长度
			vector<float> tempLength;
			tempLength.push_back(boundingBox.GetXLength());
			tempLength.push_back(boundingBox.GetYLength());
			tempLength.push_back(boundingBox.GetZLength());
			sort(tempLength.begin(), tempLength.end()); //排序
			LOGI("boxLength %f %f %f",tempLength[0],tempLength[1],tempLength[2]);
			if (tempLength[0] > 2.0f)
				markLngth = tempLength[0];
			else
				markLngth = tempLength[1];
			Color gr = Color::GRAY;
			Color wt = Color::WHITE;
			Color blk = Color::BLACK;
			Color grn = Color::GREEN;
			Color rd = Color::RED;
			Color blu = Color::BLUE;

			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(), line1Start);

			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//构造线
			Point * pointFace = new Point(forthCoordinate);//表示面的点
			pointFace->SetDrawType(1);
			pointFace->SetSize(0.8);
			measure->AddPoint(pointFace);
			Line3D *line1 = new Line3D(lineCenter, forthCoordinate);//测量线
			line1->SetColor(m_measureColor1);

			Line3D * line2 = new Line3D(center, pntInPlane);//引线
			line2->SetName("MeasureImageLeader");
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line2->SetColor(m_leaderColor);
			Line3D *line3 = new Line3D(line1Start, line1End);//测量线
			Color line3Color = Color::GREEN;
			line3->SetColor(m_selectColor);

			//line2->SetStartArrow(1);

			//构造显示文本//构造矩形
			vector<float> textLength;
			string distanceStr;

			vector<Vector3> pointArray;
			if(distance>=0.0f)
			{
				vector<float> values;
				values.push_back(distance);
				string unit;
				MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
						,values,unit);
				distanceStr = M3DTOOLS::floatToString(values.at(0))+unit;
                
			}
			else
			{
				LOGI("createLineToFace step3 Error");
				delete measure;
				measure = NULL;
				return measure;//返回空 不能测量
			}


			string typestr = language.GetCurrentTypeValue("LineFaceDistance");
			float distanctTextLength =  language.GetCurrentTypeValueLength(
					"LineFaceDistance");
			textLength.push_back(distanctTextLength);
            ComText* ctDistance = new ComText();
            CText* tDistance = new CText;
            tDistance->SetText(distanceStr);
            tDistance->SetLanguageType("LineFaceDistance");
            ctDistance->AddCText(tDistance);
            measure->m_ComTexts.push_back(ctDistance);
			float maxTextLength = *std::max_element(textLength.begin(),textLength.end());

			//根据整个场景包围盒的大小来确定文字的大小
			float fontSize = scene->GetSceneBox().Length() / 20;

			measure->AddLine(line1);
			measure->AddLine(line2);
			measure->AddLine(line3);

			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End(maxTextLength, 100.0);
			float rect2L = 450.0;
			MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start, rect1End,
					rect2L, m_measureColor1, wt, wt, blk, typestr, distanceStr, fntSize, true);//distance

			ImageBoard* imageboard=NULL;
			MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
					(maxTextLength + rect2L)/100.0f, 1);
			measure->AddImage(imageboard);
		}
		else
		{
			LOGI("createLineToFace step3 Error");
			delete measure;
			measure = NULL;

		}
	}
	return measure;

}

///
Measure* MeasureFactory::createPntToFaceDistance(int firstPntId,
		int secondFaceId, int thirdPntId, int forthPntId, Vector2& screenPnt,
		SceneManager* scene)
{

	Measure* measure = NULL;

	LOGI("createLineTOFaceAngle step1");
	InternationalManager language;
	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);

	Shape * firstShape = scene->GetShape(firstPntId);
	Shape * secondShape = scene->GetShape(secondFaceId);
	Shape * forthShape = scene->GetShape(forthPntId);

	if (firstShape && secondShape&& forthShape && firstShape->GetType() == SHAPE_POINT_HANDLE
			&& secondShape->GetType() == SHAPE_FACE && forthShape->GetType() == SHAPE_POINT_HANDLE)
	{
		LOGE("createLineTOFaceAngle step2");
		measure = new MeasureDistance;
		measure->SetMeasureType(Measure::MEASURE_TYPE_PNT_FACE_DISTANCE);
		HandlerPoint * firstPnt = (HandlerPoint *) firstShape;
		HandlerPoint * forthPnt = (HandlerPoint *) forthShape;
		Face * secondFace = (Face*) secondShape;
		Vector3 fCoordinate = firstPnt->GetPosition();
		Vector3 forthCoordinate = forthPnt->GetPosition();
		Mesh * secondMeshData = (Mesh*) secondFace->GetData();

		GeometryAttribute * secondGeo = secondMeshData->GetGeoAttribute();
		if (secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
		{
			PlaneFaceAttribute secondPlaneFaceAttribute =
					*((PlaneFaceAttribute *) secondGeo);

			Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(
					secondFace);
			GeometryHelper::Transform(secondPlaneFaceAttribute, worldMatrix2);

			Plane planeTemp(secondPlaneFaceAttribute.GetNormal(),
					secondPlaneFaceAttribute.GetOrigin());
			Vector3 pntProject = planeTemp.Project(fCoordinate);

			float Length = (fCoordinate - pntProject).Length();
			Vector3 center = (fCoordinate + pntProject) / 2;

			//把平面当成mesh面 求出最近 最远距离
			Matrix3x4 inverseMat = worldMatrix2.Inverse();
			Vector3 modelVec = inverseMat * fCoordinate;
			float distance, maxDistance;
			Vector3 projectPntMesh,maxPoint;
			MeasureCalculateHelper::PntFaceDistance(modelVec, secondMeshData,
					distance, maxDistance, projectPntMesh,maxPoint);
			projectPntMesh = worldMatrix2 * projectPntMesh;
			maxPoint = worldMatrix2 * maxPoint;

			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(), center);

			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//構造線
			Line3D * line1 = new Line3D(fCoordinate, pntProject);//投影距离
			Color line1Color = Color::BLACK;
			line1->SetColor(m_measureColor1);

			Line3D *line2 = new Line3D(center, pntInPlane);//引线
			line2->SetName("MeasureImageLeader");
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line2->SetColor(m_leaderColor);
			Point * point1 = new Point(fCoordinate);//测量点
			point1->SetDrawType(1);
			point1->SetSize(0.8);

			Point * point2 = new Point(pntProject); //投影点
			point2->SetDrawType(1);
			point2->SetSize(0.8);

			Point * point3 = new Point(maxPoint); //最大点
			point3->SetDrawType(1);
			point3->SetSize(0.8);

			Point * point4 = new Point(projectPntMesh); //最小点
			point4->SetDrawType(1);
			point4->SetSize(0.8);

			Point * pointFace = new Point(forthCoordinate);//表示面的点
			pointFace->SetDrawType(1);
			pointFace->SetSize(3.0);
			measure->AddPoint(pointFace);


			Line3D * line3 = new Line3D(fCoordinate,projectPntMesh);//最小距离，到有边界的面
			Color line3Color = Color::RED;
			line3->SetColor(m_measureColor3);

			Line3D * line4 = new Line3D(fCoordinate,maxPoint);//最大距离，到有边界的面
			Color line4Color = Color::BLUE;
			line4->SetColor(m_measureColor2);

			Line3D* line5 = new Line3D(projectPntMesh,pntProject);//到边界面最小距离和投影距离的连线
			line5->SetColor(m_exLineColor);
			line5->SetName("exLine");

			float fontSize = scene->GetSceneBox().Length() / 20;

			vector<float> textLength;
			string typestr = language.GetCurrentTypeValue("PointFaceProjectDistance");
			float typeTextLength = language.GetCurrentTypeValueLength(
					"PointFaceProjectDistance");
			textLength.push_back(typeTextLength);

			string maxDistanceTypeStr = language.GetCurrentTypeValue("PointFaceMaxDistance");
			float maxDstanceTextlength = language.GetCurrentTypeValueLength(
					"PointFaceMaxDistance");
			textLength.push_back(maxDstanceTextlength);

			string minDistanceTypeStr = language.GetCurrentTypeValue("PointFaceMinDistance");
			float minDstanceTextlength = language.GetCurrentTypeValueLength(
					"PointFaceMinDistance");
			textLength.push_back(minDstanceTextlength);

			float maxTextLength = *std::max_element(textLength.begin(),textLength.end());



			measure->AddLine(line2);

			measure->AddLine(line5);
			measure->AddPoint(point1);

			vector<float> values;
			values.push_back(Length);
			values.push_back(maxDistance);
			values.push_back(distance);
			string unit;
			MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
					,values,unit);
			string LengthStr = M3DTOOLS::floatToString(values.at(0)) + unit;
			string maxDistanceStr = M3DTOOLS::floatToString(values.at(1))+unit;
			string distanceStr = M3DTOOLS::floatToString(values.at(2)) + unit;

            ComText* ctLength = new ComText();
            CText* tLength = new CText;
            tLength->SetText(LengthStr);
            tLength->SetLanguageType("PointFaceProjectDistance");
            ctLength->AddCText(tLength);
            measure->m_ComTexts.push_back(ctLength);
            
            ComText* ctmaxDistance = new ComText();
            CText* tmaxDistance = new CText;
            tmaxDistance->SetText(maxDistanceStr);
            tmaxDistance->SetLanguageType("PointFaceMaxDistance");
            ctmaxDistance->AddCText(tmaxDistance);
            measure->m_ComTexts.push_back(ctmaxDistance);
            
            ComText* ctDistance = new ComText();
            CText* tDistance = new CText;
            tDistance->SetText(distanceStr);
            tDistance->SetLanguageType("PointFaceMinDistance");
            ctDistance->AddCText(tDistance);
            measure->m_ComTexts.push_back(ctDistance);
            
			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End(maxTextLength, 100.0);
			float rect2L = 450.0;
			Color gr = Color::GRAY;
			Color wt = Color::WHITE;
			Color blk = Color::BLACK;
			Color grn = Color::GREEN;
			Color rd = Color::RED;
			Color blu = Color::BLUE;

			Vector2 rectStart(1.0, 1.0);
			Vector2 rectEnd(maxTextLength, 300.0);
//			MeasureDisplayHelper::createShadowRect(shape2DSet, rectStart, rectEnd,
//					rect2L);
			if(distance>0)
			{
				measure->AddLine(line1);
				measure->AddLine(line3);
				measure->AddLine(line4);
				measure->AddPoint(point2);
				measure->AddPoint(point3);
				measure->AddPoint(point4);

				MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start,
						rect1End, rect2L, m_measureColor1, wt, wt, blk, typestr,
						LengthStr, fntSize, true); //length

				Vector2 rect2Start(1.0, 100.0);
				Vector2 rect2End(maxTextLength, 200.0);
				MeasureDisplayHelper::createRectImage(shape2DSet, rect2Start,
						rect2End, rect2L, m_measureColor2, wt, wt, blk,
						maxDistanceTypeStr, maxDistanceStr, fntSize, true); //maxDistance

				Vector2 rect3Start(1.0, 200.0);
				Vector2 rect3End(maxTextLength, 300.0);
				MeasureDisplayHelper::createRectImage(shape2DSet, rect3Start,
						rect3End, rect2L, m_measureColor3, wt, wt, blk,
						minDistanceTypeStr, distanceStr, fntSize, true); //distance
				ImageBoard* imageboard = NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard,
						shape2DSet, pntInPlane,
						(maxTextLength + rect2L) / 100.0f, 3);
				measure->AddImage(imageboard);
			}
			else
			{
				Vector2 rect3Start(1.0, 1.0);
				Vector2 rect3End(maxTextLength, 100.0);
				MeasureDisplayHelper::createRectImage(shape2DSet, rect3Start,
						rect3End, rect2L, m_measureColor1, wt, wt, blk,
						minDistanceTypeStr, distanceStr, fntSize, true); //distance
				ImageBoard* imageboard = NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard,
						shape2DSet, pntInPlane,
						(maxTextLength + rect2L) / 100.0f, 1);
				measure->AddImage(imageboard);
			}

		}
		else
		{
			//没有几何属性属性

			Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(
					secondFace);
			Matrix3x4 inverseMat = worldMatrix2.Inverse();
			Vector3 modelVec = inverseMat * fCoordinate;
			float distance, maxDistance;
			Vector3 projectPnt,maxPoint;
			MeasureCalculateHelper::PntFaceDistance(modelVec, secondMeshData,
					distance, maxDistance, projectPnt,maxPoint);
			projectPnt = worldMatrix2 * projectPnt;
			maxPoint = worldMatrix2 * maxPoint;

			Vector3 center = (fCoordinate + projectPnt) / 2;
			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(), projectPnt);

			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//構造線
			//Line3D * line1 = new Line3D(fCoordinate, projectPnt);
			Line3D *line2 = new Line3D(center, pntInPlane);
			line2->SetName("MeasureImageLeader");
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line2->SetColor(m_leaderColor);
			Point * point1 = new Point(fCoordinate);//测量点
			point1->SetDrawType(1);
			point1->SetSize(0.8);

			Point * point2 = new Point(projectPnt);//测量点
			point2->SetDrawType(1);
			point2->SetSize(0.8);

			Point * point3 = new Point(maxPoint);//测量点
			point3->SetDrawType(1);
			point3->SetSize(0.8);

			Point * pointFace = new Point(forthCoordinate);//表示面的点
			pointFace->SetDrawType(1);
			pointFace->SetSize(3.0);
			measure->AddPoint(pointFace);

			Line3D * line3 = new Line3D(fCoordinate, projectPnt); //最小距离，到有边界的面
			Color line3Color = Color::RED;
			line3->SetColor(m_measureColor1);

			Line3D * line4 = new Line3D(fCoordinate, maxPoint); //最小距离，到有边界的面
			Color line4Color = Color::BLUE;
						line4->SetColor(m_measureColor2);
			float fontSize = scene->GetSceneBox().Length() / 20;

			vector<float> textLength;

			string maxDistanceTypeStr = language.GetCurrentTypeValue("PointFaceMaxDistance");
			float maxDstanceTextlength = language.GetCurrentTypeValueLength(
					"PointFaceMaxDistance");
			textLength.push_back(maxDstanceTextlength);

			string minDistanceTypeStr = language.GetCurrentTypeValue("PointFaceMinDistance");
			float minDstanceTextlength = language.GetCurrentTypeValueLength(
					"PointFaceMinDistance");
			textLength.push_back(minDstanceTextlength);

			float maxTextLength = *std::max_element(textLength.begin(),textLength.end());

			//measure->AddLine(line1);
			measure->AddLine(line2);

			measure->AddPoint(point1);


			vector<float> values;
			values.push_back(distance);
			values.push_back(maxDistance);

			string unit;
			MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
					,values,unit);
			string distanceStr = M3DTOOLS::floatToString(values.at(0)) + unit;
			string maxDistanceStr = M3DTOOLS::floatToString(values.at(1))+unit;

            ComText* ctDistance = new ComText();
            CText* tDistance = new CText;
            tDistance->SetText(distanceStr);
            tDistance->SetLanguageType("PointFaceMinDistance");
            ctDistance->AddCText(tDistance);
            measure->m_ComTexts.push_back(ctDistance);
            
            ComText* ctmaxDistance = new ComText();
            CText* tmaxDistance = new CText;
            tmaxDistance->SetText(maxDistanceStr);
            tmaxDistance->SetLanguageType("PointFaceMaxDistance");
            ctmaxDistance->AddCText(tmaxDistance);
            measure->m_ComTexts.push_back(ctmaxDistance);
            
            
            
			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End(maxTextLength, 100.0);
			float rect2L = 450.0;
			Color gr = Color::GRAY;
			Color wt = Color::WHITE;
			Color blk = Color::BLACK;
			Color grn = Color::GREEN;
			Color rd = Color::RED;
			Color blu = Color::BLUE;

			Vector2 rectStart(1.0, 1.0);
			Vector2 rectEnd(maxTextLength, 200.0);
//			MeasureDisplayHelper::createShadowRect(shape2DSet, rectStart, rectEnd,
//					rect2L);
			if(distance>0)
			{
				measure->AddLine(line3);
				measure->AddLine(line4);
				measure->AddPoint(point2);
				measure->AddPoint(point3);
				MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start,
						rect1End, rect2L, m_measureColor1, wt, wt, blk,
						minDistanceTypeStr, distanceStr, fntSize, false);

				Vector2 rect2Start(1.0, 100.0);
				Vector2 rect2End(maxTextLength, 200.0);
				MeasureDisplayHelper::createRectImage(shape2DSet, rect2Start,
						rect2End, rect2L, m_measureColor2, wt, wt, blk,
						maxDistanceTypeStr, maxDistanceStr, fntSize, true);

				ImageBoard* imageboard = NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard,
						shape2DSet, pntInPlane,
						(maxTextLength + rect2L) / 100.0f, 2);
				measure->AddImage(imageboard);
			}
			else
			{
				//measure->AddLine(line3);
				//measure->AddLine(line4);
				Vector2 rect3Start(1.0, 1.0);
				Vector2 rect3End(maxTextLength, 100.0);
				MeasureDisplayHelper::createRectImage(shape2DSet, rect3Start,
						rect3End, rect2L, m_measureColor1, wt, wt, blk,
						minDistanceTypeStr, distanceStr, fntSize, true); //distance
				ImageBoard* imageboard = NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard,
						shape2DSet, pntInPlane,
						(maxTextLength + rect2L) / 100.0f, 1);
				measure->AddImage(imageboard);
			}
		}

	}

	return measure;

}

///
Measure* MeasureFactory::createFaceToFaceDistance(int firstFaceId,
		int secondFaceId, int thirdPntId, int forthPntId, Vector2& screenPnt,
		SceneManager* scene)
{

	Measure* measure = NULL;

	LOGI("createFaceTOFaceAngle step1");
	InternationalManager language;
	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);

	Shape * firstShape = scene->GetShape(firstFaceId);
	Shape * secondShape = scene->GetShape(secondFaceId);
	Shape * thirdShape = scene->GetShape(thirdPntId);
	Shape * forthShape = scene->GetShape(forthPntId);

	if (firstShape && secondShape && thirdShape&& forthShape&&
			firstShape->GetType() == SHAPE_FACE
			&& secondShape->GetType() == SHAPE_FACE
			&&thirdShape->GetType()==SHAPE_POINT_HANDLE
			&&forthShape->GetType()==SHAPE_POINT_HANDLE)
	{
		LOGE("createFaceTOFaceAngle step2");
		measure = new MeasureDistance;
		measure->SetMeasureType(Measure::MEASURE_TYPE_FACE_FACE_DISTANCE);

		Face * firstFace = (Face*) firstShape;
		Face * secondFace = (Face *) secondShape;

		Mesh * firstMeshData = (Mesh*) firstFace->GetData();
		Mesh * secondMeshData = (Mesh *) secondFace->GetData();

		HandlerPoint * thirdPnt = (HandlerPoint *) thirdShape;
		Vector3 thirdCoordinate = thirdPnt->GetPosition();
		HandlerPoint * forthPnt = (HandlerPoint *) forthShape;
		Vector3 forthCoordinate = forthPnt->GetPosition();

		GeometryAttribute * firstGeo = firstMeshData->GetGeoAttribute();
		GeometryAttribute * secondGeo = secondMeshData->GetGeoAttribute();

		if (firstGeo != NULL
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE
				&& secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
		{
			LOGI("createFaceTOFaceAngle step3");
			Color gr = Color::GRAY;
			Color wt = Color::WHITE;
			Color blk = Color::BLACK;
			Color grn = Color::GREEN;
			Color rd = Color::RED;
			Color blu = Color::BLUE;
			PlaneFaceAttribute firstPlaneFaceAttribute =
					*((PlaneFaceAttribute *) firstGeo);
			PlaneFaceAttribute secondPlaneFaceAttribute =
					*((PlaneFaceAttribute *) secondGeo);

			Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstFace);
			Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(
					secondFace);
			GeometryHelper::Transform(firstPlaneFaceAttribute, worldMatrix1);
			GeometryHelper::Transform(secondPlaneFaceAttribute, worldMatrix2);

			float distance;
			Vector3 projectPnt0, projectPnt1;
			MeasureCalculateHelper::FaceFaceDistance(firstPlaneFaceAttribute,
					secondPlaneFaceAttribute, distance, projectPnt0,
					projectPnt1);

			//求面2包围盒中点到面1的投影点
			Plane face1Plane(firstPlaneFaceAttribute.GetNormal(),thirdCoordinate);
			Vector3 boxCenter2Project = face1Plane.Project(forthCoordinate);
			Vector3 center = (boxCenter2Project + forthCoordinate) / 2;
			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(), center);

			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());



			//构造线
			Line3D *line1 = new Line3D(center, pntInPlane);//引线
			line1->SetName("MeasureImageLeader");
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line1->SetColor(m_leaderColor);
			Line3D * line2 = new Line3D(boxCenter2Project, thirdCoordinate);//辅助线
			line2->SetColor(m_exLineColor);
			line2->SetName("exLine");

			Line3D * line3 = new Line3D(boxCenter2Project,forthCoordinate);//测量线
			line3->SetColor(m_measureColor1);

			Point * point1 = new Point(boxCenter2Project);
			point1->SetSize(0.8f);
			point1->SetDrawType(1);



			Point * pointFace1 = new Point(thirdCoordinate);//表示面的点
			pointFace1->SetDrawType(1);
			pointFace1->SetSize(0.8);
			measure->AddPoint(pointFace1);

			Point * pointFace2 = new Point(forthCoordinate);//表示面的点
			pointFace2->SetDrawType(1);
			pointFace2->SetSize(0.8);
			measure->AddPoint(pointFace2);

			//line1->SetStartArrow(1);
			//	line2->SetStartArrow(1);

			//构造显示文本
			string distanceStr;
			if(distance>=0.0f)
			{
				vector<float> values;
				values.push_back(distance);
				string unit;
				MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
						,values,unit);
				distanceStr = M3DTOOLS::floatToString(values.at(0)) + unit;
				//anglestr = M3DTOOLS::floatToString(distance);
                
			}
			else
			{
				delete measure;
				measure = NULL;
				return measure;//不支持的化 返回空；
				//distanceStr = language.GetCurrentTypeValue("InvalidMeasureObject");
			}
			vector<float> textLength;
			string typestr = language.GetCurrentTypeValue("FaceFaceDistance");
			float typeStrTextLength =  language.GetCurrentTypeValueLength("FaceFaceDistance");
			textLength.push_back(typeStrTextLength);
            ComText* ctDistance = new ComText();
            CText* tDistance = new CText;
            tDistance->SetText(distanceStr);
            tDistance->SetLanguageType("FaceFaceDistance");
            ctDistance->AddCText(tDistance);
            measure->m_ComTexts.push_back(ctDistance);
			float maxTextLength = *std::max_element(textLength.begin(),textLength.end());
			//根据整个场景包围盒的大小来确定文字的大小
			float fontSize = scene->GetSceneBox().Length() / 20;

			measure->AddLine(line1);
			measure->AddLine(line2);
			measure->AddLine(line3);
			measure->AddPoint(point1);




			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End(typeStrTextLength, 100.0);
			float rect2L = 450.0;


//			MeasureDisplayHelper::createShadowRect(shape2DSet, rect1Start, rect1End,
//					rect2L);
			MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start, rect1End,
					rect2L, m_measureColor1, wt, wt, blk, typestr, distanceStr, fntSize, true);

			ImageBoard* imageboard=NULL;
			MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
					(maxTextLength + rect2L)/100.0f, 1);
			measure->AddImage(imageboard);
		}
		else
		{
			delete measure;
			measure = NULL;
		}
	}
	return measure;

}

/*************************************************************************************/
/// cdj
///
Measure* MeasureFactory::createFaceToFaceAngle(int firstFaceId,
		int secondFaceId, int thirdPntId, int forthPntId, Vector2& screenPnt,
		SceneManager* scene)
{

	Measure* measure = NULL;
	InternationalManager language;
	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);

	LOGI("createFaceTOFaceAngle step1");

	Shape * firstShape = scene->GetShape(firstFaceId);
	Shape * secondShape = scene->GetShape(secondFaceId);
	Shape * thirdShape = scene->GetShape(thirdPntId);
	Shape * forthShape = scene->GetShape(forthPntId);

	if (firstShape && secondShape &&thirdShape&& forthShape
			&&firstShape->GetType() == SHAPE_FACE
			&& secondShape->GetType() == SHAPE_FACE
			&&thirdShape->GetType()==SHAPE_POINT_HANDLE
			&&forthShape->GetType()==SHAPE_POINT_HANDLE)
	{
		LOGE("createFaceTOFaceAngle step2");
		measure = new MeasureAngle;
		measure->SetMeasureType(Measure::MEASURE_TYPE_FACE_FACE_ANGLE);

		Face * firstFace = (Face*) firstShape;
		Face * secondFace = (Face *) secondShape;

		Mesh * firstMeshData = (Mesh*) firstFace->GetData();
		Mesh * secondMeshData = (Mesh *) secondFace->GetData();

		HandlerPoint * thirdPnt = (HandlerPoint *) thirdShape;
		Vector3 thirdCoordinate = thirdPnt->GetPosition();
		HandlerPoint * forthPnt = (HandlerPoint *) forthShape;
		Vector3 forthCoordinate = forthPnt->GetPosition();

		GeometryAttribute * firstGeo = firstMeshData->GetGeoAttribute();
		GeometryAttribute * secondGeo = secondMeshData->GetGeoAttribute();

		if (firstGeo != NULL
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE
				&& secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
		{
			LOGI("createFaceTOFaceAngle step3");
			PlaneFaceAttribute firstPlaneFaceAttribute =
					*((PlaneFaceAttribute *) firstGeo);
			PlaneFaceAttribute secondPlaneFaceAttribute =
					*((PlaneFaceAttribute *) secondGeo);

			Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstFace);
			Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(
					secondFace);
			GeometryHelper::Transform(firstPlaneFaceAttribute, worldMatrix1);
			GeometryHelper::Transform(secondPlaneFaceAttribute, worldMatrix2);

			float angle;
			MeasureCalculateHelper::FaceFaceAngle(firstPlaneFaceAttribute,
					secondPlaneFaceAttribute, angle);



			Vector3 center = (thirdCoordinate + forthCoordinate) / 2;

			//angle = M_RADTODEG * angle;
			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

			//Vector3 face1Origin = firstPlaneFaceAttribute.GetOrigin();
			//Vector3 face2Origin = secondPlaneFaceAttribute.GetOrigin();

			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(), center);

			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//构造线
			Line3D *line1 = new Line3D(center, pntInPlane);
			line1->SetName("MeasureImageLeader");
			Line3D * line2 = new Line3D(thirdCoordinate, forthCoordinate);
			line2->SetColor(m_measureColor1);
			Point * point1 = new Point(thirdCoordinate);
			point1->SetDrawType(1);
			point1->SetSize(0.8f);
			measure->AddPoint(point1);

			Point * point2 = new Point(forthCoordinate);
			point2->SetDrawType(1);
			point2->SetSize(0.8f);
			measure->AddPoint(point2);


			//line1->SetStartArrow(1);
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line1->SetColor(m_leaderColor);
			//	line2->SetStartArrow(1);

			//构造显示文本
			string anglestr = M3DTOOLS::floatToString(angle);


			vector<float> textLength;
			string typestr = language.GetCurrentTypeValue("FaceFaceAngle");
			float typeStrTextLength =  language.GetCurrentTypeValueLength("FaceFaceAngle");
			textLength.push_back(typeStrTextLength);
			float maxTextLength = *std::max_element(textLength.begin(),textLength.end());

			//根据整个场景包围盒的大小来确定文字的大小
			float fontSize = scene->GetSceneBox().Length() / 20;

			measure->AddLine(line1);
			measure->AddLine(line2);
			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End(maxTextLength, 100.0);
			float rect2L = 450.0;
			Color gr = Color::GRAY;
			Color wt = Color::WHITE;
			Color blk = Color::BLACK;
			Color grn = Color::GREEN;
			Color rd = Color::RED;
			Color blu = Color::BLUE;

//			MeasureDisplayHelper::createShadowRect(shape2DSet, rect1Start, rect1End,
//					rect2L);
			MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start, rect1End,
					rect2L, m_measureColor1, wt, wt, blk, typestr, anglestr, fntSize, true);
			ImageBoard* imageboard=NULL;
			MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
					(maxTextLength + rect2L)/100.0f, 1);
			measure->AddImage(imageboard);
		}
		else
		{
			delete measure;
			measure = NULL;
		}
	}
	return measure;

}

///
Measure* MeasureFactory::createLineToFaceAngle(int firstLineId,
		int secondFaceId, int thirdPntId, int forthPntId, Vector2& screenPnt,
		SceneManager* scene)
{

	Measure* measure = NULL;
	LOGI("createLineTOFaceAngle step1");
	InternationalManager language;
	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);

	Shape * firstShape = scene->GetShape(firstLineId);
	Shape * secondShape = scene->GetShape(secondFaceId);
	Shape * forthShape = scene->GetShape(forthPntId);

	if (firstShape && secondShape && forthShape
			&&firstShape->GetType() == SHAPE_EDGE
			&& secondShape->GetType() == SHAPE_FACE
			&&forthShape->GetType()==SHAPE_POINT_HANDLE)
	{
		LOGE("createLineTOFaceAngle step2");
		measure = new MeasureAngle;
		measure->SetMeasureType(Measure::MEASURE_TYPE_LINE_FACE_ANGLE);

		Edge * firstEdge = (Edge*) firstShape;
		Face * secondFace = (Face *) secondShape;

		RefPolyLine * firstLineSet = firstEdge->GetLineData();
		Mesh * secondMeshData = (Mesh *) secondFace->GetData();

		HandlerPoint * forthPnt = (HandlerPoint *) forthShape;
		Vector3 forthCoordinate = forthPnt->GetPosition();

		GeometryAttribute * firstGeo = firstLineSet->GetGeoAttribute();
		GeometryAttribute * secondGeo = secondMeshData->GetGeoAttribute();

		Color gr = Color::GRAY;
		Color wt = Color::WHITE;
		Color blk = Color::BLACK;
		Color grn = Color::GREEN;
		Color rd = Color::RED;
		Color blu = Color::BLUE;
		if (firstGeo != NULL
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE
				&& secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
		{
			LOGI("createFaceTOFaceAngle step3");
			LineAttribute firstLineAttribute = *((LineAttribute *) firstGeo);
			PlaneFaceAttribute secondPlaneFaceAttribute =
					*((PlaneFaceAttribute *) secondGeo);

			Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstEdge);
			Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(
					secondFace);
			GeometryHelper::Transform(firstLineAttribute, worldMatrix1);
			GeometryHelper::Transform(secondPlaneFaceAttribute, worldMatrix2);

			float angle;
			MeasureCalculateHelper::LineFaceAngle(firstLineAttribute,
					secondPlaneFaceAttribute, angle);

			//camera射线
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

			Vector3 line1Start = firstLineAttribute.GetStartPoint();
			Vector3 line1End = firstLineAttribute.GetEndPoint();
			Vector3 lineCenter = (line1Start + line1End) / 2;
			Vector3 center = (lineCenter + forthCoordinate) / 2;

			//构造投影平面
			Plane projPlane(cameraRay.GetDirection(), line1Start);

			Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//构造线
			Line3D *line1 = new Line3D(lineCenter, forthCoordinate);
			line1->SetColor(m_measureColor1);
			Line3D * line2 = new Line3D(center, pntInPlane);//文本引线
			line2->SetName("MeasureImageLeader");
			//Color oriangle(1.0f,0.44f,0.0f) ;
			line2->SetColor(m_leaderColor);
			Line3D *line3 = new Line3D(line1Start, line1End);
			line3->SetColor(m_selectColor);
			Point * point1 = new Point(lineCenter);
			point1->SetDrawType(1);
			point1->SetSize(0.8f);
			measure->AddPoint(point1);

			Point * point2 = new Point(forthCoordinate);//表示面的点
			point2->SetDrawType(1);
			point2->SetSize(0.8f);
			measure->AddPoint(point2);


			//构造显示文本
			string anglestr = M3DTOOLS::floatToString(angle);

			vector<float> textLength;
			string typestr = language.GetCurrentTypeValue("LineFaceAngle");
			float typeStrTextLength =  language.GetCurrentTypeValueLength("LineFaceAngle");
			textLength.push_back(typeStrTextLength);
			float maxTextLength = *std::max_element(textLength.begin(),textLength.end());
			//根据整个场景包围盒的大小来确定文字的大小
			float fontSize = scene->GetSceneBox().Length() / 20;

			measure->AddLine(line1);
			measure->AddLine(line2);
			measure->AddLine(line3);

			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End(typeStrTextLength, 100.0);
			float rect2L = 450.0;

//			MeasureDisplayHelper::createShadowRect(shape2DSet, rect1Start, rect1End,
//					rect2L);
			MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start, rect1End,
					rect2L, m_measureColor1, wt, wt, blk, typestr, anglestr, fntSize, true);

			ImageBoard* imageboard=NULL;
			MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
					(maxTextLength + rect2L)/100.0f, 1);
			measure->AddImage(imageboard);

		}
		else
		{
			delete measure;
			measure = NULL;
		}
	}
	return measure;

}

}


