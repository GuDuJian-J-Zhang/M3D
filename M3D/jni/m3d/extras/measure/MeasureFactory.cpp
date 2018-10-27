#include "m3d/extras/measure/MeasureFactory.h"

#include "m3d/extras/measure/MeasureGroup.h"
#include "m3d/extras/measure/Measure.h"

#include "m3d/SceneManager.h"
#include "m3d/RenderManager.h"

#include "m3d/scene/ShapeNode.h"

#include "m3d/base/Ray.h"
#include "m3d/model/Line3D.h"
#include "m3d/model/Point.h"
#include "m3d/model/MeshData.h"

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
#include "m3d/extras/measure/MeasureDiametre.h"

#include "m3d/extras/measure/tools/GeometryHelper.h"
#include "m3d/extras/measure/tools/MeasureCalculateHelper.h"
#include "m3d/extras/measure/tools/MeasureDisplayHelper.h"

#include "m3d/graphics/ImageBoard.h"
#include "m3d/ResourceManager.h"

#include "sview/views/Parameters.h"
#include "m3d/base/glue/GlueObj.h"
#include <algorithm>
#include "m3d/model/Face.h"
#include "m3d/model/PolyLine.h"
#include  <sstream>

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
	const string MeasureFactory::Serializer_XML_Attribute_Length = "Length";
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

	Color MeasureFactory::m_leaderColor = Color(0.294f, 0.294f, 0.294f);
	Color MeasureFactory::m_selectColor = Color(0.0f, 1.0f, 0.0f);
	Color MeasureFactory::m_exLineColor = Color(0.51f, 1.0f, 0.51f);
	Color MeasureFactory::m_measureColor1 = Color(0.0f, 0.443f, 0.78f);
	Color MeasureFactory::m_measureColor2 = Color(0.969f, 0.373f, 0.047f);
	Color MeasureFactory::m_measureColor3 = Color(0.071f, 0.686f, 0.565f);

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
		else if (type == Measure::MEASURE_TYPE_FACE_FACE_DISTANCE) ///面到面距离
		{
			measure = createFaceToFaceDistance(firstShapeID, secondShapID,
				screenPnt, scene);
		}

		AddMeasureToScene(scene, measure);

		if (measure)
		{
			measure->SetFrontShow(false);
		}

		return measure;
	}

	string MeasureFactory::MeasureToXMLElement(SceneManager * scene, Measure * measure)
	{
		//MeasureDistance *distanceMeasure = (MeasureDistance*)measure;
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
		pMeasure->SetAttribute(Serializer_XML_Attribute_ID.c_str(), (int)measure->GetID());
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
						Color color = point->GetShapeColor();
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
						Line3D* line3D = measure->m_LineList[i];
						if (measure->m_LineList.size() > 52 && line3D->GetCanDelete())
						{
							continue;
						}
						//创建Point节点
						XMLElement * pLine3D = pDoc.NewElement(Serializer_XML_Element_Line3D.c_str());


						pLine3Ds->LinkEndChild(pLine3D);

						{

							//创建Color节点
							XMLElement * pColor = pDoc.NewElement(Serializer_XML_Element_Color.c_str());
							pLine3D->LinkEndChild(pColor);
							{
								Color color = line3D->GetShapeColor();
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
							}
							else {
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
								pText->SetAttribute(Serializer_XML_Attribute_Value.c_str(), text->GetCText(0)->GetText().c_str());
								pText->SetAttribute(Serializer_XML_Attribute_Type.c_str(), text->GetCText(0)->GetLanguageType().c_str());
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
										Color color = text->GetShapeColor();
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
		LOGI("end serialize");

		return xmlElement;
	}

	Measure * MeasureFactory::CreateMeasureFromXMLElement(SceneManager * scene, const string & xmlElement)
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
        if (xmlCenter) {
            pntInPlane.m_x = xmlCenter->FloatAttribute(Serializer_XML_Attribute_X.c_str());
            pntInPlane.m_y = xmlCenter->FloatAttribute(Serializer_XML_Attribute_Y.c_str());
            pntInPlane.m_z = xmlCenter->FloatAttribute(Serializer_XML_Attribute_Z.c_str());
        }
        
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
			}
			else {
				endPnt = temVec[1];
				Point *point2 = new Point(endPnt);
				point2->SetDrawType(1);
				point2->SetSize(0.8);
				measure->AddPoint(point2);
			}
		}
		//float lineLength = (startPnt - endPnt).Length();
		XMLElement* xmlLine3Ds = xmlMeasure->FirstChildElement(Serializer_XML_Element_Line3Ds.c_str());
		//LOGI("xmlLeaders");
		if (xmlLine3Ds)
		{
			XMLElement* xmlLine3D = xmlLine3Ds->FirstChildElement(Serializer_XML_Element_Line3D.c_str());
			Line3D* line;
			while (xmlLine3D)
			{

				Color temColor;
				Vector3 position, direction;
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
				line = new Line3D(position, direction);
				line->SetColor(temColor);
				if (type == 1) {
					line->SetName("MeasureImageLeader");
				}
				measure->AddLine(line);
				xmlLine3D = xmlLine3D->NextSiblingElement(Serializer_XML_Element_Line3D.c_str());

			}
		}

		XMLElement* xmlMeasureTexts = xmlMeasure->FirstChildElement(Serializer_XML_Element_MeasureTexts.c_str());

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
//            本身体统里的单位
            string unit = M3DTOOLS::IntToString(SVIEW::Parameters::Instance()->m_measureUnitStyle);
            distance =  MeasureDisplayHelper::GetInnerUnitValue(distance,unit);
				values.push_back(distance);
				string languageString = text->GetLanguageType().c_str();
				languages.push_back(languageString);
				string typestr = language.GetCurrentTypeValue(languageString);
				float projectDistTextLength = language.GetCurrentTypeValueLength(languageString);
				textLength.push_back(projectDistTextLength);
			}
			float maxTextLength = *std::max_element(textLength.begin(), textLength.end());

			string unit;
			if (measureType == MeasureAngle::MEASURE_TYPE_LINE_LINE_ANGLE || measureType == MeasureAngle::MEASURE_TYPE_FACE_FACE_ANGLE ||
				measureType == MeasureAngle::MEASURE_TYPE_LINE_FACE_ANGLE) {

			}
			else {
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
                //
                string lengthstr = contentStr;//M3DTOOLS::floatToString(values.at(i)) + unit;
				string typestr = language.GetCurrentTypeValue(languages.at(i));
				Vector2 rectStart;
				Vector2 rectEnd;
				Color tempColor;
				if (i == 0) {
					rectStart.m_x = rectStart.m_y = 1.0;

					rectEnd.m_x = maxTextLength;
					rectEnd.m_y = 100.0;
					tempColor = m_measureColor1;
				}
				else if (i == 1) {
					rectStart.m_x = 1.0;
					rectStart.m_y = 100.0;

					rectEnd.m_x = maxTextLength;
					rectEnd.m_y = 200.0;
					tempColor = m_measureColor2;
				}
				else if (i == 2) {
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
				shape2DSet, pntInPlane, (maxTextLength + rect2L) / 100.0f, values.size());
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

		AddMeasureToScene(scene, measure);
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
		else if (type == Measure::MEASURE_TYPE_MODEL_PROPERTY)
		{
			measure = createModelProperty(shapeID, screenPnt, scene);
		}

		AddMeasureToScene(scene, measure);

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

		if (type == Measure::MEASURE_TYPE_FACE_FACE_DISTANCE) ///面到面距离
		{
			measure = createFaceToFaceDistance(firstShapeID, secondShapID,
				thirdShapeID, forthShapID, screenPnt, scene);
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
			measure = createFaceToFaceAngle(firstShapeID, secondShapID, thirdShapeID, forthShapID, screenPnt,
				scene);
		}
		else if (type == Measure::MEASURE_TYPE_LINE_FACE_ANGLE) /// 直线与平面之间的夹角
		{
			LOGI("C++ angle measure line face function");
			measure = createLineToFaceAngle(firstShapeID, secondShapID, thirdShapeID, forthShapID, screenPnt,
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
		SceneManager* scene, string & outString)
	{
		//填充meaureNote,根据不同的测量类型创建不同的Note对象
		if (type == Measure::MEASURE_TYPE_PNT_COORD) ///点的坐标值
		{
			LOGI("C++ CreatePropertyMeasure");
			GetPntProperty(shapeID, scene, outString);
		}
		//	else if (type == Measure::MEASURE_TYPE_CRICLE_PROPERTY) ///圆或圆弧的弧长、直径（半径）、圆心
		//	{
		//		GetCircleProperty(shapeID,  scene,outString);
		//	}
		else if (type == Measure::MEASURE_TYPE_LINE_LENGTH ||
			type == Measure::MEASURE_TYPE_CRICLE_PROPERTY) ///边线的长度
		{
			GetLineProperty(shapeID, scene, outString);
		}

		else if (type == Measure::MEASURE_TYPE_FACE_PROPERTY) ////面的属性
		{
			LOGI("C++ CreateFacePropertyMeasure");
			GetFaceProperty(shapeID, scene, outString);
		}
		else if (type == Measure::MEASURE_TYPE_MODEL_PROPERTY)
		{
			GetModelProperty(shapeID, scene, outString);
		}
		else if (type == Measure::MEASURE_TYPE_MODEL_STATISTICS)
		{
			GetModelStatistics(shapeID, scene, outString);
		}
	}

	bool MeasureFactory::AddMeasureToScene(SceneManager* scene, Measure* measure)
	{
		bool ret = false;
		if (scene && measure)
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

		IShape* firstShape = scene->GetShape(firstPntId);
		IShape* secondShape = scene->GetShape(secondPntId);

		//如果的shape对象存在
		if (firstShape && secondShape && firstShape->GetType() == SHAPE_POINT_HANDLE
			&& secondShape->GetType() == SHAPE_POINT_HANDLE)
		{
			measure = new MeasureDistance;
			/*long address = ((unsigned long*)(&measure))[0];

			IShape* shape  = (MeasureDistance*)address;

			shape->GetID();*/

			measure->SetMeasureType(Measure::MEASURE_TYPE_PNT_PNT_DISTANCE);

			HandlerPoint* firstPnt = (HandlerPoint*)firstShape;
			HandlerPoint* secondPnt = (HandlerPoint*)secondShape;

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
				, valuse, unit);
			LOGI("SVIEW::Parameters::Instance()->m_measureUnitStyle %d", SVIEW::Parameters::Instance()->m_measureUnitStyle);
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

			ImageBoard* imageboard = NULL;
			MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
				(rect2L + rectLength) / 100.0f, 1);
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

		IShape* firstShape = scene->GetShape(firstPntId);
		IShape* secondShape = scene->GetShape(secondLineId);

		LOGI("createPntTOLineDistance step1");
		//如果的shape对象存在
		if (firstShape && secondShape && firstShape->GetType() == SHAPE_POINT_HANDLE
			&& secondShape->GetType() == SHAPE_EDGE)
		{
			LOGI("createPntTOLineDistance step2");
			measure = new MeasureDistance;

			measure->SetMeasureType(Measure::MEASURE_TYPE_PNT_LINE_DISTANCE);

			HandlerPoint* firstPnt = (HandlerPoint*)firstShape;
			Edge* secondEdge = (Edge*)secondShape;

			//得到边界线数据
			RefPolyLine * m_lineSet = secondEdge->GetLineData();

			GeometryAttribute* geo = secondEdge->GetGeoAttribute();
			///暂时仅支持，点到直线距离的测量
			if (geo != NULL && geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE)
			{
				LineAttribute lineAttribute = *((LineAttribute*)geo);

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
				float projectDistTextLength = language.GetCurrentTypeValueLength(
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

				float maxTextLength = *std::max_element(textLength.begin(), textLength.end());

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
					rect2End, rect2L, m_measureColor2, wt, wt, blk, maxDistanceTypeStr
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
					shape2DSet, pntInPlane, (maxTextLength + rect2L) / 100.0f, 3);
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
				EllipseAttribute ellipseAttribute = *((EllipseAttribute*)geo);

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
					lineTemp->SetCanDelete(true);
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
					shape2DSet, pntInPlane, (minDistancTextlength + rect2L) / 100.0f, 1);
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
					rect1End, rect2L, m_measureColor1, wt, wt, blk, typestr, distancestr ,
					fntSize, true);

				ImageBoard* imageboard = NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard,
					shape2DSet, pntInPlane, (minDistancTextlength + rect2L) / 100.0f, 1);
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
		LOGI("createLineTOLineDistance step1");
		InternationalManager language;
		language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);

		IShape * firstShape = (IShape*)scene->GetShape(firstLineId);
		IShape *secondShape = (IShape*)scene->GetShape(secondLineId);

		if (firstShape && secondShape && firstShape->GetType() == SHAPE_EDGE
			&& secondShape->GetType() == SHAPE_EDGE)
		{

			LOGI("createLineTOLineDistance step2");
			measure = new MeasureDistance;

			measure->SetMeasureType(Measure::MEASURE_TYPE_LINE_LINE_DISTANCE);

			Edge * firstEdge = (Edge*)firstShape;
			Edge * secondEdge = (Edge *)secondShape;

			RefPolyLine * firstLineSet = firstEdge->GetLineData();
			RefPolyLine * secondLineSet = secondEdge->GetLineData();

			GeometryAttribute * firstGeo = firstEdge->GetGeoAttribute();
			GeometryAttribute * secondGeo = secondEdge->GetGeoAttribute();

			if (firstGeo != NULL
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE
				&& secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE)
			{
				LOGI("createLineTOLineDistance step3");
				LineAttribute firstLineAttribute = *((LineAttribute *)firstGeo);
				LineAttribute secondLineAttribute = *((LineAttribute *)secondGeo);

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
				if ((projectPnt1 - pntStartPoint1).Length() > (projectPnt1 - pntEndPoint1).Length())
				{
					extern1 = pntEndPoint1;
				}
				else
				{
					extern1 = pntStartPoint1;
				}
				Line3D* line7 = new Line3D(extern1, projectPnt1);//线1延长线
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
				Line3D* line8 = new Line3D(extern2, projectPnt2);//线2延长线
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
				if (!in2)
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
					float parallelDistTextLength = language.GetCurrentTypeValueLength(
						"LineLineParallelDistance");
					textLength.push_back(parallelDistTextLength);

					string minDistanceTypeStr = language.GetCurrentTypeValue("LineLineMinDistance");
					float minDstanceTextlength = language.GetCurrentTypeValueLength(
						"LineLineMinDistance");
					textLength.push_back(minDstanceTextlength);

					float maxTextLength = *std::max_element(textLength.begin(), textLength.end());

					vector<float> values;
					values.push_back(distance);
					values.push_back(min);
					string unit;
					MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
						, values, unit);
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
						rect3End, rect2L, m_measureColor2, wt, wt, blk, minDistanceTypeStr, minStr,
						fntSize, true);//min

					ImageBoard* imageboard = NULL;
					MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
						(maxTextLength + rect2L) / 100.0f, 2);
					measure->AddImage(imageboard);
				}
				else
				{
					vector<float> textLength;
					string typestr1 = language.GetCurrentTypeValue("LineLineMinDistance");
					float minDstanceTextlength = language.GetCurrentTypeValueLength(
						"LineLineMinDistance");
					textLength.push_back(minDstanceTextlength);

					string skewLinesDistStr = language.GetCurrentTypeValue("LineLineSkewLinesDistance");
					float  skewLinesDistTextlength = language.GetCurrentTypeValueLength(
						"LineLineSkewLinesDistance");
					textLength.push_back(skewLinesDistTextlength);

					float maxTextLength = *std::max_element(textLength.begin(), textLength.end());



					vector<float> values;
					values.push_back(distance);
					values.push_back(min);
					string unit;
					MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
						, values, unit);
					string distanceStr = M3DTOOLS::floatToString(values.at(0)) + unit;
					string minStr = M3DTOOLS::floatToString(values.at(1))+ unit;
					//string lengthstr2 = M3DTOOLS::floatToString(max);

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

					Vector2 rectStart(1.0, 1.0);
					Vector2 rectEnd(maxTextLength, 200.0);
					//				MeasureDisplayHelper::createShadowRect(shape2DSet, rectStart, rectEnd,
					//						rect2L);
					Vector2 rect1Start(1.0, 1.0);
					Vector2 rect1End(maxTextLength, 100.0);

					MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start,
						rect1End, rect2L, m_measureColor1, wt, wt, blk, skewLinesDistStr, distanceStr,
						fntSize, false);//distance

					Vector2 rect3Start(1.0, 100.0);
					Vector2 rect3End(maxTextLength, 200.0);

					MeasureDisplayHelper::createRectImage(shape2DSet, rect3Start,
						rect3End, rect2L, m_measureColor2, wt, wt, blk, typestr1, minStr,
						fntSize, true);//min

					ImageBoard* imageboard = NULL;
					MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
						(maxTextLength + rect2L) / 100.0f, 2);
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

				EllipseAttribute firstEllipseAttribute = *((EllipseAttribute *)firstGeo);
				EllipseAttribute secondEllipseAttribute = *((EllipseAttribute *)secondGeo);

				Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstEdge);
				Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(
					secondEdge);
				GeometryHelper::Transform(firstEllipseAttribute, worldMatrix1);
				GeometryHelper::Transform(secondEllipseAttribute, worldMatrix2);

				float distance0, distance1;
				Vector3 projectPnt0, projectPnt1;
				int type;

				MeasureCalculateHelper::LineLineDistance(firstEllipseAttribute, secondEllipseAttribute,
					distance0, distance1, projectPnt0, projectPnt1, type);

				Vector3 center0, center1;//圆心
				float radius0, radius1;//半径
				Vector3 start0, start1;//起点
				Vector3 dirX0, dirY0, dirZ0, dirX1, dirY1, dirZ1;

				firstEllipseAttribute.GetXYZDir(dirX0, dirY0, dirZ0);
				secondEllipseAttribute.GetXYZDir(dirX1, dirY1, dirZ1);

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
						it != pntPoints.begin() + offset + dataLength; it = it + 2)
					{

						Line3D * lineTemp = new Line3D(worldMatrix1 * (*it),
							worldMatrix1 * (*(it + 1)));
						Color tempColor = Color::GREEN;
						lineTemp->SetColor(m_selectColor);
						lineTemp->SetCanDelete(true);
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
						it != pntPoints.begin() + offset + dataLength; it = it + 2)
					{

						Line3D * lineTemp = new Line3D(worldMatrix2 * (*it),
							worldMatrix2 * (*(it + 1)));
						Color tempColor = Color::GREEN;
						lineTemp->SetColor(m_selectColor);
						lineTemp->SetCanDelete(true);
						measure->AddLine(lineTemp);
					}
				}

				vector<float> values;
				values.push_back(distance0);
				values.push_back(distance1);
				string unit;
				MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
					, values, unit);
				string distance0Str = M3DTOOLS::floatToString(values.at(0)) + unit;
				string distance1Str = M3DTOOLS::floatToString(values.at(1))+ unit;

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

				if (0 == type)
				{
					//构造线
					Line3D* line1 = new Line3D(center0, center1);//圆心距
					line1->SetColor(rd);

					measure->AddLine(line1);

					Line3D* line2 = new Line3D(lineCenter, pntInPlane);//引线
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
						rect1End, rect2L, m_measureColor1, wt, wt, blk, circlesCenterDistTypeStr, distance1Str ,
						fntSize, true);//distance1

					ImageBoard* imageboard = NULL;
					MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
						(circlesCenterDistTextLength + rect2L) / 100.0f, 1);
					measure->AddImage(imageboard);
				}
				else if (1 == type)
				{
					Line3D* line1 = new Line3D(center0, center1);//圆心距
					line1->SetColor(m_measureColor1);
					measure->AddLine(line1);

					Line3D* line2 = new Line3D(projectPnt0, projectPnt1);//轴距
					line2->SetColor(m_measureColor2);
					measure->AddLine(line2);

					Line3D* line3 = new Line3D(center0, projectPnt0);//辅助线
					line3->SetColor(m_exLineColor);
					line3->SetName("exLine");
					measure->AddLine(line3);

					Line3D* line4 = new Line3D((center0 + center1) / 2, pntInPlane);//引线
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


					float maxTextLength = *std::max_element(textLength.begin(), textLength.end());

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

					ImageBoard* imageboard = NULL;
					MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
						(maxTextLength + rect2L) / 100.0f, 2);
					measure->AddImage(imageboard);
				}
				else if (2 == type)
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
					Line3D* line2 = new Line3D(lineCenter, pntInPlane);//引线
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

					ImageBoard* imageboard = NULL;
					MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
						(radiusDifferenceTextLength + rect2L) / 100.0f, 1);
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
					&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_ELLIPSE))
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

				Vector3 minProject1, minProject2;
				float minDistance = 100000.0f;
				MeasureCalculateHelper::PolylinePolylineDistance(firstLineSet, secondLineSet, minProject1,
					minProject2, minDistance, worldMatrix1, worldMatrix2);
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
						it != pntPoints.begin() + offset + dataLength; it = it + 2)
					{

						Line3D * lineTemp = new Line3D(worldMatrix1 * (*it),
							worldMatrix1 * (*(it + 1)));
						Color tempColor = Color::GREEN;
						lineTemp->SetColor(m_selectColor);
						lineTemp->SetCanDelete(true);
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
						it != pntPoints.begin() + offset + dataLength; it = it + 2)
					{

						Line3D * lineTemp = new Line3D(worldMatrix2 * (*it),
							worldMatrix2 * (*(it + 1)));
						Color tempColor = Color::GREEN;
						lineTemp->SetColor(m_selectColor);
						lineTemp->SetCanDelete(true);
						measure->AddLine(lineTemp);
					}
				}

				Vector3 lineCenter = (minProject1 + minProject2) / 2;

				//camera射线
				Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);
				//构造投影平面
				Plane projPlane(cameraRay.GetDirection(), lineCenter);
				Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

				//构造线

				Line3D * line1 = new Line3D(minProject1, minProject2);//测量线
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

				Line3D * line2 = new Line3D(lineCenter, pntInPlane);//引线
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
				float maxTextLength = *std::max_element(textLength.begin(), textLength.end());
				vector<float> values;
				values.push_back(minDistance);
				string unit;
				MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
					, values, unit);
				string minDistanceStr = M3DTOOLS::floatToString(values.at(0)) + unit;

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
					rect2L, m_measureColor1, wt, wt, blk, minDistanceTypeStr, minDistanceStr , fntSize, true);//minDistance

				ImageBoard* imageboard = NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
					(maxTextLength + rect2L) / 100.0f, 1);
				measure->AddImage(imageboard);

				LOGI("line and ellipse distance END");

			}
			else
			{
				delete measure;
				measure = NULL;
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

		IShape * firstShape = scene->GetShape(firstLineId);
		IShape * secondShape = scene->GetShape(secondFaceId);

		if (firstShape && secondShape && firstShape->GetType() == SHAPE_EDGE
			&& secondShape->GetType() == SHAPE_FACE)
		{
			LOGI("createLineTOFaceDistance step2");
			measure = new MeasureDistance;
			measure->SetMeasureType(Measure::MEASURE_TYPE_LINE_FACE_DISTANCE);

			Edge * firstEdge = (Edge*)firstShape;
			Face * secondFace = dynamic_cast<Face*>(secondShape);
			RefPolyLine * firstLineSet = firstEdge->GetLineData();
			Mesh * secondMeshData = (Mesh *)secondFace->GetData();
			GeometryAttribute * firstGeo = firstEdge->GetGeoAttribute();
			GeometryAttribute * secondGeo = secondFace->GetGeoAttribute();

			if (firstGeo != NULL
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE
				&& secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
			{
				LOGI("createLineToFace step3");
				LineAttribute firstLineAttribute = *((LineAttribute *)firstGeo);
				PlaneFaceAttribute secondPlaneFaceAttribute =
					*((PlaneFaceAttribute *)secondGeo);

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
				LOGI("boundingoxmin %s max %s", boundingBox.m_min.Tostring().c_str(), boundingBox.m_max.Tostring().c_str());



				Vector3 boxCenter = boundingBox.Center();
				GeometryHelper::Transform(boxCenter, worldMatrix2);



				//camera射线
				Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

				Vector3 line1Start = firstLineAttribute.GetStartPoint();
				Vector3 line1End = firstLineAttribute.GetEndPoint();
				Vector3 lineCenter = (line1Start + line1End) / 2;
				Vector3 center = (lineCenter + boxCenter) / 2;
				Vector3 line1Direction = (line1End - line1Start);
				Vector3 faceNormal = secondPlaneFaceAttribute.GetNormal();
				float markLngth;//表示平面的正方形长度
				vector<float> tempLength;
				tempLength.push_back(boundingBox.GetXLength());
				tempLength.push_back(boundingBox.GetYLength());
				tempLength.push_back(boundingBox.GetZLength());
				sort(tempLength.begin(), tempLength.end()); //排序
				LOGI("boxLength %f %f %f", tempLength[0], tempLength[1], tempLength[2]);
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


				Plane measurePlane(secondPlaneFaceAttribute.GetNormal(), secondPlaneFaceAttribute.GetOrigin());
				Vector3 lineCenterProject = measurePlane.Project(lineCenter);//直线中心点到测量面的投影点
				Plane projPlane(cameraRay.GetDirection(), (lineCenter + lineCenterProject) / 2.0f);
				Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());
				//构造线
				Line3D *line1 = new Line3D(lineCenter, lineCenterProject);//测量线
				line1->SetColor(m_measureColor1);

				Line3D * line2 = new Line3D((lineCenter + lineCenterProject) / 2.0f, pntInPlane);//引线
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
				if (distance >= 0.0f)
				{
					vector<float> values;
					values.push_back(distance);
					string unit;
					MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
						, values, unit);
					distanceStr = M3DTOOLS::floatToString(values.at(0))+ unit;
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
				float distanctTextLength = language.GetCurrentTypeValueLength(
					"LineFaceDistance");
				textLength.push_back(distanctTextLength);

				ComText* ctDistance = new ComText();
				CText* tDistance = new CText;
				tDistance->SetText(distanceStr);
				tDistance->SetLanguageType("LineFaceDistance");
				ctDistance->AddCText(tDistance);
				measure->m_ComTexts.push_back(ctDistance);

				float maxTextLength = *std::max_element(textLength.begin(), textLength.end());

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

				ImageBoard* imageboard = NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
					(maxTextLength + rect2L) / 100.0f, 1);
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

		IShape * firstShape = scene->GetShape(firstPntId);
		IShape * secondShape = scene->GetShape(secondFaceId);

		if (firstShape && secondShape && firstShape->GetType() == SHAPE_POINT_HANDLE
			&& secondShape->GetType() == SHAPE_FACE)
		{
			LOGI("createLineTOFaceAngle step2");
			measure = new MeasureDistance;
			measure->SetMeasureType(Measure::MEASURE_TYPE_PNT_FACE_DISTANCE);
			HandlerPoint * firstPnt = (HandlerPoint *)firstShape;
			Face * secondFace = dynamic_cast<Face*>(secondShape);
			Vector3 fCoordinate = firstPnt->GetPosition();
			Mesh * secondMeshData = secondFace->GetData();

			GeometryAttribute * secondGeo = secondFace->GetGeoAttribute();
			if (secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
			{
				PlaneFaceAttribute secondPlaneFaceAttribute =
					*((PlaneFaceAttribute *)secondGeo);

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
				Vector3 projectPntMesh, maxPoint;
				MeasureCalculateHelper::PntFaceDistance(modelVec, secondMeshData,
					distance, maxDistance, projectPntMesh, maxPoint);
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


				Line3D * line3 = new Line3D(fCoordinate, projectPntMesh);//最小距离，到有边界的面
				Color line3Color = Color::RED;
				line3->SetColor(m_measureColor3);

				Line3D * line4 = new Line3D(fCoordinate, maxPoint);//最大距离，到有边界的面
				Color line4Color = Color::BLUE;
				line4->SetColor(m_measureColor2);

				Line3D* line5 = new Line3D(projectPntMesh, pntProject);//到边界面最小距离和投影距离的连线
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

				float maxTextLength = *std::max_element(textLength.begin(), textLength.end());



				measure->AddLine(line2);

				measure->AddLine(line5);
				measure->AddPoint(point1);

				vector<float> values;
				values.push_back(Length);
				values.push_back(maxDistance);
				values.push_back(distance);
				string unit;
				MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
					, values, unit);
				string LengthStr = M3DTOOLS::floatToString(values.at(0))+ unit;
				string maxDistanceStr = M3DTOOLS::floatToString(values.at(1))  + unit;
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
				if (distance > 0)
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
						LengthStr , fntSize, true); //length

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
				Vector3 projectPnt, maxPoint;
				MeasureCalculateHelper::PntFaceDistance(modelVec, secondMeshData,
					distance, maxDistance, projectPnt, maxPoint);
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

				float maxTextLength = *std::max_element(textLength.begin(), textLength.end());

				//measure->AddLine(line1);
				measure->AddLine(line2);

				measure->AddPoint(point1);


				vector<float> values;
				values.push_back(distance);
				values.push_back(maxDistance);

				string unit;
				MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
					, values, unit);
				string distanceStr = M3DTOOLS::floatToString(values.at(0)) + unit;
				string maxDistanceStr = M3DTOOLS::floatToString(values.at(1)) + unit;

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
				if (distance > 0)
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
						minDistanceTypeStr, distanceStr , fntSize, false);

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

		Shape * firstShape = dynamic_cast<Shape*>(scene->GetShape(firstFaceId));
		Shape * secondShape = dynamic_cast<Shape*>(scene->GetShape(secondFaceId));

		if (firstShape && secondShape && firstShape->GetType() == SHAPE_FACE
			&& secondShape->GetType() == SHAPE_FACE)
		{
			LOGI("createFaceTOFaceAngle step2");
			measure = new MeasureDistance;
			measure->SetMeasureType(Measure::MEASURE_TYPE_FACE_FACE_DISTANCE);

			Face * firstFace = dynamic_cast<Face*>(firstShape);
			Face * secondFace = dynamic_cast<Face*>(secondShape);

			Mesh * firstMeshData = (Mesh*)firstFace->GetData();
			Mesh * secondMeshData = (Mesh *)secondFace->GetData();

			GeometryAttribute * firstGeo = firstFace->GetGeoAttribute();
			GeometryAttribute * secondGeo = secondFace->GetGeoAttribute();

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
					*((PlaneFaceAttribute *)firstGeo);
				PlaneFaceAttribute secondPlaneFaceAttribute =
					*((PlaneFaceAttribute *)secondGeo);

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
				Plane face1Plane(firstPlaneFaceAttribute.GetNormal(), boxCenter1);
				Vector3 boxCenter2Project = face1Plane.Project(boxCenter2);

				//构造线
				Line3D *line1 = new Line3D(center, pntInPlane);//引线
				line1->SetName("MeasureImageLeader");
				//Color oriangle(1.0f,0.44f,0.0f) ;
				line1->SetColor(m_leaderColor);
				Line3D * line2 = new Line3D(boxCenter1, boxCenter2);//辅助线
				line2->SetColor(m_measureColor1);

				Line3D * line3 = new Line3D(boxCenter2Project, boxCenter2);//测量线
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
				if (distance >= 0.0f)
				{
					vector<float> values;
					values.push_back(distance);
					string unit;
					MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
						, values, unit);
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
				float typeStrTextLength = language.GetCurrentTypeValueLength("FaceFaceDistance");
				textLength.push_back(typeStrTextLength);

				ComText* ctDistance = new ComText();
				CText* tDistance = new CText;
				tDistance->SetText(distanceStr);
				tDistance->SetLanguageType("FaceFaceDistance");
				ctDistance->AddCText(tDistance);
				measure->m_ComTexts.push_back(ctDistance);

				float maxTextLength = *std::max_element(textLength.begin(), textLength.end());
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
					rect2L, m_measureColor1, wt, wt, blk, typestr, distanceStr , fntSize, true);

				ImageBoard* imageboard = NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
					(maxTextLength + rect2L) / 100.0f, 1);
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

		IShape * firstShape = scene->GetShape(firstLineId);
		IShape * secondShape = scene->GetShape(secondLineId);

		if (firstShape && secondShape && firstShape->GetType() == SHAPE_EDGE
			&& secondShape->GetType() == SHAPE_EDGE)
		{
			LOGI("createLineTOLineAngle step2");
			measure = new MeasureAngle;
			measure->SetMeasureType(Measure::MEASURE_TYPE_LINE_LINE_ANGLE);

			Edge * firstEdge = (Edge*)firstShape;
			Edge * secondEdge = (Edge *)secondShape;

			RefPolyLine * firstLineSet = firstEdge->GetLineData();
			RefPolyLine * secondLineSet = secondEdge->GetLineData();

			GeometryAttribute * firstGeo = firstEdge->GetGeoAttribute();
			GeometryAttribute * secondGeo = secondEdge->GetGeoAttribute();

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
				LineAttribute firstLineAttribute = *((LineAttribute *)firstGeo);
				LineAttribute secondLineAttribute = *((LineAttribute *)secondGeo);

				Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstEdge);
				Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(
					secondEdge);
				GeometryHelper::Transform(firstLineAttribute, worldMatrix1);
				GeometryHelper::Transform(secondLineAttribute, worldMatrix2);

				float angle;
				MeasureCalculateHelper::LineLineAngle(firstLineAttribute,
					secondLineAttribute, angle);
				LOGI("line line degree is %f", angle);
				//angle = M_RADTODEG * angle;
				//camera射线
				Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

				Vector3 line1Start = firstLineAttribute.GetStartPoint();
				Vector3 line1End = firstLineAttribute.GetEndPoint();
				//将夹角转换到0-π/2
				if (angle > 90.0)
				{
					Vector3 temp;
					temp = line1End;
					line1End = line1Start;
					line1Start = temp;
					angle = 180.0 - angle;
				}
				Vector3 line1Vector = line1End - line1Start;
				//Vector3 line1Center = (line1Start+line1End)/2;
				Vector3 line2Start = secondLineAttribute.GetStartPoint();
				Vector3 line2End = secondLineAttribute.GetEndPoint();
				Vector3 line2Center = (line2Start + line2End) / 2;
				//平移线1，使其与线2相交
				Vector3 removeLine1End = line2Start + line1Vector;

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
				Line3D * line4 = new Line3D(line2Start, removeLine1End);//线1平移线
				Color line4Color = Color::YELLOW;
				line4->SetColor(m_exLineColor);
				line4->SetName("exLine");

				Line3D * line5 = new Line3D(line1Start, line2Start);//延长线
			//	Color line4Color = Color::BLACK;
				line5->SetColor(m_exLineColor);
				line5->SetName("exLine");

				Line3D * line6 = new Line3D(line1End, removeLine1End);//延长线
				//Color line4Color = Color::BLACK;
				line6->SetColor(m_exLineColor);
				line6->SetName("exLine");

				vector<Vector3> linesTemp;
				MeasureDisplayHelper::CreateAngleMark(line2Start, removeLine1End, line2Start, line2End,
					angle, linesTemp);
				Line3D *line1;
				if (linesTemp.size() > 0)
				{
					for (int i = 0; i < linesTemp.size() - 1; i++)
					{
						Line3D * lineTemp = new Line3D(linesTemp[i],
							linesTemp[i + 1]);
						lineTemp->SetColor(m_measureColor1);
						measure->AddLine(lineTemp);
					}
					if ((angle > 0.0f && angle < 90.0f) || (angle > 90.0f && angle < 180.0f))
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
				float typeStrTextLength = language.GetCurrentTypeValueLength("LineLineAngle");
				textLength.push_back(typeStrTextLength);

				float maxTextLength = *std::max_element(textLength.begin(), textLength.end());
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

				ImageBoard* imageboard = NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
					(maxTextLength + rect2L) / 100.0f, 1);
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

		IShape * firstShape = scene->GetShape(firstFaceId);
		IShape * secondShape = scene->GetShape(secondFaceId);


		if (firstShape && secondShape && firstShape->GetType() == SHAPE_FACE
			&& secondShape->GetType() == SHAPE_FACE)
		{
			LOGI("createFaceTOFaceAngle step2");
			measure = new MeasureAngle;
			measure->SetMeasureType(Measure::MEASURE_TYPE_FACE_FACE_ANGLE);

			Face * firstFace = dynamic_cast<Face*>(firstShape);
			Face * secondFace = dynamic_cast<Face*>(secondShape);

			Mesh * firstMeshData = (Mesh*)firstFace->GetData();
			Mesh * secondMeshData = (Mesh *)secondFace->GetData();

			GeometryAttribute * firstGeo = firstFace->GetGeoAttribute();
			GeometryAttribute * secondGeo = secondFace->GetGeoAttribute();

			if (firstGeo != NULL
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE
				&& secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
			{
				LOGI("createFaceTOFaceAngle step3");
				PlaneFaceAttribute firstPlaneFaceAttribute =
					*((PlaneFaceAttribute *)firstGeo);
				PlaneFaceAttribute secondPlaneFaceAttribute =
					*((PlaneFaceAttribute *)secondGeo);

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
				float typeStrTextLength = language.GetCurrentTypeValueLength("FaceFaceAngle");
				textLength.push_back(typeStrTextLength);
				float maxTextLength = *std::max_element(textLength.begin(), textLength.end());

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
				ImageBoard* imageboard = NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
					(maxTextLength + rect2L) / 100.0f, 1);
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

		IShape * firstShape = scene->GetShape(firstLineId);
		IShape * secondShape = scene->GetShape(secondFaceId);

		if (firstShape && secondShape && firstShape->GetType() == SHAPE_EDGE
			&& secondShape->GetType() == SHAPE_FACE)
		{
			LOGI("createLineTOFaceAngle step2");
			measure = new MeasureAngle;
			measure->SetMeasureType(Measure::MEASURE_TYPE_LINE_FACE_ANGLE);

			Edge * firstEdge = (Edge*)firstShape;
			Face * secondFace = dynamic_cast<Face*>(secondShape);

			RefPolyLine * firstLineSet = firstEdge->GetLineData();

			Mesh * secondMeshData = (Mesh *)secondFace->GetData();

			GeometryAttribute * firstGeo = firstEdge->GetGeoAttribute();
			GeometryAttribute * secondGeo = secondFace->GetGeoAttribute();

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
				LineAttribute firstLineAttribute = *((LineAttribute *)firstGeo);
				PlaneFaceAttribute secondPlaneFaceAttribute =
					*((PlaneFaceAttribute *)secondGeo);

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
				float typeStrTextLength = language.GetCurrentTypeValueLength("LineFaceAngle");
				textLength.push_back(typeStrTextLength);
				float maxTextLength = *std::max_element(textLength.begin(), textLength.end());
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

				ImageBoard* imageboard = NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
					(maxTextLength + rect2L) / 100.0f, 1);
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

		Shape* firstShape = (Shape*)scene->GetShape(shapeId);

		if (firstShape && firstShape->GetType() == SHAPE_POINT_HANDLE)
		{
			LOGI("C++ createPntProperty");
			HandlerPoint * firstPnt = (HandlerPoint *)firstShape;

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
			string zStr = M3DTOOLS::floatToString(fCoordinate.m_z);
			string typestr = "点坐标";
			string coordinateStr = xStr + yStr + zStr;
			//根据整个场景包围盒的大小来确定文字的大小
			float fontSize = scene->GetSceneBox().Length() / 2;
			LOGI("point property fontSize %f", fontSize);
			//将线加入测量对象中
			measure->AddLine(line1);
			measure->AddPoint(point1);


			Shape2DSet * shape2DSet = new Shape2DSet();
			float fntSize = 40;
			Vector2 rect1Start(1.0, 1.0);
			Vector2 rect1End((typestr.length() - 1)*fntSize / 2 + fntSize, fntSize*2.5f);
			float rect2L = fntSize*coordinateStr.length() / 2.0 + fntSize / 2;
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
				((typestr.length() - 1)*fntSize / 2 + fntSize + rect2L) / 100.0, fntSize*2.5f / 100.0);
			measure->AddImage(imageboard);

		}
		return measure;
	}

	Measure* MeasureFactory::createLineProperty(int shapeId, Vector2& screenPnt,
		SceneManager* scene)
	{
		Measure* measure = NULL;

		Shape* firstShape = (Shape*)scene->GetShape(shapeId);
		//如果的shape对象存在
		if (firstShape && firstShape->GetType() == SHAPE_EDGE)
		{
			Edge* edge = (Edge*)firstShape;

			measure = new MeasureProperty;

			measure->SetMeasureType(Measure::MEASURE_TYPE_LINE_LENGTH);
			//得到边界线数据
			RefPolyLine * m_lineSet = edge->GetLineData();

			GeometryAttribute* geo = edge->GetGeoAttribute();
			///暂时仅支持，点到直线距离的测量
			if (geo != NULL && geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE)
			{
				LineAttribute lineAttribute = *((LineAttribute*)geo);

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

				ImageBoard* imageboard = NULL;
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

		Shape* firstShape = (Shape*)scene->GetShape(shapeId);
		//如果的shape对象存在
		if (firstShape && firstShape->GetType() == SHAPE_EDGE)
		{
			Edge* edge = (Edge*)firstShape;

			measure = new MeasureProperty;

			measure->SetMeasureType(Measure::MEASURE_TYPE_CRICLE_PROPERTY);
			//得到边界线数据
			RefPolyLine * m_lineSet = edge->GetLineData();

			GeometryAttribute* geo = edge->GetGeoAttribute();
			///暂时仅支持，点到直线距离的测量
			if (geo != NULL && geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_ELLIPSE)
			{
				EllipseAttribute ellipseAttribute = *((EllipseAttribute*)geo);

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
					it != pntPoints.begin() + offset + dataLength; it = it + 2)
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
				Line3D* line2 = new Line3D(circleCenter, pntStartPoint);//半径线
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

				ImageBoard* imageboard = NULL;
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

		Shape* firstShape = (Shape*)scene->GetShape(shapeId);
		//如果的shape对象存在
		if (firstShape && firstShape->GetType() == SHAPE_FACE)
		{
			Face* face = (Face*)firstShape;

			measure = new MeasureProperty;

			measure->SetMeasureType(Measure::MEASURE_TYPE_FACE_PROPERTY);
			//得到边界线数据
			Mesh* meshData = (Mesh*)face->GetData();
			GeometryAttribute* geo = face->GetGeoAttribute();

			//计算面积
			float area = 0.0f;
			MeasureCalculateHelper::faceArea(meshData, area);

			///暂时仅支持，点到直线距离的测量
			if (geo != NULL && geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
			{
				LOGI("planceface measure");
				PlaneFaceAttribute planeFaceAttribute = *((PlaneFaceAttribute*)geo);

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

				ImageBoard* imageboard = NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
					6, 1);
				measure->AddImage(imageboard);
			}
			else if (geo != NULL
				&& geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_REVOLUTIONFACE)
			{
				RevolutionFaceAttribute revolutionFaceAttribute =
					*((RevolutionFaceAttribute*)geo);

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

				ImageBoard* imageboard = NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
					6, 1);
				measure->AddImage(imageboard);
			}
			else if (geo != NULL
				&& geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_CYLINDERFACE)
			{
				CylinderFaceAttribute revolutionFaceAttribute =
					*((CylinderFaceAttribute*)geo);

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
				ImageBoard* imageboard = NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
					6, 1);
				measure->AddImage(imageboard);
			}
			else if (geo != NULL
				&& geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_CONICALFACE)
			{
				ConicalFaceAttribute revolutionFaceAttribute =
					*((ConicalFaceAttribute*)geo);

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

				ImageBoard* imageboard = NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
					6, 1);
				measure->AddImage(imageboard);
			}
			else if (geo != NULL
				&& geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_SPHEREFACE)
			{
				SphereFaceAttribute revolutionFaceAttribute =
					*((SphereFaceAttribute*)geo);

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
				ImageBoard* imageboard = NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
					6, 1);
				measure->AddImage(imageboard);
			}
			else if (geo != NULL
				&& geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_TOROIDALFACE)
			{
				ToroidalFaceAttribute revolutionFaceAttribute =
					*((ToroidalFaceAttribute*)geo);

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

				ImageBoard* imageboard = NULL;
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

				ImageBoard* imageboard = NULL;
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

		Shape* firstShape = (Shape*)scene->GetShape(shapeId);
		//如果的shape对象存在
		if (firstShape && firstShape->GetType() == SHAPE_MODEL)
		{
			Model * model = (Model*)firstShape;

			measure = new MeasureProperty;

			measure->SetMeasureType(Measure::MEASURE_TYPE_MODEL_PROPERTY);

			string modelName = model->GetName();
			Color* modelColor = model->GetColor();
			LOGI("red %f green %f blue %f alpha %f", modelColor->m_r, modelColor->m_g, modelColor->m_b, modelColor->m_a);
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


			ImageBoard* imageboard = NULL;
			MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
				6, 2);
			measure->AddImage(imageboard);

		}

		return measure;
	}

	bool MeasureFactory::UpdateMeasureImagePos(Measure* measure, Vector2& screenPnt, SceneManager* scene)
	{

		bool ret = false;
		Vector3 tempPos;

		if (measure != NULL && scene != NULL)
		{
			vector<ImageBoard*>& imageboards = measure->GetImageBoards();

			for (int i = 0; i < imageboards.size(); i++)
			{
				ImageBoard* imageBoard = imageboards.at(i);

				if (imageBoard)
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
			for (int i = 0; i < lines.size(); i++)
			{
				Line3D *line = lines.at(i);
				if (line)
				{
					if (line->GetName() == "MeasureImageLeader"||line->GetName()=="TextImageLeader")
					{
						scene->Lock();

						line->m_EndPoint = tempPos;

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
		SceneManager* scene, string & propertyStr)
	{
		LOGI("MeasureFactory::GetPntProperty BEGIN");
		InternationalManager language;
		language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);
		propertyStr = "";
		IShape* firstShape = scene->GetShape(shapeId);

		//firstShape->ClearProperties();
		if (firstShape && firstShape->GetType() == SHAPE_POINT_HANDLE)
		{
			LOGI("C++ createPntProperty");
			HandlerPoint * firstPnt = (HandlerPoint *)firstShape;

			Vector3 fCoordinate = firstPnt->GetPosition();
			string xStr = M3DTOOLS::floatToString(fCoordinate.m_x) + ", ";
			string yStr = M3DTOOLS::floatToString(fCoordinate.m_y) + ", ";
			string zStr = M3DTOOLS::floatToString(fCoordinate.m_z);
			string typestr = language.GetCurrentTypeValue("PointPropertiesCoordinater");
			string coordinateStr = xStr + yStr + zStr;

			firstPnt->ClearProperties();
			firstPnt->AddProperty(typestr, coordinateStr);

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
		SceneManager* scene, string & propertyStr)
	{
		LOGI(" MeasureFactory::GetLineProperty BEGIN");
		InternationalManager language;
		language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);
		propertyStr = "";
		IShape* firstShape = scene->GetShape(shapeId);
		//firstShape->ClearProperties();
		//如果的shape对象存在
		if (firstShape && firstShape->GetType() == SHAPE_EDGE)
		{
			Edge* edge = (Edge*)firstShape;
			//得到边界线数据
			RefPolyLine * m_lineSet = edge->GetLineData();

			GeometryAttribute* geo = edge->GetGeoAttribute();
			///暂时仅支持，点到直线距离的测量
			if (geo != NULL && geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE)
			{
				LineAttribute lineAttribute = *((LineAttribute*)geo);

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
				EllipseAttribute ellipseAttribute = *((EllipseAttribute*)geo);

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
				MeasureCalculateHelper::PolyLineLength(m_lineSet, length);

				string lengthstr = M3DTOOLS::floatToString(length);
				string typestr = language.GetCurrentTypeValue("LinePropertiesLength");

				edge->AddProperty(typestr, lengthstr);
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
		SceneManager* scene, string & propertyStr)
	{
		//	LOGI(" MeasureFactory::GetCircleProperty BEGIN");
		//	InternationalManager language;
		//	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);
		//	propertyStr = "";
		//	Shape* firstShape = (Shape*)scene->GetShape(shapeId);
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
		SceneManager* scene, string & propertyStr)
	{

		LOGI(" MeasureFactory::GetFaceProperty BEGIN");
		InternationalManager language;
		language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);
		propertyStr = "";
		IShape* firstShape = dynamic_cast<IShape*>(scene->GetShape(shapeId));
		//firstShape->ClearProperties();
		//如果的shape对象存在
		if (firstShape && firstShape->GetType() == SHAPE_FACE)
		{
			//------------此处先保留，以后改进 TODO
			Face* face = dynamic_cast<Face*>(firstShape);
			//得到边界线数据
			Mesh* meshData = (Mesh*)face->GetData();
			GeometryAttribute* geo = face->GetGeoAttribute();
			//计算面积
			//float area = 0.0f;
			//MeasureCalculateHelper::faceArea(meshData, area);

			//int triangleNumber = meshData->GetDataLength()/3;

			//string triangleNumberStr = M3DTOOLS::IntToStringtriangleNumber);
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
		SceneManager* scene, string & propertyStr)
	{

		LOGI(" MeasureFactory::GetModelProperty BEGIN");
		//InternationalManager language;
	//	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);
		propertyStr = "";
		IShape* firstShape = scene->GetShape(shapeId);
		//firstShape->ClearProperties();
		//如果的shape对象存在
		if (firstShape && firstShape->GetType() == SHAPE_MODEL)
		{
			LOGI("firstShape && firstShape->GetType() == SHAPE_MODEL");
			Model * model = dynamic_cast<Model*>(firstShape);
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
			Model * model = (Model*)scene->GetModel();
			propertyStr = model->GetProperties();
		}
		//LOGI("propertyStr = %s", propertyStr.c_str());
		LOGI(" MeasureFactory::GetModelProperty END");

	}

	void MeasureFactory::GetModelStatistics(int shapeId,
		SceneManager* scene, string & statisticsStr)
	{
		LOGI(" MeasureFactory::GetModelStatistics BEGIN");
		statisticsStr = "";
		IShape* firstShape = scene->GetShape(shapeId);
		//firstShape->ClearProperties();
		//如果的shape对象存在
		if (firstShape && firstShape->GetType() == SHAPE_MODEL)
		{
			LOGI("firstShape && firstShape->GetType() == SHAPE_MODEL");
			Model * model = dynamic_cast<Model*>(firstShape);

			statisticsStr = model->GetStatistics();

		}
		else
		{
			Model * model = (Model*)scene->GetModel();
			statisticsStr = model->GetStatistics();
		}
		//LOGI("propertyStr = %s", statisticsStr.c_str());
		LOGI(" MeasureFactory::GetModelStatistics END");
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

		Shape * firstShape = (Shape*)scene->GetShape(firstLineId);
		Shape * secondShape = (Shape*)scene->GetShape(secondFaceId);
		Shape * forthShape = (Shape*)scene->GetShape(forthPntId);

		if (firstShape && secondShape && forthShape&& firstShape->GetType() == SHAPE_EDGE
			&& secondShape->GetType() == SHAPE_FACE && forthShape->GetType() == SHAPE_POINT_HANDLE)
		{
			LOGI("createLineTOFaceDistance step2");
			measure = new MeasureDistance;
			measure->SetMeasureType(Measure::MEASURE_TYPE_LINE_FACE_DISTANCE);

			Edge * firstEdge = (Edge*)firstShape;
			Face * secondFace = (Face *)secondShape;

			HandlerPoint * forthPnt = (HandlerPoint *)forthShape;
			Vector3 forthCoordinate = forthPnt->GetPosition();

			RefPolyLine * firstLineSet = firstEdge->GetLineData();
			Mesh * secondMeshData = (Mesh *)secondFace->GetData();

			GeometryAttribute * firstGeo = firstEdge->GetGeoAttribute();
			GeometryAttribute * secondGeo = secondFace->GetGeoAttribute();
			if (firstGeo != NULL
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE
				&& secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
			{
				LOGI("createLineToFace step3");
				LineAttribute firstLineAttribute = *((LineAttribute *)firstGeo);
				PlaneFaceAttribute secondPlaneFaceAttribute =
					*((PlaneFaceAttribute *)secondGeo);

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
				LOGI("boundingoxmin %s max %s", boundingBox.m_min.Tostring().c_str(), boundingBox.m_max.Tostring().c_str());


				Vector3 boxCenter = boundingBox.Center();
				GeometryHelper::Transform(boxCenter, worldMatrix2);

				//camera射线
				Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

				Vector3 line1Start = firstLineAttribute.GetStartPoint();
				Vector3 line1End = firstLineAttribute.GetEndPoint();
				Vector3 lineCenter = (line1Start + line1End) / 2;
				Vector3 center = (lineCenter + boxCenter) / 2;
				Vector3 line1Direction = (line1End - line1Start);
				Vector3 faceNormal = secondPlaneFaceAttribute.GetNormal();
				float markLngth;//表示平面的正方形长度
				vector<float> tempLength;
				tempLength.push_back(boundingBox.GetXLength());
				tempLength.push_back(boundingBox.GetYLength());
				tempLength.push_back(boundingBox.GetZLength());
				sort(tempLength.begin(), tempLength.end()); //排序
				LOGI("boxLength %f %f %f", tempLength[0], tempLength[1], tempLength[2]);
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
				if (distance >= 0.0f)
				{
					vector<float> values;
					values.push_back(distance);
					string unit;
					MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
						, values, unit);
					distanceStr = M3DTOOLS::floatToString(values.at(0))+ unit;

				}
				else
				{
					LOGI("createLineToFace step3 Error");
					delete measure;
					measure = NULL;
					return measure;//返回空 不能测量
				}


				string typestr = language.GetCurrentTypeValue("LineFaceDistance");
				float distanctTextLength = language.GetCurrentTypeValueLength(
					"LineFaceDistance");
				textLength.push_back(distanctTextLength);

				ComText* ctDistance = new ComText();
				CText* tDistance = new CText;
				tDistance->SetText(distanceStr);
				tDistance->SetLanguageType("LineFaceDistance");
				ctDistance->AddCText(tDistance);
				measure->m_ComTexts.push_back(ctDistance);

				float maxTextLength = *std::max_element(textLength.begin(), textLength.end());

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

				ImageBoard* imageboard = NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
					(maxTextLength + rect2L) / 100.0f, 1);
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

		Shape * firstShape = (Shape*)scene->GetShape(firstPntId);
		Shape * secondShape = (Shape*)scene->GetShape(secondFaceId);
		Shape * forthShape = (Shape*)scene->GetShape(forthPntId);

		if (firstShape && secondShape&& forthShape && firstShape->GetType() == SHAPE_POINT_HANDLE
			&& secondShape->GetType() == SHAPE_FACE && forthShape->GetType() == SHAPE_POINT_HANDLE)
		{
			LOGI("createLineTOFaceAngle step2");
			measure = new MeasureDistance;
			measure->SetMeasureType(Measure::MEASURE_TYPE_PNT_FACE_DISTANCE);
			HandlerPoint * firstPnt = (HandlerPoint *)firstShape;
			HandlerPoint * forthPnt = (HandlerPoint *)forthShape;
			Face * secondFace = (Face*)secondShape;
			Vector3 fCoordinate = firstPnt->GetPosition();
			Vector3 forthCoordinate = forthPnt->GetPosition();
			Mesh * secondMeshData = (Mesh*)secondFace->GetData();

			GeometryAttribute * secondGeo = secondFace->GetGeoAttribute();
			if (secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
			{
				PlaneFaceAttribute secondPlaneFaceAttribute =
					*((PlaneFaceAttribute *)secondGeo);

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
				Vector3 projectPntMesh, maxPoint;
				MeasureCalculateHelper::PntFaceDistance(modelVec, secondMeshData,
					distance, maxDistance, projectPntMesh, maxPoint);
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


				Line3D * line3 = new Line3D(fCoordinate, projectPntMesh);//最小距离，到有边界的面
				Color line3Color = Color::RED;
				line3->SetColor(m_measureColor3);

				Line3D * line4 = new Line3D(fCoordinate, maxPoint);//最大距离，到有边界的面
				Color line4Color = Color::BLUE;
				line4->SetColor(m_measureColor2);

				Line3D* line5 = new Line3D(projectPntMesh, pntProject);//到边界面最小距离和投影距离的连线
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

				float maxTextLength = *std::max_element(textLength.begin(), textLength.end());



				measure->AddLine(line2);

				measure->AddLine(line5);
				measure->AddPoint(point1);

				vector<float> values;
				values.push_back(Length);
				values.push_back(maxDistance);
				values.push_back(distance);
				string unit;
				MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
					, values, unit);
				string LengthStr = M3DTOOLS::floatToString(values.at(0)) + unit;
				string maxDistanceStr = M3DTOOLS::floatToString(values.at(1)) + unit;
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
				if (distance > 0)
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
				Vector3 projectPnt, maxPoint;
				MeasureCalculateHelper::PntFaceDistance(modelVec, secondMeshData,
					distance, maxDistance, projectPnt, maxPoint);
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

				float maxTextLength = *std::max_element(textLength.begin(), textLength.end());

				//measure->AddLine(line1);
				measure->AddLine(line2);

				measure->AddPoint(point1);


				vector<float> values;
				values.push_back(distance);
				values.push_back(maxDistance);

				string unit;
				MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
					, values, unit);
				string distanceStr = M3DTOOLS::floatToString(values.at(0)) + unit;
				string maxDistanceStr = M3DTOOLS::floatToString(values.at(1)) + unit;

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
				if (distance > 0)
				{
					measure->AddLine(line3);
					measure->AddLine(line4);
					measure->AddPoint(point2);
					measure->AddPoint(point3);
					MeasureDisplayHelper::createRectImage(shape2DSet, rect1Start,
						rect1End, rect2L, m_measureColor1, wt, wt, blk,
						minDistanceTypeStr, distanceStr , fntSize, false);

					Vector2 rect2Start(1.0, 100.0);
					Vector2 rect2End(maxTextLength, 200.0);
					MeasureDisplayHelper::createRectImage(shape2DSet, rect2Start,
						rect2End, rect2L, m_measureColor2, wt, wt, blk,
						maxDistanceTypeStr, maxDistanceStr , fntSize, true);

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

		IShape * firstShape = dynamic_cast<IShape*>(scene->GetShape(firstFaceId));
		IShape * secondShape = dynamic_cast<IShape*>(scene->GetShape(secondFaceId));
		IShape * thirdShape = (IShape*)scene->GetShape(thirdPntId);
		IShape * forthShape = (IShape*)scene->GetShape(forthPntId);

		if (firstShape && secondShape && thirdShape&& forthShape&&
			firstShape->GetType() == SHAPE_FACE
			&& secondShape->GetType() == SHAPE_FACE
			&&thirdShape->GetType() == SHAPE_POINT_HANDLE
			&&forthShape->GetType() == SHAPE_POINT_HANDLE)
		{
			LOGI("createFaceTOFaceAngle step2");
			measure = new MeasureDistance;
			measure->SetMeasureType(Measure::MEASURE_TYPE_FACE_FACE_DISTANCE);

			Face * firstFace = dynamic_cast<Face*>(firstShape);
			Face * secondFace = dynamic_cast<Face*>(secondShape);

			Mesh * firstMeshData = (Mesh*)firstFace->GetData();
			Mesh * secondMeshData = (Mesh *)secondFace->GetData();

			HandlerPoint * thirdPnt = (HandlerPoint *)thirdShape;
			Vector3 thirdCoordinate = thirdPnt->GetPosition();
			HandlerPoint * forthPnt = (HandlerPoint *)forthShape;
			Vector3 forthCoordinate = forthPnt->GetPosition();

			GeometryAttribute * firstGeo = firstFace->GetGeoAttribute();
			GeometryAttribute * secondGeo = secondFace->GetGeoAttribute();

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
					*((PlaneFaceAttribute *)firstGeo);
				PlaneFaceAttribute secondPlaneFaceAttribute =
					*((PlaneFaceAttribute *)secondGeo);

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
				Plane face1Plane(firstPlaneFaceAttribute.GetNormal(), thirdCoordinate);
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

				Line3D * line3 = new Line3D(boxCenter2Project, forthCoordinate);//测量线
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
				if (distance >= 0.0f)
				{
					vector<float> values;
					values.push_back(distance);
					string unit;
					MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
						, values, unit);
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
				float typeStrTextLength = language.GetCurrentTypeValueLength("FaceFaceDistance");
				textLength.push_back(typeStrTextLength);

				ComText* ctDistance = new ComText();
				CText* tDistance = new CText;
				tDistance->SetText(distanceStr);
				tDistance->SetLanguageType("FaceFaceDistance");
				ctDistance->AddCText(tDistance);
				measure->m_ComTexts.push_back(ctDistance);

				float maxTextLength = *std::max_element(textLength.begin(), textLength.end());
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
					rect2L, m_measureColor1, wt, wt, blk, typestr, distanceStr , fntSize, true);

				ImageBoard* imageboard = NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
					(maxTextLength + rect2L) / 100.0f, 1);
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

		//Shape * firstShape = (Shape*)scene->GetShape(firstFaceId);
		IShape * firstShape = scene->GetShape(firstFaceId);
		IShape * secondShape = scene->GetShape(secondFaceId);

		//Shape * secondShape = (Shape*)scene->GetShape(secondFaceId);
		IShape * thirdShape = scene->GetShape(thirdPntId);
		IShape * forthShape = scene->GetShape(forthPntId);

		if (firstShape && secondShape &&thirdShape&& forthShape
			&&firstShape->GetType() == SHAPE_FACE
			&& secondShape->GetType() == SHAPE_FACE
			&&thirdShape->GetType() == SHAPE_POINT_HANDLE
			&&forthShape->GetType() == SHAPE_POINT_HANDLE)
		{
			LOGI("createFaceTOFaceAngle step2");
			measure = new MeasureAngle;
			measure->SetMeasureType(Measure::MEASURE_TYPE_FACE_FACE_ANGLE);

			//Face * firstFace = (Face*) firstShape;
			Face * firstFace = dynamic_cast<Face*>(firstShape);

			//Face * secondFace = (Face *) secondShape;
			Face * secondFace = dynamic_cast<Face*>(secondShape);


			Mesh * firstMeshData = (Mesh*)firstFace->GetData();
			Mesh * secondMeshData = (Mesh *)secondFace->GetData();

			HandlerPoint * thirdPnt = (HandlerPoint *)thirdShape;
			Vector3 thirdCoordinate = thirdPnt->GetPosition();
			HandlerPoint * forthPnt = (HandlerPoint *)forthShape;
			Vector3 forthCoordinate = forthPnt->GetPosition();

			GeometryAttribute * firstGeo = firstFace->GetGeoAttribute();
			GeometryAttribute * secondGeo = secondFace->GetGeoAttribute();

			if (firstGeo != NULL
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE
				&& secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
			{
				LOGI("createFaceTOFaceAngle step3");
				PlaneFaceAttribute firstPlaneFaceAttribute =
					*((PlaneFaceAttribute *)firstGeo);
				PlaneFaceAttribute secondPlaneFaceAttribute =
					*((PlaneFaceAttribute *)secondGeo);

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
				float typeStrTextLength = language.GetCurrentTypeValueLength("FaceFaceAngle");
				textLength.push_back(typeStrTextLength);
				float maxTextLength = *std::max_element(textLength.begin(), textLength.end());

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
				ImageBoard* imageboard = NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
					(maxTextLength + rect2L) / 100.0f, 1);
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

		IShape * firstShape = scene->GetShape(firstLineId);
		IShape * secondShape = scene->GetShape(secondFaceId);
		IShape * forthShape = scene->GetShape(forthPntId);

		if (firstShape && secondShape && forthShape
			&&firstShape->GetType() == SHAPE_EDGE
			&& secondShape->GetType() == SHAPE_FACE
			&&forthShape->GetType() == SHAPE_POINT_HANDLE)
		{
			LOGI("createLineTOFaceAngle step2");
			measure = new MeasureAngle;
			measure->SetMeasureType(Measure::MEASURE_TYPE_LINE_FACE_ANGLE);

			Edge * firstEdge = (Edge*)firstShape;
			Face * secondFace = (Face *)secondShape;

			RefPolyLine * firstLineSet = firstEdge->GetLineData();

			Mesh * secondMeshData = (Mesh *)secondFace->GetData();
			HandlerPoint * forthPnt = (HandlerPoint *)forthShape;
			Vector3 forthCoordinate = forthPnt->GetPosition();

			GeometryAttribute * firstGeo = firstEdge->GetGeoAttribute();
			GeometryAttribute * secondGeo = secondFace->GetGeoAttribute();

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
				LineAttribute firstLineAttribute = *((LineAttribute *)firstGeo);
				PlaneFaceAttribute secondPlaneFaceAttribute =
					*((PlaneFaceAttribute *)secondGeo);

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
				float typeStrTextLength = language.GetCurrentTypeValueLength("LineFaceAngle");
				textLength.push_back(typeStrTextLength);
				float maxTextLength = *std::max_element(textLength.begin(), textLength.end());
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

				ImageBoard* imageboard = NULL;
				MeasureDisplayHelper::addImageToMemory(scene, imageboard, shape2DSet, pntInPlane,
					(maxTextLength + rect2L) / 100.0f, 1);
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

	Measure* MeasureFactory::CreateTmpDistanceMeasure(int firstShapeID, int secondShapID,
		int type, SceneManager* scene)
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
			measure = createDesignerPntToPntDistance(firstShapeID, secondShapID, scene);
		}
		else if (type == Measure::MEASURE_TYPE_PNT_LINE_DISTANCE)
		{
			measure = createDesignerPntToLineDistance(firstShapeID, secondShapID, scene);
		}
		else if (type == Measure::MEASURE_TYPE_LINE_LINE_DISTANCE)
		{
			measure = createDesignerLineToLineDistance(firstShapeID, secondShapID, scene);
		}
		else if (type == Measure::MEASURE_TYPE_SHAFT_SHAFT_DISTANCE)
		{
			measure = createDesignerShaftToShaftDistance(firstShapeID, secondShapID, scene);
		}
		else if (type == Measure::MEASURE_TYPE_CENTER_CENTER_DISTANCE)
		{
			measure = createDesignerCenterToCenterDistance(firstShapeID, secondShapID, scene);
		}
		else if (type == Measure::MEASURE_TYPE_LINE_LENGTH)
		{
			measure = createDesignerLineDistance(firstShapeID, scene);
		}
		else if (type == Measure::MEASURE_TYPE_CRICLE_PROPERTY)
		{
			measure = createDesignerArcDistance(firstShapeID, scene);
		}

		AddMeasureToScene(scene, measure);

		if (measure)
		{
			measure->SetFrontShow(true);
		}

		return measure;
	}

	Measure* MeasureFactory::createDesignerPntToPntDistance(int firstPntId, int secondPntId, SceneManager* scene)
	{
		Measure* measure = NULL;
		InternationalManager  language;
		language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);

		IShape* firstShape = scene->GetShape(firstPntId);
		IShape* secondShape = scene->GetShape(secondPntId);

		//如果的shape对象存在
		if (firstShape && secondShape && firstShape->GetType() == SHAPE_POINT_HANDLE
			&& secondShape->GetType() == SHAPE_POINT_HANDLE)
		{
			measure = new MeasureDistance;

			measure->SetMeasureType(Measure::MEASURE_TYPE_PNT_PNT_DISTANCE);

			HandlerPoint* firstPnt = (HandlerPoint*)firstShape;
			HandlerPoint* secondPnt = (HandlerPoint*)secondShape;

			Vector3 fCoordiante = firstPnt->GetPosition();
			Vector3 sCoordiante = secondPnt->GetPosition();

			float lineLength = (fCoordiante - sCoordiante).Length();
			Vector3 directionVector = sCoordiante - fCoordiante;

			
			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(0,0);
			Vector3 zDirection = cameraRay.GetDirection();

			directionVector = zDirection.CrossProduct(directionVector);
			directionVector.Normalize();

			Vector3 thirdCoord = fCoordiante + directionVector*lineLength * 0.5;
			Vector3 fourCoord = sCoordiante + directionVector*lineLength * 0.5;

			Color line1Color = Color::RED;
			Line3D* line1 = new Line3D(fCoordiante, sCoordiante);
			line1->SetName("dottedLine");
			line1->SetLineWidth(1);
			line1->SetColor(line1Color);
			Line3D* line2 = new Line3D(fCoordiante, thirdCoord);//文本引线
			line2->SetColor(line1Color);
			Line3D* line3 = new Line3D(sCoordiante, fourCoord);//文本引线
			line3->SetColor(line1Color);
			Line3D* line4 = new Line3D(thirdCoord, fourCoord);//文本引线
			line4->SetColor(line1Color);

			measure->AddLine(line1);
			measure->AddLine(line2);
			measure->AddLine(line3);
			measure->AddLine(line4);
			//return measure;
			////camera射线
			//Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
			//	screenPnt.m_x, screenPnt.m_y);

			//Vector3 lineCenter = (fCoordiante + sCoordiante) / 2;

			////构造投影平面
			//Plane projPlane(cameraRay.GetDirection(), lineCenter);

			//Vector3 pntInPlane = projPlane.Project(cameraRay.GetOrigin());

			//构造线
			//Line3D* line1 = new Line3D(fCoordiante, sCoordiante);
			//Color line1Color = Color::BLACK;
			//line1->SetColor(m_measureColor1);
			//Line3D* line2 = new Line3D(lineCenter, pntInPlane);//文本引线
			//												   //Color oriangle(1.0f,0.44f,0.0f) ;
			//line2->SetColor(m_leaderColor);
			//line2->SetName("MeasureImageLeader");

			//给线设置箭头末端符号
			//		line1->SetStartArrow(1);
			//		line1->SetEndArrow(1);

			//构造显示文本

			//测量结果单位转换
			vector<float> valuse;
			valuse.push_back(lineLength);
			string unit;
			MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
				, valuse, unit);
			LOGI("SVIEW::Parameters::Instance()->m_measureUnitStyle %d", SVIEW::Parameters::Instance()->m_measureUnitStyle);
			string lengthstr = M3DTOOLS::floatToString(valuse.at(0)) + unit;
			//根据整个场景包围盒的大小来确定文字的大小
			float fontSize = scene->GetSceneBox().Length() / 20;

			//解决协同中文本读取
			ComText* ct = new ComText();
			CText* t = new CText;
			t->SetLanguageType("PointPointDistance");
			t->SetText(lengthstr);
			ct->AddCText(t);
			measure->m_ComTexts.push_back(ct);

			Vector3 pntInPlane;
			pntInPlane = (thirdCoord + fourCoord) / 2;

			vector<Texts2D*> temptext;
			Texts2D *title = new Texts2D;
			title->m_size = 12.0f;
			title->m_texts = "内容";
			temptext.push_back(title);

			Texts2D *content = new Texts2D;
			content->m_size = 12.0f;
			content->m_texts = lengthstr;
			temptext.push_back(content);

			ImageBoard * imageBroad = MeasureDisplayHelper::createNoteTextsImageN(scene, temptext, pntInPlane);
			measure->AddImage(imageBroad);
		}
		else
		{
			LOGE("MeasureFactory::createPntToPntDistance shape is NULL");
		}

		return measure;
	}

	Measure* MeasureFactory::createDesignerPntToLineDistance(int lineId, int pntId, SceneManager* scene)
	{
		Measure* measure = NULL;

		IShape* lineShape = scene->GetShape(lineId);
		IShape* pntShape = scene->GetShape(pntId);
	
		//如果的shape对象存在
		if (lineShape && lineShape->GetType() == SHAPE_EDGE)
		{
			measure = new MeasureDistance;

			measure->SetMeasureType(Measure::MEASURE_TYPE_PNT_LINE_DISTANCE);

			Edge* lineEdge = (Edge*)lineShape;

			//得到边界线数据
			RefPolyLine * m_lineSet = lineEdge->GetLineData();

			GeometryAttribute* geo = lineEdge->GetGeoAttribute();
			///暂时仅支持，点到直线距离的测量
			if (geo && !pntShape && geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE)
			{
				LineAttribute lineAttribute = *((LineAttribute*)geo);

				Matrix3x4 worldMatrix = ShapeHelper::GetShapeWorldMatrix(
					lineEdge);

				GeometryHelper::Transform(lineAttribute, worldMatrix);

				float distance = 0;

				Vector3 pntStartPoint = lineAttribute.GetStartPoint(); //!< Line起点
				Vector3 pntEndPoint = lineAttribute.GetEndPoint(); //!< Line终点

				distance = (pntStartPoint - pntEndPoint).Length();

				Vector3 directionVector = pntStartPoint - pntEndPoint;

				Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(0, 0);
				Vector3 zDirection = cameraRay.GetDirection();

				directionVector = zDirection.CrossProduct(directionVector);
				directionVector.Normalize();

				Vector3 thirdCoord = pntStartPoint + directionVector*distance * 0.5;
				Vector3 fourCoord = pntEndPoint + directionVector*distance * 0.5;

				Color line1Color = Color::RED;
				Line3D* line1 = new Line3D(pntStartPoint, pntEndPoint);
				line1->SetColor(line1Color);
				Line3D* line2 = new Line3D(pntStartPoint, thirdCoord);//文本引线
				line2->SetColor(line1Color);
				Line3D* line3 = new Line3D(pntEndPoint, fourCoord);//文本引线
				line3->SetColor(line1Color);
				Line3D* line4 = new Line3D(thirdCoord, fourCoord);//文本引线
				line4->SetColor(line1Color);

				measure->AddLine(line1);
				measure->AddLine(line2);
				measure->AddLine(line3);
				measure->AddLine(line4);

				Point* pnt1 = new Point(pntStartPoint);
				pnt1->SetDrawType(1);
				pnt1->SetSize(0.8f);
				Point* pnt2 = new Point(pntEndPoint);
				pnt2->SetDrawType(1);
				pnt2->SetSize(0.8f);

				measure->AddPoint(pnt1);
				measure->AddPoint(pnt2);
	

				//测量结果单位转换
				vector<float> valuse;
				valuse.push_back(distance);
				string unit;
				MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
					, valuse, unit);
				string lengthstr = M3DTOOLS::floatToString(valuse.at(0)) + unit;
				
				//解决协同中文本读取
				ComText* ct = new ComText();
				CText* t = new CText;
				t->SetText(lengthstr);
				ct->AddCText(t);
				measure->m_ComTexts.push_back(ct);

				Vector3 pntInPlane;
				pntInPlane = fourCoord;

				vector<Texts2D*> temptext;
				Texts2D *title = new Texts2D;
				title->m_size = 12.0f;
				title->m_texts = "内容";
				temptext.push_back(title);

				Texts2D *content = new Texts2D;
				content->m_size = 12.0f;
				content->m_texts = lengthstr;
				temptext.push_back(content);

				ImageBoard * imageBroad = MeasureDisplayHelper::createNoteTextsImageN(scene, temptext, pntInPlane);
				measure->AddImage(imageBroad);
			}
			else if (geo && pntShape && geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE 
				&& pntShape->GetType() == SHAPE_POINT_HANDLE)
			{
				LineAttribute lineAttribute = *((LineAttribute*)geo);
				Matrix3x4 worldMatrix = ShapeHelper::GetShapeWorldMatrix(
					lineEdge);

				GeometryHelper::Transform(lineAttribute, worldMatrix);

				Vector3 pntStartPoint = lineAttribute.GetStartPoint(); 
				Vector3 pntEndPoint = lineAttribute.GetEndPoint(); 

				HandlerPoint* pnt = (HandlerPoint*)pntShape;

				Vector3 pntCoordiante = pnt->GetPosition();

				Color line1Color = Color::RED;
				Line3D* line1 = new Line3D(pntStartPoint, pntEndPoint);
				line1->SetColor(line1Color);

				Vector3 closestCoordiant;
				int inLine;
				float distance;

				MeasureCalculateHelper::PntLineDistance(pntCoordiante, lineAttribute,
					distance, inLine, closestCoordiant);

				Vector3 directionVector = pntCoordiante - closestCoordiant;


				Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(0, 0);
				Vector3 zDirection = cameraRay.GetDirection();

				directionVector = zDirection.CrossProduct(directionVector);
				directionVector.Normalize();

				Vector3 thirdCoord = pntCoordiante + directionVector*distance * 0.5;
				Vector3 fourCoord = closestCoordiant + directionVector*distance * 0.5;

				Line3D* line2 = new Line3D(pntCoordiante, closestCoordiant);
				line2->SetName("dottedLine");
				line2->SetLineWidth(1);
				line2->SetColor(line1Color);
				Line3D* line3 = new Line3D(pntCoordiante, thirdCoord);
				line3->SetCanDelete(true);
				line3->SetColor(line1Color);
				Line3D* line4 = new Line3D(closestCoordiant, fourCoord);
				line4->SetCanDelete(true);
				line4->SetColor(line1Color);
				Line3D* line5 = new Line3D(thirdCoord, fourCoord);
				line5->SetCanDelete(true);
				line5->SetColor(line1Color);

				
				Line3D* line6 = NULL;
				Vector3 projecPnt;
				if (!(MeasureCalculateHelper::pntInSegment(closestCoordiant, pntStartPoint, pntEndPoint, projecPnt)))
				{
					line6 = new Line3D(closestCoordiant, projecPnt);
					line6->SetName("dottedLine");
					line6->SetLineWidth(1);
					line6->SetColor(line1Color);
				}

				measure->AddLine(line1);
				measure->AddLine(line2);
				measure->AddLine(line3);
				measure->AddLine(line4);
				measure->AddLine(line5);
				if (line6)
				{
					measure->AddLine(line6);
				}

				Point* pnt1 = new Point(pntStartPoint);
				pnt1->SetDrawType(1);
				pnt1->SetSize(0.8f);
				Point* pnt2 = new Point(pntEndPoint);
				pnt2->SetDrawType(1);
				pnt2->SetSize(0.8f);
				Point* pnt3 = new Point(closestCoordiant);
				pnt3->SetDrawType(1);
				pnt3->SetSize(0.8f);
				Point* pnt4 = new Point(pntCoordiante);
				pnt4->SetDrawType(1);
				pnt4->SetSize(0.8f);
				measure->AddPoint(pnt1);
				measure->AddPoint(pnt2);
				measure->AddPoint(pnt3);
				measure->AddPoint(pnt4);

				//测量结果单位转换
				vector<float> valuse;
				valuse.push_back(distance);
				string unit;
				MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
					, valuse, unit);
				string lengthstr = M3DTOOLS::floatToString(valuse.at(0)) + unit;

				//解决协同中文本读取
				ComText* ct = new ComText();
				CText* t = new CText;
				t->SetText(lengthstr);
				ct->AddCText(t);
				measure->m_ComTexts.push_back(ct);

				Vector3 pntInPlane;
				pntInPlane = fourCoord;

				vector<Texts2D*> temptext;
				Texts2D *title = new Texts2D;
				title->m_size = 12.0f;
				title->m_texts = "内容";
				temptext.push_back(title);

				Texts2D *content = new Texts2D;
				content->m_size = 12.0f;
				content->m_texts = lengthstr;
				temptext.push_back(content);

				ImageBoard * imageBroad = MeasureDisplayHelper::createNoteTextsImageN(scene, temptext, pntInPlane);
				measure->AddImage(imageBroad);
			}
			else
			{
				delete measure;
				measure = NULL;
			}
		}
		else
		{
			LOGE("MeasureFactory::createPntTOLineDistance shape is NULL");
		}

		return measure;
	}

	Measure* MeasureFactory::createDesignerLineToLineDistance(int firstLineId, int secondLineId, SceneManager* scene)
	{
		Measure* measure = NULL;

		IShape* firstShape = scene->GetShape(firstLineId);
		IShape* secondShape = scene->GetShape(secondLineId);

		if (firstShape && !secondShape && firstShape->GetType() == SHAPE_EDGE)
		{
			measure = new MeasureDistance;

			measure->SetMeasureType(Measure::MEASURE_TYPE_LINE_LINE_DISTANCE);

			Edge* firstEdge = (Edge*)firstShape;
			RefPolyLine * firstLineSet = firstEdge->GetLineData();
			GeometryAttribute* firstGeo = firstEdge->GetGeoAttribute();
			///暂时仅支持，点到直线距离的测量
			if (firstGeo && !secondShape && firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE)
			{
				LineAttribute firstLineAttribute = *((LineAttribute*)firstGeo);

				Matrix3x4 worldMatrix = ShapeHelper::GetShapeWorldMatrix(
					firstEdge);

				GeometryHelper::Transform(firstLineAttribute, worldMatrix);

				float distance = 0;

				Vector3 pntStartPoint = firstLineAttribute.GetStartPoint(); //!< Line起点
				Vector3 pntEndPoint = firstLineAttribute.GetEndPoint(); //!< Line终点

				distance = (pntStartPoint - pntEndPoint).Length();

				Vector3 directionVector = pntStartPoint - pntEndPoint;

				Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(0, 0);
				Vector3 zDirection = cameraRay.GetDirection();

				directionVector = zDirection.CrossProduct(directionVector);
				directionVector.Normalize();

				Vector3 thirdCoord = pntStartPoint + directionVector*distance * 0.5;
				Vector3 fourCoord = pntEndPoint + directionVector*distance * 0.5;

				Color line1Color = Color::RED;
				Line3D* line1 = new Line3D(pntStartPoint, pntEndPoint);
				line1->SetColor(line1Color);
				Line3D* line2 = new Line3D(pntStartPoint, thirdCoord);//文本引线
				line2->SetColor(line1Color);
				Line3D* line3 = new Line3D(pntEndPoint, fourCoord);//文本引线
				line3->SetColor(line1Color);
				Line3D* line4 = new Line3D(thirdCoord, fourCoord);//文本引线
				line4->SetColor(line1Color);

				measure->AddLine(line1);
				measure->AddLine(line2);
				measure->AddLine(line3);
				measure->AddLine(line4);

				Point* pnt1 = new Point(pntStartPoint);
				pnt1->SetDrawType(1);
				pnt1->SetSize(0.8f);
				Point* pnt2 = new Point(pntEndPoint);
				pnt2->SetDrawType(1);
				pnt2->SetSize(0.8f);

				measure->AddPoint(pnt1);
				measure->AddPoint(pnt2);


				//测量结果单位转换
				vector<float> valuse;
				valuse.push_back(distance);
				string unit;
				MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
					, valuse, unit);
				string lengthstr = M3DTOOLS::floatToString(valuse.at(0)) + unit;

				//解决协同中文本读取
				ComText* ct = new ComText();
				CText* t = new CText;
				t->SetText(lengthstr);
				ct->AddCText(t);
				measure->m_ComTexts.push_back(ct);

				Vector3 pntInPlane;
				pntInPlane = (thirdCoord + fourCoord) / 2;

				vector<Texts2D*> temptext;
				Texts2D *title = new Texts2D;
				title->m_size = 12.0f;
				title->m_texts = "内容";
				temptext.push_back(title);

				Texts2D *content = new Texts2D;
				content->m_size = 12.0f;
				content->m_texts = lengthstr;
				temptext.push_back(content);

				ImageBoard * imageBroad = MeasureDisplayHelper::createNoteTextsImageN(scene, temptext, pntInPlane);
				measure->AddImage(imageBroad);
			}
			else
			{
				delete measure;
				measure = NULL;
			}
		}
		else if (firstShape && secondShape && firstLineId != secondLineId 
			&& firstShape->GetType() == SHAPE_EDGE
			&& secondShape->GetType() == SHAPE_EDGE)
		{
			measure = new MeasureDistance;

			measure->SetMeasureType(Measure::MEASURE_TYPE_LINE_LINE_DISTANCE);

			Edge * firstEdge = (Edge*)firstShape;
			Edge * secondEdge = (Edge *)secondShape;

			RefPolyLine * firstLineSet = firstEdge->GetLineData();
			RefPolyLine * secondLineSet = secondEdge->GetLineData();

			GeometryAttribute * firstGeo = firstEdge->GetGeoAttribute();
			GeometryAttribute * secondGeo = secondEdge->GetGeoAttribute();
			if (firstGeo && firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE
				&& secondGeo && secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE)
			{
				LineAttribute firstLineAttribute = *((LineAttribute *)firstGeo);
				LineAttribute secondLineAttribute = *((LineAttribute *)secondGeo);

				Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstEdge);
				Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(
					secondEdge);
				GeometryHelper::Transform(firstLineAttribute, worldMatrix1);
				GeometryHelper::Transform(secondLineAttribute, worldMatrix2);

				Vector3 pntStartPoint1 = firstLineAttribute.GetStartPoint();
				Vector3 pntEndPoint1 = firstLineAttribute.GetEndPoint();

				Vector3 pntStartPoint2 = secondLineAttribute.GetStartPoint();
				Vector3 pntEndPoint2 = secondLineAttribute.GetEndPoint();


				float distance;
				Vector3 projectPnt1, projectPnt2;
				bool parallel = true;
				MeasureCalculateHelper::LineLineDistance(firstLineAttribute,
					secondLineAttribute, distance, projectPnt1, projectPnt2,
					parallel);
				if (parallel)
				{
					Vector3 proPnt1;
					Vector3 proPnt2;
					Color line1Color = Color::RED;
					Line3D* line1 = NULL, *line2 = NULL;
					Point* pnt1 = NULL, *pnt2 = NULL;
					if (!(MeasureCalculateHelper::pntInSegment(projectPnt1, pntStartPoint1, pntEndPoint1, proPnt1)))
					{
						line1 = new Line3D(projectPnt1, proPnt1);
						line1->SetColor(line1Color);
						line1->SetName("dottedLine");

						pnt1 = new Point(projectPnt1);
						pnt1->SetDrawType(1);
						pnt1->SetSize(0.8f);
					}
					else if (projectPnt1 != pntStartPoint1 && projectPnt1 != pntEndPoint1)
					{
						pnt1 = new Point(projectPnt1);
						pnt1->SetDrawType(1);
						pnt1->SetSize(0.8f);
					}
					if (!(MeasureCalculateHelper::pntInSegment(projectPnt2, pntStartPoint2, pntEndPoint2, proPnt2)))
					{
						line2 = new Line3D(projectPnt2, proPnt2);
						line2->SetColor(line1Color);
						line2->SetName("dottedLine");

						pnt2 = new Point(projectPnt2);
						pnt2->SetDrawType(1);
						pnt2->SetSize(0.8f);
					}
					else if (projectPnt2 != pntStartPoint2 && projectPnt2 != pntEndPoint2)
					{
						pnt2 = new Point(projectPnt2);
						pnt2->SetDrawType(1);
						pnt2->SetSize(0.8f);
					}

					Vector3 directionVector = projectPnt1 - projectPnt2;
					float distance = directionVector.Length();

					//Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(0, 0);
					Vector3 zDirection = pntEndPoint1 - pntStartPoint1;

					directionVector = zDirection.CrossProduct(directionVector);
					directionVector.Normalize();

					float tmpLength = distance;
					if (distance < 500)
					{
						tmpLength = 1000;
					}
					Vector3 thirdCoord = projectPnt1 + directionVector*tmpLength * 0.5;
					Vector3 fourCoord = projectPnt2 + directionVector*tmpLength * 0.5;

					Line3D* line3 = new Line3D(pntStartPoint1, pntEndPoint1);
					line3->SetColor(line1Color);
					Line3D* line4 = new Line3D(pntStartPoint2, pntEndPoint2);
					line4->SetColor(line1Color);
					Line3D* line5 = new Line3D(projectPnt1, projectPnt2);
					line5->SetColor(line1Color);
					line5->SetName("dottedLine");
					Line3D* line6 = new Line3D(projectPnt1, thirdCoord);
					line6->SetColor(line1Color);
					line6->SetCanDelete(true);
					Line3D* line7 = new Line3D(projectPnt2, fourCoord);
					line7->SetColor(line1Color);
					line7->SetCanDelete(true);
					Line3D* line8 = new Line3D(thirdCoord, fourCoord);
					line8->SetColor(line1Color);
					line8->SetCanDelete(true);

					if (line1)
					{
						measure->AddLine(line1);
					}
					if (line2)
					{
						measure->AddLine(line2);
					}
					measure->AddLine(line3);
					measure->AddLine(line4);
					measure->AddLine(line5);
					measure->AddLine(line6);
					measure->AddLine(line7);
					measure->AddLine(line8);

					Point* pnt3 = new Point(pntStartPoint1);
					pnt3->SetDrawType(1);
					pnt3->SetSize(0.8f);
					Point* pnt4 = new Point(pntEndPoint1);
					pnt4->SetDrawType(1);
					pnt4->SetSize(0.8f);
					Point* pnt5 = new Point(pntStartPoint2);
					pnt5->SetDrawType(1);
					pnt5->SetSize(0.8f);
					Point* pnt6 = new Point(pntEndPoint2);
					pnt6->SetDrawType(1);
					pnt6->SetSize(0.8f);

					if (pnt1)
					{
						measure->AddPoint(pnt1);
					}
					if (pnt2)
					{
						measure->AddPoint(pnt2);
					}
					measure->AddPoint(pnt3);
					measure->AddPoint(pnt4);
					measure->AddPoint(pnt5);
					measure->AddPoint(pnt6);


					//测量结果单位转换
					vector<float> valuse;
					valuse.push_back(distance);
					string unit;
					MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
						, valuse, unit);
					string lengthstr = M3DTOOLS::floatToString(valuse.at(0)) + unit;

					//解决协同中文本读取
					ComText* ct = new ComText();
					CText* t = new CText;
					t->SetText(lengthstr);
					ct->AddCText(t);
					measure->m_ComTexts.push_back(ct);

					Vector3 pntInPlane;
					pntInPlane = (thirdCoord + fourCoord) / 2;

					vector<Texts2D*> temptext;
					Texts2D *title = new Texts2D;
					title->m_size = 12.0f;
					title->m_texts = "内容";
					temptext.push_back(title);

					Texts2D *content = new Texts2D;
					content->m_size = 12.0f;
					content->m_texts = lengthstr;
					temptext.push_back(content);

					ImageBoard * imageBroad = MeasureDisplayHelper::createNoteTextsImageN(scene, temptext, pntInPlane);
					measure->AddImage(imageBroad);
				}
				else
				{
					delete measure;
					measure = NULL;
				}
			}
			else
			{
				delete measure;
				measure = NULL;
			}
		}
		return measure;
	}

	Measure* MeasureFactory::createDesignerShaftToShaftDistance(int firstLineId, int secondLineId, SceneManager* scene)
	{
		Measure* measure = NULL;

		IShape* firstShape = scene->GetShape(firstLineId);
		IShape* secondShape = scene->GetShape(secondLineId);

		bool firstFinished = false, secondFinished = false;
		Vector3 firstVector, secondVector;
		Vector3 firstCenter, secondCenter;
		float firstRadius, secondRadius;
		if (firstShape && firstShape->GetType() == SHAPE_EDGE)
		{
			measure = new MeasureDistance;
			measure->SetMeasureType(Measure::MEASURE_TYPE_SHAFT_SHAFT_DISTANCE);

			firstFinished = CreateCircularArc(measure, firstShape, firstVector, firstCenter, firstRadius);
			if (!firstFinished)
			{
				delete measure;
				measure = NULL;
				return measure;
			}
		}

		if (firstFinished && secondShape && secondShape->GetType() == SHAPE_EDGE)
		{
			if (!measure)
			{
				return measure;
			}
			secondFinished = CreateCircularArc(measure, secondShape, secondVector, secondCenter, secondRadius);
			if (firstVector.Angle(secondVector) != 0 && firstVector.Angle(secondVector) != 180)
			{
				secondFinished = false;
			}
			if (!secondFinished || firstLineId == secondLineId)
			{
				delete measure;
				measure = NULL;
				return measure;
			}
		}
		if (firstFinished && secondFinished)
		{
			Vector3 upPnt1 = firstCenter + firstVector* firstRadius * 2;
			Vector3 downPnt1 = firstCenter - firstVector* firstRadius * 2;

			Vector3 upPnt2 = secondCenter + secondVector* secondRadius * 2;
			Vector3 downPnt2 = secondCenter - secondVector* secondRadius * 2;

			Point * pnt1 = new Point(upPnt1);
			pnt1->SetDrawType(1);
			pnt1->SetSize(0.8f);
			Point * pnt2 = new Point(downPnt1);
			pnt2->SetDrawType(1);
			pnt2->SetSize(0.8f);
			Point * pnt3 = new Point(upPnt2);
			pnt3->SetDrawType(1);
			pnt3->SetSize(0.8f);
			Point * pnt4 = new Point(downPnt2);
			pnt4->SetDrawType(1);
			pnt4->SetSize(0.8f);

			measure->AddPoint(pnt1);
			measure->AddPoint(pnt2);
			measure->AddPoint(pnt3);
			measure->AddPoint(pnt4);

			Color tempColor = Color::RED;
			Line3D* line1 = new Line3D(upPnt1, downPnt1);
			line1->SetName("dottedLine");
			line1->SetColor(tempColor);
			Line3D* line2 = new Line3D(upPnt2, downPnt2);
			line2->SetName("dottedLine");
			line2->SetColor(tempColor);
			measure->AddLine(line1);
			measure->AddLine(line2);


			Ray ray(firstCenter, firstVector);
			Vector3 projecPnt = ray.Project(secondCenter);
			float distance = (projecPnt - secondCenter).Length();

			Line3D* line3 = new Line3D(secondCenter, projecPnt);
			line3->SetName("dottedLine");
			line3->SetColor(tempColor);
			measure->AddLine(line3);

			if (projecPnt != firstCenter)
			{
				Point * pnt5 = new Point(projecPnt);
				pnt5->SetName("SpecialPnt");
				pnt5->SetDrawType(1);
				pnt5->SetSize(0.8f);
				measure->AddPoint(pnt5);

				Line3D* line4 = new Line3D(firstCenter, projecPnt);
				line4->SetName("dottedLine");
				line4->SetColor(tempColor);
				measure->AddLine(line4);
			}


			vector<float> valuse;
			valuse.push_back(distance);
			string unit;
			MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
				, valuse, unit);
			string lengthstr = M3DTOOLS::floatToString(valuse.at(0)) + unit;

			//解决协同中文本读取
			ComText* ct = new ComText();
			CText* t = new CText;
			t->SetText(lengthstr);
			ct->AddCText(t);
			measure->m_ComTexts.push_back(ct);

			Vector3 pntInPlane;
			pntInPlane = (projecPnt + secondCenter) / 2;

			vector<Texts2D*> temptext;
			Texts2D *title = new Texts2D;
			title->m_size = 12.0f;
			title->m_texts = "内容";
			temptext.push_back(title);

			Texts2D *content = new Texts2D;
			content->m_size = 12.0f;
			content->m_texts = lengthstr;
			temptext.push_back(content);

			ImageBoard * imageBroad = MeasureDisplayHelper::createNoteTextsImageN(scene, temptext, pntInPlane);
			measure->AddImage(imageBroad);
		}
		return measure;
	}

	Measure* MeasureFactory::createDesignerCenterToCenterDistance(int firstLineId, int secondLineId, SceneManager* scene)
	{
		Measure* measure = NULL;

		IShape* firstShape = scene->GetShape(firstLineId);
		IShape* secondShape = scene->GetShape(secondLineId);

		bool firstFinished = false, secondFinished = false;
		Vector3 firstVector, secondVector;
		Vector3 firstCenter, secondCenter;
		float firstRadius, secondRadius;
		if (firstShape && firstShape->GetType() == SHAPE_EDGE)
		{
			measure = new MeasureDistance;
			measure->SetMeasureType(Measure::MEASURE_TYPE_CENTER_CENTER_DISTANCE);

			firstFinished = CreateCircularArc(measure, firstShape, firstVector, firstCenter, firstRadius);
			if (!firstFinished)
			{
				delete measure;
				measure = NULL;
				return measure;
			}
		}

		if (firstFinished && secondShape && secondShape->GetType() == SHAPE_EDGE)
		{
			if (!measure)
			{
				return measure;
			}
			secondFinished = CreateCircularArc(measure, secondShape, secondVector, secondCenter, secondRadius);
			if (!secondFinished || firstLineId == secondLineId)
			{
				delete measure;
				measure = NULL;
				return measure;
			}
		}

		if (firstFinished && secondFinished)
		{
			Color tempColor = Color::RED;
			float distance = (firstCenter - secondCenter).Length();

			Line3D* line = new Line3D(secondCenter, firstCenter);
			line->SetName("dottedLine");
			line->SetColor(tempColor);
			measure->AddLine(line);

			vector<float> valuse;
			valuse.push_back(distance);
			string unit;
			MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
				, valuse, unit);
			string lengthstr = M3DTOOLS::floatToString(valuse.at(0)) + unit;

			//解决协同中文本读取
			ComText* ct = new ComText();
			CText* t = new CText;
			t->SetText(lengthstr);
			ct->AddCText(t);
			measure->m_ComTexts.push_back(ct);

			Vector3 pntInPlane;
			pntInPlane = (firstCenter + secondCenter) / 2;

			vector<Texts2D*> temptext;
			Texts2D *title = new Texts2D;
			title->m_size = 12.0f;
			title->m_texts = "内容";
			temptext.push_back(title);

			Texts2D *content = new Texts2D;
			content->m_size = 12.0f;
			content->m_texts = lengthstr;
			temptext.push_back(content);

			ImageBoard * imageBroad = MeasureDisplayHelper::createNoteTextsImageN(scene, temptext, pntInPlane);
			measure->AddImage(imageBroad);
		}
		return measure;
	}

	bool MeasureFactory::CreateCircular(Measure* measure, IShape* shape, Vector3& verticVector, Vector3& center, float& radius)
	{
		bool state = false;
		if (!measure)
		{
			return state;
		}
		if (shape && shape->GetType() == SHAPE_EDGE)
		{
			Edge * firstEdge = (Edge*)shape;

			RefPolyLine * firstLineSet = firstEdge->GetLineData();

			GeometryAttribute * firstGeo = firstEdge->GetGeoAttribute();

			if (firstGeo && firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_ELLIPSE)
			{
				EllipseAttribute firstEllipseAttribute = *((EllipseAttribute *)firstGeo);
				Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstEdge);
				GeometryHelper::Transform(firstEllipseAttribute, worldMatrix1);

				Vector3 center1 = firstEllipseAttribute.GetCenterPoint();
				center = center1;
				Point * centerPoint1 = new Point(center1);
				centerPoint1->SetDrawType(1);
				centerPoint1->SetSize(0.8f);

				measure->AddPoint(centerPoint1);

				float radius1 = firstEllipseAttribute.GetMajorRadius();
				Vector3 start1 = firstEllipseAttribute.GetStartPoint();
				Vector3 end1 = firstEllipseAttribute.GetEndPoint();
				Vector3 startDirection = start1 - center1;
				Vector3 endDirection = end1 - center1;
				Vector3 otherPnt = end1;
				radius = radius1;

				if (abs(startDirection.Angle(endDirection) - 180) < 0.1)
				{
					SPolyLine * xPolyLine = firstLineSet->GetRefLine();
					vector<Vector3> & pntPoints = xPolyLine->GetPoints();
					unsigned int offset = firstLineSet->GetDataOffset();
					unsigned int dataLength = firstLineSet->GetDataLength();

					for (vector<Vector3>::iterator it = pntPoints.begin() + offset;
						it != pntPoints.begin() + offset + dataLength; it++)
					{
						Vector3 tmpPnt = worldMatrix1 * (*it);

						float angle = startDirection.Angle(tmpPnt - center1);
						if (startDirection.Angle(tmpPnt - center1) > 5 && endDirection.Angle(tmpPnt - center1) > 5)
						{
							otherPnt = tmpPnt;
							break;
						}
					}
				}

				Plane projPlane(center1, start1, otherPnt);
				Vector3 normal1 = projPlane.m_normal;
				normal1.Normalize();
				verticVector = normal1;
				Vector3 vector1 = start1 - center1;
				vector1.Normalize();
				Vector3 vector2 = normal1.CrossProduct(vector1);
				vector2.Normalize();

				Vector3 startNormal = startDirection;
				startNormal.Normalize();
				Vector3 pnt1 = center1 + startNormal * radius1 / 3;
				Vector3 pnt2 = center1 - startNormal * radius1 / 3;
				startNormal = startDirection.CrossProduct(normal1);
				startNormal.Normalize();
				Vector3 pnt3 = center1 + startNormal * radius1 / 3;
				Vector3 pnt4 = center1 - startNormal * radius1 / 3;

				Color tempColor = Color::RED;
				Line3D * line1 = new Line3D(pnt1, pnt2);
				line1->SetColor(tempColor);
				Line3D * line2 = new Line3D(pnt3, pnt4);
				line2->SetColor(tempColor);

				measure->AddLine(line1);
				measure->AddLine(line2);


				Vector3 xVector, yVector, zVector;
				firstEllipseAttribute.GetXYZDir(xVector, yVector, zVector);

				Vector3 crossVector = startDirection.CrossProduct(endDirection);
				if (crossVector.Angle(zVector) > 90)
				{
					zVector *= -1;
				}
				float dotValue = crossVector.DotProduct(zVector);
				float sDotValue = 0.0f;
				if (abs(dotValue) < 0.1)
				{
					dotValue = 0;
					crossVector = startDirection.CrossProduct(otherPnt - center1);
					sDotValue = crossVector.DotProduct(zVector);
				}


				float arcAngle = 0.0f;
				bool bdottedLine = false;
				for (int i = 0, l = 32; i < l; i++)
				{
					float theta = i / (l*1.0)*M3D::M_PI * 2;
					Vector3 tmpPnt1(
						center1.m_x + radius1 * (vector1.m_x*cos(theta) + vector2.m_x* sin(theta)),
						center1.m_y + radius1 * (vector1.m_y*cos(theta) + vector2.m_y* sin(theta)),
						center1.m_z + radius1 * (vector1.m_z*cos(theta) + vector2.m_z* sin(theta))
					);
					theta = (i + 1) / (l*1.0)*M3D::M_PI * 2;
					Vector3 tmpPnt2(
						center1.m_x + radius1 * (vector1.m_x*cos(theta) + vector2.m_x* sin(theta)),
						center1.m_y + radius1 * (vector1.m_y*cos(theta) + vector2.m_y* sin(theta)),
						center1.m_z + radius1 * (vector1.m_z*cos(theta) + vector2.m_z* sin(theta))
					);


					Vector3 cross1 = startDirection.CrossProduct(tmpPnt1 - center1);
					Vector3 cross2 = startDirection.CrossProduct(tmpPnt2 - center1);
					float startDot1 = cross1.DotProduct(zVector);
					float startDot2 = cross2.DotProduct(zVector);

					cross1 = endDirection.CrossProduct(tmpPnt1 - center1);
					cross2 = endDirection.CrossProduct(tmpPnt2 - center1);

					float endDot1 = cross1.DotProduct(zVector);
					float endDot2 = cross2.DotProduct(zVector);

					Line3D * lineTemp = new Line3D(tmpPnt1, tmpPnt2);

					if (dotValue > 0) //劣弧
					{
						lineTemp->SetName("dottedLine");
						if ((startDot1 > 0 && endDot1 < 0) || (startDot2 > 0 && endDot2 < 0))
						{
							lineTemp->SetName("");
						}
					}
					else if (dotValue == 0)
					{
						Vector3 pntCross1 = (tmpPnt1 - center1).CrossProduct(otherPnt - center1);
						Vector3 pntCross2 = (tmpPnt2 - center1).CrossProduct(otherPnt - center1);

						float pntDot1 = pntCross1.DotProduct(zVector);
						float pntDot2 = pntCross2.DotProduct(zVector);

						lineTemp->SetName("dottedLine");
						if (sDotValue > 0)
						{
							if (
								(startDot1 > 0 && pntDot1 >= 0) || (startDot2 > 0 && pntDot2 >= 0) ||
								(pntDot1 <= 0 && endDot1 < 0) || (pntDot2 <= 0 && endDot2 < 0)
								)
							{
								lineTemp->SetName("");
							}
						}
						else if (sDotValue < 0)
						{
							if (
								(startDot1 < 0 && pntDot1 <= 0) || (startDot2 < 0 && pntDot2 <= 0) ||
								(pntDot1 > 0 && endDot1 > 0) || (pntDot2 > 0 && endDot2 > 0)
								)
							{
								lineTemp->SetName("");
							}
						}
					}
					else  //优弧
					{
						if ((startDot1 > 0 && endDot1 < 0) && (startDot2 > 0 && endDot2 < 0))
						{
							lineTemp->SetName("dottedLine");
						}
					}

					lineTemp->SetColor(tempColor);
					measure->AddLine(lineTemp);
				}

				state = true;
			}
		}

		return state;
	}

	Measure* MeasureFactory::createDesignerLineDistance(int firstLineId, SceneManager* scene)
	{
		Measure* measure = NULL;

		IShape* firstShape = scene->GetShape(firstLineId);

		if (firstShape && firstShape->GetType() == SHAPE_EDGE)
		{
			measure = new MeasureDistance;

			measure->SetMeasureType(Measure::MEASURE_TYPE_LINE_LENGTH);

			Edge* firstEdge = (Edge*)firstShape;
			RefPolyLine * firstLineSet = firstEdge->GetLineData();
			GeometryAttribute* firstGeo = firstEdge->GetGeoAttribute();
			if (firstGeo && firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE)
			{
				LineAttribute firstLineAttribute = *((LineAttribute*)firstGeo);

				Matrix3x4 worldMatrix = ShapeHelper::GetShapeWorldMatrix(
					firstEdge);

				GeometryHelper::Transform(firstLineAttribute, worldMatrix);

				float distance = 0;

				Vector3 pntStartPoint = firstLineAttribute.GetStartPoint(); //!< Line起点
				Vector3 pntEndPoint = firstLineAttribute.GetEndPoint(); //!< Line终点

				distance = (pntStartPoint - pntEndPoint).Length();

				Vector3 directionVector = pntStartPoint - pntEndPoint;

				Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(0, 0);
				Vector3 zDirection = cameraRay.GetDirection();

				directionVector = zDirection.CrossProduct(directionVector);
				directionVector.Normalize();

				Vector3 thirdCoord = pntStartPoint + directionVector*distance * 0.5;
				Vector3 fourCoord = pntEndPoint + directionVector*distance * 0.5;

				Color line1Color = Color::RED;
				Line3D* line1 = new Line3D(pntStartPoint, pntEndPoint);
				line1->SetColor(line1Color);
				Line3D* line2 = new Line3D(pntStartPoint, thirdCoord);//文本引线
				line2->SetCanDelete(true);
				line2->SetColor(line1Color);
				Line3D* line3 = new Line3D(pntEndPoint, fourCoord);//文本引线
				line3->SetCanDelete(true);
				line3->SetColor(line1Color);
				Line3D* line4 = new Line3D(thirdCoord, fourCoord);//文本引线
				line4->SetCanDelete(true);
				line4->SetColor(line1Color);

				measure->AddLine(line1);
				measure->AddLine(line2);
				measure->AddLine(line3);
				measure->AddLine(line4);

				Point* pnt1 = new Point(pntStartPoint);
				pnt1->SetDrawType(1);
				pnt1->SetSize(0.8f);
				Point* pnt2 = new Point(pntEndPoint);
				pnt2->SetDrawType(1);
				pnt2->SetSize(0.8f);

				measure->AddPoint(pnt1);
				measure->AddPoint(pnt2);


				//测量结果单位转换
				vector<float> valuse;
				valuse.push_back(distance);
				string unit;
				MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
					, valuse, unit);
				string lengthstr = M3DTOOLS::floatToString(valuse.at(0)) + unit;

				//解决协同中文本读取
				ComText* ct = new ComText();
				CText* t = new CText;
				t->SetText(lengthstr);
				ct->AddCText(t);
				measure->m_ComTexts.push_back(ct);

				Vector3 pntInPlane;
				pntInPlane = (thirdCoord + fourCoord) / 2;

				vector<Texts2D*> temptext;
				Texts2D *title = new Texts2D;
				title->m_size = 12.0f;
				title->m_texts = "内容";
				temptext.push_back(title);

				Texts2D *content = new Texts2D;
				content->m_size = 12.0f;
				content->m_texts = lengthstr;
				temptext.push_back(content);

				ImageBoard * imageBroad = MeasureDisplayHelper::createNoteTextsImageN(scene, temptext, pntInPlane);
				measure->AddImage(imageBroad);
			}
			else
			{
				delete measure;
				measure = NULL;
			}
		}
		return measure;
	}

	Measure* MeasureFactory::createDesignerArcDistance(int firstLineId, SceneManager* scene)
	{
		Measure* measure = NULL;

		IShape* firstShape = scene->GetShape(firstLineId);

		if (firstShape && firstShape->GetType() == SHAPE_EDGE)
		{
			measure = new MeasureDistance;
			measure->SetMeasureType(Measure::MEASURE_TYPE_CRICLE_PROPERTY);

			Vector3 firstVector;
			Vector3 firstCenter;
			float firstRadius;
			bool firstFinished = false;
			firstFinished = CreateCircularArc(measure, firstShape, firstVector, firstCenter, firstRadius);
			if (!firstFinished)
			{
				delete measure;
				measure = NULL;
				return measure;
			}
		}
		return measure;
	}

	Measure* MeasureFactory::CreateTmpFaceDistanceMeasure(int firstShapeID, int secondShapID,
		int thirdShapeID, int forthShapID,
		int type, SceneManager* scene)
	{
		Measure* measure = NULL;

		//判断场景是否为空
		if (scene == NULL)
		{
			return measure;
		}

		if (type == Measure::MEASURE_TYPE_PNT_FACE_DISTANCE)
		{
			measure = createDesignerPntToFaceDistance(firstShapeID, secondShapID, thirdShapeID, scene);
		}
		else if (type == Measure::MEASURE_TYPE_LINE_FACE_DISTANCE)
		{
			measure = createDesignerLineToFaceDistance(firstShapeID, secondShapID, thirdShapeID, scene);
		}
		else if (type == Measure::MEASURE_TYPE_FACE_FACE_DISTANCE)
		{
			measure = createDesignerFaceToFaceDistance(firstShapeID, secondShapID,
				thirdShapeID, forthShapID, scene);
		}

		AddMeasureToScene(scene, measure);

		if (measure)
		{
			measure->SetFrontShow(true);
		}

		return measure;
	}

	Measure* MeasureFactory::createDesignerPntToFaceDistance(int firstFaceId, int secondPntId,
		int thirdPntId, SceneManager* scene)
	{
		Measure* measure = NULL;

		IShape * firstShape = scene->GetShape(firstFaceId);
		IShape * secondShape = scene->GetShape(secondPntId);
		IShape * thirdShape = scene->GetShape(thirdPntId);

		if (firstShape && thirdShape
			&& firstShape->GetType() == SHAPE_FACE && thirdShape->GetType() == SHAPE_POINT_HANDLE)
		{
			measure = new MeasureDistance;
			measure->SetMeasureType(Measure::MEASURE_TYPE_PNT_FACE_DISTANCE);

			Face * firstFace = dynamic_cast<Face*>(firstShape);
			Mesh * firstMeshData = firstFace->GetData();
			GeometryAttribute * firstGeo = firstFace->GetGeoAttribute();

			HandlerPoint * thirdPnt = (HandlerPoint *)thirdShape;
			Vector3 tCoordinate = thirdPnt->GetPosition();

			if (firstGeo && firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
			{
				PlaneFaceAttribute firstPlaneFaceAttribute =
					*((PlaneFaceAttribute *)firstGeo);

				VertexSet* vertexSet = firstMeshData->GetRefMesh();
				vector<Vector3>* vectVec = vertexSet->GetPositionArray();

				Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstFace);
				GeometryHelper::Transform(firstPlaneFaceAttribute, worldMatrix1);
				

				float distance = 0;
				Vector3 projectPnt;
				MeasureCalculateHelper::PntFaceDistance(tCoordinate, firstPlaneFaceAttribute, distance, projectPnt);

				BoundingBox& box = firstMeshData->GetBoundingBox();
				distance = (box.m_max - box.m_min).Length() / 20;

				Vector3 normalVec = firstPlaneFaceAttribute.GetNormal();
				Vector3 originVec = firstPlaneFaceAttribute.GetOrigin();

				Vector3 direction = projectPnt - originVec;

				vector<Vector3> pointArray;
				MeasureDisplayHelper::CreateSelectFaceMark(pointArray, projectPnt, direction,
					normalVec, distance);//平面标志

				Color line1Color = Color::RED;

				Line3D* line1 = new Line3D(pointArray[0], pointArray[1]);  //4
				line1->SetColor(line1Color);
				line1->SetLineWidth(1);
				Line3D* line2 = new Line3D(pointArray[2], pointArray[3]);
				line2->SetColor(line1Color);
				line2->SetLineWidth(1);
				Line3D* line3 = new Line3D(pointArray[0], pointArray[2]);
				line3->SetColor(line1Color);
				line3->SetLineWidth(1);
				Line3D* line4 = new Line3D(pointArray[3], pointArray[1]);
				line4->SetColor(line1Color);
				line4->SetLineWidth(1);


				measure->AddLine(line1);
				measure->AddLine(line2);
				measure->AddLine(line3);
				measure->AddLine(line4);

				Point* pnt = new Point(tCoordinate);
				pnt->SetDrawType(1);
				pnt->SetSize(0.8f);
				measure->AddPoint(pnt);

				if (secondShape && secondShape->GetType() == SHAPE_POINT_HANDLE)
				{
					HandlerPoint * secondPnt = (HandlerPoint *)secondShape;
					Vector3 sCoordinate = secondPnt->GetPosition();

					Plane planeTemp(normalVec,originVec);
					Vector3 pntProject = planeTemp.Project(sCoordinate);

					float Length = (sCoordinate - pntProject).Length();

					Vector3 tmpDirection = sCoordinate - pntProject;
					tmpDirection.Normalize();

					Vector3 tmpPnt = tCoordinate + tmpDirection * Length;

					Color line1Color = Color::RED;
					Line3D* line1 = new Line3D(sCoordinate, pntProject);
					line1->SetName("dottedLine");
					line1->SetColor(line1Color);
					line1->SetLineWidth(1);
					Line3D* line2 = new Line3D(tCoordinate, tmpPnt);
					line2->SetName("dottedLine");
					line2->SetColor(line1Color);
					line2->SetLineWidth(1);


					Line3D* line3 = new Line3D(tCoordinate, pntProject);
					line3->SetColor(line1Color);
					line3->SetLineWidth(1);
					Line3D* line4 = new Line3D(sCoordinate, tmpPnt);
					line4->SetColor(line1Color);
					line4->SetLineWidth(1);


					measure->AddLine(line1);
					measure->AddLine(line2);
					measure->AddLine(line3);
					measure->AddLine(line4);

					Point* pnt = new Point(sCoordinate);
					pnt->SetDrawType(1);
					pnt->SetSize(0.8f);
					measure->AddPoint(pnt);


					//测量结果单位转换
					vector<float> valuse;
					valuse.push_back(Length);
					string unit;
					MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
						, valuse, unit);
					string lengthstr = M3DTOOLS::floatToString(valuse.at(0)) + unit;

					//解决协同中文本读取
					ComText* ct = new ComText();
					CText* t = new CText;
					t->SetText(lengthstr);
					ct->AddCText(t);
					measure->m_ComTexts.push_back(ct);

					Vector3 pntInPlane;
					pntInPlane = (sCoordinate + pntProject) / 2;

					vector<Texts2D*> temptext;
					Texts2D *title = new Texts2D;
					title->m_size = 12.0f;
					title->m_texts = "内容";
					temptext.push_back(title);

					Texts2D *content = new Texts2D;
					content->m_size = 12.0f;
					content->m_texts = lengthstr;
					temptext.push_back(content);

					ImageBoard * imageBroad = MeasureDisplayHelper::createNoteTextsImageN(scene, temptext, pntInPlane);
					measure->AddImage(imageBroad);

				}
			}
			else
			{
				delete measure;
				measure = NULL;
				return measure;
			}

			
		}
		return measure;
	}

	Measure* MeasureFactory::createDesignerLineToFaceDistance(int firstFaceId, int secondPntId,
		int thirdPntId, SceneManager* scene)
	{
		Measure* measure = NULL;

		IShape * firstShape = scene->GetShape(firstFaceId);
		IShape * secondShape = scene->GetShape(secondPntId);
		IShape * thirdShape = scene->GetShape(thirdPntId);

		bool firstFinished = false, secondFinished = false;
		Vector3 tCoordinate, sCoordinate;
		Vector3 planDirection;
		PlaneFaceAttribute firstPlaneFaceAttribute;
		LineAttribute secondLineAttribute;
		if (firstShape && thirdShape
			&& firstShape->GetType() == SHAPE_FACE && thirdShape->GetType() == SHAPE_POINT_HANDLE)
		{
			measure = new MeasureDistance;
			measure->SetMeasureType(Measure::MEASURE_TYPE_LINE_FACE_DISTANCE);

			Face * firstFace = dynamic_cast<Face*>(firstShape);
			Mesh * firstMeshData = firstFace->GetData();
			GeometryAttribute * firstGeo = firstFace->GetGeoAttribute();

			HandlerPoint * thirdPnt = (HandlerPoint *)thirdShape;
			tCoordinate = thirdPnt->GetPosition();

			if (firstGeo && firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
			{
				firstPlaneFaceAttribute =
					*((PlaneFaceAttribute *)firstGeo);

				VertexSet* vertexSet = firstMeshData->GetRefMesh();
				vector<Vector3>* vectVec = vertexSet->GetPositionArray();

				Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstFace);
				GeometryHelper::Transform(firstPlaneFaceAttribute, worldMatrix1);


				float distance = 0;
				Vector3 projectPnt;
				MeasureCalculateHelper::PntFaceDistance(tCoordinate, firstPlaneFaceAttribute, distance, projectPnt);

				BoundingBox& box = firstMeshData->GetBoundingBox();
				distance = (box.m_max - box.m_min).Length() / 20;

				Vector3 normalVec = firstPlaneFaceAttribute.GetNormal();
				Vector3 originVec = firstPlaneFaceAttribute.GetOrigin();
				planDirection = normalVec;

				Vector3 direction = projectPnt - originVec;

				vector<Vector3> pointArray;
				MeasureDisplayHelper::CreateSelectFaceMark(pointArray, projectPnt, direction,
					normalVec, distance);//平面标志

				Color line1Color = Color::RED;

				Line3D* line1 = new Line3D(pointArray[0], pointArray[1]);  //4
				line1->SetColor(line1Color);
				line1->SetLineWidth(1);
				Line3D* line2 = new Line3D(pointArray[2], pointArray[3]);
				line2->SetColor(line1Color);
				line2->SetLineWidth(1);
				Line3D* line3 = new Line3D(pointArray[0], pointArray[2]);
				line3->SetColor(line1Color);
				line3->SetLineWidth(1);
				Line3D* line4 = new Line3D(pointArray[3], pointArray[1]);
				line4->SetColor(line1Color);
				line4->SetLineWidth(1);


				measure->AddLine(line1);
				measure->AddLine(line2);
				measure->AddLine(line3);
				measure->AddLine(line4);

				Point* pnt = new Point(tCoordinate);
				pnt->SetDrawType(1);
				pnt->SetSize(0.8f);
				measure->AddPoint(pnt);

				firstFinished = true;
			}
			else
			{
				delete measure;
				measure = NULL;
			}
		}
		if (firstFinished && !secondFinished && secondShape && secondShape->GetType() == SHAPE_EDGE)
		{
			if (measure == NULL)
			{
				return measure;
			}

			Edge * secondEdge = (Edge*)secondShape;
			RefPolyLine * secondLineSet = secondEdge->GetLineData();
			GeometryAttribute * secondGeo = secondEdge->GetGeoAttribute();
			if (secondGeo && secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE)
			{
				secondLineAttribute = *((LineAttribute *)secondGeo);

				Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					secondEdge);

				GeometryHelper::Transform(secondLineAttribute, worldMatrix1);

				float distance = 0.0f;
				Vector3 projectPnt0, projectPnt1;

				if (!MeasureCalculateHelper::LineFaceDistance(secondLineAttribute,
					firstPlaneFaceAttribute, distance,
					projectPnt0, projectPnt1))
				{
					delete measure;
					measure = NULL;
					return measure;
				}

				Vector3 line1Start = secondLineAttribute.GetStartPoint();
				Vector3 line1End = secondLineAttribute.GetEndPoint();

				sCoordinate = (line1Start + line1End) / 2;

				Color line1Color = Color::RED;
				Line3D* line1 = new Line3D(line1Start, line1End);  //4
				line1->SetColor(line1Color);

				measure->AddLine(line1);

				Point* pnt1 = new Point(line1Start);
				pnt1->SetDrawType(1);
				pnt1->SetSize(0.8f);
				Point* pnt2 = new Point(line1End);
				pnt2->SetDrawType(1);
				pnt2->SetSize(0.8f);

				measure->AddPoint(pnt1);
				measure->AddPoint(pnt2);
				secondFinished = true;
			}
			else
			{
				delete measure;
				measure = NULL;
			}
		}

		if (firstFinished && secondFinished)
		{
			if (measure == NULL)
			{
				return measure;
			}
			float distance = 0.0f;
			Vector3 projectPnt0, projectPnt1;

			if (!MeasureCalculateHelper::LineFaceDistance(secondLineAttribute,
				firstPlaneFaceAttribute, distance,
				projectPnt0, projectPnt1))
			{
				delete measure;
				measure = NULL;
                return NULL;
			}

			Vector3 lineStart = secondLineAttribute.GetStartPoint();
			Vector3 lineEnd = secondLineAttribute.GetEndPoint();
			Vector3 lineCenter = (lineStart + lineEnd) / 2;

			Vector3 normalVec = lineStart - projectPnt0;
			normalVec.Normalize();


			Vector3 planeProPnt = tCoordinate + normalVec * distance;
			Vector3 lineProPnt = lineCenter - normalVec*distance;
			Vector3 center1 = (planeProPnt + lineCenter) / 2;
			Vector3 center2 = (tCoordinate + lineProPnt) / 2;

			Color line1Color = Color::RED;
			Line3D* line1 = new Line3D(tCoordinate, planeProPnt);
			line1->SetName("dottedLine");
			line1->SetColor(line1Color);
			line1->SetLineWidth(1);

			Line3D* line2 = new Line3D(tCoordinate, lineProPnt);
			line2->SetColor(line1Color);
			line2->SetLineWidth(1);

			Line3D* line3 = new Line3D(lineCenter, lineProPnt);
			line3->SetName("dottedLine");
			line3->SetColor(line1Color);
			line3->SetLineWidth(1);

			Line3D* line4 = new Line3D(lineCenter, planeProPnt);
			line4->SetColor(line1Color);
			line4->SetLineWidth(1);

			Line3D* line5 = new Line3D(center1, center2);
			line5->SetName("dottedLine");
			line5->SetColor(line1Color);
			line5->SetLineWidth(1);

			measure->AddLine(line1);
			measure->AddLine(line2);
			measure->AddLine(line3);
			measure->AddLine(line4);
			measure->AddLine(line5);

			Point* pnt1 = new Point(center1);
			pnt1->SetDrawType(1);
			pnt1->SetSize(0.8f);
			Point* pnt2 = new Point(center2);
			pnt2->SetDrawType(1);
			pnt2->SetSize(0.8f);

			measure->AddPoint(pnt1);
			measure->AddPoint(pnt2);

			//测量结果单位转换
			vector<float> valuse;
			valuse.push_back(distance);
			string unit;
			MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
				, valuse, unit);
			string lengthstr = M3DTOOLS::floatToString(valuse.at(0)) + unit;

			//解决协同中文本读取
			ComText* ct = new ComText();
			CText* t = new CText;
			t->SetText(lengthstr);
			ct->AddCText(t);
			measure->m_ComTexts.push_back(ct);

			Vector3 pntInPlane = (center1 + center2) / 2;

			vector<Texts2D*> temptext;
			Texts2D *title = new Texts2D;
			title->m_size = 12.0f;
			title->m_texts = "内容";
			temptext.push_back(title);

			Texts2D *content = new Texts2D;
			content->m_size = 12.0f;
			content->m_texts = lengthstr;
			temptext.push_back(content);

			ImageBoard * imageBroad = MeasureDisplayHelper::createNoteTextsImageN(scene, temptext, pntInPlane);
			measure->AddImage(imageBroad);
		}

		return measure;
	}

	Measure* MeasureFactory::createDesignerFaceToFaceDistance(int firstFaceId, int secondFaceId,
		int thirdPntId, int forthPntId, SceneManager* scene)
	{
		Measure* measure = NULL;

		IShape * firstShape = scene->GetShape(firstFaceId);
		IShape * secondShape = scene->GetShape(secondFaceId);
		IShape * thirdShape = scene->GetShape(thirdPntId);
		IShape * forthShape = scene->GetShape(forthPntId);

		bool firstFinished = false, secondFinished = false;
		PlaneFaceAttribute firstPlaneFaceAttribute, secondPlaneFaceAttribute;
		Vector3 thirdCoordinate, forthCoordinate;
		if (firstShape && thirdShape
			&& firstShape->GetType() == SHAPE_FACE && thirdShape->GetType() == SHAPE_POINT_HANDLE)
		{
			measure = new MeasureDistance;
			measure->SetMeasureType(Measure::MEASURE_TYPE_FACE_FACE_DISTANCE);

			Face * firstFace = dynamic_cast<Face*>(firstShape);
			Mesh * firstMeshData = firstFace->GetData();
			GeometryAttribute * firstGeo = firstFace->GetGeoAttribute();

			HandlerPoint * thirdPnt = (HandlerPoint *)thirdShape;
			thirdCoordinate = thirdPnt->GetPosition();

			if (firstGeo && firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
			{
				firstPlaneFaceAttribute =
					*((PlaneFaceAttribute *)firstGeo);

				Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstFace);
				GeometryHelper::Transform(firstPlaneFaceAttribute, worldMatrix1);


				float distance = 0;
				Vector3 projectPnt;
				MeasureCalculateHelper::PntFaceDistance(thirdCoordinate, firstPlaneFaceAttribute, distance, projectPnt);

				BoundingBox& box = firstMeshData->GetBoundingBox();
				distance = (box.m_max - box.m_min).Length() / 20;

				Vector3 normalVec = firstPlaneFaceAttribute.GetNormal();
				Vector3 originVec = firstPlaneFaceAttribute.GetOrigin();

				Vector3 direction = projectPnt - originVec;

				vector<Vector3> pointArray;
				MeasureDisplayHelper::CreateSelectFaceMark(pointArray, projectPnt, direction,
					normalVec, distance);//平面标志

				Color line1Color = Color::RED;

				Line3D* line1 = new Line3D(pointArray[0], pointArray[1]);  //4
				line1->SetColor(line1Color);
				line1->SetLineWidth(1);
				Line3D* line2 = new Line3D(pointArray[2], pointArray[3]);
				line2->SetColor(line1Color);
				line2->SetLineWidth(1);
				Line3D* line3 = new Line3D(pointArray[0], pointArray[2]);
				line3->SetColor(line1Color);
				line3->SetLineWidth(1);
				Line3D* line4 = new Line3D(pointArray[3], pointArray[1]);
				line4->SetColor(line1Color);
				line4->SetLineWidth(1);


				measure->AddLine(line1);
				measure->AddLine(line2);
				measure->AddLine(line3);
				measure->AddLine(line4);

				Point* pnt = new Point(thirdCoordinate);
				pnt->SetDrawType(1);
				pnt->SetSize(0.8f);
				measure->AddPoint(pnt);

				firstFinished = true;
			}
			else
			{
				delete measure;
				measure = NULL;
			}
		}
		if (firstFinished && secondShape && forthShape
			&& secondShape->GetType() == SHAPE_FACE && forthShape->GetType() == SHAPE_POINT_HANDLE
			)
		{
			if (measure == NULL)
			{
				return measure;
			}

			Face * secondFace = dynamic_cast<Face*>(secondShape);
			Mesh * secondMeshData = secondFace->GetData();
			GeometryAttribute * secondGeo = secondFace->GetGeoAttribute();

			HandlerPoint * forthPnt = (HandlerPoint *)forthShape;
			forthCoordinate = forthPnt->GetPosition();

			if (firstFaceId != secondFaceId && secondGeo 
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
			{
				secondPlaneFaceAttribute =
					*((PlaneFaceAttribute *)secondGeo);

				Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					secondFace);
				GeometryHelper::Transform(secondPlaneFaceAttribute, worldMatrix1);


				float distance = 0;
				Vector3 projectPnt;
				MeasureCalculateHelper::PntFaceDistance(forthCoordinate, secondPlaneFaceAttribute, distance, projectPnt);

				BoundingBox& box = secondMeshData->GetBoundingBox();
				distance = (box.m_max - box.m_min).Length() / 20;

				Vector3 normalVec = secondPlaneFaceAttribute.GetNormal();
				Vector3 originVec = secondPlaneFaceAttribute.GetOrigin();

				Vector3 direction = projectPnt - originVec;

				vector<Vector3> pointArray;
				MeasureDisplayHelper::CreateSelectFaceMark(pointArray, projectPnt, direction,
					normalVec, distance);//平面标志

				Color line1Color = Color::RED;

				Line3D* line1 = new Line3D(pointArray[0], pointArray[1]);  //4
				line1->SetColor(line1Color);
				line1->SetLineWidth(1);
				Line3D* line2 = new Line3D(pointArray[2], pointArray[3]);
				line2->SetColor(line1Color);
				line2->SetLineWidth(1);
				Line3D* line3 = new Line3D(pointArray[0], pointArray[2]);
				line3->SetColor(line1Color);
				line3->SetLineWidth(1);
				Line3D* line4 = new Line3D(pointArray[3], pointArray[1]);
				line4->SetColor(line1Color);
				line4->SetLineWidth(1);


				measure->AddLine(line1);
				measure->AddLine(line2);
				measure->AddLine(line3);
				measure->AddLine(line4);

				Point* pnt = new Point(forthCoordinate);
				pnt->SetDrawType(1);
				pnt->SetSize(0.8f);
				measure->AddPoint(pnt);

				secondFinished = true;
			}
			else
			{
				delete measure;
				measure = NULL;
			}
		}
		if (firstFinished && secondFinished)
		{
			if (measure == NULL)
			{
				return measure;
			}

			float distance = 0.0f;
			Vector3 projectPnt0, projectPnt1;

			if (!MeasureCalculateHelper::FaceFaceDistance(firstPlaneFaceAttribute, secondPlaneFaceAttribute, distance,
				projectPnt0, projectPnt1))
			{
				delete measure;
				measure = NULL;
				return NULL;
			}

			Vector3 originVec = firstPlaneFaceAttribute.GetOrigin();
			Vector3 direction = projectPnt0 - originVec;
			direction.Normalize();

			Vector3 pnt1 = thirdCoordinate + direction * distance;
			Vector3 pnt2 = forthCoordinate - direction * distance;

			Vector3 center1 = (thirdCoordinate + pnt2) / 2;
			Vector3 center2 = (forthCoordinate + pnt1) / 2;

			Color line1Color = Color::RED;

			Line3D* line1 = new Line3D(thirdCoordinate, pnt1);
			line1->SetName("dottedLine");
			line1->SetColor(line1Color);
			line1->SetLineWidth(1);
			Line3D* line2 = new Line3D(forthCoordinate, pnt2);
			line2->SetName("dottedLine");
			line2->SetColor(line1Color);
			line2->SetLineWidth(1);
			Line3D* line3 = new Line3D(thirdCoordinate, pnt2);
			line3->SetColor(line1Color);
			line3->SetLineWidth(1);
			Line3D* line4 = new Line3D(forthCoordinate, pnt1);
			line4->SetColor(line1Color);
			line4->SetLineWidth(1);

			Line3D* line5 = new Line3D(center1, center2);
			line5->SetName("dottedLine");
			line5->SetColor(line1Color);
			line5->SetLineWidth(1);

			measure->AddLine(line1);
			measure->AddLine(line2);
			measure->AddLine(line3);
			measure->AddLine(line4);
			measure->AddLine(line5);

			Point* point1 = new Point(center1);
			point1->SetDrawType(1);
			point1->SetSize(0.8f);

			Point* point2 = new Point(center2);
			point2->SetDrawType(1);
			point2->SetSize(0.8f);
			measure->AddPoint(point1);
			measure->AddPoint(point2);

			//测量结果单位转换
			vector<float> valuse;
			valuse.push_back(distance);
			string unit;
			MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
				, valuse, unit);
			string lengthstr = M3DTOOLS::floatToString(valuse.at(0)) + unit;

			//解决协同中文本读取
			ComText* ct = new ComText();
			CText* t = new CText;
			t->SetText(lengthstr);
			ct->AddCText(t);
			measure->m_ComTexts.push_back(ct);

			Vector3 pntInPlane;
			pntInPlane = (center1 + center2) / 2;

			vector<Texts2D*> temptext;
			Texts2D *title = new Texts2D;
			title->m_size = 12.0f;
			title->m_texts = "内容";
			temptext.push_back(title);

			Texts2D *content = new Texts2D;
			content->m_size = 12.0f;
			content->m_texts = lengthstr;
			temptext.push_back(content);

			ImageBoard * imageBroad = MeasureDisplayHelper::createNoteTextsImageN(scene, temptext, pntInPlane);
			measure->AddImage(imageBroad);
		}
		return measure;
	}

	bool MeasureFactory::CompleteDistanceMeasure(Measure* measure, int firstPntId, int secondPntId, 
		Vector2 screenPnt, SceneManager* scene)
	{
		bool state = false;
		if (!measure || !scene)
		{
			return state;
		}

		int type = measure->GetMeasureType();
		if (type == Measure::MEASURE_TYPE_PNT_PNT_DISTANCE) ///点到点测量
		{
			state =  completeDesignerPntToPntDistance(measure, firstPntId, secondPntId, screenPnt, scene);
		}
		else if (type == Measure::MEASURE_TYPE_PNT_LINE_DISTANCE)
		{
			state = completeDesignerPntToLineDistance(measure, firstPntId, secondPntId,screenPnt, scene);
		}
		else if (type == Measure::MEASURE_TYPE_LINE_LINE_DISTANCE)
		{
			state = completeDesignerLineToLineDistance(measure, firstPntId, secondPntId, screenPnt, scene);
		}
		else if (type == Measure::MEASURE_TYPE_SHAFT_SHAFT_DISTANCE)
		{
			state = completeDesignerShaftToShaftDistance(measure, firstPntId, secondPntId, screenPnt, scene);
		}
		else if (type == Measure::MEASURE_TYPE_CENTER_CENTER_DISTANCE)
		{
			state = completeDesignerCenterToCenterDistance(measure, firstPntId, secondPntId, screenPnt, scene);
		}
		else if (type == Measure::MEASURE_TYPE_LINE_LENGTH)
		{
			state = completeDesignerLineDistance(measure, firstPntId, screenPnt, scene);
		}
		else if (type == Measure::MEASURE_TYPE_CRICLE_PROPERTY)
		{
			state = completeDesignerArcDistance(measure, firstPntId, screenPnt, scene);
		}
		return state;
	}

	bool MeasureFactory::completeDesignerPntToPntDistance(Measure* measure, int firstPntId, int secondPntId,
		Vector2& screenPnt, SceneManager* scene)
	{
		bool state = false;
		if (!measure || !scene)
		{
			return state;
		}

		MeasureDistance* measureDistance = dynamic_cast<MeasureDistance*>(measure);
		if (!measureDistance)
		{
			return state;
		}

		IShape* firstShape = scene->GetShape(firstPntId);
		IShape* secondShape = scene->GetShape(secondPntId);

		//如果的shape对象存在
		if (firstShape && secondShape && firstShape->GetType() == SHAPE_POINT_HANDLE
			&& secondShape->GetType() == SHAPE_POINT_HANDLE)
		{
			HandlerPoint* firstPnt = (HandlerPoint*)firstShape;
			HandlerPoint* secondPnt = (HandlerPoint*)secondShape;

			Vector3 fCoordiante = firstPnt->GetPosition();
			Vector3 sCoordiante = secondPnt->GetPosition();
			Vector3 directionVector = sCoordiante - fCoordiante;

			Vector3 tCoordiante;
			if (!scene->GetPickPoint(screenPnt, tCoordiante, false))
			{
				return state;
			}

			Plane projPlane(fCoordiante, sCoordiante, tCoordiante);
			Vector3 normalVector = projPlane.m_normal;
			normalVector = normalVector.CrossProduct(directionVector);
			normalVector.Normalize();

			Line3D* dottedLine = NULL;
			for (vector<Line3D*>::iterator ite = measureDistance->m_LineList.begin(); ite != measureDistance->m_LineList.end();)
			{
				Line3D* tmpLine = *ite;
				if (tmpLine && tmpLine->GetName().compare("dottedLine") > -1)
				{
					dottedLine = tmpLine;
					ite++;
				}
				else
				{
					delete *ite;
					ite = measureDistance->m_LineList.erase(ite);
				}
			}

			float lineLength = 0;
			if (dottedLine)
			{
				Vector3 closePnt = dottedLine->GetClosestPoint(tCoordiante);
				lineLength = (tCoordiante - closePnt).Length();
			}

			Vector3 thirdCoord = fCoordiante + normalVector*lineLength;
			Vector3 fourCoord = sCoordiante + normalVector*lineLength;


			Color line1Color = Color::RED;
			Line3D* line2 = new Line3D(fCoordiante, thirdCoord);//文本引线
			line2->SetColor(line1Color);
			Line3D* line3 = new Line3D(sCoordiante, fourCoord);//文本引线
			line3->SetColor(line1Color);
			Line3D* line4 = new Line3D(thirdCoord, fourCoord);//文本引线
			line4->SetColor(line1Color);

			Line3D* line6 = NULL;
			Vector3 projecPnt;
			if (!(MeasureCalculateHelper::pntInSegment(tCoordiante, thirdCoord, fourCoord, projecPnt)))
			{
				line6 = new Line3D(projecPnt, tCoordiante);
				line6->SetColor(line1Color);
			}

			measure->AddLine(line2);
			measure->AddLine(line3);
			measure->AddLine(line4);
			if (line6)
			{
				measure->AddLine(line6);
			}

			if (measure->m_PointList.size() != 2)
			{
				for (int i = 0; i < measure->m_PointList.size(); i++)
				{
					delete measure->m_PointList[i];
				}
				measure->m_PointList.resize(0);

				Point* pnt1 = new Point(fCoordiante);
				pnt1->SetDrawType(1);
				pnt1->SetSize(0.8f);
				Point* pnt2 = new Point(sCoordiante);
				pnt2->SetDrawType(1);
				pnt2->SetSize(0.8f);

				measure->AddPoint(pnt1);
				measure->AddPoint(pnt2);
			}

			for (int i = 0; i < measure->m_imageBoardList.size(); i++) ///图片列表
			{
				measure->m_imageBoardList[i]->SetPosition(tCoordiante);
			}

			state = true;
		}
		return state;
	}

	bool MeasureFactory::completeDesignerPntToLineDistance(Measure* measure, int lineId, int pntId,
		Vector2& screenPnt, SceneManager* scene)
	{
		bool state = false;
		if (!measure || !scene)
		{
			return state;
		}

		MeasureDistance* measureDistance = dynamic_cast<MeasureDistance*>(measure);
		if (!measureDistance)
		{
			return state;
		}

		IShape* lineShape = scene->GetShape(lineId);
		IShape* pntShape = scene->GetShape(pntId);

		//如果的shape对象存在
		if (lineShape && pntShape && pntShape->GetType() == SHAPE_POINT_HANDLE
			&& lineShape->GetType() == SHAPE_EDGE)
		{
			HandlerPoint* pnt = (HandlerPoint*)pntShape;
			Vector3 pntCoordiante = pnt->GetPosition();

			Edge* lineEdge = (Edge*)lineShape;
			//得到边界线数据
			RefPolyLine * m_lineSet = lineEdge->GetLineData();

			GeometryAttribute* geo = lineEdge->GetGeoAttribute();
			if (geo && geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE)
			{
				LineAttribute lineAttribute = *((LineAttribute*)geo);

				Matrix3x4 worldMatrix = ShapeHelper::GetShapeWorldMatrix(
					lineEdge);

				GeometryHelper::Transform(lineAttribute, worldMatrix);

				Vector3 pntStartPoint = lineAttribute.GetStartPoint(); //!< Line起点
				Vector3 pntEndPoint = lineAttribute.GetEndPoint(); //!< Line终点

				Vector3 closestCoordiant;
				int inLine;
				float distance;

				MeasureCalculateHelper::PntLineDistance(pntCoordiante, lineAttribute,
					distance, inLine, closestCoordiant);

				Vector3 directionVector = pntCoordiante - closestCoordiant;


				Vector3 tCoordiante;
				if (!scene->GetPickPoint(screenPnt, tCoordiante, false))
				{
					return state;
				}

				Plane projPlane(closestCoordiant, pntCoordiante, tCoordiante);
				Vector3 normalVector = projPlane.m_normal;
				normalVector = normalVector.CrossProduct(directionVector);
				normalVector.Normalize();


				Ray ray(pntCoordiante, directionVector);
				Vector3 projecPnt = ray.Project(tCoordiante);
				distance = (projecPnt - tCoordiante).Length();

				Vector3 thirdCoord = closestCoordiant + normalVector*distance;
				Vector3 fourCoord = pntCoordiante + normalVector*distance;

				for (vector<Line3D*>::iterator ite = measureDistance->m_LineList.begin(); ite != measureDistance->m_LineList.end();)
				{
					Line3D* tmpLine = *ite;
					if (tmpLine && !tmpLine->GetCanDelete())
					{
						ite++;
					}
					else
					{
						delete *ite;
						ite = measureDistance->m_LineList.erase(ite);
					}
				}

				Color line1Color = Color::RED;
				Line3D* line2 = new Line3D(closestCoordiant, thirdCoord);//文本引线
				line2->SetCanDelete(true);
				line2->SetColor(line1Color);
				Line3D* line3 = new Line3D(pntCoordiante, fourCoord);//文本引线
				line3->SetCanDelete(true);
				line3->SetColor(line1Color);
				Line3D* line4 = new Line3D(thirdCoord, fourCoord);//文本引线
				line4->SetCanDelete(true);
				line4->SetColor(line1Color);

				Line3D* line6 = NULL;
				if (!(MeasureCalculateHelper::pntInSegment(tCoordiante,thirdCoord, fourCoord, projecPnt)))
				{
					line6 = new Line3D(projecPnt, tCoordiante);
					line6->SetCanDelete(true);
					line6->SetColor(line1Color);
				}

				measure->AddLine(line2);
				measure->AddLine(line3);
				measure->AddLine(line4);
				if (line6)
				{
					measure->AddLine(line6);
				}

				for (int i = 0; i < measure->m_imageBoardList.size(); i++) ///图片列表
				{
					measure->m_imageBoardList[i]->SetPosition(tCoordiante);
				}

				state = true;
			}
		}
			return state;
	}

	bool MeasureFactory::completeDesignerLineToLineDistance(Measure* measure, int firstLineId, int secondLineId,
		Vector2& screenPnt, SceneManager* scene)
	{
		bool state = false;
		if (!measure || !scene)
		{
			return state;
		}

		MeasureDistance* measureDistance = dynamic_cast<MeasureDistance*>(measure);
		if (!measureDistance)
		{
			return state;
		}

		IShape* firstShape = scene->GetShape(firstLineId);
		IShape* secondShape = scene->GetShape(secondLineId);

		//如果的shape对象存在
		if (firstShape && secondShape 
			&& firstShape->GetType() == SHAPE_EDGE
			&& secondShape->GetType() == SHAPE_EDGE)
		{
			Edge * firstEdge = (Edge*)firstShape;
			Edge * secondEdge = (Edge *)secondShape;

			RefPolyLine * firstLineSet = firstEdge->GetLineData();
			RefPolyLine * secondLineSet = secondEdge->GetLineData();

			GeometryAttribute * firstGeo = firstEdge->GetGeoAttribute();
			GeometryAttribute * secondGeo = secondEdge->GetGeoAttribute();

			if (firstGeo != NULL
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE
				&& secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE)
			{
				LineAttribute firstLineAttribute = *((LineAttribute *)firstGeo);
				LineAttribute secondLineAttribute = *((LineAttribute *)secondGeo);

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


				Vector3 directionVector = projectPnt1 - projectPnt2;


				Vector3 tCoordiante;
				if (!scene->GetPickPoint(screenPnt, tCoordiante, false))
				{
					return state;
				}

				Plane projPlane(projectPnt1, projectPnt2, tCoordiante);
				Vector3 normalVector = projPlane.m_normal;
				normalVector = directionVector.CrossProduct(normalVector);
				normalVector.Normalize();


				Ray ray(projectPnt1, directionVector);
				Vector3 projecPnt = ray.Project(tCoordiante);
				distance = (projecPnt - tCoordiante).Length();

				Vector3 thirdCoord = projectPnt1 + normalVector*distance;
				Vector3 fourCoord = projectPnt2 + normalVector*distance;

				for (vector<Line3D*>::iterator ite = measureDistance->m_LineList.begin(); ite != measureDistance->m_LineList.end();)
				{
					Line3D* tmpLine = *ite;
					if (tmpLine && !tmpLine->GetCanDelete())
					{
						ite++;
					}
					else
					{
						delete *ite;
						ite = measureDistance->m_LineList.erase(ite);
					}
				}

				Color line1Color = Color::RED;
				Line3D* line2 = new Line3D(projectPnt1, thirdCoord);//文本引线
				line2->SetCanDelete(true);
				line2->SetColor(line1Color);
				Line3D* line3 = new Line3D(projectPnt2, fourCoord);//文本引线
				line3->SetCanDelete(true);
				line3->SetColor(line1Color);
				Line3D* line4 = new Line3D(thirdCoord, fourCoord);//文本引线
				line4->SetCanDelete(true);
				line4->SetColor(line1Color);

				Line3D* line6 = NULL;
				if (!(MeasureCalculateHelper::pntInSegment(tCoordiante, thirdCoord, fourCoord, projecPnt)))
				{
					line6 = new Line3D(projecPnt, tCoordiante);
					line6->SetCanDelete(true);
					line6->SetColor(line1Color);
				}

				measure->AddLine(line2);
				measure->AddLine(line3);
				measure->AddLine(line4);
				if (line6)
				{
					measure->AddLine(line6);
				}
				
				for (int i = 0; i < measure->m_imageBoardList.size(); i++) ///图片列表
				{
					measure->m_imageBoardList[i]->SetPosition(tCoordiante);
				}

				state = true;
			}
		}
		return state;
	}

	bool MeasureFactory::completeDesignerShaftToShaftDistance(Measure* measure, int firstLineId, int secondLineId,
		Vector2& screenPnt, SceneManager* scene)
	{
		bool state = false;
		if (!measure || !scene)
		{
			return state;
		}

		MeasureDistance* measureDistance = dynamic_cast<MeasureDistance*>(measure);
		if (!measureDistance)
		{
			return state;
		}

		IShape* firstShape = scene->GetShape(firstLineId);
		IShape* secondShape = scene->GetShape(secondLineId);

		//如果的shape对象存在
		if (firstShape && secondShape
			&& firstShape->GetType() == SHAPE_EDGE
			&& secondShape->GetType() == SHAPE_EDGE)
		{
			Edge * firstEdge = (Edge*)firstShape;
			Edge * secondEdge = (Edge *)secondShape;

			RefPolyLine * firstLineSet = firstEdge->GetLineData();
			RefPolyLine * secondLineSet = secondEdge->GetLineData();

			GeometryAttribute * firstGeo = firstEdge->GetGeoAttribute();
			GeometryAttribute * secondGeo = secondEdge->GetGeoAttribute();

			if (firstGeo != NULL
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_ELLIPSE
				&& secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_ELLIPSE)
			{

				EllipseAttribute firstEllipseAttribute = *((EllipseAttribute *)firstGeo);
				EllipseAttribute secondEllipseAttribute = *((EllipseAttribute *)secondGeo);

				Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstEdge);
				Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(
					secondEdge);
				GeometryHelper::Transform(firstEllipseAttribute, worldMatrix1);
				GeometryHelper::Transform(secondEllipseAttribute, worldMatrix2);

				Vector3 center1 = firstEllipseAttribute.GetCenterPoint();
				Vector3 center2 = secondEllipseAttribute.GetCenterPoint();

				float radius1 = firstEllipseAttribute.GetMajorRadius();
				float radius2 = secondEllipseAttribute.GetMajorRadius();

				Vector3 start1 = firstEllipseAttribute.GetStartPoint();
				Vector3 end1 = firstEllipseAttribute.GetEndPoint();

				Vector3 tCoordiante;
				if (!scene->GetPickPoint(screenPnt, tCoordiante, false))
				{
					return state;
				}

				Vector3 projecPnt = center1;
				for (int i = 0; i < measure->m_PointList.size(); i++)
				{
					Point* tmpPnt = measure->m_PointList.at(i);
					if (tmpPnt && tmpPnt->GetName().compare("SpecialPnt") > -1)
					{
						projecPnt = tmpPnt->GetCoordinate();
						break;
					}
				}


				Plane projPlane(center2, projecPnt, tCoordiante);
				Vector3 normalVector = projPlane.m_normal;
				normalVector = (center2 - projecPnt).CrossProduct(normalVector);
				normalVector.Normalize();


				 Ray ray(center2, center2 - projecPnt);
				Vector3 proPnt = ray.Project(tCoordiante);
				float distance = (proPnt - tCoordiante).Length();

				Vector3 thirdCoord = center2 + normalVector*distance;
				Vector3 fourCoord = projecPnt + normalVector*distance;

				DeleteMeasureLine(measure);

				Color line1Color = Color::RED;
				Line3D* line2 = new Line3D(center2, thirdCoord);//文本引线
				line2->SetCanDelete(true);
				line2->SetColor(line1Color);
				Line3D* line3 = new Line3D(projecPnt, fourCoord);//文本引线
				line3->SetCanDelete(true);
				line3->SetColor(line1Color);
				Line3D* line4 = new Line3D(thirdCoord, fourCoord);//文本引线
				line4->SetCanDelete(true);
				line4->SetColor(line1Color);

				Line3D* line6 = NULL;
				if (!(MeasureCalculateHelper::pntInSegment(tCoordiante, thirdCoord, fourCoord, projecPnt)))
				{
					line6 = new Line3D(projecPnt, tCoordiante);
					line6->SetCanDelete(true);
					line6->SetColor(line1Color);
				}

				measure->AddLine(line2);
				measure->AddLine(line3);
				measure->AddLine(line4);
				if (line6)
				{
					measure->AddLine(line6);
				}

				for (int i = 0; i < measure->m_imageBoardList.size(); i++) ///图片列表
				{
					measure->m_imageBoardList[i]->SetPosition(tCoordiante);
				}

				state = true;
			}

		}
		return state;
	}

	bool MeasureFactory::completeDesignerCenterToCenterDistance(Measure* measure, int firstLineId, int secondLineId,
		Vector2& screenPnt, SceneManager* scene)
	{
		bool state = false;
		if (!measure || !scene)
		{
			return state;
		}

		MeasureDistance* measureDistance = dynamic_cast<MeasureDistance*>(measure);
		if (!measureDistance)
		{
			return state;
		}

		IShape* firstShape = scene->GetShape(firstLineId);
		IShape* secondShape = scene->GetShape(secondLineId);

		//如果的shape对象存在
		if (firstShape && secondShape
			&& firstShape->GetType() == SHAPE_EDGE
			&& secondShape->GetType() == SHAPE_EDGE)
		{
			Edge * firstEdge = (Edge*)firstShape;
			Edge * secondEdge = (Edge *)secondShape;

			RefPolyLine * firstLineSet = firstEdge->GetLineData();
			RefPolyLine * secondLineSet = secondEdge->GetLineData();

			GeometryAttribute * firstGeo = firstEdge->GetGeoAttribute();
			GeometryAttribute * secondGeo = secondEdge->GetGeoAttribute();

			if (firstGeo != NULL
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_ELLIPSE
				&& secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_ELLIPSE)
			{

				EllipseAttribute firstEllipseAttribute = *((EllipseAttribute *)firstGeo);
				EllipseAttribute secondEllipseAttribute = *((EllipseAttribute *)secondGeo);

				Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstEdge);
				Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(
					secondEdge);
				GeometryHelper::Transform(firstEllipseAttribute, worldMatrix1);
				GeometryHelper::Transform(secondEllipseAttribute, worldMatrix2);

				Vector3 center1 = firstEllipseAttribute.GetCenterPoint();
				Vector3 center2 = secondEllipseAttribute.GetCenterPoint();

				Vector3 tCoordiante;
				if (!scene->GetPickPoint(screenPnt, tCoordiante, false))
				{
					return state;
				}


				Plane projPlane(center1, center2, tCoordiante);
				Vector3 normalVector = projPlane.m_normal;
				normalVector = (center1 - center2).CrossProduct(normalVector);
				normalVector.Normalize();


				Ray ray(center1, center1 - center2);
				Vector3 proPnt = ray.Project(tCoordiante);
				float distance = (proPnt - tCoordiante).Length();

				Vector3 thirdCoord = center1 + normalVector*distance;
				Vector3 fourCoord = center2 + normalVector*distance;

				DeleteMeasureLine(measure);

				Color line1Color = Color::RED;
				Line3D* line2 = new Line3D(center1, thirdCoord);//文本引线
				line2->SetCanDelete(true);
				line2->SetColor(line1Color);
				Line3D* line3 = new Line3D(center2, fourCoord);//文本引线
				line3->SetCanDelete(true);
				line3->SetColor(line1Color);
				Line3D* line4 = new Line3D(thirdCoord, fourCoord);//文本引线
				line4->SetCanDelete(true);
				line4->SetColor(line1Color);

				Line3D* line6 = NULL;
				Vector3 projecPnt;
				if (!(MeasureCalculateHelper::pntInSegment(tCoordiante, thirdCoord, fourCoord, projecPnt)))
				{
					line6 = new Line3D(projecPnt, tCoordiante);
					line6->SetCanDelete(true);
					line6->SetColor(line1Color);
				}

				measure->AddLine(line2);
				measure->AddLine(line3);
				measure->AddLine(line4);
				if (line6)
				{
					measure->AddLine(line6);
				}

				for (int i = 0; i < measure->m_imageBoardList.size(); i++) ///图片列表
				{
					measure->m_imageBoardList[i]->SetPosition(tCoordiante);
				}

				state = true;
			}

		}
		return state;
	}

	bool MeasureFactory::completeDesignerLineDistance(Measure* measure, int firstLineId,
		Vector2& screenPnt, SceneManager* scene)
	{
		bool state = false;
		if (!measure || !scene)
		{
			return state;
		}

		MeasureDistance* measureDistance = dynamic_cast<MeasureDistance*>(measure);
		if (!measureDistance)
		{
			return state;
		}

		IShape* firstShape = scene->GetShape(firstLineId);

		//如果的shape对象存在
		if (firstShape && firstShape->GetType() == SHAPE_EDGE)
		{
			Edge * firstEdge = (Edge*)firstShape;
			RefPolyLine * firstLineSet = firstEdge->GetLineData();
			GeometryAttribute * firstGeo = firstEdge->GetGeoAttribute();
		
			if (firstGeo != NULL && firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE)
			{
				LineAttribute firstLineAttribute = *((LineAttribute *)firstGeo);
				Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstEdge);
				GeometryHelper::Transform(firstLineAttribute, worldMatrix1);

				Vector3 pntStartPoint = firstLineAttribute.GetStartPoint();
				Vector3 pntEndPoint = firstLineAttribute.GetEndPoint(); 
				
				Vector3 tCoordiante;
				if (!scene->GetPickPoint(screenPnt, tCoordiante, false))
				{
					return state;
				}


				Plane projPlane(pntStartPoint, pntEndPoint, tCoordiante);
				Vector3 normalVector = projPlane.m_normal;
				normalVector = (pntStartPoint - pntEndPoint).CrossProduct(normalVector);
				normalVector.Normalize();


				Ray ray(pntStartPoint, pntEndPoint - pntStartPoint);
				Vector3 proPnt = ray.Project(tCoordiante);
				float distance = (proPnt - tCoordiante).Length();

				Vector3 thirdCoord = pntStartPoint + normalVector*distance;
				Vector3 fourCoord = pntEndPoint + normalVector*distance;

				for (vector<Line3D*>::iterator ite = measureDistance->m_LineList.begin(); ite != measureDistance->m_LineList.end();)
				{
					Line3D* tmpLine = *ite;
					if (tmpLine && !tmpLine->GetCanDelete())
					{
						ite++;
					}
					else
					{
						delete *ite;
						ite = measureDistance->m_LineList.erase(ite);
					}
				}

				Color line1Color = Color::RED;
				Line3D* line2 = new Line3D(pntStartPoint, thirdCoord);//文本引线
				line2->SetCanDelete(true);
				line2->SetColor(line1Color);
				Line3D* line3 = new Line3D(pntEndPoint, fourCoord);//文本引线
				line3->SetCanDelete(true);
				line3->SetColor(line1Color);
				Line3D* line4 = new Line3D(thirdCoord, fourCoord);//文本引线
				line4->SetCanDelete(true);
				line4->SetColor(line1Color);

				Line3D* line6 = NULL;
				Vector3 projecPnt;
				if (!(MeasureCalculateHelper::pntInSegment(tCoordiante, thirdCoord, fourCoord, projecPnt)))
				{
					line6 = new Line3D(projecPnt, tCoordiante);
					line6->SetCanDelete(true);
					line6->SetColor(line1Color);
				}

				measure->AddLine(line2);
				measure->AddLine(line3);
				measure->AddLine(line4);
				if (line6)
				{
					measure->AddLine(line6);
				}

				for (int i = 0; i < measure->m_imageBoardList.size(); i++) ///图片列表
				{
					measure->m_imageBoardList[i]->SetPosition(tCoordiante);
				}

				state = true;
			}
		}

		return state;
	}

	bool MeasureFactory::completeDesignerArcDistance(Measure* measure, int firstLineId,
		Vector2& screenPnt, SceneManager* scene)
	{
		bool state = false;
		if (!measure || !scene)
		{
			return state;
		}

		MeasureDistance* measureDistance = dynamic_cast<MeasureDistance*>(measure);
		if (!measureDistance)
		{
			return state;
		}

		IShape* firstShape = scene->GetShape(firstLineId);

		//如果的shape对象存在
		if (firstShape && firstShape->GetType() == SHAPE_EDGE)
		{
			Edge * firstEdge = (Edge*)firstShape;
			RefPolyLine * firstLineSet = firstEdge->GetLineData();
			GeometryAttribute * firstGeo = firstEdge->GetGeoAttribute();

			if (firstGeo != NULL && firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_ELLIPSE)
			{
				EllipseAttribute firstEllipseAttribute = *((EllipseAttribute *)firstGeo);
				Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstEdge);
				GeometryHelper::Transform(firstEllipseAttribute, worldMatrix1);

				DeleteMeasureLine(measure);

				Vector3 center1 = firstEllipseAttribute.GetCenterPoint();
				Vector3 start1 = firstEllipseAttribute.GetStartPoint();
				Vector3 end1 = firstEllipseAttribute.GetEndPoint();
				Vector3 startDirection = (start1 - center1).Normalized();
				Vector3 endDirection = (end1 - center1).Normalized();
				float radius = firstEllipseAttribute.GetMajorRadius(); 
	
				SPolyLine * xPolyLine = firstLineSet->GetRefLine();
				vector<Vector3> & pntPoints = xPolyLine->GetPoints();
				unsigned int offset = firstLineSet->GetDataOffset();
				unsigned int dataLength = firstLineSet->GetDataLength();

				Vector3 normalDirection;
				float angle = startDirection.Angle(endDirection);
				if (abs(angle - 180) < 0.1)
				{
					dataLength = dataLength / 2;
					Vector3 tmpPnt = worldMatrix1 * pntPoints[offset + dataLength];
					normalDirection = startDirection.CrossProduct(tmpPnt - center1);
				}
				else
				{
					normalDirection = startDirection.CrossProduct(endDirection);
				}

				Plane proPlane(normalDirection, start1);
				Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(screenPnt.m_x, screenPnt.m_y);
				Vector3 pntInPlane;
				proPlane.GetInsectPnt(cameraRay, pntInPlane);

				Vector3 pntDirection = pntInPlane - center1;
				float curRadius = pntDirection.Length();
				Vector3 newStart = center1 + startDirection* curRadius;
				Vector3 newEnd = center1 + endDirection* curRadius;

				vector<Vector3> linesTemp;
				float arcLength = 0.0f;
				if (abs(angle - 180) < 0.1)
				{
					arcLength = M_PI * radius;
					float tmpAngle = startDirection.Angle(pntDirection);
					MeasureDisplayHelper::CreateDesignerAngleMark(center1, newStart, center1, pntInPlane,
						center1, curRadius, tmpAngle, linesTemp);
					MeasureDisplayHelper::CreateDesignerAngleMark(center1, pntInPlane, center1, newEnd,
						center1, curRadius, 180-tmpAngle, linesTemp);
				}
				else
				{
					float angle1 = pntDirection.Angle(startDirection);
					float angle2 = pntDirection.Angle(endDirection);
					if (dataLength < 3)
					{
						arcLength = angle * M_PI*radius / 180;
						if (abs(angle1 - angle2 - angle) < 0.1)
						{
							MeasureDisplayHelper::CreateDesignerAngleMark(center1, newStart, center1, pntInPlane,
								center1, curRadius, angle1, linesTemp);
						}
						else if (abs(angle2 - angle1 - angle) < 0.1)
						{
							MeasureDisplayHelper::CreateDesignerAngleMark(center1, newEnd, center1, pntInPlane,
								center1, curRadius, angle2, linesTemp);
						}
						else if (angle1 < angle && angle2 < angle)
						{
							MeasureDisplayHelper::CreateDesignerAngleMark(center1, newStart, center1, newEnd,
								center1, curRadius, angle, linesTemp);
						}
					}
					else
					{
						dataLength = dataLength / 2;
						Vector3 tmpPnt = worldMatrix1 * pntPoints[offset + dataLength];
						float tmpAngle1 = startDirection.Angle(tmpPnt - center1);
						float tmpAngle2 = endDirection.Angle(tmpPnt - center1);
						if (tmpAngle1 < angle && tmpAngle2 < angle)
						{
							arcLength = angle * M_PI*radius / 180;

							if (abs(angle1 - angle2 - angle) < 0.1)
							{
								MeasureDisplayHelper::CreateDesignerAngleMark(center1, newStart, center1, pntInPlane,
									center1, curRadius, angle1, linesTemp);
							}
							else if (abs(angle2 - angle1 - angle) < 0.1)
							{
								MeasureDisplayHelper::CreateDesignerAngleMark(center1, newEnd, center1, pntInPlane,
									center1, curRadius, angle2, linesTemp);
							}
							else if (angle1 < angle && angle2 < angle)
							{
								MeasureDisplayHelper::CreateDesignerAngleMark(center1, newStart, center1, newEnd,
									center1, curRadius, angle, linesTemp);
							}
						}
						else
						{
							arcLength = (360-angle) * M_PI*radius / 180;
							if (angle1 > angle2 && angle1 < angle && angle2 < angle)
							{
								MeasureDisplayHelper::CreateDesignerAngleMark(center1, newStart, center1, pntInPlane,
									center1, curRadius, angle1-360, linesTemp);
							}
							else if(angle1 <= angle2 && angle1 < angle && angle2 < angle)
							{
								MeasureDisplayHelper::CreateDesignerAngleMark(center1, pntInPlane, center1, newEnd,
									center1, curRadius, angle2 - 360, linesTemp);
							}
							else if (angle1 > angle || angle2 > angle)
							{
								MeasureDisplayHelper::CreateDesignerAngleMark(center1, newStart, center1, newEnd,
									center1, curRadius, angle - 360, linesTemp);
							}
						}
					}
				}
				if (linesTemp.size() > 0)
				{
					Color tempColor = Color::RED;
					for (int i = 0; i < linesTemp.size() - 1; i++)
					{
						Line3D * lineTemp = new Line3D(linesTemp[i],
							linesTemp[i + 1]);
						lineTemp->SetColor(tempColor);
						lineTemp->SetCanDelete(true);
						measure->AddLine(lineTemp);
					}

					Line3D * line1 = new Line3D(start1, newStart);
					line1->SetCanDelete(true);
					line1->SetColor(tempColor);
					Line3D * line2 = new Line3D(end1, newEnd);
					line2->SetCanDelete(true);
					line2->SetColor(tempColor);
					measure->AddLine(line1);
					measure->AddLine(line2);
				}

				if (measure->m_imageBoardList.size() == 0)
				{
					//测量结果单位转换
					vector<float> valuse;
					valuse.push_back(arcLength);
					string unit;
					MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
						, valuse, unit);
					string lengthstr = M3DTOOLS::floatToString(valuse.at(0)) + unit;

					//解决协同中文本读取
					ComText* ct = new ComText();
					CText* t = new CText;
					t->SetText(lengthstr);
					ct->AddCText(t);
					measure->m_ComTexts.push_back(ct);

					vector<Texts2D*> temptext;
					Texts2D *title = new Texts2D;
					title->m_size = 12.0f;
					title->m_texts = "内容";
					temptext.push_back(title);

					Texts2D *content = new Texts2D;
					content->m_size = 12.0f;
					content->m_texts = lengthstr;
					temptext.push_back(content);

					ImageBoard * imageBroad = MeasureDisplayHelper::createNoteTextsImageN(scene, temptext, pntInPlane);
					measure->AddImage(imageBroad);
				}
				else
				{
					measure->m_imageBoardList[0]->SetPosition(pntInPlane);
				}

				state = true;
			}
		}
		return state;
	}

	bool MeasureFactory::CompleteFaceDistanceMeasure(Measure* measure, int firstPntId, int secondPntId,
		int thirdShapeID, int forthShapID,
		Vector2 screenPnt, SceneManager* scene)
	{
		bool state = false;
		if (!measure || !scene)
		{
			return state;
		}

		int type = measure->GetMeasureType();
		if (type == Measure::MEASURE_TYPE_PNT_FACE_DISTANCE)
		{
			state = completeDesignerPntToFaceDistance(measure, firstPntId, secondPntId,thirdShapeID,screenPnt, scene);
		}
		else if (type == Measure::MEASURE_TYPE_LINE_FACE_DISTANCE)
		{
			state = completeDesignerLineToFaceDistance(measure, firstPntId, secondPntId, thirdShapeID, screenPnt, scene);
		}
		else if (type == Measure::MEASURE_TYPE_FACE_FACE_DISTANCE)
		{
			state = completeDesignerFaceToFaceDistance(measure, firstPntId, secondPntId, thirdShapeID, forthShapID, screenPnt, scene);
		}
		return state;
	}
	bool MeasureFactory::completeDesignerPntToFaceDistance(Measure* measure, int firstLineId, int secondPntId,
		int thirdPntId,
		Vector2& screenPnt, SceneManager* scene)
	{
		bool state = false;
		if (!measure || !scene)
		{
			return state;
		}

		MeasureDistance* measureDistance = dynamic_cast<MeasureDistance*>(measure);
		if (!measureDistance)
		{
			return state;
		}

		IShape* firstShape = scene->GetShape(firstLineId);
		IShape* secondShape = scene->GetShape(secondPntId);
		//IShape * thirdShape = scene->GetShape(thirdPntId);


		if (firstShape && secondShape
			&& firstShape->GetType() == SHAPE_FACE
			&& secondShape->GetType() == SHAPE_POINT_HANDLE)
		{
			Face * firstFace = dynamic_cast<Face*>(firstShape);
			Mesh * firstMeshData = firstFace->GetData();
			GeometryAttribute * firstGeo = firstFace->GetGeoAttribute();

			HandlerPoint * secondPnt = (HandlerPoint *)secondShape;
			Vector3 sCoordinate = secondPnt->GetPosition();

			//HandlerPoint * thirdPnt = (HandlerPoint *)thirdShape;
			//Vector3 tCoordinate = pnt->GetCoordinate();

			if (firstGeo && firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
			{
				PlaneFaceAttribute firstPlaneFaceAttribute =
					*((PlaneFaceAttribute *)firstGeo);

				VertexSet* vertexSet = firstMeshData->GetRefMesh();
				vector<Vector3>* vectVec = vertexSet->GetPositionArray();

				Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstFace);
				GeometryHelper::Transform(firstPlaneFaceAttribute, worldMatrix1);

				Plane planeTemp(firstPlaneFaceAttribute.GetNormal(),
					firstPlaneFaceAttribute.GetOrigin());
				Vector3 pntProject = planeTemp.Project(sCoordinate);
				

				float Length = (sCoordinate - pntProject).Length();

				Vector3 tCoordiante;
				if (!scene->GetPickPoint(screenPnt, tCoordiante, false))
				{
					return state;
				}

				Vector3 directionVector = sCoordinate - pntProject;
				Plane projPlane(sCoordinate, pntProject, tCoordiante);
				Vector3 normalVector = projPlane.m_normal;
				normalVector = directionVector.CrossProduct(normalVector);
				normalVector.Normalize();


				Ray ray(sCoordinate, directionVector);
				Vector3 projecPnt = ray.Project(tCoordiante);
				float distance = (projecPnt - tCoordiante).Length();

				Vector3 thirdCoord = sCoordinate + normalVector*distance;
				Vector3 fourCoord = pntProject + normalVector*distance;

				for (vector<Line3D*>::iterator ite = measureDistance->m_LineList.begin(); ite != measureDistance->m_LineList.end();)
				{
					Line3D* tmpLine = *ite;
					if (tmpLine && !tmpLine->GetCanDelete())
					{
						ite++;
					}
					else
					{
						delete *ite;
						ite = measureDistance->m_LineList.erase(ite);
					}
				}

				Color line1Color = Color::RED;
				Line3D* line1 = new Line3D(sCoordinate, thirdCoord);//文本引线
				line1->SetCanDelete(true);
				line1->SetColor(line1Color);
				line1->SetLineWidth(1);

				Line3D* line2 = new Line3D(pntProject, fourCoord);//文本引线
				line2->SetCanDelete(true);
				line2->SetColor(line1Color);
				line2->SetLineWidth(1);

				Line3D* line3 = new Line3D(thirdCoord, fourCoord);//文本引线
				line3->SetCanDelete(true);
				line3->SetColor(line1Color);
				line3->SetLineWidth(1);

				Line3D* line4 = NULL;
				if (!(MeasureCalculateHelper::pntInSegment(tCoordiante, thirdCoord, fourCoord, projecPnt)))
				{
					line4 = new Line3D(projecPnt, tCoordiante);
					line4->SetCanDelete(true);
					line4->SetColor(line1Color);
					line4->SetLineWidth(1);
				}

				measure->AddLine(line1);
				measure->AddLine(line2);
				measure->AddLine(line3);
				if (line4)
				{
					measure->AddLine(line4);
				}

				
				for (int i = 0; i < measure->m_imageBoardList.size(); i++) ///图片列表
				{
					measure->m_imageBoardList[i]->SetPosition(tCoordiante);
				}
				state = true;
			}
			else
			{
				delete measure;
				measure = NULL;
				return state;
			}
		}
		return state;
	}

	bool MeasureFactory::completeDesignerLineToFaceDistance(Measure* measure, int firstFaceId, int secondPntId,
		int thirdPntId,
		Vector2& screenPnt, SceneManager* scene)
	{
		bool state = false;
		if (!measure || !scene)
		{
			return state;
		}

		MeasureDistance* measureDistance = dynamic_cast<MeasureDistance*>(measure);
		if (!measureDistance)
		{
			return state;
		}

		IShape * firstShape = scene->GetShape(firstFaceId);
		IShape * secondShape = scene->GetShape(secondPntId);
		IShape * thirdShape = scene->GetShape(thirdPntId);

		if (firstShape && secondShape && thirdShape
			&& firstShape->GetType() == SHAPE_FACE
			&& secondShape->GetType() == SHAPE_EDGE
			&& thirdShape->GetType() == SHAPE_POINT_HANDLE)
		{
			Face * firstFace = dynamic_cast<Face*>(firstShape);
			Mesh * firstMeshData = firstFace->GetData();
			GeometryAttribute * firstGeo = firstFace->GetGeoAttribute();

			Edge * secondEdge = dynamic_cast<Edge*>(secondShape);
			RefPolyLine * secondLineSet = secondEdge->GetLineData();
			GeometryAttribute * secondGeo = secondEdge->GetGeoAttribute();

			HandlerPoint * thirdPnt = (HandlerPoint *)thirdShape;
			Vector3 tCoordinate = thirdPnt->GetPosition();

			if (firstGeo != NULL
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE
				&& secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE)
			{
				PlaneFaceAttribute firstPlaneFaceAttribute =
					*((PlaneFaceAttribute *)firstGeo);
				LineAttribute secondLineAttribute = *((LineAttribute *)secondGeo);
				
				Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstFace);
				Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(
					secondEdge);

				GeometryHelper::Transform(firstPlaneFaceAttribute, worldMatrix1);
				GeometryHelper::Transform(secondLineAttribute, worldMatrix2);


				float distance = 0.0f;
				Vector3 projectPnt0, projectPnt1;

				if (!MeasureCalculateHelper::LineFaceDistance(secondLineAttribute,
					firstPlaneFaceAttribute, distance,
					projectPnt0, projectPnt1))
				{
					delete measure;
					measure = NULL;
					return state;
				}

				//Vector3 normalVec = firstPlaneFaceAttribute.GetNormal();
				Vector3 lineStart = secondLineAttribute.GetStartPoint();
				Vector3 lineEnd = secondLineAttribute.GetEndPoint();
				Vector3 lineCenter = (lineStart + lineEnd) / 2;

				Vector3 normalVec = lineStart - projectPnt0;
				normalVec.Normalize();


				Vector3 planeProPnt = tCoordinate + normalVec * distance;
				Vector3 lineProPnt = lineCenter - normalVec*distance;
				Vector3 center1 = (planeProPnt + lineCenter) / 2;
				Vector3 center2 = (tCoordinate + lineProPnt) / 2;

				Color line1Color = Color::RED;

				for (vector<Line3D*>::iterator ite = measureDistance->m_LineList.begin(); ite != measureDistance->m_LineList.end();)
				{
					Line3D* tmpLine = *ite;
					if (tmpLine && !tmpLine->GetCanDelete())
					{
						ite++;
					}
					else
					{
						delete *ite;
						ite = measureDistance->m_LineList.erase(ite);
					}
				}

				Vector3 fCoordiante;
				if (!scene->GetPickPoint(screenPnt, fCoordiante, false))
				{
					delete measure;
					measure = NULL;
					return state;
				}


				Vector3 directionVector = center2 - center1;
				Plane projPlane(center1, center2, fCoordiante);
				Vector3 normalVector = projPlane.m_normal;
				normalVector = normalVector.CrossProduct(directionVector);
				normalVector.Normalize();


				Ray ray(center1, directionVector);
				Vector3 projecPnt = ray.Project(fCoordiante);
				distance = (projecPnt - fCoordiante).Length();

				Vector3 pnt1 = center1 + normalVector*distance;
				Vector3 pnt2 = center2 + normalVector*distance;

				Line3D* m1 = new Line3D(center1, pnt1);
				m1->SetCanDelete(true);
				m1->SetColor(line1Color);
				m1->SetLineWidth(1);
				Line3D* m2 = new Line3D(center2, pnt2);
				m2->SetCanDelete(true);
				m2->SetColor(line1Color);
				m2->SetLineWidth(1);
				Line3D* m3 = new Line3D(pnt1, pnt2);
				m3->SetCanDelete(true);
				m3->SetColor(line1Color);
				m3->SetLineWidth(1);

				Line3D* m4 = NULL;
				if (!(MeasureCalculateHelper::pntInSegment(fCoordiante, pnt1, pnt2, projecPnt)))
				{
					m4 = new Line3D(projecPnt, fCoordiante);
					m4->SetCanDelete(true);
					m4->SetColor(line1Color);
					m4->SetLineWidth(1);
				}
				measure->AddLine(m1);
				measure->AddLine(m2);
				measure->AddLine(m3);
				if (m4)
				{
					measure->AddLine(m4);
				}

				for (int i = 0; i < measure->m_imageBoardList.size(); i++) ///图片列表
				{
					measure->m_imageBoardList[i]->SetPosition(fCoordiante);
				}

				state = true;
			}
			else
			{
				delete measure;
				measure = NULL;
				return state;
			}
		}
		return state;
	}

	bool MeasureFactory::completeDesignerFaceToFaceDistance(Measure* measure, int firstFaceId, int secondFaceId,
		int thirdPntId, int forthPntId,
		Vector2& screenPnt, SceneManager* scene)
	{
		bool state = false;
		if (!measure || !scene)
		{
			return state;
		}

		MeasureDistance* measureDistance = dynamic_cast<MeasureDistance*>(measure);
		if (!measureDistance)
		{
			return state;
		}

		IShape * firstShape = scene->GetShape(firstFaceId);
		IShape * secondShape = scene->GetShape(secondFaceId);
		IShape * thirdShape = scene->GetShape(thirdPntId);
		IShape * forthShape = scene->GetShape(forthPntId);

		if (firstShape && secondShape && thirdShape&& forthShape&&
			firstShape->GetType() == SHAPE_FACE
			&& secondShape->GetType() == SHAPE_FACE
			&&thirdShape->GetType() == SHAPE_POINT_HANDLE
			&&forthShape->GetType() == SHAPE_POINT_HANDLE)
		{
			Face * firstFace = dynamic_cast<Face*>(firstShape);
			Face * secondFace = dynamic_cast<Face*>(secondShape);

			Mesh * firstMeshData = (Mesh*)firstFace->GetData();
			Mesh * secondMeshData = (Mesh *)secondFace->GetData();

			HandlerPoint * thirdPnt = (HandlerPoint *)thirdShape;
			Vector3 thirdCoordinate = thirdPnt->GetPosition();
			HandlerPoint * forthPnt = (HandlerPoint *)forthShape;
			Vector3 forthCoordinate = forthPnt->GetPosition();

			GeometryAttribute * firstGeo = firstFace->GetGeoAttribute();
			GeometryAttribute * secondGeo = secondFace->GetGeoAttribute();

			if (firstGeo != NULL
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE
				&& secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
			{
				PlaneFaceAttribute firstPlaneFaceAttribute =
					*((PlaneFaceAttribute *)firstGeo);
				PlaneFaceAttribute secondPlaneFaceAttribute =
					*((PlaneFaceAttribute *)secondGeo);

				Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstFace);
				Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(
					secondFace);
				GeometryHelper::Transform(firstPlaneFaceAttribute, worldMatrix1);
				GeometryHelper::Transform(secondPlaneFaceAttribute, worldMatrix2);

				float distance = 0.0f;
				Vector3 projectPnt0, projectPnt1;

				if (!MeasureCalculateHelper::FaceFaceDistance(firstPlaneFaceAttribute, secondPlaneFaceAttribute, distance,
					projectPnt0, projectPnt1))
				{
					delete measure;
					measure = NULL;
					return state;
				}

				Vector3 originVec = firstPlaneFaceAttribute.GetOrigin();
				Vector3 direction = projectPnt0 - originVec;
				direction.Normalize();

				Vector3 pnt1 = thirdCoordinate + direction * distance;
				Vector3 pnt2 = forthCoordinate - direction * distance;

				Vector3 center1 = (thirdCoordinate + pnt2) / 2;
				Vector3 center2 = (forthCoordinate + pnt1) / 2;

				Vector3 fCoordiante;
				if (!scene->GetPickPoint(screenPnt, fCoordiante, false))
				{
					delete measure;
					measure = NULL;
					return state;
				}

				Vector3 directionVector = center2 - center1;
				Plane projPlane(center1, center2, fCoordiante);
				Vector3 normalVector = projPlane.m_normal;
				normalVector = normalVector.CrossProduct(directionVector);
				normalVector.Normalize();

				Ray ray(center1, directionVector);
				Vector3 projecPnt = ray.Project(fCoordiante);
				distance = (projecPnt - fCoordiante).Length();


				pnt1 = center1 + normalVector*distance;
				pnt2 = center2 + normalVector*distance;

				for (vector<Line3D*>::iterator ite = measureDistance->m_LineList.begin(); ite != measureDistance->m_LineList.end();)
				{
					Line3D* tmpLine = *ite;
					if (tmpLine && !tmpLine->GetCanDelete())
					{
						ite++;
					}
					else
					{
						delete *ite;
						ite = measureDistance->m_LineList.erase(ite);
					}
				}

				Color line1Color = Color::RED;
				Line3D* m1 = new Line3D(center1, pnt1);
				m1->SetCanDelete(true);
				m1->SetColor(line1Color);
				m1->SetLineWidth(1);
				Line3D* m2 = new Line3D(center2, pnt2);
				m2->SetCanDelete(true);
				m2->SetColor(line1Color);
				m2->SetLineWidth(1);
				Line3D* m3 = new Line3D(pnt1, pnt2);
				m3->SetCanDelete(true);
				m3->SetColor(line1Color);
				m3->SetLineWidth(1);

				Line3D* m4 = NULL;
				if (!(MeasureCalculateHelper::pntInSegment(fCoordiante, pnt1, pnt2, projecPnt)))
				{
					m4 = new Line3D(projecPnt, fCoordiante);
					m4->SetCanDelete(true);
					m4->SetColor(line1Color);
					m4->SetLineWidth(1);
				}

				measure->AddLine(m1);
				measure->AddLine(m2);
				measure->AddLine(m3);
				if (m4)
				{
					measure->AddLine(m4);
				}

				for (int i = 0; i < measure->m_imageBoardList.size(); i++) ///图片列表
				{
					measure->m_imageBoardList[i]->SetPosition(fCoordiante);
				}
				state = true;
			}
		}
		return state;
	}

	void MeasureFactory::DeleteMeasureLine(Measure* measure)
	{
		if (!measure)
		{
			return;
		}
		for (vector<Line3D*>::iterator ite = measure->m_LineList.begin(); ite != measure->m_LineList.end();)
		{
			Line3D* tmpLine = *ite;
			if (tmpLine && !tmpLine->GetCanDelete())
			{
				ite++;
			}
			else
			{
				delete *ite;
				ite = measure->m_LineList.erase(ite);
			}
		}
	}

	Measure* MeasureFactory::CreateTmpAngleMeasure(int firstShapeID, int secondShapID,
		int thirdShapeID, int forthShapID,
		int type, SceneManager* scene)
	{
		Measure* measure = NULL;

		//判断场景是否为空
		if (scene == NULL)
		{
			return measure;
		}

		//填充meaureNote,根据不同的测量类型创建不同的Note对象
		if (type == Measure::MEASURE_TYPE_LINE_LINE_ANGLE) ///两条非平行线间的夹角
		{
			measure = createDesignerLineToLineAngle(firstShapeID, secondShapID, scene);
		}
		else if (type == Measure::MEASURE_TYPE_FACE_FACE_ANGLE)
		{
			measure = createDesignerFaceToFaceAngle(firstShapeID, secondShapID, thirdShapeID, forthShapID,
				scene);
		}
		else if (type == Measure::MEASURE_TYPE_LINE_FACE_ANGLE)
		{
			measure = createDesignerLineToFaceAngle(firstShapeID, secondShapID, thirdShapeID, scene);
		}
		AddMeasureToScene(scene, measure);

		if (measure)
		{
			measure->SetFrontShow(true);
		}
		return measure;
	}

	Measure* MeasureFactory::createDesignerLineToLineAngle(int firstPntId, int secondPntId, SceneManager* scene)
	{
		Measure* measure = NULL;

		IShape* firstShape = scene->GetShape(firstPntId);
		IShape* secondShape = scene->GetShape(secondPntId);

		bool firstFinished = false;
		LineAttribute firstLineAttribute;
		if (firstShape && firstShape->GetType() == SHAPE_EDGE)
		{
			measure = new MeasureAngle;

			measure->SetMeasureType(Measure::MEASURE_TYPE_LINE_LINE_ANGLE);

			Edge* firstEdge = (Edge*)firstShape;
			RefPolyLine * firstLineSet = firstEdge->GetLineData();
			GeometryAttribute* firstGeo = firstEdge->GetGeoAttribute();
			///暂时仅支持，点到直线距离的测量
			if (firstGeo && firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE)
			{
				firstLineAttribute = *((LineAttribute*)firstGeo);

				Matrix3x4 worldMatrix = ShapeHelper::GetShapeWorldMatrix(
					firstEdge);

				GeometryHelper::Transform(firstLineAttribute, worldMatrix);

				Vector3 pntStartPoint = firstLineAttribute.GetStartPoint(); //!< Line起点
				Vector3 pntEndPoint = firstLineAttribute.GetEndPoint(); //!< Line终点

				Color lineColor = Color::RED;
				Line3D* line = new Line3D(pntStartPoint, pntEndPoint);
				line->SetColor(lineColor);
				measure->AddLine(line);

				Point* pnt1 = new Point(pntStartPoint);
				pnt1->SetDrawType(1);
				pnt1->SetSize(0.8f);
				Point* pnt2 = new Point(pntEndPoint);
				pnt2->SetDrawType(1);
				pnt2->SetSize(0.8f);

				measure->AddPoint(pnt1);
				measure->AddPoint(pnt2);

				firstFinished = true;
			}
			else
			{
				delete measure;
				measure = NULL;
			}

		}
		
		if (firstFinished && secondShape &&  secondShape->GetType() == SHAPE_EDGE)
		{
			if (measure == NULL)
			{
				return measure;
			}

			Edge* secondEdge = (Edge*)secondShape;
			RefPolyLine * secondLineSet = secondEdge->GetLineData();
			GeometryAttribute* secondGeo = secondEdge->GetGeoAttribute();
			///暂时仅支持，点到直线距离的测量
			if (secondGeo && secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE)
			{
				LineAttribute secondLineAttribute = *((LineAttribute*)secondGeo);

				Matrix3x4 worldMatrix = ShapeHelper::GetShapeWorldMatrix(
					secondEdge);

				GeometryHelper::Transform(secondLineAttribute, worldMatrix);

				float distance;
				Vector3 projectPnt1, projectPnt2;
				bool parallel = true;
				MeasureCalculateHelper::LineLineDistance(firstLineAttribute,
					secondLineAttribute, distance, projectPnt1, projectPnt2,
					parallel);

				if (parallel || distance > 0.001)
				{
					delete measure;
					measure = NULL;
					return measure;
				}

				Vector3 pntStartPoint = secondLineAttribute.GetStartPoint(); //!< Line起点
				Vector3 pntEndPoint = secondLineAttribute.GetEndPoint(); //!< Line终点

				Color lineColor = Color::RED;
				Line3D* line = new Line3D(pntStartPoint, pntEndPoint);
				line->SetColor(lineColor);
				measure->AddLine(line);

				Point* pnt1 = new Point(pntStartPoint);
				pnt1->SetDrawType(1);
				pnt1->SetSize(0.8f);
				Point* pnt2 = new Point(pntEndPoint);
				pnt2->SetDrawType(1);
				pnt2->SetSize(0.8f);

				measure->AddPoint(pnt1);
				measure->AddPoint(pnt2);

				firstFinished = true;
			}
			else
			{
				delete measure;
				measure = NULL;
			}
		}

		return measure;
	}
	Measure* MeasureFactory::createDesignerFaceToFaceAngle(int firstFaceId, int secondFaceId,
		int thirdPntId, int forthPntId, SceneManager* scene)
	{
		Measure* measure = NULL;

		IShape* firstShape = scene->GetShape(firstFaceId);
		IShape* secondShape = scene->GetShape(secondFaceId);
		IShape * thirdShape = scene->GetShape(thirdPntId);
		IShape * forthShape = scene->GetShape(forthPntId);

		bool firstFinished = false;
		
		if (firstShape && thirdShape
			&& firstShape->GetType() == SHAPE_FACE && thirdShape->GetType() == SHAPE_POINT_HANDLE
			&& (!secondShape || secondShape->GetType() != SHAPE_FACE))
		{
			measure = new MeasureAngle;
			measure->SetMeasureType(Measure::MEASURE_TYPE_FACE_FACE_ANGLE);
			Face * firstFace = dynamic_cast<Face*>(firstShape);
			Mesh * firstMeshData = firstFace->GetData();
			GeometryAttribute * firstGeo = firstFace->GetGeoAttribute();

			HandlerPoint * thirdPnt = (HandlerPoint *)thirdShape;
			Vector3 thirdCoordinate = thirdPnt->GetPosition();

			if (firstGeo && firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
			{
				PlaneFaceAttribute firstPlaneFaceAttribute =
					*((PlaneFaceAttribute *)firstGeo);

				Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstFace);
				GeometryHelper::Transform(firstPlaneFaceAttribute, worldMatrix1);

				float distance = 0;
				Vector3 projectPnt;
				MeasureCalculateHelper::PntFaceDistance(thirdCoordinate, firstPlaneFaceAttribute, distance, projectPnt);

				BoundingBox& box = firstMeshData->GetBoundingBox();
				distance = (box.m_max - box.m_min).Length() / 20;

				Vector3 normalVec = firstPlaneFaceAttribute.GetNormal();
				Vector3 originVec = firstPlaneFaceAttribute.GetOrigin();

				Vector3 direction = projectPnt - originVec;

				vector<Vector3> pointArray;
				MeasureDisplayHelper::CreateSelectFaceMark(pointArray, projectPnt, direction,
					normalVec, distance);//平面标志

				Color line1Color = Color::RED;

				Line3D* line1 = new Line3D(pointArray[0], pointArray[1]);  //4
				line1->SetColor(line1Color);
				line1->SetLineWidth(1);
				Line3D* line2 = new Line3D(pointArray[2], pointArray[3]);
				line2->SetColor(line1Color);
				line2->SetLineWidth(1);
				Line3D* line3 = new Line3D(pointArray[0], pointArray[2]);
				line3->SetColor(line1Color);
				line3->SetLineWidth(1);
				Line3D* line4 = new Line3D(pointArray[3], pointArray[1]);
				line4->SetColor(line1Color);
				line4->SetLineWidth(1);


				measure->AddLine(line1);
				measure->AddLine(line2);
				measure->AddLine(line3);
				measure->AddLine(line4);

				Point* pnt = new Point(thirdCoordinate);
				pnt->SetDrawType(1);
				pnt->SetSize(0.8f);
				measure->AddPoint(pnt);

				firstFinished = true;
			}
			else
			{
				delete measure;
				measure = NULL;
			}
		}
		else if (firstShape && thirdShape && secondShape && forthShape
			&& firstShape->GetType() == SHAPE_FACE && thirdShape->GetType() == SHAPE_POINT_HANDLE
			&& secondShape->GetType() == SHAPE_FACE && forthShape->GetType() == SHAPE_POINT_HANDLE)
		{
			measure = new MeasureAngle;
			measure->SetMeasureType(Measure::MEASURE_TYPE_FACE_FACE_ANGLE);

			Face * firstFace = dynamic_cast<Face*>(firstShape);
			Face * secondFace = dynamic_cast<Face*>(secondShape);
			Mesh * firstMeshData = firstFace->GetData();
			Mesh * secondMeshData = secondFace->GetData();
			GeometryAttribute * firstGeo = firstFace->GetGeoAttribute();
			GeometryAttribute * secondGeo = secondFace->GetGeoAttribute();

			HandlerPoint * thirdPnt = (HandlerPoint *)thirdShape;
			Vector3 thirdCoordinate = thirdPnt->GetPosition();

			HandlerPoint * forthPnt = (HandlerPoint *)forthShape;
			Vector3 forthCoordinate = forthPnt->GetPosition();
			if (firstGeo && secondGeo
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
			{
				PlaneFaceAttribute firstPlaneFaceAttribute =
					*((PlaneFaceAttribute *)firstGeo);
				PlaneFaceAttribute secondPlaneFaceAttribute =
					*((PlaneFaceAttribute *)secondGeo);

				Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstFace);
				Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(
					secondFace);
				GeometryHelper::Transform(firstPlaneFaceAttribute, worldMatrix1);
				GeometryHelper::Transform(secondPlaneFaceAttribute, worldMatrix2);

				float angle;
				MeasureCalculateHelper::FaceFaceAngle(firstPlaneFaceAttribute,
					secondPlaneFaceAttribute, angle);

				Color line1Color = Color::RED;
				Vector3 linePnt1, linePnt2, centerPnt;
				float radius = 0.0f;
				if (abs(angle - 180) < 0.001 || abs(angle) < 0.001)
				{
					delete measure;
					measure = NULL;
					return measure;
				}
				else if (abs(angle - 90) <= 0.001)
				{
					Plane face1Plane(firstPlaneFaceAttribute.GetNormal(), thirdCoordinate);
					Vector3 footPnt = face1Plane.Project(forthCoordinate);

					Ray VerticalLine(footPnt, secondPlaneFaceAttribute.GetNormal());
					Vector3 firstFaceLine = VerticalLine.Project(thirdCoordinate);

					Line3D* line1 = new Line3D(footPnt, forthCoordinate);
					line1->SetColor(line1Color);
					Line3D* line2 = new Line3D(footPnt, firstFaceLine);
					line2->SetColor(line1Color);
					measure->AddLine(line1);
					measure->AddLine(line2);

					linePnt1 =  forthCoordinate;
					linePnt2 = firstFaceLine;
					centerPnt = footPnt;
					radius = (forthCoordinate - footPnt).Length() * 0.5f;

					Point* pnt1 = new Point(footPnt);
					pnt1->SetDrawType(1);
					pnt1->SetSize(0.8f);

					Point* pnt2 = new Point(forthCoordinate);
					pnt2->SetDrawType(1);
					pnt2->SetSize(0.8f);

					Point* pnt3 = new Point(firstFaceLine);
					pnt3->SetDrawType(1);
					pnt3->SetSize(0.8f);

					measure->AddPoint(pnt1);
					measure->AddPoint(pnt2);
					measure->AddPoint(pnt3);
				}
				else
				{
					Plane face1Plane(firstPlaneFaceAttribute.GetNormal(), thirdCoordinate);
					Plane face2Plane(secondPlaneFaceAttribute.GetNormal(), forthCoordinate);

					Vector3 firstPnt = face1Plane.Project(forthCoordinate);

					Ray secondRay(forthCoordinate, secondPlaneFaceAttribute.GetNormal());
					Vector3 secondPnt;
					if (!face1Plane.GetInsectPnt(secondRay, secondPnt))
					{
						secondRay.Define(forthCoordinate, -secondPlaneFaceAttribute.GetNormal());
						if (!face1Plane.GetInsectPnt(secondRay, secondPnt))
						{
							delete measure;
							measure = NULL;
							return measure;
						}
					}
					Ray firstRay(secondPnt, (firstPnt - secondPnt));

					if (!face2Plane.GetInsectPnt(firstRay, secondPnt))
					{
						firstRay.Define(secondPnt, (secondPnt - firstPnt));
						if (!face2Plane.GetInsectPnt(firstRay, secondPnt))
						{
							delete measure;
							measure = NULL;
							return measure;
						}
					}

					Vector3 forthPnt = firstRay.Project(thirdCoordinate);

					Line3D* line1 = new Line3D(forthCoordinate, secondPnt);
					line1->SetColor(line1Color);
					Line3D* line2 = new Line3D(forthPnt, secondPnt);
					line2->SetColor(line1Color);
					measure->AddLine(line1);
					measure->AddLine(line2);

					linePnt1 = forthCoordinate;
					linePnt2 = forthPnt;
					centerPnt = secondPnt;
					radius = (forthCoordinate - secondPnt).Length() * 0.5f;

					angle = (forthCoordinate - secondPnt).Angle(forthPnt - secondPnt);

					Point* pnt1 = new Point(forthCoordinate);
					pnt1->SetDrawType(1);
					pnt1->SetSize(0.8f);
					Point* pnt2 = new Point(secondPnt);
					pnt2->SetDrawType(1);
					pnt2->SetSize(0.8f);
					Point* pnt3 = new Point(forthPnt);
					pnt3->SetDrawType(1);
					pnt3->SetSize(0.8f);

					measure->AddPoint(pnt1);
					measure->AddPoint(pnt2);
					measure->AddPoint(pnt3);
				}

				if ((linePnt2 - centerPnt).Length() < radius)
				{
					Vector3 direction = linePnt2 - centerPnt;
					direction.Normalize();
					Vector3 otherPnt = centerPnt + direction * radius;
					Line3D* otherLine = new Line3D(linePnt2, otherPnt);
					otherLine->SetColor(line1Color);
					measure->AddLine(otherLine);
				}

				vector<Vector3> linesTemp;
				MeasureDisplayHelper::CreateDesignerAngleMark(centerPnt, linePnt1, centerPnt, linePnt2,
					centerPnt, radius, angle, linesTemp);

				if (linesTemp.size() > 0)
				{
					for (int i = 0; i < linesTemp.size() - 1; i++)
					{
						Line3D * lineTemp = new Line3D(linesTemp[i],
							linesTemp[i + 1]);
						lineTemp->SetColor(line1Color);
						lineTemp->SetCanDelete(true);
						measure->AddLine(lineTemp);
					}
				}
				linePnt1 = (linePnt1 + centerPnt) / 2;

				string unit("°");
				string lengthstr = M3DTOOLS::floatToString(angle) + unit;

				//解决协同中文本读取
				ComText* ct = new ComText();
				CText* t = new CText;
				t->SetText(lengthstr);
				ct->AddCText(t);
				measure->m_ComTexts.push_back(ct);

				vector<Texts2D*> temptext;
				Texts2D *title = new Texts2D;
				title->m_size = 12.0f;
				title->m_texts = "内容";
				temptext.push_back(title);

				Texts2D *content = new Texts2D;
				content->m_size = 12.0f;
				content->m_texts = lengthstr;
				temptext.push_back(content);

				ImageBoard * imageBroad = MeasureDisplayHelper::createNoteTextsImageN(scene, temptext, linePnt1);
				measure->AddImage(imageBroad);
			}
			else
			{
				delete measure;
				measure = NULL;
			}
		}

		return measure;
	}

	Measure* MeasureFactory::createDesignerLineToFaceAngle(int firstFaceId, int secondLineId,
		int thirdPntId, SceneManager* scene)
	{
		Measure* measure = NULL;

		IShape* firstShape = scene->GetShape(firstFaceId);
		IShape* secondShape = scene->GetShape(secondLineId);
		IShape * thirdShape = scene->GetShape(thirdPntId);

		bool firstFinished = false, secondFinished = false;
		PlaneFaceAttribute firstPlaneFaceAttribute;
		LineAttribute secondLineAttribute;
		Vector3 thirdCoordinate;
		float distance = 0;
		if (firstShape && thirdShape
			&& firstShape->GetType() == SHAPE_FACE && thirdShape->GetType() == SHAPE_POINT_HANDLE)
		{
			measure = new MeasureAngle;
			measure->SetMeasureType(Measure::MEASURE_TYPE_LINE_FACE_ANGLE);

			Face * firstFace = dynamic_cast<Face*>(firstShape);
			Mesh * firstMeshData = firstFace->GetData();
			GeometryAttribute * firstGeo = firstFace->GetGeoAttribute();

			HandlerPoint * thirdPnt = (HandlerPoint *)thirdShape;
			thirdCoordinate = thirdPnt->GetPosition();

			if (firstGeo && firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
			{
				firstPlaneFaceAttribute =
					*((PlaneFaceAttribute *)firstGeo);

				Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstFace);
				GeometryHelper::Transform(firstPlaneFaceAttribute, worldMatrix1);
				
				Vector3 projectPnt;
				MeasureCalculateHelper::PntFaceDistance(thirdCoordinate, firstPlaneFaceAttribute, distance, projectPnt);

				BoundingBox& box = firstMeshData->GetBoundingBox();
				distance = (box.m_max - box.m_min).Length() / 20;

				Vector3 normalVec = firstPlaneFaceAttribute.GetNormal();
				Vector3 originVec = firstPlaneFaceAttribute.GetOrigin();

				Vector3 direction = projectPnt - originVec;

				vector<Vector3> pointArray;
				MeasureDisplayHelper::CreateSelectFaceMark(pointArray, projectPnt, direction,
					normalVec, distance);//平面标志

				Color line1Color = Color::RED;

				Line3D* line1 = new Line3D(pointArray[0], pointArray[1]);  //4
				line1->SetColor(line1Color);
				line1->SetLineWidth(1);
				Line3D* line2 = new Line3D(pointArray[2], pointArray[3]);
				line2->SetColor(line1Color);
				line2->SetLineWidth(1);
				Line3D* line3 = new Line3D(pointArray[0], pointArray[2]);
				line3->SetColor(line1Color);
				line3->SetLineWidth(1);
				Line3D* line4 = new Line3D(pointArray[3], pointArray[1]);
				line4->SetColor(line1Color);
				line4->SetLineWidth(1);


				measure->AddLine(line1);
				measure->AddLine(line2);
				measure->AddLine(line3);
				measure->AddLine(line4);

				Point* pnt = new Point(thirdCoordinate);
				pnt->SetDrawType(1);
				pnt->SetSize(0.8f);
				measure->AddPoint(pnt);

				firstFinished = true;
			}
			else
			{
				delete measure;
				measure = NULL;
			}
		}
		if (firstFinished && !secondFinished && secondShape && secondShape->GetType() == SHAPE_EDGE)
		{
			if (measure == NULL)
			{
				return measure;
			}
			Edge * secondEdge = (Edge*)secondShape;
			RefPolyLine * secondLineSet = secondEdge->GetLineData();
			GeometryAttribute * secondGeo = secondEdge->GetGeoAttribute();
			if (secondGeo && secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE)
			{
				secondLineAttribute = *((LineAttribute *)secondGeo);

				Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					secondEdge);

				GeometryHelper::Transform(secondLineAttribute, worldMatrix1);

				Vector3 line1Start = secondLineAttribute.GetStartPoint();
				Vector3 line1End = secondLineAttribute.GetEndPoint();

				Color line1Color = Color::RED;
				Line3D* line1 = new Line3D(line1Start, line1End);  //4
				line1->SetColor(line1Color);

				measure->AddLine(line1);

				Point* pnt1 = new Point(line1Start);
				pnt1->SetDrawType(1);
				pnt1->SetSize(0.8f);
				Point* pnt2 = new Point(line1End);
				pnt2->SetDrawType(1);
				pnt2->SetSize(0.8f);
				measure->AddPoint(pnt1);
				measure->AddPoint(pnt2);
				secondFinished = true;
			}
			else
			{
				delete measure;
				measure = NULL;
			}
		}

		if (firstFinished && secondFinished)
		{
			float angle;
			MeasureCalculateHelper::LineFaceAngle(secondLineAttribute, firstPlaneFaceAttribute, angle);
			if (abs(angle - 180) < 0.001 || abs(angle) < 0.001)
			{
				delete measure;
				measure = NULL;
				return measure;
			}
			else
			{
				Plane facePlane(firstPlaneFaceAttribute.GetNormal(), firstPlaneFaceAttribute.GetOrigin());

				Vector3 line1Start = secondLineAttribute.GetStartPoint();
				Vector3 line1End = secondLineAttribute.GetEndPoint();
				Ray lineRay(line1Start, line1Start - line1End);

				Vector3 insectPnt, projecPnt;
				if (!facePlane.GetInsectPnt(lineRay, insectPnt))
				{
					lineRay.Define(line1Start, line1End - line1Start);
					if (!facePlane.GetInsectPnt(lineRay, insectPnt))
					{
						delete measure;
						measure = NULL;
						return measure;
					}
				}

				Color lineColor = Color::RED;
				if (!(MeasureCalculateHelper::pntInSegment(insectPnt, line1Start, line1End, projecPnt)))
				{
					Line3D * line = new Line3D(projecPnt, insectPnt);
					line->SetName("dottedLine");
					line->SetColor(lineColor);
					measure->AddLine(line);
				}

				Line3D* line1 = new Line3D(thirdCoordinate, insectPnt);
				line1->SetName("dottedLine");
				line1->SetColor(lineColor);
				measure->AddLine(line1);

				Point* pnt1 = new Point(insectPnt);
				pnt1->SetDrawType(1);
				pnt1->SetSize(0.8f);
				measure->AddPoint(pnt1);

				vector<Vector3> linesTemp;
				float radius = (line1Start - line1End).Length();
				MeasureDisplayHelper::CreateDesignerAngleMark(insectPnt, (line1Start + line1End) / 2, insectPnt, thirdCoordinate,
					insectPnt, radius, angle, linesTemp);

				for (int i = 0; i < linesTemp.size() - 1; i++)
				{
					Line3D * lineTemp = new Line3D(linesTemp[i],
						linesTemp[i + 1]);
					lineTemp->SetColor(lineColor);
					lineTemp->SetCanDelete(true);
					measure->AddLine(lineTemp);
				}

				Vector3 showPnt = insectPnt;
				if (linesTemp.size() > 0)
				{
					showPnt = linesTemp[0];
				}

				string unit("°");
				string lengthstr = M3DTOOLS::floatToString(angle) + unit;

				//解决协同中文本读取
				ComText* ct = new ComText();
				CText* t = new CText;
				t->SetText(lengthstr);
				ct->AddCText(t);
				measure->m_ComTexts.push_back(ct);

				vector<Texts2D*> temptext;
				Texts2D *title = new Texts2D;
				title->m_size = 12.0f;
				title->m_texts = "内容";
				temptext.push_back(title);

				Texts2D *content = new Texts2D;
				content->m_size = 12.0f;
				content->m_texts = lengthstr;
				temptext.push_back(content);

				ImageBoard * imageBroad = MeasureDisplayHelper::createNoteTextsImageN(scene, temptext, showPnt);
				measure->AddImage(imageBroad);
			}
		}

		return measure;
	}

	bool MeasureFactory::CompleteAngleMeasure(Measure* measure, int firstShapeID, int secondShapID,
		int thirdShapeID, int forthShapID,
		Vector2 screenPnt, SceneManager* scene)
	{
		bool state = false;
		if (!measure || !scene)
		{
			return state;
		}

		int type = measure->GetMeasureType();
		if (type == Measure::MEASURE_TYPE_LINE_LINE_ANGLE) 
		{
			state = completeDesignerLineToLineAngle(measure, firstShapeID, secondShapID, screenPnt, scene);
		}
		else if (type == Measure::MEASURE_TYPE_FACE_FACE_ANGLE)
		{
			state = completeDesignerFaceToFaceAngle(measure, firstShapeID, secondShapID,
				thirdShapeID, forthShapID, screenPnt, scene);
		}
		else if (type == Measure::MEASURE_TYPE_LINE_FACE_ANGLE)
		{
			state = completeDesignerLineToFaceAngle(measure, firstShapeID, secondShapID,
				thirdShapeID, screenPnt, scene);
		}
		return state;
	}

	bool MeasureFactory::completeDesignerLineToLineAngle(Measure* measure, int firstLineId, int secondLineId,
		Vector2& screenPnt, SceneManager* scene)
	{
		bool state = false;
		if (!measure || !scene)
		{
			return state;
		}

		MeasureAngle* measureAngle = dynamic_cast<MeasureAngle*>(measure);
		if (!measureAngle)
		{
			return state;
		}

		IShape* firstShape = scene->GetShape(firstLineId);
		IShape* secondShape = scene->GetShape(secondLineId);

		if (firstShape && secondShape 
			&& firstShape->GetType() == SHAPE_EDGE
			&& secondShape->GetType() == SHAPE_EDGE)
		{
			Edge * firstEdge = (Edge*)firstShape;
			Edge * secondEdge = (Edge *)secondShape;

			RefPolyLine * firstLineSet = firstEdge->GetLineData();
			RefPolyLine * secondLineSet = secondEdge->GetLineData();

			GeometryAttribute * firstGeo = firstEdge->GetGeoAttribute();
			GeometryAttribute * secondGeo = secondEdge->GetGeoAttribute();

			if (firstGeo != NULL
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE
				&& secondGeo != NULL
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE)
			{
				LineAttribute firstLineAttribute = *((LineAttribute *)firstGeo);
				LineAttribute secondLineAttribute = *((LineAttribute *)secondGeo);

				Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstEdge);
				Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(
					secondEdge);
				GeometryHelper::Transform(firstLineAttribute, worldMatrix1);
				GeometryHelper::Transform(secondLineAttribute, worldMatrix2);

				Vector3 line1Start = firstLineAttribute.GetStartPoint();
				Vector3 line1End = firstLineAttribute.GetEndPoint();
				Vector3 line1Center = (line1Start + line1End) / 2;

				Vector3 line2Start = secondLineAttribute.GetStartPoint();
				Vector3 line2End = secondLineAttribute.GetEndPoint();
				Vector3 line2Center = (line2Start + line2End) / 2;

				float distance;
				Vector3 projectPnt1, projectPnt2;
				bool parallel = true;
				MeasureCalculateHelper::LineLineDistance(firstLineAttribute,
					secondLineAttribute, distance, projectPnt1, projectPnt2,
					parallel);

				if (parallel || distance > 0.001)
				{
					delete measure;
					measure = NULL;
					return state;
				}

				DeleteMeasureLine(measure);

				Vector3 line1Direction = line1Center - projectPnt1;
				Vector3 line2Direction = line2Center - projectPnt1;
				line1Direction.Normalize();
				line2Direction.Normalize();

				Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

				Plane linePlane(line1Start, line1End, line2Center);
				Vector3 pntInPlane;
				linePlane.GetInsectPnt(cameraRay, pntInPlane);

				Vector3 pntDirection = pntInPlane - projectPnt1;
				float radius = pntDirection.Length();
				
				Vector3 projecPnt1, projecPnt2, projecPnt3, projecPnt4;
				Color lineColor = Color::RED;
				if (!(MeasureCalculateHelper::pntInSegment(projectPnt1, line1Start, line1End, projecPnt1)))
				{
					Line3D* tmpLine = new Line3D(projecPnt1, projectPnt1);
					tmpLine->SetCanDelete(true);
					tmpLine->SetName("dottedLine");
					tmpLine->SetColor(lineColor);
					measure->AddLine(tmpLine);
				}
				if (!(MeasureCalculateHelper::pntInSegment(projectPnt1, line2Start, line2End, projecPnt2)))
				{
					Line3D* tmpLine = new Line3D(projecPnt2, projectPnt1);
					tmpLine->SetCanDelete(true);
					tmpLine->SetName("dottedLine");
					tmpLine->SetColor(lineColor);
					measure->AddLine(tmpLine);
				}

				Vector3 radiusPnt1, radiusPnt2;

				float angleLine = line1Direction.Angle(line2Direction);
				float angle1 = pntDirection.Angle(line1Direction);
				float angle2 = pntDirection.Angle(line2Direction);

				float angle = 0.0f;
				vector<Vector3> linesTemp;
				if (angle1 <= angleLine && angle2 <= angleLine && abs(angle1 + angle2 - angleLine) < 0.01)
				{
					MeasureDisplayHelper::CreateDesignerAngleMark(projectPnt1, line1Center, projectPnt1, line2Center,
						projectPnt1, radius, angleLine, linesTemp);

					radiusPnt1 = projectPnt1 + line1Direction* radius;
					radiusPnt2 = projectPnt1 + line2Direction* radius;

					if (!(MeasureCalculateHelper::pntInSegment(radiusPnt1, line1Start, line1End, projecPnt3)) && projecPnt3 != projecPnt1)
					{
						Line3D* tmpLine = new Line3D(projecPnt3, radiusPnt1);
						tmpLine->SetCanDelete(true);
						tmpLine->SetName("dottedLine");
						tmpLine->SetColor(lineColor);
						measure->AddLine(tmpLine);
					}
					if (!(MeasureCalculateHelper::pntInSegment(radiusPnt2, line2Start, line2End, projecPnt4)) && projecPnt4 != projecPnt2)
					{
						Line3D* tmpLine = new Line3D(projecPnt4, radiusPnt2);
						tmpLine->SetCanDelete(true);
						tmpLine->SetName("dottedLine");
						tmpLine->SetColor(lineColor);
						measure->AddLine(tmpLine);
					}
				}
				else if ((angle1 > angleLine && angle1 > angle2)
					&& abs(angle1 - angle2 - angleLine) < 0.01)
				{
					angleLine = 180 - angleLine;
					MeasureDisplayHelper::CreateDesignerAngleMark(projectPnt1, line2Center, line1Center, projectPnt1,
						projectPnt1, radius, angleLine, linesTemp);

					radiusPnt1 = projectPnt1 - line1Direction* radius;
					radiusPnt2 = projectPnt1 + line2Direction* radius;

					Line3D* tmpLine = new Line3D(projectPnt1, radiusPnt1);
					tmpLine->SetCanDelete(true);
					tmpLine->SetName("dottedLine");
					tmpLine->SetColor(lineColor);
					measure->AddLine(tmpLine);
					if (!(MeasureCalculateHelper::pntInSegment(radiusPnt2, line2Start, line2End, projecPnt4)) && projecPnt4 != projecPnt2)
					{
						Line3D* tmpLine = new Line3D(projecPnt4, radiusPnt2);
						tmpLine->SetCanDelete(true);
						tmpLine->SetName("dottedLine");
						tmpLine->SetColor(lineColor);
						measure->AddLine(tmpLine);
					}
				}
				else if ((angle2 > angleLine && angle2 > angle1)
					&& abs(angle2 - angle1 - angleLine) < 0.01)
				{
					angleLine = 180 - angleLine;
					MeasureDisplayHelper::CreateDesignerAngleMark(projectPnt1, line1Center, line2Center, projectPnt1,
						projectPnt1, radius, angleLine, linesTemp);

					radiusPnt1 = projectPnt1 + line1Direction* radius;
					radiusPnt2 = projectPnt1 - line2Direction* radius;

					if (!(MeasureCalculateHelper::pntInSegment(radiusPnt1, line1Start, line1End, projecPnt3)) && projecPnt3 != projecPnt1)
					{
						Line3D* tmpLine = new Line3D(projecPnt3, radiusPnt1);
						tmpLine->SetCanDelete(true);
						tmpLine->SetName("dottedLine");
						tmpLine->SetColor(lineColor);
						measure->AddLine(tmpLine);
					}

					Line3D* tmpLine = new Line3D(projectPnt1, radiusPnt2);
					tmpLine->SetCanDelete(true);
					tmpLine->SetName("dottedLine");
					tmpLine->SetColor(lineColor);
					measure->AddLine(tmpLine);
				}
				else
				{
					MeasureDisplayHelper::CreateDesignerAngleMark(line1Center, projectPnt1, line2Center, projectPnt1,
						projectPnt1, radius, angleLine, linesTemp);

					radiusPnt1 = projectPnt1 - line1Direction* radius;
					radiusPnt2 = projectPnt1 - line2Direction* radius;

					Line3D* tmpLine = new Line3D(projectPnt1, radiusPnt1);
					tmpLine->SetCanDelete(true);
					tmpLine->SetName("dottedLine");
					tmpLine->SetColor(lineColor);
					measure->AddLine(tmpLine);

					tmpLine = new Line3D(projectPnt1, radiusPnt2);
					tmpLine->SetCanDelete(true);
					tmpLine->SetName("dottedLine");
					tmpLine->SetColor(lineColor);
					measure->AddLine(tmpLine);
				}
				
				if (linesTemp.size() > 0)
				{
					for (int i = 0; i < linesTemp.size() - 1; i++)
					{
						Line3D * lineTemp = new Line3D(linesTemp[i],
							linesTemp[i + 1]);
						lineTemp->SetColor(lineColor);
						lineTemp->SetCanDelete(true);
						measure->AddLine(lineTemp);
					}
				}

				for (int i = 0; i < measure->m_imageBoardList.size(); i++) ///图片列表
				{
					delete measure->m_imageBoardList[i];
				}
				measure->m_imageBoardList.resize(0);


				string unit("°");
				string lengthstr = M3DTOOLS::floatToString(angleLine) + unit;

				//解决协同中文本读取
				ComText* ct = new ComText();
				CText* t = new CText;
				t->SetText(lengthstr);
				ct->AddCText(t);
				measure->m_ComTexts.push_back(ct);

				vector<Texts2D*> temptext;
				Texts2D *title = new Texts2D;
				title->m_size = 12.0f;
				title->m_texts = "内容";
				temptext.push_back(title);

				Texts2D *content = new Texts2D;
				content->m_size = 12.0f;
				content->m_texts = lengthstr;
				temptext.push_back(content);

				ImageBoard * imageBroad = MeasureDisplayHelper::createNoteTextsImageN(scene, temptext, pntInPlane);
				measure->AddImage(imageBroad);

				state = true;
			}
			else
			{
				delete measure;
				measure = NULL;
			}
		}
		return state;
	}

	bool MeasureFactory::completeDesignerFaceToFaceAngle(Measure* measure, int firstFaceId, int secondFaceId,
		int thirdPntId, int forthPntId,
		Vector2& screenPnt, SceneManager* scene)
	{
		bool state = false;
		if (!measure || !scene)
		{
			return state;
		}

		MeasureAngle* measureAngle = dynamic_cast<MeasureAngle*>(measure);
		if (!measureAngle)
		{
			return state;
		}

		IShape* firstShape = scene->GetShape(firstFaceId);
		IShape* secondShape = scene->GetShape(secondFaceId);
		IShape * thirdShape = scene->GetShape(thirdPntId);
		IShape * forthShape = scene->GetShape(forthPntId);

		if (firstShape && secondFaceId && thirdShape && forthShape
			&& firstShape->GetType() == SHAPE_FACE
			&& secondShape->GetType() == SHAPE_FACE
			&& thirdShape->GetType() == SHAPE_POINT_HANDLE
			&& forthShape->GetType() == SHAPE_POINT_HANDLE)
		{
			Face * firstFace = dynamic_cast<Face*>(firstShape);
			Face * secondFace = dynamic_cast<Face*>(secondShape);
			Mesh * firstMeshData = firstFace->GetData();
			Mesh * secondMeshData = secondFace->GetData();
			GeometryAttribute * firstGeo = firstFace->GetGeoAttribute();
			GeometryAttribute * secondGeo = secondFace->GetGeoAttribute();

			HandlerPoint * thirdPnt = (HandlerPoint *)thirdShape;
			Vector3 thirdCoordinate = thirdPnt->GetPosition();

			HandlerPoint * forthPnt = (HandlerPoint *)forthShape;
			Vector3 forthCoordinate = forthPnt->GetPosition();

			if (firstGeo && secondGeo
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE)
			{
				PlaneFaceAttribute firstPlaneFaceAttribute =
					*((PlaneFaceAttribute *)firstGeo);
				PlaneFaceAttribute secondPlaneFaceAttribute =
					*((PlaneFaceAttribute *)secondGeo);

				Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(
					firstFace);
				Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(
					secondFace);
				GeometryHelper::Transform(firstPlaneFaceAttribute, worldMatrix1);
				GeometryHelper::Transform(secondPlaneFaceAttribute, worldMatrix2);

				DeleteMeasureLine(measure);

				float angle;
				MeasureCalculateHelper::FaceFaceAngle(firstPlaneFaceAttribute,
					secondPlaneFaceAttribute, angle);

				Vector3 linePnt1, linePnt2, centerPnt;
				if (abs(angle - 180) < 0.001 || abs(angle) < 0.001)
				{
					delete measure;
					measure = NULL;
					return state;
				}
				else if (abs(angle - 90) <= 0.001)
				{
					Plane face1Plane(firstPlaneFaceAttribute.GetNormal(), thirdCoordinate);
					Vector3 footPnt = face1Plane.Project(forthCoordinate);

					Ray VerticalLine(footPnt, secondPlaneFaceAttribute.GetNormal());
					Vector3 firstFaceLine = VerticalLine.Project(thirdCoordinate);

					linePnt1 = forthCoordinate;
					linePnt2 = firstFaceLine;
					centerPnt = footPnt;
				}
				else
				{
					Plane face1Plane(firstPlaneFaceAttribute.GetNormal(), thirdCoordinate);
					Plane face2Plane(secondPlaneFaceAttribute.GetNormal(), forthCoordinate);

					Vector3 firstPnt = face1Plane.Project(forthCoordinate);
					Ray secondRay(forthCoordinate, secondPlaneFaceAttribute.GetNormal());
					Vector3 secondPnt;
					if (!face1Plane.GetInsectPnt(secondRay, secondPnt))
					{
						secondRay.Define(forthCoordinate, -secondPlaneFaceAttribute.GetNormal());
						if (!face1Plane.GetInsectPnt(secondRay, secondPnt))
						{
							delete measure;
							measure = NULL;
							return measure;
						}
					}
					Ray firstRay(secondPnt, (firstPnt - secondPnt));
					if (!face2Plane.GetInsectPnt(firstRay, secondPnt))
					{
						firstRay.Define(secondPnt, (secondPnt - firstPnt));
						if (!face2Plane.GetInsectPnt(firstRay, secondPnt))
						{
							delete measure;
							measure = NULL;
							return measure;
						}
					}

					Vector3 forthPnt = firstRay.Project(thirdCoordinate);

					linePnt1 = forthCoordinate;
					linePnt2 = forthPnt;
					centerPnt = secondPnt;
				}

				Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

				Plane linePlane(linePnt1, linePnt2, centerPnt);
				Vector3 pntInPlane;
				linePlane.GetInsectPnt(cameraRay, pntInPlane);

				float radius = (pntInPlane - centerPnt).Length();
				angle = (linePnt1 - centerPnt).Angle(linePnt2 - centerPnt);

				Color line1Color = Color::RED;
				if ((linePnt2 - centerPnt).Length() < radius)
				{
					Vector3 direction = linePnt2 - centerPnt;
					direction.Normalize();
					Vector3 otherPnt = centerPnt + direction * radius;
					Line3D* otherLine = new Line3D(linePnt2, otherPnt);
					otherLine->SetColor(line1Color);
					otherLine->SetCanDelete(true);
					measure->AddLine(otherLine);
				}
				if ((linePnt1 - centerPnt).Length() < radius)
				{
					Vector3 direction = linePnt1 - centerPnt;
					direction.Normalize();
					Vector3 otherPnt = centerPnt + direction * radius;
					Line3D* otherLine = new Line3D(linePnt1, otherPnt);
					otherLine->SetColor(line1Color);
					otherLine->SetCanDelete(true);
					measure->AddLine(otherLine);
				}

				vector<Vector3> linesTemp;
				MeasureDisplayHelper::CreateDesignerAngleMark(centerPnt, linePnt1, centerPnt, linePnt2,
					centerPnt, radius, angle, linesTemp);
				if (linesTemp.size() > 0)
				{
					for (int i = 0; i < linesTemp.size() - 1; i++)
					{
						Line3D * lineTemp = new Line3D(linesTemp[i],
							linesTemp[i + 1]);
						lineTemp->SetColor(line1Color);
						lineTemp->SetCanDelete(true);
						measure->AddLine(lineTemp);
					}
				}

				float angle1 = (pntInPlane - centerPnt).Angle(linePnt1 - centerPnt);
				float angle2 = (pntInPlane - centerPnt).Angle(linePnt2 - centerPnt);
				if (angle1 - angle > 0.001 || angle2 - angle > 0.001)
				{
					linesTemp.clear();
					if (angle1 >= angle2)
					{
						MeasureDisplayHelper::CreateDesignerAngleMark(centerPnt, linePnt2, centerPnt, pntInPlane,
							centerPnt, radius, angle2, linesTemp);
					}
					else
					{
						MeasureDisplayHelper::CreateDesignerAngleMark(centerPnt, linePnt1, centerPnt, pntInPlane,
							centerPnt, radius, angle1, linesTemp);
					}
					for (int i = 0; i < linesTemp.size() - 1; i++)
					{
						Line3D * lineTemp = new Line3D(linesTemp[i],
							linesTemp[i + 1]);
						lineTemp->SetColor(line1Color);
						lineTemp->SetCanDelete(true);
						measure->AddLine(lineTemp);
					}
				}
				

				for (int i = 0; i < measure->m_imageBoardList.size(); i++) ///图片列表
				{
					measure->m_imageBoardList[i]->SetPosition(pntInPlane);
				}
				state = true;
			}
			else
			{
				delete measure;
				measure = NULL;
			}
		}
		return state;
	}

	bool MeasureFactory::completeDesignerLineToFaceAngle(Measure* measure, int firstFaceId, int secondLineId,
		int thirdPntId, Vector2& screenPnt, SceneManager* scene)
	{
		bool state = false;
		if (!measure || !scene)
		{
			return state;
		}

		MeasureAngle* measureAngle = dynamic_cast<MeasureAngle*>(measure);
		if (!measureAngle)
		{
			return state;
		}

		IShape* firstShape = scene->GetShape(firstFaceId);
		IShape* secondShape = scene->GetShape(secondLineId);
		IShape * thirdShape = scene->GetShape(thirdPntId);

		if (firstShape && secondShape && thirdShape
			&& firstShape->GetType() == SHAPE_FACE
			&& secondShape->GetType() == SHAPE_EDGE
			&& thirdShape->GetType() == SHAPE_POINT_HANDLE)
		{
			Face * firstFace = dynamic_cast<Face*>(firstShape);
			Mesh * firstMeshData = firstFace->GetData();
			GeometryAttribute * firstGeo = firstFace->GetGeoAttribute();

			Edge * secondEdge = (Edge*)secondShape;
			RefPolyLine * secondLineSet = secondEdge->GetLineData();
			GeometryAttribute * secondGeo = secondEdge->GetGeoAttribute();

			HandlerPoint * thirdPnt = (HandlerPoint *)thirdShape;
			Vector3 thirdCoordinate = thirdPnt->GetPosition();

			if (firstGeo && secondGeo
				&& firstGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_PLANEFACE
				&& secondGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE)
			{
				PlaneFaceAttribute firstPlaneFaceAttribute = *((PlaneFaceAttribute *)firstGeo);
				LineAttribute secondLineAttribute = *((LineAttribute *)secondGeo);
				Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(firstFace);
				Matrix3x4 worldMatrix2 = ShapeHelper::GetShapeWorldMatrix(secondEdge);
				GeometryHelper::Transform(firstPlaneFaceAttribute, worldMatrix1);
				GeometryHelper::Transform(secondLineAttribute, worldMatrix2);

				DeleteMeasureLine(measure);

				Plane facePlane(firstPlaneFaceAttribute.GetNormal(), firstPlaneFaceAttribute.GetOrigin());
				Vector3 line1Start = secondLineAttribute.GetStartPoint();
				Vector3 line1End = secondLineAttribute.GetEndPoint();
				Vector3 centerPnt = (line1Start + line1End) / 2;
				Ray lineRay(line1Start, line1Start - line1End);

				Vector3 insectPnt, projecPnt;
				if (!facePlane.GetInsectPnt(lineRay, insectPnt))
				{
					lineRay.Define(line1Start, line1End - line1Start);
					if (!facePlane.GetInsectPnt(lineRay, insectPnt))
					{
						delete measure;
						measure = NULL;
						return state;
					}
				}

				Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);
				Plane linePlane(thirdCoordinate, centerPnt, insectPnt);

				Vector3 pntInPlane;
				linePlane.GetInsectPnt(cameraRay, pntInPlane);

				float radius = (pntInPlane - insectPnt).Length();
				float angle = (thirdCoordinate - insectPnt).Angle(centerPnt - insectPnt);
				float length1 = (line1Start - insectPnt).Length();
				float length2 = (line1End - insectPnt).Length();
				float length3 = (thirdCoordinate - insectPnt).Length();
				Color lineColor = Color::RED;
				if (radius > length1 && radius > length2)
				{
					Vector3 direction = centerPnt  - insectPnt;
					direction.Normalize();
					Vector3 otherPnt = insectPnt + direction * radius;
					Line3D* otherLine = NULL;
					if (length1 > length2)
					{
						otherLine = new Line3D(line1Start, otherPnt);
					}
					else
					{
						otherLine = new Line3D(line1End, otherPnt);
					}
					otherLine->SetName("dottedLine");
					otherLine->SetColor(lineColor);
					otherLine->SetCanDelete(true);
					measure->AddLine(otherLine);
				}
				if (radius > length3)
				{
					Vector3 direction = thirdCoordinate - insectPnt;
					direction.Normalize();
					Vector3 otherPnt = insectPnt + direction * radius;
					Line3D* otherLine = new Line3D(thirdCoordinate, otherPnt);
					otherLine->SetName("dottedLine");
					otherLine->SetColor(lineColor);
					otherLine->SetCanDelete(true);
					measure->AddLine(otherLine);
				}

				vector<Vector3> linesTemp;
				MeasureDisplayHelper::CreateDesignerAngleMark(insectPnt, thirdCoordinate, insectPnt, centerPnt,
					insectPnt, radius, angle, linesTemp);
				if (linesTemp.size() > 0)
				{
					for (int i = 0; i < linesTemp.size() - 1; i++)
					{
						Line3D * lineTemp = new Line3D(linesTemp[i],
							linesTemp[i + 1]);
						lineTemp->SetColor(lineColor);
						lineTemp->SetCanDelete(true);
						measure->AddLine(lineTemp);
					}
				}

				float angle1 = (pntInPlane - insectPnt).Angle(thirdCoordinate - insectPnt);
				float angle2 = (pntInPlane - insectPnt).Angle(centerPnt - insectPnt);
				if (angle1 - angle > 0.001 || angle2 - angle > 0.001)
				{
					linesTemp.clear();
					if (angle1 >= angle2)
					{
						MeasureDisplayHelper::CreateDesignerAngleMark(insectPnt, centerPnt, insectPnt, pntInPlane,
							insectPnt, radius, angle2, linesTemp);
					}
					else
					{
						MeasureDisplayHelper::CreateDesignerAngleMark(insectPnt, thirdCoordinate, insectPnt, pntInPlane,
							insectPnt, radius, angle1, linesTemp);
					}
					for (int i = 0; i < linesTemp.size() - 1; i++)
					{
						Line3D * lineTemp = new Line3D(linesTemp[i],
							linesTemp[i + 1]);
						lineTemp->SetColor(lineColor);
						lineTemp->SetCanDelete(true);
						measure->AddLine(lineTemp);
					}
				}

				for (int i = 0; i < measure->m_imageBoardList.size(); i++) ///图片列表
				{
					measure->m_imageBoardList[i]->SetPosition(pntInPlane);
				}
				state = true;
			}
			else
			{
				delete measure;
				measure = NULL;
			}
		}

		return state;
	}

	Measure* MeasureFactory::CreateTmpDiametreMeasure(int shapeID, int type, SceneManager* scene)
	{
		Measure* measure = NULL;

		//判断场景是否为空
		if (scene == NULL)
		{
			return measure;
		}

		//填充meaureNote,根据不同的测量类型创建不同的Note对象
		if (type == Measure::MEASURE_TYPE_CRICLE_DIAMETRE) ///直径
		{
			measure = createDesignerDiametre(shapeID, scene);
		}
		else if (type == Measure::MEASURE_TYPE_CRICLE_RADIUS)
		{
			measure = createDesignerRadius(shapeID, scene);
		}
		
		AddMeasureToScene(scene, measure);

		if (measure)
		{
			measure->SetFrontShow(true);
		}
		return measure;
	}

	Measure* MeasureFactory::createDesignerDiametre(int lineId, SceneManager* scene)
	{
		Measure* measure = NULL;

		IShape* lineShape = scene->GetShape(lineId);

		if (lineShape && lineShape->GetType() == SHAPE_EDGE)
		{
			measure = new MeasureDiameter;
			measure->SetMeasureType(Measure::MEASURE_TYPE_CRICLE_DIAMETRE);

			Vector3 firstVector;
			Vector3 firstCenter;
			float firstRadius;
			if (!CreateCircularArc(measure, lineShape, firstVector, firstCenter, firstRadius))
			{
				delete measure;
				measure = NULL;
				return measure;
			}
		}
		return measure;
	}

	Measure* MeasureFactory::createDesignerRadius(int lineId, SceneManager* scene)
	{
		Measure* measure = NULL;
		IShape* lineShape = scene->GetShape(lineId);
		if (lineShape && lineShape->GetType() == SHAPE_EDGE)
		{
			measure = new MeasureDiameter;
			measure->SetMeasureType(Measure::MEASURE_TYPE_CRICLE_RADIUS);

			Vector3 firstVector;
			Vector3 firstCenter;
			float firstRadius;
			if (!CreateCircularArc(measure, lineShape, firstVector, firstCenter, firstRadius))
			{
				delete measure;
				measure = NULL;
				return measure;
			}
		}
		return measure;
	}

	bool MeasureFactory::CreateCircularArc(Measure* measure, IShape* shape, Vector3& verticVector, Vector3& center, float& radius)
	{
		bool state = false;
		if (!measure)
		{
			return state;
		}
		if (shape && shape->GetType() == SHAPE_EDGE)
		{
			Edge * edge = (Edge*)shape;
			RefPolyLine * lineSet = edge->GetLineData();
			GeometryAttribute * lineGeo = edge->GetGeoAttribute();
			if (lineGeo && lineGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_ELLIPSE)
			{
				EllipseAttribute lineAttribute = *((EllipseAttribute *)lineGeo);
				Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(edge);
				GeometryHelper::Transform(lineAttribute, worldMatrix1);

				center = lineAttribute.GetCenterPoint();
				float majorRadius = lineAttribute.GetMajorRadius();
				float minorRadius = lineAttribute.GetMinorRadius();
				radius = majorRadius;
				if (majorRadius != minorRadius)
				{
					return state;
				}

				Vector3 start = lineAttribute.GetStartPoint();
				Vector3 end = lineAttribute.GetEndPoint();
				Vector3 startDirection = (start - center).Normalized();
				Vector3 endDirection = (end - center).Normalized();
				verticVector = startDirection.CrossProduct(endDirection).Normalized();

				Point * centerPoint1 = new Point(center);
				centerPoint1->SetDrawType(1);
				centerPoint1->SetSize(0.8f);
				measure->AddPoint(centerPoint1);

				SPolyLine * xPolyLine = lineSet->GetRefLine();
				vector<Vector3> & pntPoints = xPolyLine->GetPoints();
				unsigned int offset = lineSet->GetDataOffset();
				unsigned int dataLength = lineSet->GetDataLength();

				vector<Vector3> linesTemp;
				vector<Vector3> dottedLinesTemp;
				float angle = startDirection.Angle(endDirection);
				if (abs(angle - 180) < 0.1)
				{
					dataLength = dataLength / 2;
					Vector3 tmpPnt = worldMatrix1 * pntPoints[offset + dataLength];
					Vector3 direction = (center - tmpPnt).Normalized();
					Vector3 oppositePnt = tmpPnt + direction * majorRadius * 2;
					verticVector = startDirection.CrossProduct(direction).Normalized();
					angle = (start - center).Angle(tmpPnt - center);
					MeasureDisplayHelper::CreateDesignerAngleMark(center, start, center, tmpPnt,
						center, majorRadius, angle, linesTemp);
					MeasureDisplayHelper::CreateDesignerAngleMark(center, tmpPnt, center, end,
						center, majorRadius, 180 - angle, linesTemp);

					MeasureDisplayHelper::CreateDesignerAngleMark(center, start, center, oppositePnt,
						center, majorRadius, angle, dottedLinesTemp);
					MeasureDisplayHelper::CreateDesignerAngleMark(center, oppositePnt, center, end,
						center, majorRadius, 180 - angle, dottedLinesTemp);
				}
				else if (abs(angle) < 0.001)
				{
					return state;
					MeasureDisplayHelper::CreateDesignerAngleMark(center, start, center, end,
						center, majorRadius, 360, linesTemp);
				}
				else
				{
					if (dataLength < 3)
					{
						MeasureDisplayHelper::CreateDesignerAngleMark(center, start, center, end,
							center, majorRadius, angle, linesTemp);
						MeasureDisplayHelper::CreateDesignerAngleMark(center, start, center, end,
							center, majorRadius, angle - 360, dottedLinesTemp);
					}
					else
					{
						dataLength = dataLength / 2;
						Vector3 tmpPnt = worldMatrix1 * pntPoints[offset + dataLength];
						float angle1 = (start - center).Angle(tmpPnt - center);
						float angle2 = (end - center).Angle(tmpPnt - center);
						if (angle1 < angle && angle2 < angle)
						{
							MeasureDisplayHelper::CreateDesignerAngleMark(center, start, center, end,
								center, majorRadius, angle, linesTemp);
							MeasureDisplayHelper::CreateDesignerAngleMark(center, start, center, end,
								center, majorRadius, angle - 360, dottedLinesTemp);
						}
						else
						{
							MeasureDisplayHelper::CreateDesignerAngleMark(center, start, center, end,
								center, majorRadius, angle - 360, linesTemp);
							MeasureDisplayHelper::CreateDesignerAngleMark(center, start, center, end,
								center, majorRadius, angle, dottedLinesTemp);
						}
					}
				}

				Color lineColor = Color::RED;
				if (linesTemp.size() > 0)
				{
					for (int i = 0; i < linesTemp.size() - 1; i++)
					{
						Line3D * lineTemp = new Line3D(linesTemp[i],
							linesTemp[i + 1]);
						lineTemp->SetColor(lineColor);
						measure->AddLine(lineTemp);
					}
				}
				if (dottedLinesTemp.size() > 0)
				{
					for (int i = 0; i < dottedLinesTemp.size() - 1; i++)
					{
						Line3D * lineTemp = new Line3D(dottedLinesTemp[i],
							dottedLinesTemp[i + 1]);
						lineTemp->SetColor(lineColor);
						lineTemp->SetName("dottedLine");
						measure->AddLine(lineTemp);
					}
				}

				Vector3 pnt1 = center + startDirection * majorRadius / 3;
				Vector3 pnt2 = center - startDirection * majorRadius / 3;
				Vector3 otherDirection = startDirection.CrossProduct(verticVector).Normalized();
				Vector3 pnt3 = center + otherDirection * majorRadius / 3;
				Vector3 pnt4 = center - otherDirection * majorRadius / 3;

				Line3D * line1 = new Line3D(pnt1, pnt2);
				line1->SetColor(lineColor);
				Line3D * line2 = new Line3D(pnt3, pnt4);
				line2->SetColor(lineColor);

				measure->AddLine(line1);
				measure->AddLine(line2);
				state = true;
			}
		}
		return state;
	}

	bool MeasureFactory::CompleteDiametreMeasure(Measure* measure, int shapeID, Vector2 screenPnt, SceneManager* scene)
	{
		bool state = false;
		if (!measure || !scene)
		{
			return state;
		}

		int type = measure->GetMeasureType();
		if (type == Measure::MEASURE_TYPE_CRICLE_DIAMETRE)
		{
			state = completeDesignerDiametre(measure, shapeID, screenPnt, scene);
		}
		else if (type == Measure::MEASURE_TYPE_CRICLE_RADIUS)
		{
			state = completeDesignerRadius(measure, shapeID, screenPnt, scene);
		}
		
		return state;
	}

	bool MeasureFactory::completeDesignerDiametre(Measure* measure, int shapeID, Vector2& screenPnt, SceneManager* scene)
	{
		bool state = false;
		if (!measure || !scene)
		{
			return state;
		}
		MeasureDiameter* measureDiameter = dynamic_cast<MeasureDiameter*>(measure);
		if (!measureDiameter)
		{
			return state;
		}

		IShape* shape = scene->GetShape(shapeID);
		if (shape && shape->GetType() == SHAPE_EDGE)
		{
			Edge * edge = (Edge*)shape;
			RefPolyLine * lineSet = edge->GetLineData();
			GeometryAttribute * lineGeo = edge->GetGeoAttribute();
			if (lineGeo && lineGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_ELLIPSE)
			{
				EllipseAttribute lineAttribute = *((EllipseAttribute *)lineGeo);
				Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(edge);
				GeometryHelper::Transform(lineAttribute, worldMatrix1);

				DeleteMeasureLine(measure);

				float radius = lineAttribute.GetMajorRadius();
				Vector3 center = lineAttribute.GetCenterPoint();
				Vector3 start = lineAttribute.GetStartPoint();
				Vector3 end = lineAttribute.GetEndPoint();
				Vector3 startDirection = start - center;
				Vector3 endDirection = end - center;

				Vector3 normal;
				float angle = startDirection.Angle(endDirection);
				if (abs(angle - 180) < 0.1)
				{
					SPolyLine * xPolyLine = lineSet->GetRefLine();
					vector<Vector3> & pntPoints = xPolyLine->GetPoints();
					unsigned int offset = lineSet->GetDataOffset();
					unsigned int dataLength = lineSet->GetDataLength();

					dataLength = dataLength / 2;
					Vector3 tmpPnt = worldMatrix1 * pntPoints[offset + dataLength];
					normal = startDirection.CrossProduct(tmpPnt - center);
				}
				else
				{
					normal = startDirection.CrossProduct(endDirection);
				}
				Plane proPlane(normal, start);
				Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

				Vector3 pntInPlane;
				proPlane.GetInsectPnt(cameraRay, pntInPlane);

				Vector3 lineDirection = (pntInPlane - center).Normalized();
				Vector3 pnt1 = center - lineDirection*radius;
				Vector3 pnt2 = center + lineDirection*radius;
				Color lineColor = Color::RED;

				if ((pntInPlane - center).Length() > radius)
				{
					pnt2 = pntInPlane;
				}
				Line3D* line = new Line3D(pnt1, pnt2);
				line->SetColor(lineColor);
				line->SetCanDelete(true);
				measure->AddLine(line);

				for (int i = 0; i < measure->m_imageBoardList.size(); i++) ///图片列表
				{
					delete measure->m_imageBoardList[i];
				}
				measure->m_imageBoardList.resize(0);


				vector<float> valuse;
				valuse.push_back(radius*2);
				string unit;
				MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
					, valuse, unit);
				string lengthstr = M3DTOOLS::floatToString(valuse.at(0)) + unit;

				//解决协同中文本读取
				ComText* ct = new ComText();
				CText* t = new CText;
				t->SetText(lengthstr);
				ct->AddCText(t);
				measure->m_ComTexts.push_back(ct);

				vector<Texts2D*> temptext;
				Texts2D *title = new Texts2D;
				title->m_size = 12.0f;
				title->m_texts = "内容";
				temptext.push_back(title);

				Texts2D *content = new Texts2D;
				content->m_size = 12.0f;
				content->m_texts = lengthstr;
				temptext.push_back(content);

				ImageBoard * imageBroad = MeasureDisplayHelper::createNoteTextsImageN(scene, temptext, pntInPlane);
				measure->AddImage(imageBroad);

				state = true;
			}
			else
			{
				delete measure;
				measure = NULL;
			}
		}

		return state;
	}

	bool MeasureFactory::completeDesignerRadius(Measure* measure, int shapeID, Vector2& screenPnt, SceneManager* scene)
	{
		bool state = false;
		if (!measure || !scene)
		{
			return state;
		}
		MeasureDiameter* measureDiameter = dynamic_cast<MeasureDiameter*>(measure);
		if (!measureDiameter)
		{
			return state;
		}

		IShape* shape = scene->GetShape(shapeID);
		if (shape && shape->GetType() == SHAPE_EDGE)
		{
			Edge * edge = (Edge*)shape;
			RefPolyLine * lineSet = edge->GetLineData();
			GeometryAttribute * lineGeo = edge->GetGeoAttribute();
			if (lineGeo && lineGeo->GetGeoAttrType() == M3D_GEOATTR_TYPE_ELLIPSE)
			{
				EllipseAttribute lineAttribute = *((EllipseAttribute *)lineGeo);
				Matrix3x4 worldMatrix1 = ShapeHelper::GetShapeWorldMatrix(edge);
				GeometryHelper::Transform(lineAttribute, worldMatrix1);

				DeleteMeasureLine(measure);

				float radius = lineAttribute.GetMajorRadius();
				Vector3 center = lineAttribute.GetCenterPoint();
				Vector3 start = lineAttribute.GetStartPoint();
				Vector3 end = lineAttribute.GetEndPoint();
				Vector3 startDirection = start - center;
				Vector3 endDirection = end - center;

				Vector3 normal;
				float angle = startDirection.Angle(endDirection);
				if (abs(angle - 180) < 0.1)
				{
					SPolyLine * xPolyLine = lineSet->GetRefLine();
					vector<Vector3> & pntPoints = xPolyLine->GetPoints();
					unsigned int offset = lineSet->GetDataOffset();
					unsigned int dataLength = lineSet->GetDataLength();

					dataLength = dataLength / 2;
					Vector3 tmpPnt = worldMatrix1 * pntPoints[offset + dataLength];
					normal = startDirection.CrossProduct(tmpPnt - center);
				}
				else
				{
					normal = startDirection.CrossProduct(endDirection);
				}

				Plane proPlane(normal, start);
				Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
					screenPnt.m_x, screenPnt.m_y);

				Vector3 pntInPlane;
				proPlane.GetInsectPnt(cameraRay, pntInPlane);

				Vector3 lineDirection = (pntInPlane - center).Normalized();
				Vector3 pnt1 = center + lineDirection*radius;
				Vector3 pnt2 = center;
				if ((pntInPlane - center).Length() > radius)
				{
					pnt2 = pntInPlane;
				}
				Color lineColor = Color::RED;
				Line3D* line = new Line3D(pnt1, pnt2);
				line->SetColor(lineColor);
				line->SetCanDelete(true);
				measure->AddLine(line);

				for (int i = 0; i < measure->m_imageBoardList.size(); i++) ///图片列表
				{
					delete measure->m_imageBoardList[i];
				}
				measure->m_imageBoardList.resize(0);


				vector<float> valuse;
				valuse.push_back(radius);
				string unit;
				MeasureDisplayHelper::SetMeasureUnit(SVIEW::Parameters::Instance()->m_measureUnitStyle
					, valuse, unit);
				string lengthstr = "R"+M3DTOOLS::floatToString(valuse.at(0)) + unit;

				//解决协同中文本读取
				ComText* ct = new ComText();
				CText* t = new CText;
				t->SetText(lengthstr);
				ct->AddCText(t);
				measure->m_ComTexts.push_back(ct);

				vector<Texts2D*> temptext;
				Texts2D *title = new Texts2D;
				title->m_size = 12.0f;
				title->m_texts = "内容";
				temptext.push_back(title);

				Texts2D *content = new Texts2D;
				content->m_size = 12.0f;
				content->m_texts = lengthstr;
				temptext.push_back(content);

				ImageBoard * imageBroad = MeasureDisplayHelper::createNoteTextsImageN(scene, temptext, pntInPlane);
				measure->AddImage(imageBroad);

				state = true;
			}
			else
			{
				delete measure;
				measure = NULL;
			}
		}

		return state;
	}

	Measure* MeasureFactory::CreateSingleMeasure(int type, SceneManager* scene)
	{
		Measure* measure = NULL;
		//判断场景是否为空
		if (scene == NULL)
		{
			return measure;
		}

		if (type == Measure::MEASURE_TYPE_PNT_PNT_DISTANCE ||
			type == Measure::MEASURE_TYPE_PNT_LINE_DISTANCE ||
			type == Measure::MEASURE_TYPE_LINE_LINE_DISTANCE ||
			type == Measure::MEASURE_TYPE_LINE_FACE_DISTANCE ||
			type == Measure::MEASURE_TYPE_LINE_LINE_DISTANCE ||
			type == Measure::MEASURE_TYPE_FACE_FACE_DISTANCE ||
			type == Measure::MEASURE_TYPE_SHAFT_SHAFT_DISTANCE ||
			type == Measure::MEASURE_TYPE_CENTER_CENTER_DISTANCE ||
			type == Measure::MEASURE_TYPE_LINE_LENGTH ||
			type == Measure::MEASURE_TYPE_CRICLE_PROPERTY)
		{
			measure = new MeasureDistance;
			measure->SetMeasureType(type);
		}
		else if (type == Measure::MEASURE_TYPE_LINE_LINE_ANGLE ||
			type == Measure::MEASURE_TYPE_FACE_FACE_ANGLE ||
			type == Measure::MEASURE_TYPE_LINE_FACE_ANGLE)
		{
			measure = new MeasureAngle;
			measure->SetMeasureType(type);
		}
		else if (type == Measure::MEASURE_TYPE_CRICLE_DIAMETRE ||
			type == Measure::MEASURE_TYPE_CRICLE_RADIUS)
		{
			measure = new MeasureDiameter;
			measure->SetMeasureType(type);
		}
		
		AddMeasureToScene(scene, measure);

		if (measure)
		{
			measure->SetFrontShow(true);
		}

		return measure;
	}

	bool MeasureFactory::CreateMeasureLine(Measure* measure, string& name, Vector3& start, Vector3& end, Color& color)
	{
		if (!measure)
		{
			return false;
		}
		Line3D* line = new Line3D(start, end);
		line->SetColor(color);
		line->SetName(name);
		measure->AddLine(line);
		return true;
	}
	bool MeasureFactory::CreateMeasurePoint(Measure* measure, int type, float size, Vector3& pnt)
	{
		if (!measure)
		{
			return false;
		}
		Point * point = new Point(pnt);
		point->SetDrawType(type);
		point->SetSize(size);
		measure->AddPoint(point);
		return true;
	}

	bool MeasureFactory::CreateMeasureImageBoard(Measure* measure, string& text, Vector3& position,SceneManager* scene)
	{
		if (!measure)
		{
			return false;
		}

		//解决协同中文本读取
		ComText* ct = new ComText();
		CText* t = new CText;
		t->SetText(text);
		ct->AddCText(t);
		measure->m_ComTexts.push_back(ct);

		vector<Texts2D*> temptext;
		Texts2D *title = new Texts2D;
		title->m_size = 12.0f;
		title->m_texts = "内容";
		temptext.push_back(title);

		Texts2D *content = new Texts2D;
		content->m_size = 12.0f;
		content->m_texts = text;
		temptext.push_back(content);

		ImageBoard * imageBroad = MeasureDisplayHelper::createNoteTextsImageN(scene, temptext, position);
		measure->AddImage(imageBroad);
		return true;
	}
}


