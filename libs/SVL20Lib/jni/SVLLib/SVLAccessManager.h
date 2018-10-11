#pragma once
#include <transport/TSocket.h>
#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>
#include <thrift/transport/TSSLSocket.h>
#include "thrift\SVLComm.h"

using namespace std;
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

/************************************************************************/
/* 为将来使用证书验证客户端预留                                        */
/************************************************************************/
class SVLAccessManager :
	public AccessManager
{	
public:
	enum TypeEnum
	{
		SERVER,
		CLIENT
	};

	SVLAccessManager(TypeEnum inType) {m_emType = inType;};
	~SVLAccessManager();

	Decision verify(const sockaddr_storage& sa) throw();
	Decision verify(const std::string& host, const char* name, int size) throw();
	Decision verify(const sockaddr_storage& sa, const char* data, int size) throw();


private:
	TypeEnum m_emType;
};

