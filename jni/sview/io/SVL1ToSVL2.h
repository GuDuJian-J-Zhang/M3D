/****************************************************************************
版权所有 (C) 2014  山东山大华天软件有限公司

文件名：	SVLVersionConvertor.h
功能：
提供SVL各版本的数据格式转换接口

更新记录：
****************************************************************************/
#ifndef _SVL1ToSVL2_H_
#define _SVL1ToSVL2_H_

#include "Stk_ProtoType.h"
#include "SVLLib/Stk_Document.h"

//Ascii码文件读取结构
class CSVL1ToSVL2
{
public:
	CSVL1ToSVL2();
	~CSVL1ToSVL2();
	
public:

	/*****************************************************************
	函数名	：SVL1To2
	功能	：将SVL1.0版本的数据转换为SVL2.0格式
	参数	：
	Stk_DocumentManager	*pStkDocMgr, SVL1.0文档管理指针
	Stk_Document* pDocument，SVL2.0文档管理指针
	返回值	：转换结果
	更新历史：
	add by likun 2017-06-23
	*****************************************************************/
	static int SVL1ToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);

	/*****************************************************************
	函数名	：ProtoType1To2
	功能	：将SVL1.0版本的ProtoType数据转换为SVL2.0格式ProtoType
	参数	：
	Stk_ProtoType *pSrcProtoType, SVL1.0版本ProtoType
	Stk_ModelPtr pNewProtoType，SVL2.0版本ProtoType
	返回值	：转换结果
	更新历史：
	add by likun 2017-06-23
	*****************************************************************/
	static int SVL1InstanceToSVL2(Stk_Instance *pSrcProtoType, HoteamSoft::SVLLib::Stk_InstancePtr pNewInstance, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);

	static int SVL1ProtoTypeToSVL2(Stk_ProtoType *pSrcProtoType, HoteamSoft::SVLLib::Stk_ModelPtr pNewProtoType, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1NodeToSVL2(Stk_Node* pSrcNode, HoteamSoft::SVLLib::Stk_ModelPtr pNewProtoType, HoteamSoft::SVLLib::Stk_MaterialPtr parentMaterialPtr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1MeshToSVL2(Stk_Mesh_Com* pSrcMeshCom, HoteamSoft::SVLLib::Stk_ModelPtr pModel, HoteamSoft::SVLLib::Stk_MeshPrimPtr pMeshPrim, HoteamSoft::SVLLib::Stk_MaterialPtr  parentMaterialPtr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1LineSetToSVL2(Stk_LineSet* pSrcLineSet, HoteamSoft::SVLLib::Stk_LineSetPtr pLineSetPtr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1SpacialPointToSVL2(Stk_SpacialPointEntity* pSrcSpacialPoint, HoteamSoft::SVLLib::Stk_SpacialPointEntityPtr pSpacialPointPtr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static HoteamSoft::SVLLib::Stk_CurveEntityPtr SVL1CurveToSVL2(Stk_CurveEntity* pSrcCurve, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1PMIToSVL2(Stk_PMI* pSrcPMI, HoteamSoft::SVLLib::Stk_PMIPtr pNewPMI, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1LeaderToSVL2(Stk_Leader* pSrcLeader, HoteamSoft::SVLLib::Stk_LeaderPtr pNewLeader, 
		HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1ExlineToSVL2(Stk_Extensionline* pSrcExline, HoteamSoft::SVLLib::Stk_ExtensionlinePtr pNewExline, 
		HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1ComTextToSVL2(Stk_ComText* pSrcComText, HoteamSoft::SVLLib::Stk_ComTextPtr pNewComText, 
		HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1OutFrameToSVL2(Stk_OutFrame* pSrcOutFrame, HoteamSoft::SVLLib::Stk_OutFramePtr pNewOutFrame, 
		HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1SpecialLineToSVL2(Stk_SpecialLine* pSrcSpecialLine, HoteamSoft::SVLLib::Stk_SpecialLinePtr pNewSpecialLine, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1ImageToSVL2(Stk_Picture* pSrcImage, HoteamSoft::SVLLib::Stk_ImagePtr pNewImage, 
		HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1UserDataToSVL2(Stk_UserData* pSrcUserData, HoteamSoft::SVLLib::Stk_UserDataPtr pNewUserData,
		HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1LineToSVL2(Stk_Line* pSrcLine, HoteamSoft::SVLLib::Stk_LinePtr pNewLine,
		HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1EllipseToSVL2Arc(Stk_Ellipse* pSrcEllipse, HoteamSoft::SVLLib::Stk_ArcPtr pNewArc,
		HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1PolylineToSVL2(Stk_PolyLine* pSrcPolyline, HoteamSoft::SVLLib::Stk_PolyLinePtr pNewPolyline,
		HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1TextToSVL2(Stk_Text* pSrcText, HoteamSoft::SVLLib::Stk_TextPtr pNewText,
		HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1EnvelopeToSVL2(Stk_Envelope* pSrcEnvelope, HoteamSoft::SVLLib::Stk_EnvelopePtr pNewEnvelope,
		HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);

	//视图
	static int SVL1ViewToSVL2(Stk_View* pSrcView, HoteamSoft::SVLLib::Stk_ViewPtr pNewView,float* modelBox,
		HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1CameraToSVL2(Stk_Camera* pSrcCamera, HoteamSoft::SVLLib::Stk_CameraPtr pNewCamera, float* modelBox,
		HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1PlaneSurfaceToSVL2ClipPlane(Stk_PlaneSurface* pSrcClipPlane, HoteamSoft::SVLLib::Stk_ClipPlanePtr pNewClipPlane,
		HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1ClippingCurveToSVL2(Stk_ClippingCurve* pSrcClippingCurve, HoteamSoft::SVLLib::Stk_ClippingCurvePtr pNewClippingCurve, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1NodeConnectorToSVL2(Stk_ProtoType* pSrcProtoType, Stk_NodeConnector* pSrcConnector, HoteamSoft::SVLLib::Stk_ConnectorPtr pNewConnector,
		HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVl1ViewConnectorToSVL2(Stk_ProtoType* pSrcProtoType, Stk_View* pSrcView, HoteamSoft::SVLLib::Stk_ViewPtr pNewView,
		HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	
	static int SVL1GeoAttrToSVL2MeshFaceAttr(Stk_GeometryAttribute* pSrcGeoAttr, HoteamSoft::SVLLib::Stk_ModelPtr pModel, HoteamSoft::SVLLib::Stk_MeshFacePtr pNewMeshFace, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1GeoAttrToSVL2MeshEdgeAttr(Stk_GeometryAttribute* pSrcGeoAttr, HoteamSoft::SVLLib::Stk_ModelPtr pModel, HoteamSoft::SVLLib::Stk_MeshEdgePtr pNewMeshFace, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);

	static int SVL1LayerInfosSetToSVL2(Stk_LayerInfosSet* pSrcLayerInfosSet, HoteamSoft::SVLLib::Stk_LayerInfosSetPtr pLayerInfosSet, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1LayerInfosSetToSVL2(Stk_Node* pSrcNode, HoteamSoft::SVLLib::Stk_ModelPtr pNewProtoType, HoteamSoft::SVLLib::Stk_MaterialPtr parentMaterialPtr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1FeatToSVL2(Stk_Feat* pSrcFeat, HoteamSoft::SVLLib::Stk_FeatPtr pTargetFeat, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1FeatMagneticLineToSVL2(Stk_Feat_MagneticLine* pSrcFeatMagneticLine, HoteamSoft::SVLLib::Stk_Feat_MagneticLinePtr pTargetFeatMagneticLine, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1FeatHoleToSVL2(Stk_Feat_Hole* pSrcFeatHole, HoteamSoft::SVLLib::Stk_Feat_HolePtr pTargetFeatHole, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1FeatPipeToSVL2(Stk_Feat_Pipe *pSrcFeatPipe, HoteamSoft::SVLLib::Stk_Feat_PipePtr pTargetFeatPipe, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1FeatMarkToSVL2(Stk_Feat_Mark *pSrcFeatMark, HoteamSoft::SVLLib::Stk_Feat_MarkPtr pTargetFeatMark, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);

	static int SVL1OutFileToSVL2(Stk_DocumentManager *pStkDocMgr, const wstring wstrFileExt, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int GetSVL1TopProtoTypeUserData(Stk_DocumentManager *pStkDocMgr, const wstring wstrUserDataName, STK_CHAR** dataPP, STK_SIZE_T &dataSize);
	//动画
	static int SVL1SAToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	//模型加载信息
	static int SVL1LoadInfToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	//预览图
	static int SVL1PreviewToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	//工艺Bom
	static int SVL1VBomToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	//模型分组数据
	static int SVL1GroupToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	//故障数据
	static int SVL1MalfunctionToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	//线缆关联信息数据
	static int SVL1CablePropertyFileToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	//热点数据
	static int SVL1HotSpotToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	//箭头数据
	static int SVL1ArrowToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	//移动端生成的视图数据
	static int SVL1ViewToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1UserNoteToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	//任务数据
	static int SVL1TaskToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
};


#endif //_SVL1ToSVL2_H_
