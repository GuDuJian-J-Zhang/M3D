#include "sview/pmiAnalysis/toolkit/PMIESymbol.h"
#include "sview/pmiAnalysis/toolkit/MathVector.h"
#include <string.h>

CPMIESymbol::CPMIESymbol(void)
{
}

CPMIESymbol::~CPMIESymbol(void)
{
}

int CPMIESymbol::SetESymbolInfo(double fHight, double fWidth, double* arrowpnt, double* arrowdir,double *pnormal, int symtype, CPOINT3Array &outPnts1,CPOINT3Array &outPnts2)
{
	//double arrowdir[3] = {0.0,0.0,0.0};
	//double refsize = 0.0;
	double symvec[3] = {0.0,0.0,0.0};
	double symnvec[3] = {0.0,0.0,0.0};

	double fcos = 0.0;
	double fsin = 0.0;
	double flong = 0.0;
	double fvalue = (fWidth/2);
	double arwlen = 3.0;

	double vec1S[3] =  {0.0,0.0,0.0};
	double vec2S[3] =  {0.0,0.0,0.0};
	double wkpntSA[2][3] = {{0.0,0.0,0.0},{0.0,0.0,0.0}};
	double cpnt[3] = {0.0,0.0,0.0};

	double symrad = 0.0;
	int dotpntnum = 37;
	double dotangle = PI/18.0;

	double slalen = 0.0;
	double slalen2 = 0.0;
	double slaang = PI/4.0;
	double slavecS[3] = {0.0,0.0,0.0};

	double trilen = 0.0;
	double spaclen = 0.0;
	double spacwid = 1.0;
	double trihgt = 0.0;
	double pntSA[4][3] = {{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0}};

	double symsize1 = 0.0;
	double symsize2 = 0.0;
	double pnt2SA[4][3] = {{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0}};
	double HVecA[3] = {1.0,0.0,0.0};
	double VVecA[3] = {0.0,1.0,0.0};

	double pnt3SA[4][3] = {{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0}};

	double tri2vec[3] = {0.0,0.0,0.0};
	double tri2len = 1.0;
	double tri2tmp[3] = {0.0,0.0,0.0};
	double tri2bdr = 1.0;

	double vecsqr1[3] = {0.0,0.0,0.0};
	POINT3 tmpPnt,tmpPnt2,tmpPnt3,centerPnt,sidePnt1,sidePnt2;

	int  		npsspnt =0;
	double      fsec = 0.0;
	memset(&tmpPnt,0,sizeof(POINT3));
	memset(&tmpPnt2,0,sizeof(POINT3));
	memset(&tmpPnt3,0,sizeof(POINT3));
	memset(&centerPnt,0,sizeof(POINT3));
	memset(&sidePnt1,0,sizeof(POINT3));
	memset(&sidePnt2,0,sizeof(POINT3));

	

	//单位化
	//refsize = sqrt(((arrowdir[0])*(arrowdir[0]))+((arrowdir[1])*(arrowdir[1]))+((arrowdir[2])*(arrowdir[2])));
	/*if(refsize <Sdistol)
	{
		判断
	}*/

	double dir_length = 0;
	CMathVector::LengthAndUnit(3,arrowdir,&dir_length,symvec);

	

	/*symnvec[0] = (-1)*symvec[1];
	symnvec[1] = symvec[0];
	
	symnvec[2] = 0;*/
	/*symnvec[0] = pnormal[0];
	symnvec[1] = pnormal[1];
	symnvec[2] = pnormal[2];*/
	CMathVector::LengthAndUnit(3,pnormal,&dir_length,symnvec);


    /*
	 * 箭头两边向量计算
	 */
	flong = sqrt( (fvalue*fvalue)+(fHight*fHight));
	fcos = fHight / flong;
	fsin = fvalue / flong ;

	vec1S[0] = symnvec[0];//symvec[0]*fcos-symvec[1]*fsin;
	vec1S[1] = symnvec[1];//symvec[0]*fsin+symvec[1]*fcos;
	vec1S[2] = symnvec[2];//0.0;

	vec2S[0] = -symnvec[0];// symvec[0]*fcos+symvec[1]*fsin;
	vec2S[1] = -symnvec[1];// -symvec[0]*fsin+symvec[1]*fcos;
	vec2S[2] = -symnvec[2];// 0.0;

	symrad = fWidth/2.0;
	slalen = flong/2.0;
	trihgt = fHight;
	trilen = fWidth;
	spaclen = fWidth;
	symsize1 = fWidth;
	symsize2 = fHight;
	tri2len = fWidth/2.0;
	tri2bdr = fHight/2.0;
	fsec = fHight/3*2;

	switch(symtype)
	{

		/*
    	 * 箭头/白箭头/黑箭头
    	 */
		case	TERM_PATSMARW:
		case	TERM_PATSMWHTARW:
		case	TERM_PATSMBLKARW:
		case	PATCROSSARW:
		
			//tmpPnt.x = arrowpnt[0] + flong*vec1S[0];
			//tmpPnt.y  = arrowpnt[1] + flong*vec1S[1];
			//tmpPnt.z  = /*0.0*/arrowpnt[2] /*+ flong* vec1S[2]*/;
			{

			
			//double basePnt[3] = {};
		
			/*basePnt[0] = arrowpnt[0]+fWidth*arrowdir[0];
			basePnt[1] = arrowpnt[1]+fWidth*arrowdir[1];
			basePnt[2] = arrowpnt[2]+fWidth*arrowdir[2];*/
		
			POINT3 basePnt;
			

			//MovPnt(3,&linvec,&srcpnt,fHight,&basePnt);
			basePnt.x = arrowpnt[0]+fHight*arrowdir[0];
			basePnt.y = arrowpnt[1]+fHight*arrowdir[1];
			basePnt.z = arrowpnt[2]+fHight*arrowdir[2];
			
			tmpPnt.x = basePnt.x + fWidth/2*vec1S[0];
			tmpPnt.y = basePnt.y + fWidth/2*vec1S[1];
			tmpPnt.z = basePnt.z + fWidth/2*vec1S[2];
			
			outPnts1.push_back(tmpPnt);

			tmpPnt.x = arrowpnt[0];
			tmpPnt.y = arrowpnt[1];
			tmpPnt.z = arrowpnt[2];
			outPnts1.push_back(tmpPnt);

			

			tmpPnt.x = basePnt.x+fWidth/2*vec2S[0];
			tmpPnt.y = basePnt.y+fWidth/2*vec2S[1];
			tmpPnt.z = basePnt.z+fWidth/2*vec2S[2];
		
			outPnts1.push_back(tmpPnt);

			if ( symtype == TERM_PATSMWHTARW) 
			{
				tmpPnt.x = basePnt.x + fWidth/2*vec1S[0];
				tmpPnt.y = basePnt.y + fWidth/2*vec1S[1];
				tmpPnt.z = basePnt.z + fWidth/2*vec1S[2];

				outPnts1.push_back(tmpPnt);
			}
        	}
			break;

		/*
    	 * 空心点/实心点
		 */
		case	TERM_PATSMWHTCIR:
		case	TERM_PATSMBLKCIR:
		case	TERM_PATSMCROSSEDCIR:

			if(symtype == TERM_PATSMBLKCIR)
			{
				dotpntnum = 6;
				dotangle = 2.0*PI/6.0;
			}

			for ( int i = 0; i < dotpntnum; i++) {
				tmpPnt.x  = arrowpnt[0]+symrad*cos( dotangle*i);
				tmpPnt.y  = arrowpnt[1]+symrad*sin( dotangle*i);
				tmpPnt.z  = /*0.0*/arrowpnt[2];
				outPnts1.push_back(tmpPnt);
			}
        	
			break;

		/*
    	 * 斜线 
		 */
		case	TERM_PATSMSLASH_TYPE:

			slalen2 = sqrt((fWidth*fWidth)+(fHight*fHight));

			slavecS[0] = symvec[0]*cos( slaang)-symvec[1]*sin( slaang);
			slavecS[1] = symvec[0]*sin( slaang)+symvec[1]*cos( slaang);
			slavecS[2] = 0.0;

			tmpPnt.x   = arrowpnt[0] + (slalen2/2)*slavecS[0];
			tmpPnt.y   = arrowpnt[1] + (slalen2/2)*slavecS[1];
			tmpPnt.z   = /*0.0*/arrowpnt[2];
			outPnts1.push_back(tmpPnt);

			tmpPnt.x   = tmpPnt.x - slalen2*slavecS[0];
			tmpPnt.y   = tmpPnt.y - slalen2*slavecS[1];
			tmpPnt.z   = /*0.0*/arrowpnt[2];
			outPnts1.push_back(tmpPnt);
			
			break;

		/*
    	 * 白三角/黑三角/无底边三角/横线
		 */
		case	TERM_PATSMWHTTRI_TYPE:
		case	TERM_PATSMBLKTRI_TYPE:
		case	TERM_PATSMWHTTRI2_TYPE:
		case	PATSSPACELIN_TYPE:

			pntSA[0][0] = arrowpnt[0]+trihgt*symvec[0];
			pntSA[0][1] = arrowpnt[1]+trihgt*symvec[1];
			pntSA[0][2] = 0.0;		

			if(symtype == PATSSPACELIN_TYPE){
				pntSA[1][0] = arrowpnt[0]+spaclen/2.0*symvec[1]+spacwid*symvec[0];     
				pntSA[1][1] = arrowpnt[1]-spaclen/2.0*symvec[0]+spacwid*symvec[1];     
				pntSA[1][2] = 0.0;                                                   
				pntSA[2][0] = arrowpnt[0]-spaclen/2.0*symvec[1]+spacwid*symvec[0];     
				pntSA[2][1] = arrowpnt[1]+spaclen/2.0*symvec[0]+spacwid*symvec[1];     
				pntSA[2][2] = 0.0;                                                   
			}
			else{
				pntSA[1][0] = arrowpnt[0]+trilen/2.0*symvec[1];
				pntSA[1][1] = arrowpnt[1]-trilen/2.0*symvec[0];
				pntSA[1][2] = 0.0;
				pntSA[2][0] = arrowpnt[0]-trilen/2.0*symvec[1];
				pntSA[2][1] = arrowpnt[1]+trilen/2.0*symvec[0];
				pntSA[2][2] = 0.0;
			}
			pntSA[3][0] = arrowpnt[0]+trihgt*symvec[0];
			pntSA[3][1] = arrowpnt[1]+trihgt*symvec[1];
			pntSA[3][2] = 0.0;

			if(symtype == PATSSPACELIN_TYPE)
			{
				
				tmpPnt.x = pntSA[1][0];
				tmpPnt.y = pntSA[1][1];
				tmpPnt.z = /*0.0*/arrowpnt[2];
				outPnts1.push_back(tmpPnt);
				
				tmpPnt.x = pntSA[2][0];
				tmpPnt.y = pntSA[2][1];
				tmpPnt.z = /*0.0*/arrowpnt[2];
				outPnts1.push_back(tmpPnt);
				
				tmpPnt.x = pntSA[2][0];
				tmpPnt.y = pntSA[2][1];
				tmpPnt.z = /*0.0*/arrowpnt[2];
				outPnts1.push_back(tmpPnt);
				
			}
			else{
				

				tmpPnt.x  = pntSA[2][0];
				tmpPnt.y  = pntSA[2][1];
				tmpPnt.z  = /*0.0*/arrowpnt[2];
				outPnts1.push_back(tmpPnt);

				tmpPnt.x  = pntSA[0][0];
				tmpPnt.y  = pntSA[0][1];
				tmpPnt.z  = /*0.0*/arrowpnt[2];
				outPnts1.push_back(tmpPnt);

				tmpPnt.x  = pntSA[1][0];
				tmpPnt.y  = pntSA[1][1];
				tmpPnt.z  = /*0.0*/arrowpnt[2];
				outPnts1.push_back(tmpPnt);

				if (symtype == TERM_PATSMWHTTRI_TYPE)
				{
					
					tmpPnt.x = pntSA[2][0];
					tmpPnt.y = pntSA[2][1];
					tmpPnt.z = /*0.0*/arrowpnt[2];
					outPnts1.push_back(tmpPnt);
				}
			}
			
			break;

		/*
    	 * 十字
		 */
		//case	TERM_PATSMCRSRL:
		//case	TERM_PATSMCRS1L:
		case	TERM_PATSMCRSBIG:

			 pnt2SA[0][0] = arrowpnt[0]-(symsize1/2.0)*HVecA[0];
			 pnt2SA[0][1] = arrowpnt[1]-(symsize1/2.0)*HVecA[1];
			 pnt2SA[0][2] = 0.0;
			 pnt2SA[1][0] = arrowpnt[0]+(symsize1/2.0)*HVecA[0];
			 pnt2SA[1][1] = arrowpnt[1]+(symsize1/2.0)*HVecA[1];
			 pnt2SA[1][2] = 0.0;
			 pnt2SA[2][0] = arrowpnt[0]-(symsize2/2.0)*VVecA[0];
			 pnt2SA[2][1] = arrowpnt[1]-(symsize2/2.0)*VVecA[1];
			 pnt2SA[2][2] = 0.0;
			 pnt2SA[3][0] = arrowpnt[0]+(symsize2/2.0)*VVecA[0];
			 pnt2SA[3][1] = arrowpnt[1]+(symsize2/2.0)*VVecA[1];
			 pnt2SA[3][2] = 0.0;

			 tmpPnt.x = pnt2SA[0][0];
			 tmpPnt.y = pnt2SA[0][1];
			 tmpPnt.z = /*0.0*/arrowpnt[2];
			 outPnts1.push_back(tmpPnt);

			 tmpPnt.x  = pnt2SA[1][0];
			 tmpPnt.y  = pnt2SA[1][1];
			 tmpPnt.z  = /*0.0*/arrowpnt[2];
			 outPnts1.push_back(tmpPnt);

			 tmpPnt.x  = arrowpnt[0];
			 tmpPnt.y  = arrowpnt[1];
			 tmpPnt.z  = /*0.0*/arrowpnt[2];
			 outPnts1.push_back(tmpPnt);

			 tmpPnt.x  = pnt2SA[2][0];
			 tmpPnt.y  = pnt2SA[2][1];
			 tmpPnt.z  = /*0.0*/arrowpnt[2];
			 outPnts1.push_back(tmpPnt);

			 tmpPnt.x  = pnt2SA[3][0];
			 tmpPnt.y  = pnt2SA[3][1];
			 tmpPnt.z  = /*0.0*/arrowpnt[2];
			 outPnts1.push_back(tmpPnt);

			break;

		/*
    	 * 矩形/填充矩形/X型十字
		 */
		case TERM_PATSMWHTSQR://引线末端位置在矩形中心。
		case TERM_PATSMBLKSQR:
		case TERM_PATSMCRSX:
			
			tmpPnt.x = arrowpnt[0] + flong*vec1S[0] - tri2bdr*symvec[0];
			tmpPnt.y = arrowpnt[1] + flong*vec1S[1] - tri2bdr*symvec[1];
			tmpPnt.z =/* 0.0*/arrowpnt[2];
			outPnts1.push_back(tmpPnt);

			tmpPnt.x = arrowpnt[0] + flong*vec1S[0] - 3*tri2bdr*symvec[0];
			tmpPnt.y = arrowpnt[1] + flong*vec1S[1] - 3*tri2bdr*symvec[1];
			tmpPnt.z = /*0.0*/arrowpnt[2];
			if(symtype == TERM_PATSMCRSX){
				outPnts2.push_back(tmpPnt);
			}
			else{
				outPnts1.push_back(tmpPnt);
			}

			tmpPnt.x = arrowpnt[0] + flong*vec2S[0] - 3*tri2bdr*symvec[0];
			tmpPnt.y = arrowpnt[1] + flong*vec2S[1] - 3*tri2bdr*symvec[1];
			tmpPnt.z = /*0.0*/arrowpnt[2];
			outPnts1.push_back(tmpPnt);

			tmpPnt.x = arrowpnt[0] + flong*vec2S[0] - tri2bdr*symvec[0];
			tmpPnt.y = arrowpnt[1] + flong*vec2S[1] - tri2bdr*symvec[1];
			tmpPnt.z = /*0.0*/arrowpnt[2];
			if(symtype == TERM_PATSMCRSX){
				outPnts2.push_back(tmpPnt);
			}
			else{
				outPnts1.push_back(tmpPnt);
			}

			
			if(symtype == TERM_PATSMWHTSQR)
			{
				tmpPnt.x = arrowpnt[0] + flong*vec1S[0] - tri2bdr*symvec[0];
				tmpPnt.y = arrowpnt[1] + flong*vec1S[1] - tri2bdr*symvec[1];
				tmpPnt.z = /*0.0*/arrowpnt[2];
				outPnts1.push_back(tmpPnt);
			}

			break;

		/*
    	 * 引线指向斜边的三角/填充三角
		 */
		case TERM_PATSMWHTTRI3:
		case TERM_PATSMBLKTRI2:

			tri2vec[0] = -symvec[1];
			tri2vec[1] = symvec[0];
			tri2vec[2] = 0.0;

			tri2tmp[0] = arrowpnt[0] + tri2len*tri2vec[0];
			tri2tmp[1] = arrowpnt[1] + tri2len*tri2vec[1];
			tri2tmp[2] = 0.0;

			tmpPnt.x  = arrowpnt[0] - tri2len*tri2vec[0];
			tmpPnt.y  = arrowpnt[1] - tri2len*tri2vec[1];
			tmpPnt.z  = /*0.0*/arrowpnt[2];
			outPnts1.push_back(tmpPnt);

			tmpPnt.x = tri2tmp[0] + tri2bdr*symvec[0];
			tmpPnt.y = tri2tmp[1] + tri2bdr*symvec[1];
			tmpPnt.z = /*0.0*/arrowpnt[2];
			outPnts1.push_back(tmpPnt);

			tmpPnt.x = tri2tmp[0] - tri2bdr*symvec[0];
			tmpPnt.y = tri2tmp[1] - tri2bdr*symvec[1];
			tmpPnt.z = /*0.0*/arrowpnt[2];
			outPnts1.push_back(tmpPnt);
		
			if(symtype == TERM_PATSMWHTTRI3)
			{
				tmpPnt.x  = arrowpnt[0] - tri2len*tri2vec[0];
				tmpPnt.y  = arrowpnt[1] - tri2len*tri2vec[1];
				tmpPnt.z  = /*0.0*/arrowpnt[2];
				outPnts1.push_back(tmpPnt);
			}

			break;

		/*
    	 * 双箭头
		 */
		case TERM_PATSMDBOPNARW:

			flong = sqrt((fsec*fsec)+(fvalue*fvalue));
			fcos = fsec / flong;
			fsin = fvalue / flong ;

			vec1S[0] = symvec[0]*fcos-symvec[1]*fsin;
			vec1S[1] = symvec[0]*fsin+symvec[1]*fcos;
			vec1S[2] = 0.0;

			vec2S[0] = symvec[0]*fcos+symvec[1]*fsin;
			vec2S[1] = -symvec[0]*fsin+symvec[1]*fcos;
			vec2S[2] = 0.0;

			tmpPnt.x = arrowpnt[0] + vec1S[0]*flong;
			tmpPnt.y = arrowpnt[1] + vec1S[1]*flong;
			tmpPnt.z = /*0.0*/arrowpnt[2];
			outPnts1.push_back(tmpPnt);

			tmpPnt.x = arrowpnt[0];
			tmpPnt.y = arrowpnt[1];
			tmpPnt.z = /*0.0*/arrowpnt[2];
			outPnts1.push_back(tmpPnt);

			tmpPnt.x = arrowpnt[0] + vec2S[0]*flong;
			tmpPnt.y = arrowpnt[1] + vec2S[1]*flong;
			tmpPnt.z = /*0.0*/arrowpnt[2];
			outPnts1.push_back(tmpPnt);


			//计算第二个线列
			tmpPnt.x = arrowpnt[0]+symvec[0]*(fsec/2);  //第二个箭头的始点
			tmpPnt.y = arrowpnt[1]+symvec[1]*(fsec/2);
			tmpPnt.z = arrowpnt[2];

			tmpPnt2.x = tmpPnt.x + vec1S[0]*flong;
			tmpPnt2.y = tmpPnt.y + vec1S[1]*flong;
			tmpPnt2.z = arrowpnt[2];

			outPnts2.push_back(tmpPnt2); //第一个点

			outPnts2.push_back(tmpPnt);   //第二个点


			tmpPnt2.x =  tmpPnt.x + vec2S[0]*flong;
			tmpPnt2.y =  tmpPnt.y + vec2S[1]*flong;
			tmpPnt2.z =  /*0.0*/arrowpnt[2];
			outPnts2.push_back(tmpPnt2); //第三个点

			break;

		/*
    	 * 双白箭头/双填充箭头
		 */
		case TERM_PATSMDBCLSARW:
		case TERM_PATSMDBFILLARW:
		
			flong = sqrt( (fsec*fsec)+(fvalue*fvalue));
			fcos = fsec / flong;
			fsin = fvalue / flong ;

			vec1S[0] = symvec[0]*fcos-symvec[1]*fsin;
			vec1S[1] = symvec[0]*fsin+symvec[1]*fcos;
			vec1S[2] = 0.0;

			vec2S[0] = symvec[0]*fcos+symvec[1]*fsin;
			vec2S[1] = -symvec[0]*fsin+symvec[1]*fcos;
			vec2S[2] = 0.0;

			tmpPnt.x = arrowpnt[0] + vec1S[0]*flong;
			tmpPnt.y = arrowpnt[1] + vec1S[1]*flong;
			tmpPnt.z = /*0.0*/arrowpnt[2];
			memcpy(&sidePnt1,&tmpPnt,sizeof(POINT3));
			outPnts1.push_back(tmpPnt);

			tmpPnt.x = arrowpnt[0];
			tmpPnt.y = arrowpnt[1];
			tmpPnt.z = /*0.0*/arrowpnt[2];
			outPnts1.push_back(tmpPnt);		

			tmpPnt.x = arrowpnt[0] + vec2S[0]*flong;
			tmpPnt.y = arrowpnt[1] + vec2S[1]*flong;
			tmpPnt.z = /*0.0*/arrowpnt[2];
			memcpy(&sidePnt2,&tmpPnt,sizeof(POINT3));
			outPnts1.push_back(tmpPnt);

			if( TERM_PATSMDBCLSARW == symtype )
			{
				tmpPnt.x = arrowpnt[0] + vec1S[0]*flong;
				tmpPnt.y = arrowpnt[1] + vec1S[1]*flong;
				tmpPnt.z = /*0.0*/arrowpnt[2];
				outPnts1.push_back(tmpPnt);
			}

			//计算第二个线列

			//计算第一个箭头的中心点
			centerPnt.x = arrowpnt[0]+fsec/2*(symvec[0]);
			centerPnt.y = arrowpnt[1]+fsec/2*(symvec[1]);
			centerPnt.z = 0;

			//计算第二个箭头底边的两个点
			sidePnt1.x = sidePnt1.x+fsec/2*(symvec[0]);
			sidePnt1.y = sidePnt1.y+fsec/2*(symvec[1]);
			sidePnt1.z = /*0*/arrowpnt[2];
			if(TERM_PATSMDBCLSARW == symtype)
				outPnts2.push_back(sidePnt1);
			else
				outPnts1.push_back(sidePnt1);

			tmpPnt3.x = sidePnt1.x;
			tmpPnt3.y = sidePnt1.y;
			tmpPnt3.z = sidePnt1.z;

			sidePnt2.x = sidePnt2.x+fsec/2*(symvec[0]);
			sidePnt2.y = sidePnt2.y+fsec/2*(symvec[1]);
			sidePnt2.z = /*0*/arrowpnt[2];
			if(TERM_PATSMDBCLSARW == symtype)
				outPnts2.push_back(sidePnt2);
			else
				outPnts1.insert(outPnts1.begin()+3, sidePnt2);
			
			//计算第二个箭头上边的两个点

			sidePnt2.x=(sidePnt2.x+centerPnt.x)/2;
			sidePnt2.y=(sidePnt2.y+centerPnt.y)/2;
			sidePnt2.z=/*0*/arrowpnt[2];
			if(TERM_PATSMDBCLSARW == symtype)
				outPnts2.push_back(sidePnt2);
			else
				outPnts1.insert(outPnts1.begin()+3, sidePnt2);

			sidePnt1.x=(sidePnt1.x+centerPnt.x)/2;
			sidePnt1.y=(sidePnt1.y+centerPnt.y)/2;
			sidePnt1.z=/*0*/arrowpnt[2];
			if(TERM_PATSMDBCLSARW == symtype)
				outPnts2.push_back(sidePnt1);
			else
				outPnts1.push_back(sidePnt1);

			if(TERM_PATSMDBCLSARW == symtype)
				outPnts2.push_back(tmpPnt3);

			break;

		/*
    	 * 逆向双填充箭头
		 */
		case TERM_PATSMDBFILLTRI:

			flong = sqrt( (fsec*fsec)+(fvalue*fvalue));
			fcos = fsec / flong;
			fsin = fvalue / flong ;

			vec1S[0] = symvec[0]*fcos-symvec[1]*fsin;
			vec1S[1] = symvec[0]*fsin+symvec[1]*fcos;
			vec1S[2] = 0.0;

			vec2S[0] = symvec[0]*fcos+symvec[1]*fsin;
			vec2S[1] = -symvec[0]*fsin+symvec[1]*fcos;
			vec2S[2] = 0.0;

			//计算第一个三角的顶点
			tmpPnt.x = arrowpnt[0] + symvec[0]*fHight;
			tmpPnt.y = arrowpnt[1] + symvec[1]*fHight;
			tmpPnt.z = arrowpnt[2];

			//临时变量存储第一个三角的顶点
			tmpPnt2.x = tmpPnt.x;
			tmpPnt2.y = tmpPnt.y;
			tmpPnt2.z = tmpPnt.z;

			//第一个三角底边的点1
			tmpPnt.x = tmpPnt.x-vec1S[0]*flong;
			tmpPnt.y = tmpPnt.y-vec1S[1]*flong;
			tmpPnt.z = arrowpnt[2];
			outPnts1.push_back(tmpPnt);

			//第一个三角的顶点
			
			outPnts1.push_back(tmpPnt2);


			//第一个三角底边的点2
			tmpPnt.x =  tmpPnt2.x - vec2S[0]*flong;
			tmpPnt.y =  tmpPnt2.y - vec2S[1]*flong;
			tmpPnt.z =  /*0.0*/arrowpnt[2];
			outPnts1.push_back(tmpPnt);

			//通过第一个三角形顶点计算第二个三角的中心点
			centerPnt.x = arrowpnt[0] + symvec[0]*fsec;
			centerPnt.y = arrowpnt[1] + symvec[1]*fsec;
			centerPnt.z = 0;

			//计算第二个箭头底边的两个点

			sidePnt2.x = arrowpnt[0]+fvalue*(symnvec[0]);
			sidePnt2.y = arrowpnt[1]+fvalue*(symnvec[1]);
			sidePnt2.z = /*0*/arrowpnt[2];
			outPnts1.push_back( sidePnt2);

			sidePnt1.x = arrowpnt[0]-fvalue*(symnvec[0]);
			sidePnt1.y = arrowpnt[1]-fvalue*(symnvec[1]);
			sidePnt1.z = /*0*/arrowpnt[2];
			outPnts1.push_back(sidePnt1);
			

			sidePnt2.x=(sidePnt2.x+centerPnt.x)/2;
			sidePnt2.y=(sidePnt2.y+centerPnt.y)/2;
			sidePnt2.z=arrowpnt[2];

			outPnts1.insert(outPnts1.begin()+3, sidePnt2);
			
			sidePnt1.x=(sidePnt1.x+centerPnt.x)/2;
			sidePnt1.y=(sidePnt1.y+centerPnt.y)/2;
			sidePnt1.z=arrowpnt[2];
			outPnts1.push_back(sidePnt1);
			
			break;

		/*
		 * 没有末端形状 
		 */
		case	TERM_PATSMNO:

			break;

		default:
			tmpPnt.x = arrowpnt[0] + flong*vec1S[0];
			tmpPnt.y  = arrowpnt[1] + flong*vec1S[1];
			tmpPnt.z  = arrowpnt[2];
			outPnts1.push_back(tmpPnt);

			tmpPnt.x = arrowpnt[0];
			tmpPnt.y = arrowpnt[1];
			tmpPnt.z = arrowpnt[2];
			outPnts1.push_back(tmpPnt);

			tmpPnt.x = arrowpnt[0] + flong*vec2S[0];
			tmpPnt.y = arrowpnt[1] + flong*vec2S[1];
			tmpPnt.z = arrowpnt[2];
			outPnts1.push_back(tmpPnt);

			
			tmpPnt.x = arrowpnt[0] + flong*vec1S[0];
			tmpPnt.y = arrowpnt[1] + flong*vec1S[1];
			tmpPnt.z = arrowpnt[2];
			outPnts1.push_back(tmpPnt);
			

			break;
	}
	return 0;
}
