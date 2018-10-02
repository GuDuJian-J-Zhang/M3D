/*
 * ModelView.cpp
 *
 *  Created on: 2013-12-6
 *      Author: bing
 */

#include "m3d/model/ModelView.h"
#include "m3d/base/json/json.h"
#include "SVLLib/Stk_API.h"
#include "m3d/utils/PathHelper.h"
#include "m3d/utils/StringHelper.h"
using namespace HoteamSoft::SVLLib;

namespace M3D {

int ModelView::m_MaxId = 1000;
ModelView::ModelView() :
		Object() {
	m_ID = ++ModelView::m_MaxId;
	m_UpDataModel = false;
	m_UpDataCamera = false;
	m_ShowCappingPlane = false;
	m_ShowClipPlane = false;
    m_ReverseClipping = false;
	m_ExplosiveType = -1;
	m_ExplosivePercent = 0;
	m_SvlType = ViewSvlUseTypeEnum::VIEW_USAGE_UNKNOWN;
	m_ViewType = Undefine;
	m_bInitView = false;
}
ModelView::ModelView(ModelView & modelView) :
		Object() {
	LOGI("cope  ModelView BEgin");
	m_MaxId++;
	this->m_ID = modelView.m_ID;
	this->m_Name = modelView.m_Name; ///视图名称
	this->m_Camera = modelView.m_Camera; ///视图所在相机 视角、方向、目标点等
	this->m_PMIList = modelView.m_PMIList; /// PMI id 列表
	this->m_ExplosiveType = modelView.m_ExplosiveType;
	this->m_ExplosivePercent = modelView.m_ExplosivePercent;
	this->m_NoteList = modelView.m_NoteList; /// Note id 列表

	this->m_SectionPlaneIDList = modelView.m_SectionPlaneIDList; //
	this->m_UpDataCamera = modelView.m_UpDataCamera; /// 标识是否更新Camera
	this->m_UpDataModel = modelView.m_UpDataModel; /// 标识是否更新模型的矩阵
	// vector<string> m_noteDataList;//存储视图关联的note数据
	this->m_noteDataList = modelView.m_noteDataList; ///存储视图关联的note数据
	this->m_PolyLines = modelView.m_PolyLines;
	this->m_PLineColors = modelView.m_PLineColors;
	this->m_InstanceAttributeMap = modelView.m_InstanceAttributeMap; ///模型的当前ins信息map
	this->m_SvlType = modelView.m_SvlType; //从svl中读取到的type
	this->m_ViewType = modelView.m_ViewType; //模型视图类型，-1=未初始化，0=默认视图（也是sview自订），1=svl中视图，2=sview自订视图
	this->m_appendInfo = modelView.m_appendInfo; ///附加信息
	this->m_bInitView = modelView.IsInitView();
	LOGI("cope  ModelView End");
}

ModelView::~ModelView() {

}

//设置Id，可能由ModelView自动生成Id，也可能从外部传入Id
void ModelView::SetID(int id) {
	m_ID = id;
	if (ModelView::m_MaxId < id)
	{
		ModelView::m_MaxId = id;
	}
}

void ModelView::AddSectionPlaneId(int id) {
	bool exist = false;
	for (int i = 0; i < m_SectionPlaneIDList.size(); i++) {
		int value = m_SectionPlaneIDList.at(i);
		if (value == id) {
			exist = true;
			break;
		}
	}

	if (!exist)
		m_SectionPlaneIDList.push_back(id);
}

void ModelView::ClearSectionPlaneId() {
	m_SectionPlaneIDList.clear();
}

vector<int> ModelView::GetNoteList() {
	return m_NoteList;
}

bool ModelView::GetUpDataModelState() {
	return m_UpDataModel;
}

void ModelView::SetUpDataModel(bool state) {
	m_UpDataModel = state;
}

bool ModelView::GetUpDataCameraState() {
	return m_UpDataCamera;
}

void ModelView::SetUpDataCamera(bool state) {
	m_UpDataCamera = state;
}

vector<int> ModelView::GetPMIList() {
	return m_PMIList;
}

vector<int> ModelView::GetSectionPlaneIDList() {
	return m_SectionPlaneIDList;
}
/**
 * 设置怕剖切面的辅助面显示标示
 * @param flag
 */
void ModelView::SetShowSectionCappingPlane(bool flag) {
	m_ShowCappingPlane = flag;
}
/**
 * 设置剖切面反面显示
 * @param flag
 */
void ModelView::SetReverseClipping(bool flag){
    m_ReverseClipping = flag;
}
/**
 * 获取剖切面反面显示
 *
 */
bool ModelView::IsReverseClipping(){
    return m_ReverseClipping;
}
/**
 * 设置怕剖切面显示标示
 * @param flag
 */
void ModelView::SetShowClipSectionPlane(bool flag) {
	m_ShowClipPlane = flag;
}
/**
 * 获取剖切面显示标示
 *
 */
bool ModelView::GetShowClipSectionPlane() {
	return m_ShowClipPlane;
}
/**
 * 获取剖切面的辅助面显示标示
 *
 */
bool ModelView::GetShowSectionCappingPlane() {
	return m_ShowCappingPlane;
}
const CameraNode&
ModelView::GetCamera() {
	return m_Camera;
}

string ModelView::GetName() {
	return m_Name;
}

int ModelView::GetID() {
	return m_ID;
}

void ModelView::AddNoteId(int id) {
	m_NoteList.push_back(id);
}

void ModelView::SetNoteIds(vector<int> ids) {
	m_NoteList.assign(ids.begin(), ids.end());
}

void ModelView::AddPMIId(int id) {
	m_PMIList.push_back(id);
}

void ModelView::SetPMIIds(vector<int> ids) {
	m_PMIList.assign(ids.begin(), ids.end());
}

void ModelView::SetCamera(CameraNode &camera) {
	m_Camera = camera;
}

void ModelView::SetName(const char* name) {
	m_Name = name;
}

string ModelView::GetProperty(string key) {
	string ret = "";

	map<string, string>::iterator it;
	it = m_appendInfo.find(key);
	if (it != m_appendInfo.end())
		ret = it->second;

	return ret;
}
void ModelView::SetProperty(string key, string value) {
	m_appendInfo.insert(make_pair(key, value));
}
void ModelView::ClearProperties() {
	m_appendInfo.erase(m_appendInfo.begin(), m_appendInfo.end());
}

bool ModelView::DeleteProperty(string key) {
	bool ret;
	map<string, string>::iterator it;
	it = m_appendInfo.find(key);
	if (it != m_appendInfo.end()) {
		m_appendInfo.erase(it);
		ret = true;
	} else {
		ret = false;
	}

	return ret;
}

vector<string>*
ModelView::GetNoteDataList(SHAPETYPE key) {

	map<SHAPETYPE, vector<string> >::iterator it = m_noteDataList.find(key);
	if (it != m_noteDataList.end())
		return &(it->second);
	else {
		vector<string> temp;
		temp.clear();
		m_noteDataList.insert(make_pair(key, temp));
		return &(m_noteDataList.at(key));
	}

}

void ModelView::ClearNoteDataList() {
	map<SHAPETYPE, vector<string> >::iterator it;
	for (it = m_noteDataList.begin(); it != m_noteDataList.end(); it++) {
		it->second.clear();
	}
}

void ModelView::SetNoteDataList(SHAPETYPE key, vector<string>& noteDataList) {
	m_noteDataList.insert(make_pair(key, noteDataList));
}
//手势线
void ModelView::SetGestureNotePolyLines(vector<PolyLine> &m_pLines) {
	m_PolyLines = m_pLines;
}
vector<PolyLine>&
ModelView::GetGestureNotePolyLines() {
	return m_PolyLines;
}
//手势线颜色
void ModelView::SetGestureNotePolyLineColors(vector<Color> &pLineColors) {
	m_PLineColors = pLineColors;
}
vector<Color>&
ModelView::GetGestureNotePolyLineColors() {
	return m_PLineColors;
}

string ModelView:: toJson(){
	Json::FastWriter writer;
	Json::Value modelViewsJson ;
	modelViewsJson["version"] = "0.1";
	Json::Value modelViewJson ;

	modelViewJson["id"] = m_ID;
	modelViewJson["name"] = m_Name;
	modelViewJson["usageType"] = m_SvlType;
	modelViewJson["isActivated"] = false;
	modelViewJson["isDefault"] = false;
	modelViewJson["transparency"] = 1.0;

//相机参数
	Json::Value cameraJson ;
	HoteamSoft::SVLLib::STK_UINT32 ProjectType = 1;
	if(m_Camera.IsOrthographic()){
		ProjectType = 1;
		cameraJson["projectType"] = ProjectType;
		float fOrthoWidth;
		float fOrthoHeight;
		m_Camera.GetOrthoSize(&fOrthoWidth, &fOrthoHeight);
		cameraJson["height"] = (fOrthoHeight/ m_Camera.GetZoom());
		cameraJson["focalDistance"] = 1.0f;
	}else{
		ProjectType = 0;
		cameraJson["projectType"] = ProjectType;
		float fPosTargetDistance = 1.0f;
//		BoundingBox box = m_View->GetSceneManager()->GetSceneBox();
//		fPosTargetDistance = m_View->GetSceneManager()->GetSceneBox().Length()
//					* CAMERA_POSFACTOR;
		cameraJson["focalDistance"] = fPosTargetDistance;
		float fHeight = (float) (fPosTargetDistance * 2.0
				* (tanf(m_Camera.GetFov() * M_DEGTORAD * 0.5f)
						/ m_Camera.GetZoom()));
		cameraJson["height"] = fHeight;
	}
	cameraJson["angle"] = 1.0;
	Quaternion rotation = m_Camera.GetRotation();
	Vector3 position = m_Camera.GetPosition();
    cameraJson["origin"] = position.Tostring();
    cameraJson["targetVector"] = m_Camera.GetDirection().Tostring();
    cameraJson["upVector"] = m_Camera.GetUp().Tostring();
	Matrix4 sbMatrix(position, rotation, 1.0f);
    HoteamSoft::SVLLib::STK_MTX32 m_mtxTransform;
    for (size_t m = 0; m < 4; m++)
    {
        for (size_t n = 0; n < 4; n++)
            m_mtxTransform.PlcMatrix[m][n] = sbMatrix.Data()[n * 4 + m];
    }
    char cTemp[256];
    string strMatirx = "";
    sprintf(cTemp, "%.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f",
            m_mtxTransform.PlcMatrix[0][0], m_mtxTransform.PlcMatrix[0][1], m_mtxTransform.PlcMatrix[0][2], m_mtxTransform.PlcMatrix[0][3],
            m_mtxTransform.PlcMatrix[1][0], m_mtxTransform.PlcMatrix[1][1], m_mtxTransform.PlcMatrix[1][2], m_mtxTransform.PlcMatrix[1][3],
            m_mtxTransform.PlcMatrix[2][0], m_mtxTransform.PlcMatrix[2][1], m_mtxTransform.PlcMatrix[2][2], m_mtxTransform.PlcMatrix[2][3],
            m_mtxTransform.PlcMatrix[3][0], m_mtxTransform.PlcMatrix[3][1], m_mtxTransform.PlcMatrix[3][2], m_mtxTransform.PlcMatrix[3][3]);
    strMatirx = cTemp;
	cameraJson["matix"] = sbMatrix.Tostring();
	cameraJson["aspectRatio"] = m_Camera.GetAspectRatio();
	cameraJson["nearDistance"] = m_Camera.GetNearClip();
	cameraJson["farDistance"] = m_Camera.GetFarClip();

	modelViewJson["camera"] = cameraJson;
	//实例属性
	Json::Value insAttributesJson;
	map<int, InstanceAttribute> mapInstanceAttribute =
			GetInstanceAttributeMap();
	for (map<int, InstanceAttribute>::const_iterator it =
			mapInstanceAttribute.begin(); it != mapInstanceAttribute.end();
			it++) {
		const InstanceAttribute &curInsAtt = it->second;
		Json::Value insAttributeJson;
		insAttributeJson["plcPath"] = PathHelper::SVLPathDecToHex(
				curInsAtt.path);
		insAttributeJson["visible"] =
				curInsAtt.visible ?
						HoteamSoft::SVLLib::STK_DISPLAY :
						HoteamSoft::SVLLib::STK_NO_DISPLAY;
		insAttributeJson["matix"] = curInsAtt.placeMatrix.Tostring();
		insAttributeJson["color"] = curInsAtt.insColor.Tostring();
		insAttributesJson[curInsAtt.path] = insAttributeJson;
	}

	modelViewJson["insAttributes"] = insAttributesJson;
	modelViewsJson["views"].append(modelViewJson);

		Json::Value valueJson ;
	//	vector<int> vecView = Ge();
//		if(vecView.size()>0){
			//	for(int i = 0 ; i<vecView.size();i++){
			//			int NodeID = vecView[i];
			//			valueJson["views"].append(NodeID) ;
			//		}
//		valueJson["id"] = this->m_ID ;
//		modelViewsJson["model_views"].append(valueJson);
//		}else
		{
	valueJson.resize(0);
	modelViewsJson["model_views"] = valueJson;
		}
	//试图关联的PMI
	vector<int> vecPMI = GetPMIList();
	Json::Value pmiJson ;
	if(vecPMI.size()>0){
		for(int i = 0 ; i<vecPMI.size();i++){
				int pmiID = vecPMI[i];
				pmiJson["pmis"].append(pmiID) ;
			}
		pmiJson["id"] = this->m_ID ;
		modelViewsJson["view_pmis"].append(pmiJson);
	}	else {
		pmiJson.resize(0);
		modelViewsJson["view_pmis"] = pmiJson;
			}
	//试图关联的View
//	vector<int> vecView = Ge();
	Json::Value viewJson ;
//	if(vecPMI.size()>0){
//	for(int i = 0 ; i<vecView.size();i++){
//			int NodeID = vecView[i];
//			viewJson["views"].append(NodeID) ;
//		}
//	viewJson["id"] = this->m_ID ;
//	modelViewsJson["view_views"] .append (viewJson);
//}else
	{
		viewJson.resize(0);
	modelViewsJson["view_views"] = viewJson;
		}
	//试图关联的批注
	vector<int> vecNote = GetNoteList();
	Json::Value noteJson ;
	LOGI( " vecNote.size() =%d",vecNote.size());
	if(vecNote.size()>0){
	for(int i = 0 ; i<vecNote.size();i++){
			int NodeID = vecNote[i];
			noteJson["notes"].append(NodeID) ;
		}
	noteJson["id"] = this->m_ID ;
	modelViewsJson["view_notes"] .append (noteJson);
	}else
	{
		noteJson.resize(0);
	modelViewsJson["view_notes"] = noteJson;
		}

	viewJsonStr = modelViewsJson.toStyledString();
	LOGE("viewJsonStr =",viewJsonStr.c_str());
    return viewJsonStr;
}


 ModelView* ModelView:: fromJson( string& jsonStr){
	 string jsonValue = jsonStr;
	ModelView *modelView = new ModelView();
	Json::Reader reader;
	Json::Value modelViewsJson;
	Json::Value viewsJson;
	Json::Value viewJson;
	if (reader.parse(jsonValue, modelViewsJson)) {
		viewsJson = modelViewsJson["views"];
	for(int i = 0 ; i< viewsJson.size();i++){
		viewJson = viewsJson[i];
		modelView->SetID(viewJson["id"].asInt());
		modelView->SetName(viewJson["name"].asCString());
		modelView->SetSvlUseType(viewJson["usageType"].asInt());
		switch (modelView->GetSvlUseType()) {
		case HoteamSoft::SVLLib::VIEW_USAGE_GENERAL_VIEW:
		case HoteamSoft::SVLLib::VIEW_USAGE_PROE_BASE_VIEW:
		case HoteamSoft::SVLLib::VIEW_USAGE_DEFAULT_VIEW:
			modelView->SetViewType(ModelView::DefaultView);
			break;
		case HoteamSoft::SVLLib::VIEW_USAGE_SV_USER_VIEW:
		case HoteamSoft::SVLLib::VIEW_USAGE_SV_USER_CLIPVIEW:
		case HoteamSoft::SVLLib::VIEW_USAGE_PROE_USER_VIEW:
			modelView->SetViewType(ModelView::UserView);
			break;
		default:
			//            continue;
			break;
		}
		bool isActivated = !viewJson["isActivated"].asBool();
		if (!isActivated
				&& modelView->GetSvlUseType()
						!= HoteamSoft::SVLLib::VIEW_USAGE_USER_CLIPPLANE) {
			modelView->SetIsInitView(true);
		} else {
			modelView->SetIsInitView(false);
		}

		//获取相机参数
		CameraNode cameraInfo;

		Json::Value camaraJson = viewJson["camera"];
        
        //投影类型
        float fHeight = camaraJson["height"].asFloat();
        cameraInfo.SetAspectRatio(camaraJson["aspectRatio"].asFloat());
        if (camaraJson["projectType"].asInt() == 1) {
            cameraInfo.SetOrthographic(true);
            cameraInfo.SetOrthoSize(
                 Vector2(fHeight * cameraInfo.GetAspectRatio(),
                                            fHeight));
        } else {
            cameraInfo.SetOrthographic(false);
        }
        cameraInfo.SetZoom(1.0f);
        cameraInfo.SetFov(camaraJson["angle"].asFloat());

        string positionStr = camaraJson["origin"].asString();
        vector<string> positionValue = StringHelper::Split(positionStr, " ");
        Vector3 positionVector3 ;
        positionVector3.m_x =  atof(positionValue[0].c_str());
        positionVector3.m_y =  atof(positionValue[1].c_str());
        positionVector3.m_z =  atof(positionValue[2].c_str());
        cameraInfo.SetPosition(positionVector3);
        string directionStr = camaraJson["targetVector"].asString();
        vector<string> directionValue = StringHelper::Split(directionStr, " ");
        Vector3 directionVector3 ;
        directionVector3.m_x =  atof(directionValue[0].c_str());
        directionVector3.m_y =  atof(directionValue[1].c_str());
        directionVector3.m_z =  atof(directionValue[2].c_str());
        cameraInfo.SetDirection(directionVector3);


		cameraInfo.SetNearClip(camaraJson["nearDistance"].asFloat());
		cameraInfo.SetFarClip(camaraJson["farDistance"].asFloat());
        //获取镜头原始位置
        Vector3 pos = cameraInfo.GetPosition();
        
        //获取Target方向向量
        Vector3 target = cameraInfo.GetDirection();
        
        LOGI("ModelView target.x =%f ;target.y =%f ;target.z =%f",target.m_x,target.m_y,target.m_z);

        string upStr = camaraJson["upVector"].asString();
			vector<string> upValue = StringHelper::Split(upStr, " ");
			Vector3 up;
			up.m_x = atof(upValue[0].c_str());
			up.m_y = atof(upValue[1].c_str());
			up.m_z = atof(upValue[2].c_str());
        LOGI("ModelView up.x =%f ;up.y =%f ;up.z =%f",up.m_x,up.m_y,up.m_z);
        HoteamSoft::SVLLib::STK_MTX32 matrix4;
        string strMatrix = camaraJson["matix"].asString();
        std::vector<std::string> vecMatrixValue = StringHelper::Split(strMatrix, " ");
        if (vecMatrixValue.size() == 16)
        {
            for (int ii = 0; ii < 4; ii++)
            {
                for (int jj = 0; jj < 4; jj++)
                {
                    matrix4.PlcMatrix[ii][jj] = atof(vecMatrixValue[ii * 4 + jj].c_str());
                }
            }
        }
        vecMatrixValue.clear();
        Quaternion rotation;
        if ((target.m_x != -1.0f || target.m_y != -1.0f
             || target.m_z != -1.0f)
            && (up.m_x != -1.0f || up.m_y != -1.0f || up.m_z != -1.0f)) {
            //构造出旋转,参考李坤的算法,貌似和已有的通过direct和up构造的方法不一致.
            Vector3 x = target.CrossProduct(up);
            Vector3 y = up;
            Vector3 z = -target;
            rotation.FromAxes(x, y, z);
        } else {
            Matrix3 matrix3(matrix4.PlcMatrix[0][0],matrix4.PlcMatrix[1][0], matrix4.PlcMatrix[2][0],
                            matrix4.PlcMatrix[0][1],matrix4.PlcMatrix[1][1], matrix4.PlcMatrix[2][1],
                            matrix4.PlcMatrix[0][2],matrix4.PlcMatrix[1][2], matrix4.PlcMatrix[2][2]);
            rotation.FromRotationMatrix(matrix3);
            pos.m_x = matrix4.PlcMatrix[3][0];
            pos.m_y = matrix4.PlcMatrix[3][1];
            pos.m_z = matrix4.PlcMatrix[3][2];
        }
        cameraInfo.SetRotation(rotation);
//        cameraInfo.SetPosition(pos);
//给视图添加相机
			modelView->SetCamera(cameraInfo);
			modelView->SetUpDataCamera(true);
			modelView->SetUpDataModel(true);
			//实例属性
			map<int, InstanceAttribute> mapInstanceAttribute;
			Json::Value insAttributesJson = viewJson["insAttributes"];
			Json::Value insAttributeJson;
			Json::Value::Members mem(insAttributesJson.getMemberNames());
			InstanceAttribute ia;
			for (Json::Value::Members::iterator it = mem.begin();
					it != mem.end(); ++it) {
				const std::string &name = *it;
				insAttributeJson = insAttributesJson[name];
				ia.path = insAttributeJson["plcpath"].asString();
				string colorStr = insAttributeJson["color"].asString();
                std::vector<std::string> vecColorValue = StringHelper::Split(colorStr, " ");
                float colorR = atof(vecColorValue[0].c_str());
                float colorG = atof(vecColorValue[1].c_str());
                float colorB = atof(vecColorValue[2].c_str());
                float colorA = atof(vecColorValue[3].c_str());
				Color dispColor(colorR, colorG, colorB, colorA);
				ia.insColor = dispColor;
				ia.hasColor = true;
				string matix = insAttributeJson["matix"].asString();
                
                HoteamSoft::SVLLib::STK_MTX32 m_matrix4;
                std::vector<std::string> m_vecMatrixValue = StringHelper::Split(matix, " ");
                if (m_vecMatrixValue.size() == 16)
                {
                    for (int ii = 0; ii < 4; ii++)
                    {
                        for (int jj = 0; jj < 4; jj++)
                        {
                            m_matrix4.PlcMatrix[ii][jj] = atof(m_vecMatrixValue[ii * 4 + jj].c_str());
                        }
                    }
                }
                m_vecMatrixValue.clear();
                ia.placeMatrix.Set((const float*) m_matrix4.PlcMatrix);
				ia.visible = insAttributeJson["visible"].asBool();
			}
		}
		//PMI 信息
		Json::Value pmisJson = modelViewsJson["view_pmis"]["pmis"];
		LOGE(" pmisJson= %s", pmisJson.toStyledString().c_str());
		Json::Value pmiJson;
		for (int it = 0; it != pmisJson.size(); it++) {
			pmiJson = pmisJson[it];
            modelView->AddPMIId(pmiJson.asInt());
		}

	}
        return modelView;
 }


//	int ModelView::GetType()
//	{
//		return m_Type;
//	}

//	void ModelView::setType( int type )
//	{
//		m_Type = type;
//	}

//	void ModelView::setEnable( bool enable )
//	{
//		m_IsEnable = enable;
//	}

} /* namespace M3D */
