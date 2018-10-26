/**@file
 *@brief	Parameters类头文件
 *@author
 *@date
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *提供Parameters类的声明
 *
 */

#ifndef M3D_PAMAMETERS_H_
#define M3D_PAMAMETERS_H_

#include "sview/SView.h"
#include "m3d/base/Color.h"

#include "tinyxml/tinyxml2.h"
 
using namespace M3D;
using   tinyxml2::XMLDocument;
using   tinyxml2::XMLElement;
using   tinyxml2::XMLNode;
using   tinyxml2::XMLText;

namespace SVIEW {

/**@class Parameters
 * @brief Parameters类 提供Parameters类的声明
 *
 * Parameters类
 *
 */
class M3D_API Parameters {
public:
	static Parameters* Instance();

public:
	string GetParameter(string key);
	void SetParameter(string key, string value);


	Color GetColor(string key);
	void SetColor(string key, Color& value);

	/**
	 * @brief 设置所有参数为默认值
	 */
	void SetDefault();


	/**
	 * @brief 从xml加载设置
	 * @return
	 */
	bool LoadFromXML();


	/**
	 * @brief 将当前设置写入xml
	 * @return
	 */
	bool SaveToXML();

	static void SetMachinName(string& machName);
	static void SetUserName(string& userName);
	static void SetUserDomainName(string& domainName);
	static void SetProjectName(string& projectName);
	static void SetRemoteIP(string& remoteIP);
	static void SetRemotePort(int port);
	static string& GetRemoteIP();
	static int GetRemotePort();

	static void SetFoucusModelScaleFactor(float defaultFactor);

	static string& GetUserName0();
	static string& GetMachinName();
	static string& GetUserDomainName();
	static string& GetProjectName();

	bool GetShowVRInMainView() const { return m_showVRInMainView; }
	void SetShowVRInMainView(bool val) { m_showVRInMainView = val; }


private:
	Parameters();
	virtual ~Parameters();
	/**
	 * 从xml doc 获取一条配置信息
	 * @param doc
	 * @param paramName
	 * @param outValue
	 * @return
	 */
	bool GetParamValueFromXML(XMLDocument* doc, const char* paramName,
			char** outValue);

	/**
	 * @brief 设置一条配置信息到xml doc
	 * @param doc
	 * @param paramName
	 * @param value
	 * @return
	 */
	bool SetParamValueToXML(XMLDocument* doc, const char* paramName,
			const char* value);
public:
	static const string TRUE_VALUE;//!<真
	static const string FALSE_VALUE;//!<假

	static const char* paramNameArray[];

	static const string ITEM_ISOPENSCALE;
    static const string ITEM_ISCONROTATE;//!<持续旋转
    static const string ITEM_ISUSE_ANIMATION_CAMERA;//!<播放动画时，使用场景漫游
    static const string ITEM_ISHEIHGPERFORMANCE;//!<使用高性能显示
    static const string ITEM_ISUSELOD;//!<使用LOD
    static const string ITEM_ISUSEVBO;//!<使用VBO
    static const string ITEM_ISONLY_SHOW_EDGE_WHEN_MOVING;//!<移动时仅显示边框线
    static const string ITEM_LITTLE_MODEL_CULLER_STYLE;//!<小件剔除方式, 0=按模型大小,1=按屏幕大小
    static const string ITEM_LITTLE_MODEL_CULLER_SIZE;//!<小件剔除大小 这种方式传值 "20"
    static const string ITEM_DEFAULT_ZOOM_FACTOR;//!<默认缩放比例
    
    static const string ITEM_GLES_VERSION;//!<opengles版本

    static const string ITEM_M3D_VERSION;//!<m3d版本

    static const string ITEM_APP_DEFAULT_WORK_PATH; //!<app的默认工作路径

    static const string ITEM_SKYBOX_PATH; //!<天空盒工作路径

    static const string ITEM_BIG_MODEL_OPTIMIZE; //!<模型优化调整系数

    static const string ITEM_SHOW_TRANSPARENT;//!<透明

    static const string ITEM_SHOW_ROTATECENTER;//!<是否显示旋转中心点

    static const string ITEM_OPEN_FIRSTPERSION_CAMERA;//!<第一人称相机

    static const string ITEM_USE_CATIAMODE;//!<使用catia方式进行节点解析

    static const string ITEM_LIGTHING_MODE;//!<使用catia方式进行节点解析
    
    static const string ITEM_SCREEN_WIDTH;//!<屏幕宽
    static const string ITEM_SCREEN_HEIGHT;//!<屏幕高
    static const string ITEM_SCREEN_PPI;//!<屏幕像素密度倒数

    static const string ITEM_TEXTNOTE_STYLE;//!<文本批注风格

    static const string ITEM_MEASURE_UNIT_STYLE;//!<单位格式

    static const string ITEM_INTERNATIONAL_LANGUAGE;//!<国际化

    static const string ITEM_IS_MODELVIEW;

    static const string ITEM_SHOW_SECTION;//!<是否显示剖切面的辅助面

    static const string ITEM_CUBE_MAP;//!<是否开启立方体环境映射

    static const string ITEM_AXIS_POS;//!<坐标轴位置
    static const string ITEM_AXIS_IMAGE;//!<坐标轴图标
    static const string ITEM_CLIPPLANE_MODE;//!<剖切面模式

	static const string ITEM_PERSPECTIVE_STYLE;//!<视图模式

	static const string ITEM_VIEW_RECORD_MODEL;//!<自定义视图纪录模型
	static const string ITEM_VIEW_RECORD_CAMERA;//!<自定义视图纪录摄像机
	static const string ITEM_VIEW_RECORD_NOTE;//!<自定义视图纪录批注
    static const string ITEM_DEFAULT_SUFFIX;//!<默认文件后缀
    static const string ITEM_USE_COMPUTERPROPERTY;//!<使用计算属性

    static const string ITEM_USE_PROTOTYPE_PROPERTY;//!<使用原型的属性
    static const string ITEM_USE_INSTANCE_PROPERTY;//!<使用实例的属性

    static const string ITEM_IS_SHOW_SPACEPOINT;

    static const string ITEM_IS_USER_DATA_BOM; //使用自定义属性BOM
	
	static const string ITEM_IS_RECALCULATE_NORMAL;
	static const string ITEM_IS_CHECKERRORPOIN;
	static const string ITEM_IS_SHOW_BINOCULAR_PICTURE;
	static const string ITEAM_VR_VIEW_MODE;

	static const string ITEM_SELECTED_STYLE;
	static const string ITEM_GAMMA;
	static const string ITEM_TONE_MAPPING_EXPOSURE;

	static const string ITEM_USE_SSAO;
	static const string ITEM_USE_GROUND_GRID;

	static const string ITEM_SSAO_RADIUS;
	static const string ITEM_SSAO_AOCLAMP;
	static const string ITEM_SSAO_LUMINFLUENCE;
	static const string ITEM_SSAO_CONTRAST;
	static const string ITEM_SSAO_BIAS;
	static const string ITEM_SSAO_DISPLACE;
	static const string ITEM_SSAO_AEAR;

	static const string ITEM_SHADOW_MAP_ENABLE;

	//----设计器参数开始
	static const string DESIGNER_ANNOTATION_USER;
	static const string DESIGNER_ANNOTATION_DEPARTMENT;
	static const string DESIGNER_ANNOTATION_CHARACTER;
	static const string DESIGNER_ANNOTATION_DISPLAYCOLOR;

	string m_annotationUser;
	string m_annotationDepartment;
	string m_annotationCharacter;
	Color m_annotationDisplayColor;
	//----设计器参数结束

	bool 	m_isOpenMirror;		//!<是否开启镜面
    bool 	m_isOpenScale;		//!<是否开启缩放
	bool 	m_IsConRotate;		//!<持续旋转
	bool 	m_UseBackImage;		//!<是否使用背景图片
    bool    m_BackTransparent;  //!<背景是否透明
	bool 	m_IsShowTrilateralEdge;	//!<是否显示三角面片边线
	bool 	m_IsOnlyShowTrilateralEdge;	//!<是否只显示三角形边框
	bool 	m_IsShowEdgeLine;	//!<是否显示边界线
	bool 	m_IsOnlyShowEdgeLine;	//!<是否只显示边界线
	bool 	m_IsShowBox;		//!<是否显示包围盒
	bool 	m_IsShowTransparent;//!<是否半透明显示
	Color 	m_BoxColor;			//!<包围盒颜色
	bool 	m_IsShowAxis;		//!<是否显示轴
	bool 	m_IsShowFPS;		//!<是否显示FPS
	Color 	m_DefaultNoteColor;	//!<默认Note颜色
	Color 	m_SelectedColor;	//!<选中颜色
	float 	m_Alpha;			//!<默认alpha值
	bool 	m_IsShowPMIs;		//!<是否显示pmi
	bool 	m_IsNoteFixSize;	//!<note是否固定大小
	bool 	m_IsNoteFrontDisplay;	//!<note是否前端显示
	bool 	m_IsMulSelect;		//!<是否允许多选
//	bool 	IsShowAxisText;		//!<是否显示坐标轴文字
	bool 	m_IsUseVBO;			//!<是否用VBO显示技术
	bool 	m_IsModelLevel;		//!<小件剔除的级别：模型
	bool 	m_IsMeshLevel;		//!<小件剔除的级别：mesh
	int 	m_RemoveSize;		//!<小件剔除的大小
	float   m_NeverSeeScreenPixelSize;//!<永远不可见模型大小
	int 	m_RemoveMode;		//!<小件剔除模式：0=按模型大小,1=按屏幕大小
	bool 	m_IsUseBoxMoving;	//!<移动时显示包围盒
	bool 	m_IsTouchRotation;	//!<是否允许触摸旋转
	string 	m_ResPath;			//!<？
	bool 	m_IsUseLOD;			//!<是否使用LOD读取和显示
	bool 	m_IsUseIndexMode;	//!<是否使用索引方式绘制
	int 	m_OpenGLESVersion;	//!<OpenGLES使用的版本号
	bool 	m_UseSpacePoint;	//!<读取空间点
	int     m_MergeLevel;//!<面合并级别
 
	bool    m_IsHighPerformanceView; //!<是否使用高性能模式
    bool    m_IsUseAnimationCamera;//!<动画中使用摄像机漫游
    bool    m_IsUseCatiaMode; //!<使用catia方式进行节点内容解析

	bool    m_IsCheckErrorPoint; //!<进行坐标特别大点检查

	bool m_IsUseGeoData;//!<解析几何坐标数据

	bool m_IsUsePmiData;//!<
	bool m_IsUseModelViewData;//!<

    string m_appWorkPath; //!<app默认工作路径
    string m_measureImageTempPath;//用来临时存储测量图片的路径
    float   m_DefaultZoomFactor;   //!<默认缩放比例

    bool   m_OptimizeBigModel; //!<大模型显示校正

    int   m_LightingMode; //!<光照模式 0正常 ，1人像  ，2其他,3高光模式
    
    int	m_textNoteStyle; //!<0 不显示 1 显示作者

    int m_measureUnitStyle;//!<测量单位格式  不显示是0 毫米 1 厘米 2 英寸 3 英尺4

    bool m_showSection;//!<是否显示示意面

    int m_internationalLanguage;//!<国际化 0表示中文 1表示英文

    string m_m3dVersion;//!<m3d版本号
    
    string m_defaultSuffix;//!<文件后缀

    int m_screenWidth; //!<屏幕宽
    int m_screenHeight;//!<屏幕高
    
    int m_screenPPI;//!<屏幕像素密度倒数

    bool m_useCubeMapping;//!<使用立方体环境映射

    bool m_isModelView;//!<是否启用默认视图

    bool m_isAutoSetRotateCenter;//!<是否开启自动选择选中中心

    bool m_isShowRotateCenter;//!<是否显示旋转中心点

    bool m_openFirstPersionCamera;//!<是否开启第一人称相机

    int m_maxFPS;//!<fps最大值

    bool firstDraw;

    bool m_cubeMode;

	int m_msaaNum;//!<多重采样比率 0 1 2 4 8 当等于0的时候不使用多重采样

	//bool m_simplityMode; //!<极简模式，使用LFace和LSceneNode构建，对测量等其他功能影响较大

	int m_bufferType;//!<顶点缓存位置 1 = GPU 2 = DISK 0 = NO

	bool m_retainName; //!<保留名称

    int m_axisPos; //!<坐标轴位置
    bool m_axisImage;//!<坐标轴图标
	int m_perspectiveStyle; //!<视图样式
	
	int m_clipPlaneMode;//!<0表示正常剖切 ,1为不剖切，2为只显示剖切线

	bool m_viewRecordModel;//!< 视图纪录模型状态
	bool m_viewRecordCamera;//!< 视图纪录摄像机状态
	bool m_viewRecordNotes;//!< 视图纪录批注状态
	bool m_readEdge;//!< 是否读取边界线

    bool m_useComputerProperty;//!< 使用计算属性
    bool m_usePrototypeProperty;//!< 使用原型的属性
    bool m_useInstanceProperty;//!< 使用实例的属性

    bool m_isShowSpacePoint;

    bool m_isUseUserDataBom; //是否使用用户自定义BOM结构

	bool m_useStereo;//是否使用立体眼镜模式

	float m_pupillaryDistanceFactor;

	//bool m_isUseFullPath;

	///获取最小移动的距离，内部根据像素密度判断
	int GetLimitDistance();

	//获取多语言
	int GetLanguage();

	bool isRecalculateNormal;

	bool m_vrViewMode;//漫游时选择0；观察模型时选择1；

	static string m_machinName;
	static string m_userName;
	static string m_userDomainName;
	static string m_projectName;

	static string m_remoteIP;
	static int m_remotePort;

	static float m_foucsScalFactor;
	static bool m_useSimplePath;

	static bool m_useBodyObject;
	bool GetLoadExternInfo() const;
	void SetLoadExternInfo(bool val);
	int m_defaultViewType;
	Vector3 m_upDirectionValue;

	bool m_isShowBinocularPicture;//是否显示VR模式中的双眼画面

	bool m_isShowCutLine;//是否显示剖切线

	int m_selectedStyle;//选择样式 1轮廓 2颜色

	Color m_outlineColor;

	bool m_shadowMapEnabled;
	int m_shadowMapType;//1 正常 2 PCF 3 soft
	bool m_doubleSided;
	float m_gammaFactor;
	float m_toneMappingExposure;
	bool m_gammaInput;
	bool m_gammaOutput;

	bool m_useSSAO;//使用环境光遮蔽
	bool m_useGroundGrid;

	//环境光遮蔽参数
	float g_LumInfluence;
	float g_SSAOContrast;
	float g_SSAO_Bias;
	float g_Displace;
	float g_Aear;
	float g_aoClamp;
	float g_radius;

	bool m_showVRInMainView;//在主窗口显示VR

private:
	static const string xmlPath;
	static Parameters* instance;
protected:
	bool m_loadExternInfo;
};

}

#endif /* M3D_PAMAMETERS_H_ */
