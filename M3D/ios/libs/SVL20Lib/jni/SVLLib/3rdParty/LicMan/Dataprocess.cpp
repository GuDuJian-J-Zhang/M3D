//#include "..\..\StdAfx.h"
#include "DataProcess.h"
#include <ctime>


//////////////////////////////////////////////////////////////////////////

//���캯��
CDataProcess::CDataProcess()
{
}

//��������
CDataProcess::~CDataProcess()
{
}

//���ݻ�ԭ
bool CDataProcess::RestoreCode(string strRegCode, RegisterCode& RegCode)
{
	string strProInfo;
	string strMac1,strMac2,strSpaceDate;
	string strProFlag,strProEdi,strVersion;

	// ע����λ���ж�
	int iLength = strRegCode.length();
	if (!(iLength == 8 || iLength == 6)) return false;

	// ��ø�����Ϣ
	strProInfo = strRegCode.substr(0, 2);
	strMac1 = strRegCode.substr(2, 2);
	if (iLength == 8)
	{
		strMac2 = strRegCode.substr(4, 2);
		strSpaceDate = strRegCode.substr(6, 2);
	}
	else
	{
		strSpaceDate = strRegCode.substr(4, 2);
	}

	// ��ȡ��Ʒ��Ϣ
	strProInfo = DigitDecrypt(strProInfo);
	strProFlag = strProInfo.substr(0, 1);
	strProEdi = strProInfo.substr(1, 1);
	strVersion = strProInfo.substr(strProInfo.length() - 2, 2);

	// ����У��
	if (!IsDigitStr(strProFlag) || !IsDigitStr(strProEdi) || !IsDigitStr(strVersion)) return false;

	// ��ȡƫ��ʱ��
	strSpaceDate = DigitDecrypt(strSpaceDate);
	if (strSpaceDate == "**")
	{
		strSpaceDate.empty();
	}

	// ���ݸ�ֵ
	RegCode.iProFlag = atoi(strProFlag.c_str());
	RegCode.iProEdition = atoi(strProEdi.c_str());
	strcpy_s(RegCode.szSpaceDate,strSpaceDate.c_str());
	strcpy_s(RegCode.szProVer,strVersion.c_str());
	strcpy_s(RegCode.szMac1,strMac1.c_str());
	if (strMac2.size() > 0)
	{
		strcpy_s(RegCode.szMac2,strMac2.c_str());
	}

	return true;
}

// �ж��ַ����Ƿ�Ϊ����
bool CDataProcess::IsDigitStr(string strContent)
{
	int len = strContent.length();
	for (int i = 0 ; i < len ; i ++)
	{
		if (strContent.at(i)>'9' || strContent.at(i)<'0')
		{
			return false;
		}
	}

	return true;
}

// ��ѹ��
string CDataProcess::DigitDecrypt(string strIn)
{
	int ilength = strIn.length();

	unsigned char* temp = new unsigned char[ilength*2+1];
	memset(temp,0,ilength*2+1);

	string strDecrypt;
	for (int i=0; i<ilength; i++)//��1�ֽڽ�������ֽ� ********
	{
		unsigned char chTemp = strIn.at(i);
		if (chTemp == 255)
		{
			temp[2*i+1]='0';
			temp[2*i]='0';
		}
		else
		{
			temp[2*i+1] = (chTemp>>4)+48;
			temp[2*i] = (chTemp&15)+48;
		}
	}
	strDecrypt = (char*)temp;
	if (temp != NULL)
	{
		delete[] temp;
	}

	return strDecrypt;
}

// �õ�����ʱ��
string CDataProcess::GetProperTime(string strSpacetime)
{
	int iSpacetime = atoi(strSpacetime.c_str());
	string strProperTime;
// 	CTime cTimeBase(2014,06,01,0,0,0);
// 
// 	CTimeSpan tSpace(iSpacetime,0,0,0);
// 
// 	CTime CTimeProper = cTimeBase + tSpace;
// 
// 	CString szProperTime;
// 	szProperTime = CTimeProper.Format("%Y%m%d");
// 	
// 	// CString->string
// 	string strProperTime = "";
// 	wstring wstrProperTime = szProperTime.GetString();
// 	int nLength = WideCharToMultiByte(CP_ACP, 0, wstrProperTime.c_str(), -1, NULL, NULL,NULL,NULL);
// 	strProperTime.resize(nLength);
// 	LPSTR lpszStr = new char[nLength];
// 	WideCharToMultiByte(CP_ACP, 0, wstrProperTime.c_str(), -1, lpszStr, nLength,NULL,NULL);
// 	strProperTime = lpszStr;
// 	delete [] lpszStr;

	return strProperTime;
}

// ��ȡ����ʱ��
int CDataProcess::GetSpaceDate(string strTime)
{
// 	//�õ���ǰʱ��
// 	CTime cCurTime;
// 	cCurTime = cCurTime.GetCurrentTime();
// 
// 	//��ȡʱ��
// 	int iYear,iMonth,iDay,iHour,iMin;
// 	iYear = atoi(strTime.substr(0, 4).c_str());
// 	iMonth = atoi(strTime.substr(4, 2).c_str());
// 	iDay = atoi(strTime.substr(6, 2).c_str());
// 	iHour = atoi(strTime.substr(8, 2).c_str());
// 	iMin = atoi(strTime.substr(strTime.length() - 2, 2).c_str());
// 	CTime cEndTime(iYear,iMonth,iDay,iHour,iMin,0);
// 
// 	CTimeSpan ts = cEndTime - cCurTime;	//��������ʹ�ü������
// 
// 	LONGLONG lSpan = ts.GetDays();
// 
// 	return (int)lSpan;

	return 0;
}