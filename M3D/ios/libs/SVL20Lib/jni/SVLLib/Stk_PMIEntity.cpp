#include "Stk_PMIEntity.h"
#include "Stk_Object.h"
#include "Stk_ObjectImpl.h"
#include "Stk_DocumentImpl.h"
#include "ChineseCode.h"
#include "StringTools.h"


#define  LEADER_NODE_NAME_LEADERTYPE "leaderType"
#define  LEADER_NODE_NAME_TERMTYPE   "termType"
#define  LEADER_NODE_NAME_TERMSIZE  "termSize"
#define  LEADER_NODE_NAME_TERMLOC   "termLoc"
#define  LEADER_NODE_NAME_TERMDIR   "termDir"
#define  LEADER_NODE_NAME_HASBENDLINE "hasBendLine"
#define  LEADER_NODE_NAME_CURVES    "curves"
#define  LEADER_NODE_NAME_LINES     "lines"
#define  LEADER_NODE_NAME_POLYLINES "polylines"
#define  LEADER_NODE_NAME_ARCS   "arcs"

#define  EXLINES_NODE_NAME_EXLINETYPE  "exLineType"
#define  EXLINES_NODE_NAME_STYLE       "style"
#define  EXLINES_NODE_NAME_CURVES      "curves"
#define  EXLINES_NODE_NAME_LINES     "lines"
#define  EXLINES_NODE_NAME_POLYLINES "polylines"
#define  EXLINES_NODE_NAME_ARCS   "arcs"

#define  SPECIALLINES_NODE_NAME_USETYPE  "useType"
#define  SPECIALLINES_NODE_NAME_CURVES    "curves"
#define  SPECIALLINES_NODE_NAME_LINES     "lines"
#define  SPECIALLINES_NODE_NAME_POLYLINES "polylines"
#define  SPECIALLINES_NODE_NAME_ARCS   "arcs"

#define  OUTFRAMES_NODE_NAME_FRAMETYPE     "frameType"
#define  OUTFRAMES_NODE_NAME_DEFTYPE       "defType"
#define  OUTFRAMES_NODE_NAME_CURVES    "curves"
#define  OUTFRAMES_NODE_NAME_LINES     "lines"
#define  OUTFRAMES_NODE_NAME_POLYLINES "polylines"
#define  OUTFRAMES_NODE_NAME_ARCS   "arcs"

namespace HoteamSoft {
	namespace SVLLib {
		class Stk_LeaderImpl : public Stk_ObjectImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_Leader)
		public:
			Stk_LeaderImpl(void);
			virtual ~Stk_LeaderImpl(void);

		public:      
			StkLeaderTypeEnum			m_eLeaderType;     //¿‡–Õ£∫≥§∂»/Ω«∂»
			StkTermTypeEnum				m_eTermType;       //ƒ©∂À¿‡–Õ
			STK_FLOAT32					m_fTermSizeA[2];   //ƒ©∂ÀøÌ∏ﬂ
			STK_PNT32					m_pntTermLoc;      //ƒ©∂À∆ ºŒª÷√
			STK_DIR32					m_dirTerm;         //ƒ©∂À∑ΩœÚ
			STK_BOOL                    m_bIsBendLine;     // «∑Ò∫¨”–∂˛Œ¨’€Õ‰œﬂ
			vector<Stk_CurveEntityPtr>	m_vcCurves;		   //œﬂÃı¡–
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
			STK_STATUS					ToJson(Value* i_pJsonNode, Document* pDoc);
		};

		inline STK_STATUS	Stk_LeaderImpl::ToJson(Value* i_pJsonNode, Document* pDoc)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode || !pDoc)
				return STK_ERROR;
			i_pJsonNode->SetObject();

			i_pJsonNode->AddMember(LEADER_NODE_NAME_LEADERTYPE, m_eLeaderType, pDoc->GetAllocator());
			i_pJsonNode->AddMember(LEADER_NODE_NAME_TERMTYPE, m_eTermType, pDoc->GetAllocator());
			
			string strValue;
			char szValue[256] = { 0 };
			//termSize
			sprintf(szValue, "%.8f", m_fTermSizeA[0]);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_fTermSizeA[1]);
			strValue += szValue;
			i_pJsonNode->AddMember(LEADER_NODE_NAME_TERMSIZE, strValue, pDoc->GetAllocator());
			
			//termLoc
			strValue = "";
			sprintf(szValue, "%.8f", m_pntTermLoc.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntTermLoc.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntTermLoc.Z);
			strValue += szValue; 
			i_pJsonNode->AddMember(LEADER_NODE_NAME_TERMLOC, strValue, pDoc->GetAllocator());

			//termDir
			strValue = "";
			sprintf(szValue, "%.8f", m_dirTerm.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirTerm.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirTerm.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(LEADER_NODE_NAME_TERMDIR, strValue, pDoc->GetAllocator());
			i_pJsonNode->AddMember(LEADER_NODE_NAME_HASBENDLINE, m_bIsBendLine, pDoc->GetAllocator());

			//curves
			Value valCurves;
			valCurves.SetObject();

			//lines
			Value valLines;
			valLines.SetArray();
			

			//polylines
			Value valPolylines;
			valPolylines.SetArray();

			//arc
			Value valArcs;
			valArcs.SetArray();

			int iLineNum = 0, iPolylineNum = 0, iArcNum = 0;
			vector<Stk_CurveEntityPtr>::iterator curvesItor = m_vcCurves.begin();
			while (curvesItor != m_vcCurves.end())
			{
				if (*curvesItor == NULL)
				{
					curvesItor++;
					continue;
				}
				Value valLine, valPolyline, valArc;
				if ((*curvesItor)->GetCurveType() == CURVE_TYPE_LINE)
				{
					(*curvesItor)->ToJson(&valLine, pDoc);
					valLines.PushBack(valLine, pDoc->GetAllocator());
					iLineNum++;
				}
				else if ((*curvesItor)->GetCurveType() == CURVE_TYPE_POLYLINE)
				{
					(*curvesItor)->ToJson(&valPolyline, pDoc);
					valPolylines.PushBack(valPolyline, pDoc->GetAllocator());
					iPolylineNum++;
				}
				else if ((*curvesItor)->GetCurveType() == CURVE_TYPE_ELLIPSE)
				{
					(*curvesItor)->ToJson(&valArc, pDoc);
					valArcs.PushBack(valArc, pDoc->GetAllocator());
					iArcNum++;
				}
				curvesItor++;
			}
			if(iLineNum > 0)
				valCurves.AddMember(LEADER_NODE_NAME_LINES, valLines, pDoc->GetAllocator());
			if(iPolylineNum > 0)
				valCurves.AddMember(LEADER_NODE_NAME_POLYLINES, valPolylines, pDoc->GetAllocator());
			if (iArcNum > 0)
				valCurves.AddMember(LEADER_NODE_NAME_ARCS, valArcs, pDoc->GetAllocator());
			i_pJsonNode->AddMember(LEADER_NODE_NAME_CURVES, valCurves, pDoc->GetAllocator());

			return STK_SUCCESS;
		}

		STK_STATUS	Stk_LeaderImpl::FromJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode)
				return STK_ERROR;
			m_eLeaderType = (StkLeaderTypeEnum)(*i_pJsonNode)[LEADER_NODE_NAME_LEADERTYPE].GetInt();
			m_eTermType = (StkTermTypeEnum)(*i_pJsonNode)[LEADER_NODE_NAME_TERMTYPE].GetInt();

			//ƒ©∂ÀøÌ∏ﬂ
			string  strValue;
			strValue = (*i_pJsonNode)[LEADER_NODE_NAME_TERMSIZE].GetString();
			std::vector<std::string> vecValue;
			CStringTools::StringSplit(strValue, " ", vecValue);
			m_fTermSizeA[0] = atof(vecValue[0].c_str());
			m_fTermSizeA[1] = atof(vecValue[1].c_str());
			vecValue.clear();
		
			//ƒ©∂À∆ ºŒª÷√
			strValue = (*i_pJsonNode)[LEADER_NODE_NAME_TERMLOC].GetString();
			CStringTools::StringSplit(strValue, " ", vecValue);
			m_pntTermLoc.X = atof(vecValue[0].c_str());
			m_pntTermLoc.Y = atof(vecValue[1].c_str());
			m_pntTermLoc.Z = atof(vecValue[2].c_str());
			vecValue.clear();

			//ƒ©∂À∑ΩœÚ
			strValue = (*i_pJsonNode)[LEADER_NODE_NAME_TERMDIR].GetString();
			CStringTools::StringSplit(strValue, " ", vecValue);
			m_dirTerm.X = atof(vecValue[0].c_str());
			m_dirTerm.Y = atof(vecValue[1].c_str());
			m_dirTerm.Z = atof(vecValue[2].c_str());
			vecValue.clear();
	
			m_bIsBendLine = (*i_pJsonNode)[LEADER_NODE_NAME_HASBENDLINE].GetBool();

			Value::MemberIterator curvesNodeIter = i_pJsonNode->FindMember(LEADER_NODE_NAME_CURVES);
			if (curvesNodeIter != i_pJsonNode->MemberEnd())
			{
				//lines
				Value::MemberIterator linesNodeIter = curvesNodeIter->value.FindMember(LEADER_NODE_NAME_LINES);
				if (linesNodeIter != curvesNodeIter->value.MemberEnd())
				{
					Value valLines = linesNodeIter->value.GetArray();
					Value valLine;
					for (int i = 0; i < valLines.Size(); i++)
					{
						valLine = valLines[i];
						Stk_LinePtr lineP = Stk_LinePtr::CreateObject();
						lineP->FromJson(&valLine, i_JsonDocP);
						m_vcCurves.push_back(lineP);
					}
				}
				//polylines
				Value::MemberIterator polylinesNodeIter = curvesNodeIter->value.FindMember(LEADER_NODE_NAME_POLYLINES);
				if (polylinesNodeIter != curvesNodeIter->value.MemberEnd())
				{
					Value valPolylines = polylinesNodeIter->value.GetArray();
					Value valPolyline;
					for (int i = 0; i < valPolylines.Size(); i++)
					{
						valPolyline = valPolylines[i];
						Stk_PolyLinePtr polylineP = Stk_PolyLinePtr::CreateObject();
						polylineP->FromJson(&valPolyline, i_JsonDocP);
						m_vcCurves.push_back(polylineP);
					}
				}
				//arc
				Value::MemberIterator arcNodeIter = curvesNodeIter->value.FindMember(LEADER_NODE_NAME_ARCS);
				if (arcNodeIter != curvesNodeIter->value.MemberEnd())
				{
					Value valArcs = arcNodeIter->value.GetArray();
					Value valArc;
					for (int i = 0; i < valArcs.Size(); i++)
					{
						valArc = valArcs[i];
						Stk_ArcPtr arcP = Stk_ArcPtr::CreateObject();
						arcP->FromJson(&valArc, i_JsonDocP);
						m_vcCurves.push_back(arcP);
					}
				}
			}

			return STK_SUCCESS;
		}

		STK_CLASS_IMPL_IMPLEMENT(Stk_Leader)

		Stk_LeaderImpl::Stk_LeaderImpl(void)
		{
			m_eLeaderType = LEADER_TYPE_UNKNOWN;
			m_eTermType = TERM_PATSMNONE;
			m_fTermSizeA[0] = 0.0;
			m_fTermSizeA[1] = 0.0;
			m_bIsBendLine = false;
		}

		Stk_LeaderImpl::~Stk_LeaderImpl(void)
		{

		}

		STK_IMPLEMENT_CLASS(Stk_Leader, Stk_Object)

		Stk_Leader::Stk_Leader() : Stk_Object(new Stk_LeaderImpl())
		{
		}


		Stk_Leader::~Stk_Leader()
		{
			DELETE_STK_CLASS_IMPL
		}

		StkLeaderTypeEnum	Stk_Leader::GetLeaderType()
		{
			return ((Stk_LeaderImpl*)m_pImpl)->m_eLeaderType;
		}

		STK_STATUS Stk_Leader::SetLeaderType(StkLeaderTypeEnum i_eType)
		{
			((Stk_LeaderImpl*)m_pImpl)->m_eLeaderType = i_eType;
			return STK_SUCCESS;
		}

		void Stk_Leader::GetTerminator(StkTermTypeEnum& o_eTermType, STK_FLOAT32 o_SizeA[2], STK_PNT32& o_pntLoc, STK_DIR32& o_dirTerm)
		{
			o_eTermType = ((Stk_LeaderImpl*)m_pImpl)->m_eTermType;
			memcpy(o_SizeA, ((Stk_LeaderImpl*)m_pImpl)->m_fTermSizeA, sizeof(STK_FLOAT32) * 2);
			o_pntLoc = ((Stk_LeaderImpl*)m_pImpl)->m_pntTermLoc;
			o_dirTerm = ((Stk_LeaderImpl*)m_pImpl)->m_dirTerm;
		}

		STK_STATUS	Stk_Leader::SetTerminator(StkTermTypeEnum i_eTermType, STK_FLOAT32 i_SizeA[2], const STK_PNT32& i_pntLoc, const STK_DIR32& i_dirTerm)
		{
			((Stk_LeaderImpl*)m_pImpl)->m_eTermType = i_eTermType;
			memcpy(((Stk_LeaderImpl*)m_pImpl)->m_fTermSizeA, i_SizeA, sizeof(STK_FLOAT32) * 2);
			((Stk_LeaderImpl*)m_pImpl)->m_pntTermLoc = i_pntLoc;
			((Stk_LeaderImpl*)m_pImpl)->m_dirTerm = i_dirTerm;
			return STK_SUCCESS;
		}

		STK_STATUS	Stk_Leader::SetBendLineFlag(STK_BOOL i_bIsBendLine)
		{
			((Stk_LeaderImpl*)m_pImpl)->m_bIsBendLine = i_bIsBendLine;
			return STK_SUCCESS;
		}

		STK_BOOL Stk_Leader::GetBendLineFlag()
		{
			return ((Stk_LeaderImpl*)m_pImpl)->m_bIsBendLine;
		}

		vector<Stk_CurveEntityPtr> Stk_Leader::GetAllCurve()
		{
			return ((Stk_LeaderImpl*)m_pImpl)->m_vcCurves;
		}

		HoteamSoft::SVLLib::Stk_CurveEntityPtr Stk_Leader::GetCurve(int iIndex)
		{
			if (iIndex < 0 ||
				iIndex >= ((Stk_LeaderImpl*)m_pImpl)->m_vcCurves.size())
			{
				return nullptr;
			}
			return ((Stk_LeaderImpl*)m_pImpl)->m_vcCurves[iIndex];
		}

		STK_STATUS Stk_Leader::AddCurve(Stk_CurveEntityPtr i_CurveP)
		{
			if (i_CurveP != NULL)
			{
				((Stk_LeaderImpl*)m_pImpl)->m_vcCurves.push_back(i_CurveP);
			}
			else
			{
				return STK_ERROR;
			}
			return STK_SUCCESS;
		}

		STK_STATUS	Stk_Leader::ToJson(void* i_DataP, void* pDoc)
		{
			return ((Stk_LeaderImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)pDoc);
		}

		STK_STATUS	Stk_Leader::FromJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_LeaderImpl*)m_pImpl)->FromJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		class Stk_ExtensionlineImpl : public Stk_ObjectImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_Extensionline)
		public:
			Stk_ExtensionlineImpl(void);
			virtual ~Stk_ExtensionlineImpl(void);
		public:
			StkExtensTypeEnum			m_eExLineType;	//—”≥§œﬂµƒ¿‡–Õ
			STK_INT32					m_uiStyle;	    //—˘ Ω
			Stk_CurveEntityPtr			m_CurveP;
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
			STK_STATUS					FromJson(Value* i_pJsonNode, Document* i_pJsonDoc);

			/*****************************************************************************
			*	@brief		–¥View ˝æ›
			*	@param[in]	Value* i_pJsonParentNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			–¥»Î≥…π¶
			*	@retval		∆‰À¸				–¥»Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					ToJson(Value* i_pJsonNode, Document* pDoc);
		};

		STK_STATUS	Stk_ExtensionlineImpl::ToJson(Value* i_pJsonNode, Document* pDoc)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode || !pDoc)
				return STK_ERROR;
			i_pJsonNode->SetObject();
			
			i_pJsonNode->AddMember(EXLINES_NODE_NAME_EXLINETYPE, m_eExLineType, pDoc->GetAllocator());
			i_pJsonNode->AddMember(EXLINES_NODE_NAME_STYLE, m_uiStyle, pDoc->GetAllocator());

			if (m_CurveP == NULL)
				return STK_SUCCESS;

			//curves
			Value valCurves;
			valCurves.SetObject();

			//lines
			Value valLines;
			valLines.SetArray();


			//polylines
			Value valPolylines;
			valPolylines.SetArray();

			//arc
			Value valArcs;
			valArcs.SetArray();
			
			Value valLine, valPolyline, valArc;
			if (m_CurveP->GetCurveType() == CURVE_TYPE_LINE)
			{
				m_CurveP->ToJson(&valLine, pDoc);
				valLines.PushBack(valLine, pDoc->GetAllocator());
				valCurves.AddMember(LEADER_NODE_NAME_LINES, valLines, pDoc->GetAllocator());
			}
			else if (m_CurveP->GetCurveType() == CURVE_TYPE_POLYLINE)
			{
				m_CurveP->ToJson(&valPolyline, pDoc);
				valPolylines.PushBack(valPolyline, pDoc->GetAllocator());
				valCurves.AddMember(LEADER_NODE_NAME_POLYLINES, valPolylines, pDoc->GetAllocator());
			}
			else if (m_CurveP->GetCurveType() == CURVE_TYPE_ELLIPSE)
			{
				m_CurveP->ToJson(&valArc, pDoc);
				valArcs.PushBack(valArc, pDoc->GetAllocator());
				valCurves.AddMember(LEADER_NODE_NAME_ARCS, valArcs, pDoc->GetAllocator());
			}
				
			i_pJsonNode->AddMember(LEADER_NODE_NAME_CURVES, valCurves, pDoc->GetAllocator());

			return STK_SUCCESS;
		}

		STK_STATUS	Stk_ExtensionlineImpl::FromJson(Value* i_pJsonNode, Document* i_pJsonDoc)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode)
				return STK_ERROR;
			m_eExLineType = (StkExtensTypeEnum)(*i_pJsonNode)[EXLINES_NODE_NAME_EXLINETYPE].GetInt();
			m_uiStyle = (StkTermTypeEnum)(*i_pJsonNode)[EXLINES_NODE_NAME_STYLE].GetInt();

			//curves
			Value::MemberIterator curvesNodeIter = i_pJsonNode->FindMember(EXLINES_NODE_NAME_CURVES);
			if (curvesNodeIter != i_pJsonNode->MemberEnd())
			{
				//lines
				Value::MemberIterator linesNodeIter = curvesNodeIter->value.FindMember(EXLINES_NODE_NAME_LINES);
				if (linesNodeIter != curvesNodeIter->value.MemberEnd())
				{
					Value valLines = linesNodeIter->value.GetArray();
					Value valLine;
					for (int i = 0; i < valLines.Size(); i++)
					{
						valLine = valLines[i];
						m_CurveP = Stk_LinePtr::CreateObject();
						m_CurveP->FromJson(&valLine, i_pJsonDoc);
					}
				}
				//polylines
				Value::MemberIterator polylinesNodeIter = curvesNodeIter->value.FindMember(EXLINES_NODE_NAME_POLYLINES);
				if (polylinesNodeIter != curvesNodeIter->value.MemberEnd())
				{
					Value valPolylines = polylinesNodeIter->value.GetArray();
					Value valPolyline;
					for (int i = 0; i < valPolylines.Size(); i++)
					{
						valPolyline = valPolylines[i];
						m_CurveP = Stk_PolyLinePtr::CreateObject();
						m_CurveP->FromJson(&valPolyline, i_pJsonDoc);
					}
				}
				//arc
				Value::MemberIterator arcNodeIter = curvesNodeIter->value.FindMember(EXLINES_NODE_NAME_ARCS);
				if (arcNodeIter != curvesNodeIter->value.MemberEnd())
				{
					Value valArcs = arcNodeIter->value.GetArray();
					Value valArc;
					for (int i = 0; i < valArcs.Size(); i++)
					{
						valArc = valArcs[i];
						m_CurveP = Stk_ArcPtr::CreateObject();
						m_CurveP->FromJson(&valArc, i_pJsonDoc);
					}
				}
			}

			return STK_SUCCESS;
		}

		STK_CLASS_IMPL_IMPLEMENT(Stk_Extensionline)

		Stk_ExtensionlineImpl::Stk_ExtensionlineImpl(void)
		{
			m_CurveP = NULL;
			m_eExLineType = EXTENS_TYPE_UNKNOWN;
		}

		Stk_ExtensionlineImpl::~Stk_ExtensionlineImpl(void)
		{

		}

		STK_IMPLEMENT_CLASS(Stk_Extensionline, Stk_Object)

		Stk_Extensionline::Stk_Extensionline(void) : Stk_Object(new Stk_ExtensionlineImpl())
		{
			((Stk_ExtensionlineImpl*)m_pImpl)->m_uiStyle = 0;
		}

		Stk_Extensionline::~Stk_Extensionline(void)
		{
			DELETE_STK_CLASS_IMPL
		}

		StkExtensTypeEnum Stk_Extensionline::GetExLineType()
		{
			return ((Stk_ExtensionlineImpl*)m_pImpl)->m_eExLineType;
		}

		STK_STATUS	Stk_Extensionline::SetExLineType(StkExtensTypeEnum i_nuType)
		{
			((Stk_ExtensionlineImpl*)m_pImpl)->m_eExLineType = i_nuType;
			return STK_SUCCESS;
		}

		STK_STATUS   Stk_Extensionline::SetCurve(Stk_CurveEntityPtr i_curveP)
		{
			((Stk_ExtensionlineImpl*)m_pImpl)->m_CurveP = i_curveP;
			return STK_SUCCESS;
		}

		STK_UINT32 Stk_Extensionline::GetStyle()
		{
			return ((Stk_ExtensionlineImpl*)m_pImpl)->m_uiStyle;
		}

		Stk_CurveEntityPtr  Stk_Extensionline::GetCurve()
		{
			return ((Stk_ExtensionlineImpl*)m_pImpl)->m_CurveP;
		}

		void Stk_Extensionline::SetStyle(STK_UINT32 i_uiStyle)
		{
			((Stk_ExtensionlineImpl*)m_pImpl)->m_uiStyle = i_uiStyle;
		}

		STK_STATUS	Stk_Extensionline::ToJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_ExtensionlineImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		STK_STATUS	Stk_Extensionline::FromJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_ExtensionlineImpl*)m_pImpl)->FromJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		class Stk_SpecialLineImpl : public Stk_ObjectImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_SpecialLine)
		public:
			Stk_SpecialLineImpl(void);
			virtual ~Stk_SpecialLineImpl(void);
		public:
			StkSpecLineUsageEnum		m_eLineUsage;		//œﬂ”√Õæ
			vector<Stk_CurveEntityPtr>	m_vcCurves;
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
		};

		STK_CLASS_IMPL_IMPLEMENT(Stk_SpecialLine)

		Stk_SpecialLineImpl::Stk_SpecialLineImpl()
		{

		}

		Stk_SpecialLineImpl::~Stk_SpecialLineImpl(void)
		{
			
		}

		STK_STATUS	Stk_SpecialLineImpl::ToJson(Value* i_pJsonNode, Document* pDoc)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode || !pDoc)
				return STK_ERROR;
			i_pJsonNode->SetObject();
			i_pJsonNode->AddMember(SPECIALLINES_NODE_NAME_USETYPE, m_eLineUsage, pDoc->GetAllocator());

			//curves
			Value valCurves;
			valCurves.SetObject();

			//lines
			Value valLines;
			valLines.SetArray();


			//polylines
			Value valPolylines;
			valPolylines.SetArray();

			//arc
			Value valArcs;
			valArcs.SetArray();

			int iLineNum = 0, iPolylineNum = 0, iArcNum = 0;
			vector<Stk_CurveEntityPtr>::iterator curvesItor = m_vcCurves.begin();
			while (curvesItor != m_vcCurves.end())
			{
				if (*curvesItor == NULL)
				{
					curvesItor++;
					continue;
				}
				Value valLine, valPolyline, valArc;
				if ((*curvesItor)->GetCurveType() == CURVE_TYPE_LINE)
				{
					(*curvesItor)->ToJson(&valLine, pDoc);
					valLines.PushBack(valLine, pDoc->GetAllocator());
					iLineNum++;
				}
				else if ((*curvesItor)->GetCurveType() == CURVE_TYPE_POLYLINE)
				{
					(*curvesItor)->ToJson(&valPolyline, pDoc);
					valPolylines.PushBack(valPolyline, pDoc->GetAllocator());
					iPolylineNum++;
				}
				else if ((*curvesItor)->GetCurveType() == CURVE_TYPE_ELLIPSE)
				{
					(*curvesItor)->ToJson(&valArc, pDoc);
					valArcs.PushBack(valArc, pDoc->GetAllocator());
					iArcNum++;
				}
				curvesItor++;
			}
			if (iLineNum > 0)
				valCurves.AddMember(LEADER_NODE_NAME_LINES, valLines, pDoc->GetAllocator());
			if (iPolylineNum > 0)
				valCurves.AddMember(LEADER_NODE_NAME_POLYLINES, valPolylines, pDoc->GetAllocator());
			if (iArcNum > 0)
				valCurves.AddMember(LEADER_NODE_NAME_ARCS, valArcs, pDoc->GetAllocator());
			i_pJsonNode->AddMember(LEADER_NODE_NAME_CURVES, valCurves, pDoc->GetAllocator());

			return STK_SUCCESS;
		}

		STK_STATUS	Stk_SpecialLineImpl::FromJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode)
				return STK_ERROR;
			m_eLineUsage = (StkSpecLineUsageEnum)(*i_pJsonNode)[SPECIALLINES_NODE_NAME_USETYPE].GetInt();

			//curves
			Value::MemberIterator curvesNodeIter = i_pJsonNode->FindMember(SPECIALLINES_NODE_NAME_CURVES);
			if (curvesNodeIter != i_pJsonNode->MemberEnd())
			{
				//lines
				Value::MemberIterator linesNodeIter = curvesNodeIter->value.FindMember(SPECIALLINES_NODE_NAME_LINES);
				if (linesNodeIter != curvesNodeIter->value.MemberEnd())
				{
					Value valLines = linesNodeIter->value.GetArray();
					Value valLine;
					for (int i = 0; i < valLines.Size(); i++)
					{
						valLine = valLines[i];
						Stk_LinePtr lineP = Stk_LinePtr::CreateObject();
						lineP->FromJson(&valLine, i_JsonDocP);
						m_vcCurves.push_back(lineP);
					}
				}
				//polylines
				Value::MemberIterator polylinesNodeIter = curvesNodeIter->value.FindMember(SPECIALLINES_NODE_NAME_POLYLINES);
				if (polylinesNodeIter != curvesNodeIter->value.MemberEnd())
				{
					Value valPolylines = polylinesNodeIter->value.GetArray();
					Value valPolyline;
					for (int i = 0; i < valPolylines.Size(); i++)
					{
						valPolyline = valPolylines[i];
						Stk_PolyLinePtr polylineP = Stk_PolyLinePtr::CreateObject();
						polylineP->FromJson(&valPolyline, i_JsonDocP);
						m_vcCurves.push_back(polylineP);
					}
				}
				//ellipse
				Value::MemberIterator arcNodeIter = curvesNodeIter->value.FindMember(SPECIALLINES_NODE_NAME_ARCS);
				if (arcNodeIter != curvesNodeIter->value.MemberEnd())
				{
					Value valArcs = arcNodeIter->value.GetArray();
					Value valArc;
					for (int i = 0; i < valArcs.Size(); i++)
					{
						valArc = valArcs[i];
						Stk_ArcPtr arcP = Stk_ArcPtr::CreateObject();
						arcP->FromJson(&valArc, i_JsonDocP);
						m_vcCurves.push_back(arcP);
					}
				}
			}
			return STK_SUCCESS;
		}

		Stk_SpecialLine::Stk_SpecialLine(void) : Stk_Object(new Stk_SpecialLineImpl())
		{

		}

		Stk_SpecialLine::~Stk_SpecialLine(void)
		{
			DELETE_STK_CLASS_IMPL
		}

		STK_IMPLEMENT_CLASS(Stk_SpecialLine, Stk_Object)

		StkSpecLineUsageEnum Stk_SpecialLine::GetSpecLinUse()
		{
			return ((Stk_SpecialLineImpl*)m_pImpl)->m_eLineUsage;
		}

		STK_STATUS	Stk_SpecialLine::SetSpecLinUse(StkSpecLineUsageEnum i_eLineType)
		{
			((Stk_SpecialLineImpl*)m_pImpl)->m_eLineUsage = i_eLineType;
			return STK_SUCCESS;
		}

		vector<Stk_CurveEntityPtr> Stk_SpecialLine::GetCurves()
		{
			return ((Stk_SpecialLineImpl*)m_pImpl)->m_vcCurves;
		}

		STK_STATUS Stk_SpecialLine::AddCurve(Stk_CurveEntityPtr i_CurveP)
		{
			if (i_CurveP == NULL)
			{
				return STK_ERROR;
			}
			((Stk_SpecialLineImpl*)m_pImpl)->m_vcCurves.push_back(i_CurveP);
			return STK_SUCCESS;
		}

		STK_STATUS	Stk_SpecialLine::ToJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_SpecialLineImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		STK_STATUS	Stk_SpecialLine::FromJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_SpecialLineImpl*)m_pImpl)->FromJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		class Stk_OutFrameImpl : public Stk_ObjectImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_OutFrame)
		public:
			Stk_OutFrameImpl(void);
			virtual ~Stk_OutFrameImpl(void);
		public:
			StkOutFrameUsageEnum			   m_eFrameType;        /*Õ‚øÚ¿‡–Õ*/
			StkOutFrameDefEnum				   m_eDefType;          /*”¶”√¿‡–Õ*/
			vector<Stk_CurveEntityPtr>		   m_vcCurves;          /*œﬂºØ*/
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
			STK_STATUS					FromJson(Value* i_pJsonNode, Document*i_JsonDocP);

			/*****************************************************************************
			*	@brief		–¥View ˝æ›
			*	@param[in]	Value* i_pJsonParentNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			–¥»Î≥…π¶
			*	@retval		∆‰À¸				–¥»Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					ToJson(Value* i_pJsonNode, Document* pDoc);
		};

		STK_STATUS	Stk_OutFrameImpl::FromJson(Value* i_pJsonNode, Document*i_JsonDocP)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode)
				return STK_ERROR;
			m_eFrameType = (StkOutFrameUsageEnum)(*i_pJsonNode)[OUTFRAMES_NODE_NAME_FRAMETYPE].GetInt();
			m_eDefType = (StkOutFrameDefEnum)(*i_pJsonNode)[OUTFRAMES_NODE_NAME_DEFTYPE].GetInt();

			//curves
			Value::MemberIterator curvesNodeIter = i_pJsonNode->FindMember(OUTFRAMES_NODE_NAME_CURVES);
			if (curvesNodeIter != i_pJsonNode->MemberEnd())
			{
				//lines
				Value::MemberIterator linesNodeIter = curvesNodeIter->value.FindMember(OUTFRAMES_NODE_NAME_LINES);
				if (linesNodeIter != curvesNodeIter->value.MemberEnd())
				{
					Value valLines = linesNodeIter->value.GetArray();
					Value valLine;
					for (int i = 0; i < valLines.Size(); i++)
					{
						valLine = valLines[i];
						Stk_LinePtr lineP = Stk_LinePtr::CreateObject();
						lineP->FromJson(&valLine, i_JsonDocP);
						m_vcCurves.push_back(lineP);
					}
				}
				//polylines
				Value::MemberIterator polylinesNodeIter = curvesNodeIter->value.FindMember(OUTFRAMES_NODE_NAME_POLYLINES);
				if (polylinesNodeIter != curvesNodeIter->value.MemberEnd())
				{
					Value valPolylines = polylinesNodeIter->value.GetArray();
					Value valPolyline;
					for (int i = 0; i < valPolylines.Size(); i++)
					{
						valPolyline = valPolylines[i];
						Stk_PolyLinePtr polylineP = Stk_PolyLinePtr::CreateObject();
						polylineP->FromJson(&valPolyline, i_JsonDocP);
						m_vcCurves.push_back(polylineP);
					}
				}
				//arc
				Value::MemberIterator arcNodeIter = curvesNodeIter->value.FindMember(OUTFRAMES_NODE_NAME_ARCS);
				if (arcNodeIter != curvesNodeIter->value.MemberEnd())
				{
					Value valArcs = arcNodeIter->value.GetArray();
					Value valArc;
					for (int i = 0; i < valArcs.Size(); i++)
					{
						valArc = valArcs[i];
						Stk_ArcPtr arcP = Stk_ArcPtr::CreateObject();
						arcP->FromJson(&valArc, i_JsonDocP);
						m_vcCurves.push_back(arcP);
					}
				}
			}
			return STK_SUCCESS;
		}

		STK_STATUS	Stk_OutFrameImpl::ToJson(Value* i_pJsonNode, Document* pDoc)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode || !pDoc)
				return STK_ERROR;
			i_pJsonNode->SetObject();
			i_pJsonNode->AddMember(OUTFRAMES_NODE_NAME_FRAMETYPE, m_eFrameType, pDoc->GetAllocator());
			i_pJsonNode->AddMember(OUTFRAMES_NODE_NAME_DEFTYPE, m_eDefType, pDoc->GetAllocator());

			//curves
			Value valCurves;
			valCurves.SetObject();

			//lines
			Value valLines;
			valLines.SetArray();


			//polylines
			Value valPolylines;
			valPolylines.SetArray();

			//arc
			Value valArcs;
			valArcs.SetArray();

			int iLineNum = 0, iPolylineNum = 0, iArcNum = 0;
			vector<Stk_CurveEntityPtr>::iterator curvesItor = m_vcCurves.begin();
			while (curvesItor != m_vcCurves.end())
			{
				if (*curvesItor == NULL)
				{
					curvesItor++;
					continue;
				}
				Value valLine, valPolyline, valArc;
				if ((*curvesItor)->GetCurveType() == CURVE_TYPE_LINE)
				{
					(*curvesItor)->ToJson(&valLine, pDoc);
					valLines.PushBack(valLine, pDoc->GetAllocator());
					iLineNum++;
				}
				else if ((*curvesItor)->GetCurveType() == CURVE_TYPE_POLYLINE)
				{
					(*curvesItor)->ToJson(&valPolyline, pDoc);
					valPolylines.PushBack(valPolyline, pDoc->GetAllocator());
					iPolylineNum++;
				}
				else if ((*curvesItor)->GetCurveType() == CURVE_TYPE_ELLIPSE)
				{
					(*curvesItor)->ToJson(&valArc, pDoc);
					valArcs.PushBack(valArc, pDoc->GetAllocator());
					iArcNum++;
				}
				curvesItor++;
			}
			if (iLineNum > 0)
				valCurves.AddMember(LEADER_NODE_NAME_LINES, valLines, pDoc->GetAllocator());
			if (iPolylineNum > 0)
				valCurves.AddMember(LEADER_NODE_NAME_POLYLINES, valPolylines, pDoc->GetAllocator());
			if (iArcNum > 0)
				valCurves.AddMember(LEADER_NODE_NAME_ARCS, valArcs, pDoc->GetAllocator());
			i_pJsonNode->AddMember(LEADER_NODE_NAME_CURVES, valCurves, pDoc->GetAllocator());

			return STK_SUCCESS;
		}

		STK_CLASS_IMPL_IMPLEMENT(Stk_OutFrame)

		Stk_OutFrameImpl::Stk_OutFrameImpl(void)
		{
			m_eFrameType = OUTFRAME_USAGE_UNKNOWN;
			m_eDefType = STK_FRAME_DEF_PMI;
		}

		Stk_OutFrameImpl::~Stk_OutFrameImpl(void)
		{

		}

		STK_IMPLEMENT_CLASS(Stk_OutFrame, Stk_Object)

		Stk_OutFrame::Stk_OutFrame() : Stk_Object(new Stk_OutFrameImpl())
		{
			
		}

		Stk_OutFrame::~Stk_OutFrame()
		{
			DELETE_STK_CLASS_IMPL
		}

		StkOutFrameUsageEnum Stk_OutFrame::GetFrameType()
		{
			return ((Stk_OutFrameImpl*)m_pImpl)->m_eFrameType;
		}

		STK_STATUS	Stk_OutFrame::SetFrameType(StkOutFrameUsageEnum i_eType)
		{
			((Stk_OutFrameImpl*)m_pImpl)->m_eFrameType = i_eType;
			return STK_SUCCESS;
		}

		StkOutFrameDefEnum	Stk_OutFrame::GetDefineType()
		{
			return ((Stk_OutFrameImpl*)m_pImpl)->m_eDefType;
		}

		STK_STATUS	Stk_OutFrame::SetDefineType(StkOutFrameDefEnum i_eType)
		{
			((Stk_OutFrameImpl*)m_pImpl)->m_eDefType = i_eType;
			return STK_SUCCESS;
		}

		const vector<Stk_CurveEntityPtr>& Stk_OutFrame::GetFrameData()
		{
			return ((Stk_OutFrameImpl*)m_pImpl)->m_vcCurves;
		}

		STK_STATUS	Stk_OutFrame::SetFrameData(Stk_CurveEntityPtr i_CurveP)
		{
			((Stk_OutFrameImpl*)m_pImpl)->m_vcCurves.push_back(i_CurveP);
			return STK_SUCCESS;
		}

		STK_STATUS	Stk_OutFrame::ToJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_OutFrameImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		STK_STATUS	Stk_OutFrame::FromJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_OutFrameImpl*)m_pImpl)->FromJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}
	}
}
