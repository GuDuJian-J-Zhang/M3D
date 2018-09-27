#pragma once
#include <string>
class RemoteClientImpl;
class RemoteClient
{
public:
	RemoteClient(const std::string& ip, int port);
	~RemoteClient();
private:
	RemoteClientImpl* m_Impl;
	friend class RemoteClientImpl;
public:
	bool Connect();
	void Deconnect();
	unsigned int	OpenDocument(const std::string& userName, 
								const std::string& projectCode, 
								const std::string& fileName);
	/*int				GetNbTopNode();
	unsigned int	GetTopNodeID(unsigned int iIdx);
	bool			GetAssemble(unsigned int assemblyID, Assembly& outAssembly);
	bool			GetMeshPrim(unsigned int meshID, MeshPrim& outMeshPrim);
	bool			GetMeshBuf(const std::vector<Accessor>& vecAccessor, Buffer& outMeshBuf);
	bool			GetMeshBuf(const std::string bufUrn, Buffer* outMeshBuf);

	int				ListTopNodeID(std::vector<int>& outTopNodeIDs);
	int				ListPrimID(std::vector<int>& vecPrimIDs);
	int				ListAssemblyID(std::vector<int>& vecAssebmlyIDs);
	int				ListAssembly(BufferList& bufList);
	int				ListPrim(BufferList& bufList);*/
};

