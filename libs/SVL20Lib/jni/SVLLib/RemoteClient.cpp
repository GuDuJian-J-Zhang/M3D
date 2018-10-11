#include "RemoteClient.h"
#include "Stk_Utility.h"
#include "ChineseCode.h"
#include <iostream>
#include "Stk_API.h"

using namespace std;
using namespace HoteamSoft::SVLLib;


class RemoteClientImpl
{
public:
	RemoteClient* publ;
	RemoteClientImpl(RemoteClient* pOwn);
	~RemoteClientImpl();
	string strIP;
	int port;
#ifdef __MOBILE__
	//__MOBILE_TODO

#else
	CRITICAL_SECTION m_cs;
#endif
};

RemoteClientImpl::RemoteClientImpl(RemoteClient* pOwn)
{
	publ = pOwn;

#ifdef __MOBILE__
	//__MOBILE_TODO

#else
	InitializeCriticalSection(&m_cs);
#endif
}

RemoteClientImpl::~RemoteClientImpl()
{
#ifdef __MOBILE__
	//__MOBILE_TODO

#else
	DeleteCriticalSection(&m_cs);
#endif
}

RemoteClient::RemoteClient(const std::string& ip, int port)
{
	m_Impl = new RemoteClientImpl(this);
	m_Impl->strIP = ip;
	m_Impl->port = port;
}


RemoteClient::~RemoteClient()
{
	delete m_Impl;
}

bool RemoteClient::Connect()
{
	wstring wstrDllPath = HoteamSoft::SVLLib::Stk_Utility::GetCurModulePath();
	string strDllPath = ChineseCode::WStringToString(wstrDllPath);

	string caPath = strDllPath + "Certificate\\ca.crt";
	string certPath = strDllPath+ "Certificate\\client\\client.crt";
	string keyPath = strDllPath + "Certificate\\client\\client.key";
	return true;
}

void RemoteClient::Deconnect()
{
}

unsigned int RemoteClient::OpenDocument(const std::string& userName, const std::string& projectCode, const std::string& fileName)
{
	return 0;
}
