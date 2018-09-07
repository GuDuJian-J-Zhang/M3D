#include "m3d.h"
#include "PmiDataXmlPattern.h"
#include "Stk_ProtoType.h"
#include <math.h>
#include <algorithm>
#include <ctype.h>

//#ifdef dont complied
CPmiDataXmlPattern::CPmiDataXmlPattern(void)
{
}

CPmiDataXmlPattern::~CPmiDataXmlPattern(void)
{
}

STK_STATUS CPmiDataXmlPattern::GetXmlCompositeText(XMLElement *pDtk_CompositeTextElem, Stk_ComText** ppStkComText)
{
	STK_STATUS state = STK_SUCCESS;
	double outerbox[2][3]={};
	XMLElement*pXXElem =NULL, *pNum_textsElem = NULL,*pOuter_Bounding_boxElem = NULL,*ptop_leftElem=NULL,*pbottom_rightElem =NULL;
	pOuter_Bounding_boxElem = pDtk_CompositeTextElem->FirstChildElement();
	if(!pOuter_Bounding_boxElem)
		return state;
	ptop_leftElem = pOuter_Bounding_boxElem->FirstChildElement();
	if(!ptop_leftElem)
		return state;
	const char *ptop_left =ptop_leftElem->FirstChild()->Value();
	GetXmlMember(ptop_left,outerbox[0]);
	pbottom_rightElem = ptop_leftElem->NextSiblingElement();
	const char *pbottom_right = pbottom_rightElem->FirstChild()->Value();
	GetXmlMember(pbottom_right,outerbox[1]);

	pNum_textsElem = pOuter_Bounding_boxElem->NextSiblingElement();
	const char *  pNum_texts = pNum_textsElem->FirstChild()->Value();

	vector<Stk_Text*> stkTexts;
	STK_BOX32 stkOuterBox;

	XMLElement*pDtk_TextElem = pNum_textsElem->NextSiblingElement();
	while (pDtk_TextElem)
	{
		Stk_Text *pStkText  = NULL;
		pStkText = new Stk_Text();
		//设置字高字宽
		XMLElement *pDtk_TextStyleElem =pDtk_TextElem->FirstChildElement();
		XMLElement *pChar_heightElem = pDtk_TextStyleElem->FirstChildElement();
		const char   *pChar_height = pChar_heightElem->FirstChild()->Value();
		STK_FLOAT32 charHeight = atof(pChar_height);

		XMLElement *pChar_widthElem = pChar_heightElem->NextSiblingElement();
		const char   *pChar_width = pChar_widthElem->FirstChild()->Value();

		
		STK_FLOAT32 charWidth = atof(pChar_width);
		pStkText->SetCharWidthHeight(charWidth, charHeight);
		//设置字符间距
		XMLElement *pChar_spacingElem = pChar_widthElem->NextSiblingElement();
		const char   *pChar_spacing = pChar_spacingElem->FirstChild()->Value();
		STK_FLOAT32 charSpac = atof(pChar_spacing);
		pStkText->SetCharSpacing(charSpac);

		XMLElement *pLine_spacingElem = pChar_spacingElem->NextSiblingElement();
		const char   *pLine_spacing = pLine_spacingElem->FirstChild()->Value();

		
		//填充缩放比例
		XMLElement *pRatioElem = pLine_spacingElem->NextSiblingElement();
		const char   *pRatio = pRatioElem->FirstChild()->Value();
		//填充字体
		

		XMLElement *pFont_nameElem = pRatioElem->NextSiblingElement();
		pStkText->SetFont((WCHAR4*)"");
		/*if(pFont_nameElem)
		{
			const char   *pFont_name = pFont_nameElem->FirstChild()->Value();
			wstring  wstk_Font;
			if(pFont_name)
			{
				UTF_8ToUnicode(wstk_Font, pFont_name);
				pStkText->SetFont((WCHAR*)wstk_Font.c_str());
			}
			
		}
		else
			pStkText->SetFont(L"");
		*/
		//设置字体的内容
		XMLElement *pTextElem = pDtk_TextStyleElem->NextSiblingElement();
		const char * pText = pTextElem->FirstChild()->Value();
		int charLen = strlen(pText);
		char *pTemText = new char[charLen+1];
		strcpy(pTemText,pText);
		pTemText = pTemText+1;
		pTemText[charLen-2] = 0;
		
		wstring sktTextInfo;
		UTF_8ToUnicode(sktTextInfo,pTemText);
		pStkText->SetText(sktTextInfo);
		XMLElement *pDtk_Oriented2dBBoxElem1 = pTextElem->NextSiblingElement();
		XMLElement *pbottom_leftElem =pDtk_Oriented2dBBoxElem1->FirstChildElement();
		const char * pbottom_left = pbottom_leftElem->FirstChild()->Value();

		XMLElement *pDtk_Oriented2dBBoxElem2 = pDtk_Oriented2dBBoxElem1->NextSiblingElement();
		//设置文本的位置
		XMLElement* pDtk_InnerLoation = pDtk_Oriented2dBBoxElem2->NextSiblingElement();
		XMLElement* pInnerLocationElem = pDtk_InnerLoation->FirstChildElement();
		const char *pInnerLoacation = pInnerLocationElem->FirstChild()->Value();
		STK_PNT32	strInnerLoc;
		double ftmpVal[3] ={ };
		memset(&strInnerLoc,0,sizeof(STK_PNT32));
		GetXmlMember(pInnerLoacation, ftmpVal);
		strInnerLoc.X = ftmpVal[0];
		strInnerLoc.Y = ftmpVal[1];
		strInnerLoc.Z = ftmpVal[2];
		pStkText->SetInnerLoc(strInnerLoc);
		//设置旋转向量
		XMLElement* pDtk_InnerXYAxis = pDtk_InnerLoation->NextSiblingElement();
		XMLElement* pXDirElem = pDtk_InnerXYAxis->FirstChildElement();
		XMLElement* pYDirElem = pXDirElem->NextSiblingElement();
		const char* pXDir = pXDirElem->FirstChild()->Value();
		const char* pYDir = pYDirElem->FirstChild()->Value();
		STK_DIR32 stkAxis[2];			//X,Y旋转向量,[0]->X向量,[1]->Y向量
		memset(&stkAxis[0],0,sizeof(STK_DIR32));
		memset(&stkAxis[1],0,sizeof(STK_DIR32));
		GetXmlMember(pXDir, ftmpVal);
		stkAxis[0].X = ftmpVal[0];
		stkAxis[0].Y = ftmpVal[1];
		stkAxis[0].Z = ftmpVal[2];
		GetXmlMember(pYDir, ftmpVal);
		stkAxis[1].X = ftmpVal[0];
		stkAxis[1].Y = ftmpVal[1];
		stkAxis[1].Z = ftmpVal[2];
		pStkText->SetInnerXYAxis(stkAxis[0],stkAxis[1]);
		pDtk_TextElem = pDtk_TextElem->NextSiblingElement();
		stkTexts.push_back(pStkText);
	}
	if(stkTexts.size()>0)
	{
		Stk_ComText* pComText = new Stk_ComText();
		pComText->SetTexts(stkTexts);
		*ppStkComText = pComText;
		stkOuterBox.BoundBox[0][0]=outerbox[0][0];
		stkOuterBox.BoundBox[0][1]=outerbox[0][1];
		stkOuterBox.BoundBox[0][2]=outerbox[0][2];
		stkOuterBox.BoundBox[1][0]=outerbox[1][0];
		stkOuterBox.BoundBox[1][1]=outerbox[1][1];
		stkOuterBox.BoundBox[1][2]=outerbox[1][2];
		(*ppStkComText)->SetOuterBox(stkOuterBox);
	}
	return state;
}
//UTF_8转UNICODE
void CPmiDataXmlPattern::UTF_8ToUnicode(wstring &wstrOut, const char *pText)
{
//	if(!pText || 0 == pText[0])
//		return;
//
//	wchar_t* pUnicodeText;
//	int iTextbyte = MultiByteToWideChar(CP_UTF8, 0, pText, -1, NULL, 0);
//	pUnicodeText = new wchar_t[iTextbyte];
//
//
//	memset(pUnicodeText, 0, iTextbyte);
//	::MultiByteToWideChar(CP_UTF8, 0, pText, -1, pUnicodeText, iTextbyte);
//
//	wstrOut = pUnicodeText;
//	delete []pUnicodeText;
//	return;

	wstrOut = StringToWString(pText);
}
XMLElement* CPmiDataXmlPattern::GetXmlCurve(XMLElement *pDtk_CurvePtrElem,Stk_CurveEntity **ppStkCurve,bool flg)
{
	STK_STATUS state = STK_SUCCESS;
	XMLElement *pdirElem = NULL;
	
	XMLElement * pDtk_XXXLinePtrElem = NULL;
	double ftmpVal[3] ={ };
	/*for(i=0;i<ncurve_num;i++)
	{*/
		// 获取Trim的信息

		
		pDtk_XXXLinePtrElem =pDtk_CurvePtrElem->FirstChildElement();
		const char* pVal =pDtk_XXXLinePtrElem->Value();
		if(stricmp(pVal,"Dtk_PolylinePtr")==0)
		{
			Stk_PolyLine*    pStkPloyLine = new Stk_PolyLine();
			//获取Trim的信息
			XMLElement *pTrimElem = pDtk_XXXLinePtrElem->FirstChildElement();
			XMLElement *pUMaxElem = pTrimElem->FirstChildElement();
			const char*pUMax = pUMaxElem->FirstChild()->Value();
			STK_FLOAT32 fUmax=atof(pUMax);

			XMLElement *pUMinElem = pUMaxElem->NextSiblingElement();
			const char *pUMin = pUMinElem->FirstChild()->Value();
			STK_FLOAT32 fUmin = atof(pUMin);

			pStkPloyLine->SetDomain(fUmax,fUmin);

			XMLElement *pPointsElem = pTrimElem->NextSiblingElement();
			XMLElement *pPntElem = pPointsElem->FirstChildElement();
			vector<STK_PNT32> cuvPoints;
			while (pPntElem)
			{
				STK_PNT32 pnt;
				memset(&pnt,0,sizeof(STK_PNT32));
				const char * pPnt =pPntElem->FirstChild()->Value();
				GetXmlMember(pPnt,ftmpVal);
				pnt.X = ftmpVal[0];
				pnt.Y = ftmpVal[1];
				pnt.Z = ftmpVal[2];
				cuvPoints.push_back(pnt);

				pPntElem = pPntElem->NextSiblingElement();
			}
			pStkPloyLine->SetPoints(cuvPoints);

			//设置线条的类型
			

			*ppStkCurve=pStkPloyLine;

			if(flg)
				pdirElem = pPointsElem->NextSiblingElement();
			

		}
		else if(stricmp(pVal,"Dtk_EllipsePtr")==0)
		{
			Stk_Ellipse *    pStkEllipse  = new Stk_Ellipse();

			STK_PNT32 stkcenterPnt ;

			STK_DIR32 stkXVec,stkYVec,stkZVec,normalVec,originVec;
		
			STK_FLOAT32  sktUmin = 0.0f, sktUmax= 0.0f;
			memset(&stkcenterPnt,0,sizeof(STK_PNT32));
			memset(&stkXVec,0,sizeof(STK_DIR32));
			memset(&stkYVec,0,sizeof(STK_DIR32));
			memset(&stkZVec,0,sizeof(STK_DIR32));
			memset(&normalVec,0,sizeof(STK_DIR32));
			memset(&originVec,0,sizeof(STK_DIR32));

			

			//获取Trim的信息
			XMLElement *pTrimElem = pDtk_XXXLinePtrElem->FirstChildElement();
			XMLElement *pUMaxElem = pTrimElem->FirstChildElement();
			const char*pUMax = pUMaxElem->FirstChild()->Value();
			STK_FLOAT32 fUmax=atof(pUMax);
			
			XMLElement *pUMinElem = pUMaxElem->NextSiblingElement();
			const char *pUMin = pUMinElem->FirstChild()->Value();
			STK_FLOAT32 fUmin = atof(pUMin);

			pStkEllipse->SetDomain(fUmax,fUmin);

			//获取起始点的信息

			double fStartPnt[3]={},fEndPnt[3]={};
			STK_PNT32 stkStartPnt,stkEndPnt;
			memset(&stkStartPnt,0,sizeof(STK_PNT32));
			memset(&stkEndPnt,0,sizeof(STK_PNT32));

			XMLElement *pStartPointElem = pUMinElem->NextSiblingElement();
			const char *pStartPoint = pStartPointElem->FirstChild()->Value();
			
			GetXmlMember(pStartPoint,fStartPnt);
			stkStartPnt.X = fStartPnt[0];
			stkStartPnt.Y = fStartPnt[1];
			stkStartPnt.Z = fStartPnt[2];
			

			XMLElement*pEndPointElem = pStartPointElem->NextSiblingElement();
			const char* pEndPoint = pEndPointElem->FirstChild()->Value();

			GetXmlMember(pEndPoint,fEndPnt);
			stkEndPnt.X = fEndPnt[0];
			stkEndPnt.Y = fEndPnt[1];
			stkEndPnt.Z = fEndPnt[2];
			
			pStkEllipse->SetCoordiantePnt(stkStartPnt,stkEndPnt);
			


			//获取椭圆的中心点
			XMLElement* pCenterElem = pTrimElem->NextSiblingElement();
			const char * pCenter = pCenterElem->FirstChild()->Value();
			GetXmlMember(pCenter,ftmpVal);
			stkcenterPnt.X = ftmpVal[0];
			stkcenterPnt.Y = ftmpVal[1];
			stkcenterPnt.Z = ftmpVal[2];

			pStkEllipse->SetCenterPoint(stkcenterPnt);

			
			//获取椭圆的X方向
			XMLElement *pxDirElem = pCenterElem->NextSiblingElement();
			const char *pxDir = pxDirElem->FirstChild()->Value();
			GetXmlMember(pxDir,ftmpVal);
			stkXVec.X = ftmpVal[0];
			stkXVec.Y = ftmpVal[1];
			stkXVec.Z = ftmpVal[2];
			


			//获取椭圆的Y方向
			XMLElement *pyDirElem = pxDirElem->NextSiblingElement();
			const char *pyDir = pyDirElem->FirstChild()->Value();
			GetXmlMember(pyDir,ftmpVal);
			stkYVec.X = ftmpVal[0];
			stkYVec.Y = ftmpVal[1];
			stkYVec.Z = ftmpVal[2];

			//获取椭圆的Z方向
			XMLElement *pzDirElem = pyDirElem->NextSiblingElement();
			const char *pzDir = pzDirElem->FirstChild()->Value();
			GetXmlMember(pzDir,ftmpVal);
			stkZVec.X = ftmpVal[0];
			stkZVec.Y = ftmpVal[1];
			stkZVec.Z = ftmpVal[2];

			pStkEllipse->SetXYZDir(stkXVec,stkYVec,stkZVec);

			//获取椭圆的normal的信息
			
			XMLElement *pNormalElem =pzDirElem->NextSiblingElement();
			const char *pNormal=pNormalElem->FirstChild()->Value();
			GetXmlMember(pNormal,ftmpVal);
			
			normalVec.X=ftmpVal[0];
			normalVec.Y=ftmpVal[1];
			normalVec.Z=ftmpVal[2];
			//设置Normal
			pStkEllipse->SetNormal(normalVec);

			//获取椭圆的origin点的信息
			XMLElement*pOriginElem =pNormalElem->NextSiblingElement();
			const char*pOrigin = pOriginElem->FirstChild()->Value();
			GetXmlMember(pOrigin,ftmpVal);
			originVec.X = ftmpVal[0];
			originVec.Y = ftmpVal[1];
			originVec.Z = ftmpVal[2];
			pStkEllipse->SetOriginDir(originVec);
			

			//获取两个半径的信息
			XMLElement *pMajorRadiusElem = pOriginElem->NextSiblingElement();
			const char *pMajorRadius = pMajorRadiusElem->FirstChild()->Value();
			STK_FLOAT32 stk_majRad = atof(pMajorRadius);
			

			XMLElement *pMinorRadiusElem = pMajorRadiusElem->NextSiblingElement();
			const char *pMinorRadius = pMinorRadiusElem->FirstChild()->Value();
			STK_FLOAT32 stk_minRad = atof(pMinorRadius);

			pStkEllipse->SetRadius(stk_majRad,stk_minRad);

			*ppStkCurve=pStkEllipse;
			if(flg)
				pdirElem = pMinorRadiusElem->NextSiblingElement();
			
		}
		//线段类型的逻辑处理部分
		else if(stricmp(pVal,"Dtk_LinePtr")==0)
		{
			XMLElement *pPnt1Elem = NULL,*pPnt2Elem = NULL, *pTmpElem = NULL;
			Stk_Line*    pStkLine = new Stk_Line();
			STK_PNT32 stkStartPnt,stkEndPnt;
			memset(&stkStartPnt,0,sizeof(STK_PNT32));
			memset(&stkEndPnt,0,sizeof(STK_PNT32));
			pTmpElem = pDtk_XXXLinePtrElem->FirstChildElement();
			while(pTmpElem)
			{
				const char * ptmpVal = pTmpElem->Value();
				if(stricmp(ptmpVal,"Points")==0)
				{
					double fPnts[3] ={};
					pPnt1Elem = pTmpElem->FirstChildElement();
					const char*pPnt1=pPnt1Elem->FirstChild()->Value();
					pPnt2Elem = pPnt1Elem->NextSiblingElement();
					const char*pPnt2=pPnt2Elem->FirstChild()->Value();
					GetXmlMember(pPnt1,fPnts);
					stkStartPnt.X=fPnts[0];
					stkStartPnt.Y=fPnts[1];
					stkStartPnt.Z=fPnts[2];

					GetXmlMember(pPnt2,fPnts);
					stkEndPnt.X = fPnts[0];
					stkEndPnt.Y = fPnts[1];
					stkEndPnt.Z = fPnts[2];

					pStkLine->SetStartPoint(stkStartPnt);
					pStkLine->SetEndPoint(stkEndPnt);
					*ppStkCurve = pStkLine;
					break;
				}
				pTmpElem = pTmpElem->NextSiblingElement();
			}
		}
		else
			*ppStkCurve = NULL;

	/*	pDtk_CurvePtrElem = pDtk_CurvePtrElem->NextSiblingElement();
	}*/
	
	
	

	return pdirElem;
}
STK_STATUS CPmiDataXmlPattern::GetXmlMember(const char * pdata,double*aQueue)
{
	STK_STATUS state =STK_SUCCESS;

	
	int nlen = strlen(pdata);
	char * pstr =new char[nlen+1];
	strcpy(pstr,pdata);
	//删除(、)和拆分成员

	char * pvalue =strtok(pstr," ");
	int i =0;
	while(pvalue)
	{
		aQueue[i] =(float)atof(pvalue);
		i++;
		pvalue=strtok(NULL," ");
		
	}
	if(pstr)
		delete pstr;
	pstr = NULL;
	return state;
}

STK_STATUS CPmiDataXmlPattern::GetXmlTerminateorDir(XMLElement *pdirElem,double* refvec)
{
	STK_STATUS state = STK_SUCCESS;
	//获取方向
	

	const char*pdir = pdirElem->FirstChild()->Value();
	GetXmlMember(pdir,refvec);

	return state;
}

STK_STATUS CPmiDataXmlPattern::GetXmlLead(XMLElement *pDtk_LeaderPtrElem,vector<Stk_Leader*> &inLeader )
{
	STK_STATUS state = STK_SUCCESS;

	/*********************************************************
	 *************************获取Leader信息******************
	 *********************************************************/

	
	//获取Leader的数量
	//INT          nLead = 0;
	//INT			 i     = 0;
	//
	//XMLElement *pNum_leadersElem = pLeadersElem->FirstChildElement();//第一个Lead数量
	//if(pNum_leadersElem)
	//{

	//	const char *pNum_leaders = pNum_leadersElem->FirstChild()->Value();//对应于 <num_leaders>

	//	nLead = atoi(pNum_leaders);

	//	XMLElement *pDtk_LeaderPtrElem = NULL;

	//	for (i = 0; i<nLead;i++)
	//	{
			int i = 0;
			Stk_Leader *pSkt_L = NULL;
			//pDtk_LeaderPtrElem = pNum_leadersElem->NextSiblingElement();//对应于<Dtk_LeaderPtr>
			if(pDtk_LeaderPtrElem)
			{

				//定义leader对象
				pSkt_L = new Stk_Leader();
				Stk_CurveEntity* pStkCurve = NULL;

				//获取引线的类型
				XMLElement *pLeaderTypeElem = pDtk_LeaderPtrElem->FirstChildElement();
				const char *pLeaderType = pLeaderTypeElem->FirstChild()->Value();//对应于<LeaderType>
				StkLeaderTypeEnum nLeaderType = LEADER_TYPE_UNKNOWN;
				if(strcmp(pLeaderType,"TypeAngular")==0)
					nLeaderType = LEADER_TYPE_ANGULAR;
				else if(strcmp(pLeaderType,"TypeLinear")==0)
					nLeaderType = LEADER_TYPE_LENGTH;
				pSkt_L->SetLeaderType(nLeaderType);

				//获取末端符号的信息
				XMLElement * pArrowElem = pLeaderTypeElem->NextSiblingElement();
				XMLElement * pLocationElem = pArrowElem->FirstChildElement();
				const char *pLocation = pLocationElem->FirstChild()->Value();

				double locationPnt[3]={};
				STK_PNT32 stkLocation;
				GetXmlMember(pLocation,locationPnt);
				stkLocation.X=locationPnt[0];
				stkLocation.Y=locationPnt[1];
				stkLocation.Z=locationPnt[2];

				double locationDir[3]={};

				
				//获取末端符号的宽度。
				XMLElement *pWidthElem = pLocationElem->NextSiblingElement();
				const char*pWidth = pWidthElem->FirstChild()->Value();

				STK_FLOAT32 sizes[2]={};
				sizes[0] = atof(pWidth);

				//获取末端符号的高度。
				XMLElement *pHeightElem = pWidthElem->NextSiblingElement();
				const char*pHeight = pHeightElem->FirstChild()->Value();
				sizes[1] = atof(pHeight);


				//获取末端符号的类型
				XMLElement *pTerminatorTypeElem = pHeightElem->NextSiblingElement();
				const char*pTerminatorType = pTerminatorTypeElem->FirstChild()->Value();

				STK_UINT32 nTerminatortype =atoi(pTerminatorType);
				
				//获取线列的类型
				XMLElement * pDtk_CurvePtrElem = pArrowElem->NextSiblingElement();

				XMLElement *pDirElem =GetXmlCurve(pDtk_CurvePtrElem,&pStkCurve,true);

				if(pStkCurve)
				{
					pStkCurve->SetDefineType(Stk_CurveEntity::DefineTypeEnum::PMI_LEADER_LINE);
					pSkt_L->SetCurve(pStkCurve);
				}



				//计算末端符号的方向
				if(pDirElem)
				{
					bool b = false;
					double refvec[3]={};
					double refsize =0.0;
					STK_DIR32 stkdir;
				
					GetXmlTerminateorDir(pDirElem,refvec);
					
					if((refvec[0]==0.0f)&&(refvec[1]==0.0f)&&(1.0f==refvec[2]||refvec[2]==-1.0f))
					{
						if(Stk_CurveEntity::CurveTypeEnum::POLYLINE_TYPE == pStkCurve->GetCurveType())
						{
							STK_PNT32 pnt0,tmpPnt;
							Stk_PolyLine *tmpPolyLineP =(Stk_PolyLine *)pStkCurve;
							vector<STK_PNT32> tmpPnts =tmpPolyLineP->GetPoints();
							int ncount = tmpPnts.size();
							
							if(ncount > 0 )
								pnt0 = tmpPnts.at(0);
							for (i= 1;i<ncount;i++)
							{
								tmpPnt = tmpPnts.at(i);
								if(tmpPnt.X==pnt0.X&&tmpPnt.Y==pnt0.Y)
								{
									b =false;
									continue;
								}
								else
								{
									b = true;
									refvec[0]=tmpPnt.X-pnt0.X;
									refvec[1]=tmpPnt.Y-pnt0.Y;
									refvec[2]=0.0;
									break;
								}
							}
							if(b)
							{
								refsize = sqrt(((refvec[0])*(refvec[0]))+((refvec[1])*(refvec[1]))+((refvec[2])*(refvec[2])));
								refvec[0]  = (float)(refvec[0]/refsize);
								refvec[1]  = (float)(refvec[1]/refsize);
								refvec[2]  = (float)(refvec[2]/refsize);	
							}
						}
					}
					stkdir.X = refvec[0];
					stkdir.Y = refvec[1];
					stkdir.Z = refvec[2];
					//设置末端符号
					pSkt_L->SetTerminator(nTerminatortype,sizes,stkLocation,stkdir);
				}

				
			}
			inLeader.push_back(pSkt_L);
	//	}
	//}
	//
	return state;
}

STK_STATUS CPmiDataXmlPattern::GetXmlExt_Line(XMLElement*pDtk_CurvePtrElem,vector<Stk_Extensionline*>&inExLine)
{
	int i = 0;

	STK_STATUS state = STK_SUCCESS;
	
	
	Stk_Extensionline *stk_extLine  = new Stk_Extensionline();;
	Stk_CurveEntity *pStkCurve = NULL; 
	//获取延长线的类型
	
	GetXmlCurve(pDtk_CurvePtrElem,&pStkCurve);
	if(pStkCurve)
	{
		pStkCurve->SetDefineType(3);
		stk_extLine->SetExLineType(Stk_Extensionline::LENGTH_TYPE);
		stk_extLine->SetCurve(pStkCurve);
		inExLine.push_back(stk_extLine);
	}

	return state;
}


STK_STATUS CPmiDataXmlPattern::GetXmlSpecialLines(XMLElement*pTmpDtk_CurvePtrElem,Stk_SpecialLine **ppStkUserLine)
{
	STK_STATUS state = STK_SUCCESS;
	Stk_CurveEntity *stkCuvP = NULL;
	Stk_SpecialLine * pCurv = new Stk_SpecialLine();
	GetXmlCurve(pTmpDtk_CurvePtrElem,&stkCuvP);
	if(stkCuvP)
	{
		stkCuvP->SetDefineType(5);
		pCurv->AddCurve(stkCuvP);
		*ppStkUserLine = pCurv;
	}


	return state;
}
STK_STATUS CPmiDataXmlPattern::GetXmlmatrix(XMLElement*pTransfo_matrixElem,STK_MTX32 *pmiMtxP)
{
	STK_STATUS state = STK_SUCCESS;
	int i=0,j=0;
	double fmatval=0.0;

	XMLElement *pRowElem = pTransfo_matrixElem->FirstChildElement();
	while (pRowElem)
	{
		const char * pRow = pRowElem->FirstChild()->Value();
		int nlen = strlen(pRow);
		char * pstr =new char[nlen+1];
		strcpy(pstr,pRow);
		//删除(、)和拆分成员
		
		char * pvalue =strtok(pstr," ");
		j=0;
		while(pvalue)
		{
			pmiMtxP->PlcMatrix[i][j] =(float)atof(pvalue);
			pvalue=strtok(NULL," ");
			j++;
		}
		if(pstr)
			delete pstr;
		pstr = NULL;
		pRowElem = pRowElem->NextSiblingElement();
		i++;
	}
	return state;
}
STK_STATUS CPmiDataXmlPattern::GetXmlModelDisplay(XMLElement*pDtk_ModelDisplayElem, Stk_View** ppView)
{
	STK_STATUS	state = STK_SUCCESS;
	double		locationPnt[3]={0};
	STK_PNT32	stkLocation;
	STK_DIR32	stkDirection,stkDirection2;
	STK_FLOAT32	stkFloat = 0.0f;
	STK_FLOAT32 stkTransparence = 1.0f ;
	//获取剖/视图的透明度信息
	const XMLAttribute* pAttrib = pDtk_ModelDisplayElem->FirstAttribute();
	if(pAttrib)
	{
		stkTransparence = pAttrib->DoubleValue();
		
	}
	Stk_View *pView = new Stk_View(1); // 临时填写1
	Stk_Camera *pCamera = new Stk_Camera(1); // 临时填写1

	XMLElement *pDtk_IDElem = pDtk_ModelDisplayElem->FirstChildElement();
	const char * pDtk_ID = pDtk_IDElem->FirstChild()->Value();
	pView->SetID(atoi(pDtk_ID));

	XMLElement *pDtk_NameElem = pDtk_IDElem->NextSiblingElement();
	const char * pDtk_Name = pDtk_NameElem->FirstChild()->Value();
	wstring wstrViewName;
	UTF_8ToUnicode(wstrViewName,pDtk_Name);
	pView->SetName(wstrViewName);
	pView->SetTransparency(stkTransparence);//设置透明度

	XMLElement *pDtk_CameraElem = pDtk_NameElem->NextSiblingElement();

	//获取Camera的信息
	//获取原点的信息
	XMLElement *pOriginPointElem = pDtk_CameraElem->FirstChildElement();
	if(pOriginPointElem)
	{

	
	const char * pOriginPoint = pOriginPointElem->FirstChild()->Value();
	GetXmlMember(pOriginPoint,locationPnt);
	stkLocation.X=locationPnt[0];
	stkLocation.Y=locationPnt[1];
	stkLocation.Z=locationPnt[2];
	pCamera->SetOriginPoint(stkLocation);

	//获取TargetVector的信息
	XMLElement *pTargetVectorElem = pOriginPointElem->NextSiblingElement();
	const char * pTargetVector = pTargetVectorElem->FirstChild()->Value();
	GetXmlMember(pTargetVector,locationPnt);
	stkDirection.X=locationPnt[0];
	stkDirection.Y=locationPnt[1];
	stkDirection.Z=locationPnt[2];
	pCamera->SetTargetVector(stkDirection);

	//获取UpVector的信息
	XMLElement *pUpVectorElem = pTargetVectorElem->NextSiblingElement();
	const char * pUpVector = pUpVectorElem->FirstChild()->Value();
	GetXmlMember(pUpVector,locationPnt);
	stkDirection.X=locationPnt[0];
	stkDirection.Y=locationPnt[1];
	stkDirection.Z=locationPnt[2];
	pCamera->SetUpVector(stkDirection);


	//获取角度的信息
	XMLElement *pAngleElem = pUpVectorElem->NextSiblingElement();
	const char *pAngle = pAngleElem->FirstChild()->Value();
	stkFloat = atof(pAngle);
	pCamera->SetAngle(stkFloat);

	//获取Focus的信息
	XMLElement *pFocusElem = pAngleElem->NextSiblingElement();
	const char*pFocus = pFocusElem->FirstChild()->Value();
	stkFloat = atof(pFocus);
	pCamera->SetFocus(stkFloat);

	//获取ZoomFocus的信息
	XMLElement *pZoomFactorElem = pFocusElem->NextSiblingElement();
	const char *pZoomFactor = pZoomFactorElem->FirstChild()->Value();
	stkFloat = atof(pZoomFactor);
	pCamera->SetZoomFactor(stkFloat);
	pView->SetCamera(pCamera);
	}

	XMLElement *pTmpElem = pDtk_CameraElem->NextSiblingElement();
	while(pTmpElem)
	{
		const char * pVal = pTmpElem->Value();
		if(stricmp(pVal,"Dtk_NodeConnector")==0)
		{
			Stk_NodeConnector *pNodeConnector = new Stk_NodeConnector(1); // 临时填写1
			XMLElement* pPathCompIdElem=pTmpElem->FirstChildElement();
			const char*   pPathCompId = pPathCompIdElem->FirstChild()->Value();
			wstring wstrPathCompId;
			UTF_8ToUnicode(wstrPathCompId,pPathCompId);
			pNodeConnector->SetLinkProto(wstrPathCompId);

			XMLElement* pNodeIDElem = pPathCompIdElem->NextSiblingElement();
			const char  * pNodeID = pNodeIDElem->FirstChild()->Value();
			pNodeConnector->SetLinkNode(atoi(pNodeID));

			XMLElement* pPointingEntityTypeElem = pNodeIDElem->NextSiblingElement();
			const XMLAttribute* pPrimaryAttribute = pPointingEntityTypeElem->FirstAttribute();
			const char * pPrimary = pPrimaryAttribute->Value();
			const char * pSecondary = pPrimaryAttribute->Next()->Value();

			XMLElement* pPointedEntityTypeElem =pPointingEntityTypeElem->NextSiblingElement();
			const char *pPointedEntityType = pPointedEntityTypeElem->FirstChild()->Value();
			pNodeConnector->SetEntityType(10,atoi(pPointedEntityType));

			pView->AddNodeConnector(pNodeConnector);
		}
		else if(stricmp(pVal,"DTK_TYPE_PLANE")==0)
		{
			Stk_PlaneSurface *pPlaneSurface = new Stk_PlaneSurface();
			XMLElement* pORIGINElem = pTmpElem->FirstChildElement();
			const char * pORIGIN = pORIGINElem->FirstChild()->Value();
			GetXmlMember(pORIGIN,locationPnt);
			stkLocation.X=locationPnt[0];
			stkLocation.Y=locationPnt[1];
			stkLocation.Z=locationPnt[2];
			pPlaneSurface->SetOrigin(stkLocation);


			XMLElement* pNormalElem = pORIGINElem->NextSiblingElement();
			const char * pNormal = pNormalElem->FirstChild()->Value();
			GetXmlMember(pNormal,locationPnt);
			stkDirection.X=locationPnt[0];
			stkDirection.Y=locationPnt[1];
			stkDirection.Z=locationPnt[2];
			pPlaneSurface->SetNormal(stkDirection);

			XMLElement* pUDirectionElem = pNormalElem->NextSiblingElement();
			const char * pUDirection = pUDirectionElem->FirstChild()->Value();
			GetXmlMember(pUDirection,locationPnt);
			stkDirection.X=locationPnt[0];
			stkDirection.Y=locationPnt[1];
			stkDirection.Z=locationPnt[2];

			XMLElement* pVDirectionElem = pUDirectionElem->NextSiblingElement();
			const char * pVDirection = pVDirectionElem->FirstChild()->Value();
			GetXmlMember(pVDirection,locationPnt);
			stkDirection2.X=locationPnt[0];
			stkDirection2.Y=locationPnt[1];
			stkDirection2.Z=locationPnt[2];

			pPlaneSurface->SetUVDir(stkDirection,stkDirection2);
			pView->SetClippingPlan(pPlaneSurface);
		}
		pTmpElem = pTmpElem->NextSiblingElement();

		
	}
	
	*ppView = pView;
	return state;

}

STK_STATUS CPmiDataXmlPattern::GetXmlFrame(XMLElement*pDtk_FrameElem,Stk_OutFrame** ppStkOutFrame)
{

	Stk_CurveEntity* pStkCurve = NULL;
	STK_STATUS state = STK_SUCCESS;
	Stk_OutFrame *pStkOutFrame =NULL;
	XMLElement* pFrameTypeElem = pDtk_FrameElem->FirstChildElement();
	const char* pFrameType = pFrameTypeElem->FirstChild()->Value();
	STK_UINT32 nFrameType=atoi(pFrameType);
	STK_UINT32 nDefType = 4;
	int i = 0;

	if(nFrameType ==0)
	{
		*ppStkOutFrame = NULL;
		return state;
	}

	pStkOutFrame = new Stk_OutFrame();
	pStkOutFrame->SetFrameType(nFrameType);

	XMLElement* pDtk_CurvesElem = pFrameTypeElem->NextSiblingElement();

	//获取Curve的数量
	XMLElement* pcurve_numElem = pDtk_CurvesElem->FirstChildElement();
	const char*pcurve_num =pcurve_numElem->FirstChild()->Value();
	STK_UINT32 curve_num = atoi(pcurve_num);
	XMLElement* pDtk_CurvePtrElem = pcurve_numElem->NextSiblingElement();
	for(i=0;i<curve_num;i++)
	{	
		pStkCurve = NULL;
		GetXmlCurve(pDtk_CurvePtrElem,&pStkCurve);
		if(pStkCurve)
		{
			pStkCurve->SetDefineType(nDefType);
			pStkOutFrame->SetFrameData(pStkCurve);
		}
		pDtk_CurvePtrElem = pDtk_CurvePtrElem->NextSiblingElement();
	}

	*ppStkOutFrame = pStkOutFrame;
	return state;
}

STK_STATUS CPmiDataXmlPattern::OpenXml( const char * FilePath,Stk_File * pFile)
{
	STK_STATUS		state = STK_SUCCESS;
	Stk_PMI			*PmiEntityP = NULL;
	Stk_ProtoType	*ProtoP = NULL;
	//WCHAR			strProtoKeyA[STK_MAXPATH];
	wstring         strProtoKeyA;
	INT				i =0,nPMI = 1;


	//创建xml文档
	XMLDocument*myDocument=new XMLDocument();
	myDocument->LoadFile(FilePath);


	//获取根元素
	XMLElement *RootElement = myDocument->RootElement();
	//获取根元素的名称
	const char * pRootName =RootElement->Value();

	XMLElement * pProtoElme=RootElement->FirstChildElement();
	
	while(pProtoElme && strcmp( pProtoElme->Value(),"Stk_ProtoType") == 0)
	{
		XMLElement * pKeyElem = pProtoElme->FirstChildElement();
		const char* pProtoKey = pKeyElem->FirstChild()->Value();
		UTF_8ToUnicode(strProtoKeyA, pProtoKey);

		//MultiByteToWideChar(CP_ACP, 0, pProtoKey, -1, strProtoKeyA, STK_MAXPATH);
		ProtoP = pFile->GetProtoTypeByKey(strProtoKeyA);
		if( ProtoP == NULL){
			pProtoElme =  pProtoElme->NextSiblingElement();
			continue;
		}

		/**********************************************************************
		 ********************获取当前ProtoType下的所有PMI要素******************
		 **********************************************************************/
		//获取第一个PMI要素的信息
		XMLElement * pPMIElme = pKeyElem->NextSiblingElement();
		if(pPMIElme)
		{
			if(stricmp(pPMIElme->Value(),"Dtk_FdtPtr")==0)
			{
				PmiEntityP = new Stk_PMI(ProtoP->GetID());
		
				//获取第一个PMI要素的ID数据
				XMLElement * pIdData = pPMIElme->FirstChildElement();
				const char* pId =pIdData->FirstChild()->Value();
				STK_ID stkidPMIid = atoi(pId);
				PmiEntityP->SetID(stkidPMIid);
				
				//获取第一个PMI要素的平行到屏幕的标志
				XMLElement * pDispFlatScreenElem = pIdData->NextSiblingElement();
				const char   * pDispFlatScreen     = pDispFlatScreenElem->FirstChild()->Value();
				STK_ID stkDispFlat = atoi((pDispFlatScreen));
				PmiEntityP->SetParallelScreen(stkDispFlat);

				//获取第一个PMI要素的显隐状态
				XMLElement *pXXXXXXXXlElem = pDispFlatScreenElem->NextSiblingElement();
				XMLElement *pDtk_BlankedStatusElem = NULL;
				XMLElement *pColorElem = NULL;
				STK_INT32 visualFlg = 0;

				if(stricmp(pXXXXXXXXlElem->Value(),"Dtk_BlankedStatus")==0)
				{	pDtk_BlankedStatusElem = pXXXXXXXXlElem;

					const char * pvisual = pDtk_BlankedStatusElem->FirstChild()->Value();
					visualFlg = atoi(pvisual);
					//获取第一个PMI要素的颜色数据
					pColorElem = pDtk_BlankedStatusElem->NextSiblingElement();
				}
				else
					//获取第一个PMI要素的颜色数据
					pColorElem = pXXXXXXXXlElem;

				PmiEntityP->SetDisplay(visualFlg?false:true);
				
				//获取颜色分量
				XMLElement *pColorElemX = pColorElem->FirstChildElement();
				STK_RGBA32 color ;
				
				while (pColorElemX)
				{
					const char *pColorX = pColorElemX->FirstChild()->Value();
					const char *pMemb= pColorElemX->Value();
					STK_FLOAT32 f = atof(pColorX);
					if(stricmp(pMemb,"r")==0)
						color.Red = f;
					else if(stricmp(pMemb,"g")==0)
						color.Green = f ;
					else if (stricmp(pMemb,"b")==0)
						color.Blue = f;
					else if(stricmp(pMemb,"a") == 0)
						color.Alpha = f;
					pColorElemX = pColorElemX->NextSiblingElement();
				}
				PmiEntityP->SetColor(color);
				
				/**********************************************************************
				 *************************获取第一个PMI要素定义的平面******************
				 **********************************************************************/
				XMLElement *pDefinePlaneElem = pColorElem->NextSiblingElement();
				STK_MTX32 PlaneMtx;
				memset(&PlaneMtx,0,sizeof(STK_MTX32)); //初始化变量
				
				GetXmlmatrix(pDefinePlaneElem,&PlaneMtx);
				//设置平面
				PmiEntityP->SetDefPlane(PlaneMtx);

				//获取第一个PMI要素定义的类型
				XMLElement *pTypeElem = pDefinePlaneElem->NextSiblingElement();
				const char *pType = pTypeElem->FirstChild()->Value();
				STK_UINT32 ntye = atoi(pType);
				//设置类型
				PmiEntityP->SetType(ntye);

				/*********************************************************
				 *************************获取Leader信息******************
				 *********************************************************/
				
				XMLElement *pLeadersElem = pTypeElem->NextSiblingElement();

				XMLElement *pnum_leadersElem = pLeadersElem->FirstChildElement();
				const  char*pnum_leaders = pnum_leadersElem->FirstChild()->Value();
				STK_UINT32 num_leaders = atoi(pnum_leaders);

				vector<Stk_Leader*> inLeader ;

				XMLElement *pDtk_LeaderPtrElem = pnum_leadersElem->NextSiblingElement();
				for(i=0;i<num_leaders;i++)
				{
					
					GetXmlLead(pDtk_LeaderPtrElem,inLeader);
					pDtk_LeaderPtrElem = pDtk_LeaderPtrElem->NextSiblingElement();
				}
				if(inLeader.size()>0)
					PmiEntityP->SetLeaders(inLeader);

				/**************************************************************
				 ***************************获取Frame的信息********************
				 **************************************************************/
				Stk_OutFrame* pStkOutFrames = NULL;
				
				XMLElement *pDtk_FrameElem = pLeadersElem->NextSiblingElement();
				
				while (pDtk_FrameElem && stricmp(pDtk_FrameElem->Value(),"Dtk_Frame")==0)
				{
					GetXmlFrame(pDtk_FrameElem,&pStkOutFrames);
					if(pStkOutFrames)
						PmiEntityP->SetOutFrame(pStkOutFrames);

					pDtk_FrameElem = pDtk_FrameElem->NextSiblingElement();
				}


				//获取复合文本的信息
				//XMLElement *pDtk_CompositeTextElem =pDtk_FrameElem->NextSiblingElement();
				XMLElement *pDtk_CompositeTextElem =pDtk_FrameElem;
				Stk_ComText* ppStkComText = NULL;
				while(pDtk_CompositeTextElem&&stricmp(pDtk_CompositeTextElem->Value(),"Dtk_CompositeText")==0)
				{
					GetXmlCompositeText(pDtk_CompositeTextElem,&ppStkComText);
					if(ppStkComText)
						PmiEntityP->AddComText(ppStkComText);
					pDtk_CompositeTextElem = pDtk_CompositeTextElem->NextSiblingElement();
				}
				
		
				/**************************************************************
				 ***************************获取延长线的信息********************
				 **************************************************************/
				XMLElement*pSpecialLinesElem = NULL,*pext_linesElem =NULL,*pXXX_linesElem=NULL ;
				//pXXX_linesElem = pDtk_CompositeTextElem->NextSiblingElement();
				vector<Stk_Extensionline*>inExLine;
				Stk_SpecialLine *pStkUserLine = NULL;
				XMLElement *pTmpDtk_CurvePtrElem = NULL;
				pXXX_linesElem =pDtk_CompositeTextElem;
				if(pXXX_linesElem)
				{
					const char* tagNameP=pXXX_linesElem->Value();
					
			
					if(stricmp(tagNameP,"ext_lines")==0)
					{
						pext_linesElem = pXXX_linesElem;
						XMLElement* pnum_ext_linesElem = pext_linesElem->FirstChildElement();
						const char* pnum_ext_lines= pnum_ext_linesElem->FirstChild()->Value();
						int num_ext_lines = atoi(pnum_ext_lines);
						pTmpDtk_CurvePtrElem = pnum_ext_linesElem->NextSiblingElement();
						for(i=0;i<num_ext_lines;i++)
						{

							GetXmlExt_Line(pTmpDtk_CurvePtrElem,inExLine);//设置线列
							
							pTmpDtk_CurvePtrElem = pTmpDtk_CurvePtrElem->NextSiblingElement();
						}
						if(inExLine.size()>0)
							PmiEntityP->SetExLines(inExLine);

					}
					else if(stricmp(tagNameP,"SpecialLines")==0)
					{

						pSpecialLinesElem=pXXX_linesElem;
						XMLElement*pnum_SpecialLinesElem = pSpecialLinesElem->FirstChildElement();
						const char* pnum_SpecialLines = pnum_SpecialLinesElem->FirstChild()->Value();
						int num_SpecialLines = atoi(pnum_SpecialLines);
						pTmpDtk_CurvePtrElem = pnum_SpecialLinesElem->NextSiblingElement();
						for(i=0;i<num_SpecialLines;i++)
						{
							
							GetXmlSpecialLines(pTmpDtk_CurvePtrElem,&pStkUserLine);
							if(pStkUserLine)
								PmiEntityP->AddSpecialLine(pStkUserLine);
							pTmpDtk_CurvePtrElem = pTmpDtk_CurvePtrElem->NextSiblingElement();
						}

					}
				}
				
				ProtoP->AddPMI(PmiEntityP);
				
				while (pPMIElme = pPMIElme->NextSiblingElement())
				{
					if(stricmp(pPMIElme->Value(),"Dtk_FdtPtr")==0)
					{
							PmiEntityP = new Stk_PMI(ProtoP->GetID());

							//获取第N个PMI要素的ID数据
							pIdData = pPMIElme->FirstChildElement();
							pId =pIdData->FirstChild()->Value();
							STK_ID stkidPMIid = atoi(pId);
							PmiEntityP->SetID(stkidPMIid);

							//获取第N个PMI要素的平行到屏幕的标志
							pDispFlatScreenElem = pIdData->NextSiblingElement();
							pDispFlatScreen     = pDispFlatScreenElem->FirstChild()->Value();
							STK_ID stkDispFlat = atoi((pDispFlatScreen));
							PmiEntityP->SetParallelScreen(stkDispFlat);

							//获取第N个PMI要素的显隐状态
							pXXXXXXXXlElem = pDispFlatScreenElem->NextSiblingElement();
							pDtk_BlankedStatusElem = NULL;
							pColorElem = NULL;
							visualFlg = 0;

							if(stricmp(pXXXXXXXXlElem->Value(),"Dtk_BlankedStatus")==0)
							{	
								pDtk_BlankedStatusElem = pXXXXXXXXlElem;

								const char * pvisual = pDtk_BlankedStatusElem->FirstChild()->Value();
								visualFlg = atoi(pvisual);
								//获取第一个PMI要素的颜色数据
								pColorElem = pDtk_BlankedStatusElem->NextSiblingElement();
							}
							else
								//获取第一个PMI要素的颜色数据
								pColorElem = pXXXXXXXXlElem;

							PmiEntityP->SetDisplay(visualFlg?false:true);
							//获取颜色分量
							pColorElemX = pColorElem->FirstChildElement();
							

							while (pColorElemX)
							{
								const char *pColorX = pColorElemX->FirstChild()->Value();
								const char *pMemb= pColorElemX->Value();
								STK_FLOAT32 f = atof(pMemb);
								if(stricmp(pMemb,"r")==0)
									color.Red = f;
								else if(stricmp(pColorX,"g")==0)
									color.Green = f ;
								else if (stricmp(pColorX,"b")==0)
									color.Blue = f;
								else if(stricmp(pColorX,"a") == 0)
									color.Alpha = f;
								PmiEntityP->SetColor(color);

								pColorElemX = pColorElemX->NextSiblingElement();

							}
							//获取第N个PMI要素定义的平面
							pDefinePlaneElem = pColorElem->NextSiblingElement();
							//此处要变换为4*4的矩阵。
							
							GetXmlmatrix(pDefinePlaneElem,&PlaneMtx);
							//设置平面
							PmiEntityP->SetDefPlane(PlaneMtx);

							//获取第一个PMI要素定义的类型
							XMLElement *pTypeElem = pDefinePlaneElem->NextSiblingElement();
							const char *pType = pTypeElem->FirstChild()->Value();
							STK_UINT32 ntye = atoi(pType);
							//设置类型
							PmiEntityP->SetType(ntye);

							/*********************************************************
							 *************************获取Leader信息******************
							 *********************************************************/
							
							pLeadersElem = pTypeElem->NextSiblingElement();

							pnum_leadersElem = pLeadersElem->FirstChildElement();
							const  char*pnum_leaders = pnum_leadersElem->FirstChild()->Value();
							STK_UINT32 num_leaders = atoi(pnum_leaders);

							vector<Stk_Leader*> inLeader ;
							pDtk_LeaderPtrElem = pnum_leadersElem->NextSiblingElement();

							for(i=0;i<num_leaders;i++)
							{
								GetXmlLead(pDtk_LeaderPtrElem,inLeader);
								pDtk_LeaderPtrElem = pDtk_LeaderPtrElem->NextSiblingElement();
							}
							if(inLeader.size()>0)
								PmiEntityP->SetLeaders(inLeader);

							/**************************************************************
							 ***************************获取Frame的信息********************
							 **************************************************************/
							
				 
							pStkOutFrames = NULL;
							
							pDtk_FrameElem = pLeadersElem->NextSiblingElement();
							
							while (pDtk_FrameElem && stricmp(pDtk_FrameElem->Value(),"Dtk_Frame")==0)
							{
								GetXmlFrame(pDtk_FrameElem,&pStkOutFrames);
								if(pStkOutFrames)
									PmiEntityP->SetOutFrame(pStkOutFrames);

								pDtk_FrameElem = pDtk_FrameElem->NextSiblingElement();
							}


							//获取复合文本的信息
							
							pDtk_CompositeTextElem =pDtk_FrameElem;
							ppStkComText = NULL;
							while(pDtk_CompositeTextElem&&stricmp(pDtk_CompositeTextElem->Value(),"Dtk_CompositeText")==0)
							{
								GetXmlCompositeText(pDtk_CompositeTextElem,&ppStkComText);
								if(ppStkComText)
									PmiEntityP->AddComText(ppStkComText);
								pDtk_CompositeTextElem = pDtk_CompositeTextElem->NextSiblingElement();
							}
							/**************************************************************
							 ***************************获取延长线的信息********************
							 **************************************************************/
							pSpecialLinesElem = NULL;
							pext_linesElem =NULL;
							pXXX_linesElem=NULL ;
							//pXXX_linesElem = pDtk_CompositeTextElem->NextSiblingElement();
							pXXX_linesElem =pDtk_CompositeTextElem;
							
							vector<Stk_Extensionline*>inExLine;
							pStkUserLine = NULL;
							pTmpDtk_CurvePtrElem = NULL;
							pXXX_linesElem = pDtk_CompositeTextElem;
							if(pXXX_linesElem)
							{
								const char* tagNameP=pXXX_linesElem->Value();
								vector<Stk_Extensionline*>inExLine;
								Stk_SpecialLine *pStkUserLine = NULL;
								XMLElement *pTmpDtk_CurvePtrElem = NULL;
								if(stricmp(tagNameP,"ext_lines")==0)
								{
									pext_linesElem = pXXX_linesElem;
									XMLElement* pnum_ext_linesElem = pext_linesElem->FirstChildElement();
									const char* pnum_ext_lines= pnum_ext_linesElem->FirstChild()->Value();
									int num_ext_lines = atoi(pnum_ext_lines);
									pTmpDtk_CurvePtrElem = pnum_ext_linesElem->NextSiblingElement();
									for(i=0;i<num_ext_lines;i++)
									{
										GetXmlExt_Line(pTmpDtk_CurvePtrElem,inExLine);//设置线列
										pTmpDtk_CurvePtrElem = pTmpDtk_CurvePtrElem->NextSiblingElement();
									}
									if(inExLine.size()>0)
										PmiEntityP->SetExLines(inExLine);

								}
								else if(stricmp(tagNameP,"SpecialLines")==0)
								{

									pSpecialLinesElem=pXXX_linesElem;
									XMLElement*pnum_SpecialLinesElem = pSpecialLinesElem->FirstChildElement();
									const char* pnum_SpecialLines = pnum_SpecialLinesElem->FirstChild()->Value();
									int num_SpecialLines = atoi(pnum_SpecialLines);
									pTmpDtk_CurvePtrElem = pnum_SpecialLinesElem->NextSiblingElement();
									for(i=0;i<num_SpecialLines;i++)
									{
										
										GetXmlSpecialLines(pTmpDtk_CurvePtrElem,&pStkUserLine);
										if(pStkUserLine)
											PmiEntityP->AddSpecialLine(pStkUserLine);
										pTmpDtk_CurvePtrElem = pTmpDtk_CurvePtrElem->NextSiblingElement();
									}

								}

							}
							ProtoP->AddPMI(PmiEntityP);
							nPMI++;
					
					}
					else if(stricmp(pPMIElme->Value(),"Dtk_ModelDisplay")==0)
					{
						XMLElement *pDtk_ModelDisplayElem = pPMIElme;
						Stk_View * pTempView = NULL;

						GetXmlModelDisplay(pDtk_ModelDisplayElem,&pTempView);
						ProtoP->AddView(pTempView);
					}
				}		
			}
		}
		pProtoElme =  pProtoElme->NextSiblingElement();
	}
	return state; 
}

int CPmiDataXmlPattern::stricmp(const char* str1,const char* str2)
{
	string strA(str1);
	string strB(str2);
	transform(strA.begin(),strA.end(),strA.begin(),::toupper);
	transform(strB.begin(),strB.end(),strB.begin(),::toupper);

	return strA.compare(strB);
}
//#endif
