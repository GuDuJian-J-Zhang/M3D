// All Rights Reserved. Copyright (C) 2017 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file	Stk_Image.cpp
*
*	@brief	ÕºœÒ¿‡
*
*	@par	¿˙ ∑:
*		2017/06/10	¥¥Ω®°£qinyp@HOTEAMSOFT
****************************************************************************/

#include "Stk_Image.h"
#include "Stk_ObjectImpl.h"
#include "ChineseCode.h"
#include "StringTools.h"
#include "Stk_BaseDocumentImpl.h"

#define IMAGE_NODE_NAME_TYPE      "type"
#define IMAGE_NODE_NAME_LOCATION  "location"
#define IMAGE_NODE_NAME_DISPLAYSIZE "displaySize"
#define IMAGE_NODE_NAME_ACTUALSIZE  "actualSize"
#define IMAGE_NODE_NAME_SCALEX     "scaleX"
#define IMAGE_NODE_NAME_SCALEY     "scaleY"
#define IMAGE_NODE_NAME_NC        "nc"
#define IMAGE_NODE_NAME_SAVESTYLE  "saveStyle"
#define IMAGE_NODE_NAME_IMAGEPATH  "imagePath"
#define IMAGE_NODE_NAME_IMAGEDATASZIE "imageDataSize"
#define IMAGE_NODE_NAME_IMAGEDATA    "imageData"


namespace HoteamSoft {
	namespace SVLLib {
        
        class Stk_ImageImpl : public Stk_ObjectImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_Image)
		public:
			Stk_ImageImpl(void);
			virtual ~Stk_ImageImpl(void);
		public:
			StkPictureTypeEnum	            m_eType;            /*Õº∆¨¿‡–Õ*/
			STK_PNT32						m_pntLocation;      /*Õº∆¨Œª÷√*/
			STK_FLOAT32						m_fDisplayWidth;	//Õº∆¨≥°æ∞Õºœ‘ æøÌ
			STK_FLOAT32						m_fDisplayHeight;	//Õº∆¨≥°æ∞Õºœ‘ æ∏ﬂ
			STK_FLOAT32						m_fActualWidth;		//Õº∆¨ µº øÌ
			STK_FLOAT32					    m_fActualHeight;	//Õº∆¨ µº ∏ﬂ
			StkPicSaveStyleEnum				m_eSaveStyle;       //¥Ê¥¢∑Ω Ω
			STK_INT32						m_nNC;				//Õº∆¨ŒªÕº
			wstring							m_wstrPath;			//¥Ê¥¢¬∑æ∂
			STK_UINT32						m_uiDataSize;       // ˝æ›¡˜¥Û–°
			STK_CHAR						*m_pszData;			//Õº∆¨ ˝æ›
			STK_FLOAT32						m_fScaleX;			//X∑ΩœÚÀı∑≈“Ú◊”
			STK_FLOAT32						m_fScaleY;			//Y∑ΩœÚÀı∑≈“Ú◊”
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
			STK_STATUS					FromJson(Value* i_pJsonNode);

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

		STK_STATUS	Stk_ImageImpl::FromJson(Value* i_pJsonNode)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode)
				return STK_ERROR;
			m_eType = (StkPictureTypeEnum)(*i_pJsonNode)[IMAGE_NODE_NAME_TYPE].GetInt();
			wstring wstrValue;
			string strValue;
			//location
			strValue = (*i_pJsonNode)[IMAGE_NODE_NAME_LOCATION].GetString();
			std::vector<std::string> vecValue;
			CStringTools::StringSplit(strValue, " ", vecValue);
			m_pntLocation.X = atof(vecValue[0].c_str());
			m_pntLocation.Y = atof(vecValue[1].c_str());
			m_pntLocation.Z = atof(vecValue[2].c_str());
			vecValue.clear();
			
			//displaySize
			strValue = (*i_pJsonNode)[IMAGE_NODE_NAME_DISPLAYSIZE].GetString();
			CStringTools::StringSplit(strValue, " ", vecValue);
			m_fDisplayWidth = atoi(vecValue[0].c_str());
			m_fDisplayHeight = atoi(vecValue[1].c_str());
			vecValue.clear();

			//actualSize
			strValue = (*i_pJsonNode)[IMAGE_NODE_NAME_ACTUALSIZE].GetString();
			CStringTools::StringSplit(strValue, " ", vecValue);
			m_fActualWidth = atoi(vecValue[0].c_str());
			m_fDisplayHeight = atoi(vecValue[1].c_str());
			vecValue.clear();

			//scaleX scaleY
			m_fScaleX = (*i_pJsonNode)[IMAGE_NODE_NAME_SCALEX].GetFloat();
			m_fScaleY = (*i_pJsonNode)[IMAGE_NODE_NAME_SCALEY].GetFloat();
			//NC
			m_nNC = (*i_pJsonNode)[IMAGE_NODE_NAME_NC].GetInt();
			//saveStyle
			m_eSaveStyle = (StkPicSaveStyleEnum)(*i_pJsonNode)[IMAGE_NODE_NAME_SAVESTYLE].GetInt();
			//imagePath
			m_wstrPath= ChineseCode::StringToWString((*i_pJsonNode)[IMAGE_NODE_NAME_IMAGEPATH].GetString());
			//imageDataSize
			m_uiDataSize = (*i_pJsonNode)[IMAGE_NODE_NAME_IMAGEDATASZIE].GetInt();
			//imageData
			m_pszData = (STK_CHAR*)(*i_pJsonNode)[IMAGE_NODE_NAME_IMAGEDATA].GetString();
            return ret;
		}

		STK_STATUS	Stk_ImageImpl::ToJson(Value* i_pJsonNode, Document* pDoc)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode || !pDoc)
				return STK_ERROR;
			i_pJsonNode->SetObject();
			i_pJsonNode->AddMember(IMAGE_NODE_NAME_TYPE, m_eType, pDoc->GetAllocator());
			string strValue;
			char szValue[256] = { 0 };
			//location
			sprintf(szValue, "%.8f", m_pntLocation.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntLocation.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntLocation.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(IMAGE_NODE_NAME_LOCATION, strValue, pDoc->GetAllocator());
			//displaySize
			strValue = "";
			sprintf(szValue, "%.8f", m_fDisplayWidth);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_fDisplayHeight);
			strValue += szValue;
			i_pJsonNode->AddMember(IMAGE_NODE_NAME_DISPLAYSIZE, strValue, pDoc->GetAllocator());
			//actualSize
			strValue = "";
			sprintf(szValue,  "%.8f", m_fActualWidth);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_fActualHeight);
			strValue += szValue;
			i_pJsonNode->AddMember(IMAGE_NODE_NAME_ACTUALSIZE, strValue, pDoc->GetAllocator());
			//scaleX
			i_pJsonNode->AddMember(IMAGE_NODE_NAME_SCALEX, m_fScaleX, pDoc->GetAllocator());
			//scaleY
			i_pJsonNode->AddMember(IMAGE_NODE_NAME_SCALEY, m_fScaleY, pDoc->GetAllocator());
			//NC
			i_pJsonNode->AddMember(IMAGE_NODE_NAME_NC, m_nNC, pDoc->GetAllocator());
			//saveStyle
			i_pJsonNode->AddMember(IMAGE_NODE_NAME_SAVESTYLE, m_eSaveStyle, pDoc->GetAllocator());
			//imagePath
			strValue = ChineseCode::WStringToString(m_wstrPath.c_str());
			i_pJsonNode->AddMember(IMAGE_NODE_NAME_IMAGEPATH, strValue, pDoc->GetAllocator());
			//imageDataSize
			i_pJsonNode->AddMember(IMAGE_NODE_NAME_IMAGEDATASZIE, m_uiDataSize, pDoc->GetAllocator());
			//imageData
			i_pJsonNode->AddMember(IMAGE_NODE_NAME_IMAGEDATA, string(m_pszData), pDoc->GetAllocator());
			return ret;
		}

		STK_CLASS_IMPL_IMPLEMENT(Stk_Image)

		Stk_ImageImpl::Stk_ImageImpl(void)
		{
			m_eType = PICTURE_TYPE_UNKNOWN;
			m_pntLocation.X = 0;
			m_pntLocation.Y = 0;
			m_pntLocation.Z = 0;
			m_fDisplayWidth = 0;
			m_fDisplayHeight = 0;
			m_fActualWidth = 0;
			m_fActualHeight = 0;
			m_eSaveStyle = PICTRE_SAVE_STYLE_OUTER;
			m_nNC = 0;
			m_wstrPath = L"";
			m_uiDataSize = 0;
			m_pszData = NULL;
		}

		Stk_ImageImpl::~Stk_ImageImpl(void)
		{

		}

		STK_IMPLEMENT_CLASS(Stk_Image, Stk_Object)

		Stk_Image::Stk_Image() : Stk_Object(new Stk_ImageImpl())
		{
			
		}


		Stk_Image::~Stk_Image()
		{
		}

		STK_STATUS Stk_Image::SetType(StkPictureTypeEnum i_eType)
		{
			((Stk_ImageImpl*)m_pImpl)->m_eType = i_eType;
			return STK_SUCCESS;
		}

		StkPictureTypeEnum Stk_Image::GetType()
		{
			return ((Stk_ImageImpl*)m_pImpl)->m_eType;
		}

		STK_STATUS Stk_Image::SetLocation(STK_PNT32 i_pntLocation)
		{
			((Stk_ImageImpl*)m_pImpl)->m_pntLocation = i_pntLocation;
			return STK_SUCCESS;
		}

		STK_PNT32 Stk_Image::GetLocation()
		{
			return ((Stk_ImageImpl*)m_pImpl)->m_pntLocation;
		}

		STK_STATUS Stk_Image::SetDisplayWidth(STK_FLOAT32 i_fDisplayWidth)
		{
			((Stk_ImageImpl*)m_pImpl)->m_fDisplayWidth = i_fDisplayWidth;
			return STK_SUCCESS;
		}

		STK_FLOAT32 Stk_Image::GetDisplayWidth()
		{
			return ((Stk_ImageImpl*)m_pImpl)->m_fDisplayWidth;
		}

		STK_STATUS Stk_Image::SetDisplayHeight(STK_FLOAT32 i_fDisplayHeight)
		{
			((Stk_ImageImpl*)m_pImpl)->m_fDisplayHeight = i_fDisplayHeight;
			return STK_SUCCESS;
		}

		STK_FLOAT32 Stk_Image::GetDisplayHeight()
		{
			return ((Stk_ImageImpl*)m_pImpl)->m_fDisplayHeight;
		}

		STK_STATUS Stk_Image::SetActualWidth(STK_FLOAT32 i_fActualWidth)
		{
			((Stk_ImageImpl*)m_pImpl)->m_fActualWidth = i_fActualWidth;
			return STK_SUCCESS;
		}

		STK_FLOAT32 Stk_Image::GetActualWidth()
		{
			return ((Stk_ImageImpl*)m_pImpl)->m_fActualWidth;
		}

		STK_STATUS Stk_Image::SetActualHeight(STK_FLOAT32 i_fActualHeight)
		{
			((Stk_ImageImpl*)m_pImpl)->m_fActualHeight = i_fActualHeight;
			return STK_SUCCESS;
		}

		STK_FLOAT32 Stk_Image::GetActualHeight()
		{
			return ((Stk_ImageImpl*)m_pImpl)->m_fActualHeight;
		}

		STK_STATUS Stk_Image::SetPictureSaveStyle(StkPicSaveStyleEnum i_eSaveStyle)
		{
			((Stk_ImageImpl*)m_pImpl)->m_eSaveStyle = i_eSaveStyle;
			return STK_SUCCESS;
		}

		StkPicSaveStyleEnum Stk_Image::GetPictureSaveStyle()
		{
			return ((Stk_ImageImpl*)m_pImpl)->m_eSaveStyle;
		}

		STK_STATUS Stk_Image::SetPictureSavePath(wstring i_wstrSavePath)
		{
			((Stk_ImageImpl*)m_pImpl)->m_wstrPath = i_wstrSavePath;
			return STK_SUCCESS;
		}

		wstring Stk_Image::GetPictureSavePath()
		{
			return ((Stk_ImageImpl*)m_pImpl)->m_wstrPath;
		}

		STK_STATUS Stk_Image::SetNC(STK_INT32 i_nNC)
		{
			((Stk_ImageImpl*)m_pImpl)->m_nNC = i_nNC;
			return STK_SUCCESS;
		}

		STK_INT32 Stk_Image::GetNC()
		{
			return ((Stk_ImageImpl*)m_pImpl)->m_nNC;
		}

		STK_STATUS Stk_Image::SetData(STK_UINT32 i_nudataSize, STK_CHAR* i_chDataP)
		{
			if ((i_nudataSize == 0 && i_chDataP != NULL) ||
				(i_nudataSize != 0) && i_chDataP == NULL)
			{
				return STK_ERROR;
			}
			((Stk_ImageImpl*)m_pImpl)->m_uiDataSize = i_nudataSize;

			if (i_nudataSize > 0)
			{
				((Stk_ImageImpl*)m_pImpl)->m_pszData = (STK_CHAR *)malloc(i_nudataSize);
				if (((Stk_ImageImpl*)m_pImpl)->m_pszData == NULL)
				{
					return STK_ERROR;
				}
				memcpy(((Stk_ImageImpl*)m_pImpl)->m_pszData, i_chDataP, i_nudataSize);
			}

			return STK_SUCCESS;
		}

		STK_STATUS Stk_Image::GetData(STK_UINT32& o_nudataSize, STK_CHAR*& o_chDataP)
		{
			o_nudataSize = ((Stk_ImageImpl*)m_pImpl)->m_uiDataSize;

			if (((Stk_ImageImpl*)m_pImpl)->m_uiDataSize > 0)
			{
				o_chDataP = (STK_CHAR *)malloc(((Stk_ImageImpl*)m_pImpl)->m_uiDataSize);
				if (((Stk_ImageImpl*)m_pImpl)->m_pszData == NULL)
				{
					return STK_ERROR;
				}
				memcpy(o_chDataP, ((Stk_ImageImpl*)m_pImpl)->m_pszData, ((Stk_ImageImpl*)m_pImpl)->m_uiDataSize);
			}

			return STK_SUCCESS;
		}

		STK_STATUS Stk_Image::SetScaleX(STK_FLOAT32 i_fScaleX)
		{
			((Stk_ImageImpl*)m_pImpl)->m_fScaleX = i_fScaleX;
			return STK_SUCCESS;
		}

		STK_FLOAT32 Stk_Image::GetScaleX()
		{
			return ((Stk_ImageImpl*)m_pImpl)->m_fScaleX;
		}

		STK_STATUS Stk_Image::SetScaleY(STK_FLOAT32 i_fScaleY)
		{
			((Stk_ImageImpl*)m_pImpl)->m_fScaleY = i_fScaleY;
			return STK_SUCCESS;
		}

		STK_FLOAT32 Stk_Image::GetScaleY()
		{
			return ((Stk_ImageImpl*)m_pImpl)->m_fScaleY;
		}

		STK_STATUS	Stk_Image::ToJson(void* i_DataP, void* pDoc)
		{
			return ((Stk_ImageImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)pDoc);
		}

		STK_STATUS	Stk_Image::FromJson(void* i_DataP)
		{
			return ((Stk_ImageImpl*)m_pImpl)->FromJson((Value*)i_DataP);
		}
	}
}
