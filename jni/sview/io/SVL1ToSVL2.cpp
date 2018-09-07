/****************************************************************************
版权所有 (C) 2014  山东山大华天软件有限公司

文件名：	SVLVersionConvertor.cpp
功能：
提供SVL各版本的数据格式转换接口

更新记录：
****************************************************************************/
#include "StdAfx.h"
#include "SVL1ToSVL2.h"
//#include "SVLVersionConvert.h"
#include <map>
#include "SVLLib/Stk_Declare_Ptr.h"
#include "SVLLib/Stk_Enum.h"
#include "SVLLib/Stk_Attribute.h"
#include "SVLLib/Stk_PMI.h"
#include "SVLLib/Stk_PMIEntity.h"
#include "SVLLib/Stk_Image.h"
#include "SVLLib/Stk_View.h"
#include "SVLLib/Stk_PlaneEntity.h"
#include "SVLLib/Stk_Connector.h"
#include "SVLLib/Stk_GeometryAttribute.h"
#include "SVLLib/Stk_MeshAttribute.h"
#include "SVLLib/Stk_LineSet.h"
#include "SVLLib/Stk_SpacialPoint.h"
#include "SVLLib/Stk_LoadInf.h"
#include "SVLLib/Stk_LayerInfosSet.h"
#include "SVLLib/Stk_Feat.h"
#include <iostream>  
#include <fstream>  
#include <stdio.h>
//#include <math.h>
#include <cstdlib>
#include <math.h>
#ifdef WIN32
 #include <io.h> 
#include <direct.h> 
#else
 #include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include "MatrixOperation.h"
#include "Stk_Preview.h"

#include "m3d/utils/FileHelper.h"
#include "m3d/utils/StringHelper.h"
#include "m3d/utils/Platform.h"
#include "SVL1ToSVL2.h"
#include "Utility.h"
#define  USERDATA_NAME_ANIMATION  L"Animation"     //动画
#define  USERDATA_NAME_USERNOTE   L"UserNote"      //批注
#define  USERDATA_NAME_CABLEINFO  L"CableInfo"     //线缆信息
#define  USERDATA_NAME_VBOM      L"VBOM"           //虚bom（工艺bom)
#define  USERDATA_NAME_ARROWDATA L"ARROWDATA"      //箭头数据
#define  USERDATA_NAME_ARROWLIST  L"ARROWLIST"     //箭头列表
#define  USERDATA_NAME_HOTSPOT   L"HotSpot"        //热点
#define  USERDATA_NAME_SERVICEMANUAL L"ServiceManual" //用户手册
#define  USERDATA_NAME_DEBUGINFO  L"DebugInfo"       //调试信息
#define  USERDATA_NAME_DEFAULTVIEWID L"DefaultViewID"  //默认视图ID
#define  USERDATA_NAME_ANCHORPOINT  L"AnchorPoint"    //锚点
#define  USERDATA_NAME_CHECKID     L"CHECKID"      

#define		F_TOL		0.0001



#define  MESH_TYPE                      L"type"
#define	 MESH_NORMAL                    L"normal"
#define  MESH_ORIGIN                    L"origin"
#define  MESH_AXISORIGIN                L"axisOrigin"
#define  MESH_AXISDIRECTION             L"axisDirection"
#define  MESH_RADIUS                    L"radius"
#define  MESH_ORIGINRADIUS              L"originRadius"
#define  MESH_MINORRADIUS               L"minorRadius"
#define  MESH_MAJORRADIUS               L"majorRadius"
#define  MESH_CENTEREPOINT              L"centerPoint"
#define  MESH_DIRECTION                 L"direction"
#define  MESH_STARTPOINT                L"startPoint"
#define  MESH_ENDPOINT                  L"endPoint"
#define  MESH_LENGTH                    L"length"
#define  MESH_XDIRECTION                L"xDirection"
#define  MESH_YDIRECTION                L"yDirection"
#define  MESH_ZDIRECTION                L"zDirection"

static std::map<int, int> m_mapOldProtoIdToNewProtoId;
static std::map<int, int> m_mapOldInsIdToNewInsId;
static map<STK_ID, HoteamSoft::SVLLib::STK_ID>    m_mapOldClipPlaneIDToNew;
static std::map<STK_ID, HoteamSoft::SVLLib::STK_ID> m_mapOldPmiIDToNew;
static std::map<STK_ID, HoteamSoft::SVLLib::STK_ID> m_mapOldViewIDToNew;
static std::map<Stk_Texture*, HoteamSoft::SVLLib::Stk_TexturePtr> m_mapTexturePtr;
static std::map<unsigned, std::wstring> m_mapImageHashToPath;

inline unsigned SDBMHash(unsigned hash, unsigned char c) {
	return c + (hash << 6) + (hash << 16) - hash;
}

unsigned Calculate(const char* str, const int len)
{
	unsigned hash = 0;

	if (!str)
		return hash;

	for (int i = 0; i < len; i++)
	{
		// Perform the actual hashing as case-insensitive
		char c = *str;
		hash = SDBMHash(hash, tolower(c));
		++str;
	}

	return hash;
}



CSVL1ToSVL2::CSVL1ToSVL2()
{
	
}

CSVL1ToSVL2::~CSVL1ToSVL2()
{
	m_mapOldPmiIDToNew.clear();
	m_mapOldInsIdToNewInsId.clear();
	m_mapOldProtoIdToNewProtoId.clear();
	m_mapOldClipPlaneIDToNew.clear();
	m_mapOldViewIDToNew.clear();
	m_mapTexturePtr.clear();
}

int CSVL1ToSVL2::SVL1ToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pStkDocMgr || pDocument.isNull())
	{
		return -1;
	}
	m_mapOldPmiIDToNew.clear();
	m_mapOldViewIDToNew.clear();
	m_mapImageHashToPath.clear();
	m_mapTexturePtr.clear();
	//输出File
	/*int iFileNum = pStkDocMgr->GetFileNum();
	for (int i = 0; i < iFileNum; i++)
	{
		Stk_File * pSrcFile = pStkDocMgr->GetFileByIndex(i);
		if(!pSrcFile)
			continue;
	}*/
	//输出ProtoType
	int iTopProtoTypeNum = pStkDocMgr->GetTopProtoTypeNum();
	for (int i = 0; i < iTopProtoTypeNum; i++)
	{
		Stk_ProtoType* pSrcProtoType = pStkDocMgr->GetTopProtoTypeByIndex(i);
		if(!pSrcProtoType)
			continue;

		HoteamSoft::SVLLib::Stk_InstancePtr pNewTopInstance = pDocument->AddTopInstance();

		int iNewModeId = -1;
		std::map<int, int>::iterator itMapOldProtoIdToNewProtoId = m_mapOldProtoIdToNewProtoId.find(pSrcProtoType->GetID());
		if (itMapOldProtoIdToNewProtoId != m_mapOldProtoIdToNewProtoId.end())
		{
			iNewModeId = itMapOldProtoIdToNewProtoId->second;
			
		}
		HoteamSoft::SVLLib::Stk_ModelPtr pNewTopProtoType = pNewTopInstance->SetModel(iNewModeId);
		//pNewTopInstance->SetInstanceName(pSrcProtoType->GetProtoName());
		if (iNewModeId == -1)
		{
			m_mapOldProtoIdToNewProtoId.insert(make_pair(pSrcProtoType->GetID(), pNewTopProtoType->GetID()));
		}
		
		//转换顶层装配中的几何数据
		iRet = SVL1ProtoTypeToSVL2(pSrcProtoType, pNewTopProtoType, pDocument);
		pNewTopProtoType->SetModelName(pSrcProtoType->GetProtoName());
		pNewTopInstance->SetInstanceName(pSrcProtoType->GetProtoName());
		pNewTopInstance->SetPlcID(0);

		vector<Stk_MetaData*> vcMetaData = pSrcProtoType->GetAllMetaDatas();
		vector<Stk_MetaData*>::iterator itorMetaData = vcMetaData.begin();
		while (itorMetaData != vcMetaData.end())
		{
			if (*itorMetaData)
			{
				wstring wstrName = (*itorMetaData)->GetTitle();
				wstring wstrValue = (*itorMetaData)->GetValue();
				pNewTopProtoType->AddAttribute(wstrName, wstrValue);
			}
			itorMetaData++;
		}

		//PMI转化
		//vector<Stk_PMI*> vcPMI = pSrcProtoType->GetPMI();
		//vector<Stk_PMI*>::iterator itorPMI = vcPMI.begin();
		//while (itorPMI != vcPMI.end())
		//{
		//	HoteamSoft::SVLLib::Stk_PMIPtr pmiP = HoteamSoft::SVLLib::Stk_PMIPtr::CreateObject();
		//	SVL1PMIToSVL2(*itorPMI, pmiP, pDocument);
		//	pNewTopProtoType->AddPMI(pmiP);
		//	int id = pmiP->GetID();
		//	itorPMI++;
		//}

		STK_BOX32 protoTypeBox;
		bool bProtoTypeBoxSetted = false;
		pNewTopProtoType->GetBoundingBox((float*)protoTypeBox.BoundBox, 6);
		if (fabs(protoTypeBox.BoundBox[0][0] - protoTypeBox.BoundBox[0][0]) > F_TOL ||
			fabs(protoTypeBox.BoundBox[0][1] - protoTypeBox.BoundBox[0][1]) > F_TOL ||
			fabs(protoTypeBox.BoundBox[0][2] - protoTypeBox.BoundBox[0][2]) > F_TOL)
		{
			bProtoTypeBoxSetted = true;
		}
		
		for(int j = 0; j < pSrcProtoType->GetChildInsNum(); j++)
		{
			Stk_Instance* pSrcIns = pSrcProtoType->GetChildIns(j);
			if(!pSrcIns)
				continue;
			HoteamSoft::SVLLib::Stk_InstancePtr pNewChildNode = pNewTopInstance->AddChild(/*pSrcIns->GetID()*/);
			SVL1InstanceToSVL2(pSrcIns, pNewChildNode,pDocument);
			pNewChildNode->SetInstanceName(pSrcIns->GetInsName());

			int iNumMetaData = pSrcIns->GetMetaDataNum();
			for (int i = 0; i < iNumMetaData; i++)
			{
				Stk_MetaData* pSrcMetaData = pSrcIns->GetMetaDataByIndex(i);
				if (pSrcMetaData)
				{
					wstring wstrName = pSrcMetaData->GetTitle();
					wstring wstrValue = pSrcMetaData->GetValue();
					pNewChildNode->AddAttribute(wstrName, wstrValue);
				}
			}

			//根据子件的包围盒，设置当前零件的包围盒
			const int iMatrixSize = MTX_LINE_COUNT * MTX_COL_COUNT;
			STK_BOX32 childInsBox;
			STK_ID plcID;
			STK_MTX32 childInsMatrix;
			pSrcIns->GetPlacement(plcID,childInsMatrix);
			if(pNewChildNode->GetModelByID(pNewChildNode->GetModelID()).isNotNull())
				pNewChildNode->GetModelByID(pNewChildNode->GetModelID())->GetBoundingBox((float*)childInsBox.BoundBox, 6);
			pNewChildNode->SetParentID(pNewTopInstance->GetID());

			float childInsMatrix2[4][4];
			MatrixOperation::MatrixTranspose((float*)childInsMatrix.PlcMatrix, (float*)childInsMatrix2, 4, 4);

			MatrixOperation::TransformPoint(childInsMatrix2, childInsBox.BoundBox[0]);
			MatrixOperation::TransformPoint(childInsMatrix2, childInsBox.BoundBox[1]);

			if (!bProtoTypeBoxSetted)
			{
				protoTypeBox = childInsBox;
				bProtoTypeBoxSetted = true;
			}
			else
			{
				for (int ii = 0; ii < 3; ii++)
				{
					if (protoTypeBox.BoundBox[0][ii] > childInsBox.BoundBox[0][ii])
					{
						protoTypeBox.BoundBox[0][ii] = childInsBox.BoundBox[0][ii];
					}
				}
				for (int ii = 0; ii < 3; ii++)
				{
					if (protoTypeBox.BoundBox[1][ii] < childInsBox.BoundBox[1][ii])
					{
						protoTypeBox.BoundBox[1][ii] = childInsBox.BoundBox[1][ii];
					}
				}
			}
		}

		//视图转化
		int iDefaultViewID = -1;
		map<STK_ID, Stk_UserData*>* mapAnimationP = pSrcProtoType->GetUserDataByName(L"DefaultViewID");
		if (mapAnimationP != NULL && mapAnimationP->size() > 0)
		{
			map<STK_ID, Stk_UserData*>::iterator itMapAnimation = mapAnimationP->begin();
			wstring wstrDataName = L"";
			STK_ID id = 0;
			STK_CHAR* dataP = NULL;
			STK_SIZE_T dataSize = 0;
			wstring wstrFileName = L"";
			itMapAnimation->second->GetUserData(wstrDataName, id, dataP, dataSize, wstrFileName);
			iDefaultViewID = (int)*dataP;
			
		}
		map<Stk_View*, HoteamSoft::SVLLib::Stk_ViewPtr> mapOldView2NewViewPtr;
		int iNumView = pSrcProtoType->GetViewNum();
		for (int i = 0; i < iNumView; i++)
		{
			Stk_View* pSrcView = pSrcProtoType->GetView(i);
			if (!pSrcView || !pSrcView->GetNode())
				continue;
			HoteamSoft::SVLLib::Stk_ViewPtr viewP = HoteamSoft::SVLLib::Stk_ViewPtr::CreateObject();
			SVL1ViewToSVL2(pSrcView, viewP, (float*)protoTypeBox.BoundBox,pDocument);

			if(iDefaultViewID == pSrcView->GetNode()->GetID())
			{
				viewP->SetDefault(true);
			}

			//SVl1ViewConnectorToSVL2(pSrcProtoType, pSrcView, viewP, pDocument);
			mapOldView2NewViewPtr.insert(make_pair(pSrcView, viewP));
			pNewTopProtoType->AddView(viewP);
		}

		//视图关联信息
		for (int i = 0; i < iNumView; i++)
		{
			Stk_View* pSrcView = pSrcProtoType->GetView(i);
			if (!pSrcView)
				continue;
			
			HoteamSoft::SVLLib::Stk_ViewPtr viewP = NULL;
			map<Stk_View*, HoteamSoft::SVLLib::Stk_ViewPtr>::iterator itorMap = mapOldView2NewViewPtr.find(pSrcView);
			if (itorMap != mapOldView2NewViewPtr.end())
			{
				viewP = itorMap->second;
			}
			SVl1ViewConnectorToSVL2(pSrcProtoType, pSrcView, viewP, pDocument);
		}
		mapOldView2NewViewPtr.clear();
		pNewTopProtoType->SetBoundingBox((float*)protoTypeBox.BoundBox);
	}

	//转换动画信息
	SVL1SAToSVL2(pStkDocMgr, pDocument);
	//转换加载配置文件
	//SVL1LoadInfToSVL2(pStkDocMgr, pDocument);

	//转换预览图
	SVL1PreviewToSVL2(pStkDocMgr, pDocument);
	//工艺Bom
	SVL1VBomToSVL2(pStkDocMgr, pDocument);
	//模型分组数据
	SVL1GroupToSVL2(pStkDocMgr, pDocument);
	//故障数据
	SVL1MalfunctionToSVL2(pStkDocMgr, pDocument);
	//线缆关联信息数据
	SVL1CablePropertyFileToSVL2(pStkDocMgr, pDocument);
	//热点数据
	SVL1HotSpotToSVL2(pStkDocMgr, pDocument);
	//任务数据
	SVL1TaskToSVL2(pStkDocMgr, pDocument);
	//箭头数据
	SVL1ArrowToSVL2(pStkDocMgr, pDocument);
	//用户批注
	SVL1UserNoteToSVL2(pStkDocMgr, pDocument);
	//移动端生成的视图数据
	SVL1ViewToSVL2(pStkDocMgr, pDocument);
	LOGI("SVL1ToSVL2 end");
	return iRet;
}

int CSVL1ToSVL2::SVL1InstanceToSVL2(Stk_Instance *pSrcInstance, HoteamSoft::SVLLib::Stk_InstancePtr pNewInstance, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcInstance || pNewInstance.isNull()|| pDocument .isNull())
	{
		return -1;
	}
	Stk_ProtoType *pSrcProtoType = pSrcInstance->GetProtoType();
	if (!pSrcProtoType)
	{
		return -1;
	}

	//转换基本信息，配置ID、配置矩阵、显隐状态、颜色、材质等
	//pNewInstance->SetInstanceName(pSrcInstance->GetInsName());
	//pNewProtoType->SetCADFilePath(pSrcProtoType->GetCADFilePath());
	//pNewInstance->SetBoundBox(pSrcInstance->)

	HoteamSoft::SVLLib::Stk_MaterialPtr  insMaterialPtr = NULL;
	if (pSrcInstance->HasColor())
	{
		HoteamSoft::SVLLib::MaterialData materialData;
		if(pSrcInstance->HasColor())
		{
			STK_RGBA32 insColor = pSrcInstance->GetColor();

			materialData.DiffuseColor[0] = insColor.Red;
			materialData.DiffuseColor[1] = insColor.Green;
			materialData.DiffuseColor[2] = insColor.Blue;
			materialData.fTransparency = insColor.Alpha;
		}
		if(fabs(materialData.fTransparency + 1.0)>F_TOL)
		{
			materialData.fTransparency = 1.0f - materialData.fTransparency;
		}
		//else
		//{
		//	materialData.fTransparency = 1.0f;
		//}
		insMaterialPtr = pDocument->FindMaterial(materialData);
		if(insMaterialPtr .isNull())
		{
			bool bHasSet = false;
			insMaterialPtr = pDocument->AddMaterial(bHasSet);
			insMaterialPtr->SetMaterialType(HoteamSoft::SVLLib::StkMaterialTypeEnum::STK_MATERIALTYPE_SIMPLAE);
			insMaterialPtr->SetTransparency(materialData.fTransparency);
			insMaterialPtr->SetDiffuseColor(materialData.DiffuseColor);
		}
	}
	
		
	float x, y, z;
	STK_ID iPlcID;
	STK_MTX32 mtxPlcMatrix;
	pSrcInstance->GetPlacement(iPlcID, mtxPlcMatrix);
	HoteamSoft::SVLLib::STK_MTX32 newPlcMatrix;
	for (int i = 0; i < MTX_LINE_COUNT; i++)
	{
		for (int j = 0; j < MTX_COL_COUNT; j++)
		{
			newPlcMatrix.PlcMatrix[i][j] = mtxPlcMatrix.PlcMatrix[i][j];
		}
	}
	pNewInstance->SetPlcID(iPlcID);
	if(pSrcInstance->GetDisplayState() == STK_DISPLAY)
	{
		pNewInstance->SetVisible(true);
	}
	else
	{
		pNewInstance->SetVisible(false);
	}
	pNewInstance->SetMatrix((float*)newPlcMatrix.PlcMatrix);
	if (insMaterialPtr .isNotNull())
	{
		pNewInstance->SetMaterialID(insMaterialPtr->GetID());
	}
	
	//转换几何数据
	bool bProtoInited = false;
	STK_ID iProtoID = pSrcProtoType->GetID();

	int iNewModeId = -1;
	std::map<int, int>::iterator itMapOldProtoIdToNewProtoId = m_mapOldProtoIdToNewProtoId.find(iProtoID);
	if (itMapOldProtoIdToNewProtoId != m_mapOldProtoIdToNewProtoId.end())
	{
		iNewModeId = itMapOldProtoIdToNewProtoId->second;
	}

	if (pDocument->GetPartByID(iNewModeId) .isNotNull())
	{
		bProtoInited = true;
	}
	HoteamSoft::SVLLib::Stk_ModelPtr pNewProtoType = pNewInstance->SetModel(iNewModeId);
	if (!bProtoInited)
	{
		iRet = SVL1ProtoTypeToSVL2(pSrcProtoType, pNewProtoType,pDocument);
		pNewProtoType->SetModelName(pSrcProtoType->GetProtoName());

		vector<Stk_MetaData*> vcMetaData = pSrcProtoType->GetAllMetaDatas();
		vector<Stk_MetaData*>::iterator itorMetaData = vcMetaData.begin();
		while (itorMetaData != vcMetaData.end())
		{
			if (*itorMetaData)
			{
				wstring wstrName = (*itorMetaData)->GetTitle();
				wstring wstrValue = (*itorMetaData)->GetValue();
				pNewProtoType->AddAttribute(wstrName, wstrValue);
			}
			itorMetaData++;
		}


		//vector<Stk_PMI*> vcPMI = pSrcProtoType->GetPMI();
		//vector<Stk_PMI*>::iterator itorPMI = vcPMI.begin();
		//while (itorPMI != vcPMI.end())
		//{
		//	HoteamSoft::SVLLib::Stk_PMIPtr pmiP = HoteamSoft::SVLLib::Stk_PMIPtr::CreateObject();
		//	SVL1PMIToSVL2(*itorPMI, pmiP, pDocument);
		//	pNewProtoType->AddPMI(pmiP);
		//	itorPMI++;
		//}
	}
	if (iNewModeId == -1)
	{
		m_mapOldProtoIdToNewProtoId.insert(make_pair(iProtoID, pNewProtoType->GetID()));
	}
	//转换实例的模型属性

	STK_BOX32 protoTypeBox;
	bool bProtoTypeBoxSetted = false;
	pNewProtoType->GetBoundingBox((float*)protoTypeBox.BoundBox, 6);
	if (fabs(protoTypeBox.BoundBox[0][0] - protoTypeBox.BoundBox[0][0]) > F_TOL ||
		fabs(protoTypeBox.BoundBox[0][1] - protoTypeBox.BoundBox[0][1]) > F_TOL ||
		fabs(protoTypeBox.BoundBox[0][2] - protoTypeBox.BoundBox[0][2]) > F_TOL)
	{
		bProtoTypeBoxSetted = true;
	}
	//处理子件
	for (int j = 0; j < pSrcProtoType->GetChildInsNum(); j++)
	{
		Stk_Instance* pSrcIns = pSrcProtoType->GetChildIns(j);
		if (!pSrcIns)
			continue;
		HoteamSoft::SVLLib::Stk_InstancePtr pNewChildNode = pNewInstance->AddChild(/*pSrcIns->GetID()*/);
		SVL1InstanceToSVL2(pSrcIns, pNewChildNode,pDocument);
		pNewChildNode->SetInstanceName(pSrcIns->GetInsName());

		int iNumMetaData = pSrcIns->GetMetaDataNum();
		for (int i = 0; i < iNumMetaData; i++)
		{
			Stk_MetaData* pSrcMetaData = pSrcIns->GetMetaDataByIndex(i);
			if (pSrcMetaData)
			{
				wstring wstrName = pSrcMetaData->GetTitle();
				wstring wstrValue = pSrcMetaData->GetValue();
				pNewChildNode->AddAttribute(wstrName, wstrValue);
			}
		}

		//根据子件的包围盒，设置当前零件的包围盒
		const int iMatrixSize = MTX_LINE_COUNT * MTX_COL_COUNT;
		STK_BOX32 childInsBox;
		STK_ID plcID;
		STK_MTX32 childInsMatrix;
		pSrcIns->GetPlacement(plcID, childInsMatrix);
		if(pNewChildNode->GetModelByID(pNewChildNode->GetModelID()) .isNotNull())
			pNewChildNode->GetModelByID(pNewChildNode->GetModelID())->GetBoundingBox((float*)childInsBox.BoundBox, 6);
		pNewChildNode->SetParentID(pNewInstance->GetID());

		float childInsMatrix2[4][4];
		MatrixOperation::MatrixTranspose((float*)childInsMatrix.PlcMatrix, (float*)childInsMatrix2, 4, 4);

		MatrixOperation::TransformPoint(childInsMatrix2, childInsBox.BoundBox[0]);
		MatrixOperation::TransformPoint(childInsMatrix2, childInsBox.BoundBox[1]);

		if (!bProtoTypeBoxSetted)
		{
			protoTypeBox = childInsBox;
			bProtoTypeBoxSetted = true;
		}
		else
		{
			for (int ii = 0; ii < 3; ii++)
			{
				if (protoTypeBox.BoundBox[0][ii] > childInsBox.BoundBox[0][ii])
				{
					protoTypeBox.BoundBox[0][ii] = childInsBox.BoundBox[0][ii];
				}
			}
			for (int ii = 0; ii < 3; ii++)
			{
				if (protoTypeBox.BoundBox[1][ii] < childInsBox.BoundBox[1][ii])
				{
					protoTypeBox.BoundBox[1][ii] = childInsBox.BoundBox[1][ii];
				}
			}
		}
	}
	pNewProtoType->SetBoundingBox((float*)protoTypeBox.BoundBox);
	
	return iRet;
}

int CSVL1ToSVL2::SVL1ProtoTypeToSVL2(Stk_ProtoType *pSrcProtoType, HoteamSoft::SVLLib::Stk_ModelPtr pNewProtoType, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcProtoType || pNewProtoType.isNull()|| pDocument .isNull())
	{
		return -1;
	}

	STK_BOX32 protoTypeBox;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			protoTypeBox.BoundBox[i][j] = 0.0f;
		}
	}
	bool bProtoTypeBoxSetted = false;
	//转换基本信息，显隐状态、颜色、材质等
	HoteamSoft::SVLLib::Stk_MaterialPtr  protoTypeMaterialPtr = NULL;
	if (pSrcProtoType->HasColor())
	{
		HoteamSoft::SVLLib::MaterialData materialData;
		STK_RGBA32 protoColor = pSrcProtoType->GetColor();

		materialData.DiffuseColor[0] = protoColor.Red;
		materialData.DiffuseColor[1] = protoColor.Green;
		materialData.DiffuseColor[2] = protoColor.Blue;
		materialData.fTransparency = protoColor.Alpha;

		if (fabs(materialData.fTransparency + 1.0) > F_TOL)
		{
			materialData.fTransparency = 1.0f - materialData.fTransparency;
		}
		//else
		//{
		//	materialData.fTransparency = 1.0f;
		//}
		protoTypeMaterialPtr = pDocument->FindMaterial(materialData);
		if (protoTypeMaterialPtr .isNull())
		{
			bool bHasSet = false;
			protoTypeMaterialPtr = pDocument->AddMaterial(bHasSet);
			protoTypeMaterialPtr->SetMaterialType(HoteamSoft::SVLLib::StkMaterialTypeEnum::STK_MATERIALTYPE_SIMPLAE);
			protoTypeMaterialPtr->SetTransparency(materialData.fTransparency);
			protoTypeMaterialPtr->SetDiffuseColor(materialData.DiffuseColor);
		}
	}

	//转换Node节点中的数据
	Stk_Node* pTopNode = pSrcProtoType->GetTopNode();
	if (pTopNode)
	{
		SVL1LayerInfosSetToSVL2(pTopNode, pNewProtoType, protoTypeMaterialPtr, pDocument);
		SVL1NodeToSVL2(pTopNode, pNewProtoType, protoTypeMaterialPtr, pDocument);
	}


	//转换几何数据
	for (int i = 0; i < pSrcProtoType->GetBodyNum(); i++)
	{
		Stk_Body* pSrcBody = pSrcProtoType->GetBody(i);
		if (!pSrcBody)
			continue;
		Stk_Node* pSrcBodyNode = pSrcBody->GetNode();
		if (!pSrcBodyNode)
			continue;
		bool bBodyVisible = pSrcBodyNode->GetDispStat();
		//获取body对应的图层
		HoteamSoft::SVLLib::Stk_LayerPtr layerPtr = NULL;
		for (int i = 0; i < pSrcBodyNode->GetInfoNum(); i++)
		{
			Stk_Information* stkInfo = pSrcBodyNode->GetInfoByIndex(i);
			STK_INT32 layerID = stkInfo->GetLayer();
			if (-1 != layerID) {
				layerPtr = pNewProtoType->GetLayerById(layerID);
				break;
			}
		}
		//转换Mesh信息
		if(bBodyVisible)
		{
			for (int j = 0; j < pSrcBody->GetMeshComNum(); j++)
			{
				Stk_Mesh_Com* pSrcMeshCom = pSrcBody->GetMeshCom(j);
				if (!pSrcMeshCom)
					continue;
				HoteamSoft::SVLLib::Stk_MeshPrimPtr pMeshPrim = pNewProtoType->AddLOD1Mesh(pSrcMeshCom->GetID());
				SVL1MeshToSVL2(pSrcMeshCom, pNewProtoType, pMeshPrim, protoTypeMaterialPtr, pDocument);
				STK_BOX32 boxMesh = pSrcMeshCom->GetBoundingBox();
				if (!bProtoTypeBoxSetted)
				{
					protoTypeBox = boxMesh;
				}
				else
				{
					for (int i = 0; i < 3; i++)
					{
						if (protoTypeBox.BoundBox[0][i] > boxMesh.BoundBox[0][i])
						{
							protoTypeBox.BoundBox[0][i] = boxMesh.BoundBox[0][i];
						}
					}
					for (int i = 0; i < 3; i++)
					{
						if (protoTypeBox.BoundBox[1][i] < boxMesh.BoundBox[1][i])
						{
							protoTypeBox.BoundBox[1][i] = boxMesh.BoundBox[1][i];
						}
					}
				}

				//添加Mesh与图层的关联
				if (layerPtr.isNotNull())
				{
					HoteamSoft::SVLLib::Stk_ConnectorPtr pConnector = HoteamSoft::SVLLib::Stk_ConnectorPtr::CreateObject();
					pConnector->SetEntityType(HoteamSoft::SVLLib::StkEntityTypeEnum::ENTITY_TYPE_LAYER, HoteamSoft::SVLLib::StkEntityTypeEnum::ENTITY_TYPE_MESH_COM);
					pConnector->SetIsLinkNode(true);
					pConnector->SetLinkNode(pMeshPrim->GetID());
					pConnector->SetIsLinkProto(false);
					pConnector->SetLinkProto(L"");
					layerPtr->AddNodeConnector(pConnector);
				}
			}
		}

		//转换线集信息
		vector<Stk_LineSet*> vecLineSets = pSrcBody->GetAllLineSets();
		for (int j = 0; j < vecLineSets.size(); j++)
		{
			Stk_LineSet* pSrcLineSet = vecLineSets[j];
			if (!pSrcLineSet)
				continue;
			HoteamSoft::SVLLib::Stk_LineSetPtr pLineSetPtr = pNewProtoType->AddLineSet(pSrcLineSet->GetID());
			pLineSetPtr->SetDispStat(bBodyVisible);
			SVL1LineSetToSVL2(pSrcLineSet, pLineSetPtr, pDocument);
			if (pSrcBody->GetNode()->GetNodeSubType() == StkNodeSubTypeEnum::NODE_SUB_TYPE_CLIPLINE)
			{
				pLineSetPtr->SetType(HoteamSoft::SVLLib::LINESET_TYPE_CLIPLINE);
				pLineSetPtr->SetID(pSrcBodyNode->GetID());
				pLineSetPtr->SetName(pSrcBodyNode->GetNodeName());
			}
		}

		//转换参照线信息
		vector<Stk_CurveEntity*> vecSubLines = pSrcBody->GetAllSubLines();
		for (int j = 0; j < vecSubLines.size(); j++)
		{
			Stk_CurveEntity* pSrcSubLine = vecSubLines[j];
			if (!pSrcSubLine)
				continue;
			HoteamSoft::SVLLib::Stk_CurveEntityPtr pSubLinePtr = SVL1CurveToSVL2(pSrcSubLine, pDocument);
			pSubLinePtr->SetCurveDispStat(bBodyVisible);
			pNewProtoType->AddSubLine(pSubLinePtr);
		}

		//转换空间点信息
		int iSpacialPointNum = pSrcBody->GetSpacialPointNum();
		for (int j = 0; j < iSpacialPointNum; j++)
		{
			Stk_SpacialPointEntity* pSrcSpacialPoint = pSrcBody->GetSpacialPointByIndex(j);
			if (!pSrcSpacialPoint)
				continue;
			HoteamSoft::SVLLib::Stk_SpacialPointEntityPtr SpacialPointPtr = pNewProtoType->AddSpacialPoint(pSrcSpacialPoint->GetID());
			SpacialPointPtr->SetDispStat(bBodyVisible);
			SVL1SpacialPointToSVL2(pSrcSpacialPoint, SpacialPointPtr, pDocument);
		}
	}



	//转换特征信息

	//转换模型属性

	//转换纹理

	


	//转换PMI信息

	//转换视图信息
	pNewProtoType->SetBoundingBox((float*)protoTypeBox.BoundBox);
	return iRet;
}


int CSVL1ToSVL2::SVL1LayerInfosSetToSVL2(Stk_Node* pSrcNode, HoteamSoft::SVLLib::Stk_ModelPtr pNewProtoType, HoteamSoft::SVLLib::Stk_MaterialPtr  parentMaterialPtr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcNode || pNewProtoType.isNull()|| pDocument .isNull())
	{
		return -1;
	}
	switch (pSrcNode->GetNodeType())
	{
	case StkNodeTypeEnum::NODE_TYPE_LAYERINFOSSET:
	{
		Stk_LayerInfosSet* pSrcLayerInfosSet = pSrcNode->GetLayerInfosSet();
		if (pSrcLayerInfosSet)
		{
			HoteamSoft::SVLLib::Stk_LayerInfosSetPtr pNewLayerInfosSet = pNewProtoType->AddLayerInfosSet(pSrcLayerInfosSet->GetID());
			SVL1LayerInfosSetToSVL2(pSrcLayerInfosSet, pNewLayerInfosSet, pDocument);
		}
		break;
	}
	case StkNodeTypeEnum::NODE_TYPE_VIRTUAL:
	{
		for (int i = 0; i < pSrcNode->GetChildNodeNum(); i++)
		{
			STK_ID plcID;
			Stk_Node* pChildNode = pSrcNode->GetChildNode(i, plcID);
			if (pChildNode)
			{
				SVL1LayerInfosSetToSVL2(pChildNode, pNewProtoType, parentMaterialPtr, pDocument);
			}
		}
		break;
	}
	default:
		break;
	}
    return iRet;
}

int CSVL1ToSVL2::SVL1NodeToSVL2(Stk_Node* pSrcNode, HoteamSoft::SVLLib::Stk_ModelPtr pNewProtoType, HoteamSoft::SVLLib::Stk_MaterialPtr  parentMaterialPtr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcNode || pNewProtoType.isNull()|| pDocument .isNull())
	{
		return -1;
	}

	//获取Node对应的图层
	HoteamSoft::SVLLib::Stk_LayerPtr layerPtr = NULL;
	for (int i = 0; i < pSrcNode->GetInfoNum(); i++)
	{
		Stk_Information* stkInfo = pSrcNode->GetInfoByIndex(i);
		STK_INT32 layerID = stkInfo->GetLayer();
		if (-1 != layerID) {
			layerPtr = pNewProtoType->GetLayerById(layerID);
			break;
		}
	}

	switch (pSrcNode->GetNodeType())
	{
	case StkNodeTypeEnum::NODE_TYPE_MESH:
	{
		Stk_Mesh_Com* pSrcMeshCom = pSrcNode->GetMesh();
		if (pSrcMeshCom)
		{
			HoteamSoft::SVLLib::Stk_MeshPrimPtr pMeshPrim = pNewProtoType->AddLOD1Mesh(pSrcMeshCom->GetID());
			SVL1MeshToSVL2(pSrcMeshCom, pNewProtoType, pMeshPrim, parentMaterialPtr, pDocument);

			//添加Mesh与图层的关联
			if (layerPtr .isNotNull())
			{
				HoteamSoft::SVLLib::Stk_ConnectorPtr pConnector = HoteamSoft::SVLLib::Stk_ConnectorPtr::CreateObject();
				pConnector->SetEntityType(HoteamSoft::SVLLib::StkEntityTypeEnum::ENTITY_TYPE_LAYER, HoteamSoft::SVLLib::StkEntityTypeEnum::ENTITY_TYPE_MESH_COM);
				pConnector->SetIsLinkNode(true);
				pConnector->SetLinkNode(pMeshPrim->GetID());
				pConnector->SetIsLinkProto(false);
				pConnector->SetLinkProto(L"");
				layerPtr->AddNodeConnector(pConnector);
			}
		}
		break;
	}
	case StkNodeTypeEnum::NODE_TYPE_PMI:
	{
		//转换PMI
		Stk_PMI* pSrcPMI = pSrcNode->GetPMI();
		if (pSrcPMI)
		{
			HoteamSoft::SVLLib::Stk_PMIPtr pmiP = HoteamSoft::SVLLib::Stk_PMIPtr::CreateObject();
			SVL1PMIToSVL2(pSrcPMI, pmiP, pDocument);
			pNewProtoType->AddPMI(pmiP);
			//添加PMI与图层的关联
			if (layerPtr.isNotNull())
			{
				HoteamSoft::SVLLib::Stk_ConnectorPtr pConnector = HoteamSoft::SVLLib::Stk_ConnectorPtr::CreateObject();
				pConnector->SetEntityType(HoteamSoft::SVLLib::StkEntityTypeEnum::ENTITY_TYPE_LAYER, HoteamSoft::SVLLib::StkEntityTypeEnum::ENTITY_TYPE_PMI);
				pConnector->SetIsLinkNode(true);
				pConnector->SetLinkNode(pmiP->GetID());
				pConnector->SetIsLinkProto(false);
				pConnector->SetLinkProto(L"");
				layerPtr->AddNodeConnector(pConnector);
			}
		}
		break;
	}
	//case StkNodeTypeEnum::NODE_TYPE_LAYERINFOSSET:
	//{
	//	Stk_LayerInfosSet* pSrcLayerInfosSet = pSrcNode->GetLayerInfosSet();
	//	if (pSrcLayerInfosSet)
	//	{
	//		HoteamSoft::SVLLib::Stk_LayerInfosSetPtr pNewLayerInfosSet = pNewProtoType->AddLayerInfosSet(pSrcLayerInfosSet->GetID());
	//		SVL1LayerInfosSetToSVL2(pSrcLayerInfosSet, pNewLayerInfosSet, pDocument);
	//	}
	//	break;
	//}
	case StkNodeTypeEnum::NODE_TYPE_VIRTUAL:
	{
		for (int i = 0; i < pSrcNode->GetChildNodeNum(); i++)
		{
			STK_ID plcID;
			Stk_Node* pChildNode = pSrcNode->GetChildNode(i, plcID);
			if (pChildNode)
			{
				SVL1NodeToSVL2(pChildNode, pNewProtoType, parentMaterialPtr, pDocument);
			}
		}
		break;
	}
	default:
		break;
	}
	
	//转换几何特征
	Stk_Feat* pSrcFeat = pSrcNode->GetFeat();
	if (pSrcFeat)
	{
		HoteamSoft::SVLLib::Stk_FeatPtr pTargetFeat = pNewProtoType->AddFeat(pSrcFeat->GetID());
		pTargetFeat->SetFeatName(pSrcNode->GetNodeName());
		SVL1FeatToSVL2(pSrcFeat, pTargetFeat, pDocument);
	}
    return iRet;
}
int CSVL1ToSVL2::SVL1MeshToSVL2(Stk_Mesh_Com* pSrcMeshCom, HoteamSoft::SVLLib::Stk_ModelPtr pModel, HoteamSoft::SVLLib::Stk_MeshPrimPtr pMeshPrim, HoteamSoft::SVLLib::Stk_MaterialPtr  parentMaterialPtr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcMeshCom || pMeshPrim.isNull()|| pDocument .isNull())
	{
		return -1;
	}
	//STK_BOX32 box = pSrcMeshCom->GetBoundingBox();
	//pMeshPrim->SetBoundBox(box.BoundBox[0], box.BoundBox[1]);

	std::vector<STK_FLOAT32> vecVertices;
	std::vector<STK_FLOAT32> vecNormals;
	std::vector<STK_FLOAT32> vecUVs;
	//std::vector<STK_UINT32> vecTrians;
	MeshLODInfo* pSrcMeshLodInfo = pSrcMeshCom->GetMeshLODInfo(0);
	if (pSrcMeshLodInfo)
	{
		int nVtxNum = pSrcMeshLodInfo->Vertices.size() / 2;
		for (int iCurMeshVexIdx = 0; iCurMeshVexIdx < pSrcMeshLodInfo->Vertices.size(); iCurMeshVexIdx++)
		{
			STK_PNT32 curVex = pSrcMeshLodInfo->Vertices[iCurMeshVexIdx];

			vecVertices.push_back(curVex.X);
			vecVertices.push_back(curVex.Y);
			vecVertices.push_back(curVex.Z);

			curVex = pSrcMeshLodInfo->Vertices[++iCurMeshVexIdx];
			vecNormals.push_back(curVex.X);
			vecNormals.push_back(curVex.Y);
			vecNormals.push_back(curVex.Z);
		}

		for (int iCurUVsIdx = 0; iCurUVsIdx < pSrcMeshLodInfo->UVs.size(); iCurUVsIdx++)
		{
			STK_PNT32 curUVs = pSrcMeshLodInfo->UVs[iCurUVsIdx];

			vecUVs.push_back(curUVs.X);
			vecUVs.push_back(curUVs.Y);
			vecUVs.push_back(curUVs.Z);
		}
	}
	

	pMeshPrim->SetVertexs(vecVertices);
	pMeshPrim->SetNormals(vecNormals);
	pMeshPrim->SetTextuercoordinate(vecUVs);

	bool bMeshHasColor = false;
	STK_RGBA32 meshColor = pSrcMeshCom->GetColor();
	if (pSrcMeshCom->HasColor())
	{
		meshColor = pSrcMeshCom->GetColor();
		bMeshHasColor = true;
	}

	Stk_Render* pStkRender = NULL;
	pSrcMeshCom->GetRenderByID(pSrcMeshCom->GetRenderID(), pStkRender);

	//转换面
	for (int i = 0; i < pSrcMeshCom->GetMeshFaceNum(); i++)
	{
		Stk_Mesh_Face* pCurMeshFace = pSrcMeshCom->GetMeshFace(i);
		if (!pCurMeshFace)
		{
			continue;
		}
		//Mesh无纹理使用Face的纹理
		if(!pStkRender)
		{
			pCurMeshFace->GetRenderByID(pCurMeshFace->GetRenderID(), pStkRender);
		}

		HoteamSoft::SVLLib::Stk_MeshFacePtr meshFacePtr = pMeshPrim->AddMeshFace(pCurMeshFace->GetID());
		if (bMeshHasColor || pCurMeshFace->HasColor() || pStkRender)
		{
			HoteamSoft::SVLLib::Stk_MaterialPtr  meshFaceMaterialPtr = NULL;
			HoteamSoft::SVLLib::MaterialData materialData;

			if(pCurMeshFace->HasColor())
			{
				STK_RGBA32 meshFaceColor = pCurMeshFace->GetColor();

				materialData.DiffuseColor[0] = meshFaceColor.Red;
				materialData.DiffuseColor[1] = meshFaceColor.Green;
				materialData.DiffuseColor[2] = meshFaceColor.Blue;
				materialData.fTransparency = meshFaceColor.Alpha;
			}
			else if (bMeshHasColor)
			{
				materialData.DiffuseColor[0] = meshColor.Red;
				materialData.DiffuseColor[1] = meshColor.Green;
				materialData.DiffuseColor[2] = meshColor.Blue;
				materialData.fTransparency = meshColor.Alpha;
			}
			if(pStkRender)
			{
				materialData.wstrDiffuseTextureImagePath = pStkRender->GetTexture()->GetTextureImage()->GetPictureSavePath();
			}

			if (fabs(materialData.fTransparency + 1.0) > F_TOL)
			{
				materialData.fTransparency = 1.0f - materialData.fTransparency;
			}
			//else
			//{
			//	materialData.fTransparency = 1.0f;
			//}
			meshFaceMaterialPtr = pDocument->FindMaterial(materialData);
			if (meshFaceMaterialPtr .isNull())
			{
				bool bHasSet = false;
				meshFaceMaterialPtr = pDocument->AddMaterial(bHasSet);
				meshFaceMaterialPtr->SetMaterialType(HoteamSoft::SVLLib::StkMaterialTypeEnum::STK_MATERIALTYPE_SIMPLAE);
				meshFaceMaterialPtr->SetTransparency(materialData.fTransparency);
				meshFaceMaterialPtr->SetDiffuseColor(materialData.DiffuseColor);

				//设置纹理贴图
				if(pStkRender)
				{
					Stk_Texture* pStkTexture = pStkRender->GetTexture();

					std::map<Stk_Texture*, HoteamSoft::SVLLib::Stk_TexturePtr>::iterator itTextureMap = m_mapTexturePtr.find(pStkTexture);
					if (itTextureMap != m_mapTexturePtr.end())
					{
						HoteamSoft::SVLLib::Stk_TexturePtr texturePtr = itTextureMap->second;
						meshFaceMaterialPtr->SetDiffuseTexture(texturePtr);
					}
					else
					{
						Stk_Picture* pStkPicture = pStkRender->GetTexture()->GetTextureImage();
						if (pStkPicture)
						{
							meshFaceMaterialPtr->SetMaterialType(HoteamSoft::SVLLib::StkMaterialTypeEnum::STK_MATERIALTYPE_PHONG);
							wstring wstrExt = L".jpg";
							StkPictureTypeEnum picType = pStkPicture->GetType();
							switch (picType)
							{
							case PICTURE_TYPE_UNKNOWN:
								break;
							case PICTURE_TYPE_JPG:
								wstrExt = L".jpg";
								break;
							case PICTURE_TYPE_BMP:
								wstrExt = L".bmp";
								break;
							case PICTURE_TYPE_PNG:
								wstrExt = L".png";
								break;
							case PICTURE_TYPE_CGM:
								wstrExt = L".cgm";
								break;
							case PICTURE_TYPE_GIF:
								wstrExt = L".gif";
								break;
							case PICTURE_TYPE_TIFF:
								wstrExt = L".tiff";
								break;
							case PICTURE_TYPE_ICO:
								wstrExt = L".ico";
								break;
								//case PICTURE_TYPE_RGB24:
								//	break;
								//case PICTURE_TYPE_RGB32:
								//	break;
								//case PICTURE_TYPE_PS:
								//	break;
								//case PICTURE_TYPE_SVG:
								//	break;
								//case PICTURE_TYPE_OLE:
								//	break;
							default:
								break;
							}
							STK_CHAR* chBuf = NULL;
							STK_UINT32 iBufSize;
							pStkPicture->GetData(iBufSize, chBuf);

							wstring wstrPicPath = L"";

							unsigned iHashValue = Calculate(chBuf, iBufSize);
							std::map<unsigned, std::wstring>::iterator itTextureImage = m_mapImageHashToPath.find(iHashValue);
							if (itTextureImage != m_mapImageHashToPath.end())
							{
								wstrPicPath = itTextureImage->second;
							}
							else
							{
								wstring wstrOutputFullPath = L"";
								wstrOutputFullPath = pDocument->GetOutputFullPath();//pStkPicture->GetPictureSavePath();

								wstrPicPath.append(L"images\\Material_");
                                wstring charID = M3D::Platform::StringToWString(M3D::StringHelper::IntToString(meshFaceMaterialPtr->GetID())) ;
								wstrPicPath.append(charID);
								wstrPicPath.append(L"_DiffuseTexture");
								wstrPicPath.append(wstrExt);

								wstring wstrImageDir = wstrOutputFullPath + L"\\images";
                                string strImageDir = M3D::Platform::WStringToString(wstrImageDir);
                                if (access(strImageDir.c_str(), 0) == -1)
								{
#ifdef WIN32
 	mkdir(strImageDir.c_str());
#else
	mkdir(strImageDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);							
#endif
								}


								wstrOutputFullPath.append(wstrPicPath);
                                std::ofstream fout(M3D::Platform::WStringToString(wstrOutputFullPath), std::ios::binary);
								fout.write(chBuf, sizeof(char) * iBufSize);
								fout.close();
								m_mapImageHashToPath.insert(make_pair(iHashValue, wstrPicPath));
							}
							free(chBuf);

                            HoteamSoft::SVLLib::Stk_ImagePtr imagePtr = pDocument->AddImage(bHasSet, M3D::Platform::WStringToString(wstrPicPath));
							imagePtr->SetNC(pStkPicture->GetNC());
							imagePtr->SetActualHeight(pStkPicture->GetActualHeight());
							imagePtr->SetActualWidth(pStkPicture->GetActualWidth());
							imagePtr->SetDisplayHeight(pStkPicture->GetDisplayHeight());
							imagePtr->SetDisplayWidth(pStkPicture->GetDisplayWidth());
							imagePtr->SetPictureSaveStyle(HoteamSoft::SVLLib::StkPicSaveStyleEnum::PICTRE_SAVE_STYLE_OUTER);
							imagePtr->SetPictureSavePath(wstrPicPath);
							imagePtr->SetScaleX(pStkPicture->GetScaleX());
							imagePtr->SetScaleY(pStkPicture->GetScaleY());
							HoteamSoft::SVLLib::Stk_TexturePtr texturePtr = HoteamSoft::SVLLib::Stk_TexturePtr::CreateObject();
							texturePtr->SetTextureImage(imagePtr);
							texturePtr->SetRotateAngle(pStkTexture->GetRotateAngle());
							texturePtr->SetUOffset(pStkTexture->GetUOffset());
							texturePtr->SetVOffset(pStkTexture->GetVOffset());
							texturePtr->SetUScale(pStkTexture->GetUScale());
							texturePtr->SetVScale(pStkTexture->GetVScale());
							texturePtr->SetFlipFlag(pStkTexture->IsUFlipped(), pStkTexture->IsVFlipped());
							texturePtr->SetType((HoteamSoft::SVLLib::StkTextureMappingTypeEnum)pStkTexture->GetType());
							meshFaceMaterialPtr->SetDiffuseTexture(texturePtr);
							m_mapTexturePtr.insert(make_pair(pStkTexture, texturePtr));
						}
					}
				}
			}

			meshFacePtr->SetMaterialID(meshFaceMaterialPtr->GetID());
		}
		else if (parentMaterialPtr .isNotNull())
		{
			meshFacePtr->SetMaterialID(parentMaterialPtr->GetID());
		}

		FaceLODInfo* pFaceLodInfo = pCurMeshFace->GetFaceLODInfo(0);
		if (pFaceLodInfo)
		{

			//for (int iFaceVtxIdx = 0; iFaceVtxIdx < pFaceLodInfo->TrianglesLOD.size(); iFaceVtxIdx++)
			//{
			//	vecTrians.push_back(pFaceLodInfo->TrianglesLOD[iFaceVtxIdx]);
			//}
			meshFacePtr->SetIndex(pFaceLodInfo->TrianglesLOD);
			//Face中为什么会存在EdgeID呢  去掉暂时没发现转换异常
			/*vector<STK_ID> vecMeshEdgeIDs = pCurMeshFace->GetMeshEdgeIDs();
			for (int m = 0; m < vecMeshEdgeIDs.size(); m++)
			{
				meshFacePtr->AddMeshEdgeID(vecMeshEdgeIDs[m]);
			}*/
		}

		//属性
		Stk_GeometryAttribute* pGeoAttr = pCurMeshFace->GetGeoAttr();
		if (pGeoAttr)
		{
			SVL1GeoAttrToSVL2MeshFaceAttr(pGeoAttr, pModel, meshFacePtr, pDocument);
		}
	}
	//pMeshPrim->SetIndex(vecTrians);

	//获取边的数量
	vector<STK_ID>vecMeshEdge = pSrcMeshCom->GetMeshEdgeIDs();
	int numMeshEdge = vecMeshEdge.size();
	for (int l = 0; l < numMeshEdge; l++)
	{
		Stk_Mesh_Edge* stkMeshEdge = pSrcMeshCom->GetMeshEdgeByID(vecMeshEdge[l]);
		if(!stkMeshEdge)
			continue;
		HoteamSoft::SVLLib::Stk_MeshEdgePtr meshedgePtr = pMeshPrim->AddMeshEdge(stkMeshEdge->GetID());
		map<STK_UINT32, vcEdgeInfo*> mapEdge = stkMeshEdge->GetAllEdgeLODInfo();
		vcEdgeInfo* pVecEdgeInfo = mapEdge.find(0)->second;
		int numEdgeIndex = pVecEdgeInfo->size();
		vector<unsigned int> vecEdgeIndex;
		for (int m = 0; m < numEdgeIndex; m++)
		{
			vecEdgeIndex.push_back(pVecEdgeInfo->at(m));
		}
		meshedgePtr->SetIndex(vecEdgeIndex);

		//属性
		Stk_GeometryAttribute* pGeoAttr = stkMeshEdge->GetGeoAttr();
		SVL1GeoAttrToSVL2MeshEdgeAttr(pGeoAttr, pModel,  meshedgePtr, pDocument);
	}

	vecVertices.clear();
	vecNormals.clear();
	//vecTrians.clear();

	return iRet;
}

int CSVL1ToSVL2::SVL1LineSetToSVL2(Stk_LineSet* pSrcLineSet, HoteamSoft::SVLLib::Stk_LineSetPtr pLineSetPtr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcLineSet || pLineSetPtr.isNull()|| pDocument .isNull())
	{
		return -1;
	}
	pLineSetPtr->SetID(pSrcLineSet->GetID());
	pLineSetPtr->SetName(pSrcLineSet->GetName());
	pLineSetPtr->SetDispStat(pSrcLineSet->GetDispStat());
	pLineSetPtr->SetType((HoteamSoft::SVLLib::StkLineSetTypeEnum)pSrcLineSet->GetType());
	STK_RGBA32 srcLineSetColor = pSrcLineSet->GetColor();
	HoteamSoft::SVLLib::STK_RGBA32 newLineSetColor;
	newLineSetColor.Red = srcLineSetColor.Red;
	newLineSetColor.Green = srcLineSetColor.Green;
	newLineSetColor.Blue = srcLineSetColor.Blue;
	newLineSetColor.Alpha = srcLineSetColor.Alpha;
	pLineSetPtr->SetColor(newLineSetColor);

	vector<Stk_CurveEntity*> vecCurve = pSrcLineSet->GetAllLines();
	for (int i = 0; i < vecCurve.size(); i++)
	{
		Stk_CurveEntity* pSrcCurve = vecCurve[i];
		if(!pSrcCurve)
			continue;
		HoteamSoft::SVLLib::Stk_CurveEntityPtr curveEntityPtr = SVL1CurveToSVL2(pSrcCurve, pDocument);
		if (curveEntityPtr .isNotNull())
		{
			pLineSetPtr->AddLine(curveEntityPtr);
		}
	}
    return iRet;
}

int CSVL1ToSVL2::SVL1SpacialPointToSVL2(Stk_SpacialPointEntity* pSrcSpacialPoint, HoteamSoft::SVLLib::Stk_SpacialPointEntityPtr pSpacialPointPtr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcSpacialPoint || pSpacialPointPtr.isNull()|| pDocument .isNull())
	{
		return -1;
	}
	pSpacialPointPtr->SetName(pSrcSpacialPoint->GetName());
	pSpacialPointPtr->SetDispStat(pSrcSpacialPoint->GetDispStat());
	pSpacialPointPtr->SetType((HoteamSoft::SVLLib::StkSpacialPointTypeEnum)pSrcSpacialPoint->GetType());
	STK_RGBA32 srcSpacialPointColor = pSrcSpacialPoint->GetColor();
	HoteamSoft::SVLLib::STK_RGBA32 newSpacialPointColor;
	newSpacialPointColor.Red = srcSpacialPointColor.Red;
	newSpacialPointColor.Green = srcSpacialPointColor.Green;
	newSpacialPointColor.Blue = srcSpacialPointColor.Blue;
	newSpacialPointColor.Alpha = srcSpacialPointColor.Alpha;
	pSpacialPointPtr->SetColor(newSpacialPointColor);

	STK_PNT32 pntPos = pSrcSpacialPoint->GetPntPosition();
	HoteamSoft::SVLLib::STK_PNT32 pntPos2;
	pntPos2.X = pntPos.X;
	pntPos2.Y = pntPos.Y;
	pntPos2.Z = pntPos.Z;
	pSpacialPointPtr->SetPntPosition(pntPos2);
    return iRet;
}

HoteamSoft::SVLLib::Stk_CurveEntityPtr CSVL1ToSVL2::SVL1CurveToSVL2(Stk_CurveEntity* pSrcCurve, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	HoteamSoft::SVLLib::Stk_CurveEntityPtr retCurveEntityPtr = NULL;
	if (!pSrcCurve || pDocument .isNull())
	{
		return retCurveEntityPtr;
	}
	
	StkCurveTypeEnum curveType = pSrcCurve->GetCurveType();
	switch (curveType)
	{
	case CURVE_TYPE_UNKNOWN:
		break;
	case CURVE_TYPE_POLYLINE:
	{
		HoteamSoft::SVLLib::Stk_PolyLinePtr stkPolyLinePtr = HoteamSoft::SVLLib::Stk_PolyLinePtr::CreateObject();
		stkPolyLinePtr->SetCurveDispStat(pSrcCurve->GetCurveDispStat());
		stkPolyLinePtr->SetCurveDispType((HoteamSoft::SVLLib::StkCurveDispTypeEnum)pSrcCurve->GetCurveDispType());
		STK_RGBA32 srcCurveColor = pSrcCurve->GetColor();
		HoteamSoft::SVLLib::STK_RGBA32 newCurveColor;
		newCurveColor.Red = srcCurveColor.Red;
		newCurveColor.Green = srcCurveColor.Green;
		newCurveColor.Blue = srcCurveColor.Blue;
		newCurveColor.Alpha = srcCurveColor.Alpha;
		stkPolyLinePtr->SetColor(newCurveColor);

		//???????
		STK_FLOAT32 fUMin = 0.0f, fUMax = 0.0f;
		((Stk_PolyLine*)pSrcCurve)->GetDomain(fUMin, fUMax);
		stkPolyLinePtr->SetRange(fUMin, fUMax);

		stkPolyLinePtr->SetPoints((vector<HoteamSoft::SVLLib::STK_PNT32>&)((Stk_PolyLine*)pSrcCurve)->GetPoints());

		retCurveEntityPtr = stkPolyLinePtr;
	}
	break;
	case CURVE_TYPE_ELLIPSE:
	{
		HoteamSoft::SVLLib::Stk_ArcPtr stkEllipsePtr = HoteamSoft::SVLLib::Stk_ArcPtr::CreateObject();
		stkEllipsePtr->SetCurveDispStat(pSrcCurve->GetCurveDispStat());
		stkEllipsePtr->SetCurveDispType((HoteamSoft::SVLLib::StkCurveDispTypeEnum)pSrcCurve->GetCurveDispType());
		STK_RGBA32 srcCurveColor = pSrcCurve->GetColor();
		HoteamSoft::SVLLib::STK_RGBA32 newCurveColor;
		newCurveColor.Red = srcCurveColor.Red;
		newCurveColor.Green = srcCurveColor.Green;
		newCurveColor.Blue = srcCurveColor.Blue;
		newCurveColor.Alpha = srcCurveColor.Alpha;
		stkEllipsePtr->SetColor(newCurveColor);

		//特有信息
		STK_FLOAT32 fUMin = 0.0f, fUMax = 0.0f;
		((Stk_Ellipse*)pSrcCurve)->GetDomain(fUMin, fUMax);
		stkEllipsePtr->SetRange(fUMin, fUMax);

		STK_PNT32 centerPnt = ((Stk_Ellipse*)pSrcCurve)->GetCenterPoint();
		HoteamSoft::SVLLib::STK_PNT32 centerPnt2;
		centerPnt2.X = centerPnt.X;
		centerPnt2.Y = centerPnt.Y;
		centerPnt2.Z = centerPnt.Z;
		stkEllipsePtr->SetCenterPoint(centerPnt2);

		STK_DIR32 dirX, dirY, dirZ;
		((Stk_Ellipse*)pSrcCurve)->GetXYZDir(dirX, dirY, dirZ);
		HoteamSoft::SVLLib::STK_DIR32 dirX2, dirY2, dirZ2;
		dirX2.X = dirX.X;
		dirX2.Y = dirX.Y;
		dirX2.Z = dirX.Z;
		dirY2.X = dirY.X;
		dirY2.Y = dirY.Y;
		dirY2.Z = dirY.Z;
		dirZ2.X = dirZ.X;
		dirZ2.Y = dirZ.Y;
		dirZ2.Z = dirZ.Z;
		stkEllipsePtr->SetXYZDir(dirX2, dirY2, dirZ2);

		STK_FLOAT32 fMajorR, fMinorR;
		((Stk_Ellipse*)pSrcCurve)->GetRadius(fMajorR, fMinorR);
		stkEllipsePtr->SetRadius(fMajorR, fMinorR);

		STK_PNT32 startPnt, endPnt;
		((Stk_Ellipse*)pSrcCurve)->GetCoordinatePnt(startPnt, endPnt);
		HoteamSoft::SVLLib::STK_PNT32 startPnt2, endPnt2;
		startPnt2.X = startPnt.X;
		startPnt2.Y = startPnt.Y;
		startPnt2.Z = startPnt.Z;
		endPnt2.X = endPnt.X;
		endPnt2.Y = endPnt.Y;
		endPnt2.Z = endPnt.Z;
		stkEllipsePtr->SetCoordiantePnt(startPnt2, endPnt2);

		STK_FLOAT32 fStartPar, fEndPar;
		((Stk_Ellipse*)pSrcCurve)->GetParameter(fStartPar, fEndPar);
		stkEllipsePtr->SetParameter(fStartPar, fEndPar);

		STK_DIR32 dirNormal = ((Stk_Ellipse*)pSrcCurve)->GetNormal();
		HoteamSoft::SVLLib::STK_DIR32 dirNormal2;
		dirNormal2.X = dirNormal.X;
		dirNormal2.Y = dirNormal.Y;
		dirNormal2.Z = dirNormal.Z;
		stkEllipsePtr->SetNormal(dirNormal2);

		STK_DIR32 dirOrigin = ((Stk_Ellipse*)pSrcCurve)->GetOriginDir();
		HoteamSoft::SVLLib::STK_DIR32 dirOrigin2;
		dirOrigin2.X = dirOrigin.X;
		dirOrigin2.Y = dirOrigin.Y;
		dirOrigin2.Z = dirOrigin.Z;
		stkEllipsePtr->SetOriginDir(dirOrigin2);

		retCurveEntityPtr = stkEllipsePtr;
	}
	break;
	case CURVE_TYPE_LINE:
	{
		HoteamSoft::SVLLib::Stk_LinePtr stkLinePtr = HoteamSoft::SVLLib::Stk_LinePtr::CreateObject();
		stkLinePtr->SetCurveDispStat(pSrcCurve->GetCurveDispStat());
		stkLinePtr->SetCurveDispType((HoteamSoft::SVLLib::StkCurveDispTypeEnum)pSrcCurve->GetCurveDispType());
		STK_RGBA32 srcCurveColor = pSrcCurve->GetColor();
		HoteamSoft::SVLLib::STK_RGBA32 newCurveColor;
		newCurveColor.Red = srcCurveColor.Red;
		newCurveColor.Green = srcCurveColor.Green;
		newCurveColor.Blue = srcCurveColor.Blue;
		newCurveColor.Alpha = srcCurveColor.Alpha;
		stkLinePtr->SetColor(newCurveColor);

		//特有信息
		STK_PNT32 startPnt = ((Stk_Line*)pSrcCurve)->GetStartPoint();
		HoteamSoft::SVLLib::STK_PNT32 startPnt2;
		startPnt2.X = startPnt.X;
		startPnt2.Y = startPnt.Y;
		startPnt2.Z = startPnt.Z;
		stkLinePtr->SetStartPoint(startPnt2);

		STK_PNT32 endPnt = ((Stk_Line*)pSrcCurve)->GetEndPoint();
		HoteamSoft::SVLLib::STK_PNT32 endPnt2;
		endPnt2.X = endPnt.X;
		endPnt2.Y = endPnt.Y;
		endPnt2.Z = endPnt.Z;
		stkLinePtr->SetEndPoint(endPnt2);

		retCurveEntityPtr = stkLinePtr;
	}
	break;
	case CURVE_TYPE_NURBSCURVE:
	{
		HoteamSoft::SVLLib::Stk_NurbsCurvePtr stkNurbsCurvePtr = HoteamSoft::SVLLib::Stk_NurbsCurvePtr::CreateObject();
		stkNurbsCurvePtr->SetCurveDispStat(pSrcCurve->GetCurveDispStat());
		stkNurbsCurvePtr->SetCurveDispType((HoteamSoft::SVLLib::StkCurveDispTypeEnum)pSrcCurve->GetCurveDispType());
		STK_RGBA32 srcCurveColor = pSrcCurve->GetColor();
		HoteamSoft::SVLLib::STK_RGBA32 newCurveColor;
		newCurveColor.Red = srcCurveColor.Red;
		newCurveColor.Green = srcCurveColor.Green;
		newCurveColor.Blue = srcCurveColor.Blue;
		newCurveColor.Alpha = srcCurveColor.Alpha;
		stkNurbsCurvePtr->SetColor(newCurveColor);

		//特有信息
		STK_FLOAT32 fUMin = 0.0f, fUMax = 0.0f;
		((Stk_NurbsCurve*)pSrcCurve)->GetDomain(fUMin, fUMax);
		stkNurbsCurvePtr->SetDomain(fUMin, fUMax);

		STK_UINT32 iDegree = ((Stk_NurbsCurve*)pSrcCurve)->GetDegree();
		stkNurbsCurvePtr->SetDegree(iDegree);

		vector<STK_UINT32> vecKnotMultiplicity = ((Stk_NurbsCurve*)pSrcCurve)->GetKnotMultiplicity();
		for (int j = 0; j < vecKnotMultiplicity.size(); j++)
		{
			stkNurbsCurvePtr->AddKnotMultiplicity(vecKnotMultiplicity[j]);
		}

		vector<STK_FLOAT32> vecKnots = ((Stk_NurbsCurve*)pSrcCurve)->GetKnots();
		for (int j = 0; j < vecKnots.size(); j++)
		{
			stkNurbsCurvePtr->AddKnot(vecKnots[j]);
		}

		vector<STK_PNT32> vecPoints = ((Stk_NurbsCurve*)pSrcCurve)->GetPoints();
		for (int j = 0; j < vecPoints.size(); j++)
		{
			HoteamSoft::SVLLib::STK_PNT32 points2;
			points2.X = vecPoints[j].X;
			points2.Y = vecPoints[j].Y;
			points2.Z = vecPoints[j].Z;
			stkNurbsCurvePtr->AddPoint(points2);
		}

		vector<STK_FLOAT32> vecWeights = ((Stk_NurbsCurve*)pSrcCurve)->GetWeights();
		for (int j = 0; j < vecWeights.size(); j++)
		{
			stkNurbsCurvePtr->AddWeight(vecWeights[j]);
		}

		retCurveEntityPtr = stkNurbsCurvePtr;
	}
	break;
	case CURVE_TYPE_HYPERBOLA:
	{
		HoteamSoft::SVLLib::Stk_HyperbolaPtr stkHyperbolaPtr = HoteamSoft::SVLLib::Stk_HyperbolaPtr::CreateObject();
		stkHyperbolaPtr->SetCurveDispStat(pSrcCurve->GetCurveDispStat());
		stkHyperbolaPtr->SetCurveDispType((HoteamSoft::SVLLib::StkCurveDispTypeEnum)pSrcCurve->GetCurveDispType());
		STK_RGBA32 srcCurveColor = pSrcCurve->GetColor();
		HoteamSoft::SVLLib::STK_RGBA32 newCurveColor;
		newCurveColor.Red = srcCurveColor.Red;
		newCurveColor.Green = srcCurveColor.Green;
		newCurveColor.Blue = srcCurveColor.Blue;
		newCurveColor.Alpha = srcCurveColor.Alpha;
		stkHyperbolaPtr->SetColor(newCurveColor);

		//特有信息
		STK_FLOAT32 fUMin = 0.0f, fUMax = 0.0f;
		((Stk_Hyperbola*)pSrcCurve)->GetDomain(fUMin, fUMax);
		stkHyperbolaPtr->SetDomain(fUMin, fUMax);

		STK_PNT32 centerPnt = ((Stk_Hyperbola*)pSrcCurve)->GetCenterPoint();
		HoteamSoft::SVLLib::STK_PNT32 centerPnt2;
		centerPnt2.X = centerPnt.X;
		centerPnt2.Y = centerPnt.Y;
		centerPnt2.Z = centerPnt.Z;
		stkHyperbolaPtr->SetCenterPoint(centerPnt2);

		STK_DIR32 dirX, dirY, dirZ;
		((Stk_Hyperbola*)pSrcCurve)->GetXYZDir(dirX, dirY, dirZ);
		HoteamSoft::SVLLib::STK_DIR32 dirX2, dirY2, dirZ2;
		dirX2.X = dirX.X;
		dirX2.Y = dirX.Y;
		dirX2.Z = dirX.Z;
		dirY2.X = dirY.X;
		dirY2.Y = dirY.Y;
		dirY2.Z = dirY.Z;
		dirZ2.X = dirZ.X;
		dirZ2.Y = dirZ.Y;
		dirZ2.Z = dirZ.Z;
		stkHyperbolaPtr->SetXYZDir(dirX2, dirY2, dirZ2);

		STK_FLOAT32 fSemiAxis, fSemiImgAxis;
		fSemiAxis = ((Stk_Hyperbola*)pSrcCurve)->GetSemiAxis();
		stkHyperbolaPtr->SetSemiAxis(fSemiAxis);
		fSemiImgAxis = ((Stk_Hyperbola*)pSrcCurve)->GetSemiImgAxis();
		stkHyperbolaPtr->SetSemiImgAxis(fSemiImgAxis);

		STK_PNT32 startPnt, endPnt;
		startPnt = ((Stk_Hyperbola*)pSrcCurve)->GetStartPoint();
		endPnt = ((Stk_Hyperbola*)pSrcCurve)->GetEndPoint();
		HoteamSoft::SVLLib::STK_PNT32 startPnt2, endPnt2;
		startPnt2.X = startPnt.X;
		startPnt2.Y = startPnt.Y;
		startPnt2.Z = startPnt.Z;
		endPnt2.X = endPnt.X;
		endPnt2.Y = endPnt.Y;
		endPnt2.Z = endPnt.Z;
		stkHyperbolaPtr->SetStartPoint(startPnt2);
		stkHyperbolaPtr->SetEndPoint(endPnt2);

		retCurveEntityPtr = stkHyperbolaPtr;
	}
	break;
	case CURVE_TYPE_PARABOLA:
	{
		HoteamSoft::SVLLib::Stk_ParabolaPtr stkParabolaPtr = HoteamSoft::SVLLib::Stk_ParabolaPtr::CreateObject();
		stkParabolaPtr->SetCurveDispStat(pSrcCurve->GetCurveDispStat());
		stkParabolaPtr->SetCurveDispType((HoteamSoft::SVLLib::StkCurveDispTypeEnum)pSrcCurve->GetCurveDispType());
		STK_RGBA32 srcCurveColor = pSrcCurve->GetColor();
		HoteamSoft::SVLLib::STK_RGBA32 newCurveColor;
		newCurveColor.Red = srcCurveColor.Red;
		newCurveColor.Green = srcCurveColor.Green;
		newCurveColor.Blue = srcCurveColor.Blue;
		newCurveColor.Alpha = srcCurveColor.Alpha;
		stkParabolaPtr->SetColor(newCurveColor);

		//特有信息
		STK_FLOAT32 fUMin = 0.0f, fUMax = 0.0f;
		((Stk_Parabola*)pSrcCurve)->GetDomain(fUMin, fUMax);
		stkParabolaPtr->SetDomain(fUMin, fUMax);

		STK_PNT32 centerPnt = ((Stk_Parabola*)pSrcCurve)->GetCenterPoint();
		HoteamSoft::SVLLib::STK_PNT32 centerPnt2;
		centerPnt2.X = centerPnt.X;
		centerPnt2.Y = centerPnt.Y;
		centerPnt2.Z = centerPnt.Z;
		stkParabolaPtr->SetCenterPoint(centerPnt2);

		STK_DIR32 dirX, dirY, dirZ;
		((Stk_Parabola*)pSrcCurve)->GetXYZDir(dirX, dirY, dirZ);
		HoteamSoft::SVLLib::STK_DIR32 dirX2, dirY2, dirZ2;
		dirX2.X = dirX.X;
		dirX2.Y = dirX.Y;
		dirX2.Z = dirX.Z;
		dirY2.X = dirY.X;
		dirY2.Y = dirY.Y;
		dirY2.Z = dirY.Z;
		dirZ2.X = dirZ.X;
		dirZ2.Y = dirZ.Y;
		dirZ2.Z = dirZ.Z;
		stkParabolaPtr->SetXYZDir(dirX2, dirY2, dirZ2);

		STK_PNT32 startPnt, endPnt;
		startPnt = ((Stk_Parabola*)pSrcCurve)->GetStartPoint();
		endPnt = ((Stk_Parabola*)pSrcCurve)->GetEndPoint();
		HoteamSoft::SVLLib::STK_PNT32 startPnt2, endPnt2;
		startPnt2.X = startPnt.X;
		startPnt2.Y = startPnt.Y;
		startPnt2.Z = startPnt.Z;
		endPnt2.X = endPnt.X;
		endPnt2.Y = endPnt.Y;
		endPnt2.Z = endPnt.Z;
		stkParabolaPtr->SetStartPoint(startPnt2);
		stkParabolaPtr->SetEndPoint(endPnt2);

		STK_FLOAT32 fFocalDistance;
		fFocalDistance = ((Stk_Parabola*)pSrcCurve)->GetFocalDistance();
		stkParabolaPtr->SetFocalDistance(fFocalDistance);

		retCurveEntityPtr = stkParabolaPtr;
	}
	break;
	default:
		break;
	}
	if (retCurveEntityPtr .isNotNull())
	{
		retCurveEntityPtr->SetID(pSrcCurve->GetID());
	}
	return retCurveEntityPtr;
}

int CSVL1ToSVL2::SVL1PMIToSVL2(Stk_PMI* pSrcPMI, HoteamSoft::SVLLib::Stk_PMIPtr pNewPMI,
	HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcPMI || pNewPMI.isNull()|| pDocument .isNull())
		return -1;
	m_mapOldPmiIDToNew.insert(make_pair(pSrcPMI->GetID(), pNewPMI->GetID()));
	pNewPMI->SetName(pSrcPMI->GetName());
	pNewPMI->SetType((HoteamSoft::SVLLib::StkPMITypeEnum)pSrcPMI->GetType());
	//defPlane
	HoteamSoft::SVLLib::STK_MTX32 newMtx;
	STK_MTX32 srcMtx = pSrcPMI->GetDefPlane();
	for (int i = 0; i < MTX_LINE_COUNT; i++)
	{
		for (int j = 0; j < MTX_COL_COUNT; j++)
		{
			newMtx.PlcMatrix[i][j] = srcMtx.PlcMatrix[i][j];
		}
	}
	pNewPMI->SetDefPlane(newMtx);

	//color
	STK_RGBA32 srcColor = pSrcPMI->GetColor();
	HoteamSoft::SVLLib::STK_RGBA32 newColor;
	newColor.Red = srcColor.Red;
	newColor.Green = srcColor.Green;
	newColor.Blue = srcColor.Blue;
	newColor.Alpha = srcColor.Alpha;
	pNewPMI->SetColor(newColor);

	int iStyle = 0x0;
	iStyle += (pSrcPMI->GetNode()->GetDispStat() ? 0x00000001 : 0x00000000);
	iStyle += (pSrcPMI->IsParallelScreen() ? 0x00000100 : 0x00000000);
	iStyle += (pSrcPMI->IsFix() ? 0x00001000 : 0x00000000);
	iStyle += (pSrcPMI->IsOldVersion() ? 0x00010000 : 0x00000000);
	iStyle += (pSrcPMI->IsAdjusted() ? 0x00100000 : 0x00000000);
	iStyle += (pSrcPMI->IsFront() ? 0x01000000 : 0x00000000);
	pNewPMI->SetStyle(iStyle);
	pNewPMI->SetPMISource((HoteamSoft::SVLLib::StkPMISourceEnum)pSrcPMI->GetPMISource());
	
	//dirX
	STK_DIR32 dirX = pSrcPMI->GetAxisX();
	HoteamSoft::SVLLib::STK_DIR32 newDirX;
	newDirX.X = dirX.X;
	newDirX.Y = dirX.Y;
	newDirX.Z = dirX.Z;
	pNewPMI->SetAxisX(newDirX);

	//dirY
	STK_DIR32 dirY = pSrcPMI->GetAxisY();
	HoteamSoft::SVLLib::STK_DIR32 newDirY;
	newDirY.X = dirY.X;
	newDirY.Y = dirY.Y;
	newDirY.Z = dirY.Z;
	pNewPMI->SetAxisY(newDirY);

	//symBasePnt
	STK_PNT32 srcPnt = pSrcPMI->GetSymBasePoint();
	HoteamSoft::SVLLib::STK_PNT32 newPnt;
	newPnt.X = srcPnt.X;
	newPnt.Y = srcPnt.Y;
	newPnt.Z = srcPnt.Z;
	pNewPMI->SetSymBasePoint(newPnt);

	//leaders
	vector<Stk_Leader*> vcLeaders = pSrcPMI->GetLeaders();
	vector<Stk_Leader*>::iterator itorLeader = vcLeaders.begin();
	while (itorLeader != vcLeaders.end())
	{
		HoteamSoft::SVLLib::Stk_LeaderPtr leaderP = HoteamSoft::SVLLib::Stk_LeaderPtr::CreateObject();
		SVL1LeaderToSVL2(*itorLeader, leaderP, pDocument);
		pNewPMI->AddLeader(leaderP);
		itorLeader++;
	}
	//exLines
	vector<Stk_Extensionline*> vcExlines = pSrcPMI->GetExLines();
	vector<Stk_Extensionline*>::iterator itorExlines = vcExlines.begin();
	while (itorExlines != vcExlines.end())
	{
		HoteamSoft::SVLLib::Stk_ExtensionlinePtr exLineP = HoteamSoft::SVLLib::Stk_ExtensionlinePtr::CreateObject();
		SVL1ExlineToSVL2(*itorExlines, exLineP, pDocument);
		pNewPMI->AddExline(exLineP);
		itorExlines++;
	}
	//comTexts
	vector<Stk_ComText*> vcComTexts = pSrcPMI->GetComText();
	vector<Stk_ComText*>::iterator itorComText = vcComTexts.begin();
	while (itorComText != vcComTexts.end())
	{
		HoteamSoft::SVLLib::Stk_ComTextPtr comTextsP = HoteamSoft::SVLLib::Stk_ComTextPtr::CreateObject();
		SVL1ComTextToSVL2(*itorComText, comTextsP, pDocument);
		pNewPMI->AddComText(comTextsP);
		itorComText++;
	}
	//outFrames
	vector<Stk_OutFrame*> vcOutFrames = pSrcPMI->GetOutFrame();
	vector<Stk_OutFrame*>::iterator itorOutFrame = vcOutFrames.begin();
	while (itorOutFrame != vcOutFrames.end())
	{
		HoteamSoft::SVLLib::Stk_OutFramePtr outFrameP = HoteamSoft::SVLLib::Stk_OutFramePtr::CreateObject();
		SVL1OutFrameToSVL2(*itorOutFrame, outFrameP, pDocument);
		pNewPMI->SetOutFrame(outFrameP);
		itorOutFrame++;
	}
	//specialLines
	vector<Stk_SpecialLine*> vcSpecialLine = pSrcPMI->GetSpecialLines();
	vector<Stk_SpecialLine*>::iterator itorSpecialLine = vcSpecialLine.begin();
	while (itorSpecialLine != vcSpecialLine.end())
	{
		HoteamSoft::SVLLib::Stk_SpecialLinePtr specialLineP = HoteamSoft::SVLLib::Stk_SpecialLinePtr::CreateObject();
		SVL1SpecialLineToSVL2(*itorSpecialLine, specialLineP, pDocument);
		pNewPMI->AddSpecialLine(specialLineP);
		itorSpecialLine++;
	}
	//images
	int iNumPicture = pSrcPMI->GetPictureNum();
	for (int i = 0; i < iNumPicture; i++)
	{
		Stk_Picture* pPicture =  pSrcPMI->GetPictureByIndex(i);
		HoteamSoft::SVLLib::Stk_ImagePtr imageP = HoteamSoft::SVLLib::Stk_ImagePtr::CreateObject();
		SVL1ImageToSVL2(pPicture, imageP, pDocument);
		pNewPMI->AddImage(imageP);
	}
	//userdatas
	//Animation
	map<STK_ID, Stk_UserData*>* pMapAnimation = pSrcPMI->GetUserDataByName(USERDATA_NAME_ANIMATION);
	if (pMapAnimation)
	{
		map<STK_ID, Stk_UserData*>::iterator  itorAnimation = pMapAnimation->begin();
		while (itorAnimation != pMapAnimation->end())
		{
			Stk_UserData* pUserData = itorAnimation->second;
			HoteamSoft::SVLLib::Stk_UserDataPtr userdataP = HoteamSoft::SVLLib::Stk_UserDataPtr::CreateObject();
			SVL1UserDataToSVL2(pUserData, userdataP, pDocument);
			pNewPMI->AddUserData(userdataP);
			itorAnimation++;
		}
	}
	//UserNote
	map<STK_ID, Stk_UserData*>* pMapUserNote = pSrcPMI->GetUserDataByName(USERDATA_NAME_USERNOTE);
	if (pMapUserNote)
	{
		map<STK_ID, Stk_UserData*>::iterator  itorUserNote = pMapUserNote->begin();
		while (itorUserNote != pMapUserNote->end())
		{
			Stk_UserData* pUserData = itorUserNote->second;
			HoteamSoft::SVLLib::Stk_UserDataPtr userdataP = HoteamSoft::SVLLib::Stk_UserDataPtr::CreateObject();
			SVL1UserDataToSVL2(pUserData, userdataP, pDocument);
			pNewPMI->AddUserData(userdataP);
			itorUserNote++;
		}
	}
	//CableInfo
	map<STK_ID, Stk_UserData*>* pMapCableInfo = pSrcPMI->GetUserDataByName(USERDATA_NAME_CABLEINFO);
	if (pMapCableInfo)
	{
		map<STK_ID, Stk_UserData*>::iterator  itorCableInfo = pMapCableInfo->begin();
		while (itorCableInfo != pMapCableInfo->end())
		{
			Stk_UserData* pUserData = itorCableInfo->second;
			HoteamSoft::SVLLib::Stk_UserDataPtr userdataP = HoteamSoft::SVLLib::Stk_UserDataPtr::CreateObject();
			SVL1UserDataToSVL2(pUserData, userdataP, pDocument);
			pNewPMI->AddUserData(userdataP);
			itorCableInfo++;
		}
	}
	//VBOM
	map<STK_ID, Stk_UserData*>* pMapVbom = pSrcPMI->GetUserDataByName(USERDATA_NAME_VBOM);
	if (pMapVbom)
	{
		map<STK_ID, Stk_UserData*>::iterator  itorVbom = pMapVbom->begin();
		while (itorVbom != pMapVbom->end())
		{
			Stk_UserData* pUserData = itorVbom->second;
			HoteamSoft::SVLLib::Stk_UserDataPtr userdataP = HoteamSoft::SVLLib::Stk_UserDataPtr::CreateObject();
			SVL1UserDataToSVL2(pUserData, userdataP, pDocument);
			pNewPMI->AddUserData(userdataP);
			itorVbom++;
		}
	}
	//ARROWDATA
	map<STK_ID, Stk_UserData*>* pMapArrowData = pSrcPMI->GetUserDataByName(USERDATA_NAME_ARROWDATA);
	if (pMapArrowData)
	{
		map<STK_ID, Stk_UserData*>::iterator  itorArrowData = pMapArrowData->begin();
		while (itorArrowData != pMapArrowData->end())
		{
			Stk_UserData* pUserData = itorArrowData->second;
			HoteamSoft::SVLLib::Stk_UserDataPtr userdataP = HoteamSoft::SVLLib::Stk_UserDataPtr::CreateObject();
			SVL1UserDataToSVL2(pUserData, userdataP, pDocument);
			pNewPMI->AddUserData(userdataP);
			itorArrowData++;
		}
	}
	//ARROWLIST
	map<STK_ID, Stk_UserData*>* pMapArrowList = pSrcPMI->GetUserDataByName(USERDATA_NAME_ARROWLIST);
	if (pMapArrowList)
	{
		map<STK_ID, Stk_UserData*>::iterator  itorArrowList = pMapArrowList->begin();
		while (itorArrowList != pMapArrowList->end())
		{
			Stk_UserData* pUserData = itorArrowList->second;
			HoteamSoft::SVLLib::Stk_UserDataPtr userdataP = HoteamSoft::SVLLib::Stk_UserDataPtr::CreateObject();
			SVL1UserDataToSVL2(pUserData, userdataP, pDocument);
			pNewPMI->AddUserData(userdataP);
			itorArrowList++;
		}
	}
	//HotSpot
	map<STK_ID, Stk_UserData*>* pMapHotSpot = pSrcPMI->GetUserDataByName(USERDATA_NAME_HOTSPOT);
	if (pMapHotSpot)
	{
		map<STK_ID, Stk_UserData*>::iterator  itorHotSpot = pMapHotSpot->begin();
		while (itorHotSpot != pMapHotSpot->end())
		{
			Stk_UserData* pUserData = itorHotSpot->second;
			HoteamSoft::SVLLib::Stk_UserDataPtr userdataP = HoteamSoft::SVLLib::Stk_UserDataPtr::CreateObject();
			SVL1UserDataToSVL2(pUserData, userdataP, pDocument);
			pNewPMI->AddUserData(userdataP);
			itorHotSpot++;
		}
	}
	//ServiceManual
	map<STK_ID, Stk_UserData*>* pMapServiceManual = pSrcPMI->GetUserDataByName(USERDATA_NAME_SERVICEMANUAL);
	if (pMapServiceManual)
	{
		map<STK_ID, Stk_UserData*>::iterator  itorServiceManual = pMapServiceManual->begin();
		while (itorServiceManual != pMapServiceManual->end())
		{
			Stk_UserData* pUserData = itorServiceManual->second;
			HoteamSoft::SVLLib::Stk_UserDataPtr userdataP = HoteamSoft::SVLLib::Stk_UserDataPtr::CreateObject();
			SVL1UserDataToSVL2(pUserData, userdataP, pDocument);
			pNewPMI->AddUserData(userdataP);
			itorServiceManual++;
		}
	}
	//DebugInfo
	map<STK_ID, Stk_UserData*>* pMapDebugInfo = pSrcPMI->GetUserDataByName(USERDATA_NAME_DEBUGINFO);
	if (pMapDebugInfo)
	{
		map<STK_ID, Stk_UserData*>::iterator  itorDebugInfo = pMapDebugInfo->begin();
		while (itorDebugInfo != pMapDebugInfo->end())
		{
			Stk_UserData* pUserData = itorDebugInfo->second;
			HoteamSoft::SVLLib::Stk_UserDataPtr userdataP = HoteamSoft::SVLLib::Stk_UserDataPtr::CreateObject();
			SVL1UserDataToSVL2(pUserData, userdataP, pDocument);
			pNewPMI->AddUserData(userdataP);
			itorDebugInfo++;
		}
	}
	//DefaultPoint
	map<STK_ID, Stk_UserData*>* pMapDefaultPoint = pSrcPMI->GetUserDataByName(USERDATA_NAME_DEFAULTVIEWID);
	if (pMapDefaultPoint)
	{
		map<STK_ID, Stk_UserData*>::iterator  itorDefaultPoint = pMapDefaultPoint->begin();
		while (itorDefaultPoint != pMapDefaultPoint->end())
		{
			Stk_UserData* pUserData = itorDefaultPoint->second;
			HoteamSoft::SVLLib::Stk_UserDataPtr userdataP = HoteamSoft::SVLLib::Stk_UserDataPtr::CreateObject();
			SVL1UserDataToSVL2(pUserData, userdataP, pDocument);
			pNewPMI->AddUserData(userdataP);
			itorDefaultPoint++;
		}
	}
	//AnchorPoint
	map<STK_ID, Stk_UserData*>* pMapAnchorPoint = pSrcPMI->GetUserDataByName(USERDATA_NAME_ANCHORPOINT);
	if (pMapAnchorPoint)
	{
		map<STK_ID, Stk_UserData*>::iterator  itorAnchorPoint = pMapAnchorPoint->begin();
		while (itorAnchorPoint != pMapAnchorPoint->end())
		{
			Stk_UserData* pUserData = itorAnchorPoint->second;
			HoteamSoft::SVLLib::Stk_UserDataPtr userdataP = HoteamSoft::SVLLib::Stk_UserDataPtr::CreateObject();
			SVL1UserDataToSVL2(pUserData, userdataP, pDocument);
			pNewPMI->AddUserData(userdataP);
			itorAnchorPoint++;
		}
	}
	//CHECKID
	map<STK_ID, Stk_UserData*>* pMapCheckID = pSrcPMI->GetUserDataByName(USERDATA_NAME_CHECKID);
	if (pMapCheckID)
	{
		map<STK_ID, Stk_UserData*>::iterator  itorCheckID = pMapCheckID->begin();
		while (itorCheckID != pMapCheckID->end())
		{
			Stk_UserData* pUserData = itorCheckID->second;
			HoteamSoft::SVLLib::Stk_UserDataPtr userdataP = HoteamSoft::SVLLib::Stk_UserDataPtr::CreateObject();
			SVL1UserDataToSVL2(pUserData, userdataP, pDocument);
			pNewPMI->AddUserData(userdataP);
			itorCheckID++;
		}
	}
	return iRet;
}

int CSVL1ToSVL2::SVL1LeaderToSVL2(Stk_Leader* pSrcLeader, HoteamSoft::SVLLib::Stk_LeaderPtr pNewLeader,
	HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcLeader || pNewLeader.isNull()|| pDocument .isNull())
		return -1;
	pNewLeader->SetLeaderType((HoteamSoft::SVLLib::StkLeaderTypeEnum)pSrcLeader->GetLeaderType());
	
	//引出线末端
	StkTermTypeEnum srcTermType;
	STK_FLOAT32 size[2];
	STK_PNT32 pntLoc;
	STK_DIR32 dirTerm;
	pSrcLeader->GetTerminator(srcTermType, size, pntLoc, dirTerm);
	HoteamSoft::SVLLib::STK_PNT32 newPntLoc;
	newPntLoc.X = pntLoc.X;
	newPntLoc.Y = pntLoc.Y;
	newPntLoc.Z = pntLoc.Z;
	HoteamSoft::SVLLib::STK_DIR32 newDirTerm;
	newDirTerm.X = dirTerm.X;
	newDirTerm.Y = dirTerm.Y;
	newDirTerm.Z = dirTerm.Z;
	pNewLeader->SetTerminator((HoteamSoft::SVLLib::StkTermTypeEnum)srcTermType, size, newPntLoc, newDirTerm);
	pNewLeader->SetBendLineFlag(pSrcLeader->IsStubDelta());
	
	vector<Stk_CurveEntity*> vcCurveEntity = pSrcLeader->GetAllCurve();
	vector<Stk_CurveEntity*>::iterator itorCurveEntity = vcCurveEntity.begin();
	while (itorCurveEntity != vcCurveEntity.end())
	{
		StkCurveTypeEnum eCurveType = (*itorCurveEntity)->GetCurveType();
		if (eCurveType == StkCurveTypeEnum::CURVE_TYPE_LINE)
		{
			Stk_Line* pSrcLine = (Stk_Line*)(*itorCurveEntity);
			HoteamSoft::SVLLib::Stk_LinePtr pNewCurveEntity = HoteamSoft::SVLLib::Stk_LinePtr::CreateObject();
			SVL1LineToSVL2(pSrcLine, pNewCurveEntity, pDocument);
			pNewLeader->AddCurve(pNewCurveEntity);
		}
		else if (eCurveType == StkCurveTypeEnum::CURVE_TYPE_ELLIPSE)
		{
			Stk_Ellipse* pSrcEllipse = (Stk_Ellipse*)(*itorCurveEntity);
			HoteamSoft::SVLLib::Stk_ArcPtr pNewCurveEntity = HoteamSoft::SVLLib::Stk_ArcPtr::CreateObject();
			SVL1EllipseToSVL2Arc(pSrcEllipse, pNewCurveEntity, pDocument);
			pNewLeader->AddCurve(pNewCurveEntity);

		}else if(eCurveType == StkCurveTypeEnum::CURVE_TYPE_POLYLINE)
		{
			Stk_PolyLine* pSrcPolyline = (Stk_PolyLine*)(*itorCurveEntity);
			HoteamSoft::SVLLib::Stk_PolyLinePtr pNewCurveEntity = HoteamSoft::SVLLib::Stk_PolyLinePtr::CreateObject();
			SVL1PolylineToSVL2(pSrcPolyline, pNewCurveEntity, pDocument);
			pNewLeader->AddCurve(pNewCurveEntity);
		}
		itorCurveEntity++;
	}
	return iRet;
}

int CSVL1ToSVL2::SVL1ExlineToSVL2(Stk_Extensionline* pSrcExline, HoteamSoft::SVLLib::Stk_ExtensionlinePtr pNewExline,
	HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcExline || pNewExline.isNull()|| pDocument .isNull())
		return -1;
	pNewExline->SetExLineType((HoteamSoft::SVLLib::StkExtensTypeEnum)pSrcExline->GetExLineType());
	pNewExline->SetStyle(pSrcExline->IsDisplay());

	Stk_CurveEntity* pCurveEntity = pSrcExline->GetCurve();
	StkCurveTypeEnum eCurveType = pCurveEntity->GetCurveType();
	if (eCurveType == StkCurveTypeEnum::CURVE_TYPE_LINE)
	{
		Stk_Line* pSrcLine = (Stk_Line*)pCurveEntity;
		HoteamSoft::SVLLib::Stk_LinePtr pNewCurveEntity = HoteamSoft::SVLLib::Stk_LinePtr::CreateObject();
		SVL1LineToSVL2(pSrcLine, pNewCurveEntity, pDocument);
		pNewExline->SetCurve(pNewCurveEntity);
	}
	else if (eCurveType == StkCurveTypeEnum::CURVE_TYPE_ELLIPSE)
	{
		Stk_Ellipse* pSrcEllipse = (Stk_Ellipse*)pCurveEntity;
		HoteamSoft::SVLLib::Stk_ArcPtr pNewCurveEntity = HoteamSoft::SVLLib::Stk_ArcPtr::CreateObject();
		SVL1EllipseToSVL2Arc(pSrcEllipse, pNewCurveEntity, pDocument);
		pNewExline->SetCurve(pNewCurveEntity);

	}
	else if (eCurveType == StkCurveTypeEnum::CURVE_TYPE_POLYLINE)
	{
		Stk_PolyLine* pSrcPolyline = (Stk_PolyLine*)pCurveEntity;
		HoteamSoft::SVLLib::Stk_PolyLinePtr pNewCurveEntity = HoteamSoft::SVLLib::Stk_PolyLinePtr::CreateObject();
		SVL1PolylineToSVL2(pSrcPolyline, pNewCurveEntity, pDocument);
		pNewExline->SetCurve(pNewCurveEntity);
	}

	return iRet;
}

int CSVL1ToSVL2::SVL1ComTextToSVL2(Stk_ComText* pSrcComText, HoteamSoft::SVLLib::Stk_ComTextPtr pNewComText,
	HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcComText || pNewComText.isNull()|| pDocument .isNull())
		return -1;
	
	//outBox
	STK_BOX32  box = pSrcComText->GetOuterBox();
	HoteamSoft::SVLLib::STK_BOX32 newBox;
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 3; j++)
			newBox.BoundBox[i][j] = box.BoundBox[i][j];
	pNewComText->SetOuterBox(newBox);
	int nFrameType = pSrcComText->GetFrameType();
	int iFrameType = nFrameType;
	if(iFrameType != 0xffffffffcdcdcdcd && iFrameType != 0xFFFFFFFFBAADF00D)
		pNewComText->SetFrameType((HoteamSoft::SVLLib::StkFrameTypeEnum)iFrameType);

	vector<HoteamSoft::SVLLib::Stk_TextPtr> vcNewText;
	vector<Stk_Text*> vcTexts = pSrcComText->GetTexts();
	vector<Stk_Text*>::iterator itorText = vcTexts.begin();
	while (itorText != vcTexts.end())
	{
		HoteamSoft::SVLLib::Stk_TextPtr textP = HoteamSoft::SVLLib::Stk_TextPtr::CreateObject();
		SVL1TextToSVL2(*itorText, textP, pDocument);
		vcNewText.push_back(textP);
		itorText++;
	}
	pNewComText->SetTexts(vcNewText);

	Stk_Envelope* pSrcEnvelope = pSrcComText->GetEnvelope();
	HoteamSoft::SVLLib::Stk_EnvelopePtr envelopeP = HoteamSoft::SVLLib::Stk_EnvelopePtr::CreateObject();
	SVL1EnvelopeToSVL2(pSrcEnvelope, envelopeP, pDocument);
	pNewComText->SetEnvelope(envelopeP);
	return iRet;
}

int CSVL1ToSVL2::SVL1OutFrameToSVL2(Stk_OutFrame* pSrcOutFrame, HoteamSoft::SVLLib::Stk_OutFramePtr pNewOutFrame,
	HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcOutFrame || pNewOutFrame.isNull()|| pDocument .isNull())
		return -1;
	pNewOutFrame->SetFrameType((HoteamSoft::SVLLib::StkOutFrameUsageEnum)pSrcOutFrame->GetFrameType());
	pNewOutFrame->SetDefineType((HoteamSoft::SVLLib::StkOutFrameDefEnum)pSrcOutFrame->GetDefineType());

	vector<Stk_CurveEntity*> vcCurveEntity = pSrcOutFrame->GetFrameData();
	vector<Stk_CurveEntity*>::iterator itorCurveEntity = vcCurveEntity.begin();
	while (itorCurveEntity != vcCurveEntity.end())
	{
		StkCurveTypeEnum eCurveType = (*itorCurveEntity)->GetCurveType();
		if (eCurveType == StkCurveTypeEnum::CURVE_TYPE_LINE)
		{
			Stk_Line* pSrcLine = (Stk_Line*)(*itorCurveEntity);
			HoteamSoft::SVLLib::Stk_LinePtr pNewCurveEntity = HoteamSoft::SVLLib::Stk_LinePtr::CreateObject();
			SVL1LineToSVL2(pSrcLine, pNewCurveEntity, pDocument);
			pNewOutFrame->SetFrameData(pNewCurveEntity);
		}
		else if (eCurveType == StkCurveTypeEnum::CURVE_TYPE_ELLIPSE)
		{
			Stk_Ellipse* pSrcEllipse = (Stk_Ellipse*)(*itorCurveEntity);
			HoteamSoft::SVLLib::Stk_ArcPtr pNewCurveEntity = HoteamSoft::SVLLib::Stk_ArcPtr::CreateObject();
			SVL1EllipseToSVL2Arc(pSrcEllipse, pNewCurveEntity, pDocument);
			pNewOutFrame->SetFrameData(pNewCurveEntity);

		}
		else if (eCurveType == StkCurveTypeEnum::CURVE_TYPE_POLYLINE)
		{
			Stk_PolyLine* pSrcPolyline = (Stk_PolyLine*)(*itorCurveEntity);
			HoteamSoft::SVLLib::Stk_PolyLinePtr pNewCurveEntity = HoteamSoft::SVLLib::Stk_PolyLinePtr::CreateObject();
			SVL1PolylineToSVL2(pSrcPolyline, pNewCurveEntity, pDocument);
			pNewOutFrame->SetFrameData(pNewCurveEntity);
		}
		itorCurveEntity++;
	}

	return iRet;
}

int CSVL1ToSVL2::SVL1SpecialLineToSVL2(Stk_SpecialLine* pSrcSpecialLine, HoteamSoft::SVLLib::Stk_SpecialLinePtr pNewSpecialLine, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcSpecialLine || pNewSpecialLine.isNull()|| pDocument .isNull())
		return -1;
	pNewSpecialLine->SetSpecLinUse((HoteamSoft::SVLLib::StkSpecLineUsageEnum)pSrcSpecialLine->GetSpecLinUse());
	
	vector<Stk_CurveEntity*> vcCurveEntity = pSrcSpecialLine->GetCurves();
	vector<Stk_CurveEntity*>::iterator itorCurveEntity = vcCurveEntity.begin();
	while (itorCurveEntity != vcCurveEntity.end())
	{
		StkCurveTypeEnum eCurveType = (*itorCurveEntity)->GetCurveType();
		if (eCurveType == StkCurveTypeEnum::CURVE_TYPE_LINE)
		{
			Stk_Line* pSrcLine = (Stk_Line*)(*itorCurveEntity);
			HoteamSoft::SVLLib::Stk_LinePtr pNewCurveEntity = HoteamSoft::SVLLib::Stk_LinePtr::CreateObject();
			SVL1LineToSVL2(pSrcLine, pNewCurveEntity, pDocument);
			pNewSpecialLine->AddCurve(pNewCurveEntity);
		}
		else if (eCurveType == StkCurveTypeEnum::CURVE_TYPE_ELLIPSE)
		{
			Stk_Ellipse* pSrcEllipse = (Stk_Ellipse*)(*itorCurveEntity);
			HoteamSoft::SVLLib::Stk_ArcPtr pNewCurveEntity = HoteamSoft::SVLLib::Stk_ArcPtr::CreateObject();
			SVL1EllipseToSVL2Arc(pSrcEllipse, pNewCurveEntity, pDocument);
			pNewSpecialLine->AddCurve(pNewCurveEntity);

		}
		else if (eCurveType == StkCurveTypeEnum::CURVE_TYPE_POLYLINE)
		{
			Stk_PolyLine* pSrcPolyline = (Stk_PolyLine*)(*itorCurveEntity);
			HoteamSoft::SVLLib::Stk_PolyLinePtr pNewCurveEntity = HoteamSoft::SVLLib::Stk_PolyLinePtr::CreateObject();
			SVL1PolylineToSVL2(pSrcPolyline, pNewCurveEntity, pDocument);
			pNewSpecialLine->AddCurve(pNewCurveEntity);
		}
		itorCurveEntity++;
	}

	return iRet;
}

int CSVL1ToSVL2::SVL1ImageToSVL2(Stk_Picture* pSrcImage, HoteamSoft::SVLLib::Stk_ImagePtr pNewImage,
	HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcImage || pNewImage.isNull()|| pDocument .isNull())
		return -1;
	//location
	pNewImage->SetType((HoteamSoft::SVLLib::StkPictureTypeEnum)pSrcImage->GetType());
	STK_PNT32 srcPnt = pSrcImage->GetLocation();
	HoteamSoft::SVLLib::STK_PNT32 newPnt;
	newPnt.X = srcPnt.X;
	newPnt.Y = srcPnt.Y;
	newPnt.Z = srcPnt.Z;
	pNewImage->SetLocation(newPnt);
	//displaySize
	pNewImage->SetDisplayHeight(pSrcImage->GetDisplayHeight());
	pNewImage->SetDisplayWidth(pSrcImage->GetDisplayWidth());
	//actualSize
	pNewImage->SetActualHeight(pSrcImage->GetActualHeight());
	pNewImage->SetActualWidth(pSrcImage->GetActualWidth());
	//NC
	pNewImage->SetNC(pSrcImage->GetNC());
	//saveStyle
	pNewImage->SetPictureSaveStyle((HoteamSoft::SVLLib::StkPicSaveStyleEnum)pSrcImage->GetPictureSaveStyle());
	pNewImage->SetPictureSavePath(pSrcImage->GetPictureSavePath());
	//imageData
	STK_UINT32 uiDataNum;
	STK_CHAR* pszData = NULL;
	pSrcImage->GetData(uiDataNum, pszData);
	pNewImage->SetData(uiDataNum, pszData);
	return iRet;
}

int CSVL1ToSVL2::SVL1UserDataToSVL2(Stk_UserData* pSrcUserData, HoteamSoft::SVLLib::Stk_UserDataPtr pNewUserData,
	HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcUserData || pNewUserData.isNull()|| pDocument .isNull())
		return -1;
	wstring wstrDataName, wstrFileName;
	STK_ID id;
	STK_CHAR* data;
	STK_SIZE_T dataSize = 0;
	pSrcUserData->GetUserData(wstrDataName, id, data, dataSize, wstrFileName);
	pNewUserData->SetUserData(wstrDataName, id, data, dataSize, wstrFileName);
	return iRet;
}

int CSVL1ToSVL2::SVL1LineToSVL2(Stk_Line* pSrcLine, HoteamSoft::SVLLib::Stk_LinePtr pNewLine,
	HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcLine || pNewLine.isNull()|| pDocument .isNull())
		return -1;
	//pNewLine->SetID(pSrcLine->GetID());
	pNewLine->SetUsageType((HoteamSoft::SVLLib::StkCurveUsageEnum)pSrcLine->GetUsageType());
	//color
	STK_RGBA32 srcColor = pSrcLine->GetColor();
	HoteamSoft::SVLLib::STK_RGBA32 newColor;
	newColor.Red = srcColor.Red;
	newColor.Green = srcColor.Green;
	newColor.Blue = srcColor.Blue;
	newColor.Alpha = srcColor.Alpha;
	pNewLine->SetColor(newColor);
	
	pNewLine->SetCurveDispStat(pSrcLine->GetCurveDispStat());
	pNewLine->SetCurveDispType((HoteamSoft::SVLLib::StkCurveDispTypeEnum)pSrcLine->GetCurveDispType());
	STK_PNT32 startPoint, endPoint;
	startPoint = pSrcLine->GetStartPoint();
	endPoint = pSrcLine->GetEndPoint();
	HoteamSoft::SVLLib::STK_PNT32 newStartPoint, newEndPoint;
	newStartPoint.X = startPoint.X;
	newStartPoint.Y = startPoint.Y;
	newStartPoint.Z = startPoint.Z;
	newEndPoint.X = endPoint.X;
	newEndPoint.Y = endPoint.Y;
	newEndPoint.Z = endPoint.Z;
	pNewLine->SetStartPoint(newStartPoint);
	pNewLine->SetEndPoint(newEndPoint);
	return iRet;
}

int CSVL1ToSVL2::SVL1EllipseToSVL2Arc(Stk_Ellipse* pSrcEllipse, HoteamSoft::SVLLib::Stk_ArcPtr pNewArc,
	HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcEllipse || pNewArc.isNull()|| pDocument .isNull())
		return -1;
	pNewArc->SetUsageType((HoteamSoft::SVLLib::StkCurveUsageEnum)pSrcEllipse->GetUsageType());
	//pNewArc->SetID(pSrcEllipse->GetID());
	pNewArc->SetCurveDispStat(pSrcEllipse->GetCurveDispStat());
	pNewArc->SetCurveDispType((HoteamSoft::SVLLib::StkCurveDispTypeEnum)pSrcEllipse->GetCurveDispType());
	
	STK_RGBA32 srcColor = pSrcEllipse->GetColor();
	HoteamSoft::SVLLib::STK_RGBA32 newColor;
	newColor.Red = srcColor.Red;
	newColor.Green = srcColor.Green;
	newColor.Blue = srcColor.Blue;
	newColor.Alpha = srcColor.Alpha;
	pNewArc->SetColor(newColor);
	
	STK_FLOAT32 minValue, maxValue;
	pSrcEllipse->GetDomain(minValue, maxValue);
	pNewArc->SetRange(minValue, maxValue);

	STK_PNT32 centerPoint = pSrcEllipse->GetCenterPoint();
	HoteamSoft::SVLLib::STK_PNT32 newCenterPoint;
	newCenterPoint.X = centerPoint.X;
	newCenterPoint.Y = centerPoint.Y;
	newCenterPoint.Z = centerPoint.Z;
	pNewArc->SetCenterPoint(newCenterPoint);

	//xDir yDir zDir
	STK_DIR32 xDir, yDir, zDir, normalDir, oriDir;
	pSrcEllipse->GetXYZDir(xDir, yDir, zDir);
	HoteamSoft::SVLLib::STK_DIR32 newXDir, newYDir, newZDir, newNormalDir, newOriDir;
	newXDir.X = xDir.X; newXDir.Y = xDir.Y; newXDir.Z = xDir.Z;
	newYDir.X = yDir.X; newYDir.Y = yDir.Y; newYDir.Z = yDir.Z;
	newZDir.X = zDir.X; newZDir.Y = zDir.Y; newZDir.Z = zDir.Z;
	pNewArc->SetXYZDir(newXDir, newYDir, newZDir);

	//normal
	normalDir = pSrcEllipse->GetNormal();
	newNormalDir.X = normalDir.X;
	newNormalDir.Y = normalDir.Y;
	newNormalDir.Z = normalDir.Z;
	pNewArc->SetNormal(newNormalDir);

	//originDir
	oriDir = pSrcEllipse->GetOriginDir();
	newOriDir.X = oriDir.X;
	newOriDir.Y = oriDir.Y;
	newOriDir.Z = oriDir.Z;
	pNewArc->SetOriginDir(newOriDir);

	//majorRadius minorRadius
	STK_FLOAT32 majRadius, minRadius;
	pSrcEllipse->GetRadius(majRadius, minRadius);
	pNewArc->SetRadius(majRadius,minRadius);

	//beginPnt endPnt
	STK_PNT32 beginPnt, endPnt;
	pSrcEllipse->GetCoordinatePnt(beginPnt, endPnt);
	HoteamSoft::SVLLib::STK_PNT32 newBeginPnt, newEndPnt;
	newBeginPnt.X = beginPnt.X; newEndPnt.X = endPnt.X;
	newBeginPnt.Y = beginPnt.Y; newEndPnt.Y = endPnt.Y;
	newBeginPnt.Z = beginPnt.Z; newEndPnt.Z = endPnt.Z;
	pNewArc->SetCoordiantePnt(newBeginPnt, newEndPnt);

	//beginPar endPar
	STK_FLOAT32 beginPar, endPar;
	pSrcEllipse->GetParameter(beginPar, endPar);
	pNewArc->SetParameter(beginPar, endPar);
	return iRet;
}

int CSVL1ToSVL2::SVL1PolylineToSVL2(Stk_PolyLine* pSrcPolyline, HoteamSoft::SVLLib::Stk_PolyLinePtr pNewPolyline,
	HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcPolyline || pNewPolyline.isNull()|| pDocument .isNull())
		return -1;
	//pNewPolyline->SetID(pSrcPolyline->GetID());
	pNewPolyline->SetUsageType((HoteamSoft::SVLLib::StkCurveUsageEnum)pSrcPolyline->GetUsageType());
	pNewPolyline->SetCurveDispStat(pSrcPolyline->GetCurveDispStat());
	pNewPolyline->SetCurveDispType((HoteamSoft::SVLLib::StkCurveDispTypeEnum)pSrcPolyline->GetCurveDispType());

	STK_RGBA32 srcColor = pSrcPolyline->GetColor();
	HoteamSoft::SVLLib::STK_RGBA32 newColor;
	newColor.Red = srcColor.Red;
	newColor.Green = srcColor.Green;
	newColor.Blue = srcColor.Blue;
	newColor.Alpha = srcColor.Alpha;
	pNewPolyline->SetColor(newColor);

	STK_FLOAT32 minValue, maxValue;
	pSrcPolyline->GetDomain(minValue, maxValue);
	pNewPolyline->SetRange(minValue, maxValue);

	vector<HoteamSoft::SVLLib::STK_PNT32> vcNewPoints;
	vector<STK_PNT32> vcPoints = pSrcPolyline->GetPoints();
	vector<STK_PNT32>::iterator itorPoint = vcPoints.begin();
	while (itorPoint != vcPoints.end())
	{
		HoteamSoft::SVLLib::STK_PNT32 newPoint;
		newPoint.X = (*itorPoint).X;
		newPoint.Y = (*itorPoint).Y;
		newPoint.Z = (*itorPoint).Z;
		vcNewPoints.push_back(newPoint);
		itorPoint++;
	}
	pNewPolyline->SetPoints(vcNewPoints);
	return iRet;
}

int CSVL1ToSVL2::SVL1TextToSVL2(Stk_Text* pSrcText, HoteamSoft::SVLLib::Stk_TextPtr pNewText,
	HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcText || pNewText.isNull()|| pDocument .isNull())
		return -1;
	//innerLoc
	STK_PNT32 innerLoc = pSrcText->GetInnerLoc();
	HoteamSoft::SVLLib::STK_PNT32 newInnerLoc;
	newInnerLoc.X = innerLoc.X;
	newInnerLoc.Y = innerLoc.Y;
	newInnerLoc.Z = innerLoc.Z;
	pNewText->SetInnerLoc(newInnerLoc);
	//axis
	STK_DIR32 xAxis, yAxis;
	pSrcText->GetInnerXYAxis(xAxis, yAxis);
	HoteamSoft::SVLLib::STK_DIR32 newXAxis, newYAxis;
	newXAxis.X = xAxis.X; newXAxis.Y = xAxis.Y; newXAxis.Z = xAxis.Z;
	newYAxis.X = yAxis.X; newYAxis.Y = yAxis.Y; newYAxis.Z = yAxis.Z;
	pNewText->SetInnerXYAxis(newXAxis, newYAxis);
	//angle
	#ifdef WIN32
 	if (!_isnan(pSrcText->GetRotationAngle()))
#else
 	if (!isnan(pSrcText->GetRotationAngle()))
#endif
		pNewText->SetRotationAngle(pSrcText->GetRotationAngle());
	//size
	STK_FLOAT32 width, height;
	pSrcText->GetCharWidthHeight(width, height);
	pNewText->SetCharWidthAndHeight(width, height);
	//charSpace 
	pNewText->SetCharSpace(pSrcText->GetCharSpacing());
	//usetype
	pNewText->SetUsageType((HoteamSoft::SVLLib::StkTextTypeEnum)pSrcText->GetUsageType());
	//outBox
	STK_BOX32 srcOutBox = pSrcText->GetOuterBox();
	HoteamSoft::SVLLib::STK_BOX32 newOutBox;
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 3; j++)
			newOutBox.BoundBox[i][j] = srcOutBox.BoundBox[i][j];
	pNewText->SetOuterBox(newOutBox);
	pNewText->SetFontName(pSrcText->GetFonts());
	if(pSrcText->GetFontType() != 0xffffffffcdcdcdcd && pSrcText->GetFontType() != 0xFFFFFFFFBAADF00D)
		pNewText->SetFontType((HoteamSoft::SVLLib::StkFontTypeEnum)pSrcText->GetFontType());
	if(pSrcText->GetAttributeType() != 0xffffffffcdcdcdcd && pSrcText->GetAttributeType() != 0xFFFFFFFFBAADF00D)
		pNewText->SetAttributeType((HoteamSoft::SVLLib::StkTextAttributeTypeEnum)pSrcText->GetAttributeType());
	pNewText->SetTextContent(pSrcText->GetText());
	return iRet;
}

int CSVL1ToSVL2::SVL1EnvelopeToSVL2(Stk_Envelope* pSrcEnvelope, HoteamSoft::SVLLib::Stk_EnvelopePtr pNewEnvelope,
	HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcEnvelope || pNewEnvelope.isNull()|| pDocument .isNull())
		return -1;
	pNewEnvelope->SetStyle(pSrcEnvelope->IsHaveFrame());
	STK_RGBA32 srcFrameColor = pSrcEnvelope->GetFrameColor();
	HoteamSoft::SVLLib::STK_RGBA32 newFrameColor, newBackgroundColor;
	newFrameColor.Red = srcFrameColor.Red;
	newFrameColor.Green = srcFrameColor.Green;
	newFrameColor.Blue = srcFrameColor.Blue;
	newFrameColor.Alpha = srcFrameColor.Alpha;
	pNewEnvelope->SetFrameColor(newFrameColor);
	STK_RGBA32 srcBackgroundColor = pSrcEnvelope->GetBackgroundColor();
	newBackgroundColor.Red = srcBackgroundColor.Red;
	newBackgroundColor.Green = srcBackgroundColor.Green;
	newBackgroundColor.Blue = srcBackgroundColor.Blue;
	newBackgroundColor.Alpha = srcBackgroundColor.Alpha;
	pNewEnvelope->SetBackgroundColor(newBackgroundColor);
	//vector<StkAnchorTypeEnum>& vcAnchorType = pSrcEnvelope->GetAllAnchorTypes();
	//vector<StkAnchorTypeEnum>::iterator itorAnchorType = vcAnchorType.begin();
	//while (itorAnchorType != vcAnchorType.end())
	for(int i = 0; i < 10; i++)
	{
		STK_PNT32 anchorPoint;

		if(STK_SUCCESS == pSrcEnvelope->GetAnchorPointByType((StkAnchorTypeEnum)(TYPE_ANCHOR_TOP_LEFT+i), anchorPoint))
		{
			HoteamSoft::SVLLib::STK_PNT32 newAnchorPoint;
			newAnchorPoint.X = anchorPoint.X;
			newAnchorPoint.Y = anchorPoint.Y;
			newAnchorPoint.Z = anchorPoint.Z;
			pNewEnvelope->SetAnchorPoint((HoteamSoft::SVLLib::StkAnchorTypeEnum)(TYPE_ANCHOR_TOP_LEFT + i), newAnchorPoint);
		}
	}
	return iRet;
}

int CSVL1ToSVL2::SVL1ViewToSVL2(Stk_View* pSrcView, HoteamSoft::SVLLib::Stk_ViewPtr pNewView, float* modelBox,
	HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcView || pNewView.isNull()|| pDocument .isNull())
		return -1;
	//pNewView->SetID(pSrcView->GetID());
	pNewView->SetName(pSrcView->GetName());
	pNewView->SetUsageType((HoteamSoft::SVLLib::StkViewUsageEnum)pSrcView->GetUsageType());
	pNewView->SetActivated(pSrcView->IsActivated());
	pNewView->SetTransparency(pSrcView->GetTransparency());
	m_mapOldViewIDToNew.insert(make_pair(pSrcView->GetID(), pNewView->GetID()));
	//camera
	Stk_Camera* pCamera = pSrcView->GetCamera();
	if (pCamera)
	{
		HoteamSoft::SVLLib::Stk_CameraPtr cameraP = HoteamSoft::SVLLib::Stk_CameraPtr::CreateObject();
		SVL1CameraToSVL2(pCamera, cameraP, modelBox, pDocument);
		pNewView->SetCamera(cameraP);
	}
	//insAttributes
	vector<wstring> vcPlcPath = pSrcView->GetAllInsAttrPlcPath();
	vector<wstring>::iterator itorPlcPath = vcPlcPath.begin();
	while (itorPlcPath != vcPlcPath.end())
	{
		HoteamSoft::SVLLib::STK_RGBA32 newColor;
		STK_RGBA32 srcColor;
		pSrcView->GetInsColorByPlcPath(*itorPlcPath, srcColor);
		newColor.Red = srcColor.Red; newColor.Green = srcColor.Green; 
		newColor.Blue = srcColor.Blue; newColor.Alpha = srcColor.Alpha;
		pNewView->AddInsColor(*itorPlcPath, newColor);
		StkDisplayStateEnum srcDisplayState;
		pSrcView->GetInsDisplayStateByPlcPath(*itorPlcPath, srcDisplayState);
		pNewView->AddInsDisplayState(*itorPlcPath, (HoteamSoft::SVLLib::StkDisplayStateEnum)srcDisplayState);
		STK_ID idPlacement;
		STK_MTX32 srcMatrix;
		HoteamSoft::SVLLib::STK_MTX32 newMatrix;
		pSrcView->GetInsPlacementByPlcPath(*itorPlcPath, idPlacement, srcMatrix);
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				newMatrix.PlcMatrix[i][j] = srcMatrix.PlcMatrix[i][j];
		pNewView->AddInsPlacement(*itorPlcPath, idPlacement, newMatrix);
		itorPlcPath++;
	}

	//clipPlanes
	vector<Stk_SurfaceEntity*> vcSurfaceEntity = pSrcView->GetAllClippingPlan();
	vector<Stk_SurfaceEntity*>::iterator itorSurfaceEntity = vcSurfaceEntity.begin();
	while (itorSurfaceEntity != vcSurfaceEntity.end())
	{
		if ((*itorSurfaceEntity)->GetType() == SURFACE_TYPE_PLANE)
		{
			HoteamSoft::SVLLib::Stk_ClipPlanePtr clipPlaneP = HoteamSoft::SVLLib::Stk_ClipPlanePtr::CreateObject();
			SVL1PlaneSurfaceToSVL2ClipPlane((Stk_PlaneSurface*)*itorSurfaceEntity, clipPlaneP, pDocument);
			pNewView->AddPlaneEntity(clipPlaneP);
		}
		itorSurfaceEntity++;
	}
	return iRet;
}

int CSVL1ToSVL2::SVl1ViewConnectorToSVL2(Stk_ProtoType* pSrcProtoType, Stk_View* pSrcView, HoteamSoft::SVLLib::Stk_ViewPtr pNewView, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcProtoType || !pSrcView || pNewView.isNull()|| pDocument .isNull())
		return -1;
	//connectors
	vector<Stk_NodeConnector*> vcConnector = pSrcView->GetNodeConnector();
	vector<Stk_NodeConnector*>::iterator itorConnector = vcConnector.begin();
	while (itorConnector != vcConnector.end())
	{
		if (*itorConnector)
		{
			if ((*itorConnector)->IsLinkNode())
			{
				StkEntityTypeEnum ePointingEnt, ePointedEnt;
				(*itorConnector)->GetEntityType(ePointingEnt, ePointedEnt);
				Stk_Node* pNode = NULL;
				pSrcProtoType->GetProtoIDManager()->GetNodeByID((*itorConnector)->GetLinkNode(), pNode);
				if (pNode)
				{
					StkNodeTypeEnum eNodeType = pNode->GetNodeType();
					if (ePointedEnt == ENTITY_TYPE_PMI)
					{
						Stk_PMI* pPmi = pNode->GetPMI();
						if (pPmi)
						{
							map<STK_ID, HoteamSoft::SVLLib::STK_ID>::iterator itorMap = m_mapOldPmiIDToNew.find(pPmi->GetID());
							if (itorMap != m_mapOldPmiIDToNew.end())
								pNewView->AddConnector(HoteamSoft::SVLLib::ENTITY_TYPE_PMI, itorMap->second);
						}
					}
					else if (ePointedEnt == ENTITY_TYPE_VIEW)
					{
						Stk_View* pView = pNode->GetView();
						if (pView)
						{
							map<STK_ID, HoteamSoft::SVLLib::STK_ID>::iterator itorMap = m_mapOldViewIDToNew.find(pView->GetID());
							if (itorMap != m_mapOldViewIDToNew.end())
								pNewView->AddConnector(HoteamSoft::SVLLib::ENTITY_TYPE_VIEW, itorMap->second);
						}
					}
				}
			}

			//HoteamSoft::SVLLib::Stk_ConnectorPtr conntectorP = HoteamSoft::SVLLib::Stk_ConnectorPtr::CreateObject();
			//SVL1NodeConnectorToSVL2(pSrcProtoType, *itorConnector, conntectorP, pDocument);
			//pNewView->AddConnector(conntectorP);
		}
		itorConnector++;
	}
	return iRet;
}

int CSVL1ToSVL2::SVL1CameraToSVL2(Stk_Camera* pSrcCamera, HoteamSoft::SVLLib::Stk_CameraPtr pNewCamera, float* modelBox,
	HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcCamera || pNewCamera.isNull()|| pDocument .isNull())
		return -1;
	pNewCamera->SetProjectType(pSrcCamera->GetProjectType());
	pNewCamera->SetAngle(pSrcCamera->GetAngle());
	//origin
	STK_PNT32 srcOriginPnt = pSrcCamera->GetOriginPoint();
	HoteamSoft::SVLLib::STK_PNT32 newOriginPnt;
	newOriginPnt.X = srcOriginPnt.X;
	newOriginPnt.Y = srcOriginPnt.Y;
	newOriginPnt.Z = srcOriginPnt.Z;
	pNewCamera->SetOriginPoint(newOriginPnt);
	//targetVector
	STK_DIR32 srcDir = pSrcCamera->GetTargetVector();
	HoteamSoft::SVLLib::STK_DIR32 newDir, newUpDir;
	newDir.X = srcDir.X;
	newDir.Y = srcDir.Y;
	newDir.Z = srcDir.Z;
	pNewCamera->SetTargetVector(newDir);
	//upVector
	STK_DIR32 srcUpDir = pSrcCamera->GetUpVector();
	newUpDir.X = srcUpDir.X;
	newUpDir.Y = srcUpDir.Y;
	newUpDir.Z = srcUpDir.Z;
	pNewCamera->SetUpVector(newUpDir);
	if(pSrcCamera->GetMatrixFlag())
	{
		//matrix
		STK_MTX32 srcMatrix = pSrcCamera->GetMatrix();
		HoteamSoft::SVLLib::STK_MTX32 newMatrix;
		for (int i = 0; i < MTX_LINE_COUNT; i++)
			for (int j = 0; j < MTX_COL_COUNT; j++)
				newMatrix.PlcMatrix[i][j] = srcMatrix.PlcMatrix[i][j];
		newMatrix.PlcMatrix[3][0] = srcOriginPnt.X;
		newMatrix.PlcMatrix[3][1] = srcOriginPnt.Y;
		newMatrix.PlcMatrix[3][2] = srcOriginPnt.Z;
		pNewCamera->SetMatrix(newMatrix);
	}

	float boxSize[3];
	boxSize[0] = modelBox[3] - modelBox[0];
	boxSize[1] = modelBox[4] - modelBox[1];
	boxSize[2] = modelBox[5] - modelBox[2];
	float fBoxSize = sqrt(boxSize[0] * boxSize[0] + boxSize[1] * boxSize[1] + boxSize[2] * boxSize[2]);
	float fCamHeight = fBoxSize / pSrcCamera->GetZoomFactor();
	pNewCamera->SetHeight(fCamHeight);
	//focalDistance
	if (pSrcCamera->GetFocus() < 2.0f)
	{
		pNewCamera->SetFocalDistance(fCamHeight);
	}
	else
	{
		pNewCamera->SetFocalDistance(pSrcCamera->GetFocus());
	}
	
	return iRet;
}

int CSVL1ToSVL2::SVL1PlaneSurfaceToSVL2ClipPlane(Stk_PlaneSurface* pSrcpPlane, HoteamSoft::SVLLib::Stk_ClipPlanePtr pNewClipPlane,
	HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcpPlane || pNewClipPlane.isNull()|| pDocument .isNull())
		return -1;
	HoteamSoft::SVLLib::STK_PNT32 newOrigin;
	HoteamSoft::SVLLib::STK_DIR32 newNormal, newUDir, newVDir;
	STK_PNT32 srcOrigin = pSrcpPlane->GetOrigin();
	newOrigin.X = srcOrigin.X;
	newOrigin.Y = srcOrigin.Y;
	newOrigin.Z = srcOrigin.Z;
	pNewClipPlane->SetOrigin(newOrigin);
	STK_DIR32 srcNormal = pSrcpPlane->GetNormal();
	newNormal.X = srcNormal.X;
	newNormal.Y = srcNormal.Y;
	newNormal.Z = srcNormal.Z;
	pNewClipPlane->SetNormal(newNormal);
	STK_DIR32 srcUDir, srcVDir;
	pSrcpPlane->GetUVDir(srcUDir, srcVDir);
	newUDir.X = srcUDir.X; newUDir.Y = srcUDir.Y; newUDir.Z = srcUDir.Z;
	newVDir.X = srcVDir.X; newVDir.Y = srcVDir.Y; newVDir.Z = srcVDir.Z;
	pNewClipPlane->SetUVDir(newUDir, newVDir);

	pNewClipPlane->SetDistance(pSrcpPlane->GetDistance());
	m_mapOldClipPlaneIDToNew.insert(make_pair(pSrcpPlane->GetID(), pNewClipPlane->GetID()));
	//clippingCurve
	Stk_ClippingCurve* pClippingCurve = pSrcpPlane->GetClippingCurve();
	if (pClippingCurve)
	{
		HoteamSoft::SVLLib::Stk_ClippingCurvePtr clippingCurveP = HoteamSoft::SVLLib::Stk_ClippingCurvePtr::CreateObject();
		SVL1ClippingCurveToSVL2(pClippingCurve, clippingCurveP, pDocument);
		pNewClipPlane->SetClippingCurve(clippingCurveP);
	}
	return iRet;
}

int CSVL1ToSVL2::SVL1ClippingCurveToSVL2(Stk_ClippingCurve* pSrcClippingCurve, HoteamSoft::SVLLib::Stk_ClippingCurvePtr pNewClippingCurve, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcClippingCurve || pNewClippingCurve.isNull()|| pDocument .isNull())
		return -1;
	pNewClippingCurve->SetDisplay(pSrcClippingCurve->GetDisplay());
	pNewClippingCurve->SetDrawStyle((HoteamSoft::SVLLib::StkClippingCurveDrawStyleEnum)pSrcClippingCurve->GetDrawStyle());
	pNewClippingCurve->SetDrawType((HoteamSoft::SVLLib::StkClippingCurveDrawTypeEnum)pSrcClippingCurve->GetDrawType());
	pNewClippingCurve->SetAngle(pSrcClippingCurve->GetAngle());
	pNewClippingCurve->SetRatio(pSrcClippingCurve->GetRatio());
	HoteamSoft::SVLLib::STK_RGBA32 newColor;
	STK_RGBA32 srcColor = pSrcClippingCurve->GetColor();
	newColor.Red = srcColor.Red;
	newColor.Green = srcColor.Green;
	newColor.Blue = srcColor.Blue;
	newColor.Alpha = srcColor.Alpha;
	pNewClippingCurve->SetColor(newColor);
	return iRet;
}

int CSVL1ToSVL2::SVL1NodeConnectorToSVL2(Stk_ProtoType* pSrcProtoType, Stk_NodeConnector* pSrcConnector, HoteamSoft::SVLLib::Stk_ConnectorPtr pNewConnector,
	HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcProtoType || !pSrcProtoType->GetProtoIDManager() || !pSrcConnector || pNewConnector == nullptr || pDocument == nullptr)
		return -1;
	StkEntityTypeEnum pointedEntityType, pointingEntityType;
	pSrcConnector->GetEntityType(pointedEntityType, pointingEntityType);
	pNewConnector->SetEntityType((HoteamSoft::SVLLib::StkEntityTypeEnum)pointedEntityType, (HoteamSoft::SVLLib::StkEntityTypeEnum)pointingEntityType);
	pNewConnector->SetIsLinkProto(pSrcConnector->IsLinkProto());
	pNewConnector->SetLinkProto(pSrcConnector->GetLinkProto());
	pNewConnector->SetIsLinkNode(pSrcConnector->IsLinkNode());
	if (pSrcConnector->IsLinkNode())
	{
		Stk_Node* pNode = NULL;
		pSrcProtoType->GetProtoIDManager()->GetNodeByID(pSrcConnector->GetLinkNode(), pNode);
		if (pNode)
		{
			StkNodeTypeEnum eNodeType = pNode->GetNodeType();
			if (eNodeType == NODE_TYPE_PMI)
			{
				Stk_PMI* pPmi = pNode->GetPMI();
				if (pPmi)
				{
					map<STK_ID, HoteamSoft::SVLLib::STK_ID>::iterator itorMap = m_mapOldPmiIDToNew.find(pPmi->GetID());
					if (itorMap != m_mapOldPmiIDToNew.end())
						pNewConnector->SetLinkNode(itorMap->second);
				}
			}else if (eNodeType == NODE_TYPE_VIEW)
			{

			}
		}
	}
	vector<SubEntity> vcSubEntity = pSrcConnector->GetSubEntity();
	vector<SubEntity>::iterator itorSubEntity = vcSubEntity.begin();
	while (itorSubEntity != vcSubEntity.end())
	{
		HoteamSoft::SVLLib::SubEntity subEntity;
		subEntity.idEntity = (*itorSubEntity).idEntity;
		subEntity.nuEntityType = (HoteamSoft::SVLLib::StkEntityTypeEnum)(*itorSubEntity).nuEntityType;
		subEntity.pntPick.X = (*itorSubEntity).pntPick.X;
		subEntity.pntPick.Y = (*itorSubEntity).pntPick.Y;
		subEntity.pntPick.Z = (*itorSubEntity).pntPick.Z;
		subEntity.dirPick.X = (*itorSubEntity).dirPick.X;
		subEntity.dirPick.Y = (*itorSubEntity).dirPick.Y;
		subEntity.dirPick.Z = (*itorSubEntity).dirPick.Z;
		pNewConnector->AddSubEntity(subEntity);
		itorSubEntity++;
	}
	return iRet;
}

int CSVL1ToSVL2::SVL1GeoAttrToSVL2MeshFaceAttr(Stk_GeometryAttribute* pSrcGeoAttr, HoteamSoft::SVLLib::Stk_ModelPtr pModel, HoteamSoft::SVLLib::Stk_MeshFacePtr pNewMeshFace, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcGeoAttr  || pDocument .isNull()|| pModel.isNull())
		return -1;
	if (pSrcGeoAttr->GetGeoAttrType() == GEOATTR_TYPE_PLANEFACE)
	{
		Stk_PlaneFaceAttribute* pSrcAttribute = (Stk_PlaneFaceAttribute*)pSrcGeoAttr;
		STK_DIR32 srcNormal = pSrcAttribute->GetNormal();
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_TYPE, L"1");
		string strValue;
		char szValue[32] = { 0 };
		//normal
		sprintf(szValue, "%.8f", srcNormal.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcNormal.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcNormal.Z);
		strValue += szValue;
		wstring wstrValue = M3D::Platform::StringToWString(strValue);
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_NORMAL, wstrValue);
		STK_PNT32 srcOrigin = pSrcAttribute->GetOrigin();
		//geo_origin
		strValue.clear();
		sprintf(szValue, "%.8f", srcOrigin.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcOrigin.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcOrigin.Z);
		strValue += szValue;
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_ORIGIN, M3D::Platform::StringToWString(strValue));
	}
	else if (pSrcGeoAttr->GetGeoAttrType() == GEOATTR_TYPE_REVOLUTIONFACE || pSrcGeoAttr->GetGeoAttrType() == GEOATTR_TYPE_CYLINDERFACE || pSrcGeoAttr->GetGeoAttrType() == GEOATTR_TYPE_CONICALFACE || pSrcGeoAttr->GetGeoAttrType() == GEOATTR_TYPE_SPHEREFACE)
	{
		Stk_RevolutionFaceAttribute* pSrcAttribute = (Stk_RevolutionFaceAttribute*)pSrcGeoAttr;
		if(pSrcGeoAttr->GetGeoAttrType() == GEOATTR_TYPE_REVOLUTIONFACE)
			pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_TYPE, L"2");
		else if(pSrcGeoAttr->GetGeoAttrType() == GEOATTR_TYPE_CYLINDERFACE)
			pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_TYPE, L"3");
		else if(pSrcGeoAttr->GetGeoAttrType() == GEOATTR_TYPE_CONICALFACE)
			pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_TYPE, L"4");
		else if(pSrcGeoAttr->GetGeoAttrType() == GEOATTR_TYPE_SPHEREFACE)
			pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_TYPE, L"5");
		STK_PNT32 srcAxisOrigin = pSrcAttribute->GetAxisOrigin();
		string strValue;
		char szValue[32] = { 0 };
		//geo_axisOrigin
		sprintf(szValue, "%.8f", srcAxisOrigin.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Z);
		strValue += szValue;
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_AXISORIGIN, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		STK_DIR32 srcDirection = pSrcAttribute->GetRevoAxis();
		//geo_axisDirection
		sprintf(szValue, "%.8f", srcDirection.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Z);
		strValue += szValue;
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_AXISDIRECTION, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		//radius
		sprintf(szValue, "%.8f", pSrcAttribute->GetRadius());
		strValue += szValue;
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_RADIUS, M3D::Platform::StringToWString(strValue));
	}
	else if (pSrcGeoAttr->GetGeoAttrType() == GEOATTR_TYPE_TOROIDALFACE)
	{
		Stk_ToroidalFaceAttribute* pSrcAttribute = (Stk_ToroidalFaceAttribute*)pSrcGeoAttr;
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_TYPE, L"6");
		STK_PNT32 srcAxisOrigin = pSrcAttribute->GetAxisOrigin();
		string strValue;
		char szValue[32] = { 0 };
		//axisOrigin
		sprintf(szValue, "%.8f", srcAxisOrigin.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Z);
		strValue += szValue;
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_AXISORIGIN, M3D::Platform::StringToWString(strValue));
		STK_DIR32 srcDirection = pSrcAttribute->GetRevoAxis();
		strValue.clear();
		//axisDirection
		sprintf(szValue, "%.8f", srcDirection.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Z);
		strValue += szValue;
		//axisDirection
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_AXISDIRECTION, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		sprintf(szValue, "%.8f", pSrcAttribute->GetRadius());
		strValue += szValue;
		//originRadius
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_ORIGINRADIUS, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		sprintf(szValue, "%.8f", pSrcAttribute->GetMajorRadius());
		strValue += szValue;
		//majorRadius
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_MAJORRADIUS, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		sprintf(szValue, "%.8f", pSrcAttribute->GetMinorRadius());
		strValue += szValue;
		//minorRadius
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_MINORRADIUS, M3D::Platform::StringToWString(strValue));
	}
	else if (pSrcGeoAttr->GetGeoAttrType() == GEOATTR_TYPE_LINE)
	{
		Stk_LineAttribute* pSrcAttribute = (Stk_LineAttribute*)pSrcGeoAttr;
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_TYPE, L"7");
		string strValue;
		char szValue[32] = { 0 };
		STK_PNT32 srcCenterPoint = pSrcAttribute->GetCenterPoint();
		sprintf(szValue, "%.8f", srcCenterPoint.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcCenterPoint.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcCenterPoint.Z);
		strValue += szValue;
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_CENTEREPOINT, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		STK_PNT32 srcStartPoint = pSrcAttribute->GetStartPoint();
		sprintf(szValue, "%.8f", srcStartPoint.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcStartPoint.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcStartPoint.Z);
		strValue += szValue;
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_STARTPOINT, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		STK_PNT32 srcEndPoint = pSrcAttribute->GetEndPoint();
		sprintf(szValue, "%.8f", srcEndPoint.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcEndPoint.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcEndPoint.Z);
		strValue += szValue;
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_ENDPOINT, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		STK_DIR32 srcDirection = pSrcAttribute->GetDirection();
		sprintf(szValue, "%.8f", srcDirection.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Z);
		strValue += szValue;
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_DIRECTION, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		sprintf(szValue, "%.8f", pSrcAttribute->GetLength());
		strValue += szValue;
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_LENGTH, M3D::Platform::StringToWString(strValue));
	}
	else if (pSrcGeoAttr->GetGeoAttrType() == GEOATTR_TYPE_ELLIPSE)
	{
		Stk_EllipseAttribute* pSrcAttribute = (Stk_EllipseAttribute*)pSrcGeoAttr;
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_TYPE, L"8");
		string strValue;
		char szValue[32] = { 0 };
		STK_PNT32 srcCenterPoint = pSrcAttribute->GetCenterPoint();
		sprintf(szValue, "%.8f", srcCenterPoint.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcCenterPoint.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcCenterPoint.Z);
		strValue += szValue;
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_CENTEREPOINT, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		STK_PNT32 srcStartPoint = pSrcAttribute->GetStartPoint();
		sprintf(szValue, "%.8f", srcStartPoint.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcStartPoint.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcStartPoint.Z);
		strValue += szValue;
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_STARTPOINT, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		STK_PNT32 srcEndPoint = pSrcAttribute->GetEndPoint();
		sprintf(szValue, "%.8f", srcEndPoint.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcEndPoint.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcEndPoint.Z);
		strValue += szValue;
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_ENDPOINT, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		sprintf(szValue, "%.8f", pSrcAttribute->GetMajorRadius());
		strValue += szValue;
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_MAJORRADIUS, M3D::Platform::StringToWString(strValue));
		sprintf(szValue, "%.8f", pSrcAttribute->GetMinorRadius());
		strValue += szValue;
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_MINORRADIUS, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		STK_DIR32 srcXDir, srcYDir, srcZDir;
		pSrcAttribute->GetXYZDir(srcXDir, srcYDir, srcZDir);
		sprintf(szValue, "%.8f", srcXDir.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcXDir.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcXDir.Z);
		strValue += szValue;
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_XDIRECTION, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		sprintf(szValue, "%.8f", srcYDir.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcYDir.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcYDir.Z);
		strValue += szValue;
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_YDIRECTION, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		sprintf(szValue, "%.8f", srcZDir.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcZDir.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcZDir.Z);
		strValue += szValue;
		pDocument->AddMeshFaceAttribute(pModel->GetID(), pNewMeshFace->GetID(), MESH_ZDIRECTION, M3D::Platform::StringToWString(strValue));
		strValue.clear();
	}
	return iRet;
}

int CSVL1ToSVL2::SVL1GeoAttrToSVL2MeshEdgeAttr(Stk_GeometryAttribute* pSrcGeoAttr, HoteamSoft::SVLLib::Stk_ModelPtr pModel, HoteamSoft::SVLLib::Stk_MeshEdgePtr pNewMeshEdge, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcGeoAttr || pDocument .isNull())
		return -1;
	if (pSrcGeoAttr->GetGeoAttrType() == GEOATTR_TYPE_PLANEFACE)
	{
		Stk_PlaneFaceAttribute* pSrcAttribute = (Stk_PlaneFaceAttribute*)pSrcGeoAttr;
		STK_DIR32 srcNormal = pSrcAttribute->GetNormal();
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_TYPE, L"1");
		string strValue;
		char szValue[32] = { 0 };
		//normal
		sprintf(szValue, "%.8f", srcNormal.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcNormal.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcNormal.Z);
		strValue += szValue;
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_NORMAL, M3D::Platform::StringToWString(strValue));
		STK_PNT32 srcOrigin = pSrcAttribute->GetOrigin();
		//origin
		strValue.clear();
		sprintf(szValue, "%.8f", srcOrigin.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcOrigin.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcOrigin.Z);
		strValue += szValue;
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_ORIGIN, M3D::Platform::StringToWString(strValue));
	}
	else if (pSrcGeoAttr->GetGeoAttrType() == GEOATTR_TYPE_REVOLUTIONFACE || pSrcGeoAttr->GetGeoAttrType() == GEOATTR_TYPE_CYLINDERFACE || pSrcGeoAttr->GetGeoAttrType() == GEOATTR_TYPE_CONICALFACE || pSrcGeoAttr->GetGeoAttrType() == GEOATTR_TYPE_SPHEREFACE)
	{
		Stk_RevolutionFaceAttribute* pSrcAttribute = (Stk_RevolutionFaceAttribute*)pSrcGeoAttr;
		if (pSrcGeoAttr->GetGeoAttrType() == GEOATTR_TYPE_REVOLUTIONFACE)
			pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_TYPE, L"2");
		else if (pSrcGeoAttr->GetGeoAttrType() == GEOATTR_TYPE_CYLINDERFACE)
			pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_TYPE, L"3");
		else if (pSrcGeoAttr->GetGeoAttrType() == GEOATTR_TYPE_CONICALFACE)
			pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_TYPE, L"4");
		else if (pSrcGeoAttr->GetGeoAttrType() == GEOATTR_TYPE_SPHEREFACE)
			pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_TYPE, L"5");
		STK_PNT32 srcAxisOrigin = pSrcAttribute->GetAxisOrigin();
		string strValue;
		char szValue[32] = { 0 };
		//axisOrigin
		sprintf(szValue, "%.8f", srcAxisOrigin.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Z);
		strValue += szValue;
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_AXISORIGIN, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		STK_DIR32 srcDirection = pSrcAttribute->GetRevoAxis();
		//axisDirection
		sprintf(szValue, "%.8f", srcDirection.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Z);
		strValue += szValue;
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_AXISDIRECTION, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		//radius
		sprintf(szValue, "%.8f", pSrcAttribute->GetRadius());
		strValue += szValue;
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_RADIUS, M3D::Platform::StringToWString(strValue));
	}
	else if (pSrcGeoAttr->GetGeoAttrType() == GEOATTR_TYPE_TOROIDALFACE)
	{
		Stk_ToroidalFaceAttribute* pSrcAttribute = (Stk_ToroidalFaceAttribute*)pSrcGeoAttr;
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_TYPE, L"6");
		STK_PNT32 srcAxisOrigin = pSrcAttribute->GetAxisOrigin();
		string strValue;
		char szValue[32] = { 0 };
		//axisOrigin
		sprintf(szValue, "%.8f", srcAxisOrigin.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Z);
		strValue += szValue;
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_AXISORIGIN, M3D::Platform::StringToWString(strValue));
		STK_DIR32 srcDirection = pSrcAttribute->GetRevoAxis();
		strValue.clear();
		//axisDirection
		sprintf(szValue, "%.8f", srcDirection.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Z);
		strValue += szValue;
		//axisDirection
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_AXISDIRECTION, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		sprintf(szValue, "%.8f", pSrcAttribute->GetRadius());
		strValue += szValue;
		//originRadius
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_ORIGINRADIUS, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		sprintf(szValue, "%.8f", pSrcAttribute->GetMajorRadius());
		strValue += szValue;
		//majorRadius
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_MAJORRADIUS, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		sprintf(szValue, "%.8f", pSrcAttribute->GetMinorRadius());
		strValue += szValue;
		//minorRadius
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_MINORRADIUS, M3D::Platform::StringToWString(strValue));
	}
	else if (pSrcGeoAttr->GetGeoAttrType() == GEOATTR_TYPE_LINE)
	{
		Stk_LineAttribute* pSrcAttribute = (Stk_LineAttribute*)pSrcGeoAttr;
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_TYPE, L"7");
		string strValue;
		char szValue[32] = { 0 };
		STK_PNT32 srcCenterPoint = pSrcAttribute->GetCenterPoint();
		sprintf(szValue, "%.8f", srcCenterPoint.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcCenterPoint.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcCenterPoint.Z);
		strValue += szValue;
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_CENTEREPOINT, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		STK_PNT32 srcStartPoint = pSrcAttribute->GetStartPoint();
		sprintf(szValue, "%.8f", srcStartPoint.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcStartPoint.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcStartPoint.Z);
		strValue += szValue;
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_STARTPOINT, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		STK_PNT32 srcEndPoint = pSrcAttribute->GetEndPoint();
		sprintf(szValue, "%.8f", srcEndPoint.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcEndPoint.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcEndPoint.Z);
		strValue += szValue;
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_ENDPOINT, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		STK_DIR32 srcDirection = pSrcAttribute->GetDirection();
		sprintf(szValue, "%.8f", srcDirection.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Z);
		strValue += szValue;
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_DIRECTION, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		sprintf(szValue, "%.8f", pSrcAttribute->GetLength());
		strValue += szValue;
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_LENGTH, M3D::Platform::StringToWString(strValue));
	}
	else if (pSrcGeoAttr->GetGeoAttrType() == GEOATTR_TYPE_ELLIPSE)
	{
		Stk_EllipseAttribute* pSrcAttribute = (Stk_EllipseAttribute*)pSrcGeoAttr;
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_TYPE, L"8");
		string strValue;
		char szValue[32] = { 0 };
		STK_PNT32 srcCenterPoint = pSrcAttribute->GetCenterPoint();
		sprintf(szValue, "%.8f", srcCenterPoint.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcCenterPoint.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcCenterPoint.Z);
		strValue += szValue;
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_CENTEREPOINT, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		STK_PNT32 srcStartPoint = pSrcAttribute->GetStartPoint();
		sprintf(szValue, "%.8f", srcStartPoint.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcStartPoint.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcStartPoint.Z);
		strValue += szValue;
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_STARTPOINT, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		STK_PNT32 srcEndPoint = pSrcAttribute->GetEndPoint();
		sprintf(szValue, "%.8f", srcEndPoint.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcEndPoint.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcEndPoint.Z);
		strValue += szValue;
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_ENDPOINT, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		sprintf(szValue, "%.8f", pSrcAttribute->GetMajorRadius());
		strValue += szValue;
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_MAJORRADIUS, M3D::Platform::StringToWString(strValue));
		sprintf(szValue, "%.8f", pSrcAttribute->GetMinorRadius());
		strValue += szValue;
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_MINORRADIUS, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		STK_DIR32 srcXDir, srcYDir, srcZDir;
		pSrcAttribute->GetXYZDir(srcXDir, srcYDir, srcZDir);
		sprintf(szValue, "%.8f", srcXDir.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcXDir.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcXDir.Z);
		strValue += szValue;
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_XDIRECTION, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		sprintf(szValue, "%.8f", srcYDir.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcYDir.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcYDir.Z);
		strValue += szValue;
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_YDIRECTION, M3D::Platform::StringToWString(strValue));
		strValue.clear();
		sprintf(szValue, "%.8f", srcZDir.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcZDir.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcZDir.Z);
		strValue += szValue;
		pDocument->AddMeshEdgeAttribute(pModel->GetID(), pNewMeshEdge->GetID(), MESH_ZDIRECTION, M3D::Platform::StringToWString(strValue));
		strValue.clear();
	}
	return iRet;
}

int CSVL1ToSVL2::SVL1LayerInfosSetToSVL2(Stk_LayerInfosSet* pSrcLayerInfosSet, HoteamSoft::SVLLib::Stk_LayerInfosSetPtr pLayerInfosSet, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcLayerInfosSet || pLayerInfosSet.isNull()|| pDocument .isNull())
	{
		return -1;
	}

	int nCount = pSrcLayerInfosSet->GetLayerNum();
	for (int i = 0; i < nCount; i++)
	{
		Stk_Layer* stkLayer = pSrcLayerInfosSet->GetLayerByIndex(i);
		HoteamSoft::SVLLib::Stk_LayerPtr newLayer = HoteamSoft::SVLLib::Stk_LayerPtr::CreateObject();
		pLayerInfosSet->AddLayer(newLayer);
		newLayer->SetID(stkLayer->GetID());
		newLayer->SetLayerName(stkLayer->GetLayerName());
		newLayer->SetDisplay(stkLayer->IsDisplay());
		newLayer->SetLayer(stkLayer->GetLayer());

		//int iConnectorNum = stkLayer->GetNodeConnectorNum();
		//for (int j = 0; j < iConnectorNum; j++)
		//{
		//	Stk_NodeConnector* pNodeConnector = stkLayer->GetNodeConnectorByIndex(j);
		//	if(!pNodeConnector)
		//		continue;
		//	HoteamSoft::SVLLib::Stk_ConnectorPtr pConnector = HoteamSoft::SVLLib::Stk_ConnectorPtr::CreateObject();
		//	StkEntityTypeEnum ePointingEnt, ePointedEnt;
		//	pNodeConnector->GetEntityType(ePointingEnt, ePointedEnt);
		//	pConnector->SetEntityType((HoteamSoft::SVLLib::StkEntityTypeEnum) ePointingEnt, (HoteamSoft::SVLLib::StkEntityTypeEnum)ePointedEnt);
		//	pConnector->SetIsLinkNode(pNodeConnector->IsLinkNode());
		//	pConnector->SetLinkNode(pNodeConnector->GetLinkNode());
		//	pConnector->SetIsLinkProto(pNodeConnector->IsLinkProto());
		//	pConnector->SetLinkProto(pNodeConnector->GetLinkProto());
		//	newLayer->AddNodeConnector(pConnector);
		//}
	}
	vector<Stk_LayerFilter*> filters = pSrcLayerInfosSet->GetAllLayerFilters();

	for (vector<Stk_LayerFilter*>::iterator iter = filters.begin(); iter != filters.end(); ++iter)
	{
		Stk_LayerFilter *pFilter = *iter;
		if (pFilter->GetLayerFilterName().length() > 0 && pFilter->GetLayerFilterName().at(0) == 5716)
		{
			pFilter->SetLayerFilterName(L"");
		}
		HoteamSoft::SVLLib::Stk_LayerFilterPtr newLayerFilter = HoteamSoft::SVLLib::Stk_LayerFilterPtr::CreateObject();
		pLayerInfosSet->AddLayerFilter(newLayerFilter);
		newLayerFilter->SetID(pFilter->GetID());
		newLayerFilter->SetLayerFilterName(pFilter->GetLayerFilterName());
		newLayerFilter->SetDescription(pFilter->GetDescription());

		map<STK_INT32, STK_BOOL>& selLayerMap = pFilter->GetAllMapLayerStates();
		for (map<STK_INT32, STK_BOOL>::iterator mapIter = selLayerMap.begin(); mapIter != selLayerMap.end(); ++mapIter)
		{
			newLayerFilter->AddMapLayerState(mapIter->first, mapIter->second);
		}
	}
	
	return iRet;
}

int CSVL1ToSVL2::SVL1FeatToSVL2(Stk_Feat* pSrcFeat, HoteamSoft::SVLLib::Stk_FeatPtr pTargetFeat, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pSrcFeat || pTargetFeat.isNull()|| pDocument .isNull())
	{
		return -1;
	}
	pTargetFeat->SetID(pSrcFeat->GetID());
	pTargetFeat->SetFeatType((HoteamSoft::SVLLib::StkFeatTypeEnum)pSrcFeat->GetFeatType());
	if(pSrcFeat->GetFeatName().length()>0)
	{
		pTargetFeat->SetFeatName(pSrcFeat->GetFeatName());
	}
	pTargetFeat->SetActiveed(pSrcFeat->IsActiveed());
	StkFeatTypeEnum eFeatType = pSrcFeat->GetFeatType();
	switch (eFeatType)
	{
	case FEAT_TYPE_PIPE:
	{
		Stk_Feat_Pipe* pSrcFeatPipe = pSrcFeat->GetFeatPipe();
		if (pSrcFeatPipe != NULL)
		{
			HoteamSoft::SVLLib::Stk_Feat_PipePtr pTargetPipe = HoteamSoft::SVLLib::Stk_Feat_PipePtr::CreateObject();
			iRet = SVL1FeatPipeToSVL2(pSrcFeatPipe, pTargetPipe, pDocument);
			pTargetFeat->SetFeatPipe(pTargetPipe);
		}
		break;
	}
	case FEAT_TYPE_HOLE:	
	{
		
		break;
	}
	case FEAT_TYPE_MAGNETIC_LINE:
	{
		Stk_Feat_MagneticLine* pSrcFeatMagneticLine = pSrcFeat->GetMagneticLine();
		if (pSrcFeatMagneticLine != NULL)
		{
			HoteamSoft::SVLLib::Stk_Feat_MagneticLinePtr pTargetFeatMagneticLine = HoteamSoft::SVLLib::Stk_Feat_MagneticLinePtr::CreateObject();
			iRet = SVL1FeatMagneticLineToSVL2(pSrcFeatMagneticLine, pTargetFeatMagneticLine, pDocument);
			pTargetFeat->SetMagneticLine(pTargetFeatMagneticLine);
		}
		break;
	}
	case FEAT_TYPE_MARK:
	{
		Stk_Feat_Mark* pSrcFeatMark = pSrcFeat->GetFeatMark();
		if (pSrcFeatMark != NULL)
		{
			HoteamSoft::SVLLib::Stk_Feat_MarkPtr pTargetFeatMark = HoteamSoft::SVLLib::Stk_Feat_MarkPtr::CreateObject();
			iRet = SVL1FeatMarkToSVL2(pSrcFeatMark, pTargetFeatMark, pDocument);
			pTargetFeat->SetFeatMark(pTargetFeatMark);
		}
		break;
	}
	default:
		break;
	}

	return iRet;
}

int CSVL1ToSVL2::SVL1FeatMagneticLineToSVL2(Stk_Feat_MagneticLine* pSrcFeatMagneticLine, HoteamSoft::SVLLib::Stk_Feat_MagneticLinePtr pTargetFeatMagneticLine, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	pTargetFeatMagneticLine->SetID(pSrcFeatMagneticLine->GetID());
	pTargetFeatMagneticLine->SetArrangementType((HoteamSoft::SVLLib::StkMagneticArrangementEnum)pSrcFeatMagneticLine->GetArrangementType());
	STK_RGBA32 srcColor = pSrcFeatMagneticLine->GetColor();
	HoteamSoft::SVLLib::STK_RGBA32 targetColor;
	targetColor.Red = srcColor.Red;
	targetColor.Green = srcColor.Green;
	targetColor.Blue = srcColor.Blue;
	if (fabs(srcColor.Alpha + 1.0f) > F_TOL)
	{
		targetColor.Alpha = 1.0f - srcColor.Alpha;
	}
	else
	{
		targetColor.Alpha = srcColor.Alpha;
	}
	pTargetFeatMagneticLine->SetColor(targetColor);
	pTargetFeatMagneticLine->SetEffective(pSrcFeatMagneticLine->IsEffective());
	STK_PNT32 srcStartPnt = pSrcFeatMagneticLine->GetStartPoint();
	STK_PNT32 srcEndPnt = pSrcFeatMagneticLine->GetEndPoint();
	HoteamSoft::SVLLib::STK_PNT32 targetStartPnt, targetEndPnt;
	targetStartPnt.X = srcStartPnt.X;
	targetStartPnt.Y = srcStartPnt.Y;
	targetStartPnt.Z = srcStartPnt.Z;
	targetEndPnt.X = srcEndPnt.X;
	targetEndPnt.Y = srcEndPnt.Y;
	targetEndPnt.Z = srcEndPnt.Z;
	pTargetFeatMagneticLine->SetStartPoint(targetStartPnt);
	pTargetFeatMagneticLine->SetEndPoint(targetEndPnt);
	return iRet;
}

int CSVL1ToSVL2::SVL1FeatHoleToSVL2(Stk_Feat_Hole* pSrcFeatHole, HoteamSoft::SVLLib::Stk_Feat_HolePtr pTargetFeatHole, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	pTargetFeatHole->SetID(pSrcFeatHole->GetID());
	pTargetFeatHole->SetAnchorType((HoteamSoft::SVLLib::StkHoleAnchorTypeEnum)pSrcFeatHole->GetAnchorType());
	pTargetFeatHole->SetType((HoteamSoft::SVLLib::StkHoleTypeEnum)pSrcFeatHole->GetType());
	pTargetFeatHole->SetBottomType((HoteamSoft::SVLLib::StkHoleBottomTypeEnum)pSrcFeatHole->GetBottomType());
	pTargetFeatHole->SetDepth(pSrcFeatHole->GetDepth());
	pTargetFeatHole->SetDepthHasTol(pSrcFeatHole->IsHasDepthTol());
	pTargetFeatHole->SetDepthWithTol(pSrcFeatHole->GetDepthWithTol());
	pTargetFeatHole->SetDiameter(pSrcFeatHole->GetDiameter());
	pTargetFeatHole->SetDiameterHasTol(pSrcFeatHole->IsHasDiameterTol());
	pTargetFeatHole->SetDiameterWithTol(pSrcFeatHole->GetDiameterWithTol());
	pTargetFeatHole->SetBottomAngle(pSrcFeatHole->GetBottomAngle());
	pTargetFeatHole->SetBottomAngleHasTol(pSrcFeatHole->IsHasBottomAngleTol());
	pTargetFeatHole->SetBottomAngleTol(pSrcFeatHole->GetBottomAngleTol());
	pTargetFeatHole->SetCounterAngle(pSrcFeatHole->GetCounterAngle());
	pTargetFeatHole->SetCounterAngleHasTol(pSrcFeatHole->IsHasCounterAngleTol());
	pTargetFeatHole->SetCounterAngleTol(pSrcFeatHole->GetCounterAngleTol());
	pTargetFeatHole->SetCounterDepth(pSrcFeatHole->GetCounterDepth());
	pTargetFeatHole->SetCounterDepthHasTol(pSrcFeatHole->IsHasCounterDepthTol());
	pTargetFeatHole->SetCounterDepthTol(pSrcFeatHole->GetCounterDepthTol());
	pTargetFeatHole->SetCounterDiameter(pSrcFeatHole->GetCounterDepth());
	pTargetFeatHole->SetCounterDiameterHasTol(pSrcFeatHole->IsHasCounterDiameterTol());
	pTargetFeatHole->SetCounterDiameterTol(pSrcFeatHole->GetCounterDiameterTol());

	//孔轴特征
	Stk_Feat_Axis* pSrcFeatAxis = pSrcFeatHole->GetFeatAxis();
	if(pSrcFeatAxis)
	{
		HoteamSoft::SVLLib::Stk_Feat_Axis * pTargetFeatAxis = new HoteamSoft::SVLLib::Stk_Feat_Axis();
		pTargetFeatHole->SetFeatAxis(pTargetFeatAxis);
		STK_DIR32 srcDir = pSrcFeatAxis->GetDirection();
		STK_PNT32 srcOrigin = pSrcFeatAxis->GetOrigin();
		HoteamSoft::SVLLib::STK_DIR32 targetDir;
		HoteamSoft::SVLLib::STK_PNT32 targetOrigin;
		targetDir.X = srcDir.X;
		targetDir.Y = srcDir.Y;
		targetDir.Z = srcDir.Z;
		targetOrigin.X = srcOrigin.X;
		targetOrigin.Y = srcOrigin.Y;
		targetOrigin.Z = srcOrigin.Z;
		pTargetFeatAxis->SetDirection(targetDir);
		pTargetFeatAxis->SetOrigin(targetOrigin);
	}

	//孔螺纹特征
	Stk_FeatInnerThread* pSrcFeatInnerThread = pSrcFeatHole->GetInnerThread();
	if (pSrcFeatInnerThread)
	{
		HoteamSoft::SVLLib::Stk_FeatInnerThread * pTargetFeatInnerThread = new HoteamSoft::SVLLib::Stk_FeatInnerThread();
		pTargetFeatHole->SetInnerThread(pTargetFeatInnerThread);
		pTargetFeatInnerThread->SetThreadLimitType((HoteamSoft::SVLLib::StkThreadLimitTypeEnum)pSrcFeatInnerThread->GetThreadLimitType());
		pTargetFeatInnerThread->SetThreadType((HoteamSoft::SVLLib::StkThreadTypeEnum)pSrcFeatInnerThread->GetThreadType());
		pTargetFeatInnerThread->SetDepth(pSrcFeatInnerThread->GetDepth());
		pTargetFeatInnerThread->SetDepthHasTol(pSrcFeatInnerThread->IsDepthHasTol());
		pTargetFeatInnerThread->SetDepthTol(pSrcFeatInnerThread->GetDepthTol());
		pTargetFeatInnerThread->SetDiameter(pSrcFeatInnerThread->GetDiameter());
		pTargetFeatInnerThread->SetDiameterHasTol(pSrcFeatInnerThread->IsHasDiameterTol());
		//pTargetFeatInnerThread->SetDiameterTol(pFeatInnerThread->GetDiameterTol());
		pTargetFeatInnerThread->SetPitch(pSrcFeatInnerThread->GetPitch());
		pTargetFeatInnerThread->SetPitchHasTol(pSrcFeatInnerThread->IsHasPitchTol());
		pTargetFeatInnerThread->SetPitchTol(pSrcFeatInnerThread->GetPitchTol());
		pTargetFeatInnerThread->SetRightThreaded(pSrcFeatInnerThread->IsRightThreaded());

		Stk_NodeConnector* pSrcConnector = pSrcFeatInnerThread->GetThreadLimitReference();
		if(pSrcConnector)
		{
			HoteamSoft::SVLLib::Stk_ConnectorPtr pTargetConnector = HoteamSoft::SVLLib::Stk_ConnectorPtr::CreateObject();
			pTargetFeatInnerThread->SetThreadLimitReference(pTargetConnector);
			SVL1NodeConnectorToSVL2(pSrcFeatHole->GetProtoType(), pSrcConnector, pTargetConnector, pDocument);				
		}
	}

	//Limit特征
	Stk_FeatLimit* pSrcFeatLimit = pSrcFeatHole->GetFeatLimit();
	if (pSrcFeatLimit)
	{
		HoteamSoft::SVLLib::Stk_FeatLimit * pTargetFeatLimit = new HoteamSoft::SVLLib::Stk_FeatLimit();
		pTargetFeatHole->SetFeatLimit(pTargetFeatLimit);
		pTargetFeatLimit->SetType((HoteamSoft::SVLLib::StkLimitTypeEnum)pSrcFeatLimit->GetType());
		pTargetFeatLimit->SetValue(pSrcFeatLimit->GetValue());
		
		Stk_NodeConnector* pSrcConnector = pSrcFeatLimit->GetReferenceObject();
		if (pSrcConnector)
		{
			HoteamSoft::SVLLib::Stk_ConnectorPtr pTargetConnector = HoteamSoft::SVLLib::Stk_ConnectorPtr::CreateObject();
			pTargetFeatLimit->SetReferenceObject(pTargetConnector);
			SVL1NodeConnectorToSVL2(pSrcFeatHole->GetProtoType(), pSrcConnector, pTargetConnector, pDocument);
		}
	}

	return iRet;
}


int CSVL1ToSVL2::SVL1FeatPipeToSVL2(Stk_Feat_Pipe *pSrcFeatPipe, HoteamSoft::SVLLib::Stk_Feat_PipePtr pTargetFeatPipe, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	pTargetFeatPipe->SetID(pSrcFeatPipe->GetID());
	pTargetFeatPipe->SetPipeType((HoteamSoft::SVLLib::StkPipeTypeEnum)pSrcFeatPipe->GetPipeType());
	pTargetFeatPipe->SetSectionType((HoteamSoft::SVLLib::StkPipeSectionTypeEnum)pSrcFeatPipe->GetSectionType());
	pTargetFeatPipe->SetDiameter(pSrcFeatPipe->GetDiameter());
	pTargetFeatPipe->SetTurnRadius(pSrcFeatPipe->GetTurnRadius());
	pTargetFeatPipe->SetHeight(pSrcFeatPipe->GetHeight());
	pTargetFeatPipe->SetWidth(pSrcFeatPipe->GetWidth());
	
	int iPipeFiberNum = pSrcFeatPipe->GetPipeFiberNum();
	for(int i = 0; i < iPipeFiberNum; i++)
	{
		Stk_Pipe_Fiber_Segment* pSrcPipeFiber = pSrcFeatPipe->GetPipeFiber(i);
		if(!pSrcPipeFiber)
			continue;
		HoteamSoft::SVLLib::Stk_Pipe_Fiber_Segment* pTargetPipeFiber = new HoteamSoft::SVLLib::Stk_Pipe_Fiber_Segment();
		pTargetFeatPipe->AddPipeFiber(pTargetPipeFiber);
		STK_PNT32 srcCenterPnt, srcStartPnt, srcEndPnt;
		srcCenterPnt = pSrcPipeFiber->GetCenterPoint();
		srcStartPnt = pSrcPipeFiber->GetStartPoint();
		srcEndPnt = pSrcPipeFiber->GetEndPoint();
		HoteamSoft::SVLLib::STK_PNT32 targetCenterPnt, targetStartPnt, targetEndPnt;
		targetCenterPnt.X = srcCenterPnt.X;
		targetCenterPnt.Y = srcCenterPnt.Y;
		targetCenterPnt.Z = srcCenterPnt.Z;
		targetStartPnt.X = srcStartPnt.X;
		targetStartPnt.Y = srcStartPnt.Y;
		targetStartPnt.Z = srcStartPnt.Z;
		targetEndPnt.X = srcEndPnt.X;
		targetEndPnt.Y = srcEndPnt.Y;
		targetEndPnt.Z = srcEndPnt.Z;

		pTargetPipeFiber->SetCenterPoint(targetCenterPnt);
		pTargetPipeFiber->SetStartPoint(targetStartPnt);
		pTargetPipeFiber->SetEndPoint(targetEndPnt);

		Stk_CurveEntity* pSrcCurve = pSrcPipeFiber->GetCurve();
		if (pSrcCurve)
		{
			HoteamSoft::SVLLib::Stk_CurveEntityPtr pTargetCurve = SVL1CurveToSVL2(pSrcCurve, pDocument);;
			pTargetPipeFiber->SetCurve(pTargetCurve);
		}
	}
	
	return iRet;
}

int CSVL1ToSVL2::SVL1FeatMarkToSVL2(Stk_Feat_Mark *pSrcFeatMark, HoteamSoft::SVLLib::Stk_Feat_MarkPtr pTargetFeatMark, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	pTargetFeatMark->SetID(pSrcFeatMark->GetID());
	StkMarkTypeEnum eFeatMarkType = pSrcFeatMark->GetType();
	pTargetFeatMark->SetType((HoteamSoft::SVLLib::StkMarkTypeEnum)eFeatMarkType);
	STK_RGBA32 srcColor = pSrcFeatMark->GetColor();
	HoteamSoft::SVLLib::STK_RGBA32 targetColor;
	targetColor.Red = srcColor.Red;
	targetColor.Green = srcColor.Green;
	targetColor.Blue = srcColor.Blue;
	targetColor.Alpha = srcColor.Alpha;
	pTargetFeatMark->SetColor(targetColor);
	STK_MTX32 srcMatrix = pSrcFeatMark->GetDefPlane();
	HoteamSoft::SVLLib::STK_MTX32 targetMatrix;
	for (int i = 0; i < MTX_LINE_COUNT; i++)
	{
		for (int j = 0; j < MTX_COL_COUNT; j++)
		{
			targetMatrix.PlcMatrix[i][j] = srcMatrix.PlcMatrix[i][j];
		}

	}
	pTargetFeatMark->SetDefPlane(targetMatrix);
	pTargetFeatMark->SetCurveWidth(pSrcFeatMark->GetCurveWidth());
	pTargetFeatMark->SetCurveType((HoteamSoft::SVLLib::StkCurveDispTypeEnum)pSrcFeatMark->GetCurveType());
	
	switch (eFeatMarkType)
	{
	case MARK_TYPE_RECTANGLE:
	{
		HoteamSoft::SVLLib::Stk_Mark_RectAnglePtr pRectAnglePtr = HoteamSoft::SVLLib::Stk_Mark_RectAnglePtr::DynamicCast(pTargetFeatMark);
		pRectAnglePtr->SetFillStyle((HoteamSoft::SVLLib::StkMarkFillStyleEnum)((Stk_Mark_RectAngle*)pSrcFeatMark)->GetFillStyle());
		STK_PNT32 srcLeftTopPnt, srcRightBottomPnt;
		((Stk_Mark_RectAngle*)pSrcFeatMark)->GetAnchorPoint(srcLeftTopPnt, srcRightBottomPnt);
		
		HoteamSoft::SVLLib::STK_PNT32 targetLeftTopPnt, targetRightBottomPnt;
		targetLeftTopPnt.X = srcLeftTopPnt.X;
		targetLeftTopPnt.Y = srcLeftTopPnt.Y;
		targetLeftTopPnt.Z = srcLeftTopPnt.Z;

		targetRightBottomPnt.X = srcRightBottomPnt.X;
		targetRightBottomPnt.Y = srcRightBottomPnt.Y;
		targetRightBottomPnt.Z = srcRightBottomPnt.Z;
		pRectAnglePtr->SetAnchorPoint(targetLeftTopPnt, targetRightBottomPnt);
	}
		break;
	case MARK_TYPE_ELLIPSE:
	{
		HoteamSoft::SVLLib::Stk_Mark_EllipsePtr pEllipseMarkPtr = HoteamSoft::SVLLib::Stk_Mark_EllipsePtr::DynamicCast(pTargetFeatMark);
		pEllipseMarkPtr->SetFillStyle((HoteamSoft::SVLLib::StkMarkFillStyleEnum)((Stk_Mark_Ellipse*)pSrcFeatMark)->GetFillStyle());
		STK_PNT32 srcCenterPnt = ((Stk_Mark_Ellipse*)pSrcFeatMark)->GetCenter();
		HoteamSoft::SVLLib::STK_PNT32 targetCenterPnt;
		targetCenterPnt.X = srcCenterPnt.X;
		targetCenterPnt.Y = srcCenterPnt.Y;
		targetCenterPnt.Z = srcCenterPnt.Z;
		pEllipseMarkPtr->SetCenter(targetCenterPnt);

		pEllipseMarkPtr->SetMajorRadius(((Stk_Mark_Ellipse*)pSrcFeatMark)->GetMajorRadius());
		pEllipseMarkPtr->SetMinorRadius(((Stk_Mark_Ellipse*)pSrcFeatMark)->GetMinorRadius());

		STK_DIR32 srcNormal = ((Stk_Mark_Ellipse*)pSrcFeatMark)->GetNormal();
		HoteamSoft::SVLLib::STK_DIR32 targetNormal;
		targetNormal.X = srcNormal.X;
		targetNormal.Y = srcNormal.Y;
		targetNormal.Z = srcNormal.Z;
		pEllipseMarkPtr->SetNormal(targetNormal);

		STK_DIR32 srcOriginDir = ((Stk_Mark_Ellipse*)pSrcFeatMark)->GetOriginDir();
		HoteamSoft::SVLLib::STK_DIR32 targetOriginDir;
		targetOriginDir.X = srcOriginDir.X;
		targetOriginDir.Y = srcOriginDir.Y;
		targetOriginDir.Z = srcOriginDir.Z;
		pEllipseMarkPtr->SetOriginDir(targetOriginDir);
	}
		break;
	case MARK_TYPE_FREEHAND:
	{
		HoteamSoft::SVLLib::Stk_Mark_FreeHandPtr pFreeHandMarkPtr = HoteamSoft::SVLLib::Stk_Mark_FreeHandPtr::DynamicCast(pTargetFeatMark);
		STK_FLOAT32 fUMin, fUMax;
		((Stk_Mark_FreeHand*)pSrcFeatMark)->GetDomain(fUMin, fUMax);
		pFreeHandMarkPtr->SetDomain(fUMin, fUMax);
		int iPointNum = ((Stk_Mark_FreeHand*)pSrcFeatMark)->GetPointsNum();
		for (int i = 0; i < iPointNum; i++)
		{
			STK_PNT32 srcPnt = ((Stk_Mark_FreeHand*)pSrcFeatMark)->GetPointByIndex(i);
			HoteamSoft::SVLLib::STK_PNT32 targetPnt;
			targetPnt.X = srcPnt.X;
			targetPnt.Y = srcPnt.Y;
			targetPnt.Z = srcPnt.Z;
			pFreeHandMarkPtr->AddPoint(targetPnt);
		}
	}
		break;
	default:
		break;
	}
	
	return iRet;
}



int CSVL1ToSVL2::SVL1OutFileToSVL2(Stk_DocumentManager *pStkDocMgr, const wstring wstrFileExt, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pStkDocMgr || wstrFileExt.empty() || pDocument .isNull())
	{
		return -1;
	}
	wstring wstrSrcFilePath = pStkDocMgr->GetFileLoadPath();
	int iPos = wstrSrcFilePath.find_last_of('.');
	wstrSrcFilePath = wstrSrcFilePath.substr(0, iPos);
	wstrSrcFilePath.append(L".");
	wstrSrcFilePath.append(wstrFileExt);

	wstring wstrOutputFullPath = pDocument->GetOutputFullPath();
	wstring wstrOutputName = pDocument->GetOutputName();
	wstring wstrNewFileName = wstrOutputName + L"." + wstrFileExt;
	wstring wstrNewFilePath = wstrOutputFullPath + wstrOutputName + L"." + wstrFileExt;

	std::fstream fs;
	string strSrcFilePath = M3D::Platform::WStringToString(wstrSrcFilePath);
	fs.open(strSrcFilePath.c_str(), ios::in | ios::binary);
	if (fs.is_open()) {
		fstream fsCoper(M3D::Platform::WStringToString(wstrNewFilePath), ios::binary | ios::out);
		fsCoper << fs.rdbuf();
		fs.close();
		pDocument->AddSVLXFileItem(M3D::Platform::WStringToString(wstrNewFileName));
	}
	else
	{
		return -1;
	}

	return iRet;
}

int CSVL1ToSVL2::GetSVL1TopProtoTypeUserData(Stk_DocumentManager *pStkDocMgr, const wstring wstrUserDataName, STK_CHAR** dataPP, STK_SIZE_T &dataSize)
{
	int iRet = -1;
	
	if (!pStkDocMgr || wstrUserDataName.empty() || !dataPP)
	{
		return -1;
	}
	*dataPP = NULL;
	dataSize = 0;
	int iTopProtoTypeNum = pStkDocMgr->GetTopProtoTypeNum();
	if (iTopProtoTypeNum <= 0)
	{
		return -1;
	}
	Stk_ProtoType* pSrcProtoType = pStkDocMgr->GetTopProtoTypeByIndex(0);
	if (!pSrcProtoType)
		return -1;

	map<STK_ID, Stk_UserData*>* mapDataP = pSrcProtoType->GetUserDataByName(wstrUserDataName);
	if (mapDataP != NULL && mapDataP->size() > 0)
	{
		map<STK_ID, Stk_UserData*>::iterator itMapAnimation = mapDataP->begin();
		wstring wstrDataName = L"";
		STK_ID id = 0;
		wstring wstrFileName = L"";
		itMapAnimation->second->GetUserData(wstrDataName, id, *dataPP, dataSize, wstrFileName);
		if (dataSize > 0)
		{
			iRet = 0;
		}
	}
	
	return iRet;
}

int CSVL1ToSVL2::SVL1SAToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pStkDocMgr || pDocument .isNull())
	{
		return -1;
	}
	wstring wstrSrcFilePath = pStkDocMgr->GetFileLoadPath();
	int iPos = wstrSrcFilePath.find_last_of('.');
	wstring wstrSrcSAFilePath = wstrSrcFilePath.substr(0, iPos);
	wstrSrcSAFilePath.append(L".sa");

	wstring wstrOutputFullPath = pDocument->GetOutputFullPath();//pStkPicture->GetPictureSavePath();
	wstring wstrOutputName = pDocument->GetOutputName();
	wstring wstrNewSAFileName = wstrOutputName + L".animation";
	wstring wstrNewSAFilePath = wstrOutputFullPath + wstrOutputName + L".animation";
	
	std::fstream fs;
	string strSrcFilePath = M3D::Platform::WStringToString(wstrSrcSAFilePath);
	fs.open(strSrcFilePath.c_str(), ios::in | ios::binary);
	if (fs.is_open()) {
		fstream fsCoper(M3D::Platform::WStringToString(wstrNewSAFilePath), ios::binary | ios::out);
		fsCoper << fs.rdbuf();
		fs.close();
	}
	else
	{
		STK_CHAR* dataP = NULL;
		STK_SIZE_T dataSize = 0;
		if(GetSVL1TopProtoTypeUserData(pStkDocMgr, L"Animation", &dataP, dataSize)==0)
		{
			std::ofstream fout(M3D::Platform::WStringToString(wstrNewSAFilePath), std::ios::binary);
			fout.write(dataP, sizeof(char) * dataSize);
			fout.close();
		}
		else
		{
			return -1;
		}
	}
	pDocument->AddSVLXFileItem(M3D::Platform::WStringToString(wstrNewSAFileName));

	if (iRet == 0)
	{
		wstring wstrSrcResFolderPath = pStkDocMgr->GetFileLoadPath();
		int iPos = wstrSrcResFolderPath.find_last_of('.');
		wstrSrcResFolderPath = wstrSrcResFolderPath.substr(0, iPos);
		wstrSrcResFolderPath.append(L"_sares");

		wstring wstrNewResFolderName = L"sares";
		wstring wstrNewResFolderPath = wstrOutputFullPath + L"sares";

		
		if (wstrSrcResFolderPath != wstrNewResFolderPath)
		{
			string strNewResFolderPath = M3D::Platform::WStringToString(wstrNewResFolderPath);
			//rmdir(strNewResFolderPath.c_str());
            if (M3D::FileHelper::FileExist(M3D::Platform::WStringToString(wstrSrcResFolderPath).c_str())) {
                M3D::FileHelper::copy_folder(M3D::Platform::WStringToString(wstrSrcResFolderPath).c_str(),
                                             M3D::Platform::WStringToString(wstrNewResFolderPath).c_str());
            }
		}
		pDocument->AddSVLXFileItem(M3D::Platform::WStringToString(wstrNewResFolderName));
	}
	return iRet;
}
int CSVL1ToSVL2::SVL1TaskToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pStkDocMgr || pDocument.isNull())
		return -1;
	wstring wstrSrcFilePath = pStkDocMgr->GetFileLoadPath();
	int iPos = wstrSrcFilePath.find_last_of('.');
	wstring wstrSrcTaskFilePath = wstrSrcFilePath.substr(0, iPos);
	wstrSrcTaskFilePath.append(L".task");

	wstring wstrOutputFullPath = pDocument->GetOutputFullPath();//pStkPicture->GetPictureSavePath();
	wstring wstrOutputName = pDocument->GetOutputName();
	wstring wstrNewTaskFileName = wstrOutputName + L".task";
	wstring wstrNewTaskFilePath = wstrOutputFullPath + wstrOutputName + L".task";

	std::fstream fs;
	fs.open(M3D::Platform::WStringToString(wstrSrcTaskFilePath), ios::in | ios::binary);
	if (fs.is_open()) {
		fstream fsCoper(M3D::Platform::WStringToString(wstrNewTaskFilePath), ios::binary | ios::out);
		fsCoper << fs.rdbuf();
		fs.close();
	}
	else
		return -1;
//	pDocument->AddSVLXFileItem(M3D::Platform::WStringToString(wstrNewTaskFileName));
	return iRet;
}
int CSVL1ToSVL2::SVL1LoadInfToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pStkDocMgr || pDocument .isNull())
	{
		return -1;
	}
	iRet = SVL1OutFileToSVL2(pStkDocMgr, L"info", pDocument);
	
	return iRet;
}

int CSVL1ToSVL2::SVL1PreviewToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pStkDocMgr || pDocument .isNull())
	{
		return -1;
	}
	wstring wstrPreViewFileName = pDocument->GetOutputName();
	wstring wstrOutputFullPath = pDocument->GetOutputFullPath();//pStkPicture->GetPictureSavePath();

																//设置预览图
	Stk_Preview* pPreview = pStkDocMgr->GetTopProtoTypeByIndex(0)->GetFile()->GetFilePreview();

	if (pPreview)
	{
		STK_CHAR* chBuf = pPreview->GetStream();
		STK_UINT32 iBufSize = pPreview->GetStreamSize();
		if(iBufSize >0 )
		{
			wstring wstrExt = L".jpg";
			StkPreviewTypeEnum picType = pPreview->GetType();
			switch (picType)
			{
			case PICTURE_TYPE_UNKNOWN:
				break;
			case PICTURE_TYPE_JPG:
				wstrExt = L".jpg";
				break;
			case PICTURE_TYPE_BMP:
				wstrExt = L".bmp";
				break;
			case PICTURE_TYPE_PNG:
				wstrExt = L".png";
				break;
			case PICTURE_TYPE_CGM:
				wstrExt = L".cgm";
				break;
			case PICTURE_TYPE_GIF:
				wstrExt = L".gif";
				break;
			case PICTURE_TYPE_TIFF:
				wstrExt = L".tiff";
				break;
			case PICTURE_TYPE_ICO:
				wstrExt = L".ico";
				break;
			default:
				break;
			}
			wstring wstrPreviewFilePath = wstrOutputFullPath;
			wstrPreViewFileName.append(wstrExt);

			//wstring wstrImageDir = wstrOutputFullPath + L"\\images";
			//if (_taccess(wstrImageDir.c_str(), 0) == -1)
			//{
			//	_tmkdir(wstrImageDir.c_str());
			//}

			wstrPreviewFilePath.append(wstrPreViewFileName);
			std::ofstream fout(M3D::Platform::WStringToString(wstrPreviewFilePath), std::ios::binary);
			fout.write(chBuf, sizeof(char) * iBufSize);
			fout.close();

			pDocument->AddSVLXFileItem(M3D::Platform::WStringToString(wstrPreViewFileName));
		}
	}
	return iRet;
}

int CSVL1ToSVL2::SVL1VBomToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pStkDocMgr || pDocument .isNull())
	{
		return -1;
	}
	iRet = SVL1OutFileToSVL2(pStkDocMgr, L"vbom", pDocument);

	if(iRet!= 0)
	{
		wstring wstrOutputFullPath = pDocument->GetOutputFullPath();//pStkPicture->GetPictureSavePath();
		wstring wstrOutputName = pDocument->GetOutputName();
		wstring wstrNewFileName = wstrOutputName + L".vbom";
		wstring wstrNewFilePath = wstrOutputFullPath + wstrOutputName + L".vbom";

		STK_CHAR* dataP = NULL;
		STK_SIZE_T dataSize = 0;
		if (GetSVL1TopProtoTypeUserData(pStkDocMgr, L"VBOM", &dataP, dataSize) == 0)
		{
			std::ofstream fout(M3D::Platform::WStringToString(wstrNewFilePath), std::ios::binary);
			fout.write(dataP, sizeof(char) * dataSize);
			fout.close();
			pDocument->AddSVLXFileItem(M3D::Platform::WStringToString(wstrNewFileName));
			iRet = 0;
		}
	}

	return iRet;
}
int CSVL1ToSVL2::SVL1GroupToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pStkDocMgr || pDocument .isNull())
	{
		return -1;
	}
	iRet = SVL1OutFileToSVL2(pStkDocMgr, L"group", pDocument);

	return iRet;
}

int CSVL1ToSVL2::SVL1MalfunctionToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pStkDocMgr || pDocument .isNull())
	{
		return -1;
	}
	iRet = SVL1OutFileToSVL2(pStkDocMgr, L"manual", pDocument);

	if (iRet != 0)
	{
		wstring wstrOutputFullPath = pDocument->GetOutputFullPath();//pStkPicture->GetPictureSavePath();
		wstring wstrOutputName = pDocument->GetOutputName();
		wstring wstrNewFileName = wstrOutputName + L".manual";
		wstring wstrNewFilePath = wstrOutputFullPath + wstrOutputName + L".manual";

		STK_CHAR* dataP = NULL;
		STK_SIZE_T dataSize = 0;
		if (GetSVL1TopProtoTypeUserData(pStkDocMgr, L"ServiceManual", &dataP, dataSize) == 0)
		{
			std::ofstream fout(M3D::Platform::WStringToString(wstrNewFilePath), std::ios::binary);
			fout.write(dataP, sizeof(char) * dataSize);
			fout.close();
			pDocument->AddSVLXFileItem(M3D::Platform::WStringToString(wstrNewFileName));
			iRet = 0;
		}
	}
	if (iRet == 0)
	{
		wstring wstrOutputFullPath = pDocument->GetOutputFullPath();//pStkPicture->GetPictureSavePath();
		wstring wstrSrcResFolderPath = pStkDocMgr->GetFileLoadPath();
		int iPos = wstrSrcResFolderPath.find_last_of('.');
		wstrSrcResFolderPath = wstrSrcResFolderPath.substr(0, iPos);
		wstrSrcResFolderPath.append(L"_manual");

		wstring wstrNewResFolderName = L"manual";
		wstring wstrNewResFolderPath = wstrOutputFullPath + L"manual";

		if (wstrSrcResFolderPath != wstrNewResFolderPath)
		{
			M3D::FileHelper::copy_folder(M3D::Platform::WStringToString(wstrSrcResFolderPath).c_str(), M3D::Platform::WStringToString(wstrNewResFolderPath).c_str());
			//CopyDir cd;
			//cd.copy(M3D::Platform::WStringToString(wstrSrcResFolderPath), M3D::Platform::WStringToString(wstrNewResFolderPath));
		}
		pDocument->AddSVLXFileItem(M3D::Platform::WStringToString(wstrNewResFolderName));
	}
	return iRet;
}

int CSVL1ToSVL2::SVL1CablePropertyFileToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pStkDocMgr || pDocument .isNull())
	{
		return -1;
	}
	iRet = SVL1OutFileToSVL2(pStkDocMgr, L"inf", pDocument);

	if (iRet != 0)
	{
		wstring wstrOutputFullPath = pDocument->GetOutputFullPath();//pStkPicture->GetPictureSavePath();
		wstring wstrOutputName = pDocument->GetOutputName();
		wstring wstrNewFileName = wstrOutputName + L".inf";
		wstring wstrNewFilePath = wstrOutputFullPath + wstrOutputName + L".inf";

		STK_CHAR* dataP = NULL;
		STK_SIZE_T dataSize = 0;
		if (GetSVL1TopProtoTypeUserData(pStkDocMgr, L"CableInfo", &dataP, dataSize) == 0)
		{
			std::ofstream fout(M3D::Platform::WStringToString(wstrNewFilePath), std::ios::binary);
			fout.write(dataP, sizeof(char) * dataSize);
			fout.close();
			pDocument->AddSVLXFileItem(M3D::Platform::WStringToString(wstrNewFileName));
			iRet = 0;
		}
	}

	return iRet;
}

int CSVL1ToSVL2::SVL1HotSpotToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pStkDocMgr || pDocument .isNull())
	{
		return -1;
	}
	iRet = SVL1OutFileToSVL2(pStkDocMgr, L"hotspot", pDocument);
	wstring wstrOutputFullPath = pDocument->GetOutputFullPath();//pStkPicture->GetPictureSavePath();
	wstring wstrOutputName = pDocument->GetOutputName();
	if (iRet != 0)
	{
		wstring wstrNewFileName = wstrOutputName + L".hotspot";
		wstring wstrNewFilePath = wstrOutputFullPath + wstrOutputName + L".hotspot";

		STK_CHAR* dataP = NULL;
		STK_SIZE_T dataSize = 0;
		if (GetSVL1TopProtoTypeUserData(pStkDocMgr, L"HotSpot", &dataP, dataSize) == 0)
		{
			std::ofstream fout(M3D::Platform::WStringToString(wstrNewFilePath), std::ios::binary);
			fout.write(dataP, sizeof(char) * dataSize);
			fout.close();
			pDocument->AddSVLXFileItem(M3D::Platform::WStringToString(wstrNewFileName));
			iRet = 0;
		}
	}
	if(iRet == 0)
	{
		wstring wstrSrcResFolderPath = pStkDocMgr->GetFileLoadPath();
		int iPos = wstrSrcResFolderPath.find_last_of('.');
		wstrSrcResFolderPath = wstrSrcResFolderPath.substr(0, iPos);
		wstrSrcResFolderPath.append(L"_hotspotres");

		wstring wstrNewResFolderName = L"hotspotres";
		wstring wstrNewResFolderPath = wstrOutputFullPath + L"hotspotres";

		if (wstrSrcResFolderPath != wstrNewResFolderPath)
		{
			//CopyDir cd;
            if (M3D::FileHelper::FileExist(M3D::Platform::WStringToString(wstrSrcResFolderPath).c_str())) {
                M3D::FileHelper::copy_folder(M3D::Platform::WStringToString(wstrSrcResFolderPath).c_str(),
                                             M3D::Platform::WStringToString(wstrNewResFolderPath).c_str());
            }
            
			
		}
		pDocument->AddSVLXFileItem(M3D::Platform::WStringToString(wstrNewResFolderName));
	}

	return iRet;
}


int CSVL1ToSVL2::SVL1ArrowToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pStkDocMgr || pDocument .isNull())
	{
		return -1;
	}
	
	wstring wstrOutputFullPath = pDocument->GetOutputFullPath();//pStkPicture->GetPictureSavePath();
	wstring wstrOutputName = pDocument->GetOutputName();
	wstring wstrNewFileName = wstrOutputName + L".arrow";
	wstring wstrNewFilePath = wstrOutputFullPath + wstrOutputName + L".arrow";

	STK_CHAR* dataP = NULL;
	STK_SIZE_T dataSize = 0;
	if (GetSVL1TopProtoTypeUserData(pStkDocMgr, L"ARROWLIST", &dataP, dataSize) == 0)
	{
		std::ofstream fout(M3D::Platform::WStringToString(wstrNewFilePath), std::ios::binary);
		fout.write(dataP, sizeof(char) * dataSize);
		fout.close();
		pDocument->AddSVLXFileItem(M3D::Platform::WStringToString(wstrNewFileName));
		iRet = 0;
	}
	

	return iRet;
}

int CSVL1ToSVL2::SVL1ViewToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pStkDocMgr || pDocument .isNull())
	{
		return -1;
	}
	iRet = SVL1OutFileToSVL2(pStkDocMgr, L"views", pDocument);

	return iRet;
}

int CSVL1ToSVL2::SVL1UserNoteToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pStkDocMgr || pDocument .isNull())
	{
		return -1;
	}

	wstring wstrOutputFullPath = pDocument->GetOutputFullPath();//pStkPicture->GetPictureSavePath();
	wstring wstrOutputName = pDocument->GetOutputName();
	wstring wstrNewFileName = wstrOutputName + L".usernote";
	wstring wstrNewFilePath = wstrOutputFullPath + wstrOutputName + L".usernote";

	STK_CHAR* dataP = NULL;
	STK_SIZE_T dataSize = 0;
	if (GetSVL1TopProtoTypeUserData(pStkDocMgr, L"UserNote", &dataP, dataSize) == 0)
	{
		std::ofstream fout(M3D::Platform::WStringToString(wstrNewFilePath), std::ios::binary);
		fout.write(dataP, sizeof(char) * dataSize);
		fout.close();
		pDocument->AddSVLXFileItem(M3D::Platform::WStringToString(wstrNewFileName));
		iRet = 0;
	}
	return iRet;
}

