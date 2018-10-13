// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_View.cpp
 *
 *	@brief	视图类
 *
 *	@par	历史:
 *		2013/08/10	创建。Gaoqq@HOTEAMSOFT
****************************************************************************/

#include "StdAfx.h"
#include "Stk_View.h"
#include "Stk_Manager.h"
#include "Stk_Platform.h"
#include "Stk_DocumentManager.h"
#ifdef _WINDOWS_
#include "Stk_LogFile.h"
#endif

/************************************************************************//**
 *	@brief		Stk_View构造函数
 *	@param[in]	i_stkProtoTypeP		ProtoType对象指针
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_View::Stk_View(Stk_ProtoType* i_stkProtoTypeP)
{
	m_CameraP = NULL;
	m_bHasClipPlan = FALSE;
	m_bIsActivated = TRUE;
	m_stkProtoP = i_stkProtoTypeP;
	m_ID = 0;
	m_eUsageType = VIEW_USAGE_UNKNOWN;
	m_fTransparency = 1.0;
	m_NodeP = NULL;
}

/************************************************************************//**
 *	@brief		Stk_View析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		如果已关联到Node，会自动析构Node并解除Node父子关联
 *	@attention	无
 ****************************************************************************/

Stk_View::~Stk_View(void)
{
	if (m_CameraP != NULL)
	{
		delete m_CameraP;
		m_CameraP = NULL;
	}
	//if(m_pClippingPlan != NULL){
	//	delete m_pClippingPlan;
	//	m_pClippingPlan = NULL;
	//}
	for (int ii = 0; ii < m_vcNodeConnectors.size(); ii++)
	{
		if (m_vcNodeConnectors[ii] != NULL){
			delete m_vcNodeConnectors[ii];
			m_vcNodeConnectors[ii] = NULL;
		}
	}
	for (int ii = 0; ii < m_vcClippingPlans.size(); ii++)
	{
		if (m_vcClippingPlans[ii] != NULL)
		{
			delete m_vcClippingPlans[ii];
			m_vcClippingPlans[ii] = NULL;
		}
	}
	// 实例属性释放
	map<wstring, Stk_InsAttribute*>::iterator itorInsAttr = m_mapInsAttribute.begin();
	while(itorInsAttr != m_mapInsAttribute.end())
	{
		if (itorInsAttr->second != NULL)
		{
			delete itorInsAttr->second;
			itorInsAttr->second = NULL;
		}
		itorInsAttr++;
	}

	// 删除所从属的Node节点
	if (m_NodeP!=NULL)
	{
		m_NodeP->SetView(NULL); //防止析构Node时再次析构View造成死循环
		delete m_NodeP;
		m_NodeP = NULL;
	}

	m_stkProtoP->GetProtoIDManager()->DelViewByID(m_ID);
}

/************************************************************************//**
 *	@brief		设置视图ID
 *	@param[in]	i_ID
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_View::SetID(STK_ID i_ID)
{
	m_ID = i_ID;
}

/************************************************************************//**
 *	@brief		获取视图ID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_ID	视图ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_ID Stk_View::GetID()
{
	return m_ID;
}

/************************************************************************//**
 *	@brief		注册View ID
 *	@param[in]	i_idView		视图ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS		注册成功
 *	@retval		STK_ERROR		注册失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_View::RegisterID(STK_ID i_idView)
{
	STK_STATUS ret=m_stkProtoP->GetProtoIDManager()->RegViewID(this, i_idView);
	if (ret==STK_SUCCESS)
	{
		m_ID = i_idView;
		return STK_SUCCESS;
	} 
	else
	{
		return STK_ERROR;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		设置透明度
 *	@param[in]	i_fTrans		透明度
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_View::SetTransparency(STK_FLOAT32 i_fTrans)
{
	m_fTransparency = i_fTrans;
}

/************************************************************************//**
 *	@brief		获取透明度
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_fTransparency	透明度
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_FLOAT32 Stk_View::GetTransparency()
{
	return m_fTransparency;
}

/************************************************************************//**
 *	@brief		设置视图定义类型
 *	@param[in]	i_eUsageType	视图定义类型
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_View::SetUsageType(StkViewUsageEnum i_eUsageType)
{
	m_eUsageType = i_eUsageType;
}

/************************************************************************//**
 *	@brief		获取视图定义类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_eUsageType	视图的应用类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/

StkViewUsageEnum Stk_View::GetUsageType()
{
	return m_eUsageType;
}

/************************************************************************//**
 *	@brief		设置视图名
 *	@param[in]	inName			视图名
 *	@param[out]	无
 *	@retval		无
 *	@note		为保持Node与要素名称相同，在为要素设置名称时，同时设置Node名
 *	@attention	无
 ****************************************************************************/

void Stk_View::SetName(const wstring &inName)
{
	m_wstrName = inName;
	if (m_NodeP != NULL)
	{
		m_NodeP->SetNodeName(inName);
	}
}

/************************************************************************//**
 *	@brief		获取视图名
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrName		视图名
 *	@note		无
 *	@attention	无
 ****************************************************************************/

wstring Stk_View::GetName(void)
{
	return m_wstrName;
}

/************************************************************************//**
 *	@brief		改变View名称
 *	@param[in]	i_strName		View名称
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		保持从Node到要素方向的名字一致
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_View::ChangeName(const wstring &i_strName)
{
	m_wstrName = i_strName;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		设置照相机
 *	@param[in]	i_CameraP		照相机对象
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_View::SetCamera(Stk_Camera* i_CameraP)
{
	m_CameraP = i_CameraP;
}

/************************************************************************//**
 *	@brief		获取照相机
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_CameraP		照相机对象
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Camera*	Stk_View::GetCamera(void)
{
	return m_CameraP;
}

/************************************************************************//**
 *	@brief		读取文件中的Body数据
 *	@param[in]	i_FileP				文件对象指针
 *	@param[in]	i_ViewHeadP			View记录头指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_ERROR			读取失败
 *	@retval		STK_ERROR_MEMORY	内存申请失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_View::ReadFile(Stk_File *i_FileP, StkSegmentHead *i_ViewHeadP)
{
	StkSegmentHead	head={0};
	StkControl		**RecordPP = NULL;
	STK_UINT32		ii;
	StkViewRec		*ViewRecP = NULL;
	StkViewRec2		*ViewRec2P = NULL;
	StkViewRec3		*ViewRec3P = NULL;
	StkViewRec4		*ViewRec4P = NULL;
	StkViewRec5		*ViewRec5P = NULL;
	StkViewRec6		*ViewRec6P = NULL;
	Stk_Camera		*StkCameraP = NULL;
	StkCameraRec	*CamRecP = NULL;
	StkCameraMatrixRec	*CamMtxRecP = NULL;
	Stk_NodeConnector *StkNodeConnectorP = NULL;
	STK_STATUS			eState;
	StkLineSetRec4*		LineSetRecP4 = NULL;
	StkEntityNameRec*	stkEntityNameRecP = NULL;

	// 注册ID
	RegisterID(i_ViewHeadP->id);	//注册ID，放到ReadFile里面

	/*
	 * 读取PMI段的数据
	 */
	RecordPP = (StkControl **)calloc(i_ViewHeadP->recnum, sizeof(StkControl*));
	if (i_ViewHeadP->recnum != 0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = i_FileP->ReadSegmentAllRecordData(i_ViewHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}

	/*
	 * 读取VIEW段的各记录
	 */
	for (ii = 0; ii < i_ViewHeadP->recnum; ii++)
	{
		if (RecordPP[ii]->type == REC_TYPE_VIEW)
		{
			/*
			 *	填充View基础部分
			 */
			if (RecordPP[ii]->version == 1)
			{
				ViewRecP = (StkViewRec *)RecordPP[ii];
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(ViewRecP->name);
#else
				m_wstrName = ViewRecP->name;
#endif
				//if( ViewRecP->hasItem&STK_VIEW_HAS_CLIP){ //不需要
				//	m_bHasClipPlan = TRUE;
				//}
				if (ViewRecP->isItem & STK_VIEW_IS_ACTIVED)
				{
					SetActivated(TRUE);
				}
			}
			else if (RecordPP[ii]->version == 2)
			{
				ViewRec2P = (StkViewRec2 *)RecordPP[ii];
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(ViewRec2P->name);
#else
				m_wstrName = ViewRec2P->name;
#endif
				if (ViewRec2P->isItem & STK_VIEW_IS_ACTIVED)
				{
					SetActivated(TRUE);
				}
			}
			else if (RecordPP[ii]->version == 3)
			{
				ViewRec3P = (StkViewRec3 *)RecordPP[ii];
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(ViewRec3P->name);
#else
				m_wstrName = ViewRec3P->name;
#endif
				if (ViewRec3P->isItem & STK_VIEW_IS_ACTIVED)
				{
					SetActivated(TRUE);
				}
				SetUsageType((StkViewUsageEnum)ViewRec3P->DefType); // 定义类型
			}
			else if (RecordPP[ii]->version == 4)
			{
				ViewRec4P = (StkViewRec4 *)RecordPP[ii];
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(ViewRec4P->name);
#else
				m_wstrName = ViewRec4P->name;
#endif
				if (ViewRec4P->isItem & STK_VIEW_IS_ACTIVED)
				{
					SetActivated(TRUE);
				}
				m_fTransparency = ViewRec4P->Transparency;
				SetUsageType((StkViewUsageEnum)ViewRec4P->usetype); // 应用类型 （发现之前的有误，这里修复正确）
			}
			else if (RecordPP[ii]->version == 5)
			{
				ViewRec5P = (StkViewRec5 *)RecordPP[ii];
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(ViewRec5P->name);
#else
				m_wstrName = ViewRec5P->name;
#endif
				if (ViewRec5P->isItem & STK_VIEW_IS_ACTIVED)
				{
					SetActivated(TRUE);
				}
				m_fTransparency = ViewRec5P->Transparency;
				SetUsageType((StkViewUsageEnum)ViewRec5P->usetype); // 应用类型 （发现之前的有误，这里修复正确）
			}
			else if (RecordPP[ii]->version == 6)
			{
				ViewRec6P = (StkViewRec6 *)RecordPP[ii];
				if (ViewRec6P->isItem & STK_VIEW_IS_ACTIVED)
				{
					SetActivated(TRUE);
				}
				m_fTransparency = ViewRec6P->Transparency;
				SetUsageType((StkViewUsageEnum)ViewRec6P->usetype); // 应用类型 （发现之前的有误，这里修复正确）
			}
			else
			{
				//错误
			}
		}
		else if (RecordPP[ii]->type == REC_TYPE_ENTITY_NAME)
		{
			if (RecordPP[ii]->version == 1)
			{
				stkEntityNameRecP = (StkEntityNameRec*)RecordPP[ii];
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(stkEntityNameRecP->atr); 
#else
				m_wstrName = stkEntityNameRecP->atr; 
#endif
			}
		}
		else if (RecordPP[ii]->type == REC_TYPE_CAMERA)
		{
			if (RecordPP[ii]->version == 1)
			{
				CamRecP = (StkCameraRec*)RecordPP[ii];
				StkCameraP = new Stk_Camera( m_stkProtoP);
				StkCameraP->SetID( CamRecP->ID);
				StkCameraP->SetOriginPoint( CamRecP->OriginPoint);
				StkCameraP->SetTargetVector( CamRecP->TargetVector);
				StkCameraP->SetUpVector( CamRecP->UpVector);
				StkCameraP->SetAngle( CamRecP->Angle);
				StkCameraP->SetFocus( CamRecP->Focus);
				StkCameraP->SetZoomFactor( CamRecP->ZoomFactor);
				StkCameraP->SetProjectType( CamRecP->ProjType);
				SetCamera(StkCameraP);
			}
		}
		else if (RecordPP[ii]->type == REC_TYPE_CAMERA_MTX)
		{
			if (RecordPP[ii]->version == 1)
			{
				CamMtxRecP = (StkCameraMatrixRec*)RecordPP[ii];
				StkCameraP = new Stk_Camera( m_stkProtoP);
				StkCameraP->SetID( CamMtxRecP->ID);
				StkCameraP->SetOriginPoint(CamMtxRecP->OriginPoint);
				StkCameraP->SetZoomFactor(CamMtxRecP->ZoomFactor);
				StkCameraP->SetProjectType(CamMtxRecP->ProjType);

				double dMatrix[4][4];
				for (int ii = 0; ii < 4; ii++)
				{
					for (int jj = 0; jj < 4; jj++)
					{
						dMatrix[ii][jj] = CamMtxRecP->Matrix.PlcMatrix[ii][jj];
					}
				}
				StkCameraP->SetMatrix(dMatrix);
				SetCamera(StkCameraP);
			}
		}
		else if (RecordPP[ii]->type == REC_TYPE_CLIP_PLAN)
		{
			if (RecordPP[ii]->version == 1)
			{
				StkClipPlanRec *ClipPlanP = NULL;
				Stk_PlaneSurface *StkPlaneP = NULL;
				StkPlaneP = new Stk_PlaneSurface();
				ClipPlanP = (StkClipPlanRec*)RecordPP[ii];
				//StkPlaneP->SetFaceType(StkSurfaceTypeEnum::SURFACE_TYPE_PLANE);
				StkPlaneP->SetID(ClipPlanP->ID);
				StkPlaneP->SetOrigin(ClipPlanP->OriginPoint);
				StkPlaneP->SetNormal(ClipPlanP->Normal);
				StkPlaneP->SetUVDir(ClipPlanP->UVector, ClipPlanP->VVector);

				AddClippingPlan(StkPlaneP);
			}
			else if (RecordPP[ii]->version == 2)
			{
				StkClipPlanRec2 *ClipPlanP = NULL;
				Stk_PlaneSurface *StkPlaneP = NULL;
				StkPlaneP = new Stk_PlaneSurface();
				ClipPlanP = (StkClipPlanRec2*)RecordPP[ii];
				//StkPlaneP->SetFaceType(StkSurfaceTypeEnum::SURFACE_TYPE_PLANE);
				StkPlaneP->SetID(ClipPlanP->ID);
				StkPlaneP->SetOrigin(ClipPlanP->OriginPoint);
				StkPlaneP->SetNormal(ClipPlanP->Normal);
				StkPlaneP->SetUVDir(ClipPlanP->UVector, ClipPlanP->VVector);
				StkPlaneP->SetDistance(ClipPlanP->fDistance);

				AddClippingPlan(StkPlaneP);
			}
		}
		else if (RecordPP[ii]->type == REC_TYPE_CLIPPING_CURVE)
		{
			if (RecordPP[ii]->version == 1)
			{
				Stk_PlaneSurface*		stkPlaneSurfaceP = NULL;
				StkClippingCurveRec*	stkClippingCurveRecP = NULL;
				Stk_ClippingCurve*		stkClippingCurveP = NULL;
				stkClippingCurveP = new Stk_ClippingCurve();
				stkClippingCurveRecP = (StkClippingCurveRec*)RecordPP[ii];
				stkClippingCurveP->SetDrawStyle((StkClippingCurveDrawStyleEnum)stkClippingCurveRecP->nuDrawStyle);
				stkClippingCurveP->SetDrawType((StkClippingCurveDrawTypeEnum)stkClippingCurveRecP->nuDrawType);
				stkClippingCurveP->SetAngle(stkClippingCurveRecP->fAngle);
				stkClippingCurveP->SetRatio(stkClippingCurveRecP->fRatio);
				stkClippingCurveP->SetColor(stkClippingCurveRecP->rgbaCurve);
				if (stkClippingCurveRecP->IsItem & STK_CLIPPINGCURVE_IS_DISPLAY)
				{
					stkClippingCurveP->SetDisplay(true);
				}
				stkPlaneSurfaceP = (Stk_PlaneSurface*)m_vcClippingPlans[m_vcClippingPlans.size() - 1];
				stkPlaneSurfaceP->SetClippingCurve(stkClippingCurveP);
			}
		}
		else
		{

		}
	}

	// 释放内存
	eState = i_FileP->ReadSegmentFinishData(i_ViewHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return STK_ERROR_HEADRD;
	}
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}

	/*
	 *	读取下一段的数据
	 */
	while (1)
	{
		eState = i_FileP->ReadSegmentHeadData(&head);
		if (eState!=STK_SUCCESS)
		{
			return STK_ERROR_HEADRD;
		}
		switch (head.kind)
		{
		//当下一段是NodeConnector段时
		case SEG_TYPE_CONNECTOR:
			StkNodeConnectorP = new Stk_NodeConnector(m_stkProtoP);
			eState=StkNodeConnectorP->ReadFile(i_FileP, &head);
			if (eState!=STK_SUCCESS)
			{
				delete StkNodeConnectorP;
				return eState;
			}

			AddNodeConnector(StkNodeConnectorP);
			break;
		case SEG_TYPE_INSATTRIBUTE:
			eState = ReadInsAttribute(i_FileP, &head);
			if (eState != STK_SUCCESS)
			{
				return eState;
			}
			break;
		//当下一段是View-END段时
		case SEG_TYPE_VIEW_END:
			goto rtn;
			break;
		default:
			StkControl **TmpRecordPP = NULL;
			TmpRecordPP = (StkControl **)calloc(head.recnum, sizeof(StkControl*));
			if (head.recnum !=0 && TmpRecordPP == NULL)
			{
				return STK_ERROR_MEMORY;
			}
			eState = i_FileP->ReadSegmentAllRecordData(&head, (StkControl **)TmpRecordPP);
			if (eState!=STK_SUCCESS)
			{
				return STK_ERROR_HEADRD;
			}
			eState = i_FileP->ReadSegmentFinishData(&head, (StkControl **)TmpRecordPP);// 释放内存
			if (eState!=STK_SUCCESS)
			{
				return STK_ERROR_HEADRD;
			}
			if (TmpRecordPP != NULL)
			{
				free(TmpRecordPP);
				TmpRecordPP = NULL;
			}
			break;
		}
	}

rtn:
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		写View数据
 *	@param[in]	i_stkFileP			文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其它				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_View::WriteFile(Stk_File *i_stkFileP)
{
	StkSegmentHead		viewHead = {0};
	StkSegmentHead		endHead = {0};
	StkViewRec6			viewRec6 = {0};
	StkCameraRec		camRec = {0};
	StkCameraMatrixRec	camMtxRec = {0};
	Stk_Camera *stkCameraP = NULL;
	STK_STATUS			ret = STK_SUCCESS;
	vector<Stk_SurfaceEntity*> vcStkSurEntitys;
	Stk_PlaneSurface	*stkPlnSurP = NULL;
	STK_BOOL			bMatrixFlag = FALSE;
	map<wstring, Stk_InsAttribute*>::iterator itorInsAttr = m_mapInsAttribute.begin();
#ifdef _WINDOWS_
	WCHAR			strConvertLog[MAX_PATH + 512];
	string			strViewName;
	Stk_SVLLog		stkSVLLog;
	Stk_LogFileEx*	stkSVLLogFile;
#endif
	StkEntityNameRec*	stkEntityNameRecP = NULL;
	STK_UINT32			nuEntityNameSize = 0;
	StkClippingCurveRec	clippingCurveRec = {0};

	if(i_stkFileP == NULL)
	{
		return STK_ERROR;
	}
#ifdef _WINDOWS_
	stkSVLLog = i_stkFileP->GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = i_stkFileP->GetDocumentManager()->GetSVLLogFile();
#endif

	/*
	 *	【第一步】 输出View段
	 */
#ifdef _WINDOWS_
	strViewName = Stk_Platform::WStringToString(m_wstrName);
	if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
	{
		_stprintf_s(strConvertLog,  _countof(strConvertLog), 
			_T("【View段】输出 名称：%s View ID：%d"), m_wstrName.c_str(), m_ID);
		stkSVLLogFile->Log(strConvertLog);
	}
#endif

	// 【第1.1步】输出Table HEAD Record
	viewHead.id = m_ID;
	viewHead.kind = SEG_TYPE_VIEW;
	ret = i_stkFileP->WriteSegmentHeadData(&viewHead);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：View段-头记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	// 【第1.2步】输出View Record
	viewRec6.Control.type = REC_TYPE_VIEW;
	viewRec6.Control.version = _VER_VIEW_RECORD /*6*/;
	viewRec6.Control.length = sizeof(StkViewRec6) - sizeof(StkControl);
	viewRec6.isItem = 0;
	viewRec6.hasItem = 0;
	if(m_bIsActivated){
		viewRec6.isItem |= STK_VIEW_IS_ACTIVED;
	}
	if(m_bHasClipPlan){
		viewRec6.hasItem |= STK_VIEW_HAS_CLIP;
	}

	//预留
	viewRec6.usetype = m_eUsageType;
	viewRec6.DefType = 0;
	viewRec6.Transparency = m_fTransparency;
	viewRec6.dmy1 = 0;
	viewRec6.dmy2 = 0;

	ret = i_stkFileP->WriteSegmentRecordData(&viewHead, (StkControl *)&viewRec6);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：View Base记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	// 【第1.3步】输出要素名称记录
	nuEntityNameSize = m_wstrName.size() + 1;
	stkEntityNameRecP = (StkEntityNameRec*)calloc(1, sizeof(StkEntityNameRec) - sizeof(STK_WCHAR) + nuEntityNameSize*sizeof(STK_WCHAR));
	if(stkEntityNameRecP == NULL)
	{
		return STK_ERROR;
	}
	stkEntityNameRecP->Control.type = REC_TYPE_ENTITY_NAME;
	stkEntityNameRecP->Control.length = sizeof(StkEntityNameRec) - sizeof(STK_WCHAR) + nuEntityNameSize*sizeof(STK_WCHAR) - sizeof(StkControl);
	stkEntityNameRecP->Control.version = _VER_ENTITY_NAME_RECORD;

	stkEntityNameRecP->nuNameSize = nuEntityNameSize;
#ifdef __MOBILE__
	memcpy(stkEntityNameRecP->atr,Stk_Platform::WStringToSTKWCHARStr(m_wstrName),(m_wstrName.size()+1)*sizeof(STK_WCHAR));
#else
	wcscpy_s(stkEntityNameRecP->atr, nuEntityNameSize, (WCHAR*)m_wstrName.c_str());
#endif

	ret = i_stkFileP->WriteSegmentRecordData(&viewHead, (StkControl*)stkEntityNameRecP);
	if(ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	if (stkEntityNameRecP != NULL)
	{
		free(stkEntityNameRecP);
		stkEntityNameRecP = NULL;
	}

	// 【第1.4步】输出Camera Record
	stkCameraP = GetCamera();
	if (stkCameraP != NULL)
	{
		bMatrixFlag = stkCameraP->GetMatrixFlag();
		if (bMatrixFlag != TRUE)
		{
			camRec.Control.type = REC_TYPE_CAMERA;
			camRec.Control.version = _VER_CAMERA_RECORD /* 1 */;
			camRec.Control.length = sizeof(StkCameraRec) - sizeof(StkControl);
			camRec.ID = stkCameraP->GetID();
			camRec.OriginPoint = stkCameraP->GetOriginPoint();
			camRec.TargetVector = stkCameraP->GetTargetVector();
			camRec.UpVector = stkCameraP->GetUpVector();
			camRec.Angle = stkCameraP->GetAngle();
			camRec.Focus = stkCameraP->GetFocus();
			camRec.ZoomFactor = stkCameraP->GetZoomFactor();
			camRec.ProjType = stkCameraP->GetProjectType();

			ret = i_stkFileP->WriteSegmentRecordData(&viewHead, (StkControl *)&camRec);
			if(ret < STK_SUCCESS)
			{
#ifdef _WINDOWS_
				if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
				{
					_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：Camera记录写入失败！"));
					stkSVLLogFile->Log(strConvertLog);
				}
#endif
				return STK_ERROR;
			}
		}
		else
		{
			camMtxRec.Control.type = REC_TYPE_CAMERA_MTX;
			camMtxRec.Control.version = _VER_CAMERA_RECORD;
			camMtxRec.Control.length = sizeof(StkCameraMatrixRec) - sizeof(StkControl);

			camMtxRec.ID = stkCameraP->GetID();
			camMtxRec.OriginPoint = stkCameraP->GetOriginPoint();
			camMtxRec.ZoomFactor = stkCameraP->GetZoomFactor();
			camMtxRec.ProjType = stkCameraP->GetProjectType();
			camMtxRec.Matrix = stkCameraP->GetMatrix();

			ret = i_stkFileP->WriteSegmentRecordData(&viewHead, (StkControl *)&camMtxRec);
			if(ret < STK_SUCCESS)
			{
#ifdef _WINDOWS_
				if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
				{
					_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：Camera记录写入失败！"));
					stkSVLLogFile->Log(strConvertLog);
				}
#endif
				return STK_ERROR;
			}
		}
	}
	
	// 【第1.5步】输出Clip Plane Record
	vcStkSurEntitys = GetAllClippingPlan();
	for (int ii = 0; ii < vcStkSurEntitys.size(); ii++)
	{
		if (vcStkSurEntitys[ii]->GetType() == SURFACE_TYPE_PLANE)
		{
			StkClipPlanRec2 clipRec = {0};
			clipRec.Control.type =  REC_TYPE_CLIP_PLAN;
			clipRec.Control.version = _VER_CLIP_PLAN_RECORD /* 2 */;
			clipRec.Control.length = sizeof(StkClipPlanRec2) - sizeof(StkControl);
			stkPlnSurP = (Stk_PlaneSurface*)vcStkSurEntitys[ii];
			clipRec.ID = stkPlnSurP->GetID();
			clipRec.OriginPoint = stkPlnSurP->GetOrigin();
			clipRec.Normal = stkPlnSurP->GetNormal();
			stkPlnSurP->GetUVDir( clipRec.UVector, clipRec.VVector );
			clipRec.fDistance = stkPlnSurP->GetDistance();

			ret = i_stkFileP->WriteSegmentRecordData(&viewHead, (StkControl *)&clipRec);
			if(ret < STK_SUCCESS)
			{
#ifdef _WINDOWS_
				if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
				{
					_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：Clip Plane记录写入失败！"));
					stkSVLLogFile->Log(strConvertLog);
				}
#endif
				return STK_ERROR;
			}
		}
		// 剖切线
		if (vcStkSurEntitys[ii]->GetType() == SURFACE_TYPE_PLANE)
		{
			Stk_ClippingCurve*	stkClippingCurveP = NULL;
			stkClippingCurveP = ((Stk_PlaneSurface*)vcStkSurEntitys[ii])->GetClippingCurve();
			if (stkClippingCurveP != NULL)
			{
				clippingCurveRec.Control.type = REC_TYPE_CLIPPING_CURVE;
				clippingCurveRec.Control.length = sizeof(StkClippingCurveRec) - sizeof(StkControl);
				clippingCurveRec.Control.version = _VER_CLIPPING_CURVE_RECORD;	/* 1 */
				clippingCurveRec.nuDrawStyle = stkClippingCurveP->GetDrawStyle();
				clippingCurveRec.nuDrawType = stkClippingCurveP->GetDrawType();
				clippingCurveRec.fAngle = stkClippingCurveP->GetAngle();
				clippingCurveRec.fRatio = stkClippingCurveP->GetRatio();
				clippingCurveRec.rgbaCurve = stkClippingCurveP->GetColor();
				if (stkClippingCurveP->GetDisplay() & STK_CLIPPINGCURVE_IS_DISPLAY)
				{
					clippingCurveRec.IsItem |= STK_CLIPPINGCURVE_IS_DISPLAY;
				}

				ret = i_stkFileP->WriteSegmentRecordData(&viewHead, (StkControl *)&clippingCurveRec);
				if(ret < STK_SUCCESS)
				{
#ifdef _WINDOWS_
					if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
					{
						_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：Clipping Curve记录写入失败！"));
						stkSVLLogFile->Log(strConvertLog);
					}
#endif
					return STK_ERROR;
				}
			}
		}
	}

	// 【1.4 完成View段的输出】
	ret = i_stkFileP->WriteSegmentFinishData(&viewHead);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：View段Finish时记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	/*
	 *	【第2步】 输出NodeConnector段
	 */
	vector<Stk_NodeConnector*> vcNodeConnectorP = GetNodeConnector();
	for(int ii = 0; ii < vcNodeConnectorP.size(); ii++){
		vcNodeConnectorP[ii]->WriteFile(i_stkFileP);
	}

	/*
	 *	【第3步】 输出ProtoType下的所有子实例(包括子级ProtoType的子实例)属性(颜色、显隐、材质)段
	 */
	while(itorInsAttr != m_mapInsAttribute.end())
	{
		ret = WriteInsAttribute(itorInsAttr->first, itorInsAttr->second, i_stkFileP);
		if (ret < STK_SUCCESS)
		{
			return STK_ERROR;
		}
		itorInsAttr++;
	}

	/*
	 *	【第4步】 输出View-END段
	 */
	endHead.id = m_ID;
	endHead.kind = SEG_TYPE_VIEW_END;
	ret = i_stkFileP->WriteSegmentHeadData(&endHead);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：View-END段写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	return STK_SUCCESS;
}
/************************************************************************//**
 *	@brief		设置剖面
 *	@param[in]	i_ClippingPlanP	剖面类对象指针
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_View::SetClippingPlan(Stk_SurfaceEntity* i_ClippingPlanP)
{
	if (i_ClippingPlanP != NULL)
	{
		m_vcClippingPlans.push_back(i_ClippingPlanP);
		m_bHasClipPlan = TRUE;
	}
}

/************************************************************************//**
 *	@brief		获取剖面对象数据
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		剖面对象数据指针
 *	@note		无
 *	@attention	无
 ****************************************************************************/

//Stk_SurfaceEntity* Stk_View::GetClippingPlan(void)
//{
//	if (m_vcClippingPlans.size() != 0)
//	{
//		vector<Stk_SurfaceEntity*>::iterator itor;
//		itor = m_vcClippingPlans.end()-1;
//		return *itor;
//	}
//	else
//	{
//		return NULL;
//	}
//}

/************************************************************************//**
 *	@brief		判断是否有剖面
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bHasClipPlan	剖面状态
 *	@note		无
 *	@attention	无
 ****************************************************************************/

//STK_BOOL Stk_View::HasClippingPlan()
//{
//	return m_bHasClipPlan;
//}

/************************************************************************//**
 *	@brief		添加剖面到数组中
 *	@param[in]	i_ClippingPlanP		剖面
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_View::AddClippingPlan(Stk_SurfaceEntity* i_ClippingPlanP)
{
	m_vcClippingPlans.push_back(i_ClippingPlanP); 
	m_bHasClipPlan = TRUE;
}

/************************************************************************//**
 *	@brief		获取所有的剖面
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vcClippingPlans	所有的剖面
 *	@note		无
 *	@attention	无
 ****************************************************************************/

const vector<Stk_SurfaceEntity*>& Stk_View::GetAllClippingPlan()
{
	return m_vcClippingPlans;
}

/************************************************************************//**
 *	@brief		设置相机活动状态
 *	@param[in]	i_bActivated	相机活动状态
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_View::SetActivated(STK_BOOL i_bActivated)
{
	m_bIsActivated = i_bActivated;
}

/************************************************************************//**
 *	@brief		判断相机是否活动
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bIsActivated	相机是否活动
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOOL Stk_View::IsActivated()
{
	return m_bIsActivated;
}

/************************************************************************//**
 *	@brief		添加视图与PMI之间的关联类对象数据
 *	@param[in]	inNodeConnector		视图与PMI之间的关联
 *	@param[out]	无
 *	@retval		STK_SUCCESS			添加成功
 *	@retval		STK_ERROR			添加失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_View::AddNodeConnector(Stk_NodeConnector* inNodeConnector)
{
	if (inNodeConnector == NULL)
	{
		return STK_ERROR;
	}
	m_vcNodeConnectors.push_back(inNodeConnector);
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取所有视图与PMI之间的关联类对象数据
 *	@param[in]	无		
 *	@param[out]	无
 *	@retval		m_vcNodeConnectors	所有的视图与PMI之间的关联
 *	@note		无
 *	@attention	无
 ****************************************************************************/

const vector<Stk_NodeConnector*>& Stk_View::GetNodeConnector()
{
	return m_vcNodeConnectors;
}

/************************************************************************//**
 *	@brief		删除视图与PMI之间的关联
 *	@param[in]	i_idNode			节点ID号
 *	@param[in]	i_PointedEnt		被指向要素类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS			删除成功
 *	@retval		STK_ERROR			删除失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_View::DelNodeConnector(STK_ID i_idNode, STK_UINT32& i_PointedEnt)
{
	STK_ID				idNode;
	StkEntityTypeEnum	nuPointingEnt, nuPointedEnt;
	for (int i=0; i < m_vcNodeConnectors.size(); i++)
	{
		Stk_NodeConnector *pConnector = m_vcNodeConnectors.at(i);
		pConnector->GetEntityType(nuPointingEnt,nuPointedEnt);
		idNode = pConnector->GetLinkNode();
		if (idNode == i_idNode && nuPointingEnt == 10 && nuPointedEnt == i_PointedEnt)
		{
			m_vcNodeConnectors.erase(m_vcNodeConnectors.begin() + i);
			return STK_SUCCESS;
		}
	}
	return STK_ERROR;
}

/************************************************************************//**
 *	@brief		创建视图与PMI之间的关联
 *	@param[in]	i_idNode			PMI所属Node的ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS			删除成功
 *	@note		注意是NodeID，而不是PMI ID
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_View::AddPMIConnector(STK_ID i_idNode)
{
	Stk_NodeConnector *nodeConnector = new Stk_NodeConnector(m_stkProtoP);

	nodeConnector->SetEntityType(ENTITY_TYPE_VIEW, ENTITY_TYPE_PMI);
	nodeConnector->SetLinkNode(i_idNode);
	AddNodeConnector(nodeConnector);

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		删除视图与PMI之间的关联
 *	@param[in]	i_idNode			PMI所属Node的ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS			删除成功
 *	@retval		STK_ERROR			删除失败
 *	@note		注意是NodeID，而不是PMI ID
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_View::DelPMIConnector(STK_ID i_idNode)
{
	STK_UINT32	nuPointedEntity  = ENTITY_TYPE_PMI;
	STK_STATUS stkState = DelNodeConnector(i_idNode, nuPointedEntity);
	if (stkState == STK_SUCCESS)
	{
		return stkState;
	}
	return STK_ERROR;
}

/*************************************************************************//**
 *	@brief		设置所对应的Node
 *	@param[in]	i_NodeP				所对应的Node
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_View::SetNode(Stk_Node* i_NodeP)
{
	m_NodeP = i_NodeP;
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取所对应的Node
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		所对应的Node
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
Stk_Node* Stk_View::GetNode()
{
	return m_NodeP;
}

/************************************************************************//**
 *	@brief		添加子实例的显隐状态
 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
 *	@param[in]	i_eDisplayState		显隐状态
 *	@param[out]	无
 *	@retval		STK_SUCCESS			添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_View::AddInsDisplayState(wstring i_wstrInsPlcPath, StkDisplayStateEnum i_eDisplayState)
{
	Stk_InsAttribute*	stkInsAttributeP = NULL;
	
	if (i_wstrInsPlcPath == L"")
	{
		return STK_ERROR;
	}

	map<wstring, Stk_InsAttribute*>::iterator itor = m_mapInsAttribute.find(i_wstrInsPlcPath);

	if (itor != m_mapInsAttribute.end())
	{
		stkInsAttributeP = itor->second;
		stkInsAttributeP->SetDisplayState(i_eDisplayState);
	}
	else
	{
		stkInsAttributeP = new Stk_InsAttribute();
		stkInsAttributeP->SetDisplayState(i_eDisplayState);
		m_mapInsAttribute.insert(pair<wstring, Stk_InsAttribute*>(i_wstrInsPlcPath, stkInsAttributeP));
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取指定配置路径下子实例的显隐状态
 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
 *	@param[out]	o_eDisplayState		子实例的显隐状态
 *	@retval		子实例的显隐状态
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_View::GetInsDisplayStateByPlcPath(wstring i_wstrInsPlcPath, StkDisplayStateEnum& o_eDisplayState)
{
	Stk_InsAttribute*	stkInsAttributeP = NULL;
	map<wstring, Stk_InsAttribute*>::iterator itor = m_mapInsAttribute.find(i_wstrInsPlcPath);
	if (itor != m_mapInsAttribute.end())
	{
		stkInsAttributeP = itor->second;
		o_eDisplayState = stkInsAttributeP->GetDisplayState();
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}
}

/************************************************************************//**
 *	@brief		添加子实例的颜色
 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
 *	@param[in]	i_rgbaIns			子实例的颜色
 *	@param[out]	无
 *	@retval		STK_SUCCESS			添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_View::AddInsColor(wstring i_wstrInsPlcPath, STK_RGBA32 i_rgbaIns)
{
	Stk_InsAttribute*	stkInsAttributeP = NULL;

	if (i_wstrInsPlcPath == L"")
	{
		return STK_ERROR;
	}

	map<wstring, Stk_InsAttribute*>::iterator itor = m_mapInsAttribute.find(i_wstrInsPlcPath);
	if (itor != m_mapInsAttribute.end())
	{
		stkInsAttributeP = itor->second;
		stkInsAttributeP->SetColor(i_rgbaIns);
	}
	else
	{
		stkInsAttributeP = new Stk_InsAttribute();
		stkInsAttributeP->SetColor(i_rgbaIns);
		m_mapInsAttribute.insert(pair<wstring, Stk_InsAttribute*>(i_wstrInsPlcPath, stkInsAttributeP));
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取指定配置路径下的子实例的颜色
 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
 *	@param[out]	o_rgbaIns			子实例的颜色
 *	@retval		指定的子实例的颜色
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_View::GetInsColorByPlcPath(wstring i_wstrInsPlcPath, STK_RGBA32& o_rgbaIns)
{
	Stk_InsAttribute*	stkInsAttributeP = NULL;
	map<wstring, Stk_InsAttribute*>::iterator itor = m_mapInsAttribute.find(i_wstrInsPlcPath);
	if (itor != m_mapInsAttribute.end())
	{
		stkInsAttributeP = itor->second;
		o_rgbaIns = stkInsAttributeP->GetColor();
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}
}

/************************************************************************//**
 *	@brief		添加配置路径下子实例的材质
 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
 *	@param[in]	i_matIns			子实例的材质
 *	@param[out]	无
 *	@retval		STK_SUCCESS			添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_View::AddInsMaterial(wstring i_wstrInsPlcPath, STK_MAT32 i_matIns)
{
	Stk_InsAttribute*	stkInsAttributeP = NULL;

	if (i_wstrInsPlcPath == L"")
	{
		return STK_ERROR;
	}

	map<wstring, Stk_InsAttribute*>::iterator itor = m_mapInsAttribute.find(i_wstrInsPlcPath);
	
	if (itor != m_mapInsAttribute.end())
	{
		stkInsAttributeP = itor->second;
		stkInsAttributeP->SetMaterial(i_matIns);
	}
	else
	{
		stkInsAttributeP = new Stk_InsAttribute();
		stkInsAttributeP->SetMaterial(i_matIns);
		m_mapInsAttribute.insert(pair<wstring, Stk_InsAttribute*>(i_wstrInsPlcPath, stkInsAttributeP));
	}

	return STK_SUCCESS;

	// 将STK_MAT32更改为Stk_Material后，需要将Stk_Material增加到Stk_File中。
	// GetProtoType()->GetFile()->AddMaterial(Stk_Material* i_stkMaterialP);
}

/************************************************************************//**
 *	@brief		获取指定的子实例的材质
 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
 *	@param[out]	o_matIns			子实例的材质
 *	@retval		指定的子实例的材质
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_View::GetInsMaterialByPlcPath(wstring i_wstrInsPlcPath, STK_MAT32& o_matIns)
{
	Stk_InsAttribute*	stkInsAttributeP = NULL;
	map<wstring, Stk_InsAttribute*>::iterator itor = m_mapInsAttribute.find(i_wstrInsPlcPath);
	if (itor != m_mapInsAttribute.end())
	{
		stkInsAttributeP = itor->second;
		o_matIns = stkInsAttributeP->GetMaterial();
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}
}

/************************************************************************//**
 *	@brief		添加配置路径下子实例相对于父级的配置矩阵
 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
 *	@param[in]	i_idPlacement		子实例的配置ID
 *	@param[in]	i_mtxInsPlc			子实例的配置矩阵
 *	@param[out]	无
 *	@retval		STK_SUCCESS			添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_View::AddInsPlacement(wstring i_wstrInsPlcPath, STK_ID i_idPlacement, STK_MTX32 i_mtxInsPlc)
{
	Stk_InsAttribute*	stkInsAttributeP = NULL;

	if (i_wstrInsPlcPath == L"")
	{
		return STK_ERROR;
	}

	map<wstring, Stk_InsAttribute*>::iterator itor = m_mapInsAttribute.find(i_wstrInsPlcPath);
	if (itor != m_mapInsAttribute.end())
	{
		stkInsAttributeP = itor->second;
		stkInsAttributeP->SetPlacement(i_idPlacement, i_mtxInsPlc);
	}
	else
	{
		stkInsAttributeP = new Stk_InsAttribute();
		stkInsAttributeP->SetPlacement(i_idPlacement, i_mtxInsPlc);
		m_mapInsAttribute.insert(pair<wstring, Stk_InsAttribute*>(i_wstrInsPlcPath, stkInsAttributeP));
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取指定的子实例相对于父级的配置矩阵
 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
 *	@param[out]	o_idPlacement		子实例的配置ID
 *	@param[out]	o_mtxInsPlc			子实例的配置矩阵
 *	@retval		指定的子实例的配置矩阵
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_View::GetInsPlacementByPlcPath(wstring i_wstrInsPlcPath, STK_ID& o_idPlacement, STK_MTX32& o_mtxInsPlc)
{
	Stk_InsAttribute*	stkInsAttributeP = NULL;
	map<wstring, Stk_InsAttribute*>::iterator itor = m_mapInsAttribute.find(i_wstrInsPlcPath);
	if (itor != m_mapInsAttribute.end())
	{
		stkInsAttributeP = itor->second;
		stkInsAttributeP->GetPlacement(o_idPlacement, o_mtxInsPlc);
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}
}

/************************************************************************//**
 *	@brief		写ProtoType下的所有子实例(包括子级ProtoType的子实例)属性(颜色、显隐、材质)段
 *	@param[in]	i_wstrInsPlcPath	实例配置路径
 *	@param[in]	i_stkInsAttrP		实例属性
 *	@param[in]	i_stkFileP			文件指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其他				写入失败
 *	@note		无
 *	@attention
 ****************************************************************************/
STK_STATUS Stk_View::WriteInsAttribute(wstring i_wstrInsPlcPath, Stk_InsAttribute* i_stkInsAttrP, Stk_File* i_stkFileP)
{
	StkSegmentHead		InsAttributeHead = {0};
	StkInstanceAtr*		InsPlcPathRecP = NULL;
	STK_STATUS			eState = STK_SUCCESS;

	if (i_stkFileP == NULL)
	{
		return STK_ERROR;
	}

	// 【第1步】 输出段头记录
	InsAttributeHead.id = 0;
	InsAttributeHead.kind = SEG_TYPE_INSATTRIBUTE;
	eState = i_stkFileP->WriteSegmentHeadData(&InsAttributeHead);
	if (eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// 【第2步】 输出实例配置路径记录
	STK_UINT32 nuCharSize = i_wstrInsPlcPath.size() + 1;
	InsPlcPathRecP = (StkInstanceAtr*)calloc(1, sizeof(StkControl) + sizeof(STK_UINT32)+ nuCharSize * sizeof(STK_WCHAR));
	InsPlcPathRecP->Control.type = REC_TYPE_INS_ATR;
	InsPlcPathRecP->Control.version = _VER_INS_ATR_RECORD;
	InsPlcPathRecP->charnum = nuCharSize;
	InsPlcPathRecP->Control.length = sizeof(STK_UINT32) + nuCharSize * sizeof(STK_WCHAR);
#ifdef __MOBILE__
	memcpy(InsPlcPathRecP->atr,Stk_Platform::WStringToSTKWCHARStr(i_wstrInsPlcPath),(i_wstrInsPlcPath.size()+1)*sizeof(STK_WCHAR));
#else
	wcscpy_s(InsPlcPathRecP->atr, nuCharSize, (WCHAR*)i_wstrInsPlcPath.c_str());
#endif

	eState = i_stkFileP->WriteSegmentRecordData(&InsAttributeHead, (StkControl *)InsPlcPathRecP);
	if( eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// 【第3步】 输出实例属性记录
	eState = i_stkInsAttrP->WriteFile(&InsAttributeHead, i_stkFileP);
	if (eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// 【第4步】 完成实例属性段的输出
	eState = i_stkFileP->WriteSegmentFinishData(&InsAttributeHead);
	if (eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		读ProtoType下的所有子实例(包括子级ProtoType的子实例)属性(颜色、显隐、材质)段
 *	@param[in]	i_stkFileP			文件指针
 *	@param[in]	i_InsAttrHeadP		实例属性段头
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其他				写入失败
 *	@note		无
 *	@attention
 ****************************************************************************/
STK_STATUS Stk_View::ReadInsAttribute(Stk_File* i_stkFileP, StkSegmentHead *i_InsAttrHeadP)
{
	STK_STATUS			eState = STK_SUCCESS;
	StkControl**		RecordPP = NULL;
	StkInstanceAtr*		InsPlcPathRecP = NULL;
	StkInstanceColor*	InsColorRecP = NULL;
	StkInstanceDisplay*	InsDisplayRecP = NULL;
	StkInstanceMater*	InsMaterialRecP = NULL;
	StkInsPlacement*	InsPlacementRecP = NULL;

	wstring				wstrPlcPath = L"";
	STK_RGBA32			rgbaIns;
	STK_MAT32			matIns;
	STK_ID				idPlacement = 0;
	STK_MTX32			mtxInsPlacement;
	StkDisplayStateEnum	eDisplay = STK_DISPLAY;
	
	/*
	 *	读取实例属性段的数据
	 */
	RecordPP = (StkControl**)calloc(1, sizeof(StkControl *) * i_InsAttrHeadP->recnum);
	if (i_InsAttrHeadP->recnum != 0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}

	eState = i_stkFileP->ReadSegmentAllRecordData(i_InsAttrHeadP, (StkControl **)RecordPP);
	if (eState != STK_SUCCESS)
	{
		return eState;
	}
	
	/*
	 *	读取属性段各记录
	 */
	for (int ii = 0; ii < i_InsAttrHeadP->recnum; ii++)
	{
		switch(RecordPP[ii]->type)
		{
		case REC_TYPE_INS_ATR:
			if (RecordPP[ii]->version == 1)
			{
				InsPlcPathRecP = (StkInstanceAtr*)RecordPP[ii];
#ifdef __MOBILE__
				wstrPlcPath = Stk_Platform::STKWCHARStrToWString(InsPlcPathRecP->atr);
#else
				wstrPlcPath = InsPlcPathRecP->atr;
#endif
			}
			break;
		case REC_TYPE_INS_COL:
			if (RecordPP[ii]->version == 1)
			{
				InsColorRecP = (StkInstanceColor*)RecordPP[ii];
				memcpy(&rgbaIns, InsColorRecP->color, sizeof(STK_FLOAT32)*4);
				eState = AddInsColor(wstrPlcPath, rgbaIns);
				if (eState != STK_SUCCESS)
				{
					return eState;
				}
			}
			break;
		case REC_TYPE_INS_DSP:
			if (RecordPP[ii]->version == 1)
			{
				InsDisplayRecP = (StkInstanceDisplay*)RecordPP[ii];
				eDisplay = (StkDisplayStateEnum)InsDisplayRecP->disp;
				eState = AddInsDisplayState(wstrPlcPath, eDisplay);
				if (eState != STK_SUCCESS)
				{
					return eState;
				}
			}
			break;
		case REC_TYPE_INS_MAT:
			if (RecordPP[ii]->version == 1)
			{
				InsMaterialRecP = (StkInstanceMater*)RecordPP[ii];
				memcpy(&matIns, InsMaterialRecP->mat, sizeof(STK_FLOAT32)*3);
				eState = AddInsMaterial(wstrPlcPath, matIns);
				if (eState != STK_SUCCESS)
				{
					return eState;
				}
			}
			break;
		case REC_TYPE_INS_PLACEMENT:
			if (RecordPP[ii]->version == 1)
			{
				InsPlacementRecP = (StkInsPlacement*)RecordPP[ii];
				idPlacement = InsPlacementRecP->plcId;
				memcpy(mtxInsPlacement.PlcMatrix, InsPlacementRecP->Matrix, sizeof(STK_FLOAT32)*4*4);
				eState = AddInsPlacement(wstrPlcPath, idPlacement, mtxInsPlacement);
				if (eState != STK_SUCCESS)
				{
					return eState;
				}
			}
			break;
		default:
			break;
		}
	}

	// 释放内存
	eState = i_stkFileP->ReadSegmentFinishData(i_InsAttrHeadP, (StkControl **)RecordPP);
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}

	return eState;
}

/************************************************************************//**
 *	@brief		获取包含实例属性的所有配置路径
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		所有配置路径
 *	@note		无
 *	@attention	无
 ****************************************************************************/
vector<wstring> Stk_View::GetAllInsAttrPlcPath()
{
	vector<wstring>	vcInsPlcPaths;
	map<wstring, Stk_InsAttribute*>::iterator itor = m_mapInsAttribute.begin();
	while(itor != m_mapInsAttribute.end())
	{
		vcInsPlcPaths.push_back(itor->first);
		itor++;
	}
	return vcInsPlcPaths;
}

/************************************************************************//**
 *	@brief		删除指定配置路径的实例属性
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS	删除成功
 *	@retval		STK_ERROR	删除失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_View::DeleteInsAttrByPlcPath(wstring i_wstrInsPlcPath)
{
	map<wstring, Stk_InsAttribute*>::iterator itor = m_mapInsAttribute.find(i_wstrInsPlcPath);
	if (itor != m_mapInsAttribute.end())
	{
		delete itor->second;
		m_mapInsAttribute.erase(itor);
		return STK_SUCCESS;
	}
	return STK_ERROR;
}

/****************************************************************************
 *	@brief		删除当前视图中所有的实例属性
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS	删除成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_View::DeleteAllInsAttribute()
{
	map<wstring, Stk_InsAttribute*>::iterator itor = m_mapInsAttribute.begin();
	
	while(itor != m_mapInsAttribute.end())
	{
		if (itor->second != NULL)
		{
			delete itor->second;
			m_mapInsAttribute.erase(itor);
		}
		itor++;
	}

	return STK_SUCCESS;
}