#include "sview/io/SvlWriter.h"


#include "Sview/views/view.h"
#include "m3d/utils/PathHelper.h"
#include "m3d/model/Model.h"
#include "m3d/model/Body.h"
#include "m3d/model/MeshData.h"
#include "m3d/base/Vector3.h"
#include "m3d/model/Face.h"
#include "m3d/base/Color.h"
#include "m3d/base/Matrix4.h"
#include "m3d/extras/Operation.h"
#include "m3d/extras/modelmanager/AddFileOperation.h"
#include "m3d/extras/modelmanager/CopyModelOperation.h"
#include "m3d/extras/modelmanager/MoveModelOperation.h"
#include "m3d/extras/modelmanager/RemoveModelOperation.h"
#include "m3d/extras/modelmanager/ModelManager.h"
#include "m3d/extras/modelmanager/ModelAssemblyHelper.h"
#include "m3d/extras/OperationHistoryManager.h"
#include "m3d/utils/Platform.h"

#include "prebuilt/svl2/include/Stk_Define.h"
#include "prebuilt/svl2/include/Stk_Platform.h"
#include "prebuilt/svl2/include/Stk_DocumentManager.h"
#include "prebuilt/svl2/include/Stk_Instance.h"
#include "prebuilt/svl2/include/Stk_ProtoType.h"
#include "prebuilt/svl2/include/Stk_GeoAttribute.h"
#include "prebuilt/svl2/include/Stk_CompositeText.h"
#include "prebuilt/svl2/include/Stk_LineSetEntity.h"


using M3D::Body;
using M3D::Body;
using M3D::VertexSet;
using M3D::Vector3;
using M3D::Matrix4;
using M3D::Face;
using M3D::Color;
using M3D::Mesh;
using M3D::Operation;
using M3D::OperationHistoryManager;
using M3D::AddFileOperation;
using M3D::CopyModelOperation;
using M3D::MoveModelOperation;
using M3D::RemoveModelOperation;
using M3D::PathHelper;
using M3D::ModelManager;
using M3D::ModelAssemblyHelper;
using M3D::Platform;

namespace SVIEW {
SvlWriter::SvlWriter(bool isSVL) {
	isSavingSVL = isSVL;
}
SvlWriter::~SvlWriter() {

}

//int SvlWriter::SaveTo(Model* model,const string &targetPath) {
//	if (isSavingSVL) //在model不完全包含svl中的属性信息之前，保存svl文件使用修改旧svl的方式
//	{
//		return SaveByModifyOldSVL();
//	} else {
//		return SaveToNewSVL(model,targetPath);
//	}
//
//	return 1;
//}

int SvlWriter::SaveTo(View* view,const string &targetPath)
{
	m_view = view;
	m_targetPath = targetPath;

		if (isSavingSVL) //在model不完全包含svl中的属性信息之前，保存svl文件使用修改旧svl的方式
		{
			return SaveByModifyOldSVL();
		} else {
			Model* topModel = (Model*)view->GetModel();
			if(topModel)
			{
				return SaveToNewSVL(topModel,targetPath);
			}
		}

	return 1;
}

//保存到一个新的svl（数据从model获取，如stl格式）
int SvlWriter::SaveToNewSVL(Model* model,const string &targetPath) {
	LOGI("SvlWriter::SaveToNewSVL begin");
	this->init();
	int ret = -1;
	Stk_File* stkFileP = NULL;

	//填充DocumentManager信息
	STK_ID idMesh = 1;
	STK_ID idBody = 1;
	STK_SIZE_T szVtx = 0;
	STK_SIZE_T szTri = 0;
	STK_UINT32 i;
	STK_RGBA32 stkMeshColor = { 0.8, 0.8, 0.6, 0.0 };

	Stk_DocumentManager *m_pDocumentManager = new Stk_DocumentManager();

	Stk_ProtoType* m_pTopSVLProto = new Stk_ProtoType(L"", m_pDocumentManager);
	m_pTopSVLProto->RegisterID(1);

	//SetProtoTypeDataRecursion
	Model *rootModel = model;

//	for(map<string,Model *>::iterator i = this->m_CurModelMap.begin();
//			i!=m_CurModelMap.end();i++)
//	{
//		LOGI("SvlWriter::SaveToNewSVL tmpPath:%s",(*i).first.c_str());
//	}

//	map<string, Model *>::iterator ite = this->m_CurModelMap.find("0");
//	rootModel = (*ite).second;

	if (rootModel != NULL) {
		SetProtoTypeDataRecursion(rootModel, m_pTopSVLProto,
				m_pDocumentManager);

//				strName = strFilePath.Right(strFilePath.GetLength() - strFilePath.ReverseFind('\\')-1);
//				strName = strName.Left(strName.ReverseFind('.'));
		wstring wstrName = StringToWString(rootModel->GetName());
		m_pTopSVLProto->SetProtoName(wstrName);
		m_pTopSVLProto->SetLODCount(1);
        m_pTopSVLProto->SetCADFileTop(true);
		wstring wstrTargetPath =  GetSVLUTF8String(targetPath);
//        wstring wstrCadPath =  GetSVLUTF8String("");
        m_pTopSVLProto->SetCADFilePath(wstrTargetPath); //以STL路径作为CADFilePath
		m_pTopSVLProto->RegCADFile(wstrTargetPath, stkFileP); //发行Stk_File

		stkFileP->SetDataSource( GetSVLUTF8String("STL File"));
		stkFileP->SetConvTookit( GetSVLUTF8String("SViewMobile"));
		stkFileP->SetSVLConversion( GetSVLUTF8String("SViewMobile"));
		stkFileP->SetPolygonNum(szTri);
		stkFileP->SetProtoTypeNum(1);
        m_pDocumentManager->AddTopProtoType(m_pTopSVLProto);
		m_pDocumentManager->SetAllFilesCompressType(TYPE_COMPRESS_RATIO);
		m_pDocumentManager->SetFileSaveType(FILESAVE_TYPE_UNIQUE);
		STK_STATUS stkStatus = m_pDocumentManager->SaveDocumentAs(wstrTargetPath, FILESAVE_TYPE_UNIQUE);
		if (STK_STATUS::STK_SUCCESS == stkStatus)
		{
			ret = 0;
		}
		else
		{
			ret = stkStatus;
		}

		LOGI("SvlWriter::SaveToNewSVL endWithCode:%d",stkStatus);
	}

	delete m_pDocumentManager;

	return ret;
}

int SvlWriter::SetProtoTypeDataRecursion(Model* curModel,
		Stk_ProtoType* curProto, Stk_DocumentManager * curDocMgr) {
	LOGI("SvlWriter::SetProtoTypeDataRecursion begin modelPath:%s",curModel->GetPlcPath().c_str());
	int level = 0; //先写死lodlevel

	Stk_Body *pBody = new Stk_Body(curProto);
	pBody->RegisterID(1);

	LOGI("SvlWriter::SetProtoTypeDataRecursion bodyCnt:%d",curModel->GetBodys()->size());
	//fill geometry info of curProto
	for (int ibody = 0; ibody < curModel->GetBodys()->size(); ibody++) {
		//填充body中的顶点数据到Stk_Mesh_Com
		Body* curBody = (Body*)curModel->GetBodys()->at(ibody);
		//TODO
		//VertexSet* bodymeshData = (VertexSet*) curBody->GetData(level);
		VertexSet* bodymeshData = NULL;
		vector<Vector3>* positionArray = bodymeshData->GetPositionArray();
		vector<Vector3>* normalArray = bodymeshData->GetNormalArray();

		Stk_Mesh_Com *pMesh = new Stk_Mesh_Com(curProto);
		pMesh->SetClosed(false);

		Color* bodyColor = curBody->GetColor();
		STK_RGBA32 stkMeshColor;
		stkMeshColor.Red = bodyColor->m_r;
		stkMeshColor.Green = bodyColor->m_g;
		stkMeshColor.Blue = bodyColor->m_b;
		stkMeshColor.Alpha = 1-bodyColor->m_a;
		pMesh->SetColor(stkMeshColor);

		MeshLODInfo* MeshLODP = new MeshLODInfo();
		MeshLODP->PloygonNum = bodymeshData->GetVertexCount();
		MeshLODP->AngularTol = 0.0;
		MeshLODP->ChordError = 0.0;

		for (int ivec = 0; ivec < bodymeshData->GetVertexCount(); ivec++) {
			// 填充顶点
			STK_PNT32 pnt;
			pnt.X = positionArray->at(ivec).m_x;
			pnt.Y = positionArray->at(ivec).m_y;
			pnt.Z = positionArray->at(ivec).m_z;
			MeshLODP->Vertices.push_back(pnt);

			// 填充法线
			STK_PNT32 nor;
			nor.X = normalArray->at(ivec).m_x;
			nor.Y = normalArray->at(ivec).m_y;
			nor.Z = normalArray->at(ivec).m_z;

			MeshLODP->Vertices.push_back(nor);
		}

		pMesh->RegisterID(STK_NEW_ID);
		pMesh->SetMeshLODInfo(0, MeshLODP);

		pMesh->SetMeshType(MESH_STORE_LOD_UNSHARE_VERTEX);

		//body总索引
		vector<M3D_INDEX_TYPE>* bodyIndexArray = bodymeshData->GetIndexArray();
		//faces
		for (int iFace = 0; iFace < curBody->GetFaces().size(); iFace++) {
			Face* curFace = (Face*)curBody->GetFaces().at(iFace);
			//TODO
			Mesh* faceMeshData = NULL;
			//Mesh* faceMeshData = (Mesh*) curFace->GetData(level);
			Stk_Mesh_Face *pFace = new Stk_Mesh_Face(curProto);
			FaceLODInfo* FaceLODP = new FaceLODInfo();
			int curIndex = 0;

			//if (bodymeshData->IsUseIndex()) {
				for (int iElementIndex = 0;
						iElementIndex < faceMeshData->GetDataLength();
						iElementIndex++) {
					int pos = faceMeshData->GetDataOffset()+iElementIndex;
					FaceLODP->TrianglesLOD.push_back((int) bodyIndexArray->at(pos));
				}
			//} else {
//				for (int iElementIndex = 0;
//						iElementIndex < faceMeshData->GetIndexArray()->size();
//						iElementIndex++) {
//					curIndex += iElementIndex;
//					FaceLODP->TrianglesLOD.push_back(curIndex);
//				}
			//}
			pFace->RegisterID(STK_NEW_ID);
			pFace->SetFaceLODInfo(level, FaceLODP);
			Color* faceColor =  curFace->GetColor();
			STK_RGBA32 stkColor;
			stkColor.Red = faceColor->m_r;
			stkColor.Green = faceColor->m_g;
			stkColor.Blue = faceColor->m_b;
			stkColor.Alpha = 1-faceColor->m_a;
			pFace->SetColor(stkColor);
			pMesh->AddMeshFace(pFace);

		}
		pBody->AddMeshCom(pMesh);
	}

	//创建Node节点
	Stk_Node *pNode = new Stk_Node(curDocMgr, curProto);
	pNode->SetNodeType(NODE_TYPE_BODY);
	pNode->RegisterID(STK_NEW_ID);
	pNode->SetBody(pBody);
	//设置顶级Node节点及节点模式
	curProto->SetNodeMode(true);
	curProto->SetTopNode(pNode);

	LOGI("SvlWriter::SetProtoTypeDataRecursion end modelPath:%s",curModel->GetPlcPath().c_str());
	//todo 子级ins及proto
    return 0;
}

int SvlWriter::SaveByModifyOldSVL()
{
    LOGI("SaveByModifyOldSVL start");

	this->init();
	//根据操作历史纪录 对SVL文件进行相同的操作
	OperationHistoryManager* historyCmdMgr =  m_view->GetModelManager()->GetCommandHistoryManager();

	if(historyCmdMgr)
	{
		const list<Operation*>& OpList = historyCmdMgr->GetOperationList();

		list<Operation*>::const_iterator it = OpList.begin();
		while(it != OpList.end())
		{
			//LOGI("Operation start");
			Operation* operation = *it;
			this->ApplyOperationToSVLFile(operation,m_stkDocMgr);
			it++;
			//LOGI("Operation end");
		}
	}

	//
	SaveAnthorInfoToSVLFile();

	//save
	wstring tarFile = GetSVLUTF8String(m_targetPath);
	m_stkDocMgr->SaveDocumentAs(tarFile, FILESAVE_TYPE_UNIQUE);

    LOGI("SaveByModifyOldSVL end");
	return 0;
}

bool SvlWriter::SaveAnthorInfoToSVLFile()
{
    ModifyProto(m_topProtoType, (Model*)m_view->GetModel());
    
    for(int i=0;i<m_allModels.size();i++){
		Model* model = (Model*)m_allModels.at(i);
		string strPlcPath = PathHelper::M3DHexPathToSVLDec(
				PathHelper::GetM3DPath(model));
		wstring wstrPlcPath = GetSVLUTF8String(strPlcPath);
		Stk_Instance* insance = m_stkDocMgr->GetInstanceByPath(wstrPlcPath);
		if (insance)
		{
			ModifyInstance(insance, model);
		}
		else
		{
			LOGE("SaveAnthorInfoToSVLFile ins:%s not found ! ",strPlcPath.c_str());
		}
    }
	return true;
}
    
void SvlWriter::ModifyProto(Stk_ProtoType* proto, M3D::Model* model)
{
	const M3D::Color* color = model->GetColor();
	LOGE("ModifyProto %p",color);
	if(color == NULL){
		return;
	}
	if (*color != *M3D::Color::Default) {

		STK_RGBA32 protoColor = { color->m_r, color->m_g, color->m_b, 1
				- color->m_a };
		proto->SetColor(protoColor);
	}
	//修改名称
	wstring wstrName = GetSVLUTF8String(model->GetName());

	proto->SetProtoName(wstrName);

}
    
void SvlWriter::ModifyInstance(Stk_Instance* ins, Model* model) {
	//修改颜色
	const M3D::Color* color = model->GetColor();
	if(color == NULL){
			return;
		}
	if (*color != *M3D::Color::Default) {
		ins->SetColor(color->m_r, color->m_g, color->m_b, 1 - color->m_a);
	}
	Matrix3x4* plcMatrix = model->GetPlaceMatrix();
	STK_MTX32 insPlcMatrix;
	memcpy(insPlcMatrix.PlcMatrix, plcMatrix->ToMatrix4().Data(),
			16 * sizeof(float));

	PathHelper::GetSVLPath(model);

	//修改装配矩阵
	ins->SetPlacement(model->GetPlcId(), insPlcMatrix);
	//修改名称
	wstring wstrName = GetSVLUTF8String(model->GetName());
	ins->SetInsName(wstrName);
}

Stk_ProtoType* SvlWriter::GetDefaultProto(Stk_DocumentManager * stkDocMgr)
{
	Stk_ProtoType* topProtoType = NULL;
	int topProtoNum = stkDocMgr->GetTopProtoTypeNum();
	if (topProtoNum > 1) {
		LOGE("模型有多个TopProtoType，默认使用了第一个！");
	}

	//获取默认原型
	topProtoType = stkDocMgr->GetTopProtoTypeByIndex(0);
	STK_STATUS status = stkDocMgr->SetDefaultTopProto(topProtoType);
	if (status != STK_SUCCESS) {
		LOGE("SvlReader::ReadFile SetDefaultTopProto Error!");
		return NULL;
	}

	return topProtoType;
}

wstring SvlWriter::GetSVLUTF8String(const string& m3dString)
{
	return Platform::StringToWString(m3dString, "utf-8");
}

bool SvlWriter::ApplyOperationToSVLFile(Operation* operation,Stk_DocumentManager * stkDocMgr)
{
	bool applyState = false;
	if(operation)
	{
		int operationType = operation->GetType();

//		LOGE("stkDocMgr->AddAssemble command->GetType() %d",command->GetType());
		if(operationType == AddFileOperation::TYPE)
		{
			AddFileOperation* addFileOp = (AddFileOperation*)operation;
            std::vector<Stk_ProtoType*> stkProtoTypeList;
            wstring wFilePath = this->GetSVLUTF8String(addFileOp->GetFilePath());
            
            STK_STATUS stkStatus = stkDocMgr->LoadCADSubFile(wFilePath, stkProtoTypeList);
            if(stkStatus != STK_SUCCESS){
                LOGE("stkDocMgr->AddAssemble filePath Error %s ",addFileOp->GetFilePath().c_str());
            }
 
			string destPath = PathHelper::M3DHexPathToSVLDec(addFileOp->GetUpperPlcPath());
            wstring wdestPath = this->GetSVLUTF8String(destPath);
//			LOGE("stkDocMgr->AddAssemble filePath %s",destPath.c_str());
//			LOGE("stkDocMgr->AddAssemble wFilePath %s",wFilePath.c_str());

			Stk_ProtoType* stkProtoTypeP =stkProtoTypeList[0];
			if(stkProtoTypeP)
			{
				wstring wstrPlcPath;
				STK_STATUS ret = stkDocMgr->AddAssemble(wdestPath,stkProtoTypeP,wstrPlcPath);
				string strPlcPath = Platform::WStringToString(wstrPlcPath);
//				LOGE("stkDocMgr->AddAssemble %s",strPlcPath.c_str());
			}

		}else if(operationType == CopyModelOperation::TYPE)
		{
			CopyModelOperation* copyModelOp = (CopyModelOperation*)operation;
            
            string srcPlcPath = PathHelper::M3DHexPathToSVLDec(copyModelOp->GetSrcPlcPath());
            string destPlcPath = PathHelper::M3DHexPathToSVLDec(copyModelOp->GetDestPlcPath());
            
            wstring wsrcPath = this->GetSVLUTF8String(srcPlcPath);
            wstring wdestPlcPath = this->GetSVLUTF8String(destPlcPath);
            wstring wgenPlcPath;

            stkDocMgr->CopyAssemble(wsrcPath,wdestPlcPath,wgenPlcPath);
        
		}else if(operationType == MoveModelOperation::TYPE)
		{
			MoveModelOperation* moveModelOp = (MoveModelOperation*)operation;
			string srcPath =  PathHelper::M3DHexPathToSVLDec(moveModelOp->GetForwPlcPath());
			string afterPlcPath =  PathHelper::M3DHexPathToSVLDec(moveModelOp->GetAfterPlcPath());

			wstring wsrcPath = this->GetSVLUTF8String(srcPath);
			wstring wafterPlcPath = this->GetSVLUTF8String(afterPlcPath);

			wstring newWPlcPath = stkDocMgr->DragAssemble(wsrcPath,wafterPlcPath);

			string newPlcPath = Platform::WStringToString(newWPlcPath);

//			LOGE("newPlcPath:%s",newPlcPath.c_str());

		}else if(operationType == RemoveModelOperation::TYPE)
		{
			RemoveModelOperation* removeModelOp = (RemoveModelOperation*)operation;

			string removeInstancePath  = PathHelper::M3DHexPathToSVLDec(removeModelOp->GetPlcPath()) ;

//			LOGE("stkDocMgr->AddAssemble removeInstancePath %s",removeInstancePath.c_str());
 
			wstring removeInstanceWPath = this->GetSVLUTF8String(removeInstancePath);

			STK_STATUS ret = stkDocMgr->DelInstanceByPlcPath(removeInstanceWPath);

//			LOGE("DelInstanceByPlcPath:%d",ret);
		}
//		else if(cmdType == AddFileCommand::TYPE)
//		{
//
//		}else if(cmdType == AddFileCommand::TYPE)
//		{
//
//		}
        applyState = true;
	}

	return applyState;
}

void SvlWriter::init()
{
	//Writer::SaveTo(targetPath);
	m_stkDocMgr = new Stk_DocumentManager();

	wstring wsFile = GetSVLUTF8String(m_SourcePath);
 
	STK_STATUS status = m_stkDocMgr->LoadDocument(wsFile);
	if (status != STK_SUCCESS)
	{
		LOGE("SvlWriter::SaveTo Error!");
		return ;
	}

	m_topProtoType = NULL;
	int topProtoNum = m_stkDocMgr->GetTopProtoTypeNum();
	if (topProtoNum > 1) {
		LOGE("模型有多个TopProtoType，默认使用了第一个！");
	}

	//获取默认原型
	m_topProtoType = m_stkDocMgr->GetTopProtoTypeByIndex(0);
	status = m_stkDocMgr->SetDefaultTopProto(m_topProtoType);
	if (status != STK_SUCCESS) {
		LOGE("SvlReader::ReadFile SetDefaultTopProto Error!");
		return;
	}

	Model* topModel = (Model*)m_view->GetModel();
	if(!topModel)
	{
		return;
	}
    //注意不对顶级装配进行修改
    m_allModels.clear();
    topModel->GetAllSubModels(m_allModels);
}

}
