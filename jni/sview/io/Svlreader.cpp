/*
 * display.cpp
 *
 *  Created on: 2013-5-31
 *      Author: billy
 */
#include "sview/io/Svlreader.h"
 
#include "Utility.h"

#include "m3d/model/Model.h"
#include "m3d/model/MeshData.h"
#include "m3d/model/PMIData.h"
#include "m3d/model/PolyLine.h"
#include "m3d/model/Body.h"
#include "m3d/utils/Platform.h"

#include "m3d/base/BoundingBox.h"
#include "m3d/base/Color.h"

#ifdef __MOBILE__
#include "FTGL/ftgles.h"
#elif _WIN32
#include "ftgl/ftgl.h"
#endif

#include "sview/PMIAnalysis/PMICreator.h"
#include "sview/views/Parameters.h"
#include "m3d/model/ModelView.h"
#include "m3d/model/ShapeProperty.h"

#include "m3d/base/Vector3.h"
#include "m3d/graphics/CameraNode.h"

#include "m3d/graphics/SectionPlane.h"
#include "m3d/graphics/Material.h"

#include "m3d/model/ComText.h"
#include "m3d/base/Matrix4.h"

#include "m3d/base/Quaternion.h"
#include "m3d/model/ShapeSet.h"
#include "m3d/model/ModelShape.h"
#include "m3d/model/Point.h"
#include "m3d/SceneManager.h"
#include "m3d/utils/StringHelper.h"
#include "m3d/graphics/Texture2D.h"
#include "m3d/ResourceManager.h"

#include "sview/views/Parameters.h"
 
#include "prebuilt/svl2/include/Stk_DocumentManager.h"
#include "prebuilt/svl2/include/Stk_Instance.h"
#include "prebuilt/svl2/include/Stk_ProtoType.h"
#include "prebuilt/svl2/include/Stk_GeoAttribute.h"
#include "prebuilt/svl2/include/Stk_CompositeText.h"
#include "prebuilt/svl2/include/Stk_LineSetEntity.h"
#include "sview/views/View.h"
#include "m3d/model/ExtendInfoManager.h"

using std::ifstream;
using std::ostream;
using std::ios;
using M3D::BoundingBox;
using M3D::Color;
using M3D::PolyLine;
using M3D::Point3D;
using M3D::ModelView;
using M3D::Vector3;
using M3D::ComText;
using M3D::Body;
using M3D::Matrix4;
using M3D::Quaternion;
using M3D::ShapeSet;
using M3D::Point;
using M3D::BaseMaterial;
using M3D::Platform;
using M3D::ModelShape;
using M3D::ExtendInfoManager;
using M3D::SceneManager;

#define SHOW_COLOR_LOG false
#define LOGCOLOR(...) if(SHOW_COLOR_LOG) READER_LOGE(__VA_ARGS__)

 
namespace SVIEW {

SVLReaderListener::SVLReaderListener():Stk_Listener()
{
	m_reader = NULL;
}

SVLReaderListener::~SVLReaderListener()
{

}

void SVLReaderListener::LoadingProcess(STK_FLOAT32 i_fLoadingPercent)
{
	if(m_reader)
	{
		if(i_fLoadingPercent<=1 && i_fLoadingPercent>=0){
			m_reader->SetPercent(0.02 + i_fLoadingPercent*0.7);
		}
	}
}

void SVLReaderListener::SetReader(SvlReader* reader)
{
	this->m_reader = reader;
}

SvlReader::SvlReader(void) :
		Reader() {
	m_M3DModel = NULL;
	m_stkDocMgr = NULL;

	isReadFaceMeshData = true;
	m_IsUseLOD = Parameters::Instance()->m_IsUseLOD;
	
	m_readMaxLodLevel = 0;
	m_readMinLodLevel = 1;

	bool useCatiaMode = Parameters::Instance()->m_IsUseCatiaMode;
	this->SetUseCatia(useCatiaMode);

	m_readEdge = Parameters::Instance()->m_readEdge;

	m_usePrototypeProperty = Parameters::Instance()->m_usePrototypeProperty;;
	m_useInstanceProperty = Parameters::Instance()->m_useInstanceProperty;;
}

SvlReader::~SvlReader(void) {
	Clear();
}

void SvlReader::Clear() {
	if (m_M3DModel) {
        m_M3DModel->Release();
		m_M3DModel = NULL;
	}
    
	map<int, Model *>::iterator ite;
	for (ite = m_ModelMap.begin(); ite != m_ModelMap.end(); ite++) {
		ite->second->Release();
		ite->second = NULL;
	}
	m_ModelMap.clear();
}

Model*
SvlReader::GetModel(int id) {
	if (m_M3DModel == NULL)
    {
        m_M3DModel = ReadFile(this->GetFile());
        
        //如果读取到了model，这里添加一次引用
        if(m_M3DModel)
        {
            m_M3DModel->AddRef();

			m_M3DModel->SetDrawDataPrepared(true,true);
        }
    }
 
	if (id == -1)
		return m_M3DModel;

	map<int, Model*>::iterator iter;
	iter = m_ModelMap.find(id);
	if (iter != m_ModelMap.end()) {
		return iter->second;
	}
	return NULL;
}

void SvlReader::AddProtoType(Model * model) {
	m_ProtoTypeMap.insert(pair<int, Model *>(model->GetProtoTypeId(), model));
}

Model*
SvlReader::FindModelFromProtoTypeID(int id) {
	map<int, Model*>::iterator iter;
	iter = m_ProtoTypeMap.find(id);
	if (iter != m_ProtoTypeMap.end()) {
		return iter->second;
	}
	return NULL;
}

Model*
SvlReader::ReadFile(string file) {
	totaolInsCount = 0;
	m_currentInsIndex= 0;
	this->SetPercent(0.0f/100);

	this->AddSourceFile(file);

	LOGI("LoadReadConfig Begin");
	LoadReadConfig();
	LOGI("LoadReadConfig End");
#ifdef SHOW_READTIME

	long startTime = GetTimeLong();
	long dmTime = 0;
	long dataTime = 0;
	long pmiTime = 0;
	long viewTime = 0;
	long computeBoxTime = 0;

#endif

	InitializeID();
	m_ProtoTypeMap.clear();

	Model* model = NULL;
#ifdef WIN32
 	m_stkDocMgr = Stk_DocumentManager::NewDocumentManager();
#else
 	m_stkDocMgr = new Stk_DocumentManager();
#endif
 

	this->SetPercent(2.0f/100);

	wstring wsFile = Platform::StringToWString(file,"auto");

    LOGI("----------LoadReadSVL begin-------");
    SVLReaderListener* svlReaderListener = new SVLReaderListener();
    svlReaderListener->SetReader(this);
    m_stkDocMgr->SetListener(svlReaderListener);

	//Logger::Instance()->PrintLog("SVLReader DataManager Test===FillAssembly begin==");
	STK_STATUS status = m_stkDocMgr->LoadDocument(wsFile);
	//Logger::Instance()->PrintLog("SVLReader DataManager Test===FillAssembly end==");

	if (status != STK_SUCCESS) {
		READER_LOGE("SvlReader:: DataManager ReadFile Error! code: %d",status);
        //删除datamanager的相关资源
        if (m_stkDocMgr != NULL) {
#ifdef WIN32
		Stk_DocumentManager::DeleteDocumentManager(m_stkDocMgr);
#else
		m_stkDocMgr->~Stk_DocumentManager();
#endif
         m_stkDocMgr = NULL;
       }
        
		return model;
	}

    delete svlReaderListener;

	this->SetPercent(72.0f/100);

	LOGI("--------LoadReadSVL end--------%d",3);
	//isUseLOD=m_stkFile->GetPolygonNum()>50*10000?true:false;
	//READER_LOGI("trinum is -------------------%d", m_stkFile->GetPolygonNum());

#ifdef SHOW_READTIME

	dmTime = GetTimeLong();
	READER_LOGI("dmTime:%d", dmTime-startTime);

#endif

	Stk_ProtoType* topProtoType = NULL;
	int topProtoNum = m_stkDocMgr->GetTopProtoTypeNum();
	if (topProtoNum > 1) {
		READER_LOGE("模型有多个TopProtoType，默认使用了第一个！");
	}

	//获取默认原型
	topProtoType = m_stkDocMgr->GetTopProtoTypeByIndex(0);
	status = m_stkDocMgr->SetDefaultTopProto(topProtoType);
	if (status != STK_SUCCESS) {
		READER_LOGE("SvlReader::ReadFile SetDefaultTopProto Error!");
		return model;
	}

	totaolInsCount = topProtoType->GetTotalInstanceNum(topProtoType);

	//获取文件版本
	Stk_File *stkFile = topProtoType->GetFile();
	wstring wsvlVersion = stkFile->GetSVLVersion();
	string svlVersion = Platform::WStringToString(wsvlVersion,"utf8");
	//READER_LOGI("SvlReader::ReadFile svlVersion:%s", svlVersion.c_str());

	//从svl中一次性读取所有的svl格式点render数据
	this->AddFileMaterialToResourceMgr(stkFile);

	//获取动画文件
	map<STK_ID, Stk_UserData*>* mapAnimationP = topProtoType->GetUserDataByName(
			L"Animation");
	if (mapAnimationP != NULL && mapAnimationP->size() > 0) {
		map<STK_ID, Stk_UserData*>::iterator itMapAnimation =
				mapAnimationP->begin();
		wstring wstrDataName = L"";
		STK_ID id = 0;
		STK_CHAR* dataP = NULL;
		STK_SIZE_T dataSize = 0;
		wstring wstrFileName = L"";
		itMapAnimation->second->GetUserData(wstrDataName, id, dataP, dataSize,
				wstrFileName);

		this->SetXMLAnimationData(dataP);
	}

	//获取用户自定义属性信息
	this->ReadUserData(topProtoType);

	//构造顶级实例
	Stk_Instance* ins = new Stk_Instance(m_stkDocMgr);
	STK_MTX32 topInstanceMatrix =
	{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0,1
	};
	ins->SetPlacement(0, topInstanceMatrix);
	ins->SetProtoID(topProtoType->GetID());
	ins->SetInsName(topProtoType->GetProtoName());
	ins->SetPlcID(0);

	STK_BOOL upHasColor; /*(i) 父实例是否有颜色			*/
	STK_RGBA32 upInsColor; /*(i) 父实例的颜色			*/

	READER_LOGI("ins top hasColor:%d", ins->HasColor());

	if (ins->HasColor()) {
		upInsColor = ins->GetColor();
		upHasColor = TRUE;
	} else {
		upInsColor.Red = 0.8f;
		upInsColor.Green = 0.8f;
		upInsColor.Blue = 0.8f;
		upInsColor.Alpha = 1.0f;
		upHasColor = FALSE;
	}

	model = new Model();
	this->m_M3DModel = model;

	model->SetModelExtInfo(m_view->GetSceneManager()->GetExtendInfoManager());

	m_StkPMIList.clear();
	m_StkViewList.clear();

	GetInstanceData(model, ins, upHasColor, upInsColor,true);

	//读取pmi
	int pmiCount = 0;
	vector<PMIData*> pOutPMIData;
	GetPMIInfo(&m_StkPMIList, &pmiCount, pOutPMIData);
	if (pOutPMIData.size()>0)
	{
		map<int, PMIData*> pmis;
		for (int i = 0; i < pOutPMIData.size(); i++) {
			PMIData* pTmpPMI = pOutPMIData.at(i);
			pmis.insert(
				map<int, PMIData*>::value_type(pTmpPMI->GetID(), pTmpPMI));
		}
		model->SetPMIs(pmis);
	}

	READER_LOGI("SvlReader::GetInstanceData 读取pmi结束 count:%d", pmiCount);

#ifdef SHOW_READTIME
	dataTime = GetTimeLong();
	READER_LOGE("dataTime:%d", dataTime-dmTime);
#endif

	//读取视图 ModelView
	if (!IsCancel()) {
		int viewCount = 0;
		vector<ModelView*> pOutViewData;
		GetViewInfo(&m_StkViewList, model, &viewCount, pOutViewData);

		for (int i = 0; i < viewCount; i++) {
			model->AddModelView(pOutViewData.at(i));
		}
		READER_LOGI(
				"SvlReader::GetInstanceData 读取视图 end ModelViewCount:%d", viewCount);
	}

#ifdef SHOW_READTIME
	viewTime = GetTimeLong();
	READER_LOGE("viewTime:%d", viewTime-pmiTime);

#endif
	//protoTypeArray.clear();
	READER_LOGI("SvlReader::GetInstanceData 读取视图 end ModelViewCount 111");

	//读取topProto存储的额外显示隐藏信息
	ReadTopProtoInfo(ins->GetProtoType(),model);

	//计算BOX
	model->UpdataInfo();

#ifdef SHOW_READTIME
	computeBoxTime = GetTimeLong();
	READER_LOGE("computeBoxTime:%d", computeBoxTime-viewTime);
#endif

	//删除datamanager的相关资源
	if (m_stkDocMgr != NULL) {

		ins->SetProtoID(0);
		delete ins;
		ins = NULL;
#ifdef WIN32
		Stk_DocumentManager::DeleteDocumentManager(m_stkDocMgr);
#else
		m_stkDocMgr->~Stk_DocumentManager();
#endif
		m_stkDocMgr = NULL;

		READER_LOGE("Svlreader::ReadFile delete m_stkDocMgr end");
	}

#ifdef SHOW_READTIME
	READER_LOGE("totalTime:%d", GetTimeLong()-startTime);

#endif

	this->SetPercent(98.0f/100);


	//将颜色转换成材质，根据需要进行

	ConvertModelColorToMaterial(model);

	LOGI("SVL Read ok-----");

	return model;
}

void SvlReader::ConvertModelColorToMaterial(Model* model)
{
	vector<Body*>* bodies = model->GetBodys();
	if (bodies)
	{
		for (int i = 0; i < bodies->size(); i++)
		{
			Body* subBody = bodies->at(i);// ->FindVisiableObject(renderAction);
			vector<Face*>& subFaces = subBody->GetFaces();

			for (int j = 0;j<subFaces.size();j++)
			{
				Face* subFace = subFaces.at(j);
				Color* initColor = subFace->GetInitColor();
				if (initColor)
				{
					BaseMaterial* baseMaterial = this->CovertColorToMaterial(*initColor);

					if (baseMaterial)
					{
						subFace->SetMaterial(baseMaterial);
					}
				}
			}
		}
	}

	vector<Model*>& subModels = model->GetChildren();
	for (int i = 0;i<subModels.size();i++)
	{
		Model* subModel = subModels.at(i);
		this->ConvertModelColorToMaterial(subModel);
	}
}

void SvlReader::GetInstanceData(Model* model, Stk_Instance* ins,
		STK_BOOL upHasColor, /*(i) 父实例是否有颜色			*/
		STK_RGBA32 upInsColor, /*(i) 父实例的颜色				*/
		STK_BOOL upVisible) {
	if (this->IsCancel()) {
		return;
	}

	if(totaolInsCount>0)
	{
		m_currentInsIndex++;
		this->SetPercent(72.0f/100+m_currentInsIndex*1.0f/totaolInsCount*0.2);
	}

	Stk_ProtoType *pProtoType = NULL;
	Stk_Instance *childIns = NULL;
	//model->SetID(this->GetNewID()); /// 对model赋值了自动生成的id

	//设置配置矩阵
	STK_ID outPlcID;
	STK_MTX32 outMatrix;
	ins->GetPlacement(outPlcID, outMatrix);
	model->SetPlcId(outPlcID);

	float* plcMatrixData = outMatrix.PlcMatrix[0];
	model->SetPlaceMatrix(Matrix4(plcMatrixData));

	//instanceID
	//if (pStrParentPlacePath != NULL) {
		model->SetInstanceID(ins->GetID());
	//	READER_LOGI("instanceID:%d", ins->GetID());
	//} else {
	//	model->SetInstanceID(0);
	//}
	model->AddRef();
	m_ModelMap.insert(pair<int, Model*>(model->GetID(), model));

	m_modelPlcPathMap.insert(pair<string,Model*>(model->GetPlcPath(),model));

	pProtoType = ins->GetProtoType();
	if (pProtoType == NULL) {
		READER_LOGE("SvlReader::GetInstanceData : ProtoType is NULL!! Please check the convertor!");
		return;
	}
	model->SetProtoTypeId(pProtoType->GetID());
	if (model->GetName().size() == 0) {
		string name = Platform::WStringToString(ins->GetInsName(),"auto");
		if(pProtoType->GetProtoName().length() == 0)
		{
			name = "Default Ins";
		}
		model->SetName(name);
	}

	READER_LOGI(
			"SvlReader::GetInstanceData : name:%s", model->GetName().c_str());

	GetProtoTypeData(model, pProtoType, upHasColor, upInsColor); //装配中包含几何数据
	//添加自定义属性
	if(this->m_useInstanceProperty && ins->GetMetaDataNum()>0){
		ShapeProperty* shapeProperty = model->GetShapeProperty();
		if(!shapeProperty)
		{
			shapeProperty = new ShapeProperty();
			model->SetShapeProperty(shapeProperty);
		}
		shapeProperty->AddRef();

		vector<Stk_MetaData*> protoTypeProperties;

		for(int i = 0;i<ins->GetMetaDataNum();i++)
		{
			protoTypeProperties.push_back(ins->GetMetaDataByIndex(i));
		}

		FillShapeProperty(protoTypeProperties,shapeProperty);

		shapeProperty->Release();
	}

	//instance 的可见性设置
	if (ins->GetDisplayState() == STK_INS_NODISP || upVisible == FALSE) {
		model->SetOrigVisible(false);
	} else {
		model->SetOrigVisible(true);
	}

	STK_UINT32 Type = pProtoType->GetType();
	if (true || Type == PROTO_TYPE_ASSY)
	{
//		READER_LOGI("Children:%d", pProtoType->GetChildInsNum());
		for (int ii = 0; ii < pProtoType->GetChildInsNum(); ii++) {
			if (IsCancel())
				return;
			STK_RGBA32 curInsColor = { 0, 0, 0, 0 };
			STK_BOOL hasColor;
			STK_BOOL upInsVisible;

			childIns = pProtoType->GetChildIns(ii);

			if (childIns->GetDisplayState() == STK_INS_NODISP || upVisible == FALSE) {
				upInsVisible = false;
			}
			else {
				upInsVisible = true;
			}

			// 实例颜色
			if (upHasColor)
			{
				curInsColor = upInsColor;
				hasColor = TRUE;
			}else if (childIns->HasColor()) {
				curInsColor = childIns->GetColor();
				hasColor = TRUE;
				if(curInsColor.Red<0 || curInsColor.Green<0 || curInsColor.Blue<0)
				{
					hasColor = FALSE;
				}
			} else {
				curInsColor.Red = 0.5f;
				curInsColor.Green = 0.5f;
				curInsColor.Blue = 0.5f;
				curInsColor.Alpha = 0.0f;
				hasColor = FALSE;
			}
			//LOGCOLOR("child ins:hasColor:%d,curInsColor:%X",hasColor,curInsColor);

			if (!this->IsCancel()) {
				Model* submodel = new Model();
				GetInstanceData(submodel, childIns, hasColor, curInsColor,
						upInsVisible);
				model->AddSubModel(submodel);

				if (curInsColor.Red<0 && curInsColor.Alpha>0.0f)
				{
					model->SetInitAlpha(curInsColor.Alpha);
				}
			}
		}
	}
//	pmiTime = getTimeLong();
//	READER_LOGE("pmiTime:%d", pmiTime-dataTime);
}

void SvlReader::ReadTopProtoInfo(Stk_ProtoType* topProto, Model* topModel)
{
	//copy from 设计器
	const vector<wstring>& strPathList = topProto->GetAllInsAttrPlcPath();
	int nbList = strPathList.size();
	StkDisplayStateEnum emDisplayState;

	map<string, Model*>::iterator findMode;

	map<string, Model*>::iterator findEnd = m_modelPlcPathMap.end();

	for (int i = 0; i < nbList; i++)
	{
		if (STK_ERROR == topProto->GetInsDisplayStateByPlcPath(strPathList[i], emDisplayState))
			continue;

		findMode = m_modelPlcPathMap.find("0|" + Platform::WStringToString(strPathList[i],"utf8"));

		if (findMode != findEnd)
		{
			Model* model = findMode->second;

			if (emDisplayState == STK_NO_DISPLAY)
			{
				model->SetOrigVisible(false);
			}
			else
			{
				model->SetOrigVisible(true);
			}
		}
	}
}

void SvlReader::GetProtoTypeData(Model* model, Stk_ProtoType *pProtoType,
		STK_BOOL upHasColor, const STK_RGBA32& upInsColor) {
//	READER_LOGI("SvlReader::GetProtoTypeData begin.");
	//查找是否已经读取了物理文件，如果读取过则通过物理文件进行显示数据的拷贝
	Model* pModel = FindModelFromProtoTypeID(pProtoType->GetID());
	Color instancColor(upInsColor.Red,upInsColor.Green,upInsColor.Blue,1-upInsColor.Alpha);
	if (pModel != NULL)
	{
		CopyModelDrawData(model, pModel);

		//如果实例包含颜色，将颜色值赋值给拷贝出来的pModel对象
		if(upHasColor)
		{
			Color instancColor(upInsColor.Red,upInsColor.Green,upInsColor.Blue,1-upInsColor.Alpha);
			model->SetInitColor(instancColor);
		}

//		LOGE("model11 %s,uphasColor %d Color %s",model->GetName().c_str(),upHasColor,instancColor.Tostring().c_str());
		return;
	}
	//
	//将protoType  中的材质信息，render数据
	this->AddFileMaterialToResourceMgr(pProtoType->GetFile());

	AddProtoDataToModel(model, pProtoType, upHasColor, upInsColor);
	model->SetOrigVisible(pProtoType->GetDisplayState());
//	LOGE("model22 %s,uphasColor %dColor %s",model->GetName().c_str(),upHasColor,instancColor.Tostring().c_str());
}

//获取PMI信息
bool SvlReader::GetPMIInfo(vector<Stk_PMI*>* pStkPMIList, int *outPMINum,
		vector<PMIData*>& outPMIData) {

	int count;
	int Error = FALSE;
	CHAR strID[32];
	int PMINum = 0;
	int MaxNum = 1000;

	//vector<FTPoint> textpnt;
	vector<int> num;
	vector<int> type;

	FPOINT3 pnts;
	int idx = 0, j = 0, n = 0;

	vector<FPOINT3> vecPolylinePnts;

	if (pStkPMIList->size() <= 0) {
		return true;
	}

	outPMIData = vector<PMIData*>();
	PMICreator *pPMICreator = new PMICreator;

	for (int i = 0; i < pStkPMIList->size(); i++) {

		Stk_PMI *pmi = pStkPMIList->at(i);
		STK_ID PMIid = pmi->GetNode()->GetID();
		string PMIName = "PMI::";
//		 _itoa_s( PMIid, strID, 10);
		sprintf(strID, "%d", PMIid);
		PMIName += strID;
 
		//保存名称
		PMIData *pPMIData = new PMIData;
		pPMIData->SetID(PMIid);
//		 pPMISep->setName((SbName)(PMIName.c_str()));
		READER_LOGI("Read PMI: i:%d pmiId:%d ", i, pPMIData->GetID());

		//设置PMI显隐状态
		pPMIData->SetVisible(pmi->IsDisplay());

		//设置PMIPick状态

//		//设置PMI颜色 pmi暂无颜色
//		 STK_RGBA32 pmiColor;
//		 pmiColor = pmi->GetColor();
		Color blue(0, 0, 1, 1);
		pPMIData->SetInitColor(blue); //设置为蓝色

		//平行于屏幕类型PMI
		STK_BOOL ParallelScreenflg = pmi->IsParallelScreen();

		pPMIData->m_IsParallelScreen = ParallelScreenflg;
		pPMIData->m_Type = pmi->GetType();

		// 设置定义平面
		STK_MTX32 DefPlane = pmi->GetDefPlane();
		pPMIData->SetDefPlane(Matrix3x4((float*) DefPlane.PlcMatrix));

		vector<STK_PNT32> pnt;
		vector<STK_UINT32> Indices;

		/*
		 * 创建PMI延长线 start
		 */
//		count = 0;
//		pnt.clear();
//		Indices.clear();
//		Error = pPMICreator->CreateExtensionLines(pmi, pnt, Indices);
//		if (Error != TRUE) {
//			continue;
//		}
//		if (pnt.size() > 0 && Indices.size() > 0) {
//			//ISSUE:PMI android 生成延长线顶点列表
//			SetPolyLineForPMI(pPMIData, pnt, Indices);
//		}
		vecPolylinePnts.clear();
		vector<Stk_Extensionline*> vecExtline = pmi->GetExLines();
		if(vecExtline.size()>0)
			pPMICreator->createExtensionLines(vecExtline, vecPolylinePnts);
		n = vecPolylinePnts.size();
		//READER_LOGI("末端符号的数量为：(%d)", n);
//				for (idx = 0; idx < n; idx++) {
//					pnts = vecPolylinePnts.at(idx);
//					for (j = 0; j < pnts.size(); j++)
//						READER_LOGI(
//								"末端符号点的坐标为：(%f,%f,%f)", pnts.at(j).X, pnts.at(j).Y, pnts.at(j).Z);
//				}
		if (n > 0)
		{
			//ISSUE:PMI android 创建末端符号
			SetPolyLineForPMI(pPMIData, vecPolylinePnts);
		}
		vecPolylinePnts.clear();

		/*
		 * 创建PMI延长线 end
		 */
		READER_LOGI("创建PMI延长线");
		/*
		 * 创建外框 start
		 */
		count = 0;
		pnt.clear();
		Indices.clear();
		Error = pPMICreator->CreateOutFrame(pmi, pnt, Indices);
		if (Error != TRUE) {
			continue;
		}

		if (pnt.size() > 0 && Indices.size() > 0) {
			//ISSUE:PMI android 生成外框顶点列表
			SetPolyLineForPMI(pPMIData, pnt, Indices);
		}
		/*
		 * 创建外框 end
		 */
		READER_LOGI("创建外框 end");
		/*
		 * 创建包围框 start
		 */
		count = 0;
		pnt.clear();
		Indices.clear();
		Error = pPMICreator->CreateAnchorPoints(pmi, pnt, Indices);
		if (Error != TRUE) {
			continue;
		}
		if (pnt.size() > 0 && Indices.size() > 0) {
			//ISSUE:PMI android 生成包围框顶点列表
			SetPolyLineForPMI(pPMIData, pnt, Indices);
		}
		/*
		 * 创建包围框 end
		 */
		READER_LOGI("创建包围框 end");
		/*
		 * 创建末端符号 start
		 */
//		count = 0;
//		pnt.clear();
//		Indices.clear();
		StkTermTypeEnum nEndSymbol = TERM_PATSMNONE;
		//Error = pPMICreator->CreateEndSymbol(pmi, pnt, Indices, nEndSymbol);
		vecPolylinePnts.clear();
		Error = pPMICreator->CreateEndSymbol(pmi, vecPolylinePnts, nEndSymbol);
//		if (Error != TRUE) {
//			continue;
//		}

		n = vecPolylinePnts.size();
		READER_LOGI("末端符号的数量为：(%d)", n);
		for (idx = 0; idx < n; idx++) {
			pnts = vecPolylinePnts.at(idx);
			for (j = 0; j < pnts.size(); j++)
				READER_LOGI(
						"末端符号点的坐标为：(%f,%f,%f)", pnts.at(j).X, pnts.at(j).Y, pnts.at(j).Z);
		}
		if (n > 0) {
			//ISSUE:PMI android 创建末端符号
			SetPolyLineForPMI(pPMIData, vecPolylinePnts);
		}
		vecPolylinePnts.clear();
		/*
		 * 创建末端符号 end
		 */
		READER_LOGI("创建末端符号 end");
		/*
		 * 创建文字 start
		 */


		STK_BOX32 textBox;
		memset(&textBox, 0, sizeof(STK_BOX32));

		//ISSUE:PMI android 创建文字
		vector<Stk_ComText*> vecComTexts = pmi->GetComText();
		int nComTextSize = vecComTexts.size();
		READER_LOGI("PMI android 创建文字 start cnt:%d",nComTextSize);

		pPMICreator->ClearTextBox();

		//----------transport from sview2.0 SceneCreator.cpp -----------

		//如果是平行于屏幕类型
		if (ParallelScreenflg) {
			vector<Stk_Leader*> pLeads = pmi->GetLeaders();
			Stk_CurveEntity *pCurve = NULL;
			Stk_PolyLine *pPolyLinePtr = NULL;
			Stk_Leader *pLeader = NULL;

			//引线相关的某个顶点
			STK_PNT32 stubChrPnt;

			//引线相关的某个标志
			bool bStubChrFlg = false;

			vector<STK_PNT32> leaderPnts;
		
			float ofWidth = 0.0, ofHeight = 0.0f;
			STK_DIR32 oDirXVec, oDirYVec;
			float oSpecing = 0.0f;
			int nComTextsNum = 0;
			int u = 0;

			READER_LOGI("如果有引线... size:%d", pLeads.size());
			//如果有引线，且类型为CURVE_TYPE_POLYLINE，且顶点个数>2则设置标志和读取pnt。
			if (pLeads.size() > 0) {
				pLeader = pLeads.at(0);
				pCurve = pLeader->GetCurve();

				//如果是折线
				if (pCurve != NULL && pCurve->GetCurveType() == 1) { //if (nTypeEnum == CURVE_TYPE_POLYLINE)

					pPolyLinePtr = (Stk_PolyLine *) pCurve;
					leaderPnts = pPolyLinePtr->GetPoints();
					if (leaderPnts.size() > 2) {
						bStubChrFlg = true;
						stubChrPnt = leaderPnts.at(1); //第二个顶点
//						pPMIData->m_outramLocation = Vector3(stubChrPnt.X,stubChrPnt.Y,stubChrPnt.Z);
					}
				}
			}

			nComTextsNum = vecComTexts.size();
			READER_LOGI("循环每个Stk_ComText文本 nComTextsNum:%d", nComTextsNum);
			//循环每个Stk_ComText文本
			for (u = 0; u < nComTextsNum; u++) {
				int v = 0, w = 0, nTextNum = 0, nSize2 = 0, nLin = 0;

				STK_INT32 nMaxLen = 0;

				Stk_Text *Text = NULL, *tmpTxtP = NULL;
				STK_PNT32 Pos;
				Stk_ComText *stkComText = vecComTexts.at(u); //当前Stk_ComText

				STK_BOX32 outerBox;
				vector<Stk_Text*> Texts = stkComText->GetTexts(); //获取所有Stk_Text
				vector<STK_INT32> aNember;
				//typedef vector<STK_INT32> SameLine;
				map<STK_INT32, vector<STK_INT32> > mapLine;

				vector<Stk_Text*> Texts2;
				vector<Stk_Text*> useSVLTexts2;
				STK_PNT32 Loc1;
				nTextNum = Texts.size();

				memset(&Pos, 0, sizeof(STK_PNT32));
				memset(&Loc1, 0, sizeof(STK_PNT32));
				memset(&outerBox, 0, sizeof(STK_PNT32));

				outerBox = stkComText->GetOuterBox();
				Loc1.X = outerBox.BoundBox[0][0];
				Loc1.Y = outerBox.BoundBox[1][1];

				READER_LOGI("是否固定位置:%d", pPMIData->m_Type==8000);
				//如果PMI不是固定位置的类型：比如固定在左下角的“技术要求”
				if (8000 != pPMIData->m_Type) {
					//循环每行文字，根据y坐标判断有多少行，
					//并且将每行对应的文本索引（多个）记录下来
					for (v = 0; v < nTextNum; v++) {
						Text = Texts.at(v);
						Pos = Text->GetInnerLoc();
						bool bFirst = true;

						if(v == 0)
						{
							pPMIData->m_outramLocation = Vector3(Pos.X,Pos.Y,Pos.Z);
						}

						//向aNember中push 当前文本向后
						//y坐标相同的除第二 （[v + 1]）个文本外的索引值？
						for (w = v + 1; w < nTextNum; w++) {
							tmpTxtP = Texts.at(w);
							if (Pos.Y == tmpTxtP->GetInnerLoc().Y) {
								if (bFirst) {
									aNember.push_back(w - 1);
									bFirst = false;
								}
								aNember.push_back(w);
								v = w;
							}
						}

						//如果一个相同y的也没有，则push当前索引
						if (aNember.size() <= 0)
							aNember.push_back(v);

						//将aNember插入到mapLine
						mapLine.insert(pair<STK_INT32, vector<STK_INT32> >(nLin, aNember));
						nLin++; //行号？
						aNember.clear();
					}

					//最终有mapLine行，循环每行
					for (v = 0; v < mapLine.size(); v++) {
						nLin = mapLine.size();
						aNember = mapLine[v];
						int member = aNember.size();
						int linNo = 0;
						wstring tmpWstrs;
						//获取第0个文本的属性？
						Texts.at(0)->GetCharWidthHeight(ofWidth, ofHeight);
						Texts.at(0)->GetInnerXYAxis(oDirXVec, oDirYVec);
						oSpecing = Texts.at(0)->GetCharSpacing();

						READER_LOGI(
								"faceScreenText :W:%f,H:%f", ofWidth, ofHeight);

						float maxH = 0.8f;
						if (ofWidth > maxH) {
							ofWidth = maxH;
							ofHeight = maxH;
						}

						//循环每个当前行的文本，将文本串加入tmpWstrs
						for (w = 0; w < member; w++) {
							linNo = aNember.at(w);

							Text = Texts.at(linNo);
							tmpWstrs.append(Text->GetText());
						}

						//将文本设置给pTmpstr，并push入Texts2
						if (tmpWstrs.size()) {
							//临时Text
							Stk_Text *pTmpstr = new Stk_Text;
							pTmpstr->SetCharWidthHeight(ofWidth, ofHeight);
							pTmpstr->SetInnerXYAxis(oDirXVec, oDirYVec);
							pTmpstr->SetCharSpacing(oSpecing);

							//设置引线相关的顶点
							if (bStubChrFlg)
								pTmpstr->SetInnerLoc(stubChrPnt);
							pTmpstr->SetText(tmpWstrs);
							STK_PNT32 pntTmp =
									Texts.at(aNember.at(0))->GetInnerLoc();
							pTmpstr->SetInnerLoc(pntTmp);
							Texts2.push_back(pTmpstr);
						}
					} //end for (v = 0; v < mapLine.size(); v++) 最终组装了Texts2

					//如果mapline为空，则复制Texts到Texts2
					if (mapLine.size() <= 0) {
						for (v = 0; v < Texts.size(); v++)
							Texts2.push_back(Texts.at(v));
					}

					//获取最长行
					nSize2 = Texts2.size();
					for (v = 0; v < nSize2; v++) {
						int nSz = Texts2.at(v)->GetText().size();
						if (nMaxLen < nSz)
							nMaxLen = nSz;
					}
					//如果引线参数为真，加入Text2
					if (bStubChrFlg) {
						Stk_Text *pStubTxt = new Stk_Text();
						pStubTxt->SetText(L"───");

						//画一个和最长行相同长度的--线
						if (nMaxLen > 0) {
							wstring wstr;
							wstr.append(nMaxLen, L'─');
							pStubTxt->SetText(wstr);
						}

						Texts2.at(0)->GetCharWidthHeight(ofWidth, ofHeight);
						Texts2.at(0)->GetInnerXYAxis(oDirXVec, oDirYVec);
						oSpecing = Texts2.at(0)->GetCharSpacing();

						pStubTxt->SetCharWidthHeight(ofWidth, ofHeight);
						pStubTxt->SetInnerXYAxis(oDirXVec, oDirYVec);
						pStubTxt->SetCharSpacing(oSpecing);
						pStubTxt->SetInnerLoc(stubChrPnt);

						Texts2.push_back(pStubTxt);
					}
				} else {
					//^^ if (8000 != pPMISep->m_Type)  若是8000型则直接赋值。
					useSVLTexts2 = Texts;
				}

				//TODO:设置对齐方式？？
				//Paralleltext->justification = SoText2::Justification::LEFT;
				nSize2 = Texts2.size();
				//文本个数？
				//Paralleltext->string.setNum(nSize2);
				//循环Texts2的每个文本
				vector<Stk_Text*> texts;

				if(nSize2>0)
				{
				
					for (j = 0; j < nSize2; j++) {
						Stk_Text* newText = new Stk_Text();
						Stk_Text* tmpText = Texts2[j];
						STK_FLOAT32 charSpacing = tmpText->GetCharSpacing();

						newText->SetCharSpacing(charSpacing);

						STK_FLOAT32 charWidth, charHeight;
						tmpText->GetCharWidthHeight(charWidth, charHeight);
						newText->SetCharWidthHeight(charWidth, charHeight);

						STK_PNT32 innerLoc = tmpText->GetInnerLoc();
						newText->SetInnerLoc(innerLoc);

						STK_DIR32 x, y;
						tmpText->GetInnerXYAxis(x, y);
						newText->SetInnerXYAxis(x, y);

						newText->SetText(tmpText->GetText());
						texts.push_back(newText);

						string tmpStr = Platform::WStringToString(tmpText->GetText());
						READER_LOGI("Text:%s", tmpStr.c_str());
						READER_LOGI(
							"charWidth:%f,charHeight:%f ", charWidth, charHeight);

						pPMIData->fontSize = charHeight;
					}
				
					//释放Texts2
					for (int k = 0;k<Texts2.size();k++)
					{
						delete Texts2[k];
					}
				}
				else if (useSVLTexts2.size()>0)
				{
					Texts2 = useSVLTexts2;
					for (j = 0; j < nSize2; j++) {
						Stk_Text* newText = new Stk_Text();
						Stk_Text* tmpText = Texts2[j];
						STK_FLOAT32 charSpacing = tmpText->GetCharSpacing();

						newText->SetCharSpacing(charSpacing);

						STK_FLOAT32 charWidth, charHeight;
						tmpText->GetCharWidthHeight(charWidth, charHeight);
						newText->SetCharWidthHeight(charWidth, charHeight);

						STK_PNT32 innerLoc = tmpText->GetInnerLoc();
						newText->SetInnerLoc(innerLoc);

						STK_DIR32 x, y;
						tmpText->GetInnerXYAxis(x, y);
						newText->SetInnerXYAxis(x, y);

						newText->SetText(tmpText->GetText());
						texts.push_back(newText);

						string tmpStr = Platform::WStringToString(tmpText->GetText());
						READER_LOGI("Text:%s", tmpStr.c_str());
						READER_LOGI(
							"charWidth:%f,charHeight:%f ", charWidth, charHeight);

						pPMIData->fontSize = charHeight;
					}
					//引用的是SVL中的数据，不用释放
				}


				Stk_ComText comText;
				comText.SetTexts(texts);

				ComText* myText = ConvertStkComTextToComText(&comText);

				//填充文本
				pPMIData->m_ComTexts.push_back(myText);

			}
		}
		//-----------------------------end transport---------------------------------

		//如果不是面朝屏幕的类型PMI
		for (idx = 0; idx <nComTextSize&& !ParallelScreenflg; idx++) {
			//复制Stk_ComText 到 ComText
			Stk_ComText* tmpComText = pmi->GetComText()[idx];
			ComText* comText = ConvertStkComTextToComText(tmpComText);
			pPMIData->m_ComTexts.push_back(comText);
		}

		BoundingBox box = pPMIData->GetComTextsBox();

		//LOGE("Comtexts box %s",box.Tostring().c_str());

		if(box.Defined())
		{
			textBox.BoundBox[0][0] = box.m_min.m_x;
			textBox.BoundBox[0][1] = box.m_min.m_y;
			textBox.BoundBox[0][2] = box.m_min.m_z;

			textBox.BoundBox[1][0] = box.m_max.m_x;
			textBox.BoundBox[1][1] = box.m_max.m_y;
			textBox.BoundBox[1][2] = box.m_max.m_z;
		}

		/*
		 * 创建文字 end
		 */
		READER_LOGI("创建文字 end");
		/*
		 * 创建PMI引出线 start
		 */
		count = 0;
		pnt.clear();
		Indices.clear();

		//Error = pPMICreator->CreateLeaders(pmi, pnt, Indices);

//		if (Error != TRUE) {
//			continue;
//		}
//		if (pnt.size() > 0 && Indices.size() > 0) {
//
//			//ISSUE:PMI android 生成引出线顶点列表
//		SetPolyLineForPMI(pPMIData, pnt, Indices);
//		}
		StkPMITypeEnum EnumType = pmi->GetType();
		vector<Stk_Leader*> pLeaders = pmi->GetLeaders();

		//READER_LOGI("PMI的类型为：(%d)",EnumType );
		if (
		//StkPMITypeEnum::PMI_TYPE_AngularDimension==EnumType
//				||StkPMITypeEnum::PMI_TYPE_DiameterDimension == EnumType
//				||StkPMITypeEnum::PMI_TYPE_RadiusDimension   == EnumType
//				||StkPMITypeEnum::PMI_TYPE_DistanceDimension == EnumType
//				||StkPMITypeEnum::PMI_TYPE_LengthDimension   == EnumType
//				||StkPMITypeEnum::PMI_TYPE_TypeDimension == EnumType
		1) {


			pPMICreator->createLeader(pLeaders, textBox, vecPolylinePnts, pmi);

		} else {
			for (idx = 0; idx < pLeaders.size(); idx++)
				pPMICreator->createLeader(pLeaders[idx], vecPolylinePnts, pmi);
		}

		n = vecPolylinePnts.size();
		READER_LOGI("引出线的数量为：(%d)", n);
		FPOINT3 _tempPnts;
		for (idx = 0; idx < n; idx++) {
			_tempPnts = vecPolylinePnts.at(idx);
			for (j = 0; j < _tempPnts.size(); j++)
				READER_LOGI(
						"引出线点的坐标为：(%f,%f,%f)", _tempPnts.at(j).X, _tempPnts.at(j).Y, _tempPnts.at(j).Z);
		}
		if (n > 0)
			SetPolyLineForPMI(pPMIData, vecPolylinePnts);
		/*
		 * 创建PMI引出线 end
		 */
		READER_LOGI("创建PMI引出线 end");
		/*
		 * 创建PMI特殊线 start
		 */
		count = 0;
		pnt.clear();
		Indices.clear();
		Error = pPMICreator->CreateSpeciallLines(pmi, pnt, Indices);
		if (Error != TRUE) {
			continue;
		}
		if (pnt.size() > 0 && Indices.size() > 0) {

			//ISSUE:PMI android 生成特殊线顶点列表
			SetPolyLineForPMI(pPMIData, pnt, Indices);
		}
		/*
		 * 创建PMI特殊线 end
		 */

		READER_LOGI("创建PMI特殊线 end");

		outPMIData.push_back(pPMIData);
		PMINum++;
	}

	*outPMINum = PMINum;
	READER_LOGI("PMINum:%d", PMINum);

	delete pPMICreator;
	return TRUE;
}

ComText* SvlReader::ConvertStkComTextToComText(Stk_ComText* stkComText) {
	ComText* comText = new ComText();

	STK_BOX32 outerBox = stkComText->GetOuterBox();
	Vector3 minPos(outerBox.BoundBox[0]); //可能不对
	Vector3 maxPos(outerBox.BoundBox[1]);
	BoundingBox box(minPos, maxPos);
	comText->SetBox(box);

	vector<Stk_Text*> tmpTexts = stkComText->GetTexts();

	for (int j = 0; j < tmpTexts.size(); j++) {
		CText* newText = new CText();
		Stk_Text* tmpText = tmpTexts[j];
		STK_FLOAT32 charSpacing = tmpText->GetCharSpacing();

		//newText->SetCharSpacing(charSpacing);

		STK_FLOAT32 charWidth, charHeight;
		tmpText->GetCharWidthHeight(charWidth, charHeight);
		newText->SetCharWidthHeight(charWidth, charHeight);

		//STK_WCHAR oText[64] =
		//{ 0 };
		wstring tmpWS = tmpText->GetFonts();
//		if (NULL != oText)
//		{
//			wstring tmpWS = Platform::STKWCHARStrToWString(
//					(STK_WCHAR *) oText);
//			string tmpfont;
		//WStringToString(tmpWS, tmpfont);
		//newText->SetFonts(tmpfont.c_str());
		//newText->SetFont(Platform::WStringToString(tmpWS).c_str());
//			READER_LOGI("font is %s",tmpfont.c_str());
		//}

		STK_PNT32 innerLoc = tmpText->GetInnerLoc();
		Vector3 inLoc(innerLoc.X, innerLoc.Y, innerLoc.Z);
		newText->SetInnerLoc(inLoc);

		STK_DIR32 x, y;
		tmpText->GetInnerXYAxis(x, y);
		Vector3 xAxis(x.X, x.Y, x.Z);
		Vector3 yAxis(y.X, y.Y, y.Z);
		newText->SetInnerXYAxis(xAxis, yAxis);
		string textString = Platform::WStringToString(tmpText->GetText(), "auto");
		newText->SetText(textString);
		comText->AddCText(newText);

	}
	return comText;
}

//为PMIData组合添加PolyLine
void SvlReader::SetPolyLineForPMI(PMIData* pPMIData,
		vector<STK_PNT32>& vertexBuf, vector<STK_UINT32>& indexBuf) {
	//转化pnt列为float列
	float * tmpVertexBuf = new float[3 * (vertexBuf.size())];
	for (int j = 0; j < vertexBuf.size(); j++) {
		tmpVertexBuf[j * 3] = vertexBuf[j].X;
		tmpVertexBuf[j * 3 + 1] = vertexBuf[j].Y;
		tmpVertexBuf[j * 3 + 2] = vertexBuf[j].Z;
	}

	//处理index列
	int indexNum = 0;
	M3D_INDEX_TYPE * tmpIndexBuf = new M3D_INDEX_TYPE[indexBuf.size()];
	for (int j = 0; j < indexBuf.size(); j++) {
		tmpIndexBuf[indexNum] = indexBuf[j];
		//如果为-1则：
		if (indexBuf[j] == -1 && j != 0) {
			//保存当前折线
			PolyLine *pLine = new PolyLine();
			pLine->SetBuffer(vertexBuf.size(), tmpVertexBuf, indexNum,
					tmpIndexBuf);
			pPMIData->m_Lines.push_back(pLine);

			//从新开始计数
			indexNum = 0;
			//delete[] tmpIndexBuf;
			//tmpIndexBuf = new M3D_INDEX_TYPE[indexBuf.size()];
			continue;
		}
		indexNum++;
	}

	//保存当前折线
	PolyLine *pLine = new PolyLine();
	pLine->SetBuffer(vertexBuf.size(), tmpVertexBuf, indexNum, tmpIndexBuf);
	pPMIData->m_Lines.push_back(pLine);

	delete[] tmpVertexBuf;
	delete[] tmpIndexBuf;
}

//为PMIData组合添加PolyLine
void SvlReader::SetPolyLineForPMI(PMIData* pPMIData,
		vector<FPOINT3>& vecPolylinePnts) {

	for (int iLine = 0; iLine < vecPolylinePnts.size(); iLine++) {
		FPOINT3 &curLine = vecPolylinePnts.at(iLine);
		//vertexBuffer
		//转化pnt列为float列
		float * tmpVertexBuf = new float[3 * (curLine.size())];
		M3D_INDEX_TYPE * tmpIndexBuf = new M3D_INDEX_TYPE[curLine.size()];
		memset(tmpIndexBuf, 0, curLine.size());

		for (int j = 0; j < curLine.size(); j++) {
			tmpVertexBuf[j * 3] = curLine[j].X;
			tmpVertexBuf[j * 3 + 1] = curLine[j].Y;
			tmpVertexBuf[j * 3 + 2] = curLine[j].Z;

			//indexBuffer
			tmpIndexBuf[j] = j;
		}
		//
		PolyLine *pLine = new PolyLine();
		pLine->SetBuffer(curLine.size(), tmpVertexBuf, curLine.size(),
				tmpIndexBuf);
		pPMIData->m_Lines.push_back(pLine);

		delete[] tmpVertexBuf;
		delete[] tmpIndexBuf;
	}

}
//获取视图信息
bool SvlReader::GetViewInfo(vector<Stk_View*> *pStkViewList, Model* inModel,
		int *outViewNum, vector<ModelView*>& outViewData) {
	READER_LOGI("Svlreader::GetViewInfo");
	int count;
	int Error = FALSE;
	CHAR strID[32];
	int ViewNum = 0;
	Stk_View * pViewData = NULL;
	Stk_Camera * pCameraData = NULL;
	Stk_PlaneSurface *pPlaneSurfaceData = NULL;
	vector<Stk_NodeConnector*> nodeConnectorsData;
	*outViewNum = 0;

	//获取视图个数
	ViewNum = pStkViewList->size();
	READER_LOGI("View Num %d",ViewNum);

	if (ViewNum <= 0) {
		return true;
	}

	//*outViewData = (ModelView **) malloc(sizeof(ModelView*) * ViewNum);

	for (int i = 0; i < ViewNum; i++) {
		pViewData = NULL;
		pCameraData = NULL;
		pPlaneSurfaceData = NULL;
		nodeConnectorsData.clear();

		pViewData = pStkViewList->at(i);

		if (pViewData == NULL) {
			continue;
		}

		wstring viewName = pViewData->GetName();
		bool viewIsActivated = pViewData->IsActivated();
		StkViewUsageEnum viewType = pViewData->GetUsageType();
		switch(viewType)
		{
		case VIEW_USAGE_GENERAL_VIEW:
		case VIEW_USAGE_SV_USER_VIEW:
		case VIEW_USAGE_SV_USER_CLIPVIEW:
		case VIEW_USAGE_PROE_BASE_VIEW:
		case VIEW_USAGE_PROE_USER_VIEW:
			//go ahead

			break;
		default:
//			continue;
			break;
		}

		ModelView * pView = new ModelView(); // Stk_View(pProtoType->GetID());
		//获取视图基本信息
		int oldId = pViewData->GetID();
		int viewID = i + 1; //pViewData->GetID();
		READER_LOGI("ViewId:%d new:%d", oldId, viewID);

		pView->SetID(oldId);
		string nameString = Platform::WStringToString(viewName,"auto");
		pView->SetName(nameString.c_str());
		READER_LOGI("ViewName:%s", nameString.c_str());
		//pView->setEnable(viewIsActivated);
//		pView->setHasClippingPlan(viewHasClipPlan);
		pView->SetSvlUseType(pViewData->GetUsageType());
		pView->SetViewType(ModelView::OrignalView); //svl中的视图都做为原始视图

		//获取Camera信息
		pCameraData = pViewData->GetCamera();
		if (pCameraData != NULL) {
			CameraNode cameraInfo;
			cameraInfo.SetID(pCameraData->GetID());

			//获取镜头原始位置
			Vector3 pos(pCameraData->GetOriginPoint().X,
					pCameraData->GetOriginPoint().Y,
					pCameraData->GetOriginPoint().Z);

			//获取Target方向向量
			Vector3 target(pCameraData->GetTargetVector().X,
					pCameraData->GetTargetVector().Y,
					pCameraData->GetTargetVector().Z);

			Vector3 up(pCameraData->GetUpVector().X,
					pCameraData->GetUpVector().Y, pCameraData->GetUpVector().Z);


			if((target.m_x!= -1.0f || target.m_y != -1.0f || target.m_z != -1.0f)&&
				 (up.m_x != -1.0f || up.m_y != -1.0f || up.m_z != -1.0f))
			{
				//构造出旋转,参考李坤的算法,貌似和已有的通过direct和up构造的方法不一致.
				Vector3 x =target.CrossProduct(up);
				Vector3 y = up;
				Vector3 z = -target;

				Quaternion rotation;
				rotation.FromAxes(x,y,z);

				cameraInfo.SetRotation(rotation);
				cameraInfo.SetPosition(pos);

				cameraInfo.SetFov(pCameraData->GetAngle());//镜头角度
				//cameraInfo.focus = pCameraData->GetFocus();//焦点
				cameraInfo.SetZoom(pCameraData->GetZoomFactor());//缩放因子
				cameraInfo.SetOrthographic(pCameraData->GetProjectType());//投影类型

				pView->SetCamera(cameraInfo);

				pView->SetUpDataCamera(true);
			}else
			{
				pView->SetUpDataCamera(false);
			}

			READER_LOGI(
					"viewId:%d ref cameraId:%d", pView->GetID(), cameraInfo.GetID());
		}

		//获取剖面信息
		const vector<Stk_SurfaceEntity *>& allPlaneList =
				pViewData->GetAllClippingPlan();
		for (int j = 0; j < allPlaneList.size(); j++) {

			Stk_PlaneSurface * pPlaneSurfaceData =
					(Stk_PlaneSurface*) allPlaneList[j];

			Vector3 tmpOrigin(pPlaneSurfaceData->GetOrigin().X,
					pPlaneSurfaceData->GetOrigin().Y,
					pPlaneSurfaceData->GetOrigin().Z); //起始范围

			//法线修正为负值（负值就对了）
			Vector3 tmpNormal(-pPlaneSurfaceData->GetNormal().X,
					-pPlaneSurfaceData->GetNormal().Y,
					-pPlaneSurfaceData->GetNormal().Z); //法线

			STK_DIR32 UDIR, VDIR;
			pPlaneSurfaceData->GetUVDir(UDIR, VDIR);

			Vector3 tmpUDir(UDIR.X, UDIR.Y, UDIR.Z); //U方向
			Vector3 tmpVDir(VDIR.X, VDIR.Y, VDIR.Z); //V方向
			SectionPlane *plane = new SectionPlane(tmpOrigin, tmpNormal, tmpUDir, tmpVDir);

			inModel->AddSectionPlane(plane);
			pView->AddSectionPlaneId(plane->GetID());

		}

		//获取关联信息
		StkEntityTypeEnum PointingEnt, PointedEnt;
		nodeConnectorsData = pViewData->GetNodeConnector();
		int nodeConnectorNum = nodeConnectorsData.size();
		vector<Stk_NodeConnector*> nodeConnectors;
		for (int j = 0; j < nodeConnectorNum; j++) {

			Stk_NodeConnector *Connector = nodeConnectorsData.at(j);
			Connector->GetEntityType(PointingEnt, PointedEnt);
			if (/*PointingEnt == 10 &&*/PointedEnt == 900) { //类型由巩长义提供，还未确认完
															 //获取PMI的id
				int PMIId = Connector->GetLinkNode();
//				READER_LOGI("viewId:%d ref PMI_Id:%d",pView->getID(), PMIId);
				pView->AddPMIId(PMIId);
			}
		}

		outViewData.push_back(pView);
		(*outViewNum)++;
	}

	return TRUE;
}

void SvlReader::AddProtoDataToModel(Model* model, Stk_ProtoType *pProtoType,
		STK_BOOL upHasColor, const STK_RGBA32& upInsColor) {
	//use node way
	READER_LOGI("SvlReader::AddProtoDataToModel start");

	this->AddProtoType(model); //将model加入Model 原型 Map
	Stk_Node* topNode = pProtoType->GetTopNode();
	if(topNode!=NULL)
	{
		ShapeSet* shapeSet = NULL;
		//使用catia节点方式，对model赋值node数据，不使用直接删除
		if(this->UseCatia())
		{
			shapeSet = new ShapeSet();
			model->SetShapeSet(shapeSet);
		}

		//创建顶级body，用于存储点和lineset等数据
		Body* topBody = new Body();
		topBody->AddRef();
		GetStkNodeData(model,topBody,topNode,pProtoType->GetLODCount(),
				upHasColor,upInsColor,&m_StkPMIList,&m_StkViewList,shapeSet);

		model->AddBody(topBody);
		topBody->Release();
	}
	else
	{
		READER_LOGE("SvlReader::AddProtoDataToModel topNodd is NULL!!!");
	}

	//添加自定义属性
	if (this->m_usePrototypeProperty && pProtoType->GetAllMetaDatas().size() > 0) {
		ShapeProperty* shapeProperty = new ShapeProperty();
		shapeProperty->AddRef();
		FillShapeProperty(pProtoType->GetAllMetaDatas(), shapeProperty);
		model->SetShapeProperty(shapeProperty);
		shapeProperty->Release();
	}

	READER_LOGI("SvlReader::AddProtoDataToModel end");
}

void SvlReader::GetStkNodeData(Model* pModel,Body* pBody, Stk_Node* pNode,
int protoTypeLODCount,
		STK_BOOL upHasColor, const STK_RGBA32& upInsColor,vector<Stk_PMI*>* pStkPMIList,
		vector<Stk_View*>* pStkViewList,ShapeSet *pShapeSet)
{
	ShapeSet *pCurShapeSet = pShapeSet;

	if(pCurShapeSet)
	{
		string curNodeName("");
		if (!pNode->GetNodeName().empty())
		{
			curNodeName = Platform::WStringToString(pNode->GetNodeName());
		}

		pCurShapeSet->SetOrigVisible(pNode->GetDispStat());
		pCurShapeSet->SetName(curNodeName);
		pCurShapeSet->SetID((long) pNode->GetID());
	}

	switch (pNode->GetNodeType())
	{
	case NODE_TYPE_VIRTUAL:
	{
		if(pCurShapeSet)
		pCurShapeSet->SetType(ShapeSet::ShapeSetType::Undefine);
	}
		break;
	case NODE_TYPE_BODY:
	{
		Stk_Body *pCurBody = pNode->GetBody();
		if (pCurBody != NULL && pNode->GetDispStat())
		{
			STK_RGBA32 bodyColor;
			GetBodyColor(bodyColor, pCurBody, upHasColor, upInsColor); //得到Body的颜色
			STK_SIZE_T MeshNum = pCurBody->GetMeshComNum(); //得到Body下复合面的个数
			STK_SIZE_T linesNum = pCurBody->GetAllLineSets().size();
			STK_SIZE_T pntNum = pCurBody->GetSpacialPointNum();

			if (MeshNum > 0)
			{
				//一个MeshCom构建了一个Body与之对应
				for (int meshIndex = 0; meshIndex < MeshNum; meshIndex++)
				{
					/*svl 中的复合面对应SView中Body
					 * SView中不存在复合面的概念，*/
					Stk_Mesh_Com *pMeshCom = pCurBody->GetMeshCom(meshIndex);//		if (!pModel->GetShapeNode())
						if (pMeshCom != NULL)
						{
							if (!pModel->GetModelShape())
							{
								ModelShape* tShapeNode = new ModelShape();
								pModel->SetModelShape(tShapeNode);
							}

							Body* body = new Body();
							//将svl复合面数据加入M3D Body中
							AddProtoMeshComDataToBody(body, pMeshCom, protoTypeLODCount,
								upHasColor, bodyColor, upInsColor);
							pModel->AddBody(body);

							//设置显示隐藏状态
							body->SetOrigVisible(pNode->GetDispStat());

							//将shape对象关联到node节点上
							//TODO
							/*if (pCurShapeSet)
							{
								pCurShapeSet->AddShape(body);
							}*/
						}
				}
				if (pCurShapeSet)
					pCurShapeSet->SetType(ShapeSet::ShapeSetType::Plane);
			}

			if (linesNum > 0 && pCurShapeSet)
			{
				//将线集数据加入对象集中
				AddLineSetsToShapeCollection(pBody, pNode, pCurShapeSet);

				if (pCurShapeSet)
					pCurShapeSet->SetType(ShapeSet::ShapeSetType::Line);
			}

			if (pntNum > 0 && pCurShapeSet)
			{
				//点集（特殊点）
				AddPointSetsToShapeCollection(pBody, pNode, pCurShapeSet);

				if (pCurShapeSet)
					pCurShapeSet->SetType(ShapeSet::ShapeSetType::Point);
			}

		}
		else
		{
			READER_LOGE("body is NULL! node:%s", pNode->GetNodeName().c_str());
		}
	}
		break;
	case NODE_TYPE_MESH:{
		Stk_Mesh_Com *pMeshCom = pNode->GetMesh();
		if (pMeshCom != NULL) {
			if (!pModel->GetModelShape())
			{
				ModelShape* tShapeNode = new ModelShape();
				pModel->SetModelShape(tShapeNode);
				pModel->SetDrawDataPrepared(true);
			}

            Body* body = new Body();
            STK_RGBA32 bodyColor;
            STK_RGBA32 meshComColor = pMeshCom->GetColor();
            GetMeshComColor(meshComColor, pMeshCom, upHasColor, upInsColor);
            
            //将svl复合面数据加入M3D Body中
            AddProtoMeshComDataToBody(body, pMeshCom, protoTypeLODCount, upHasColor,
                                      bodyColor, upInsColor);
            //设置显示隐藏状态
            body->SetOrigVisible(pNode->GetDispStat());

            pModel->AddBody(body);

            //将shape对象关联到node节点上
			//TODO
          /*  if (pCurShapeSet)
            {
                pCurShapeSet->AddShape(body);
                body->SetOrigVisible(pCurShapeSet->IsOrigVisible());
            }*/
        }
				
	}
		break;
	case NODE_TYPE_PMI: //遇到pmi记录下来，放到后面处理。（暂时解决方案）
	{
		Stk_PMI *pPMI = pNode->GetPMI();
		if (pPMI != NULL)
		{
			if (pModel == this->m_M3DModel || pPMI->IsDisplay())
			{
				pStkPMIList->push_back(pPMI);
			}
		}
	}
		break;
	case NODE_TYPE_VIEW:
	{
		Stk_View *pView = pNode->GetView();
		//LOGI("SvlReader::GetStkNodeData view: name:%s type:%d",tmpName.c_str(),pView->GetUsageType());
		if (pView != NULL )//&& pView->GetUsageType() != VIEW_USAGE_UNKNOWN)
		{
			string tmpName = Platform::WStringToString(pView->GetName());
			pStkViewList->push_back(pView);
		}
	}
		break;
	case NODE_TYPE_CAMERA: //!< Camera型
		break;
	case NODE_TYPE_META: //!< 属性
	{
		if(pCurShapeSet)
		{
			Stk_MetaData* pStkMetaData = pNode->GetMetaData();
			if (pStkMetaData)
			{
//				string metaTitle("");
				string metaValue("");
//				string metaUnit("");
//				if(pStkMetaData->GetTitle().length()>0)
//				{
//					Platform::WStringToString(pStkMetaData->GetTitle(), metaTitle);
//				}

//				if(pStkMetaData->GetValue().length()>0)
//				{
//					Platform::WStringToString(pStkMetaData->GetValue(), metaValue);
//				}
//				if(pStkMetaData->GetUnits().length()>0)
//				{
//					Platform::WStringToString(pStkMetaData->GetUnits(), metaUnit);
//				}
//				char buff[100];
//				snprintf(buff, sizeof(buff), "MetaDataNode: title:'%s' value:'%s' unit:'%s'"
//						,metaTitle.c_str(),metaValue.c_str(),metaUnit.c_str());
				//std::string buffAsStdStr = buff;

				if (!pNode->GetNodeName().empty())
				{
					if(pStkMetaData->GetValue().length()>0)
					{
						string name = Platform::WStringToString(pNode->GetNodeName()+ L"="+pStkMetaData->GetValue());
						pCurShapeSet->SetName(name);
					}
				}
//				LOGI("%s",buff);
			}
		}

		break;
	}
	case NODE_TYPE_LAYERINFOSSET: //!< 图层信息集
		break;
	}

	//每个节点都遍历
	int childNodeNum = pNode->GetChildNodeNum();
	for (int iNode = 0; iNode < childNodeNum; iNode++)
	{
		STK_ID childNodePlcID;
		Stk_Node* pChildNode = pNode->GetChildNode(iNode, childNodePlcID);
		StkNodeTypeEnum type = pChildNode->GetNodeType();

		ShapeSet* shapeSet = NULL;

		if(this->UseCatia())
		{
			shapeSet = new ShapeSet();
		}

		GetStkNodeData(pModel, pBody, pChildNode, protoTypeLODCount, upHasColor,
				upInsColor, pStkPMIList, pStkViewList, shapeSet);

		if(pCurShapeSet && shapeSet)
		{
			if (shapeSet->GetChildren().size() > 0 || shapeSet->GetShapes().size() > 0
					|| pChildNode->GetNodeType() == NODE_TYPE_META || pChildNode->GetNodeType() ==NODE_TYPE_PMI)
			{
				pCurShapeSet->AddChild(shapeSet);
			}
//			{
//				pCurShapeSet->AddChild(shapeSet);
//			}
		}
	}

//	READER_LOGI("SvlReader::GetStkNodeData end name:%s.",curNodeName.c_str());
}

void SvlReader::SetUseCatia(bool useCatia)
{
	this->m_useCATIAMode = useCatia;
}

bool SvlReader::UseCatia()
{
	return this->m_useCATIAMode;
}

string SvlReader::GetSVLMaterialID(Stk_File* protoType, unsigned int renderID)
{
	string materialID  ;

	if(renderID>=0)
	{
		materialID = "SVLRender" +
				StringHelper::IntToString(protoType->GetFileID())
		+ "|"
		+
		StringHelper::IntToString(renderID);
	}

	return materialID;
}

void SvlReader::SVLRenderToMaterial(Material* material,Stk_Render* stkRenderP)
{
	if(stkRenderP && material)
	{
		Stk_Texture* stkTexture = stkRenderP->GetTexture();
		if(stkTexture)
		{
			ResourceManager* resourceMgr = this->m_view->GetSceneManager()->GetResourceManager();

			Stk_Picture*  stkPictureP = stkTexture->GetTextureImage();			//!< 图片
			if(stkPictureP)
			{
				//获取图片数据长度
				STK_UINT32 o_nudataSize;
				STK_CHAR* o_chDataP = NULL;
				stkPictureP->GetData(o_nudataSize,o_chDataP);

				if(o_nudataSize > 0 && o_chDataP)
				{
					//将数据赋值给图片对象
					string textName = material->GetName();

					Texture2D* texture2d  =(Texture2D*)resourceMgr->GetOrCreateTexture(textName,Texture::TEXTURE_2D);
					if (!texture2d->GetImage())
					{
						//构造image，按道理来说image也应该重用
						Image* image = new Image();
						image->SetData((unsigned char*)o_chDataP, o_nudataSize);
						AddRefMe(image);
						texture2d->SetImage(image);
						ReleaseMe(image);
					}
					material->SetDiffuseMap(texture2d);
				}

				float uScale = stkTexture->GetUScale();
				float vScale = stkTexture->GetVScale();
				//如果u v 不同时为1
				if(Abs(uScale-1)<0.001 && Abs(vScale-1.0f)<0.001 ||Abs(uScale)<0.001f || Abs(vScale)<0.001f )
				{
				}else
				{
					//构造变换纹理坐标变换矩阵
					material->CreateTexture2DTransform();
					Matrix4* matrix4 = material->GetTexture2DTransform();

					if(matrix4)
					{
						matrix4->m_m00 = stkTexture->GetUScale();
						matrix4->m_m11 =stkTexture->GetVScale();
					}
				}
			}
		}
	}
}

void SvlReader::ReadUserData(Stk_ProtoType *pTopProtoType)
{
	if(this->m_view)
	{
		if(SVIEW::Parameters::Instance()->m_isUseUserDataBom == true)
		{
			map<STK_ID,Stk_UserData*>* mapUserData = pTopProtoType->GetUserDataByName(L"VBOM");
				if(mapUserData != NULL && mapUserData->size() > 0)
				{
					map<STK_ID,Stk_UserData*>::iterator itor = mapUserData->begin();
					if (itor == mapUserData->end())
						return;
					wstring wstrDataName = L"";
					STK_ID id = 0;
					STK_CHAR* dataP = NULL;
					STK_SIZE_T dataSize = 0;
					wstring wstrFileName = L"";
					itor->second->GetUserData(wstrDataName,id,dataP,dataSize,wstrFileName);

					string key = "VBOM";
					string value = WStringToString(wstrFileName);
					this->m_view->AddUserData(key,value);
				}
            
		}
        map<STK_ID,Stk_UserData*>* mapServiceManualP= pTopProtoType->GetUserDataByName(L"ServiceManual");
        if(mapServiceManualP != NULL && mapServiceManualP->size() > 0)
        {
            map<STK_ID,Stk_UserData*>::iterator itMapServiceManual = mapServiceManualP->begin();
            wstring wstrDataName = L"";
            STK_ID id = 0;
            STK_CHAR* dataP = NULL;
            STK_SIZE_T dataSize = 0;
            wstring wstrFileName = L"";
            itMapServiceManual->second->GetUserData(wstrDataName,id,dataP,dataSize,wstrFileName);
            string key = "ServiceManual";
			#ifdef WIN32
			wstring wstrOut = Platform::UTF_8ToUnicode(dataP);
			string value = Platform::WStringToString(wstrOut, "auto");
			#else 
			string value = dataP;
			#endif
            this->m_view->AddUserData(key,value);
        }
        map<STK_ID,Stk_UserData*>* mapUserData1 = pTopProtoType->GetUserDataByName(L"HotSpot");
        if(mapUserData1 != NULL && mapUserData1->size() > 0)
        {
            map<STK_ID,Stk_UserData*>::iterator itor = mapUserData1->begin();
            if (itor == mapUserData1->end())
                return;
            wstring wstrDataName = L"";
            STK_ID id = 0;
            STK_CHAR* dataP = NULL;
            STK_SIZE_T dataSize = 0;
            wstring wstrFileName = L"";
            itor->second->GetUserData(wstrDataName,id,dataP,dataSize,wstrFileName);
            
            string key = "HotSpot";
			#ifdef WIN32
			wstring wstrOut = Platform::UTF_8ToUnicode(dataP);
            string value = Platform::WStringToString(wstrOut,"auto");
			#else 
			string value = dataP;
			#endif
            this->m_view->AddUserData(key,value);
        }
        map<STK_ID,Stk_UserData*>* mapDefaultViewData= pTopProtoType->GetUserDataByName(L"DefaultViewID");
        if(mapDefaultViewData != NULL && mapDefaultViewData->size() > 0)
        {
            map<STK_ID,Stk_UserData*>::iterator itor = mapDefaultViewData->begin();
            if (itor == mapDefaultViewData->end())
                return;
            wstring wstrDataName = L"";
            STK_ID id = 0;
            STK_CHAR* dataP = NULL;
            STK_SIZE_T dataSize = 0;
            wstring wstrFileName = L"";
            itor->second->GetUserData(wstrDataName,id,dataP,dataSize,wstrFileName);
            STK_ID defaultID = (STK_ID)*dataP;
            string key = "DefaultViewID";
            string valueStr = StringHelper::IntToString(defaultID);;
            this->m_view->AddUserData(key,valueStr);
        }
	}
}

void SvlReader::GetProtoBodyData(Body* body, Stk_Body *pCurBody,
		Stk_ProtoType *pProtoType, STK_BOOL upHasColor,
		const STK_RGBA32& upInsColor) {
	/*Model* model;
	 STK_RGBA32 bodyColor;
	 GetBodyColor(bodyColor, pCurBody, upHasColor, upInsColor);
	 Color c(bodyColor.Red, bodyColor.Green, bodyColor.Blue, bodyColor.Alpha);
	 body->SetColor(c);

	 STK_SIZE_T MeshNum = pCurBody->GetMeshComNum();
	 Stk_Mesh_Com *pMeshCom = NULL;
	 for (int i = 0; i < MeshNum; i++)
	 {
	 if (IsCancel())
	 return;
	 pMeshCom = pCurBody->GetMeshCom(i);
	 Body* meshBody = new Body();
	 model->AddBody(meshBody);
	 GetProtoMeshComData(meshBody, pMeshCom, pProtoType, upHasColor,
	 upInsColor);
	 }*/
}

void SvlReader::AddProtoMeshComDataToBody(Body* body, Stk_Mesh_Com *pMeshCom,
		int protoTypeLODCount, STK_BOOL upHasColor, const STK_RGBA32& bodyColor,
		const STK_RGBA32& upInsColor) {
	READER_LOGI("SvlReader::AddProtoMeshComDataToBody start");
	STK_RGBA32 meshComColor = bodyColor;
	GetMeshComColor(meshComColor, pMeshCom, upHasColor, upInsColor);

	Color c(meshComColor.Red, meshComColor.Green, meshComColor.Blue,
			1 - meshComColor.Alpha);
	body->SetInitColor(c);

	int lodCount = 1;

	//获取外部读取LOD配置
	if (this->m_IsUseLOD)
		lodCount = protoTypeLODCount;

	READER_LOGI("lodcount is %d", lodCount);
	bool bUniClr = IsMeshHasUniColor(pMeshCom, upHasColor, meshComColor);//Body下所有的面是否具有相同颜色
	//bool bUniClr = true; //高性能模式强制开启面合并
	//将ComMesh下所有的面的的点数据都填充到Body中
	
	VertexSet* bodymeshData = new VertexSet();
	bodymeshData->AddRef();
	
	FillBody(body, bodymeshData, pMeshCom, lodCount);
	READER_LOGI("FillBody end");

	if (bUniClr && false) //面合并模式
			{
		//将面合后的面添加到Body中，使用此函数只会得到一个最终的合并Face
		AddMergeFaceToBody(body, bodymeshData, pMeshCom, upHasColor, meshComColor, lodCount);
	} else //正常模式
	{
		AddFaceToBody(body,bodymeshData, pMeshCom, upHasColor, meshComColor, lodCount);
	}

	bodymeshData->Release();

	READER_LOGI("AddProtoMeshComDataToBody end");
}

bool SvlReader::IsMeshHasUniColor(Stk_Mesh_Com *pMeshCom, STK_BOOL upHasColor, const STK_RGBA32& meshcolor)
{
	bool uniColorFlag = false;
	STK_RGBA32 faceColor;
	STK_RGBA32 firstfaceColor;
	Stk_Mesh_Face* pMeshFace;
	int FaceNum = pMeshCom->GetMeshFaceNum();

	for (int i=0;i<FaceNum;i++)
	{
		pMeshFace = pMeshCom->GetMeshFace(i);
		if (i == 0) {
			if (upHasColor)
			{
				uniColorFlag = true;
				faceColor = meshcolor;
				break;
			}
			else
			{
				faceColor = pMeshFace->GetColor();
				firstfaceColor = pMeshFace->GetColor();
			}
			continue;
		}

		faceColor = pMeshFace->GetColor();

		if (Abs(faceColor.Red - firstfaceColor.Red)>0.01 || 
			Abs(faceColor.Green - firstfaceColor.Green)>0.01 ||
			Abs(faceColor.Blue - firstfaceColor.Blue)>0.01)
		{
			uniColorFlag = false;
			break;
		}
	}

	return uniColorFlag;
}

void SvlReader::AddFaceToBody(Body* body, VertexSet* bodyMesh, Stk_Mesh_Com *pMeshCom,
							  STK_BOOL upHasColor, const STK_RGBA32& meshcolor, int lodCount) {
	//READER_LOGE("SvlReader::GetProtoFaceData start");
	int FaceNum = pMeshCom->GetMeshFaceNum();
	Stk_Mesh_Face* pMeshFace = NULL;
	map<unsigned int, MeshLODInfo*> allMeshLOD = pMeshCom->GetAllMeshLOD();

	/*根据stk_Mesh_com下包含的stk_mesh_face，创建SView中的Face*/
	for (int faceIndex = 0; faceIndex < FaceNum; faceIndex++) {
		pMeshFace = pMeshCom->GetMeshFace(faceIndex);

		Face* pFace = new Face();
		body->AddFace(pFace);

		STK_RGBA32 faceColor;
		if (pMeshCom->IsUniColor()) {
			/*如果使用统一颜色，获取统一颜色的值*/
			GetMergeFaceColor(faceColor, pMeshCom, upHasColor, meshcolor);
		} else {
			/*不使用统一颜色*/
			GetFaceColor(faceColor, pMeshFace, upHasColor, meshcolor);
		}

		////读取材质，首先取face自己的，如果没有则取上级pMeshCom的。
		unsigned int renderId  = pMeshFace->GetRenderID();

		if(!this->GetMaterialById(renderId))
		{
			renderId = pMeshCom->GetRenderID();
		}

		Color c(faceColor.Red, faceColor.Green, faceColor.Blue,
			1 - faceColor.Alpha);

		if(renderId>0){
			pFace->SetMaterial(this->GetMaterialById(renderId));
			//Material* material = this->GetMaterialById(renderId);
			//if (material) {
			//	c = material->GetAmbient();
			//}
		}

		pFace->SetInitColor(c);

		int readLevel = 0;
		int readCount = lodCount > 1 ? 2 : 1;
		for (int lodLevel = 0; lodLevel < readCount; lodLevel++) {
			if (lodLevel == 0) {
				readLevel = m_readMaxLodLevel;
			} else if (lodLevel == 1) {
				readLevel =
						lodCount - 1 > m_readMinLodLevel ?
								m_readMinLodLevel : lodCount - 1;
			}
			/*添加指定级别的LOD数据到Face中*/
			AddLodDataToFace(pFace, pMeshFace, bodyMesh,pMeshCom, allMeshLOD, readLevel);
		}
	}
	//READER_LOGE("SvlReader::GetProtoFaceData end");
}
void SvlReader::FillBody(Body* body, VertexSet* bodymeshData, Stk_Mesh_Com *pMeshCom, int lodCount) {
	int readLevel = 0;
	int readCount = lodCount > 1 ? 2 : 1;
	for (int lodLevel = 0; lodLevel < readCount; lodLevel++) {
		if (lodLevel == 0) {
			readLevel = m_readMaxLodLevel;
		} else if (lodLevel == 1) {
			readLevel =
					lodCount - 1 > m_readMinLodLevel ?
							m_readMinLodLevel : lodCount - 1;
		}
		AddLodDataToBody(body, bodymeshData, pMeshCom, readLevel);
	}

	map<unsigned int, MeshLODInfo*> allMeshLOD = pMeshCom->GetAllMeshLOD();

	/*高性能模式下，不读取Body的边界线信息和几何属性信息*/
	if (this->m_readEdge) {
		if (pMeshCom->GetMeshEdgeType() == MESH_EDGE_STORE_IN_EDGE) {
			AddEdgeDataToBody(body, pMeshCom, allMeshLOD, this->TOPLODLEVEL);
		}
	}
}
void SvlReader::AddLodDataToBody(Body* body, VertexSet*  bodymeshData, Stk_Mesh_Com *pMeshCom,
		int readLevel) {
	//若果Mesh不存在realLevel的LOD数据，则返回
	MeshLODInfo* pMeshLODInfo = pMeshCom->GetMeshLODInfo(readLevel);
	if (pMeshLODInfo == NULL) {
//		READER_LOGI("body has no data lod Level %d", readLevel);
		return;
	}

	vector<STK_PNT32>* outVertices = &pMeshLODInfo->Vertices;

	//Body中存储的所有点的个数
	int totalVerNum = outVertices->size() / 2;
	if (CanUseIndex(totalVerNum)) //使用索引将所有的顶点数据直接加入BodyMeshData中
			{
		//bodymeshData = new VertexSet();
		bodymeshData->SetUseIndex(true);
		bodymeshData->AddRef();

		vector<Vector3>* positionArray = bodymeshData->GetPositionArray();
		vector<Vector3>* normalArray = bodymeshData->GetNormalArray();
		vector<Vector3>* textureCoordsArray = bodymeshData->GetTextureCoordArray();

		//如果添加两次可能会导致ID重复
//		bodymeshData->SetID(body->GetID());
		positionArray->reserve(totalVerNum);
		normalArray->reserve(totalVerNum);

		bool hasTextureCoord = false;
		//纹理坐标分配空间
		vector<STK_PNT32>* textureCoords = &pMeshLODInfo->UVs;
		if(textureCoords->size()>0)
		{
			textureCoordsArray->reserve(totalVerNum);
			hasTextureCoord = true;
		}

		for (int k = 0; k < totalVerNum; k++) {
			positionArray->push_back(
					Vector3((*outVertices)[k * 2].X, (*outVertices)[k * 2].Y,
							(*outVertices)[k * 2].Z));

			normalArray->push_back(
					Vector3((*outVertices)[k * 2 + 1].X,
							(*outVertices)[k * 2 + 1].Y,
							(*outVertices)[k * 2 + 1].Z));

			if(hasTextureCoord)
			{
				textureCoordsArray->push_back(
						Vector3((*textureCoords)[k].X,
								(*textureCoords)[k].Y,
								(*textureCoords)[k].Z));
			}
		}

		/*		if (bodymeshData->GetVertexCount() > 0) {
					int addLevel = ConvertToSViewUseLevel(readLevel);

					bodymeshData->AddRef();
				} */

	   bodymeshData->Release();

	} else // 不使用索引，仅创建BodyMeshData变量，供读取Face数据时填充
	{
		//bodymeshData = new VertexSet();
		bodymeshData->SetUseIndex(false);
		bodymeshData->AddRef();

		if (totalVerNum > 0) {
			//将复合面数据加入Body中
			int indexCout = this->GetComMeshAllFaceIndexCount(pMeshCom,
					readLevel);

			bodymeshData->GetPositionArray()->reserve(indexCout);
			bodymeshData->GetNormalArray()->reserve(indexCout);

			//纹理坐标分配空间
			vector<STK_PNT32>* textureCoords = &pMeshLODInfo->UVs;
			if(textureCoords->size()>0)
			{
				vector<Vector3>* textureCoordsArray = bodymeshData->GetTextureCoordArray();
				textureCoordsArray->reserve(totalVerNum);
			}

			//int addLevel = ConvertToSViewUseLevel(readLevel);
			//bodymeshData->AddRef();
		} 

		bodymeshData->Release();
	}

	//给Body中添加XPolyLine用于存储Body下和Body中Face下所有的Edge显示数据
	//仅仅读取精细度最高的LOD边界数据
	if (readLevel == TOPLODLEVEL) {
		SPolyLine* bodyPolyLine = new SPolyLine(); //构建一个XPolyLine用于存储点数据
		body->SetPolyLine(bodyPolyLine);
//		AddLineToClearManager(body->GetID(), bodyPolyLine, readLevel);
	}

}

bool SvlReader::FillEdges(Stk_Mesh_Edge* stk_edge,
		vector<STK_PNT32>* outVertices, SPolyLine* bodyPolyLine,
		RefPolyLine * edgeLine) {
	map<STK_UINT32, vcEdgeInfo*> edgelod = stk_edge->GetAllEdgeLODInfo();
	vcEdgeInfo* topEdgelodinfo = edgelod.at(this->TOPLODLEVEL);
	vector<STK_UINT32>& edge = *topEdgelodinfo;
	edgeLine->SetDataOffset(bodyPolyLine->GetPoints().size());
	unsigned int dataLength = edge.size();
	edgeLine->SetDataLength(2 * (dataLength - 1));

	if (dataLength >= 2) {
		//为了实现线合并的显示效果，按照下面这种方法存储边界线顶点
		//首先加入第一个点
		bodyPolyLine->AddPoints(
				Vector3((*outVertices)[edge[0] * 2].X,
						(*outVertices)[edge[0] * 2].Y,
						(*outVertices)[edge[0] * 2].Z));
		//纪录一下索引信息,由于SVL数据顶点和法线放到一起了， SVLX是分开放的，因此索引不用乘2
		bodyPolyLine->AddPointsIndex(edge[0]);

		//加入中间部分点
		for (int j = 1; j < dataLength - 1; j++) {
			Vector3 pnt((*outVertices)[edge[j] * 2].X,
					(*outVertices)[edge[j] * 2].Y,
					(*outVertices)[edge[j] * 2].Z);
			//中间的点，每个点要添加两次
			bodyPolyLine->AddPoints(pnt);
			bodyPolyLine->AddPoints(pnt);

			//纪录一下索引信息,由于SVL数据顶点和法线放到一起了， SVLX是分开放的，因此索引不用乘2
			bodyPolyLine->AddPointsIndex(edge[j]);
			bodyPolyLine->AddPointsIndex(edge[j]);
		}

		//在加入最后一个点
		bodyPolyLine->AddPoints(
				Vector3((*outVertices)[edge[dataLength - 1] * 2].X,
						(*outVertices)[edge[dataLength - 1] * 2].Y,
						(*outVertices)[edge[dataLength - 1] * 2].Z));
		//纪录一下索引信息,由于SVL数据顶点和法线放到一起了， SVLX是分开放的，因此索引不用乘2
		bodyPolyLine->AddPointsIndex(edge[dataLength - 1]);

		GetEdgeGeoAttribute(edgeLine, stk_edge);
		return true;
	} else {
		return false;
	}
}

bool SvlReader::FillLine(Stk_CurveEntity* curLine,RefPolyLine * edgeLine,SPolyLine* polyLine )
{
	bool ret = false;

	switch (curLine->GetCurveType())
	{
	case CURVE_TYPE_UNKNOWN:
		break;
	case CURVE_TYPE_POLYLINE:
	{
		Stk_PolyLine* curPolyLine = (Stk_PolyLine*) curLine;

		const vector<STK_PNT32>& pnts = curPolyLine->GetPoints();

		edgeLine->SetDataOffset(polyLine->GetPoints().size());
		unsigned int dataLength = pnts.size();
		edgeLine->SetDataLength(2 * (dataLength - 1));

		if (dataLength >= 2)
		{
			//为了实现线合并的显示效果，按照下面这种方法存储边界线顶点
			//首先加入第一个点
			polyLine->AddPoints(Vector3(pnts[0].X, pnts[0].Y, pnts[0].Z));
			//加入中间部分点
			for (int j = 1; j < dataLength - 1; j++)
			{
				Vector3 pnt(pnts[j].X, pnts[j].Y, pnts[j].Z);
				//中间的点，每个点要添加两次
				polyLine->AddPoints(pnt);
				polyLine->AddPoints(pnt);
			}
			//在加入最后一个点
			polyLine->AddPoints(
					Vector3(pnts[dataLength - 1].X, pnts[dataLength - 1].Y,
							pnts[dataLength - 1].Z));
			ret = true;
		}
		break;
	}
	case CURVE_TYPE_ELLIPSE:
		break;
	case CURVE_TYPE_LINE:
	{
		Stk_Line* stk_line = (Stk_Line*) curLine;

		STK_PNT32 startPnt = stk_line->GetStartPoint();
		STK_PNT32 endPnt = stk_line->GetEndPoint();

		edgeLine->SetDataOffset(polyLine->GetPoints().size());
		unsigned int dataLength = 2;
		edgeLine->SetDataLength(2 * (dataLength - 1));

		if (dataLength >= 2)
		{
			//为了实现线合并的显示效果，按照下面这种方法存储边界线顶点
			//首先加入第一个点
			polyLine->AddPoints(Vector3(startPnt.X,startPnt.Y,startPnt.Z));
			//加入中间部分点
			//在加入最后一个点
			polyLine->AddPoints(
					Vector3(endPnt.X,endPnt.Y,endPnt.Z));
			ret = true;
		}
		break;
	}
	case CURVE_TYPE_NURBSCURVE:
		break;
	case CURVE_TYPE_HYPERBOLA:
		break;
	case CURVE_TYPE_PARABOLA:
		break;
	}

	return ret;
}

int SvlReader::ConvertToSViewUseLevel(int svlLodLevel) {
	int sviewLODLevel = this->TOPLODLEVEL;
	if (svlLodLevel != this->TOPLODLEVEL) {
		sviewLODLevel = this->TOPLODLEVEL + 1;
	}
	return sviewLODLevel;
}

int SvlReader::GetComMeshAllFaceIndexCount(Stk_Mesh_Com *pMeshCom,
		int readLevel) {
	int FaceNum = pMeshCom->GetMeshFaceNum();
	int id = -1;
	//统计索引总个数
	int indexSize = 0;

	/*这个循环求得合并后面具有的点个数，进行空间的一次性分配，加快读取速度*/
	for (int faceIndex = 0; faceIndex < FaceNum; faceIndex++) {
		id = pMeshCom->GetMeshFace(0)->GetID();
		Stk_Mesh_Face* stk_face = pMeshCom->GetMeshFace(faceIndex);

		FaceLODInfo* stk_faceLodInfo = stk_face->GetFaceLODInfo(readLevel);

		if (stk_faceLodInfo == NULL) {
			continue;
		}
		indexSize += stk_faceLodInfo->TrianglesLOD.size();
	}

	return indexSize;
}
void SvlReader::AddMergeFaceToBody(Body* body, VertexSet* bodymeshData, Stk_Mesh_Com *pMeshCom,
		STK_BOOL upHasColor, const STK_RGBA32& meshcolor, int lodCount) {
	//READER_LOGE("SvlReader::GetProtoMergeFaceData start");

	STK_RGBA32 faceColor;
	GetMergeFaceColor(faceColor, pMeshCom, upHasColor, meshcolor);
	Color c(faceColor.Red, faceColor.Green, faceColor.Blue,
			1 - faceColor.Alpha);

	Face* pFace = new Face();
	pFace->SetInitColor(c);
	body->AddFace(pFace);

	int readLevel = 0;
	int readCount = lodCount > 1 ? 2 : 1;
	for (int lodLevel = 0; lodLevel < readCount; lodLevel++) {
		if (lodLevel == 0) {
			readLevel = m_readMaxLodLevel;
		} else if (lodLevel == 1) {
			readLevel =
					lodCount - 1 > m_readMinLodLevel ?
							m_readMinLodLevel : lodCount - 1;
		}
		AddMergeLodDataToFace(pFace,bodymeshData, pMeshCom, readLevel);
	}

	//READER_LOGE("SvlReader::GetProtoMergeFaceData end");
}

void SvlReader::AddEdgeDataToFace(Face* pFace, Stk_Mesh_Face* pMeshFace,
		MeshLODInfo* meshLodInfo, int readLodLevel) {
	if (readLodLevel != this->TOPLODLEVEL) {
		return;
	}

	map<STK_UINT32, FaceLODInfo*> allFaceLod = pMeshFace->GetAllFaceLOD();
	FaceLODInfo* faceinfo = allFaceLod[readLodLevel];

	MeshLODInfo* pMeshLODInfo = meshLodInfo;
	vector<STK_PNT32>* outVertices = &pMeshLODInfo->Vertices;

//	vector<vcPolylineIndices>& faceEdgesArray = faceinfo->EdgeLODIdx;

	SPolyLine* bodyPolyLine = pFace->GetBody()->GetPolyLine();

	vector<STK_ID> edgesID = pMeshFace->GetMeshEdgeIDs();

	for (int i = 0; i < edgesID.size(); i++) {
		Stk_Mesh_Edge* stk_edge = pMeshFace->GetMeshEdgeByID(edgesID[i]);
		if (stk_edge)
		{
			map<STK_UINT32, vcEdgeInfo*> edgelod = stk_edge->GetAllEdgeLODInfo();
			vcEdgeInfo* topEdgelodinfo = edgelod.at(this->TOPLODLEVEL);
			vector<STK_UINT32>& edge = *topEdgelodinfo;

			RefPolyLine * edgeLine = new RefPolyLine(bodyPolyLine);

			if (FillEdges(stk_edge, outVertices, bodyPolyLine, edgeLine)) {
				Edge* edge = new Edge();
				edge->AddData(edgeLine, readLodLevel);
				pFace->AddEdge(edge, readLodLevel);
			}
			else {
				edgeLine->Release();
			}
		}
	}
}

void SvlReader::AddEdgeDataToBody(Body* body, Stk_Mesh_Com *pMeshCom,
		map<unsigned int, MeshLODInfo*>& allMeshLOD, int readLodLevel) {
	if (readLodLevel != this->TOPLODLEVEL) {
		return;
	}

	MeshLODInfo* pMeshLODInfo = allMeshLOD[readLodLevel];

	vector<STK_PNT32>* outVertices = &pMeshLODInfo->Vertices;

	vector<STK_ID> edgesID = pMeshCom->GetMeshEdgeIDs();

	SPolyLine* bodyPolyLine = body->GetPolyLine();

	for (int i = 0; i < edgesID.size(); i++) {
		Stk_Mesh_Edge* stk_edge = pMeshCom->GetMeshEdgeByID(edgesID[i]);
		RefPolyLine * edgeLine = new RefPolyLine(bodyPolyLine);
		if (FillEdges(stk_edge, outVertices, bodyPolyLine, edgeLine)) {
			Edge* edge = new Edge();
			edge->AddData(edgeLine, readLodLevel);
			body->AddEdge(edge);
//			AddLineToClearManager(edge->GetID(), edgeLine, readLodLevel);
		} else {
//			delete edgeLine;
			edgeLine->Release();
		}
	}
}

void SvlReader::AddLineSetsToShapeCollection(Body *pBody,Stk_Node* pNode,ShapeSet* shapeSet)
{
	//线集
	vector<Stk_LineSet*>& allLineSet = pNode->GetBody()->GetAllLineSets();
	//不存在数据，返回
	if(allLineSet.size() == 0)
	{
		return;
	}

	//构建一个用来存储所有线数据的线对象，让其较小的线对象从中引用数据段。
	SPolyLine* bodyPolyLine = pBody->GetPolyLine();

	if(bodyPolyLine == NULL)
	{
		bodyPolyLine = new SPolyLine(); //构建一个XPolyLine用于存储点数据
		pBody->SetPolyLine(bodyPolyLine);
//		AddLineToClearManager(pBody->GetID(), bodyPolyLine, TOPLODLEVEL);
//		bodyPolyLine
	}

	//遍历所有的线集
	for (int lsIndex = 0; lsIndex < allLineSet.size(); lsIndex++)
	{
		Stk_LineSet* curLineSet = allLineSet.at(lsIndex);
		vector<Stk_CurveEntity*>& allLines = curLineSet->GetAllLines();
		//遍历当前线集中的线对象
		for (int lineIndex = 0; lineIndex < allLines.size(); lineIndex++)
		{
			Stk_CurveEntity* curLine = allLines.at(lineIndex);
			RefPolyLine * edgeLine = new RefPolyLine(bodyPolyLine);
			//是否存在显示数据
			if (FillLine(curLine, edgeLine, bodyPolyLine))
			{
				Edge* edge = new Edge();

				//设置线的显隐状态
				edge->SetOrigVisible(pNode->GetDispStat());

				STK_RGBA32 lineClr = curLine->GetColor();
				Color edgeLineColor(lineClr.Red, lineClr.Green, lineClr.Blue,
						1 - lineClr.Alpha);
				edge->SetColor(edgeLineColor);

				edge->AddData(edgeLine, TOPLODLEVEL);
				pBody->AddEdge(edge);
//				AddLineToClearManager(edge->GetID(), edgeLine, TOPLODLEVEL);

				//关联到节点中
				if (shapeSet){
					//TODO XL
					//shapeSet->AddShape(edge);
					//edge->SetOrigVisible(shapeSet->IsOrigVisible());
				}
			}
			else
			{
				//不存在显示数据，释放空间
				edgeLine->Release();
//				delete edgeLine;
			}
		}
	}
}

void SvlReader::AddPointSetsToShapeCollection(Body *pBody,Stk_Node* pNode,
		ShapeSet* shapeSet)
{
	Stk_Body* body = pNode->GetBody();
	int pntNum = body->GetSpacialPointNum();

	for(int i = 0;i < pntNum;i++)
	{
		Stk_SpacialPointEntity*	 pntEntity = body->GetSpacialPointByIndex(i);

		STK_PNT32 stk_coordinate = pntEntity->GetPntPosition();

		Vector3 coordinate(stk_coordinate.X,stk_coordinate.Y,stk_coordinate.Z);

		Point* point = new Point(coordinate);

		pBody->AddPoint(point);

		//其上级节点的显隐状态决定了，自身的显隐状态
		point->SetOrigVisible(pNode->GetDispStat());

		STK_RGBA32 stk_PntClr = pntEntity->GetColor();
		Color pntColor(stk_PntClr.Red, stk_PntClr.Green, stk_PntClr.Blue,
				1 - stk_PntClr.Alpha);
		point->SetColor(pntColor);

		//关联到节点中
		if (shapeSet)
		{
			shapeSet->AddShape(point);
			point->SetOrigVisible(shapeSet->IsOrigVisible());
		}
	}
}

void SvlReader::GetTriMeshGeoAttribute(IVertexSet* pTriMeshData,
		Stk_Mesh_Face* pMeshFace, int readLodLevel) {
	if (readLodLevel != this->TOPLODLEVEL) {
		return;
	}
	Stk_GeometryAttribute* geoAttribute = pMeshFace->GetGeoAttr();

	if (geoAttribute != NULL) {
		GeometryAttribute* triMeshDataGeoAttribute =
				GetGeometryAttributeFromStk(geoAttribute);
		pTriMeshData->SetGeoAttribute(triMeshDataGeoAttribute);
	}
}

GeometryAttribute* SvlReader::GetGeometryAttributeFromStk(
		Stk_GeometryAttribute* stk_geo) {
	if (stk_geo == NULL) {
		return NULL;
	}
	GeometryAttribute* geoAttribute = NULL;

	StkGeoAttrTypeEnum geoType = stk_geo->GetGeoAttrType();

	switch (geoType) {
	case GEOATTR_TYPE_UNKNOWN:
		break;
	case GEOATTR_TYPE_PLANEFACE: /*!< 平面							*/
		geoAttribute = GetPlanfaceGeoFromStk(stk_geo);
		break;
	case GEOATTR_TYPE_REVOLUTIONFACE:/*!< 旋转面							*/
		geoAttribute = GetRevolutionfaceGeoFromStk(stk_geo);
		break;
	case GEOATTR_TYPE_CYLINDERFACE:/*!< 圆柱面							*/
		geoAttribute = GetCylinderfaceGeoFromStk(stk_geo);
		break;
	case GEOATTR_TYPE_CONICALFACE:/*!< 圆锥面							*/
		geoAttribute = GetConicalfaceGeoFromStk(stk_geo);
		break;
	case GEOATTR_TYPE_SPHEREFACE:/*!< 球面							*/
		geoAttribute = GetSpherefaceGeoFromStk(stk_geo);
		break;
	case GEOATTR_TYPE_TOROIDALFACE:/*!< 轮胎面							*/
		geoAttribute = GetToroidaifaceFromStk(stk_geo);
		break;
	case GEOATTR_TYPE_LINE: //!< 直线
		geoAttribute = GetLineGeoFromStk(stk_geo);
		break;
	case GEOATTR_TYPE_ELLIPSE: //!< 椭圆弧
		geoAttribute = GetEllipseGeoFromStk(stk_geo);
		break;

	}
	return geoAttribute;
}
GeometryAttribute* SvlReader::GetPlanfaceGeoFromStk(
		Stk_GeometryAttribute* stk_geo) {
	if (stk_geo->GetGeoAttrType() != GEOATTR_TYPE_PLANEFACE) {
		return NULL;
	}

	Stk_PlaneFaceAttribute* stk_geoPlan = (Stk_PlaneFaceAttribute*) (stk_geo);
	PlaneFaceAttribute* geoAttribute = new PlaneFaceAttribute();
	STK_DIR32 dir = stk_geoPlan->GetNormal();
	geoAttribute->SetNormal(Vector3(dir.X, dir.Y, dir.Z));
	STK_PNT32 pnt = stk_geoPlan->GetOrigin();
	geoAttribute->SetOrigin(Vector3(pnt.X, pnt.Y, pnt.Z));

	return geoAttribute;
}
GeometryAttribute* SvlReader::GetRevolutionfaceGeoFromStk(
		Stk_GeometryAttribute* stk_geo) {
	if (stk_geo->GetGeoAttrType() != GEOATTR_TYPE_REVOLUTIONFACE) {
		return NULL;
	}

	Stk_RevolutionFaceAttribute* stk_geoRevo =
			(Stk_RevolutionFaceAttribute*) (stk_geo);
	RevolutionFaceAttribute* geoAttribute = new RevolutionFaceAttribute();
	geoAttribute->SetRadius(stk_geoRevo->GetRadius());
	STK_DIR32 axis = stk_geoRevo->GetRevoAxis();
	geoAttribute->SetRevoAxis(Vector3(axis.X, axis.Y, axis.Z));
	STK_PNT32 axisorig = stk_geoRevo->GetAxisOrigin();
	geoAttribute->SetAxisOrigin(Vector3(axisorig.X, axisorig.Y, axisorig.Z));

	return geoAttribute;
}

GeometryAttribute* SvlReader::GetCylinderfaceGeoFromStk(
		Stk_GeometryAttribute* stk_geo) {
	if (stk_geo->GetGeoAttrType() != GEOATTR_TYPE_CYLINDERFACE) {
		return NULL;
	}
	Stk_CylinderFaceAttribute* stk_geoCyline =
			(Stk_CylinderFaceAttribute*) (stk_geo);
	CylinderFaceAttribute* geoAttribute = new CylinderFaceAttribute();

	return geoAttribute;
}
GeometryAttribute* SvlReader::GetConicalfaceGeoFromStk(
		Stk_GeometryAttribute* stk_geo) {
	if (stk_geo->GetGeoAttrType() != GEOATTR_TYPE_CONICALFACE) {
		return NULL;
	}
	Stk_ConicalFaceAttribute* stk_geoConical =
			(Stk_ConicalFaceAttribute*) (stk_geo);
	ConicalFaceAttribute* geoAttribute = new ConicalFaceAttribute();

	return geoAttribute;
}
GeometryAttribute* SvlReader::GetSpherefaceGeoFromStk(
		Stk_GeometryAttribute* stk_geo) {
	if (stk_geo->GetGeoAttrType() != GEOATTR_TYPE_SPHEREFACE) {
		return NULL;
	}
	Stk_SphereFaceAttribute* stk_geoSphere =
			(Stk_SphereFaceAttribute*) (stk_geo);
	SphereFaceAttribute* geoAttribute = new SphereFaceAttribute();

	return geoAttribute;
}
GeometryAttribute* SvlReader::GetToroidaifaceFromStk(
		Stk_GeometryAttribute* stk_geo) {
	if (stk_geo->GetGeoAttrType() != GEOATTR_TYPE_TOROIDALFACE) {
		return NULL;
	}
	Stk_ToroidalFaceAttribute* stk_geoToroidail =
			(Stk_ToroidalFaceAttribute*) (stk_geo);
	ToroidalFaceAttribute* geoAttribute = new ToroidalFaceAttribute();
	geoAttribute->SetMajorRaius(stk_geoToroidail->GetMajorRadius());
	geoAttribute->SetMinorRadius(stk_geoToroidail->GetMinorRadius());

	geoAttribute->SetRadius(stk_geoToroidail->GetRadius());
	STK_DIR32 axis = stk_geoToroidail->GetRevoAxis();
	geoAttribute->SetRevoAxis(Vector3(axis.X, axis.Y, axis.Z));
	STK_PNT32 axisorig = stk_geoToroidail->GetAxisOrigin();
	geoAttribute->SetAxisOrigin(Vector3(axisorig.X, axisorig.Y, axisorig.Z));

	return geoAttribute;
}
GeometryAttribute* SvlReader::GetLineGeoFromStk(
		Stk_GeometryAttribute* stk_geo) {
	if (stk_geo->GetGeoAttrType() != GEOATTR_TYPE_LINE) {
		return NULL;
	}
	Stk_LineAttribute* stk_geoLine = (Stk_LineAttribute*) (stk_geo);
	LineAttribute* geoAttribute = new LineAttribute();

	STK_PNT32 centerPointPnt = stk_geoLine->GetCenterPoint();
	STK_DIR32 dirDirection = stk_geoLine->GetDirection();
	STK_PNT32 pntStartPnt = stk_geoLine->GetStartPoint();
	STK_PNT32 pntEndPnt = stk_geoLine->GetEndPoint();

	geoAttribute->SetCenterPoint(
			Vector3(centerPointPnt.X, centerPointPnt.Y, centerPointPnt.Z));
	geoAttribute->SetDirection(
			Vector3(dirDirection.X, dirDirection.Y, dirDirection.Z));
	geoAttribute->SetStartPoint(
			Vector3(pntStartPnt.X, pntStartPnt.Y, pntStartPnt.Z));
	geoAttribute->SetEndPoint(Vector3(pntEndPnt.X, pntEndPnt.Y, pntEndPnt.Z));
	geoAttribute->SetLength(stk_geoLine->GetLength());

	return geoAttribute;
}
GeometryAttribute* SvlReader::GetEllipseGeoFromStk(
		Stk_GeometryAttribute* stk_geo) {
	if (stk_geo->GetGeoAttrType() != GEOATTR_TYPE_ELLIPSE) {
		return NULL;
	}
	Stk_EllipseAttribute* stk_geoEllipse = (Stk_EllipseAttribute*) (stk_geo);
	EllipseAttribute* geoAttribute = new EllipseAttribute();

	STK_PNT32 pntCenterPoint = stk_geoEllipse->GetCenterPoint(); //!< Ellipse的中心点
	float fMajorRadius = stk_geoEllipse->GetMajorRadius(); //!< Ellipse的长轴半径
	float fMinorRadius = stk_geoEllipse->GetMinorRadius(); //!< Ellipse的短轴半径
	STK_PNT32 pntStartPoint = stk_geoEllipse->GetStartPoint(); //!< 边界线起点
	STK_PNT32 pntEndPoint = stk_geoEllipse->GetEndPoint(); //!< 边界线终点
	STK_DIR32 dirX; //!< X方向向量
	STK_DIR32 dirY; //!< Y方向向量
	STK_DIR32 dirZ; //!< Z方向向量

	stk_geoEllipse->GetXYZDir(dirX, dirY, dirZ);
	geoAttribute->SetCenterPoint(
			Vector3(pntCenterPoint.X, pntCenterPoint.Y, pntCenterPoint.Z));

	geoAttribute->SetMajorRadius(fMajorRadius);
	geoAttribute->SetMinorRadius(fMinorRadius);

	geoAttribute->SetStartPoint(
			Vector3(pntStartPoint.X, pntStartPoint.Y, pntStartPoint.Z));
	geoAttribute->SetEndPoint(
			Vector3(pntEndPoint.X, pntEndPoint.Y, pntEndPoint.Z));
	geoAttribute->SetXYZDir(Vector3(dirX.X, dirX.Y, dirX.Z),
			Vector3(dirY.X, dirY.Y, dirY.Z), Vector3(dirZ.X, dirZ.Y, dirZ.Z));

	return geoAttribute;
}

void SvlReader::GetEdgeGeoAttribute(Curve* curve, Stk_Mesh_Edge* stk_edge) {

	Stk_GeometryAttribute* geoAttribute = stk_edge->GetGeoAttr();

	if (geoAttribute != NULL) {
		GeometryAttribute* triMeshDataGeoAttribute =
				GetGeometryAttributeFromStk(geoAttribute);
		curve->SetGeoAttribute(triMeshDataGeoAttribute);
	}
}

void SvlReader::GetFaceColor(STK_RGBA32& faceColor, Stk_Mesh_Face* pMeshFace,
		STK_BOOL upHasColor, const STK_RGBA32& meshcolor) {

	// 构成面颜色
	if (!upHasColor && pMeshFace->HasColor()) {
		faceColor = pMeshFace->GetColor();
		if (faceColor.Alpha < 0)
			faceColor.Alpha = 0.0f;
	}
	else
	{
		faceColor = meshcolor;
	}

	if (faceColor.Red < 0 |faceColor.Green<0|faceColor.Blue<0) {
		faceColor.Red = 0.8f;
		faceColor.Green = 0.8f;
		faceColor.Blue = 0.8f;
		faceColor.Alpha = 0.0f;
	}
	//READER_LOGE("GetFaceColor is %f %f %f %f",
	//faceColor.Red,faceColor.Green,faceColor.Blue,faceColor.Alpha);

}

void SvlReader::AddLodDataToFace(Face* pFace, Stk_Mesh_Face* pMeshFace, VertexSet* bodyMesh,
		Stk_Mesh_Com *pMeshCom, map<unsigned int, MeshLODInfo*>& allMeshLOD,
		int readLodLevel) {
	/*判断是否有此级别的MeshLOD数据*/
	if (allMeshLOD.find(readLodLevel) == allMeshLOD.end()) {
		return;
	}

	/*判断是否有此级别的faceLod数据*/
	FaceLODInfo* pFaceLODInfo = pMeshFace->GetFaceLODInfo(readLodLevel);

	if (pFaceLODInfo == NULL) {
		return;
	}

	MeshLODInfo* pMeshLODInfo = allMeshLOD[readLodLevel];

//	READER_LOGE("realLodLevel %d body lodCount %d",readLodLevel,pFace->GetBody()->GetLodCount());

	//获取上级Body的存储数据
	if (bodyMesh) {
		Mesh* facemeshData = new Mesh(bodyMesh);

		vector<STK_UINT32> *outPolyIndeies = &pFaceLODInfo->TrianglesLOD;

		//	READER_LOGI("outPolyIndeies size: %d",outPolyIndeies->size());
		bool useIndex = bodyMesh->IsUseIndex();

		/*存在数据*/
		if (outPolyIndeies->size() > 0) {
			if (useIndex) {
				/*使用索引，将索引数据填充到上级Body的MeshData中*/
				AddIndexDataToMesh(facemeshData, outPolyIndeies, readLodLevel);
			}
			else {
				vector<STK_PNT32>* outVertices = &pMeshLODInfo->Vertices;
				vector<STK_PNT32>* outTexturecoords = &pMeshLODInfo->UVs;
				/*不使用索引，将坐标位置和向量存入到上级Body的MeshData中*/
				AddVertexDataToMesh(facemeshData, outPolyIndeies, outVertices, outTexturecoords);
			}

			if (facemeshData->GetVertexCount() > 0) {
				int addLevel = ConvertToSViewUseLevel(readLodLevel);
				//AddMeshToClearManager(addLevel, facemeshData);
				//TODO
				//pFace->AddData(facemeshData, readLodLevel);

				/*高性能模型下，不读取边界和几何属性信息*/
				GetTriMeshGeoAttribute(facemeshData, pMeshFace, readLodLevel);

				//如果边界数据存储在Face中
				if (pMeshCom->GetMeshEdgeType() == MESH_EDGE_STORE_IN_FACE) {
						/*添加边界线到Face中*/
						AddEdgeDataToFace(pFace, pMeshFace, pMeshLODInfo,
							readLodLevel);
				}
				pFace->SetData(facemeshData);
			}
		}
		else {
			facemeshData->Release();
		}
	}
}
void SvlReader::GetMergeFaceColor(STK_RGBA32& faceColor, Stk_Mesh_Com *pMeshCom,
		STK_BOOL upHasColor, const STK_RGBA32& meshcolor) {
	Stk_Mesh_Face* pMeshFace;
	int FaceNum = pMeshCom->GetMeshFaceNum();
	// 构成面颜色
	if (FaceNum > 0) {
		// 构成面颜色
		//颜色统一时，只保存第一个构成面的颜色
		pMeshFace = pMeshCom->GetMeshFace(0);
		if (!upHasColor && pMeshFace->HasColor()) {
			faceColor = pMeshFace->GetColor();
			if (faceColor.Alpha < 0)
				faceColor.Alpha = 0.0f;
		}
		else
		{
			faceColor = meshcolor;
		}
	}

	if (faceColor.Red < 0 | faceColor.Green < 0 | faceColor.Blue < 0) {
		faceColor.Red = 0.8f;
		faceColor.Green = 0.8f;
		faceColor.Blue = 0.8f;
		faceColor.Alpha = 0.0f;
	}
}

void SvlReader::AddMergeLodDataToFace(Face* pFace, VertexSet* bodymeshData, Stk_Mesh_Com *pMeshCom,
		int readLevel) {
//	READER_LOGE("SvlReader::GetMergeFaceLodData step1");
	MeshLODInfo* pMeshLODInfo = pMeshCom->GetMeshLODInfo(readLevel);

	if (pMeshLODInfo == NULL) {
		return;
	}

//	int indexSize = this->GetComMeshAllFaceIndexCount(pMeshCom, readLevel);
//	int totalVerNum = 0;

	//获取上级Body的存储数据
	//TODO
	VertexSet* bodyMesh = bodymeshData;
	//VertexSet* bodyMesh = (VertexSet*)((Body*)(pFace->GetBody()))->GetData(
	//ConvertToSViewUseLevel(readLevel));

	//如果Body中没有显示数据则不进行Face数据的遍历和添加
	if (!bodyMesh) {
		return;
	}

	Mesh* faceMeshData = new Mesh(bodyMesh);
	faceMeshData->AddRef();
	/*判断是否可以使用所以*/
	bool useIndex = bodyMesh->IsUseIndex();

	//faceMeshData->SetID(pFace->GetID());

	bool firstSetValue = true;

	int FaceNum = pMeshCom->GetMeshFaceNum();

	for (int faceIndex = 0; faceIndex < FaceNum; faceIndex++) {
		//	READER_LOGE("SvlReader::GetMergeFaceLodData step3---1");
		Stk_Mesh_Face* pMeshFace = pMeshCom->GetMeshFace(faceIndex);
		FaceLODInfo* pFaceLODInfo = pMeshFace->GetFaceLODInfo(readLevel);
		if (pFaceLODInfo == NULL) //此Face没有LOD数据，跳过
		{
			continue;
		}

		vector<STK_UINT32> *outPolyIndeies = &pFaceLODInfo->TrianglesLOD;
		if (outPolyIndeies->size() > 0) {
			if (useIndex) {
				/*使用索引，给Face的Mesh数据添加其上级Body Mesh数据的引用*/
				int faceIndexNum = outPolyIndeies->size();
				VertexSet* bodyMeshData = faceMeshData->GetRefMesh();
				vector<M3D_INDEX_TYPE>* bodyIndexArray =
						bodyMeshData->GetIndexArray();
				unsigned int offset = bodyIndexArray->size(); //通常情况下使用Mesh肯定包含坐标位置，所以使用顶点位置确定偏移量没问题

				//第一次时，设置偏移位置
				if (firstSetValue) {
					firstSetValue = false;
					faceMeshData->SetDataOffset(0);
				}
				//后面仅仅增加数据的长度
				faceMeshData->SetDataLength(offset + faceIndexNum);

				///预留空间，加快数据存入速度
				//bodyIndexArray->reserve(bodyIndexArray->size() + faceIndexNum);

				for (int k = 0; k < faceIndexNum; k++) {
					bodyIndexArray->push_back((*outPolyIndeies)[k]);
				}
			} else {
				int totalTrgnum = 0;
				unsigned int i0;
				unsigned int i1;
				unsigned int i2;

				totalTrgnum = outPolyIndeies->size() / 3;

				VertexSet* bodyMeshData = faceMeshData->GetRefMesh();

				vector<Vector3>* bodyPosArray =
						bodyMeshData->GetPositionArray();
				vector<Vector3>* bodyNormalArray =
						bodyMeshData->GetNormalArray();

				unsigned int offset = bodyPosArray->size(); //通常情况下使用Mesh肯定包含坐标位置，所以使用顶点位置确定偏移量没问题

				if (firstSetValue) {
					firstSetValue = false;
					faceMeshData->SetDataOffset(0);
				}

				faceMeshData->SetDataLength(offset + totalTrgnum * 3);

				vector<STK_PNT32>* outVertices = &pMeshLODInfo->Vertices;
				for (int k = 0; k < totalTrgnum; k++) {
					i0 = (*outPolyIndeies)[k * 3];
					i1 = (*outPolyIndeies)[k * 3 + 1];
					i2 = (*outPolyIndeies)[k * 3 + 2];

					Vector3 pnt0((*outVertices)[i0 * 2].X,
							(*outVertices)[i0 * 2].Y, (*outVertices)[i0 * 2].Z);
					Vector3 pnt1((*outVertices)[i1 * 2].X,
							(*outVertices)[i1 * 2].Y, (*outVertices)[i1 * 2].Z);
					Vector3 pnt2((*outVertices)[i2 * 2].X,
							(*outVertices)[i2 * 2].Y, (*outVertices)[i2 * 2].Z);

					bodyPosArray->push_back(pnt0);
					bodyPosArray->push_back(pnt1);
					bodyPosArray->push_back(pnt2);

					Vector3 Normal0((*outVertices)[i0 * 2 + 1].X,
							(*outVertices)[i0 * 2 + 1].Y,
							(*outVertices)[i0 * 2 + 1].Z);
					Vector3 Normal1((*outVertices)[i1 * 2 + 1].X,
							(*outVertices)[i1 * 2 + 1].Y,
							(*outVertices)[i1 * 2 + 1].Z);
					Vector3 Normal2((*outVertices)[i2 * 2 + 1].X,
							(*outVertices)[i2 * 2 + 1].Y,
							(*outVertices)[i2 * 2 + 1].Z);

					bodyNormalArray->push_back(Normal0);
					bodyNormalArray->push_back(Normal1);
					bodyNormalArray->push_back(Normal2);
				}
			}
		}
	}

	if (faceMeshData->GetVertexCount() > 0) {
		/*如果faceMeshData有数据，则将其加入到Face中*/
		int addLevel = ConvertToSViewUseLevel(readLevel);
//		AddMeshToClearManager(addLevel, faceMeshData);
		//TODO
		pFace->SetData(faceMeshData);
		//pFace->AddData(faceMeshData, readLevel);
	} 

		/*没有数据，则删除faceMeshData*/
//		faceMeshData->Clear();
//		delete faceMeshData;
		faceMeshData->Release();
//	READER_LOGE("SvlReader::GetMergeFaceLodData end");
}

void SvlReader::GetBodyColor(STK_RGBA32& bodycolor, Stk_Body *pCurBody,
		STK_BOOL upHasColor, const STK_RGBA32& upInsColor) {
	bodycolor.Red = 0.5f;
	bodycolor.Green = 0.5f;
	bodycolor.Blue = 0.5f;
	bodycolor.Alpha = 0.0f;

	if (upHasColor) {
		bodycolor = upInsColor;
	} else if (pCurBody->HasColor()) {
		bodycolor = pCurBody->GetColor();
	}
	//READER_LOGI("SvlReader::GetBodyColor:%X",bodycolor);
}

void SvlReader::GetMeshComColor(STK_RGBA32& meshComColor,
		Stk_Mesh_Com *pMeshCom, STK_BOOL upHasColor,
		const STK_RGBA32& upInsColor) {
	// 复合图形颜色
	if (upHasColor) {
		meshComColor = upInsColor;
	} else if (pMeshCom->HasColor()) {
		meshComColor = pMeshCom->GetColor();
	}
}


BaseMaterial*  SvlReader::GetMaterialById(unsigned int renderId)
{
	BaseMaterial* material = NULL;

	map<int,BaseMaterial*>::iterator it = m_protoTypeMaterialCache.find(renderId);

	if(it != m_protoTypeMaterialCache.end())
	{
		material = it->second;
	}else
	{
		READER_LOGE("GetMaterialById error: material not found!");
	}

	return material;
}

void SvlReader::AddFileMaterialToResourceMgr(Stk_File *pStkFile)
{
	if(pStkFile)
	{
		int renderNum = pStkFile->GetRenderNum();
		READER_LOGI("renderNum %d",renderNum);
		//每个proto建立一个缓存
		m_protoTypeMaterialCache.clear();
		if(renderNum>0)
		{
			for(int i=0;i<renderNum;i++)
			{
				Stk_Render* stkRenderP = pStkFile->GetRenderByIndex(i);
				if(stkRenderP)
				{
					unsigned int renderId = stkRenderP->GetID();
					string materialID = GetSVLMaterialID(pStkFile,stkRenderP->GetID());

					if(materialID.length()>0)
					{
						//将材质加入全局资源管理器中，进行资源管理
						Material* material = (Material*)m_view->GetSceneManager()->GetResourceManager()->GetOrCreateMaterial(materialID);
						if(material)
						{
							SVLRenderToMaterial(material,stkRenderP);
							//将material加入缓存列表中 便于快速查找
							m_protoTypeMaterialCache.insert(std::pair<int,BaseMaterial*>(renderId,material));
							//material->AddRef();
						}
					}

				}
			}
		}
	}
}

bool SvlReader::IsExistProtoModel(Model* model, Stk_ProtoType *pProtoType) {
	Model* pModel = FindModelFromProtoTypeID(pProtoType->GetID());
	if (pModel != NULL) {
		return true;
	}

	return false;
}

void SvlReader::CopyModelDrawData(Model* model, Model* orig) {
//	model->SetProtoModel(orig);
	if (orig->GetBodys())
	{
		if (!model->GetModelShape())
		{
			ModelShape* tShapeNode = new ModelShape();
			model->SetModelShape(tShapeNode);
		}
		vector<Body *>::iterator ite1;
		for (ite1 = orig->GetBodys()->begin(); ite1 != orig->GetBodys()->end();
			ite1++) {
			Body* body = new Body(*((Body*)*ite1));
			model->AddBody(body);
		}
		//如果使用catia 则进行node节点信息的copy
		if (this->UseCatia())
		{
			ShapeSet* node = new ShapeSet();
			*node = *orig->GetShapeCollection();
			model->SetShapeSet(node);
			model->UpdateContainsShapesId();
			model->GetShapeCollection()->UpdataRelevateShape(model);
		}
		model->SetOrigVisible(orig->IsOrigVisible());
	}

	if(orig->GetShapeProperty() != NULL)
	{
		if(this->m_usePrototypeProperty)
		{
			model->SetShapeProperty(orig->GetShapeProperty());
		}
		//else if(this->m_useInstanceProperty)
		//{
		//	ShapeProperty* origshapeProperty = orig->GetShapeProperty();

		//	ShapeProperty* shapeProperty = new ShapeProperty();
		//	shapeProperty->SetProperties(origshapeProperty->GetProperties());
		//	model->SetShapeProperty(shapeProperty);
		//	shapeProperty->AddRef();
		//	shapeProperty->Release();
		//}
	}
}

bool SvlReader::IsUseHighPerformance() {
	return true;
}

void SvlReader::SetHighPerformance(bool highPerformance) {
	this->isHighPerformance = highPerformance;
}

void SvlReader::LoadReadConfig() {
	SetHighPerformance(Parameters::Instance()->m_IsHighPerformanceView);
}

void SvlReader::AddIndexDataToMesh(Mesh* comMeshTriData,
		vector<STK_UINT32> *outPolyIndeies, int readLodLevel) {
	Mesh* faceMeshData = comMeshTriData;
	if (!faceMeshData) {
		return;
	}

	int faceIndexNum = outPolyIndeies->size();

	VertexSet* bodyMeshData = faceMeshData->GetRefMesh();
	///设置使用索引标志
	bodyMeshData->SetUseIndex(true);

	vector<M3D_INDEX_TYPE>* bodyIndexArray = bodyMeshData->GetIndexArray();
	M3D_OFFSET_TYPE offset = bodyIndexArray->size(); //通常情况下使用Mesh肯定包含坐标位置，所以使用顶点位置确定偏移量没问题
	faceMeshData->SetDataOffset(offset);
	faceMeshData->SetDataLength(faceIndexNum);

	///预留空间，加快数据存入速度
	//bodyIndexArray->reserve(bodyIndexArray->size() + faceIndexNum);
	for (int k = 0; k < faceIndexNum; k++) {
		bodyIndexArray->push_back((*outPolyIndeies)[k]);
	}
}

void SvlReader::AddVertexToFace(VertexSet* comMeshTriData,
		vector<STK_UINT32> *outPolyIndeies, vector<STK_PNT32>* outVertices,
		int readLodLevel) {

}

void SvlReader::AddVertexDataToMesh(Mesh* comMeshTriData,
		vector<STK_UINT32> *outPolyIndeies, vector<STK_PNT32>* outVertices,
		vector<STK_PNT32>* outTextureCoords) {
	Mesh* faceMeshData = comMeshTriData;
	if (!faceMeshData) {
		return;
	}
	int totalTrinum = 0;
	unsigned int i0;
	unsigned int i1;
	unsigned int i2;

	totalTrinum = outPolyIndeies->size() / 3;

	VertexSet* bodyMeshData = faceMeshData->GetRefMesh();

	bodyMeshData->SetUseIndex(false);

	vector<Vector3>* bodyPosArray = bodyMeshData->GetPositionArray();
	vector<Vector3>* bodyNormalArray = bodyMeshData->GetNormalArray();
	vector<Vector3>* textureCoordsArray = bodyMeshData->GetTextureCoordArray();

	unsigned int offset = bodyPosArray->size(); //通常情况下使用Mesh肯定包含坐标位置，所以使用顶点位置确定偏移量没问题
	faceMeshData->SetDataOffset(offset);
	faceMeshData->SetDataLength(totalTrinum * 3);

	bool hasTextureCoord = false;
	//纹理坐标分配空间
	vector<STK_PNT32>* textureCoords = outTextureCoords;
	if(textureCoords->size()>0)
	{
		hasTextureCoord = true;
	}

	for (int k = 0; k < totalTrinum; k++) {
		i0 = (*outPolyIndeies)[k * 3];
		i1 = (*outPolyIndeies)[k * 3 + 1];
		i2 = (*outPolyIndeies)[k * 3 + 2];

		Vector3 pnt0((*outVertices)[i0 * 2].X, (*outVertices)[i0 * 2].Y,
				(*outVertices)[i0 * 2].Z);
		Vector3 pnt1((*outVertices)[i1 * 2].X, (*outVertices)[i1 * 2].Y,
				(*outVertices)[i1 * 2].Z);
		Vector3 pnt2((*outVertices)[i2 * 2].X, (*outVertices)[i2 * 2].Y,
				(*outVertices)[i2 * 2].Z);
		bodyPosArray->push_back(pnt0);
		bodyPosArray->push_back(pnt1);
		bodyPosArray->push_back(pnt2);

		Vector3 Normal0((*outVertices)[i0 * 2 + 1].X,
				(*outVertices)[i0 * 2 + 1].Y, (*outVertices)[i0 * 2 + 1].Z);
		Vector3 Normal1((*outVertices)[i1 * 2 + 1].X,
				(*outVertices)[i1 * 2 + 1].Y, (*outVertices)[i1 * 2 + 1].Z);
		Vector3 Normal2((*outVertices)[i2 * 2 + 1].X,
				(*outVertices)[i2 * 2 + 1].Y, (*outVertices)[i2 * 2 + 1].Z);

		bodyNormalArray->push_back(Normal0);
		bodyNormalArray->push_back(Normal1);
		bodyNormalArray->push_back(Normal2);

		if(hasTextureCoord)
		{
			Vector3 textureCoord0((*outTextureCoords)[i0 + 1].X,
							(*outTextureCoords)[i0 + 1].Y, (*outTextureCoords)[i0  + 1].Z);
			Vector3 textureCoord1((*outTextureCoords)[i1  + 1].X,
							(*outTextureCoords)[i1+ 1].Y, (*outTextureCoords)[i1  + 1].Z);
			Vector3 textureCoord2((*outTextureCoords)[i2 + 1].X,
							(*outTextureCoords)[i2 + 1].Y, (*outTextureCoords)[i2  + 1].Z);

			textureCoordsArray->push_back(textureCoord0);
			textureCoordsArray->push_back(textureCoord1);
			textureCoordsArray->push_back(textureCoord2);
		}
	}
}
const string& SvlReader::GetXMLAnimationData() {
	return this->m_xmlAnimatinData;
}
void SvlReader::SetXMLAnimationData(const string& xmlData) {
	this->m_xmlAnimatinData = xmlData;
	//READER_LOGE(" SvlReader::SetXMLAnimationData  %s",this->xmlAnimationData.c_str());
}

void SvlReader::FillShapeProperty(const vector<Stk_MetaData*>& protoTypeProperties,ShapeProperty* shapeProperty)
{
	vector<ShapePropertyItem>& shapeProperties = shapeProperty->GetProperties();
	shapeProperties.clear();
	for(int i =0 ;i<protoTypeProperties.size();i++)
	{
		Stk_MetaData* metaData = protoTypeProperties.at(i);
		if(metaData)
		{
			ShapePropertyItem propertyItem ;
			propertyItem.m_strTitle = Platform::WStringToString(metaData->GetTitle(), "auto");
			propertyItem.m_eValueType = (ShapePropertyValueTypeEnum)metaData->GetType();
			propertyItem.m_strValue = Platform::WStringToString(metaData->GetValue(), "auto");
			propertyItem.m_strUnits = Platform::WStringToString(metaData->GetUnits(), "auto");

			shapeProperties.push_back(propertyItem);
		}
	}
}


} //ns
