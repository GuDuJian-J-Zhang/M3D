// All Rights Reserved. Copyright (C) 2017 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file	Stk_PMI.cpp
*
*	@brief	PMI¿‡
*
*	@par	¿˙ ∑:
*		2017/06/10	¥¥Ω®°£qinyp@HOTEAMSOFT
****************************************************************************/

#include "Stk_PMI.h"
#include "Stk_ObjectImpl.h"
#include "Stk_DocumentImpl.h"
#include "Stk_Declare_Ptr.h"
#include "Stk_PMIDocument.h"
#include "ChineseCode.h"
#include "StringTools.h"

#define  PMI_NODE_NAME_NAME "name"
#define  PMI_NODE_NAME_TYPE "type"
#define  PMI_NODE_NAME_BASEDATA "baseData"
#define  PMI_NODE_NAME_DEFPLANE "defPlane"
#define  PMI_NODE_NAME_COLOR "color"
#define  PMI_NODE_NAME_STYLE "style"
#define  PMI_NODE_NAME_USETYPE "usetype"
#define  PMI_NODE_NAME_SOURCE "source"
#define  PMI_NODE_NAME_DIRX "dirX"
#define  PMI_NODE_NAME_DIRY "dirY"
#define  PMI_NODE_NAME_SYMBASEPNT "symBasePnt"
#define  PMI_NODE_NAME_VISIBLE "visible"
#define  PMI_NODE_NAME_LEADERS "leaders"
#define  PMI_NODE_NAME_EXLINES "exLines"
#define  PMI_NODE_NAME_COMTEXTS "comTexts"
#define  PMI_NODE_NAME_OUTFRAMES "outFrames"
#define  PMI_NODE_NAME_SPECIALLINES "specialLines"
#define  PMI_NODE_NAME_IMAGES   "images"
#define  PMI_NODE_NAME_USERDATAS  "userdatas"
#define  PMI_NODE_NAME_ID  "id"

namespace HoteamSoft {
	namespace SVLLib {
		class Stk_PMIDocument;
		class Stk_PMIImpl : public Stk_ObjectImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_PMI)
		public:
			Stk_PMIImpl(Stk_PMIDocument* i_PMIDocP, int i_iPartID);
			virtual ~Stk_PMIImpl(void);
		public:
			Stk_PMIDocumentPtr m_pPMIDoc;     
			STK_UINT32		   m_iModelID;	  //À˘ Ù¡„º˛
			//STK_UINT32 m_uiID;                //ID
			wstring    m_wstrName;			  //√˚≥∆
			STK_MTX32  m_mtxDefPlane;         //∂®“Â∆Ω√Ê
			STK_RGBA32 m_rgbaPMI;			  //—’…´
			StkPMITypeEnum	m_eType;          //¿‡–Õ
			STK_UINT32 m_uiStyle;             //—˘ Ω
			STK_UINT32 m_uiUseType;          // π”√∑Ω Ω
			StkPMISourceEnum m_ePMISource;    //PMI¿¥‘¥
			STK_DIR32  m_dirX;                //PMIµƒx∑ΩœÚœÚ¡ø
			STK_DIR32  m_dirY;                //PMIµƒy∑ΩœÚœÚ¡ø
			STK_PNT32  m_pntSymBase;          //∑˚∫≈ª˘◊ºµ„
			STK_BOOL   m_bIsDisplay;          //œ‘“˛◊¥Ã¨
			vector<Stk_LeaderPtr>   m_vcLeaders;                  //“˝≥ˆœﬂ
			vector<Stk_ExtensionlinePtr>	m_vcExLines;            //—”≥§œﬂ
			vector<Stk_ComTextPtr>		m_vcComTexts;           //∏¥∫œŒƒ±æ
			vector<Stk_OutFramePtr>		m_vcOutFrames;	        //Õ‚øÚ
			vector<Stk_SpecialLinePtr>	m_vcSpecialLines;		//Ãÿ ‚œﬂ¡–
			map<wstring, map<STK_ID, Stk_UserDataPtr>*>	m_mapUserDatas;	//!< ”√ªß ˝æ›
			vector<Stk_ImagePtr>		m_vcImages;			//Õº∆¨(Õº’¬)
			STK_SIZE_T					m_szUserDataCnt;			//”√ªß ˝æ› ˝¡ø
		public:
			/****************************************************************************
			*	@brief		∂¡»°Œƒº˛÷–µƒBody ˝æ›
			*	@param[in]	i_pJsonNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		STK_ERROR			∂¡»° ß∞‹
			*	@retval		STK_ERROR_MEMORY	ƒ⁄¥Ê…Í«Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					FromJson(Value* i_pJsonNode, Document* i_JsonDocP);

			/*****************************************************************************
			*	@brief		–¥View ˝æ›
			*	@param[in]	Value* i_pJsonParentNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			–¥»Î≥…π¶
			*	@retval		∆‰À¸				–¥»Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					ToJson(Value* i_pJsonNode, Document* i_JsonDocP);

			/****************************************************************************
			*	@brief		≤Â»Î”√ªß ˝æ›
			*	@param[in]	i_stkUserDataP		”√ªß ˝æ›÷∏’Î
			*	@retval		STK_SUCCESS			≥…π¶
			*	@retval		∆‰À¸				 ß∞‹
			*	@note
			*				Õ®π˝°∞”√ªß ˝æ›√˚≥∆°±+°∞”√ªß ˝æ›ID°±µƒ∑Ω Ω¿¥◊˜Œ™Œ®“ª±Í ∂
			*				  ¿˝»Á°∞∂Øª≠°±+°∞1°± ±Ì æµ⁄“ª∏ˆ∂Øª≠ °∞∂Øª≠°±+°∞2°± ±Ì æµ⁄∂˛∏ˆ∂Øª≠
			*				  µ±¡Ω¥Œ…Ë÷√µƒŒ®“ª±Í ∂œ‡Õ¨ ±£¨ª·◊‘∂Ø∏≤∏«µÙ«∞¥Œµƒ ˝æ›
			*	@attention	Œﬁ
			****************************************************************************/
			STK_STATUS					AddUserData(Stk_UserDataPtr i_stkUserDataP);
		};

		STK_STATUS	Stk_PMIImpl::AddUserData(Stk_UserDataPtr i_stkUserDataP)
		{
			map<wstring, map<STK_ID, Stk_UserDataPtr>*>::iterator  itor;
			map<STK_ID, Stk_UserDataPtr>::iterator UDitor;
			STK_STATUS stat = STK_SUCCESS;

			if (i_stkUserDataP == NULL)
			{
				return STK_ERROR;
			}

			wstring		wstrDataName = L"";
			wstring		wstrFileName = L"";
			STK_ID		UserDataID = 0;
			STK_CHAR	*dataP = NULL;
			STK_SIZE_T	dataSize = 0;

			stat = i_stkUserDataP->GetUserData(wstrDataName, UserDataID, dataP, dataSize, wstrFileName);
			if (stat != STK_SUCCESS)
			{
				return STK_ERROR;
			}

			// ≤È—ØŒ®“ª±Í ∂
			itor = m_mapUserDatas.find(wstrDataName);
			if (itor != m_mapUserDatas.end())
			{
				UDitor = (itor->second)->find(UserDataID);
				if (UDitor != (itor->second)->end())
				{
					// ≤Â»ÎUserData
					(itor->second)->erase(UDitor);
					(itor->second)->insert(pair<STK_ID, Stk_UserDataPtr>(UserDataID, i_stkUserDataP));
				}
				else
				{
					// ≤Â»ÎUserData
					(itor->second)->insert(pair<STK_ID, Stk_UserDataPtr>(UserDataID, i_stkUserDataP));
					m_szUserDataCnt++;
				}
			}
			else {
				// ≤Â»ÎUserData
				map<STK_ID, Stk_UserDataPtr> *UDmap = new map<STK_ID, Stk_UserDataPtr>();
				UDmap->insert(pair<STK_ID, Stk_UserDataPtr>(UserDataID, i_stkUserDataP));
				m_mapUserDatas.insert(pair<wstring, map<STK_ID, Stk_UserDataPtr>*>(wstrDataName, UDmap));
				m_szUserDataCnt++;
			}

			return STK_SUCCESS;
		}

		STK_STATUS Stk_PMIImpl::FromJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode || !i_JsonDocP)
				return STK_ERROR;
			Value::MemberIterator idIter = (*i_pJsonNode).FindMember(PMI_NODE_NAME_ID);
			if (idIter != (*i_pJsonNode).MemberEnd())
			{
				m_ID = idIter->value.GetInt();
			}
			m_wstrName = ChineseCode::UTF_8ToUnicode((*i_pJsonNode)[PMI_NODE_NAME_NAME].GetString());
			m_eType = (StkPMITypeEnum)(*i_pJsonNode)[PMI_NODE_NAME_TYPE].GetInt();
			int iVisible = (*i_pJsonNode)[PMI_NODE_NAME_VISIBLE].GetBool();
			
			wstring wstrValue;
			string strValue;
			//æÿ’Û ◊™ªØ
			strValue = (*i_pJsonNode)[PMI_NODE_NAME_DEFPLANE].GetString();
			std::vector<std::string> vecDefPlaneValue;
			CStringTools::StringSplit(strValue, " ", vecDefPlaneValue);
			for(int i = 0; i < 16; i++)
			{
				m_mtxDefPlane.PlcMatrix[i/4][i%4] = atof(vecDefPlaneValue[i].c_str());
			}
			vecDefPlaneValue.clear();

			Value::MemberIterator styleNodeIter = (*i_pJsonNode).FindMember(PMI_NODE_NAME_STYLE);
			if (styleNodeIter != (*i_pJsonNode).MemberEnd())
				m_uiStyle = styleNodeIter->value.GetInt();
			Value::MemberIterator useTypeNodeIter = (*i_pJsonNode).FindMember(PMI_NODE_NAME_USETYPE);
			if (useTypeNodeIter != (*i_pJsonNode).MemberEnd())
				m_uiUseType = useTypeNodeIter->value.GetUint();
			//m_uiUseType = (*i_pJsonNode)[PMI_NODE_NAME_USETYPE].GetInt();
			//—’…´ ◊™ªØ
			strValue = (*i_pJsonNode)[PMI_NODE_NAME_COLOR].GetString();
			std::vector<std::string> vecColorValue;
			CStringTools::StringSplit(strValue, " ", vecColorValue);
			m_rgbaPMI.Red = atof(vecColorValue[0].c_str());
			m_rgbaPMI.Green = atof(vecColorValue[1].c_str());
			m_rgbaPMI.Blue = atof(vecColorValue[2].c_str());
			m_rgbaPMI.Alpha = atof(vecColorValue[3].c_str());
			vecColorValue.clear();

			m_ePMISource = (StkPMISourceEnum)(*i_pJsonNode)[PMI_NODE_NAME_SOURCE].GetInt();
			//PMIµƒx∑ΩœÚœÚ¡ø
			strValue = (*i_pJsonNode)[PMI_NODE_NAME_DIRX].GetString();
			std::vector<std::string> vecDirXValue;
			CStringTools::StringSplit(strValue, " ", vecDirXValue);
			m_dirX.X = atof(vecDirXValue[0].c_str());
			m_dirX.Y = atof(vecDirXValue[1].c_str());
			m_dirX.Z = atof(vecDirXValue[2].c_str());
			vecDirXValue.clear();

			//PMIµƒy∑ΩœÚœÚ¡ø
			strValue = (*i_pJsonNode)[PMI_NODE_NAME_DIRY].GetString();
			std::vector<std::string> vecDirYValue;
			CStringTools::StringSplit(strValue, " ", vecDirYValue);
			m_dirY.X = atof(vecDirYValue[0].c_str());
			m_dirY.Y = atof(vecDirYValue[1].c_str());
			m_dirY.Z = atof(vecDirYValue[2].c_str());
			vecDirYValue.clear();

			//PMIµƒ∑˚∫≈ª˘◊ºµ„
			strValue = (*i_pJsonNode)[PMI_NODE_NAME_SYMBASEPNT].GetString();
			std::vector<std::string> vecSysBasePntValue;
			CStringTools::StringSplit(strValue, " ", vecSysBasePntValue);
			m_pntSymBase.X = atof(vecSysBasePntValue[0].c_str());
			m_pntSymBase.Y = atof(vecSysBasePntValue[1].c_str());
			m_pntSymBase.Z = atof(vecSysBasePntValue[2].c_str());
			vecSysBasePntValue.clear();
		
			//leaders
			Value::MemberIterator leadersNodeIter = i_pJsonNode->FindMember(PMI_NODE_NAME_LEADERS);
			if (leadersNodeIter != i_pJsonNode->MemberEnd())
			{
				Value leaders =  leadersNodeIter->value.GetArray();
				Value leader;
				for (int i = 0; i < leaders.Size(); i++)
				{
					leader = leaders[i];
					Stk_LeaderPtr leaderP = Stk_LeaderPtr::CreateObject();
					leaderP->FromJson(&leader, i_JsonDocP);
					m_vcLeaders.push_back(leaderP);
				}
			}

			//exLines
			Value::MemberIterator exlinesNodeIter = i_pJsonNode->FindMember(PMI_NODE_NAME_EXLINES);
			if (exlinesNodeIter != i_pJsonNode->MemberEnd())
			{
				Value exLines = exlinesNodeIter->value.GetArray();
				Value exLine;
				for (int i = 0; i < exLines.Size(); i++)
				{
					exLine = exLines[i];
					Stk_ExtensionlinePtr exlineP = Stk_ExtensionlinePtr::CreateObject();
					exlineP->FromJson(&exLine, i_JsonDocP);
					m_vcExLines.push_back(exlineP);
				}
			}

			//comTexts
			Value::MemberIterator comTextsNodeIter = i_pJsonNode->FindMember(PMI_NODE_NAME_COMTEXTS);
			if (comTextsNodeIter != i_pJsonNode->MemberEnd())
			{
				Value comTexts = comTextsNodeIter->value.GetArray();
				Value comText;
				for (int i = 0; i < comTexts.Size(); i++)
				{
					comText = comTexts[i];
					Stk_ComTextPtr comTextP = Stk_ComTextPtr::CreateObject();
					comTextP->FromJson(&comText, i_JsonDocP);
					m_vcComTexts.push_back(comTextP);
				}
			}
			//outFrameÕ‚øÚ
			Value::MemberIterator outFrameNodeIter = i_pJsonNode->FindMember(PMI_NODE_NAME_OUTFRAMES);
			if (outFrameNodeIter != i_pJsonNode->MemberEnd())
			{
				Value outFrames = outFrameNodeIter->value.GetArray();
				Value outFrame;
				for(int i = 0;i < outFrames.Size(); i++)
				{
					outFrame = outFrames[i];
					Stk_OutFramePtr outFrameP = Stk_OutFramePtr::CreateObject();
					outFrameP->FromJson(&outFrame, i_JsonDocP);
					m_vcOutFrames.push_back(outFrameP);
				}
			}
			//specialLinesÃÿ ‚œﬂ
			Value::MemberIterator specialLinesNodeIter = i_pJsonNode->FindMember(PMI_NODE_NAME_SPECIALLINES);
			if (specialLinesNodeIter != i_pJsonNode->MemberEnd())
			{
				Value specialLines = specialLinesNodeIter->value.GetArray();
				Value specialLine;
				for (int i = 0; i < specialLines.Size(); i++)
				{
					specialLine = specialLines[i];
					Stk_SpecialLinePtr specialLineP = Stk_SpecialLinePtr::CreateObject();
					specialLineP->FromJson(&specialLine, i_JsonDocP);
					m_vcSpecialLines.push_back(specialLineP);
				}
			}

			//userdata ”√ªß ˝æ›
			Value::MemberIterator userdatasNodeIter = i_pJsonNode->FindMember(PMI_NODE_NAME_USERDATAS);
			if (userdatasNodeIter != i_pJsonNode->MemberEnd())
			{
				Value userdatas = userdatasNodeIter->value.GetArray();
				Value userdata;
				for (int i = 0; i < userdatas.Size(); i++)
				{
					userdata = userdatas[i];
					Stk_UserDataPtr userdataP = Stk_UserDataPtr::CreateObject();
					userdataP->FromJson(&userdata);
					AddUserData(userdataP);
				}
			}

			//picture Õº∆¨
			Value::MemberIterator imagesNodeIter = i_pJsonNode->FindMember(PMI_NODE_NAME_IMAGES);
			if (imagesNodeIter != i_pJsonNode->MemberEnd())
			{
				Value images = imagesNodeIter->value.GetArray();
				Value image;
				for (int i = 0; i < images.Size(); i++)
				{
					image = images[i];
					Stk_ImagePtr imageP = Stk_ImagePtr::CreateObject();
					imageP->FromJson(&image);
					m_vcImages.push_back(imageP);
				}
			}

			return STK_SUCCESS;
		}

		STK_STATUS	Stk_PMIImpl::ToJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_ERROR;
			if (!i_pJsonNode || !i_JsonDocP)
				return ret;
			i_pJsonNode->SetObject();

			i_pJsonNode->AddMember(PMI_NODE_NAME_ID, m_ID, i_JsonDocP->GetAllocator());
			string strName;
			ChineseCode::UnicodeToUTF_8(m_wstrName.c_str(), strName);
			i_pJsonNode->AddMember(PMI_NODE_NAME_NAME, strName, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(PMI_NODE_NAME_TYPE, m_eType, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(PMI_NODE_NAME_VISIBLE, m_bIsDisplay, i_JsonDocP->GetAllocator());
			
			//∂®“Â∆Ω√Ê
			string strValue = "";
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					string strDefPlane;
					char szDefPlane[256] = { 0 };
					sprintf(szDefPlane, "%.8f", m_mtxDefPlane.PlcMatrix[i][j]);
					strDefPlane = szDefPlane;
					if (!(i == 0 && j == 0))
						strValue += " ";
					strValue += strDefPlane;
				}
			}
			i_pJsonNode->AddMember(PMI_NODE_NAME_DEFPLANE, strValue, i_JsonDocP->GetAllocator());
			
			strValue = "";
			//—’…´
			char szColor[256] = { 0 };
			sprintf(szColor, "%.8f", m_rgbaPMI.Red);
			strValue += szColor;
			strValue += " ";
			sprintf(szColor, "%.8f", m_rgbaPMI.Green);
			strValue += szColor;
			strValue += " ";
			sprintf(szColor, "%.8f", m_rgbaPMI.Blue);
			strValue += szColor;
			strValue += " ";
			sprintf(szColor, "%.8f", m_rgbaPMI.Alpha);
			strValue += szColor;
			i_pJsonNode->AddMember(PMI_NODE_NAME_COLOR, strValue, i_JsonDocP->GetAllocator());

			i_pJsonNode->AddMember(PMI_NODE_NAME_STYLE, m_uiStyle, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(PMI_NODE_NAME_USETYPE, m_uiUseType, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(PMI_NODE_NAME_SOURCE, m_ePMISource, i_JsonDocP->GetAllocator());
			
			//dirX
			strValue = "";
			char szDirX[256] = { 0 };
			sprintf(szDirX, "%.8f", m_dirX.X);
			strValue += szDirX;
			strValue += " ";
			sprintf(szDirX, "%.8f", m_dirX.Y);
			strValue += szDirX;
			strValue += " ";
			sprintf(szDirX, "%.8f", m_dirX.Z);
			strValue += szDirX;
			i_pJsonNode->AddMember(PMI_NODE_NAME_DIRX, strValue, i_JsonDocP->GetAllocator());

			//DIRY
			strValue = "";
			char szDirY[256] = { 0 };
			sprintf(szDirY, "%.8f", m_dirY.X);
			strValue += szDirY;
			strValue += " ";
			sprintf(szDirY, "%.8f", m_dirY.Y);
			strValue += szDirY;
			strValue += " ";
			sprintf(szDirY, "%.8f", m_dirY.Z);
			strValue += szDirY;
			i_pJsonNode->AddMember(PMI_NODE_NAME_DIRY, strValue, i_JsonDocP->GetAllocator());

			//sysbasepnt
			strValue = "";
			char szSysBasePnt[256] = { 0 };
			sprintf(szSysBasePnt, "%.8f", m_pntSymBase.X);
			strValue += szSysBasePnt;
			strValue += " ";
			sprintf(szSysBasePnt, "%.8f", m_pntSymBase.Y);
			strValue += szSysBasePnt;
			strValue += " ";
			sprintf(szSysBasePnt, "%.8f", m_pntSymBase.Z);
			strValue += szSysBasePnt;
			i_pJsonNode->AddMember(PMI_NODE_NAME_SYMBASEPNT, strValue, i_JsonDocP->GetAllocator());

			//i_pJsonNode->AddMember(PMI_NODE_NAME_BASEDATA, valBaseData, i_JsonDocP->GetAllocator());
			
			//leaders
			Value valLeaders;
			valLeaders.SetArray();
			vector<Stk_LeaderPtr>::iterator leaderItor = m_vcLeaders.begin();
			while (leaderItor != m_vcLeaders.end())
			{
				if (*leaderItor == NULL)
				{
					leaderItor++;
					continue;
				}
				Value valLeader;
				(*leaderItor)->ToJson(&valLeader, i_JsonDocP);
				valLeaders.PushBack(valLeader, i_JsonDocP->GetAllocator());
				leaderItor++;
			}
			if(m_vcLeaders.size() != 0)
				i_pJsonNode->AddMember(PMI_NODE_NAME_LEADERS, valLeaders, i_JsonDocP->GetAllocator());

			//exLines
			Value valExlines;
			valExlines.SetArray();
			vector<Stk_ExtensionlinePtr>::iterator exlinesItor = m_vcExLines.begin();
			while (exlinesItor != m_vcExLines.end())
			{
				if (*exlinesItor == NULL)
				{
					exlinesItor++;
					continue;
				}
				Value valExline;
				(*exlinesItor)->ToJson(&valExline, i_JsonDocP);
				valExlines.PushBack(valExline, i_JsonDocP->GetAllocator());
				exlinesItor++;
			}
			if(m_vcExLines.size() != 0)
				i_pJsonNode->AddMember(PMI_NODE_NAME_EXLINES, valExlines, i_JsonDocP->GetAllocator());

			//comTexts
			Value valComTexts;
			valComTexts.SetArray();
			vector<Stk_ComTextPtr>::iterator comTextsItor = m_vcComTexts.begin();
			while (comTextsItor != m_vcComTexts.end())
			{
				if (*comTextsItor == NULL)
				{
					comTextsItor++;
					continue;
				}
				Value valComText;
				(*comTextsItor)->ToJson(&valComText, i_JsonDocP);
				valComTexts.PushBack(valComText, i_JsonDocP->GetAllocator());
				comTextsItor++;
			}
			if(m_vcComTexts.size() != 0)
				i_pJsonNode->AddMember(PMI_NODE_NAME_COMTEXTS, valComTexts, i_JsonDocP->GetAllocator());

			//outFrames
			Value valOutFrames;
			valOutFrames.SetArray();
			vector<Stk_OutFramePtr>::iterator outFramesItor = m_vcOutFrames.begin();
			while (outFramesItor != m_vcOutFrames.end())
			{
				if (*outFramesItor == NULL)
				{
					outFramesItor++;
					continue;
				}
				Value valOutFrame;
				(*outFramesItor)->ToJson(&valOutFrame, i_JsonDocP);
				valOutFrames.PushBack(valOutFrame, i_JsonDocP->GetAllocator());
				outFramesItor++;
			}
			if(m_vcOutFrames.size() != 0)
				i_pJsonNode->AddMember(PMI_NODE_NAME_OUTFRAMES, valOutFrames, i_JsonDocP->GetAllocator());

			//specicalLine
			Value valSpeciallines;
			valSpeciallines.SetArray();
			vector<Stk_SpecialLinePtr>::iterator speciallinesItor = m_vcSpecialLines.begin();
			while (speciallinesItor != m_vcSpecialLines.end())
			{
				if (*speciallinesItor == NULL)
				{
					speciallinesItor++;
					continue;
				}
				Value valSpecialline;
				(*speciallinesItor)->ToJson(&valSpecialline, i_JsonDocP);
				valSpeciallines.PushBack(valSpecialline, i_JsonDocP->GetAllocator());
				speciallinesItor++;
			}
			if(m_vcSpecialLines.size() != 0)
				i_pJsonNode->AddMember(PMI_NODE_NAME_SPECIALLINES, valSpeciallines, i_JsonDocP->GetAllocator());

			//images
			Value valImages;
			valImages.SetArray();
			vector<Stk_ImagePtr>::iterator imagesIter = m_vcImages.begin();
			while (imagesIter != m_vcImages.end())
			{
				if (*imagesIter == nullptr)
				{
					imagesIter++;
					continue;
				}
				Value valImage;
				(*imagesIter)->ToJson(&valImage, i_JsonDocP);
				valImages.PushBack(valImage, i_JsonDocP->GetAllocator());
				imagesIter++;
			}
			if(m_vcImages.size() != 0)
				i_pJsonNode->AddMember(PMI_NODE_NAME_IMAGES, valImages, i_JsonDocP->GetAllocator());

			//useDatas
			Value valUserDatas;
			valUserDatas.SetArray();
			map<wstring, map<STK_ID, Stk_UserDataPtr>*>::iterator itorMap = m_mapUserDatas.begin();
			while (itorMap != m_mapUserDatas.end())
			{
				map<STK_ID, Stk_UserDataPtr>* mapIDUserData = itorMap->second;
				map<STK_ID, Stk_UserDataPtr>::iterator itorIDUserData = mapIDUserData->begin();
				while (itorIDUserData != mapIDUserData->end())
				{
					Value valUserData;
					(itorIDUserData->second)->ToJson(&valUserData, i_JsonDocP);
					valUserDatas.PushBack(valUserData, i_JsonDocP->GetAllocator());
					itorIDUserData++;
				}
				itorMap++;
			}
			if(m_mapUserDatas.size() != 0)
				i_pJsonNode->AddMember(PMI_NODE_NAME_USERDATAS, valUserDatas, i_JsonDocP->GetAllocator());

			return STK_SUCCESS;
		}

		STK_CLASS_IMPL_IMPLEMENT(Stk_PMI)

		STK_IMPLEMENT_CLASS(Stk_PMI, Stk_Object)

		Stk_PMIImpl::Stk_PMIImpl(Stk_PMIDocument* i_PMIDocP, int i_iModelID)
		{
			m_pPMIDoc = i_PMIDocP;
			m_iModelID = i_iModelID;
			//m_uiID = -1;
			m_wstrName = L"";
			for (int ii = 0; ii < 4; ii++)
			{
				for (int jj = 0; jj < 4; jj++)
				{
					if (ii == jj)
					{
						m_mtxDefPlane.PlcMatrix[ii][jj] = 1.0;
					}
					else
					{
						m_mtxDefPlane.PlcMatrix[ii][jj] = 0.0;
					}
				}
			}
			m_rgbaPMI.Red = -1.0f;
			m_rgbaPMI.Green = -1.0f;
			m_rgbaPMI.Blue = -1.0f;
			m_rgbaPMI.Alpha = -1.0f;
			m_eType = PMI_TYPE_UNKNOWN;
			m_uiStyle = -1;
			m_uiUseType = -1;
			m_ePMISource = PMI_SOURCE_CONVERT;
			m_bIsDisplay = true;
			m_szUserDataCnt = 0;
		}

		Stk_PMIImpl::~Stk_PMIImpl(void)
		{
			m_szUserDataCnt = 0;
		}

		Stk_PMI::Stk_PMI() :Stk_Object(new Stk_PMIImpl(NULL, 0))
		{
		}

		Stk_PMI::~Stk_PMI()
		{
			DELETE_STK_CLASS_IMPL
		}

		/*void Stk_PMI::SetID(STK_ID i_ID)
		{
			((Stk_PMIImpl*)m_pImpl)->m_uiID = i_ID;
		}*/

		/*STK_ID	Stk_PMI::GetID()
		{
			return ((Stk_PMIImpl*)m_pImpl)->m_uiID;
		}*/

		void Stk_PMI::Init(Stk_PMIDocument* i_PMIDocP, int i_iModelID)
		{
			((Stk_PMIImpl*)m_pImpl)->m_pPMIDoc = i_PMIDocP;
			((Stk_PMIImpl*)m_pImpl)->m_iModelID = i_iModelID;
		}

		void Stk_PMI::SetName(const wstring &i_strName)
		{
			((Stk_PMIImpl*)m_pImpl)->m_wstrName = i_strName;
		}

		wstring	Stk_PMI::GetName()
		{
			return ((Stk_PMIImpl*)m_pImpl)->m_wstrName;
		}

		const STK_MTX32& Stk_PMI::GetDefPlane()
		{
			return ((Stk_PMIImpl*)m_pImpl)->m_mtxDefPlane;
		}

		STK_STATUS	Stk_PMI::SetDefPlane(const STK_MTX32& i_mtxDefPlane)
		{
			((Stk_PMIImpl*)m_pImpl)->m_mtxDefPlane = i_mtxDefPlane;
			return STK_SUCCESS;
		}

		const STK_RGBA32& Stk_PMI::GetColor()
		{
			return ((Stk_PMIImpl*)m_pImpl)->m_rgbaPMI;
		}

		STK_STATUS	Stk_PMI::SetColor(const STK_RGBA32& i_rgbaPMI)
		{
			((Stk_PMIImpl*)m_pImpl)->m_rgbaPMI = i_rgbaPMI;
			return STK_SUCCESS;
		}

		StkPMITypeEnum	Stk_PMI::GetType()
		{
			return ((Stk_PMIImpl*)m_pImpl)->m_eType;
		}

		STK_STATUS	Stk_PMI::SetType(StkPMITypeEnum i_nuType)
		{
			((Stk_PMIImpl*)m_pImpl)->m_eType = i_nuType;
			return STK_SUCCESS;
		}

		const STK_UINT32& Stk_PMI::GetStyle()
		{
			return ((Stk_PMIImpl*)m_pImpl)->m_uiStyle;
		}

		STK_STATUS  Stk_PMI::SetStyle(const STK_UINT32& i_uiStyle)
		{
			((Stk_PMIImpl*)m_pImpl)->m_uiStyle = i_uiStyle;
			return STK_SUCCESS;
		}

		const STK_INT32& Stk_PMI::GetUseType()
		{
			return ((Stk_PMIImpl*)m_pImpl)->m_uiUseType;
		}

		STK_STATUS Stk_PMI::SetUseType(const STK_UINT32& i_uiUseType)
		{
			((Stk_PMIImpl*)m_pImpl)->m_uiUseType = i_uiUseType;
			return STK_SUCCESS;
		}

		STK_STATUS	Stk_PMI::SetPMISource(StkPMISourceEnum i_ePMISource)
		{
			((Stk_PMIImpl*)m_pImpl)->m_ePMISource = i_ePMISource;
			return STK_SUCCESS;
		}

		StkPMISourceEnum Stk_PMI::GetPMISource()
		{
			return ((Stk_PMIImpl*)m_pImpl)->m_ePMISource;
		}

		STK_STATUS Stk_PMI::SetAxisX(const STK_DIR32& i_dirX)
		{
			((Stk_PMIImpl*)m_pImpl)->m_dirX = i_dirX;
			return STK_SUCCESS;
		}

		STK_DIR32&	Stk_PMI::GetAxisX()
		{
			return ((Stk_PMIImpl*)m_pImpl)->m_dirX;
		}

		STK_STATUS Stk_PMI::SetAxisY(const STK_DIR32& i_dirY)
		{
			((Stk_PMIImpl*)m_pImpl)->m_dirY = i_dirY;
			return STK_SUCCESS;
		}

		STK_DIR32&	Stk_PMI::GetAxisY()
		{
			return ((Stk_PMIImpl*)m_pImpl)->m_dirY;
		}

		STK_STATUS	Stk_PMI::SetSymBasePoint(const STK_PNT32& i_pntSymBase)
		{
			((Stk_PMIImpl*)m_pImpl)->m_pntSymBase = i_pntSymBase;
			return STK_SUCCESS;
		}

		STK_PNT32&	Stk_PMI::GetSymBasePoint()
		{
			return ((Stk_PMIImpl*)m_pImpl)->m_pntSymBase;
		}

		STK_BOOL Stk_PMI::IsDisplay()
		{
			return (((Stk_PMIImpl*)m_pImpl)->m_uiStyle & 0x00000001) != 0;
		}

		void Stk_PMI::SetDisplay(STK_BOOL i_bIsDisplay)
		{
			((Stk_PMIImpl*)m_pImpl)->m_uiStyle &= 0x11111110;
			((Stk_PMIImpl*)m_pImpl)->m_uiStyle |= (i_bIsDisplay ? 0x00000001 : 0x00000000);
		}

		const vector<Stk_LeaderPtr>&	Stk_PMI::GetLeaders()
		{
			return ((Stk_PMIImpl*)m_pImpl)->m_vcLeaders;
		}

		STK_STATUS	Stk_PMI::SetLeaders(const vector<Stk_LeaderPtr>& i_vcLeaders)
		{
			((Stk_PMIImpl*)m_pImpl)->m_vcLeaders = i_vcLeaders;
			return STK_SUCCESS;
		}

		STK_STATUS	Stk_PMI::AddLeader(Stk_LeaderPtr i_leaderP)
		{
			if (i_leaderP != NULL)
			{
				((Stk_PMIImpl*)m_pImpl)->m_vcLeaders.push_back(i_leaderP);
			}
			else
			{
				return STK_ERROR;
			}
			return STK_SUCCESS;
		}

		STK_STATUS  Stk_PMI::AddExline(Stk_ExtensionlinePtr i_exlineP)
		{
			if (i_exlineP != NULL)
				((Stk_PMIImpl*)m_pImpl)->m_vcExLines.push_back(i_exlineP);
			else
				return STK_ERROR;
			return STK_SUCCESS;
		}

		const vector<Stk_ExtensionlinePtr>& Stk_PMI::GetExLines()
		{
			return ((Stk_PMIImpl*)m_pImpl)->m_vcExLines;
		}

		STK_STATUS Stk_PMI::SetExLines(const vector<Stk_ExtensionlinePtr>& i_vcExLines)
		{
			((Stk_PMIImpl*)m_pImpl)->m_vcExLines = i_vcExLines;
			return STK_SUCCESS;
		}

		const vector<Stk_ComTextPtr>& Stk_PMI::GetComText()
		{
			return ((Stk_PMIImpl*)m_pImpl)->m_vcComTexts;
		}

		STK_STATUS Stk_PMI::AddComText(Stk_ComTextPtr i_ComTextP)
		{
			if (i_ComTextP != NULL)
			{
				((Stk_PMIImpl*)m_pImpl)->m_vcComTexts.push_back(i_ComTextP);
			}
			else
			{
				return STK_ERROR;
			}
			return STK_SUCCESS;
		}

		const vector<Stk_OutFramePtr>& Stk_PMI::GetOutFrame()
		{
			return ((Stk_PMIImpl*)m_pImpl)->m_vcOutFrames;
		}

		STK_STATUS Stk_PMI::SetOutFrame(Stk_OutFramePtr i_OutFrameP)
		{
			// …Ë÷√∂®“Â¿‡–Õ
			i_OutFrameP->SetDefineType(STK_FRAME_DEF_PMI);

			// ≤Â»Î ˝◊È
			if (i_OutFrameP != NULL)
			{
				((Stk_PMIImpl*)m_pImpl)->m_vcOutFrames.push_back(i_OutFrameP);
			}
			else
			{
				return STK_ERROR;
			}
			return STK_SUCCESS;
		}

		const vector<Stk_SpecialLinePtr>& Stk_PMI::GetSpecialLines()
		{
			return ((Stk_PMIImpl*)m_pImpl)->m_vcSpecialLines;
		}

		STK_STATUS Stk_PMI::AddSpecialLine(Stk_SpecialLinePtr i_SpecLineP)
		{
			if (i_SpecLineP != NULL)
			{
				((Stk_PMIImpl*)m_pImpl)->m_vcSpecialLines.push_back(i_SpecLineP);
			}
			else
			{
				return STK_ERROR;
			}
			return STK_SUCCESS;
		}

		STK_STATUS Stk_PMI::AddImage(Stk_ImagePtr i_stkImageP)
		{
			if (i_stkImageP != NULL)
			{
				((Stk_PMIImpl*)m_pImpl)->m_vcImages.push_back(i_stkImageP);
			}
			return STK_SUCCESS;
		}

		STK_UINT32 Stk_PMI::GetImageNum()
		{
			return ((Stk_PMIImpl*)m_pImpl)->m_vcImages.size();
		}

		Stk_ImagePtr Stk_PMI::GetImageByIndex(STK_UINT32 i_nuIndex)
		{
			return ((Stk_PMIImpl*)m_pImpl)->m_vcImages.at(i_nuIndex);
		}

		STK_STATUS	Stk_PMI::ToJson(void* i_DataP, void* i_JsonDocP)
		{
            return ((Stk_PMIImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)i_JsonDocP);
        }
        
        STK_STATUS	Stk_PMI::FromJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_PMIImpl*)m_pImpl)->FromJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		STK_STATUS Stk_PMI::AddUserData(Stk_UserDataPtr i_stkUserDataP)
		{
			return ((Stk_PMIImpl*)m_pImpl)->AddUserData(i_stkUserDataP);
		}

		map<STK_ID, Stk_UserDataPtr>* Stk_PMI::GetUserDataByName(wstring i_wstrDataName)
		{
			map<wstring, map<STK_ID, Stk_UserDataPtr>*>::iterator  itor;
			STK_STATUS stat = STK_SUCCESS;

			// ≤È—ØŒ®“ª±Í ∂
			itor = ((Stk_PMIImpl*)m_pImpl)->m_mapUserDatas.find(i_wstrDataName);
			if (itor == ((Stk_PMIImpl*)m_pImpl)->m_mapUserDatas.end())
			{
				return NULL;
			}
			else
			{
				return itor->second;
			}
		}

		STK_STATUS Stk_PMI::DelUserData(wstring i_wstrDataName, STK_ID i_id)
		{
			map<wstring, map<STK_ID, Stk_UserDataPtr>*>::iterator  itor;
			map<STK_ID, Stk_UserDataPtr>::iterator UDitor;
			STK_STATUS	stat = STK_SUCCESS;

			// ≤È—ØŒ®“ª±Í ∂
			itor = ((Stk_PMIImpl*)m_pImpl)->m_mapUserDatas.find(i_wstrDataName);
			if (itor != ((Stk_PMIImpl*)m_pImpl)->m_mapUserDatas.end())
			{
				UDitor = (itor->second)->find(i_id);
				if (UDitor != (itor->second)->end())
				{
					//  Õ∑≈Stk_UserData*
					(itor->second)->erase(UDitor);
					if (((Stk_PMIImpl*)m_pImpl)->m_szUserDataCnt > 0)
					{
						((Stk_PMIImpl*)m_pImpl)->m_szUserDataCnt--;
					}
					return STK_SUCCESS;
				}
				else
				{
					// ≤ª¥Ê‘⁄
					return STK_ERROR;
				}
			}
			else {
				// ≤ª¥Ê‘⁄
				return STK_ERROR;
			}
		}

		STK_BOOL	Stk_PMI::IsParallelScreen()
		{
			return (((Stk_PMIImpl*)m_pImpl)->m_uiStyle & 0x00000100) != 0;
		}

		void	Stk_PMI::SetParallelScreen(STK_BOOL i_bIsParallelScreen)
		{
			((Stk_PMIImpl*)m_pImpl)->m_uiStyle &= 0x11111011;
			((Stk_PMIImpl*)m_pImpl)->m_uiStyle |= (i_bIsParallelScreen ? 0x00000100 : 0x00000000);
		}

		STK_BOOL	Stk_PMI::IsFix()
		{
			return (((Stk_PMIImpl*)m_pImpl)->m_uiStyle & 0x00001000) != 0;
		}

		void	Stk_PMI::SetFix(STK_BOOL i_bIsFix)
		{
			((Stk_PMIImpl*)m_pImpl)->m_uiStyle &= 0x11110111;
			((Stk_PMIImpl*)m_pImpl)->m_uiStyle |= (i_bIsFix ? 0x00001000 : 0x00000000);
		}

		STK_BOOL Stk_PMI::IsOldVersion()
		{
			return (((Stk_PMIImpl*)m_pImpl)->m_uiStyle & 0x00010000) != 0;
		}

		STK_STATUS	Stk_PMI::SetOldVersion(STK_BOOL i_bOldVersion)
		{
			((Stk_PMIImpl*)m_pImpl)->m_uiStyle &= 0x11101111;
			((Stk_PMIImpl*)m_pImpl)->m_uiStyle |= (i_bOldVersion ? 0x00010000 : 0x00000000);
			return STK_SUCCESS;
		}

		STK_BOOL	Stk_PMI::IsAdjusted()
		{
			return (((Stk_PMIImpl*)m_pImpl)->m_uiStyle & 0x00100000) != 0;
		}

		void	Stk_PMI::SetAdjusted(STK_BOOL i_bIsAdjusted)
		{
			((Stk_PMIImpl*)m_pImpl)->m_uiStyle &= 0x11011111;
			((Stk_PMIImpl*)m_pImpl)->m_uiStyle |= (i_bIsAdjusted ? 0x00100000 : 0x00000000);
		}

		STK_BOOL	Stk_PMI::IsFront()
		{
			return (((Stk_PMIImpl*)m_pImpl)->m_uiStyle & 0x01000000) != 0;
		}

		STK_STATUS	Stk_PMI::SetFront(STK_BOOL i_bFront)
		{
			((Stk_PMIImpl*)m_pImpl)->m_uiStyle &= 0x10111111;
			((Stk_PMIImpl*)m_pImpl)->m_uiStyle |= (i_bFront ? 0x01000000 : 0x00000000);
			return STK_SUCCESS;
		}
	}
}
