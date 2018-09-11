#include "m3d/extras/note/NoteFactory.h"

#include "m3d/sceneManager.h"
#include "m3d/RenderManager.h"
#include "m3d/ResourceManager.h"
#include "m3d/scene/shapeNode.h"

#include "m3d/base/ray.h"
#include "m3d/base/color.h"
#include "m3d/model/line3d.h"
#include "m3d/model/point.h"

#include "m3d/extras/note/TextNote.h"
#include "m3d/extras/note/VoiceNote.h"
#include "m3d/extras/note/SequenceNumberNote.h"
#include "m3d/extras/note/ThreeDGesturesNote.h"

#include "m3d/SceneManager.h"
#include "m3d/graphics/cameranode.h"
#include "m3d/graphics/Texture.h"
#include "m3d/base/Plane.h"
#include "m3d/utils/m3dtools.h"
#include "m3d/model/Edge.h"
#include "m3d/model/curve.h"
#include "m3d/model/GeoAttribute.h"
#include "m3d/base/Matrix3x4.h"

#include "m3d/utils/ShapeHelper.h"
#include "m3d/utils/StringHelper.h"
#include "m3d/handler/HandlerPoint.h"
#include "m3d/extras/note/NoteGroup.h"

#include "tinyxml/tinyxml2.h"
#include "m3d/extras/measure/tools/MeasureDisplayHelper.h"
#include  <ctime>
#include <cctype>
#include "m3d/graphics/ImageBoard.h"

#include "m3d/action/CallBackAction.h"
#include "m3d/base/BoundingBox.h"
#include "m3d/model/Model.h"
#include "m3d/model/Body.h"
#include "m3d/model/Face.h"
#include "m3d/model/MeshData.h"
#include "m3d/extras/modelmanager/ModelAssemblyHelper.h"
#include "m3d/action/RayPickAction.h"
#include "m3d/base/Ray.h"
#include "sview/views/Parameters.h"
using namespace tinyxml2;

namespace M3D
{

const string NoteFactory::Serializer_XML_Element_SVL = "SVL";

const string NoteFactory::Serializer_XML_Element_Header = "Header";
const string NoteFactory::Serializer_XML_Element_Version = "Version";
const string NoteFactory::Serializer_XML_Element_Author = "Author";
const string NoteFactory::Serializer_XML_Element_Created = "Created";
const string NoteFactory::Serializer_XML_Element_Generator = "Generator";

const string NoteFactory::Serializer_XML_Element_Model = "Model";
const string NoteFactory::Serializer_XML_Element_Users = "Users";
const string NoteFactory::Serializer_XML_Element_User = "User";
const string NoteFactory::Serializer_XML_Element_Notes = "Notes";
const string NoteFactory::Serializer_XML_Element_TextNote = "TextNote";
const string NoteFactory::Serializer_XML_Element_VoiceNote = "VoiceNote";
const string NoteFactory::Serializer_XML_Element_SequenceNote = "SequenceNote";
const string NoteFactory::Serializer_XML_Element_ThreeDGestureNote = "ThreeDGestureNote";
const string NoteFactory::Serializer_XML_Element_Color = "Color";

const string NoteFactory::Serializer_XML_Element_VoiceData = "VoiceData";
const string NoteFactory::Serializer_XML_Element_ThreeDGestureNoteLine = "ThreeDGestureNoteLine";

const string NoteFactory::Serializer_XML_Element_Leaders = "Leaders";
const string NoteFactory::Serializer_XML_Element_Leader = "Leader";
const string NoteFactory::Serializer_XML_Element_Terminator = "Terminator";
const string NoteFactory::Serializer_XML_Element_Location = "Location";
const string NoteFactory::Serializer_XML_Element_Direction = "Direction";
const string NoteFactory::Serializer_XML_Element_Polyline = "Polyline";
const string NoteFactory::Serializer_XML_Element_Point = "Point";
const string NoteFactory::Serializer_XML_Element_ExtendLines = "ExtendLines";

const string NoteFactory::Serializer_XML_Element_CompositeTexts = "CompositeTexts";
const string NoteFactory::Serializer_XML_Element_CompositeText = "CompositeText";
const string NoteFactory::Serializer_XML_Element_Texts = "Texts";
const string NoteFactory::Serializer_XML_Element_Text = "Text";
const string NoteFactory::Serializer_XML_Element_TextStyle = "TextStyle";
const string NoteFactory::Serializer_XML_Element_CharHeight = "CharHeight";
const string NoteFactory::Serializer_XML_Element_LineSpacing = "LineSpacing";
const string NoteFactory::Serializer_XML_Element_Font = "Font";
const string NoteFactory::Serializer_XML_Element_UsageType = "UsageType";

// public final static String Serializer_XML_Element_Location = "Location";
const string NoteFactory::Serializer_XML_Element_Rotation = "Rotation";
const string NoteFactory::Serializer_XML_Element_OuterBox = "OuterBox";
const string NoteFactory::Serializer_XML_Element_ProjectMatrix = "ProjectMatrix";
const string NoteFactory::Serializer_XML_Element_Attributes = "Attributes";
const string NoteFactory::Serializer_XML_Element_Attribute = "Attribute";

const string NoteFactory::Serializer_XML_Attribute_ID = "ID";
const string NoteFactory::Serializer_XML_Attribute_SpacePoint = "SpacePoint";
const string NoteFactory::Serializer_XML_Attribute_UserID = "UserID";
const string NoteFactory::Serializer_XML_Attribute_Created = "Created";
const string NoteFactory::Serializer_XML_Attribute_IsParallelScreen = "IsParallelScreen";
const string NoteFactory::Serializer_XML_Attribute_IsFix = "IsFix";
const string NoteFactory::Serializer_XML_Attribute_Visible = "Visible";
const string NoteFactory::Serializer_XML_Attribute_R = "R";
const string NoteFactory::Serializer_XML_Attribute_G = "G";
const string NoteFactory::Serializer_XML_Attribute_B = "B";
const string NoteFactory::Serializer_XML_Attribute_Color = "Color";
const string NoteFactory::Serializer_XML_Attribute_X = "X";
const string NoteFactory::Serializer_XML_Attribute_Y = "Y";
const string NoteFactory::Serializer_XML_Attribute_Z = "Z";
const string NoteFactory::Serializer_XML_Attribute_Value = "Value";
const string NoteFactory::Serializer_XML_Attribute_Type = "Type";
const string NoteFactory::Serializer_XML_Attribute_Key = "Key";
const string NoteFactory::Serializer_XML_Attribute_URI = "Uri";
const string NoteFactory::Serializer_XML_Attribute_Width = "Width";
const string NoteFactory::Serializer_XML_Attribute_Height = "Height";
const string NoteFactory::Serializer_XML_Attribute_Name = "Name";
const string NoteFactory::Serializer_XML_Attribute_Min = "Min";
const string NoteFactory::Serializer_XML_Attribute_Max = "Max";

const string NoteFactory::Serializer_XML_Attribute_Format = "Format";

NoteFactory::NoteFactory(void)
{

}

NoteFactory::~NoteFactory()
{

}

void NoteFactory::GetMeshSet(BoundingBox& box, vector<Vector3>&triangleSet, SceneManager* scene)
{
	InsectMeshSetInfo insectMeshSetInfo ;
	insectMeshSetInfo.box =&box;
	insectMeshSetInfo.meshSet = &triangleSet;
	CallBackAction * getAllInsectMeshAction = new CallBackAction();
	getAllInsectMeshAction->SetActionData(&insectMeshSetInfo);
	getAllInsectMeshAction->SetActionFun(GetInsectShape);
	scene->ExecuteAction(getAllInsectMeshAction);
	delete getAllInsectMeshAction;

}
void NoteFactory::GetInsectShape(void* data, Model* node)
{
	if (node != NULL && node->GetType() == SHAPE_NODE)
	{
		InsectMeshSetInfo * insectMeshSetInfo = (InsectMeshSetInfo *)data;
		BoundingBox& shapeNodeBox = node->GetWorldBoundingBox();
		if (insectMeshSetInfo->box->IsInside(shapeNodeBox) != OUTSIDE)
		{
			ShapeNode *shapeNode = (ShapeNode *)node;
			Model * model = (Model *)shapeNode->GetShape();
			GetInsectModel(insectMeshSetInfo, model);
		}

	}
}
void NoteFactory::GetInsectModel(void * data, Model * model)
{
	if (model)
	{
		InsectMeshSetInfo * insectMeshSetInfo = (InsectMeshSetInfo *)data;
		BoundingBox &box = model->GetBoundingBox();
		BoundingBox lineBox = *(insectMeshSetInfo->box);
		vector<Body*>*bodies = model->GetBodys();
		Matrix3x4 mat = ModelAssemblyHelper::GetWorldMatrix(model); //TODO
		lineBox.Transform(mat.Inverse());
		int bodiesSize = bodies->size();
		if (lineBox.IsInside(box) != OUTSIDE)
		{
			vector<Vector3> lineDatas;
			for (int i = 0; i < bodiesSize; i++)
			{
				GetInsectBody(lineDatas, (Body*)bodies->at(i),lineBox);
			}
			int pointSize = lineDatas.size();
			for (int i = 0; i < pointSize; i++)
			{
				Vector4 temp(lineDatas.at(i), 1.0);

				lineDatas.at(i) = (mat * temp); //转换到世界坐标系
			}
			insectMeshSetInfo->meshSet->insert(insectMeshSetInfo->meshSet->end(), lineDatas.begin(), lineDatas.end());
		}
	}
}
void NoteFactory::GetInsectBody(vector<Vector3>& lineDatas, Body * body,BoundingBox& lineBox)
{
	if (body)
	{
//		BoundingBox & box = body->GetBoundingBox();
		vector<Face*>&faces = body->GetFaces();

		int facesSize = faces.size();
//		if (insectMeshSetInfo->box->IsInside(box) != OUTSIDE)
		{
			for (int i = 0; i < facesSize; i++)
			{
				GetInsectFace(lineDatas, (Face*)faces[i], lineBox);
			}
		}
	}

}
void NoteFactory::GetInsectFace(vector<Vector3>& lineDatas, Face * face, BoundingBox& lineBox)
{
	if (face)
	{
		//BoundingBox & box = face->GetBoundingBox();
		Mesh * mesh = (Mesh*)face->GetData();
	//	if (insectMeshSetInfo->box->IsInside(box) != OUTSIDE)
		{
			GetInsectMesh(lineDatas, mesh, lineBox);
		}
	}
}
void NoteFactory::GetInsectMesh(vector<Vector3>& lineDatas, Mesh * mesh, BoundingBox& lineBox)
{
	if (mesh)
	{
		VertexSet * pSet = mesh->GetRefMesh();
		unsigned int dataOffset = mesh->GetDataOffset();
		unsigned int dataLength = mesh->GetDataLength();
		vector<Vector3>* positionArray = pSet->GetPositionArray();
		if (pSet->IsUseIndex())
		{
			vector<M3D_INDEX_TYPE>* indexArray = pSet->GetIndexArray();
			for (vector<M3D_INDEX_TYPE>::iterator it = indexArray->begin() + dataOffset;
				it != indexArray->begin() + dataOffset + dataLength; it = it + 3)
			{
				Vector3 points[4];
				points[0] = positionArray->at(*it);
				points[1] = positionArray->at(*(it + 1));
				points[2] = positionArray->at(*(it + 2));
				points[3] = positionArray->at(*it);
				for (int i = 0; i < 3; i++)
				{
					if (lineBox.IsInside(points[i], points[i + 1]) != OUTSIDE)
					{
						lineDatas.push_back(points[0]);
						lineDatas.push_back(points[1]);
						lineDatas.push_back(points[2]);
						break;
					}
				}
			}
		}
		else
		{
			for (vector<Vector3>::iterator it = positionArray->begin() + dataOffset;
				it != positionArray->begin() + dataOffset + dataLength; it = it + 3)
			{
				Vector3 points[4];
				points[0] = (*it);
				points[1] = (*(it + 1));
				points[2] = (*(it + 2));
				points[3] = (*it);
				for (int i = 0; i < 3; i++)
				{
					if (lineBox.IsInside(points[i], points[i + 1]) != OUTSIDE)
					{
						lineDatas.push_back(points[0]);
						lineDatas.push_back(points[1]);
						lineDatas.push_back(points[2]);
						break;
					}
				}
			}
		}
	}
}

string NoteFactory::ThreeDGestureNoteToXMLElement(SceneManager* scene,ThreeDGesturesNote* threeDGesturesNote)
{
	//序列化
	string xmlElement = "";

	XMLDocument pDoc; //创建文档
	//添加声明
	//XMLDeclaration * pDeclaration = pDoc->NewDeclaration("xml version=\"1.0\" encoding=\"utf-8\" standalone=\"no\"");
	//pDoc->LinkEndChild(pDeclaration);

	//创建根节点

	XMLElement *pThreeDGestureNote = pDoc.NewElement(Serializer_XML_Element_ThreeDGestureNote.c_str());
	pDoc.LinkEndChild(pThreeDGestureNote);
	pThreeDGestureNote->SetAttribute(Serializer_XML_Attribute_ID.c_str(), (int)threeDGesturesNote->GetID());

    pThreeDGestureNote->SetAttribute(Serializer_XML_Attribute_SpacePoint.c_str(),
                                     threeDGesturesNote->GetIsShowSpacePoint());
	{
		vector<Line3D*>& line3DSet = threeDGesturesNote->m_LineList;
		//int line3DSetSize = line3DSet.size();
//		vector<int>& line3DIndex = threeDGesturesNote->GetLine3DIndex();
//		int line3DSetIndexSize = line3DIndex.size();
		if (line3DSet.size() == 0)
		{
			return "";
		}
		int allSize = threeDGesturesNote->GetOriginalProjectPns().size();
		for (int i = 0; i < allSize; i = i + 1)
		{
			Color& originalProjectColor =
				threeDGesturesNote->GetOriginalProjectColors().at(i);
			int originalProjectWidth =
				threeDGesturesNote->GetOriginalProjectWidths().at(i);
			XMLElement * p3DGestureNoteLine = pDoc.NewElement(Serializer_XML_Element_ThreeDGestureNoteLine.c_str());
			pThreeDGestureNote->LinkEndChild(p3DGestureNoteLine);
			/*p3DGestureNoteLine->SetAttribute(Serializer_XML_Attribute_Width.c_str(), line3DSet[i]->GetLineWidth());
			string colorStr = line3DSet[i]->GetColor()->Tostring();*/
			p3DGestureNoteLine->SetAttribute(
				Serializer_XML_Attribute_Width.c_str(),
				originalProjectWidth);
			string colorStr = originalProjectColor.Tostring();
			p3DGestureNoteLine->SetAttribute(Serializer_XML_Attribute_Color.c_str(),colorStr.c_str() );
			string allPoisitionStr = "";
			vector<Vector3>& originalProjectPnts = threeDGesturesNote->GetOriginalProjectPns().at(i);
			for (int j = 0; j < originalProjectPnts.size(); j++)
			{
				string pointStr;
				pointStr =originalProjectPnts.at(j).Tostring();

				allPoisitionStr = allPoisitionStr+pointStr+" ";
			}

			XMLText * pPositionText = pDoc.NewText(allPoisitionStr.c_str());
			p3DGestureNoteLine->LinkEndChild(pPositionText);
		}
	}

	{
		XMLElement * pAttributes = pDoc.NewElement(Serializer_XML_Element_Attributes.c_str());
		pThreeDGestureNote->LinkEndChild(pAttributes);
		for (map<string, string>::iterator it = threeDGesturesNote->GetPropertyMap().begin();
				it != threeDGesturesNote->GetPropertyMap().end(); it++)
		{
			XMLElement * pAttribute = pDoc.NewElement(Serializer_XML_Element_Attribute.c_str());
			pAttributes->LinkEndChild(pAttribute);
			pAttribute->SetAttribute(Serializer_XML_Attribute_Key.c_str(), it->first.c_str());
			pAttribute->SetAttribute(Serializer_XML_Attribute_Value.c_str(), it->second.c_str());
		}

	}
	tinyxml2::XMLPrinter priter;
	pDoc.Accept(&priter);
	xmlElement = priter.CStr();
	return xmlElement;
}

Note* NoteFactory::CreateThreeDGestureNoteFromXMLElement(SceneManager* scene,const string& xmlElement)
{
	ThreeDGesturesNote * threeDGesturesNote = NULL;
	if (xmlElement.length() == 0)
	{
		return NULL;
	}
	threeDGesturesNote = new ThreeDGesturesNote;
	const char* xml = xmlElement.c_str();
	XMLDocument doc;
	doc.Parse(xml);

	XMLElement* pThreeDGestureNote = doc.FirstChildElement(Serializer_XML_Element_ThreeDGestureNote.c_str());
	if (pThreeDGestureNote)
	{
		int id = pThreeDGestureNote->IntAttribute(Serializer_XML_Attribute_ID.c_str());
		threeDGesturesNote->SetID(id);
        bool isShowSpacePoint = pThreeDGestureNote->BoolAttribute(Serializer_XML_Attribute_SpacePoint.c_str());
        threeDGesturesNote->SetIsShowSpacePoint(isShowSpacePoint);
        SVIEW::Parameters::Instance()->m_isShowSpacePoint = isShowSpacePoint;
	}

	XMLElement * p3DGestureNoteLine = pThreeDGestureNote->FirstChildElement(Serializer_XML_Element_ThreeDGestureNoteLine.c_str());

	while (p3DGestureNoteLine)
	{
		float width = p3DGestureNoteLine->FloatAttribute(Serializer_XML_Attribute_Width.c_str());
		threeDGesturesNote->SetOriginalProjectWidths(width);
		string colorStr = p3DGestureNoteLine->Attribute(Serializer_XML_Attribute_Color.c_str());
		vector<string> colorStrSet = StringHelper::Split(colorStr, " ");
		Color lineColor;
		if (colorStrSet.size() == 4)
		{
			float r = StringHelper::StringToFloat(colorStrSet[0]);
			float g = StringHelper::StringToFloat(colorStrSet[1]);
			float b = StringHelper::StringToFloat(colorStrSet[2]);
			float a = StringHelper::StringToFloat(colorStrSet[3]);
			lineColor.SetColor(r,g,b,a);
		}
		threeDGesturesNote->SetOriginalProjectColors(lineColor);
		const char * positionData = p3DGestureNoteLine->GetText();
		string positionStr;
		if (positionData != NULL)
		{
			positionStr.assign(positionData);
		}
		vector<string> positionStrSet = StringHelper::Split(positionStr, " ");
		int positionSize = positionStrSet.size();
		vector<Vector3> originPosition;
		vector<Vector3> originScreenPnts;
		for(int i=0;i<positionSize-2;i = i+3)
		{
			float sX = StringHelper::StringToFloat(positionStrSet[i]);
			float sY = StringHelper::StringToFloat(positionStrSet[i+1]);
			float sZ = StringHelper::StringToFloat(positionStrSet[i+2]);
			Vector3 startPoint(sX,sY,sZ);
			IntVector2 screenPnt = scene->GetCamera()->GetViewPort().WorldToScreenPoint(startPoint);
			Vector3 tempVec3(screenPnt.m_x,screenPnt.m_y,0.0f);
			originScreenPnts.push_back(tempVec3);
			originPosition.push_back(startPoint);
		}
		threeDGesturesNote->GetOriginalProjectPns().push_back(originPosition);//将投影点加入note
		vector<vector<Vector3> > finalPoints;
		InsectPoint(scene, originScreenPnts, originPosition, finalPoints);
		for (int i = 0; i < finalPoints.size(); i++)
		{
			for (int j = 0; j < finalPoints[i].size() - 1; j++)
			{
				Line3D * line = new Line3D(finalPoints[i][j], finalPoints[i][j + 1]);
				line->SetColor(lineColor);
				line->SetLineWidth(width);
				threeDGesturesNote->AddLine(line);
			}
		}

//		vector<int>& line3dindex = threeDGesturesNote->GetLine3DIndex();
//		int line3dindexSize = line3dindex.size();
//		threeDGesturesNote->SetLine3DIndex(line3dindexSize,line3dindexSize+positionSize/6-1);
		p3DGestureNoteLine = p3DGestureNoteLine->NextSiblingElement(Serializer_XML_Element_ThreeDGestureNoteLine.c_str());
	}

	map<string, string> attributeList;
	XMLElement* xmlAttributes = pThreeDGestureNote->FirstChildElement(Serializer_XML_Element_Attributes.c_str());
	if (xmlAttributes)
	{
		XMLElement* xmlAttribute = xmlAttributes->FirstChildElement(Serializer_XML_Element_Attribute.c_str());
		while (xmlAttribute)
		{
			string key = xmlAttribute->Attribute(Serializer_XML_Attribute_Key.c_str());
			string value = xmlAttribute->Attribute(Serializer_XML_Attribute_Value.c_str());
			threeDGesturesNote->SetProperty(key, value);
			//attributeList.insert (make_pair(key,value));
			xmlAttribute = xmlAttribute->NextSiblingElement(Serializer_XML_Element_Attribute.c_str());
		}
	}

	AddNoteToScene(scene, threeDGesturesNote);
	return threeDGesturesNote;
}

void NoteFactory::InsectPoint(SceneManager * scene,vector<Vector3> & originScreenPnts,vector<Vector3> & origin3DPnts,vector<vector<Vector3> > &finalPoints)
{
	int pointCount = originScreenPnts.size();
	for (int i = 0; i < pointCount - 1; i++)
	{
		vector<Vector3> singleSegmentPoints;
//		singleSegmentPoints.push_back(origin3DPnts[i]);
		Vector2 start(originScreenPnts[i].m_x, originScreenPnts[i].m_y);
		Vector2 end(originScreenPnts[i + 1].m_x, originScreenPnts[i + 1].m_y);
		Vector2 direction = end - start;
		float length = direction.Length() / 20.0;
		direction.Normalize(); //单位化

		for (int j = 0; j <= 20; j++)
		{
			Vector2 tempScreecPnt = j * length * direction + start;
			Vector3 insectPoint;
			bool state = true;
			if(SVIEW::Parameters::Instance()->m_isShowSpacePoint)
			{
				state = scene->GetPickPoint(tempScreecPnt,insectPoint,false);
			}
			else
			{
				state = scene->GetPickPoint(tempScreecPnt,insectPoint,true);
			}
			if (!state)
			{
				//insectPoint = scene->GetCamera()->GetViewPort().ScreenToWorldPoint(tempScreecPnt.m_x, tempScreecPnt.m_y, 0.5);
			}
			else
			{
				singleSegmentPoints.push_back(insectPoint);
			}
		}
//		singleSegmentPoints.push_back(origin3DPnts[i + 1]);
		if(singleSegmentPoints.size()>0)
		{
		finalPoints.push_back(singleSegmentPoints);
		}
	}
}

void NoteFactory::SplitPolyLine(Gesture3DInfo & originalInfo,Gesture3DInfo& currentInfo,SceneManager * scene)
{
	CameraNode * camera = scene->GetCamera();
	int size = originalInfo.m_pointSet.size();
	for (int i = 0; i < size; i++)
	{
		PolyLine& polyLine = originalInfo.m_pointSet.at(i);//原始的屏幕点
		vector<Vector3> projectPoints; //原始投影点
		vector<int>newPolyLineIndex;
		bool isNeedCreate = true;
		vector<Vector3> & pointSet = polyLine.GetPointList();
		int pointCount = pointSet.size();
		for (int j = 0; j < pointCount; j++)
		{
			Vector3 insectPoint1;
			Vector2 screenPnt(pointSet[j].m_x, pointSet[j].m_y);
			bool intersect1 = scene->GetPickPoint(screenPnt, insectPoint1, true);
			if (!intersect1)
			{
				isNeedCreate = true;
			}
			else
			{
				if(isNeedCreate)
				{
					newPolyLineIndex.push_back(j);
					isNeedCreate  = false;
				}
				projectPoints.push_back(insectPoint1);
			}
		}
	}
}

Note* NoteFactory::CreateThreeDGestureNote(Gesture3DInfo& gesture3DInfos, SceneManager * scene)
{
	ThreeDGesturesNote * note = new ThreeDGesturesNote;
	CameraNode * camera = scene->GetCamera();
	int size = gesture3DInfos.m_pointSet.size();
	for (int i = 0; i < size; i++)
	{
		PolyLine& polyLine = gesture3DInfos.m_pointSet.at(i);
		Color& color = gesture3DInfos.m_colorSet.at(i);
		note->GetOriginalProjectColors().push_back(color);
		int lineWidth = gesture3DInfos.m_lineWidthSet.at(i);
		note->GetOriginalProjectWidths().push_back(lineWidth);
		vector<Vector3> & pointSet = polyLine.GetPointList();
		int pointCount = pointSet.size();

		if(pointCount<2)
		{
			return NULL;
		}
		vector<Vector3> projectPoints; //原始投影点
		for (int j = 0; j < pointCount; j++)
		{
			Vector3 insectPoint1;
			Vector2 screenPnt(pointSet[j].m_x, pointSet[j].m_y);
			bool intersect1 = scene->GetPickPoint(screenPnt, insectPoint1, false);
			if (!intersect1)
			{
			//	insectPoint1 = camera->GetViewPort().ScreenToWorldPoint(screenPnt.m_x, screenPnt.m_y, 0.5);
			}
			else
			{
			projectPoints.push_back(insectPoint1);
			}
		}
		note->GetOriginalProjectPns().push_back(projectPoints);//将投影点加入note
		vector<Vector3> triangleSet;

		{
			vector<vector<Vector3> > finalPoints;
			InsectPoint(scene, pointSet, projectPoints, finalPoints);
//			int pountSize = 0;
//			int lastLineSize = note->m_LineList.size();
//			vector<int> indexs;
			for (int i = 0; i < finalPoints.size(); i++)
			{
				for (int j = 0; j < finalPoints[i].size() - 1; j++)
				{
					Line3D * line = new Line3D(finalPoints[i][j], finalPoints[i][j + 1]);
//					if(j==0)
//					{
//						indexs.push_back(pountSize+lastLineSize);//开始点
//					}
					line->SetColor(color);
					line->SetLineWidth(lineWidth);
					note->AddLine(line);
//					pountSize++;
				}
			}
//			note->SetLine3DIndex(lastLineSize, lastLineSize + pountSize - 1);
//			indexs.push_back(lastLineSize + pountSize - 1);
//			note->SetOrigin3DPointIndex(indexs);
		}
	}

	if (note) {
        note->SetIsShowSpacePoint(SVIEW::Parameters::Instance()->m_isShowSpacePoint);
		AddNoteToScene(scene, note);
	}

	return note;
}

Note* NoteFactory::CreateCommonNote(CommonNoteInfo& commonNoteInfo,SceneManager * scene)
{
	CommonNote * note = new CommonNote;
	CameraNode * camera = scene->GetCamera();
	Vector3 fCoordiante = commonNoteInfo.m_start;
	Vector3 anchorCoordiante = commonNoteInfo.m_end;

	//根据整个场景包围盒的大小来确定文字的大小
	float fontSize = scene->GetSceneBox().Length() / 20;

	Point * point1 = new Point(fCoordiante);
	point1->SetDrawType(1);
	point1->SetSize(100.8);

	Point * point2 = new Point(anchorCoordiante); //为用到，暂留
	point2->SetDrawType(1);
	point2->SetSize(100.8);

	//构造线
	Line3D* line1 = new Line3D(fCoordiante, anchorCoordiante);
	Color line1Color = Color(0.03, 0.38, 0.73);
	line1->SetColor(line1Color);
	line1->SetName("TextImageLeader");

	string createImagePath = commonNoteInfo.m_imagePath;

	LOGI("miangePath,%s %f %f",createImagePath.c_str(),commonNoteInfo.m_end.m_x, commonNoteInfo.m_start.m_y);
	if (createImagePath.length() > 0)
	{
		Vector2 imageBoardSize(commonNoteInfo.m_imageSize.m_x, commonNoteInfo.m_imageSize.m_y);
		//imageBoardSize = ShapeHelper::GetCommonSize(scene, imageBoardSize);

		ImageBoard * imageBroad = new ImageBoard(anchorCoordiante,
				imageBoardSize);

		imageBroad->SetTexture(
				scene->GetResourceManager()->GetOrCreateTexture(createImagePath,
						Texture::TEXTURE_2D));

		//将线加入测量对象中
		note->AddLine(line1);
		note->AddImage(imageBroad);
		note->AddPoint(point1);
	}

	note->SetFrontShow(false); //是否前端显示

	if (note)
	{
		AddNoteToScene(scene, note);
		LOGI("CreateCommonNote ok");
	}

	return note;
}

Note* NoteFactory::CreateVoiceNote(Vector2& screenPnt, const string& text, int type, SceneManager* scene)
{
	Note* note = NULL;
	note = CreatePntVoiceNote(screenPnt, text, scene);
	if (note)
	{
		AddNoteToScene(scene, note);
	}
	return note;
}

Note* NoteFactory::CreateTextNote(int firtShapId, Vector2& screenPnt, const string& text, int type, SceneManager* scene,
		Properties* properties)
{
	Note* note = NULL;
	note = CreatePntTextNote(firtShapId, screenPnt, text, scene, properties);
	if (note)
	{
		AddNoteToScene(scene, note);
//		LOGI("CreateTextNote 1");
	}
	return note;
}

Note*
NoteFactory::CreateSequenceNumberNote(int firtShapId, Vector2& screenPnt, const string& text, SceneManager* scene,
		Properties* properties)
{
	Note* note = NULL;
	note = CreatePntSequenceNumberNote(firtShapId, screenPnt, text, scene, properties);
	if (note)
	{
		AddNoteToScene(scene, note);
	}
	return note;
}

Note* NoteFactory::CreateTextNote(SceneManager* scene)
{
	Note* note = NULL;
	//创建指向点的文本批注
	note = new TextNote();

	if (note)
	{
		AddNoteToScene(scene, note);
	}

	return note;
}

bool NoteFactory::UpdateNoteImagePos(Note* note, Vector2& screenPnt, SceneManager* scene)
{
	bool ret = false;
	Vector3 tempPos;

	if (note != NULL && scene != NULL)
	{
		vector<ImageBoard*>& imageboards = note->GetImageBoards();

		for (int i = 0; i < imageboards.size(); i++)
		{
			ImageBoard* imageBoard = imageboards.at(i);

			if (imageBoard)
			{
//				if(imageBoard->GetName() == "TextsImage")
				{
					scene->Lock();

					Vector3 newPos = imageBoard->GetPosition();

					Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(screenPnt.m_x, screenPnt.m_y);
					Plane projPlane(cameraRay.GetDirection(), newPos);

					newPos = projPlane.Project(cameraRay.GetOrigin()); //得到文本点到平面的投影点

					imageBoard->SetPosition(newPos);
					tempPos = newPos;
					if (note->GetType() == SHAPE_TEXT_NOTE)
					{
						TextNote * temNote = (TextNote*) note; //TODO 暂时这么写 保存位置 为了协同更改位置
						temNote->SetTextsPos(newPos);
					}
					else if (note->GetType() == SHAPE_SEQUENCE_NUMBER_NOTE)
					{
						SequenceNumberNote * tempNote = (SequenceNumberNote*) note;
						tempNote->SetTextsPos(newPos);
					}
					scene->UnLock();
				}
			}

			scene->GetRenderManager()->RequestRedraw();
		}

		vector<Line3D*> &lines = note->m_LineList;
		for (int i = 0; i < lines.size(); i++)
		{
			Line3D *line = lines.at(i);
			if (line)
			{
				if (line->GetName() == "TextImageLeader" || line->GetName() == "SequenceNumberImageLeader")
				{
					scene->Lock();

					line->m_EndPoint = tempPos;

					scene->UnLock();

				}
			}
		}

		return ret;
	}

	return ret;
}

bool NoteFactory::UpdateNoteTextValue(Note* note, string text, SceneManager * scene)
{
	bool ret = false;
	if (note != NULL && scene != NULL)
	{
		vector<ImageBoard*>& imageboards = note->GetImageBoards();
		for (int i = 0; i < imageboards.size(); i++)
		{
			ImageBoard* imageBoard = imageboards.at(i);
			if (imageBoard)
			{
				scene->Lock();

				Vector3 pos = imageBoard->GetPosition();

				//释放掉原来的imageboarad内存

				delete imageBoard;
				imageBoard = NULL;

				vector<Texts2D*> textVector;

				Texts2D *title = new Texts2D;
				title->m_size = 14;
				title->m_texts = "内容";
				textVector.push_back(title);

				Texts2D* temptext = new Texts2D;
				temptext->m_size = 14.0f;
				temptext->m_texts = text;
				textVector.push_back(temptext);
				ImageBoard * imageBoardNew = NULL;
				if (note->GetType() == SHAPE_SEQUENCE_NUMBER_NOTE)
				{

					imageBoardNew = MeasureDisplayHelper::CreateSequenceNumberImage(scene, temptext, pos);
					//保存更新内容
					if (note->m_ComTexts.size() < 1)
					{
						ComText* ct = new ComText();
						CText* t = new CText;
						t->SetText(text);
						ct->AddCText(t);
						note->m_ComTexts.push_back(ct);
					}
					else
					{
						note->m_ComTexts.at(0)->GetCText(0)->SetText(text);
					}
				}
				else if (note->GetType() == SHAPE_TEXT_NOTE)
				{
					imageBoardNew = MeasureDisplayHelper::createNoteTextsImageN(scene, textVector, pos);

					if (note->m_ComTexts.size() < 1)
					{
						ComText* ct = new ComText();
						CText* t = new CText;
						t->SetText(text);
						ct->AddCText(t);
						note->m_ComTexts.push_back(ct);
					}
					else
					{
						note->m_ComTexts.at(0)->GetCText(0)->SetText(text);
					}
				}

				//更新imageBoard
				imageboards.at(i) = imageBoardNew;

				scene->GetRenderManager()->RequestRedraw();

				ret = true;
				scene->UnLock();
			}
		}

	}
	return ret;
}

Note* NoteFactory::CreateTextNoteFromXMLElement(SceneManager* scene, const string& xmlElement)
{
	LOGI("NoteFactory::CreateTextNoteFromXMLElement BEGIN");
	TextNote* textNote = new TextNote;
	float fSize;
	string contentStr;
	Vector3 startPnt, endPnt;
	if (xmlElement.length() == 0)
	{
		return textNote;
	}

	const char* xml = xmlElement.c_str();
	XMLDocument doc;
	doc.Parse(xml);

	XMLElement* xmlNote = doc.FirstChildElement(Serializer_XML_Element_TextNote.c_str());

	if (xmlNote == NULL)
	{
		return NULL;
	}
	int id = xmlNote->IntAttribute(Serializer_XML_Attribute_ID.c_str());
	//textNote->SetID(id);
	bool isParallelScreen = xmlNote->BoolAttribute(Serializer_XML_Attribute_IsParallelScreen.c_str());
	textNote->SetParallelScreen(isParallelScreen);
	bool isFix = xmlNote->BoolAttribute(Serializer_XML_Attribute_IsFix.c_str());
	textNote->SetFix(isFix);
	bool visible = xmlNote->BoolAttribute(Serializer_XML_Attribute_Visible.c_str());
	textNote->SetVisible(visible);

	XMLElement* xmlLeaders = xmlNote->FirstChildElement(Serializer_XML_Element_Leaders.c_str());
	//LOGI("xmlLeaders");
	if (xmlLeaders)
	{
		XMLElement* xmlLeader = xmlLeaders->FirstChildElement(Serializer_XML_Element_Leader.c_str());
		if (xmlLeader)
		{
			//LOGI("xmlLeader");
			XMLElement* xmlPolyline = xmlLeader->FirstChildElement(Serializer_XML_Element_Polyline.c_str());
			if (xmlPolyline)
			{
				//LOGI("xmlPolyline");
				XMLElement* xmlPoint = xmlPolyline->FirstChildElement(Serializer_XML_Element_Point.c_str());
				Vector3 temVec[2];
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
				endPnt = temVec[1]; //先这样写着
				textNote->SetNotePos(temVec[0]);
				textNote->SetTextsPos(temVec[1]);
				//	LOGI("startPnt %s",textNote->GetNotePos().Tostring().c_str());
				//	LOGI("endPnt %s",textNote->GetTextsPos().Tostring().c_str());

			}
		}
	}

	XMLElement* xmlCompositeTexts = xmlNote->FirstChildElement(Serializer_XML_Element_CompositeTexts.c_str());

	if (xmlCompositeTexts)
	{
		//LOGI("Begin xmlCompositeTexts");
		XMLElement* compositeText = xmlCompositeTexts->FirstChildElement(Serializer_XML_Element_CompositeText.c_str());

		if (compositeText)
		{
			//LOGI("Begin compositeText");
			XMLElement* texts = compositeText->FirstChildElement(Serializer_XML_Element_Texts.c_str());
			if (texts)
			{
				//LOGI("Begin Texts");
				XMLElement* xmlText = texts->FirstChildElement(Serializer_XML_Element_Text.c_str());

				string value = xmlText->Attribute(Serializer_XML_Attribute_Value.c_str());
				contentStr = value;
				//LOGI("textNote->m_ComTexts.size() %d",textNote->m_ComTexts.size());
				if (textNote->m_ComTexts.size() > 0)
				{
					CText * tempText = new CText;
					tempText->SetText(value);
					textNote->m_ComTexts[0]->AddCText(tempText);
				}
				else
				{
					//	LOGI("textNote->m_ComTexts pushback");
					ComText * tempComeText = new ComText;
					CText * tempText = new CText;
					tempText->SetText(value);
					tempComeText->AddCText(tempText);
					textNote->m_ComTexts.push_back(tempComeText);
					//	LOGI("textNote->m_ComTexts.size() %d",textNote->m_ComTexts.size());
				}

				if (xmlText)
				{
					XMLElement* xmlTextStyle = xmlText->FirstChildElement(Serializer_XML_Element_TextStyle.c_str());

					if (xmlTextStyle)
					{
						XMLElement* xmlCharHeight = xmlTextStyle->FirstChildElement(
								Serializer_XML_Element_CharHeight.c_str());
						//	LOGI("xmlCharHeight %p", xmlCharHeight);
						if (xmlCharHeight)
						{
							//	LOGI("begin xmlCharHeight");
							string fontTextTemp = "1";
							if (!xmlCharHeight->GetText())
							{
								fontTextTemp = "15";
							}
							else
							{
								fontTextTemp = xmlCharHeight->GetText();
							}
							//LOGI(
							//	"xmlCharHeight get text %s", xmlCharHeight->GetText());
							//LOGI("begin StringHelper::StringToFloat");
							float fontSize = StringHelper::StringToFloat(fontTextTemp);
							if (fontSize < 1.0)
								fontSize = 15.0f;
							fSize = fontSize;
							//LOGI("End xmlCharHeight");
						}
						//float width, height;
					}
					//LOGI("End xmlTextStyle");

					XMLElement* xmlLocaltion = xmlText->FirstChildElement(Serializer_XML_Element_Location.c_str());
					string localtion = "1.222 1.222 1.222";
					xmlLocaltion->GetText();

					if (!xmlLocaltion->GetText())
					{
						localtion = "1.222 1.222 1.222";
					}
					else
					{
						localtion = xmlLocaltion->GetText();
					}
					//LOGI("xmlLocaltion get text %s", xmlLocaltion->GetText());
					vector<string> floats = StringHelper::Split(localtion, " ");

					if (floats.size() == 3)
					{
						float x = StringHelper::StringToFloat(floats[0]);
						float y = StringHelper::StringToFloat(floats[1]);
						float z = StringHelper::StringToFloat(floats[2]);

						Vector3 tempV(x, y, z);
						//textNote->SetTextsPos(tempV);
					}
				}
			}
		}
	}
	//LOGI("End compositeTexts");

	//LOGI("Begin xmlAttributes");
	//解析Attributes元素及子元素
	map<string, string> attributeList;
	XMLElement* xmlAttributes = xmlNote->FirstChildElement(Serializer_XML_Element_Attributes.c_str());
	if (xmlAttributes)
	{
		XMLElement* xmlAttribute = xmlAttributes->FirstChildElement(Serializer_XML_Element_Attribute.c_str());
		while (xmlAttribute)
		{
			string key = xmlAttribute->Attribute(Serializer_XML_Attribute_Key.c_str());
			string value = xmlAttribute->Attribute(Serializer_XML_Attribute_Value.c_str());
			textNote->SetProperty(key, value);
			//attributeList.insert (make_pair(key,value));
			xmlAttribute = xmlAttribute->NextSiblingElement(Serializer_XML_Element_Attribute.c_str());
		}
	}
	//LOGI("End xmlAttributes");

	Line3D * line1 = new Line3D(startPnt, endPnt); //引线
	line1->SetName("TextImageLeader");
	Color line1Color = Color(0.03, 0.38, 0.73);
	line1->SetColor(line1Color);
	Point * point1 = new Point(startPnt);
	point1->SetDrawType(1);
	point1->SetSize(0.8);
	Point * point2 = new Point(endPnt);
	point2->SetDrawType(1);
	point2->SetSize(0.8);
	std::vector<Texts2D*> textsArray;

	Texts2D * title = new Texts2D;
	title->m_texts = "内容";
	title->m_size = fSize;
	textsArray.push_back(title);
	Texts2D * contents = new Texts2D;
	contents->m_texts = contentStr;
	contents->m_size = fSize;
	textsArray.push_back(contents);

	//名字未判断空行，以后加上
	Texts2D * key1 = new Texts2D;
	key1->m_texts = "UserName";
	key1->m_size = fSize;
	textsArray.push_back(key1);
	Texts2D * value1 = new Texts2D;
	value1->m_texts = textNote->GetProperty("UserName");
	value1->m_size = fSize;
	textsArray.push_back(value1);

	Texts2D * key2 = new Texts2D;
	key2->m_texts = "CreateTime";
	key2->m_size = fSize;
	textsArray.push_back(key2);
	Texts2D * value2 = new Texts2D;
	value2->m_texts = textNote->GetProperty("CreateTime");
	value2->m_size = fSize;
	textsArray.push_back(value2);

	Texts2D * key3 = new Texts2D;
	key3->m_texts = "Guid";
	key3->m_size = fSize;
	textsArray.push_back(key3);
	Texts2D * value3 = new Texts2D;
	value3->m_texts = textNote->GetProperty("Guid");
	value3->m_size = fSize;
	textsArray.push_back(value3);

	ImageBoard * imageBord = MeasureDisplayHelper::createNoteTextsImageN(scene, textsArray, endPnt);
	LOGI("ImageBoard pointer %p", imageBord);

	textNote->AddLine(line1);
	textNote->AddPoint(point1);
	//textNote->AddPoint(point2);
	textNote->AddImage(imageBord);

	textNote->SetFrontShow(false); //设置是否前端显示
	if (textNote)
	{
		AddNoteToScene(scene, textNote);
	}
	LOGI("NoteFactory::CreateTextNoteFromXMLElement END");
	return textNote;
}

Note* NoteFactory::CreateTextNoteFromJSON(SceneManager* scene, const string& JSONValue)
{
	Note* textNote = NULL;

	if (textNote)
	{
		AddNoteToScene(scene, textNote);
	}

	return textNote;
}

string NoteFactory::TextNoteToXMLElement(SceneManager* scene, TextNote* textNote)
{
	LOGI("begin serialize");
	//序列化
	string xmlElement = "";

	XMLDocument pDoc; //创建文档
	//添加声明
	//XMLDeclaration * pDeclaration = pDoc->NewDeclaration("xml version=\"1.0\" encoding=\"utf-8\" standalone=\"no\"");
	//pDoc->LinkEndChild(pDeclaration);

	//创建根节点
	XMLElement *pTextNote = pDoc.NewElement(Serializer_XML_Element_TextNote.c_str());
	pDoc.LinkEndChild(pTextNote);
	pTextNote->SetAttribute(Serializer_XML_Attribute_ID.c_str(), (int)textNote->GetID());
	pTextNote->SetAttribute(Serializer_XML_Attribute_IsParallelScreen.c_str(), textNote->IsParallelScreen());
	pTextNote->SetAttribute(Serializer_XML_Attribute_IsFix.c_str(), textNote->IsFix());
	pTextNote->SetAttribute(Serializer_XML_Attribute_Visible.c_str(), textNote->IsVisible());
	{
		//创建Color节点
		XMLElement * pColor = pDoc.NewElement(Serializer_XML_Element_Color.c_str());
		pTextNote->LinkEndChild(pColor);
		Color* color = textNote->GetColor();
		pColor->SetAttribute(Serializer_XML_Attribute_R.c_str(), color->m_r);
		pColor->SetAttribute(Serializer_XML_Attribute_G.c_str(), color->m_g);
		pColor->SetAttribute(Serializer_XML_Attribute_B.c_str(), color->m_b);

	}

	{
		//Leaders节点
		XMLElement * pLeaders = pDoc.NewElement(Serializer_XML_Element_Leaders.c_str());
		pTextNote->LinkEndChild(pLeaders);
		{
//			//创建Leader节点
//			if (textNote->getHaseLeader())
//			{
			XMLElement * pLeader = pDoc.NewElement(Serializer_XML_Element_Leader.c_str());
			pLeader->SetAttribute(Serializer_XML_Attribute_Type.c_str(), 1);
			pLeaders->LinkEndChild(pLeader);

			{
				//创建Terminator
				XMLElement * pTerminator = pDoc.NewElement(Serializer_XML_Element_Terminator.c_str());
				pLeader->LinkEndChild(pTerminator);
				pTerminator->SetAttribute(Serializer_XML_Attribute_Type.c_str(), 4); //TODO:Type
				pTerminator->SetAttribute(Serializer_XML_Attribute_Width.c_str(), 3); //TODO:
				pTerminator->SetAttribute(Serializer_XML_Attribute_Height.c_str(), 1); //TODO:
				{
					//创建Location
					XMLElement * pLocation = pDoc.NewElement(Serializer_XML_Element_Location.c_str());
					pTerminator->LinkEndChild(pLocation);
					pLocation->SetAttribute(Serializer_XML_Attribute_X.c_str(), 0.0f);
					pLocation->SetAttribute(Serializer_XML_Attribute_Y.c_str(), 0.0f);
					pLocation->SetAttribute(Serializer_XML_Attribute_Z.c_str(), 0.0f);

					//创建Direction
					XMLElement * pDirection = pDoc.NewElement(Serializer_XML_Element_Direction.c_str());
					pTerminator->LinkEndChild(pDirection);
					pDirection->SetAttribute(Serializer_XML_Attribute_X.c_str(), -0.7); //TODO
					pDirection->SetAttribute(Serializer_XML_Attribute_Y.c_str(), 0.7); //TODO
					pDirection->SetAttribute(Serializer_XML_Attribute_Z.c_str(), 0.0); //TODO
				}

				//创建Polyline
				XMLElement * pPolyline = pDoc.NewElement(Serializer_XML_Element_Polyline.c_str());
				pLeader->LinkEndChild(pPolyline);
				pPolyline->SetAttribute(Serializer_XML_Attribute_ID.c_str(), 0); //TODO:
				pPolyline->SetAttribute(Serializer_XML_Attribute_Type.c_str(), 1); //TODO:
				pPolyline->SetAttribute(Serializer_XML_Attribute_Min.c_str(), 0); //TODO:
				pPolyline->SetAttribute(Serializer_XML_Attribute_Max.c_str(), 0); //TODO:
				{
					//Line3D *line  = new Line3D(textNote->GetNotePos(),textNote->GetTextsPos());
					//创建Point
					{
						XMLElement * pPoint = pDoc.NewElement(Serializer_XML_Element_Point.c_str());
						pPolyline->LinkEndChild(pPoint);
						pPoint->SetAttribute(Serializer_XML_Attribute_X.c_str(), textNote->GetNotePos().m_x); //TODO
						pPoint->SetAttribute(Serializer_XML_Attribute_Y.c_str(), textNote->GetNotePos().m_y); //TODO
						pPoint->SetAttribute(Serializer_XML_Attribute_Z.c_str(), textNote->GetNotePos().m_z); //TODO
					}

					//创建Point
					{
						XMLElement * pPoint = pDoc.NewElement(Serializer_XML_Element_Point.c_str());
						pPolyline->LinkEndChild(pPoint);
						pPoint->SetAttribute(Serializer_XML_Attribute_X.c_str(), textNote->GetTextsPos().m_x); //TODO
						pPoint->SetAttribute(Serializer_XML_Attribute_Y.c_str(), textNote->GetTextsPos().m_y); //TODO
						pPoint->SetAttribute(Serializer_XML_Attribute_Z.c_str(), textNote->GetTextsPos().m_z); //TODO
					}

				}
			}
//			}
		}
		{
			//ExtendLines
			XMLElement * pExtendLines = pDoc.NewElement(Serializer_XML_Element_ExtendLines.c_str());
			pTextNote->LinkEndChild(pExtendLines);
		}

		{
			//CompositeTexts
			XMLElement * pCompositeTexts = pDoc.NewElement(Serializer_XML_Element_CompositeTexts.c_str());
			pTextNote->LinkEndChild(pCompositeTexts);
			if (textNote->m_ComTexts.size() > 0)
			{
				//CompositeText
				XMLElement * pCompositeText = pDoc.NewElement(Serializer_XML_Element_CompositeText.c_str());
				pCompositeTexts->LinkEndChild(pCompositeText);
				{
					//Texts
					XMLElement * pTexts = pDoc.NewElement(Serializer_XML_Element_Texts.c_str());
					pCompositeText->LinkEndChild(pTexts);
					{
						//Text
						XMLElement * pText = pDoc.NewElement(Serializer_XML_Element_Text.c_str());
						pTexts->LinkEndChild(pText);
						pText->SetAttribute(Serializer_XML_Attribute_Value.c_str(),
								textNote->m_ComTexts.at(0)->GetCText(0)->GetText().c_str());
						{
							//TextStyle
							XMLElement * pTextStyle = pDoc.NewElement(Serializer_XML_Element_TextStyle.c_str());
							pText->LinkEndChild(pTextStyle);
							{
								//CharHeight
								{
									XMLElement * pCharHeight = pDoc.NewElement(
											Serializer_XML_Element_CharHeight.c_str());
									pTextStyle->LinkEndChild(pCharHeight);
									float width = 10.0f, height = 10.0f;
									textNote->m_ComTexts.at(0)->GetCText(0)->GetCharWidthHeight(width, height);
									tinyxml2::XMLText * pContent = pDoc.NewText(M3DTOOLS::floatToString(width).c_str());
									pCharHeight->LinkEndChild(pContent);
								}
								//LineSpacing
								{
									XMLElement * pLineSpacing = pDoc.NewElement(
											Serializer_XML_Element_LineSpacing.c_str());
									pTextStyle->LinkEndChild(pLineSpacing);
									float space = 1.0f;
									space = textNote->m_ComTexts.at(0)->GetCText(0)->GetCharSpacing();
									tinyxml2::XMLText * pContent = pDoc.NewText(M3DTOOLS::floatToString(space).c_str());
									pLineSpacing->LinkEndChild(pContent);
								}
								//Font
								{
									XMLElement * pFont = pDoc.NewElement(Serializer_XML_Element_Font.c_str());
									pTextStyle->LinkEndChild(pFont);
									pFont->SetAttribute(Serializer_XML_Attribute_Name.c_str(), ""); //TODO
								}
								//UsageType
								{
									XMLElement * pUsageType = pDoc.NewElement(Serializer_XML_Element_UsageType.c_str());
									pTextStyle->LinkEndChild(pUsageType);
									pUsageType->SetAttribute(Serializer_XML_Attribute_Name.c_str(), 2); //TODO
								}
							}

							//Location
							{
								XMLElement * pLocation = pDoc.NewElement(Serializer_XML_Element_Location.c_str());
								pText->LinkEndChild(pLocation);

								string temstr = M3DTOOLS::floatToString(textNote->GetNotePos().m_x) + " "
										+ M3DTOOLS::floatToString(textNote->GetNotePos().m_y) + " "
										+ M3DTOOLS::floatToString(textNote->GetNotePos().m_z);
								tinyxml2::XMLText * pContent = pDoc.NewText(temstr.c_str());
								pLocation->LinkEndChild(pContent);

							}
							//Rotation
							{
								XMLElement * pRotation = pDoc.NewElement(Serializer_XML_Element_Rotation.c_str());
								pText->LinkEndChild(pRotation);
								tinyxml2::XMLText * pContent = pDoc.NewText(
										"1.000000 0.000000 0.000000 0.000000 1.000000 0.000000"); //TODO
								pRotation->LinkEndChild(pContent);
							}
						}
					}

					//OuterBox
					{
						XMLElement * pOuterBox = pDoc.NewElement(Serializer_XML_Element_OuterBox.c_str());
						pTexts->LinkEndChild(pOuterBox);
						tinyxml2::XMLText * pContent = pDoc.NewText(
								"1.000000 1.000000 1.000000-1.000000 -1.000000 -1.000000"); //TODO
						pOuterBox->LinkEndChild(pContent);
					}
				}
			}
		}

//		{
//			//Attributes
//			XMLElement * pAttributes = pDoc.NewElement(Serializer_XML_Element_Attributes.c_str());
//			pTextNote->LinkEndChild(pAttributes);
//			{
//				//Attribute
//				XMLElement * pAttribute = pDoc.NewElement(Serializer_XML_Element_Attribute.c_str());
//				pAttributes->LinkEndChild(pAttribute);
//				pAttribute->SetAttribute(Serializer_XML_Attribute_Key.c_str(),"UserName");
//				pAttribute->SetAttribute(Serializer_XML_Attribute_Value.c_str(),textNote->GetProperty("UserName").c_str());
//
//			}
//
//			{
//				//Attribute
//				XMLElement * pAttribute = pDoc.NewElement(Serializer_XML_Element_Attribute.c_str());
//				pAttributes->LinkEndChild(pAttribute);
//				pAttribute->SetAttribute(Serializer_XML_Attribute_Key.c_str(),"CreateTime");
//				pAttribute->SetAttribute(Serializer_XML_Attribute_Value.c_str(),textNote->GetProperty("CreateTime").c_str());
//			}
//
//			{
//				//Attribute
//				XMLElement * pAttribute = pDoc.NewElement(Serializer_XML_Element_Attribute.c_str());
//				pAttributes->LinkEndChild(pAttribute);
//				pAttribute->SetAttribute(Serializer_XML_Attribute_Key.c_str(),"Guid");
//				pAttribute->SetAttribute(Serializer_XML_Attribute_Value.c_str(),textNote->GetProperty("Guid").c_str());
//			}
//
//		}

		{
			XMLElement * pAttributes = pDoc.NewElement(Serializer_XML_Element_Attributes.c_str());
			pTextNote->LinkEndChild(pAttributes);
			for (map<string, string>::iterator it = textNote->GetPropertyMap().begin();
					it != textNote->GetPropertyMap().end(); it++)
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

string NoteFactory::TextNoteToJSONValue(SceneManager* scene, TextNote* textNote)
{
	string JSONValue = "";

	return JSONValue;
}

Note* NoteFactory::CreateVoiceNoteFromXMLElement(SceneManager* scene, const string& xmlElement)
{

	VoiceNote* voiceNote = NULL;
	if (xmlElement.length() == 0)
	{
		return NULL;
	}

	const char* xml = xmlElement.c_str();
	XMLDocument doc;
	doc.Parse(xml);

	XMLElement* pVoiceNote = doc.FirstChildElement(Serializer_XML_Element_VoiceNote.c_str());
	if (pVoiceNote)
		int id = pVoiceNote->IntAttribute(Serializer_XML_Attribute_ID.c_str());

	XMLElement * pLocation = pVoiceNote->FirstChildElement(Serializer_XML_Element_Location.c_str());
	if (pLocation)
	{
		string localtion = pLocation->GetText();
		vector<string> floats = StringHelper::Split(localtion, " ");
		if (3 == floats.size())
		{
			float x = StringHelper::StringToFloat(floats[0]);
			float y = StringHelper::StringToFloat(floats[1]);
			float z = StringHelper::StringToFloat(floats[2]);

			Vector3 tempV(x, y, z);
			LOGI("POS %s", tempV.Tostring().c_str());
			Vector2 voiceNoteSize(1.0f, 1.0f);

			voiceNote = new VoiceNote(tempV, voiceNoteSize, scene);

			voiceNote->SetPosition(tempV);
		}
	}
	XMLElement * pVoiceData = pVoiceNote->FirstChildElement(Serializer_XML_Element_VoiceData.c_str());
	if (pVoiceData && pVoiceData->FirstChild())
	{
		string uri = pVoiceData->Attribute(Serializer_XML_Attribute_URI.c_str());
		voiceNote->SetUri(uri);
		LOGI("uri = %s", uri.c_str());

		string format = pVoiceData->Attribute(Serializer_XML_Attribute_Format.c_str());
		voiceNote->SetFormat(format);
		LOGI("format = %s", format.c_str());

		const char * fileData = pVoiceData->GetText();
		string voiceData;
		if (fileData != NULL)
		{
			voiceData.assign(fileData);
		}
		voiceNote->SetVoiceData(voiceData);
		LOGI("voiceData = %s", voiceData.c_str());
	}

	//解析Attributes元素及子元素
	map<string, string> attributeList;
	XMLElement* xmlAttributes = pVoiceNote->FirstChildElement(Serializer_XML_Element_Attributes.c_str());
	if (xmlAttributes)
	{
		XMLElement* xmlAttribute = xmlAttributes->FirstChildElement(Serializer_XML_Element_Attribute.c_str());
		while (xmlAttribute)
		{
			string key = xmlAttribute->Attribute(Serializer_XML_Attribute_Key.c_str());
			string value = xmlAttribute->Attribute(Serializer_XML_Attribute_Value.c_str());
			LOGI("key = %s , Value = %s", key.c_str(), value.c_str());
			voiceNote->SetProperty(key, value);
			//attributeList.insert (make_pair(key,value));
			xmlAttribute = xmlAttribute->NextSiblingElement(Serializer_XML_Element_Attribute.c_str());
		}
	}

	if (voiceNote)
	{
		AddNoteToScene(scene, voiceNote);
	}

	return voiceNote;

}

string NoteFactory::VoiceNoteToXMLElement(SceneManager* scene, VoiceNote* voiceNote)
{
	LOGI("NoteFactory::VoiceNoteToXMLElement BEGIN");
	//序列化
	string xmlElement = "";

	XMLDocument pDoc; //创建文档
	//添加声明
	//XMLDeclaration * pDeclaration = pDoc->NewDeclaration("xml version=\"1.0\" encoding=\"utf-8\" standalone=\"no\"");
	//pDoc->LinkEndChild(pDeclaration);

	//创建根节点

	XMLElement *pVoiceNote = pDoc.NewElement(Serializer_XML_Element_VoiceNote.c_str());
	pDoc.LinkEndChild(pVoiceNote);
	pVoiceNote->SetAttribute(Serializer_XML_Attribute_ID.c_str(), (int)voiceNote->GetID());

	{
		XMLElement * pLocation = pDoc.NewElement(Serializer_XML_Element_Location.c_str());
		pVoiceNote->LinkEndChild(pLocation);

		float X = voiceNote->GetPosition().m_x;
		float Y = voiceNote->GetPosition().m_y;
		float Z = voiceNote->GetPosition().m_z;

		string locationText = M3DTOOLS::floatToString(X) + " " + M3DTOOLS::floatToString(Y) + " "
				+ M3DTOOLS::floatToString(Z);

		LOGI("locationText %s", locationText.c_str());
		XMLText * pLocationText = pDoc.NewText(locationText.c_str());
		pLocation->LinkEndChild(pLocationText);
	}
	{
		XMLElement * pVoiceData = pDoc.NewElement(Serializer_XML_Element_VoiceData.c_str());
		pVoiceNote->LinkEndChild(pVoiceData);

		pVoiceData->SetAttribute(Serializer_XML_Attribute_URI.c_str(), voiceNote->GetUri().c_str()); //TODO
		pVoiceData->SetAttribute(Serializer_XML_Attribute_Format.c_str(), voiceNote->GetFormat().c_str()); //TODO

		XMLText * pVoiceDataText = pDoc.NewText(voiceNote->GetVoiceData().c_str());
		pVoiceData->LinkEndChild(pVoiceDataText);
	}

	{
		XMLElement * pAttributes = pDoc.NewElement(Serializer_XML_Element_Attributes.c_str());
		pVoiceNote->LinkEndChild(pAttributes);
		for (map<string, string>::iterator it = voiceNote->GetPropertyMap().begin();
				it != voiceNote->GetPropertyMap().end(); it++)
		{
			XMLElement * pAttribute = pDoc.NewElement(Serializer_XML_Element_Attribute.c_str());
			pAttributes->LinkEndChild(pAttribute);
			pAttribute->SetAttribute(Serializer_XML_Attribute_Key.c_str(), it->first.c_str());
			pAttribute->SetAttribute(Serializer_XML_Attribute_Value.c_str(), it->second.c_str());
		}

	}
	tinyxml2::XMLPrinter priter;
	pDoc.Accept(&priter);
	xmlElement = priter.CStr();
	LOGI("NoteFactory::VoiceNoteToXMLElement END");
	return xmlElement;

}

bool NoteFactory::AddNoteToScene(SceneManager* scene, Note* note)
{
	bool ret = false;

	//判断场景是否为空
	if (scene == NULL || note == NULL)
	{
		return ret;
	}

	//得到测量组节点，创建的测量节点加入到此组中
	NoteGroup* noteGroup = scene->GetNoteGroup();

	//创建一个节点，用来挂载创建的测量measureNote对象
	ShapeNode* node = new ShapeNode();

	node->SetShape(note);

	//给新创建的Node设置一个名字
	char strID[10];
	sprintf(strID, "%d", node->GetID());
	node->SetName(noteGroup->GetName() + "|" + strID);

	//修改场景结构，加锁
	scene->Lock();

	//将创建的测量对象加入到测量组中
	noteGroup->AddChild(node);

	scene->AddShapeIDToMap(note);

	scene->GetRenderManager()->RequestRedraw();
	//完成修改，解锁
	scene->UnLock();

	ret = true;

	return ret;
}

Note* NoteFactory::CreatePntTextNote(int firtShapId, Vector2& screenPnt, const string& text, SceneManager* scene,
		Properties* properties)
{
	TextNote* note = NULL;
//
	Shape* firstShape = (Shape*)scene->GetShape(firtShapId);
	if(!firstShape)
	{
		LOGI("firstShape is NULL");
	}
	//如果的shape对象存在
	if (firstShape && firstShape->GetType() == SHAPE_POINT_HANDLE)
	{
		note = new TextNote;

		HandlerPoint* firstPnt = (HandlerPoint*) firstShape;

		Vector3 fCoordiante = firstPnt->GetPosition();

		note->SetNotePos(fCoordiante);

		Vector3 anchorCoordiante;

		if (scene->GetPickPoint(screenPnt, anchorCoordiante, false))
		{
			note->SetTextsPos(anchorCoordiante);
			//构造显示文本
			string lengthstr = text;
			//根据整个场景包围盒的大小来确定文字的大小
			float fontSize = scene->GetSceneBox().Length() / 20;

			Point * point1 = new Point(fCoordiante);
			point1->SetDrawType(1);
			point1->SetSize(0.8);

			Point * point2 = new Point(anchorCoordiante); //为用到，暂留
			point2->SetDrawType(1);
			point2->SetSize(0.8);

			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(screenPnt.m_x, screenPnt.m_y);
			Plane projPlane(cameraRay.GetDirection(), fCoordiante);

			Vector3 pntInPlane = projPlane.Project(anchorCoordiante); //得到文本点到平面的投影点

			//构造线
			Line3D* line1 = new Line3D(fCoordiante, pntInPlane);
			Color line1Color = Color(0.03, 0.38, 0.73);
			line1->SetColor(line1Color);
			line1->SetName("TextImageLeader");

			note->SetTextsPos(pntInPlane); //将原始文本点变为其在camera平面的投影点

			vector<Texts2D*> temptext;
			Texts2D *title = new Texts2D;
			title->m_size = 14;
			title->m_texts = "内容";
			temptext.push_back(title);

			Texts2D *content = new Texts2D;
			content->m_size = 14.0f;
			content->m_texts = lengthstr;
			temptext.push_back(content);

			ImageBoard * imageBroad = MeasureDisplayHelper::createNoteTextsImageN(scene, temptext, pntInPlane);

			//将线加入测量对象中
			note->AddLine(line1);
			note->AddImage(imageBroad);
			note->AddPoint(point1);

			note->CopyProperties(properties);

			ComText* ct = new ComText();
			CText* t = new CText;

			t->SetText(text);
			//t->SetCharWidthHeight(content->m_size ,content->m_size );
			ct->AddCText(t);
			note->m_ComTexts.push_back(ct);

			note->SetFrontShow(false); //是否前端显示
		}
	}
	else
	{
		LOGE("MeasureFactory::createPntToPntDistance shape is NULL");
	}
	LOGI("note 1");

	return note;
}

Note*
NoteFactory::CreatePntSequenceNumberNote(int notePointShapId, Vector2& screenPnt, const string& text,
		SceneManager* scene, Properties* properties)
{
	SequenceNumberNote* note = NULL;
//
	Shape* notePointShape = (Shape*)scene->GetShape(notePointShapId);
	if(!notePointShape)
	{
		LOGI("notePointShape is NULL");
	}
	//如果的shape对象存在
	if (notePointShape && notePointShape->GetType() == SHAPE_POINT_HANDLE)
	{
		note = new SequenceNumberNote;

		HandlerPoint* firstPnt = (HandlerPoint*) notePointShape;

		Vector3 fCoordiante = firstPnt->GetPosition();

		note->SetNotePos(fCoordiante);

		Vector3 anchorCoordiante;

		if (scene->GetPickPoint(screenPnt, anchorCoordiante, false))
		{
			//构造显示文本
			string lengthstr = text;
			//根据整个场景包围盒的大小来确定文字的大小
			float fontSize = scene->GetSceneBox().Length() / 20;

			Point * point1 = new Point(fCoordiante);
			point1->SetDrawType(1);
			point1->SetSize(0.8);

			Point * point2 = new Point(anchorCoordiante); //为用到，暂留
			point2->SetDrawType(1);
			point2->SetSize(0.8);

			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(screenPnt.m_x, screenPnt.m_y);
			Plane projPlane(cameraRay.GetDirection(), fCoordiante);

			Vector3 pntInPlane = projPlane.Project(anchorCoordiante); //得到文本点到平面的投影点

			//构造线
			Line3D* line1 = new Line3D(fCoordiante, pntInPlane);
			Color line1Color = Color(0.03, 0.38, 0.73);
			line1->SetColor(line1Color);
			line1->SetName("SequenceNumberImageLeader");

			note->SetTextsPos(pntInPlane); //将原始文本点变为其在camera平面的投影点

			Texts2D* temptext = new Texts2D;
			temptext->m_size = 14.0f;
			temptext->m_texts = lengthstr;

			ImageBoard * imageBroad = MeasureDisplayHelper::CreateSequenceNumberImage(scene, temptext, pntInPlane);

			//将线加入测量对象中
			note->AddLine(line1);
			note->AddImage(imageBroad);
			note->AddPoint(point1);

			note->CopyProperties(properties);

			ComText* ct = new ComText();
			CText* t = new CText;

			t->SetText(text);
			//t->SetCharWidthHeight(content->m_size ,content->m_size );
			ct->AddCText(t);
			note->m_ComTexts.push_back(ct);

			note->SetFrontShow(false); //是否前端显示
		}
	}
	else
	{
		LOGE("MeasureFactory::createPntToPntDistance shape is NULL");
	}

	return note;
}

Note* NoteFactory::CreatePntVoiceNote(Vector2& screenPnt, const string& text, SceneManager* scene)
{
	Note* note = NULL;
	Vector3 anchorCoordiante;
	//LOGE("");
	if (scene->GetPickPoint(screenPnt, anchorCoordiante, false))
	{
		Vector2 voiceNoteSize(1.0f, 1.0f);
		note = new VoiceNote(anchorCoordiante,voiceNoteSize, scene);
	}

	return note;
}

Note* NoteFactory::CreateSequenceNoteFromXMLElement(SceneManager* scene, const string& xmlElement)
{
	LOGI("NoteFactory::CreateSequenceNoteFromXMLElement BEGIN");
	SequenceNumberNote * seauenceNote = new SequenceNumberNote;

	float fSize = 15.0f;
	string contentStr;
	Vector3 startPnt, endPnt;
	if (xmlElement.length() == 0)
	{
		return seauenceNote;
	}

	const char* xml = xmlElement.c_str();
	XMLDocument doc;
	doc.Parse(xml);

	XMLElement* xmlNote = doc.FirstChildElement(Serializer_XML_Element_SequenceNote.c_str());

	int id = xmlNote->IntAttribute(Serializer_XML_Attribute_ID.c_str());

	XMLElement* xmlLeaders = xmlNote->FirstChildElement(Serializer_XML_Element_Leaders.c_str());
	//LOGI("xmlLeaders");
	if (xmlLeaders)
	{
		XMLElement* xmlLeader = xmlLeaders->FirstChildElement(Serializer_XML_Element_Leader.c_str());
		if (xmlLeader)
		{
			//LOGI("xmlLeader");
			XMLElement* xmlPolyline = xmlLeader->FirstChildElement(Serializer_XML_Element_Polyline.c_str());
			if (xmlPolyline)
			{
				//LOGI("xmlPolyline");
				XMLElement* xmlPoint = xmlPolyline->FirstChildElement(Serializer_XML_Element_Point.c_str());
				Vector3 temVec[2];
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
				endPnt = temVec[1]; //先这样写着
				seauenceNote->SetNotePos(temVec[0]);
				seauenceNote->SetTextsPos(temVec[1]);
				//	LOGE("startPnt %s",textNote->GetNotePos().Tostring().c_str());
				//	LOGE("endPnt %s",textNote->GetTextsPos().Tostring().c_str());

			}
		}
	}

	XMLElement* xmlCompositeTexts = xmlNote->FirstChildElement(Serializer_XML_Element_CompositeTexts.c_str());

	if (xmlCompositeTexts)
	{
		//LOGI("Begin xmlCompositeTexts");
		XMLElement* compositeText = xmlCompositeTexts->FirstChildElement(Serializer_XML_Element_CompositeText.c_str());

		if (compositeText)
		{
			//LOGI("Begin compositeText");
			XMLElement* texts = compositeText->FirstChildElement(Serializer_XML_Element_Texts.c_str());
			if (texts)
			{
				//LOGI("Begin Texts");
				XMLElement* xmlText = texts->FirstChildElement(Serializer_XML_Element_Text.c_str());

				if (xmlText)
				{
					string value = xmlText->Attribute(Serializer_XML_Attribute_Value.c_str());
					contentStr = value;
					//LOGI("textNote->m_ComTexts.size() %d",textNote->m_ComTexts.size());
					if (seauenceNote->m_ComTexts.size() > 0)
					{
						CText * tempText = new CText;
						tempText->SetText(value);
						seauenceNote->m_ComTexts[0]->AddCText(tempText);
					}
					else
					{
						//	LOGI("textNote->m_ComTexts pushback");
						ComText * tempComeText = new ComText;
						CText * tempText = new CText;
						tempText->SetText(value);
						tempComeText->AddCText(tempText);
						seauenceNote->m_ComTexts.push_back(tempComeText);
						//	LOGI("textNote->m_ComTexts.size() %d",textNote->m_ComTexts.size());
					}

					XMLElement* xmlLocaltion = xmlText->FirstChildElement(Serializer_XML_Element_Location.c_str());
					string localtion = "1.222 1.222 1.222";
					xmlLocaltion->GetText();

					if (!xmlLocaltion->GetText())
					{
						localtion = "1.222 1.222 1.222";
					}
					else
					{
						localtion = xmlLocaltion->GetText();
					}
					//LOGI("xmlLocaltion get text %s", xmlLocaltion->GetText());
					vector<string> floats = StringHelper::Split(localtion, " ");

					if (floats.size() == 3)
					{
						float x = StringHelper::StringToFloat(floats[0]);
						float y = StringHelper::StringToFloat(floats[1]);
						float z = StringHelper::StringToFloat(floats[2]);

						Vector3 tempV(x, y, z);
						//textNote->SetTextsPos(tempV);
					}
				}
			}
		}
	}
	//LOGI("End compositeTexts");

	//LOGI("Begin xmlAttributes");
	//解析Attributes元素及子元素
	map<string, string> attributeList;
	XMLElement* xmlAttributes = xmlNote->FirstChildElement(Serializer_XML_Element_Attributes.c_str());
	if (xmlAttributes)
	{
		XMLElement* xmlAttribute = xmlAttributes->FirstChildElement(Serializer_XML_Element_Attribute.c_str());
		while (xmlAttribute)
		{
			string key = xmlAttribute->Attribute(Serializer_XML_Attribute_Key.c_str());
			string value = xmlAttribute->Attribute(Serializer_XML_Attribute_Value.c_str());
			seauenceNote->SetProperty(key, value);
			//attributeList.insert (make_pair(key,value));
			xmlAttribute = xmlAttribute->NextSiblingElement(Serializer_XML_Element_Attribute.c_str());
		}
	}
	//LOGI("End xmlAttributes");

	float pointSize = 0.8;
	Line3D * line1 = new Line3D(startPnt, endPnt); //引线
	line1->SetName("TextImageLeader");
	Color line1Color = Color(0.03, 0.38, 0.73); //默认颜色
	line1->SetColor(line1Color);
	Point * point1 = new Point(startPnt);
	point1->SetDrawType(1);
	point1->SetSize(pointSize);
	Point * point2 = new Point(endPnt);
	point2->SetDrawType(1);
	point2->SetSize(pointSize);
	std::vector<Texts2D*> textsArray;

	Texts2D * title = new Texts2D;
	title->m_texts = "内容";
	title->m_size = fSize;
	textsArray.push_back(title);
	Texts2D * contents = new Texts2D;
	contents->m_texts = contentStr;
	contents->m_size = fSize;
	textsArray.push_back(contents);

	ImageBoard * imageBord = MeasureDisplayHelper::CreateSequenceNumberImage(scene, contents, endPnt);
	LOGI("ImageBoard pointer %p", imageBord);

	seauenceNote->AddLine(line1);
	seauenceNote->AddPoint(point1);
	//textNote->AddPoint(point2);
	seauenceNote->AddImage(imageBord);

	seauenceNote->SetFrontShow(false); //设置是否前端显示
	if (seauenceNote)
	{
		AddNoteToScene(scene, seauenceNote);
	}
	LOGI("NoteFactory::CreateSequenceNoteFromXMLElement END");
	return seauenceNote;
}

string NoteFactory::SequenceNoteToXMLElement(SceneManager* scene, SequenceNumberNote* sequenceNote)
{
	LOGI("begin serialize");
	//序列化
	string xmlElement = "";

	XMLDocument pDoc; //创建文档
	//创建根节点
	XMLElement *pSequenceNote = pDoc.NewElement(Serializer_XML_Element_SequenceNote.c_str());
	pDoc.LinkEndChild(pSequenceNote);
	pSequenceNote->SetAttribute(Serializer_XML_Attribute_ID.c_str(), (int)sequenceNote->GetID());
	{
		//Leaders节点
		XMLElement * pLeaders = pDoc.NewElement(Serializer_XML_Element_Leaders.c_str());
		pSequenceNote->LinkEndChild(pLeaders);
		{
			XMLElement * pLeader = pDoc.NewElement(Serializer_XML_Element_Leader.c_str());
			pLeader->SetAttribute(Serializer_XML_Attribute_Type.c_str(), 1);
			pLeaders->LinkEndChild(pLeader);

			{
				//创建Polyline
				XMLElement * pPolyline = pDoc.NewElement(Serializer_XML_Element_Polyline.c_str());
				pLeader->LinkEndChild(pPolyline);
				pPolyline->SetAttribute(Serializer_XML_Attribute_ID.c_str(), 0); //TODO:
				pPolyline->SetAttribute(Serializer_XML_Attribute_Type.c_str(), 1); //TODO:
				pPolyline->SetAttribute(Serializer_XML_Attribute_Min.c_str(), 0); //TODO:
				pPolyline->SetAttribute(Serializer_XML_Attribute_Max.c_str(), 0); //TODO:
				{
					//Line3D *line  = new Line3D(textNote->GetNotePos(),textNote->GetTextsPos());
					//创建Point
					{
						XMLElement * pPoint = pDoc.NewElement(Serializer_XML_Element_Point.c_str());
						pPolyline->LinkEndChild(pPoint);
						pPoint->SetAttribute(Serializer_XML_Attribute_X.c_str(), sequenceNote->GetNotePos().m_x); //TODO
						pPoint->SetAttribute(Serializer_XML_Attribute_Y.c_str(), sequenceNote->GetNotePos().m_y); //TODO
						pPoint->SetAttribute(Serializer_XML_Attribute_Z.c_str(), sequenceNote->GetNotePos().m_z); //TODO
					}

					//创建Point
					{
						XMLElement * pPoint = pDoc.NewElement(Serializer_XML_Element_Point.c_str());
						pPolyline->LinkEndChild(pPoint);
						pPoint->SetAttribute(Serializer_XML_Attribute_X.c_str(), sequenceNote->GetTextsPos().m_x); //TODO
						pPoint->SetAttribute(Serializer_XML_Attribute_Y.c_str(), sequenceNote->GetTextsPos().m_y); //TODO
						pPoint->SetAttribute(Serializer_XML_Attribute_Z.c_str(), sequenceNote->GetTextsPos().m_z); //TODO
					}

				}
			}
//			}
		}
		{
			//CompositeTexts
			XMLElement * pCompositeTexts = pDoc.NewElement(Serializer_XML_Element_CompositeTexts.c_str());
			pSequenceNote->LinkEndChild(pCompositeTexts);
			if (sequenceNote->m_ComTexts.size() > 0)
			{
				//CompositeText
				XMLElement * pCompositeText = pDoc.NewElement(Serializer_XML_Element_CompositeText.c_str());
				pCompositeTexts->LinkEndChild(pCompositeText);
				{
					//Texts
					XMLElement * pTexts = pDoc.NewElement(Serializer_XML_Element_Texts.c_str());
					pCompositeText->LinkEndChild(pTexts);
					{
						//Text
						XMLElement * pText = pDoc.NewElement(Serializer_XML_Element_Text.c_str());
						pTexts->LinkEndChild(pText);
						pText->SetAttribute(Serializer_XML_Attribute_Value.c_str(),
								sequenceNote->m_ComTexts.at(0)->GetCText(0)->GetText().c_str());
						{
							//Location
							{
								XMLElement * pLocation = pDoc.NewElement(Serializer_XML_Element_Location.c_str());
								pText->LinkEndChild(pLocation);

								string temstr = M3DTOOLS::floatToString(sequenceNote->GetNotePos().m_x) + " "
										+ M3DTOOLS::floatToString(sequenceNote->GetNotePos().m_y) + " "
										+ M3DTOOLS::floatToString(sequenceNote->GetNotePos().m_z);
								tinyxml2::XMLText * pContent = pDoc.NewText(temstr.c_str());
								pLocation->LinkEndChild(pContent);

							}
						}
					}
				}
			}
		}

		{
			XMLElement * pAttributes = pDoc.NewElement(Serializer_XML_Element_Attributes.c_str());
			pSequenceNote->LinkEndChild(pAttributes);
			for (map<string, string>::iterator it = sequenceNote->GetPropertyMap().begin();
					it != sequenceNote->GetPropertyMap().end(); it++)
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

}

