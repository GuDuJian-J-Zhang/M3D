//*************************************************************************/
///     COPYRIGHT NOTICE 
///     Copyright (c)       （版权声明）
///     All rights reserved. 
/// 
///     @file     （本文件的文件名eg：Test.h）
///     @brief （本文件实现的功能的简述）
/// 
///    （本文件实现的功能的详述）
/// 
///     @version        
///     @author      
///     @date                 （文件创建日期，eg：2009年7月15日）
/// 
/// 
///     修订说明：
//*************************************************************************/

#include "Parameters.h"
#include "m3d/utils/StringHelper.h"
#include <sstream>
#include "Utility.h"
#include <stdio.h>
#include "../extern/PerspectiveData.h"
#include "m3d/utils/md5.h"

using namespace M3D;

using std::stringstream;

namespace SVIEW {

const string Parameters::TRUE_VALUE = "true";
const string Parameters::FALSE_VALUE = "false";
const string Parameters::xmlPath = "mnt/sdcard/sview/setting.xml";

const string Parameters::ITEM_ISOPENSCALE ="isOpenScale";
const string Parameters::ITEM_ISCONROTATE = "isConRotate";
const string Parameters::ITEM_ISUSE_ANIMATION_CAMERA = "isUseAnimationCamera";
const string Parameters::ITEM_ISHEIHGPERFORMANCE = "highPerformanceView";
const string Parameters::ITEM_ISUSELOD = "useLOD";///使用LOD
const string Parameters::ITEM_ISUSEVBO = "useVBO";///使用VBO
const string Parameters::ITEM_ISONLY_SHOW_EDGE_WHEN_MOVING = "useBoxMoving";///移动时仅显示边框线
const string Parameters::ITEM_LITTLE_MODEL_CULLER_STYLE = "removeMode";///小件剔除方式, 0=按模型大小,1=按屏幕大小
const string Parameters::ITEM_LITTLE_MODEL_CULLER_SIZE = "removeSize";///小件剔除大小 这种方式传值 "20"
const string Parameters::ITEM_DEFAULT_ZOOM_FACTOR = "defaultZoomFactor";///默认缩放比例
const string Parameters::ITEM_GLES_VERSION = "openGLESVersion";
const string Parameters::ITEM_M3D_VERSION = "m3DVersion";
const string Parameters::ITEM_SHOW_TRANSPARENT = "showTransparent";///

const string Parameters::ITEM_APP_DEFAULT_WORK_PATH = "app_default_work_path";///app的默认工作路径

const string Parameters::ITEM_BIG_MODEL_OPTIMIZE = "bigModelRenderAdjust";///app的默认工作路径

const string Parameters::ITEM_USE_CATIAMODE = "useCatiaMode";//使用catia方式进行节点解析

const string Parameters::ITEM_LIGTHING_MODE = "lightingMode";//使用catia方式进行节点解析

const string Parameters::ITEM_SCREEN_WIDTH = "screenWidth";//

const string Parameters::ITEM_SCREEN_HEIGHT = "screenHeight";//

const string Parameters::ITEM_SCREEN_PPI = "screenPPI";//
const string Parameters::ITEM_TEXTNOTE_STYLE = "textNoteStyle";///
const string Parameters::ITEM_MEASURE_UNIT_STYLE = "measureUnitStyle";
const string Parameters::ITEM_INTERNATIONAL_LANGUAGE = "internationalLanguage";

const string Parameters::ITEM_IS_MODELVIEW = "isModelView";

const string Parameters::ITEM_SHOW_SECTION = "showSection";
const string Parameters::ITEM_SHOW_ROTATECENTER = "isShowRotateCenter";
const string Parameters::ITEM_OPEN_FIRSTPERSION_CAMERA = "openFirstPersionCamera";
const string Parameters::ITEM_CUBE_MAP = "cubeMapMode";
const string Parameters::ITEM_AXIS_POS = "axisPosition";
const string Parameters::ITEM_AXIS_IMAGE = "axisImage";
const string Parameters::ITEM_CLIPPLANE_MODE = "clipPlaneMode";
const string Parameters::ITEM_PERSPECTIVE_STYLE = "front";
const string Parameters::ITEM_IS_SHOW_SPACEPOINT = "isShowSpacePoint";

const string Parameters::ITEM_VIEW_RECORD_MODEL = "viewRecordModel";
const string Parameters::ITEM_VIEW_RECORD_CAMERA = "viewRecordCamera";
const string Parameters::ITEM_VIEW_RECORD_NOTE = "viewRecordNote";

const string Parameters::ITEM_DEFAULT_SUFFIX = "defaultSuffix";

const string Parameters::ITEM_USE_COMPUTERPROPERTY = "useComputerCalculate";

const string Parameters::ITEM_USE_PROTOTYPE_PROPERTY = "usePrototypeProperty";
const string Parameters::ITEM_USE_INSTANCE_PROPERTY = "useInstanceProperty";

const string Parameters::ITEM_IS_USER_DATA_BOM = "isUserDataBOM";

const string Parameters::ITEM_IS_RECALCULATE_NORMAL = "isRecalculateNormal";
const string Parameters::ITEM_IS_CHECKERRORPOIN = "isCheckErrorPoint";

const string Parameters::ITEM_IS_SHOW_BINOCULAR_PICTURE = "isShowBinocularPicture";
const string Parameters::ITEAM_VR_VIEW_MODE = "vrViewMode";

const string Parameters::ITEM_SELECTED_STYLE = "selectedStyle";

 const string Parameters::ITEM_GAMMA = "gamma";
 const string Parameters::ITEM_TONE_MAPPING_EXPOSURE = "toneMappingExposure";

const string Parameters::ITEM_USE_SSAO = "useSSAO";
const string Parameters::ITEM_USE_GROUND_GRID = "useGroundGrid";
const string Parameters::ITEM_SSAO_RADIUS = "ssaoRadius";
const string Parameters::ITEM_SSAO_AOCLAMP = "ssaoAoClamp";
const string Parameters::ITEM_SSAO_LUMINFLUENCE = "ssaoLuminfluence";
const string Parameters::ITEM_SSAO_CONTRAST = "ssaoContrast";
const string Parameters::ITEM_SSAO_BIAS="ssaoBias";
const string Parameters::ITEM_SSAO_DISPLACE="ssaoDisplace";
const string Parameters::ITEM_SSAO_AEAR = "ssaoAear";

const string Parameters::ITEM_SHADOW_MAP_ENABLE = "shadowMapEnable";
const string Parameters::ITEM_FILE_KEYFILE = "KeyFile";
const string Parameters::ITEM_FILE_PASSWORD = "Password";

const string Parameters::DESIGNER_ANNOTATION_USER = "annotationUser";
const string Parameters::DESIGNER_ANNOTATION_DEPARTMENT = "annotationDepartment";
const string Parameters::DESIGNER_ANNOTATION_CHARACTER = "annotationCharacter";
const string Parameters::DESIGNER_ANNOTATION_DISPLAYCOLOR = "annotationDisplayColor";

string Parameters::m_machinName = "";
string Parameters::m_userName = "";
string Parameters::m_userDomainName = "";
string Parameters:: m_projectName ="";
string Parameters::m_remoteIP ="";
int Parameters::m_remotePort = 8080;

float Parameters::m_foucsScalFactor = 0.5;

bool Parameters::m_useSimplePath = false;

bool Parameters::m_useBodyObject = true;

bool Parameters::GetLoadExternInfo() const
{
	return m_loadExternInfo;
}

Parameters* Parameters::instance = NULL;

const char* Parameters::paramNameArray[] = { ITEM_ISCONROTATE.c_str(),
	ITEM_ISUSE_ANIMATION_CAMERA.c_str(),
	ITEM_ISHEIHGPERFORMANCE.c_str(),
	ITEM_ISUSEVBO.c_str(),
	ITEM_ISUSELOD.c_str(),
	"showFPS",
	ITEM_ISONLY_SHOW_EDGE_WHEN_MOVING.c_str(),
	ITEM_LITTLE_MODEL_CULLER_SIZE.c_str(),
	"showPMI",
	"showModelEdge",
	"showTriEdge",
	"showModelBox",
	"boxColor",
	"showAxis",
	"selectedColor",
	"alpha",
	"mulSelect",
	ITEM_LITTLE_MODEL_CULLER_STYLE.c_str(),
	ITEM_DEFAULT_ZOOM_FACTOR.c_str(),
	ITEM_SHOW_TRANSPARENT.c_str()
};

Parameters* Parameters::Instance()
{
	if (Parameters::instance == NULL)
	{
		Parameters::instance = new Parameters();
	}
	return Parameters::instance;
}

Parameters::Parameters() {
	SetDefault();
	//	if (!loadFromXML()) {
	//		saveToXML();
	//	}
}

Parameters::~Parameters() {
	SaveToXML();
}

int Parameters::GetLimitDistance()
{
	if (this->m_screenPPI>100)
	{
		return this->m_screenPPI / 20;
	}
	return 4;
}


int Parameters::GetLanguage()
{
	return m_internationalLanguage;
}

string Parameters::GetParameter(string key) {
	string value;
	if (key == "showFPS")
		value = m_IsShowFPS ? TRUE_VALUE : FALSE_VALUE;
	else if (key == ITEM_ISUSEVBO)
		value = m_IsUseVBO ? TRUE_VALUE : FALSE_VALUE;
	else if (key == ITEM_ISUSELOD)
		value = m_IsUseLOD ? TRUE_VALUE : FALSE_VALUE;
	else if (key == "showPMI"){
		value = m_IsShowPMIs ? TRUE_VALUE : FALSE_VALUE;
	}
	else if (key == "showModelEdge")
		value = m_IsShowEdgeLine ? TRUE_VALUE : FALSE_VALUE;
	else if (key == "showTriEdge"){
		value = m_IsShowTrilateralEdge ? TRUE_VALUE : FALSE_VALUE;
	}
	else if (key == "showModelBox"){
		value = m_IsShowBox ? TRUE_VALUE : FALSE_VALUE;
	}
	else if(key ==  ITEM_SHOW_TRANSPARENT)
	   {
	        value = m_IsShowTransparent ? TRUE_VALUE : FALSE_VALUE;
	    }
	else if (key == "showAxis")
		value = m_IsShowAxis ? TRUE_VALUE : FALSE_VALUE;
	else if (key == ITEM_ISONLY_SHOW_EDGE_WHEN_MOVING)
		value = m_IsUseBoxMoving ? TRUE_VALUE : FALSE_VALUE;
	else if (key == ITEM_LITTLE_MODEL_CULLER_STYLE)
	{
		value = m_RemoveMode == 0 ? "0" : "1";
	}
	else if (key == ITEM_GLES_VERSION)
	{
		if (this->m_OpenGLESVersion == 1)
		{
			value = "1";
		}
		else if (this->m_OpenGLESVersion == 2)
		{
			value = "2";
		}
	}
	else if (key == ITEM_LITTLE_MODEL_CULLER_SIZE) {
		stringstream ss;
		ss << m_RemoveSize;
		ss >> value;
	} else if (key == "fixTextSize") {
		value = m_IsNoteFixSize ? TRUE_VALUE : FALSE_VALUE;
	} else if (key == "fixTextFront") {
		value = m_IsNoteFrontDisplay ? TRUE_VALUE : FALSE_VALUE;
	} else if (key == ITEM_ISCONROTATE) {
		value = m_IsConRotate ? TRUE_VALUE : FALSE_VALUE;
	}else if(key == ITEM_ISOPENSCALE){
		value = m_isOpenScale?TRUE_VALUE:FALSE_VALUE;
	}
	else if (key == "resPath")
	{
		value =m_ResPath;
	}else if (key == ITEM_ISUSE_ANIMATION_CAMERA)
    {
        value = m_IsUseAnimationCamera ? TRUE_VALUE : FALSE_VALUE;
    }else if(key ==  ITEM_ISHEIHGPERFORMANCE)
    {
        value = m_IsHighPerformanceView ? TRUE_VALUE : FALSE_VALUE;
 
    }
    else if(key ==  "mulSelect")
    {
        value = m_IsMulSelect ? TRUE_VALUE : FALSE_VALUE;
    }
    else if(key ==  ITEM_APP_DEFAULT_WORK_PATH)
    {
    	value = this->m_appWorkPath;
    }else if(key ==  ITEM_USE_CATIAMODE)
    {
    	value =  this->m_IsUseCatiaMode ? TRUE_VALUE : FALSE_VALUE;
    }else if(key ==  ITEM_BIG_MODEL_OPTIMIZE)
    {
    	value =  this->m_OptimizeBigModel ? TRUE_VALUE : FALSE_VALUE;
//    	value =  M3DTOOLS::floatToString(this->m_modelOptimizeZoomFactor);
    }
    else if(key ==  ITEM_LIGTHING_MODE)
    {
        	value =  StringHelper::IntToString(this->m_LightingMode);
    }else if(key ==  ITEM_SCREEN_WIDTH)
    {
        value =  StringHelper::IntToString(this->m_screenWidth);
    }else if(key ==  ITEM_SCREEN_HEIGHT)
    {
        value =  StringHelper::IntToString(this->m_screenHeight);
    } else if(key ==  ITEM_SCREEN_PPI)
    {
        value =  StringHelper::floatToString(this->m_screenPPI);
    }
	else if (key == ITEM_TEXTNOTE_STYLE)
	{
		value = StringHelper::IntToString(this->m_textNoteStyle);
	}
	else if(key == ITEM_MEASURE_UNIT_STYLE)
	{
		value = StringHelper::IntToString(this->m_measureUnitStyle);
	}
	else if(key == ITEM_INTERNATIONAL_LANGUAGE)
	{
		value = StringHelper::IntToString(this->m_internationalLanguage);
	}

	else if(key == ITEM_IS_MODELVIEW)
	{
		value = this->m_isModelView?TRUE_VALUE:FALSE_VALUE;
	}

	else if(key == ITEM_SHOW_SECTION)
	{
		value = this->m_showSection?TRUE_VALUE:FALSE_VALUE;
	}
	else if(key == ITEM_M3D_VERSION)
	{
		value = this->m_m3dVersion;
	}
	else if(key == ITEM_SHOW_ROTATECENTER)
	{
		value = this->m_isShowRotateCenter ? TRUE_VALUE:FALSE_VALUE;
	}
	else if(key == ITEM_OPEN_FIRSTPERSION_CAMERA)
	{
		value = this->m_openFirstPersionCamera?TRUE_VALUE:FALSE_VALUE;
	}
	else if(key == ITEM_CUBE_MAP)
	{
		value = this->m_cubeMode?TRUE_VALUE:FALSE_VALUE;
	}else if(key == ITEM_AXIS_POS)
	{
		value =StringHelper::IntToString(this->m_axisPos);
	}
    else if(key == ITEM_AXIS_IMAGE)
    {
        value = this->m_axisImage?TRUE_VALUE:FALSE_VALUE;
    }
    else if(key ==  ITEM_CLIPPLANE_MODE)
    {
        	value =  StringHelper::IntToString(this->m_clipPlaneMode);
    }else if(key == ITEM_PERSPECTIVE_STYLE)
	{
		value = StringHelper::IntToString(this->m_perspectiveStyle);
	}else if(key == ITEM_VIEW_RECORD_MODEL)
	{
		value = this->m_viewRecordModel;
	}else if(key == ITEM_VIEW_RECORD_CAMERA)
	{
		value = this->m_viewRecordCamera;
	}else if(key == ITEM_VIEW_RECORD_NOTE)
	{
		value = this->m_viewRecordNotes;
	}else if (key == ITEM_DEFAULT_SUFFIX)
    {
        value = this->m_defaultSuffix;
    }else if (key == ITEM_USE_COMPUTERPROPERTY)
    {
        value = this->m_useComputerProperty;
    }else if (key == ITEM_USE_PROTOTYPE_PROPERTY)
    {
        value = this->m_usePrototypeProperty;
    }else if (key == ITEM_USE_INSTANCE_PROPERTY)
    {
        value = this->m_useInstanceProperty;
    }

	else if(key == ITEM_IS_SHOW_SPACEPOINT)
	{
		value = this->m_isShowSpacePoint?TRUE_VALUE:FALSE_VALUE;
	}
	else if(key == ITEM_IS_USER_DATA_BOM)
	{
		value = this->m_isUseUserDataBom? TRUE_VALUE : FALSE_VALUE;;
	}
	else if (key == ITEM_IS_RECALCULATE_NORMAL)
	{
		value = this->isRecalculateNormal ? TRUE_VALUE : FALSE_VALUE;;
	} 
	  else if (key == ITEM_IS_CHECKERRORPOIN)
    {
        value = this->m_IsCheckErrorPoint ? TRUE_VALUE : FALSE_VALUE;;
    }
	  else if (key == ITEM_IS_SHOW_BINOCULAR_PICTURE)
	  {
		  value = this->m_isShowBinocularPicture ? TRUE_VALUE : FALSE_VALUE;
	  }
	  else if (key == ITEAM_VR_VIEW_MODE)
	  {
		  value = StringHelper::IntToString(this->m_vrViewMode);
	  }	else if (key == "IsShowCutLine")
	{
	 value = this->m_isShowCutLine ? TRUE_VALUE : FALSE_VALUE;
    } else if (key == "backTransparent")
    {
        value = this->m_BackTransparent ? TRUE_VALUE : FALSE_VALUE;
    }
	  else if (key == ITEM_SELECTED_STYLE)
	  {
		  value = StringHelper::IntToString(this->m_selectedStyle);
	  }
	  else if (key == ITEM_GAMMA)
	  {
		  value = StringHelper::floatToString(this->m_gammaFactor);
	  }
	  else if (key == ITEM_TONE_MAPPING_EXPOSURE)
	  {
		  value = StringHelper::floatToString(this->m_toneMappingExposure);
	  }
	  else if (key == ITEM_USE_GROUND_GRID)
	  {
		  value = this->m_useGroundGrid ? TRUE_VALUE : FALSE_VALUE;
	  }
	  else if (key == ITEM_USE_SSAO)
	  {
		  value = this->m_useSSAO ? TRUE_VALUE : FALSE_VALUE;
	  }
	  else if (key == ITEM_SSAO_RADIUS)
	  {
		  value = StringHelper::floatToString(this->g_radius);
	  }
	  else if (key == ITEM_SSAO_AOCLAMP)
	  {
		  value = StringHelper::floatToString(this->g_aoClamp);
	  }
	  else if (key == ITEM_SSAO_BIAS)
	  {
		  value = StringHelper::floatToString(this->g_SSAO_Bias);
	  }
	  else if (key == ITEM_SSAO_CONTRAST)
	  {
		  value = StringHelper::floatToString(this->g_SSAOContrast);
	  }
	  else if (key == ITEM_SSAO_DISPLACE)
	  {
		  value = StringHelper::floatToString(this->g_Displace);
	  }
	  else if (key == ITEM_SSAO_LUMINFLUENCE)
	  {
		  value = StringHelper::floatToString(this->g_LumInfluence);
	  }
	  else if (key == ITEM_SSAO_AEAR)
	  {
		  value = StringHelper::floatToString(this->g_Aear);
	  }
	  else if (key == DESIGNER_ANNOTATION_USER)
	  {
		  value = this->m_annotationUser;
	  }
	  else if (key == DESIGNER_ANNOTATION_DEPARTMENT)
	  {
		  value = this->m_annotationDepartment;
	  }
	  else if (key == DESIGNER_ANNOTATION_CHARACTER)
	  {
		  value = this->m_annotationCharacter;
	  }
	  else if (key == ITEM_SHADOW_MAP_ENABLE)
	  {
		  value = this->m_shadowMapEnabled ? TRUE_VALUE : FALSE_VALUE;
	  }
	  else if (key == ITEM_FILE_KEYFILE)
	  {
		  value = this->m_strKeyFile;
	  }
	  else if (key == "CullFace") {
		  value = StringHelper::IntToString(m_iCullFace);
	  }
	//LOGI("getParameter: key:%s,value:%s",key.c_str(),value.c_str());
	return value;
}

void Parameters::SetParameter(string key, string value) {
	if (key == "showFPS")
		m_IsShowFPS = value == TRUE_VALUE;
	else if (key == ITEM_ISUSEVBO)
		m_IsUseVBO = value == TRUE_VALUE;
	else if (key == ITEM_ISUSELOD)
		m_IsUseLOD = value == TRUE_VALUE;
	else if (key == "showPMI")
		m_IsShowPMIs = value == TRUE_VALUE;
	else if (key == "showModelEdge")
		m_IsShowEdgeLine = value ==TRUE_VALUE ;
	else if (key == "showTriEdge")
		m_IsShowTrilateralEdge= value ==  TRUE_VALUE ;
	else if (key == "showModelBox")
		 m_IsShowBox = value ==TRUE_VALUE;
	else if(key ==  ITEM_SHOW_TRANSPARENT)
	    {
			m_IsShowTransparent = (value == TRUE_VALUE);
	    }
	else if (key == "showAxis")
		m_IsShowAxis = value == TRUE_VALUE;
	else if (key == ITEM_LITTLE_MODEL_CULLER_STYLE) {
		if (value == "0") {
			m_RemoveMode = 0;
		} else if (value == "1") {
			m_RemoveMode = 1;
		}
	} else if (key == ITEM_LITTLE_MODEL_CULLER_SIZE){
		m_RemoveSize = atoi(value.c_str());

	}else if (key == ITEM_GLES_VERSION){
		//LOGI("opengl version value = %s",value.c_str());
		if (value == "1")
		{
			this->m_OpenGLESVersion = 1;
		}
		else if (value == "2")
		{
			this->m_OpenGLESVersion = 2;
		}
	}
	else if (key == ITEM_ISONLY_SHOW_EDGE_WHEN_MOVING)
		m_IsUseBoxMoving = value == TRUE_VALUE;
	else if (key == "useBackImage")
		m_UseBackImage = value == TRUE_VALUE;
	else if (key == "boxColor") {
		//BoxColor.SetColor(r,g,b,a);
	} else if (key == "selectedColor") {
		//UseBackImage = value == TRUEVALUE;
    }else if (key == "backTransparent") {
        m_BackTransparent = value == TRUE_VALUE;
    } else if (key == "alpha") {
		//Alpha = value == TRUEVALUE;
	} else if (key == "mulSelect")
		m_IsMulSelect = value == TRUE_VALUE;
	else if (key == "fixTextSize") {
		m_IsNoteFixSize = value == TRUE_VALUE;
	} else if (key == "fixTextFront") {
		m_IsNoteFrontDisplay = value == TRUE_VALUE;
	} else if (key == ITEM_ISCONROTATE) {
		m_IsConRotate = (value == TRUE_VALUE);
	} else if(key == ITEM_ISOPENSCALE){
		m_isOpenScale= (value ==TRUE_VALUE);
	}
	else if (key == "resPath")
	{
		m_ResPath = value;
	}else if (key == ITEM_ISUSE_ANIMATION_CAMERA)
    {
        m_IsUseAnimationCamera = (value  == TRUE_VALUE);
    }else if (key == ITEM_ISHEIHGPERFORMANCE)
	{
		m_IsHighPerformanceView = value == TRUE_VALUE ;
	}else if(key == ITEM_DEFAULT_ZOOM_FACTOR){
		this->m_DefaultZoomFactor = atof(value.c_str());
	}else if(key == ITEM_APP_DEFAULT_WORK_PATH){
		this->m_appWorkPath = value;
	}else if(key ==  ITEM_USE_CATIAMODE)
    {
		this->m_IsUseCatiaMode= value ==  TRUE_VALUE ;
    }else if(key ==  ITEM_BIG_MODEL_OPTIMIZE)
    {
    	m_OptimizeBigModel = value == TRUE_VALUE;
    }else if(key ==  ITEM_LIGTHING_MODE)
    {
    	this->m_LightingMode = StringHelper::StringToInt(value);
    }else if(key ==  ITEM_SCREEN_WIDTH)
    {
        this->m_screenWidth = StringHelper::StringToInt(value);
    }else if(key ==  ITEM_SCREEN_HEIGHT)
    {
        this->m_screenHeight = StringHelper::StringToInt(value);
    } else if(key ==  ITEM_SCREEN_PPI)
    {
        this->m_screenPPI =  StringHelper::StringToFloat(value);
    }
	else if (key == ITEM_TEXTNOTE_STYLE)
	{
		this->m_textNoteStyle = StringHelper::StringToInt(value);
	}
	else if(key == ITEM_MEASURE_UNIT_STYLE)
	{
		this->m_measureUnitStyle = StringHelper::StringToInt(value);
	}
	else if(key == ITEM_INTERNATIONAL_LANGUAGE)
	{
		this->m_internationalLanguage = StringHelper::StringToInt(value);
	}
	else if(key == ITEM_IS_MODELVIEW)
	{
		this->m_isModelView = value==TRUE_VALUE;
	}
	else if(key == ITEM_SHOW_SECTION)
	{
		this->m_showSection = value==TRUE_VALUE;
	}
	else if(key == ITEM_M3D_VERSION)
	{
		this->m_m3dVersion = value;
	}
	else if(key == ITEM_SHOW_ROTATECENTER)
	{
		this->m_isShowRotateCenter = value ==TRUE_VALUE;
	}
	else if(key == ITEM_OPEN_FIRSTPERSION_CAMERA)
	{
		this->m_openFirstPersionCamera = value == TRUE_VALUE;
	}
	else if(key == ITEM_CUBE_MAP)
	{
		this->m_cubeMode = value == TRUE_VALUE;
	}else if(key == ITEM_AXIS_POS)
	{
		this->m_axisPos =  StringHelper::StringToInt(value);
    }else if(key == ITEM_AXIS_IMAGE)
    {
        this->m_axisImage = value == TRUE_VALUE;
    }
    else if(key ==  ITEM_CLIPPLANE_MODE)
    {
    	this->m_clipPlaneMode =  StringHelper::StringToInt(value);
    }	else if (key == ITEM_PERSPECTIVE_STYLE)
	{
		this->m_perspectiveStyle =  StringHelper::StringToInt(value);
	}else if(key == ITEM_VIEW_RECORD_MODEL)
	{
		 this->m_viewRecordModel = value == TRUE_VALUE;
	}else if(key == ITEM_VIEW_RECORD_CAMERA)
	{
		this->m_viewRecordCamera = value == TRUE_VALUE;
	}else if(key == ITEM_VIEW_RECORD_NOTE)
	{
		this->m_viewRecordNotes = value == TRUE_VALUE;
    }else if (key == ITEM_DEFAULT_SUFFIX)
    {
        this->m_defaultSuffix = value;
    }else if (key == ITEM_USE_COMPUTERPROPERTY)
    {
        this->m_useComputerProperty = value == TRUE_VALUE;
    }else if (key == ITEM_USE_PROTOTYPE_PROPERTY)
    {
        this->m_usePrototypeProperty = value == TRUE_VALUE;
    }else if (key == ITEM_USE_INSTANCE_PROPERTY)
    {
        this->m_useInstanceProperty = value == TRUE_VALUE;
    }
	else if(key == ITEM_IS_SHOW_SPACEPOINT)
	{
		this->m_isShowSpacePoint = value==TRUE_VALUE;
	}else if(key == ITEM_IS_USER_DATA_BOM)
	{
		this->m_isUseUserDataBom = value==TRUE_VALUE;
	}
	else if (key == ITEM_IS_RECALCULATE_NORMAL)
	{
		this->isRecalculateNormal = value == TRUE_VALUE;
	}
	else if (key == ITEM_IS_CHECKERRORPOIN)
	{
		this->m_IsCheckErrorPoint = value == TRUE_VALUE;
	}
	else if (key == ITEM_IS_SHOW_BINOCULAR_PICTURE)
	{
		this->m_isShowBinocularPicture = value == TRUE_VALUE;
	}
	else if (key == ITEAM_VR_VIEW_MODE)
	{
		this->m_vrViewMode = StringHelper::StringToInt(value);
	}
	else if (key == "IsShowCutLine")
	{
		this->m_isShowCutLine = value == TRUE_VALUE;
	}
	else if (key == ITEM_SELECTED_STYLE)
	{
		this->m_selectedStyle = StringHelper::StringToInt(value);
	}
	else if (key == ITEM_GAMMA)
	{
		this->m_gammaFactor = StringHelper::StringToFloat(value);
	}
	else if (key == ITEM_TONE_MAPPING_EXPOSURE)
	{
		this->m_toneMappingExposure = StringHelper::StringToFloat(value);
	}
	else if (key == ITEM_USE_GROUND_GRID)
	{
		this->m_useGroundGrid = value == TRUE_VALUE;
	}
	else if (key == ITEM_USE_SSAO)
	{
		this->m_useSSAO = value == TRUE_VALUE;
	}
	else if (key == ITEM_SSAO_RADIUS)
	{		
		this->g_radius = StringHelper::StringToFloat(value);
	}
	else if (key == ITEM_SSAO_AOCLAMP)
	{		
		this->g_aoClamp = StringHelper::StringToFloat(value);
	}
	else if (key == ITEM_SSAO_BIAS)
	{		
		this->g_SSAO_Bias = StringHelper::StringToFloat(value);
	}
	else if (key == ITEM_SSAO_CONTRAST)
	{		
		this->g_SSAOContrast = StringHelper::StringToFloat(value);
	}
	else if (key == ITEM_SSAO_DISPLACE)
	{		
		this->g_Displace = StringHelper::StringToFloat(value);
	}
	else if (key == ITEM_SSAO_LUMINFLUENCE)
	{		
		this->g_LumInfluence = StringHelper::StringToFloat(value);
	}
	else if (key == ITEM_SSAO_AEAR)
	{		
		this->g_Aear = StringHelper::StringToFloat(value);
	}
	else if (key == DESIGNER_ANNOTATION_USER)
	{
		this->m_annotationUser = value;
	}
	else if (key == DESIGNER_ANNOTATION_DEPARTMENT)
	{
		this->m_annotationDepartment = value;
	}
	else if (key == DESIGNER_ANNOTATION_CHARACTER)
	{
		this->m_annotationCharacter = value;
	}
	else if (key == ITEM_SHADOW_MAP_ENABLE)
	{
		this->m_shadowMapEnabled = value == TRUE_VALUE;
	}
	else if(key == ITEM_FILE_KEYFILE)
	{
		this->m_strKeyFile = value;
		//根据密钥的MD5值设置文件密码
		ifstream in(m_strKeyFile.c_str(), ios::binary);
		if (!in)
			return ;
		MD5 md5;
		std::streamsize length;
		char buffer[1024];
		while (!in.eof()) {
			in.read(buffer, 1024);
			length = in.gcount();
			if (length > 0)
				md5.update(buffer, length);
		}
		in.close();
		this->m_strPassword = md5.toString();
	}
	else if (key == "CullFace") {
		m_iCullFace = StringHelper::StringToInt(value);
	}
	//LOGI("SetParameter: key:%s,value:%s",key.c_str(),value.c_str());
}

Color Parameters::GetColor(string key)
{
	Color value;
	if (key == DESIGNER_ANNOTATION_DISPLAYCOLOR)
	{
		value = this->m_annotationDisplayColor;
	}
	return value;
}
void Parameters::SetColor(string key, Color& value)
{
	if (key == DESIGNER_ANNOTATION_DISPLAYCOLOR)
	{
		this->m_annotationDisplayColor = value;
	}
}

void Parameters::SetDefault() {
	m_isOpenMirror = false;
	m_isOpenScale = true;
	m_IsConRotate = false;
	m_UseBackImage = true;
    m_BackTransparent = false;
	m_IsShowTrilateralEdge = false;
	m_IsOnlyShowTrilateralEdge = false;
	m_IsShowBox = false;
	m_IsShowTransparent = false;
	m_BoxColor.SetColor(0.0f, 0.0f, 1.0f, 1.0f);
	m_IsShowAxis = true;
	m_DefaultNoteColor.SetColor(0.0f, 0.0f, 1.0f, 1.0f);
	m_SelectedColor.SetColor(1, 0, 0, 1.0);
	m_Alpha = 0.5f;
	m_IsShowPMIs = true;
	m_IsNoteFixSize = true;
	m_IsNoteFrontDisplay = false;
	m_IsMulSelect = false;
	m_IsUseVBO = true;
	m_IsModelLevel = true;
	m_IsMeshLevel = true;
	m_RemoveSize = 10;
	m_RemoveMode = 1;
	m_IsUseBoxMoving = false;
	m_IsShowFPS = false;
	m_IsUseLOD = true;
	m_IsShowEdgeLine = true;
	m_IsUseIndexMode = true;
	m_OpenGLESVersion = 2; /** OpenGLESVersion = 1,使用1.1版本,OpenGLESVersion = 2，使用2.0版本 */
	m_IsHighPerformanceView = true;
	m_UseSpacePoint = false;
	m_MergeLevel = 1;

	//m_simplityMode = true;
	m_bufferType = 1;
    
	m_NeverSeeScreenPixelSize = 30;

	m_retainName = true;

	m_DefaultZoomFactor = 1.5f;
	m_measureImageTempPath = "/data/temp/measure/"; ///app默认工作路径
	m_appWorkPath = "";
	m_OptimizeBigModel = false;
	m_LightingMode = 0;
    m_screenWidth =  1024;
    m_screenHeight = 768;
    m_screenPPI = 150;
    m_textNoteStyle = 0;
    m_measureUnitStyle = 0;
    m_internationalLanguage = 0;

	m_IsUseCatiaMode = false;

    m_m3dVersion = "4.0.0";

    m_showSection = true;
    m_isShowRotateCenter = false;
    m_openFirstPersionCamera = true;
    m_maxFPS = 40;    //设置允许渲染的最大帧率
	m_perspectiveStyle = 0;

    m_cubeMode = false;
    m_clipPlaneMode = 0;

	m_viewRecordModel = true;
	m_viewRecordCamera = true;
	m_viewRecordNotes = true;
    m_defaultSuffix = "svl";
	m_readEdge = true; //默认读取边界线
    m_useComputerProperty = false;

    m_usePrototypeProperty = false;
    m_useInstanceProperty = false;
    m_isShowSpacePoint = false;

    m_isUseUserDataBom = false;
	m_useStereo = false;

	m_pupillaryDistanceFactor = 0.0f;
	
	m_msaaNum = 4;
    m_useStereo = false;
    m_axisPos = 2;
    m_axisImage = false;
	isRecalculateNormal = false;
	m_vrViewMode = 0;
	m_IsCheckErrorPoint = true;
	//m_isUseFullPath = true;
	m_IsUseGeoData = true;
	m_IsUsePmiData = true;
	m_IsUseModelViewData = true;

	m_defaultViewType = PerspectiveData::ISOMETRIC;
	m_upDirectionValue = Vector3::UP;
	m_isShowBinocularPicture = false;
	m_isShowCutLine = false;

	SetLoadExternInfo(true);

	m_selectedStyle = 3;//1 outline 2 outline and color 3 color

	m_outlineColor = Color::RED;

	m_shadowMapEnabled = false;
	 m_gammaFactor = 2.2;
	 m_toneMappingExposure = 1.0;
	 m_gammaInput = false;
	 m_gammaOutput = false;
	 m_doubleSided = false;
	 m_useSSAO = false;
	 m_useGroundGrid = false;

	 g_LumInfluence = 0.7f;
	 g_SSAOContrast = 3.0f;
	 g_SSAO_Bias = 0.1f;
	 g_Displace = 0.2f;
	 g_Aear = 4.0f;
	 g_aoClamp = 0.24f;
	 g_radius = 4.0f;


	 m_annotationUser = "";
	 m_annotationDepartment = "";
	 m_annotationCharacter = "";
	 m_annotationDisplayColor.SetColor(1.0f, 0.0f, 0.0f, 1.0f);

	 m_showVRInMainView = false;
	 m_strKeyFile = "";
	 m_strPassword = "";

	 m_shadowMapType = 2;
	 m_iCullFace = 0;
}

bool Parameters::LoadFromXML() {

	LOGI("Parameters::loadFromXML");

	bool hasXmlFile = false;
	XMLDocument* doc = new XMLDocument();
	doc->LoadFile(xmlPath.c_str());
	int errorID = doc->ErrorID();
	if (!errorID) {
		hasXmlFile = true;

		char* outValue = (char*) malloc(300);
		memset(outValue, 0, 300);
//		char outValue[300] = {0};

		for (int i = 0;
				i < sizeof(paramNameArray) / sizeof(paramNameArray[0]);
				i++) {

			bool found = GetParamValueFromXML(doc, paramNameArray[i],
					(char**) &outValue);

			if (found && outValue != NULL && strcmp(outValue, "") != 0) {
				SetParameter(paramNameArray[i], outValue);
			}
			//LOGI("load param:%s='%s'", paramNameArray[i], outValue);
		}
		free(outValue);
	}
	delete doc;
	doc = 0;

	return hasXmlFile;
}

bool Parameters::SaveToXML() {
	bool sucess = false;

	XMLDocument* doc = new XMLDocument();
	doc->LoadFile(xmlPath.c_str());
	int errorID = doc->ErrorID();

	doc->ToText();

	if (errorID == tinyxml2::XML_ERROR_FILE_NOT_FOUND) {
		int lastSpliterPos = xmlPath.find_last_of('\\');
		if (lastSpliterPos == -1) {
			lastSpliterPos = xmlPath.find_last_of('/');
		}
//		LOGI("lastSpliterPos:%d",lastSpliterPos);
		if (lastSpliterPos != -1) {
			string xmlPathDir = xmlPath.substr(0, lastSpliterPos);
			CreateMulityPath(xmlPathDir.c_str());
//			LOGI("create dir end.");
		}
	} else if (errorID == tinyxml2::XML_ERROR_EMPTY_DOCUMENT) {
		remove(xmlPath.c_str());
		doc = new XMLDocument();
//		LOGI("removeFile end.");
	}

	for (int i = 0; i < sizeof(paramNameArray) / sizeof(paramNameArray[0]);
			i++) {
		string value = GetParameter(paramNameArray[i]);
		SetParamValueToXML(doc, paramNameArray[i], value.c_str());
		//LOGI("save param: %s value:%s", paramNameArray[i], value.c_str());
	}

	doc->SaveFile(xmlPath.c_str());

	errorID = doc->ErrorID();
	if (errorID == 0) {
		sucess = true;
	} else {
		LOGE("save xml file err:%d %s", errorID, doc->GetErrorStr1());
	}

	delete doc;
	doc = 0;

	return sucess;
}

void Parameters::SetMachinName(string& machName)
{
	Parameters::m_machinName = machName;
}

void Parameters::SetUserName(string& userName)
{
	Parameters::m_userName = userName;
}

void Parameters::SetUserDomainName(string& domainName)
{
	Parameters::m_userDomainName = domainName;
}

void Parameters::SetRemoteIP(string& remoteIP)
{
	Parameters::m_remoteIP = remoteIP;
}

void Parameters::SetRemotePort(int port)
{
	Parameters::m_remotePort = port;
}

string& Parameters::GetRemoteIP()
{
	return Parameters::m_remoteIP;
}

int Parameters::GetRemotePort()
{
	return Parameters::m_remotePort;
}


void Parameters::SetFoucusModelScaleFactor(float defaultFactor)
{
	  Parameters::m_foucsScalFactor = defaultFactor;
}

void Parameters::SetProjectName(string& projectName)
{
	Parameters::m_projectName = projectName;
}

string& Parameters::GetMachinName()
{
	return Parameters::m_machinName;
}

string& Parameters::GetUserName0()
{
	return Parameters::m_userName;
}

string& Parameters::GetUserDomainName()
{
	return Parameters::m_userDomainName;
}

string& Parameters::GetProjectName() {
	return Parameters::m_projectName;
}

bool Parameters::GetParamValueFromXML(XMLDocument* doc, const char* paramName,
		char** outValue) {

//	LOGI("Parameters::getParamValueFromXML name:%s",paramName);
	XMLElement* paramsRootElement = doc->FirstChildElement("params");
	XMLElement* paramElement = paramsRootElement->FirstChildElement("param");

	bool found = false;
	do {
		XMLElement* nameElement = paramElement->FirstChildElement("name");
		const char* name = nameElement->GetText();
		if (strcmp(name, paramName) == 0) {
			found = true;
//			LOGI("param: %s found",name);
			XMLElement* valueElement = paramElement->FirstChildElement("value");
			if (valueElement == NULL) {
				LOGE("valueElement not found!.");
				break;
			}
			if (valueElement->GetText() == NULL) {
				strcpy(*outValue, "");
			} else {
				strcpy(*outValue, valueElement->GetText());
			}
//			LOGI("param: %s value:%s",name,*outValue);
		} else {
			paramElement = paramElement->NextSiblingElement("param");

		}
	} while (!found && paramElement != NULL);

//	LOGI("param: %s found value:%s",paramName,*outValue);
	return found;
}

bool Parameters::SetParamValueToXML(XMLDocument* doc, const char* paramName,
		const char* value) {
	bool found = false;
	XMLElement* paramsRootElement = doc->FirstChildElement("params");
	if (paramsRootElement == NULL) {
		paramsRootElement = doc->NewElement("params");
		doc->InsertEndChild(paramsRootElement);
	}
	XMLElement* paramElement = paramsRootElement->FirstChildElement("param");
	if (paramElement != NULL) {
		do {
			XMLElement* nameElement = paramElement->FirstChildElement("name");
			const char* name = nameElement->GetText();
			if (strcmp(name, paramName) == 0) {
				found = true;
			} else {
				paramElement = paramElement->NextSiblingElement("param");
			}
		} while (!found && paramElement != NULL);
	}

	if (found) {
		XMLElement* valueElement = paramElement->FirstChildElement("value");
		valueElement->DeleteChildren();
		valueElement->InsertFirstChild(doc->NewText(value));

	} else {
		paramElement = doc->NewElement("param");
		doc->InsertEndChild(paramElement);

		XMLElement* nameElement = doc->NewElement("name");
		nameElement->InsertFirstChild(doc->NewText(paramName));
		paramElement->InsertEndChild(nameElement);

		XMLElement* valueElement = doc->NewElement("value");
		valueElement->InsertFirstChild(doc->NewText(value));
		paramElement->InsertEndChild(valueElement);

		paramsRootElement->InsertEndChild(paramElement);
	}

	return found;
}

void Parameters::SetLoadExternInfo(bool val)
{
	m_loadExternInfo = val;
}

}
