// All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file	Stk_CompositeText.cpp
*
*	@brief	Œƒ±æ°¢∏¥∫œŒƒ±æ¿‡µƒ µœ÷
*
*	@par	¿˙ ∑:
*
****************************************************************************/

#include "Stk_CompositeText.h"
#include "Stk_ObjectImpl.h"
#include "ChineseCode.h"
#include "StringTools.h"
#include "Stk_BaseDocumentImpl.h"
#include <math.h>

#define  COMTEXT_NODE_NAME_OUTBOX "outBox"
#define  COMTEXT_NODE_NAME_FRAMETYPE "frameType"
#define  COMTEXT_NODE_NAME_TEXTS    "texts"
#define  COMTEXT_NODE_NAME_ENVELOPE "envelope"

#define  TEXT_NODE_NAME_INNERLOC  "innerLoc"
#define  TEXT_NODE_NAME_AXIS      "axis"
#define  TEXT_NODE_NAME_ANGLE    "angle"
#define  TEXT_NODE_NAME_SIZE    "size"
#define  TEXT_NODE_NAME_CHARSPACE "charSpace"
#define  TEXT_NODE_NAME_USETYPE   "usetype"
#define  TEXT_NODE_NAME_OUTBOX   "outBox"
#define  TEXT_NODE_NAME_FONTNAME "fontName"
#define  TEXT_NODE_NAME_FONTSTYLE "fontStyle"
#define  TEXT_NODE_NAME_FONTTYPE  "fontType"
#define  TEXT_NODE_NAME_TEXTATTRIBUTE "textAttribute"
#define  TEXT_NODE_NAME_CONTENT    "content"

#define  ENVELOPE_NODE_NAME_STYLE "style"
#define  ENVELOPE_NODE_NAME_BACKGROUNDCOLOR "backgroundColor"
#define  ENVELOPE_NODE_NAME_FRAMECOLOR "frameColor"
#define  ENVELOPE_NODE_NAME_ANCHORS "anchors"
#define  ENVELOPE_NODE_NAME_ANCHORS_TYPE "type"
#define  ENVELOPE_NODE_NAME_AHCHORS_PNT "pnt"

namespace HoteamSoft {
	namespace SVLLib {

		class Stk_ComTextImpl : public Stk_ObjectImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_ComText)
		public:
			Stk_ComTextImpl(void);
			virtual ~Stk_ComTextImpl(void);
		public:
			STK_BOX32						m_boxBounding;                //∏¥∫œŒƒ±æ∞¸Œß∫–
			StkFrameTypeEnum				m_eFrameType;                 //Õ‚øÚ¿‡–Õ
			vector<Stk_TextPtr>				m_vcTexts;					  //Œƒ±æ
			Stk_EnvelopePtr					m_stkEnvelopeP;		          //∞¸¬ÁøÚ∂‘œÛ
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

		STK_STATUS	Stk_ComTextImpl::FromJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode)
				return STK_ERROR;
			
			string strValue;
			//outBox
			strValue = (*i_pJsonNode)[COMTEXT_NODE_NAME_OUTBOX].GetString();
			string strStartPoint, strEndPoint;
			int iIdxComma = strValue.find_first_of(',');
			strStartPoint = strValue.substr(0, iIdxComma);
			strEndPoint = strValue.substr(iIdxComma + 1, strValue.size() - iIdxComma - 1);
			
			std::vector<std::string> vecValue;
			CStringTools::StringSplit(strStartPoint, " ", vecValue);
			m_boxBounding.BoundBox[0][0] = atof(vecValue[0].c_str());
			m_boxBounding.BoundBox[0][1] = atof(vecValue[1].c_str());
			m_boxBounding.BoundBox[0][2] = atof(vecValue[2].c_str());
			vecValue.clear();
			CStringTools::StringSplit(strEndPoint, " ", vecValue);
			m_boxBounding.BoundBox[1][0] = atof(vecValue[0].c_str());
			m_boxBounding.BoundBox[1][1] = atof(vecValue[1].c_str());
			m_boxBounding.BoundBox[1][2] = atof(vecValue[2].c_str());
			vecValue.clear();

			m_eFrameType = (StkFrameTypeEnum)(*i_pJsonNode)[COMTEXT_NODE_NAME_FRAMETYPE].GetInt();

			//texts
			Value::MemberIterator textsNodeIter = i_pJsonNode->FindMember(COMTEXT_NODE_NAME_TEXTS);
			if (textsNodeIter != i_pJsonNode->MemberEnd())
			{
				Value texts = textsNodeIter->value.GetArray();
				Value text;
				for (int i = 0; i < texts.Size(); i++)
				{
					text = texts[i];
					Stk_TextPtr textP = Stk_TextPtr::CreateObject();
					textP->FromJson(&text, i_JsonDocP);
					m_vcTexts.push_back(textP);
				}
			}
			//envelope
			Value::MemberIterator envelopeIter = i_pJsonNode->FindMember(COMTEXT_NODE_NAME_ENVELOPE);
			if (envelopeIter != i_pJsonNode->MemberEnd())
			{
				m_stkEnvelopeP = Stk_EnvelopePtr::CreateObject();
				m_stkEnvelopeP->FromJson(&envelopeIter->value, i_JsonDocP);
			}
			return STK_SUCCESS;
		}

		STK_STATUS Stk_ComTextImpl::ToJson(Value* i_pJsonNode, Document* pDoc)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode || !pDoc)
				return STK_ERROR;
			i_pJsonNode->SetObject();
			i_pJsonNode->AddMember(COMTEXT_NODE_NAME_FRAMETYPE, m_eFrameType, pDoc->GetAllocator());
			//outBox
			string strValue;
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					string strOutBox;
					char szOutBox[256] = { 0 };
					if (!(i == 0 && j == 0))
						strValue += " ";
					sprintf(szOutBox, "%.8f", m_boxBounding.BoundBox[i][j]);
					strOutBox = szOutBox;
					strValue += strOutBox;
				}
				if(i == 0)
					strValue += ",";
			}
			i_pJsonNode->AddMember(COMTEXT_NODE_NAME_OUTBOX, strValue, pDoc->GetAllocator());


			//texts
			Value valTexts;
			valTexts.SetArray();

			vector<Stk_TextPtr>::iterator textItor = m_vcTexts.begin();
			while (textItor != m_vcTexts.end())
			{
				if (*textItor == NULL)
				{
					textItor++;
					continue;
				}
				Value valText;
				(*textItor)->ToJson(&valText, pDoc);
				valTexts.PushBack(valText, pDoc->GetAllocator());
				textItor++;
			}
			i_pJsonNode->AddMember(COMTEXT_NODE_NAME_TEXTS, valTexts, pDoc->GetAllocator());

			//envelope
			Value valEnvelope;
			valEnvelope.SetObject();

			if(m_stkEnvelopeP != nullptr)
				m_stkEnvelopeP->ToJson(&valEnvelope, pDoc);
			i_pJsonNode->AddMember(COMTEXT_NODE_NAME_ENVELOPE, valEnvelope, pDoc->GetAllocator());
			return ret;
		}

		Stk_ComTextImpl::Stk_ComTextImpl()
		{
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					m_boxBounding.BoundBox[i][j] = 0.0;
				}
			}
			m_eFrameType = TYPE_FRAME_UNKNOWN;
		}

		Stk_ComTextImpl::~Stk_ComTextImpl()
		{
			m_stkEnvelopeP = NULL;
			m_eFrameType = TYPE_FRAME_UNKNOWN;
		}

		STK_CLASS_IMPL_IMPLEMENT(Stk_ComText)

		Stk_ComText::Stk_ComText() : Stk_Object(new Stk_ComTextImpl())
		{
			
		}

		Stk_ComText::~Stk_ComText()
		{
			DELETE_STK_CLASS_IMPL
		}
		STK_IMPLEMENT_CLASS(Stk_ComText, Stk_Object)

		STK_STATUS	Stk_ComText::SetOuterBox(const STK_BOX32 &i_boxBounding)
		{
			((Stk_ComTextImpl*)m_pImpl)->m_boxBounding = i_boxBounding;
			return STK_SUCCESS;
		}

		STK_BOX32	Stk_ComText::GetOuterBox()
		{
			return ((Stk_ComTextImpl*)m_pImpl)->m_boxBounding;
		}

		STK_STATUS	Stk_ComText::SetFrameType(StkFrameTypeEnum i_eFrameType)
		{
			((Stk_ComTextImpl*)m_pImpl)->m_eFrameType = i_eFrameType;
			return STK_SUCCESS;
		}

		StkFrameTypeEnum Stk_ComText::GetFrameType()
		{
			return ((Stk_ComTextImpl*)m_pImpl)->m_eFrameType;
		}

		STK_STATUS Stk_ComText::SetEnvelope(Stk_EnvelopePtr i_stkEnvelopeP)
		{
			((Stk_ComTextImpl*)m_pImpl)->m_stkEnvelopeP = i_stkEnvelopeP;
			return STK_SUCCESS;
		}

		Stk_EnvelopePtr Stk_ComText::GetEnvelope()
		{
			return ((Stk_ComTextImpl*)m_pImpl)->m_stkEnvelopeP;
		}

		STK_SIZE_T Stk_ComText::GetNumTexts()
		{
			return ((Stk_ComTextImpl*)m_pImpl)->m_vcTexts.size();
		}

		vector<Stk_TextPtr> Stk_ComText::GetTexts()
		{
			return ((Stk_ComTextImpl*)m_pImpl)->m_vcTexts;
		}

		STK_STATUS Stk_ComText::SetTexts(const vector<Stk_TextPtr>& i_vcTexts)
		{
			((Stk_ComTextImpl*)m_pImpl)->m_vcTexts = i_vcTexts;
			return STK_SUCCESS;
		}

		STK_STATUS	Stk_ComText::ToJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_ComTextImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		STK_STATUS	Stk_ComText::FromJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_ComTextImpl*)m_pImpl)->FromJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		class Stk_TextImpl : public Stk_ObjectImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_Text)
		public:
			Stk_TextImpl(void);
			virtual ~Stk_TextImpl(void);
		public:
			STK_PNT32                           m_pntInnerLoc;                //∂®Œªµ„
			STK_DIR32                           m_dirAxis[2];                 //x y–˝◊™œÚ¡ø
			STK_DOUBLE64                        m_fRotationAngle;             //–˝◊™Ω«∂»
			STK_FLOAT32                         m_fCharSize[2];               //◊÷∑˚øÌ∏ﬂ
			STK_FLOAT32                         m_fCharSpace;                   //◊÷∑˚º‰∏Ù
			wstring                             m_wstrFontName;               //◊÷ÃÂ√˚≥∆	
			wstring                             m_wstrTextContent;            //◊÷∑˚¥Æƒ⁄»›
			StkTextTypeEnum					    m_eUsageType;                 //Œƒ±æµƒ”¶”√¿‡–Õ
			STK_BOX32						    m_boxBounding;                //∏¥∫œŒƒ±æøÚ
			StkFontStyleTypeEnum			    m_eFontStyle;                 //◊÷ÃÂ∑Á∏Ò
			StkFontTypeEnum					    m_eFontType;                  //◊÷ÃÂ¿‡–Õ
			StkTextAttributeTypeEnum		    m_eTextAttribute;             //Œƒ±æ Ù–‘
			vector<Stk_OutFramePtr>			    m_vcOutFrames;		          //Õ‚øÚ
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

		Stk_TextImpl::Stk_TextImpl(void)
		{
			m_pntInnerLoc.X = 0.0;
			m_pntInnerLoc.Y = 0.0;
			m_pntInnerLoc.Z = 0.0;
			m_dirAxis[0].X = 0.0;
			m_dirAxis[0].Y = 0.0;
			m_dirAxis[0].Z = 0.0;
			m_dirAxis[1].X = 0.0;
			m_dirAxis[1].Y = 0.0;
			m_dirAxis[1].Z = 0.0;
			m_fCharSize[0] = 0.0;
			m_fCharSize[1] = 0.0;
			m_fCharSpace = 0.0f;
			m_wstrFontName = L"";
			m_wstrTextContent = L"";
			m_eUsageType = text_type_value;
			m_fRotationAngle = 0.0;
			m_eFontStyle = TYPE_STYLE_REGULAR;
			m_eFontType = TYPE_FONT_UNKNOWN;
			m_eTextAttribute = TYPE_ATTRIBUTE_NONE;
		}

		Stk_TextImpl::~Stk_TextImpl(void)
		{

		}

		STK_STATUS	Stk_TextImpl::FromJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode)
				return STK_ERROR;
			
			string strValue;
			std::vector<std::string> vecValue;
			//innerLoc
			Value::MemberIterator itorInnerLoc = i_pJsonNode->FindMember(TEXT_NODE_NAME_INNERLOC);
			if (itorInnerLoc != i_pJsonNode->MemberEnd())
			{
				strValue = itorInnerLoc->value.GetString();
				CStringTools::StringSplit(strValue, " ", vecValue);
				m_pntInnerLoc.X = atof(vecValue[0].c_str());
				m_pntInnerLoc.Y = atof(vecValue[1].c_str());
				m_pntInnerLoc.Z = atof(vecValue[2].c_str());
				vecValue.clear();
			}
			//axis
			Value::MemberIterator itorAxis = i_pJsonNode->FindMember(TEXT_NODE_NAME_AXIS);
			if (itorAxis != i_pJsonNode->MemberEnd())
			{
				strValue = itorAxis->value.GetString();
				CStringTools::StringSplit(strValue, " ", vecValue);
				m_dirAxis[0].X = atof(vecValue[0].c_str());
				m_dirAxis[0].Y = atof(vecValue[1].c_str());
				m_dirAxis[0].Z = atof(vecValue[2].c_str());
				m_dirAxis[1].X = atof(vecValue[3].c_str());
				m_dirAxis[1].Y = atof(vecValue[4].c_str());
				m_dirAxis[1].Z = atof(vecValue[5].c_str());
				vecValue.clear();
			}
			//angle
			Value::MemberIterator itorAngle = i_pJsonNode->FindMember(TEXT_NODE_NAME_ANGLE);
			if (itorAngle != i_pJsonNode->MemberEnd())
			{
				m_fRotationAngle = itorAngle->value.GetDouble();
			}
			//size
			vecValue.clear();
			Value::MemberIterator itorSize = i_pJsonNode->FindMember(TEXT_NODE_NAME_SIZE);
			if (itorSize != i_pJsonNode->MemberEnd())
			{
				strValue = itorSize->value.GetString();
				CStringTools::StringSplit(strValue, " ", vecValue);
				m_fCharSize[0] = atof(vecValue[0].c_str());
				m_fCharSize[1] = atof(vecValue[1].c_str());
			}
			//charSpace
			Value::MemberIterator itorCharSpace = i_pJsonNode->FindMember(TEXT_NODE_NAME_CHARSPACE);
			if (itorCharSpace != i_pJsonNode->MemberEnd())
			{
				m_fCharSpace = itorCharSpace->value.GetFloat();
			}
			//usetype
			Value::MemberIterator itorUseType = i_pJsonNode->FindMember(TEXT_NODE_NAME_USETYPE);
			if (itorUseType != i_pJsonNode->MemberEnd())
			{
				m_eUsageType = (StkTextTypeEnum)itorUseType->value.GetInt();
			}
			//outBox
			vecValue.clear();
			Value::MemberIterator itorOutBox = i_pJsonNode->FindMember(TEXT_NODE_NAME_OUTBOX);
			if (itorOutBox != i_pJsonNode->MemberEnd())
			{
				strValue = itorOutBox->value.GetString();
				CStringTools::StringSplit(strValue, ",", vecValue);
				std::vector<std::string> vecOutBoxValue;
				CStringTools::StringSplit(vecValue[0], " ", vecOutBoxValue);
				m_boxBounding.BoundBox[0][0] = atof(vecOutBoxValue[0].c_str());
				m_boxBounding.BoundBox[0][1] = atof(vecOutBoxValue[1].c_str());
				m_boxBounding.BoundBox[0][2] = atof(vecOutBoxValue[2].c_str());
				vecOutBoxValue.clear();
				CStringTools::StringSplit(vecValue[1], " ", vecOutBoxValue);
				m_boxBounding.BoundBox[1][0] = atof(vecOutBoxValue[0].c_str());
				m_boxBounding.BoundBox[1][1] = atof(vecOutBoxValue[1].c_str());
				m_boxBounding.BoundBox[1][2] = atof(vecOutBoxValue[2].c_str());
				vecOutBoxValue.clear();
			}
			//fontName
			Value::MemberIterator itorFontName = i_pJsonNode->FindMember(TEXT_NODE_NAME_FONTNAME);
			if (itorFontName != i_pJsonNode->MemberEnd())
			{
				 m_wstrFontName = ChineseCode::UTF_8ToUnicode(itorFontName->value.GetString());
			}
			//fontStyle
			Value::MemberIterator itorFontStyle = i_pJsonNode->FindMember(TEXT_NODE_NAME_FONTSTYLE);
			if (itorFontStyle != i_pJsonNode->MemberEnd())
			{
				m_eFontStyle = (StkFontStyleTypeEnum)itorFontStyle->value.GetInt();
			}
			//fontType
			Value::MemberIterator itorFontType = i_pJsonNode->FindMember(TEXT_NODE_NAME_FONTTYPE);
			if (itorFontType != i_pJsonNode->MemberEnd())
			{
				m_eFontType = (StkFontTypeEnum)itorFontType->value.GetInt();
			}
			//textAttribute
			Value::MemberIterator itorTextAttribute = i_pJsonNode->FindMember(TEXT_NODE_NAME_TEXTATTRIBUTE);
			if (itorTextAttribute != i_pJsonNode->MemberEnd())
			{
				m_eTextAttribute = (StkTextAttributeTypeEnum)itorTextAttribute->value.GetInt();
			}
			//content
			Value::MemberIterator itorContent = i_pJsonNode->FindMember(TEXT_NODE_NAME_CONTENT);
			if (itorContent != i_pJsonNode->MemberEnd())
			{
				m_wstrTextContent = ChineseCode::UTF_8ToUnicode(itorContent->value.GetString());
			}
			return ret;
		}

		STK_STATUS	Stk_TextImpl::ToJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode || !i_JsonDocP)
				return STK_ERROR;
			i_pJsonNode->SetObject();
			
			string strValue;
			char szValue[256] = { 0 };
			//innerLoc
			sprintf(szValue, "%.8f", m_pntInnerLoc.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntInnerLoc.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntInnerLoc.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(TEXT_NODE_NAME_INNERLOC, strValue, i_JsonDocP->GetAllocator());
			//axis
			strValue.clear();
			sprintf(szValue, "%.8f", m_dirAxis[0].X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirAxis[0].Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirAxis[0].Z);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirAxis[1].X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirAxis[1].Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirAxis[1].Z);
			strValue += szValue;
			i_pJsonNode->AddMember(TEXT_NODE_NAME_AXIS, strValue, i_JsonDocP->GetAllocator());
			//angle
#ifdef __MOBILE__
			if (isnan(m_fRotationAngle))
				m_fRotationAngle = 0.0;
#else
			if (_isnan(m_fRotationAngle))
				m_fRotationAngle = 0.0;
#endif
			i_pJsonNode->AddMember(TEXT_NODE_NAME_ANGLE, m_fRotationAngle, i_JsonDocP->GetAllocator());
			//size
			strValue.clear();
			sprintf(szValue, "%.8f", m_fCharSize[0]);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_fCharSize[1]);
			strValue += szValue;
			i_pJsonNode->AddMember(TEXT_NODE_NAME_SIZE, strValue, i_JsonDocP->GetAllocator());
			//charSpace
			Value valCharSpace;
			valCharSpace.SetObject();
			i_pJsonNode->AddMember(TEXT_NODE_NAME_CHARSPACE, m_fCharSpace, i_JsonDocP->GetAllocator());
			//usetype
			i_pJsonNode->AddMember(TEXT_NODE_NAME_USETYPE, m_eUsageType, i_JsonDocP->GetAllocator());
			//outBox
			strValue.clear();
			sprintf(szValue, "%.8f", m_boxBounding.BoundBox[0][0]);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_boxBounding.BoundBox[0][1]);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_boxBounding.BoundBox[0][2]);
			strValue += szValue;
			strValue += ",";
			sprintf(szValue, "%.8f", m_boxBounding.BoundBox[1][0]);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_boxBounding.BoundBox[1][1]);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_boxBounding.BoundBox[1][2]);
			strValue += szValue;
			i_pJsonNode->AddMember(TEXT_NODE_NAME_OUTBOX, strValue, i_JsonDocP->GetAllocator());
			//fontName
			ChineseCode::UnicodeToUTF_8(m_wstrFontName.c_str(), strValue);
			i_pJsonNode->AddMember(TEXT_NODE_NAME_FONTNAME, strValue, i_JsonDocP->GetAllocator());
			//fontStyle
			i_pJsonNode->AddMember(TEXT_NODE_NAME_FONTSTYLE, m_eFontStyle, i_JsonDocP->GetAllocator());
			//fontType
			i_pJsonNode->AddMember(TEXT_NODE_NAME_FONTTYPE, m_eFontType, i_JsonDocP->GetAllocator());
			//content
			ChineseCode::UnicodeToUTF_8(m_wstrTextContent.c_str(), strValue);
			i_pJsonNode->AddMember(TEXT_NODE_NAME_CONTENT, strValue, i_JsonDocP->GetAllocator());
			//textAttribute
			i_pJsonNode->AddMember(TEXT_NODE_NAME_TEXTATTRIBUTE, m_eTextAttribute, i_JsonDocP->GetAllocator());
			return ret;
		}

		STK_CLASS_IMPL_IMPLEMENT(Stk_Text)

		Stk_Text::Stk_Text() : Stk_Object(new Stk_TextImpl())
		{
			
		}

		Stk_Text::~Stk_Text()
		{
			DELETE_STK_CLASS_IMPL
		}

		STK_IMPLEMENT_CLASS(Stk_Text, Stk_Object)

		STK_PNT32 Stk_Text::GetInnerLoc()
		{
			return ((Stk_TextImpl*)m_pImpl)->m_pntInnerLoc;
		}

		STK_STATUS Stk_Text::SetInnerLoc(const STK_PNT32& i_pntLocation)
		{
			((Stk_TextImpl*)m_pImpl)->m_pntInnerLoc = i_pntLocation;
			return STK_SUCCESS;
		}

		void Stk_Text::GetInnerXYAxis(STK_DIR32& o_dirXAxis, STK_DIR32& o_dirYAxis)
		{
			o_dirXAxis = ((Stk_TextImpl*)m_pImpl)->m_dirAxis[0];
			o_dirYAxis = ((Stk_TextImpl*)m_pImpl)->m_dirAxis[1];
		}

		STK_STATUS Stk_Text::SetInnerXYAxis(const STK_DIR32& i_dirXAxis, const STK_DIR32& i_dirYAxis)
		{
			((Stk_TextImpl*)m_pImpl)->m_dirAxis[0] = i_dirXAxis;
			((Stk_TextImpl*)m_pImpl)->m_dirAxis[1] = i_dirYAxis;
			return STK_SUCCESS;
		}

		STK_STATUS	Stk_Text::SetRotationAngle(STK_DOUBLE64 i_fRotationAngle)
		{
			((Stk_TextImpl*)m_pImpl)->m_fRotationAngle = i_fRotationAngle;
			return STK_SUCCESS;
		}

		STK_DOUBLE64 Stk_Text::GetRotationAngle()
		{
			return ((Stk_TextImpl*)m_pImpl)->m_fRotationAngle;
		}

		void Stk_Text::GetCharWidthAndHeight(STK_FLOAT32& o_fWidth, STK_FLOAT32& o_fHeight)
		{
			o_fWidth = ((Stk_TextImpl*)m_pImpl)->m_fCharSize[0];
			o_fHeight = ((Stk_TextImpl*)m_pImpl)->m_fCharSize[1];
		}

		STK_STATUS Stk_Text::SetCharWidthAndHeight(const STK_FLOAT32& i_fWidth, const STK_FLOAT32& i_fHeight)
		{
			((Stk_TextImpl*)m_pImpl)->m_fCharSize[0] = i_fWidth;
			((Stk_TextImpl*)m_pImpl)->m_fCharSize[1] = i_fHeight;
			return STK_SUCCESS;
		}

		STK_FLOAT32	Stk_Text::GetCharSpace()
		{
			return ((Stk_TextImpl*)m_pImpl)->m_fCharSpace;
		}

		STK_STATUS	Stk_Text::SetCharSpace(const STK_FLOAT32& i_fCharSpace)
		{
			((Stk_TextImpl*)m_pImpl)->m_fCharSpace = i_fCharSpace;
			return STK_SUCCESS;
		}

		wstring	Stk_Text::GetFontName()
		{
			return ((Stk_TextImpl*)m_pImpl)->m_wstrFontName;
		}

		void Stk_Text::SetFontName(wstring i_wstrFontName)
		{
			((Stk_TextImpl*)m_pImpl)->m_wstrFontName = i_wstrFontName;
		}

		wstring	Stk_Text::GetTextContent()
		{
			return ((Stk_TextImpl*)m_pImpl)->m_wstrTextContent;
		}

		STK_STATUS	Stk_Text::SetTextContent(wstring i_wstrText)
		{
			((Stk_TextImpl*)m_pImpl)->m_wstrTextContent = i_wstrText;
			return STK_SUCCESS;
		}

		void Stk_Text::SetUsageType(StkTextTypeEnum i_Type)
		{
			((Stk_TextImpl*)m_pImpl)->m_eUsageType = i_Type;
		}

		StkTextTypeEnum	Stk_Text::GetUsageType()
		{
			return ((Stk_TextImpl*)m_pImpl)->m_eUsageType;
		}

		STK_STATUS	Stk_Text::SetOuterBox(const STK_BOX32 &i_boxBounding)
		{
			((Stk_TextImpl*)m_pImpl)->m_boxBounding = i_boxBounding;
			return STK_SUCCESS;
		}
		
		STK_BOX32 Stk_Text::GetOuterBox()
		{
			return ((Stk_TextImpl*)m_pImpl)->m_boxBounding;
		}

		StkFontStyleTypeEnum Stk_Text::GetFontStyle()
		{
			return ((Stk_TextImpl*)m_pImpl)->m_eFontStyle;
		}

		StkFontTypeEnum Stk_Text::GetFontType()
		{
			return ((Stk_TextImpl*)m_pImpl)->m_eFontType;
		}

		STK_STATUS Stk_Text::SetFontType(StkFontTypeEnum i_eFontType)
		{
			((Stk_TextImpl*)m_pImpl)->m_eFontType = i_eFontType;
			return STK_SUCCESS;
		}

		STK_STATUS	Stk_Text::SetAttributeType(StkTextAttributeTypeEnum i_eAttributeType)
		{
			((Stk_TextImpl*)m_pImpl)->m_eTextAttribute = i_eAttributeType;
			return STK_SUCCESS;
		}

		StkTextAttributeTypeEnum Stk_Text::GetAttributeType()
		{
			return ((Stk_TextImpl*)m_pImpl)->m_eTextAttribute;
		}

		STK_UINT32 Stk_Text::GetOutFrameNum()
		{
			return ((Stk_TextImpl*)m_pImpl)->m_vcOutFrames.size();
		}

		Stk_OutFramePtr Stk_Text::GetOutFrameByIndex(STK_UINT32 i_nuKeyPos)
		{
			Stk_OutFramePtr stkOutFrameP = NULL;

			if (i_nuKeyPos >= 0 && i_nuKeyPos < ((Stk_TextImpl*)m_pImpl)->m_vcOutFrames.size())
			{
				stkOutFrameP = ((Stk_TextImpl*)m_pImpl)->m_vcOutFrames[i_nuKeyPos];
			}

			return stkOutFrameP;
		}

		STK_STATUS	Stk_Text::ToJson(void* i_DataP, void* pDoc)
		{
			return ((Stk_TextImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)pDoc);
		}
		
		STK_STATUS	Stk_Text::FromJson(void* i_DataP, void* i_JsonDocP)				
		{
			return ((Stk_TextImpl*)m_pImpl)->FromJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		STK_STATUS Stk_Text::AddOutFrame(Stk_OutFramePtr i_OutFrameP)
		{
			// …Ë÷√∂®“Â¿‡–Õ
			i_OutFrameP->SetDefineType(STK_FRAME_DEF_TEXT);

			// ≤Â»Î ˝◊È
			if (i_OutFrameP != NULL)
			{
				((Stk_TextImpl*)m_pImpl)->m_vcOutFrames.push_back(i_OutFrameP);
			}
			else
			{
				return STK_ERROR;
			}
			return STK_SUCCESS;
		}

		class Stk_EnvelopeImpl : public Stk_ObjectImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_Envelope)
		public:
			Stk_EnvelopeImpl(void);
			virtual ~Stk_EnvelopeImpl(void);
		public:
			STK_RGBA32							m_rgbaBackground;    //∞¸¬ÁøÚ±≥æ∞…´
			STK_RGBA32							m_rgbaFrame;         //∞¸¬ÁøÚÕ‚øÚ
			STK_INT32                            m_iStyle;         // «∑Ò∫¨”–∞¸¬ÁøÚ±ﬂøÚ
			map<StkAnchorTypeEnum, STK_PNT32>	m_mapAnchorPoints;   //√™µ„
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

		STK_STATUS	Stk_EnvelopeImpl::FromJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode)
				return STK_ERROR;
			//style
			Value::MemberIterator itorStyle = i_pJsonNode->FindMember(ENVELOPE_NODE_NAME_STYLE);
			if (itorStyle != i_pJsonNode->MemberEnd())
			{
				m_iStyle = itorStyle->value.GetInt();
			}
			//background
			std::vector<std::string> vecValue;
			string strValue;
			Value::MemberIterator itorBackgroundColor = i_pJsonNode->FindMember(ENVELOPE_NODE_NAME_BACKGROUNDCOLOR);
			if (itorBackgroundColor != i_pJsonNode->MemberEnd())
			{
				strValue = itorBackgroundColor->value.GetString();
				CStringTools::StringSplit(strValue, " ", vecValue);
				m_rgbaBackground.Red = atof(vecValue[0].c_str());
				m_rgbaBackground.Green = atof(vecValue[1].c_str());
				m_rgbaBackground.Blue = atof(vecValue[2].c_str());
				m_rgbaBackground.Alpha = atof(vecValue[3].c_str());
			}
			//frameColor
			vecValue.clear();
			Value::MemberIterator itorFrameColor = i_pJsonNode->FindMember(ENVELOPE_NODE_NAME_FRAMECOLOR);
			if (itorFrameColor != i_pJsonNode->MemberEnd())
			{
				strValue = itorFrameColor->value.GetString();
				CStringTools::StringSplit(strValue, " ", vecValue);
				m_rgbaFrame.Red = atof(vecValue[0].c_str());
				m_rgbaFrame.Green = atof(vecValue[1].c_str());
				m_rgbaFrame.Blue = atof(vecValue[2].c_str());
				m_rgbaFrame.Alpha = atof(vecValue[3].c_str());
			}
			//anchors
			vecValue.clear();
			Value::MemberIterator itorAnchors = i_pJsonNode->FindMember(ENVELOPE_NODE_NAME_ANCHORS);
			if (itorAnchors != i_pJsonNode->MemberEnd())
			{
				Value anchors = itorAnchors->value.GetArray();
				Value anchor;
				for (int i = 0; i < anchors.Size(); i++)
				{
					//map<StkAnchorTypeEnum, STK_PNT32>	m_mapAnchorPoints;
					anchor = anchors[i];
					StkAnchorTypeEnum anchorType = (StkAnchorTypeEnum)anchor[ENVELOPE_NODE_NAME_ANCHORS_TYPE].GetInt();
					string strPnt = anchor[ENVELOPE_NODE_NAME_AHCHORS_PNT].GetString();
					CStringTools::StringSplit(strPnt, " ", vecValue);
					STK_PNT32 pnt;
					pnt.X = atof(vecValue[0].c_str());
					pnt.Y = atof(vecValue[1].c_str());
					pnt.Z = atof(vecValue[2].c_str());
					m_mapAnchorPoints.insert(make_pair(anchorType, pnt));
				}
			}
			return ret;
		}

		STK_STATUS	Stk_EnvelopeImpl::ToJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode)
				return STK_ERROR;
			i_pJsonNode->SetObject();
			//style
			i_pJsonNode->AddMember(ENVELOPE_NODE_NAME_STYLE, m_iStyle, i_JsonDocP->GetAllocator());
			string strValue;
			char szValue[256] = { 0 };
			//backgroundColor
			sprintf(szValue, "%.8f", m_rgbaBackground.Red);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_rgbaBackground.Green);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_rgbaBackground.Blue);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_rgbaBackground.Alpha);
			strValue += szValue;
			i_pJsonNode->AddMember(ENVELOPE_NODE_NAME_BACKGROUNDCOLOR, strValue, i_JsonDocP->GetAllocator());
			//frameColor
			strValue.clear();
			sprintf(szValue, "%.8f", m_rgbaFrame.Red);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_rgbaFrame.Green);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_rgbaFrame.Blue);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_rgbaFrame.Alpha);
			strValue += szValue;
			i_pJsonNode->AddMember(ENVELOPE_NODE_NAME_FRAMECOLOR, strValue, i_JsonDocP->GetAllocator());
			//anchors
			Value valAnchors;
			valAnchors.SetArray();
			map<StkAnchorTypeEnum, STK_PNT32>::iterator itorMap = m_mapAnchorPoints.begin();
			while (itorMap != m_mapAnchorPoints.end())
			{
				Value valAnchor;
				valAnchor.SetObject();
				valAnchor.AddMember(ENVELOPE_NODE_NAME_ANCHORS_TYPE, itorMap->first, i_JsonDocP->GetAllocator());
				strValue.clear();
				sprintf(szValue, "%.8f", itorMap->second.X);
				strValue += szValue;
				strValue += " ";
				sprintf(szValue, "%.8f", itorMap->second.Y);
				strValue += szValue;
				strValue += " ";
				sprintf(szValue, "%.8f", itorMap->second.Z);
				strValue += szValue;
				valAnchor.AddMember(ENVELOPE_NODE_NAME_AHCHORS_PNT, strValue, i_JsonDocP->GetAllocator());
				valAnchors.PushBack(valAnchor, i_JsonDocP->GetAllocator());
				itorMap++;
			}
			if(m_mapAnchorPoints.size() != 0)
				i_pJsonNode->AddMember(ENVELOPE_NODE_NAME_ANCHORS, valAnchors, i_JsonDocP->GetAllocator());
			return ret;
		}

		STK_CLASS_IMPL_IMPLEMENT(Stk_Envelope)
		Stk_EnvelopeImpl::Stk_EnvelopeImpl()
		{
			m_iStyle = 0;
		}

		Stk_EnvelopeImpl::~Stk_EnvelopeImpl()
		{
			
		}

		STK_IMPLEMENT_CLASS(Stk_Envelope, Stk_Object)

		Stk_Envelope::Stk_Envelope() : Stk_Object(new Stk_EnvelopeImpl())
		{

		}

		Stk_Envelope::~Stk_Envelope()
		{
			DELETE_STK_CLASS_IMPL
		}

		STK_STATUS	Stk_Envelope::SetBackgroundColor(STK_RGBA32 i_rgbaBackground)
		{
			((Stk_EnvelopeImpl*)m_pImpl)->m_rgbaBackground = i_rgbaBackground;
			return STK_SUCCESS;
		}

		STK_RGBA32 Stk_Envelope::GetBackgroundColor()
		{
			return ((Stk_EnvelopeImpl*)m_pImpl)->m_rgbaBackground;
		}

		STK_STATUS	Stk_Envelope::SetFrameColor(STK_RGBA32 i_rgbaFrame)
		{
			((Stk_EnvelopeImpl*)m_pImpl)->m_rgbaFrame = i_rgbaFrame;
			return STK_SUCCESS;
		}

		STK_RGBA32 Stk_Envelope::GetFrameColor()
		{
			return ((Stk_EnvelopeImpl*)m_pImpl)->m_rgbaFrame;
		}

		STK_STATUS	Stk_Envelope::SetStyle(STK_INT32 i_iStyle)
		{
			((Stk_EnvelopeImpl*)m_pImpl)->m_iStyle = i_iStyle;
			return STK_SUCCESS;
		}

		STK_BOOL Stk_Envelope::GetStyle()
		{
			return ((Stk_EnvelopeImpl*)m_pImpl)->m_iStyle;
		}

		STK_STATUS Stk_Envelope::SetAnchorPoint(StkAnchorTypeEnum i_eType, STK_PNT32 i_pntAnchor)
		{
			if (i_eType != TYPE_ANCHOR_NUMBER)
			{
				map<StkAnchorTypeEnum, STK_PNT32>::iterator itor = ((Stk_EnvelopeImpl*)m_pImpl)->m_mapAnchorPoints.find(i_eType);
				if (itor != ((Stk_EnvelopeImpl*)m_pImpl)->m_mapAnchorPoints.end())
				{
					((Stk_EnvelopeImpl*)m_pImpl)->m_mapAnchorPoints.erase(itor);
				}
				((Stk_EnvelopeImpl*)m_pImpl)->m_mapAnchorPoints.insert(pair<StkAnchorTypeEnum, STK_PNT32>(i_eType, i_pntAnchor));
				return STK_SUCCESS;
			}
			return STK_ERROR;
		}

		vector<StkAnchorTypeEnum>& Stk_Envelope::GetAllAnchorTypes()
		{
			vector<StkAnchorTypeEnum>	vcAnchorTypes;
			map<StkAnchorTypeEnum, STK_PNT32>::iterator itor = ((Stk_EnvelopeImpl*)m_pImpl)->m_mapAnchorPoints.begin();
			while (itor != ((Stk_EnvelopeImpl*)m_pImpl)->m_mapAnchorPoints.end())
			{
				vcAnchorTypes.push_back(itor->first);
				itor++;
			}
			return vcAnchorTypes;
		}

		STK_STATUS Stk_Envelope::GetAnchorPointByType(StkAnchorTypeEnum i_eType, STK_PNT32& o_pntAnchor)
		{
			map<StkAnchorTypeEnum, STK_PNT32>::iterator itor = ((Stk_EnvelopeImpl*)m_pImpl)->m_mapAnchorPoints.begin();
			while (itor != ((Stk_EnvelopeImpl*)m_pImpl)->m_mapAnchorPoints.end())
			{
				if (itor->first == i_eType)
				{
					o_pntAnchor = itor->second;
					return STK_SUCCESS;
				}
				itor++;
			}
			return STK_ERROR_ANCHOR_NOT_FOUND;
		}

		STK_STATUS	Stk_Envelope::ToJson(void* i_DataP, void* pDoc)
		{
			return ((Stk_EnvelopeImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)pDoc);
		}

		STK_STATUS	Stk_Envelope::FromJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_EnvelopeImpl*)m_pImpl)->FromJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}
	}
}
