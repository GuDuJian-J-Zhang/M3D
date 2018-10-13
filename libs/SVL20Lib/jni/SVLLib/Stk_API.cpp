#include <map>
#include "Stk_API.h"
#include "Stk_Log.h"
#include <stdlib.h>
#ifdef _WIN32
#include <wtypes.h>
#include <tchar.h>
#endif
#include "document.h"
#include "stringbuffer.h"
#include "writer.h"
#include "ChineseCode.h"
#include "Stk_Utility.h"
using namespace std;
using rapidjson::Value;
using rapidjson::Document;
using rapidjson::StringBuffer;
using rapidjson::Writer;
using rapidjson::StringRef;
namespace HoteamSoft {
	namespace SVLLib {

		typedef std::map<unsigned int, Stk_DocumentPtr> DocMAP;
		typedef std::map<unsigned int, unsigned long > ColorMAP;

#ifndef WIN32
		static void _split_whole_name(const char *whole_name, char *fname, char *ext)
		{
			char *p_ext;
			p_ext = strstr((char *)whole_name, ".");
			if (NULL != p_ext)
			{
				strcpy(ext, p_ext);
				snprintf(fname, p_ext - whole_name + 1, "%s", whole_name);
			}
			else
			{
				ext[0] = '\0';
				strcpy(fname, whole_name);
			}
		}
		void _splitpath(const char *path, char *drive, char *dir, char *fname, char *ext)
		{
			char *p_whole_name;
			drive[0] = '\0';
			if (NULL == path)
			{
				dir[0] = '\0';
				fname[0] = '\0';
				ext[0] = '\0';
				return;
			}
			if ('/' == path[strlen(path)])
			{
				strcpy(dir, path);
				fname[0] = '\0';
				ext[0] = '\0';
				return;
			}
			p_whole_name = strstr((char *)path, "/");
			if (NULL != p_whole_name)
			{
				p_whole_name++;
				_split_whole_name(p_whole_name, fname, ext);
				snprintf(dir, p_whole_name - path, "%s", path);
			}
			else
			{
				_split_whole_name(path, fname, ext);
				dir[0] = '\0';
			}
		}

		//¥”INIŒƒº˛∂¡»°◊÷∑˚¥Æ¿‡–Õ ˝æ›
		char *GetIniKeyString(char *title,char *key,char *filename)
		{
		    FILE *fp;
		    char szLine[1024];
		    static char tmpstr[1024];
		    int rtnval;
		    int i = 0;
		    int flag = 0;
		    char *tmp;

		    if((fp = fopen(filename, "r")) == NULL)
		    {
		        printf("have   no   such   file \n");
		        return "";
		    }
		    while(!feof(fp))
		    {
		        rtnval = fgetc(fp);
		        if(rtnval == EOF)
		        {
		            break;
		        }
		        else
		        {
		            szLine[i++] = rtnval;
		        }
		        if(rtnval == '\n')
		        {
		#ifndef WIN32
		            i--;
		#endif
		            szLine[--i] = '\0';
		            i = 0;
		            tmp = strchr(szLine, '=');

		            if(( tmp != NULL )&&(flag == 1))
		            {
		                if(strstr(szLine,key)!=NULL)
		                {
		                    //◊¢ Õ––
		                    if ('#' == szLine[0])
		                    {
		                    }
		                    else if ( '\/' == szLine[0] && '\/' == szLine[1] )
		                    {

		                    }
		                    else
		                    {
		                        //’“¥Úkey∂‘”¶±‰¡ø
		                        strcpy(tmpstr,tmp+1);
		                        fclose(fp);
		                        return tmpstr;
		                    }
		                }
		            }
		            else
		            {
		                strcpy(tmpstr,"[");
		                strcat(tmpstr,title);
		                strcat(tmpstr,"]");
		                if( strncmp(tmpstr,szLine,strlen(tmpstr)) == 0 )
		                {
		                    //’“µΩtitle
		                    flag = 1;
		                }
		            }
		        }
		    }
		    fclose(fp);
		    return "";
		}

		//¥”INIŒƒº˛∂¡»°’˚¿‡–Õ ˝æ›
		int GetIniKeyInt(char *title,char *key,char *filename)
		{
		    return atoi(GetIniKeyString(title,key,filename));
		}

#endif

//!< ∞¸∫¨µƒ ˝æ›∂®“Â
struct Stk_API::Stk_Handle
{
	DocMAP m_pDocuments;	//!< À˘“™π‹¿ÌµƒŒƒµµ
	ColorMAP m_pColorMap;  //—’…´∆•≈‰±Ì
	Stk_LogPtr						m_pLog;			//!< »’÷æ
};

Stk_API::Stk_API(void):_Private(new Stk_Handle)
{
	_Private->m_pLog = Stk_LogPtr::CreateObject();
	//≥ı ºªØ—’…´∆•≈‰±Ì
	InitColorMap();
}

Stk_API::~Stk_API(void)
{
	if (NULL != _Private)
	{
		delete _Private;
		_Private = NULL;
	}
}

//!< Stk_DocumentManagerµƒππ‘Ï∫Ø ˝£¨‘⁄¥¥Ω®Stk_DocumentManager ±◊‘∂Ø—π»ÎAPI
//Stk_DocumentManager::Stk_DocumentManager(void):Stk_Object(new Stk_DocumentManagerImpl)
//{
//	Stk_DocumentManagerPtr pDoc(this);
//	if (STK_APP()->_Private->m_pDocuments.find(pDoc) < 0)
//		STK_APP()->_Private->m_pDocuments.push_back(pDoc);
//}
//!< µ•¿˝∏≥÷µ
Stk_API *Stk_API::_CurrentAPI = NULL;
/**************************************************************************
 *	@brief		ªÒ»°API
 *	@param[in]	Œﬁ
 *	@param[out]	Œﬁ
 *	@retval		÷∏’Î
 *	@note		Œﬁ
 *	@attention	Œﬁ
**************************************************************************/
Stk_API* Stk_API::GetAPI()
{
	if (NULL == _CurrentAPI)
	{
		_CurrentAPI = new Stk_API;
	}
	return _CurrentAPI;
}
/**************************************************************************
 *	@brief		Ω· ¯API
 *	@param[in]	Œﬁ
 *	@param[out]	Œﬁ
 *	@retval		Œﬁ
 *	@note		Œﬁ
 *	@attention	 π”√API∫Û“™µ˜”√¥ÀΩ”ø⁄Ω¯––πÿ±’
**************************************************************************/
void Stk_API::EndAPI()
{
	if (NULL != _CurrentAPI)
	{
		delete _CurrentAPI;
		_CurrentAPI = NULL;
	}	
}

/**********************************************************************//**
 *	@brief		…Ë÷√»’÷æŒƒº˛
 *	@param[in]	Stk_Log_Type i_type		»’÷æ–¥»Î∑Ω Ω£®–¥»ÎŒƒº˛ªÚøÿ÷∆Ã®£©
 *	@param[in]	Stk_Log_Level i_level	»’÷æº∂±£¨µÕ”⁄¥Àº∂±‘Ú≤ª¥Ú”°£¨
										ƒ¨»œ◊Ó∏ﬂΩÁ±
 *	@param[in]	Stk_string i_strPath	»’÷æ¬∑æ∂£¨–¥»Îøÿ÷∆Ã® ±¥À÷µ≤ª–Ë…Ë÷√
 *	@param[out]	Œﬁ
 *	@retval		≥…π¶:STK_SUCCESS		 ß∞‹:∆‰À˚
 *	@note		Œﬁ
 *	@attention	Œﬁ
**************************************************************************/
STK_STATUS Stk_API::SetLog(Stk_Log_Type i_type, 
						   Stk_Log_Level i_level /* = STK_LOG_LEVEL_HIGHEST */, 
						   Stk_string i_strPath /* = L"" */)
{
	if (_Private->m_pLog.isNull())
	{
		return STK_ERROR;
	}
	_Private->m_pLog->SetLogType(i_type);
	_Private->m_pLog->SetLogLevel(i_level);
	_Private->m_pLog->SetLogPath(i_strPath);

	return STK_SUCCESS;
}
/**********************************************************************//**
 *	@brief		¥Ú”°»’÷æ
 *	@param[in]	Stk_string i_strMsg	»’÷æ–≈œ¢
 *	@param[in]	Stk_Log_Level i_level	»’÷æº∂±
 *	@param[in]	int i_type				»’÷æ∏Ωº”–≈œ¢£¨ƒ¨»œ≤ª∏Ωº”–≈œ¢£¨
										STK_LOG_ADDITION_TIME:∏Ωº” ±º‰–≈œ¢
 *	@param[out]	Œﬁ
 *	@retval		≥…π¶:STK_SUCCESS		 ß∞‹:∆‰À˚
 *	@note		Œﬁ
 *	@attention	Œﬁ
**************************************************************************/
STK_STATUS Stk_API::WriteLog(Stk_string i_strMsg, 
							 Stk_Log_Level i_level /* = STK_LOG_LEVEL_HIGHEST */, 
							 int i_type /* = STK_LOG_ADDITION_NONE */)
{
	if (_Private->m_pLog.isNull())
	{
		return STK_ERROR;
	}

	return _Private->m_pLog->PrintLog(i_strMsg, i_level, i_type);
}
/**********************************************************************//**
 *	@brief		»’÷æ÷–ªª––
 *	@param[in]	Œﬁ
 *	@param[out]	Œﬁ
 *	@retval		≥…π¶:STK_SUCCESS		 ß∞‹:∆‰À˚
 *	@note		Œﬁ
 *	@attention	Œﬁ
**************************************************************************/
STK_STATUS Stk_API::WriteLogEndl()
{
	if (_Private->m_pLog.isNull())
	{
		return STK_ERROR;
	}

	return _Private->m_pLog->PrintEndl();
}

Stk_Document* Stk_API::GetDocument(unsigned int docID)
{
	Stk_Document* pDoc = NULL;
	DocMAP::iterator itor = _Private->m_pDocuments.find(docID);
	if (itor == _Private->m_pDocuments.end()) {
		_Private->m_pDocuments.insert(DocMAP::value_type(docID, Stk_DocumentPtr::CreateObject()));
		itor = _Private->m_pDocuments.find(docID);
		pDoc = itor->second.get();
	}
	else {
		pDoc = itor->second.get();
	}
	return pDoc;
}

STK_STATUS Stk_API::StartupService(int inPort)
{
	STK_STATUS error = STK_SUCCESS;
	//error = (STK_STATUS)StartThriftService(inPort);
	return error;
}

void Stk_API::InitColorMap()
{
	//ªÒ»°—’…´∆•≈‰Œƒº˛
	char drive[256] = {0};
	char dir[256] = {0};
	char fname[256] = {0};
	char ext[256] = {0};
	char dirBuf[256] = {0};




#ifdef __MOBILE__
	char* p = getenv("_");
	 if( p != NULL )
	 {
	  strcpy( dirBuf, p );
	  _splitpath(dirBuf, drive, dir, fname, ext);
	  strcpy(dirBuf,drive);
	  strcat(dirBuf,dir);
	  strcat(dirBuf,"ColorMap.json");
	 }
#else
	 GetModuleFileNameA(NULL, dirBuf, 256);
	 _splitpath(dirBuf, drive, dir, fname, ext);
	 _makepath(dirBuf, drive, dir, "ColorMap", "json");
	 //∂¡»°JsonŒƒº˛
#endif
	 //∂¡»°JsonŒƒº˛
	 FILE* f = fopen(dirBuf, "rb");
	if (f == NULL)
	{
        return;
    }
    IOStream* readStream = new IOStream(f);
	size_t mFileLength = readStream->FileSize();
	std::vector<char> colorData(mFileLength + 1);
	colorData[mFileLength] = '\0';

	if (readStream->Read(&colorData[0], 1, mFileLength) != mFileLength) {
		delete readStream;
		return;
	}
	delete readStream;
	//Ω‚ŒˆJson ˝æ›
	Document doc;
	doc.ParseInsitu(&colorData[0]);
	if (doc.HasParseError()) {
		return;
	}
	if (!doc.IsObject()) {
		return;
	}
	//±È¿˙À˘”–JasonΩ⁄µ„
	for (Value::ConstMemberIterator iter = doc.MemberBegin(); iter != doc.MemberEnd(); iter++)
	{
		Value jKey;
		Value jValue;
		Document::AllocatorType allocator;
		jKey.CopyFrom(iter->name, allocator);
		jValue.CopyFrom(iter->value, allocator);
		if (jKey.IsString() && jValue.IsInt())
		{
			string strKey = jKey.GetString();
			int key = atoi(strKey.c_str());
			int value = jValue.GetInt();
			_Private->m_pColorMap.insert(make_pair(key, value));
		}
	}
	
}

void Stk_API::InitConfig()
{
	//ªÒ»°DLL¬∑æ∂
	wstring wstrDllPath = HoteamSoft::SVLLib::Stk_Utility::GetCurModulePath();
	//string strDllPath = ChineseCode::WStringToString(wstrDllPath);
	wstrDllPath += L"SVLLib.ini";
#ifdef __MOBILE__
	string strDir = ChineseCode::WStringToString(wstrDllPath);
	m_bUseSSL = GetIniKeyInt("General","USESSL",(char*)strDir.c_str());
	m_bComPact = GetIniKeyInt("General","COMPACT",(char*)strDir.c_str());
#else
	wchar_t dirBuf[MAX_PATH];

	swprintf_s(dirBuf, MAX_PATH, L"%s", wstrDllPath.c_str());
	//sprintf_s(dirBuf, MAX_PATH, "%s", strDllPath.c_str());
	//∂¡»°iniŒƒº˛
	m_bUseSSL = GetPrivateProfileInt(L"General", L"USESSL", 0, dirBuf);
	m_bComPact = GetPrivateProfileInt(L"General", L"COMPACT", 0, dirBuf);
#endif
}

int Stk_API::GetColor(int colorIndex)
{
	int color = 13421772;
	ColorMAP::iterator colorItor = _Private->m_pColorMap.find(colorIndex);
	if (colorItor != _Private->m_pColorMap.end())
	{
		color = colorItor->second;
	}
	return color;
}

void Stk_API::SetRequestFileFun(RequestFilePathCallback inRequestFilePathFun)
{
	_RequestFilePathFun = inRequestFilePathFun;
}

HoteamSoft::SVLLib::RequestFilePathCallback Stk_API::GetRequestFileFun()
{
	return _RequestFilePathFun;
}

	}
}
