//#include "..\..\StdAfx.h"
#include "Util.h"

#ifdef _WINDOWS_
#include <Windows.h>
#include "nb30.h"
#include "Iphlpapi.h"
#pragma comment(lib,"Iphlpapi.lib")
#endif

// 构造函数
CUtil::CUtil(void)
{
}

// 析构函数
CUtil::~CUtil(void)
{
}

// 获取本机所有Mac
void CUtil::GetAllMac(std::vector<std::string>& sa)
{
#ifdef _WINDOWS_
	CString m_macaddress;
	UINT uErrorCode = 0;
	IP_ADAPTER_INFO iai;
	ULONG uSize = 0;
	DWORD dwResult = GetAdaptersInfo( &iai, &uSize );//GetAdaptersInfo():This function retrieves adapter information for the local computer.
	//第一个参数是一个IP_ADAPTER_INFO的结构体,第二个是一个输入输出类型的参数,[in, out]Pointer to the size, in bytes, of the buffer indicated by the pAdapterInfo parameter. 
	//If this size is insufficient to hold the adapter information, this function fills in the buffer with the required size, and returns an error code of ERROR_BUFFER_OVERFLOW. 

	if (dwResult==0)
	{
		m_macaddress.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"),iai.Address[0],iai.Address[1],iai.Address[2],iai.Address[3],iai.Address[4],iai.Address[5]);
	}
	if (dwResult==ERROR_BUFFER_OVERFLOW)
	{
		IP_ADAPTER_INFO* piai = ( IP_ADAPTER_INFO* )HeapAlloc( GetProcessHeap( ), 0, uSize );
		if( piai != NULL )
		{
			dwResult = GetAdaptersInfo( piai, &uSize );
			if( ERROR_SUCCESS == dwResult )
			{
				while (piai!=NULL)
				{
					m_macaddress.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"),piai->Address[0],piai->Address[1],piai->Address[2],piai->Address[3],piai->Address[4],piai->Address[5]);
					sa.Add(m_macaddress);
					piai=piai->Next;
				}				
			}
			else
			{
				uErrorCode = 0xF0000000U + dwResult;
			}
		}
		VERIFY( HeapFree( GetProcessHeap( ), 0, piai ) );
	}
#endif
}