//#include "m3d/utils/MeasureTools.h"
//
//#include "m3d/SceneManager.h"
//
//#include "m3d/scene/SceneNode.h"
//#include "m3d/scene/ShapeNode.h"
//#include "m3d/scene/AxisNode.h"
//#include "m3d/scene/FPSNode.h"
//#include "m3d/graphics/CameraNode.h"
//#include "m3d/scene/SceneNode.h"
//#include "m3d/scene/ShapeNode.h"
//
//#include "m3d/model/Shape.h"
//#include "m3d/model/Model.h"
//#include "m3d/model/GeoAttribute.h"
//#include "m3d/model/Body.h"
//#include "m3d/model/Face.h"
//#include "m3d/model/Edge.h"
//
//#include "m3d/model/CText.h"
//#include "m3d/model/ComText.h"
//
//#include "m3d/utils/M3DTools.h"
//#include "m3d/utils/MeasureTools.h"
//
//#include "sview/views/Parameters.h"
//
//#include "m3d/action/Action.h"
//#include "m3d/action/RayPickAction.h"
//
//#include "m3d/model/GeoAttribute.h"
//#include "m3d/model/ModelView.h"
//#include "m3d/graphics/CameraNode.h"
//#include "m3d/base/Matrix4.h"
//#include "m3d/SceneManager.h"
//#include "m3d/graphics/SectionPlane.h"
//#include "m3d/extras/measure/SMeasureLineLengthNote.h"
//#include "m3d/extras/measure/SMeasureRadiusNote.h"
//#include "m3d/extras/measure/SMeasureDiameterNote.h"
//
//namespace M3D
//{
//Shape* MeasureTools::AddLineLengthNote(SceneManager* sceneMgr, Shape* shape,
//		float x, float y)
//{
//	SMeasureLineLengthNote* measureNote = NULL;
//	if (shape != NULL)
//	{
//		if (shape->GetType() == SHAPE_EDGE)
//		{
//			Edge* edge = (Edge*) shape;
//			//	LOGE("AddLineLengthNote step1");
//			GeometryAttribute* geo = edge->GetLineData()->GetGeoAttribute();
//			//		LOGE("AddLineLengthNote step2");
//			if (geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_LINE)
//			{
//				//		LOGE("AddLineLengthNote step3 ");
//				//Vector3 anchorPnt = sceneMgr->GetPickPoint(x, y);
//
//				LineAttribute* lineGeo = (LineAttribute*) geo;
//				Matrix3x4 mMatrix = sceneMgr->GetShapeModelMatrix(edge);
//				Vector3 startPnt = lineGeo->GetStartPoint();
//				Vector3 endPnt = lineGeo->GetEndPoint();
//				//float lineLength = lineGeo->GetLength();
//
//				float lineLength = (startPnt - endPnt).Length();
//
//				//		LOGE("AddLineLengthNote step 4");
//				startPnt = mMatrix*startPnt;
//				endPnt = mMatrix*endPnt;
//
//				//		LOGE("AddLineLengthNote step 5");
//				measureNote = new SMeasureLineLengthNote();
//				measureNote->setRelevantShape(edge);
//
//				Vector3 rayStart;
//				Vector3 rayEnd;
////				sceneMgr->GetPickRay(x, y, rayStart, rayEnd);
//
//				//LOGE("rayStart :%f %f %f", rayStart.x,rayStart.y,rayStart.z);
//				//LOGE("rayEnd :%f %f %f", rayEnd.x,rayEnd.y,rayEnd.z);
//
//				Line3D rayLine = Line3D(rayStart, rayEnd);
//				Line3D* line1 = new Line3D(startPnt, endPnt);
//
//				//LOGE("line1->GetDirection() :%f %f %f", line1->GetDirection().x,line1->GetDirection().y,line1->GetDirection().z);
//				//LOGE("rayLine.GetDirection() :%f %f %f", rayLine.GetDirection().x,rayLine.GetDirection().y,rayLine.GetDirection().z);
//				Vector3 faceNormal =
//						(line1->GetDirection().CrossProduct(rayLine.GetDirection())).CrossProduct(line1->GetDirection());
//
//				SectionPlane plane = SectionPlane(startPnt, faceNormal);
//
//				Vector3 pntInPlan = plane.GetInsectPnt(rayStart,
//						rayLine.GetDirection());
//
//				//LOGE("pntinPlan :%f %f %f", pntInPlan.x,pntInPlan.y,pntInPlan.z);
//
//				Vector3 pntInLine = line1->GetClosestPoint(pntInPlan);
//
//				Vector3 newStart = startPnt + pntInPlan - pntInLine;
//				Vector3 newEnd = endPnt + pntInPlan - pntInLine;
//
//				Line3D* line2 = new Line3D(startPnt, newStart);
//				Line3D* line3 = new Line3D(endPnt, newEnd);
//				Line3D* line4 = new Line3D(newStart, newEnd);
//				Line3D* line5 = new Line3D(pntInPlan, newStart);
//
//				line1->SetStartArrow(1);
//				line1->SetEndArrow(1);
//
//				line4->SetStartArrow(1);
//				line4->SetEndArrow(1);
//
//				line5->SetStartArrow(1);
//				line5->SetEndArrow(1);
//
//				//	LOGE("Length:%f",lineLength);
//				string lengthstr ="L" + M3DTOOLS::floatToString(lineLength);
//				//	LOGE("AddLineLengthNote step 6");
//
//				//measureNote->BeginWrite();
//
//				measureNote->AddLine(line1);
//				measureNote->AddLine(line2);
//				measureNote->AddLine(line3);
//				measureNote->AddLine(line4);
//				measureNote->AddLine(line5);
//				measureNote->AddText(lengthstr, pntInPlan, 1.5);
//				sceneMgr->AddShape(measureNote);
//
//				//measureNote->EndWrite();
//				//LOGE("MeasureNote id is %d",measureNote->GetID());
//				//		LOGE("AddLineLengthNote step 7");
//			}
//		}
//	}
//
//	return measureNote;
//}
//void MeasureTools::OpenEditLineLengthNote(SceneManager* sceneMgr, Shape* shape,
//		float x, float y)
//{
//
//}
//void MeasureTools::EditLineLengthNote(SceneManager* sceneMgr, Shape* shape,
//		float x, float y)
//{
////		SMeasureLineLengthNote* measureNote = NULL;
////		if (shape != NULL)
////		{
////			if (shape->GetType() == SHAPE_MEASURE_LENGTH)
////			{
////				SMeasureLineLengthNote* measureNote  = (Edge*) shape;
////				//	LOGE("AddLineLengthNote step1");
////
////					//		LOGE("AddLineLengthNote step3 ");
////					//Vector3 anchorPnt = sceneMgr->GetPickPoint(x, y);
////
////					LineAttribute* lineGeo = (LineAttribute*) geo;
////					Matrix mMatrix = sceneMgr->GetShapeModelMatrix(edge);
////					Vector3 startPnt = lineGeo->GetStartPoint();
////					Vector3 endPnt = lineGeo->GetEndPoint();
////					//float lineLength = lineGeo->GetLength();
////
////					float lineLength = VectorMagnitude(startPnt - endPnt) ;
////
////					//		LOGE("AddLineLengthNote step 4");
////					startPnt = Matrix::VectorTransform(startPnt, mMatrix.GetData());
////					endPnt = Matrix::VectorTransform(endPnt, mMatrix.GetData());
////
////
////					Vector3 rayStart;
////					Vector3 rayEnd;
////					sceneMgr->GetPickRay(x,y,rayStart,rayEnd);
////
////					//LOGE("rayStart :%f %f %f", rayStart.x,rayStart.y,rayStart.z);
////					//LOGE("rayEnd :%f %f %f", rayEnd.x,rayEnd.y,rayEnd.z);
////
////					Line3D rayLine = Line3D(rayStart,rayEnd);
////					Line3D* line1 = new Line3D(startPnt,endPnt);
////
////					//LOGE("line1->GetDirection() :%f %f %f", line1->GetDirection().x,line1->GetDirection().y,line1->GetDirection().z);
////					//LOGE("rayLine.GetDirection() :%f %f %f", rayLine.GetDirection().x,rayLine.GetDirection().y,rayLine.GetDirection().z);
////					Vector3 faceNormal = VectorCross( VectorCross(line1->GetDirection(),rayLine.GetDirection()),line1->GetDirection());
////
////					Plane plane = Plane(startPnt,faceNormal);
////
////					Vector3 pntInPlan = plane.GetInsectPnt(rayStart,rayLine.GetDirection());
////
////					//LOGE("pntinPlan :%f %f %f", pntInPlan.x,pntInPlan.y,pntInPlan.z);
////
////					Vector3 pntInLine = line1->GetClosestPoint(pntInPlan);
////
////					Vector3 newStart = startPnt + pntInPlan - pntInLine;
////					Vector3 newEnd = endPnt + pntInPlan - pntInLine;
////
////
////					Line3D* line2 =  new Line3D(startPnt,newStart);
////					Line3D* line3 =  new Line3D(endPnt,newEnd);
////					Line3D* line4 = new Line3D(newStart,newEnd);
////					Line3D* line5 = new Line3D(pntInPlan, newStart);
////
////					line1->SetStartArrow(1);
////					line1->SetEndArrow(1);
////
////					line4->SetStartArrow(1);
////					line4->SetEndArrow(1);
////
////					line5->SetStartArrow(1);
////					line5->SetEndArrow(1);
////
////					//	LOGE("Length:%f",lineLength);
////					string lengthstr = M3DTOOLS::floatToString(lineLength);
////					//	LOGE("AddLineLengthNote step 6");
////
////					measureNote->BeginWrite();
////
////					measureNote->AddLine(line1);
////					measureNote->AddLine(line2);
////					measureNote->AddLine(line3);
////					measureNote->AddLine(line4);
////					measureNote->AddLine(line5);
////					measureNote->AddText(lengthstr,pntInPlan, 1.5);
////
////					measureNote->EndWrite();
////					//LOGE("MeasureNote id is %d",measureNote->GetID());
////					//		LOGE("AddLineLengthNote step 7");
////			}
////		}
////
////		return  measureNote ;
//}
//void MeasureTools::CloseEditLineLengthNote(SceneManager* sceneMgr, Shape* shape,
//		float x, float y)
//{
//
//}
//
//Shape* MeasureTools::AddRadiusLengthNote(SceneManager* sceneMgr, Shape* shape,
//		float x, float y)
//{
//	//LOGE("AddRadiusLengthNote step0");
//	SMeasureRadiusNote* measureNote = NULL;
//	if (shape != NULL)
//	{
//		if (shape->GetType() == SHAPE_EDGE)
//		{
//			Edge* edge = (Edge*) shape;
//			//LOGE("AddRadiusLengthNote step1");
//			GeometryAttribute* geo = edge->GetLineData()->GetGeoAttribute();
//			//		LOGE("AddLineLengthNote step2");
//			if (geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_ELLIPSE)
//			{
//				//		LOGE("AddLineLengthNote step3 ");
//				//Vector3 anchorPnt = sceneMgr->GetPickPoint(x, y);
//				EllipseAttribute* ellipseGeo = (EllipseAttribute*) geo;
// 			Matrix3x4 mMatrix = sceneMgr->GetShapeModelMatrix(edge);
//				Vector3 startPnt = ellipseGeo->GetStartPoint();
//				Vector3 endPnt = ellipseGeo->GetEndPoint();
//				//float lineLength = lineGeo->GetLength();
//
//				float lineLength =ellipseGeo->GetMajorRadius();
//				//VectorMagnitude(startPnt - endPnt);
//
//				//		LOGE("AddLineLengthNote step 4");
//				startPnt = mMatrix*startPnt;
//				endPnt = mMatrix*endPnt;
//
//				//		LOGE("AddLineLengthNote step 5");
//				measureNote = new SMeasureRadiusNote();
//				measureNote->setRelevantShape(edge);
//
//				Vector3 rayStart;
//				Vector3 rayEnd;
//				//TODO
////				sceneMgr->GetPickRay(x, y, rayStart, rayEnd);
//
//				//LOGE("rayStart :%f %f %f", rayStart.x,rayStart.y,rayStart.z);
//				//LOGE("rayEnd :%f %f %f", rayEnd.x,rayEnd.y,rayEnd.z);
//
//				Line3D rayLine = Line3D(rayStart, rayEnd);
//				Line3D* line1 = new Line3D(startPnt, endPnt);
//
//				//LOGE("line1->GetDirection() :%f %f %f", line1->GetDirection().x,line1->GetDirection().y,line1->GetDirection().z);
//				//LOGE("rayLine.GetDirection() :%f %f %f", rayLine.GetDirection().x,rayLine.GetDirection().y,rayLine.GetDirection().z);
//				Vector3 faceNormal = (line1->GetDirection().CrossProduct(rayLine.GetDirection())).CrossProduct(line1->GetDirection());
//
//				SectionPlane plane = SectionPlane(startPnt, faceNormal);
//
//				Vector3 pntInPlan = plane.GetInsectPnt(rayStart,
//						rayLine.GetDirection());
//
//				//LOGE("pntinPlan :%f %f %f", pntInPlan.x,pntInPlan.y,pntInPlan.z);
//
//				Vector3 pntInLine = line1->GetClosestPoint(pntInPlan);
//
//				Vector3 newStart = startPnt + pntInPlan - pntInLine;
//				Vector3 newEnd = endPnt + pntInPlan - pntInLine;
//
//				Line3D* line2 = new Line3D(startPnt, newStart);
//				Line3D* line3 = new Line3D(endPnt, newEnd);
//				Line3D* line4 = new Line3D(newStart, newEnd);
//				Line3D* line5 = new Line3D(pntInPlan, newStart);
//
//				line1->SetStartArrow(1);
//				line1->SetEndArrow(1);
//
//				line4->SetStartArrow(1);
//				line4->SetEndArrow(1);
//
//				line5->SetStartArrow(1);
//				line5->SetEndArrow(1);
//
//				//	LOGE("Length:%f",lineLength);
//				string lengthstr = "R"+M3DTOOLS::floatToString(lineLength);
//				//	LOGE("AddLineLengthNote step 6");
//
//				//measureNote->BeginWrite();
//
//				measureNote->AddLine(line1);
//				measureNote->AddLine(line2);
//				measureNote->AddLine(line3);
//				measureNote->AddLine(line4);
//				measureNote->AddLine(line5);
//				measureNote->AddText(lengthstr, pntInPlan, 1.5);
//				sceneMgr->AddShape(measureNote);
//
//				//measureNote->EndWrite();
//				//LOGE("MeasureNote id is %d",measureNote->GetID());
//				//		LOGE("AddLineLengthNote step 7");
//			}
//		}
//	}
//
//	return measureNote;
//}
//void MeasureTools::OpenEditRadiusLengthNote(SceneManager* sceneMgr,
//		Shape* shape, float x, float y)
//{
//
//}
//
//void MeasureTools::EditRadiusLengthNote(SceneManager* sceneMgr, Shape* shape,
//		float x, float y)
//{
//
//}
//
//void MeasureTools::CloseEditRadiusLengthNote(SceneManager* sceneMgr,
//		Shape* shape, float x, float y)
//{
//
//}
//
//Shape* MeasureTools::AddDiameterLengthNote(SceneManager* sceneMgr, Shape* shape,
//		float x, float y)
//{
//	//LOGE("AddRadiusLengthNote step0");
//	SMeasureDiameter* measureNote = NULL;
//		if (shape != NULL)
//		{
//			if (shape->GetType() == SHAPE_EDGE)
//			{
//				Edge* edge = (Edge*) shape;
//				//LOGE("AddRadiusLengthNote step1");
//				GeometryAttribute* geo = edge->GetLineData()->GetGeoAttribute();
//				//		LOGE("AddLineLengthNote step2");
//				if (geo->GetGeoAttrType() == M3D_GEOATTR_TYPE_ELLIPSE)
//				{
//					//		LOGE("AddLineLengthNote step3 ");
//					//Vector3 anchorPnt = sceneMgr->GetPickPoint(x, y);
//					EllipseAttribute* ellipseGeo = (EllipseAttribute*) geo;
//					Matrix3x4 mMatrix = sceneMgr->GetShapeModelMatrix(edge);
//					Vector3 startPnt = ellipseGeo->GetStartPoint();
//					Vector3 endPnt = ellipseGeo->GetEndPoint();
//					//float lineLength = lineGeo->GetLength();
//
//					float lineLength = ellipseGeo->GetMajorRadius()*2;
//					//VectorMagnitude(startPnt - endPnt);
//
//					//		LOGE("AddLineLengthNote step 4");
//					startPnt = mMatrix*startPnt;
//					endPnt = mMatrix*endPnt;
//
//					//		LOGE("AddLineLengthNote step 5");
//					measureNote = new SMeasureDiameter();
//					measureNote->setRelevantShape(edge);
//
//					Vector3 rayStart;
//					Vector3 rayEnd;
////					sceneMgr->GetPickRay(x, y, rayStart, rayEnd);
//
//					//LOGE("rayStart :%f %f %f", rayStart.x,rayStart.y,rayStart.z);
//					//LOGE("rayEnd :%f %f %f", rayEnd.x,rayEnd.y,rayEnd.z);
//
//					Line3D rayLine = Line3D(rayStart, rayEnd);
//					Line3D* line1 = new Line3D(startPnt, endPnt);
//
//					//LOGE("line1->GetDirection() :%f %f %f", line1->GetDirection().x,line1->GetDirection().y,line1->GetDirection().z);
//					//LOGE("rayLine.GetDirection() :%f %f %f", rayLine.GetDirection().x,rayLine.GetDirection().y,rayLine.GetDirection().z);
//					Vector3 faceNormal = (line1->GetDirection().CrossProduct(rayLine.GetDirection())).CrossProduct(line1->GetDirection());
//					SectionPlane plane = SectionPlane(startPnt, faceNormal);
//
//					Vector3 pntInPlan = plane.GetInsectPnt(rayStart,
//							rayLine.GetDirection());
//
//					//LOGE("pntinPlan :%f %f %f", pntInPlan.x,pntInPlan.y,pntInPlan.z);
//
//					Vector3 pntInLine = line1->GetClosestPoint(pntInPlan);
//
//					Vector3 newStart = startPnt + pntInPlan - pntInLine;
//					Vector3 newEnd = endPnt + pntInPlan - pntInLine;
//
//					Line3D* line2 = new Line3D(startPnt, newStart);
//					Line3D* line3 = new Line3D(endPnt, newEnd);
//					Line3D* line4 = new Line3D(newStart, newEnd);
//					Line3D* line5 = new Line3D(pntInPlan, newStart);
//
//					line1->SetStartArrow(1);
//					line1->SetEndArrow(1);
//
//					line4->SetStartArrow(1);
//					line4->SetEndArrow(1);
//
//					line5->SetStartArrow(1);
//					line5->SetEndArrow(1);
//
//					//	LOGE("Length:%f",lineLength);
//					string lengthstr = "D"+M3DTOOLS::floatToString(lineLength);
//					//	LOGE("AddLineLengthNote step 6");
//
//					//measureNote->BeginWrite();
//
//					measureNote->AddLine(line1);
//					measureNote->AddLine(line2);
//					measureNote->AddLine(line3);
//					measureNote->AddLine(line4);
//					measureNote->AddLine(line5);
//					measureNote->AddText(lengthstr, pntInPlan, 1.5);
//					sceneMgr->AddShape(measureNote);
//
//					//measureNote->EndWrite();
//					//LOGE("MeasureNote id is %d",measureNote->GetID());
//					//		LOGE("AddLineLengthNote step 7");
//				}
//			}
//		}
//
//		return measureNote;
//}
//void MeasureTools::OpenEditDiameterLengthNote(SceneManager* sceneMgr, Shape* shape,
//		float x, float y)
//{
//
//}
//void MeasureTools::EditDiameterLengthNote(SceneManager* sceneMgr, Shape* shape,
//		float x, float y)
//{
//
//}
//void MeasureTools::CloseEditDiameterLengthNote(SceneManager* sceneMgr,
//		Shape* shape, float x, float y)
//{
//
//}
//
//
//}
//
