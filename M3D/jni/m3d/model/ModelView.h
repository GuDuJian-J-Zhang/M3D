/**@file
 *@brief	ModelView类头文件
 *@author	bing
 *@date		2013-12-6
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *ModelView类头文件
 *
 */

#ifndef M3D_MODELVIEW_H_
#define M3D_MODELVIEW_H_

#include "m3d/M3D.h"
#include "m3d/graphics/MovedMatrixInfo.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/model/InstanceAttribute.h"
#include "Stk_Enum.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/model/PolyLine.h"

namespace M3D {

/**@class ModelView
 * @brief ModelView类
 *
 * 视图对象：用于存储当前摄像机位置和方向、缩放并且用于与PMI、剖面、测量、批注等对象关联显示。
 */
class M3D_API ModelView: public Object {
public:

	/*
	 * 视图类型（自定） -1=未初始化，
	 * 0=默认视图（也是sview自订），
	 * 1=svl中视图，2=sview自订视图
	 *
	 */
	enum ViewTypeEnum {
		Undefine = -1, DefaultView = 0, OrignalView = 1, UserView = 2,
	};

	/*
	 * 静态方法，
	 * 设置视图当前最大值
	 * （用于读取视图文件后，新建视图前）
	 */
	static void SetMaxID(int id) {
		ModelView::m_MaxId = id;
	}
	;

	ModelView();
	ModelView(ModelView & modelView);
	virtual ~ModelView();

	/**
	 * 设置id
	 * @param id
	 */
	void SetID(int id);

	/**
	 * 设置名称
	 * @param name
	 */
	void SetName(const char* name);

	/**
	 * 设置Camera
	 * @param camera
	 */
	void SetCamera(CameraNode &camera);

	/**
	 * 新增PMI的id关联
	 * @param id
	 */
	void AddPMIId(int id);

	/**
	 * 设置PMI的关联ids
	 * @param ids
	 */
	void SetPMIIds(vector<int> ids);

	/**
	 * 新增Note的id关联
	 * @param id
	 */
	void AddNoteId(int id);

	/**设置Note的Id的关联列表
	 *
	 * @param ids
	 */
	void SetNoteIds(vector<int> ids);

	/**
	 * 新增剖视平面的id关联
	 * @param id
	 */
	void AddSectionPlaneId(int id);
	/**
	 * 设置剖切面剖切方向
	 * @param direction
	 */
	void SetSectionPlaneDirection(int direction);
	/**
	 * 获取剖切面剖切方向
	 *
	 */
	int GetSectionPlaneDirection();
	/**
	 * 设置剖切面剖切比例
	 * @param percentage
	 */
	void SetSectionPlanePercentage(float percentage);
	/**
	 * 获取剖切面剖切比例
	 *
	 */
	float GetSectionPlanePercentage();
	/**
	 * 设置怕剖切面的辅助面显示标示
	 * @param flag
	 */
	void SetShowSectionCappingPlane(bool flag);
	/**
	 * 设置怕剖切面显示标示
	 * @param flag
	 */
	void SetShowCutSectionPlane(bool flag);
	/**
	 * 获取剖切面显示标示
	 *
	 */
	bool GetShowCutSectionPlane();
	/**
	 * 获取剖切面的辅助面显示标示
	 *
	 */
	bool GetShowSectionCappingPlane();
	/**
	 * 获取id
	 * @return
	 */
	int GetID();

	/**
	 * 获取名称
	 * @return
	 */
	string GetName();

	/**
	 * 获取Camera
	 * @return
	 */
	const CameraNode& GetCamera();

	/**
	 * 获取PMI的id列表
	 * @return
	 */
	vector<int> GetPMIList();

	/**
	 * 设置是否更新相机标志
	 * @param state
	 */
	void SetUpDataCamera(bool state);

	/**
	 * 获取是否更新相机标志
	 * @return
	 */
	bool GetUpDataCameraState();

	/**
	 * 设置是否更新模型标志
	 * @param state
	 */
	void SetUpDataModel(bool state);

	/**
	 * 获取是狗更新模型标志
	 * @return
	 */
	bool GetUpDataModelState();

	/**
	 * 获取Note的id列表
	 * @return
	 */
	vector<int> GetNoteList();

	/**
	 * 获取剖视平面的id列表
	 * @return
	 */
	vector<int> GetSectionPlaneIDList();

	/**
	 * 获取模型的当前矩阵信息map
	 * @return
	 */
	const map<int, InstanceAttribute>& GetInstanceAttributeMap() {
		return m_InstanceAttributeMap;
	}

	/**
	 * 设置模型的当前矩阵信息map
	 * @param value
	 */
	void SetInstanceAttributeMap(map<int, InstanceAttribute>& value) {
		m_InstanceAttributeMap = value;
	}

	/**
	 * 新增ModelMatrix到Map
	 * @param path
	 * @param info
	 */
	void AddInstanceAttribute(int insID, InstanceAttribute& info) {
		m_InstanceAttributeMap.insert(
				pair<int, InstanceAttribute>(insID, info));
	}

	/**
	 * 获取属性
	 * @param key
	 * @return
	 */
	string GetProperty(string key);
	/**
	 * 设置属性
	 * @param key
	 * @param value
	 */
	void SetProperty(string key, string value);
	/**
	 *清空所有属性
	 */
	void ClearProperties();
	/**
	 * 删除属性
	 * @param key
	 * @return
	 */
	bool DeleteProperty(string key);

	/**
	 * 获取指定key的xml属性数据
	 * @param key
	 * @return
	 */
	vector<string>* GetNoteDataList(SHAPETYPE key);

	/**
	 * 设置指定类型shape对象的序列化数据
	 * @param key
	 * @param noteDataList
	 */
	void SetNoteDataList(SHAPETYPE key, vector<string>& noteDataList);

	/**
	 * 获取指定类型的shape对象的序列化字符串数据
	 * @return
	 */
	map<SHAPETYPE, vector<string> > & GetNoteMap() {
		return m_noteDataList;
	}
	/**
	 * 清空View关联的所有Note对象数据
	 */
	void ClearNoteDataList();

	/**
	 * 手势线
	 * @param m_pLines
	 */
	void SetGestureNotePolyLines(vector<PolyLine> &m_pLines);
	/**
	 * 获取当前视图对应的手势批注数据
	 * @return
	 */
	vector<PolyLine>& GetGestureNotePolyLines();

	/**
	 * 手势线颜色
	 * @param pLineColors
	 */
	void SetGestureNotePolyLineColors(vector<Color> &pLineColors);
	/**
	 * 获取收拾批注线颜色
	 * @return
	 */
	vector<Color>& GetGestureNotePolyLineColors();

	/**
	 * 设置当前视图类型
	 * @param type
	 */
	void SetViewType(ViewTypeEnum type) {
		m_ViewType = type;
	}

	/**
	 * 获取当前视图类型
	 * @return
	 */
	ViewTypeEnum GetViewType() {
		return m_ViewType;
	}

	/**
	 * 设置当前视图对应SVL文件中的视图类型
	 * @param type
	 */
	void SetSvlUseType(StkViewUsageEnum type) {
		m_SvlType = type;
	}

	/**
	 * 获取当前视图对应的SVL格式文件中的类型
	 * @return
	 */
	StkViewUsageEnum GetSvlUseType() {
		return m_SvlType;
	}

//	设置爆炸方向
	void setExplosiveType(int type) {
		m_ExplosiveType = type;
	}
	//	设置爆炸方向
	int getExplosiveType() {
		return m_ExplosiveType;
	}
	//	设置爆炸百分比
	void setExplosivePercent(float percent) {
		m_ExplosivePercent = percent;
	}
	//	设置爆炸百分比
	float getExplosivePercent() {
		return m_ExplosivePercent;
	}

private:
	int m_ID; //!<视图ID
	string m_Name; //!<视图名称
	CameraNode m_Camera; ///视图所在相机 视角、方向、目标点等
	vector<int> m_PMIList; //!< PMI id 列表
	vector<int> m_NoteList; //!< Note id 列表
	int m_ExplosiveType;
	float m_ExplosivePercent;
	vector<int> m_SectionPlaneIDList; //!<
	bool m_UpDataCamera; //!< 标识是否更新Camera
	bool m_UpDataModel; //!< 标识是否更新模型的矩阵

	int m_Direction; //剖切方向
	float m_Percentage; //剖切比例
	bool m_ShowCutPlane; //显示剖切面
	bool m_ShowCappingPlane; //显示剖切辅助面

	map<SHAPETYPE, vector<string> > m_noteDataList; //!<存储视图关联的note数据
	vector<PolyLine> m_PolyLines; //!<存储视图关联的手势批注线数据
	vector<Color> m_PLineColors; //!<存储视图关联的手势批注线颜色数据
	map<int, InstanceAttribute> m_InstanceAttributeMap; //!<模型的当前ins信息map
	StkViewUsageEnum m_SvlType; //!<从svl中读取到的type
	ViewTypeEnum m_ViewType; //!<模型视图类型，-1=未初始化，0=默认视图（也是sview自订），1=svl中视图，2=sview自订视图

	map<string, string> m_appendInfo; //!<附加信息

private:
	static int m_MaxId; //!<
};

}
#endif /* MODELVIEW_H_ */
