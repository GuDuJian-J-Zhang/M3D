/*
 * ModelView.cpp
 *
 *  Created on: 2013-12-6
 *      Author: bing
 */

#include "m3d/model/ModelView.h"
#include "m3d/base/json/json.h"

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
	m_Direction = -1000;
	m_Percentage = -1;
    m_DirectionX = -1000;
    m_PercentageX = -1;
    m_DirectionY = -1000;
    m_PercentageY = -1;
    m_DirectionZ = -1000;
    m_PercentageZ = -1;
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
	this->m_Direction = modelView.m_Direction;
	this->m_Percentage = modelView.m_Percentage;
    this->m_DirectionX = modelView.m_DirectionX;
    this->m_PercentageX = modelView.m_PercentageX;
    this->m_DirectionY = modelView.m_DirectionY;
    this->m_PercentageY = modelView.m_PercentageY;
    this->m_DirectionZ = modelView.m_DirectionZ;
    this->m_PercentageZ = modelView.m_PercentageZ;
	this->m_appendInfo = modelView.m_appendInfo; ///附加信息
	this->m_bInitView = modelView.IsInitView();
	LOGI("cope  ModelView End");
}

ModelView::~ModelView() {

}

//设置Id，可能由ModelView自动生成Id，也可能从外部传入Id
void ModelView::SetID(int id) {
	m_ID = id;
//	if (ModelView::m_MaxId < id)
//	{
//		ModelView::m_MaxId = id;a
//	}
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
 * 设置剖切面剖切方向
 * @param direction
 */
void ModelView::SetSectionPlaneDirection(int direction) {
	m_Direction = direction;
}
/**
 * 获取剖切面剖切方向
 *
 */
int ModelView::GetSectionPlaneDirection() {
	return m_Direction;
}
/**
 * 设置剖切面剖切方向
 * @param direction
 */
void ModelView::SetSectionPlaneDirectionAndPercentage(int directionX,int directionY,int directionZ, float percentageX,float percentageY,float percentageZ) {
    m_DirectionX = directionX;
    m_DirectionY = directionY;
    m_DirectionZ = directionZ;
    m_PercentageX = percentageX;
    m_PercentageY = percentageY;
    m_PercentageZ = percentageZ;
}

/**
 * 设置剖切面剖切比例
 * @param percentage
 */
void ModelView::SetSectionPlanePercentage(float percentage) {
	m_Percentage = percentage;
}
/**
 * 获取剖切面剖切比例
 *
 */
float ModelView::GetSectionPlanePercentage() {
	return m_Percentage;
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
	Json::Value viewJson ;
	Json::Value cameraJson ;
	viewJson["ID"] = m_ID;
	viewJson["name"] = m_Name;
	viewJson["usageType"] = m_ViewType;
	viewJson["isActivated"] = false;
	viewJson["transparency"] = 1;

	cameraJson["projectType"] = m_Camera.IsOrthographic();

	//TODO 相机暂时没提供以下接口
	cameraJson["nearDistance"] = m_Camera.GetPosition().Tostring();
	cameraJson["farDistance"] = m_Camera.GetPosition().Tostring();
	cameraJson["focalDistance"] = m_Camera.GetPosition().Tostring();
	cameraJson["viewVolumeSize"] = m_Camera.GetPosition().Tostring();
	//TODO end
	cameraJson["pos"] = m_Camera.GetPosition().Tostring();
	cameraJson["quat"] = m_Camera.GetDirection().Tostring();
	viewJson["camera"] = cameraJson;

	//TODO 暂定内容
	viewJson["clipPlanes"] = "";
	viewJson["connectors"] = "0,0,0";
	viewJson["insAttributes"] = "";
    viewJson["PMIs"]= "1,2,3";
    viewJson["notes"]= "1,2,3";

	viewJsonStr = writer.write(viewJson);
        return viewJsonStr;
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
