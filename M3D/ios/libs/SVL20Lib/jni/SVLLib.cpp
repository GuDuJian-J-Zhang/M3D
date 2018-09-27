//#include <jni.h>

#include "com_example_svllib_SVLLibTest.h"

#include <string>
#include "Stk_Document.h"

#include "Stk_UnitTest.h"
#include <iostream>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "Stk_LoadInf.h"
#include "Utility.h"

using namespace std;
using namespace HoteamSoft::SVLLib;
//全局变量
Stk_DocumentPtr StkDocument = NULL;
typedef struct {
	//Stk_InstancePtr stk_instance;
	int modelID;
}structInstance;
structInstance tempIns;
vector<structInstance> vecInstance;
/************************************************************************/
/* 输出当前时间                                                                  */
/************************************************************************/
void showCurrentTime()
{
	/*struct tm newtime;
	wchar_t temp[41];
	time_t now;

	time(&now);
	localtime_s(&newtime, &now);
	_tcsftime(temp, 40, _T("%Y-%m-%d %H:%M:%S "), &newtime);
	tprintf(L"%s", temp);*/
}
/************************************************************************/
/* 输出内存情况                                                                     */
/************************************************************************/
void showMemoryInfo(void)
{
	/*HANDLE handle=GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(handle,&pmc,sizeof(pmc));
	cout<<pmc.WorkingSetSize/1000/1000 <<"M/"<<pmc.PeakWorkingSetSize/1000/1000<<"M + "<<pmc.PagefileUsage/1000/1000 <<"M/"<<pmc.PeakPagefileUsage/1000/1000 <<"M"<<endl;
	*/
}
/************************************************************************/
/* 测试Node节点对象                                                                     */
/************************************************************************/
void NodeTest(Stk_NodePtr curNode)
{
	float w, x, y, z;
	vector<float> vecVertex, vecNor;
	vector<unsigned int> trianIdx;
	//wprintf(L"%s\r\n", curNode->GetNodeName().c_str());
	Stk_PartPtr stkPart = curNode->GetPart();
	STK_RGBA32 curColor = curNode->GetColor();
	if (stkPart.isNotNull())
	{
		int numMesh = stkPart->GetNbMesh();
		for (int i = 0; i < numMesh; i++)
		{
			Stk_MeshPrimPtr stkMesh = stkPart->GetMesh(i);
			stkMesh->GetMeshBuf(vecVertex, vecNor, trianIdx);
		}
	}
	int nbChild = curNode->GetNbChild();
	STK_MTX32 mtx;
	curNode->SetMatrix(mtx);
	for (int i = 0; i < nbChild; i++)
	{
		NodeTest(curNode->GetChild(i));
	}
}
//测试实例节点
void TestModel()
{
	for (int i =0; i < vecInstance.size(); i++)
	{
		//Stk_ModelPtr stkModel = vecInstance[i].stk_instance->GetModelByID(vecInstance[i].modelID);
		Stk_ModelPtr stkModel = StkDocument->GetModelByID(vecInstance[i].modelID);
		//Stk_ModelPtr stkModel = StkDocument->GetModelByID(1015);
		//STK_RGBA32 curColor = curNode->GetColor();
		if (stkModel.isNotNull())
		{
			vector<float> vecVertex, vecNor;
			vector<unsigned int> trianIdx;
#if 0
			//获取包围盒信息
			float bBox[6];
			//stkModel->GetBoundingBox(bBox, 6);
			int numMesh = stkModel->GetNbMesh();
			for (int i = 0; i < numMesh; i++)
			{
				Stk_MeshPrimPtr stkMesh = stkModel->GetMesh(i);
				stkMesh->GetMeshBuf(vecVertex, vecNor, trianIdx);
			}
#endif
			//获取包围盒信息
			float bBox[6];
			stkModel->GetBoundingBox(bBox, 6);
			stkModel->LoadLOD1Mesh();
			int numMesh = stkModel->GetNbLOD1Mesh();
			for (int i = 0; i < numMesh; i++)
			{
				Stk_MeshPrimPtr stkMesh = stkModel->GetLOD1Mesh(i);
				//stkMesh->GetMeshBuf(vecVertex, vecNor, trianIdx);
				vecVertex = stkMesh->GetVertexs();
				vecNor = stkMesh->GetNormals();
				cout <<"--Vertex数量:" << vecVertex.size() << endl;
				//获取面的个数
				int numMeshFace = stkMesh->GetNbMeshFace();
				for (int j = 0; j < numMeshFace; j++)
				{
					Stk_MeshFacePtr meshFacePtr = stkMesh->GetMeshFaceByIdx(j);
					meshFacePtr->GetIndex(trianIdx);
					//获取边的材质信息
					STK_UINT32 materialID = meshFacePtr->GetMaterialID();
					Stk_MaterialPtr materialPtr = StkDocument->GetMaterialByID(materialID);
					if(materialPtr != nullptr)
					{
						StkMaterialTypeEnum enumMaterialType = materialPtr->GetMaterialType();
						float fAlapha;
						float fDiffuseColor[3] = { 0 };
						materialPtr->GetTransparency(fAlapha);
						materialPtr->GetDiffuseColor(fDiffuseColor);
					}
					//获取边的个数
					vector<int> vecMeshEdgeID;
					meshFacePtr->GetEdges(vecMeshEdgeID);
					for (int k = 0; k < vecMeshEdgeID.size(); k++)
					{
						Stk_MeshEdgePtr meshEdge = stkMesh->GetMeshEdgeByID(vecMeshEdgeID[k]);
						if (meshEdge.isNull())
						{
							continue;
						}
						meshEdge->GetIndex(trianIdx);
					}
				}
				LOGI("FaceNum:%d",numMeshFace);
			}
			LOGI("MeshNum:%d",numMesh);
			//释放Model
			stkModel->UnloadLOD1Mesh();
		}
	}
}
/************************************************************************/
/* 测试文件加载信*/
/************************************************************************/
void infoTest()
{
	int instanceCount, modelCount;
	Stk_LoadInf* infoPtr = Stk_LoadInf::GetInstance();
	instanceCount = infoPtr->GetInstanceCount();
	modelCount = infoPtr->GetModelCount();
}
void InstanceTest(Stk_InstancePtr curInstance)
{
	float w, x, y, z;
	//wprintf(L"%s\r\n", curNode->GetNodeName().c_str());
	float matrix[12] = {0};
	curInstance->GetMatrix(matrix, 12);
	vector<Stk_InstancePtr> vecChildIns = curInstance->getChildren();
	int nbChild = vecChildIns.size();
	int InstanceID = curInstance->GetID();
	//tempIns.stk_instance = curInstance;
	tempIns.modelID = curInstance->GetModelID();
	vecInstance.push_back(tempIns);
	//curInstance->GetQuaternion(w, x, y, z);
	//curInstance->GetTranslation(x, y, z);
	for (int i = 0; i < nbChild; i++)
	{
		InstanceTest(vecChildIns[i]);
	}
}
//使用实例读取文件
void ReadFileTest()
{
	StkDocument = Stk_DocumentPtr::CreateObject();
	//wstring wstrFilePath = L"E:\\数模\\svlx\\svlx\\100-B-1-B1-B.rvm.json";
	wstring wstrFilePath = L"/storage/emulated/0/sview/sample/elbow.asm/elbow.asm.bom";
	//wstring wstrFilePath = L"C:\\SView\\Model\\SVL\\RVM-SVL-2.0.2\\QYA.rvm.svlx";
	//wstring wstrFilePath = L"C:\\SView\\Model\\SVL\\RVM-SVL-2.0.2\\JZ25-1.RVM.svlx";
	//wstring wstrFilePath = L"C:\\SView\\Model\\SVL\\ProE-SVL\\elbow.asm.4.svl.json";
	//wstring wstrFilePath = L"C:\\SView\\Model\\SVL\\ProE-SVL\\elbow.asm.svlx.json";
	//wstring wstrFilePath = L"D:\\develop\\objtosvlx\\ObjToSvlx\\ObjToSvlx\\tmp\\jiezhi.obj.svlx";
	StkDocument->OpenLocal(wstrFilePath);
	//StkDocument->OpenFile(wstrFilePath);

	FILE* file = fopen("/storage/emulated/0/sview/sample/elbow.asm/elbow.asm.bom","rb");
	if (!file)
	{
		LOGI("FileOpenFault111");
	}
#if 1
	//加载FileInfo
	StkDocument->LoadFileInfo();
	int numInstance = Stk_LoadInf::GetInstance()->GetInstanceCount();
	int numModel = Stk_LoadInf::GetInstance()->GetModelCount();
	//加载Bom
	showCurrentTime();
	cout<<"Bom加载前:";
	showMemoryInfo();
	StkDocument->LoadBom();
	cout<<"实例数量:"<<StkDocument->GetNbInstance()<<endl;
	LOGI("insCount:%d",StkDocument->GetNbInstance());
	showCurrentTime();
	cout<<"Bom加载后:";
	showMemoryInfo();
	vector<Stk_InstancePtr> vecTopInstance;
	vecTopInstance = StkDocument->getTopInstances();
	int numTopInstance = vecTopInstance.size();
	for (int i = 0; i < numTopInstance; i++)
	{
		Stk_InstancePtr instance =  vecTopInstance[i];
		InstanceTest(instance);
	}
	showCurrentTime();
	cout<<"装配结构创建后:";
	showMemoryInfo();
	//释放Bom
	vecTopInstance.clear();
	vecTopInstance.shrink_to_fit();
	StkDocument->UnloadBom();
	showCurrentTime();
	cout<<"Bom释放后:";
	showMemoryInfo();
	//加载材质信息
	StkDocument->LoadMaterial();
#endif
	//加载Model
	StkDocument->LoadModel();
	cout<<"Model数量:"<<StkDocument->GetNbModel()<<endl;
	LOGI("ModelCount:%d",StkDocument->GetNbModel());
	showCurrentTime();
	cout<<"Model加载后:";
	showMemoryInfo();
	//测试Model信息
	TestModel();
	showCurrentTime();
	cout<<"Model解析后:";
	showMemoryInfo();
	//释放Model
	StkDocument->UnloadModel();
	 //EmptyWorkingSet(GetCurrentProcess());
}
/************************************************************************/
/* 测试解析压缩后的内存数据                                                                 */
/************************************************************************/
void ReadCompressBufTest()
{
	StkDocument = Stk_DocumentPtr::CreateObject();

	//string strFilePath = "C:\\SView\\Model\\SVL\\RVM-SVL-2.0.2\\100-B-1-B1-B.rvm.svlx";
	//string strFilePath = "C:\\SView\\Model\\SVL\\RVM-SVL-2.0.2\\JZ25-1.RVM.svlx";
	//string strFilePath = "C:\\SView\\Model\\SVL\\RVM-SVL-2.0.2\\JZ25-1-noRef.RVM.svlx";
	string strFilePath = "/storage/emulated/0/sview/sample/elbow.asm.svlx";

	//将SVLX数据读取到内存中
	STK_UINT32 mBlockByteSize;
	STK_CHAR* mBlockMemory = NULL;
	Stk_FilePtr filePtr = Stk_FilePtr::CreateObject();
	filePtr->SetName(strFilePath);

	filePtr->OpenAsBin("rb");
	mBlockByteSize = filePtr->FileSize();
	mBlockMemory = (STK_CHAR*)malloc(mBlockByteSize);
	filePtr->ReadBuffer(mBlockMemory, mBlockByteSize, 0);

	//连续创建内存
#if 0
	for (int i = 0; i < 56000; i++)
	{
		char* tempChar = new char[1];
	}
#endif
	LOGI("mBlockByteSize:%d",mBlockByteSize);
	//打开文件
	StkDocument->OpenFile(mBlockByteSize, mBlockMemory);
	showCurrentTime();
	cout<<"Bom加载前:";
	showMemoryInfo();
	LOGI("aaa");
	StkDocument->LoadBom();
	LOGI("ins count%d",StkDocument->GetNbInstance());
	cout<<"实例数量:"<<StkDocument->GetNbInstance()<<endl;
	showCurrentTime();
	cout<<"Bom加载后:";
	showMemoryInfo();
	vector<Stk_InstancePtr> vecTopInstance;
	vecTopInstance = StkDocument->getTopInstances();
	int numTopInstance = vecTopInstance.size();
	for (int i = 0; i < numTopInstance; i++)
	{
		Stk_InstancePtr instance =  vecTopInstance[i];
		InstanceTest(instance);
	}
	showCurrentTime();
	cout<<"装配结构创建后:";
	showMemoryInfo();
	//释放Bom
	vecTopInstance.clear();
	vecTopInstance.shrink_to_fit();
	//StkDocument->UnloadBom();
	showCurrentTime();
	cout<<"Bom释放后:";
	showMemoryInfo();
	//加载材质信息
	StkDocument->LoadMaterial();
	//加载Model信息
	StkDocument->LoadModel();
	cout<<"Model数量:"<<StkDocument->GetNbModel()<<endl;
	LOGI("ModelCount:%d",StkDocument->GetNbModel());
	showCurrentTime();
	cout<<"Model加载后:";
	showMemoryInfo();
	//测试Model信息
	TestModel();
	showCurrentTime();
	cout<<"Model解析后:";
	showMemoryInfo();
	StkDocument->SetOutputFullPath(L"/storage/emulated/0/sview/sample/elbow");
	StkDocument->WriteFile();
	if (mBlockMemory != NULL)
	{
		free(mBlockMemory);
		mBlockMemory = NULL;
	}



}
/************************************************************************/
/*测试Bom信息                                                                      */
/************************************************************************/
void loadBomTest()
{
	StkDocument = Stk_DocumentPtr::CreateObject();
	string strFilePath = "C:\\SView\\Model\\SVL\\RVM-SVL-2.0.2\\100-B-1-B1-B.rvm.bom";
	//string strFilePath = "C:\\SView\\Model\\SVL\\RVM-SVL-2.0.2\\JZ25-1.RVM.svlx";
	//string strFilePath = "C:\\SView\\Model\\SVL\\RVM-SVL-2.0.2\\JZ25-1-noRef.RVM.svlx";
	//将SVLX数据读取到内存中
	STK_UINT32 mBlockByteSize;
	STK_CHAR* mBlockMemory = NULL;
	Stk_FilePtr filePtr = Stk_FilePtr::CreateObject();
	filePtr->SetName(strFilePath);
	filePtr->OpenAsBin("rb");
	mBlockByteSize = filePtr->FileSize();
	mBlockMemory = (STK_CHAR*)malloc(mBlockByteSize);
	filePtr->ReadBuffer(mBlockMemory, mBlockByteSize, 0);
	StkDocument->LoadBom(mBlockByteSize, mBlockMemory, STK_MEMBLOCK_TYPE_COMPRESS_7Z);
	vector<Stk_InstancePtr> vecTopInstance;
	vecTopInstance = StkDocument->getTopInstances();
	int numTopInstance = vecTopInstance.size();
	for (int i = 0; i < numTopInstance; i++)
	{
		Stk_InstancePtr instance =  vecTopInstance[i];
		InstanceTest(instance);
	}
}
/************************************************************************/
/* 测试FileInfo数据加载                                                                    */
/************************************************************************/
void loadInfoTest()
{
	StkDocument = Stk_DocumentPtr::CreateObject();
	string strFilePath = "C:\\SView\\Model\\SVL\\RVM-SVL-2.0.2\\100-B-1-B1-B.rvm.info";
	//string strFilePath = "C:\\SView\\Model\\SVL\\RVM-SVL-2.0.2\\JZ25-1.RVM.svlx";
	//string strFilePath = "C:\\SView\\Model\\SVL\\RVM-SVL-2.0.2\\JZ25-1-noRef.RVM.svlx";
	//将SVLX数据读取到内存中
	STK_UINT32 mBlockByteSize;
	STK_CHAR* mBlockMemory = 0;
	Stk_FilePtr filePtr = Stk_FilePtr::CreateObject();
	filePtr->SetName(strFilePath);
	filePtr->OpenAsBin("rb");
	mBlockByteSize = filePtr->FileSize();
	mBlockMemory = (STK_CHAR*)malloc(mBlockByteSize);
	filePtr->ReadBuffer(mBlockMemory, mBlockByteSize, 0);
	StkDocument->LoadFileInfo(mBlockByteSize, mBlockMemory, STK_MEMBLOCK_TYPE_COMPRESS_7Z);
	//测试Info信息
	infoTest();
}
/************************************************************************/
/* 测试Model数据加载                                                                     */
/************************************************************************/
void loadModelTest()
{
	StkDocument = Stk_DocumentPtr::CreateObject();
	string strFilePath = "C:\\SView\\Model\\SVL\\RVM-SVL-2.0.2\\100-B-1-B1-B.rvm.lod";
	//string strFilePath = "C:\\SView\\Model\\SVL\\RVM-SVL-2.0.2\\JZ25-1.RVM.svlx";
	//string strFilePath = "C:\\SView\\Model\\SVL\\RVM-SVL-2.0.2\\JZ25-1-noRef.RVM.svlx";
	//将SVLX数据读取到内存中
	STK_UINT32 mBlockByteSize;
	STK_CHAR* mBlockMemory = 0;
	Stk_FilePtr filePtr = Stk_FilePtr::CreateObject();
	filePtr->SetName(strFilePath);
	filePtr->OpenAsBin("rb");
	mBlockByteSize = filePtr->FileSize();
	mBlockMemory = (STK_CHAR*)malloc(mBlockByteSize);
	filePtr->ReadBuffer(mBlockMemory, mBlockByteSize, 0);
	StkDocument->LoadModel(mBlockByteSize, mBlockMemory, STK_MEMBLOCK_TYPE_COMPRESS_7Z);
	//测试Model数据

}
/************************************************************************/
/* 测试Material数据加载                                                                     */
/************************************************************************/
void LoadMaterialTest()
{
	StkDocument = Stk_DocumentPtr::CreateObject();
	string strFilePath = "C:\\SView\\Model\\SVL\\RVM-SVL-2.0.2\\100-B-1-B1-B.rvm.material";
	//string strFilePath = "C:\\SView\\Model\\SVL\\RVM-SVL-2.0.2\\JZ25-1.RVM.svlx";
	//string strFilePath = "C:\\SView\\Model\\SVL\\RVM-SVL-2.0.2\\JZ25-1-noRef.RVM.svlx";
	//将SVLX数据读取到内存中
	STK_UINT32 mBlockByteSize;
	STK_CHAR* mBlockMemory = 0;
	Stk_FilePtr filePtr = Stk_FilePtr::CreateObject();
	filePtr->SetName(strFilePath);
	filePtr->OpenAsBin("rb");
	mBlockByteSize = filePtr->FileSize();
	mBlockMemory = (STK_CHAR*)malloc(mBlockByteSize);
	filePtr->ReadBuffer(mBlockMemory, mBlockByteSize, 0);
	StkDocument->LoadMaterial(mBlockByteSize, mBlockMemory, STK_MEMBLOCK_TYPE_COMPRESS_7Z);
}


JNIEXPORT void JNICALL Java_com_example_svllib_SVLLibTest_Test
  (JNIEnv *, jobject)
{
	ReadCompressBufTest();
	//ReadFileTest();
}


