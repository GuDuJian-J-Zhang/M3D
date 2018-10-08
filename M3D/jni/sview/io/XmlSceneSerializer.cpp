/*
 * XmlHelper.cpp
 *
 *  Created on: 2014-1-7
 *      Author: bing
 */

#include "sview/io/XmlSceneSerializer.h"

#include <stdio.h>
#include <algorithm>
#include <cctype>
#include "m3d/model/ModelView.h"
#include "m3d/base/Vector3.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/graphics/MovedMatrixInfo.h"
#include <string>
#include "m3d/model/Note.h"
#include "m3d/model/PolyLine.h"
#include "m3d/model/Line3D.h"
#include "m3d/model/CText.h"
#include "m3d/model/ComText.h"
#include <assert.h>
#include "m3d/graphics/CameraNode.h"
#include "m3d/extras/note/TextNote.h"
#include "m3d/utils/M3DTools.h"
#include "sview/views/Parameters.h"

namespace SVIEW {

XmlSceneSerializer::XmlSceneSerializer() {
	m_Doc = NULL;
}

XmlSceneSerializer::~XmlSceneSerializer() {
	if (m_Doc != NULL) {
		delete m_Doc;
		m_Doc = NULL;
	}
}

bool XmlSceneSerializer::CreateDocument() {
	LOGI("XmlSceneSerializer::CreateDocument()");
	m_Doc = new XMLDocument();

	//SVL
	XMLElement* svlElement = m_Doc->NewElement("SVL");
	m_Doc->InsertEndChild(svlElement);

	return true;
}

bool XmlSceneSerializer::CreateModel() {
	LOGI("XmlSceneSerializer::CreateModel()");

	//SVL
	XMLElement* svlElement = m_Doc->FirstChildElement("SVL");
	assert(svlElement != NULL);

	//Model
	XMLElement* modelElement = m_Doc->NewElement("Model");
	svlElement->InsertEndChild(modelElement);

	//Notes
	XMLElement* notesElement = m_Doc->NewElement("Notes");
	modelElement->InsertEndChild(notesElement);

	//Views
	XMLElement* viewsElement = m_Doc->NewElement("Views");
	modelElement->InsertEndChild(viewsElement);

	return true;
}

/**
 * @note 加载xml文件到doc。
 */
bool XmlSceneSerializer::Load(string& xmlPath, bool createIfNotFound) {
	bool success = false;
	m_Doc = new XMLDocument();
	tinyxml2::XMLError xmlerror = m_Doc->LoadFile(xmlPath.c_str());

	if (xmlerror == tinyxml2::XML_SUCCESS) {
		success = true;
	} else {
		LOGI("XmlSceneSerializer::Load errid:%d", xmlerror);
		delete m_Doc;
		m_Doc = NULL;
	}

	return success;
}
string XmlSceneSerializer::LoadStringFromModelView() {
	tinyxml2::XMLPrinter printer;
	m_Doc->Accept(&printer);
	std::string xmltext = printer.CStr();
	return xmltext;
}

ModelView * XmlSceneSerializer::LoadModelViewFromXml(string &xmlString) {
	ModelView* curView = NULL;
	const char* xml = xmlString.c_str();
	XMLDocument* myDocument = new XMLDocument();
	if (myDocument->Parse(xml) == tinyxml2::XML_SUCCESS) {
		LOGI("XML_SUCCESS");
		//SVL
		XMLElement* svlElement = myDocument->FirstChildElement("SVL");
		assert(svlElement !=NULL);
//    	    LOGI("SVL");
		//Model
		XMLElement* modelElement = svlElement->FirstChildElement("Model");
		assert(modelElement !=NULL);
//    	    LOGI("Model");
		//Views
		XMLElement* viewsElement = modelElement->FirstChildElement("Views");
		assert(viewsElement !=NULL);
//    	    LOGI("Views");
		XMLElement* curViewElement = viewsElement->FirstChildElement("View");
//    	    LOGI("View");
		if (curViewElement != NULL) {
			curView = GetViewFromElement(curViewElement);
//    	        LOGI("curView: %d",curView->GetID());
		}
	} else {
		LOGE("XmlSceneSerializer::view file format error !!!");
	}

	delete myDocument;

	return curView;
}
bool XmlSceneSerializer::Save(string& xmlPath) {

	//LOGI("save Error");
	bool ret = false;
	if (m_Doc != NULL) {

		//如果未找到目录，逐级创建
		int lastSpliterPos = xmlPath.find_last_of('\\');
		if (lastSpliterPos == -1) {
			lastSpliterPos = xmlPath.find_last_of('/');
		}
		if (lastSpliterPos != -1) {
			string xmlPathDir = xmlPath.substr(0, lastSpliterPos);

			//创建目录
			CreateMulityPath(xmlPathDir.c_str());
		}

		m_Doc->SaveFile(xmlPath.c_str());
		int errorID = m_Doc->ErrorID();
		if (errorID == 0) {
			ret = true;
		} else {
			LOGE("save xml file err:%d %s", errorID, m_Doc->GetErrorStr1());
		}
	}

	return ret;
}

bool XmlSceneSerializer::CreateViews(vector<ModelView *>* viewList) {
	bool ret = false;
	assert(m_Doc != NULL);

	//SVL
	XMLElement* svlElement = m_Doc->FirstChildElement("SVL");
	assert(svlElement !=NULL);

	//Model
	XMLElement* modelElement = svlElement->FirstChildElement("Model");
	assert(modelElement !=NULL);

	//Viewsw'w'w'w
	XMLElement* viewsElement = modelElement->FirstChildElement("Views");
	assert(viewsElement !=NULL);

	vector<float> floatList;
	char outStr[BUFSIZ] = { 0 };
	memset(outStr, 0, BUFSIZ);

	for (int vIndex = 0; vIndex < viewList->size(); vIndex++) {
		ModelView *pView = viewList->at(vIndex);
		assert(viewsElement !=NULL);

		LOGI(
				"XmlSceneSerializer::CreateViews: %s %d,%d", pView->GetName().c_str(), pView->GetID(), pView->GetViewType());
		//View
		XMLElement* viewElement = m_Doc->NewElement("View");
		viewElement->SetAttribute("ID", pView->GetID());
		viewElement->SetAttribute("Name", pView->GetName().c_str());
		bool needSave = false;
		memset(outStr, 0, BUFSIZ);
		switch (pView->GetViewType()) {
		case ModelView::DefaultView:
			needSave = true;
			sprintf(outStr, "%s", "DefaultView");
			break;
		case ModelView::OrignalView:
			sprintf(outStr, "%s", "OrignalView");
			break;
		case ModelView::UserView:
			needSave = true;
			sprintf(outStr, "%s", "UserView");
			break;
		}
		if (!needSave) {
			continue;
		}

		viewElement->SetAttribute("Type", outStr);
		viewsElement->InsertEndChild(viewElement);
		memset(outStr, 0, BUFSIZ);
		if (Parameters::Instance()->m_viewRecordCamera) {
			//获得视图中的相机参数？
			const CameraNode &camera_info = pView->GetCamera();
			//准备相机节点
			XMLElement* cameraElement = m_Doc->NewElement("Camera");
			viewElement->InsertEndChild(cameraElement);

			Vector3 OriginPoint = camera_info.GetPosition();
			XmlSceneSerializer::StringFromPoint(OriginPoint, outStr);
			//cameraElement->SetAttribute("OriginPoint", outStr);
			cameraElement->SetAttribute("Position", outStr);
			//LOGI("Position:%s", outStr);

			const Quaternion &rotation = camera_info.GetRotation();
			memset(outStr, 0, BUFSIZ);
			floatList.clear();
			floatList.push_back(rotation.m_x);
			floatList.push_back(rotation.m_y);
			floatList.push_back(rotation.m_z);
			floatList.push_back(rotation.m_w);
			XmlSceneSerializer::StringFromFloatList(floatList, outStr);
			cameraElement->SetAttribute("Rotation", outStr);
			//		LOGI("Rotation:%s", outStr);

			Vector3 UpVec = camera_info.GetUp();
			memset(outStr, 0, BUFSIZ);
			XmlSceneSerializer::StringFromPoint(UpVec, outStr);
			cameraElement->SetAttribute("UpVector", outStr);
			cameraElement->SetAttribute("ZoomFactor", camera_info.GetZoom());
			cameraElement->SetAttribute("Orthographic",
					camera_info.IsOrthographic());
			cameraElement->SetAttribute("NearClip", camera_info.GetNearClip());
			cameraElement->SetAttribute("FarClip", camera_info.GetFarClip());
			cameraElement->SetAttribute("FOV", camera_info.GetFov());

			//		LOGI("UpVector:%s", outStr);
			//		LOGI("ZoomFactor:%f", camera_info.GetZoom());
			//		LOGI("Orthographic:%d", camera_info.IsOrthographic());
			//		LOGI("NearClip:%f", camera_info.GetNearClip());
			//		LOGI("FarClip:%f", camera_info.GetFarClip());
			//		LOGI("FOV:%f", camera_info.GetFov());

			//        //StateMatrix
			//        vector<float> floatList(camera_info.state_matrix.Data(),
			//            camera_info.state_matrix.Data() + 16);
			//        memset(outStr, 0, BUFSIZ);
			//        XmlSceneSerializer::StringFromFloatList(floatList, outStr);
			//        cameraElement->SetAttribute("StateMatrix", outStr);

			//TODO:lights
			XMLElement* lightsElement = m_Doc->NewElement("Lights");
			viewsElement->InsertEndChild(lightsElement);
		}
		//TODO:backgroundColor
		XMLElement* backgroundColorElement = m_Doc->NewElement(
				"BackgroundColor");
		viewElement->InsertEndChild(backgroundColorElement);

		//Notes
		if (Parameters::Instance()->m_viewRecordNotes) {
			XMLElement* noteElement = m_Doc->NewElement("Notes");
			viewElement->InsertEndChild(noteElement);

			if (noteElement != NULL) {
				this->CreateGestureNote(noteElement, pView);
			}
			map<SHAPETYPE, vector<string> > & noteDataMap = pView->GetNoteMap();
			LOGI("noteMap size %d", noteDataMap.size());
			for (map<SHAPETYPE, vector<string> >::iterator it =
					noteDataMap.begin(); it != noteDataMap.end(); it++) {
				vector<string> &noteDataList = it->second;
				//			assert(noteDataList != NULL);

				for (int i = 0; i < noteDataList.size(); i++) {
					LOGE("step 1");
					LOGI("noteDataList.size()", noteDataList.size());
					//			XMLText * noteText = m_Doc->NewText(noteDataList.at(i).c_str());
					//			noteElement->LinkEndChild(noteText);
					//
					const char* xml = noteDataList.at(i).c_str();
					XMLDocument doc;
					doc.Parse(xml);

					XMLNode * subNode = Clone(doc.RootElement(), m_Doc);
					noteElement->LinkEndChild(subNode);
				}

				//tinyxml2::XMLPrinter priter;
				//noteElement->Accept(&priter);
				//LOGE("noteElement  :  %s", priter.CStr());
			}

			//			noteTexts += noteDataList.at(i);
			//			XMLElement* noteElement = new XMLElement(*(doc->RootElement()));
			//

			//		string endNote = noteDataList.at(noteDataList.size()-1);
			//		endNote = endNote.substr(0,endNote.find_last_of('/')-1);
			//		noteTexts +=endNote;
			//		noteElement->SetValue(noteTexts.c_str());

			//		vector<int> noteIdList = pView->GetNoteList();
			//		XMLElement* noteElement = m_Doc->NewElement("Notes");
			//		viewElement->InsertEndChild(noteElement);
			//
			//		if (noteIdList.size() > 0) {
			//			memset(outStr, 0, BUFSIZ);
			//			XmlSceneSerializer::StringFromIntList(noteIdList, outStr);
			//			XMLText * noteText = m_Doc->NewText(outStr);
			//			noteElement->LinkEndChild(noteText);
			//			//noteElement->SetValue(outStr);
			//		}
		}

		//PMI IDs
		XMLElement* pmiElement = m_Doc->NewElement("PMIs");
		viewElement->InsertEndChild(pmiElement);
		//Explosive
		XMLElement* explosiveElement = m_Doc->NewElement("Explosive");

			int direction = pView->getExplosiveType();
			int percent = pView->getExplosivePercent();
			explosiveElement->SetAttribute("Direction", direction);
			explosiveElement->SetAttribute("Percent", percent);

		viewElement->InsertEndChild(explosiveElement);

		vector<int> pmiIdList = pView->GetPMIList();
		if (pmiIdList.size() > 0) {
			memset(outStr, 0, BUFSIZ);
			XmlSceneSerializer::StringFromIntList(pmiIdList, outStr);
			pmiElement->SetValue(outStr);
		}

		//SectionPlanes
		XMLElement* sectionPlanesElement = m_Doc->NewElement("SectionPlanes");
		viewElement->InsertEndChild(sectionPlanesElement);

		sectionPlanesElement->SetAttribute("Direction",
				pView->GetSectionPlaneDirection());
		sectionPlanesElement->SetAttribute("Percent",
				pView->GetSectionPlanePercentage());
		sectionPlanesElement->SetAttribute("ShowSectionPlane",
				pView->GetShowCutSectionPlane());
		sectionPlanesElement->SetAttribute("ShowCappingPlane",
				pView->GetShowSectionCappingPlane());
//		vector<int> spIdList = pView->GetSectionPlaneIDList();
//		if (spIdList.size() > 0)
//		{
//			memset(outStr, 0, BUFSIZ);
//			XmlSceneSerializer::StringFromIntList(spIdList, outStr);
//			sectionPlanesElement->SetValue(outStr);
//		}

		if (Parameters::Instance()->m_viewRecordModel) {
			//instances
			LOGI("Instances %d", pView->GetInstanceAttributeMap().size());
			XMLElement* instancesElement = m_Doc->NewElement("Instances");
			viewElement->InsertEndChild(instancesElement);

			for (map<int, InstanceAttribute>::const_iterator it =
					pView->GetInstanceAttributeMap().begin();
					it != pView->GetInstanceAttributeMap().end(); it++) {
				//MovedMatrixInfo* mi = &it->second;//todo:保存id，修改视图类结构存储instanceAtt属性？
				InstanceAttribute ia = it->second;
				//id etc
				XMLElement* instanceElement = m_Doc->NewElement("Instance");
				instancesElement->InsertEndChild(instanceElement);
				instanceElement->SetAttribute("ID", ia.id);
				instanceElement->SetAttribute("Path", ia.path.c_str());
				instanceElement->SetAttribute("Visible", ia.visible);
				instanceElement->SetAttribute("HasColor", ia.hasColor);
				floatList.clear();
				floatList.assign(ia.insColor.Data(), ia.insColor.Data() + 4);
				memset(outStr, 0, BUFSIZ);
				XmlSceneSerializer::StringFromFloatList(floatList, outStr);
				instanceElement->SetAttribute("Color", outStr);
				instanceElement->SetAttribute("MaterialID", ia.materialId);

				//PlaceMatrix
				floatList.assign(ia.placeMatrix.Data(),
						ia.placeMatrix.Data() + 16);
				memset(outStr, 0, BUFSIZ);
				XmlSceneSerializer::StringFromFloatList(floatList, outStr);
				instanceElement->SetAttribute("PlaceMatrix", outStr);

				//			LOGI("ID:%d", ia.id);
				//			LOGI("Path:%s", ia.path.c_str());
				//			LOGI("Visible:%b", ia.visible);
				//			LOGI("Color:%f %f %f %f", ia.insColor.m_r,ia.insColor.m_g,ia.insColor.m_b,ia.insColor.m_a);
				//			LOGI("MaterialID:%d", ia.materialId);
				//			//LOGI("PlaceMatrix:%d", ia.id);
				//			LOGI("PlaceMatrix:");
				//			LogFloatArray(floatList.data(),floatList.size());

			}
		}
	}

//LOGI("XmlHelper::SaveModelViewXML end viewId:%d", pView->GetID());

	return ret;
}

void XmlSceneSerializer::CreateNote(Note *pNote) {
	if (NULL == pNote) {
		LOGE("Note is Null!");
		return;
	}

	if (m_Doc == NULL) {
		LOGE("m_Doc is NULL!");
		return;
	}

	LOGI("XmlHelper::SaveNote Id:%d", pNote->GetID());

	//查找Note节点，没有则创建
	bool foundCurrentObject = false;
	//SVL
	XMLElement* svlElement = m_Doc->FirstChildElement("SVL");
	assert(svlElement !=NULL);

	//Model
	XMLElement* modelElement = svlElement->FirstChildElement("Model");
	assert(modelElement !=NULL);

	//Notes
	XMLElement* notesElement = modelElement->FirstChildElement("Notes");
	assert(notesElement !=NULL);

	//不管有没有Note，只往里增加一个Note
	XMLElement* curNoteElement = NULL;

	//TODO:根据note类型处理
//    switch (pNote->GetNoteType())
//      {
//    case Note::Undefine:
//      break;
//    case Note::TextNote:

	if (SHAPE_TEXT_NOTE == pNote->GetType())
		curNoteElement = CreateTextNote(pNote);
//
//      break;
//    default:
//      break;
//      }

	if (curNoteElement != NULL) {
		notesElement->InsertEndChild(curNoteElement);
	}

	LOGI("XmlHelper::SaveNote end Id:%d", pNote->GetID());
}

XMLElement*
XmlSceneSerializer::CreateTextNote(Note *pNote) {

	LOGE("XmlSceneSerializer::CreateTextNote begin");
	char outStr[BUFSIZ] = { 0 };
	memset(outStr, 0, BUFSIZ);
	XMLElement *curNoteElement = m_Doc->NewElement("TextNote");

	sprintf(outStr, "%d", pNote->GetID());
	curNoteElement->SetAttribute("ID", outStr); //pNote->GetNoteID().c_str());
	string createDate("");
	curNoteElement->SetAttribute("Created", createDate.c_str());
	curNoteElement->SetAttribute("IsParallelScreen", false);
	curNoteElement->SetAttribute("IsFix", false);
	curNoteElement->SetAttribute("Visible", true);

//              XMLElement *projectMatrixElement = m_Doc->NewElement("ProjectMatrix");
//              curNoteElement->InsertEndChild(projectMatrixElement);

	XMLElement *colorElement = m_Doc->NewElement("Color");
	Color textColor(0.0f, 0.0f, 1.0f, 1.0f);
	memset(outStr, 0, BUFSIZ);
	StringFromColor(textColor, outStr);
	colorElement->SetAttribute("Value", "#0000FF");

	//Leaders 对应如下格式
	/*
	 <Leaders>
	 <Leader Type="1">
	 <Terminator Type="4" Width="3.3600" Height="1.1200">
	 <Location X="0.000000" Y="-0.000002" Z="0.000000" />
	 <Direction X="0.724472" Y="0.689304" Z="0.000000" />
	 </Terminator>
	 <Polyline ID="0" Type="1" Min="0.000000" Max="0.000000">
	 <Point X="0.000000" Y="-0.000002" Z="0.000000" />
	 <Point X="28.721828" Y="27.327566" Z="0.000000" />
	 </Polyline>
	 </Leader>
	 </Leaders>
	 * */
	XMLElement* leadersElement = m_Doc->NewElement("Leaders");
	curNoteElement->InsertEndChild(leadersElement);
	XMLElement *curLeaderElement = m_Doc->NewElement("Leader");
	leadersElement->InsertEndChild(curLeaderElement);
	curLeaderElement->SetAttribute("Type", 1);

	//没用到
	XMLElement *terminatorElement = m_Doc->NewElement("Terminator");
	terminatorElement->SetAttribute("Type", 4);
	terminatorElement->SetAttribute("Width", 3.36);
	terminatorElement->SetAttribute("Height", 1.12);

	//没用到
	XMLElement *locationElement = m_Doc->NewElement("Location");
	terminatorElement->InsertEndChild(locationElement);
	locationElement->SetAttribute("X", 0);
	locationElement->SetAttribute("Y", 0);
	locationElement->SetAttribute("Z", 0);

	//没用到
	XMLElement *directionElement = m_Doc->NewElement("Direction");
	terminatorElement->InsertEndChild(directionElement);
	directionElement->SetAttribute("X", 0);
	directionElement->SetAttribute("Y", 0);
	directionElement->SetAttribute("Z", 0);

	XMLElement *polylineElement = m_Doc->NewElement("Polyline");
	curLeaderElement->InsertEndChild(polylineElement);

	polylineElement->SetAttribute("ID", 0);
	polylineElement->SetAttribute("Type", 1);
	polylineElement->SetAttribute("Min", 0.0f);
	polylineElement->SetAttribute("Max", 0.0f);

//	for (vector<PolyLine*>::iterator itPolyLine = pNote->m_PolyLineList.begin();
//			itPolyLine != pNote->m_PolyLineList.end(); itPolyLine++) {
//		for (vector<Vector3>::iterator itPoint =
//				(*itPolyLine)->GetPointList().begin();
//				itPoint != (*itPolyLine)->GetPointList().end(); itPoint++) {
//			XMLElement *pointElement = m_Doc->NewElement("Point");
//			pointElement->SetAttribute("X", itPoint->m_x);
//			pointElement->SetAttribute("Y", itPoint->m_y);
//			pointElement->SetAttribute("Z", itPoint->m_z);
//			polylineElement->InsertEndChild(pointElement);
//		}
//	}

	//创建Point
	{
		XMLElement * pPoint = m_Doc->NewElement("Point");
		polylineElement->LinkEndChild(pPoint);
		pPoint->SetAttribute("X", ((TextNote*) pNote)->GetNotePos().m_x); //TODO
		pPoint->SetAttribute("Y", ((TextNote*) pNote)->GetNotePos().m_y); //TODO
		pPoint->SetAttribute("Z", ((TextNote*) pNote)->GetNotePos().m_z); //TODO
	}

	//创建Point
	{
		XMLElement * pPoint = m_Doc->NewElement("Point");
		polylineElement->LinkEndChild(pPoint);
		pPoint->SetAttribute("X", ((TextNote*) pNote)->GetTextsPos().m_x); //TODO
		pPoint->SetAttribute("Y", ((TextNote*) pNote)->GetTextsPos().m_y); //TODO
		pPoint->SetAttribute("Z", ((TextNote*) pNote)->GetTextsPos().m_z); //TODO

	}

	//ExtendLines
	XMLElement *extendLinesElement = m_Doc->NewElement("ExtendLines");
	curNoteElement->InsertEndChild(extendLinesElement);

	//CompositeTexts 对应如下格式
	/*
	 *       <CompositeTexts>
	 <CompositeText>
	 <Texts>
	 <Text Value="3D 批注：带引线">
	 <TextStyle>
	 <CharHeight>3.500000</CharHeight>
	 <LineSpacing>1.100000</LineSpacing>
	 <Font Name="" />
	 </TextStyle>
	 <Location>28.721828 27.327566 0.000000</Location>
	 <Rotation> 1.000000 0.000000 0.000000 0.000000 1.000000 0.000000 </Rotation>
	 </Text>
	 </Texts>
	 <OuterBox>1.000000 1.000000 1.000000-1.000000 -1.000000 -1.000000</OuterBox>
	 </CompositeText>
	 </CompositeTexts>
	 * */
	XMLElement* comTextsElement = m_Doc->NewElement("CompositeTexts");
	curNoteElement->InsertEndChild(comTextsElement);
	LOGE("pNote->m_ComTexts.size() %d", pNote->m_ComTexts.size());
	for (int iComText = 0; iComText < pNote->m_ComTexts.size(); iComText++) {
		ComText *curComText = pNote->m_ComTexts.at(iComText);
		XMLElement *comTextElement = m_Doc->NewElement("CompositeText");
		comTextsElement->InsertEndChild(comTextElement);

		XMLElement *textsElement = m_Doc->NewElement("Texts");
		comTextElement->InsertEndChild(textsElement);

		LOGI("texts count:%d", curComText->GetCTextList().size());

		for (int iCText = 0; iCText < curComText->GetCTextList().size();
				iCText++) {
			CText *curText = curComText->GetCTextList().at(iCText);
			XMLElement *curTextElement = m_Doc->NewElement("Text");
			textsElement->InsertEndChild(curTextElement);

			curTextElement->SetAttribute("Value", curText->GetText().c_str());

			XMLElement *textStyleElement = m_Doc->NewElement("TextStyle");
			curTextElement->InsertEndChild(textStyleElement);
			{
				float charHeight, lineSpacing;
				curText->GetCharWidthHeight(charHeight, lineSpacing);

				XMLElement *charHeightElement = m_Doc->NewElement("CharHeight");
				textStyleElement->InsertEndChild(charHeightElement);
				memset(outStr, 0, BUFSIZ);
				sprintf(outStr, "%f", charHeight);

				float width, height;
				curText->GetCharWidthHeight(width, height);
				//outStr=M3DTOOLS::floatToString(	width).c_str();//TODO

				XMLText *charHeightTextValueXMLText = m_Doc->NewText(
						M3DTOOLS::floatToString(width).c_str()); //TODO
				charHeightElement->InsertEndChild(charHeightTextValueXMLText);

				XMLElement *lineSpacingElement = m_Doc->NewElement(
						"LineSpacing");
				textStyleElement->InsertEndChild(lineSpacingElement);
				memset(outStr, 0, BUFSIZ);
				sprintf(outStr, "%f", lineSpacing);
				XMLText *lineSpacingTextValueXMLText = m_Doc->NewText(outStr);
				lineSpacingElement->InsertEndChild(lineSpacingTextValueXMLText);

				XMLElement *fontElement = m_Doc->NewElement("Font");
				textStyleElement->InsertEndChild(fontElement);
				memset(outStr, 0, BUFSIZ);
				curText->GetFontFilePath(outStr);
				fontElement->SetAttribute("Name", outStr);
			}

			Vector3 innerLocPoint = curText->GetInnerLoc();
			Vector3 xAxis, yAxis;
			curText->GetInnerXYAxis(xAxis, yAxis);

			XMLElement *locationElement = m_Doc->NewElement("Location");
			curTextElement->InsertEndChild(locationElement);
			memset(outStr, 0, BUFSIZ);
			StringFromPoint(innerLocPoint, outStr);

			string temstr = M3DTOOLS::floatToString(
					((TextNote*) pNote)->GetNotePos().m_x) + " "
					+ M3DTOOLS::floatToString(
							((TextNote*) pNote)->GetNotePos().m_y) + " "
					+ M3DTOOLS::floatToString(
							((TextNote*) pNote)->GetNotePos().m_z);

			XMLText *locationValueXMLText = m_Doc->NewText(temstr.c_str());
			locationElement->InsertEndChild(locationValueXMLText);

			XMLElement *rotationElement = m_Doc->NewElement("Rotation");
			curTextElement->InsertEndChild(rotationElement);
			memset(outStr, 0, BUFSIZ);
			vector<float> tmpFloatList;
			tmpFloatList.push_back(xAxis.m_x);
			tmpFloatList.push_back(xAxis.m_y);
			tmpFloatList.push_back(xAxis.m_z);
			tmpFloatList.push_back(yAxis.m_x);
			tmpFloatList.push_back(yAxis.m_y);
			tmpFloatList.push_back(yAxis.m_z);

			StringFromFloatList(tmpFloatList, outStr);
			XMLText *rotationValueXMLText = m_Doc->NewText(outStr);
			rotationElement->InsertEndChild(rotationValueXMLText);

		}
	}

	{
		//Attributes
		XMLElement * pAttributes = m_Doc->NewElement("Attributes");
		curNoteElement->LinkEndChild(pAttributes);
		{
			//Attribute
			XMLElement * pAttribute = m_Doc->NewElement("Attribute");
			pAttributes->LinkEndChild(pAttribute);
			pAttribute->SetAttribute("Key", "UserName");
			pAttribute->SetAttribute("Value",
					pNote->GetProperty("UserName").c_str());

		}

		{
			//Attribute
			XMLElement * pAttribute = m_Doc->NewElement("Attribute");

			pAttributes->LinkEndChild(pAttribute);

			pAttribute->SetAttribute("Key", "CreateTime");
			pAttribute->SetAttribute("Value",
					pNote->GetProperty("CreateTime").c_str());
		}

		{
			//Attribute
			XMLElement * pAttribute = m_Doc->NewElement("Attribute");
			pAttributes->LinkEndChild(pAttribute);
			pAttribute->SetAttribute("Key", "Guid");
			pAttribute->SetAttribute("Value",
					pNote->GetProperty("Guid").c_str());
		}

	}

	LOGE("XmlSceneSerializer::CreateTextNote end");

	return curNoteElement;
}

XMLElement*
XmlSceneSerializer::CreateGestureNote(XMLElement* viewElement,
		ModelView*pView) {
	int i = 0;
	char outStr[BUFSIZ] = { 0 };
	memset(outStr, 0, BUFSIZ);
	XMLElement *curNoteElement = m_Doc->NewElement("GestureNote");
	curNoteElement->SetAttribute("ID", "0"); //pNote->GetNoteID().c_str());
	string createDate("");
	curNoteElement->SetAttribute("Created", createDate.c_str());
	curNoteElement->SetAttribute("UserID", "1");
	curNoteElement->SetAttribute("Layer", "0");
	viewElement->InsertEndChild(curNoteElement);

	XMLElement *projectMatrixElement = m_Doc->NewElement("ProjectMatrix");
	curNoteElement->InsertEndChild(projectMatrixElement);

	XMLElement *polylinesElement = m_Doc->NewElement("Polylines");
	curNoteElement->InsertEndChild(polylinesElement);

	while (i < pView->GetGestureNotePolyLines().size()) {

		XMLElement *polylineElement = m_Doc->NewElement("Polyline");
		polylinesElement->InsertEndChild(polylineElement);

		polylineElement->SetAttribute("ID", 0);
		polylineElement->SetAttribute("Type", 1);
		polylineElement->SetAttribute("Min", 0.0f);
		polylineElement->SetAttribute("Max", 0.0f);
		Color textColor = pView->GetGestureNotePolyLineColors().at(i);
		vector<float> floatList;
		floatList.clear();
		floatList.assign(textColor.Data(), textColor.Data() + 4);
		memset(outStr, 0, BUFSIZ);
		XmlSceneSerializer::StringFromFloatList(floatList, outStr);
		polylineElement->SetAttribute("Color", outStr);

		PolyLine polyLine = pView->GetGestureNotePolyLines().at(i);
		vector<Vector3> pointList = polyLine.GetPointList();
		for (int k = 0; k < pointList.size(); k++) {
			Vector3 vector3 = pointList.at(k);
			//创建Point
			{
				vector<float> tmpFloatList;
				tmpFloatList.push_back(vector3.m_x);
				tmpFloatList.push_back(vector3.m_y);
				tmpFloatList.push_back(vector3.m_z);
				StringFromFloatList(tmpFloatList, outStr);
				XMLElement * pPoint = m_Doc->NewElement("Point");
				XMLText *locationValueXMLText = m_Doc->NewText(outStr);
				pPoint->InsertEndChild(locationValueXMLText);
				polylineElement->LinkEndChild(pPoint);
			}
		}

		LOGE("XmlSceneSerializer::CreateTextNote end");

		i++;
	}

	return curNoteElement;
}
//ModelView* XmlSceneSerializer::LoadModelView( int viewId) {
//	LOGI("XmlHelper::LoadModelViewFromXML viewId:%d", viewId);
//
//	ModelView* retView = NULL;
//
//	bool foundView = false;
//	XMLElement* rootElement = XmlSceneSerializer::FindElement(m_Doc, "root", false);
//	XMLElement* viewsElement = XmlSceneSerializer::FindElement(rootElement, "views",
//			false);
//	XMLElement* curViewElement = XmlSceneSerializer::FindElement(viewsElement, "view",
//			false);
//	while (curViewElement != NULL) {
//		int id = curViewElement->IntAttribute("id");
//		if (id != 0 && id == viewId) {
//			foundView = true;
//			break;
//		}
//		curViewElement = curViewElement->NextSiblingElement("view");
//	}
//
//	if (foundView) {
//		retView = getViewFromElement(curViewElement);
//	}
//
//	LOGI("XmlHelper::LoadModelViewFromXML end viewId:%d", viewId);
//	return retView;
//}
void XmlSceneSerializer::CreateNoteStringFromDoc(ModelView * pView,
		XMLElement* viewElement) {
	LOGI("CreateTextNoteStringFromDoc");

	vector<string> noteList;
	XMLElement * notes = viewElement->FirstChildElement("Notes");
	if (notes) {
		LOGI("CreateTextNoteStringFromDoc have notes");
		XMLElement* textnote = notes->FirstChildElement("TextNote");
		while (textnote) {
			LOGI("CreateTextNoteStringFromDoc have textNotes");
//			const char* xml;
//			XMLDocument doc;
//			//doc.Parse(xml);
//			XMLNode * subNode = Clone(textnote, &doc);
//			tinyxml2::XMLPrinter priter;
//			subNode->Accept(&priter);
			tinyxml2::XMLPrinter priter;
			textnote->Accept(&priter);
			string textNoteString = priter.CStr();
			noteList.push_back(textNoteString);
			LOGI("textNoteList element %s", textNoteString.c_str());
			textnote = textnote->NextSiblingElement("TextNote");
		}

		if (noteList.size() > 0)
			pView->SetNoteDataList(SHAPE_TEXT_NOTE, noteList);

		noteList.clear();

		//LOGI("CreateVoiceNoteStringFromDoc have notes");
		XMLElement* voicenote = notes->FirstChildElement("VoiceNote");
		while (voicenote) {
			LOGI("CreateVoiceNoteStringFromDoc have vocieNotes");
			tinyxml2::XMLPrinter priter;
			voicenote->Accept(&priter);
			string voiceNoteString = priter.CStr();
			noteList.push_back(voiceNoteString);
			LOGI("voiceNoteList element %s", voiceNoteString.c_str());
			voicenote = voicenote->NextSiblingElement("VoiceNote");
		}

		if (noteList.size() > 0)
			pView->SetNoteDataList(SHAPE_VOICE_NOTE, noteList);

		noteList.clear();

		XMLElement* sequenceNote = notes->FirstChildElement("SequenceNote");
		while (sequenceNote) {
			LOGI("CreateTextNoteStringFromDoc have sequenceNote");
			tinyxml2::XMLPrinter priter;
			sequenceNote->Accept(&priter);
			string sequenceNoteString = priter.CStr();
			noteList.push_back(sequenceNoteString);
			LOGI(
					"sequenceNoteStringList element %s", sequenceNoteString.c_str());
			sequenceNote = sequenceNote->NextSiblingElement("SequenceNote");
		}

		if (noteList.size() > 0)
			pView->SetNoteDataList(SHAPE_SEQUENCE_NUMBER_NOTE, noteList);
		noteList.clear();

		XMLElement* threeDGestureNote = notes->FirstChildElement(
				"ThreeDGestureNote");
		while (threeDGestureNote) {
			LOGI("CreateTextNoteStringFromDoc have threeDGestureNote");
			tinyxml2::XMLPrinter priter;
			threeDGestureNote->Accept(&priter);
			string threeDGestureNoteString = priter.CStr();
			noteList.push_back(threeDGestureNoteString);
			LOGI(
					" threeDGestureNoteStringList element %s", threeDGestureNoteString.c_str());
			threeDGestureNote = threeDGestureNote->NextSiblingElement(
					"ThreeDGestureNote");
		}

		if (noteList.size() > 0)
			pView->SetNoteDataList(SHAPE_THREED_GESTURE_NOTE, noteList);

		vector<PolyLine> polyLineList;
		vector<Color> colors;
		XMLElement* gestureNote = notes->FirstChildElement("GestureNote");
		while (gestureNote) {
			XMLElement* poylinesElement = gestureNote->FirstChildElement(
					"Polylines");
			assert(poylinesElement !=NULL);

			XMLElement* polylineElement = poylinesElement->FirstChildElement(
					"Polyline");
			while (polylineElement != NULL) {

				vector<float> floatList;
				floatList.clear();
				StringToFloatList(polylineElement->Attribute("Color"),
						floatList);
				Color insColor(floatList[0], floatList[1], floatList[2],
						floatList[3]);
				colors.push_back(insColor);
				PolyLine polyLine = PolyLine();
				XMLElement* pointElement = polylineElement->FirstChildElement(
						"Point");
				while (pointElement != NULL) {
					vector<float> outFloatList;
					outFloatList.clear();
					StringToFloatList(pointElement->GetText(), outFloatList);
					Vector3 vec3(outFloatList[0], outFloatList[1],
							outFloatList[2]);
					polyLine.AddPoint(vec3);
					pointElement = pointElement->NextSiblingElement("Point");
				}
				polyLineList.push_back(polyLine);
				polylineElement = polylineElement->NextSiblingElement(
						"Polyline");
			}
			gestureNote = gestureNote->NextSiblingElement("GestureNote");
		}
		pView->SetGestureNotePolyLines(polyLineList);
		pView->SetGestureNotePolyLineColors(colors);
	}

}
ModelView*
XmlSceneSerializer::GetViewFromElement(XMLElement* viewElement) {
	LOGI("GetViewFromElement");
	vector<float> outFloatList;

	ModelView* retView = new ModelView();
	//retView->SetID(viewElement->IntAttribute("ID"));
	retView->SetName(viewElement->Attribute("Name"));
	string viewTypeStr = viewElement->Attribute("Type");
	ModelView::ViewTypeEnum viewType;
	//Undefine = -1, DefaultView = 0, OrignalView = 1, UserView = 2,
	if (viewTypeStr == "DefaultView") {
		viewType = ModelView::DefaultView;
	} else if (viewTypeStr == "UserView") {
		viewType = ModelView::UserView;
	} else {
		viewType = ModelView::Undefine; //todo:需报错
	}

	retView->SetViewType(viewType);

	XMLElement* cameraElement = viewElement->FirstChildElement("Camera");
	if (cameraElement) {
		CameraNode camera_info;
		Vector3 pos;
		StringToPoint(cameraElement->Attribute("Position"), pos);
		camera_info.SetPosition(pos);

		outFloatList.clear();
		XmlSceneSerializer::StringToFloatList(
				cameraElement->Attribute("Rotation"), outFloatList);
		Quaternion rotation;
		rotation.m_x = outFloatList.at(0);
		rotation.m_y = outFloatList.at(1);
		rotation.m_z = outFloatList.at(2);
		rotation.m_w = outFloatList.at(3);
		camera_info.SetRotation(rotation);

		camera_info.SetZoom(cameraElement->FloatAttribute("ZoomFactor"));
		camera_info.SetOrthographic(
				cameraElement->BoolAttribute("Orthographic"));
		camera_info.SetNearClip(cameraElement->FloatAttribute("NearClip"));
		camera_info.SetFarClip(cameraElement->FloatAttribute("FarClip"));
		camera_info.SetFov(cameraElement->FloatAttribute("FOV"));
		retView->SetCamera(camera_info);
	}

	//noteids TODO 需要将textnote转换为字符串
//	XMLElement* noteRefsElement = viewElement->FirstChildElement("Notes");
//	if (NULL != noteRefsElement) {
//		const char* idsStr = noteRefsElement->GetText();
//		if (idsStr != NULL) {
//			vector<int> noteIdList;
//			StringToIntList(idsStr, noteIdList);
//			retView->SetNoteIds(noteIdList);
//		}
//	}

	//vector<string> noteDataList;
	CreateNoteStringFromDoc(retView, viewElement);

	//PMI ids
	XMLElement* pmiRefsElement = viewElement->FirstChildElement("PMIs");
	if (NULL != pmiRefsElement) {
		const char* idsStr = pmiRefsElement->GetText();
		if (idsStr != NULL) {
			vector<int> pmiIdList;
			StringToIntList(idsStr, pmiIdList);
			retView->SetPMIIds(pmiIdList);
		}
	}

	XMLElement* sectionElement = viewElement->FirstChildElement(
			"SectionPlanes");
	if (NULL != sectionElement) {
		retView->SetSectionPlaneDirection(
				sectionElement->IntAttribute("Direction"));
		retView->SetSectionPlanePercentage(
				sectionElement->FloatAttribute("Percent"));
		retView->SetShowCutSectionPlane(
				sectionElement->BoolAttribute("ShowSectionPlane"));
		retView->SetShowSectionCappingPlane(
				sectionElement->BoolAttribute("ShowCappingPlane"));
	}

	//Explosive
	XMLElement* explosiveElement = viewElement->FirstChildElement("Explosive");
	if (NULL != explosiveElement) {
		vector<int> states;
		int direction = explosiveElement->IntAttribute("Direction");
		int percent = explosiveElement->IntAttribute("Percent");

		LOGI(
				"GetViewFromElement --- direction: %d;percent: %d", direction, percent);
		retView->setExplosiveType(direction);
		retView->setExplosivePercent(percent);

	}

	//Instances
	XMLElement* instancesElement = viewElement->FirstChildElement("Instances");
	if (instancesElement) {
		XMLElement* curInstanceElement = instancesElement->FirstChildElement(
				"Instance");
		while (curInstanceElement != NULL) {
			InstanceAttribute ia;
			ia.id = curInstanceElement->IntAttribute("ID");
			ia.path = curInstanceElement->Attribute("Path");
			ia.visible = curInstanceElement->BoolAttribute("Visible");
			ia.hasColor = curInstanceElement->BoolAttribute("HasColor");
			StringToColor(curInstanceElement->Attribute("Color"), ia.insColor);
			ia.materialId = curInstanceElement->IntAttribute("MaterialID");
			//PlaceMatrix
			outFloatList.clear();
			XmlSceneSerializer::StringToFloatList(
					curInstanceElement->Attribute("PlaceMatrix"), outFloatList);
			ia.placeMatrix.Set(outFloatList.data());
			retView->AddInstanceAttribute(ia.id, ia);
			curInstanceElement = curInstanceElement->NextSiblingElement(
					"Instance");

			//		LOGI("ID:%d", ia.id);
			//		LOGI("Path:%s", ia.path.c_str());
			//		LOGI("Visible:%b", ia.visible);
			//		LOGI("Color:%f %f %f %f", ia.insColor.m_r,ia.insColor.m_g,ia.insColor.m_b,ia.insColor.m_a);
			//		LOGI("MaterialID:%d", ia.materialId);
			//		//LOGI("PlaceMatrix:%d", ia.id);
			//		LOGI("PlaceMatrix:");
			//		LogFloatArray(outFloatList.data(),outFloatList.size());
		}
	}

	return retView;
}

//从doc中获取所有视图对象
void XmlSceneSerializer::LoadAllModelView(vector<ModelView*>* pOutViewList) {
	LOGI("XmlSceneSerializer::LoadAllModelView");
	bool foundView = false;

	//SVL
	XMLElement* svlElement = m_Doc->FirstChildElement("SVL");
	assert(svlElement !=NULL);

	//Model
	XMLElement* modelElement = svlElement->FirstChildElement("Model");
	assert(modelElement !=NULL);

	//Views
	XMLElement* viewsElement = modelElement->FirstChildElement("Views");
	assert(viewsElement !=NULL);

	XMLElement* curViewElement = viewsElement->FirstChildElement("View");

	while (curViewElement != NULL) {
		ModelView* curView = GetViewFromElement(curViewElement);
		pOutViewList->push_back(curView);
		curViewElement = curViewElement->NextSiblingElement("View");
	}
}

Note&
XmlSceneSerializer::GetNoteFromElement(XMLElement* noteElement,
		Note& textNote) {
	LOGI("XmlSceneSerializer::GetNoteFromElement");
	vector<float> outFloatList;

	//textNote.SetType(SHAPETYPE::SHAPE_TEXT_ANNO);//?
	string ID = noteElement->Attribute("ID");
	//textNote.SetNoteID(ID);
	string createDate = noteElement->Attribute("Created");
	bool IsParallelScreen = noteElement->BoolAttribute("IsParallelScreen");
	bool IsFix = noteElement->BoolAttribute("IsFix");
	bool Visible = noteElement->BoolAttribute("Visible");

	XMLElement *projectMatrixElement = noteElement->FirstChildElement(
			"ProjectMatrix");
	outFloatList.clear();
	StringToFloatList(projectMatrixElement->Value(), outFloatList);

	XMLElement *colorElement = noteElement->FirstChildElement("Color");
	string tmpStr = colorElement->Attribute("value");
	Color textColor;
	StringToColor(tmpStr.c_str(), textColor);

//	note.SetID(noteElement->IntAttribute("id"));
//	note.SetName(noteElement->Attribute("name"));

	//Leaders 对应如下格式
	/*
	 <Leaders>
	 <Leader Type="1">
	 <Terminator Type="4" Width="3.3600" Height="1.1200">
	 <Location X="0.000000" Y="-0.000002" Z="0.000000" />
	 <Direction X="0.724472" Y="0.689304" Z="0.000000" />
	 </Terminator>
	 <Polyline ID="0" Type="1" Min="0.000000" Max="0.000000">
	 <Point X="0.000000" Y="-0.000002" Z="0.000000" />
	 <Point X="28.721828" Y="27.327566" Z="0.000000" />
	 </Polyline>
	 </Leader>
	 </Leaders>
	 * */
	XMLElement* leadersElement = noteElement->FirstChildElement("Leaders");
	if (leadersElement != NULL) {
		XMLElement *curLeaderElement = leadersElement->FirstChildElement(
				"Leader");
		while (curLeaderElement != NULL) {
			int leaderType = curLeaderElement->IntAttribute("Type");

			XMLElement *terminatorElement = curLeaderElement->FirstChildElement(
					"Terminator");
			int terminatorType = terminatorElement->IntAttribute("Type");
			float terminatorWidth = terminatorElement->FloatAttribute("Width");
			float terminatorHeight = terminatorElement->FloatAttribute(
					"Height");

			XMLElement *locationElement = terminatorElement->FirstChildElement(
					"Location");
			outFloatList.clear();
			StringToFloatList(locationElement->Value(), outFloatList);
			Vector3 *vLocation = new Vector3(outFloatList.data());

			XMLElement *directionElement = terminatorElement->FirstChildElement(
					"Direction");
			outFloatList.clear();
			StringToFloatList(directionElement->Value(), outFloatList);
			Vector3 *vDirection = new Vector3(outFloatList.data());

			XMLElement *polylineElement = curLeaderElement->FirstChildElement(
					"Polyline");
			XMLElement *curPointElement = polylineElement->FirstChildElement(
					"Point");

			//读取point数组并展开为顶点索引组合
			vector<Vector3> tmpPointList;
			vector<M3D_INDEX_TYPE> tmpIndexList;
			int iTmpIndex = 0;
			while (curPointElement != NULL) {
				outFloatList.clear();
				StringToFloatList(curPointElement->Value(), outFloatList);
				Vector3 tmpPoint(outFloatList[0], outFloatList[1],
						outFloatList[3]);
				tmpPointList.push_back(tmpPoint);
				tmpIndexList.push_back(iTmpIndex);

				iTmpIndex++;
				curPointElement = curPointElement->NextSiblingElement("Point");
			}

			PolyLine *pPolyLine = new PolyLine();
			pPolyLine->SetBuffer(tmpPointList.size() * 3,
					(float*) tmpPointList.data(), tmpIndexList.size(),
					tmpIndexList.data());

			curLeaderElement = leadersElement->NextSiblingElement("Leader");
		}
	}

	//ExtendLines
	XMLElement *extendLinesElement = noteElement->FirstChildElement(
			"ExtendLines");

	//CompositeTexts 对应如下格式
	/*
	 *       <CompositeTexts>
	 <CompositeText>
	 <Texts>
	 <Text Value="3D 批注：带引线">
	 <TextStyle>
	 <CharHeight>3.500000</CharHeight>
	 <LineSpacing>1.100000</LineSpacing>
	 <Font Name="" />
	 </TextStyle>
	 <Location>28.721828 27.327566 0.000000</Location>
	 <Rotation> 1.000000 0.000000 0.000000 0.000000 1.000000 0.000000 </Rotation>
	 </Text>
	 </Texts>
	 <OuterBox>1.000000 1.000000 1.000000-1.000000 -1.000000 -1.000000</OuterBox>
	 </CompositeText>
	 </CompositeTexts>
	 * */
	XMLElement* comTextsElement = noteElement->FirstChildElement(
			"CompositeTexts");
	if (comTextsElement != NULL) {
		int i = 0;
		XMLElement *comTextElement = comTextsElement->FirstChildElement(
				"CompositeText");
		while (comTextElement != NULL) {
			ComText *pComText = new ComText();
			//TODO:pComText->SetIsFaceToCamera(comTextElement->BoolAttribute("isFaceToCamera"));
			XMLElement *textsElement = comTextElement->FirstChildElement(
					"Texts");
			XMLElement *pTextElement = textsElement->FirstChildElement("Text");
			while (pTextElement != NULL) {
				CText *pText = new CText();

				pText->SetText(pTextElement->Attribute("Value"));

				XMLElement *textStyleElement = pTextElement->FirstChildElement(
						"TextStyle");
				XMLElement *textStyleCharHeightElement =
						textStyleElement->FirstChildElement("CharHeight");
				float charWidth = (float) atof(
						textStyleCharHeightElement->Value());
				pText->SetCharWidthHeight(charWidth, charWidth);

				XMLElement *textStyleLineSpacingElement =
						textStyleElement->FirstChildElement("LineSpacing");
				float lineSpacing = atof(textStyleLineSpacingElement->Value());
				pText->SetCharSpacing(lineSpacing);

				XMLElement *textStyleFontElement =
						textStyleElement->FirstChildElement("Font");
				//pText->SetFont(textStyleFontElement->Attribute("Name"));

				XMLElement *textLocElement = pTextElement->FirstChildElement(
						"Location");
				Vector3 innerLocPoint;
				StringToPoint(textLocElement->Value(), innerLocPoint);
				pText->SetInnerLoc(innerLocPoint);

				XMLElement *textRotElement = pTextElement->FirstChildElement(
						"Rotation");
				vector<float> tmpFloatList;
				StringToFloatList(textRotElement->Value(), tmpFloatList);
				Vector3 xAxis(tmpFloatList[0], tmpFloatList[1],
						tmpFloatList[2]);
				Vector3 yAxis(tmpFloatList[3], tmpFloatList[4],
						tmpFloatList[5]);
//				StringToPoint(pTextElement->Attribute("xAxis"),xAxis);
//				StringToPoint(pTextElement->Attribute("yAxis"),yAxis);
				pText->SetInnerXYAxis(xAxis, yAxis);

//
				pComText->AddCText(pText);
				pTextElement = pTextElement->NextSiblingElement("Text");
			}

			textNote.AddText(pComText);
			comTextElement = comTextElement->NextSiblingElement(
					"CompositeText");
			i++;
		}
		LOGI("all ComTexts cnt:%d", i);
	}

	return textNote;
}

XMLNode*
XmlSceneSerializer::Clone(XMLNode* node, XMLDocument* doc) {
	XMLNode* newNode = node->ShallowClone(doc);
	XMLNode* nextChild = node->FirstChild();

	while (nextChild) {
		newNode->LinkEndChild(Clone(nextChild, doc));
		nextChild = nextChild->NextSibling();
	}
	return newNode;
}
/**
 *
 * @param outNoteList
 * @return
 */
vector<Note*>&
XmlSceneSerializer::LoadAllNotes(vector<Note*>& outNoteList) {
	//SVL
	XMLElement* svlElement = m_Doc->FirstChildElement("SVL");
	assert(svlElement !=NULL);

	//Model
	XMLElement* modelElement = svlElement->FirstChildElement("Model");
	assert(modelElement !=NULL);

	//Notes
	XMLElement* notesElement = modelElement->FirstChildElement("Notes");
	assert(modelElement !=NULL);

	XMLElement* curNoteElement = notesElement->FirstChildElement("TextNote");
	while (curNoteElement != NULL) {
		Note *pNote = new Note();
		GetNoteFromElement(curNoteElement, *pNote);
		outNoteList.push_back(pNote);
		curNoteElement = curNoteElement->NextSiblingElement("TextNote");
	}

	return outNoteList;
}

void XmlSceneSerializer::LoadAllGestureNotes(vector<PolyLine>& polyLineList,
		vector<Color>& colors) {
	//SVL
	XMLElement* svlElement = m_Doc->FirstChildElement("SVL");
	assert(svlElement !=NULL);

	//Model
	XMLElement* modelElement = svlElement->FirstChildElement("Model");
	assert(modelElement !=NULL);

	//Notes
	XMLElement* notesElement = modelElement->FirstChildElement("Notes");
	assert(modelElement !=NULL);

	XMLElement* curNoteElement = notesElement->FirstChildElement("GestureNote");
	while (curNoteElement != NULL) {
		XMLElement* poylinesElement = curNoteElement->FirstChildElement(
				"Polylines");
		assert(poylinesElement !=NULL);

		XMLElement* polylineElement = poylinesElement->FirstChildElement(
				"Polyline");
		while (polylineElement != NULL) {
			Color insColor;
			StringToColor(polylineElement->Attribute("Color"), insColor);
			colors.push_back(insColor);
			PolyLine polyLine = PolyLine();
			XMLElement* pointElement = polylineElement->FirstChildElement(
					"Point");
			while (pointElement != NULL) {

				Vector3 vec3;
				StringToPoint(pointElement->GetText(), vec3);
				polyLine.AddPoint(vec3);
				pointElement = pointElement->NextSiblingElement("Point");
			}
			polyLineList.push_back(polyLine);
			polylineElement = polylineElement->NextSiblingElement("Polyline");
		}
		curNoteElement = curNoteElement->NextSiblingElement("GestureNote");
	}
}

vector<Note*>&
XmlSceneSerializer::LoadAllViewNotes(vector<Note*>& outNoteList) {

	return outNoteList;
}

void XmlSceneSerializer::StringFromPoint(M3D::Vector3& point, char* outStr) {
	sprintf(outStr, "%f %f %f", point.m_x, point.m_y, point.m_z);
	//LOGI("StringFromPoint:%s",outStr);
}

void XmlSceneSerializer::StringToPoint(const char* inStr,
		M3D::Vector3& outPoint) {
	const char spliter[] = " ";
	//split str in c
	char str[1000];
	strncpy(str, inStr, sizeof(str));
	char* parts[100];
	unsigned int index = 0;
	parts[index] = strtok(str, spliter);

	while (parts[index] != 0) {
		++index;
		parts[index] = strtok(0, spliter);
	}

	if (parts[2] != 0) {
		outPoint.m_x = atof(parts[0]);
		outPoint.m_y = atof(parts[1]);
		outPoint.m_z = atof(parts[2]);
	}
}

void XmlSceneSerializer::StringFromIntList(vector<int>& intList, char* outStr) {
	for (int i = 0; i < intList.size(); i++) {
		int id = intList.at(i);
		if (i == 0) {
			sprintf(outStr, "%d", id);
		} else {
			sprintf(outStr, "%s %d", outStr, id);
		}
	}

	//LOGI("XmlHelper::intListToString :%s", outStr);
}

vector<int>&
XmlSceneSerializer::StringToIntList(const char* intListStr,
		vector<int>& outIntList) {
	LOGI("StringToIntList:'%s'", intListStr);
	const char spliter[] = " ";
	const int nlen = strlen(intListStr);
	char* pstr = new char[nlen + 1];
	strcpy(pstr, intListStr);

	char * pvalue = strtok(pstr, spliter);
	while (pvalue) {
		outIntList.push_back((int) atoi(pvalue));
		pvalue = strtok(NULL, spliter);
	}
	delete pstr;
	return outIntList;

}

void XmlSceneSerializer::StringToColor(const char* hexStr, Color& targetColor) {
	string tmpStr(hexStr);
	std::transform(tmpStr.begin(), tmpStr.end(), tmpStr.begin(),
			[](unsigned char c)
			{	return std::tolower(c);});
	int r = 0, g = 0, b = 0, a = 0;
	sscanf(tmpStr.c_str(), "#%2x%2x%2x", r, g, b);
//	Color textColor(r/255.0f,g/255.0f,b/255.0f,1.0f);
	targetColor.m_r = r / 255.0f;
	targetColor.m_g = g / 255.0f;
	targetColor.m_b = b / 255.0f;
	targetColor.m_a = 1.0f;
}

void XmlSceneSerializer::StringFromFloatList(vector<float>& floatList,
		char* outStr) {
	for (int i = 0; i < floatList.size(); i++) {
		float value = floatList.at(i);
		if (i == 0) {
			sprintf(outStr, "%f", value);
		} else {
			sprintf(outStr, "%s %f", outStr, value);
		}
	}
	//LOGI("XmlHelper::floatListToString :%s", outStr);
}

void XmlSceneSerializer::StringFromColor(Color &color, char* outStr) {
	sprintf(outStr, "#%x%x%x", color.m_r, color.m_g, color.m_b);
}

vector<float>&
XmlSceneSerializer::StringToFloatList(const char* floatListStr,
		vector<float>& outFloatList) {
	const char spliter[] = " ";

	int nlen = strlen(floatListStr);
	char* pstr = new char[nlen + 1];
	strcpy(pstr, floatListStr);

	char * pvalue = strtok(pstr, spliter);
	while (pvalue) {
		outFloatList.push_back((float) atof(pvalue));
		pvalue = strtok(NULL, spliter);
	}
	delete pstr;
	return outFloatList;
}
}
